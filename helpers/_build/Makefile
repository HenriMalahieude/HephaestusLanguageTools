CC = gcc
BFLAGS = -Wall
DY_FLAGS = -fPIC

OUTDIR ?= .
SRC_LOC ?= ..
STATIC_OUTDIR ?= $(OUTDIR)/static
DYNAMI_OUTDIR ?= $(OUTDIR)/dynamic

#NOTE: NEEDS UNIQUE .c FILE NAMES
SPACE := $(subst ,, )
VPATH := $(subst $(SPACE),:,$(shell ls -d $(SRC_LOC)/*/))

#Get all source files in source, and get objects
SOURCES := $(shell find $(SRC_LOC) -name '*.c')
OBJECTS := $(addsuffix .o, $(notdir $(basename $(SOURCES))))

#Macro for creating all file locations needed
ALL_OUTDIRS := $(DYNAMI_OUTDIR) $(STATIC_OUTDIR)

#Formatting for output to dynamic or static location
DYN_OBJS := $(addprefix $(DYNAMI_OUTDIR)/, $(OBJECTS)) #$(foreach t,$(TARGETS),$(DYNAMI_OUTDIR)/t)
STT_OBJS := $(addprefix $(STATIC_OUTDIR)/, $(OBJECTS)) #$(foreach t,$(TARGETS),$(STATIC_OUTDIR)/t)

NULL :=
TAB := $(NULL)    $(NULL)

MAKEFLAGS += --no-builtin-rules
.PHONY: all clean

all: $(ALL_OUTDIRS) $(DYN_OBJS) $(STT_OBJS) 
	$(info Done)

$(DYNAMI_OUTDIR)/%.o: %.c
	$(info [d]$(TAB)$<)
	@$(CC) $< -c -o $@ $(BFLAGS) $(DY_FLAGS)

$(STATIC_OUTDIR)/%.o: %.c 
	$(info [s]$(TAB)$<)
	@$(CC) $< -c -o $@ $(BFLAGS)

$(ALL_OUTDIRS):
	mkdir -p $@

clean:
	-rm -rf $(DYNAMI_OUTDIR)
	-rm -rf $(STATIC_OUTDIR)
