# Net-SNMP-External-MIB
A guide to adding new mib to snmp deamon, using Net-SNMP. 

In this guide we work with the library Net-SNMP v5.4.4 and with SNMP v2c messages. 

Step 1:

Dowload the package of net-snmp 5.4.4. 
You can acheive it from this link: https://sourceforge.net/projects/net-snmp/files/net-snmp/5.4.4/

Step 2:

Extract the downloaded package and called it 'net-snmp-5.4.4'.

Step 3:

Create your own mib:
NET-SNMP-EXTERNALS-MIB.txt is mib we created for our guide this mib check our nic status and increase/decrease counter any second.
The mib contains 3 objects: 
- NicStatus (Integer32)
- NicStatusDescription (OCT STRING 0-255)
- Counter (Integer32)
and contains notify about them:
- NicNotification (NOTIFICATION TYPE)
- CounterNotification (NOTIFICATION TYPE)

When you create your own mib you could copy the format from our NET-SNMP-EXTERNALS-MIB.txt and add your objects.

In this mib we achieve support in two modes: get and trap.
snmpget - for request knowledge about object (like NicStatus), the manager request from the agent this piece of knowledge.
snmptrap - for send knowledge about object, the agent send this piece to the manager.

After you create your own mib you can continue to the next step.

Step 4:
Implement the code to each object and notification.
The creation of the mib is not enougth for our agent to know what is the value of the object or when to send objects' value.

Net-SNMP do the link works for us, we just need to create .c and .h file for each object and notification and place them in 'net-snmp-5.4.4/agent/mibgroup' location.
For our example we create:
- nicNotification.c nicNotification.h
- netSnmpNicStatus.c netSnmpNicStatus.h
- netSnmpNicStatusDescription.c netSnmpNicStatusDescription.h
- counter.c counter.h

Inside the code you can declare if send traps any x seconds or add pre-conditions or filter that decides if send the trap in that moment or not, and chance object's value.
For example we can declare that our 'Counter' will increase his value by 1 any 30 seconds and only if the value is even send the trap.
So we send trap of 'Counter' for values: 2,4,6,...
And if our manager want to know what the current value he just need to send get request(by command: snmpget).


Step 5:

You have to configure the package to find them and compile them into the agent. To do this, you run the configure script giving it the extra mibs names you want it to load:

$ ./configure --prefix='path/to/directory' --enable-embedded-perl --enable-shared --exec-prefix='path/to/directory' --with-mib-modules="nicNotification netSnmpNicStatus netSnmpNicStatusDescription counter"

Build your new agent with your new code in it by running make:

$ make
	

Install the whole lot by running make install:

$ sudo make install

Step 6:

Start the snmp agent deamon (called snmpd):

$ sudo 'path/to/directory'/sbin/snmpd 

If you want to read condiguration from spesific file:

$ sudo 'path/to/directory'/sbin/snmpd -c 'path/to/configure/snmpd.conf' -C  


snmpget example:
- snmpget -v2c -Mshare/snmp/mibs -c public 127.0.0.1 NET-SNMP-EXTERNALS-MIB::netSnmpNicStatusDescription.0

share/snmp/mibs - the location of the mibs inside 'net-snmp-5.4.4'


And that all, you can open wireshark and look at the snmp messages traffic.

	

