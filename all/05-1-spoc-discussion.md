#lec10 进程／线程概念spoc练习

NOTICE
- 有"w5l1"标记的题是助教要提交到学堂在线上的。
- 有"w5l1"和"spoc"标记的题是要求拿清华学分的同学要在实体课上完成，并按时提交到学生对应的git repo上。
- 有"hard"标记的题有一定难度，鼓励实现。
- 有"easy"标记的题很容易实现，鼓励实现。
- 有"midd"标记的题是一般水平，鼓励实现。

## 个人思考题

### 11.1 进程的概念

- 什么是进程？什么是程序？
- 程序和进程联系和区别是什么？

### 11.2 进程控制块

- 进程控制块的功能是什么？
- 进程控制块中包括什么信息？

### 11.3 进程状态

- 进程生命周期中的相关事件有些什么？它们对应的进程状态变化是什么？
- 进程切换过程中的几个关键代码分析
- 时钟中断触发调度函数的启动
- 当前进程的现场保存代码
- 进程切换代码
　＞　下一个运行进程的现场恢复

### 11.4 三状态进程模型

- 运行、就绪和等待三种状态的含义？
- 分析的4个相关状态转换代码和状态修改代码

### 11.5 挂起进程模型

- 引入挂起状态的目的是什么？内存中的什么内容放到外存中，就算是挂起状态？

### 11.6 线程的概念

- 引入线程的目的是什么？什么是线程？
- 进程与线程的联系和区别是什么？

### 11.8 内核线程

- 用户线程与内核线程的区别是什么？

## SPOC小组思考题

(1) (spoc)设计一个简化的进程管理子系统，可以管理并调度如下简化进程.给出了[参考代码](https://github.com/chyyuu/ucore_lab/blob/master/related_info/lab4/process-concept-homework.py)，请理解代码，并完成＂YOUR CODE"部分的内容．　可２个人一组

### 进程的状态 

 - RUNNING - 进程正在使用CPU
 - READY   - 进程可使用CPU
 - DONE    - 进程结束

### 进程的行为
 - 使用CPU, 
 - 发出YIELD请求,放弃使用CPU


### 进程调度
 - 使用FIFO/FCFS：先来先服务

### 关键模拟变量
 - 进程控制块
```
PROC_CODE = 'code_'
PROC_PC = 'pc_'
PROC_ID = 'pid_'
PROC_STATE = 'proc_state_'
```
 - 当前进程 curr_proc 
 - 进程列表：proc_info 描述了进程的行为特征：（１）使用CPU ;(2)等待I/O
```
   -l PROCESS_LIST, --processlist= X1:Y1,X2:Y2,...
   X 是进程的执行指令数; 
   Ｙ是执行CPU的比例(0..100) ，如果是100，表示不会发出yield操作
```
 - 进程切换行为：系统决定何时(when)切换进程:进程结束或进程发出yield请求

### 关键函数
 - 系统执行过程：run
 - 执行状态切换函数:　move_to_ready/running/done　
 - 调度函数：next_proc

### 执行实例

#### 例１
```
$./process-simulation.py -l 5:50
Process 0
  yld
  yld
  cpu
  cpu
  yld

Important behaviors:
  System will switch when the current process is FINISHED or ISSUES AN YIELD
Time     PID: 0 
  1     RUN:yld 
  2     RUN:yld 
  3     RUN:cpu 
  4     RUN:cpu 
  5     RUN:yld 

```

   
#### 例２
```
$./process-simulation.py  -l 5:50,5:50
Produce a trace of what would happen when you run these processes:
Process 0
  yld
  yld
  cpu
  cpu
  yld

Process 1
  cpu
  yld
  cpu
  cpu
  yld

Important behaviors:
  System will switch when the current process is FINISHED or ISSUES AN YIELD
Time     PID: 0     PID: 1 
  1     RUN:yld      READY 
  2       READY    RUN:cpu 
  3       READY    RUN:yld 
  4     RUN:yld      READY 
  5       READY    RUN:cpu 
  6       READY    RUN:cpu 
  7       READY    RUN:yld 
  8     RUN:cpu      READY 
  9     RUN:cpu      READY 
 10     RUN:yld      READY 
 11     RUNNING       DONE 
```
