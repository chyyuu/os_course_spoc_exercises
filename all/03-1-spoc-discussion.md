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

```
最先匹配
优点:
1) 实现简单;
2) 在高地址空间有大块的空闲分区;
3) 合并的开销小;
缺点:
1) 多次分配后容易造成大量内部碎片, 在之后寻找空闲分区的开销也越来越大;
2) 在分配大块空间时较慢.

最佳匹配
优点:
1) 大部分分配的空间尺寸较小时, 效果较好
2) 可避免大的空闲分区被拆分;
3) 可减小外部碎片的大小;
4) 实现相对简单;
缺点:
1) 释放分区较慢;
2) 容易造成较多无法使用(太小)的外部碎片.

最差匹配
优点:
1) 中等大小的分配较多时, 效果最好;
2) 可避免出现太多的小碎片(因每次分配最大的空闲分区);
缺点:
1) 释放分区较慢;
2) 外部碎片;
3) 容易破坏大的空闲分区, 因此以后难以分配大的分区;

伙伴系统
优点:
1) 实现简单;
2) 合并较快;
3) 对需要的分区大小s, 2^(u-1) < s <= 2^u, 最多造成2^(u-1)-1的内部碎片. 因此当分配大小较小时, 可确保碎片较小.
缺点:
1) 承优点3), 当分配大小较大时, 碎片可能较大 (达到接近50%的内部碎分)

建议分配算法
把连续地址分成多个小块(如每个块512bytes), 在分配时返回一个块地址列表如下
[(0x0000, 5), (0x0100, 4)], 如此分配 (0x0000开始的) 512x5 + (0x0100开始的) 512 x 4 bytes. 如此可以有效利用分部碎片, 也可以限制内部碎片的大小. 此方法在内存资源不足时, 在空间利用率上较好. 但在资源充裕时没有明显效果.
```

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
> 搜索百度中, 同学帮助

```
struct pmm_manager {
    const char *name;                                 // XXX_pmm_manager's name
    void (*init)(void);                               // initialize internal description&management data structure
                                                      // (free block list, number of free block) of XXX_pmm_manager 
    void (*init_memmap)(struct Page *base, size_t n); // setup description&management data structcure according to
                                                      // the initial free physical memory space 
    struct Page *(*alloc_pages)(size_t n);            // allocate >=n pages, depend on the allocation algorithm 
    void (*free_pages)(struct Page *base, size_t n);  // free >=n pages with "base" addr of Page descriptor structures(memlayout.h)
    size_t (*nr_free_pages)(void);                    // return the number of free pages 
    void (*check)(void);                              // check the correctness of XXX_pmm_manager 
};

class pmm_manager:
	max_size = 0
	free_page_list = []	
	def __init__(self, size):
		self.init(size)

	def init(self, size):
		if size < 0:
			print 'Positive size required'
			return
		self.max_size = size;
		self.free_page_list = [(0, size)]

	def alloc_pages(self, request_size):
		if (request_size <= 0 or request_size > self.max_size):
			return (-1, -1)

		for i in range(len(self.free_page_list)):
			(start, size) = self.free_page_list[i]
			if size >= request_size:
				if size == request_size:
					del self.free_page_list[i]
				else:
					self.free_page_list[i] = (start + request_size, size - request_size)
				print 'Alloc : ', (start, request_size)
				return (start, request_size)
		return (-1, -1)
	
	def free_pages(self, start, size):
		if (start < 0 or start >= self.max_size or size <= 0 or start + size >= self.max_size):
			return (-1, -1)
		elif self.free_page_list == []:
			self.free_page_list.append((start, size))
		else:
			(ex_start, ex_size) = self.free_page_list[0]
			if start < ex_start:
				end = start + size
				if end > ex_start:
					print 'free_pages failed : overlap'
				if end == ex_start:
					self.free_page_list[0] = (start, size + ex_size)
				else:
					self.free_page_list.insert(0, (start, size))
			else:
				list_len = len(self.free_page_list)
				last_flag = list_len - 1
				(ex_start, ex_size) = self.free_page_list[last_flag]
				if ex_start < start:
					ex_end = ex_start + ex_size
					if ex_end > start: 
						print 'free_pages failed : overlap'
					if ex_end == start:
						self.free_page_list[last_flag] = (ex_start, ex_size + size)
					else:
						self.free_page_list.append((start, size))
				else:
					l_flag = 0
					r_flag = list_len - 1
					while (l_flag + 1 < r_flag):
						m_flag = (l_flag + r_flag) / 2
						(m_start, m_size) = self.free_page_list[m_flag]
						if m_start > start:
							r_flag = m_flag
						elif m_start < start:
							l_flag = m_flag
						else:
							print 'free_pages error'
							break
					(r_start, r_size) = self.free_page_list[r_flag]
					(l_start, l_size) = self.free_page_list[l_flag]
					l_end = l_start + l_size
					end = start + size
					if l_end > start or end > r_start:
						print 'free_pages failed : overlap'
					if l_end == start:
						if (end == r_start):
							self.free_page_list[l_flag] = (l_start, l_size + size + r_size)
							del self.free_page_list[r_flag]
						else:
							self.free_page_list[l_flag] = (l_start, l_size + size)
					elif (end == r_start):
						self.free_page_list[r_flag] = (start, size + r_size)
					else:
						self.free_page_list.insert(r_flag, (start, size))
		print self.free_page_list

	def nr_free_pages(self):
		print len(self.free_page_list)
		print self.free_page_list

	def check(self):
		if self.free_page_list == []:
			print 'check() succeeded!'
		else:
			next_start, next_size = self.free_page_list[0]
			next_end = next_start + next_size

			(this_start, this_end) = (-1, -1)
			for i in range(1, len(self.free_page_list)):
				(this_start, this_end) = (next_start, next_end)
				(next_start, next_size) = self.free_page_list[i]
				next_end = next_start + next_size
				if this_end > next_start:
					print 'check() failed!'
			print 'check() succeeded!'

if __name__ == '__main__':
	print 2012080061 % 4
	#2, 实现最先匹配

	alloc_list = []
	mgr = pmm_manager(512)
	alloc_list.append(mgr.alloc_pages(10))
	alloc_list.append(mgr.alloc_pages(15))
	alloc_list.append(mgr.alloc_pages(100))
	alloc_list.append(mgr.alloc_pages(50))
	alloc_list.append(mgr.alloc_pages(125))
	alloc_list.append(mgr.alloc_pages(200))

	start, size = alloc_list[3]
	mgr.free_pages(start, size)
	start, size = alloc_list[5]
	mgr.free_pages(start, size)
	start, size = alloc_list[4]
	mgr.free_pages(start, size)
	start, size = alloc_list[0]
	mgr.free_pages(start, size)
	alloc_list.append(mgr.alloc_pages(5))

	mgr.check()
	mgr.nr_free_pages()
```

--- 

## 扩展思考题

阅读[slab分配算法](http://en.wikipedia.org/wiki/Slab_allocation)，尝试在应用程序中实现slab分配算法，给出设计方案和测试用例。

## “连续内存分配”与视频相关的课堂练习

### 5.1 计算机体系结构和内存层次
MMU的工作机理？

- [x]  

>  http://en.wikipedia.org/wiki/Memory_management_unit
	答：MMU是内存管理单元，负责进行虚实地址转换

L1和L2高速缓存有什么区别？

- [x]  

>  http://superuser.com/questions/196143/where-exactly-l1-l2-and-l3-caches-located-in-computer
>  Where exactly L1, L2 and L3 Caches located in computer?

>  http://en.wikipedia.org/wiki/CPU_cache
>  CPU cache
>  高速缓存是对数据和指令的缓存，TLB是页表的缓存 L1和L2相比，L1更快更小更贵

### 5.2 地址空间和地址生成
编译、链接和加载的过程了解？

- [x]  

>  编译将源文件转换成目标文件 链接将库函数与编译成的目标文件合在一起，确定程序中一些地址的偏移量 加载将程序加入内存准备执行，确定程序中的地址

动态链接如何使用？

- [x]  

>  


### 5.3 连续内存分配
什么是内碎片、外碎片？

> 内碎片:由于存量信息容量与最小存储空间单位不完全相符而造成的空间碎片.
  外碎片:由于大量信息由于先后写入、置换、删除而形成的空间碎片.

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



