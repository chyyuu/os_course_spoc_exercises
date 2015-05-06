# 同步互斥(lec 18) spoc 思考题


- 有"spoc"标记的题是要求拿清华学分的同学要在实体课上完成，并按时提交到学生对应的ucore_code和os_exercises的git repo上。

## 个人思考题

### 基本理解
 - 什么是信号量？它与软件同步方法的区别在什么地方？
 - 什么是自旋锁？它为什么无法按先来先服务方式使用资源？
 - 下面是一种P操作的实现伪码。它能按FIFO顺序进行信号量申请吗？
```
 while (s.count == 0) {  //没有可用资源时，进入挂起状态；
        调用进程进入等待队列s.queue;
        阻塞调用进程;
}
s.count--;              //有可用资源，占用该资源； 
```

> 参考回答： 它的问题是，不能按FIFO进行信号量申请。
> 它的一种出错的情况
```
一个线程A调用P原语时，由于线程B正在使用该信号量而进入阻塞状态；注意，这时value的值为0。
线程B放弃信号量的使用，线程A被唤醒而进入就绪状态，但没有立即进入运行状态；注意，这里value为1。
在线程A处于就绪状态时，处理机正在执行线程C的代码；线程C这时也正好调用P原语访问同一个信号量，并得到使用权。注意，这时value又变回0。
线程A进入运行状态后，重新检查value的值，条件不成立，又一次进入阻塞状态。
至此，线程C比线程A后调用P原语，但线程C比线程A先得到信号量。
```

### 信号量使用

 - 什么是条件同步？如何使用信号量来实现条件同步？
 - 什么是生产者-消费者问题？
 - 为什么在生产者-消费者问题中先申请互斥信息量会导致死锁？

### 管程

 - 管程的组成包括哪几部分？入口队列和条件变量等待队列的作用是什么？
 - 为什么用管程实现的生产者-消费者问题中，可以在进入管程后才判断缓冲区的状态？
 - 请描述管程条件变量的两种释放处理方式的区别是什么？条件判断中while和if是如何影响释放处理中的顺序的？

### 哲学家就餐问题

 - 哲学家就餐问题的方案2和方案3的性能有什么区别？可以进一步提高效率吗？

### 读者-写者问题

 - 在读者-写者问题的读者优先和写者优先在行为上有什么不同？
 - 在读者-写者问题的读者优先实现中优先于读者到达的写者在什么地方等待？
 
## 小组思考题

1. （spoc） 每人用python threading机制用信号量和条件变量两种手段分别实现[47个同步问题](07-2-spoc-pv-problems.md)中的一题。向勇老师的班级从前往后，陈渝老师的班级从后往前。请先理解[]python threading 机制的介绍和实例](https://github.com/chyyuu/ucore_lab/tree/master/related_info/lab7/semaphore_condition)

具体代码见本目录下的ReaderWriteProblem文件夹里的python代码

我做的是读写者问题的扩展一，这个问题在基本的读写者问题上新增了两个要求：一是要求平等第对待读者和写者，而是要求每个资源可以同时读取的线程数有限。

第二个要求比较简单，先说第二个要求吧。新增一个信号量threadReadSem，设置初始资源数为最大同时读取线程数。每个线程要读时，需要申请threadReadSem，读完后释放threadReadSem。当当前正在读的线程大于最大可读数时，新的读线程会在申请threadReadSem进入等待，这样就可以保证“每个资源可以同时读取的线程数有限”。

第二个问题稍微复杂一些。课上老师举的例子是读优先。即写需要获得WriteSemaphore。读的时候，第一个开始读的读者申请WriteSemaphore，最后一个读完的读者释放WriteSemaphore。所以写者只有等所有人读完后才能写。如何才叫“平等对待”呢？关于这点的理解，我和wiki上几个学长的观点类似，即：当前有k个读者正在读，若某个写着要写，那么写着等待这k个读者读完后开始写，在写着等待这k个读者的过程中，不再允许新的读者开始读。这样的话，每个写着不必等到所有读者读完，而只需 等已经开始读的读者读完。这种“平等”是比较合理的。

具体的实现上，和wiki上的学长有些区别。wiki上的学长是用了一个所以再设置一个排队信号量queue，每次读写进程要求访问文件都要在此信号量上排队，根据先来先得公平竟争。我的思路总体来说属于同一类，但是略有区别。我的思路如下：

    设置一个读者增加信号量AddReadSemaphore，AddReadSemaphore为一个二值信号量。每个写者要写时，先申请AddReadSemaphore，写完后释放AddReadSemaphore。每个读者要读时，首先申请AddReadSemaphore。若申请失败，说明此时有写者准备要写了，不允许增加新的读者。只有等当前读者读完，等待的写者写完，新的读者才能获取AddReadSemaphore并开始写。若AddReadSemaphore申请成功，表示现在没有写者在等待写。由于AddReadSemaphore是二值信号量，所以不能等读完才释放，否则没办法同时多个读者读，所以读者应该在AddReadSemaphore申请成功，读者计数ReaderCount 加1成功后即释放，以便其他读者开始写。

具体来说，设计如下四个信号：

      threadWriteSem 表示能否写，二值信号量
      threadCountSem 控制全局读者计数，二值信号量
      threadReadSem 表示能否度 初值为最大同时读取线程数
      threadAddReadSem 表示能否允许新的读者开始读，二值信号量
      
ReaderCount为当前正在读的线程计数
      
读写流程如下：

    写者：
    申请threadAddReadSem，进入等待读状态，不再允许新的读者开始读
    申请threadWriteSem，不再允许其他读者或者写者读写
    临界区，进行写
    释放threadWriteSem
    释放threadAddReadSem
    
    读者：
    申请threadAddReadSem，当没有写者等待写时可继续
    申请threadReadSem，当前读的线程不超最大限制数目时可继续
    申请threadCountSem
    如果ReaderCount为0，申请threadWriteSem，不允许写者再写
    ReaderCount加1
    释放threadCountSem
    释放threadAddReadSem
    临界区，进行读
    申请threadCountSem
    ReaderCount减1
    如果ReaderCount为0，说明当前正在读的读者都读完了，释放threadWriteSem，让等待写的写者写
    释放threadCountSem
    释放threadReadSem
    
测例如下，设置最大同时读线程数为2

        threads.append(Writer("thread"+str(0),threadCountSem,threadWriteSem,threadAddReadSem))
        threads.append(Reader("thread"+str(1),threadCountSem,threadWriteSem,threadReadSem,threadAddReadSem))
        threads.append(Reader("thread"+str(2),threadCountSem,threadWriteSem,threadReadSem,threadAddReadSem))
        threads.append(Reader("thread"+str(3),threadCountSem,threadWriteSem,threadReadSem,threadAddReadSem))
    
        threads.append(Writer("thread"+str(4),threadCountSem,threadWriteSem,threadAddReadSem))
        threads.append(Reader("thread"+str(5),threadCountSem,threadWriteSem,threadReadSem,threadAddReadSem))
    
        for thread in threads: 
          thread.start() 
          
先让一个写者开始写，又让三个读者开始读，又当一个写者开始写，又让一个读者开始读。读写的临界区了做了1000*1000次的循环，确保下一个thread start时上一个start尚未执行完毕。打印输出结果如下：

     Thread thread0  is waiting for writing!
     Thread thread0  is writing!
     Thread thread0  's writing finish!
     Thread thread1  is waiting for  reading!
     Thread thread1  is reading! ReaderCount is 1 now
     Thread thread2  is waiting for  reading!
     Thread thread2  is reading! ReaderCount is 2 now
      Thread thread3  is waiting for  reading!
     Thread thread1  's reading finish! ReaderCount is 2 now
     Thread thread3  is reading! ReaderCount is 2 now
      Thread thread4  is waiting for writing!
     Thread thread2  's reading finish! ReaderCount is 2 now
     Thread thread3  's reading finish! ReaderCount is 1 now
     Thread thread4  is writing!
     Thread thread4  's writing finish!
     Thread thread5  is waiting for  reading!
     Thread thread5  is reading! ReaderCount is 1 now
     Thread thread5  's reading finish! ReaderCount is 1 now
    
对上述结果做简要说明：线程0，4是写者，其余是读者， “is waiting for writing!”是在申请threadAddReadSem成功后打印的， “is waiting for  reading”是在threadAddReadSem申请成功后打印的。从上述结果可看出，线程0写完，线程1开始写，进入临界区；线程二开始写，进入临界区；线程三申请threadAddReadSem成功，但是最大同时读线程为2，threadReadSem申请失败。等到1读完，3才进入临界区。此时4要写，申请threadAddReadSem成功，进入等待状态。5应该要开始写，但是4进入等待写，5申请threadAddReadSem失败，所以此时没有打印5的信息。而是看到，2和3都读完后，4进入写临界区。等4写完，5才开始读。
    