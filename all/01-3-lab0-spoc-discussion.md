# lab0 SPOC思考题

## 个人思考题

---

能否读懂ucore中的AT&T格式的X86-32汇编语言？请列出你不理解的汇编语言。
- [x]  
<<<<<<< HEAD
大体能够读懂，在汇编程序设计课程上学习过汇编语言
>  

虽然学过计算机原理和x86汇编（根据THU-CS的课程设置），但对ucore中涉及的哪些硬件设计或功能细节不够了解？
- [x]  
暂时没有
>   

请给出你觉得的中断的作用是什么？使用中断有何利弊？
- [x]
利：
1，使系统能够及时响应外部事件
2，使cpu允许多个外设同时工作，提高了cpu利用率
3，使cpu可以及时处理各种软硬件故障
弊：中断会终止当前主程序，打乱主程序时序
>   
=======

>  http://www.imada.sdu.dk/Courses/DM18/Litteratur/IntelnATT.htm
>  inb一般应用程序用不到的指令等。

虽然学过计算机原理和x86汇编（根据THU-CS的课程设置），但对ucore中涉及的哪些硬件设计或功能细节不够了解？
- [x]  

> 中断寄存器和非通用寄存器等。

>>>>>>> 256b3fc3a653c25424da063396f58e89e540b30c

哪些困难（请分优先级）会阻碍你自主完成lab实验？
- [x]  
1，对于操作系统的理解不足
2，代码风格
3，编程能力
>   

如何把一个在gdb中或执行过程中出现的物理/线性地址与你写的代码源码位置对应起来？
- [x]  
<<<<<<< HEAD
gcc -g
>   

了解函数调用栈对lab实验有何帮助？
- [x]  
知道你函数栈的结构，便于调试
>   
=======

> 1. 在gdb中通过break加行号得到物理地址，list加*物理地址得到行号。
> 2. 用nm, objdump工具可以看到

了解函数调用栈对lab实验有何帮助？
- [x]  

> 除了错可以调试 
> 对于函数的调用过程和程序的运行过程有更好的理解。
> 便于调试以及检查。 
>>>>>>> 256b3fc3a653c25424da063396f58e89e540b30c

你希望从lab中学到什么知识？
- [x]  
对操作系统产生全面的了解
>   

---

## 小组讨论题

---

搭建好实验环境，请描述碰到的困难和解决的过程。
<<<<<<< HEAD
- [x]  没用虚拟机，自己慢慢配的系统
      对于understand等工具不太会用
> 
=======
- [x]  

> 困难：在virtualbox中设置虚拟机的时候找不到Linux的64位选项。
> 解决：需要通过BIOS设置将电脑的虚拟化功能打开（本电脑LenovoY480的VT功能是锁的，需要打开）。
> 开始时选择了UBUNTU 32位，不能启动，后来换成64位就能顺利运行
>>>>>>> 256b3fc3a653c25424da063396f58e89e540b30c

熟悉基本的git命令行操作命令，从github上
的 http://www.github.com/chyyuu/ucore_lab 下载
ucore lab实验
- [x]  
<<<<<<< HEAD
ok
> 

尝试用qemu+gdb（or ECLIPSE-CDT）调试lab1
- [x]  
ok
> 

对于如下的代码段，请说明”：“后面的数字是什么含义
```
/* Gate descriptors for interrupts and traps */
struct gatedesc {
=======

> clone 仓库 
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
>>>>>>> 256b3fc3a653c25424da063396f58e89e540b30c
    unsigned gd_off_15_0 : 16;        // low 16 bits of offset in segment
    unsigned gd_ss : 16;            // segment selector
    unsigned gd_args : 5;            // # args, 0 for interrupt/trap gates
    unsigned gd_rsv1 : 3;            // reserved(should be zero I guess)
    unsigned gd_type : 4;            // type(STS_{TG,IG32,TG32})
    unsigned gd_s : 1;                // must be 0 (system)
    unsigned gd_dpl : 2;            // descriptor(meaning new) privilege level
    unsigned gd_p : 1;                // Present
    unsigned gd_off_31_16 : 16;        // high bits of offset in segment
<<<<<<< HEAD
};
```

- :后面的数字是占用的位数（bit）
=======
 };
 ```
>>>>>>> 256b3fc3a653c25424da063396f58e89e540b30c

- [x]  

<<<<<<< HEAD
=======
> 每一个filed(域，成员变量)在struct(结构)中所占的位数; 也称“位域”，用于表示这个成员变量占多少位(bit)。

>>>>>>> 256b3fc3a653c25424da063396f58e89e540b30c
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


- intr结果为 0xE0E0000010002

> 

请分析 [list.h](https://github.com/chyyuu/ucore_lab/blob/master/labcodes/lab2/libs/list.h)内容中大致的含义，并能include这个文件，利用其结构和功能编写一个数据结构链表操作的小C程序
```C
#include <stdio.h>
#include <list.h>

struct Ints {
  int data;
  list_entry_t le;
};

#define le2struct(ptr) to_struct((ptr), struct Ints, le)
#define to_struct(ptr, type, member) \
  ((type *)((char *)(ptr) - offsetof(type, member)))
#define offsetof(type, member) \
  ((size_t)(&((type *)0)->member))

int main() {
  struct Ints one, two, three, *now_int;
  list_entry_t *now;
  one.data = 1;
  two.data = 2;
  three.data = 3;
  list_init(&one.le);
  list_add_before(&one.le, &two.le);
  list_add_after(&one.le, &three.le);

  now = &two.le;
  while (1) {
    now_int = le2struct(now);
    printf("Current: %d\n", now_int->data);
    now = now->next;
    if (now == &two.le)
      break;
  }

  return 0;
}
```
- 输出

```
Current: 2
Current: 1
Current: 3
```
- [x]  0x10002

> https://github.com/chyyuu/ucore_lab/blob/master/related_info/lab0/lab0_ex3.c

请分析 [list.h](https://github.com/chyyuu/ucore_lab/blob/master/labcodes/lab2/libs/list.h)内容中大致的含义，并能include这个文件，利用其结构和功能编写一个数据结构链表操作的小C程序
- [x]  

> 

---

## 开放思考题

---

是否愿意挑战大实验（大实验内容来源于你的想法或老师列好的题目，需要与老师协商确定，需完成基本lab，但可不参加闭卷考试），如果有，可直接给老师email或课后面谈。
- [x]  
否
>  

---
