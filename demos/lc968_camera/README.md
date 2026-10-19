# LC968 · 监控二叉树

对应笔记：**四、树上的动态规划 → 5. 监控二叉树 / 状态机 DP**

## 验证什么

每个节点向父亲只汇报一个状态：

| 状态 | 含义 | 对父亲的要求 |
|---|---|---|
| `NEED` | 自己没被覆盖，也没装摄像头 | 请父亲装一个 |
| `COVERED` | 自己已被覆盖，但自己没装 | 无 |
| `CAMERA` | 自己装了摄像头 | 无，还能覆盖父亲 |

转移优先级不能乱：先看孩子有没有 `NEED`，再看有没有 `CAMERA`，最后才是全 `COVERED`。

## 编译运行

```bash
gcc -O2 -Wall -Wextra -o lc968_demo lc968_demo.c
./lc968_demo
```

## 预期输出

```text
example 1          got=1 expect=1 OK
example 2          got=2 expect=2 OK
example 3          got=1 expect=1 OK
full 7 nodes       got=2 expect=2 OK
chain 4 nodes      got=2 expect=2 OK
```
