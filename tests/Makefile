MCC = ../ra
SIM = ./s3sim

.SUFFIXES: %.o %.c

#CFLAGS += -v

tests :=  \
	test_000.opt \
	test_001.opt \
	test_002.opt \
	test_003.opt \
	test_004.opt \
	test_005.opt \
	test_006.opt \
	test_007.opt \
	test_008.opt \
	test_009.opt \
	test_010.opt 

%.opt: %.asm
	$(MCC) $(CFLAGS) -o $@ $<

all: clean $(tests)

clean:
	rm -Rf *.opt	
