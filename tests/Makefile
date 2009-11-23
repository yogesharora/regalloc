MCC = ../ra
SIM = ./s3sim

.SUFFIXES: %.o %.c

CFLAGS += -v

tests :=  \
        graded_001.opt \
	graded_002.opt \
	graded_003.opt \
	graded_004.opt \
	graded_005.opt \
	graded_006.opt \
	graded_007.opt \
	graded_008.opt \

%.opt: %.asm
	$(MCC) $(CFLAGS) -o $@ $<

all: clean $(tests)

clean:
	rm -Rf *.opt	
