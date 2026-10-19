# LC215 · 数组中的第 K 个最大元素

对应笔记：**三、树的结构与性质 → 3. 堆**

## 验证什么

小根堆维护“最大的 k 个数”俱乐部：

- 每个元素先入堆；
- 堆大小超过 `k` 就弹出堆顶（俱乐部里最小的）；
- 遍历结束后堆顶就是第 k 大。

程序包含 6 组手写用例（覆盖 `k=1`、`k=n`、负数、重复值），以及 2000 个随机数组、每个数组尝试所有 `k` 的对拍（本机共 50207 次 `(数组, k)` 校验）。

## 编译运行

```bash
gcc -O2 -Wall -Wextra -o lc215_demo lc215_demo.c
./lc215_demo
```

## 预期输出

```text
case 1: n=6  k=2   got 5    expect 5    OK
case 2: n=9  k=4   got 4    expect 4    OK
case 3: n=3  k=2   got -3   expect -3   OK
case 4: n=1  k=1   got 7    expect 7    OK
case 5: n=2  k=2   got 1    expect 1    OK
case 6: n=3  k=2   got 5    expect 5    OK
random cross-check vs sort: 50207 values, fails=0 -> all pass
```

不同 C 库的 `rand()` 序列可能让总校验次数略有差异，但 `fails=0` 应保持不变。后面还会打印 `nums=[3,2,1,5,6,4], k=2` 的逐帧堆变化。
