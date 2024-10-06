CC = gcc
BFLAGS = -Wall
DY_FLAGS = -fPIC

STATIC_OUTDIR ?= ./static
DYNAMI_OUTDIR ?= ./dynamic

SRC_LOC := ..

#Get all source files in source
SOURCES := $(shell find $(SRC_LOC) -name '*.c')

#Get all different helper types
HLP_DIRS := $(foreach t, $(shell ls -d $(SRC_LOC)/*/),$(if $(findstring _build,$t),,$t))
HLP_DIRS := $(addsuffix /, $(basename $(subst .,,$(subst /,,$(dir $(HLP_DIRS))))))

#Macro for creating all file locations needed
ALL_DIRS := $(DYNAMI_OUTDIR) $(STATIC_OUTDIR) $(addprefix $(DYNAMI_OUTDIR)/,$(HLP_DIRS)) $(addprefix $(STATIC_OUTDIR)/,$(HLP_DIRS))

#Only the .o
OBJECTS := $(notdir $(SOURCES:.c=.o))

#From "../parent/source.c" to "parent/source.o" for building
PARENTS := $(addsuffix /, $(basename $(subst .,,$(subst /,,$(dir $(SOURCES))))))
TARGETS := $(join $(PARENTS), $(OBJECTS))

#Formatting for output to dynamic or static location
DYN_OBJS := $(addprefix $(DYNAMI_OUTDIR)/, $(TARGETS)) #$(foreach t,$(TARGETS),$(DYNAMI_OUTDIR)/t)
STT_OBJS := $(addprefix $(STATIC_OUTDIR)/, $(TARGETS)) #$(foreach t,$(TARGETS),$(STATIC_OUTDIR)/t)

NULL :=
TAB := $(NULL)    $(NULL)

.PHONY: all clean

all: $(ALL_DIRS) $(DYN_OBJS) $(STT_OBJS) 
	
.SECONDEXPANSION:
$(DYNAMI_OUTDIR)/%.o: $(SRC_LOC)/%.c $(SRC_LOC)/$$(subst /,,$$(dir %))/*.h
	$(info [d]$(TAB)$<)
	@$(CC) $< -c -o $@ $(BFLAGS) $(DY_FLAGS)

$(STATIC_OUTDIR)/%.o: $(SRC_LOC)/%.c $$(SRC_LOC)/$$(subst /,,$$(dir %))/*.h
	$(info [s]$(TAB)$<)
	@$(CC) $< -c -o $@ $(BFLAGS)

$(ALL_DIRS):
	@mkdir -p $@

clean:
	-rm -rf $(DYNAMI_OUTDIR)
	-rm -rf $(STATIC_OUTDIR)
