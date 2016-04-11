# lab4 spoc 思考题

## 个人思考题

### 13.1 总体介绍

(1) ucore的线程控制块数据结构是什么？

> 线程控制块TCB，在ucore中是数据结构proc_struct
> 
> ```c
> struct proc_struct {
    enum proc_state state;                      // Process state
    int pid;                                    // Process ID
    int runs;                                   // the running times of Proces
    uintptr_t kstack;                           // Process kernel stack
    volatile bool need_resched;                 // bool value: need to be rescheduled to release CPU?
    struct proc_struct *parent;                 // the parent process
    struct mm_struct *mm;                       // Process's memory management field
    struct context context;                     // Switch here to run process
    struct trapframe *tf;                       // Trap frame for current interrupt
    uintptr_t cr3;                              // CR3 register: the base addr of Page Directroy Table(PDT)
    uint32_t flags;                             // Process flag
    char name[PROC_NAME_LEN + 1];               // Process name
    list_entry_t list_link;                     // Process link list 
    list_entry_t hash_link;                     // Process hash list
};
> ```

### 13.2 关键数据结构

(2) 如何知道ucore的两个线程同在一个进程？

> 比较CR3

(3) context和trapframe分别在什么时候用到？

> context在线程切换时用到；
> trapframe在中断、异常、系统调用时用到。

(4) 用户态或内核态下的中断处理有什么区别？在trapframe中有什么体现？

> 用户态中断处理硬件除了保存ErrorCode、EIP、CS、EFLAGS还要保存SS、ESP，

### 13.3 执行流程

(5) do_fork中的内核线程执行的第一条指令是什么？它是如何过渡到内核线程对应的函数的？

```
tf.tf_eip = (uint32_t) kernel_thread_entry;
/kern-ucore/arch/i386/init/entry.S
/kern/process/entry.S
```

> 第一条指令为“pushl %edx”。在kernel_thread函数中设置trapframe中的reg\_rbx为函数入口地址,reg\_edx为函数参数。在进程切换后进入entry.s执行：

```
    pushl %edx              # push arg
    call *%ebx              # call fn
```

(6)内核线程的堆栈初始化在哪？
```
tf和context中的esp
```
> 在do\_fork函数中调用setup\_kstack函数，进行内核堆栈的初始化

(7)fork()父子进程的返回值是不同的。这在源代码中的体现中哪？

> get\_pid函数，通过遍历链表找到唯一的pid返回给新的线程

(8)内核线程initproc的第一次执行流程是什么样的？能跟踪出来吗？

## 小组练习与思考题

(1)(spoc) 理解内核线程的生命周期。

> 需写练习报告和简单编码，完成后放到git server 对应的git repo中

### 掌握知识点
1. 内核线程的启动、运行、就绪、等待、退出
2. 内核线程的管理与简单调度
3. 内核线程的切换过程

### 练习用的[lab4 spoc exercise project source code](https://github.com/chyyuu/ucore_lab/tree/master/related_info/lab4/lab4-spoc-discuss)


请完成如下练习，完成代码填写，并形成spoc练习报告

### 1. 分析并描述创建分配进程的过程

> 注意 state、pid、cr3，context，trapframe的含义

### 练习2：分析并描述新创建的内核线程是如何分配资源的

> 注意 理解对kstack, trapframe, context等的初始化


当前进程中唯一，操作系统的整个生命周期不唯一，在get_pid中会循环使用pid，耗尽会等待

### 练习3：

阅读代码，在现有基础上再增加一个内核线程，并通过增加cprintf函数到ucore代码中
能够把内核线程的生命周期和调度动态执行过程完整地展现出来

### 练习4 （非必须，有空就做）：增加可以睡眠的内核线程，睡眠的条件和唤醒的条件可自行设计，并给出测试用例，并在spoc练习报告中给出设计实现说明

### 扩展练习1: 进一步裁剪本练习中的代码，比如去掉页表的管理，只保留段机制，中断，内核线程切换，print功能。看看代码规模会小到什么程度。
