
#include <sys/socket.h>

#include "geoip.h"
#include "yellow.h"

static sa_family_t addr_get_type(t_addr const *addr)
{
	return addr->sa_family;
}

int init_geo(t_geoip_db *db)
{
	db->gi4 = GeoIP_open(GEOIP_DB4, GEOIP_INDEX_CACHE);
	if (db->gi4 == NULL)
	{
		return GEOIP_DB_FAIL;
	}
	GeoIP_set_charset(db->gi4, GEOIP_CHARSET_UTF8);

	db->gi6 = GeoIP_open(GEOIP_DB6, GEOIP_INDEX_CACHE);
	if (db->gi6 == NULL)
	{
		return GEOIP_DB_FAIL;
	}
	GeoIP_set_charset(db->gi6, GEOIP_CHARSET_UTF8);
	return GEOIP_OK;
}

int addr_get_geo(t_geoip_db *db, t_addr const *addr, t_geoip **geo)
{
	if (addr_get_type(addr) == AF_INET)
	{
		/* IPv4 */
		*geo = GeoIP_record_by_addr(db->gi4, addr->sa_data);
	}
	else if (addr_get_type(addr) == AF_INET6)
	{
		/* IPv6 */
		*geo = GeoIP_record_by_addr_v6(db->gi6, addr->sa_data);
	}
	else
	{
		return GEOIP_WRONG_FAMILY;
	}
	if (*geo == NULL)
	{
		return GEOIP_NOT_FOUND;
	}
	return GEOIP_OK;
}


