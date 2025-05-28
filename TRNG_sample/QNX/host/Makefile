CC      = $(HOST_CROSS_COMPILE)gcc
LD      = $(HOST_CROSS_COMPILE)ld
AR      = $(HOST_CROSS_COMPILE)ar
NM      = $(HOST_CROSS_COMPILE)nm
OBJCOPY = $(HOST_CROSS_COMPILE)objcopy
OBJDUMP = $(HOST_CROSS_COMPILE)objdump
READELF = $(HOST_CROSS_COMPILE)readelf

OBJS = main.o

CFLAGS += -Wall -I../ta/include -I$(TEEC_EXPORT)/include -I./include
#Add/link other required libraries here
LDADD += -lteec -L$(TEEC_EXPORT)/lib

BINARY = optee_example_icumfw_trng

.PHONY: all
all: $(BINARY)

$(BINARY): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $< $(LDADD)

.PHONY: clean
clean:
	rm -f $(OBJS) $(BINARY)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
