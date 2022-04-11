GCC = gcc
GCCFLAGS = -g -Wall -std='c99' -lpthread -lrt

OBJECTS := $(wildcard *.o)
EXECUTABLES := solve slave view

EXEC = Application Slave View

APPLICATION_C = application.c
APPLICATION_O = application.o

SLAVE_C = slave.c
SLAVE_O = slave.o

VIEW_C = view.c
VIEW_O = view.o

SMANAGER_C = slaveManagerADT.c
SMANAGER_O = slaveManagerADT.o

POSSHMADT_C = posShmADT.c
POSSHMADT_O = posShmADT.o

ERRORS_C = myerror.c
ERRORS_O = myerror.o

all: $(EXEC)

Slave:	
	@$(GCC) $(GCCFLAGS) $(SLAVE_C) $(ERRORS_C) -o slave
	@echo "Slave successfully compiled"

Application:	
	@$(GCC) $(GCCFLAGS)  $(SMANAGER_C) $(ERRORS_C) $(APPLICATION_C) $(POSSHMADT_C) -o solve
	@echo "Solve successfully compiled"

View:
	@$(GCC) $(GCCFLAGS) $(ERRORS_C) $(VIEW_C) $(POSSHMADT_C) -o view
	@echo "View successfully compiled"

clean:
	@rm -rf $(OBJECTS) $(EXECUTABLES)
	@rm -rf check

check:
	@mkdir check
	@cppcheck --quiet --enable=all --force --inconclusive . 2> ./check/cppout.txt

	@pvs-studio-analyzer trace -- make
	@pvs-studio-analyzer analyze	
	@plog-converter -a '64:1,2,3;GA:1,2,3;OP:1,2,3' -t tasklist -o ./check/report.tasks ./PVS-Studio.log 
	
	@mv strace_out check
	@rm PVS-Studio.log

	@valgrind --leak-check=full --show-leak-kinds=all -v ./solve test/* 2> ./check/solve.valgrind
	@valgrind --leak-check=full --show-leak-kinds=all -v ./solve test/* > ./view  2> ./check/pipe.valgrind	
	
.PHONY: all clean test  