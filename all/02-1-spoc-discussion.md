#lec 3 SPOC Discussion

## 第三讲 启动、中断、异常和系统调用-思考题

## 3.3 中断、异常和系统调用比较
 1. Linux的系统调用有哪些？大致的功能分类有哪些？  (w2l1)

> Linux的系统调用的数量从2.4的190个到2.6的300多个，越来越多的系统调用数一方面大大增强了Linux对应用的支持，另外一方面也给应用的移植带来了很大的挑战。<br>
  大致的功能分类有进程控制、文件系统控制、系统控制、内存管理、网络管理、socket控制、用户管理、进程间通信等。
 
 2. 以ucore lab8的answer为例，uCore的系统调用有哪些？大致的功能分类有哪些？(w2l1)

> uCore的系统调用共22个，包括exit、fork、wait、exec、yield、kill、getpid、putc、pgdir、gettime、lab6_set_priority、sleep、open、close、read、write、seek、fstat、fsync、getcwd、getdirentry、dup。<br>
  大致的功能分类有进程管理、内存管理、文件操作等。
 
## 3.4 linux系统调用分析
 1. 通过分析[lab1_ex0](https://github.com/chyyuu/ucore_lab/blob/master/related_info/lab1/lab1-ex0.md)了解Linux应用的系统调用编写和含义。(w2l1)

> objdump用于反汇编目标文件或者可执行文件。 <br>
  nm用于显示关于对象文件、可执行文件以及对象文件库里的符号信息。<br>
  file用于检测文件类型。<br>
  系统调用本质是向应用程序提供一些基本的函数的接口，这些函数都位于操作系统的内核。
 
 2. 通过调试[lab1_ex1](https://github.com/chyyuu/ucore_lab/blob/master/related_info/lab1/lab1-ex1.md)了解Linux应用的系统调用执行过程。(w2l1)

> strace常用来跟踪进程执行时的系统调用和所接收的信号。在Linux中，进程不能直接访问硬件设备，当进程需要访问硬件设备(比如读取磁盘文件、接收网络数据等)时，必须由用户态模式切换至内核态模式，通过系统调用访问硬件设备。strace可以跟踪到一个进程产生的系统调用，包括参数、返回值、执行消耗的时间。<br>
  系统调用大致执行过程：用户程序 ----> C库（即API）：INT 0x80 ----> system_call ----> 系统调用服务例程 ----> 内核程序 ----> 返回。
