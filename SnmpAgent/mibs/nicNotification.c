
/*
 * start be including the appropriate header files
 */
#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>

/*
 * contains prototypes
 */
#include "nicNotification.h"

/*
 * our initialization routine
 * (to get called, the function name must match init_FILENAME()
 */
void
init_nicNotification(void)
{
    DEBUGMSGTL(("example_nic_notification",
            "initializing (setting callback alarm)\n"));
    snmp_alarm_register(0,     /* seconds */
                        0,      /* repeat (every X seconds). */
                        send_nic_notification,      /* our callback */
                        NULL    /* no callback data needed */
    );
}

/* Example of check function that declare the value of the object */
int check_nic_status(char* device)
{
    /* Some code: tests and checks the device.
     * In case of failure:
     *  return fail status.
     * In case of known status:
     *  return status.
     * Default:
     * */
    printf("Check nic status\n");
    return 0; /* mean nic status: success */
}

void send_nic_notification(unsigned int clientreg, void *clientarg)
{
    /*
     * define the OID for the notification we're going to send
     * NET-SNMP-EXTERNALS-MIB::netSnmpNicNotification
     */
    oid             notification_oid[] =
            { 1, 3, 6, 1, 4, 1, 8072, 666, 3, 0, 1 };
    size_t          notification_oid_len = OID_LENGTH(notification_oid);

    /*
     * In the notification, we have to assign our notification OID to
     * the snmpTrapOID.0 object. Here is it's definition.
     */
    oid             objid_snmptrap[] = { 1, 3, 6, 1, 6, 3, 1, 1, 4, 1, 0 };
    size_t          objid_snmptrap_len = OID_LENGTH(objid_snmptrap);

    /*
     * define the OIDs for the varbinds we're going to include
     *  with the notification -
     * NET-SNMP-EXTERNALS-MIB::netSnmpNicStatus  and
     * NET-SNMP-EXTERNALS-MIB::netSnmpNicStatusDescription
     */
    oid      nic_status_oid[]   = { 1, 3, 6, 1, 4, 1, 8072, 666, 3, 2, 1, 0 };
    size_t   nic_status_oid_len = OID_LENGTH(nic_status_oid);
    oid      nic_status_description_oid[]   = { 1, 3, 6, 1, 4, 1, 8072, 666, 3, 2, 2, 0 };
    size_t   nic_status_description_oid_len = OID_LENGTH(nic_status_description_oid);

    /*
     * here is where we store the variables to be sent in the trap
     */
    netsnmp_variable_list *notification_vars = NULL;
    const char *nic_status_description = "Nic binding success";
    static int  nic_status = 0; /* default value */

    check_nic_status("wlp8s0");

    DEBUGMSGTL(("example_nic_notification", "defining the trap\n"));

    /*
     * add in the trap definition object
     */
    snmp_varlist_add_variable(&notification_vars,
            /*
             * the snmpTrapOID.0 variable
             */
                              objid_snmptrap, objid_snmptrap_len,
            /*
             * value type is an OID
             */
                              ASN_OBJECT_ID,
            /*
             * value contents is our notification OID
             */
                              (u_char *) notification_oid,
            /*
             * size in bytes = oid length * sizeof(oid)
             */
                              notification_oid_len * sizeof(oid));

    /*
     * add in the additional objects defined as part of the trap
     */

    snmp_varlist_add_variable(&notification_vars,
                              nic_status_oid, nic_status_oid_len,
                              ASN_INTEGER,
                              (u_char *)&nic_status,
                              sizeof(nic_status));

    snmp_varlist_add_variable(&notification_vars,
                              nic_status_description_oid, nic_status_description_oid_len,
                              ASN_OCTET_STR,
                              (u_char *)&nic_status_description,
                              strlen(nic_status_description));


    /*
     * send the trap out.  This will send it to all registered
     * receivers (see the "SETTING UP TRAP AND/OR INFORM DESTINATIONS"
     * section of the snmpd.conf manual page.
     */

    DEBUGMSGTL(("example_nic_notification", "sending trap \n"));
    send_v2trap(notification_vars);
    /*
     * free the created notification variable list
     */
    DEBUGMSGTL(("example_nic_notification", "cleaning up\n"));
    snmp_free_varbind(notification_vars);
}