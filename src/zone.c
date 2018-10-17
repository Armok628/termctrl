#include "zone.h"
const char *grass_syms="\"';:.,`";
const int n_grass_syms=sizeof(grass_syms)/sizeof(grass_syms[0])-1;
struct tile *zonegen(struct tile *z/*,enum terrain t*/)
{
	if (!z)
		z=calloc(Z_AREA,sizeof(struct tile));
	for (int i=0;i<Z_AREA;i++) {
		z[i].bg_c=rand()%2?GREEN:LIGHT_GREEN;
		z[i].bg=grass_syms[rand()%n_grass_syms];
		/**/
		if (!(rand()%500)) {
			z[i].e=spawn(&monstertest);
		}
		/**/
	}
	/**/
	for (int i=0;i<Z_AREA/96;i++)
		rand_room(z);
	fix_rooms(z);
	fix_gaps(z);
	fix_gaps(z);
	fix_gaps(z);
	/**/
	return z;
}
