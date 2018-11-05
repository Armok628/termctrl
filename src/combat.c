#include "combat.h"
static inline bool chance(int p)
{
	return (rand()%100)<=p;
}
int max_damage(struct entity *a,struct entity *d)
{
	return 25+a->str-d->str;
}
int dodge(struct entity *a,struct entity *d)
{
	return 50+(d->agi-a->agi)*5/2;
}
int miss(struct entity *a)
{
	return 100-5*a->agi;
}
bool attack(struct tile *z,int a,int d)
{ // Returns true if defender is killed
	struct entity *atkr=z[a].e,*defr=z[d].e;
	if (chance(dodge(atkr,defr)))
		report("%e attacks %e but is dodged",atkr,defr);
	else if (chance(miss(atkr)))
		report("%e attacks %e but misses",atkr,defr);
	else {
		int dmg=rand()%max_damage(atkr,defr);
		defr->hp-=dmg;
		report("%e attacks %e for %d damage",atkr,defr,dmg);
		if (defr->hp<=0) {
			kill_entity(z,d);
			fputs(", killing it!",stdout);
			return true;
		}
		return false;
	}
	return false;
}
