IMPERAS_HOME := $(shell getpath.exe "$(IMPERAS_HOME)")

# Build locally without using a VLNV library
NOVLNV=1

# Build using two passes so that each pass includes separate Makefiles
MAKEPASS?=0
ifeq ($(MAKEPASS),0)
all: application
application:
	$(MAKE) MAKEPASS=2
clean:
	$(MAKE) MAKEPASS=2 clean
endif

# Pass 2 build the Application
ifeq ($(MAKEPASS),2)

OPTIMISATION?=-O2

CROSS=MIPS32
SRC?=divider.c dijkstra_0.c dijkstra_1.c dijkstra_2.c dijkstra_3.c dijkstra_4.c dijkstra_5.c dijkstra_6.c dijkstra_7.c dijkstra_8.c dijkstra_9.c dijkstra_10.c dijkstra_11.c dijkstra_12.c dijkstra_13.c dijkstra_14.c dijkstra_15.c dijkstra_16.c dijkstra_17.c dijkstra_18.c dijkstra_19.c dijkstra_20.c dijkstra_21.c dijkstra_22.c dijkstra_23.c dijkstra_24.c dijkstra_25.c dijkstra_26.c dijkstra_27.c dijkstra_28.c dijkstra_29.c dijkstra_30.c dijkstra_31.c dijkstra_32.c dijkstra_33.c dijkstra_34.c dijkstra_35.c dijkstra_36.c dijkstra_37.c dijkstra_38.c dijkstra_39.c dijkstra_40.c dijkstra_41.c dijkstra_42.c dijkstra_43.c dijkstra_44.c dijkstra_45.c dijkstra_46.c dijkstra_47.c dijkstra_48.c dijkstra_49.c dijkstra_50.c dijkstra_51.c dijkstra_52.c dijkstra_53.c dijkstra_54.c dijkstra_55.c dijkstra_56.c dijkstra_57.c dijkstra_58.c dijkstra_59.c dijkstra_60.c dijkstra_61.c dijkstra_62.c dijkstra_63.c dijkstra_64.c dijkstra_65.c dijkstra_66.c dijkstra_67.c dijkstra_68.c dijkstra_69.c dijkstra_70.c dijkstra_71.c dijkstra_72.c dijkstra_73.c dijkstra_74.c dijkstra_75.c dijkstra_76.c dijkstra_77.c dijkstra_78.c dijkstra_79.c dijkstra_80.c dijkstra_81.c dijkstra_82.c dijkstra_83.c dijkstra_84.c dijkstra_85.c dijkstra_86.c dijkstra_87.c dijkstra_88.c dijkstra_89.c dijkstra_90.c dijkstra_91.c dijkstra_92.c dijkstra_93.c dijkstra_94.c dijkstra_95.c dijkstra_96.c dijkstra_97.c dijkstra_98.c

SRCINC?=

INCOBJ=$(patsubst %.c,%.$(CROSS).o,$(SRCINC))
EXE=$(patsubst %.c,%.$(CROSS).elf,$(SRC))


-include $(IMPERAS_HOME)/lib/$(IMPERAS_ARCH)/CrossCompiler/$(CROSS).makefile.include
ifeq ($($(CROSS)_CC),)
    IMPERAS_ERROR := $(error "Error : $(CROSS)_CC not set. Please check installation of toolchain for $(CROSS)")
endif

all: $(EXE)

%.$(CROSS).elf: %.$(CROSS).o $(INCOBJ)
	@    echo "# Linking $(CROSS) $@"
	$(V) $(IMPERAS_LINK) -o $@ $^ $(IMPERAS_LDFLAGS) -lm

%.$(CROSS).o: %.c
	@    echo "# Compiling $(CROSS) $<"
	$(V) $(IMPERAS_CC) -c -o $@ $< -D$(CROSS) $(OPTIMISATION)

%.dbg.$(CROSS).elf: %.dbg.$(CROSS).o $(INCOBJ)
	@    echo "# Linking $(CROSS) $@"
	$(V) $(IMPERAS_LINK) -o $@ $^ $(IMPERAS_LDFLAGS) -lm

%.dbg.$(CROSS).o: %.c
	@    echo "# Compiling $(CROSS) (Debug) $<"
	$(V) $(IMPERAS_CC) -c -o $@ $< -D$(CROSS) -O0 -g -gdwarf-2

clean::
	- rm -f *.$(CROSS).elf *.$(CROSS).o

endif

