# lab4 spoc 思考题

- 有"spoc"标记的题是要求拿清华学分的同学要在实体课上完成，并按时提交到学生对应的ucore_code和os_exercises的git repo上。

## 个人思考题

### 总体介绍

(1) ucore的线程控制块数据结构是什么？

### 关键数据结构

(2) 如何知道ucore的两个线程同在一个进程？

(3) context和trapframe分别在什么时候用到？

(4) 用户态或内核态下的中断处理有什么区别？在trapframe中有什么体现？

### 执行流程

(5) do_fork中的内核线程执行的第一条指令是什么？它是如何过渡到内核线程对应的函数的？
```
tf.tf_eip = (uint32_t) kernel_thread_entry;
/kern-ucore/arch/i386/init/entry.S
/kern/process/entry.S
```

(6)内核线程的堆栈初始化在哪？
```
tf和context中的esp
```

(7)fork()父子进程的返回值是不同的。这在源代码中的体现中哪？

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

lab4-spoc-disscussion简化版本的UCore内核线程创建为例,分析描述如下:

在kern_init函数中有如下代码:

    print_kerninfo();
    pmm_init();                 // init physical memory management
    pic_init();                 // init interrupt controller
    idt_init();                 // init interrupt descriptor table
    proc_init();                // init process table
    clock_init();               // init clock interrupt
    intr_enable();              // enable irq interrupt
    
当完成了物理内存初始化/中断初始化等工作后,就调用proc_init进行线程的初始化工作.在proc_int函数中.首先创建了一个名为idleproc的线程.idleproc是内核中的第一个线程(没有父线程),其作用是完成内核中各个子系统的一些简单初始化,然后就等待被调度.

创建idleproc线程时,首先通过alloc_proc函数分配一块内存空间用于存放该线程的PCB.用alloc_proc创建PCB时,进行了一些简单的初始化.在proc.才中可以看到,初始化代码如下:

    memset(proc, 0, sizeof(struct proc_struct));
    proc->state = PROC_UNINIT;
    proc->pid = -1;
    proc->cr3 = boot_cr3;
    
state是线程所处的状态,用于线程的调度管理.此处设置idleproc的初始状态为PROC_UNINIT,表示进程刚刚开始创建,尚未完成创建工作.这一状态可以理解为原理课上讲的进程模型中的"创建"状态.pid是线程的标识,此处设为-1,也是表示尚未完成创建.而cr3是线程的一级页表基址,cr3的值代表了线程的虚拟空间.因为idel是内核线程,其总是运行在内核态,所以用的空间实际上是UCore的内核空间,故其cr3值为boot_cr3.

完成PCB的创建后,proc_init函数中还会进一步进行进行其他状态的设置.如,将pid设为0,因为这是第一个线程,将状态设为PROC_RUNNABLE,即就绪状态,将线程的内核栈起始地址设为UCore的内核栈起始地址,设置need_resched为1.need_resched表示该线程需要被调度,当idel线程运行时,会立即被调度函数用其他线程替代.此外,还有设置idle线程的名字等等.

至此,已经完成了idelproc的创建,然后就可以通过调用kernel_thread创建"实际的"内核线程,这些内核线程用来完成一些实际的功能,在lab4中,就是打印简单的"Hello World".此时创建新线程,不同于idleproc的创建,而是通过do_fork函数来进行创建的.在调用do_fork之前,通过中断帧trapframe设置了新线程的一些参数,如设置新此案称的代码段和数据段等寄存器为内核段相应的值,设置ebx为fn.此处将新线程要执行的函数init_main以函数指针的形式传给kernel_thread,并将其基址保存在了reg_ebx中.调用do_fork函数.

do_fork函数主要完成以下工作:

    1. 调用alloc_proc函数分配PCB
    2. 调用setup_stack设置内核堆栈.在设置idleproc时,是直接将其内核堆栈的基址设为UCore内核堆栈的基址.单因为内核堆栈空间有限,不可能每个内核线程都拥有这个UCore内核堆栈.在idleproc之后的线程里,内核堆栈需要分配.setup_stack函数可以看出,此处为每个内核线程分配了两个page的空间作为其内核堆栈.
    3. 调用copy_thread函数设置进程在内核正常运行和调度所需的中断帧trapframe和上下文context.
    4.把进程控制块放入hash_list和proc_list
    5.唤醒线程,设置为就绪状态
    6.返回新创建的子线程的id
    
至此,就完成了一个内核线程的创建



> 注意 state、pid、cr3，context，trapframe的含义

### 练习2：分析并描述新创建的内核线程是如何分配资源的

由于idleproc比较特殊,就以其他的内核线程为例进行分析.

首先分配的资源为PCB,即分配并初始化一个proc struct

其次,需要设置线程的内核堆栈.通过setup_kstack函数为新线程在UCore内核堆栈中分配一定的空间,作为该线程的内核堆栈.setup_kstack的代码如下:

        struct Page *page = alloc_pages(KSTACKPAGE);
        if (page != NULL) {
            proc->kstack = (uintptr_t)page2kva(page);
            return 0;
        }
        return -E_NO_MEM;
        
可以看出,分配给每个内核线程的内核堆栈大小是两个page.

之后,需要在copy_thread函数中设置新线程的中断帧和上下文.trapframe在kernel_thread中就进行了一定的初始化,因为内核线程都是运行在内核堆栈,所以将trapframe中保存的cs寄存器设置值为KERNEL_CS;ds,es,ss寄存器设置为KERNEL_DS.另外新线程要执行的函数是init_main,将该函数的起始地址保存在reg_ebx中,将eip指针值设为kernel_thread_entry.在copy_thread函数中,有如下语句:

        proc->tf = (struct trapframe *)(proc->kstack + KSTACKSIZE) - 1;
        *(proc->tf) = *tf;
        proc->tf->tf_regs.reg_eax = 0;
        proc->tf->tf_esp = esp;
        proc->tf->tf_eflags |= FL_IF;
        proc->context.eip = (uintptr_t)forkret;
        proc->context.esp = (uintptr_t)(proc->tf);

通过将新线程内核栈栈顶的地址转为trapframe指针,并将设置为在kernel_thread中设置的trapframe.然后修改esp值为0.然后设置context上下文.当该进程被调度时,操作系统能够根据context的内容回复线程的执行.因为现在线程是新创建,所以对context的设置是初始化,即设置的是新线程要执行的第一条指令和其堆栈的指针.根据之前的设置,新线程的内核栈定是trapframe是首地址,所以只需要esp设置为中断帧起始处即可.新线程要执行的第一条指令是forket函数,forkret函数完成do_fork的一些返回处理工作.由此,就完成了对新线程的资源分配.
> 注意 理解对kstack, trapframe, context等的初始化


当前进程中唯一，操作系统的整个生命周期不唯一，在get_pid中会循环使用pid，耗尽会等待

### 练习3：阅读代码，在现有基础上再增加一个内核线程，并通过增加cprintf函数到ucore代码中
能够把进程的生命周期和调度动态执行过程完整地展现出来

新增加了一个内核线程,代码见该目录下lab4-spoc-discuss/kern/process/proc.c,编译运行,输出如下:

    kernel_thread, pid = 1, name = init1
    kernel_thread, pid = 2, name = init2
    kernel_thread, pid = 3, name = myinit
    kernel_thread, pid = 1, name = init1 , arg  init main1: Hello world!! 
    kernel_thread, pid = 2, name = init2 , arg  init main2: Hello world!! 
    kernel_thread, pid = 3, name = myinit , arg  init main3: Hello! I am MtMoon! I have been created! 
    kernel_thread, pid = 1, name = init1 ,  en.., Bye, Bye. :)
    do_exit: proc pid 1 will exit
    do_exit: proc  parent c02ff008
    kernel_thread, pid = 2, name = init2 ,  en.., Bye, Bye. :)
    do_exit: proc pid 2 will exit
    do_exit: proc  parent c02ff008
    kernel_thread, pid = 3, name = myinit ,  en.., Bye, Bye. :) 
    do_exit: proc pid 3 will exit


### 练习4 （非必须，有空就做）：增加可以睡眠的内核线程，睡眠的条件和唤醒的条件可自行设计，并给出测试用例，并在spoc练习报告中给出设计实现说明

### 扩展练习1: 进一步裁剪本练习中的代码，比如去掉页表的管理，只保留段机制，中断，内核线程切换，print功能。看看代码规模会小到什么程度。


