GCC = gcc
GCCFLAGS = -g -Wall -std='c99' -lpthread

EXEC = Application Slave

SLAVE_C = slave.c
SLAVE_O = slave.o

SMANAGER_C = slaveManagerADT.c
SMANAGER_O = slaveManagerADT.o

ERRORS_C = myerror.c
ERRORS_O = myerror.o

APPLICATION_C = application.c
APPLICATION_O = application.o

all: $(EXEC)

Slave:	
	$(GCC) $(GCCFLAGS) $(SLAVE_C) $(ERRORS_C) -o slave

Application:	
	$(GCC) $(GCCFLAGS)  $(SMANAGER_C) $(ERRORS_C) $(APPLICATION_C) -o solve

clean:
	rm -rf $(SLAVE_O) $(SMANAGER_O) $(ERRORS_O) $(APPLICATION_O) 

