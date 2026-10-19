# LC337 · 打家劫舍 III

对应笔记：**四、树上的动态规划 → 3. 打家劫舍问题**

## 验证什么

后序遍历返回一对状态：

- `take`：抢当前节点时，子树最多能拿多少；
- `skip`：不抢当前节点时，子树最多能拿多少。

父节点只需要这两个数，不需要知道更下层怎么选。

## 编译运行

```bash
gcc -O2 -Wall -Wextra -o lc337_demo lc337_demo.c
./lc337_demo
```

## 预期输出

```text
example 1        got=7 expect=7 OK
example 2        got=9 expect=9 OK
example 3        got=1 expect=1 OK
small root       got=5 expect=5 OK
```
