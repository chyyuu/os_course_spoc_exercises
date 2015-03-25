#lec8 虚拟内存spoc练习


NOTICE
- 有"w4l2"标记的题是助教要提交到学堂在线上的。
- 有"w4l2"和"spoc"标记的题是要求拿清华学分的同学要在实体课上完成，并按时提交到学生对应的git repo上。
- 有"hard"标记的题有一定难度，鼓励实现。
- 有"easy"标记的题很容易实现，鼓励实现。
- 有"midd"标记的题是一般水平，鼓励实现。


## 个人思考题
---
(1)(w4l2)下面是一个体现内存访问局部性好的简单应用程序例子，请参考，在linux中写一个简单应用程序，体现内存局部性差，并给出其执行时间。
```
#include <stdio.h>
#define NUM 1024
#define COUNT 10
int A[NUM][NUM];
void main (void) {
  int i,j,k;
  for (k = 0; k<COUNT; k++)
  for (i = 0; i < NUM; i++)
  for (j = 0; j	 < NUM; j++)
      A[i][j] = i+j;
  printf("%d count computing over!\n",i*j*k);
}
```
可以用下的命令来编译和运行此程序：
```
gcc -O0 -o goodlocality goodlocality.c
time ./goodlocality
```
可以看到其执行时间。

编译运行上述程序：
real	0m0.046s
user	0m0.046s
sys	0m0.000s

可以看到其执行时间为0.046s

对上述程序做简单修改如下，得到局部性不好的一段程序：
#include <stdio.h>
#define NUM 1024
#define COUNT 10
int A[NUM][NUM];
void main (void) {
  int i,j,k;
  for (k = 0; k<COUNT; k++)
  for (i = 0; i < NUM; i++)
  for (j = 0; j	 < NUM; j++)
      A[j][i] = i+j;
  printf("%d count computing over!\n",i*j*k);
}

编译运行，
real	0m0.180s
user	0m0.180s
sys	0m0.000s

其运行时间为0.18s，是局部性好的程序的4倍。
这是因为，存储的数组实际上是一个1024*1024的矩阵，而计算机一般情况下是按行顺序来存的。标准的X86-32体系下，一页大小为4K，一个int为4B，即上述二维数组大小为1024页。第一段程序按照行来累加，这样最多缺页1024次，第二段程序按照列累加，这样最多缺页1024*1024次。

## 小组思考题目
----

（1）缺页异常可用于虚拟内存管理中。如果在中断服务例程中进行缺页异常的处理时，再次出现缺页异常，这时计算机系统（软件或硬件）会如何处理？请给出你的合理设计和解释。
可以考虑允许异常嵌套。即在缺页异常处理程序时再发生异常，则嵌套递归执行。但是操作系统得确保嵌套的异常处理程序不会缺失同一个页，否则会死循环。
实际上，中断服务例程一般是常住内存的，否则放在外面出问题则会宕机。

（2）如果80386机器的一条机器指令(指字长4个字节)，其功能是把一个32位字的数据装入寄存器，指令本身包含了要装入的字所在的32位地址。这个过程最多会引起几次缺页中断？
如果该指令存在内存中，按照$pc寄存其中的指令地址去取指，可能会有一次页缺失
取到指令后，读取内存数据时，也可能会有一次页缺失。
如果指令或数据的32字节横跨了两个page，那就会发生多次页缺失，则最多可能四次。
再考虑页表缺失，最多可能出现6次。


（3）(spoc) 有一台假想的计算机，页大小（page size）为32 Bytes，支持8KB的虚拟地址空间（virtual address space）,有4KB的物理内存空间（physical memory），采用二级页表，一个页目录项（page directory entry ，PDE）大小为1 Byte,一个页表项（page-table entries
PTEs）大小为1 Byte，1个页目录表大小为32 Bytes，1个页表大小为32 Bytes。页目录基址寄存器（page directory base register，PDBR）保存了页目录表的物理地址（按页对齐）。

PTE格式（8 bit） :
```
  VALID | PFN6 ... PFN0
```
PDE格式（8 bit） :
```
  VALID | PT6 ... PT0
```
其
```
VALID==1表示，表示映射存在；VALID==0表示，表示内存映射不存在（有两种情况：a.对应的物理页帧swap out在硬盘上；b.既没有在内存中，页没有在硬盘上）。
PFN6..0:页帧号或外存中的后备页号
PT6..0:页表的物理基址>>5

虚地址13位
实地址12位
offset 5位
页表项index 5位
页目录表项index 5位
但实际上，本题中只有八个页表，所以只需要3位
PDBR content: 0xd80
```

已经建立好了1个页目录表和8个页表，且页目录表的index为0~7的页目录项分别对应了这8个页表。

在[物理内存模拟数据文件](./04-1-spoc-memdiskdata.md)中，给出了4KB物理内存空间和4KBdisk空间的值，PDBR的值。

请回答下列虚地址是否有合法对应的物理内存，请给出对应的pde index, pde contents, pte index, pte contents，the value of addr in phy page OR disk sector。
```
程序见mmu2.cpp

Virtual Address 6653:
pde index: 0x19  pde  contentes:(valid 0 , pt  0x7f)
Fault (page directory entry not valid)

Virtual Address 1c13:
pde index: 0x07  pde  contentes:(valid 1 , pt  0x3d)
pte index: 0x00  pte  contentes:(valid 1 , pfn  0x76)
Translates to pa: 3795 value:18

Virtual Address 6890:
pde index: 0x1a  pde  contentes:(valid 0 , pt  0x7f)
Fault (page directory entry not valid)

Virtual Address 0af6:
pde index: 0x02  pde  contentes:(valid 1 , pt  0x21)
pte index: 0x13  pte  contentes:(valid 0 , pfn  0x7f)
Fault (page table entry not valid) the pa is:4079


Virtual Address 1e6f:
pde index: 0x07  pde  contentes:(valid 1 , pt  0x3d)
pte index: 0x13  pte  contentes:(valid 0 , pfn  0x16)
To Disk Sector Address : 719 value:23

```

回答可参考以如下表示：
```
11101 01011 10000
Virtual Address 7570:
  --> pde index:0x1d  pde contents:(valid 1, pfn 0x33)
    --> pte index:0xb  pte contents:(valid 0, pfn 0x7f)
      --> Fault (page table entry not valid)
      
Virtual Address 21e1:
  --> pde index:0x8  pde contents:(valid 0, pfn 0x7f)
      --> Fault (page directory entry not valid)

Virtual Address 7268:
  --> pde index:0x1c  pde contents:(valid 1, pfn 0x5e)
    --> pte index:0x13  pte contents:(valid 1, pfn 0x65)
      --> Translates to Physical Address 0xca8 --> Value: 16

Virtual Address 106f:
  --> pde index:0x3  pde contents:(valid 1, pfn 0x2d)
    --> pte index:0x14  pte contents:(valid 0, pfn 0x06)
      --> To Disk Sector Address 0x167 --> Value: 2c
```

## 扩展思考题
---
(1)请分析原理课的缺页异常的处理流程与lab3中的缺页异常的处理流程（分析粒度到函数级别）的异同之处。
