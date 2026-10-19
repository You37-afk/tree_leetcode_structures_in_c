#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct TreeNode
{
    int val;
    struct TreeNode* left;
    struct TreeNode* right;
};

static int countFBT(int n)
{
    if(n==1) return 1;
    int total=0;
    for(int i=1;i<n-1;i+=2)
        total+=countFBT(i)*countFBT(n-1-i);
    return total;
}

static struct TreeNode* makeNode(int v)
{
    struct TreeNode* p=malloc(sizeof(*p));
    p->val=v;
    p->left=p->right=NULL;
    return p;
}

static struct TreeNode** build(int n,int* size)
{
    if(n==1)
    {
        struct TreeNode** res=malloc(sizeof(*res));
        res[0]=makeNode(0);
        *size=1;
        return res;
    }

    struct TreeNode** res=malloc(sizeof(*res)*countFBT(n));
    int k=0;
    for(int i=1;i<n-1;i+=2)
    {
        int ln,rn;
        struct TreeNode** L=build(i,&ln);
        struct TreeNode** R=build(n-1-i,&rn);
        for(int a=0;a<ln;a++)
        {
            for(int b=0;b<rn;b++)
            {
                struct TreeNode* root=makeNode(0);
                root->left=L[a];
                root->right=R[b];
                res[k++]=root;
            }
        }
        free(L); /* 只释放清单壳，节点已挂进新树 */
        free(R);
    }
    *size=k;
    return res;
}

struct TreeNode** allPossibleFBT(int n,int* returnSize)
{
    if(n%2==0)
    {
        *returnSize=0;
        return NULL;
    }
    return build(n,returnSize);
}

static int verify(struct TreeNode* t)
{
    if(t->val!=0) return -1;
    if(!t->left && !t->right) return 1;
    if(!t->left || !t->right) return -1;
    int a=verify(t->left);
    if(a<0) return -1;
    int b=verify(t->right);
    if(b<0) return -1;
    return 1+a+b;
}

static void expr(struct TreeNode* t,char* buf,int* pos)
{
    buf[(*pos)++]='0';
    if(t->left)
    {
        buf[(*pos)++]='(';
        expr(t->left,buf,pos);
        buf[(*pos)++]=',';
        expr(t->right,buf,pos);
        buf[(*pos)++]=')';
    }
}

int main(void)
{
    for(int n=1;n<=9;n++)
    {
        int sz=-1;
        struct TreeNode** list=allPossibleFBT(n,&sz);
        int expect=(n%2) ? countFBT(n) : 0;
        printf("== n=%d ==  %d tree(s) (expect %d) %s\n",n,sz,expect,
               sz==expect ? "COUNT OK" : "COUNT FAIL <<<");

        char b[64][128];
        int dup=0;
        for(int i=0;i<sz;i++)
        {
            int v=verify(list[i]);
            int pos=0;
            expr(list[i],b[i],&pos);
            b[i][pos]=0;
            printf("   [%2d] size=%d %s   %s\n",
                   i,v,v==n ? "OK" : "FAIL <<<",b[i]);
            for(int j=0;j<i;j++)
                if(strcmp(b[i],b[j])==0) dup=1;
        }
        if(dup) printf("   DUPLICATE FOUND <<<\n");
    }

    int sz=-1;
    struct TreeNode** list=allPossibleFBT(19,&sz);
    int allok=(sz==4862);
    for(int i=0;allok && i<sz;i++)
        if(verify(list[i])!=19) allok=0;
    printf("== n=19 (max odd) ==  count=%d (expect 4862) %s\n",sz,
           allok ? "ALL TREES OK" : "FAIL <<<");

    return 0;
}
