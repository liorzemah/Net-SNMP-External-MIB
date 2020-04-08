#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>
#include "mibs/nicNotification.h"
#include "mibs/netSnmpNicStatus.h"
#include "mibs/netSnmpNicStatusDescription.h"
#include "mibs/counter.h"

int main(){
    /* initialize the agent library */
    init_agent("snmpd");

    /* initialize mib code */
    init_nicNotification();
    init_counter();
    init_netSnmpNicStatus();
    init_netSnmpNicStatusDescription();

    /* snmpAgentDeamon will be used to read snmpd.conf files. */
    init_snmp("snmpd");

    /* For snmp master agent, initial the ports */
    init_master_agent();  /* open the port to listen on (defaults to udp:161),
*                                                  used for recognized external mib's objects*/


    /* Prevent close of the agent */
    while(1)
    {
        agent_check_and_process(1); /* 0 == don't block */
    }
    return 1;
}
