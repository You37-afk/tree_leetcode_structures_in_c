# LC103 · 二叉树的锯齿形层序遍历对拍

对应笔记：**一、树的遍历 → 4.1 二叉树的锯齿形层序遍历**

## 验证什么

把三种实现放在同一批随机树上对拍：

1. **原版（队列 + 栈交替）**：最常见的错误写法；
2. **双栈版**：标准解法；
3. **参照版**：普通 BFS 逐层收集，奇数层整行反转。逻辑直白，作为正确性基准。

随机树按 LeetCode 层序数组生成：数组长度 1~55，除根外每个位置以 15%~90% 的概率为 `null`，只有父节点存在的位置才可达。

## 编译运行

```bash
gcc -O2 -Wall -Wextra -o lc103_fuzz lc103_fuzz.c
./lc103_fuzz            # 默认 seed=28, trials=3000
./lc103_fuzz 20260905 3000   # 也可以指定种子和轮数
```

Windows 下生成 `lc103_fuzz.exe`，运行 `./lc103_fuzz.exe`。

## 预期输出（seed=28）

```text
== random differential test ==
seed=28  trees=3000  (level-order length 1..55, null p=15%..90%)
wrong version mismatches : 776 / 3000 (25.87%)
fixed version mismatches : 0 / 3000

== stress cases ==
right-skewed chain, 1500 levels : PASS
full binary tree, 12 levels (4095 nodes) : PASS
```

这就是笔记表 3 中 `776 棵 ≈ 26%` 与 `0 棵` 的来源。

## 说明

- 使用固定种子的 xorshift32，而不是 `rand()`，所以不同编译器/平台上结果一致，可以直接复现。
- 种子 28 复现笔记里记录的 776/3000。换种子后具体数字会有小幅浮动（错误率约 26%），但“原版大量出错、双栈版 0 错”的结论不变。
- 程序会打印前 3 个最小反例的树（层序数组）与两种输出，方便直接定位问题。
