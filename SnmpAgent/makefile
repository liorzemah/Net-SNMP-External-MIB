CC=gcc

TARGETS=snmpAgentDeamon

CFLAGS=-I. `net-snmp-config --cflags`
BUILDAGENTLIBS=`net-snmp-config --agent-libs`

# shared library flags (assumes gcc)
DLFLAGS=-fPIC -shared

all: $(TARGETS)

snmpAgentDeamon: snmpAgentDeamon.o
	$(CC) -o snmpAgentDeamon snmpAgentDeamon.o  mibs/nicNotification.c mibs/netSnmpNicStatus.c mibs/netSnmpNicStatusDescription.c mibs/counter.c $(BUILDAGENTLIBS)

