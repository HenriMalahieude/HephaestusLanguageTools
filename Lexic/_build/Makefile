CC = gcc
AR = ar
BFLAGS = -Wall
DY_FLAGS = -fPIC

OUTDIR ?= .
SRC_LOC ?= ../private
HLP_LOC ?= ../../helpers
STATIC_OUTDIR ?= $(OUTDIR)/object
DYNAMI_OUTDIR ?= $(OUTDIR)/shared

HLP_HDRS := $(shell find $(HLP_LOC) -name '*.h')
SRC_HDRS := $(shell find $(SRC_LOC)/.. -name '*.h')

SOURCES := $(wildcard $(SRC_LOC)/*.c)
OBJECTS := $(foreach t, $(SOURCES),$(basename $(notdir $t)).o)

DYN_OBJS := $(foreach t, $(OBJECTS),$(DYNAMI_OUTDIR)/$t)
STT_OBJS := $(foreach t, $(OBJECTS),$(STATIC_OUTDIR)/$t)

#Print Help
NULL :=
TAB := $(NULL)    $(NULL)
define ENDL


endef

.PHONY: all clean clean_all dynamic static stt_helpers dyn_helpers
	
all: dynamic static

dynamic: dyn_helpers $(DYNAMI_OUTDIR) $(DYNAMI_OUTDIR)/liblexic.so

static: stt_helpers $(STATIC_OUTDIR) $(STATIC_OUTDIR)/liblexic.a

# Dynamic Library Creation
$(DYNAMI_OUTDIR)/liblexic.so: $(DYN_OBJS)
	$(info [!] Linking lexic dynamically!$(ENDL))
	@$(CC) -shared -o $(DYNAMI_OUTDIR)/liblexic.so $(DYNAMI_OUTDIR)/*.o $(HLP_LOC)/_build/shared/*.o $(BFLAGS) $(DY_FLAGS)

$(DYNAMI_OUTDIR)/%.o: $(SRC_LOC)/%.c $(SRC_HDRS) $(HLP_HDRS)
	$(info [.]$(TAB)$@)
	@$(CC) $< -c -o $@ $(BFLAGS) $(DY_FLAGS)

# Static Library Creation
$(STATIC_OUTDIR)/liblexic.a: $(STT_OBJS)
	$(info [!] Linking lexic statically!$(ENDL))
	@$(AR) rcs $(STATIC_OUTDIR)/liblexic.a $(STATIC_OUTDIR)/*.o $(HLP_LOC)/_build/object/*.o

$(STATIC_OUTDIR)/%.o: $(SRC_LOC)/%.c $(SRC_HDRS) $(HLP_HDRS)
	$(info [.]$(TAB)$@)
	@$(CC) $< -c -o $@ $(BFLAGS)

dyn_helpers:
	@$(MAKE) -s -C $(HLP_LOC)/_build dynamic

stt_helpers:
	@$(MAKE) -s -C $(HLP_LOC)/_build static

$(STATIC_OUTDIR) $(DYNAMI_OUTDIR):
	@mkdir -p $@

clean_all: clean
	$(info Cleaning helper objects)
	@make -s -C $(HLP_LOC)/_build clean

clean:
	-rm -rf $(STATIC_OUTDIR)
	-rm -rf $(DYNAMI_OUTDIR)
