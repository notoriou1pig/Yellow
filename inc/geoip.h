
#ifndef		GEOIP_H__
# define	GEOIP_H__

# include <sys/socket.h>
# include <GeoIPCity.h>

# include "yellow.h"

# define GEOIP_OK 0
# define GEOIP_WRONG_FAMILY -1
# define GEOIP_NOT_FOUND -2
# define GEOIP_DB_FAIL -3

# define GEOIP_DB4 "/usr/share/yellow/GeoLiteCity.dat"
# define GEOIP_DB6 "/usr/share/yellow/GeoLiteCityv6.dat"

/*
 * Interesting fields : country_name, region, city, postal_code
 */
typedef GeoIPRecord t_geoip;
typedef struct sockaddr t_addr;

typedef struct s_geoip_db
{
	GeoIP *gi4;
	GeoIP *gi6;
} t_geoip_db;

int init_geo(t_geoip_db *db);
int addr_get_geo(t_geoip_db *db, t_addr const *addr, t_geoip **geo);

#endif	/* !GEOIP_H__ */

