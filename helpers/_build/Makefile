CC = gcc
BFLAGS = -Wall
DY_FLAGS = -fPIC

OUTDIR ?= .
SRC_LOC ?= ..
STATIC_OUTDIR ?= $(OUTDIR)/object
DYNAMI_OUTDIR ?= $(OUTDIR)/shared

#NOTE: NEEDS UNIQUE .c FILE NAMES
SPACE := $(subst ,, )
VPATH := $(subst $(SPACE),:,$(shell ls -d $(SRC_LOC)/*/))

#Get all source files in source, and get objects
HEADERS := $(shell find $(SRC_LOC) -name '*.h')
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

all: dynamic static
	
dynamic: $(DYNAMI_OUTDIR) $(DYN_OBJS)

static: $(STATIC_OUTDIR) $(STT_OBJS)

$(DYNAMI_OUTDIR)/%.o: %.c $(HEADERS)
	$(info [h]$(TAB)$@)
	@$(CC) $< -c -o $@ $(BFLAGS) $(DY_FLAGS)

$(STATIC_OUTDIR)/%.o: %.c $(HEADERS)
	$(info [h]$(TAB)$@)
	@$(CC) $< -c -o $@ $(BFLAGS)

$(DYNAMI_OUTDIR) $(STATIC_OUTDIR):
	@mkdir -p $@

clean:
	-rm -rf $(DYNAMI_OUTDIR)
	-rm -rf $(STATIC_OUTDIR)
