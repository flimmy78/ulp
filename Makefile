TOP_DIR ?= $(shell pwd)
AUTOCONFIG_HEAD_FILE = $(TOP_DIR)/autoconfig.h
AUTOCONFIG_PROJ_FILE ?= ""
AUTOCONFIG_MAKE_FILE = $(TOP_DIR)/.config
IAR_TOOL = python $(TOP_DIR)/scripts/iar.py
IAR_FILE = $(TOP_DIR)/projects/bldc/bldc.ewp
ENV_FILE = $(TOP_DIR)/make.env
M ?= src/

#create iar project file according to the result of 'make xconfig'
-include $(AUTOCONFIG_MAKE_FILE)
-include $(M)Makefile
-include $(ENV_FILE)

iar: detect_config iar_clr iar_cfg iar_inc iar_add
	@echo "projects/bldc/bldc.ewp has been created!"

iar_help:
	$(IAR_TOOL)
iar_clr:
	$(IAR_TOOL) clr $(IAR_FILE)
iar_cfg:
ifeq ($(CONFIG_CPU_STM32),y)
	$(IAR_TOOL) cfg $(IAR_FILE) 'STM32F10xxB	ST STM32F10xxB' 'stm32f103rb.icf'
endif
ifeq ($(CONFIG_CPU_LM3S),y)
	$(IAR_TOOL) cfg $(IAR_FILE) 'LM3Sx9xx	Luminary LM3Sx9xx' 'lm3s.icf'
endif
ifeq ($(CONFIG_CPU_SAM3U),y)
	$(IAR_TOOL) cfg $(IAR_FILE) 'AT91SAM3U4	Atmel AT91SAM3U4' 'sam3u.icf'
endif
iar_inc:
	$(IAR_TOOL) inc $(IAR_FILE) ./
	$(IAR_TOOL) inc $(IAR_FILE) src/include/

iar_add:
	@echo target=$@ M=$(M): obj-y = $(obj-y) inc-y = $(inc-y)
	@for dir in $(inc-y); do\
		if [ -d $(M)$$dir ];\
		then \
			$(IAR_TOOL) inc $(IAR_FILE) $(M)$$dir; \
		fi \
	done
	@$(IAR_TOOL) add $(IAR_FILE) $(M) $(obj-y)
	@for dir in $(obj-y); do\
		if [ -d $(M)$$dir ];\
		then \
			make -s -C $(TOP_DIR) M=$(M)$$dir $@; \
		fi \
	done

#xconfig
TKSCRIPTS_DIR = $(TOP_DIR)/scripts/tkconfig
PARSER = tkparse.exe

export PARSER

xconfig: $(PARSER) iar_script
	@$(TKSCRIPTS_DIR)/$(PARSER) src/Kconfig > main.tk
	@cat $(TKSCRIPTS_DIR)/header.tk main.tk $(TKSCRIPTS_DIR)/tail.tk > lconfig.tk
	@chmod a+x lconfig.tk
	@if test -r "/usr/bin/wish84.exe"; then /usr/bin/wish84.exe -f lconfig.tk; \
	else \
	  wish -f lconfig.tk; \
	fi; \
	if test $$? = "2" ; then                   \
		unix2dos autoconfig.h; \
		echo file .config/autoconfig.h has been created!; \
		if test $(AUTOCONFIG_PROJ_FILE) != "" ; then	\
			echo saving file .config to file $(AUTOCONFIG_PROJ_FILE);	\
			cp .config $(AUTOCONFIG_PROJ_FILE);	\
		fi \
	fi

iar_script:
	@cp projects/ulp/ulp.ewd projects/bldc/bldc.ewd
	@cp projects/ulp/ulp.ewp projects/bldc/bldc.ewp
	@cp projects/ulp/ulp.eww projects/bldc/bldc.eww

$(PARSER):
	@make -s -C $(TKSCRIPTS_DIR) $@

xconfig_clean:
	@make -s -C $(TKSCRIPTS_DIR) clean
	@rm -rf .tmp main.tk lconfig.tk

detect_config:
	@if [ -z $(wildcard $(AUTOCONFIG_HEAD_FILE)) ];\
	then \
		echo "System not configured!!!"; \
		echo "Please try: ";\
		echo "  make xconfig"; \
		echo "  make"; \
		echo ""; \
		exit 1; \
	fi

unconfig:
	@rm -rf $(AUTOCONFIG_HEAD_FILE)
	@rm -rf $(AUTOCONFIG_MAKE_FILE)
	@rm -rf $(ENV_FILE)

clean: xconfig_clean
	@rm -rf $(TOP_DIR)/projects/bldc/Debug
	@rm -rf $(TOP_DIR)/projects/bldc/Release
	@rm -rf $(TOP_DIR)/projects/bldc/settings
	@rm -rf $(TOP_DIR)/projects/bldc/bldc.dep

distclean: clean iar_clr unconfig

%_config:
	@if test -r $(subst _config,.config,$@) ; \
	then \
		echo file $(subst _config,.config,$@) is used as template;\
	else \
		echo creating new file $(subst _config,.config,$@) from defconfig;\
		cp defconfig $(subst _config,.config,$@); \
	fi
	@echo AUTOCONFIG_PROJ_FILE = $(subst _config,.config,$@) > $(ENV_FILE)
	@rm -rf .config
	@cp -f $(subst _config,.config,$@) .config ; \
	make xconfig
