# lab5 spoc 思考题

- 有"spoc"标记的题是要求拿清华学分的同学要在实体课上完成，并按时提交到学生对应的ucore_code和os_exercises的git repo上。


## 个人思考题

### 总体介绍

 - 第一个用户进程创建有什么特殊的？
 
 - 系统调用的参数传递过程？
 
 - getpid的返回值放在什么地方了？

### 进程的内存布局

 - 尝试在进程运行过程中获取内核堆栈和用户堆栈的调用栈？
 - 尝试在进程运行过程中获取内核空间中各进程相同的页表项（代码段）和不同的页表项（内核堆栈）？

### 执行ELF格式的二进制代码-do_execve的实现

 - 在do_execve中进程清空父进程时，当前进程是哪一个？在什么时候开始使用新加载进程的地址空间？
 - 新加载进程的第一级页表的建立代码在哪？

### 执行ELF格式的二进制代码-load_icode的实现

 - 第一个内核线程和第一个用户进程的创建有什么不同？
 - 尝试跟踪分析新创建的用户进程的开始执行过程？

### 进程复制

 - 为什么新进程的内核堆栈可以先于进程地址空间复制进行创建？
 - 进程复制的代码在哪？复制了哪些内容？
 - 进程复制过程中有哪些修改？为什么要修改？

### 内存管理的copy-on-write机制
 - 什么是写时复制？
 - 写时复制的页表在什么时候进行复制？共享地址空间和写时复制有什么不同？

## 小组练习与思考题

### (1)(spoc) 在真实机器的u盘上启动并运行ucore lab,

请准备一个空闲u盘，然后请参考如下网址完成练习

https://github.com/chyyuu/ucore_lab/blob/master/related_info/lab1/lab1-boot-with-grub2-in-udisk.md

> 注意，grub_kernel的源码在ucore_lab的lab1_X的git branch上，位于 `ucore_lab/labcodes_answer/lab1_result`

(报告可课后完成)请理解grub multiboot spec的含义，并分析ucore_lab是如何实现符合grub multiboot spec的，并形成spoc练习报告。

### (2)(spoc) 理解用户进程的生命周期。

> 需写练习报告和简单编码，完成后放到git server 对应的git repo中

### 练习用的[lab5 spoc exercise project source code](https://github.com/chyyuu/ucore_lab/tree/master/related_info/lab5/lab5-spoc-discuss)


#### 掌握知识点
1. 用户进程的启动、运行、就绪、等待、退出
2. 用户进程的管理与简单调度
3. 用户进程的上下文切换过程
4. 用户进程的特权级切换过程
5. 用户进程的创建过程并完成资源占用
6. 用户进程的退出过程并完成资源回收

> 注意，请关注：内核如何创建用户进程的？用户进程是如何在用户态开始执行的？用户态的堆栈是保存在哪里的？

阅读代码，在现有基础上再增加一个用户进程A，并通过增加cprintf函数到ucore代码中，
能够把个人思考题和上述知识点中的内容展示出来：即在ucore运行过程中通过`cprintf`函数来完整地展现出来进程A相关的动态执行和内部数据/状态变化的细节。(越全面细致越好)

在lab5中的UCore用户进程从开始创建到退出,有如下几个阶段:
    
    1. 由initproc在其执行函数init_main中调用kernel_thread函数创建一个内核线程,该内个线程为用户进程的壳
    
    2.initproc挂起,进入wait状态,等待创建的子线程执行
    
    3.内核子线程执行其执行函数user_main,user_main通过调用__KERNEL_EXECVE等函数,最用调用系统函数SYS_exec执行,其服务例程为do_execve,do_execve清空原线程mm,再调用load_icode
    
    4. load_icode从内存(由于没有文件系统,此时用户函数已在内存中)中加载用户函数,并设置用户态的内存空间和栈.然后  设置trapframe到刚建立的用户态空间
    
    5.系统调用逐层返回,恢复中断,转到之前设置的eip,即用户函数的入口开始执行
    
    6.用户函数执行完后,return,退出,执行系统函数do_exit,回收相关资源,并调度其他进程

在本次练习的UCore代码中,让原来创建的用户线程执行user/exit.c,再创建另一个用户进程,其执行的代码是user/hello.c中的内容,命名为用户进程A.具体程序代码见该目录下lab5-spoc-discuss中相应的内容,cprint打印出的信息如下,可以看到,从内核线程init开始,到创建内核线程A,再在A的内核函数中执行execve,加载hello.c,执行完毕后退出的过程.

    In proc_run, switch form kernel thread idle to kernel thread init
    In kernel_thread,the  trapframe of A has been created! It's going to use do_fork
    In do_fork, kernel thread   PCB(proc_struct) has been allocated!
    In do_fork, kernel thread   kernel stack has been allocated!
    In do_fork, kernel thread  memory has been copied!
    In do_fork, kernel thread   trapframe has been setted to the stack!
    In do_fork, kernel thread   the PCB has been added into the proc_list!
    In do_fork,  is going to be waked up!!
    In wakeup_proc,  has been waked up!!
    In init_main(), kernel thread A is going to be created!
    In kernel_thread, kernel thread A is being created,the trapframe of A is to be created!
    In kernel_thread,the  trapframe of A has been created! It's going to use do_fork
    In do_fork, kernel thread  I am A PCB(proc_struct) has been allocated!
    In do_fork, kernel thread I am A  kernel stack has been allocated!
    In do_fork, kernel thread I am A memory has been copied!
    In do_fork, kernel thread I am A  trapframe has been setted to the stack!
    In do_fork, kernel thread I am A  the PCB has been added into the proc_list!
    In do_fork, I am A is going to be waked up!!
    In wakeup_proc, I am A has been waked up!!
    In proc_run, switch form kernel thread init to kernel thread I am A
    Now kernel thread I am A is running, it's going to do execve!
    kernel_execve: pid = 3, name = "hello".
    In load_icode, user programmer is to be loaded
    In load_icode, the mm and stack of user programmer has been settedIn do_execve user programmed has been loaded, now it's user thread hello. It's going to do user function!
    Hello world!!.
    I am user process A, my pid is 3.
    hello pass.
    In do_exit, user process hello is gotiong to exit, and its resources will be called back!
    In wakeup_proc, init has been waked up!!
    In proc_run, switch form kernel thread init to kernel thread 
    kernel_execve: pid = 2, name = "exit".
    
请完成如下练习，完成代码填写，并形成spoc练习报告
