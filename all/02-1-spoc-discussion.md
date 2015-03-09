#lec 3 SPOC Discussion

## 第三讲 启动、中断、异常和系统调用-思考题

## 3.1 BIOS
 1. 比较UEFI和BIOS的区别。
 1. 描述PXE的大致启动流程。

## 3.2 系统启动流程
 1. 了解NTLDR的启动流程。windows
 1. 了解GRUB的启动流程。linux
 1. 比较NTLDR和GRUB的功能有差异。
 1. 了解u-boot的功能。

## 3.3 中断、异常和系统调用比较
 1. 举例说明Linux中有哪些中断，哪些异常？ 
 1. Linux的系统调用有哪些？大致的功能分类有哪些？
    linux的系统调用总共大约250个，其中我们经常使用的如read write close open等文件操作类。
    大的分类的话，可分为：
    控制硬件：即硬件资源的调用
    系统状态设置和内核数据读取：设置系统状态，开关内核服务等
    进程管理
    细分的话，可以分为如下几类：
    1 进程控制 for,kclone,exit等
    2 文件操作 open close read等
    3 文件系统操作 stat chown等
    4 系统控制 ioctl iopl等
    5 内存管理 mlock mmap等
    6 网络管理 gethostid等
    7 用户管理 geteuid等
    8 进程通信管理等

```
  + 采分点：说明了Linux的大致数量（上百个），说明了Linux系统调用的主要分类（文件操作，进程管理，内存管理等）
  - 答案没有涉及上述两个要点；（0分）
  - 答案对上述两个要点中的某一个要点进行了正确阐述（1分）
  - 答案对上述两个要点进行了正确阐述（2分）
  - 答案除了对上述两个要点都进行了正确阐述外，还进行了扩展和更丰富的说明（3分）
 ```
 
 1. 以ucore lab8的answer为例，uCore的系统调用有哪些？大致的功能分类有哪些？(w2l1)
 在lab8/kern/syscall里可以看到，一共有22个系统调用，如下：
sys_exit,
sys_fork,
sys_wait,
sys_exec,
sys_yield,
sys_kill,
sys_getpid,
sys_putc,
sys_pgdir,
sys_gettime,
sys_lab6_set_priority,
sys_sleep,
sys_open,
sys_close,
sys_read,
sys_write,
sys_seek,
sys_fstat,
sys_fsync,
sys_getcwd,
sys_getdirentry,
sys_dup,
分类大致也就是文件操作，进程管理，内存管理这些

 ```
  + 采分点：说明了ucore的大致数量（二十几个），说明了ucore系统调用的主要分类（文件操作，进程管理，内存管理等）
  - 答案没有涉及上述两个要点；（0分）
  - 答案对上述两个要点中的某一个要点进行了正确阐述（1分）
  - 答案对上述两个要点进行了正确阐述（2分）
  - 答案除了对上述两个要点都进行了正确阐述外，还进行了扩展和更丰富的说明（3分）
 ```
 
## 3.4 linux系统调用分析
 1. 通过分析[lab1_ex0](https://github.com/chyyuu/ucore_lab/blob/master/related_info/lab1/lab1-ex0.md)了解Linux应用的系统调用编写和含义。(w2l1)

file 是检测文件类型的命令，可以查看文件的类型和组织方式的信息
执行命令 file lab1-ex0.exe 可看到打印出的内容为：
lab1-ex0.exe: ELF 64-bit LSB  executable, x86-64, version 1 (SYSV), dynamically linked (uses shared libs), for GNU/Linux 2.6.24, BuildID[sha1]=893580e2a35a7a0f4175444a439d85c84961f263, not stripped
可看出lab1-ex0.exe是ELF的文件格式，64为的可执行文件，动态链接库等等信息

nm命令用于显示关于对象文件、可执行文件以及对象文件库里的符号信息
一些常用的参数如下：
1、"-A"，列出符号名的时候同时显示来自于哪个文件。

这在同时列出多个文件（比如一个链接库）的符号时比较有用
2、"-a"，列出所有符号

这将会把调试符号也列出来。默认状态下调试符号不会被列出。
3、"-l"，列出符号在源代码中对应的行号

指定这个参数后，nm将利用调试信息找出文件名以及符号的行号。对于一个已定义符号，将会找出这个符号定义的行号，对于未定义符号,显示为空
4、"-n"，根据符号的地址来排序

默认是按符号名称的字母顺序排序的
5、"-u"，只列出未定义符号

例如执行命令  nm -l lab1-ex0.exe
可看到打印出相应的符号信息：
000000000000000b a SYS_execve
0000000000000001 a SYS_exit
0000000000000002 a SYS_fork
0000000000000013 a SYS_lseek
000000000000005a a SYS_mmap
000000000000005b a SYS_munmap
0000000000000005 a SYS_open
0000000000000066 a SYS_socketcal
等 a表示这是 	Local absolute 符号。同时还可看到在lab1-ex0.s中的符号main
0000000000601045 D main	/home/yxy/ucore_lab/related_info/lab1/lab1-ex0.s:8
显示是在元文件的第八行




 ```
  + 采分点：说明了objdump，nm，file的大致用途，说明了系统调用的具体含义
  - 答案没有涉及上述两个要点；（0分）
  - 答案对上述两个要点中的某一个要点进行了正确阐述（1分）
  - 答案对上述两个要点进行了正确阐述（2分）
  - 答案除了对上述两个要点都进行了正确阐述外，还进行了扩展和更丰富的说明（3分）
 
 ```
 
 1. 通过调试[lab1_ex1](https://github.com/chyyuu/ucore_lab/blob/master/related_info/lab1/lab1-ex1.md)了解Linux应用的系统调用执行过程。(w2l1)
 

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
 1. 以ucore lab8的answer为例，尝试修改并运行代码，分析ucore应用的系统调用执行过程。
 
## 3.6 请分析函数调用和系统调用的区别
 1. 请从代码编写和执行过程来说明。
   1. 说明`int`、`iret`、`call`和`ret`的指令准确功能
 
