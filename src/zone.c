#include "zone.h"
#define RCHAR(s) (s[rand()%(sizeof(s)-1)])
#define PICK(a,b) (rand()%2?a:b)
#define INIT_BGS(bg_expr,bg_c_expr) \
for (int i=0;i<Z_AREA;i++) { \
	z[i].bg=bg_expr; \
	z[i].bg_c=bg_c_expr; \
}
const char *grass=",.;:\"'`";
const char *sand="~_";
struct tile *zonegen(struct tile *z,enum terrain t)
{
	if (!z)
		z=calloc(Z_AREA,sizeof(struct tile));
	switch (t) {
	case COLD_FIELD:
		INIT_BGS(RCHAR(grass),PICK(WHITE,LIGHT_GRAY));
		break;
	case FIELD:
		INIT_BGS(RCHAR(grass),PICK(GREEN,LIGHT_GREEN));
		break;
	case HOT_FIELD:
		INIT_BGS(RCHAR(grass),PICK(YELLOW,BROWN));
		break;
	default:
		INIT_BGS(' ',BLACK);
		return z;
	}
	/* Temporary */
	for (int i=0;i<Z_AREA;i++)
		if (!(rand()%500)) {
			z[i].e=spawn(&monstertest);
		}
	for (int i=0;i<Z_AREA/96;i++)
		rand_room(z);
	fix_rooms(z);
	fix_gaps(z);
	fix_gaps(z);
	fix_gaps(z);
	/**/
	return z;
}
