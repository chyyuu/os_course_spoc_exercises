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

代码位于[Here](https://github.com/BrieflyX/ucore_lab/blob/master/related_info/lab7/deadlock/bankers-homework.py)
```
import os, sys
import random
import numpy as np
import itertools

class Bankers(object):
    def __init__(self, totalResource):
        #initiating
        self.RESOURCE = totalResource

    def SignProcesses(self, max_, allocated_):
        self.max = max_
        self.allocated = allocated_
        self.need = self.CalcNeed()
        self.avaliable = self.CalcAvaliable()
        self.finished = [False]*len(self.allocated)

    def Difference(self,a,b):
        #return matrix subtracted from a by b
        res = []
        for i in range(len(a)):
            tmp = []
            for j in range(len(a[i])):
                tmp.append(a[i][j]-b[i][j])
            res.append(tmp)
        return res

    def CalcNeed(self):
        #calc request by subtracting signed matrix from max matrix
        return self.Difference(self.max,self.allocated)

    def CalcAvaliable(self):
        """Calc Avaliable Resource"""
        a = self.allocated
        res = []
        for j in range(len(a[0])):
            tmp = 0
            for i in range(len(a)):
                tmp += a[i][j]
            res.append(self.RESOURCE[j] - tmp)
        return res

    def ExecuteProcess(self,index):

        #check if less avaliable than Request
        # YOUR CODE, YOUR ID 2012010685 
        for i,j in zip(self.avaliable, self.need[index]):
            if i < j:
                return False
        #check END here

        #allocating what they need.
        # YOUR CODE, YOUR ID 2012010685
        for i in range(len(self.need[index])):
            self.allocated[index][i] += self.need[index][i]
            self.need[index][i] = 0
        self.avaliable = self.CalcAvaliable()
        return True
        #allocating END here 

    def TempSafeCheckAfterRelease(self):
        #check if at least one request can be done after previous process done. not check whole sequances.
        #if every element of Requests can't accepted after previous process done, this mean it is not safe state
        # YOUR CODE, YOU ID 2012010685
        for request in self.need:
            can_be_done = True
            for i, j in zip(self.avaliable, request):
                if i < j:
                    can_be_done = False
            if can_be_done:
                return True
        return False
        #check END here

    def print_matrixes(self):
        print "_____________________________________________"
        print "MAX\t\tAllocated\tNeed"
        for idx in range(len(self.max)):
            print "%s\t%s\t%s" % (self.max[idx],self.allocated[idx], self.need[idx])
        print "_____________________________________________"
        print "Resources:"
        print "Total: %s\tAvailable: %s\n" % (self.RESOURCE, self.avaliable)

    def ReleasingProcess(self,index):
        for i in range(0,len(self.RESOURCE)):
            self.finished[index] = True
            self.allocated[index][i] = 0
        self.avaliable = self.CalcAvaliable()

    def Execute(self):
        i = 0
        # get all permutation of processes
        perm = itertools.permutations(range(procnum), procnum)
        permArray = np.asarray(list(perm))

        for arr in permArray:
            for i in arr:
                if self.finished[i] == False:
                    print "Executing..."
                    print "Request: "
                    print self.need[i]
                    #check if less avaliable than Request
                    if self.ExecuteProcess(i):
                        print "Dispatching Done..."

                        self.print_matrixes()

                        print "-----Releasing Process------"

                        self.ReleasingProcess(i)

                        self.print_matrixes()

                        #check if at least one request can be done after previous process done. not check whole sequances.
                        #if every element of Requests can't accepted after previous process done, this mean it is not safe state
                        if not (self.TempSafeCheckAfterRelease()):
                            print "SAFE STATE: NOT SAFE - There are no sequances can avoid Deadlock"
                            return False
                        processes.append(i)
                    else:
                        print "HOLD: not enough Resource"

                if i == len(self.allocated)-1:
                    i = 0
                else:
                    i += 1

                check = True
                for k in range(0,len(self.allocated)):
                    if self.finished[k] == False:
                        check = False
                        break
                if check == True:
                    return True
                    break
        #every permutation of processes is false
        return False

def getmax():
    res = []
    for j in range(procnum):
        tmp = []
        for i in range(len(total_resources)):
            randnum=random.random()
            remain_max=0
            if j >0:
                remain_max=total_resources[i]
                for k in range(j):
                    remain_max=remain_max-res[k][i]
                if remain_max < 0:
                    remain_max=0
            else:
                remain_max=total_resources[i]
            tmp.append((int)(randnum*remain_max*0.8))
        res.append(tmp)
    return res

def getallocated():
    res = []
    for j in range(procnum):
        tmp = []
        for i in range(len(total_resources)):
            randnum=random.random()
            remain=0
            if j >0:
                remain=max[j][i]
                for k in range(j):
                    remain=remain-res[k][i]
                if remain < 0:
                    remain=0
            else:
                remain=max[j][i]
            tmp.append((int)(randnum*remain))
        res.append(tmp)
    return res

print "start here"
# random seed
seed = 2
random.seed(seed)
# the number of process list
procnum = 3
# the number of type of resource
resnum =  4
# the max total value of resource
restotalval = 30
# the total resources list
total_resources=[]
# the total processes
processes=[]
# set the real total value of resource in total_resources
for i in range(resnum):
    total_resources.append((int)(restotalval*random.random()))
# init the Banker
b = Bankers(total_resources)
# get the max request values of resources from process
max=getmax()
# get the already gotted values of resources from process
allocated=getallocated()
# init need matrix, available vector
b.SignProcesses(max, allocated)
# print all theses matrixes
b.print_matrixes()
# executing Banker algorithm
result=b.Execute()
# show results
if result:
    print "SUCCESS proc lists ",processes
else:
    print "Failed"

total_resources = [6, 5, 7, 6]
processes=[]
b = Bankers(total_resources)

max = [
    [3, 3, 2, 2],
    [1, 2, 3, 4],
    [1, 3, 5, 0],
]
allocated = [
    [1, 2, 2, 1],
    [1, 0, 3, 3],
    [1, 2, 1, 0],
]

b.SignProcesses(max, allocated)
b.print_matrixes()
result=b.Execute()
if result:
    print "SUCCESS proc lists ",processes
else:
    print "Failed"


total_resources = [10, 10, 8, 5]
processes=[]
b = Bankers(total_resources)
max = [
        [10, 8, 2,5],
        [6, 1, 3,1],
        [3, 1, 4,2],
        [5, 4, 2,1]
    ]
allocated = [
        [3, 0, 0,3],
        [1, 1, 2,0],
        [2, 1, 2,1],
        [0, 0, 2,0]
    ]
b.SignProcesses(max, allocated)
b.print_matrixes()
result=b.Execute()
if result:
    print "SUCCESS proc lists ",processes
else:
    print "Failed"
```

 - (spoc) 以小组为单位，请思考在lab1~lab5的基础上，是否能够实现IPC机制，请写出如何实现信号，管道或共享内存（三选一）的设计方案。
 
共享内存的设计：
每一个新创建的共享内存对象都用一个shmid_kernel数据结构来表达，该数据结构中保存共享内存的相关信息、占用的内存页面个数、内存页面数组、attaches等信息。系统中所有的shmid_kernel数据结构都保存在shm_segs向量表中，该向量表的每一个元素都是一个指向shmid_kernel数据结构的指针。
attaches描述被共享的物理内存对象所映射的各进程的虚拟内存区域。每一个希望共享这块内存的进程都必须通过系统调用将其关联（attach）到它的虚拟内存中。这一过程将为该进程创建了一个新的描述这块共享内存的vm_area_struct数据结构。vm_area_struct用于维系共享内存和使用它的进程之间的关系。
 
sharememory的不同进程之间的key是约定的。
首先是shmget的实现，这个函数是用于得到一个共享内存标识符或创建一个共享内存对象并返回共享内存标识符。参数有key，size和shmflg。key是约定的，当shmflg&IPC_CREAT为真时，如果内核中不存在键值与key相等的共享内存，则新建一个共享内存；如果存在这样的共享内存，返回此共享内存的标识符。返回值是共享内存的标识符。
shmat用于将共享内存区对象映射到调用进程的地址空间。输入为shmid shmaddr shmflg，连接共享内存标识符为shmid的共享内存，连接成功后将共享内存区对象映射到调用进程的地址空间。返回附加好的共享内存地址。
shmdt函数，输入为shmaddr，用于断开共享内存连接。
shmctl函数用于实现对共享内存的控制管理。
 
共享内存的保护：引入信号量保护共享内存，实现同步互斥。
线程fork的时候，子线程共用父线程的资源。进程fork的时候，fork后子进程继承已连接的共享内存地址。exec后该子进程与已连接的共享内存地址自动脱离(detach)。进程结束后，已连接的共享内存地址会自动脱离(detach)
由于共享内存作为进程间通信的交互部分，所以完全可以对其进行锁定，使得不能swap换入换出，从而规避由swap引入的相关问题。具体实现交由shmctl函数对共享内存进行控制。控制操作包括获得共享内存对象的状态，设置共享内存对象的参数（如uid、gid、mode、ctime等），将共享内存对象在内存中锁定和释放（在对象的mode上增加或去除SHM_LOCKED标志），释放共享内存对象资源等。
共享内存的释放应该是基于引用计数规则，也即每个进程结束对共享内存的访问之后shmdt断开连接，最后一个断开连接的进程负责释放共享内存。

 - (spoc) 扩展：用C语言实现某daemon程序，可检测某网络服务失效或崩溃，并用信号量机制通知重启网络服务。[信号机制的例子](https://github.com/chyyuu/ucore_lab/blob/master/related_info/lab7/ipc/signal-ex1.c)

 - (spoc) 扩展：用C语言写测试用例，测试管道、消息队列和共享内存三种通信机制进行不同通信间隔和通信量情况下的通信带宽、通信延时、带宽抖动和延时抖动方面的性能差异。[管道的例子](https://github.com/chyyuu/ucore_lab/blob/master/related_info/lab7/ipc/pipe-ex2.c)
