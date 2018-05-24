# 操作系统概述

---

## **提前准备**

（请在上课前完成）

* 完成lec1的视频学习和提交对应的在线练习
* git pull ucore\_os\_lab, ucore\_os\_docs, os\_tutorial\_lab, os\_course\_exercises in github repos。这样可以在本机上完成课堂练习。
* 知道OS课程的入口网址，会使用在线视频平台，在线练习/实验平台，在线提问平台\(piazza\)
  * [http://os.cs.tsinghua.edu.cn/oscourse/OS2018spring](http://os.cs.tsinghua.edu.cn/oscourse/OS2018spring)


* 会使用linux shell命令，如ls, rm, mkdir, cat, less, more, gcc等，也会使用linux系统的基本操作。
* 在piazza上就学习中不理解问题进行提问。



# 思考题

## 填空题

* 当前常见的操作系统主要用C编程语言编写。
* "Operating system"这个单词起源于____。
* 在计算机系统中，控制和管理____、有效地组织____运行的系统软件称作____。
* 允许多用户将若干个作业提交给计算机系统集中处理的操作系统称为____操作系统
* 你了解的当前世界上使用最多的32bit CPU是____，其上运行最多的操作系统是____。
* 应用程序通过____接口获得操作系统的服务。
* 现代操作系统的特征包括____，____，____，____。
* 操作系统内核的架构包括____，____，____。


## 问答题

- 请总结你认为操作系统应该具有的特征有什么？并对其特征进行简要阐述。


- 为什么现在的操作系统基本上用C语言来实现？为什么没有人用python，java来实现操作系统？

---

## 可选练习题

---

- 请分析并理解[v9\-computer](https://github.com/chyyuu/os_tutorial_lab/blob/master/v9_computer/docs/v9_computer.md)以及模拟v9\-computer的em.c。理解：在v9\-computer中如何实现时钟中断的；v9 computer的CPU指令，关键变量描述有误或不全的情况；在v9\-computer中的跳转相关操作是如何实现的；在v9\-computer中如何设计相应指令，可有效实现函数调用与返回；OS程序被加载到内存的哪个位置,其堆栈是如何设置的；在v9\-computer中如何完成一次内存地址的读写的；在v9\-computer中如何实现分页机制；


- 请编写一个小程序，在v9-cpu下，能够输出字符


- 输入的字符并输出你输入的字符


- 请编写一个小程序，在v9-cpu下，能够产生各种异常/中断


- 请编写一个小程序，在v9-cpu下，能够统计并显示内存大小

