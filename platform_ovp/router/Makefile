IMPERAS_HOME := $(shell getpath.exe "$(IMPERAS_HOME)")
NOVLNV=1
CFLAGS=-DENDIANBIG
include $(HEMPS_OVP_PATH)/platform_ovp/peripheral/Makefile.pse

cleanall: clean
	$(V) - rm -f *.igen.*
