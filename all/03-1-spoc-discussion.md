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
最优匹配算法：优势：在分配的尺寸比较小的时候，可以避免那些大的空闲分区被拆分，减少了外部碎片的大小，相对简单，效果比较好。缺点：产生了外部碎片，容易产生较多的小碎片，释放分区较慢。
最差匹配算法： 优点： 在分配的尺寸中等时，可以避免出现太多的小碎片，效果较好。缺点：释放分区较慢，产生外部碎片，容易破坏大的空闲分区，不利于后续分配大的分区。
最先匹配算法： 优点： 比较简单，在高地址空间有大块的空闲分区 缺点：产生了外部碎片，分配尺寸较大时速度较慢。
buddy system分配算法：优点：分配效率高，还可回收内存，避免内存碎片的产生，提高了物理内存的利用率， 缺点：实现较为复杂。

## 小组思考题

请参考ucore lab2代码，采用`struct pmm_manager` 根据你的`学号 mod 4`的结果值，选择四种（0:最优匹配，1:最差匹配，2:最先匹配，3:buddy systemm）分配算法中的一种或多种，在应用程序层面(可以 用python,ruby,C++，C，LISP等高语言)来实现，给出你的设思路，并给出测试用例。 (spoc)
>
#include<stdio.h>
#include<stdlib.h>

#define N 	1024


typedef struct node
{
	char *addr; //指向mem处开始的地址
	int size;  //空闲块的大小 
	struct node *next; //指向下一个空闲块 
}node;

char mem[N]; 
node freelist; 

void init()
{
	node *ptr = (node *)malloc(sizeof(node));
	ptr->addr = mem;
	ptr->size = N;
	ptr->next = NULL;
	freelist.next = ptr;
}

void removenode(node *ptr, node *prev)
{	
	prev->next = ptr->next;
	free(ptr);
}

/* 首次适配法：从自由空闲区中选取第一个合适空闲区来分配 
 * 返回分配内存区首地址
 */
char *firstfit(int size) 
{
	node *ptr, *prev;
	char *memptr;
	
	for(prev=&freelist, ptr=prev->next; ptr; prev=ptr, ptr=ptr->next)
		if(ptr->size > size)
		{
			memptr = ptr->addr;
			ptr->size -= size; //从空闲区去掉size空间
			ptr->addr += size; //空闲区首地址往后移size个单位
			return memptr; //返回申请到的空闲区 首地址
		}
		else if(ptr->size == size)
		{
			memptr = ptr->addr;
			removenode(ptr, prev);
			return memptr;
		}
	return NULL; 
}


void addtofreelist(char *memptr, int size)
{
	node *prev, *ptr, *newptr;
	
	for(prev=&freelist, ptr=prev->next; ptr && ptr->addr <= memptr; prev=ptr, ptr=ptr->next)
	{} 
	newptr = new node();
	newptr->addr = memptr;
	newptr->size = size;
	newptr->next = ptr;
	prev->next = newptr;
}

/* combine blocks of list if necessary */
void combine()
{
	node *prev, *ptr;
	
	for(prev=&freelist, ptr=prev->next; ptr; prev=ptr, ptr=ptr->next)
		if(prev != &freelist && prev->addr+prev->size == ptr->addr)
		{
			prev->next = ptr->next;
			prev->size = prev->size + ptr->size;
			free(ptr);
		}
}

/* prt: sizeof(int) contains size of the pool allocated 
 * 返回分配的空间首地址(不包括最前面的长度的4个字节)
 */
char *memalloc(int size)
{
	char *ptr = firstfit(size + sizeof(int)); //此处选择分配算法
	printf("allocating %d using firstfit...\n", size);
	if(ptr == NULL)
		return NULL;
	*(int *)ptr = size; 
	return ptr+sizeof(int);
}

void memfree(char *ptr)
{
	int size = *(int *)(ptr-sizeof(int));
	printf("freeing %d...\n", size);
	addtofreelist(ptr-sizeof(int), size+sizeof(int));
	combine();
}

void printfreelist()
{
	node *ptr;
	printf("\t");
	for(ptr=freelist.next; ptr != NULL; ptr=ptr->next)
		printf("{%u %d}", ptr->addr, ptr->size);
	putchar('\n');
}

main()
{
	char *p1, *p2, *p3, *p4, *p5;
	init();
	printfreelist();
	
	p1 = memalloc(10);//note：分配10个字节，但其前面还有4个字节用于指示长度的，所以共用了14字节
	printfreelist();
	
	p2 = memalloc(15);
	printfreelist();
	
	p3 = memalloc(23);
	printfreelist();
	
	p4 = memalloc(3);
	printfreelist();
	
	p5 = memalloc(8);
	printfreelist();
	
	memfree(p1);
	printfreelist();
	
	memfree(p5);
	printfreelist();
	
	memfree(p3);
	printfreelist();
	
	p1 = memalloc(23);
	printfreelist();
	
	p1 = memalloc(23);
	printfreelist();
	
	memfree(p2);
	printfreelist();
	
	p1 = memalloc(3);
	printfreelist();
	
	memfree(p4);
	printfreelist();
	
	p2 = memalloc(1);
	printfreelist();
	
	memfree(p1);
	printfreelist();
	
	memfree(p2);
	printfreelist();
	
	system("pause");
	return 0;
	
}


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

