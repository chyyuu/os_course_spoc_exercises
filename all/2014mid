# 2014 操作系统期中考试参考答案

WARNING: 仅供参考！

### 第一题

(1)

(1a) 4

(1b) 4096

(2) *注：我们考查的是哪些由__硬件__保存。*

(2a) 保存。不保存无法返回。

(2b) 保存。不保存无法切换堆栈。

(2c) 不保存。由中断服务例程负责保存通用寄存器。

(2d) 保存。不保存在返回后由于状态不一致，继续执行可能出错。

(2e) 不保存。

(3) B

(4)

|    | W=0 U=0 A=0 D=0 | W=1 U=0 A=0 D=0 | W=0 U=1 A=1 D=0 | W=1 U=1 A=1 D=1 |
|:--:| --------------- | --------------- | --------------- | --------------- |
|在ring0读|A→1  |A→1      |无变化|无变化|
|在ring0写|不可写|A→1, D→1 |不可写|无变化|
|在ring3读|无权限|无权限    |无变化|无变化|
|在ring3写|无权限|无权限    |不可写|无变化|

*注：“不可写”和“无权限”都可以叫做“缺页异常”。这里假设 P 位均为1，即所访问的页是存在的。*

### 第二题

(1)

(1a) `proc_struct`

(1b) `pid`

(1c) `mm`

(1d) `cr3`

(1e) `kstack`?

(2)

(2a) 否（不会被改变）

(2b) 否（这时页表还是原来那个）

(3)

(3a) 否（不一定异常）

(3b) 是（一定异常）

(4)

(4a) 是

(4b) 是

(4c) 是

(4d) 是

(5) COW 的描述参见实验指导书[Lab 5](https://objectkuan.gitbooks.io/ucore-docs/content/lab5/lab5_2_1_exercises.html)。

### 第三题

(1) 一个磁盘主引导扇区只有512字节。且第510个（倒数第二个）字节是0x55， 第511个（倒数第一个）字节是0xAA。

(2) Bootloader 执行的第一条指令是16行，进入保护模式后执行的第一条指令是61行，转换到保护模式可以增大内存空间，且可以对内存的不同区域设置不同的权限位。

(3) 107行跳转至 elf 的入口执行。

### 第四题

(1) 接口：

```c
pid_t fork(void);
```

功能：复制父进程的所有变量和内存、通用寄存器。子进程的返回0，父进程的返回子进程标识符。

(2) 为了解答此题，你需要先对[wait](http://linux.die.net/man/2/wait)的行为有所了解。

源码参见[2014_4.c](https://github.com/paulzfm/os_course_spoc_exercises/blob/master/midterm-answer/2014_4.c)，里面有详细的注释说明程序的行为，特别注意`getppid()`与`getpid()`与原题相比反过来了，这样才能让输出的顺序对文字描述对应上。原题中，完整的输出如下：

```
xyong@portal:~/work$ ./a.out
Here I am in the program! Time to wait = 5
F S UID PID PPID C PRI NI ADDR SZ WCHAN TTY TIME CMD
0 S 1000 11739 11738 0 80 0 - 6926 wait pts/0 00:00:00 bash
0 S 1000 11862 11739 0 80 0 - 1041 wait pts/0 00:00:00 a.out
0 S 1000 11863 11862 0 80 0 - 1101 wait pts/0 00:00:00 sh
0 R 1000 11864 11863 0 80 0 - 2433 - pts/0 00:00:00 ps
I'm the parent at Line 33. My parent's process ID is 11739, My process ID is 11862, status = 0.
I'm the child. My parent's process ID is 11862, My process ID is 11865, status = 17.
Bye now!
Child 11865 exited with status 17.
I'm the parent at Line 43. My parent's process ID is 11739, My process ID is 11862, status = 17.

xyong@portal:~/work$ ./a.out 3
Here I am in the program! Time to wait = 3
F S UID PID PPID C PRI NI ADDR SZ WCHAN TTY TIME CMD
0 S 1000 11739 11738 0 80 0 - 6926 wait pts/0 00:00:00 bash
0 S 1000 11866 11739 0 80 0 - 1041 wait pts/0 00:00:00 a.out
0 S 1000 11867 11866 0 80 0 - 1101 wait pts/0 00:00:00 sh
0 R 1000 11868 11867 0 80 0 - 2433 - pts/0 00:00:00 ps
I'm the parent at Line 33. My parent's process ID is 11739, My process ID is 11866, status = 0.
I'm the child. My parent's process ID is 11866, My process ID is 11869, status = 17.
Bye now!
Child 11869 exited with status 17.
I'm the parent at Line 43. My parent's process ID is 11739, My process ID is 11866, status = 17.
```

### 第五题

(1) 函数调用 (`call`)：返回地址压栈、跳转到子程序入口；

返回 (`ret`)：从栈中弹出一个数，以此作为返回地址并跳转。

(2) 参数从右往左依次压栈传递。

(3) 可以参见[我的 Lab 1 答案](https://github.com/paulzfm/ucore-os-lab/blob/master/labcodes/lab1/kern/debug/kdebug.c)。

### 第六题

(1) 简而言之，硬件中断：外部；软件中断：内部、异常；系统调用：内部、用户态程序主动调用。

(2) 外部设置中断信号，CPU 响应中断，保护现场，跳转至中断服务例程，中断服务例程处理中断，CPU 恢复现场。

(3) 硬件中断到来时，客户操作系统可以感知中断，做出处理。由于客户操作系统核心态处于 Ring 1，当其需要 Ring 0 特权时，产生异常，由宿主操作系统接管，在 Ring 0 执行，完毕后再返回到 Ring 1。
