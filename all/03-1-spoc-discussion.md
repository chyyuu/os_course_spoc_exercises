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

> 
###最先匹配：  
定义：分配第一个大于需求大小的块  
优点：易于实现、块分配较快、块合并较快（可用块相邻则可合并）、由于分配从头开始，所以尾部有很多大块可用  
缺点：产生很多外部碎片、分配大块时较慢  
###最优匹配:  
定义：分配所有大于需求大小的块中最小的块  
优点:易于实现、若使用排序则块分配较快、减小了碎片尺寸，使分配效率更高、避免大块被打散、适宜于分配小块较多  
缺点：合并慢、剩余的碎片往往毫无用处，某种角度上浪费了空间  
###最差匹配： 
定义：分配最大的块  
优点：易于实现、若使用排序则块分配较快、产生的碎片某些情况下可用性更高、适宜于分配中等块较多  
缺点：合并慢、倾向于把大块打散，使得难以分配大块  
###伙伴系统： 
定义：一种特殊的分配模式，分配的块大小为2的幂，若所需大小小于块的一半，则将块分半；否则，将块全部分给它  
优点：合并速度很快、采用恰当的数据结构，分配速度也很快  
缺点：会产生内部碎片，最坏的情况下利用率只有50%、由于特殊的合并算法，有可能有2块连续空间却不能合并，导致不能分配  
###改进：  
最佳匹配适用于较小的块，而最差匹配适用于中等块，据此可以采用二者的结合，对需求的空间先进行判断，之后选择具体哪一种。
还可以由以上几种方法结合碎片管理系统，来达到更高的利用率。比如使用最佳匹配和紧凑算法，可以达到更高利用率。

## 小组思考题

请参考ucore lab2代码，采用`struct pmm_manager` 根据你的`学号 mod 4`的结果值，选择四种（0:最优匹配，1:最差匹配，2:最先匹配，3:buddy systemm）分配算法中的一种或多种，在应用程序层面(可以 用python,ruby,C++，C，LISP等高语言)来实现，给出你的设思路，并给出测试用例。 (spoc)

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
- [x]  

>
本人学号2012011291.分配到buddysystem  
示例代码如下。主要函数为bsAlloc与bsFree。测试样例直接写到了main函数中。执行即可。  
代码说明：  
用二叉树的每个节点对应一个内存块。节点中记录对应内存块的地址、尺寸、是否可用。根节点代表初始时的整块可用内存。  
在一次malloc中，首先查找整棵树找到最佳匹配节点，然后进行尺寸减半分裂直至符合要求。  
在一次free中，首先释放当前节点对应的空间，然后看他的兄弟节点是否能进行合并，进行递归的合并。  
我的实现优点是较为简单且实现了基本思想，不足时许多地方没有考虑性能要求。  
测试样例解释如下：  
初始空间为64M  
p1=7M，可以分配在起始地址0  
p2=13M,可以分配在16  
p3=16M,可以分配在32  
p4=32M,没有可用空间，不能分配  
此时free p1，倒出了8M空间，并与另外8M合并形成16M  
p4仍无法分配  
此时freep2，到出16M空间，与刚刚的16M合并形成32M  
p4可以分配  
最终执行结果符合上述。  
  
```
#include<iostream>
#include<cmath>
using namespace std;
class Node
{
public:
	int addr;	//记录节点所管理内存的初始位置
	int size;	//记录节点所管理内存的大小
	bool able;	//节点是否可用
	Node* parent;
	Node* lchild;
	Node* rchild;
	Node(int Addr,int Size,Node* Parent,Node* Lchild,Node* Rchild)
	{
		addr=Addr;
		size=Size;
		parent=Parent;
		lchild=Lchild;
		rchild=Rchild;
		able=true;
	}
	void split()	//一分为2，产生2个儿子节点
	{
		Node* l=new Node(addr,size/2,this,NULL,NULL);
		Node* r=new Node(addr+size/2,size/2,this,NULL,NULL);
		lchild=l;
		rchild=r;
	}
};
bool isLeaf(Node* n)
{
	return n->lchild == NULL;
}
bool isRoot(Node* n)
{
	return n->parent == NULL;
}
Node* findBrother(Node* n)
{
	if(isRoot(n))
		return NULL;
	if(n == (n->parent->lchild))
		return n->parent->rchild;
	else
		return n->parent->lchild;
}
int next2(int a)	//寻找最接近的2的幂
{
	if(a==1)
		return 1;
	int counter=1;
	while(a>2)
	{
		counter++;
		a=a/2;
	}
	return pow(2,counter);
}
class Process	//定义一个抽象的进程，大小为size，addr为待分配地址,pnode为与地址关联的节点。若无，-1，节点null
{
public:
	int addr;
	int size;
	int needSize;
	Node* pnode;
	Process(int s)
	{
		size=s;
		needSize=next2(size);
		addr=-1;
		pnode=NULL;
	}
};
Node* findNode(int needSize,Node* n)	//在n的子树中找到最适应节点，若无，返回NULL
{
	if(n->size < needSize)
		return NULL;
	if(isLeaf(n) && n->able==true)
		return n;
	if(isLeaf(n) && n->able==false)
		return NULL;
	Node* temp1=findNode(needSize,n->lchild);
	Node* temp2=findNode(needSize,n->rchild);
	if(!temp1)
	{
		if(!temp2)
			return  NULL;
		return temp2;
	}
	else
	{
		if(!temp2)
			return temp1;
		if(temp1->size <= temp2->size)
			return temp1;
		else
			return temp2;
	}
}
bool bsAlloc(Process& p,Node* head)	//若成功，对p的addr赋值，并返回true，否则 false
{
	Node* temp=findNode(p.needSize,head);	//找到最适应节点
	//Node* temp=head;
	if(!temp)
		return false;
	while(temp->size > p.needSize)	//分裂直到大小一致
	{
		temp->split();
		temp=temp->lchild;
	}
	p.pnode=temp;
	p.addr=temp->addr;
	temp->able=false;
	return true;
}
void bsFree(Process& p,Node* head)
{
	p.addr=-1;
	p.pnode->able=true;	//释放
	Node* temp=p.pnode;
	p.pnode=NULL;
	Node* bro=findBrother(temp);
	while(bro && isLeaf(bro) && bro->able==true)	//合并
	{
		Node* d1=temp;
		Node* d2=bro;
		temp=temp->parent;
		bro=findBrother(temp);
		temp->lchild=NULL;
		temp->rchild=NULL;
		temp->able=true;
		delete d1,d2;
	}
}
int main()
{
	Node* head=new Node(0,64,NULL,NULL,NULL);	//测试样例。地址从0开始，大小为64M
	Process p1(7);
	Process p2(13);
	Process p3(16);
	Process p4(32);
	bool flag1=bsAlloc(p1,head);
	cout<<"p1是否成功:"<<flag1<<" addr="<<p1.addr<<endl;
	bool flag2=bsAlloc(p2,head);
	cout<<"p2是否成功:"<<flag2<<" addr="<<p2.addr<<endl;
	bool flag3=bsAlloc(p3,head);
	cout<<"p3是否成功:"<<flag3<<" addr="<<p3.addr<<endl;
	bool flag4=bsAlloc(p4,head);
	cout<<"p4是否成功:"<<flag4<<" addr="<<p4.addr<<endl;
	bsFree(p1,head);
	cout<<"free 1"<<endl;
	flag4=bsAlloc(p4,head);
	cout<<"p4是否成功:"<<flag4<<" addr="<<p4.addr<<endl;
	bsFree(p2,head);
	cout<<"free 2"<<endl;
	flag4=bsAlloc(p4,head);
	cout<<"p4是否成功:"<<flag4<<" addr="<<p4.addr<<endl;
	system("pause");
	return 0;
}
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



