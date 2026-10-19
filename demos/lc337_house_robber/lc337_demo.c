#include <stdio.h>
#include <stdlib.h>

struct TreeNode
{
    int val;
    struct TreeNode* left;
    struct TreeNode* right;
};

static struct TreeNode* new_node(int v,struct TreeNode* l,struct TreeNode* r)
{
    struct TreeNode* p=malloc(sizeof(*p));
    if(p==NULL) exit(1);
    p->val=v;
    p->left=l;
    p->right=r;
    return p;
}

struct Rob
{
    int take;
    int skip;
};

static struct Rob dfs(struct TreeNode* u)
{
    if(u==NULL) return (struct Rob){0,0};
    struct Rob L=dfs(u->left);
    struct Rob R=dfs(u->right);
    struct Rob cur;
    cur.take=u->val+L.skip+R.skip;
    int left_best=L.take>L.skip ? L.take : L.skip;
    int right_best=R.take>R.skip ? R.take : R.skip;
    cur.skip=left_best+right_best;
    return cur;
}

int rob(struct TreeNode* root)
{
    struct Rob r=dfs(root);
    return r.take>r.skip ? r.take : r.skip;
}

static struct TreeNode* build(const int* a,int n)
{
    if(n==0 || a[0]<0) return NULL;
    struct TreeNode** q=malloc((size_t)n*sizeof(*q));
    int head=0,tail=0;
    struct TreeNode* root=new_node(a[0],NULL,NULL);
    q[tail++]=root;
    int i=1;
    while(head<tail && i<n)
    {
        struct TreeNode* cur=q[head++];
        if(i<n && a[i]>=0)
        {
            cur->left=new_node(a[i],NULL,NULL);
            q[tail++]=cur->left;
        }
        i++;
        if(i<n && a[i]>=0)
        {
            cur->right=new_node(a[i],NULL,NULL);
            q[tail++]=cur->right;
        }
        i++;
    }
    free(q);
    return root;
}

static void free_tree(struct TreeNode* u)
{
    if(u==NULL) return;
    free_tree(u->left);
    free_tree(u->right);
    free(u);
}

static void check(const char* name,struct TreeNode* root,int expect)
{
    int got=rob(root);
    printf("%-16s got=%d expect=%d %s\n",name,got,expect,got==expect ? "OK" : "FAIL");
    free_tree(root);
}

int main(void)
{
    int e1[]={3,2,3,-1,3,-1,1};
    int e2[]={3,4,5,1,3,-1,1};
    int e3[]={1};
    int e4[]={1,2,3};

    check("example 1",build(e1,(int)(sizeof(e1)/sizeof(e1[0]))),7);
    check("example 2",build(e2,(int)(sizeof(e2)/sizeof(e2[0]))),9);
    check("example 3",build(e3,(int)(sizeof(e3)/sizeof(e3[0]))),1);
    check("small root",build(e4,(int)(sizeof(e4)/sizeof(e4[0]))),5);

    return 0;
}
