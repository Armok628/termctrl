#include "sort.h"
void swap(int *a,int *b)
{
	int c=*a;
	*a=*b;
	*b=c;
}
void insertion_sort(int *arr,int len,bool (*f)(int,int))
{
	for (int i=0;i<len-1;i++)
		for (int j=i+1;j&&f(arr[j],arr[j-1]);j--)
			swap(&arr[j-1],&arr[j]);
}
bool ascending(int a,int b)
{
	return a<b;
}
bool descending(int a,int b)
{
	return a>b;
}
void reverse(int *arr,int len)
{
	for (int i=0;i<len/2;i++)
		swap(&arr[i],&arr[len-i-1]);
}
void rand_fixed_sum(int *ret,int len,int sum)
{
	int arr[len+1];
	arr[0]=0;
	for (int i=1;i<len;i++)
		arr[i]=rand()%sum;
	arr[len]=sum;
	insertion_sort(arr,len,&ascending);
	for (int i=0;i<len;i++)
		ret[i]=arr[i+1]-arr[i];
}
