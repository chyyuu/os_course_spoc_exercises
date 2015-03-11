# lab1 SPOC思考题

## 个人思考题

NOTICE
- 有"w2l2"标记的题是助教要提交到学堂在线上的。
- 有"w2l2"和"spoc"标记的题是要求拿清华学分的同学要在实体课上完成，并按时提交到学生对应的git repo上。
- 有"hard"标记的题有一定难度，鼓励实现。
- 有"easy"标记的题很容易实现，鼓励实现。
- 有"midd"标记的题是一般水平，鼓励实 现。
---

请描述ucore OS配置和驱动外设时钟的准备工作包括哪些步骤？ (w2l2)
```
  首先需要初始化终端描述符表IDT，在init目录下的init.c里，kern_init需要完成IDT的初始化。
  IDT的初始化简单来说需要完成以下几点：
  1 设置中断描述符表的每一个中断描述符
  2 用lidt指令讲IDT表的起始地址存于IDTR寄存器中
  其中包含设置时钟中断的中断描述符表

  然后需要设置8253时钟外设的初始化，在clock.c的clock_init函数中，函数如下：
  void
clock_init(void) {
    // set 8253 timer-chip
    outb(TIMER_MODE, TIMER_SEL0 | TIMER_RATEGEN | TIMER_16BIT);
    outb(IO_TIMER1, TIMER_DIV(100) % 256);
    outb(IO_TIMER1, TIMER_DIV(100) / 256);

    // initialize time counter 'ticks' to zero
    ticks = 0;

    cprintf("++ setup timer interrupts\n");
    pic_enable(IRQ_TIMER);
}
  包括设置时钟中断的时间间隔等信息
  在clock_init函数中，设置完相关信息后，需要用pic_enable函数进行使能。
  在此之前，还需要设置时钟中断的中断服务例程，这在trap.c的trap_dispatch函数中，对相应的case进行处理。
  这些设置完成后需要调用 intr_enable(); 函数使能中断，该函数实际上是一条汇编指令sti
  另外，在init.c的函数里，进行以上步骤之前需要调用pic_init();函数，初始化8259A的中断控制器。

  
  
  + 采分点：说明了ucore OS在让外设时钟正常工作的主要准备工作
  - 答案没有涉及如下3点；（0分）
  - 描述了对IDT的初始化，包了针时钟中断的中断描述符的设置（1分）
  - 除第二点外，进一步描述了对8259中断控制器的初始过程（2分）
  - 除上述两点外，进一步描述了对8253时钟外设的初始化，或描述了对EFLAG操作使能中断（3分）
 ```
- [x]  

>  

lab1中完成了对哪些外设的访问？ (w2l2)
 ```
  lab1访问的外设包括串口，并口，CGA，时钟和硬盘
  + 采分点：说明了ucore OS访问的外设
  - 答案没有涉及如下3点；（0分）
  - 说明了时钟（1分）
  - 除第二点外，进一步说明了串口（2分）
  - 除上述两点外，进一步说明了并口，或说明了CGA，或说明了键盘（3分）
 ```
- [x]  

>  

lab1中的cprintf函数最终通过哪些外设完成了对字符串的输出？ (w2l2)
 ```
  cprintf用到的外设是串口，并口和CGA
  以任意一个文件里的cprintf函数调用分析，如init.c中的lab1_print_cur_status函数，可找到cprintf的定义在stdio.c中，
  int
cprintf(const char *fmt, ...) {
    va_list ap;
    int cnt;
    va_start(ap, fmt);
    cnt = vcprintf(fmt, ap);
    va_end(ap);
    return cnt;
}
  其中宏va_start和宏va_end用到的是GCC内置的_buildin函数。用Understand的butterfly追踪vcprintf的调用关系，
  能够找到其调用了cons_putc这一函数，继续对cons_putc进行追踪，可发现cons_putc 调用了三个函数：ipt_putc,
  serial_putc和cga_putc,这三个函数分别进行了并口，串口和CGA的操作。如果继续跟踪的话，可以看到串口/并口都是调用了
  delay，inb，outb这三个函数，而cga_putc则用到了outb和memmove，memmove把要输出的字符存到制定的内存取。应该可以理解为
  是一个显存缓冲区。
	
  + 采分点：说明了cprintf函数用到的3个外设
  - 答案没有涉及如下3点；（0分）
  - 说明了串口（1分）
  - 除第二点外，进一步说明了并口（2分）
  - 除上述两点外，进一步说明了CGA（3分）
 ```
- [x]  

>  

---

## 小组思考题

---

lab1中printfmt函数用到了可变参，请参考写一个小的linux应用程序，完成实现定义和调用一个可变参数的函数。(spoc)
- 如下是一个简单的变参函数，用于整数求和，第一个固定参数指出求和数的个数
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

int mySum(int num, ...) {
    int sum=0;
    va_list ap;
    va_start(ap,num);
    int i = 0;
    for(i = 0; i<num; i++) {
        sum += va_arg(ap,int);
    }
    va_end(ap);
    return sum;
}

void main() {

    int sum = mySum(6,1,2,3,4,5,6);
    printf("the sum is:%d \n", sum);
    return;
}


  



如果让你来一个阶段一个阶段地从零开始完整实现lab1（不是现在的填空考方式），你的实现步骤是什么？（比如先实现一个可显示字符串的bootloader（描述一下要实现的关键步骤和需要注意的事项），再实现一个可加载ELF格式文件的bootloader（再描述一下进一步要实现的关键步骤和需要注意的事项）...） (spoc)
-   首先，实现一个简单的bootloader，改bootloader只是单纯的能打印一个表示启动信息的字符串
    改bootloader需要注意，由于是在初始时是在实模式下，16位寻址空间，要注意段模式的设置，实际上此时未启用段模式，则需要
    设置一个gdt，设置相应的寻址规则。
    其次，需要让bootloader加载ELF格式的kernel。则需要先设置elf头，program header等信息。在读取的时候，要注意只能
    一个扇区一个扇区地读，先读取elf头，判断elf头是否符合格式等等。
    然后跳转至UCore 的起始地址执行UCore，kern里的各种初始化也需要进行设置。

> 


如何能获取一个系统调用的调用次数信息？如何可以获取所有系统调用的调用次数信息？请简要说明可能的思路。(spoc)
- 我们想象的一个思路是在中断描述符中增加相应的信息。现在一个中断描述符中保存着中断服务例程的段选择子，offset等信息，可增加一个域，用以记录改服务例程被调用的次数。每次该段选择子被加载时，同时把调用次数加1.当然，因为能使用的位数有限，可设置为记录到一定次数后就清零。IDT初始化时可以把改计数位置0.这是记录，获取的话，因为IDT应该在内核态使用，是不是可以增加一个额外的系统调用接口，负责获取指定系统调用的调用次数。也可以单独设一个类似IDT的全局表来专门负责存储调用次数信息。  

> 

如何裁减lab1, 实现一个可显示字符串"THU LAB1"且依然能够正确加载ucore OS的bootloader？如果不能完成实现，请说明理由。
- [x]  

> 

对于ucore_lab中的labcodes/lab1，我们知道如果在qemu中执行，可能会出现各种稀奇古怪的问题，比如reboot，死机，黑屏等等。请通过qemu的分析功能来动态分析并回答lab1是如何执行并最终为什么会出现这种情况？
- [x]  

> 

对于ucore_lab中的labcodes/lab1,如果出现了reboot，死机，黑屏等现象，请思考设计有效的调试方法来分析常在现背后的原因。
- [x]  

> 

---

## 开放思考题

---

如何修改lab1, 实现在出现除零错误异常时显示一个字符串的异常服务例程的lab1？
- [x]  

> 


在lab1/bin目录下，通过`objcopy -O binary kernel kernel.bin`可以把elf格式的ucore kernel转变成体积更小巧的binary格式的ucore kernel。为此，需要如何修改lab1的bootloader, 能够实现正确加载binary格式的ucore OS？ (hard)
- [x]  

>

GRUB是一个通用的bootloader，被用于加载多种操作系统。如果放弃lab1的bootloader，采用GRUB来加载ucore OS，请问需要如何修改lab1, 能够实现此需求？ (hard)
- [x]  

>


如果没有中断，操作系统设计会有哪些问题或困难？在这种情况下，能否完成对外设驱动和对进程的切换等操作系统核心功能？
- [x]  

>  

---

## 各知识点的小练习

### 4.1 启动顺序
---
读入ucore内核的代码？

- [x]  

> 

跳转到ucore内核的代码？

- [x]  

> 

全局描述符表的初始化代码？

- [x]  

> 

GDT内容的设置格式？初始映射的基址和长度？特权级的设置位置？

- [x]  

> 

可执行文件格式elf的各个段的数据结构？

- [x]  

> 

如果ucore内核的elf是否要求连续存放？为什么？

- [x]  

> 
---

### 4.2 C函数调用的实现
---

函数调用的stackframe结构？函数调用的参数传递方法有哪几种？
- [x]  

> 

系统调用的stackframe结构？系统调用的参数传递方法有哪几种？

- [x]  

> 
---

### 4.3 GCC内联汇编
---

使用内联汇编的原因？

- [x]  

> 特权指令、性能优化

对ucore中的一段内联汇编进行完整的解释？

- [x]  

> 
---

### 4.4 x86中断处理过程
---

4.4 x86中断处理过程

中断描述符表IDT的结构？

- [x]  

> 

中断描述表到中断服务例程的地址计算过程？

- [x]  

> 

中断处理中硬件压栈内容？用户态中断和内核态中断的硬件压栈有什么不同？

- [x]  

> 

中断处理中硬件保存了哪些寄存器？

- [x]  

> 
---

### 4.5 练习一 ucore编译过程
---

gcc编译、ld链接和dd生成两个映像对应的makefile脚本行？

- [x]  

> 
---

### 4.6 练习二 qemu和gdb的使用
---

qemu的命令行参数含义解释？

- [x]  

> 

gdb命令格式？反汇编、运行、断点设置

- [x]  

> 
---

### 练习三 加载程序
---

A20的使能代码分析？

- [x]  

> 

生成主引导扇区的过程分析？

- [x]  

> 

保护模式的切换代码？

- [x]  

> 

如何识别elf格式？对应代码分析？

- [x]  

> 

跳转到elf的代码？

- [x]  

> 
函数调用栈获取？

- [x]  

> 
---

### 4.8 练习四和五 ucore内核映像加载和函数调用栈分析
---

如何识别elf格式？对应代码分析？

- [x]  

> 

跳转到elf的代码？

- [x]  

> 

函数调用栈获取？
- [x]  

> 
---


### 4.9 练习六 完善中断初始化和处理
---

各种设备的中断初始化？
- [x]  

> 
中断描述符表IDT的排列顺序？
- [x]  

> 中断号
CPU加电初始化后中断是使能的吗？为什么？

- [x]  

> 
中断服务例程的入口地址在什么地方设置的？

- [x]  

> 
alltrap的中断号是在哪写入到trapframe结构中的？

- [x]  

> 
trapframe结构？
- [x]  

> 
---
