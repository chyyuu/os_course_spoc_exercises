# 同步互斥(lec 17) spoc 思考题


- 有"spoc"标记的题是要求拿清华学分的同学要在实体课上完成，并按时提交到学生对应的ucore_code和os_exercises的git repo上。

## 个人思考题

### 背景
 - 请给出程序正确性的定义或解释。
 - 在一个新运行环境中程序行为与原来的预期不一致，是错误吗？
 - 程序并发执行有什么好处和障碍？
 - 什么是原子操作？

### 现实生活中的同步问题

 - 家庭采购中的同步问题与操作系统中进程同步有什么区别？
 - 如何通过枚举和分类方法检查同步算法的正确性？
 - 尝试描述方案四的正确性。
 - 互斥、死锁和饥饿的定义是什么？

### 临界区和禁用硬件中断同步方法

 - 什么是临界区？
 - 临界区的访问规则是什么？
 - 禁用中断是如何实现对临界区的访问控制的？有什么优缺点？

### 基于软件的同步方法

 - 尝试通过枚举和分类方法检查Peterson算法的正确性。
 - 尝试准确描述Eisenberg同步算法，并通过枚举和分类方法检查其正确性。

### 高级抽象的同步方法

 - 如何证明TS指令和交换指令的等价性？
 - 为什么硬件原子操作指令能简化同步算法的实现？
 
## 小组思考题

1. （spoc）阅读[简化x86计算机模拟器的使用说明](https://github.com/chyyuu/ucore_lab/blob/master/related_info/lab7/lab7-spoc-exercise.md)，理解基于简化x86计算机的汇编代码。

> 已阅读

2. （spoc)了解race condition. 进入[race-condition代码目录](https://github.com/chyyuu/ucore_lab/tree/master/related_info/lab7/race-condition)。

 - 执行 `./x86.py -p loop.s -t 1 -i 100 -R dx`， 请问`dx`的值是什么？

> -1

 - 执行 `./x86.py -p loop.s -t 2 -i 100 -a dx=3,dx=3 -R dx` ， 请问`dx`的值是什么？

> 两个线程dx的初始值均为3，而切换中断为100，所以先运行线程1，结束后运行线程2。两个线程中的dx都减为－1时退出。

 - 执行 `./x86.py -p loop.s -t 2 -i 3 -r -a dx=3,dx=3 -R dx`， 请问`dx`的值是什么？

> 两个线程dx的初始值均为3，而切换中断为3，但是两个线程有各自的dx值，不会互相影响，所以两个线程中的dx都为－1。

 - 变量x的内存地址为2000, `./x86.py -p looping-race-nolock.s -t 1 -M 2000`, 请问变量x的值是什么？

> 1

 - 变量x的内存地址为2000, `./x86.py -p looping-race-nolock.s -t 2 -a bx=3 -M 2000`, 请问变量x的值是什么？为何每个线程要循环3次？

> 6。每个线程循环3次，每次将内存加一。因为在线程1运行之前切换时栈会保存寄存器的值，在切换之后栈会对寄存器的值进行恢复，所以第二个线程中bx的值也为3。

 - 变量x的内存地址为2000, `./x86.py -p looping-race-nolock.s -t 2 -M 2000 -i 4 -r -s 0`， 请问变量x的值是什么？

> x的值为1或2。这里会产生冲突，因为线程的切换时间随机，所以当两个线程互斥地访问时会得到结果2，如果冲突，那么得到结果1。

 - 变量x的内存地址为2000, `./x86.py -p looping-race-nolock.s -t 2 -M 2000 -i 4 -r -s 1`， 请问变量x的值是什么？

> 1或2

 - 变量x的内存地址为2000, `./x86.py -p looping-race-nolock.s -t 2 -M 2000 -i 4 -r -s 2`， 请问变量x的值是什么？ 

> 1或2

 - 变量x的内存地址为2000, `./x86.py -p looping-race-nolock.s -a bx=1 -t 2 -M 2000 -i 1`， 请问变量x的值是什么？ 

> 1。因为两个线程的第一句汇编指令都讲内存中的值取出，值为0，增加之后再储存都为1。

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

```
flag.s未实现锁机制:
Thread 0 Thread 1
1000 mov flag, %ax
1001 test $0, %ax
------ Interrupt ------ ------ Interrupt ------
1000 mov flag, %ax
1001 test $0, %ax
------ Interrupt ------ ------ Interrupt ------
1002 jne .acquire
1003 mov $1, flag
------ Interrupt ------ ------ Interrupt ------
1002 jne .acquire
1003 mov $1, flag
这两个进程都有可能得到相同的flag值，即两个进程可能会同时进入临界区。
peterson.s未实现锁机制:
Thread 0 Thread 1
1000 lea flag, %fx
1001 mov %bx, %cx
------ Interrupt ------ ------ Interrupt ------
1000 lea flag, %fx
1001 mov %bx, %cx
------ Interrupt ------ ------ Interrupt ------
1002 neg %cx
1003 add $1, %cx
------ Interrupt ------ ------ Interrupt ------
1002 neg %cx
1003 add $1, %cx
------ Interrupt ------ ------ Interrupt ------
1004 mov $1, 0(%fx,%bx,4)
1005 mov %cx, turn
------ Interrupt ------ ------ Interrupt ------
1004 mov $1, 0(%fx,%bx,4)
1005 mov %cx, turn
------ Interrupt ------ ------ Interrupt ------
1006 mov 0(%fx,%cx,4), %ax
1007 test $1, %ax
------ Interrupt ------ ------ Interrupt ------
1006 mov 0(%fx,%cx,4), %ax
1007 test $1, %ax
------ Interrupt ------ ------ Interrupt ------
1008 jne .fini
1012 mov count, %ax
------ Interrupt ------ ------ Interrupt ------
1008 jne .fini
1012 mov count, %ax
两个进程有可能同时将自身的flag设成1然后就都不能进入临界区;
test-and-set.s实现了锁机制,因为不管在任何位置切换进程都能保证有且只有进程进入临街区
ticket.s实现了锁机制,因为不论在那个位置切换进程都会使得每个进程获取到进入临界区时其他进程都不能获取到进入临界区的锁;
test-and-test-set.s能够实现锁机制，采用反证法，如果两个线程都进入了临界区，那么两个线程的ax都为0，
那么执行原子操作的时候都是mutex为0，然后将mutex设置为1。然而这里出现了一个矛盾，
因为两个线程执行xchg的顺序有先后，所以当第一个线程运行完后，mutex中的值必然为1，
所以另一个线程不会得到0，则产生矛盾，假设错误。所以该程序实现了锁机制。
```
