# lec2：lab0 SPOC思考题

## **提前准备**
（请在上课前完成，option）

- 完成lec2的视频学习
- git pull ucore_os_lab, os_tutorial_lab, os_course_exercises  in github repos。这样可以在本机上完成课堂练习。
- 了解代码段，数据段，执行文件，执行文件格式，堆，栈，控制流，函数调用,函数参数传递，用户态（用户模式），内核态（内核模式）等基本概念。思考一下这些基本概念在不同操作系统（如linux, ucore,etc.)与不同硬件（如 x86, riscv, v9-cpu,etc.)中是如何相互配合来体现的。
- 安装好ucore实验环境，能够编译运行ucore labs中的源码。
- 会使用linux中的shell命令:objdump，nm，file, strace，gdb等，了解这些命令的用途。
- 会编译，运行，使用v9-cpu的dis,xc, xem命令（包括启动参数），阅读v9-cpu中的v9\-computer.md文档，了解汇编指令的类型和含义等，了解v9-cpu的细节。
- 了解基于v9-cpu的执行文件的格式和内容，以及它是如何加载到v9-cpu的内存中的。
- 在piazza上就学习中不理解问题进行提问。

---

## 思考题

- 你理解的对于类似ucore这样需要进程/虚存/文件系统的操作系统，在硬件设计上至少需要有哪些直接的支持？至少应该提供哪些功能的特权指令？

  答：需要的支持有：硬件支持时钟中断；虚存管理的地址映射机制（MMU）；安全可靠的存储介质来支持文件系统。应该提供的特权指令有：中断使能、设置内存寻址方式、页表管理、执行I/O文件系统 操作等特权指令。

- 你理解的x86的实模式和保护模式有什么区别？你认为从实模式切换到保护模式需要注意那些方面？

  答：实模式为16位，保护模式为32位，且实模式没有像保护模式那样提供强大的保护机制。我认为从实模式切换到保护模式需要注意的方面有：注意32位寄存器和16位寄存器的差别，注意两种模式下的中断机制的差异，以及在切换过程中地址的偏移等等。

- 物理地址、线性地址、逻辑地址的含义分别是什么？它们之间有什么联系？

  答：1. 物理地址：是处理器提交到总线上用于访问计算机系统中的内存和外设的最终地址。

  ​	2. 线性地址：线性地址是逻辑地址到物理地址变换之间的中间层，是处理器通过段(Segment)机制控制下的形成的地址空间。

  ​	3. 逻辑地址：在有地址变换功能的计算机中，访问指令给出的地址叫逻辑地址。

   	4. 它们之间的联系：可以通过段机制和页机制进行转换。

- 你理解的risc-v的特权模式有什么区别？不同模式在地址访问方面有何特征？

  答：risc-v架构定义了四种工作模式，又称特权模式：机器模式M，监督模式S，用户模式U，Hypervisor模式H，规定M为必选模式，另外三种为可选模式。标准 RISC-V ISA 设置了一个 12 位的编码空间（csr[11:0]）可用于 4096 个 CSR。根据约定，CSR 地址的高 4 位（csr[11:8]）用于编码CSR根据特权级读写的可访问性。最高2位（csr[11:10]）指示这个寄存器是否是可以读/写（00、01或者10），还是只读 的（11）。后面 2 位（csr[9:8]）指示了能够访问这个 CSR 所需要的最低特权级（用户级是 00， 管理员级是 01）。

- 理解ucore中list_entry双向链表数据结构及其4个基本操作函数和ucore中一些基于它的代码实现（此题不用填写内容）

- 对于如下的代码段，请说明":"后面的数字是什么含义
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

答：“:”后的数字表示每一个域在结构体中所占的位数。这样的实现把struct的变量定义精确到了bit的程度。

- 对于如下的代码段，

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
SETGATE(intr, 1,2,3,0);
```
请问执行上述指令后， intr的值是多少？

答：intr的值为0x00030002。

### 课堂实践练习

#### 练习一

1. 请在ucore中找一段你认为难度适当的AT&T格式X86汇编代码，尝试解释其含义。

   答：选择switch.S中的代码。

   ```
   .text
   .globl switch_to
   switch_to:                      # switch_to(from, to)
   
       # save from's registers
       movl 4(%esp), %eax          # eax points to from
       popl 0(%eax)                # save eip !popl
       movl %esp, 4(%eax)
       movl %ebx, 8(%eax)
       movl %ecx, 12(%eax)
       movl %edx, 16(%eax)
       movl %esi, 20(%eax)
       movl %edi, 24(%eax)
       movl %ebp, 28(%eax)
   
       # restore to's registers
       movl 4(%esp), %eax          # not 8(%esp): popped return address already
                                   # eax now points to to
       movl 28(%eax), %ebp
       movl 24(%eax), %edi
       movl 20(%eax), %esi
       movl 16(%eax), %edx
       movl 12(%eax), %ecx
       movl 8(%eax), %ebx
       movl 4(%eax), %esp
   
       pushl 0(%eax)               # push eip
   
       ret
   ```

   switch.S 是用于切换上下文。开始时，栈顶是返回地址，下面（`esp-4`）是`from`（因为参数是从右往左压栈的），再下面是`to`。系统先从栈中取出`from`，然后把该函数的返回地址弹出，保存到`from->eip`中。然后依次保存各个通用寄存器（段寄存器不需要保存，因为内核线程之间这些寄存器都一样）。因为`eax`中保存的总是返回值，所以可以不保存它，简化代码。之后就是从栈中再取出`to`，恢复通用寄存器，最后把`to->eip`入栈，保证返回之后能跳转到正确地址。

2. (option)请在rcore中找一段你认为难度适当的RV汇编代码，尝试解释其含义。

#### 练习二

宏定义和引用在内核代码中很常用。请枚举ucore或rcore中宏定义的用途，并举例描述其含义。

答：1. 利用宏进行复杂数据结构中的数据访问； 

 	2. 利用宏进行数据类型转换；如 to_struct,;
 	3. 常用功能的代码片段优化；如 ROUNDDOWN, SetPageDirty。


## 问答题

#### 在配置实验环境时，你遇到了那些问题，是如何解决的。

答：由于之前就有过使用各种虚拟机镜像的经验，并且这次我直接使用了已经配置好的实验环境和虚拟机，貌似没有遇到什么大问题。

## 参考资料
 - [Intel格式和AT&T格式汇编区别](http://www.cnblogs.com/hdk1993/p/4820353.html)
 - [x86汇编指令集  ](http://hiyyp1234.blog.163.com/blog/static/67786373200981811422948/)
 - [PC Assembly Language, Paul A. Carter, November 2003.](https://pdos.csail.mit.edu/6.828/2016/readings/pcasm-book.pdf)
 - [*Intel 80386 Programmer's Reference Manual*, 1987](https://pdos.csail.mit.edu/6.828/2016/readings/i386/toc.htm)
 - [IA-32 Intel Architecture Software Developer's Manuals](http://www.intel.com/content/www/us/en/processors/architectures-software-developer-manuals.html)
 - [v9 cpu architecture](https://github.com/chyyuu/os_tutorial_lab/blob/master/v9_computer/docs/v9_computer.md)
 - [RISC-V cpu architecture](http://www.riscvbook.com/chinese/)
 - [OS相关经典论文](https://github.com/chyyuu/aos_course_info/blob/master/readinglist.md)
