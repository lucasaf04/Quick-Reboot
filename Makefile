.PHONY:	all

all:
	@$(MAKE) -C quick-reboot-app
	@$(MAKE) -C quick-reboot-overlay

clean:
	@$(MAKE) -C quick-reboot-app clean
	@$(MAKE) -C quick-reboot-overlay clean
