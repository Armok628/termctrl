#include "think.h"
static inline bool member(struct entitytype *s,struct entitytype **arr)
{
	for (int i=0;arr[i];i++)
		if (arr[i]==s)
			return true;
	return false;
}
bool enemy(struct entity *d,struct entity *a)
{ // Returns true if d is an enemy of a
	return member(d->type,a->type->enemies);
}
bool friend(struct entity *d,struct entity *a)
{ // Returns true if d is a friend of a
	return member(d->type,a->type->friends);
}
bool would_flee(struct entity *r,struct entity *e)
{
	if (friend(r,e))
		return false;
	int r_f=2*r->res+r->str+r->agi;
	int e_f=2*e->str+2*e->agi;
	int d=max_damage(e,r);
	if (e->wis<10)
		d=rand()%d;
	return r_f<e_f||d>r->hp;
}
char reverse_dir(char c)
{
	return ('0'+'0'+10)-c; // (10-(c-'0'))+'0'
}
static inline bool occupied(struct tile *z,int pos)
{
	return z[pos].fg||z[pos].e;
}
char alt_dir(struct tile *z,int pos,char c)
{
	static char *cw="0412753896",*ccw="0236159478";
	if (c=='0'||c=='5')
		return c;
	int to_c=pos+input_offset(c);
	if (legal_move(pos,to_c)&&!occupied(z,c))
		return c;
	char a=c,b=c;
	for (int i=0;i<5;i++) {
		a=cw[a-'0'];
		b=ccw[b-'0'];
		int to_a=pos+input_offset(a);
		int to_b=pos+input_offset(b);
		bool oa=true,ob=true;
		if (legal_move(pos,to_a))
			oa=occupied(z,to_a);
		if (legal_move(pos,to_b))
			ob=occupied(z,to_b);
		if (!oa&&!ob)
			break;
		else if (!oa)
			return a;
		else if (!ob)
			return b;
	}
	return rand()%2?a:b;
}
char think(struct tile *z,int pos)
{
	struct entity *e=z[pos].e;
	char moves[9];
	int n_m=0;
	for (char c='1';c<='9';c++) { // Look for opportunities
		int to=pos+input_offset(c);
		if (c=='5'||!legal_move(pos,to))
			continue;
		struct entity *t=z[to].e;
		if (t&&!friend(e,t)&&would_flee(e,t)&&!would_flee(t,e))
			moves[n_m++]=alt_dir(z,pos,reverse_dir(c));
		else if (t&&enemy(t,e))
			moves[n_m++]=c;
	}
	if (n_m)
		return moves[rand()%n_m];
	for (char c='1';c<='9';c++) { // No opportunities; look for any legal moves
		int to=pos+input_offset(c);
		if (c=='5'||!legal_move(pos,to))
			continue;
		if (z[to].fg&&(z[to].fg!='+'||~e->flags&OPENS_DOORS))
			continue;
		if (z[to].e&&friend(z[to].e,e))
			continue;
		moves[n_m++]=c;
	}
	return n_m?moves[rand()%n_m]:'0';
}
