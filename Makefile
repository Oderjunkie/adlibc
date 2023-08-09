all : adlibc.o
.PHONY : clean

csources := $(wildcard ./*.c)
asmsources := $(wildcard ./*.s)
sources := $(csources) $(asmsources)

override CFLAGS += -nostdlib
LDFLAGS += -z noexecstack

%.o : %.c
	$(CC) $(CFLAGS) -c -o $@ $^
%.o : %.s
	$(AS) $(ASFLAGS) -c -o $@ $^
adlibc.o : $(csources:.c=.o) $(asmsources:.s=.o)
	$(LD) $(LDFLAGS) -r -o $@ $^
clean :
	rm $(csources:.c=.o) $(asmsources:.s=.o)
