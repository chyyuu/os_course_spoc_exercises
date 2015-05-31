# 死锁与IPC(lec 20) spoc 思考题


- 有"spoc"标记的题是要求拿清华学分的同学要在实体课上完成，并按时提交到学生对应的ucore_code和os_exercises的git repo上。

## 个人思考题

### 死锁概念 
 - 尝试举一个生活中死锁实例。
 - 可重用资源和消耗资源有什么区别？

### 可重用和不可撤销；
 - 资源分配图中的顶点和有向边代表什么含义？
 - 出现死锁的4个必要条件是什么？

### 死锁处理方法 
 - 死锁处理的方法有哪几种？它们的区别在什么地方？
 - 安全序列的定义是什么？

### 进程的最大资源需要量小于可用资源与前面进程占用资源的总合；
 - 安全、不安全和死锁有什么区别和联系？

### 银行家算法 
 - 什么是银行家算法？
 - 安全状态判断和安全序列是一回事吗？

### 死锁检测 
 - 死锁检测与安全状态判断有什么区别和联系？

> 死锁检测、安全状态判断和安全序列判断的本质就是资源分配图中的循环等待判断。

### 进程通信概念 
 - 直接通信和间接通信的区别是什么？
  本质上来说，间接通信可以理解为两个直接通信，间接通信中假定有一个永远有效的直接通信方。
 - 同步和异步通信有什么区别？
### 信号和管道 
 - 尝试视频中的信号通信例子。
 - 写一个检查本机网络服务工作状态并自动重启相关服务的程序。
 - 什么是管道？

### 消息队列和共享内存 
 - 写测试用例，测试管道、消息队列和共享内存三种通信机制进行不同通信间隔和通信量情况下的通信带宽、通信延时、带宽抖动和延时抖动方面的性能差异。
 
## 小组思考题

 - （spoc） 每人用python实现[银行家算法](https://github.com/chyyuu/ucore_lab/blob/master/related_info/lab7/deadlock/bankers-homework.py)。大致输出可参考[参考输出](https://github.com/chyyuu/ucore_lab/blob/master/related_info/lab7/deadlock/example-output.txt)。除了`YOUR CODE`部分需要填写代码外，在算法的具体实现上，效率也不高，可进一步改进执行效率。

 - (spoc) 以小组为单位，请思考在lab1~lab5的基础上，是否能够实现IPC机制，请写出如何实现信号，管道或共享内存（三选一）的设计方案。
 
 > 参考 Linux 实现共享内存的方式，这是 System V 风格的共享内存。首先有这样一个表示共享内存区域的结构体：

```c
/* Data structure describing a shared memory segment.  */
struct shmid_ds
  {
    struct ipc_perm shm_perm;           /* operation permission struct */
    size_t shm_segsz;                   /* size of segment in bytes */
    __time_t shm_atime;                 /* time of last shmat() */
#ifndef __x86_64__
    unsigned long int __glibc_reserved1;
#endif
    __time_t shm_dtime;                 /* time of last shmdt() */
#ifndef __x86_64__
    unsigned long int __glibc_reserved2;
#endif
    __time_t shm_ctime;                 /* time of last change by shmctl() */
#ifndef __x86_64__
    unsigned long int __glibc_reserved3;
#endif
    __pid_t shm_cpid;                   /* pid of creator */
    __pid_t shm_lpid;                   /* pid of last shmop */
    shmatt_t shm_nattch;                /* number of current attaches */
    __syscall_ulong_t __glibc_reserved4;
    __syscall_ulong_t __glibc_reserved5;
  };
```

> 然后在 sys/shm.h 中定义几个方法，功能如下：

```c
/* The following System V style IPC functions implement a shared memory
   facility.  The definition is found in XPG4.2.  */

/* Shared memory control operation.  */
extern int shmctl (int __shmid, int __cmd, struct shmid_ds *__buf) __THROW;

/* Get shared memory segment.  */
extern int shmget (key_t __key, size_t __size, int __shmflg) __THROW;

/* Attach shared memory segment.  */
extern void *shmat (int __shmid, const void *__shmaddr, int __shmflg)
     __THROW;

/* Detach shared memory segment.  */
extern int shmdt (const void *__shmaddr) __THROW;
```

> shmget 就是用一个 key 返回一个对应到共享内存段的标识符。
 
> shmat 将标识符 attach 到调用进程的内存地址空间的一个地址上。
 
> shmdt detach 一个当前进程的 shmaddr 指向的内存段。有一个引用计数，最后 detach 的 process 负责释放内存。
 
 - (spoc) 扩展：用C语言实现某daemon程序，可检测某网络服务失效或崩溃，并用信号量机制通知重启网络服务。[信号机制的例子](https://github.com/chyyuu/ucore_lab/blob/master/related_info/lab7/ipc/signal-ex1.c)

 - (spoc) 扩展：用C语言写测试用例，测试管道、消息队列和共享内存三种通信机制进行不同通信间隔和通信量情况下的通信带宽、通信延时、带宽抖动和延时抖动方面的性能差异。[管道的例子](https://github.com/chyyuu/ucore_lab/blob/master/related_info/lab7/ipc/pipe-ex2.c)
