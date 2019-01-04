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
			int i=pos+dx+dy*WORLD_WIDTH;
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
{ // Announce a faction's destruction and free its memory
	report("%s has been obliterated!",f->name);
	release_color(f->color);
	free(f->name);
	free(f);
}
static struct faction *territory_search=NULL;
// ^ Static variable gets around callback limitations
bool in_territory(struct worldtile *w,int p)
{ // True if w[p] is in the sought territory
	return w[p].faction==territory_search;
}
bool townworthy(struct worldtile *w,int p)
{
	if (!in_territory(w,p))
		return false;
	for (int dx=-1;dx<=1;dx++)
		for (int dy=-1;dy<=1;dy++) {
			int i=p+dx+dy*WORLD_WIDTH;
			if (!legal_world_move(p,i))
				continue;
			if (w[i].town&&w[i].pop>0)
				return false;
		}
	return true;
}
void new_town(struct worldtile *w,struct faction *f)
{
	report("%s builds a new town",f->name);
	territory_search=f;
	int n=rand_loc(w,&townworthy);
	if (!n)
		return;
	w[n].town=true;
	w[n].town+=(1+TOWN_POP_CAP/1000)*f->stagnation;
}
void spread_to_tile(struct worldtile *w,struct faction *f,int target)
{ // Try to spread faction f to tile target in w
	struct faction *t=w[target].faction;
	switch (rand()%3) {
		case 0:
			if (t)
				t->size--;
			f->size++;
			w[target].faction=f;
			break;
		case 1:
			if (t)
				t->size--;
			w[target].faction=NULL;
			break;
		case 2:
			break;
	}
	if (w[target].town&&w[target].pop>0) {
		w[target].pop-=200;
		if (w[target].pop<-100) {
			w[target].town=false;
			w[target].pop=0;
		}
	}
	//draw_world_pos(w,target);
}
void spread_faction(struct worldtile *w,struct faction *f)
{ // Give one faction a chance to grow, map-wide
	int oldsize=f->size;
	if (!f)
		return;
	struct faction *t[WORLD_AREA]; // Territory map
	for (int i=0;i<WORLD_AREA;i++)
		t[i]=w[i].faction;
	for (int i=0;i<WORLD_AREA;i++) {
		if (t[i]!=f)
			continue;
		int target=spread_influence(w,i);
		if (target==i)
			continue;
		if (!t[i]&&rand()%2) { // 1/2 chance to spread to unoccupied land
			w[target].faction=f;
			f->size++;
		} else
			spread_to_tile(w,f,target);
	}
	if (f->size-oldsize<2)
		f->stagnation++;
	else
		f->stagnation=0;
}
void resolve_stagnation(struct worldtile *w,struct faction *f)
{ // Create a colony or rebellion if growth has stagnated
	if (!(rand()%200))
		new_town(w,f); // (possibly create a new town)
	if ((rand()%200)>f->stagnation)
		return;
	if (rand()%2) {
		report("%s seeks to colonize distant lands",f->name);
		form_colony(w,f);
	} else {
		int n=rand()%(2+f->size/200);
		report("%d rebellion%s form%s under %s",n,n==1?"":"s",n==1?"s":"",f->name);
		for (int i=0;i<n;i++)
			random_rebellion(w,f);
	}
	f->stagnation=0;
}
struct faction *factions[MAX_FACTIONS];
int num_factions=0;
void remove_faction(int i)
{
	destroy_faction(factions[i]);
	factions[i]=factions[--num_factions];
}
void cull_dead_factions(void)
{
	for (int i=0;i<num_factions;i++)
		if (!factions[i]->size) {
			remove_faction(i);
			i--;
		}
}
void advance_world(struct worldtile *w)
{ // Give all factions a chance to spread, and grow towns
	for (int i=0;i<WORLD_AREA;i++)
		if (w[i].town&&w[i].faction)
			w[i].pop+=w[i].pop<TOWN_POP_CAP;
	for (int i=0;i<num_factions;i++) {
		spread_faction(w,factions[i]);
		resolve_stagnation(w,factions[i]);
		factions[i]->age++;
	}
	if (num_factions&&!(rand()%20)) {
		// ^ 1/20 chance: Give one faction 5 turns to grow
		struct faction *f=factions[rand()%num_factions];
		report("%s surges forth!",f->name);
		for (int i=0;i<5;i++) {
			spread_faction(w,f);
		}
	}
	cull_dead_factions();
}
struct faction *new_faction(void)
{ // Allocate memory for a new faction
	struct faction *f=calloc(1,sizeof(struct faction));
	f->color=BLACK;
	return f;
}
struct faction *random_faction(void)
{ // Generate a faction with a random name and color and add it to the list
	if (num_factions>=MAX_FACTIONS) // If there are already too many,
		return factions[rand()%num_factions]; // pick one
	// Otherwise make a new one
	struct faction *f=new_faction();
	f->name=capitalize(random_word(3+rand()%3));
	f->color=choose_color();
	factions[num_factions++]=f;
	return f;
}
void place_uprising(struct worldtile *w,int i,struct faction *r,int n)
{ // Place a faction and let it spread for n turns
	if (w[i].faction)
		w[i].faction->size--;
	w[i].faction=r;
	r->size++;
	//draw_world_pos(w,i);
	for (int i=0;i<n;i++)
		spread_faction(w,r);
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
	// Unused faction will be removed on next cull_dead_factions()
}
bool coastal(struct worldtile *w,int p)
{ // True if w[p] has adjacent water
	if (w[p].elev<500)
		return false;
	for (int dx=-1;dx<=1;dx++)
		for (int dy=-1;dy<=1;dy++)
			if ((dx||dy)&&w[p+dx+dy*WORLD_HEIGHT].elev<500)
				return true;
	return false;
}
bool colonizable(struct worldtile *w,int p)
{ // True if coastal and outside sought territory
	return coastal(w,p)&&!in_territory(w,p);
}
void annex(struct worldtile *w,struct faction *r,struct faction *e)
{ // Turn all of one faction's territory into another's
	for (int i=0;i<WORLD_AREA;i++)
		if (w[i].faction==e) {
			w[i].faction=r;
			r->size++;
			e->size--;
			//draw_world_pos(w,i);
		}
}
void form_colony(struct worldtile *w,struct faction *f)
{ // Start an uprising on an enemy or unoccupied coast
	territory_search=f;
	int landing=rand_loc(w,&colonizable);
	if (!landing)
		return;
	struct faction *colony=new_faction();
	place_uprising(w,landing,colony,1+f->size/50);
	if (rand()%4||(num_factions==MAX_FACTIONS-1&&colony->size>f->size))
		annex(w,f,colony);
	else {
		struct faction *a=random_faction();
		if (num_factions==MAX_FACTIONS-1)
			report("The colony breaks away, choosing to join %s",a->name);
		else
			report("The colony breaks away, choosing the name %s",a->name);
		annex(w,a,colony);
	}
	free(colony);
}
bool descending_age(int a,int b)
{
	return factions[a]->age>factions[b]->age;
}
bool descending_size(int a,int b)
{
	return factions[a]->size>factions[b]->size;
}
