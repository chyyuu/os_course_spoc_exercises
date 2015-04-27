# lab6 spoc 思考题

- 有"spoc"标记的题是要求拿清华学分的同学要在实体课上完成，并按时提交到学生对应的ucore_code和os_exercises的git repo上。


## 个人思考题

### 总体介绍

 - ucore中的调度点在哪里，完成了啥事？
 - 进程控制块中与调度相关的字段有哪些？
 - ucore的就绪队列数据结构在哪定义？在哪进行修改？
 - ucore的等待队列数据结构在哪定义？在哪进行修改？

### 调度算法支撑框架

 - 调度算法支撑框架中的各个函数指针的功能是啥？会被谁在何种情况下调用？
 - 调度函数schedule()的调用函数分析，可以了解进程调度的原因。请分析ucore中所有可能的调度位置，并说明可能的调用原因。
 
### 时间片轮转调度算法

 - 时间片轮转调度算法是如何基于调度算法支撑框架实现的？
 - 时钟中断如何调用RR_proc_tick()的？

### stride调度算法

 - stride调度算法的思路？ 
 - stride算法的特征是什么？
 - stride调度算法是如何避免stride溢出问题的？
 - 无符号数的有符号比较会产生什么效果？
 - 什么是斜堆(skew heap)?

## 小组练习与思考题

### (1)(spoc) 理解调度算法支撑框架的执行过程

即在ucore运行过程中通过`cprintf`函数来完整地展现出来多个进程在调度算法和框架的支撑下，在相关调度点如何动态调度和执行的细节。(约全面细致约好)

调度点即调用schedule函数的位置，在lab6中，有如下位置调用了schedule函数：

do_exit(proc.c中) 当用户线程执行结束后，退出，cpu控制权交出

do_wait(proc.c中) 进程进入挂起状态，等待子进程结束退出

init_main(proc.c中)第一个实际的内核线程initproc，第一个用户进程是通过initproc创建的子进程加载的。initproc会等待所有用户子进程结束，然后调用schedule函数

cpu_idle(proc.c中)idle内核线程不断查询就绪队列，找到就绪进程并调用schedule函数进行调度。

trap(trap.c中)，用户态进程被打断，如果前进程控制块的成员变量need_resched设置为1，则当前线程会放弃CPU控制权

在各个调度点利用printf函数打印相关的信息，执行make run-priority，可以看到多个线程的调度过程。打印出的信息如下：

可以看到，首先是在cpu_idle里，initproc马上要被调度，之后进入stride_pick_next()进行选择，可以看到，被选择的是initproc，因为之后进入了initmain。然后在init_main里，调用了do_wait，initproc进入等待状态，随后调用proc2.之后就可以看到，用户进程里fork的各个子线程依次被调用然后退出。具体代码详见本目录下的lab6-spoc-discuss。

    Now it's in cpu_idle, current id 0 needs to be scheduled
     It's in stride_pick_next(), proc id 1 is selected! The stride of 1 is 2147483647 
    Now it's in init_main(), current proc is kernel thread, id is 1, name is init. It's going to do schedule()
     init_proc is waiting for the children
    Now it's in do_wait(), current proc is id 1. It's going to do schedule()
    It's in stride_pick_next(), proc id 2 is selected! The stride of 2 is 2147483647 
    kernel_execve: pid = 2, name = "priority".
    main: fork ok,now need to wait pids.
    Now it's in do_wait(), current proc is id 2. It's going to do schedule()
    It's in stride_pick_next(), proc id 7 is selected! The stride of 7 is 2147483647 
    child pid 7, acc 2148000, time 1002
    Now it's in do_exit(), current proc is id 7. It's going to do schedule()
    It's in stride_pick_next(), proc id 6 is selected! The stride of 6 is 2147483647 
    child pid 6, acc 4000, time 1004
    Now it's in do_exit(), current proc is id 6. It's going to do schedule()
    It's in stride_pick_next(), proc id 5 is selected! The stride of 5 is 2147483647 
    child pid 5, acc 4000, time 1007
    Now it's in do_exit(), current proc is id 5. It's going to do schedule()
    It's in stride_pick_next(), proc id 4 is selected! The stride of 4 is 2147483647 
    child pid 4, acc 4000, time 1010
    Now it's in do_exit(), current proc is id 4. It's going to do schedule()
    It's in stride_pick_next(), proc id 3 is selected! The stride of 3 is 2147483647 
    child pid 3, acc 4000, time 1013
    Now it's in do_exit(), current proc is id 3. It's going to do schedule()
    It's in stride_pick_next(), proc id 2 is selected! The stride of 2 is -1789569708 
    main: pid 3, acc 4000, time 1013
    main: pid 4, acc 4000, time 1013
    main: pid 5, acc 4000, time 1013
    main: pid 6, acc 4000, time 1013
    main: pid 7, acc 2148000, time 1013
    main: wait pids over
    stride sched correct result: 1 1 1 1 537
    Now it's in do_exit(), current proc is id 2. It's going to do schedule()
    It's in stride_pick_next(), proc id 1 is selected! The stride of 1 is -2 
    It's in stride_pick_next(), proc id 1 is selected! The stride of 1 is 2147483645 
    all user-mode processes have quit.
    init check memory pass.
    kernel panic at kern/process/proc.c:460:
        initproc exit.






请完成如下练习，完成代码填写，并形成spoc练习报告
> 需写练习报告和简单编码，完成后放到git server 对应的git repo中

### 练习用的[lab6 spoc exercise project source code](https://github.com/chyyuu/ucore_lab/tree/master/labcodes_answer/lab6_result)


