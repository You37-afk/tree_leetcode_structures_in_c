# LC894 · 所有可能的满二叉树

对应笔记：**三、树的结构与性质 → 2. 特殊二叉树**

## 验证什么

`build(n)` 的契约：

- 每棵树恰好有 `n` 个节点，且每个节点有 0 或 2 个孩子；
- 所有 `n` 节点的满二叉树都在结果里；
- 没有重复。

递推与建树共用同一条式子：

```text
c(1) = 1
c(n) = Σ c(i) · c(n-1-i),  i = 1, 3, ..., n-2
```

`countFBT` 只数不建，用来给结果数组一次 `malloc` 恰好大小，同时充当建树版的自检对照。

## 编译运行

```bash
gcc -O2 -Wall -Wextra -o lc894_demo lc894_demo.c
./lc894_demo
```

## 预期输出（节选）

```text
== n=1 ==  1 tree(s) (expect 1) COUNT OK
== n=5 ==  2 tree(s) (expect 2) COUNT OK
== n=7 ==  5 tree(s) (expect 5) COUNT OK
== n=9 ==  14 tree(s) (expect 14) COUNT OK
== n=19 (max odd) ==  count=4862 (expect 4862) ALL TREES OK
```

程序会逐个检查每棵树的节点数，并打印 n=1..9 的全部树形。

## 说明

偶数 `n` 直接返回空清单，因为满二叉树的节点数一定是奇数。
