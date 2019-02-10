#include "erode.h"
void erode(short *arr,int w,int h)
{
	//short *tmp=malloc(w*h*sizeof(short));
	short tmp[w*h];
	for (int x=1;x<w-1;x++)
	for (int y=1;y<h-1;y++) {
		int sum=0,i=x+y*w;
		for (int dx=-1;dx<=1;dx++)
		for (int dy=-w;dy<=w;dy+=w)
			sum+=arr[i+dx+dy];
		tmp[i]=sum/9;
	}
	for (int x=1;x<w-1;x++)
	for (int y=1;y<h-1;y++) {
		int i=x+y*w;
		arr[i]=tmp[i];
	}
	//free(tmp);
}
