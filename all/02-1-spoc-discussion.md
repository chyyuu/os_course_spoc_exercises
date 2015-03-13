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
 1. Linux的系统调用有哪些？大致的功能分类有哪些？  (w2l1)

```
  + 采分点：说明了Linux的大致数量（上百个），说明了Linux系统调用的主要分类（文件操作，进程管理，内存管理等）
  - 答案没有涉及上述两个要点；（0分）
  - 答案对上述两个要点中的某一个要点进行了正确阐述（1分）
  - 答案对上述两个要点进行了正确阐述（2分）
  - 答案除了对上述两个要点都进行了正确阐述外，还进行了扩展和更丰富的说明（3分）
 ```
> Linux有上百个系统调用，大致的功能分类有：进程管理，文件操作，系统控制，内存管理，网络管理，socket控制，用户管理，进程间通信等. 举例如下:fork 创建一个新进程 clone 按指定条件创建子进程.
open 打开文件 creat 创建新文件
_sysctl 读/写系统参数 acct 启用或禁止进程记账
mlock 内存页面加锁 munlock 内存页面解锁
gethostid 获取主机标识号 sethostid 设置主机标识号
socket 建立socket bind 绑定socket到端口
getuid 获取用户标识号 setuid 设置用户标志号
sigpending 为指定的被阻塞信号设置队列 sigsuspend 挂起进程等待特定信号
 
 1. 以ucore lab8的answer为例，uCore的系统调用有哪些？大致的功能分类有哪些？(w2l1)
 
 ```
  + 采分点：说明了ucore的大致数量（二十几个），说明了ucore系统调用的主要分类（文件操作，进程管理，内存管理等）
  - 答案没有涉及上述两个要点；（0分）
  - 答案对上述两个要点中的某一个要点进行了正确阐述（1分）
  - 答案对上述两个要点进行了正确阐述（2分）
  - 答案除了对上述两个要点都进行了正确阐述外，还进行了扩展和更丰富的说明（3分）
 ```
 > ucore有二十几个系统调用，大致的功能分类有进程管理，文件操作，内存管理等.
   进程管理: sys_exit, sys_fork, sys_wait, sys_exec, sys_yield, sys_getpid, sys_lab6_set_priority, sys_sleep.
   进程间通信: sys_kill.
   系统控制:  sys_gettime.
   文件管理: sys_open, sys_close, sys_read, sys_write, sys_seek, sys_fstat, sys_fsync, sys_getcwd, sys_getdirentry,                       sys_dup。
## 3.4 linux系统调用分析

 1. 通过分析[lab1_ex0](https://github.com/chyyuu/ucore_lab/blob/master/related_info/lab1/lab1-ex0.md)了解Linux应用的系统调用编写和含义。(w2l1)
 

 ```
  + 采分点：说明了objdump，nm，file的大致用途，说明了系统调用的具体含义
  - 答案没有涉及上述两个要点；（0分）
  - 答案对上述两个要点中的某一个要点进行了正确阐述（1分）
  - 答案对上述两个要点进行了正确阐述（2分）
  - 答案除了对上述两个要点都进行了正确阐述外，还进行了扩展和更丰富的说明（3分）
 
 ```
 > 分析过程使用了objdump，nm，file三个工具，objdump用于反汇编，nm用于列出目标文件符号清单, file用于文件处理。 系统调用是指用户程序直接或间接通过系统调用接口使用操作系统提供的服务，调用时通过中断进入内核态，执行完相关代码后将结果返回用户程序。
 
 1. 通过调试[lab1_ex1](https://github.com/chyyuu/ucore_lab/blob/master/related_info/lab1/lab1-ex1.md)了解Linux应用的系统调用执行过程。(w2l1)
 

 ```
  + 采分点：说明了strace的大致用途，说明了系统调用的具体执行过程（包括应用，CPU硬件，操作系统的执行过程）
  - 答案没有涉及上述两个要点；（0分）
  - 答案对上述两个要点中的某一个要点进行了正确阐述（1分）
  - 答案对上述两个要点进行了正确阐述（2分）
  - 答案除了对上述两个要点都进行了正确阐述外，还进行了扩展和更丰富的说明（3分）
 ```
 > strace用来跟踪进程执行时的系统调用和所接收的信号，跟踪进程产生的系统调用，包括每一系统调用所执行的时间等.      Linux系统调用执行过程：应用程序调用系统调用接口，中断服务程序根据中断号转系统调用，系统调用根据函数号执行相应函数，函数执行完毕返回用户程序.
 
## 3.5 ucore系统调用分析
 1. ucore的系统调用中参数传递代码分析。
 1. ucore的系统调用中返回结果的传递代码分析。
 1. 以ucore lab8的answer为例，分析ucore 应用的系统调用编写和含义。
 1. 以ucore lab8的answer为例，尝试修改并运行ucore OS kernel代码，使其具有类似Linux应用工具`strace`的功能，即能够显示出应用程序发出的系统调用，从而可以分析ucore应用的系统调用执行过程。
 
## 3.6 请分析函数调用和系统调用的区别
 1. 请从代码编写和执行过程来说明。
   1. 说明`int`、`iret`、`call`和`ret`的指令准确功能

> int和iret用在 系统调用里面, call和ret用在函数里面, 都是处理完后结果返回给用户的命令.
