# lab2 SPOC思考题

NOTICE
- 有"w4l1"标记的题是助教要提交到学堂在线上的。
- 有"w4l1"和"spoc"标记的题是要求拿清华学分的同学要在实体课上完成，并按时提交到学生对应的git repo上。
- 有"hard"标记的题有一定难度，鼓励实现。
- 有"easy"标记的题很容易实现，鼓励实现。
- 有"midd"标记的题是一般水平，鼓励实现。

## 个人思考题
---

x86保护模式中权限管理无处不在，下面哪些时候要检查访问权限()  (w4l1)
- [x] 内存寻址过程中 段表：特权级/越界/读写权限/ 页表也有相应检查
- [x] 代码跳转过程中 短跳转：越界 长跳转段优先级
- [x] 中断处理过程中 相应权限
- [ ] ALU计算过程中
 

请描述ucore OS建立页机制的准备工作包括哪些步骤？ (w4l1) 
   
   1. 初始化GDT，使能段机制。UCore采用段页式机制，虽然为了简化，偏重于页机制，段映射时实际上采取了对等映射，即段机制实质上只发挥了保护机制的作用，真正其映射作用的是页机制。虽然如此，在建立页机制前仍然需要先初始化GDT，建立段机制。而段机制的建立经历了四个不同的阶段。在bootloader结束后，通过将cro寄存器的0bit为置为1，使能保护模式，同时开启段机制（在bootasm.S中）。
   2. 物理内存探测。建立页机制前，需要进行物理内存探测，初始化可用内存。该部分执行代码在bootasm.S中 从probe_memory处到finish_probe处。过BIOS中断获取内存可调用参数为e820h的INT 15h BIOS中断。BIOS通过系统内存映射地址描述符格式来表示系统物理内存布局。内存映射地址描述符定义在memlayout.h的结构体e820map。这一结构体被存储在0x8000的内存起始地址，头4个字节是nr_map，其map大小。
   该部分工作完成后，还有一下工作
   (1) 调用pmm_manager 的init函数，进行初始化
   (2) 调用pmm.c中的page_init()函数，进行页初始化。该函数要做比较多的工作，起始涉及用page结构体来保存可用内存的信息，以及将这些page结构体存在内存中，然后计算freemem，这是实际能用的内存的起始地址(不包括page结构体链表的存储)。
   (3) 调用pmm_manager的check函数检查内存分配管理算法是否正确
  3.二级页表的建立。在pmm.c中的pmm_init函数，执行完上述过程后，开始建立而级页表。
    二级页表与一级页表大小都是4k。首先通过alloc分配一个页，用来存储二级页表。
    代码是pmm_init里的：
    boot_pgdir = boot_alloc_page();
    memset(boot_pgdir, 0, PGSIZE);
    boot_cr3 = PADDR(boot_pgdir);
    同时将一级页表的地址暂存。
    然后通过以下代码：
     boot_pgdir[PDX(VPT)] = PADDR(boot_pgdir) | PTE_P | PTE_W;
     boot_map_segment(boot_pgdir, KERNBASE, KMEMSIZE, 0, PTE_W);
     把0~KERNSIZE（共0x38000000字节，896MB，3670016个物理页）的物理地址一一映射到页目录项和页表项的内容
    
  4. 使能页进制。调用enable_paging函数。在用enable_page函数到执行gdt_init函数之前，内核使用的还是旧的段表映射。通过 boot_pgdir[0] = boot_pgdir[PDX(KERNBASE)];使内核正常工作。
    在gtd_init函数之后，通过boot_pgdir[0] = 0;解除临时映射
    enable_paging函数中，将之前暂存的cr3寄存器的值写入cr3寄存器，将cr0寄存器的31bit位置为1，使能页机制。
    
    
   
  + 采分点：说明了ucore OS在让页机制正常工作的主要准备工作
  - 答案没有涉及如下3点；（0分）
  - 描述了对GDT的初始化,完成了段机制（1分）
  - 除第二点外进一步描述了对物理内存的探测和空闲物理内存的管理。（2分）
  - 除上述两点外，进一步描述了页表建立初始过程和设置CR0控寄存器某位来使能页（3分）

 ```
- [x]  

>  

---

## 小组思考题
---

（1）（spoc）请用lab1实验的基准代码（即没有修改的需要填空的源代码）来做如下实验： 执行`make qemu`，会得到一个输出结果，请给出合理的解释：为何qemu退出了？【提示】需要对qemu增加一些用于基于执行过的参数，重点是分析其执行的指令和产生的中断或异常。 

- 很奇怪，执行到kern_init的函数while(1)循环，然后单步跟踪时一直死循环，直接运行则会退出，暂时找不到中断触发点


> 

（2）(spoc)假定你已经完成了lab1的实验,接下来是对lab1的中断处理的回顾：请把你的学号对37(十进制)取模，得到一个数x（x的范围是-1<x<37），然后在你的答案的基础上，修init.c中的kern_init函数，在大约36行处，即

```
    intr_enable();              // enable irq interrupt
```
语句之后，加入如下语句(把x替换为你学号 mod 37得的值)：
```
    asm volatile ("int $x");
```    
然后，请回答加入这条语句后，执行`make qemu`的输出结果与你没有加入这条语句后执行`make qemu`的输出结果的差异，并解释为什么有差异或没差异？ 

x=19；
有差异。与没有加该条语句相比，在产生时钟中断之前，进入中断并输出了如下结果：
trapframe at 0x7b5c
  edi  0x00000001
  esi  0x00000000
  ebp  0x00007bc8
  oesp 0x00007b7c
  ebx  0x00010094
  edx  0x000000a1
  ecx  0x00000000
  eax  0x000000ff
  ds   0x----0010
  es   0x----0010
  fs   0x----0023
  gs   0x----0023
  trap 0x00000013 SIMD Floating-Point Exception
  err  0x00000000
  eip  0x00100070
  cs   0x----0008
  flag 0x00000207 CF,PF,IF,IOPL=0
kernel panic at kern/trap/trap.c:184:
    unexpected trap in kernel.

该条语句实际上是调用了中断号为19的中断，而在trap.c的trap_dispatch函数中可以看到，lab1支持的中断只包括32,36,21,152,153,46,47。其他都属于default，将调用如下代码：
   if ((tf->tf_cs & 3) == 0) {
        	//cprintf("lalalalala\n");
            print_trapframe(tf);
            panic("unexpected trap in kernel.\n");
        }
        产生错误

（3）对于lab2的输出信息，请说明数字的含义
```
e820map:
  memory: 0009fc00, [00000000, 0009fbff], type = 1.
  memory: 00000400, [0009fc00, 0009ffff], type = 2.
  memory: 00010000, [000f0000, 000fffff], type = 2.
  memory: 07ee0000, [00100000, 07fdffff], type = 1.
  memory: 00020000, [07fe0000, 07ffffff], type = 2.
  memory: 00040000, [fffc0000, ffffffff], type = 2.
```
修改lab2，让其显示` type="some string"` 让人能够读懂，而不是不好理解的数字1,2  (easy) 
- 以上信息，显示的是每个可用内存的大小，起始地址，结束地址，和内存类型，其代码位于pmm.c的page_init函数中。
其中type类型的取指如下：
Values for System Memory Map address type:
01h    memory, available to OS
02h    reserved, not available (e.g. system ROM, memory-mapped device)
03h    ACPI Reclaim Memory (usable by OS after reading ACPI tables)
04h    ACPI NVS Memory (OS is required to save this memory between NVS sessions)
other  not defined yet -- treat as Reserved
对应相应的值进行修改即可。

> 

（4）(spoc)有一台只有页机制的简化80386的32bit计算机，有地址范围位0~256MB的物理内存空间（physical memory），可表示大小为256MB，范围为0xC0000000~0xD0000000的虚拟地址空间（virtual address space）,页大小（page size）为4KB，采用二级页表，一个页目录项（page directory entry ，PDE）大小为4B,一个页表项（page-table entries PTEs）大小为4B，1个页目录表大小为4KB，1个页表大小为4KB。
```
PTE格式（32 bit） :
  PFN19 ... PFN0|NOUSE9 ... NOUSE0|WRITABLE|VALID
PDE格式（32 bit） :
  PT19 ... PT0|NOUSE9 ... NOUSE0|WRITABLE|VALID
 
其中： 
NOUSE9 ... NOUSE0为保留位，要求固定为0
WRITABLE：1表示可写，0表示只读
VLAID：1表示有效，0表示无效
```

假设ucore OS已经为此机器设置好了针对如下虚拟地址<-->物理地址映射的二级页表，设置了页目录基址寄存器（page directory base register，PDBR）保存了页目录表的物理地址（按页对齐），其值为0。已经建立好了从物理地址0x1000~0x41000的二级页表，且页目录表的index为0x300~0x363的页目录项的(PT19 ... PT0)的值=(index-0x300+1)。
请写出一个translation程序（可基于python, ruby, C, C++，LISP等），输入是一个虚拟地址和一个物理地址，能够自动计算出对应的页目录项的index值,页目录项内容的值，页表项的index值，页表项内容的值。即(pde_idx, pde_ctx, pte_idx, pte_cxt)

请用如下值来验证你写的程序的正确性：
一页4K，offset为12位，每个页表4k，每个页表有1024项，共需要页表65536/1024=64个，所以页目录表有64项，即页目录表index需要6位，页表index需要10位
```
va 0xc2265b1f, pa 0x0d8f1b1f

0x2265b1f 

1000   1001100101   101100011111
pde_index = 0x8
pde_ctx = 0x9
pteindex = 0x265
pte_cxt 的地址为37477 
在实体课上讨论后，有些争议，然后就按照理解做。代码见本目录下的va.cpp 是实体课上两人一组写的

va 0xcc386bbc, pa 0x0414cbbc
va 0xc7ed4d57, pa 0x07311d57
va 0xca6cecc0, pa 0x0c9e9cc0
va 0xc18072e8, pa 0x007412e8
va 0xcd5f4b3a, pa 0x06ec9b3a
va 0xcc324c99, pa 0x0008ac99
va 0xc7204e52, pa 0x0b8b6e52
va 0xc3a90293, pa 0x0f1fd293
va 0xce6c3f32, pa 0x007d4f32
```

参考的输出格式为：
```
va 0xcd82c07c, pa 0x0c20907c, pde_idx 0x00000336, pde_ctx  0x00037003, pte_idx 0x0000002c, pte_ctx  0x0000c20b
```

- [x]  

> 

---

## 开放思考题

---

（1）请简要分析Intel的x64 64bit体系结构下的分页机制是如何实现的 
```
  + 采分点：说明Intel x64架构的分页机制的大致特点和页表执行过程
  - 答案没有涉及如下3点；（0分）
  - 正确描述了x64支持的物理内存大小限制（1分）
  - 正确描述了x64下的多级页表的级数和多级页表的结构（2分）
  - 除上述两点外，进一步描述了在多级页表下的虚拟地址-->物理地址的映射过程（3分）
 ```
- [x]  

>  

（2）Intel8086不支持页机制，但有hacker设计过包含未做任何改动的8086CPU的分页系统。猜想一下，hacker是如何做到这一点的？提示：想想MMU的逻辑位置

- [x]  

> 

---
