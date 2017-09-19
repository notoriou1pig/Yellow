#include <pcap.h>

#include "yellow.h"
#include "prototypes.h"

static char	*_init_fname(const char *filename)
{
	char 	*fname;

	fname = xmalloc(strlen(filename) + 1);
	fname = strncpy(fname, filename, strlen(filename));
	return fname;
}

static void	_write_header(pcap_t *pfile)
{
	/*
	** 4 Bytes - Magic number (endianess)
	** 2 Bytes - Version Major
	** 2 Bytes - Version Minor
	** 4 Bytes - Timezone Offset
	** 4 Bytes - Timestamp accuracy
	** 4 bytes - Snapshot length
	** 4 Bytes - Link-layer header
	*/
	struct pcap_file_header	phdr;

	phdr.magic = 0xa1b2c3d4;
	phdr.version_major = PCAP_VERSION_MAJOR;
	phdr.version_minor = PCAP_VERSION_MINOR;
	phdr.thiszone = 0x0;
	phdr.sigfigs = 0x0;
	phdr.snaplen = 0xffff;
	// To be checked afterward
	phdr.linktype = 0x1;
	if (write(pfile->fd, &phdr, sizeof(phdr)) == -1)
		err(1, "write");
}

static void	_switch_packet(t_packet *p)
{
#if __BYTE_ORDER == __LITTLE_ENDIAN
  p->datalink_layer->type.low = p->datalink_layer->type.low +
    p->datalink_layer->type.high;
  p->datalink_layer->type.high = p->datalink_layer->type.low -
    p->datalink_layer->type.high;
  p->datalink_layer->type.low = p->datalink_layer->type.low -
    p->datalink_layer->type.high;
#endif   
}

static void _write_packet(pcap_dumper_t *dfile, t_packet *packet)
{
	/*
	** 4 Bytes - Timestamp / second value
	** 4 Bytes - Timestamp / microsecond value
	** 4 Bytes - Length of captured packet data (header not included)
	** 4 Bytes - Un-truncated length of captured packet data
	*/
	struct pcap_pkthdr	pkthdr;

	pkthdr.ts.tv_sec = packet->ts.tv_sec;
	pkthdr.ts.tv_usec = packet->ts.tv_usec;
	pkthdr.caplen = packet->size;
	pkthdr.len = packet->size;
	_switch_packet(packet);
	pcap_dump((u_char *)dfile, &pkthdr, (u_char *)packet->data);
}

void				pcap_export(const char *fname, t_packet *p)
{
	pcap_t 			*pfile;
	pcap_dumper_t	*dfile;

	if (p == NULL)
	  return ;
	if (fname == NULL)
		fname = _init_fname(fname);
	pfile = pcap_open_dead(DLT_EN10MB, 262144);
	if ((dfile = pcap_dump_open(pfile, fname)) == NULL)
	{
		(void) fprintf(stderr, "%s\n", pcap_geterr(pfile));
		pcap_close(pfile);
		exit(0);
	}
	_write_header(pfile);
	while (p->prev != NULL)
		p = p->prev;
	while (p != NULL)
	{
		_write_packet(dfile, p);
		p = p->next;
	}
	pcap_dump_close(dfile);
	pcap_close(pfile);
}
