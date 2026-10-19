# LC543 · 二叉树的直径

对应笔记：**三、树的结构与性质 → 6. 树的直径、最大路径和**

## 验证什么

后序 DFS 返回“从当前节点往下走到某个后代的最长边数”，同时用

```text
through = L + R + 2
```

更新全局最大值。重点是：**直径不一定经过根节点**，所以答案必须在每个节点都尝试更新，而不能只在根节点算一次。

程序会按后序顺序打印每个节点的 `L`、`R`、`through` 和当前 `best`。

## 编译运行

```bash
gcc -O2 -Wall -Wextra -o lc543_demo lc543_demo.c
./lc543_demo
```

## 预期输出（节选）

```text
例2：1(2(3(5),4(,6)),)，直径 5-3-2-4-6
  node 5: L=-1 R=-1  through=0  best=0
  node 3: L= 0 R=-1  through=1  best=1
  node 6: L=-1 R=-1  through=0  best=1
  node 4: L=-1 R= 0  through=1  best=1
  node 2: L= 1 R= 1  through=4  best=4
  node 1: L= 2 R=-1  through=3  best=4
diameter = 4
```

另外还有例 1（直径经过根，答案 3）和例 3（单节点，答案 0）。
