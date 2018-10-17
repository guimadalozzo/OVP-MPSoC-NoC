NOVLNV=1
#SRC=hemps_platform.cpp
#include $(IMPERAS_HOME)/ImperasLib/source/buildutils/Makefile.platform

#
# This will take the platform.c (or SRC=<name>) file in this directory
# Compile to .exe using the HOST compiler
# copy to the $(WORKDIR) directory, along with any local
# .xml files
#
IMPERAS_HOME := $(shell getpath.exe "$(IMPERAS_HOME)")
include $(IMPERAS_HOME)/bin/Makefile.include

ifndef IMPERAS_HOME
  IMPERAS_ERROR := $(error "IMPERAS_HOME not defined")
endif

include $(IMPERAS_HOME)/ImperasLib/buildutils/Makefile.common

SRC?=platform.cpp
SUFFIX=$(suffix $(SRC))

ifeq ($(SUFFIX),)
  IMPERAS_ERROR:=$(error Please specify full platform source file name. File suffix could not be determined!)
endif

WatchdogFLAGS = -w -fpermissive -pthread -Wno-attributes

ifeq ($(SUFFIX),.c)
  CC=$(HOST_GCC) $(WatchdogFLAGS)
else
  CC=$(HOST_GXX) $(WatchdogFLAGS)
endif

PLATFORM_CFLAGS = $(SIM_CFLAGS)
PLATFORM_LDFLAGS = $(SIM_LDFLAGS) 

EXECUTABLE?=$(SRC:$(SUFFIX)=.$(IMPERAS_ARCH).exe)
SHAREDOBJ?=$(SRC:$(SUFFIX)=.$(SHRSUF))
PLATOBJ?=$(OBJDIR)/$(SRC:$(SUFFIX)=.o)

all: $(WORKDIR)/$(EXECUTABLE) $(WORKDIR)/$(SHAREDOBJ)

$(WORKDIR)/$(EXECUTABLE): $(PLATOBJ)
	$(V) mkdir -p $(@D)
	@    echo "# Host Linking Platform $@"
	$(V) $(CC) -o $@ $^ $(PLATFORM_LDFLAGS) $(LDFLAGS) $(IMPERAS_ICMSTUBS)
	$(V) # if we are not compiling locally, copy out the .xml files to the destination
ifeq ($(COPYXML),1)
	@    echo "# Copying XML $(XML) to $(@D)"
	$(V) cp $(XML) $(@D)
endif

$(WORKDIR)/$(SHAREDOBJ): $(PLATOBJ)
	$(V) mkdir -p $(@D)
	@    echo "# Host Linking Platform object $@"
	$(V) $(CC) -shared -o $@ $^ $(PLATFORM_LDFLAGS) $(LDFLAGS) $(IMPERAS_ICMSTUBS)

$(OBJDIR)/%.o:%.c
	$(V) mkdir -p $(@D)
	@    echo "# Host Compiling Platform $@"
	$(V) $(CC) -c -o $@ $^ $(PLATFORM_CFLAGS) $(CFLAGS)

$(OBJDIR)/%.o:%.c??
	$(V) mkdir -p $(@D)
	@    echo "# Host Compiling Platform $@"
	$(V) $(CC) -c -o $@ $^ $(PLATFORM_CFLAGS) $(CFLAGS)

clean:
	$(V) - rm -f $(WORKDIR)/$(EXECUTABLE) $(WORKDIR)/$(SHAREDOBJ)
	$(V) - rm -rf $(OBJDIR)
