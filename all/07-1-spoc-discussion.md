# 同步互斥(lec 17) spoc 思考题


- 有"spoc"标记的题是要求拿清华学分的同学要在实体课上完成，并按时提交到学生对应的ucore_code和os_exercises的git repo上。

## 个人思考题

### 17.1 背景
1. 请给出程序正确性的定义或解释。

 > 我们只能在确定的限制条件下来讨论“正确性”的定义或解释，没有无条件的“正确”。

2. 在一个新运行环境中程序行为与原来的预期不一致，是错误吗？
3. 程序并发执行有什么好处和障碍？
4. 什么是原子操作？

### 17.2 现实生活中的同步问题

1. 家庭采购中的同步问题的5种解决方案的核心思路是什么？举例描述可能的漏洞。

 > 方案一：先检查，后加相同的标签

 > 方案二：先加相同的标签，后检查

 > 方案三：先加不同的标签，后检查

 > 方案四：先加不同的标签，后进行非对称检查

 > 方案五：原子操作

2. 家庭采购中的同步问题与操作系统中进程同步有什么区别？
2. 如何通过枚举和分类方法检查同步算法的正确性？
3. 尝试描述方案四的正确性。
4. 互斥、死锁和饥饿的定义是什么？

### 17.3 临界区和禁用硬件中断同步方法

1. 什么是临界区？

 > 临界资源、互斥执行、代码

2. 临界区的访问规则是什么？

 > 空闲则入、忙则等待、有限等待、让权等待

3. 禁用中断是如何实现对临界区的访问控制的？有什么优缺点？

### 17.4 基于软件的同步方法

1. 软件同步方法中的5种解决方案（三种尝试方案、Peterson算法和Eisenberg算法）的核心思路是什么？举例描述可能的漏洞。

 > 方案一：turn 表示允许进入临界区的线程标识；

 > 方案二：flag[i] 表示线程i是否在临界区；
先判断，后修改变量；

 > 方案二：flag[i] 表示线程i想要进入临界区；
先修改变量，后判断；

 > Peterson算法：turn 表示进入临界区的线程标识，flag[i] 表示线程i想要进入临界区；
先修改变量，后判断；后修改者等待；
只适用于两个进程；

 > Eisenberg算法：flag[i] 表示线程i想要进入临界区，turn 表示进入临界区的线程标识（有多个想进入时）；
进入区：先修改flag，后判断是否有多个想进入；后修改者等待；
退出区：修改turn；
适用于多个进程；

2. 尝试通过枚举和分类方法检查Peterson算法的正确性。
3. 尝试准确描述Eisenberg同步算法，并通过枚举和分类方法检查其正确性。
4. (spoc)基于“python, ruby, C, C++，LISP、JavaScript”等语言模拟实现Eisenberg同步算法，并给出覆盖所有枚举分类的测试用例，在实现报告写出设计思路和测试结果分析。

### 17.5 高级抽象的同步方法

1. 如何证明TS指令和交换指令的等价性？
2. 自旋锁（spinlock）和无忙等待锁是如何实现同步的？它们有什么不同？
3. 为什么硬件原子操作指令能简化同步算法的实现？
 
## 小组思考题

1. （spoc）阅读[简化x86计算机模拟器的使用说明](https://github.com/chyyuu/ucore_lab/blob/master/related_info/lab7/lab7-spoc-exercise.md)，理解基于简化x86计算机的汇编代码。

2. （spoc)了解race condition. 进入[race-condition代码目录](https://github.com/chyyuu/ucore_lab/tree/master/related_info/lab7/race-condition)。

 - 执行 `./x86.py -p loop.s -t 1 -i 100 -R dx`， 请问`dx`的值是什么？
 - 执行 `./x86.py -p loop.s -t 2 -i 100 -a dx=3,dx=3 -R dx` ， 请问`dx`的值是什么？
 - 执行 `./x86.py -p loop.s -t 2 -i 3 -r -a dx=3,dx=3 -R dx`， 请问`dx`的值是什么？
 - 变量x的内存地址为2000, `./x86.py -p looping-race-nolock.s -t 1 -M 2000`, 请问变量x的值是什么？
 - 变量x的内存地址为2000, `./x86.py -p looping-race-nolock.s -t 2 -a bx=3 -M 2000`, 请问变量x的值是什么？为何每个线程要循环3次？
 - 变量x的内存地址为2000, `./x86.py -p looping-race-nolock.s -t 2 -M 2000 -i 4 -r -s 0`， 请问变量x的值是什么？
 - 变量x的内存地址为2000, `./x86.py -p looping-race-nolock.s -t 2 -M 2000 -i 4 -r -s 1`， 请问变量x的值是什么？
 - 变量x的内存地址为2000, `./x86.py -p looping-race-nolock.s -t 2 -M 2000 -i 4 -r -s 2`， 请问变量x的值是什么？ 
 - 变量x的内存地址为2000, `./x86.py -p looping-race-nolock.s -a bx=1 -t 2 -M 2000 -i 1`， 请问变量x的值是什么？ 

3. （spoc） 了解software-based lock, hardware-based lock, [software-hardware-lock代码目录](https://github.com/chyyuu/ucore_lab/tree/master/related_info/lab7/software-hardware-locks)

  - 理解flag.s,peterson.s,test-and-set.s,ticket.s,test-and-test-and-set.s 请通过x86.py分析这些代码是否实现了锁机制？请给出你的实验过程和结论说明。能否设计新的硬件原子操作指令Compare-And-Swap,Fetch-And-Add？
```
Compare-And-Swap

int CompareAndSwap(int *ptr, int expected, int new) {
  int actual = *ptr;
  if (actual == expected)
    *ptr = new;
  return actual;
}
```

```
Fetch-And-Add

int FetchAndAdd(int *ptr) {
  int old = *ptr;
  *ptr = old + 1;
  return old;
}
```
