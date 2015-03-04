# lab0 SPOC思考题

## 个人思考题

---

能否读懂ucore中的AT&T格式的X86-32汇编语言？请列出你不理解的汇编语言。
- [x]  

>  http://www.imada.sdu.dk/Courses/DM18/Litteratur/IntelnATT.htm
    基本懂

虽然学过计算机原理和x86汇编（根据THU-CS的课程设置），但对ucore中涉及的哪些硬件设计或功能细节不够了解？
- [x]  

>   中断、段页式管理、分时、内存管理、进程间的数据交互


哪些困难（请分优先级）会阻碍你自主完成lab实验？
- [x]  

>   git使用不熟练、实验环境搭配困难、相关细节不明确

如何把一个在gdb中或执行过程中出现的物理/线性地址与你写的代码源码位置对应起来？
- [x]  

>   代码源码位置为逻辑地址，它通过段机制映射成线性地址，线性地址通过页机制映射成物理地址。

了解函数调用栈对lab实验有何帮助？
- [x]  

>   能够更有效地实现资源管理和分配以及进程间的调度

你希望从lab中学到什么知识？
- [x]  

>   如何处理中断、段页式管理、分时、内存管理和进程间数据交互

---

## 小组讨论题

---

搭建好实验环境，请描述碰到的困难和解决的过程。
- [x]  

> 基本搭建完成。

熟悉基本的git命令行操作命令，从github上的[ucore git repo](http://www.github.com/chyyuu/ucore_lab)下载ucore lab实验
- [x]  

> 下载完成。

尝试用qemu+gdb（or ECLIPSE-CDT）调试lab1
- [x]  

> 调试完成。

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

> 表示该变量所占用的bit数

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

> 65538(10进制)

请分析 [list.h](https://github.com/chyyuu/ucore_lab/blob/master/labcodes/lab2/libs/list.h)内容中大致的含义，并能include这个文件，利用其结构和功能编写一个数据结构链表操作的小C程序
- [x]  

> list.h这个文件构建了一个双向循环链表结构并规定了它的一些基本操作，比如初始化、往前插入、往后插入、删除等。
  程序如下：
  #include "list.h"
  #include <cstdlib>
  #include <iostream>
  using namespace std;

  int main()
  {
      list_entry_t* elm1 = new list_entry_t;
      list_entry_t* elm2 = new list_entry_t;
      list_entry_t* elm3 = new list_entry_t;
      list_init(elm1);
      list_add(elm1, elm2);
      list_add_before(elm2, elm3);
      cout << "elm3's next: " << elm3->next << ", and elm2: " << elm2 << endl;
      list_del(elm2);
      cout << "elm3's next: " << elm3->next << ", and elm1: " << elm1 << endl;
  }
  程序的预期结果应该为：第一次输出的elm3->next和elm2相等，第二次输出的elm3->next和elm1相等。
  实际结果如下：
  elm3's next: 0x100105530, and elm2: 0x100105530
  elm3's next: 0x100105520, and elm1: 0x100105520
  与预期结果相同


---

## 开放思考题

---

是否愿意挑战大实验（大实验内容来源于你的想法或老师列好的题目，需要与老师协商确定，需完成基本lab，但可不参加闭卷考试），如果有，可直接给老师email或课后面谈。
- [x]  

>  能力有限，加之本学期课程略多，故不选择参加。

---
