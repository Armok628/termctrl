#include "faction.h"
static color_t faction_colors[]={RED,GREEN,BROWN,BLUE,PURPLE,TEAL,LIGHT_GRAY};
static int n_faction_colors=7;
color_t choose_color(void)
{
	static range_t faction_color_range={RED,LIGHT_GRAY};
	if (MAX_FACTIONS<=7) {
		// If factions are only as numerous as their possible colors,
		// make sure colors don't repeat.
		int n=rand()%n_faction_colors;
		color_t c=faction_colors[n];
		faction_colors[n]=faction_colors[--n_faction_colors];
		return c;
	} else {
		// Otherwise, pick a valid color randomly
		return rrand(faction_color_range);
	}
}
void release_color(color_t c)
{
	if (MAX_FACTIONS<=7) {
		faction_colors[n_faction_colors++]=c;
	}
}
int spread_influence(struct worldtile *w,int pos)
{ // Decide which adjacent location to spread influence to
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
void destroy_faction(struct faction *f)
{ // Remove a faction from the factions list and free its memory
	int i=0;
	for (;i<num_factions;i++)
		if (factions[i]==f)
			break;
	release_color(f->color);
	free(f->name);
	free(f);
	factions[i]=factions[--num_factions];
}
void decr_size(struct faction *f)
{ // Decrement f's size, and remove it if it no longer exists
	if (!f)
		return;
	f->size--;
	if (f->size)
		return;
	report("%s has been obliterated!",f->name);
	destroy_faction(f);
}
void spread_faction(struct worldtile *w,struct faction *f)
{ // Give one faction a chance to grow, map-wide
	int oldsize=f->size;
	if (!f)
		return;
	struct faction *t[W_AREA]; // Territory map
	for (int i=0;i<W_AREA;i++)
		t[i]=w[i].faction;
	for (int i=0;i<W_AREA;i++) {
		if (t[i]!=f)
			continue;
		int target=spread_influence(w,i);
		if (target==i)
			continue;
		if (!t[i]&&rand()%2) {
			w[target].faction=f;
			incr_size(f);
		} else {
			struct faction *t=w[target].faction;
			switch (rand()%3) {
			case 0:
				decr_size(t);
				incr_size(f);
				w[target].faction=f;
				break;
			case 1:
				decr_size(t);
				w[target].faction=NULL;
				break;
			case 2:
				break;
			}
		}
	}
	if (f->size-oldsize<2) {
		f->stagnation++;
	} else
		f->stagnation=0;
}
void resolve_stagnation(struct worldtile *w,struct faction *f)
{ // Create a raid or rebellion if f's growth has stagnated
	if ((rand()%200)<f->stagnation) {
		f->stagnation=0;
		int n;
		switch (rand()%2) {
		case 0:
			report("%s seeks to colonize distant lands",f->name);
			form_colony(w,f);
			break;
		case 1:
			n=rand()%(2+f->size/200);
			report("%d rebellion%s form%s under %s",n,n==1?"":"s",n==1?"s":"",f->name);
			for (int i=0;i<n;i++)
				random_rebellion(w,f);
		}
	}
}
struct faction *factions[MAX_FACTIONS];
int num_factions=0;
void spread_all_factions(struct worldtile *w)
{ // Give all factions a chance to spread
	for (int i=0;i<num_factions;i++) {
		spread_faction(w,factions[i]);
		resolve_stagnation(w,factions[i]);
	}
	if (num_factions&&!(rand()%20)) { // 1/20 chance: Give one faction 5 turns to grow
		struct faction *f=factions[rand()%num_factions];
		report("%s surges forth!",f->name);
		for (int i=0;i<5;i++) {
			spread_faction(w,f);
		}
	}
}
struct faction *random_faction(void)
{ // Generate a faction with a random name and color
	if (num_factions>=MAX_FACTIONS) // If there are already too many,
		return factions[rand()%num_factions]; // pick one
	// Otherwise make a new one
	struct faction *f=malloc(sizeof(struct faction));
	f->name=random_word(3+rand()%3);
	f->name[0]+='A'-'a';
	f->color=choose_color();
	f->size=0;
	f->stagnation=0;
	factions[num_factions++]=f;
	return f;
}
void place_uprising(struct worldtile *w,int i,struct faction *r,int n)
{ // Let a faction spread for n turns
	decr_size(w[i].faction);
	w[i].faction=r;
	incr_size(r);
	for (int i=0;i<n;i++)
		spread_faction(w,r);
}
static struct faction *territory_search=NULL;
// ^ Static variable gets around callback limitations
bool in_territory(struct worldtile *w,int p)
{ // True if w[p] is in the sought territory
	return w[p].faction==territory_search;
}
void random_rebellion(struct worldtile *w,struct faction *f)
{ // Summon an uprising in a random area of f's territory
	if (!f)
		return;
	struct faction *r=random_faction();
	territory_search=f;
	int p=rand_loc(w,&in_territory);
	if (p)
		place_uprising(w,p,r,1+f->size/20);
}
bool coastal(struct worldtile *w,int p)
{ // True if w[p] has adjacent water
	if (w[p].elev<500)
		return false;
	for (int dx=-1;dx<=1;dx++)
		for (int dy=-1;dy<=1;dy++)
			if ((dx||dy)&&w[p+dx+dy*W_HEIGHT].elev<500)
				return true;
	return false;
}
bool colonizable(struct worldtile *w,int p)
{ // True if coastal and outside sought territory
	return coastal(w,p)&&!in_territory(w,p);
}
void annex(struct worldtile *w,struct faction *r,struct faction *e)
{ // Turn all of one faction's territory into another's
	if (e->size>r->size||e==r)
		return;
	for (int i=0;i<W_AREA;i++)
		if (w[i].faction==e) {
			w[i].faction=r;
			incr_size(r);
		}
	destroy_faction(e);
}
void form_colony(struct worldtile *w,struct faction *f)
{ // Start an uprising on an enemy or unoccupied coast
	struct faction *party=random_faction();
	territory_search=f;
	int landing=rand_loc(w,&colonizable);
	if (!landing)
		return;
	place_uprising(w,landing,party,1+f->size/50);
	if (rand()%3)
		annex(w,f,party);
}
