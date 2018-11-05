#include "faction.h"
int spread_influence(struct worldtile *w,int pos)
{
	struct faction *f=w[pos].faction;
	int choices[9],n=0;
	for (int dx=-1;dx<=1;dx++)
		for (int dy=-1;dy<=1;dy++) {
			int i=pos+dx+dy*W_WIDTH;
			if (!legal_world_move(pos,i))
				continue;
			if (w[i].elev>=500&&w[i].faction!=f)
				choices[n++]=i;
		}
	if (!n)
		return pos;
	return choices[rand()%n];
}
void spread_faction(struct worldtile *w,struct faction *f)
{
	for (int i=0;i<W_AREA;i++) {
		if (w[i].faction!=f)
			continue;
		int target=spread_influence(w,i);
		if (target==i)
			continue;
		if (!w[i].faction&&rand()%2)
			w[target].faction=f;
		else
			switch (rand()%3) {
			case 0:
				w[target].faction=f;
				break;
			case 1:
				w[target].faction=NULL;
				break;
			case 2:
				break;
			}
	}
}
