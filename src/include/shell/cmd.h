/*
 * 	miaofng@2009 initial version
 */
#ifndef __CMD_H_
#define __CMD_H_

#include <stdio.h>
#include <linux/list.h>

typedef struct {
	char *name;
	int (*func)(int argc, char *argv[]);
	char *help;
} cmd_t;

struct cmd_list_s {
	char *cmdline;
	int len;
	struct list_head list;
};

struct cmd_queue_s {
	int flag;
	struct list_head cmd_list;
};

#define CMD_FLAG_REPEAT 1

#pragma section=".shell.cmd" 4
#define DECLARE_SHELL_CMD(cmd) \
	const cmd_t *##cmd##_entry@".shell.cmd" = &##cmd;

/*cmd module i/f*/
void cmd_Init(void);
void cmd_Update(void);

/*cmd queue ops*/
int cmd_queue_init(struct cmd_queue_s *);
int cmd_queue_update(struct cmd_queue_s *);
int cmd_queue_exec(struct cmd_queue_s *, const char *);

#endif /*__CMD_H_*/
