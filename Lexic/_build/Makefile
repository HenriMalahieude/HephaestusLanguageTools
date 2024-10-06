CC = gcc
AR = ar
BFLAGS = -Wall
DY_FLAGS = -fPIC

SRC_LOC := ../private
HLP_LOC := ../../helpers
STATIC_OUTDIR ?= ./static
DYNAMI_OUTDIR ?= ./dynamic

HLP_HDRS := $(shell find $(HLP_LOC) -name '*.h')
SRC_HDRS := $(shell find $(SRC_LOC)/.. -name '*.h')

HELPERS := $(shell find $(HLP_LOC) -name '*.c')
SOURCES := $(shell find $(SRC_LOC) -name '*.c')

OBJECTS := $(foreach t, $(SOURCES),$(basename $(notdir $t)).o)
DYN_OBJS := $(foreach t, $(OBJECTS),$(DYNAMI_OUTDIR)/$t)
STT_OBJS := $(foreach t, $(OBJECTS),$(STATIC_OUTDIR)/$t)

#TODO: Helpers are rebuilt at every call to 'make', need to fix that somehow...
# It's difficult to handle helper files, so there are two modes 'ohd', and 'ohs' which allows us a mode when creating the helper objs
# Do tell me if there is a better way. Hell, make a pull request. It better be either: more understandable than what I have, or less lines
HLP_DOBJS := $(foreach t, $(HELPERS),$(basename $t).ohd)
HLP_SOBJS := $(foreach t, $(HELPERS),$(basename $t).ohs)

#Print Help
NULL :=
TAB := $(NULL)    $(NULL)
define ENDL


endef

.PHONY: all clean
	
all: dynamic static

# Dynamic Library Creation
dynamic: $(DYN_OBJS) $(HLP_DOBJS)
	$(info [!] Linking lexic dynamically!$(ENDL))
	@$(CC) -shared -o $(DYNAMI_OUTDIR)/liblexic.so $(DYNAMI_OUTDIR)/*.o $(BFLAGS) $(DY_FLAGS)

$(DYNAMI_OUTDIR)/%.o: $(SRC_LOC)/%.c $(SRC_HDRS) $(HLP_HDRS)
	$(info [.]$(TAB)$@)
	@$(CC) $< -c -o $@ $(BFLAGS) $(DY_FLAGS)

$(HLP_LOC)/%.ohd: $(HLP_LOC)/%.c $(HLP_HDRS)
	$(info [h]$(TAB)$(DYNAMI_OUTDIR)/$(basename $(notdir $<)).o)
	@$(CC) $< -c -o $(DYNAMI_OUTDIR)/$(basename $(notdir $<)).o $(BFLAGS) $(DY_FLAGS)

# Static Library Creation
static: $(STT_OBJS) $(HLP_SOBJS)
	$(info [!] Linking lexic statically!$(ENDL))
	@$(AR) rcs $(STATIC_OUTDIR)/liblexic.a $(STATIC_OUTDIR)/*.o

$(STATIC_OUTDIR)/%.o: $(SRC_LOC)/%.c $(SRC_HDRS) $(HLP_HDRS)
	$(info [.]$(TAB)$@)
	@$(CC) $< -c -o $@ $(BFLAGS)

$(HLP_LOC)/%.ohs: $(HLP_LOC)/%.c $(HLP_HDRS)
	$(info [h]$(TAB)$(STATIC_OUTDIR)/$(basename $(notdir $<)).o)
	@$(CC) $< -c -o $(STATIC_OUTDIR)/$(basename $(notdir $<)).o $(BFLAGS)

clean:
	rm $(STATIC_OUTDIR)/*.o $(DYNAMI_OUTDIR)/*.o 2>/dev/null || true
	rm $(STATIC_OUTDIR)/*.a 2>/dev/null || true
	rm $(DYNAMI_OUTDIR)/*.so 2>/dev/null || true
