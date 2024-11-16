#locations
HLP_SRC := $(abspath ./helpers)

#Directories
OUT_LIB = $(abspath ./_build/lib)
OUT_LXC = $(abspath ./_build/Lexic)
OUT_SYC = $(abspath ./_build/Syntac)
OUT_MTC = $(abspath ./_build/Mantic)
OUT_HLP = $(abspath ./_build/Helpers)
DIRS := $(OUT_LXC) $(OUT_SYC) $(OUT_MTC) $(OUT_HLP) $(OUT_LIB)

.PHONY: all clean clean_all

.SUFFIXES:

all: $(DIRS) helpers Lexic #Syntac Mantic (unimplemented)
	@mkdir -p $(OUT_LIB)/shared
	@mkdir -p $(OUT_LIB)/object
	$(foreach t,$(shell find -name *.so), @cp $t $(OUT_LIB)/shared/)
	$(foreach t,$(shell find -name *.a) , @cp $t $(OUT_LIB)/object/)
	$(info [!!!] Linked libs into $(OUT_LIB))

Lexic: $(OUT_LXC) helpers
	@$(MAKE) -s -C _build/ NAME=Lexic OUTDIR=$(OUT_LXC) SRC_LOC=$(abspath ./Lexic/private) HLP_LOC=$(HLP_SRC)

Syntac: $(OUT_SYC) helpers
	$(error Unimplemented)
	$(MAKE) -C _build/ NAME=Syntac OUTDIR=$(OUT_SYC) SRC_LOC=$(abspath ./Syntac/private) HLP_LOC=$(HLP_SRC)

Mantic: $(OUT_MTC) helpers
	$(error Unimplemented)
	$(MAKE) -C _build/ NAME=Mantic OUTDIR=$(OUT_MTC) SRC_LOC=$(abspath ./Mantic/private) HLP_LOC=$(HLP_SRC)

helpers: $(OUT_HLP)
	@$(MAKE) -s -C $(HLP_SRC) OUTDIR=$(OUT_HLP)

$(DIRS):
	@mkdir -p $@

clean:
	-@rm -rf $(DIRS)
