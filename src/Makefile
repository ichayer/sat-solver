GCC = gcc
GCCFLAGS = -g -Wall -std='c99' -lpthread -lrt

EXECUTABLES := solve slave view

OUTPUT = results.txt

EXEC = Application Slave View

APPLICATION_C = application.c

SLAVE_C = slave.c

VIEW_C = view.c

SMANAGER_C = ADTs/slaveManagerADT.c

POSSHMADT_C = ADTs/posShmADT.c

LIB_C = lib.c

all: $(EXEC)

Slave:	
	@$(GCC) $(GCCFLAGS) $(SLAVE_C) $(LIB_C) -o slave
	@echo "Slave successfully compiled"

Application:	
	@$(GCC) $(GCCFLAGS)  $(SMANAGER_C) $(LIB_C) $(APPLICATION_C) $(POSSHMADT_C) -o solve
	@echo "Solve successfully compiled"

View:
	@$(GCC) $(GCCFLAGS) $(LIB_C) $(VIEW_C) $(POSSHMADT_C) -o view
	@echo "View successfully compiled"

clean:
	@rm -rf $(EXECUTABLES)
	@rm -rf $(OUTPUT)
	@rm -rf check

check:
	@mkdir check
	@cppcheck --quiet --enable=all --force --inconclusive . 2> ./check/cppout.txt

	@pvs-studio-analyzer trace -- make
	@pvs-studio-analyzer analyze	
	@plog-converter -a '64:1,2,3;GA:1,2,3;OP:1,2,3' -t tasklist -o ./check/report.tasks ./PVS-Studio.log 
	
	@mv strace_out check
	@rm PVS-Studio.log
	
.PHONY: all clean check