/* 第一个 flipEquiv 是故意保留的错误写法：root1==root2==NULL 不表示两者都为空。
   第二个是正解。编译 warning 是预期现象。 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

struct TreeNode
{
    int val;
    struct TreeNode* left;
    struct TreeNode* right;
};

struct TreeNode* invertTree(struct TreeNode* root)
{
    if(root==NULL) return NULL;
    struct TreeNode* temp=root->left;
    root->left=root->right;
    root->right=temp;
    invertTree(root->left);
    invertTree(root->right);
    return root;
}

bool isSameTree(struct TreeNode* p,struct TreeNode* q)
{
    if(p==NULL&&q==NULL) return true;
    else if(p==NULL || q==NULL) return false;
    else if(p->val!=q->val) return false;
    else return isSameTree(p->left,q->left) && isSameTree(p->right,q->right);
}

bool flipEquiv(struct TreeNode* root1,struct TreeNode* root2)
{
    if(root1==root2==NULL) return true;
    if(root1==NULL || root2==NULL) return false;
    if(root1==root2)
    {
        while(1)
        {
            if(isSameTree(root1->left,root2->left) && isSameTree(root1->right,root2->right)) return true;
            else
            {
                invertTree(root1->left);
                invertTree(root1->right);
                if(!(isSameTree(root1->left,root2->left) && isSameTree(root1->right,root2->right))) return false;
                break;
            }
        }
    }
    bool left=flipEquiv(root1->left,root2->left);
    bool right=flipEquiv(root1->right,root2->right);
    return left&&right;
}

bool flipEquiv_ok(struct TreeNode* root1,struct TreeNode* root2)
{
    if(root1==NULL && root2==NULL) return true;
    if(root1==NULL || root2==NULL) return false;
    if(root1->val!=root2->val) return false;
    return (flipEquiv_ok(root1->left,root2->left) && flipEquiv_ok(root1->right,root2->right))
        || (flipEquiv_ok(root1->left,root2->right) && flipEquiv_ok(root1->right,root2->left));
}

#define SENT (-99999)

static struct TreeNode* newNode(int v)
{
    struct TreeNode* p=malloc(sizeof(*p));
    p->val=v;
    p->left=p->right=NULL;
    return p;
}

static struct TreeNode* buildLevel(const int* v,int n)
{
    if(n==0 || v[0]==SENT) return NULL;
    struct TreeNode* root=newNode(v[0]);
    struct TreeNode** q=malloc((size_t)n*sizeof(*q));
    int head=0,tail=0,i=1;
    q[tail++]=root;
    while(head<tail && i<n)
    {
        struct TreeNode* cur=q[head++];
        if(i<n)
        {
            if(v[i]!=SENT)
            {
                cur->left=newNode(v[i]);
                q[tail++]=cur->left;
            }
            i++;
        }
        if(i<n)
        {
            if(v[i]!=SENT)
            {
                cur->right=newNode(v[i]);
                q[tail++]=cur->right;
            }
            i++;
        }
    }
    free(q);
    return root;
}

static void freeTree(struct TreeNode* r)
{
    if(!r) return;
    freeTree(r->left);
    freeTree(r->right);
    free(r);
}

static void printTree(struct TreeNode* r)
{
    if(!r)
    {
        printf("[空]");
        return;
    }
    struct TreeNode** q=malloc(256*sizeof(*q));
    int head=0,tail=0;
    q[tail++]=r;
    printf("[");
    int first=1;
    while(head<tail && tail<255)
    {
        struct TreeNode* cur=q[head++];
        printf("%s",first ? "" : ",");
        first=0;
        if(!cur)
        {
            printf("null");
            continue;
        }
        printf("%d",cur->val);
        q[tail++]=cur->left;
        q[tail++]=cur->right;
    }
    printf("]");
    free(q);
}

static int passed_wrong=0,passed_ok=0,total=0;

static void test(const char* name,const int* a,int na,const int* b,int nb,bool want)
{
    struct TreeNode* t1=buildLevel(a,na);
    struct TreeNode* t2=buildLevel(b,nb);
    bool go=flipEquiv_ok(t1,t2);
    bool gu=flipEquiv(t1,t2);
    total++;
    if(gu==want) passed_wrong++;
    if(go==want) passed_ok++;
    printf("%-22s 期望 %-5s | 原版 %-5s %-4s | 正解 %-5s %-4s\n",name,want ? "true" : "false",
           gu ? "true" : "false",gu==want ? "ok" : "错的",
           go ? "true" : "false",go==want ? "ok" : "错的");
    freeTree(t1);
    freeTree(t2);
}

int main(void)
{
    int a1[]={1,2,3,4,5,6,SENT,SENT,SENT,7,8};
    int b1[]={1,3,2,SENT,6,4,5,SENT,SENT,SENT,SENT,8,7};
    int e0[]={SENT};
    int e1[]={1};
    int e2[]={2};
    int e3[]={1,2};
    int e4[]={1,SENT,2};
    int e5[]={1,3};
    int e6[]={1,2,3};
    int e7[]={1,5,3};
    int e8[]={1,2,SENT,3};
    int e9[]={1,2,SENT,SENT,3};

    struct TreeNode* t1=buildLevel(a1,11);
    struct TreeNode* t2=buildLevel(b1,13);
    printf("root1 = ");
    printTree(t1);
    printf("\nroot2 = ");
    printTree(t2);
    freeTree(t1);
    freeTree(t2);
    printf("\n\n");

    printf("%-22s %-12s | %s\n","用例","","");
    printf("---------------------------------------------------------------\n");
    test("① 例题1（经典翻转对）",a1,11,b1,13,true);
    test("② 两棵都是空树",e0,1,e0,1,true);
    test("③ 空树 vs 单节点",e0,1,e1,1,false);
    test("④ 单节点 1 vs 1",e1,1,e1,1,true);
    test("⑤ 单节点 1 vs 2",e1,1,e2,1,false);
    test("⑥ 两棵完全一样",e6,3,e6,3,true);
    test("⑦ 就翻一层 [1,2] vs [1,null,2]",e3,2,e4,3,true);
    test("⑧ 值不同 [1,2] vs [1,3]",e3,2,e5,2,false);
    test("⑨ 形状同值不同",e6,3,e7,3,false);
    test("⑩ 深处才翻 [1,2,null,3]",e8,4,e9,5,true);
    printf("---------------------------------------------------------------\n");
    printf("原版 %d/%d 通过    正解 %d/%d 通过\n\n",passed_wrong,total,passed_ok,total);
    return 0;
}
