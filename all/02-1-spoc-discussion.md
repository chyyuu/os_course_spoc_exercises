#lec 3 SPOC Discussion

## 第三讲 启动、中断、异常和系统调用-思考题

## 3.1 BIOS
 1. 比较UEFI和BIOS的区别。

 |  项目  |  legacy BIOS   |   UEFI   |
 |-------|---------|-----------------|
 | 工作原理 | 读取磁盘的0扇区，选择启动设备；工作在 16 位模式 | 存储在硬盘中的 ESP 分区中的一个 .efi 文件 |
 | 分区表   |  MBR 分区表，32 位  |  GUID 分区表，64 位  |
 | 启动速度 |   慢   |   快，甚至可以提前执行一些 UEFI app   |
 | 安全性   |  易受 Bootkit 攻击  |  可以有效避免 malware  |  

 1. 描述PXE的大致启动流程。

    - 客户端个人电脑开机后， 在 TCP/IP Bootrom 获得控制权之前先做自我测试；
    - Bootprom 送出 BOOTP/DHCP 要求以取得 IP；
    - 如果服务器收到个人电脑所送出的要求，就会送回 BOOTP/DHCP 回应，
    内容包括客户端的 IP 地址，预设网关，及开机映像文件。否则，服务器会忽略这个要求；  
    - Bootprom 由 TFTP 通讯协议从服务器下载开机映像文件;  
    - 个人电脑通过这个开机映像文件开机，这个开机文件可以只是单纯的开机程式也可以是操作系统；  
    - 远程客户端根据下载的文件启动机器。  

## 3.2 系统启动流程
 1. 了解NTLDR的启动流程。

```
 - Accesses the file system on the boot drive (either FAT or NTFS).  
 - If Windows was put in the hibernation state, the contents of hiberfil.sys are loaded into memory and the system resumes where it left off.  
 - Otherwise, reads boot.ini and prompts the user with the boot menu accordingly.  
 - If a non NT-based OS is selected, NTLDR loads the associated file listed in boot.ini and gives it control.  
 - If an NT-based OS is selected, NTLDR runs ntdetect.com, which gathers information about the computer's hardware.  
 - Starts Ntoskrnl.exe, passing to it the information returned by ntdetect.com.
 ```

 2. 了解GRUB的启动流程。

```
 - The first-stage loader (stage1) is loaded and executed either by the BIOS from the MBR or by another boot loader from the partition boot sector.  
 - If necessary, an intermediate stage loader (stage1.5) is loaded and executed by the first-stage loader.
 - The second-stage loader (stage2) is then loaded and executed. This displays the GRUB startup menu,
 which allows the user to choose an operating system or to examine and edit startup parameters.  
 - After an operating system is chosen, GRUB loads its kernel into memory and passes control to the kernel.
 Alternatively, GRUB can pass control of the boot process to another boot loader, using chain loading.
 ```

 3. 比较NTLDR和GRUB的功能有差异。

>  [Comparison of bootloaders](http://en.wikipedia.org/wiki/Comparison_of_boot_loaders)

 4. 了解u-boot的功能。

* 系统引导支持NFS挂载、RAMDISK(压缩或非压缩)形式的根文件系统；支持NFS挂载、从FLASH中引导压缩或非压缩系统内核；
* 基本辅助功能强大的操作系统接口功能；可灵活设置、传递多个关键参数给操作系统，适合系统在不同开发阶段的调试要求与产品发布，尤以Linux支持最为强劲；支持目标板环境参数多种存储方式，如FLASH、NVRAM、EEPROM；
* CRC32校验可校验FLASH中内核、RAMDISK镜像文件是否完好；
* 设备驱动串口、SDRAM、FLASH、以太网、LCD、NVRAM、EEPROM、键盘、USB、PCMCIA、PCI、RTC等驱动支持；
* 上电自检功能SDRAM、FLASH大小自动检测；SDRAM故障检测；CPU型号；
* 特殊功能XIP内核引导。

## 3.3 中断、异常和系统调用比较
 1. 举例说明Linux中有哪些中断，哪些异常？
 ```
 例如键盘事件就是中断，程序除 0 就是异常。
 以下是 Linux 的 table interrupt vector：
 ```

 | Identifier | Description  |
 |------------|--------------|
 | 0 | Divide error |
 | 1 | Debug exception |
 | 2 | Non-maskable interrupt |
 | 3 | Breakpoint |
 | 4 | Overflow |
 | 5 | Bounds check |
 | 6 | Invalid opcode |
 | 7 | Coprocessor not available |
 | 8 | Double fault |
 | 9 | (reserved) |
 | 10 | Invalid TSS |
 | 11 | Segment not present |
 | 12 | Stack exception |
 | 13 | General protection fault |
 | 14 | Page fault |
 | 15 | (reserved) |
 | 16 | Coprocessor error |
 | 17 | alignment error (80486) |
 | 18-31 | (reserved) |
 | 32-255 | External (HW) interrupts |

 1. Linux的系统调用有哪些？大致的功能分类有哪些？  (w2l1)
 ```
 上百个。包含下面几个分类：

    - 进程控制，创建新进程、中止进程等
    - 文件系统控制，包括文件读写、文件系统操作等
    - 系统控制，重新启动、获取系统资源情况、取得系统时钟等
    - 内存管理，内存页面加锁、解锁等
    - 网络管理，域名、主机号等
    - socket 控制，建立 sock ，绑定端口等
    - 用户管理，操作用户标号、组标号
    - 进程间通信，包括信号、消息、管道、信号量、共享内存等
 ```

```
  + 采分点：说明了Linux的大致数量（上百个），说明了Linux系统调用的主要分类（文件操作，进程管理，内存管理等）
  - 答案没有涉及上述两个要点；（0分）
  - 答案对上述两个要点中的某一个要点进行了正确阐述（1分）
  - 答案对上述两个要点进行了正确阐述（2分）
  - 答案除了对上述两个要点都进行了正确阐述外，还进行了扩展和更丰富的说明（3分）
 ```
 
 2. 以ucore lab8的answer为例，uCore的系统调用有哪些？大致的功能分类有哪些？(w2l1)
 
 |  编号  |   系统调用     |      作用        |       分类      |
 |-------|--------------|------------------|-----------------|
 |   1   |   sys_exit   |   中止进程        |     进程控制     |
 |   2   |   sys_fork   |   创建一个新进程   |     进程控制     |
 |   3   |   sys_wait   |   等待子进程中止   |     进程控制     |
 |   4   |   sys_exec   |   运行可执行文件   |     进程控制     |
 |   5   |   sys_yield  |   进程主动让出处理器,并将自己等候调度队列队尾 | 进程控制 |
 |   6   |   sys_kill   |   向进程发信号     |    进程间通信    |
 |   7   |   sys_getpid |   获取进程标识号    |    进程控制     |
 |   8   |   sys_putc   |   打印字符(?)      |    文件系统控制  |
 |   9   |   sys_pgdir  |   获得当前进程的路径 |    进程控制     |
 |   10  |   sys_gettime|   取时间          |     系统控制     |
 |   11  |   sys_lab6_set_priority |   设置优先级  | 进程控制   |
 |   12  |   sys_sleep  |   使进程睡眠指定的时间 |   进程控制     |
 |   13  |   sys_open   |   打开文件         |    文件系统控制  |
 |   14  |   sys_close  |   关闭文件         |    文件系统控制  |
 |   15  |   sys_read   |   读取文件         |    文件系统控制  |
 |   16  |   sys_write  |   写入文件         |    文件系统控制  |
 |   17  |   sys_seek   |   移动文件指针      |    文件系统控制  |
 |   18  |   sys_fstat  |   取文件状态信息    |    文件系统控制  |
 |   19  |   sys_fsync  |   把文件在内存中的部分写回磁盘 | 文件系统控制 |
 |   20  |   sys_getcwd |   取得当前工作目录   |    文件系统控制  |
 |   21  |   sys_getdirentry | 取 direntry  |    文件系统控制  |
 |   22  |   sys_dup    |   复制已打开的文件描述字|  文件系统控制  |  

 ```
  + 采分点：说明了ucore的大致数量（二十几个），说明了ucore系统调用的主要分类（文件操作，进程管理，内存管理等）
  - 答案没有涉及上述两个要点；（0分）
  - 答案对上述两个要点中的某一个要点进行了正确阐述（1分）
  - 答案对上述两个要点进行了正确阐述（2分）
  - 答案除了对上述两个要点都进行了正确阐述外，还进行了扩展和更丰富的说明（3分）
 ```
 
## 3.4 linux系统调用分析
 1. 通过分析[lab1_ex0](https://github.com/chyyuu/ucore_lab/blob/master/related_info/lab1/lab1-ex0.md)了解Linux应用的系统调用编写和含义。(w2l1)

> objdump是用查看目标文件或者可执行的目标文件的构成的GCC工具,常用于反汇编，常用命令包括：

> -d 反汇编目标文件中的需要执行指令的那些section；

> -f 显示文件头信息；

> -h 显示目标文件各个section的头部摘要信息；

> -s 显示指定section的全部内容；

> -t 显示文件的符号表。

> nm用于查看目标文件以及目标文件库的符号信息。

> file用于通过文件的头部信息来识别文件类型以及编码格式等信息。

> lab1-ex0.s文件执行过程为，将系统调用需要的参数（依次分别为系统调用号write、写入位置STDOUT、写入起始位置以及长度）存入相应寄存器，执行int中断指令来进行系统调用输出“hello world”。

> 系统调用的本质即为一组运行在操作系统内核中的一系列函数。这些函数是用户程序与操作系统之间的接口，提供了非常基本的功能。系统调用均是在内核（trap）中实现，其最终均转换为一组汇编命令，相对于其他函数而言，其效率是非常高的。

 ```
  + 采分点：说明了objdump，nm，file的大致用途，说明了系统调用的具体含义
  - 答案没有涉及上述两个要点；（0分）
  - 答案对上述两个要点中的某一个要点进行了正确阐述（1分）
  - 答案对上述两个要点进行了正确阐述（2分）
  - 答案除了对上述两个要点都进行了正确阐述外，还进行了扩展和更丰富的说明（3分）
 
 ```
 
 2. 通过调试[lab1_ex1](https://github.com/chyyuu/ucore_lab/blob/master/related_info/lab1/lab1-ex1.md)了解Linux应用的系统调用执行过程。(w2l1)

> strace用于跟踪进程执行时的系统调用和所接收的信号，包括参数，返回值，执行消耗的时间等。其中-c参数表示统计每一系统调用的执行时间,次数和出错的次数等信息。调试部分信息如下：

| % time | seconds | usecs/call | calls | errors | syscall |
|-------|-------|-------|-------|-------|-------|
| 22.41 | 0.000067 | 8 | 8 |  | mmap |

> more /proc/interrupts 的某行输出信息如下：

|  | CPU0 | CPU1 | CPU2 | CPU3 |  |  |
|-------|-------|-------|-------|-------|-------|-------|
| 40: | 450 | 151298 | 32 | 28 | PCI-MSI-edge | eth0 |

> 其中eth0表示来自网卡的中断，PCI-MSI-edge表示中断类型，40表示中断号，其它数字表示各核的执行的中断次数。

> 系统调用的执行过程：用户程序调用C库函数（API）；执行指令int$0x80触发软中断使系统调用陷入内核，同时将系统调用号以及所需参数一并传入内核；系统调用处理程序检查参数的有效性，并执行服务；完成后返回用户态。

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
 
