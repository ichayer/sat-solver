
all:
	@cd src; make all;

clean:
	@cd src; make clean;

check:
	@cd src; make check;
	
.PHONY: all clean check 