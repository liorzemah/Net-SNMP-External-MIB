//
// Created by lior on 08/04/20.
//



/*
 * start be including the appropriate header files
 */
#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>

/*
 * contains prototypes
 */
#include "counter.h"

static int  counter = 5;
static int  direction = 1;
/*
 * our initialization routine
 * (to get called, the function name must match init_FILENAME()
 */
void
init_counter(void)
{
    static oid  netSnmpCounter_oid[] =
            { 1, 3, 6, 1, 4, 1, 8072, 666, 3, 2, 3, 0 };


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

    netsnmp_register_read_only_int_instance("netSnmpCounter",
                                            netSnmpCounter_oid,
                                            OID_LENGTH(netSnmpCounter_oid),
                                            &counter, NULL);


    DEBUGMSGTL(("example_nic_notification",
            "initializing (setting callback alarm)\n"));
    snmp_alarm_register(1,     /* seconds */
                        1,      /* repeat (every X seconds). */
                        send_counter_notification,      /* our callback */
                        NULL    /* no callback data needed */
    );
}

/**
 * change direction of counter (up or down) by the it's value
 */
void check_direction()
{
    if (counter >= 10)
    {
        direction = -1;
    }
    if (counter <= 0)
    {
        direction = 1;
    }
}

void send_counter_notification(unsigned int clientreg, void *clientarg)
{
    /*
     * define the OID for the notification we're going to send
     * NET-SNMP-EXTERNALS-MIB::netSnmpCounterNotification
     */
    oid             notification_oid[] =
            { 1, 3, 6, 1, 4, 1, 8072, 666, 3, 0, 2 };
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
     * NET-SNMP-EXTERNALS-MIB::netSnmpCounter
     */
    oid      counter_oid[]   = { 1, 3, 6, 1, 4, 1, 8072, 666, 3, 2, 3, 0 };
    size_t   counter_oid_len = OID_LENGTH(counter_oid);

    /*
     * here is where we store the variables to be sent in the trap
     */
    netsnmp_variable_list *notification_vars = NULL;
    counter += direction;
    check_direction();

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
                              counter_oid, counter_oid_len,
                              ASN_INTEGER,
                              (u_char *)&counter,
                              sizeof(counter));

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
