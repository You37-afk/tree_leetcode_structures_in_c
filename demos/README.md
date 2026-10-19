# Demo 索引

每个目录一个独立可编译的 C 程序，对应笔记中的一处结论。所有 demo 只用标准库。

| 目录 | 源文件 | 对应笔记 | 一句话说明 |
|---|---|---|---|
| `lc103_zigzag_fuzz/` | `lc103_fuzz.c` | 一、4.1 | 3000 棵随机树对拍：队列+栈交替 vs 双栈，复现 776/3000 |
| `lc215_kth_largest/` | `lc215_demo.c` | 三、3 堆 | 小根堆维护最大的 k 个数，50207 组随机对拍 |
| `lc337_house_robber/` | `lc337_demo.c` | 四、3 | 后序返回 `take/skip`，4 组用例 |
| `lc543_diameter/` | `lc543_demo.c` | 三、6 | 后序逐节点报告，验证直径不一定过根 |
| `lc894_fbt_count/` | `lc894_demo.c` | 三、2 | 满二叉树枚举，n=19 共 4862 棵 |
| `lc951_flip_equiv/` | `lc951_demo.c` | 三、4 | 故意保留错误写法，错误版 5/10 vs 正解 10/10 |
| `lc968_camera/` | `lc968_demo.c` | 四、5 | 三状态树形 DP，5 组用例 |

## 统一编译方式

```bash
cd <目录>
gcc -O2 -Wall -Wextra -o <程序名> <源文件>
./<程序名>
```

除了 `lc951_flip_equiv` 会故意产生两条指针比较 warning（README 已说明），其余 demo 在 `-Wall -Wextra` 下应无 warning。
