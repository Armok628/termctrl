#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
void sort(int *arr,int size,bool (*f)(int,int))
{
	// Split
	if (size<2)
		return;
	int h=size/2;
	if (size>2) {
		sort(arr,h,f);
		sort(&arr[h],size-h,f);
	}
	// Merge
	int l=0,r=h;
	int tmp[size];
	for (int i=0;l<h||r<size;i++)
		if (r<size&&!f(arr[l],arr[r]))
			tmp[i]=arr[r++];
		else if (l<h)
			tmp[i]=arr[l++];
		else
			tmp[i]=arr[r++];
	for (int i=0;i<size;i++)
		arr[i]=tmp[i];
}
bool ascending(int a,int b)
{
	return a<b;
}
bool descending(int a,int b)
{
	return a>b;
}
