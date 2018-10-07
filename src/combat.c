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
		report("e s e s",atkr,"attacks",defr,"but is dodged");
	else if (chance(miss(atkr)))
		report("e s e s",atkr,"attacks",defr,"but misses");
	else {
		int dmg=rand()%max_damage(atkr,defr);
		defr->hp-=dmg;
		report("e s e s d s",atkr,"attacks",defr,"for",dmg,"damage");
		if (defr->hp<=0) {
			kill(z,d);
			fputs(", killing it!",stdout);
			return true;
		}
		return false;
	}
	return false;
}
