# 文件系统(lec 21) spoc 思考题

## 个人思考题
### 文件系统和文件 
 1. 文件系统的功能是什么？

>  负责数据持久保存，功能是数据存储和访问

>  具体功能：文件分配、文件管理、数据可靠和安全

 1. 什么是文件？

>  文件系统中具有符号名的基本数据单位。

### 文件描述符
 1. 打开文件时，文件系统要维护哪些信息？

>  文件指针、打开文件计数、访问权限、文件位置和数据缓存

 1. 文件系统的基本数据访问单位是什么？这对文件系统有什么影响？
 1. 文件的索引访问有什么特点？如何优化索引访问的速度？

### 目录、文件别名和文件系统种类
 1. 什么是目录？

>  由文件索引项组成的特殊文件。

 1. 目录的组织结构是什么样的？

>  树结构、有向图

 1. 目录操作有哪些种类？
 1. 什么是文件别名？软链接和硬链接有什么区别？
 1. 路径遍历的流程是什么样的？如何优化路径遍历？
 1. 什么是文件挂载？
 1. 为什么会存在大量的文件类型？

### 虚拟文件系统 
 1. 虚拟文件系统的功能是什么？

>  对上对下的接口、高效访问实现

 1. 文件卷控制块、文件控制块和目录项的相互关系是什么？
 1. 可以把文件控制块放到目录项中吗？这样做有什么优缺点？


### 文件缓存和打开文件
 1. 文件缓存和页缓存有什么区别和联系？
 1. 为什么要同时维护进程的打开文件表和操作系统的打开文件表？这两个打开文件表有什么区别和联系？为什么没有线程的打开文件表？
 
### 文件分配
 1. 文件分配的三种方式是如何组织文件数据块的？各有什么特征？
 1. UFS多级索引分配是如何组织文件数据块的位置信息的？

### 空闲空间管理和冗余磁盘阵列RAID
 1. 硬盘空闲空间组织和文件分配有什么异同？
 1. RAID-0、1、4和5分别是如何组织磁盘数据块的？各有什么特征？

## 小组思考题
 1. (spoc)完成Simple File System的功能，支持应用程序的一般文件操作。具体帮助和要求信息请看[sfs-homework](https://github.com/chyyuu/ucore_lab/blob/master/related_info/lab8/sfs-homework.md)

> 问题1： 
```
mkdir("/g")
creat("/q")
creat("/u")
link("/u","/x")
mkdir("/t")
creat("/g/c")
unlink("/x")
mkdir("/g/w")
fd=open("/g/c", O_WRONLY|O_APPEND); write(fd, buf, BLOCKSIZE); close(fd)
fd=open("/g/c", O_WRONLY|O_APPEND); write(fd, buf, BLOCKSIZE); close(fd);
```

> 问题2：
见[sfs-homework.py](https://github.com/OneSida/ucore_lab/blob/master/related_info/lab8/sfs-homework.py)

> 问题3：
见[sfs-homework-softlink.py](https://github.com/OneSida/ucore_lab/blob/master/related_info/lab8/sfs-homework-softlink.py)


 1. (spoc)FAT、UFS、YAFFS、NTFS这几种文件系统中选一种，分析它的文件卷结构、目录结构、文件分配方式，以及它的变种。
  wikipedia上的文件系统列表参考
  - http://en.wikipedia.org/wiki/List_of_file_systems
  - http://en.wikipedia.org/wiki/File_system
  - http://en.wikipedia.org/wiki/List_of_file_systems

  请同学们依据自己的选择，在下面链接处回复分析结果。
  - [https://piazza.com/class/i5j09fnsl7k5x0?cid=416 FAT文件系统分析]
  - [https://piazza.com/class/i5j09fnsl7k5x0?cid=417 NTFS文件系统分析]
  - [https://piazza.com/class/i5j09fnsl7k5x0?cid=418 UFS文件系统分析]
  - [https://piazza.com/class/i5j09fnsl7k5x0?cid=419 ZFS文件系统分析]
  - [https://piazza.com/class/i5j09fnsl7k5x0?cid=420 YAFFS文件系统分析]

```
FAT32

1.文件卷结构
AT32文件系统由DBR及其保留扇区，FAT1，FAT2和DATA四个部分组成
这些结构是在分区被格式化时创建出来的，含义解释如下：
DBR及其保留扇区：DBR的含义是DOS引导记录，也称为操作系统引导记录，在DBR之后往往会有一些保留扇区。
FAT1：FAT的含义是文件分配表，FAT32一般有两份FAT，FAT1是第一份，也是主FAT。
FAT2：FAT2是FAT32的第二份文件分配表，也是FAT1的备份。
DATA：DATA也就是数据区，是FAT32文件系统的主要区域，其中包含目录区域。

2.目录项
1) 0-- 7字节 文件正名。 
(2) 8--10字节 文件扩展名。 
(3) 11字节 文件属性，按二进制位定义，最高两位保留未用，0至5位分别是只读位、隐藏位、系统位、卷标位、子目录位、归档位。 
(4) 11--13字节 仅长文件名目录项用，用来存储其对应的短文件名目录项的文件名字节校验和等。 
(5) 13--15字节 24位二进制的文件建立时间，其中的高5位为小时，次6位为分钟。 
(6) 16--17字节 16位二进制的文件建立日期，其中的高7位为相对于1980年的年份值，次4位为月份，后5位为月内日期。 
(7) 18--19字节 16位二进制的文件最新访问日期，定义同(6)。 
(8) 20--21字节 起始簇号的高16位。 
(9) 22--23字节 16位二进制的文件最新修改时间，其中的高5位为小时，次6位为分钟，后5位的二倍为秒数。 
(10)24--25字节 16位二进制的文件最新修改日期，定义同(6)。 
(11)26--27字节 起始簇号的低16位。 
(12)28--31字节 32位的文件字节长度

3.文件分配方式
一个分区分成同等大小的簇，也就是连续空间的小块。簇的大小随着FAT文件系统的类型以及分区大小而不同，
典型的簇大小介于2KB到32KB之间。每个文件根据它的大小可能占有一个或者多个簇；
这样，一个文件就由这些这些（称为单链表）簇链所表示。

4.变种
FAT12    FAT16    FAT32
```
