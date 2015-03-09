#lec 3 SPOC Discussion

## 第三讲 启动、中断、异常和系统调用-思考题

## 3.1 BIOS
 1. 比较UEFI和BIOS的区别。

UEFI是一个统一的标准，而BIOS会因机器而异。UEFI功能更强大，首先它具备文件系统的支持，能够直接读取FAT分区中的文件；其次，可开发出直接在UEFI下运行的应用程序（.efi文件）。

 1. 描述PXE的大致启动流程。

## 3.2 系统启动流程
 1. 了解NTLDR的启动流程。
 1. 了解GRUB的启动流程。
 1. 比较NTLDR和GRUB的功能有差异。
 1. 了解u-boot的功能。

## 3.3 中断、异常和系统调用比较
 1. 举例说明Linux中有哪些中断，哪些异常？
 1. Linux的系统调用有哪些？大致的功能分类有哪些？  (w2l1)

Linux的系统调用有read, write等300多个，大致可分为进程控制，文件系统控制，系统控制，内存管理，网络管理，socket控制，用户管理，进程间通信8个类别。
 
 1. 以ucore lab8的answer为例，uCore的系统调用有哪些？大致的功能分类有哪些？(w2l1)

ucore的系统调用有read, write等22个，大致可分为进程控制，文件系统控制，系统控制，内存管理，进程间通信5个类别。
 
## 3.4 linux系统调用分析
 1. 通过分析[lab1_ex0](https://github.com/chyyuu/ucore_lab/blob/master/related_info/lab1/lab1-ex0.md)了解Linux应用的系统调用编写和含义。(w2l1)

objdump可以用来反编译，nm可以用来列出目标文件的符号清单，file可以用来查看文件类型。linux通过int 0x80指令来实现系统调用，参数和返回值则是eax, ..., edx等寄存器上的值。
 
 1. 通过调试[lab1_ex1](https://github.com/chyyuu/ucore_lab/blob/master/related_info/lab1/lab1-ex1.md)了解Linux应用的系统调用执行过程。(w2l1)
 
strace可以用来跟踪进程执行时的系统调用和所接收的信号。按照要求执行后，可以看到一个系统调用的列表，详细列出了这个hello world程序运行时的系统调用过程。
 
## 3.5 ucore系统调用分析
 1. ucore的系统调用中参数传递代码分析。
 1. ucore的系统调用中返回结果的传递代码分析。
 1. 以ucore lab8的answer为例，分析ucore 应用的系统调用编写和含义。
 1. 以ucore lab8的answer为例，尝试修改并运行ucore OS kernel代码，使其具有类似Linux应用工具`strace`的功能，即能够显示出应用程序发出的系统调用，从而可以分析ucore应用的系统调用执行过程。
 
## 3.6 请分析函数调用和系统调用的区别
 1. 请从代码编写和执行过程来说明。
   1. 说明`int`、`iret`、`call`和`ret`的指令准确功能
 
