# 2015 操作系统期中考试参考答案

WARNING: 仅供参考！

[试题链接](http://166.111.68.197:11123/oscourse/OS2015/MidExam?action=AttachFile&do=view&target=20150412-期中考试试题v3b.pdf)

### 第一题

4次。由于可以不对齐访问，那么在访问某个数据时，就可能该数据横跨两个页表的情况，即最多缺失2次。而指令和数据访问都有可能缺失，故一共最多缺失4次。

### 第二题

(1) 0xffff0 (cs * 16 + eip)

(2) 0xfe05b (同上)

(3.1) 0x8

(3.2) 0x10

(3.3)~(3.6) 0x7c6d

(4.1)~(4.2) 0x10002a

(5.1)~(5.4), (5.6) 0x100000

(5.5) 0xc0100000


### 第三题

(1)

- Clock：把页组织成环形链表，从指针当前位置遍历，当某页未被访问（访问位为0）时，置换该页；否则，置该页访问位为0，继续查找。
- FIFO：把页组织成队列，替换掉最先进入队列的页（对首），新的页加在队尾。

(2) 以 Clock 为例分析

3个物理页帧时

| 页帧 | a | b | c | d | a | b | e | a | b | c | d | e |
|:---:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|
| 1   | a |   |   | d |   |   | e |   |   |   |   |   |
| 2   |   | b |   |   | a |   |   |   |   | c |   |   |
| 3   |   |   | c |   |   | b |   |   |   |   | d |   |
| 指针 | 1 | 1 | 1 | 2 | 3 | 1 | 2 |   |   | 3 | 1 |   |
| 缺页 | x | x | x | x | x | x | x |   |   | x | x |   |

缺页9次。

4个物理页帧时

| 页帧 | a | b | c | d | a | b | e | a | b | c | d | e |
|:---:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|
| 1   | a |   |   |   |   |   | e |   |   |   | d |   |
| 2   |   | b |   |   |   |   |   | a |   |   |   | e |
| 3   |   |   | c |   |   |   |   |   | b |   |   |   |
| 4   |   |   |   | d |   |   |   |   |   | c |   |   |
| 指针 | 1 | 1 | 1 | 1 |   |   | 2 | 3 | 4 | 1 | 1 | 2 |
| 缺页 | x | x | x | x |   |   | x | x | x | x | x | x |

缺页10次。

(3) Belady现象：分配的物理页面数增加，缺页次数反而升高的异常现象。FIFO和Clock都存在。例子请参见[Lecture 9.4](http://166.111.68.197:11123/oscourse/OS2015autumn/lecture09?action=AttachFile&do=get&target=20151117-lecture-09-4.mp4)（即(2)中序列）。

[Piazza](https://piazza.com/class/i5j09fnsl7k5x0?cid=762) 上刚好有个关于 Belady 现象的讨论。

### 第四题

(1) 切换前的进程`prev`

(2) 在刚进入`switch_to`时，`prev`的上下文起始地址为`esp + 4`，`next`的上下文起始地址为`esp + 8`。

(3) `lcr3(next->cr3);`

(4) `load_esp0(next->kstack + KSTACKSIZE);`

### 第五题

(1) 一级页表保存二级页表的入口地址，二级页表保存映射的物理页号，虚拟地址中虚页号分为一级页表索引和二级页表索引。

(2) 页大小32B：5位。一个页表大小32B，一个页表项1B，故一级页表索引和二级页表索引各5位。

(2.1)

```c
int pde_index = (vaddr >> 10);
```

(2.2)

```c
int pte_index = (vaddr >> 5) & 0x1F;
```

(2.3)

```c
int pde = page[pdbr + pde_index];
if (pde >> 7 == 0) // throw page fault
int pte_start = (pde & 0x7F) >> 5;
int pte = page[pte_start + pte_index];
int paddr = (pte << 5) + (vaddr & 0x1F);
```

(2.4) 用 Piazza 练习上的程序即可跑出结果。可以参考[我们组的程序](https://github.com/paulzfm/os_course_spoc_exercises/blob/master/MV/mv.py)。

### 第六题

```
Parent Process: Global variable: 21 Stack variable: 19
Child Process:  Global variable: 22 Stack variable: 18
```

程序见：[2015_6.c](https://github.com/paulzfm/os_course_spoc_exercises/blob/master/midterm-answer/2015_6.c)
