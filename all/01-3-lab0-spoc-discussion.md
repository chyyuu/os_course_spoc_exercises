# lab0 SPOC思考题

## 个人思考题

---

能否读懂ucore中的AT&T格式的X86-32汇编语言？请列出你不理解的汇编语言。


- 大概能看懂，但是对某些语法不是很熟悉，因为在学计算机组成原理时接触的是MIPS汇编。尤其对
lab1_print_cur_status(void) {
    static int round = 0;
    uint16_t reg1, reg2, reg3, reg4;
    asm volatile (
            "mov %%cs, %0;"
            "mov %%ds, %1;"
            "mov %%es, %2;"
            "mov %%ss, %3;"
            : "=m"(reg1), "=m"(reg2), "=m"(reg3), "=m"(reg4));
    cprintf("%d: @ring %d\n", round, reg1 & 3);
    cprintf("%d:  cs = %x\n", round, reg1);
    cprintf("%d:  ds = %x\n", round, reg2);
    cprintf("%d:  es = %x\n", round, reg3);
    cprintf("%d:  ss = %x\n", round, reg4);
    round ++;
}
这样的扩展GCC内联汇编不了解。但是希望在做实验的过程中能逐步掌握理解。
>  


虽然学过计算机原理和x86汇编（根据THU-CS的课程设置），但对ucore中涉及的哪些硬件设计或功能细节不够了解？
- 学计算机组成原理的时候，大作业中设计的CPU为16位的简单系统，而UCore基于的80386相比之下要复杂很多，
寄存器更多。计原和汇编时主要还是关注MIPS汇编，对X86汇编不太熟悉。尤其是涉及到访存的时候，Intel 80386
同时采用段页式寻址也更为复杂，我们在计原课上只是了解简单的段式或者页式。


>   

请给出你觉得的中断的作用是什么？使用中断有何利弊？
- 中断是并行的基础，能够使CPU在运行过程中响应外部事件并及时处理，可以使计算机更好地利用系统资源，提升运行效率。相比轮询的方式，中断把外部事件请教交由外设发出，如果中断频繁发生，则会在一定程度上降低效率。  



哪些困难（请分优先级）会阻碍你自主完成lab实验？
- 1.需要阅读大量代码，理解实验框架 2. 对课程原理部分的知识掌握不牢 4. 调试困难 5.代码量较大  

>   

如何把一个在gdb中或执行过程中出现的物理/线性地址与你写的代码源码位置对应起来？
- 利用gdb能够打印查看程序的源码。要满足词，程序编译时需要加上参数-g，该参数能把
源程序的相应信息在编译时加到可执行文件里。一般来说，单个程序再内存中是连续存放的，gdb显示出的物理地址要和源码对应，也可以使用info line命令来查看源代码在内存中的地址，这样就可以大致定位相应的代码和物理地址位置。info line后面可以跟
“行号”，“函数名”，“文件名:行号”，“文件名:函数名”，这个命令会打印出所指定的
源码在运行时的内存地址

> 1. 在gdb中通过break加行号得到物理地址，list加*物理地址得到行号。
> 2. 用nm, objdump工具可以看到

了解函数调用栈对lab实验有何帮助？
- 可以清晰地了解各个函数间的调用关系，从而更清楚地理解整个实验框架的结构层次，对实验很有帮助 

> 除了错可以调试 
> 对于函数的调用过程和程序的运行过程有更好的理解。
> 便于调试以及检查。 

你希望从lab中学到什么知识？
- 可以清晰地了解各个函数间的调用关系，理解整个实验框架的结构层次,尤其是对多个函数嵌套调用的情况，能更有效地对lab实验框架的代码进行分析

>   

---

## 小组讨论题

---

搭建好实验环境，请描述碰到的困难和解决的过程。
- 不太想用虚拟机，感觉自己的机子上跑虚拟机很慢。装了linux的双系统，正在搭环境。qemu和gdb配置到Eclipse-CDT上有些麻烦  

> 困难：在virtualbox中设置虚拟机的时候找不到Linux的64位选项。
> 解决：需要通过BIOS设置将电脑的虚拟化功能打开（本电脑LenovoY480的VT功能是锁的，需要打开）。
> 开始时选择了UBUNTU 32位，不能启动，后来换成64位就能顺利运行


> gitclone http://www.github.com/chyyuu/ucore_lab

尝试用qemu+gdb（or ECLIPSE-CDT）调试lab1
- [x]   

> 清除文件夹：make clean 
> 编译lab1：make 
> 调出debug命令行：make debug

对于如下的代码段，请说明”：“后面的数字是什么含义
```
 /* Gate descriptors for interrupts and traps */
 struct gatedesc {
    unsigned gd_off_15_0 : 16;        // low 16 bits of offset in segment
    unsigned gd_ss : 16;            // segment selector
    unsigned gd_args : 5;            // # args, 0 for interrupt/trap gates
    unsigned gd_rsv1 : 3;            // reserved(should be zero I guess)
    unsigned gd_type : 4;            // type(STS_{TG,IG32,TG32})
    unsigned gd_s : 1;                // must be 0 (system)
    unsigned gd_dpl : 2;            // descriptor(meaning new) privilege level
    unsigned gd_p : 1;                // Present
    unsigned gd_off_31_16 : 16;        // high bits of offset in segment
 };
 ```

- [x]  

> 每一个filed(域，成员变量)在struct(结构)中所占的位数; 也称“位域”，用于表示这个成员变量占多少位(bit)。

对于如下的代码段，
```
#define SETGATE(gate, istrap, sel, off, dpl) {            \
    (gate).gd_off_15_0 = (uint32_t)(off) & 0xffff;        \
    (gate).gd_ss = (sel);                                \
    (gate).gd_args = 0;                                    \
    (gate).gd_rsv1 = 0;                                    \
    (gate).gd_type = (istrap) ? STS_TG32 : STS_IG32;    \
    (gate).gd_s = 0;                                    \
    (gate).gd_dpl = (dpl);                                \
    (gate).gd_p = 1;                                    \
    (gate).gd_off_31_16 = (uint32_t)(off) >> 16;        \
}
```
如果在其他代码段中有如下语句，
```
unsigned intr;
intr=8;
SETGATE(intr, 0,1,2,3);
```
请问执行上述指令后， intr的值是多少？

- [x]  0x10002

> https://github.com/chyyuu/ucore_lab/blob/master/related_info/lab0/lab0_ex3.c

请分析 [list.h](https://github.com/chyyuu/ucore_lab/blob/master/labcodes/lab2/libs/list.h)内容中大致的含义，并能include这个文件，利用其结构和功能编写一个数据结构链表操作的小C程序
- [x]  

> 

---

## 开放思考题

---

是否愿意挑战大实验（大实验内容来源于你的想法或老师列好的题目，需要与老师协商确定，需完成基本lab，但可不参加闭卷考试），如果有，可直接给老师email或课后面谈。
- 课程尚未开始，不知道课程难度如何，是否有余力  

>  

---
