
# FIXME :
- Makefile
- Reverse the chained list
- Unit Tests
- main.c - do_action : send_packet(), read_cli() et write_out();

# Feature List

## Mandatory
- [TODO] Packets dump
- [TODO] Packets modification
- [TODO] Packet forging
- [TODO] Packet filtering
- [TODO] pcap Import/Export
- [TODO] (.DEB) Debian 7 64/32

## Level 1
- reverse DNS
- [DONE] GeoIP
- [DONE] Global sniffing (Promiscuous mode)
- [DRO?] (.RPM) RHEL/CentOS/Fedora 22 64/32
- [DROP] Arch linux User Repository

###DeadLine 15 November

## Level 2
- Packet blocking
- Network replay
- Traffic replication

## Optional Level
- TCP stream following
- Scripting

# Other
Run the binary with root rights

The packet chained list comming reversed. Need to be put in the right order.
(Certainly need preprocessing so it will be done at this time)

The dumping is very bad.
=> Build a function pointer table.

The Makefile didn't put objects file in the right folder.
(For now, they stay in the src folder)

# Note :

To build with geoloc enabled (geoloc.c and -lGeoIP) :

```shell
make -e GEOLOC=1
```
