# Part of SuperNOVAS
#
# Generates headless README.md and HTML documentation 
#
# Author: Attila Kovacs

# Check if there is a doxygen we can run
ifndef DOXYGEN
  DOXYGEN := $(shell which doxygen)
else
  $(shell test -f $(DOXYGEN))
endif

# Build only if there is doxygen
ifeq ($(.SHELLSTATUS),0)
  TARGET = all
else
  ifneq ($(DOXYGEN),none)
    $(info WARNING! Doxygen is not available. Will skip)
  endif
endif

.PHONY: default
default: $(TARGET)

# Doxygen documentation (HTML and man pages) under docs/
.PHONY: all
all: Doxyfile ../../src/c99 ../../include
	@echo "   [doxygen]"
	@$(DOXYGEN) Doxyfile

.PHONY: clean
clean:

.PHONY: distclean
distclean: clean
	rm -rf html *.tag



