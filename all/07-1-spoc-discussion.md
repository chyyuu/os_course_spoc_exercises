# 同步互斥(lec 17) spoc 思考题


- 有"spoc"标记的题是要求拿清华学分的同学要在实体课上完成，并按时提交到学生对应的ucore_code和os_exercises的git repo上。


## 个人思考题

### 背景
 - 请给出程序正确性的定义或解释。
 
    到目前为止未发现错误，很难做到之后保证不出错
    
 - 在一个新运行环境中程序行为与原来的预期不一致，是错误吗？
 - 程序并发执行有什么好处和障碍？
 - 什么是原子操作？

### 现实生活中的同步问题

 - 家庭采购中的同步问题与操作系统中进程同步有什么区别？
 - 如何通过枚举和分类方法检查同步算法的正确性？
 - 尝试描述方案四的正确性。
 - 互斥、死锁和饥饿的定义是什么？

### 临界区和禁用硬件中断同步方法

 - 什么是临界区？
 - 临界区的访问规则是什么？
 - 禁用中断是如何实现对临界区的访问控制的？有什么优缺点？

### 基于软件的同步方法

 - 尝试通过枚举和分类方法检查Peterson算法的正确性。
 - 尝试准确描述Eisenberg同步算法，并通过枚举和分类方法检查其正确性。

### 高级抽象的同步方法

 - 如何证明TS指令和交换指令的等价性？
 - 为什么硬件原子操作指令能简化同步算法的实现？
 
## 小组思考题

1. （spoc）阅读[简化x86计算机模拟器的使用说明](https://github.com/chyyuu/ucore_lab/blob/master/related_info/lab7/lab7-spoc-exercise.md)，理解基于简化x86计算机的汇编代码。

基于简化x86.py的汇编语言模拟提供了%ax，%bx，%cx，%dx死歌寄存器，以及程序计数器PC和堆栈寄存器sp

提供额汇编指令主要包括四类：MOV指令，加/减发算数运算指令，test指令和跳转指令。mov，加减法，test都支持在寄存器间，内存间，以及寄存器和内存间进行操作。其提供的汇编指令类似一般的X86汇编。

如下面这段简单的循环：

    .main
    .top
    sub  $1,%dx
    test $0,%dx     
    jgte .top         
    halt
    
即每次把%dx寄存器中的值减一，当该值大于等于0时一直跳转到.top处循环，否则结束


2. （spoc)了解race condition. 进入[race-condition代码目录](https://github.com/chyyuu/ucore_lab/tree/master/related_info/lab7/race-condition)。

 - 执行 `./x86.py -p loop.s -t 1 -i 100 -R dx`， 请问`dx`的值是什么？
 - 执行 `./x86.py -p loop.s -t 2 -i 100 -a dx=3,dx=3 -R dx` ， 请问`dx`的值是什么？
 - 执行 `./x86.py -p loop.s -t 2 -i 3 -r -a dx=3,dx=3 -R dx`， 请问`dx`的值是什么？
 - 变量x的内存地址为2000, `./x86.py -p looping-race-nolock.s -t 1 -M 2000`, 请问变量x的值是什么？
 - 变量x的内存地址为2000, `./x86.py -p looping-race-nolock.s -t 2 -a bx=3 -M 2000`, 请问变量x的值是什么？为何每个线程要循环3次？
 - 变量x的内存地址为2000, `./x86.py -p looping-race-nolock.s -t 2 -M 2000 -i 4 -r -s 0`， 请问变量x的值是什么？
 - 变量x的内存地址为2000, `./x86.py -p looping-race-nolock.s -t 2 -M 2000 -i 4 -r -s 1`， 请问变量x的值是什么？
 - 变量x的内存地址为2000, `./x86.py -p looping-race-nolock.s -t 2 -M 2000 -i 4 -r -s 2`， 请问变量x的值是什么？ 
 - 变量x的内存地址为2000, `./x86.py -p looping-race-nolock.s -a bx=1 -t 2 -M 2000 -i 1`， 请问变量x的值是什么？ 
 
 对上述问题的实验即回答如下：
 
*1.* 

loop.s的汇编代码如下：

    .main
    .top
    sub  $1,%dx
    test $0,%dx     
    jgte .top         
    halt
    
执行语句./x86.py -p loop.s -t 1 -i 100 -R dx，增加-c选项打印出结果，输出如下：‘

    dx          Thread 0         
    0   
   -1   1000 sub  $1,%dx
   -1   1001 test $0,%dx
   -1   1002 jgte .top
   -1   1003 halt
   
由此可知，%dx寄存器的初值默认为0

*2.*

两个进程的%dx寄存器初值都设置为3，interrupt为100，则应该是进程0先执行，%dx寄存器的值由3减到-1，退出；然后是进程1执行，%dx寄存器的值又由3减到-1，退出。实际运行的情况也如此，打印输出如下：

     dx          Thread 0                Thread 1         
        3   
        2   1000 sub  $1,%dx
        2   1001 test $0,%dx
        2   1002 jgte .top
        1   1000 sub  $1,%dx
        1   1001 test $0,%dx
        1   1002 jgte .top
        0   1000 sub  $1,%dx
        0   1001 test $0,%dx
        0   1002 jgte .top
       -1   1000 sub  $1,%dx
       -1   1001 test $0,%dx
       -1   1002 jgte .top
       -1   1003 halt
        3   ----- Halt;Switch -----  ----- Halt;Switch -----  
        2                            1000 sub  $1,%dx
        2                            1001 test $0,%dx
        2                            1002 jgte .top
        1                            1000 sub  $1,%dx
        1                            1001 test $0,%dx
        1                            1002 jgte .top
        0                            1000 sub  $1,%dx
        0                            1001 test $0,%dx
        0                            1002 jgte .top
       -1                            1000 sub  $1,%dx
       -1                            1001 test $0,%dx
       -1                            1002 jgte .top
       -1                            1003 halt
       
*3.*

因为interrupt设置为了3，有另个进程，且是随机打断，则两个进程会在随机打断时不断切换，交替运行。每次切换，都要保存原进程的上下文，包括%dx寄存器，然后恢复下一个进程的%dx。所以%dx的值是不断变换的，但最终都是从3减到-1.程序运行结果打印如下：

     dx          Thread 0                Thread 1         
        3   
        2   1000 sub  $1,%dx
        2   1001 test $0,%dx
        2   1002 jgte .top
        3   ------ Interrupt ------  ------ Interrupt ------  
        2                            1000 sub  $1,%dx
        2                            1001 test $0,%dx
        2   ------ Interrupt ------  ------ Interrupt ------  
        1   1000 sub  $1,%dx
        2   ------ Interrupt ------  ------ Interrupt ------  
        2                            1002 jgte .top
        1                            1000 sub  $1,%dx
        1                            1001 test $0,%dx
        1   ------ Interrupt ------  ------ Interrupt ------  
        1   1001 test $0,%dx
        1   ------ Interrupt ------  ------ Interrupt ------  
        1                            1002 jgte .top
        1   ------ Interrupt ------  ------ Interrupt ------  
        1   1002 jgte .top
        0   1000 sub  $1,%dx
        0   1001 test $0,%dx
        1   ------ Interrupt ------  ------ Interrupt ------  
        0                            1000 sub  $1,%dx
        0                            1001 test $0,%dx
        0                            1002 jgte .top
        0   ------ Interrupt ------  ------ Interrupt ------  
        0   1002 jgte .top
       -1   1000 sub  $1,%dx
       -1   1001 test $0,%dx
        0   ------ Interrupt ------  ------ Interrupt ------  
       -1                            1000 sub  $1,%dx
       -1   ------ Interrupt ------  ------ Interrupt ------  
       -1   1002 jgte .top
       -1   1003 halt
       -1   ----- Halt;Switch -----  ----- Halt;Switch -----  
       -1                            1001 test $0,%dx
       -1   ------ Interrupt ------  ------ Interrupt ------  
       -1                            1002 jgte .top
       -1   ------ Interrupt ------  ------ Interrupt ------  
       -1                            1003 halt


*4.*

执行程序，输出如下：

     2000          Thread 0         
        0   
        0   1000 mov 2000, %ax
        0   1001 add $1, %ax
        1   1002 mov %ax, 2000
        1   1003 sub  $1, %bx
        1   1004 test $0, %bx
        1   1005 jgt .top
        1   1006 halt
        
可看出内存的初始值为0.

*5.*

变量x的值有0增加到6.因为内部循环是把%bx逐步减1，而%bx初值设置为了3，两个进程都各自保存与操作自己的%bx，所以两个进程会各循环三次。但是因为两个进程读写的都是同一个物理内存地址2000，所以该地址的值会被累加六次，最终有0变为6。

*6.*

执行程序，输出的结果如下：

     2000          Thread 0                Thread 1         
        0   
        0   1000 mov 2000, %ax
        0   1001 add $1, %ax
        0   ------ Interrupt ------  ------ Interrupt ------  
        0                            1000 mov 2000, %ax
        0                            1001 add $1, %ax
        1                            1002 mov %ax, 2000
        1                            1003 sub  $1, %bx
        1   ------ Interrupt ------  ------ Interrupt ------  
        1   1002 mov %ax, 2000
        1   1003 sub  $1, %bx
        1   1004 test $0, %bx
        1   ------ Interrupt ------  ------ Interrupt ------  
        1                            1004 test $0, %bx
        1                            1005 jgt .top
        1   ------ Interrupt ------  ------ Interrupt ------  
        1   1005 jgt .top
        1   1006 halt
        1   ----- Halt;Switch -----  ----- Halt;Switch -----  
        1   ------ Interrupt ------  ------ Interrupt ------  
        1                            1006 halt
        
可以看到，变量x的值由0变为。由于没有设置%bx，%bx的初值为0，所以两个进程都只执行一次。但是从上面的执行结果可以看出，由于没有互斥锁，进程0读取x的值(为0)，将其加1，此时进程0的%ax为1；然后进程1完成读读取加1写回的操作，此时x的值为1。再切换回进程0，进程0将自己的%ax(为1)写回x，x仍然是1.

*7.*

结果和6类似，x最终为1

*8.*

结果同上，依然为1

*9.*

打印程序输出如下：

     2000          Thread 0                Thread 1         
        0   
        0   1000 mov 2000, %ax
        0   ------ Interrupt ------  ------ Interrupt ------  
        0                            1000 mov 2000, %ax
        0   ------ Interrupt ------  ------ Interrupt ------  
        0   1001 add $1, %ax
        0   ------ Interrupt ------  ------ Interrupt ------  
        0                            1001 add $1, %ax
        0   ------ Interrupt ------  ------ Interrupt ------  
        1   1002 mov %ax, 2000
        1   ------ Interrupt ------  ------ Interrupt ------  
        1                            1002 mov %ax, 2000
        1   ------ Interrupt ------  ------ Interrupt ------  
        1   1003 sub  $1, %bx
        1   ------ Interrupt ------  ------ Interrupt ------  
        1                            1003 sub  $1, %bx
        1   ------ Interrupt ------  ------ Interrupt ------  
        1   1004 test $0, %bx
        1   ------ Interrupt ------  ------ Interrupt ------  
        1                            1004 test $0, %bx
        1   ------ Interrupt ------  ------ Interrupt ------  
        1   1005 jgt .top
        1   ------ Interrupt ------  ------ Interrupt ------  
        1                            1005 jgt .top
        1   ------ Interrupt ------  ------ Interrupt ------  
        1   1006 halt
        1   ----- Halt;Switch -----  ----- Halt;Switch -----  
        1   ------ Interrupt ------  ------ Interrupt ------  
        1                            1006 halt

可以看出，由于没有互斥锁，两个进程依次读取了值为0的x，然后各自加1后写回，所以x的值仍然为1

3. （spoc） 了解software-based lock, hardware-based lock, [software-hardware-lock代码目录](https://github.com/chyyuu/ucore_lab/tree/master/related_info/lab7/software-hardware-locks)

  - 理解flag.s,peterson.s,test-and-set.s,ticket.s,test-and-test-and-set.s 请通过x86.py分析这些代码是否实现了锁机制？请给出你的实验过程和结论说明。能否设计新的硬件原子操作指令Compare-And-Swap,Fetch-And-Add？

*1.* flag.s

flag.s虽然利用flag的置位实现了一个锁机制，但是这个机制并不能总是保证临界区的原子操作。例如，执行下述语句：

    ./x86.py -p flag.s -t 2 -i 2 -c

打印结果如下：

           Thread 0                Thread 1         
    
    1000 mov  flag, %ax
    1001 test $0, %ax
    ------ Interrupt ------  ------ Interrupt ------  
                             1000 mov  flag, %ax
                             1001 test $0, %ax
    ------ Interrupt ------  ------ Interrupt ------  
    1002 jne  .acquire
    1003 mov  $1, flag
    ------ Interrupt ------  ------ Interrupt ------  
                             1002 jne  .acquire
                             1003 mov  $1, flag
    ------ Interrupt ------  ------ Interrupt ------  
    1004 mov  count, %ax
    1005 add  $1, %ax
    ------ Interrupt ------  ------ Interrupt ------  
                             1004 mov  count, %ax
                             1005 add  $1, %ax
    ------ Interrupt ------  ------ Interrupt ------  
    1006 mov  %ax, count
    1007 mov  $0, flag
    ------ Interrupt ------  ------ Interrupt ------  
                             1006 mov  %ax, count
                             1007 mov  $0, flag
    ------ Interrupt ------  ------ Interrupt ------  
    1008 sub  $1, %bx
    1009 test $0, %bx
    ------ Interrupt ------  ------ Interrupt ------  
                             1008 sub  $1, %bx
                             1009 test $0, %bx
    ------ Interrupt ------  ------ Interrupt ------  
    1010 jgt .top
    1011 halt
    ----- Halt;Switch -----  ----- Halt;Switch -----  
    ------ Interrupt ------  ------ Interrupt ------  
                             1010 jgt .top
                             1011 halt
    
可以看到，flag初始为0，进程1读取到flag为0，但是在其将flag置为1之前，就被打断，进程2随后页读取了flag为0，这样二者都同时对count进行了操作。

*2.*peterson.s

peterson.s实现了peterson算法，有效地实现了锁机制。执行命令：

    ./x86.py -p peterson.s -t 2 -i 2 -a bx=0,bx=1 -c

可以看到程序的输出如下：

           Thread 0                Thread 1         
    
    1000 lea flag, %fx
    1001 mov %bx, %cx
    ------ Interrupt ------  ------ Interrupt ------  
                             1000 lea flag, %fx
                             1001 mov %bx, %cx
    ------ Interrupt ------  ------ Interrupt ------  
    1002 neg %cx
    1003 add $1, %cx
    ------ Interrupt ------  ------ Interrupt ------  
                             1002 neg %cx
                             1003 add $1, %cx
    ------ Interrupt ------  ------ Interrupt ------  
    1004 mov $1, 0(%fx,%bx,4)
    1005 mov %cx, turn
    ------ Interrupt ------  ------ Interrupt ------  
                             1004 mov $1, 0(%fx,%bx,4)
                             1005 mov %cx, turn
    ------ Interrupt ------  ------ Interrupt ------  
    1006 mov 0(%fx,%cx,4), %ax
    1007 test $1, %ax
    ------ Interrupt ------  ------ Interrupt ------  
                             1006 mov 0(%fx,%cx,4), %ax
                             1007 test $1, %ax
    ------ Interrupt ------  ------ Interrupt ------  
    1008 jne .fini
    1009 mov turn, %ax
    ------ Interrupt ------  ------ Interrupt ------  
                             1008 jne .fini
                             1009 mov turn, %ax
    ------ Interrupt ------  ------ Interrupt ------  
    1010 test %cx, %ax
    1011 je .spin1
    ------ Interrupt ------  ------ Interrupt ------  
                             1010 test %cx, %ax
                             1011 je .spin1
    ------ Interrupt ------  ------ Interrupt ------  
    1012 mov count, %ax
    1013 add $1, %ax
    ------ Interrupt ------  ------ Interrupt ------  
                             1006 mov 0(%fx,%cx,4), %ax
                             1007 test $1, %ax
    ------ Interrupt ------  ------ Interrupt ------  
    1014 mov %ax, count
    1015 mov $0, 0(%fx,%bx,4)
    ------ Interrupt ------  ------ Interrupt ------  
                             1008 jne .fini
                             1009 mov turn, %ax
    ------ Interrupt ------  ------ Interrupt ------  
    1016 mov %cx, turn
    1017 halt
    ----- Halt;Switch -----  ----- Halt;Switch -----  
    ------ Interrupt ------  ------ Interrupt ------  
                             1010 test %cx, %ax
                             1011 je .spin1
    ------ Interrupt ------  ------ Interrupt ------  
                             1006 mov 0(%fx,%cx,4), %ax
                             1007 test $1, %ax
    ------ Interrupt ------  ------ Interrupt ------  
                             1008 jne .fini
                             1012 mov count, %ax
    ------ Interrupt ------  ------ Interrupt ------  
                             1013 add $1, %ax
                             1014 mov %ax, count
    ------ Interrupt ------  ------ Interrupt ------  
                             1015 mov $0, 0(%fx,%bx,4)
                             1016 mov %cx, turn
    ------ Interrupt ------  ------ Interrupt ------  
                             1017 halt
                             
可以看出，一开始进程0和进程1都将flag[self]设为了1，都申请了资源。由peterson算法，有下述语句：

    mov %cx, turn           # turn       = 1 - self
    
两个进程都把turn交给了对方，则先设置turn的进程获得资源。从上面的程序输出页可以看出，此时进程0获得资源先执行，进程1则一直在spin1和spin2之间循环，等待进程了0释放资源。


*3.*test-and-set.s

test-and-set.s也较好地实现了锁机制。从下面的汇编语句中可以看出：

    .acquire
    mov  $1, %ax        
    xchg %ax, mutex     # atomic swap of 1 and mutex
    test $0, %ax        # if we get 0 back: lock is free!
    jne  .acquire       # if not, try again
    
    # critical section
    mov  count, %ax     # get the value at the address
    add  $1, %ax        # increment it
    mov  %ax, count     # store it back

是利用一个互斥锁，只有取到互斥锁的值为0，才能获得资源。利用原子操作xchg，取锁的值的同时将其设为1，因为xchg是原子操作，所以能保证只有一个进程可从mutex中获得0.实际程序执行的部分结果如下：

    ax          Thread 0                Thread 1         
    
        0   
        1   1000 mov  $1, %ax
        0   1001 xchg %ax, mutex
        0   ------ Interrupt ------  ------ Interrupt ------  
        1                            1000 mov  $1, %ax
        1                            1001 xchg %ax, mutex
        0   ------ Interrupt ------  ------ Interrupt ------  
        0   1002 test $0, %ax
        0   1003 jne  .acquire
        1   ------ Interrupt ------  ------ Interrupt ------  
        1                            1002 test $0, %ax
        1                            1003 jne  .acquire
        0   ------ Interrupt ------  ------ Interrupt ------  
        0   1004 mov  count, %ax
        1   1005 add  $1, %ax
        1   ------ Interrupt ------  ------ Interrupt ------  
        1                            1000 mov  $1, %ax
        1                            1001 xchg %ax, mutex
        1   ------ Interrupt ------  ------ Interrupt ------  
        1   1006 mov  %ax, count
        1   1007 mov  $0, mutex
        1   ------ Interrupt ------  ------ Interrupt ------  
        1                            1002 test $0, %ax
        1                            1003 jne  .acquire
        1   ------ Interrupt ------  ------ Interrupt ------  
        1   1008 sub  $1, %bx
        1   1009 test $0, %bx
        1   ------ Interrupt ------  ------ Interrupt ------  
        1                            1000 mov  $1, %ax
        0                            1001 xchg %ax, mutex
        1   ------ Interrupt ------  ------ Interrupt ------  
        1   1010 jgt .top
        1   1011 halt
        0   ----- Halt;Switch -----  ----- Halt;Switch -----  
        0   ------ Interrupt ------  ------ Interrupt ------  
        0                            1002 test $0, %ax
        0                            1003 jne  .acquire
        0   ------ Interrupt ------  ------ Interrupt ------  
        1                            1004 mov  count, %ax
        2                            1005 add  $1, %ax

可以看到，进程0先获得互斥锁，进入了临界区。进程而则一直在.acquire部分循环等待进程1释放锁

*4.*ticket.s

ticket.s也实现了锁机制。其中利用了一条原子指令fetchadd。fetchadd %ax, ticket 即先将ticket中的值取到%ax中，再将该值加1后写入ticket。fetchadd指令执行完毕后，%ax中保存的是原来ticket中的值，而ticket中的值增加了以。执行命令：./x86.py -p ticket.s -t 2 -i 2 -R ax  -c，可以看到程序输出如下：

       ax          Thread 0                Thread 1         
    
        0   
        1   1000 mov $1, %ax
        0   1001 fetchadd %ax, ticket
        0   ------ Interrupt ------  ------ Interrupt ------  
        1                            1000 mov $1, %ax
        1                            1001 fetchadd %ax, ticket
        0   ------ Interrupt ------  ------ Interrupt ------  
        0   1002 mov turn, %cx
        0   1003 test %cx, %ax
        1   ------ Interrupt ------  ------ Interrupt ------  
        1                            1002 mov turn, %cx
        1                            1003 test %cx, %ax
        0   ------ Interrupt ------  ------ Interrupt ------  
        0   1004 jne .tryagain
        0   1005 mov  count, %ax
        1   ------ Interrupt ------  ------ Interrupt ------  
        1                            1004 jne .tryagain
        1                            1002 mov turn, %cx
        0   ------ Interrupt ------  ------ Interrupt ------  
        1   1006 add  $1, %ax
        1   1007 mov  %ax, count
        1   ------ Interrupt ------  ------ Interrupt ------  
        1                            1003 test %cx, %ax
        1                            1004 jne .tryagain
        1   ------ Interrupt ------  ------ Interrupt ------  
        1   1008 mov $1, %ax
        0   1009 fetchadd %ax, turn
        1   ------ Interrupt ------  ------ Interrupt ------  
        1                            1002 mov turn, %cx
        1                            1003 test %cx, %ax
        0   ------ Interrupt ------  ------ Interrupt ------  
        0   1010 sub  $1, %bx
        0   1011 test $0, %bx
        1   ------ Interrupt ------  ------ Interrupt ------  
        1                            1004 jne .tryagain
        1                            1005 mov  count, %ax
        0   ------ Interrupt ------  ------ Interrupt ------  
        0   1012 jgt .top
        0   1013 halt
        1   ----- Halt;Switch -----  ----- Halt;Switch -----  
        1   ------ Interrupt ------  ------ Interrupt ------  
        2                            1006 add  $1, %ax
        2                            1007 mov  %ax, count
        2   ------ Interrupt ------  ------ Interrupt ------  
        1                            1008 mov $1, %ax
        1                            1009 fetchadd %ax, turn
        1   ------ Interrupt ------  ------ Interrupt ------  
        1                            1010 sub  $1, %bx
        1                            1011 test $0, %bx
        1   ------ Interrupt ------  ------ Interrupt ------  
        1                            1012 jgt .top
        1                            1013 halt
        
初始时turn为0，ticket为0。进程0获取ticket为0，然后ticket变为1；进程1获取ticket为1，然后ticket变为2.此时，turn==0，所以进程0执行，进程1等待。进程0释放时，再次利用fetchadd将turn加1，此时turn为1.则进程1执行。



*5*.test-and-test-and-set.s

tets-and-test-and-set.s也实现了锁机制。虽然一开始的mov  mutex, %ax，两个进程获得的值都是0，但是后来利用原子操作xchg将mutex和ax交换，则必有一个进程获得0，一个进程获得1.程序部分输出如下：

     ax          Thread 0                Thread 1         
    
        0   
        0   1000 mov  mutex, %ax
        0   1001 test $0, %ax
        0   ------ Interrupt ------  ------ Interrupt ------  
        0                            1000 mov  mutex, %ax
        0                            1001 test $0, %ax
        0   ------ Interrupt ------  ------ Interrupt ------  
        0   1002 jne .acquire
        1   1003 mov  $1, %ax
        0   ------ Interrupt ------  ------ Interrupt ------  
        0                            1002 jne .acquire
        1                            1003 mov  $1, %ax
        1   ------ Interrupt ------  ------ Interrupt ------  
        0   1004 xchg %ax, mutex
        0   1005 test $0, %ax
        1   ------ Interrupt ------  ------ Interrupt ------  
        1                            1004 xchg %ax, mutex
        1                            1005 test $0, %ax
        0   ------ Interrupt ------  ------ Interrupt ------  
        0   1006 jne .acquire
        0   1007 mov  count, %ax
        1   ------ Interrupt ------  ------ Interrupt ------  
        1                            1006 jne .acquire
        1                            1000 mov  mutex, %ax
        0   ------ Interrupt ------  ------ Interrupt ------  
        1   1008 add  $1, %ax
        1   1009 mov  %ax, count
        1   ------ Interrupt ------  ------ Interrupt ------  
        1                            1001 test $0, %ax
        1                            1002 jne .acquire
        1   ------ Interrupt ------  ------ Interrupt ------  
        1   1010 mov  $0, mutex
        1   1011 sub  $1, %bx
        1   ------ Interrupt ------  ------ Interrupt ------  
        0                            1000 mov  mutex, %ax
        0                            1001 test $0, %ax
        1   ------ Interrupt ------  ------ Interrupt ------  
        1   1012 test $0, %bx
        1   1013 jgt .top
        0   ------ Interrupt ------  ------ Interrupt ------  
        0                            1002 jne .acquire
        1                            1003 mov  $1, %ax
        1   ------ Interrupt ------  ------ Interrupt ------  
        1   1014 halt
        1   ----- Halt;Switch -----  ----- Halt;Switch -----  
        0                            1004 xchg %ax, mutex
        0   ------ Interrupt ------  ------ Interrupt ------  
        0                            1005 test $0, %ax
        0                            1006 jne .acquire
        0   ------ Interrupt ------  ------ Interrupt ------  
        1                            1007 mov  count, %ax
        2                            1008 add  $1, %ax

  
  
Fetch-And-Add能够实现
  
```
Compare-And-Swap

int CompareAndSwap(int *ptr, int expected, int new) {
  int actual = *ptr;
  if (actual == expected)
    *ptr = new;
  return actual;
}
```

```
Fetch-And-Add

int FetchAndAdd(int *ptr) {
  int old = *ptr;
  *ptr = old + 1;
  return old;
}
```
