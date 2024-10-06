CC = gcc
BFLAGS = -Wall
DY_FLAGS = -fPIC

STATIC_OUTDIR ?= ./static
DYNAMI_OUTDIR ?= ./dynamic

SRC_LOC := ..

HEADERS := $(shell find $(SRC_LOC) -name '*.h')
SOURCES := $(shell find $(SRC_LOC) -name '*.c')
#HLP_DIRS := $(foreach t, $(shell ls -d $(SRC_LOC)/*/),$(if $(findstring _build,$t),,$t))
#HLP_DIRS := $(addsuffix /, $(basename $(subst .,,$(subst /,,$(dir $(HLP_DIRS))))))

OBJECTS := $(notdir $(SOURCES:.c=.o)) #$(addsuffix .o, $(notdir $(basename $(SOURCES))))

PARENTS := $(addsuffix /, $(basename $(subst .,,$(subst /,,$(dir $(SOURCES))))))
TARGETS := $(join $(PARENTS), $(OBJECTS))

DYN_OBJS := $(addprefix $(DYNAMI_OUTDIR)/, $(TARGETS)) #$(foreach t,$(TARGETS),$(DYNAMI_OUTDIR)/t)
STT_OBJS := $(addprefix $(STATIC_OUTDIR)/, $(TARGETS)) #$(foreach t,$(TARGETS),$(STATIC_OUTDIR)/t)

NULL :=
TAB := $(NULL)    $(NULL)

.PHONY: all clean

all: $(STATIC_OUTDIR) $(DYNAMI_OUTDIR) $(DYN_OBJS) $(STT_OBJS) 
	

$(DYNAMI_OUTDIR)/%.o: $(SRC_LOC)/%.c $(HEADERS)
	$(info [d]$(TAB)$<)
	@$(CC) $< -c -o $(DYNAMI_OUTDIR)/$(basename $(notdir $@)).o $(BFLAGS) $(DY_FLAGS)

$(STATIC_OUTDIR)/%.o: $(SRC_LOC)/%.c $(HEADERS)
	$(info [s]$(TAB)$<)
	@$(CC) $< -c -o $(STATIC_OUTDIR)/$(basename $(notdir $@)).o $(BFLAGS)

$(DYNAMI_OUTDIR):
	mkdir $@

$(STATIC_OUTDIR):
	mkdir $@

clean:
	-rm -rf $(DYNAMI_OUTDIR)
	-rm -rf $(STATIC_OUTDIR)
