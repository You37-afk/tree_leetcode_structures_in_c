/* LC103: wrong(queue+stack) vs fixed(double stack) vs reference(BFS).
   gcc -O2 -Wall -Wextra -o lc103_fuzz lc103_fuzz.c
   default seed=28 reproduces 776/3000. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct TreeNode
{
    int val;
    struct TreeNode* left;
    struct TreeNode* right;
};

static struct TreeNode* new_node(int v)
{
    struct TreeNode* p=(struct TreeNode*)malloc(sizeof(*p));
    if(!p)
    {
        fprintf(stderr,"malloc failed\n");
        exit(1);
    }
    p->val=v;
    p->left=p->right=NULL;
    return p;
}

static void free_tree(struct TreeNode* u)
{
    if(!u) return;
    free_tree(u->left);
    free_tree(u->right);
    free(u);
}

static int count_nodes(const struct TreeNode* u)
{
    if(!u) return 0;
    return 1+count_nodes(u->left)+count_nodes(u->right);
}

static unsigned rng_state;

static unsigned rnd(void)
{
    unsigned x=rng_state;
    x^=x<<13;
    x^=x>>17;
    x^=x<<5;
    rng_state=x;
    return x;
}

static struct TreeNode* gen_tree(int len,int p_null)
{
    int* present=(int*)malloc((size_t)len*sizeof(*present));
    struct TreeNode** nodes=(struct TreeNode**)malloc((size_t)len*sizeof(*nodes));
    if(!present || !nodes) exit(1);

    present[0]=1;
    for(int i=1;i<len;i++)
        present[i]=((int)(rnd()%100)>=p_null);

    int next_val=0;
    for(int i=0;i<len;i++)
    {
        if(!present[i])
        {
            nodes[i]=NULL;
            continue;
        }
        if(i>0 && nodes[(i-1)/2]==NULL)
        {
            nodes[i]=NULL;
            present[i]=0;
            continue;
        }
        nodes[i]=new_node(++next_val);
    }
    for(int i=0;i<len;i++)
    {
        if(!nodes[i]) continue;
        int l=2*i+1,r=2*i+2;
        if(l<len) nodes[i]->left=nodes[l];
        if(r<len) nodes[i]->right=nodes[r];
    }

    struct TreeNode* root=nodes[0];
    free(nodes);
    free(present);
    return root;
}

typedef struct
{
    int** ans;
    int* cols;
    int size;
} Result;

static void free_result(Result* r)
{
    for(int i=0;i<r->size;i++) free(r->ans[i]);
    free(r->ans);
    free(r->cols);
    r->ans=NULL;
    r->cols=NULL;
    r->size=0;
}

static Result run_wrong(struct TreeNode* root)
{
    Result r={0};
    if(!root) return r;

    int cap=count_nodes(root)+2;
    struct TreeNode** Queue=(struct TreeNode**)malloc((size_t)cap*sizeof(*Queue));
    struct TreeNode** Stack=(struct TreeNode**)malloc((size_t)cap*sizeof(*Stack));
    int front=0,rear=0,top=-1,level=0;

    r.ans=(int**)malloc((size_t)cap*sizeof(*r.ans));
    r.cols=(int*)malloc((size_t)cap*sizeof(*r.cols));
    Queue[rear++]=root;

    while(1)
    {
        if(level%2==0)
        {
            if(front>=rear) break;
            int level_size=rear-front;
            if(level_size<0 || level_size>cap)
            {
                fprintf(stderr,"queue overflow\n");
                exit(2);
            }
            int* vals=(int*)malloc((size_t)level_size*sizeof(*vals));
            r.cols[r.size]=level_size;
            top=-1;
            for(int i=0;i<level_size;i++)
            {
                struct TreeNode* node=Queue[front++];
                vals[i]=node->val;
                if(node->left) Stack[++top]=node->left;
                if(node->right) Stack[++top]=node->right;
            }
            r.ans[r.size++]=vals;
        }
        else
        {
            if(top==-1) break;
            int level_size=top+1;
            if(level_size<0 || level_size>cap)
            {
                fprintf(stderr,"stack overflow\n");
                exit(2);
            }
            int* vals=(int*)malloc((size_t)level_size*sizeof(*vals));
            r.cols[r.size]=level_size;
            front=rear=0;
            for(int j=0;j<level_size;j++)
            {
                struct TreeNode* node=Stack[top--];
                vals[j]=node->val;
                if(node->left) Queue[rear++]=node->left;
                if(node->right) Queue[rear++]=node->right;
            }
            r.ans[r.size++]=vals;
        }
        level++;
    }
    free(Queue);
    free(Stack);
    return r;
}

static Result run_fixed(struct TreeNode* root)
{
    Result r={0};
    if(!root) return r;

    int cap=count_nodes(root)+2;
    struct TreeNode** Queue=(struct TreeNode**)malloc((size_t)cap*sizeof(*Queue));
    struct TreeNode** Stack=(struct TreeNode**)malloc((size_t)cap*sizeof(*Stack));
    int rear=0,top=-1,level=0;

    r.ans=(int**)malloc((size_t)cap*sizeof(*r.ans));
    r.cols=(int*)malloc((size_t)cap*sizeof(*r.cols));
    Queue[rear++]=root;

    while(1)
    {
        if(level%2==0)
        {
            if(rear<=0) break;
            int level_size=rear;
            int* vals=(int*)malloc((size_t)level_size*sizeof(*vals));
            r.cols[r.size]=level_size;
            top=-1;
            for(int i=0;i<level_size;i++)
            {
                struct TreeNode* node=Queue[--rear];
                vals[i]=node->val;
                if(node->left) Stack[++top]=node->left;
                if(node->right) Stack[++top]=node->right;
            }
            r.ans[r.size++]=vals;
        }
        else
        {
            if(top==-1) break;
            int level_size=top+1;
            if(level_size<0 || level_size>cap)
            {
                fprintf(stderr,"stack overflow\n");
                exit(2);
            }
            int* vals=(int*)malloc((size_t)level_size*sizeof(*vals));
            r.cols[r.size]=level_size;
            rear=0;
            for(int j=0;j<level_size;j++)
            {
                struct TreeNode* node=Stack[top--];
                vals[j]=node->val;
                if(node->right) Queue[rear++]=node->right;
                if(node->left) Queue[rear++]=node->left;
            }
            r.ans[r.size++]=vals;
        }
        level++;
    }
    free(Queue);
    free(Stack);
    return r;
}

static Result run_ref(struct TreeNode* root)
{
    Result r={0};
    if(!root) return r;

    int cap=count_nodes(root)+2;
    struct TreeNode** q=(struct TreeNode**)malloc((size_t)cap*sizeof(*q));
    int head=0,tail=0;

    r.ans=(int**)malloc((size_t)cap*sizeof(*r.ans));
    r.cols=(int*)malloc((size_t)cap*sizeof(*r.cols));
    q[tail++]=root;

    while(head<tail)
    {
        int level_size=tail-head;
        int* vals=(int*)malloc((size_t)level_size*sizeof(*vals));
        for(int i=0;i<level_size;i++)
        {
            struct TreeNode* node=q[head++];
            vals[i]=node->val;
            if(node->left) q[tail++]=node->left;
            if(node->right) q[tail++]=node->right;
        }
        if(r.size%2==1)
        {
            for(int i=0,j=level_size-1;i<j;i++,j--)
            {
                int t=vals[i];
                vals[i]=vals[j];
                vals[j]=t;
            }
        }
        r.cols[r.size]=level_size;
        r.ans[r.size++]=vals;
    }
    free(q);
    return r;
}

static int result_equal(const Result* a,const Result* b)
{
    if(a->size!=b->size) return 0;
    for(int i=0;i<a->size;i++)
    {
        if(a->cols[i]!=b->cols[i]) return 0;
        if(memcmp(a->ans[i],b->ans[i],(size_t)a->cols[i]*sizeof(int))!=0) return 0;
    }
    return 1;
}

static void print_result(const Result* r)
{
    printf("[");
    for(int i=0;i<r->size;i++)
    {
        printf("[");
        for(int j=0;j<r->cols[i];j++)
            printf("%d%s",r->ans[i][j],j+1<r->cols[i] ? "," : "");
        printf("]%s",i+1<r->size ? "," : "");
    }
    printf("]");
}

static void print_tree(const struct TreeNode* root)
{
    int cap=count_nodes(root)*4+8;
    const struct TreeNode** q=(const struct TreeNode**)malloc((size_t)cap*sizeof(*q));
    int* out=(int*)malloc((size_t)cap*sizeof(*out));
    int head=0,tail=0,n=0;

    q[tail++]=root;
    while(head<tail)
    {
        const struct TreeNode* u=q[head++];
        if(!u)
        {
            out[n++]=-1;
            continue;
        }
        out[n++]=u->val;
        q[tail++]=u->left;
        q[tail++]=u->right;
    }
    while(n>0 && out[n-1]==-1) n--;

    printf("[");
    for(int i=0;i<n;i++)
        printf(i ? ",%d" : "%d",out[i]);
    printf("]");
    free(q);
    free(out);
}

static struct TreeNode* build_chain(int n)
{
    struct TreeNode* root=new_node(1);
    struct TreeNode* cur=root;
    for(int i=2;i<=n;i++)
    {
        cur->right=new_node(i);
        cur=cur->right;
    }
    return root;
}

static struct TreeNode* build_full(int levels)
{
    int n=(1<<levels)-1;
    struct TreeNode** nodes=(struct TreeNode**)malloc((size_t)n*sizeof(*nodes));
    for(int i=0;i<n;i++) nodes[i]=new_node(i+1);
    for(int i=0;i<n;i++)
    {
        int l=2*i+1,r=2*i+2;
        if(l<n) nodes[i]->left=nodes[l];
        if(r<n) nodes[i]->right=nodes[r];
    }
    struct TreeNode* root=nodes[0];
    free(nodes);
    return root;
}

int main(int argc,char** argv)
{
    unsigned seed=argc>1 ? (unsigned)strtoul(argv[1],NULL,10) : 28u;
    int trials=argc>2 ? atoi(argv[2]) : 3000;
    int wrong_bad=0,fixed_bad=0,shown=0;

    if(seed==0) seed=1;
    rng_state=seed;

    for(int t=0;t<trials;t++)
    {
        int len=1+(int)(rnd()%55);
        int p_null=15+(int)(rnd()%76);
        struct TreeNode* root=gen_tree(len,p_null);
        Result ref=run_ref(root);
        Result fix=run_fixed(root);
        Result bad=run_wrong(root);

        if(!result_equal(&bad,&ref))
        {
            wrong_bad++;
            if(shown<3)
            {
                shown++;
                printf("mismatch #%d  tree=",shown);
                print_tree(root);
                printf("\n  wrong=");
                print_result(&bad);
                printf("\n  ref  =");
                print_result(&ref);
                printf("\n");
            }
        }
        if(!result_equal(&fix,&ref)) fixed_bad++;

        free_result(&ref);
        free_result(&fix);
        free_result(&bad);
        free_tree(root);
    }

    printf("\n== random differential test ==\n");
    printf("seed=%u  trees=%d  (level-order length 1..55, null p=15%%..90%%)\n",seed,trials);
    printf("wrong version mismatches : %d / %d (%.2f%%)\n",wrong_bad,trials,
           trials ? 100.0*wrong_bad/trials : 0.0);
    printf("fixed version mismatches : %d / %d\n",fixed_bad,trials);

    printf("\n== stress cases ==\n");
    struct TreeNode* chain=build_chain(1500);
    Result c_ref=run_ref(chain),c_fix=run_fixed(chain);
    printf("right-skewed chain, 1500 levels : %s\n",result_equal(&c_fix,&c_ref) ? "PASS" : "FAIL");
    free_result(&c_ref);
    free_result(&c_fix);
    free_tree(chain);

    struct TreeNode* full=build_full(12);
    Result f_ref=run_ref(full),f_fix=run_fixed(full);
    printf("full binary tree, 12 levels (%d nodes) : %s\n",(1<<12)-1,
           result_equal(&f_fix,&f_ref) ? "PASS" : "FAIL");
    free_result(&f_ref);
    free_result(&f_fix);
    free_tree(full);

    return 0;
}
