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
	@rm -rf report.tasks PVS-Studio.log strace_out cppout.txt *.valgrind

test: 
#   Put the result on cppout.txt
	@cppcheck --quiet --enable=all --force --inconclusive . 2> cppout.txt
#	Put the result on report.tasks
	@pvs-studio-analyzer trace -- make
	@pvs-studio-analyzer analyze	
	plog-converter -a '64:1,2,3;GA:1,2,3;OP:1,2,3' -t tasklist -o report.tasks PVS-Studio.log 
#	Put the result on __.valgrind	
	@valgrind --leak-check=full --show-leak-kinds=all -v ./slave 2> slave.valgrind
	@valgrind --leak-check=full --show-leak-kinds=all -v ./solve 2> solve.valgrind
	@valgrind --leak-check=full --show-leak-kinds=all -v ./view 2> view.valgrind


.PHONY: all clean test  