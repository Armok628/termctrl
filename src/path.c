#include "path.h"
static int dists[AREA];
bool plan_path(char *area,int from,int to)
{
	for (int i=0;i<AREA;i++)
		dists[i]=area[i]=='%'?-2:-1;
	dists[from]=-1;
	dists[to]=0;
	bool possible=true;
	for (int d=1;possible&&dists[from]<0;d++) {
		possible=false;
		for (int i=0;i<AREA;i++) {
			if (dists[i]!=d-1)
				continue;
			for (int dx=-1;dx<=1;dx++)
			for (int dy=-1;dy<=1;dy++) {
				int n=i+dx+dy*WIDTH;
				int xd=n%WIDTH-i%WIDTH;
				if ((!dx&&!dy)||xd<-1||xd>1||n<0||n>=AREA)
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
//// Temporary
void print_dists(void)
{
	for (int i=0;i<AREA;i++) {
		if (dists[i]==-2)
			printf("\033[1;30m%%");
		else if (dists[i]<0)
			printf("\033[m ");
		else {
			printf("\033[1;%dm%d",31+(6+dists[i]/10)%7,dists[i]%10);
		}
		if (i%WIDTH==WIDTH-1)
			putchar('\n');
	}
	printf("\033[m");
}
void show_path(char *area,int from,int to)
{
	plan_path(area,from,to);
	area[from]='O';
	int p=from;
	while (p!=to) {
		int mins[8],min=dists[p];
		int n_mins=0;
		for (int dx=-1;dx<=1;dx++)
		for (int dy=-1;dy<=1;dy++) {
			int n=p+dx+dy*WIDTH;
			int xd=n%WIDTH-p%WIDTH;
			if (xd<-1||xd>1||n<0||n>=AREA)
				continue;
			if (dists[n]<0)
				continue;
			if (dists[n]<min) {
				min=dists[n];
				n_mins=1;
				mins[0]=n;
			} else if (dists[n]==min) {
				mins[n_mins++]=n;
			}
		}
		if (!n_mins)
			return;
		p=mins[n_mins/2];//mins[rand()%n_mins];
		area[p]='*';
	}
	area[to]='X';
}
