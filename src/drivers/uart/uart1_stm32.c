/*
 * 	miaofng@2009 initial version
 *	miaofng@2010 update to new api format
 */

#include "config.h"
#include "uart.h"
#include "stm32f10x.h"

#define uart USART1
#define dma_ch_tx DMA1_Channel4
#define dma_ch_rx DMA1_Channel5

#if CONFIG_UART1_TF_SZ > 0
#define ENABLE_TX_DMA 1
#define TX_FIFO_SZ CONFIG_UART1_TF_SZ
static char uart_fifo_tx[TX_FIFO_SZ];
static char uart_fifo_tn; //nr of bytes to send
static char uart_fifo_tp; //pos of tx fifo start
static void uart_SetupTxDMA(void *p, int n);
#endif

#if CONFIG_UART1_RF_SZ > 0
#define ENABLE_RX_DMA 1
#define RX_FIFO_SZ CONFIG_UART1_RF_SZ
static char uart_fifo_rx[RX_FIFO_SZ];
static char uart_fifo_rn;
static void uart_SetupRxDMA(void *p, int n);
#endif

static int uart_Init(const uart_cfg_t *cfg)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef uartinfo;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	/*configure PA9<uart1.tx>, PA10<uart1.rx>*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/*init serial port*/
	USART_StructInit(&uartinfo);
	uartinfo.USART_BaudRate = cfg->baud;
	USART_Init(uart, &uartinfo);

#ifdef ENABLE_TX_DMA
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	uart_SetupTxDMA(uart_fifo_tx, 0);
	USART_DMACmd(uart, USART_DMAReq_Tx, ENABLE);
	uart_fifo_tn = 0;
	uart_fifo_tp = 0;
#endif

#ifdef ENABLE_RX_DMA
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	uart_SetupRxDMA(uart_fifo_rx, RX_FIFO_SZ);
	USART_DMACmd(uart, USART_DMAReq_Rx, ENABLE);
	uart_fifo_rn = 0;
#endif

	USART_Cmd(uart, ENABLE);
	return 0;
}

static int uart_putchar(int data)
{
	char c = (char) data;
#ifdef ENABLE_TX_DMA
	int i, j, n;
	
	//wait ... no enough fifo space to use
	do {
		n = DMA_GetCurrDataCounter(dma_ch_tx);
	} while(TX_FIFO_SZ - n < 1);
	
	//copy
	DMA_Cmd(dma_ch_tx, DISABLE);
	n = DMA_GetCurrDataCounter(dma_ch_tx);
	uart_fifo_tp += uart_fifo_tn - n;
	if(uart_fifo_tp + n + 2 >  TX_FIFO_SZ) {
		for(i = 0, j = uart_fifo_tp; i < n; i ++, j ++) {
			if(i != j)
				uart_fifo_tx[i] = uart_fifo_tx[j];
		}
		uart_fifo_tp = 0;
	}
	else {
		i = uart_fifo_tp + n;
	}
	
	uart_fifo_tx[i ++] = c;
	n ++;
	
	uart_fifo_tn = n;
	uart_SetupTxDMA(uart_fifo_tx + uart_fifo_tp, n);
#else
	USART_SendData(uart, c);
	while(USART_GetFlagStatus(uart, USART_FLAG_TXE) == RESET);
#endif
	return 0;
}

static int uart_IsNotEmpty(void)
{
	int ret;
#ifdef ENABLE_RX_DMA
	int rn = RX_FIFO_SZ - DMA_GetCurrDataCounter(dma_ch_rx);
	ret = rn - uart_fifo_rn;
	ret += (ret < 0) ? RX_FIFO_SZ : 0;
#else
	ret = (int) USART_GetFlagStatus(uart, USART_FLAG_RXNE);
#endif
	return ret;
}

static int uart_getch(void)
{
	int ret;
	
	/*wait for a char*/
	while(1) {
		ret = uart_IsNotEmpty();
		if(ret) break;
	}

#ifdef ENABLE_RX_DMA
	ret = uart_fifo_rx[uart_fifo_rn];
	uart_fifo_rn ++;
	if(uart_fifo_rn >= RX_FIFO_SZ)
		uart_fifo_rn = 0;
#else
	ret = USART_ReceiveData(uart);
#endif
	return ret;
}

#ifdef ENABLE_TX_DMA
static void uart_SetupTxDMA(void *p, int n)
{
	DMA_InitTypeDef DMA_InitStructure;

	DMA_Cmd(dma_ch_tx, DISABLE);
	DMA_DeInit(dma_ch_tx);
	DMA_InitStructure.DMA_PeripheralBaseAddr = (int)(&uart->DR);
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)p;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
	DMA_InitStructure.DMA_BufferSize = n;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority = DMA_Priority_Low;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(dma_ch_tx, &DMA_InitStructure);
	DMA_Cmd(dma_ch_tx, ENABLE);
}
#endif

#ifdef ENABLE_RX_DMA
static void uart_SetupRxDMA(void *p, int n)
{
	DMA_InitTypeDef DMA_InitStructure;

	DMA_Cmd(dma_ch_rx, DISABLE);
	DMA_DeInit(dma_ch_rx);
	DMA_InitStructure.DMA_PeripheralBaseAddr = (int)(&uart->DR);
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)p;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = n;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_Low;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(dma_ch_rx, &DMA_InitStructure);
	DMA_Cmd(dma_ch_rx, ENABLE);
}
#endif

uart_bus_t uart1 = {
	.init = uart_Init,
	.putchar = uart_putchar,
	.getchar = uart_getch,
	.isready = uart_IsNotEmpty,
};
