cmake_minimum_required(VERSION 3.14)
project(SnmpAgent C)

set(CMAKE_C_STANDARD 99)

set(SNMP_LIBS_DIR /path/to/dir/lib)  # Change location to the place which all snmp libs found.

add_executable(SnmpAgent snmpAgentDeamon.c mibs/nicNotification.c mibs/nicNotification.h mibs/netSnmpNicStatus.c mibs/netSnmpNicStatus.h mibs/netSnmpNicStatusDescription.c mibs/netSnmpNicStatusDescription.h mibs/counter.c mibs/counter.h)

target_link_libraries(SnmpAgent
        ${SNMP_LIBS_DIR}/libnetsnmpagent.so
        ${SNMP_LIBS_DIR}/libnetsnmphelpers.so
        ${SNMP_LIBS_DIR}/libnetsnmptrapd.so
        ${SNMP_LIBS_DIR}/libnetsnmpmibs.so
        ${SNMP_LIBS_DIR}/libnetsnmp.so
        perl
        )