# lab0 SPOC思考题

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

---
