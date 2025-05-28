
export V?=0

# If _HOST or _TA specific compilers are not specified, then use CROSS_COMPILE
TA_CROSS_COMPILE ?= $(CROSS_COMPILE)

.PHONY: all
all:
	make -C ta CROSS_COMPILE=$(TA_CROSS_COMPILE) LDFLAGS=""

.PHONY: clean
clean:
	make -C ta clean
