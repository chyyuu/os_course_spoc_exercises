# lab0 SPOC思考题

##**提前准备**
（请在上课前完成）

 - 完成lec2的视频学习和提交对应的在线练习
 - git pull ucore_os_lab, v9_cpu, os_course_spoc_exercises  　in github repos。这样可以在本机上完成课堂练习。
 - 了解代码段，数据段，执行文件，执行文件格式，堆，栈，控制流，函数调用,函数参数传递，用户态（用户模式），内核态（内核模式）等基本概念。思考一下这些基本概念在linux, ucore, v9-cpu中的os*.c中是如何具体体现的。
 - 安装好ucore实验环境，能够编译运行ucore labs中的源码。
 - 会使用linux中的shell命令:objdump，nm，file, strace，gdb等，了解这些命令的用途。
 - 会编译，运行，使用v9-cpu的dis,xc, xem命令（包括启动参数），阅读v9-cpu中的cpu.md文档，了解汇编指令的类型和含义等，了解v9-cpu的细节。如能debug dis, xc, xem更佳。
 - 了解基于v9-cpu的执行文件的格式和内容，以及它是如何加载到v9-cpu的内存中的。
 - 在piazza上就学习中不理解问题进行提问。

## 个人思考题

---

能否读懂ucore中的AT&T格式的X86-32汇编语言？请列出你不理解的汇编语言。
- [x]  

> 基本可以，注释很清晰。

虽然学过计算机原理和x86汇编（根据THU-CS的课程设置），但对ucore中涉及的哪些硬件设计或功能细节不够了解？
- [x]  

> 整体的工程结构还不是很清楚，有很多地方还不太了解。


哪些困难（请分优先级）会阻碍你自主完成lab实验？
- [x]  

>  高 - 在实验初期可能对整个工程的结构不够熟悉，导致上手较慢。
高 - 后期工程比较庞大，调试起来可能会比较困难。
中 - 碰上其他课程的ddl。

如何把一个在gdb中或执行过程中出现的物理/线性地址与你写的代码源码位置对应起来？
- [x]  

> 在gdb中出现的地址是线性地址，在这个位置设断点再次运行，当停止在断点时反编译附近的代码，再在源码（汇编）中搜索出相应代码即可。若要对应至c代码，查看相应函数入口名称即可对应回去。

了解函数调用栈对lab实验有何帮助？
- [x]  

> 更好地了解程序运行时的机制，利于调试。

你希望从lab中学到什么知识？
- [x]  

> 实现一个操作系统的步骤和细节。

---

## 小组讨论题

---

搭建好实验环境，请描述碰到的困难和解决的过程。
- [x]  

> 我使用了在windows下使用virtualbox加载虚拟硬盘的搭建方法，基本无困难。

熟悉基本的git命令行操作命令，从github上
的 http://www.github.com/chyyuu/ucore_lab 下载
ucore lab实验
- [x]  

> 已下载。

尝试用qemu+gdb（or ECLIPSE-CDT）调试lab1
- [x]   

> 正在尝试中。

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

> 这涉及到位域的概念，":"之前的变量叫做位域，这些位域并不需要占用一个完整的字节。":"之后的数字表示这个位域占多少位。

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

- [x]

> 这段代码编译不过。加上强制类型转换后，答案为65538。

请分析 [list.h](https://github.com/chyyuu/ucore_lab/blob/master/labcodes/lab2/libs/list.h)内容中大致的含义，并能include这个文件，利用其结构和功能编写一个数据结构链表操作的小C程序
- [x]  

> list.h定义了一个简单的链表结构。向链表中插入一个元素：
```
#include "list.h"
int main() {
list_entry_t* head, tmp;
list_init(head);
list_add(head, tmp);
return 0;
}
```

---

## 开放思考题

---

是否愿意挑战大实验（大实验内容来源于你的想法或老师列好的题目，需要与老师协商确定，需完成基本lab，但可不参加闭卷考试），如果有，可直接给老师email或课后面谈。
- [x]  

> 

## v9-cpu相关题目
---

### 提前准备
```
sudo apt-get install hexedit
cd YOUR v9-cpu DIR
git pull 
cd YOUR os_course_spoc_exercise DIR
git pull 
```

分析和实验funcall.c，需要完成的内容包括： 
-[X]

 - 修改代码，可正常显示小组两位同学的学号（用字符串） 
 - 生成funcall.c的汇编码，理解其实现并给汇编码写注释
 - 尝试用xem的简单调试功能单步调试代码
 - 回答如下问题：
   - funcall中的堆栈有多大？是内核态堆栈还是用户态堆栈
   - funcall中的全局变量ret放在内存中何处？如何对它寻址？
   - funcall中的字符串放在内存中何处？如何对它寻址？
   - 局部变量i在内存中的何处？如何对它寻址？
   - 当前系统是处于中断使能状态吗？
   - funcall中的函数参数是如何传递的？函数返回值是如何传递的？
   - 分析并说明funcall执行文件的格式和内容
　

分析和实验os0.c，需要完成的内容包括： 
-[X]

 - 生成os0.c的汇编码，理解其实现并给汇编码写注释
 - 尝试用xem的简单调试功能单步调试代码
 - 回答如下问题：
   - 何处设置的中断使能？   
   - 系统何时处于中断屏蔽状态？
   - 如果系统处于中断屏蔽状态，如何让其中断使能？
   - 系统产生中断后，CPU会做哪些事情？（在没有软件帮助的情况下）
   - CPU执行RTI指令的具体完成工作是哪些？

[HARD]分析和实验os1/os3.c，需要完成的内容包括： 
-[X]
 
 - os1中的task1和task2的栈的起始和终止地址是什么？
 - os1是如何实现任务切换的？
 - os3中的task1和task2的栈的起始和终止地址是什么？
 - os3是如何实现任务切换的？
 - os3的用户态task能够破坏内核态的系统吗？
