//
// Created by lior on 07/04/20.
//

#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>
#include <net-snmp/agent/watcher.h>

#include <net-snmp/agent/instance.h>
#include <net-snmp/agent/scalar.h>
#include "netSnmpNicStatusDescription.h"

#define STR_MAX_SIZE 256

/*
 * the storage for our string. It must be static or allocated.
 * we use static here for simplicity.
 */
static char netSnmpNicStatusDescriptionObject[STR_MAX_SIZE] = "Succeeded";

/*
 * our initialization routine, automatically called by the agent
 * (to get called, the function name must match init_FILENAME())
 */
void
init_netSnmpNicStatusDescription(void)
{
    /*
     * the OID we want to register our string at.  This should be a
     * fully qualified instance.  In our case, it's a scalar at:
     * NET-SNMP-EXTERNALS-MIB::netSnmpNicStatusDescriptionObject.0  (note the trailing
     *  0 which is required for any instantiation of any scalar object)
     */

    static oid  netSnmpNicStatusDescriptionObject_oid[] =
            { 1, 3, 6, 1, 4, 1, 8072, 666, 3, 2, 2, 0 };


    /*
     * variables needed for registration
     */
    netsnmp_handler_registration *reginfo;
    netsnmp_watcher_info* watcher_info;
    int watcher_flags;

    /*
     * If we wanted a callback when the value was retrieved or set
     * (even though the details of doing this are handled for you),
     * you could change the NULL pointer below to a valid handler
     * function.
     *
     * Change RWRITE to RONLY for a read-only string.
     */
    reginfo = netsnmp_create_handler_registration("netSnmpNicStatusDescriptionObject", NULL,
                                                  netSnmpNicStatusDescriptionObject_oid,
                                                  OID_LENGTH(netSnmpNicStatusDescriptionObject_oid),
                                                  HANDLER_CAN_RONLY);

    /*
     * the three options for a string watcher are:
     *   fixed size string (length never changes)
     *   variable size (length can be 0 - MAX, for some MAX)
     *   c string (length can be 0 - MAX-1 for some max, \0 is not a valid
     *     character in the string, the length is provided by strlen)
     *
     * we'll use a variable length string.
     */
    watcher_flags = WATCHER_MAX_SIZE;

    /*
     * create the watcher info for our string.
     */
    watcher_info = netsnmp_create_watcher_info(netSnmpNicStatusDescriptionObject, strlen(netSnmpNicStatusDescriptionObject),
                                ASN_OCTET_STR, watcher_flags);


    /*
     * the line below registers our "my_string" variable above as
     * accessible and makes it writable.
     */
    netsnmp_register_watched_instance(reginfo, watcher_info);

}
