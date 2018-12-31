# 操作系统概述

---

## 提前准备

（请在上课前完成）

* 完成 lec1 的视频学习和提交对应的在线练习
* git pull ucore\_os\_lab, ucore\_os\_docs, os\_tutorial\_lab, os\_course\_exercises in github repos。这样可以在本机上完成课堂练习。
* 知道 OS 课程的入口网址，会使用在线视频平台，在线练习/实验平台，在线提问平台\(piazza\)
  * [http://os.cs.tsinghua.edu.cn/oscourse/OS2017spring](http://os.cs.tsinghua.edu.cn/oscourse/OS2017spring)
* 了解学习方法

```
  for (i = 1; i <= 13; i++) {
    1. 完成第 i 周课前视频学习
    2. 完成第 i 周 OpenEdx 上的在线基本练习 ( 直接在 OpenEdx 上提交 )
    3. 上 SPOC 课并完成课上练习（ 课堂上完成并提交到 piazza 上的老师指定的特定帖下 ）
    4. 在 deadline 前，按序完成 ucore_lab 实验
    5. if (i == 7) 参加期中考试;
    6. if (i == 13) 参加期末考试;
    7. if (碰到问题)　到 piazza 的论坛上提问;
   }
```

* 会使用 linux shell 命令，如 ls, rm, mkdir, cat, less, more, gcc 等，也会使用 linux 系统的基本操作。
* 了解 v9-computer 会使用 v9-computer 的 dis, xc, xem 命令（包括启动参数）,阅读 v9-computer 中的 v9_computer.md 文档，了解汇编指令的类型和含义等，了解 v9-computer 的细节。
* [v9 computer](https://github.com/chyyuu/os_tutorial_lab/blob/master/v9_computer/docs/v9_computer.md)

* 在 piazza 上就学习中不理解问题进行提问。

* 观看 [Peter Denning’s talk video, “Perspectives on OS Foundations”, presented at the SOSP History Day Workshop (SOSP‘15).](https://www.youtube.com/watch?v=xs6cDaXbsUA)


# 操作系统概述思考题

---

## 个人思考题

---
* 当前常见的操作系统包括 {%s%} Windows, GNU/Linux, MacOS, iOS, Android, DoS, VxWorks, uCOS, FreeRTOS, eCOS, QNX{%ends%}。
* 当前常见的操作系统主要用 {%s%} C, C++, ASM{%ends%} 编程语言编写。
* 操作系统是 {%s%} 管理系统资源，控制程序执行，改善人机界面，提供各种服务，合理组织计算机工作流程和为用户使用计算机提供良好的运行环境{%ends%}的一种系统软件。
* 「Operating system」这个单词起源于{%s%} Operator {%ends%}。
* 在计算机系统中，控制和管理各种资源、有效地组织多道程序运行的系统软件称作{%s%} 操作系统 {%ends%}。
* 允许多用户将若干个作业提交给计算机系统集中处理的操作系统称为{%s%} 批处理 {%ends%}操作系统
* 你了解的当前世界上使用最多的 32 bit CPU 是{%s%} ARM {%ends%}，其上运行最多的操作系统是{%s%} Android{%ends%}。
* 应用程序通过{%s%} 系统调用 {%ends%}接口获得操作系统的服务。
* 推动操作系统发展的因素有{%s%} ARM {%ends%}。
* 现代操作系统的特征包括{%s%}并行性，虚拟性，异步性（不确定性）、共享性和持久性 {%ends%}。
* 操作系统内核的架构设计包括{%s%}宏内核（单体内核, monolithic kernel），微内核 (microkernel)，外核（exokernel）{%ends%}。


---

- 分析你所认识的操作系统（Windows、MacOS、Linux、FreeBSD、Android、iOS）所具有的独特和共性的功能？

{%s%} 

不同点：
1)面向的目标不同： Windows 面向 PC, 服务器、；Linux / FreeBSD 面向嵌入式，PC，服务器； Android / iOS 面向移动终端；MacOS 面向 PC。 
2）开放性不同：Windows / iOS / MacOS 是封闭，不开放源码，开发过程不开放；Linux 基于 GPL 协议开放源码，开发过程不开放；FreeBSD 基于 BSD 协议开放源码，开发过程不开放；Android 基于 Apache/GPL 等协议开放源码，但开发过程不开放。
3）支持硬件不同： Windows 支持 x86, ARM CPU； MacOS 支持 x86；Linux 支持绝大部分 32 / 64bit CPU； FreeBSD 支持一部分 32 / 64bit CPU；iOS 支持 ARM CPU。

相同点：
2)类型：都是通用操作系统，具有通用操作系统的功能和特点。
  

{%ends%}

- 如何评价 Windows 的封闭发展模式和 Linux 的开放发展模式？

{%s%} 

这是两种发展模式，没有对错 / 优劣，二者都有各自的生存和发展的途径。

{%ends%}

- 请总结你认为操作系统应该具有的特征有什么？并对其特征进行简要阐述。

{%s%} 

从总体上看，操作系统具有五个方面的特征：虚拟性（Virtualization）、并发性（concurrency）、异步性、共享性和持久性（persistency）。在虚拟性方面，可以从操作系统对内存，CPU的抽象和处理上有更好的理解；对于并发性和共享性方面，可以从操作系统支持多个应用程序「同时」运行的情况来理解；对于异步性，可以从操作系统调度，中断处理对应用程序执行造成的影响等几个方面来理解；对于持久性方面，可以从操作系统中的文件系统支持把数据方便地从磁盘等存储介质上存入和取出来理解。

{%ends%}

- 请给出你觉得的更准确的操作系统的定义？

{%s%} 

操作系统是计算机系统机构中的一个系统软件，它的职能主要有两个：对下面（也就是计算机硬件），有效地组织和管理计算机系统中的硬件资源（包括处理器、内存、硬盘、显示器、键盘、鼠标等各种外设）；对上面（应用程序或用户），提供简洁的服务功能接口，屏蔽硬件管理带来的差异性和复杂性，使得应用程序和用户能够灵活、方便、有效地使用计算机。为了完成这两个职能，操作系统需要起到资源管理器的作用，能在其内部实现中安全，合理地组织，分配，使用与处理计算机中的软硬件资源，使整个计算机系统能高效可靠地运行。

{%ends%}

- 你希望从操作系统课学到什么知识？

{%s%} 

通过学习与实践来更好地理解操作系统原理和概念，掌握软件与硬件的交互，掌握大型软件的分析与开发方法。

{%ends%}

- 操作系统面临什么挑战？

{%s%} 

当前，大数据、人工智能、机器学习、高速网络、AR / VR、异构/新型计算机硬件与外设（CPU + GPU, CPU + FPGA, NVRAM等）、系统安全、系统正确性等对操作系统等系统软件带来了新的挑战。如何有效支持和利用这些技术是未来操作系统的挑战与方向。

{%ends%}

- 你用过什么操作系统？如何评价这些系统？

{%s%} 

用过的包括：Windows / MacOS 适合一般办公用户，Android / iOS适合移动终端用户，Linux / FreeBSD适合专业计算机人士

{%ends%}

- 你所知道的操作系统技术有什么？哪些是操作系统特有的？

{%s%} 

进程管理、内存管理、文件系统管理、同步互斥、外设适配等，都是OS特有的。

{%ends%}

- 什么是操作系统内核？什么是微内核？什么是外核（Exokernel）？

{%s%} 

操作系统内核是指大多数操作系统的核心部分。它由操作系统中用于管理存储器、文件、外设和系统资源的那些部分组成。操作系统内核通常运行进程，并提供进程间的通信。微内核（ Microkernelkernel ）微内核是内核的一种精简形式。微内核结构由一个非常简单的硬件抽象层和一组比较关键的原语或系统调用组成，这些原语仅仅包括了建立一个系统必需的几个部分，如线程管理，地址空间和进程间通信等。通常与内核集成在一起的系统服务层被分离出来，变成可以根据需求加入的组件，这样就可提供更好的可扩展性和更加有效的应用环境。使用微内核设计，对系统进行升级，只要用新模块替换旧模块，不需要改变整个操作系统。其最大的问题是性能由于进程间频繁通信而会受到影响。外核（ Exokernel ）结构：是一种极简化的微核心，约在 1994 年由 MIT PDOS 组提出。其设计理念是让用户程序的设计者来决定硬件接口的设计。外核本身非常的小，它通常只负责硬件资源保护和硬件资源复用相关的服务。在外核上运行一个针对具体应用的 LibOS，并给 LibOS 提供硬件功能支持。单一应用运行在 LibOS 上，可获得性能的优势。

{%ends%}

- 你理解的虚拟化是什么？

{%s%} 

首先来看看内存的虚拟化。程序员在写应用程序的时候，不用考虑其程序的起始内存地址要放到计算机内存的具体某个位置，而是用字符串符号定义了各种变量和函数，直接在代码中便捷地使用这些符号就行了。这是由于操作系统建立了一个地址固定，空间巨大的虚拟内存给应用程序来运行，这是空间虚拟化。这里的每个符号在运行时是要对应到具体的内存地址的。这些内存地址的具体数值是什么？程序员不用关心。为什么？因为编译器会自动帮我们把这些符号翻译成地址，形成可执行程序。程序使用的内存是否占得太大了？在一般情况下，程序员也不用关心。

再来看 CPU 虚拟化。不同的应用程序可以在内存中并发运行，相同的应用程序也可有多个拷贝在内存中并发运行。而每个程序都「认为」自己完全独占了 CPU 在运行，这是「时间虚拟化」。这其实也是操作系统给了运行的应用程序一个虚拟幻象。其实是操作系统把时间分成小段，每个应用程序占用其中一小段时间片运行，用完这一时间片后，操作系统会切换到另外一个应用程序，让它运行。由于时间片很短，操作系统的切换开销也很小，人眼基本上是看不出的，反而感觉到多个程序各自在独立「并行」执行，从而实现了时间虚拟化。

{%ends%}

---

## 小组讨论题

---

- 目前的台式 PC 机标准配置和价格？

{%s%} 

x86 多核 CPU，4GB+ 内存，128GB+ 硬盘 /SSD，19 寸显示器，400 瓦机箱，键盘 / 鼠标，价格大约 2000～3000 RMB 就够了。

如果配置不要求太高，1000 RMB 可以买一个笔记本。

{%ends%}

- 为什么现在的操作系统基本上用 C 语言来实现？

{%s%} 

C 语言足够高级，成熟，可以方便编写 OS 和跨平台；且足够底层，能够很好地描述计算机硬件细节（比如位操作）；C 编译器可以生成高效的机器语言组成的执行代码，性能有保证；C 语言产生的一个原因就是为了更好地编写 UNIX 操作系统。

{%ends%}

- 为什么没有人用 python，java 来实现操作系统？

{%s%} 

无法保证性能，不够底层。

{%ends%}

- 请评价用 C++ 来实现操作系统的利弊？

{%s%} 

 C++ 作为高级编程语言，实现操作系统可以很好的模块化和面向对象的特征。但不足是现在基于 C++ 写出的 OS 还很少，不够成熟，C++ 语言的一些机制相对比较复杂，难以掌握，开发，调试，对性能也有一定影响。

{%ends%}

---

## 开放思考题

---

- 请评价用 LISP, Haskell, GO, D，RUST 等实现操作系统的利弊？

{%s%} 

有基于上述语言的实验性操作系统，相对而言，使用广泛性，成熟度和产业接受度不高。

{%ends%}

- 为什么微软的 Windows 没有在手机终端领域取得领先地位？

{%s%} 

起步太晚，吃 PC 的老本，领导对未来方向把握不够，现在的市场已经被 Android / iOS 瓜分了，再追已经很困难了。

{%ends%}

- 你认为未来（10 年内）的操作系统应该具有什么样的特征和功能？

{%s%} 

支持机器学习，人工智能，各种新型硬件，出现更自然的人机交互接口，无处不在

{%ends%}

---

## v9-computer相关题目

---

- 请分析模拟 v9\-computer 的 em.c。理解：在 v9\-computer 中如何实现时钟中断；v9 computer 的 CPU 指令，关键变量描述有误或不全的情况；在 v9\-computer 中的跳转相关操作是如何实现的；在 v9\-computer 中如何设计相应指令，可有效实现函数调用与返回；OS 程序被加载到内存的哪个位置, 其堆栈是如何设置的；在 v9\-computer 中如何完成一次内存地址的读写的；在 v9\-computer 中如何实现分页机制；

{%s%} 

https://github.com/chyyuu/os_tutorial_lab/blob/master/v9_computer/tools/em.c       



https://github.com/chyyuu/os_tutorial_lab/blob/master/v9_computer/docs/v9_computer.md

{%ends%}

- 请编写一个小程序，在 v9-cpu 下，能够输出字符

{%s%} 

https://github.com/chyyuu/os_tutorial_lab/tree/master/v9_computer/os_helloworld

{%ends%}

- 请编写一个小程序，在 v9-cpu 下，能够接收你输入的字符并输出你输入的字符

{%s%} 

{%ends%}

- 请编写一个小程序，在 v9-cpu 下，能够产生各种异常 / 中断

{%s%} 

https://github.com/chyyuu/os_tutorial_lab/tree/master/v9_computer/os_bad_phys_addr  

https://github.com/chyyuu/os_tutorial_lab/tree/master/v9_computer/os_divid_by_zero   

https://github.com/chyyuu/os_tutorial_lab/tree/master/v9_computer/os_invalid_intruction

https://github.com/chyyuu/os_tutorial_lab/tree/master/v9_computer/os_page_fault  

https://github.com/chyyuu/os_tutorial_lab/tree/master/v9_computer/os_timer_interrupt  

{%ends%}

- 请编写一个小程序，在 v9-cpu 下，能够统计并显示内存大小

{%s%} 

{%ends%}

