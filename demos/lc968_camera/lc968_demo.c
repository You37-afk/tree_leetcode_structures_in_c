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

enum
{
    NEED=0,
    COVERED=1,
    CAMERA=2
};

static int dfs(struct TreeNode* u,int* cameras)
{
    if(u==NULL) return COVERED;
    int l=dfs(u->left,cameras);
    int r=dfs(u->right,cameras);
    if(l==NEED || r==NEED)
    {
        (*cameras)++;
        return CAMERA;
    }
    if(l==CAMERA || r==CAMERA)
    {
        return COVERED;
    }
    return NEED;
}

int minCameraCover(struct TreeNode* root)
{
    int cameras=0;
    if(dfs(root,&cameras)==NEED)
    {
        cameras++;
    }
    return cameras;
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
    int got=minCameraCover(root);
    printf("%-18s got=%d expect=%d %s\n",name,got,expect,got==expect ? "OK" : "FAIL");
    free_tree(root);
}

int main(void)
{
    int e1[]={0,0,-1,0,0};
    int e2[]={0,0,-1,0,-1,0,-1,-1,0};
    int e3[]={0};
    int full7[]={0,0,0,0,0,0,0};
    int chain4[]={0,0,-1,0,-1,0};

    check("example 1",build(e1,(int)(sizeof(e1)/sizeof(e1[0]))),1);
    check("example 2",build(e2,(int)(sizeof(e2)/sizeof(e2[0]))),2);
    check("example 3",build(e3,(int)(sizeof(e3)/sizeof(e3[0]))),1);
    check("full 7 nodes",build(full7,(int)(sizeof(full7)/sizeof(full7[0]))),2);
    check("chain 4 nodes",build(chain4,(int)(sizeof(chain4)/sizeof(chain4[0]))),2);

    return 0;
}
