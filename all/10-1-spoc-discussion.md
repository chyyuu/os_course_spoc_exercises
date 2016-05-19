# IO设备(lec 23) spoc 思考题

## 小组思考题
1. (spoc)请以键盘输入、到标准输出设备stdout的printf输出、串口输出、磁盘文件复制为例，描述ucore操作系统I/O从请求到完成的整个执行过程，并分析I/O过程的时间开销。
 
 **描述ucore操作系统中“到标准输出设备stdout的printf输出”从请求到完成的整个执行过程，并分析I/O过程的时间开销。**
 
 1. `cprintf(const char *fmt, ...)`
 2. `vcprintf(const char *fmt, va_list ap)`
 3. `vprintfmt(void (*putch)(int, void*, int), int fd, void *putdat, const char *fmt, va_list ap)`
 4. `cputch(int c, int *cnt)`
 5. `cons_putc(int c)`
 6. `lpt_putc(int c)`并口输出；`cga_putc(int c)`屏幕输出；`serial_putc(int c)`串口输出
 7. 以下4个函数调用，生成4个outb汇编语句
 
    ```c
    outb(addr_6845, 14);
    outb(addr_6845 + 1, crt_pos >> 8);
    outb(addr_6845, 15);
    outb(addr_6845 + 1, crt_pos);
    ```
 
2. (spoc)完成磁盘访问与磁盘寻道算法的作业，具体帮助和要求信息请看[disksim指导信息](https://github.com/chyyuu/ucore_lab/blob/master/related_info/lab8/disksim-homework.md)和[disksim参考代码](https://github.com/chyyuu/ucore_lab/blob/master/related_info/lab8/disksim-homework.py)


