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

2. （spoc)了解race condition. 进入[race-condition代码目录](https://github.com/chyyuu/ucore_lab/tree/master/related_info/lab7/race-condition)。

- 执行 `./x86.py -p loop.s -t 1 -i 100 -R dx`， 请问`dx`的值是什么？
 - （对于所有执行loop.s的命令而言，都是将edx寄存器赋上初值，并且执行loop.s中的汇编命令，也即edx中的值自减1直至小于0，也即等于-1，所以dx的值，也即edx寄存器的值，变化如下。）dx初值为0，经过sub后变为-1，然后程序退出。
 - 执行 `./x86.py -p loop.s -t 2 -i 100 -a dx=3,dx=3 -R dx` ， 请问`dx`的值是什么？
 - dx初值均为3，存在两个线程，并且时间片大小为100，所以dx变化过程为对两个线程而言是独立的，也即分别由3开始，自减直至为-1然后线程结束。
 - 执行 `./x86.py -p loop.s -t 2 -i 3 -r -a dx=3,dx=3 -R dx`， 请问`dx`的值是什么？
 - 和上一条对比，差异体现在时间片大小为3，所以两个线程先后地交替地分别占用CPU，对于整个运行过程而言，dx的值的变化是不规律的，每次线程切换的时候，由于上下文被保存，所以线程的dx值的变化是分立的，对于两个线程各自而言dx值的变化过程都是从3开始自减直到为-1
 - 变量x的内存地址为2000, `./x86.py -p looping-race-nolock.s -t 1 -M 2000`, 请问变量x的值是什么？
 - （对于looping-race-nolock.s而言，内存地址为2000对应的变量会被初始化为0，然后执行自增操作，直到寄存器ebx内的值自减后小于0，即等于-1.）由于x初始化为0，自增后x为1，ebx寄存器内的值为0，自减后为-1退出线程，所以x值为1
 - 变量x的内存地址为2000, `./x86.py -p looping-race-nolock.s -t 2 -a bx=3 -M 2000`, 请问变量x的值是什么？为何每个线程要循环3次？
 - 由于x是保存在内存中，而非寄存器中，每次读取，自增然后保存。所以x的值会在线程1的三次循环中自增到3，然后在线程2的三次循环中自增到6.线程循环3次是因为循环次数由ebx中的值决定，而寄存器值对于线程而言是分立的。
 - 变量x的内存地址为2000, `./x86.py -p looping-race-nolock.s -t 2 -M 2000 -i 4 -r -s 0`， 请问变量x的值是什么？
 - 与上一题不同的是，这一题里面时间片的大小被限制了，所以线程之间会来回切换，由于引入了随机，所以触发时间中断的位置不同，所以x的值可能为1，也可能为2
 - 变量x的内存地址为2000, `./x86.py -p looping-race-nolock.s -t 2 -M 2000 -i 4 -r -s 1`， 请问变量x的值是什么？
 - 同上
 - 变量x的内存地址为2000, `./x86.py -p looping-race-nolock.s -t 2 -M 2000 -i 4 -r -s 2`， 请问变量x的值是什么？ 
 - 同上
 - 变量x的内存地址为2000, `./x86.py -p looping-race-nolock.s -a bx=1 -t 2 -M 2000 -i 1`， 请问变量x的值是什么？ 
 - 由于两个线程分别严格地每次切换后只执行1条指令，然后切换，所以寄存器eax在两个线程中分别自增，但是两个寄存器取得的x值都是0，所以两个eax结果都是1，赋值后x结果为1.

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

> flag.s

试图通过flag实现锁机制，但因为flag的赋值操作并不是原子操作，如果两个线程同时设置flag将会导致同时进入临界区或者无限等待。

> peterson.s

算法正确，利用flag和turn同时进行判断，如果turn被同时赋值，则最终只会保留一个值。线程仍能正确运行。

> test-and-set.s

错误，在进入临界区前不能对mutex进行设置，否则会造成混乱。

> test-and-test-and-set.s

正确，当mutex为0时才可以进入临界区，因为xchg为原子操作，不会同时有多个线程在进入临界区时对mutex赋值。

- CompareAndSwap()以及FetchAndAdd原子操作实现见gitlab上对应位置的代码

- [GitLab地址](http://south.cs.tsinghua.edu.cn/BrieflyX/ucore_lab/blob/master/related_info/lab7/software-hardware-locks/x86.py)
