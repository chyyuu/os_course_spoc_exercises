# lec5 SPOC思考题


NOTICE
- 有"w3l1"标记的题是助教要提交到学堂在线上的。
- 有"w3l1"和"spoc"标记的题是要求拿清华学分的同学要在实体课上完成，并按时提交到学生对应的git repo上。
- 有"hard"标记的题有一定难度，鼓励实现。
- 有"easy"标记的题很容易实现，鼓励实现。
- 有"midd"标记的题是一般水平，鼓励实现。


## 个人思考题
---

请简要分析最优匹配，最差匹配，最先匹配，buddy systemm分配算法的优势和劣势，并尝试提出一种更有效的连续内存分配算法 (w3l1)
```
  + 采分点：说明四种算法的优点和缺点
  - 答案没有涉及如下3点；（0分）
  - 正确描述了二种分配算法的优势和劣势（1分）
  - 正确描述了四种分配算法的优势和劣势（2分）
  - 除上述两点外，进一步描述了一种更有效的分配算法（3分）
 ```
- 最优匹配：优点：大部分分配的尺寸很小时效果比较好；缺点：产生很多外部碎片和无用的小碎片，释放分区时比较缓慢
- 最差匹配：优点：中等大小分配较多时效果比较好，可以有效避免产生很多小碎片；缺点：释放分区比较慢，容易产生外部碎片，容易破坏比较大的分区空间，因此后续难以分配比较大的分区。
- 最先匹配：优点：简单，在高地址有大块空闲分区；缺点：容易产生外部碎片，分配大块时比较缓慢
- buddy system分配算法：优点：快速合并，低外部碎片；缺点：内部碎片。
- 循环首次适应算法。该算法是由首次适应算法演变而成的。在为进程分配内存空间时，不再每次从链首开始查找，而是从上次找到的空闲分区开始查找，直至找到一个能满足需求的空闲分区，并从中划出一块来分给作业。该算法能使空闲中的内存分区分布得更加均匀，但将会缺乏大的空闲分区。

>  

## 小组思考题

请参考ucore lab2代码，采用`struct pmm_manager` 根据你的`学号 mod 4`的结果值，选择四种（0:最优匹配，1:最差匹配，2:最先匹配，3:buddy systemm）分配算法中的一种或多种，在应用程序层面(可以 用python,ruby,C++，C，LISP等高语言)来实现，给出你的设思路，并给出测试用例。 (spoc)

```
//学号2012011357 选择方式1：最差匹配
#include <vector>
#include <cstdlib>
#include <algorithm>
#define MEM_SIZE (1<<12)
using namespace std;
struct block { 
    char * base;
    size_t size;
    bool free;
    block(char * _base, size_t _size) {
        base = _base, size = _size, free = true;
    }        
};
bool size_cmp(const block& a, const block& b) { 
    return a.size > b.size;
}
bool address_cmp(const block& a, const block& b) { 
    return a.base <i b.base;
}
class pmm_manager {
    char mem[MEM_SIZE];
    size_t nr_free;
    vector<block> blocks;
public:
    void init() {
        nr_free = 0;
        block tblock(mem, MEM_SIZE);
        blocks.push_back(tblock);
    }                                                   
    char * alloc_pages(size_t n) { 
        char * p;
        for (int i = 0; i < blocks.size(); i ++) { 
            if (!blocks[i].free) continue;
            if (blocks[i].size == n) { 
                blocks[i].free = false;
                p = blocks[i].base;
                nr_free -= n;
                return p;
            } else if (blocks[i].size > n) {
                block tblock(blocks[i].base + n, blocks[i].size - n);
                blocks.push_back(tblock);
                blocks[i].free = false;
                blocks[i].size = n;
                p = blocks[i].base;
                sort(blocks.begin(), blocks.end(), size_cmp);
                nr_free -= n;
                return p;
            }
        }
        return NULL;
    }
    void free_pages(char * base, size_t n) { 
        sort(blocks.begin(), blocks.end(), address_cmp);
        int index, before = -1, after = 1, b_size = 0, a_size = 0;
        char * start;
        for (int i = 0; i < blocks.size(); i ++) { 
            if(blocks[i].base == base) index = i;
            blocks[i].free = true;
        }
        while (index + before >= 0 && blocks[index + before].free) { 
            b_size += blocks[index + before].size;
            before --;
        }
        start = blocks[index + before + 1].base;
        while (index + after < blocks.size() && blocks[index + after].free) { 
            a_size += blocks[index + after].size;
            after ++;
        }
        blocks.erase(blocks.begin() + index + before + 1, blocks.begin() + index + after - 1);
        block tblock(start, n + a_size + b_size);
        blocks.push_back(tblock);
        sort(blocks.begin(), blocks.end(), size_cmp);
    }
    size_t nr_free_pages() { 
        return nr_free;
    }
    void check() {}
};
int main() { 
    class pmm_manager pmm;
    pmm.init();
    char * a = pmm.alloc_pages(256);
    char * b = pmm.alloc_pages(128);
    char * c = pmm.alloc_pages(512);
    pmm.free_pages(b, 128);
    return 0;
}


```
```
如何表示空闲块？ 如何表示空闲块列表？ 
[(start0, size0),(start1,size1)...]
在一次malloc后，如果根据某种顺序查找符合malloc要求的空闲块？如何把一个空闲块改变成另外一个空闲块，或消除这个空闲块？如何更新空闲块列表？
在一次free后，如何把已使用块转变成空闲块，并按照某种顺序（起始地址，块大小）插入到空闲块列表中？考虑需要合并相邻空闲块，形成更大的空闲块？
如果考虑地址对齐（比如按照4字节对齐），应该如何设计？
如果考虑空闲/使用块列表组织中有部分元数据，比如表示链接信息，如何给malloc返回有效可用的空闲块地址而不破坏
元数据信息？
伙伴分配器的一个极简实现
http://coolshell.cn/tag/buddy
```

--- 

## 扩展思考题

阅读[slab分配算法](http://en.wikipedia.org/wiki/Slab_allocation)，尝试在应用程序中实现slab分配算法，给出设计方案和测试用例。

## “连续内存分配”与视频相关的课堂练习

### 5.1 计算机体系结构和内存层次
MMU的工作机理？

- [x]  

>  http://en.wikipedia.org/wiki/Memory_management_unit

L1和L2高速缓存有什么区别？

- [x]  

>  http://superuser.com/questions/196143/where-exactly-l1-l2-and-l3-caches-located-in-computer
>  Where exactly L1, L2 and L3 Caches located in computer?

>  http://en.wikipedia.org/wiki/CPU_cache
>  CPU cache

### 5.2 地址空间和地址生成
编译、链接和加载的过程了解？

- [x]  

>  

动态链接如何使用？

- [x]  

>  


### 5.3 连续内存分配
什么是内碎片、外碎片？

- [x]  

>  

为什么最先匹配会越用越慢？

- [x]  

>  

为什么最差匹配会的外碎片少？

- [x]  

>  

在几种算法中分区释放后的合并处理如何做？

- [x]  

>  

### 5.4 碎片整理
一个处于等待状态的进程被对换到外存（对换等待状态）后，等待事件出现了。操作系统需要如何响应？

- [x]  

>  

### 5.5 伙伴系统
伙伴系统的空闲块如何组织？

- [x]  

>  

伙伴系统的内存分配流程？

- [x]  

>  

伙伴系统的内存回收流程？

- [x]  

>  

struct list_entry是如何把数据元素组织成链表的？

- [x]  

>  



