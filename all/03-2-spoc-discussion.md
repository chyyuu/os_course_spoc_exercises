# lec6 SPOC思考题


NOTICE
- 有"w3l2"标记的题是助教要提交到学堂在线上的。
- 有"w3l2"和"spoc"标记的题是要求拿清华学分的同学要在实体课上完成，并按时提交到学生对应的git repo上。
- 有"hard"标记的题有一定难度，鼓励实现。
- 有"easy"标记的题很容易实现，鼓励实现。
- 有"midd"标记的题是一般水平，鼓励实现。


## 个人思考题
---

（1） (w3l2) 请简要分析64bit CPU体系结构下的分页机制是如何实现的
        理论上64位CPU应该支持2^64即16EB的内存寻址，但实际上存在着人为的内存限制。根据查到到资料，X86-64和AMD的64位CPU，其地址总线都是40位的，也就是实际上只支持2^40的内存寻址空间。
        虽然无法达到16EB，但是40位的寻址空间也足够大。80386 32位采用的两级页表明显不够。
        在X86-64体系结构下，物理地址空间实际使用40位，线性地址空间64位，但是只低用48位做页式地址转换，高16位填充与按第47位做符号扩展。因为实际上用不了64为，如果还用64位做页式转换的话，页表浪费的内存就会很大。
        采用64位地址空间的x86-86被称为是运行在“长模式”下，该模式可以看成是对PAE模式的一个扩充。长模式允许的物理页帧大小可以为4K/2M或者是1G。长模式下线性地址到物理地址映射采用4级地址映射。
        以4K大小的页帧为例，线性地址中各个位如下： 　　
        0－11位：页内偏移offset　　
       12－20位：Table 　　
       21－29位：Directory
       30－38位：Directory Ptr
       39－47位：PML4
       48 - 63位：符号扩展位
       其中映射过程如下：
       PML4为Page-Map Level 4 Table，是64为体系结构中新增的以及页面（之前为三级页表） 
       1. 由PML4查找PML4表项，指向PDP（Page-Directory Pointer Table）
       2. 有Directory Ptr在PDP中查找Dir.Pointer Entry，指向PDE（Page-Directory）
       3. 由Directory在PDE中查找PTE（Page-Table）
       4. 有Table在PTE中获取物理页帧号
       5. 物理页帧号加上offset即为物理地址
       　　　　

  + 采分点：说明64bit CPU架构的分页机制的大致特点和页表执行过程
  - 答案没有涉及如下3点；（0分）
  - 正确描述了64bit CPU支持的物理内存大小限制（1分）
  - 正确描述了64bit CPU下的多级页表的级数和多级页表的结构或反置页表的结构（2分）
  - 除上述两点外，进一步描述了在多级页表或反置页表下的虚拟地址-->物理地址的映射过程（3分）
 ```
- [x]  

>  

## 小组思考题
---

（1）(spoc) 某系统使用请求分页存储管理，若页在内存中，满足一个内存请求需要150ns。若缺页率是10%，为使有效访问时间达到0.5ms,求不在内存的页面的平均访问时间。请给出计算步骤。 
解： 设不在内存的平均访问时间是xms
       缺页率10%，即有10%的访问不在内存中。有效访问时间为0.5ms
       则 0.5 = 0.00015*0.9 + x*0.1
       解得x=4.998 ms
- [x]  

（2）(spoc) 有一台假想的计算机，页大小（page size）为32 Bytes，支持32KB的虚拟地址空间（virtual address space）,有4KB的物理内存空间（physical memory），采用二级页表，一个页目录项（page directory entry ，PDE）大小为1 Byte,一个页表项（page-table entries
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
VALID==1表示，表示映射存在；VALID==0表示，表示映射不存在。
PFN6..0:页帧号
#PT6..0:页表的物理基址>>5#   按页大小对齐，所以只用7位就可以，后5位可以不用存
```
在[物理内存模拟数据文件](./03-2-spoc-testdata.md)中，给出了4KB物理内存空间的值，请回答下列虚地址是否有合法对应的物理内存，请给出对应的pde index, pde contents, pte index, pte contents。

虚地址15位 实地址12位 页表号为5位 页表目录表项号为5位
15位的虚地址各个位表示如下：
0~4 offset
5~9 PTE
10~14 PDE

PDBR值为544（十进制） 即第17页
物理页号pfn为7位

手算比较麻烦，写了段程序来做，代码见MMU.cpp

```
Virtual Address 6c74
11011 00011 10100
pde index: 0x1b  pde  contentes:(valid 1 , pt  0x20)
pte index: 0x03  pte  contentes:(valid 1 , pfn  0x61)
pa: 3124 value:6

Virtual Address 6b22
11010 11001 00010
pde index: 0x1a  pde  contentes:(valid 1 , pt  0x52)
pte index: 0x19  pte  contentes:(valid 1 , pfn  0x47)
pa: 2274 value:26


Virtual Address 03df
00000 11110 11111
pde index: 0x00  pde  contentes:(valid 1 , pt  0x5a)
pte index: 0x1e  pte  contentes:(valid 1 , pfn  0x05)
pa: 191 value:15

      
Virtual Address 69dc
11010 01110 11100
pde index: 0x1a  pde  contentes:(valid 1 , pt  0x52)
pte index: 0x0e  pte  contentes:(valid 0 , pfn  0x7f)
Fault (page table entry not valid)


Virtual Address 317a
01100 01011 11010
pde index: 0x0c  pde  contentes:(valid 1 , pt  0x18)
pte index: 0x0b  pte  contentes:(valid 1 , pfn  0x35)
pa: 1722 value:30


Virtual Address 4546
10001 01010 00110
pde index: 0x11  pde  contentes:(valid 1 , pt  0x21)
pte index: 0x0a  pte  contentes:(valid 0 , pfn  0x7f)
Fault (page table entry not valid)

      
Virtual Address 2c03
01011 00000 00011
pde index: 0x0b  pde  contentes:(valid 1 , pt  0x44)
pte index: 0x00  pte  contentes:(valid 1 , pfn  0x57)
pa: 2787 value:22

Virtual Address 7fd7
11111 11110 10111
pde index: 0x1f  pde  contentes:(valid 1 , pt  0x12)
pte index: 0x1e  pte  contentes:(valid 0 , pfn  0x7f)
Fault (page table entry not valid)


Virtual Address 390e
01110 01000 01110
pde index: 0x0e  pde  contentes:(valid 0 , pt  0x7f)
Fault (page directory entry not valid)

Virtual Address 748b
11101 00100 01011
pde index: 0x1d  pde  contentes:(valid 1 , pt  0x00)
pte index: 0x04  pte  contentes:(valid 0 , pfn  0x7f)
Fault (page table entry not valid)

```



（3）请基于你对原理课二级页表的理解，并参考Lab2建页表的过程，设计一个应用程序（可基于python, ruby, C, C++，LISP等）可模拟实现(2)题中描述的抽象OS，可正确完成二级页表转换。


（4）假设你有一台支持[反置页表](http://en.wikipedia.org/wiki/Page_table#Inverted_page_table)的机器，请问你如何设计操作系统支持这种类型计算机？请给出设计方案。


--- 

## 扩展思考题

阅读64bit IBM Powerpc CPU架构是如何实现[反置页表](http://en.wikipedia.org/wiki/Page_table#Inverted_page_table)，给出分析报告。

--- 
