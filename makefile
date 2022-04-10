GCC = gcc
GCCFLAGS = -g -Wall -std='c99' -lpthread -lrt

EXEC = Application Slave View

SLAVE_C = slave.c
SLAVE_O = slave.o

SMANAGER_C = slaveManagerADT.c
SMANAGER_O = slaveManagerADT.o

POSSHMADT_C = posShmADT.c
POSSHMADT_O = posShmADT.o

VIEW_C = view.c
VIEW_O = view.o

ERRORS_C = myerror.c
ERRORS_O = myerror.o

APPLICATION_C = application.c
APPLICATION_O = application.o

all: $(EXEC)

Slave:	
	$(GCC) $(GCCFLAGS) $(SLAVE_C) $(ERRORS_C) -o slave

Application:	
	$(GCC) $(GCCFLAGS)  $(SMANAGER_C) $(ERRORS_C) $(APPLICATION_C) $(POSSHMADT_C) -o solve

View:
	$(GCC) $(GCCFLAGS) $(ERRORS_C) $(VIEW_C) $(POSSHMADT_C) -o view

clean:
	rm -rf $(SLAVE_O) $(SMANAGER_O) $(ERRORS_O) $(APPLICATION_O) $(POSSHMADT_O) solve slave view

