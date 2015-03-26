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

- 执行提供的代码，得到的输出结果如下：
```
moocos-> gcc -O0 -o 1 1.c
[~]
moocos-> time ./1
10485760 count computing over!

real	0m0.155s
user	0m0.041s
sys	0m0.008s
[~]

```
- 改变访问顺序为A[j][i] 得到的结果为：
```
moocos-> gcc -O0 -o 1 1.c
[~]
moocos-> time ./1
10485760 count computing over!

real	0m0.514s
user	0m0.220s
sys	0m0.018s
[~]

```
- 可以看到两者时间差别比较大。

## 小组思考题目
----

（1）缺页异常可用于虚拟内存管理中。如果在中断服务例程中进行缺页异常的处理时，再次出现缺页异常，这时计算机系统（软件或硬件）会如何处理？请给出你的合理设计和解释。

（2）如果80386机器的一条机器指令(指字长4个字节)，其功能是把一个32位字的数据装入寄存器，指令本身包含了要装入的字所在的32位地址。这个过程最多会引起几次缺页中断？

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
PFN6..0:页帧号
PT6..0:页表的物理基址>>5
```

已经建立好了1个页目录表和8个页表，且页目录表的index为0~7的页目录项分别对应了这8个页表。

在[物理内存模拟数据文件](./04-1-spoc-memdiskdata.md)中，给出了4KB物理内存空间和4KBdisk空间的值，PDBR的值。

请回答下列虚地址是否有合法对应的物理内存，请给出对应的pde index, pde contents, pte index, pte contents，the value of addr in phy page OR disk sector。
```
Virtual Address 6653:
Virtual Address 1c13:
Virtual Address 6890:
Virtual Address 0af6:
Virtual Address 1e6f:
```

回答可参考以如下表示：
```
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
- 执行代码：
```
#include <fstream>
using namespace std;

int main() {
    ifstream fin("page");
    char tmp[10];
    unsigned int page[4096], disk[4096];
    for (int i = 0; i < 128; i ++) {
        fin >> tmp;
        fin >> tmp;
        for (int j = 0; j < 32; j ++)
            fin >> hex >> page[i * 32 + j];
    }
    fin.close();
    fin.open("disk");
    for (int i = 0; i < 128; i ++) {
        fin >> tmp;
        fin >> tmp;
        for (int j = 0; j < 32; j ++)
            fin >> hex >> disk[i * 32 + j];
    }
    fin.close();
    fin.open("input.txt");
    ofstream fout("output.txt");
    const unsigned int PDBR = 0xd80;
    unsigned int va, pde_idx, pde_ctt, pte_idx, pte_ctt, pa;
    while (fin >> hex >> va) {
        fout << "Virtual Address " << hex << va << ":\n";
        pde_idx = (va >> 10);
        pde_ctt = page[PDBR + pde_idx];
        fout << "  --> pde index:0x" << hex << pde_idx << "  pde contents:(valid " 
            << (pde_ctt >> 7) << ", pfn 0x" << hex << (pde_ctt & 0x7f) << ")\n";
        if (pde_ctt >> 7) {
            pte_idx = (va >> 5) & 0x1f;
            pte_ctt = page[((pde_ctt & 0x7f) << 5) + pte_idx];
            fout << "    --> pte index:0x" << hex << pte_idx << "  pte contents:(valid "
                << (pte_ctt >> 7) << ", pfn 0x" << hex << (pte_ctt & 0x7f) << ")\n";
            pa = ((pte_ctt & 0x7f) << 5) + (va & 0x1f);
            if (pte_ctt >> 7) {
                fout << "      --> To Physical Address 0x" << hex << pa
                    << " --> Value: " << hex << page[pa] << endl;
            } else {
                fout << "      --> To Disk Sector Address 0x" << hex << pa
                    << " --> Value: " << hex << disk[pa] << endl;
            }        
        } else {
            fout << "    --> Fault (page directory entry not valid)\n";
        }       
        fout << endl;
    }    
    fin.close();
    fout.close();
    return 0;   
}  
```
- 运行结果：
```
Virtual Address 6653:
  --> pde index:0x19  pde contents:(valid 0, pfn 0x7f)
    --> Fault (page directory entry not valid)

Virtual Address 1c13:
  --> pde index:0x7  pde contents:(valid 1, pfn 0x3d)
    --> pte index:0x0  pte contents:(valid 1, pfn 0x76)
      --> To Physical Address 0xed3 --> Value: 12

Virtual Address 6890:
  --> pde index:0x1a  pde contents:(valid 0, pfn 0x7f)
    --> Fault (page directory entry not valid)

Virtual Address af6:
  --> pde index:0x2  pde contents:(valid 1, pfn 0x21)
    --> pte index:0x17  pte contents:(valid 0, pfn 0x7f)
      --> To Disk Sector Address 0xff6 --> Value: 3

Virtual Address 1e6f:
  --> pde index:0x7  pde contents:(valid 1, pfn 0x3d)
    --> pte index:0x13  pte contents:(valid 0, pfn 0x16)
      --> To Disk Sector Address 0x2cf --> Value: 1c

```

## 扩展思考题
---
(1)请分析原理课的缺页异常的处理流程与lab3中的缺页异常的处理流程（分析粒度到函数级别）的异同之处。
