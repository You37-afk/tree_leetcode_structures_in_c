# LC951 · 翻转等价二叉树

对应笔记：**三、树的结构与性质 → 4. 树的对称与镜像**

## 验证什么

这个 demo **故意保留了一个错误写法**，用来展示 C 语言里非常隐蔽的一行：

```c
if(root1==root2==NULL) return true;
```

它并不是“两个都为空”，而是先算 `root1 == root2`（得到 0/1），再拿这个整数和 `NULL` 比较。空树、一边为空、值不同等用例会因此判错。

程序把错误版和四行正解放在同一组 10 个用例上对比：

- 错误版：5/10；
- 正解：10/10。

## 编译运行

```bash
gcc -O2 -Wall -Wextra -o lc951_demo lc951_demo.c
./lc951_demo
```

编译时会出现两条 warning（`comparison between pointer and integer` / `suggest parentheses`）——这是**预期现象**，正是这个 demo 要展示的陷阱。

## 预期输出（节选）

```text
② 两棵都是空树       期望 true  | 原版 false 错的 | 正解 true  ok
③ 空树 vs 单节点     期望 false | 原版 true  错的 | 正解 false ok
⑤ 单节点 1 vs 2      期望 false | 原版 true  错的 | 正解 true  ok
...
原版 5/10 通过    正解 10/10 通过
```

## 正解

```c
bool flipEquiv(struct TreeNode* a,struct TreeNode* b)
{
    if(!a && !b) return true;
    if(!a || !b) return false;
    if(a->val!=b->val) return false;
    return (flipEquiv(a->left,b->left) && flipEquiv(a->right,b->right))
        || (flipEquiv(a->left,b->right) && flipEquiv(a->right,b->left));
}
```
