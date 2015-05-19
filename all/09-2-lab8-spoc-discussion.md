# lab8 文件系统 (lec 22) spoc 思考题


- 有"spoc"标记的题是要求拿清华学分的同学要在实体课上完成，并按时提交到学生对应的ucore_code和os_exercises的git repo上。

## 个人思考题

### 总体介绍
 1. 文件系统中的文件、目录、索引节点和安装点这几种数据结构分别支持些什么操作？
 2. 请简要描述ucore文件系统支持的文件系统抽象

 > 文件、目录、索引节点和安装点

### ucore 文件系统架构

 1. 请简要阐述ucore 文件系统架构的四个组成部分

 > 系统调用接口、VFS、SFS和I/O接口

 2. 请简要说明进程proc_struct、文件file、inode之间的关系。 
 
 3. ucore中的进程打开文件表和系统打开文件表对应到具体的哪个数据结构上？

### Simple File System分析

 1. SFS在硬盘上的四大部分主要是什么，有何作用？
 
 > superblock, root-dir inode, freeman, data block

 2. 硬盘上的SFS是如何加载到ucore中并初始化的？
 3. 硬盘上的inode和内存中的inode的关系和区别是什么?
 4. 描述file, dir, inode在内存和磁盘上的格式和相关操作。

### Virtual File System分析

 1. file数据结构的主要内容是什么？与进程的关系是什么？
 2. inode数据结构的主要内容是什么？与file的数据结构的关系是什么？
 3. inode_ops包含哪些与文件相关的操作？
 4. VFS是如何把键盘、显示输出和磁盘文件统一到一个系统调用访问框架下的？ 

### I/O 设备接口分析

 1. device数据结构的主要内容是什么？与fs的关系是什么？与inode的关系是什么？
 2. 比较ucore中I/O接口、SFS文件系统接口和文件系统的系统调用接口的操作函数有什么异同？
 
## 小组思考题

1. (spoc) 理解文件访问的执行过程，即在ucore运行过程中通过`cprintf`函数来完整地展现出来读一个文件在ucore中的整个执行过程，(越全面细致越好)
完成代码填写，并形成spoc练习报告，需写练习报告和简单编码，完成后放到git server 对应的git repo中

以lab8_result 中sh.c 的读取为例展现读文件的过程。读文件按照文件系统架构的层次可分为如下的部分：文件访问的系统调用接口->VFS接口->FS（lab8中为SFS）接口->文件系统IO设备接口。

读取sh.c文件并执行，其中调用的主线是在proc.c的user_mian中，通过在内核线程加载usermain函数，而user_main函数调用系统调用，加载sh文件，然后执行下面的调用链：

    sys_exec->do_execve->sysfile_open->file_open->vfs_open->vfs_lookup->vop_lookup(sfs_lookup)->sfs_lookup_once-        >sfs_load_inode->sfs_create_inode->vop_init(inode_init)
    在file_open中，创建了一个新的inode指针,最后的inode_init将具体的SFS操作和inode绑定
    
    上述初始化完成后，读取sh中的elf文件内容时，是实际的读取调用链：
    do_execve->load_icode->load_icode_read->sysfile_read->file_read->vop_read(sfs_read)->sfs_io->sfs_io_nolock->sfs_rblock->sfs_rwblock_nolock->dop_io
    到dop_io时，已从SFS层进入设备io层
    

上述过程即为读取的整个调用链，打印结果如下，虚线上面是初始化部分，下面是实际读取部分

    kernel_execve: pid = 2, name = "sh".
    Now it's in pro.c, function do_execve, file path sh 
    Now it's in sysfile.c, function sysfile_open, file path sh 
    Now it's in file.c, function file_open, file path sh 
    Now it's in vfsfile.c, function vfs_open, file path sh 
    Now it's in vfs_lookup.c, function vfs_lookup, file path sh 
    Now it's in sfs_inode.c, function sfs_lookup, file path sh 
    Now it's in sfs_io.c, function sfs_rwblock_nolock
    Now it's in sfs_io.c, function sfs_rwblock_nolock
    Now it's in sfs_io.c, function sfs_rwblock_nolock
    Now it's in sfs_io.c, function sfs_rwblock_nolock
    Now it's in sfs_io.c, function sfs_rwblock_nolock
    Now it's in sfs_io.c, function sfs_rwblock_nolock
    Now it's in sfs_io.c, function sfs_rwblock_nolock
    Now it's in sfs_io.c, function sfs_rwblock_nolock
    Now it's in sfs_inode.c, function sfs_lookup_once, file name sh 
    Now it's in sfs_io.c, function sfs_rwblock_nolock
    Now it's in sfs_inode.c, function sfs_load_inode
    Now it's in sfs_inode.c, function sfs_create_inode
    Now it's in inode.c, function inode_init 
    _____________________________________________________-
    Now it's in proc.c, function load_icode
    Now it's in proc.c, function load_icode_read
    Now it's in syscall, function sysfile_read 
    Now it's in function file_read, the fd has been changed to file structure 
    Now it's in sfs_inode.c, function sfs_read
    Now it's in sfs_inode.c, function sfs_io
    Now it's in sfs_inode.c, function sfs_io_nolock
    Now it's in sfs_io.c, function sfs_rwblock_nolock
    Now it's in proc.c, function load_icode_read
    Now it's in syscall, function sysfile_read 
    Now it's in function file_read, the fd has been changed to file structure 
    Now it's in sfs_inode.c, function sfs_read
    Now it's in sfs_inode.c, function sfs_io
    Now it's in sfs_inode.c, function sfs_io_nolock
    Now it's in sfs_io.c, function sfs_rwblock_nolock





2. （spoc） 在下面的实验代码的基础上，实现基于文件系统的pipe IPC机制

### 练习用的[lab8 spoc exercise project source code](https://github.com/chyyuu/ucore_lab/tree/master/labcodes_answer/lab8_result)