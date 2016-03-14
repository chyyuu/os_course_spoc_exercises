# lec6 SPOC思考题


## 小组思考题
---

（1）(spoc) 某系统使用请求分页存储管理，若页在内存中，满足一个内存请求需要150ns (10^-9s)。若缺页率是10%，为使有效访问时间达到0.5us(10^-6s),求不在内存的页面的平均访问时间。请给出计算步骤。 

- [x]  

> 500=0.9\*150+0.1\*x

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
PT6..0:页表的物理基址>>5
```
在[物理内存模拟数据文件](./03-2-spoc-testdata.md)中，给出了4KB物理内存空间的值，请回答下列虚地址是否有合法对应的物理内存，请给出对应的pde index, pde contents, pte index, pte contents。

```
Virtual Address 6c74
Virtual Address 6b22
Virtual Address 03df
Virtual Address 69dc
Virtual Address 317a
Virtual Address 4546
Virtual Address 2c03
Virtual Address 7fd7
Virtual Address 390e
Virtual Address 748b
```

**[回答]**

前两个虚地址的转换结果：

```
Virtual Address 0x6c74:
	--> pde index: 0x1b pde contents: (valid 0x1, pt 0x20)
		--> pte index: 0x3 pte contents: (valid 0x1, pfn 0x61)
			--> Translated to Physical Address 0xc34 --> Value: 0x6
Virtual Address 0x6b22:
	--> pde index: 0x1a pde contents: (valid 0x1, pt 0x52)
		--> pte index: 0x19 pte contents: (valid 0x1, pfn 0x47)
			--> Translated to Physical Address 0x8e2 --> Value: 0x1a

```


（3）请基于你对原理课二级页表的理解，并参考Lab2建页表的过程，设计一个应用程序（可基于python, ruby, C, C++，LISP等）可模拟实现(2)题中描述的抽象OS，可正确完成二级页表转换。

**[回答]**
代码请见[convert.py](convert.py)

程序运行结果

```
Virtual Address 0x6c74:
	--> pde index: 0x1b pde contents: (valid 0x1, pt 0x20)
		--> pte index: 0x3 pte contents: (valid 0x1, pfn 0x61)
			--> Translated to Physical Address 0xc34 --> Value: 0x6
Virtual Address 0x6b22:
	--> pde index: 0x1a pde contents: (valid 0x1, pt 0x52)
		--> pte index: 0x19 pte contents: (valid 0x1, pfn 0x47)
			--> Translated to Physical Address 0x8e2 --> Value: 0x1a
Virtual Address 0x3df:
	--> pde index: 0x0 pde contents: (valid 0x1, pt 0x5a)
		--> pte index: 0x1e pte contents: (valid 0x1, pfn 0x5)
			--> Translated to Physical Address 0xbf --> Value: 0xf
Virtual Address 0x69dc:
	--> pde index: 0x1a pde contents: (valid 0x1, pt 0x52)
		--> pte index: 0xe pte contents: (valid 0x0, pfn 0x7f)
			--> Fault (page table entry not valid)
Virtual Address 0x317a:
	--> pde index: 0xc pde contents: (valid 0x1, pt 0x18)
		--> pte index: 0xb pte contents: (valid 0x1, pfn 0x35)
			--> Translated to Physical Address 0x6ba --> Value: 0x1e
Virtual Address 0x4546:
	--> pde index: 0x11 pde contents: (valid 0x1, pt 0x21)
		--> pte index: 0xa pte contents: (valid 0x0, pfn 0x7f)
			--> Fault (page table entry not valid)
Virtual Address 0x2c03:
	--> pde index: 0xb pde contents: (valid 0x1, pt 0x44)
		--> pte index: 0x0 pte contents: (valid 0x1, pfn 0x57)
			--> Translated to Physical Address 0xae3 --> Value: 0x16
Virtual Address 0x7fd7:
	--> pde index: 0x1f pde contents: (valid 0x1, pt 0x12)
		--> pte index: 0x1e pte contents: (valid 0x0, pfn 0x7f)
			--> Fault (page table entry not valid)
Virtual Address 0x390e:
	--> pde index: 0xe pde contents: (valid 0x0, pt 0x7f)
		--> Fault (page directory entry not valid)
Virtual Address 0x748b:
	--> pde index: 0x1d pde contents: (valid 0x1, pt 0x0)
		--> pte index: 0x4 pte contents: (valid 0x0, pfn 0x7f)
			--> Fault (page table entry not valid)
```


（4）假设你有一台支持[反置页表](http://en.wikipedia.org/wiki/Page_table#Inverted_page_table)的机器，请问你如何设计操作系统支持这种类型计算机？请给出设计方案。

 (5)[X86的页面结构](http://os.cs.tsinghua.edu.cn/oscourse/OS2015/lecture06#head-1f58ea81c046bd27b196ea2c366d0a2063b304ab)
