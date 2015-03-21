#lec 3 SPOC Discussion

## 第三讲 启动、中断、异常和系统调用-思考题

## 3.1 BIOS
 1. 比较UEFI和BIOS的区别。
 1. 描述PXE的大致启动流程。

## 3.2 系统启动流程
 1. 了解NTLDR的启动流程。
 1. 了解GRUB的启动流程。
 1. 比较NTLDR和GRUB的功能有差异。
 1. 了解u-boot的功能。

## 3.3 中断、异常和系统调用比较
 1. 举例说明Linux中有哪些中断，哪些异常？
 	中断:NMI中断、边界监测中断、缺段中断
 	异常：调试异常、堆栈异常、一般保护异常、页异常
 1. Linux的系统调用有哪些？大致的功能分类有哪些？  (w2l1)
 		Linux的系统调用有二百多个，不一一列举
		大致功能分为：
			进程控制：fock、clone、execve、exit等
			文件系统控制：fcnt1、open、creat等
			系统控制：ioct1、acct、setrusage、uselib等
			内存管理：brk、sbrk、mlock、nmap等
			网络管理：gethostid、gethostname、sethostid等
			socket控制：socketcall、connect、bind、accept等
			用户管理：getuid、setuid、setegid等
			进程间通信：ipc
```
  + 采分点：说明了Linux的大致数量（上百个），说明了Linux系统调用的主要分类（文件操作，进程管理，内存管理等）
  - 答案没有涉及上述两个要点；（0分）
  - 答案对上述两个要点中的某一个要点进行了正确阐述（1分）
  - 答案对上述两个要点进行了正确阐述（2分）
  - 答案除了对上述两个要点都进行了正确阐述外，还进行了扩展和更丰富的说明（3分）
 ```
 
 1. 以ucore lab8的answer为例，uCore的系统调用有哪些？大致的功能分类有哪些？(w2l1)
 
 ```文件操作：SYS_open SYS_close SYS_read SYS_write SYS_seek SYS_fstat SYS_fsync SYS_getcwd SYS_getdirentry
	进程管理：SYS_exit SYS_fork SYS_wait SYS_yield SYS_kill SYS_getpid SYS_lab6_set_priority SYS_sleep SYS_exec 
	内存管理：SYS_pgdir 其它：SYS_putc（输出） SYS_gettime（获取时间）SYS_dup（输出输入重定向）
	
  + 采分点：说明了ucore的大致数量（二十几个），说明了ucore系统调用的主要分类（文件操作，进程管理，内存管理等）
  - 答案没有涉及上述两个要点；（0分）
  - 答案对上述两个要点中的某一个要点进行了正确阐述（1分）
  - 答案对上述两个要点进行了正确阐述（2分）
  - 答案除了对上述两个要点都进行了正确阐述外，还进行了扩展和更丰富的说明（3分）
 ```
 
## 3.4 linux系统调用分析
 1. 通过分析[lab1_ex0](https://github.com/chyyuu/ucore_lab/blob/master/related_info/lab1/lab1-ex0.md)了解Linux应用的系统调用编写和含义。(w2l1)
 objdump可以显示object文件各个section的数据、汇编代码、反汇编结果、对应的高级语言的代码（需要加入调试信息）等等。 
 nm可以打印object文件中的符号表。 
 file可以输出有关文件类型的信息。
 
 系统调用是用户程序与操作系统交互的接口，本例中使用的是write系统调用，作用是向指定文件描述符写入数据。

 ```
  + 采分点：说明了objdump，nm，file的大致用途，说明了系统调用的具体含义
  - 答案没有涉及上述两个要点；（0分）
  - 答案对上述两个要点中的某一个要点进行了正确阐述（1分）
  - 答案对上述两个要点进行了正确阐述（2分）
  - 答案除了对上述两个要点都进行了正确阐述外，还进行了扩展和更丰富的说明（3分）
 
 ```
 
 1. 通过调试[lab1_ex1](https://github.com/chyyuu/ucore_lab/blob/master/related_info/lab1/lab1-ex1.md)了解Linux应用的系统调用执行过程。(w2l1)
 	strace常用来跟踪进程执行时的系统调用和所接收的信号。
 	moocos-> strace -c ./lab1-ex1.exe
hello world
% time     seconds  usecs/call     calls    errors syscall
------ ----------- ----------- --------- --------- ----------------
 25.42    0.000015           2         8           mmap
 22.03    0.000013           3         4           mprotect
 13.56    0.000008           8         1           write
 13.56    0.000008           8         1           munmap
 10.17    0.000006           3         2           open
  8.47    0.000005           2         3         3 access
  1.69    0.000001           1         1           read
  1.69    0.000001           0         3           fstat
  1.69    0.000001           1         1           brk
  1.69    0.000001           1         1           execve
  0.00    0.000000           0         2           close
  0.00    0.000000           0         1           arch_prctl
------ ----------- ----------- --------- --------- ----------------
100.00    0.000059                    28         3 total
mmap负责分配内存，
mprotect设置内存保护，
fstat返回文件的信息，
access检查文件权限，
open、close、read、write负责进行对文件相应的操作，
munmap映射文件到内存。
hello world程序中并没有操作磁盘上的文件，有文件操作的原因是Linux中各种设备都被抽象为文件，包括标准输入输出。
还有一些文件访问是访问库文件。execve执行的就是本文件，可执行文件是在strace执行的。brk(0)返回data段的地址，arch_prctl设置进程状态。

程序调用printf以后，printf最终会调用系统调用sys_write 此时CPU收到软中断0x80，转操作系统中断处理程序，0x80为系统调用 系统调用处理完成后，就会返回原程序

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
 
