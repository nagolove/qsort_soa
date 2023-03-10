# GNU Make workspace makefile autogenerated by Premake

ifndef config
  config=one
endif

ifndef verbose
  SILENT = @
endif

ifeq ($(config),one)
  qsort_config = one
endif

PROJECTS := qsort

.PHONY: all clean help $(PROJECTS) 

all: $(PROJECTS)

qsort:
ifneq (,$(qsort_config))
	@echo "==== Building qsort ($(qsort_config)) ===="
	@${MAKE} --no-print-directory -C . -f qsort.make config=$(qsort_config)
endif

clean:
	@${MAKE} --no-print-directory -C . -f qsort.make clean

help:
	@echo "Usage: make [config=name] [target]"
	@echo ""
	@echo "CONFIGURATIONS:"
	@echo "  one"
	@echo ""
	@echo "TARGETS:"
	@echo "   all (default)"
	@echo "   clean"
	@echo "   qsort"
	@echo ""
	@echo "For more information, see https://github.com/premake/premake-core/wiki"