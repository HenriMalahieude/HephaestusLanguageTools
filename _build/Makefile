CC = gcc
AR = ar
BFLAGS = -Wall
DY_FLAGS = -fPIC

NAME ?=Lexic
$(eval REMAINDDER_NAME := $$$(NAME))
ABRV :=$(subst $(REMAINDDER_NAME),,$(NAME))

OUTDIR ?= .
SRC_LOC ?= ../Lexic/private
HLP_LOC ?= ../helpers
HLP_OBJDIR ?= ./Helpers
STATIC_OUTDIR := $(OUTDIR)/object
DYNAMI_OUTDIR := $(OUTDIR)/shared

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

.PHONY: all clean clean_all dynamic static 
	
all: $(HLP_OBJDIR) dynamic static 
	$(info [!!$(ABRV)] $(NAME) Library Fully Compiled$(ENDL))

dynamic: $(HLP_OBJDIR) $(DYNAMI_OUTDIR) $(DYNAMI_OUTDIR)/liblexic.so
	$(info [!d$(ABRV)]$(TAB)$(NAME) Dynamically Complete)

static: $(HLP_OBJDIR) $(STATIC_OUTDIR) $(STATIC_OUTDIR)/liblexic.a
	$(info [!s$(ABRV)]$(TAB)$(NAME) Statically Complete)

# Dynamic Library Creation
$(DYNAMI_OUTDIR)/liblexic.so: $(DYN_OBJS)
	$(info [dl$(ABRV)]$(TAB)Linking dynamically!)
	$(CC) -shared -o $(DYNAMI_OUTDIR)/liblexic.so $(DYNAMI_OUTDIR)/*.o $(HLP_OBJDIR)/shared/*.o $(BFLAGS) $(DY_FLAGS)

$(DYNAMI_OUTDIR)/%.o: $(SRC_LOC)/%.c $(SRC_HDRS) $(HLP_HDRS)
	$(info [.d$(ABRV)]$(TAB)$(TAB)$(notdir $@))
	$(CC) $< -c -o $@ $(BFLAGS) $(DY_FLAGS)

# Static Library Creation
$(STATIC_OUTDIR)/liblexic.a: $(STT_OBJS)
	$(info [sl$(ABRV)]$(TAB)Linking lexic statically!)
	$(AR) rcs $(STATIC_OUTDIR)/liblexic.a $(STATIC_OUTDIR)/*.o $(HLP_OBJDIR)/object/*.o

$(STATIC_OUTDIR)/%.o: $(SRC_LOC)/%.c $(SRC_HDRS) $(HLP_HDRS)
	$(info [.o$(ABRV)]$(TAB)$(TAB)$(notdir $@))
	$(CC) $< -c -o $@ $(BFLAGS)

$(STATIC_OUTDIR) $(DYNAMI_OUTDIR):
	mkdir -p $@

$(HLP_OBJDIR):
	$(info [XXX] Helper Objects weren't created?)
	$(error No Helper Objects)
