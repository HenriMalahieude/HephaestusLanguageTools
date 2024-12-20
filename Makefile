#locations
HLP_SRC := $(abspath ./helpers)

#Directories
OUT_LIB = $(abspath ./_build/lib)
OUT_LXC = $(abspath ./_build/Lexic)
OUT_SYC = $(abspath ./_build/Syntac)
OUT_MTC = $(abspath ./_build/Mantic)
OUT_HLP = $(abspath ./_build/Helpers)
DIRS := $(OUT_LXC) $(OUT_SYC) $(OUT_MTC) $(OUT_HLP) $(OUT_LIB)

define ENDL


endef

.PHONY: all clean

.SUFFIXES:

all: $(DIRS) helpers Lexic #Syntac Mantic (unimplemented)
	$(info [!!!] Linked libs into $(OUT_LIB)$(ENDL))

Lexic: $(OUT_LXC) helpers
	@$(MAKE) -s -C _build/ NAME=Lexic OUTLIB=$(OUT_LIB) OUTDIR=$(OUT_LXC) SRC_LOC=$(abspath ./Lexic/private) HLP_LOC=$(HLP_SRC)

Syntac: $(OUT_SYC) helpers
	$(error Unimplemented)
	$(MAKE) -C _build/ NAME=Syntac OUTLIB=$(OUT_LIB) OUTDIR=$(OUT_SYC) SRC_LOC=$(abspath ./Syntac/private) HLP_LOC=$(HLP_SRC)

Mantic: $(OUT_MTC) helpers
	$(error Unimplemented)
	$(MAKE) -C _build/ NAME=Mantic OUTLIB=$(OUT_LIB) OUTDIR=$(OUT_MTC) SRC_LOC=$(abspath ./Mantic/private) HLP_LOC=$(HLP_SRC)

helpers: $(OUT_HLP)
	@$(MAKE) -s -C $(HLP_SRC) OUTDIR=$(OUT_HLP)

$(DIRS):
	@mkdir -p $@

clean:
	-@rm -rf $(DIRS)
