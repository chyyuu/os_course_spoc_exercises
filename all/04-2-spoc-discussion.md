#lec9 虚存置换算法spoc练习

## 个人思考题
1. 置换算法的功能？

2. 全局和局部置换算法的不同？

3. 最优算法、先进先出算法和LRU算法的思路？

4. 时钟置换算法的思路？

5. LFU算法的思路？

6. 什么是Belady现象？

7. 几种局部置换算法的相关性：什么地方是相似的？什么地方是不同的？为什么有这种相似或不同？

8. 什么是工作集？

9. 什么是常驻集？

10. 工作集算法的思路？

11. 缺页率算法的思路？

12. 什么是虚拟内存管理的抖动现象？

13. 操作系统负载控制的最佳状态是什么状态？

## 小组思考题目

----
(1)（spoc）请证明为何LRU算法不会出现belady现象
虚拟页数量为M，访问的页序列为b(t)，t = 1,2,3...，b(t)表示在t时刻的访问页号，0< b(t) < M+1。在时刻t，物理页帧的集合为S(t)，其元素范围在[0..N]中，物理页帧的集合大小为N(N < M)。

如果b(t)属于S(t)，则表示命中，且物理页帧在t+1时刻的集合

S(t + 1) = S(t)

如果b(t)不属于S(t)，则表示缺页，这时有一个块要被换出，换出的页为v(t)。则物理页帧在t+1时刻的集合

S(t + 1) = (S(t) - {v(t)}) + {b(t)}

设S(1) = {1, 2, ..., N} S'(1) = {1, 2, ..., N} 即S(1) = S'(1)
 
证明LRU算法在物理页帧集合S'的大小为N+k（0 < k < M-N）时的缺页率比在物理页帧集合S大小为N的缺失率要低。

证明：使用数学归纳法，假设在时间t-1时，s(t－1)包含于s'(t－1)中。证明s(t)包含于s'(t)。考虑四种情况

1. b(t)属于s(t-1), b(t)属于s'(t-1)
  这种情况下s(t)和s‘(t)没有发生改变，包含关系依然存在,根据原来的假设，我们仍然可以的出结论。
2. b(t)不属于s(t-1), b(t)不属于s'(t-1)
   这种情况下两个集合都会发生替换，替换掉栈底元素，将b(t)插入栈顶。是否有s'(t)的栈底元素被替换，而s(t)中仍然有这个元素存在的情况？这种情况不会存在，设s(t-1)的栈底为a，而s'(t-1)的栈底为b，则b不会在s(t-1)中存在。可知a在s'(t-1)中也存在，那么在s'(t-1)中有b排在a的后面，则b的访问时间比a的访问时间长，而如果b在s(t-1)中存在，则不会是栈底，那么就有a的访问时间比b长，这样产生了一个矛盾。则这种情况不存在。
3. b(t)不属于s(t-1), b(t)属于s'(t-1)
   这种情况下s(t)会发生替换，替换掉栈底元素，将b(t)插入栈顶，因为本来就有b(t)属于s'(t-1)，这样也有两个集合的包含关系成立。
4. b(t)属于s(t-1), b(t)不属于s'(t-1)
  根据归纳假设，这种情况不存在，不予考虑。

综上所述，命题成立。则LRU不存在belady现象。

(2)（spoc）根据你的`学号 mod 4`的结果值，确定选择四种替换算法（0：LRU置换算法，1:改进的clock 页置换算法，2：工作集页置换算法，3：缺页率置换算法）中的一种来设计一个应用程序（可基于python, ruby, C, C++，LISP等）模拟实现，并给出测试。请参考如python代码或独自实现。
 - [页置换算法实现的参考实例](https://github.com/chyyuu/ucore_lab/blob/master/related_info/lab3/page-replacement-policy.py)
    设计模拟实现工作集页置换算法，程序如下：
   #include <stdio.h>


int main()
{
	int cache[1000], tag[1000];

	int i;
	for (i = 0; i < 1000; i++) {
		cache[i] = -1;
     }

	int  window = 10, page = -1, size = 0;
	int hit = 0;

	while (1)
	{
		hit = 0;
		printf("please input page:\n");
		scanf("%d", &page);

		if (page < 0 | page >= 1000)
		{
			printf("Error input\n");
			break;
		}

		for (i = 0; i < size; i++)
		{
			tag[i]++;
			if (cache[i] == page)
			{
				printf("page hit\n");
				tag[i] = 0;
				hit = 1;
			}

			else if (tag[i] >= window)
			{
				cache[i] = cache[size-1];
				tag[i] = tag[size-1];
				size--;
				i--;
			}
		}

		if (hit == 0)
		{
			printf("page miss\n");
			cache[size] = page;
			tag[size] = 0;
			size++;
		}
		
		for (i = 0; i < size; i++)
		{
			printf("{%d, %d}, ", cache[i], tag[i]);
		} 
		printf("\n"); 
	}
	 
	return 0;
}

测试数据：
12
13
12
15
17
15
123
123
输出为：
page miss
{12, 0}, 

page miss
{12, 1}, {13, 0}, 

page hit
{12, 0}, {13, 1}, 

page miss
{12, 1}, {13, 2}, {15, 0}, 

page miss
{12, 2}, {13, 3}, {15, 1}, {17, 0}, 
please input page:
15
page hit
{12, 3}, {13, 4}, {15, 0}, {17, 1}, 

page miss
{12, 4}, {13, 5}, {15, 1}, {17, 2}, {123, 0}, 

page hit
{12, 5}, {13, 6}, {15, 2}, {17, 3}, {123, 0}, 
## 扩展思考题
（1）了解LIRS页置换算法的设计思路，尝试用高级语言实现其基本思路。此算法是江松博士（导师：张晓东博士）设计完成的，非常不错！

参考信息：

 - [LIRS conf paper](http://www.ece.eng.wayne.edu/~sjiang/pubs/papers/jiang02_LIRS.pdf)
 - [LIRS journal paper](http://www.ece.eng.wayne.edu/~sjiang/pubs/papers/jiang05_LIRS.pdf)
 - [LIRS-replacement ppt1](http://dragonstar.ict.ac.cn/course_09/XD_Zhang/(6)-LIRS-replacement.pdf)
 - [LIRS-replacement ppt2](http://www.ece.eng.wayne.edu/~sjiang/Projects/LIRS/sig02.ppt)
