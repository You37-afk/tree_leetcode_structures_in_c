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
    if(p==NULL)
    {
        fprintf(stderr,"malloc failed\n");
        exit(1);
    }
    p->val=v;
    p->left=l;
    p->right=r;
    return p;
}

static void free_tree(struct TreeNode* u)
{
    if(u==NULL) return;
    free_tree(u->left);
    free_tree(u->right);
    free(u);
}

int height(struct TreeNode* u,int* best)
{
    if(u==NULL) return -1;
    int lh=height(u->left,best);
    int rh=height(u->right,best);
    int through=lh+rh+2;
    if(through>*best) *best=through;
    printf("  node %d: L=%2d R=%2d  through=%d  best=%d\n",u->val,lh,rh,through,*best);
    return (lh>rh ? lh : rh)+1;
}

int diameterOfBinaryTree(struct TreeNode* root)
{
    int best=0;
    height(root,&best);
    return best;
}

int main(void)
{
    struct TreeNode* t1=
        new_node(1,
                 new_node(2,new_node(4,NULL,NULL),new_node(5,NULL,NULL)),
                 new_node(3,NULL,NULL));
    printf("例1：1(2(4,5),3)\n");
    printf("diameter = %d\n\n",diameterOfBinaryTree(t1));
    free_tree(t1);

    struct TreeNode* t2=
        new_node(1,
                 new_node(2,
                          new_node(3,new_node(5,NULL,NULL),NULL),
                          new_node(4,NULL,new_node(6,NULL,NULL))),
                 NULL);
    printf("例2：1(2(3(5),4(,6)),)，直径 5-3-2-4-6\n");
    printf("diameter = %d\n\n",diameterOfBinaryTree(t2));
    free_tree(t2);

    struct TreeNode* t3=new_node(7,NULL,NULL);
    printf("例3：单节点 7\n");
    printf("diameter = %d\n",diameterOfBinaryTree(t3));
    free_tree(t3);

    return 0;
}
