//
// Created by lior on 07/04/20.
//

#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>
#include "netSnmpNicStatus.h"
/*
 * the variable we want to tie an OID to.  The agent will handle all
 * * GET and SET requests to this variable changing it's value as needed.
 */

static int netSnmpNicStatusObject = 0;

/*
 * our initialization routine, automatically called by the agent
 * (to get called, the function name must match init_FILENAME())
 */
void
init_netSnmpNicStatus(void)
{
    static oid  netSnmpNicStatusObject_oid[] =
            { 1, 3, 6, 1, 4, 1, 8072, 666, 3, 2, 1, 0 };


    /*
     * the line below registers our variables defined above as
     * accessible and makes it writable.  A read only version of any
     * of these registration would merely call
     * register_read_only_long_instance() instead.  The functions
     * called below should be consistent with your MIB, however.
     *
     * If we wanted a callback when the value was retrieved or set
     * (even though the details of doing this are handled for you),
     * you could change the NULL pointer below to a valid handler
     * function.
     */

    netsnmp_register_read_only_int_instance("netSnmpNicStatusObject",
                                            netSnmpNicStatusObject_oid,
                                            OID_LENGTH(netSnmpNicStatusObject_oid),
                                            &netSnmpNicStatusObject, NULL);
}
