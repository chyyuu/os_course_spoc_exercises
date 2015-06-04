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

```
  对于64位系统来说，可以使用的地址空间位2^64，这个空间很大。
  目前只采用了48位的地址，但是仍然比较大，所以采用多级页表来进行内存管理。
  ia64/ppc64/alpha使用3级页表，x86_64使用4级页表。
  在多级页表下，得到当前的虚拟地址后，从中得到每一级页表查询的偏置量。
  第一级页表的地址由PDBR和偏置相加得到，从中查询到第二级页表的起始地址，和第二个偏置相加得到第二级页表的表项。
  最后一级页表中的内容移位后和偏移相加得到最终的物理地址。
  在64位系统中也通常会使用反置页表，每个物理页帧对应一个表项。
  当得到虚拟地址后，通过计算hash值找到页表中的表项，并和所有冲突项中的虚拟地址对比，如果相同则得到对应物理地址。
 ```
<<<<<<< HEAD
- 64位系统的地址空间有2^64，算是比较大的空间。目前使用48位地址，虽然有所减少，但仍然是很大的空间，采用多级也表进行管理。ia64/ppc64/alpha使用三级页表，x86_64使用四级。
- 多级页表下，得到虚地址后查找页表的偏置量，第一级页表的地址由PDBR和偏置相加得到，并由此查询第二级页表的起始地址。与第二级偏置相加即为第二级的表项。最后一级页表的内容移位后和偏移相加即是最终的物理地址。
- 反置页表也是经常被使用的，每个物理页帧对应一个表项。得到虚地址后通过哈希函数找到表项，并和冲突项中所有的虚拟地址对比，相同则得到对应的物理地址。 

>  
=======
>>>>>>> 790bc39b948c60181c85f58654a11682d075cbe2

## 小组思考题
---

（1）(spoc) 某系统使用请求分页存储管理，若页在内存中，满足一个内存请求需要150ns (10^-9s)。若缺页率是10%，为使有效访问时间达到0.5us(10^-6s),求不在内存的页面的平均访问时间。请给出计算步骤。 

<<<<<<< HEAD
- 500 = 0.9 * 150 + 0.1*x  
- x = 3650ns = 3.65us
=======
```
  500 = 0.9 * 150 + 0.1 * x
  x = 3650 ns = 3.65 us
```
>>>>>>> 790bc39b948c60181c85f58654a11682d075cbe2

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
<<<<<<< HEAD
```
#include <iostream>
#include <fstream>
#include <sstream>
int memo[4096];//4KB内存
uint32_t get_page(uint32_t v_addr)
{
uint32_t ans = 0;
uint32_t pde = v_addr & 0x00007c00;
uint32_t pte = v_addr & 0x000003e0;
return ans;
}
using namespace std;
int main(int argc, const char * argv[]) {
ifstream fin("1.txt");
char s[100];
int c = 0, num;
while (fin >> s) {
if (strcmp(s, "page") == 0) {
fin >> s;
continue;
}
sscanf(s, "%x", &num);
memo[c] = num;
c++;
}
int x;
while(true)
{
scanf("%x", &x);
int offset = x %32;
x = x>>5;
int  pte = x %32;
x = x>>5;
int pde = x %32;
int pdbr = 544;
int pde_2 = pdbr + pde;
int valid1 = memo[pde_2] >> 7;
if(valid1 == 0)
{
printf("Fault (page directory entry not valid)\n");
continue;
}
else
{
int pter = memo[pde_2] % (1<<7);
printf("pde index : %x pde contents :(valid %d, pfn %x)\n",pde,valid1,pter);
int pte_2 = (pter << 5) + pte;
int pframe = memo[pte_2];
int valid2 = pframe >> 7;
if(valid2 ==0)
{
printf("Fault (page table entry not valid)\n");       
continue;  
}
else
{
printf("pte index : %x pte contents :(valid %d, pfn %x)\n",pte,valid2,pframe %(1<<7));
int pp = ((pframe %(1<<7)) << 5) + offset;
printf("%x\n",memo[pp]);
}
}
}
return 0;
}
```
```
Virtual Address 6c74:
--> pde index:0x1b  pde contents:(valid 1, pfn 0x20)
--> pte index:0x03  pte contents:(valid 1, pfn 0x61)
--> Translates to Physical Address 0x0c34 --> Value: 0x06

Virtual Address 6b22:
--> pde index:0x1a  pde contents:(valid 1, pfn 0x52)
--> pte index:0x19  pte contents:(valid 1, pfn 0x47)
--> Translates to Physical Address 0x08e2 --> Value: 0x1a

Virtual Address 03df:
--> pde index:0x00  pde contents:(valid 1, pfn 0x5a)
--> pte index:0x1e  pte contents:(valid 1, pfn 0x05)
--> Translates to Physical Address 0x00bf --> Value: 0x0f

Virtual Address 69dc:
--> pde index:0x1a  pde contents:(valid 1, pfn 0x52)
--> pte index:0x0e  pte contents:(valid 0, pfn 0x7f)
--> Fault (page table entry not valid)

Virtual Address 317a:
--> pde index:0x0c  pde contents:(valid 1, pfn 0x18)
--> pte index:0x0b  pte contents:(valid 1, pfn 0x35)
--> Translates to Physical Address 0x06ba --> Value: 0x1e

Virtual Address 4546:
--> pde index:0x1b  pde contents:(valid 1, pfn 0x21)
--> pte index:0xb  pte contents:(valid 0, pfn 0x7f)
--> Fault (page table entry not valid)

Virtual Address 2c03:
--> pde index:0x11  pde contents:(valid 1, pfn 0x44)
--> pte index:0x0a  pte contents:(valid 1, pfn 0x57)
--> Translates to Physical Address 0x0ae3 --> Value: 0x16

Virtual Address 7fd7:
--> pde index:0x1f  pde contents:(valid 1, pfn 0x12)
--> pte index:0x1e  pte contents:(valid 0, pfn 0x7f)
--> Fault (page table entry not valid)

Virtual Address 390e:
--> pde index:0x0e  pde contents:(valid 0, pfn 0x7f)
--> Fault (page table entry not valid)

Virtual Address 748b:
--> pde index:0x1d  pde contents:(valid 1, pfn 0x00)
--> pte index:0x04  pte contents:(valid 0, pfn 0x7f)
--> Fault (page table entry not valid)
```
=======


```
运行如下程序：

#include <fstream>
#include <string>
#include <iostream>
#include <iomanip>

using namespace std;

int main() {
 	ifstream fin("memory.txt");
    string tmp;
    int mem[4096];
    for (int i = 0; i < 128; i ++) {
        fin >> tmp;
        fin >> tmp;
        for (int j = 0; j < 32; j ++)
         	fin >> hex >> mem[i * 32 + j];
 	}
 	fin.close();
 	fin.open("input.txt");
 	ofstream fout("output.txt");
  	const int PDBR = 544;
 	int va, pa, pde, pte, off0, off1, off2;
 	for (int i = 0; i < 10; i ++) {
 	    fout << "Virtual Address ";
 	    fin >> hex >> va;
		fout << setfill('0') << setw(4) << hex << va << ":" << endl;
  		off0 = va >> 10, off1 = (va & 0x3e0) >> 5, off2 = va & 0x1f;
  		pde = mem[PDBR + off0];
		fout << "  --> pde index:0x" << hex << off0 << " pde contents:(valid " 
	 		<< (pde >> 7) << ", pfn 0x" << (pde & 0x7f) << ")" << endl;
		if (pde >> 7) {
  			pte = mem[((pde & 0x7f) << 5) + off1];
			fout << "    --> pte index:0x" << hex << off1 << "  pte contents:(valid " 
   				<< (pte >> 7) << ", pfn 0x" << (pte & 0x7f) << ")" << endl;
			if (pte >> 7) {
			    pa = ((pte & 0x7f) << 5) + off2;
			    fout << "      --> Translates to Physical Address 0x" << hex << pa 
       				<< " --> Value: " << mem[pa] << endl;
			} else fout << "      --> Fault (page table entry not valid)" << endl;
		} else fout << "      --> Fault (page directory entry not valid)" << endl;
		fout << endl;
  	}
  	fin.close();
  	fout.close();
  	return 0;           
}    

得到如下结果：

Virtual Address 6c74:
  --> pde index:0x1b pde contents:(valid 1, pfn 0x20)
    --> pte index:0x3  pte contents:(valid 1, pfn 0x61)
      --> Translates to Physical Address 0xc34 --> Value: 6

Virtual Address 6b22:
  --> pde index:0x1a pde contents:(valid 1, pfn 0x52)
    --> pte index:0x19  pte contents:(valid 1, pfn 0x47)
      --> Translates to Physical Address 0x8e2 --> Value: 1a

Virtual Address 03df:
  --> pde index:0x0 pde contents:(valid 1, pfn 0x5a)
    --> pte index:0x1e  pte contents:(valid 1, pfn 0x5)
      --> Translates to Physical Address 0xbf --> Value: f

Virtual Address 69dc:
  --> pde index:0x1a pde contents:(valid 1, pfn 0x52)
    --> pte index:0xe  pte contents:(valid 0, pfn 0x7f)
      --> Fault (page table entry not valid)

Virtual Address 317a:
  --> pde index:0xc pde contents:(valid 1, pfn 0x18)
    --> pte index:0xb  pte contents:(valid 1, pfn 0x35)
      --> Translates to Physical Address 0x6ba --> Value: 1e

Virtual Address 4546:
  --> pde index:0x11 pde contents:(valid 1, pfn 0x21)
    --> pte index:0xa  pte contents:(valid 0, pfn 0x7f)
      --> Fault (page table entry not valid)

Virtual Address 2c03:
  --> pde index:0xb pde contents:(valid 1, pfn 0x44)
    --> pte index:0x0  pte contents:(valid 1, pfn 0x57)
      --> Translates to Physical Address 0xae3 --> Value: 16

Virtual Address 7fd7:
  --> pde index:0x1f pde contents:(valid 1, pfn 0x12)
    --> pte index:0x1e  pte contents:(valid 0, pfn 0x7f)
      --> Fault (page table entry not valid)

Virtual Address 390e:
  --> pde index:0xe pde contents:(valid 0, pfn 0x7f)
      --> Fault (page directory entry not valid)

Virtual Address 748b:
  --> pde index:0x1d pde contents:(valid 1, pfn 0x0)
    --> pte index:0x4  pte contents:(valid 0, pfn 0x7f)
      --> Fault (page table entry not valid)
>>>>>>> 790bc39b948c60181c85f58654a11682d075cbe2

```


（3）请基于你对原理课二级页表的理解，并参考Lab2建页表的过程，设计一个应用程序（可基于python, ruby, C, C++，LISP等）可模拟实现(2)题中描述的抽象OS，可正确完成二级页表转换。

（4）假设你有一台支持[反置页表](http://en.wikipedia.org/wiki/Page_table#Inverted_page_table)的机器，请问你如何设计操作系统支持这种类型计算机？请给出设计方案。
- 建立反置列表，通过页帧对应页标号；查找时利用哈希加速，建立hashlist，方便产生冲突时确定正确地页帧。

（5）[X86的页面结构](http://os.cs.tsinghua.edu.cn/oscourse/OS2015/lecture06#head-1f58ea81c046bd27b196ea2c366d0a2063b304ab)
--- 

## 扩展思考题

阅读64bit IBM Powerpc CPU架构是如何实现[反置页表](http://en.wikipedia.org/wiki/Page_table#Inverted_page_table)，给出分析报告。

--- 
