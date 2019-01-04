#include "path.h"
static int dists[ZONE_AREA];
bool plan_path(struct tile *z,int from,int to)
{
	for (int i=0;i<ZONE_AREA;i++)
		dists[i]=is_wall(&z[i])?-2:-1;
	dists[from]=-1;
	dists[to]=0;
	bool possible=true;
	for (int d=1;possible&&dists[from]<0;d++) {
		possible=false;
		for (int i=0;i<ZONE_AREA;i++) {
			if (dists[i]!=d-1)
				continue;
			for (int dx=-1;dx<=1;dx++)
			for (int dy=-1;dy<=1;dy++) {
				int n=i+dx+dy*ZONE_WIDTH;
				int xd=n%ZONE_WIDTH-i%ZONE_WIDTH;
				if ((!dx&&!dy)||xd<-1||xd>1||n<0||n>=ZONE_AREA)
					continue;
				if (dists[n]==-1) {
					dists[n]=d;
					possible=true;
				}
			}
		}
	}
	return possible;
}
char go_to(struct tile *z,int from,int to)
{
	if (!plan_path(z,from,to))
		return '0';
	int c[8],n_c=0,min=dists[from];
	for (int dx=-1;dx<=1;dx++)
	for (int dy=-1;dy<=1;dy++) {
		int n=from+dx+dy*ZONE_WIDTH;
		if (dists[n]<min) {
			n_c=1;
			min=dists[n];
			c[0]=n;
		} else if (dists[n]==min)
			c[n_c++]=n;
	}
	if (!n_c)
		return '0';
	return c[n_c>>1];//c[rand()%n_c];
}
