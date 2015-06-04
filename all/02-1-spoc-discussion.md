#lec 3 SPOC Discussion

## 第三讲 启动、中断、异常和系统调用-思考题

## 3.3 中断、异常和系统调用比较
 1. Linux的系统调用有哪些？大致的功能分类有哪些？  (w2l1)
    进程控制：
	fork：创建一个新进程
	exit：终止进程
	ptrace：进程追踪
	wait：等待子进程结束
	pause：挂起进程 等待信号
    文件系统控制：
	open：打开文件
	read：读取文件
	close：关闭文件描述字
	pwrite：对文件随机写
	chdir：改变当前目录
	mkdir：创建新目录
	stat：取文件信息
    系统控制：
	reboot：重新启动
	time：取得系统时间
	init_module：初始化模块
	sysinfo：取得系统信息
    内存管理：
	brk：改变数据段的分配
	mlock：内存页面加锁
	sync：将内存缓冲区数据写回硬盘
	getpagesize：获取页面大小
    网络管理：
	getdomainname：取域名
    socket控制：
	socket：建立socket
    用户管理：
	getuid：获取用户标号
    进程间通信：
	ipc：进程间通信总控制调用

<<<<<<< HEAD
```
  + 采分点：说明了Linux的大致数量（上百个），说明了Linux系统调用的主要分类（文件操作，进程管理，内存管理等）
  - 答案没有涉及上述两个要点；（0分）
  - 答案对上述两个要点中的某一个要点进行了正确阐述（1分）
  - 答案对上述两个要点进行了正确阐述（2分）
  - 答案除了对上述两个要点都进行了正确阐述外，还进行了扩展和更丰富的说明（3分）
 ```
 
 1. 以ucore lab8的answer为例，uCore的系统调用有哪些？大致的功能分类有哪些？(w2l1)
    文件操作：
	open close read write readdir opendir closedir
    进程管理：
	sched_class_enqueue run_timer_list sched_class_pick_next
    内存管理：
	pmm_init pmm_manager vmm_init ide_init swap_init
    
=======
> Linux的系统调用的数量从2.4的190个到2.6的300多个，越来越多的系统调用数一方面大大增强了Linux对应用的支持，另外一方面也给应用的移植带来了很大的挑战。<br>
  大致的功能分类有进程控制、文件系统控制、系统控制、内存管理、网络管理、socket控制、用户管理、进程间通信等。
>>>>>>> 790bc39b948c60181c85f58654a11682d075cbe2
 
 2. 以ucore lab8的answer为例，uCore的系统调用有哪些？大致的功能分类有哪些？(w2l1)

> uCore的系统调用共22个，包括exit、fork、wait、exec、yield、kill、getpid、putc、pgdir、gettime、lab6_set_priority、sleep、open、close、read、write、seek、fstat、fsync、getcwd、getdirentry、dup。<br>
  大致的功能分类有进程管理、内存管理、文件操作等。
 
## 3.4 linux系统调用分析
 1. 通过分析[lab1_ex0](https://github.com/chyyuu/ucore_lab/blob/master/related_info/lab1/lab1-ex0.md)了解Linux应用的系统调用编写和含义。(w2l1)
<<<<<<< HEAD
	objdump:对可执行文件进行反汇编
 	nm:用来列出目标文件的符号清单
 	file:确定文件类型
	系统调用：由操作系统实现提供的所有系统调用所构成的集合即程序接口或应用编程接口(Application Programming Interface，API)。是应用程序同系统之间的接口。
	主要功能是为管理硬件资源和为应用程序开发人员提供良好的环境来使应用程序具有更好的兼容性，为了达到这个目的，内核提供一系列具备预定功能的多内核函数，通过一组称为系统调用（system call)的接口呈现给用户。系统调用把应用程序的请求传给内核，调用相应的的内核函数完成所需的处理，将处理结果返回给应用程序。
 
```
  + 采分点：说明了objdump，nm，file的大致用途，说明了系统调用的具体含义
  - 答案没有涉及上述两个要点；（0分）
  - 答案对上述两个要点中的某一个要点进行了正确阐述（1分）
  - 答案对上述两个要点进行了正确阐述（2分）
  - 答案除了对上述两个要点都进行了正确阐述外，还进行了扩展和更丰富的说明（3分）
 
 ```
 
 1. 通过调试[lab1_ex1](https://github.com/chyyuu/ucore_lab/blob/master/related_info/lab1/lab1-ex1.md)了解Linux应用的系统调用执行过程。(w2l1)
 	strace:常用来跟踪进程执行时的系统调用和所接收的信号。在Linux世界，进程不能直接访问硬件设备，当进程需要访问硬件设备(比如读取磁盘文件，接收网络数据等等)时，必须由用户态模式切换至内核态模式，通 过系统调用访问硬件设备。strace可以跟踪到一个进程产生的系统调用,包括参数，返回值，执行消耗的时间。
	用户程序—>C库（即API）—>system_call—>系统调用服务例程—>内核程序
	（系统调用是通过软中断指令实现的。）
 ```
  + 采分点：说明了strace的大致用途，说明了系统调用的具体执行过程（包括应用，CPU硬件，操作系统的执行过程）
  - 答案没有涉及上述两个要点；（0分）
  - 答案对上述两个要点中的某一个要点进行了正确阐述（1分）
  - 答案对上述两个要点进行了正确阐述（2分）
  - 答案除了对上述两个要点都进行了正确阐述外，还进行了扩展和更丰富的说明（3分）
 ```
 
## 3.5 ucore系统调用分析
 1. ucore的系统调用中参数传递代码分析。
 1. ucore的系统调用中返回结果的传递代码分析。
 1. 以ucore lab8的answer为例，分析ucore 应用的系统调用编写和含义。
 1. 以ucore lab8的answer为例，尝试修改并运行ucore OS kernel代码，使其具有类似Linux应用工具`strace`的功能，即能够显示出应用程序发出的系统调用，从而可以分析ucore应用的系统调用执行过程。
 
## 3.6 请分析函数调用和系统调用的区别
 1. 请从代码编写和执行过程来说明。
   1. 说明`int`、`iret`、`call`和`ret`的指令准确功能
 
=======

> objdump用于反汇编目标文件或者可执行文件。 <br>
  nm用于显示关于对象文件、可执行文件以及对象文件库里的符号信息。<br>
  file用于检测文件类型。<br>
  系统调用本质是向应用程序提供一些基本的函数的接口，这些函数都位于操作系统的内核。
 
 2. 通过调试[lab1_ex1](https://github.com/chyyuu/ucore_lab/blob/master/related_info/lab1/lab1-ex1.md)了解Linux应用的系统调用执行过程。(w2l1)

> strace常用来跟踪进程执行时的系统调用和所接收的信号。在Linux中，进程不能直接访问硬件设备，当进程需要访问硬件设备(比如读取磁盘文件、接收网络数据等)时，必须由用户态模式切换至内核态模式，通过系统调用访问硬件设备。strace可以跟踪到一个进程产生的系统调用，包括参数、返回值、执行消耗的时间。<br>
  系统调用大致执行过程：用户程序 ----> C库（即API）：INT 0x80 ----> system_call ----> 系统调用服务例程 ----> 内核程序 ----> 返回。
>>>>>>> 790bc39b948c60181c85f58654a11682d075cbe2
