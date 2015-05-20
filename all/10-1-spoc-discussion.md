# IO设备(lec 23) spoc 思考题

## 个人思考题
### IO特点 
 - 字符设备的特点是什么？
 - 块设备的特点是什么？
 - 网络设备的特点是什么？

### IO结构
 - IO访问的手段有哪些？
 - 请描述IO请求到完成的整个执行过程
 - CPU与device通信的手段有哪些？

> 显式的IO指令，如x86的in, out； 或者是memory读写方式，即把device的寄存器，内存等映射到物理内存中 

### IO数据传输
 - IO数据传输有哪几种？
 - 轮询方式的特点是什么？
 - 中断方式的特点是什么？
 - DMA方式的特点是什么？

### 磁盘调度
 - 请简要阐述磁盘的工作过程
 - 请用一表达式（包括寻道时间，旋转延迟，传输时间）描述磁盘I/O传输时间
 - 请说明磁盘调度算法的评价指标
 - FIFO磁盘调度算法的特点是什么?
 - 最短服务时间优先(SSTF)磁盘调度算法的特点是什么?
 - 扫描(SCAN)磁盘调度算法的特点是什么?
 - 循环扫描(C-SCAN)磁盘调度算法的特点是什么?
 - C-LOOK磁盘调度算法的特点是什么?
 - N步扫描(N-step-SCAN)磁盘调度算法的特点是什么?
 - 双队列扫描(FSCAN)磁盘调度算法的特点是什么?

### 磁盘缓存
 - 磁盘缓存的作用是什么？
 - 请描述单缓存(Single Buffer Cache)的工作原理
 - 请描述双缓存(Double Buffer Cache)的工作原理
 - 请描述访问频率置换算法(Frequency-based Replacement)的基本原理

## 小组思考题
 - (spoc)完成磁盘访问与磁盘寻道算法的作业，具体帮助和要求信息请看[disksim指导信息](https://github.com/chyyuu/ucore_lab/blob/master/related_info/lab8/disksim-homework.md)和[disksim参考代码](https://github.com/chyyuu/ucore_lab/blob/master/related_info/lab8/disksim-homework.py)
答：
```
原程序478行有问题，已改正。
问题一FIFO:
./disksim.py  采用FIFO -a 0		5.5*30（旋转）+30（传输）=195
./disksim.py   -a 6				360-15（旋转）+30（传输）=345
./disksim.py   -a 30			80（寻道）+360-80-15(旋转)+30(传输)=375
./disksim.py   -a 7,30,8		7：0+15+30=45
								30：80+220+30=330
								8：80+310+30=420
								总时间=795
./disksim.py   -a 10,11,12,13,24,1
								Block:  10  Seek:  0  Rotate:105  Transfer: 30  Total: 135
								Block:  11  Seek:  0  Rotate:  0  Transfer: 30  Total:  30
								Block:  12  Seek: 40  Rotate:320  Transfer: 30  Total: 390
								Block:  13  Seek:  0  Rotate:  0  Transfer: 30  Total:  30
								Block:  24  Seek: 40  Rotate:260  Transfer: 30  Total: 330
								Block:   1  Seek: 80  Rotate:280  Transfer: 30  Total: 390

								TOTALS      Seek:160  Rotate:965  Transfer:180  Total:1305

问题二SSTF：
./disksim.py   -a 10,11,12,13，24,1
							找最近的磁道。顺序：10,11,1,12,13,24
								Block:  10  Seek:  0  Rotate:105  Transfer: 30  Total: 135
								Block:  11  Seek:  0  Rotate:  0  Transfer: 30  Total:  30
								Block:   1  Seek:  0  Rotate: 30  Transfer: 30  Total:  60
								Block:  12  Seek: 40  Rotate:260  Transfer: 30  Total: 330
								Block:  13  Seek:  0  Rotate:  0  Transfer: 30  Total:  30
								Block:  24  Seek: 40  Rotate:260  Transfer: 30  Total: 330

								TOTALS      Seek: 80  Rotate:655  Transfer:180  Total: 915
								
问题三 SCAN, C-SCAN
（1）SCAN./disksim.py   -a 10,11,12,13,24,1
						从磁头开始位置，先向左再向右扫一遍：
							Block:  10  Seek:  0  Rotate:105  Transfer: 30  Total: 135
							Block:  11  Seek:  0  Rotate:  0  Transfer: 30  Total:  30
							Block:   1  Seek:  0  Rotate: 30  Transfer: 30  Total:  60
							Block:  12  Seek: 40  Rotate:260  Transfer: 30  Total: 330
							Block:  13  Seek:  0  Rotate:  0  Transfer: 30  Total:  30
							Block:  24  Seek: 40  Rotate:260  Transfer: 30  Total: 330

							TOTALS      Seek: 80  Rotate:655  Transfer:180  Total: 915
（2）C-SCAN./disksim.py   -a 10,11,12,13,24,1
						C—SCAN：从磁头位置开始，一直保持一个方向扫描。本例中同上
							Block:  10  Seek:  0  Rotate:105  Transfer: 30  Total: 135
							Block:  11  Seek:  0  Rotate:  0  Transfer: 30  Total:  30
							Block:   1  Seek:  0  Rotate: 30  Transfer: 30  Total:  60
							Block:  12  Seek: 40  Rotate:260  Transfer: 30  Total: 330
							Block:  13  Seek:  0  Rotate:  0  Transfer: 30  Total:  30
							Block:  24  Seek: 40  Rotate:260  Transfer: 30  Total: 330

							TOTALS      Seek: 80  Rotate:655  Transfer:180  Total: 915
```

