#include "faction.h"
int spread_influence(struct worldtile *w,int pos)
{
	int choices[9],n=0;
	for (int dx=-1;dx<=1;dx++)
		for (int dy=-1;dy<=1;dy++) {
			int i=pos+dx+dy*W_WIDTH;
			if (!legal_world_move(pos,i))
				continue;
			if (w[i].elev>=500)
				choices[n++]=i;
		}
	if (!n)
		return pos;
	return choices[rand()%n];
}
void spread_factions(struct worldtile *w)
{
	struct faction *change[W_AREA]={0};
	for (int i=0;i<W_AREA;i++) {
		struct faction *f=w[i].faction;
		if (!f)
			continue;
		int target=spread_influence(w,i);
		if (target==i)
			continue;
		if (!change[target])
			change[target]=f;
		else {
			switch (rand()%3) {
			case 0:
				change[target]=f;
				break;
			case 1:
				change[target]=NULL;
				break;
			case 2:
				break;
			}
		}
	}
	for (int i=0;i<W_AREA;i++)
		if (change[i])
			w[i].faction=change[i];
}
