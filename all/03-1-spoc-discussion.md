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

>  优缺点见下表

| 分配算法 | 优点 | 缺点 |
|--------|------|-----|
| 最优匹配 | 尽可能多的利用空间，fragment 较少 | 每次需要遍历所有 block ，较慢 |
| 最差匹配 | 对于分配的大小基本一致的情况，空间利用较好 | 较慢，在现实中不实用 |
| 最快匹配 | 不用遍历，较快 | 空间利用不佳 |
| buddy  | free 较快，只需要 log 时间，external fragment 少 | internal fragment 很多 |

> slab分配算法：slab分配算法采用cache 存储内核对象。对象的数量与slab的大小有关。

> slab 可能的三种状态：
- 满的：slab 中的所有对象被标记为使用。
- 空的：slab 中的所有对象被标记为空闲。
- 部分：slab 中的对象有的被标记为使用，有的被标记为空闲。

> 开始，所有对象都标记为空闲。当需要内核数据结构的对象时，可以直接从cache 上直接获取，并将对象初始化为使用。

> slab 分配器首先从部分空闲的slab 进行分配。如没有，则从空的slab 进行分配。如没有，则从物理连续页上分配新的slab，并把它赋给一个cache ，然后再从新slab 分配空间。

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

> 首次适应算法：

```
# [(start1, end1), (start2, end2), ...]
# memory size is 512K
mem_size = 512
spare = [[0, mem_size]]
used = list()

def malloc(length):
    # debug
    print 'alloc: ', length

    if len(spare) == 0:
        print 'memory used up. abort. '
        return

    # sort spare spaces according to address
    spare.sort(key = lambda tup: tup[0])
    # find the smallest spare block that is larger than length
    flag = -1
    for i in range(len(spare)):
        if abs(spare[i][0] - spare[i][1]) >= length:
            flag = i
            break

    if flag == -1:
        print 'too large to alloc. abort. '
        return

    block = spare.pop(flag)
    used.append([block[0], block[0] + length])
    if abs(block[0] - block[1]) != length:
        spare.append([block[0] + length, block[1]])

def mfree(addr):
    # debug
    print 'free: ', addr

    flag = -1
    for i in range(len(used)):
        if used[i][0] == addr:
            flag = i
            break

    if flag == -1:
        print 'the block is not used. fail to free. '
        return

    block = used.pop(flag)
    front = -1
    back = -1
    for idx, tup in enumerate(spare):
        if (tup[1] == block[0]):
            block[0] = tup[0]
            front = idx

        if (tup[0] == block[1]):
            block[1] = tup[1]
            back = idx
    if front >= 0:
        spare.pop(front)
    if back >= 0:
        spare.pop(back)
    spare.append(block)

def mem_stat():
    print 'used: ', used
    print 'free: ', spare, '\n'

# TEST CASES
malloc(128)
malloc(128)
malloc(128)
mem_stat()
mfree(128)
mem_stat()
mfree(256)
mem_stat()
malloc(64)
malloc(64)
mem_stat()
mfree(127)
mfree(128)
mem_stat()
malloc(128)
mem_stat()
malloc(32)
mem_stat()
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


