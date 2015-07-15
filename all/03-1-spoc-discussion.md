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
# 1.最先匹配：
优点：（1）实现起来比较简单
            （2）由于是按地址从低地址到高地址找可用的空间分区，所以低地址的分区会优先被分配，则高地址有大块的空              闲分区
缺点：（1）容易产生外部碎片
            （2）分配大块时较慢。因为是从低地址开始找，低地址的块都被分配出去，产生各种大小的碎片。要分配大块时则需要一直向高地址遍历，比较费时。
 
# 2.  最优匹配
优点 ： 大部分分配的尺寸较小时，效果很好
        （1） 可以避免大的空闲分区被拆分。由于是从小到大找能用的最小空闲分区，则大块不易被拆分。
         （2）可以减少外部碎片的大小。找的是可用的最小的，因此分出区后，原块余下的碎片也最小
         （3） 实现上来说相对简单
缺点 （1） 依然会有外部碎片
         （2）释放分区较慢。因为排序是按大小排，但释放分区时需要合并地址上相邻的空间块，则需要额外的按地址的搜索，速度较慢。  
         （3） 容易产生大量的无用小碎片
         
# 3.最差匹配
优点：（1）中等大小分配较多时，效果最好
            （2） 可以避免出现太多的无用小碎片
            （3） 分配速度最快。由于是分配满足条件的最大的，空间分区按大小从大到小排，则只需判断第一项即可。
 缺点：（1） 释放分区较慢。同最优匹配一样，按大小排，按地址合并，需要额外的搜索。
             （2） 有外部碎片
             （3） 容易破坏大的空间分区，使得后续难以分配大的分区
             
# 4.伙伴系统
优点： （1） 分配速度较快。从算法上看，可以看做是二分查找，log（n）级别的复杂度，相对较快。
缺点： （1）实现相对复杂
             （2）容易产生很多的内部碎片。很少有进程需要的内存大小正好是2的整数次幂，因此大部分分出去的分区都会产生内部碎片。      
             
# 更有效的连续内存分配算法
可以有一些改进的思路，如伙伴算法，不必每次释放内存时都合并，做相应的表计，等实际使用时在合并
对最优匹配发，因为产生的都是很小的外部碎片，每次查找时，可以不必从头开始找，而是从上一次找到的空闲分区的指针或编号开始找，因为对某个空前分区，分出去之后，产生的外部碎片必然小于后面的空闲分区，因而后面空闲分区在该按大小排序的链表中rank不变，所以这样做可以提高效率。
            


  + 采分点：说明四种算法的优点和缺点
  - 答案没有涉及如下3点；（0分）
  - 正确描述了二种分配算法的优势和劣势（1分）
  - 正确描述了四种分配算法的优势和劣势（2分）
  - 除上述两点外，进一步描述了一种更有效的分配算法（3分）
 ```
- [x]  

>  

## 小组思考题


请参考ucore lab2代码，采用`struct pmm_manager` 根据你的`学号 mod 4`的结果值，选择四种（0:最优匹配，1:最差匹配，2:最先匹配，3:buddy systemm）分配算法中的一种，在应用程序层面来实现，并给出测试用例。 (spoc)
2012011364 最先匹配
代码如下：
#include "list.h"
#include "string.h"
#include "my_pmm.h"

free_area_t free_area;

#define free_list (free_area.free_list)
#define nr_free (free_area.nr_free)

static void my_init(void) {
    list_init(&free_list);
    nr_free = 0;
    printf("list initialization completed\n");
}

static void MySort( list_entry_t *head) //链表升序冒泡排序，le为头指针
{
	list_entry_t* p = NULL;
	list_entry_t* q = NULL;
	if (list_next(head) == head) { //空链表
        return;
	}
	p = list_next(head) ;
	for (; p!=head; p=list_next(p) ){
        struct Page* pagep = le2page(p, page_link);
        if (pagep->property == 0) continue;

		for (q=list_next(p) ; q!=head;q=list_next(q) ) {

            struct Page* pageq = le2page(q, page_link);
            if (pageq->property == 0) continue;
            if (p == q) continue;

            //printf("lalal %d %d \n",pagep->property, pageq->property );
			if(pagep->property > pageq->property ){
                printf("lalal %d %d \n",pagep->property, pageq->property );

                list_entry_t* ptail = p;
                list_entry_t* qtail = q;
                size_t incount = 1;
                while (incount< pagep->property) {
                    ptail = list_next(ptail);
                    incount++;
                }
                //printf("haha1 %d\n",le2page(p->prev, page_link)->property);
                //if (p == head) printf("nana /n");
                incount = 1;
                while (incount< pageq->property) {
                    qtail = list_next(qtail);
                    incount++;
                }
                //printf("haha2 %d\n",le2page(qtail->next, page_link)->property);
                //printf("%d\n",incount);

                //list_entry_t* qnext = list_next(qtail) ;
                //while ( qnext != head && le2page(qnext, page_link)->property == 0 )  qnext = list_next(qnext) ;

                //list_entry_t* pnext = list_next(ptail) ;
               // while ( pnext != head && le2page(pnext, page_link)->property == 0 )  pnext = list_next(pnext) ;

                //printf("lalal %d %d\n",le2page(pnext, page_link)->property, le2page(qnext, page_link)->property);

                //list_entry_t* ppre = list_prev(p) ;
                //while (ppre != head && le2page(ppre, page_link)->property == 0)  ppre = list_prev(ppre) ;

                //list_entry_t* qpre = list_prev(q) ;
                //while (qpre != head && le2page(qpre, page_link)->property == 0)  qpre = list_prev(qpre) ;

                //printf("lalal2 %d %d\n",le2page(ppre, page_link)->property, le2page(qpre, page_link)->property);


                 //printf("lalal3 %d %d\n",le2page(p, page_link)->property, le2page(q, page_link)->property);
                list_entry_t* qnext = list_next(qtail) ;
                ptail->next = qnext;
                qnext->prev = ptail;

                list_entry_t* ppre = list_prev(p) ;

                qtail->next = p;
                p->prev = qtail;

                //if (ppre == head) printf("lalala3\n");

                q->prev = ppre;
                ppre->next = q;

                p->prev = qtail;
                qtail->next = p;

                list_entry_t* temp = p;
                p = q;
                q = temp;


            }
		}
	}
	return;
}

//初始化可用內存
static void my_init_memmap(struct Page *base, size_t n) {
    struct Page *p = base;
    for (p = base; p < base + n; p ++) {
        p->flags = 0;
        p->property = 0; //全部page置为不可用
        list_add_before(&free_list, &(p->page_link));
    }
    nr_free += n;
    base->property = n;
    printf("innitialization completed, the size of initial free block is: %d\n", n);
}

//最先匹配算法
static struct Page * my_alloc_pages(size_t n) {
    if (n > nr_free) {
        printf("requested memory too large!/n");
        return NULL;
    }
    list_entry_t *le, *len;
    le = &free_list;

    size_t fragmentsize = 0;
    while ((le=list_next(le)) != &free_list) {
    	struct Page *p = le2page(le, page_link); //一个空闲分区由多个page合在一起

    	if (p->property >= n) {
            //printf("%d \n", p->property);
            size_t oldproperty = p->property;
    		int i;
    		for (i=0; i<n; i++) {
    			len = list_next(le);
    			struct Page *pp = le2page(le, page_link);
    			pp->property = 0; //SetPageReserved
    			list_del(le);
    			le = len; //退出后le变成head后的第一个了
    		}

    		if(oldproperty>n){
                //printf("%d \n",oldproperty);
    			(le2page(le,page_link))->property = oldproperty - n; //如果恰好等于n，则该空闲分区被完全个分出去，未产生碎片
    			fragmentsize = oldproperty - n;
    		}

    		p->property = 0;
    		nr_free -= n;
    		printf("alloc succeed! new fragment size:%d\n", fragmentsize);
    		MySort(&free_list);
    		return p;
    	}
    }
    printf("alloc failed!\n");
    return NULL;
}

static void my_free_pages(struct Page *base, size_t n) {

    if (n<=0 || base->property != 0) {
        printf("memory error!/n");
        return;
    }

    list_entry_t *le = &free_list;
    struct Page * p;
    while ((le=list_next(le)) != &free_list) {
    	p = le2page(le, page_link);
    	if(p>base){
    		break;
    	}
    } //退出循环后，le指向base的后一个空闲block首page

    //printf("lalal2 %d %d\n",n, le2page(le2, page_link)->property);

    for (p=base; p<base+n; p++) {
    	list_add_before(le, &(p->page_link)); //把被释放的page插回
    }

    base->flags = 0;
    base->property = n;

    //合并相邻空闲块
    p = le2page(le,page_link) ;
    if ( base+n == p ) { //若释放后的空闲块与le开始的空闲块相邻则合并，le在base后
     //printf("lalal5 %d\n",n);
      base->property += p->property;
      p->property = 0;
    }

    //le = base->page_link;
    //while (le != head && le2page(le, page_link)->property == 0)  le = list_prev(le) ;
    le = list_prev(&(base->page_link));
    p = le2page(le, page_link); //le指向base的前一个page

    if (le!=&free_list && p==base-1) {
      while(le!=&free_list){

    	  if(p->property) { //找到base前的相邻空闲块
              //printf("lalal5 %d\n",n);
    		  p->property += base->property;
    		  base->property = 0;
    		  break;
    	  }
    	  le = list_prev(le);
    	  //while (le != &free_list && le2page(le, page_link)->property == 0)  le = list_prev(le) ;
    	  p = le2page(le,page_link);
      }
    }

    nr_free += n; //总可用空闲块增加
    MySort(&free_list);
    return ;
}

static size_t my_nr_free_pages(void) {
    return nr_free;
}

static void my_show() {
    // show current free pagenumber
    printf("there are %d pages left now\n",nr_free);
    list_entry_t *le = &free_list;
    struct Page * p;
    size_t blockcount = 0;
    while ((le=list_next(le)) != &free_list) { //遍历链表
        //printf("lala\n");
    	p = le2page(le, page_link);
    	if(p->property != 0){
            blockcount++;
    		printf("block: %d blocksize: %d \n",blockcount, p->property);
    	}
    } //退出循环后，le指向base的后一个空闲page

}

const struct pmm_manager my_pmm_manager = {
    .name = "my_pmm_manager",
    .init = my_init,
    .init_memmap = my_init_memmap,
    .alloc_pages = my_alloc_pages,
    .free_pages = my_free_pages,
    .nr_free_pages = my_nr_free_pages,
    .check = NULL,
    .show = my_show
};

测试代码如下：
#include "my_pmm.h"

int main()
{
    my_pmm_manager.init();
    size_t n = 512;
    struct Page base[n+1];
    my_pmm_manager.init_memmap(base, n) ;

    my_pmm_manager.show();
    struct Page* pagegot30 = NULL;
    struct Page* pagegot20 = NULL;
    struct Page* pagegot35 = NULL;
    struct Page* pagegot40 = NULL;
    struct Page* pagegot25 = NULL;
    struct Page* pagegot10 = NULL;
    struct Page* pagegot80 = NULL;

    pagegot30 = my_pmm_manager.alloc_pages(30);
    pagegot35 = my_pmm_manager.alloc_pages(35);
    pagegot40 = my_pmm_manager.alloc_pages(40);
    pagegot20 = my_pmm_manager.alloc_pages(20);
    pagegot10 = my_pmm_manager.alloc_pages(10);
    pagegot80 = my_pmm_manager.alloc_pages(80);
    my_pmm_manager.show();
    //剩下最后一块297的
    my_pmm_manager.free_pages(pagegot35, 35);
    my_pmm_manager.show();

    //释放10和20的，会合并成30的，然后排在35那块前面
    my_pmm_manager.free_pages(pagegot10, 10);
    my_pmm_manager.show();
    my_pmm_manager.free_pages(pagegot20, 20);
    //my_pmm_manager.free_pages(pagegot30, 30);
    my_pmm_manager.show();

    //分配一块32的，会切35的空闲块，产生3的碎片并排在最前面
    struct Page* pagegot32 = my_pmm_manager.alloc_pages(32);
    my_pmm_manager.show();



    return 0;
}


结果如下：
list initialization completed
innitialization completed, the size of initial free block is: 512
there are 512 pages left now
block: 1 blocksize: 512 
alloc succeed! new fragment size:482
alloc succeed! new fragment size:447
alloc succeed! new fragment size:407
alloc succeed! new fragment size:387
alloc succeed! new fragment size:377
alloc succeed! new fragment size:297
there are 297 pages left now
block: 1 blocksize: 297 
there are 332 pages left now
block: 1 blocksize: 35 
block: 2 blocksize: 297 
lalal 35 10 
there are 342 pages left now
block: 1 blocksize: 10 
block: 2 blocksize: 35 
block: 3 blocksize: 297 
there are 362 pages left now
block: 1 blocksize: 30 
block: 2 blocksize: 35 
block: 3 blocksize: 297 
alloc succeed! new fragment size:3
lalal 30 3 
there are 330 pages left now
block: 1 blocksize: 3 
block: 2 blocksize: 30 
block: 3 blocksize: 297 



如何表示空闲块？ 如何表示空闲块列表？ 
[(start0, size0),(start1,size1)...]
在一次malloc后，如果根据某种顺序查找符合malloc要求的空闲块？如何把一个空闲块改变成另外一个空闲块，或消除这个空闲块？如何更新空闲块列表？
在一次free后，如何把已使用块转变成空闲块，并按照某种顺序（起始地址，块大小）插入到空闲块列表中？考虑需要合并相邻空闲块，形成更大的空闲块？
如果考虑地址对齐（比如按照4字节对齐），应该如何设计？
如果考虑空闲/使用块列表组织中有部分元数据，比如表示链接信息，如何给malloc返回有效可用的空闲块地址而不破坏
元数据信息？
伙伴分配器的一个极简实现
http://coolshell.cn/tag/buddy


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



