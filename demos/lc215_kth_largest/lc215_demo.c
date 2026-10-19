#include <stdio.h>
#include <stdlib.h>

static void heapPush(int* heap,int* size,int x)
{
    int j=(*size)++;
    heap[j]=x;
    while(j>0)
    {
        int p=(j-1)/2;
        if(heap[p]<=heap[j]) break;
        int t=heap[p];
        heap[p]=heap[j];
        heap[j]=t;
        j=p;
    }
}

static void heapPopTop(int* heap,int* size)
{
    (*size)--;
    heap[0]=heap[*size];
    int p=0;
    for(;;)
    {
        int l=2*p+1,r=2*p+2,m=p;
        if(l<*size && heap[l]<heap[m]) m=l;
        if(r<*size && heap[r]<heap[m]) m=r;
        if(m==p) break;
        int t=heap[p];
        heap[p]=heap[m];
        heap[m]=t;
        p=m;
    }
}

int findKthLargest(int* nums,int numsSize,int k)
{
    int* heap=malloc((k+1)*sizeof(int)); /* k+1: push 后、pop 前会临时多一个 */
    int size=0;
    for(int i=0;i<numsSize;i++)
    {
        heapPush(heap,&size,nums[i]);
        if(size>k) heapPopTop(heap,&size);
    }
    int ans=heap[0];
    free(heap);
    return ans;
}

static void printHeap(const int* h,int size)
{
    printf("[");
    for(int i=0;i<size;i++)
    {
        if(i) printf(",");
        printf("%d",h[i]);
    }
    printf("]");
}

static void traceDemo(void)
{
    int a[]={3,2,1,5,6,4};
    int k=2,heap[16],size=0;

    printf("trace: nums=[3,2,1,5,6,4]  k=2   (heap[0] = club min)\n");
    for(int i=0;i<6;i++)
    {
        heapPush(heap,&size,a[i]);
        printf("  read %d: push -> ",a[i]);
        printHeap(heap,size);
        if(size>k)
        {
            heapPopTop(heap,&size);
            printf(", over k, pop top -> ");
            printHeap(heap,size);
        }
        printf("   club min = %d\n",heap[0]);
    }
    printf("  => kth largest = %d\n\n",heap[0]);
}

static int cmpInt(const void* a,const void* b)
{
    int x=*(const int*)a,y=*(const int*)b;
    return (x>y)-(x<y);
}

static int bySort(const int* nums,int n,int k)
{
    int* copy=malloc(n*sizeof(int));
    for(int i=0;i<n;i++) copy[i]=nums[i];
    qsort(copy,n,sizeof(int),cmpInt);
    int ans=copy[n-k];
    free(copy);
    return ans;
}

int main(void)
{
    int bad=0;
    struct { int nums[16]; int n,k,expect; } cases[]={
        {{3,2,1,5,6,4},6,2,5},
        {{3,2,3,1,2,4,5,5,6},9,4,4},
        {{-1,-5,-3},3,2,-3},
        {{7},1,1,7},
        {{2,1},2,2,1},
        {{5,5,5},3,2,5},
    };
    int nc=(int)(sizeof cases/sizeof cases[0]);
    for(int i=0;i<nc;i++)
    {
        int got=findKthLargest(cases[i].nums,cases[i].n,cases[i].k);
        printf("case %d: n=%-2d k=%-2d  got %-4d expect %-4d %s\n",
               i+1,cases[i].n,cases[i].k,got,cases[i].expect,
               got==cases[i].expect ? "OK" : "FAIL");
        if(got!=cases[i].expect) bad=1;
    }

    srand(12345);
    int fails=0,total=0;
    for(int t=0;t<2000;t++)
    {
        int n=rand()%50+1;
        int arr[50];
        for(int i=0;i<n;i++) arr[i]=rand()%41-20;
        for(int k=1;k<=n;k++)
        {
            total++;
            if(findKthLargest(arr,n,k)!=bySort(arr,n,k)) fails++;
        }
    }
    printf("random cross-check vs sort: %d values, fails=%d -> %s\n\n",
           total,fails,fails ? "FAIL" : "all pass");
    if(fails) bad=1;

    traceDemo();
    return bad ? 1 : 0;
}
