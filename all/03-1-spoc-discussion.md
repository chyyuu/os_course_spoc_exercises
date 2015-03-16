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
- [x]  

> 1. 最先匹配

>    优点：实现简单；在高地址空间中能留有大块的空闲分区，可以提供给某些需求较高的程序。

>    缺点：会留有外部碎片；当外部碎片越多，分配大块空间就越慢。

> 2. 最优匹配

>    优点：可避免空闲分区被拆分；可减小外部碎片的大小；实现相对简单。

>	 缺点：容易生成很多难以分配的小碎片；释放分区较慢。

> 3. 最差匹配

>    优点：中等大小的分配较多时效果最好；能避免出现太多的小碎片。

>    缺点：释放分区较慢；会产生外部碎片；由于每次寻找最大的空间，所以很容易破坏大分区，使得后续大需求程序难以分配到资源。

> 4. 伙伴系统

>    优点：空闲分区分配和回收速度快、算法简单。

>    缺点：内存利用率低效,主要是因为所有的内存请求都必须以2的幂次方大小的空间来满足。

> 5. 更有效的分配算法

>    对于小需求的程序采用最优分配，大需求的程序采用最差分配

## 小组思考题

请参考ucore lab2代码，采用`struct pmm_manager` 根据你的`学号 mod 4`的结果值，选择四种（0:最优匹配，1:最差匹配，2:最先匹配，3:buddy systemm）分配算法中的一种或多种，在应用程序层面(可以 用python,ruby,C++，C，LISP等高语言)来实现，给出你的设计思路，并给出测试用例。 (spoc)
> 参考coolshell的设计思路，不同的是我对于每个节点都存储了该节点对应段的最大存储量，使得在分配和释放时能够减少一部分的乘法运算。

> 根据伙伴系统的分割特性，利用完全二叉树的方法管理内存，使用结构体组模拟完全二叉树，可以直接借助下标运算完成对左右子节点和父节点的访问。每一个节点表示一个固定大小的内存段，每一层代表对整体内存的一个分割方法，每一个父节点通过二分产生两个子节点（如父节点代表16单位的内存段，则两个子节点分别代表8单位的内存段，左子节点地址低于右子节点，地址连续）。

> 建立结构体buddyNode代表二叉树中的节点，size为其表示的内存段的大小，为固定值；left为其表示的内存段可用空间大小，为可变值，作为每一个内存段的状态标记。

> 分配时，首先将所请求的内存大小补全至刚好大于它的2的幂次（如25补全至32），然后利用深度优先遍历找到对应大小与补全大小相等的节点，将此节点标记为已被占用（表现为left等于0），然后向上依次更新父节点的left值（取两子节点的最大left）。

> 释放时，根据提供的地址，从叶节点向上寻找left为0的节点，将其置为自己的size，同时向上更新父节点的left（若两子节点left之和与自己的size相等则置为自己的size，否则取两子节点的最大left）。

> 代码（附测试用例）如下:

```
#include <cstdlib>
#include <iostream>
using namespace std;

#define PARENT(i) (i-1)/2
#define LCHILD(i) 2*i+1
#define RCHILD(i) 2*i+2

struct buddyNode
{
    unsigned size;
    unsigned left;
};

bool checkPower2(int a)
{
    if(a == 1)
        return true;
    if(a <= 0)
        return false;
    
    int i = 1;
    while(i < a)
        i *= 2;
    if(i == a)
        return true;
    else
        return false;
}

buddyNode* init(int size)
{
    if(size < 1 || !checkPower2(size))
    {
        cout << "Size Error! It must be power of 2." << endl;
        return NULL;
    }
    buddyNode* tree = new buddyNode[2*size - 1];
    int nodeSize = size * 2;
    for(int i = 0; i < 2*size - 1; i++)
    {
        if(checkPower2(i + 1))
            nodeSize /= 2;
        tree[i].size = tree[i].left = nodeSize;
    }
    return tree;
}

int nearestPower2(int a)
{
    if(a <= 0)
    {
        cout << "nearestPower2 Error!!" << endl;
        exit(1);
    }
    if(checkPower2(a))
        return a;
    else
    {
        int i = 1;
        while(i < a)
            i *= 2;
        return i;
    }
        
}

int buddyAlloc(buddyNode* tree, int neededSize)
{
    int index = 0;
    int nodeSize;
    int addr = 0;
    
    if(tree == NULL)
    {
        cout << "buddyAlloc null pointer Error!!" << endl;
        return -1;
    }
    if(neededSize <= 0)
    {
        cout << "buddyAlloc negative-0 size Error!!" << endl;
        return -1;
    }
    if(tree[0].left == 0)
    {
        cout << "No more space!!" << endl;
        return -1;
    }
    
    if(!checkPower2(neededSize))
        neededSize = nearestPower2(neededSize);
    for(nodeSize = tree[0].size; nodeSize != neededSize; nodeSize /= 2)
    {
        if(tree[LCHILD(index)].left >= neededSize)
            index = LCHILD(index);
        else
            index = RCHILD(index);
    }
    tree[index].left = 0;
    addr = (index + 1) * nodeSize - tree[0].size;
    
    while(index > 0)
    {
        index = PARENT(index);
        tree[index].left = max(tree[LCHILD(index)].left, tree[RCHILD(index)].left);
    }
    return addr;
}

void buddyFree(buddyNode* tree, int addr)
{
    if(tree == NULL)
    {
        cout << "buddyAlloc null pointer Error!!" << endl;
        return;
    }
    if(addr < 0)
    {
        cout << "buddyAlloc negative address Error!!" << endl;
        return;
    }
    if(addr >= tree[0].size)
    {
        cout << "buddyAlloc address overflow!!" << endl;
        return;
    }
    
    int index = addr + tree[0].size - 1;
    while(tree[index].left != 0)
    {
        index =PARENT(index);
        if(index == 0)
            break;
    }
    tree[index].left = tree[index].size;
    while(index > 0)
    {
        index = PARENT(index);
        if(tree[LCHILD(index)].left + tree[RCHILD(index)].left == tree[index].size)
            tree[index].left = tree[index].size;
        else
            tree[index].left = max(tree[LCHILD(index)].left, tree[RCHILD(index)].left);
    }
}

int main()
{
    cout << "首先建立一块虚拟的内存，大小为16单位,每单位对应一位地址（0~15）。" << endl;
    buddyNode* tree = init(16);
    int first = buddyAlloc(tree, 7);
    cout << "第一个进程需要7单位内存，它得到的地址空间起始点为：" << first << endl;
    cout << "第二个进程需要3单位内存，它得到的地址空间起始点为：" << buddyAlloc(tree, 3) << endl;
    cout << "释放掉第一个进程的内存。" << endl;
    buddyFree(tree, first);
    cout << "第三个进程需要4单位内存，它得到的地址空间起始点为：" << buddyAlloc(tree, 4) << endl;
    buddyAlloc(tree,7);
    
    return 0;
}
```

> 运行结果为：

```
首先建立一块虚拟的内存，大小为16单位,每单位对应一位地址（0~15）。
第一个进程需要7单位内存，它得到的地址空间起始点为：0
第二个进程需要3单位内存，它得到的地址空间起始点为：8
释放掉第一个进程的内存。
第三个进程需要4单位内存，它得到的地址空间起始点为：0
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



