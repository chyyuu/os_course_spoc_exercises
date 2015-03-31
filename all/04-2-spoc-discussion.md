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
```
只需要证明对任意的正整数t，s(t)始终包含于s'(t)即可
显然，当初始的时候，两个集合都是空，此命题成立，假设 i <= t - 1的时候都成立，只需要证明s(t)包含于s'(t)
分三种情况：
b(t) 属于 s(t),又属于 s'(t)
此时s(t)和s'(t)二者都不会发生变化，成立；
b(t) 不属于 s(t),但属于 s'(t)
替换之后肯定有b(t) 属于 s(t)，所以s(t)包含于s'(t)，成立；
b(t) 不属于 s(t),也不属于 s'(t)
根据假设，s(t-1)包含于s’(t-1)，我们假设进行替换的元素是x，那么x就是拥有最长的未访问时间的元素，
也就是说x位于s‘栈的栈底，如果x同时也被s'替换掉（x是s'的栈底），本来包含于s'（t-1）的s（t-1）两者同时减去一个元素，加上b（t）就是新的
s（t）和s'（t）；如果x不在s’栈的栈底，那么s'的栈底y拥有比x更久的未访问时间，即y是要被替换的元素，同上，这个时候包含关系仍然成立。
综上所述，对任意的正整数t，s(t)始终包含于s'(t)，所以LRU不会存在belady现象。
```

(2)（spoc）根据你的`学号 mod 4`的结果值，确定选择四种替换算法（0：LRU置换算法，1:改进的clock 页置换算法，2：工作集页置换算法，3：缺页率置换算法）中的一种来设计一个应用程序（可基于python, ruby, C, C++，LISP等）模拟实现，并给出测试。请参考如python代码或独自实现。
 - [页置换算法实现的参考实例](https://github.com/chyyuu/ucore_lab/blob/master/related_info/lab3/page-replacement-policy.py)
 ```
 #include<iostream>
#include<stdio.h>
using namespace std;

char read[10]={'c', 'c', 'd', 'b', 'c', 'e', 'c', 'e', 'a', 'd'};

bool work[10];
bool oldwork[10];
int worksize = 0;

int window = 4;

bool lack;

int main()
{
	for(int i=0; i<10; i++)
	{
		//lack?
		lack = 0;
		int readpage = read[i]-'a';
		if(work[readpage] == 0)
			lack = 1;
		
		//插入新页
		
		//update work & oldwork:	
		for(int j=0; j<10; j++)
		{
			oldwork[j] = work[j];
			work[j]= 0;
		}
		work[readpage] = 1;
		for(int q=1; (q<window) && (i-q>=0); q++)
			work[read[i-q]-'a'] = 1;
			
		//进行页替换
		for(int j=0; j<10; j++)
			if(oldwork[j] && !work[j])
				;//剔除页
		
		//work size:
		worksize = 0;
		for(int j=0; j<10; j++)
			if(work[j])
				worksize ++;
				
		cout << i+1 << ":\tvisit " << read[i] << " \tlack? " << lack << " \tsize: " << worksize << " \t";
		for(int j=0; j<10; j++)
			if(work[j])
				cout << (char)(j+'a') << " ";
		cout << endl;
	}
	return 0;
}
 ```
## 扩展思考题
（1）了解LIRS页置换算法的设计思路，尝试用高级语言实现其基本思路。此算法是江松博士（导师：张晓东博士）设计完成的，非常不错！

参考信息：

 - [LIRS conf paper](http://www.ece.eng.wayne.edu/~sjiang/pubs/papers/jiang02_LIRS.pdf)
 - [LIRS journal paper](http://www.ece.eng.wayne.edu/~sjiang/pubs/papers/jiang05_LIRS.pdf)
 - [LIRS-replacement ppt1](http://dragonstar.ict.ac.cn/course_09/XD_Zhang/(6)-LIRS-replacement.pdf)
 - [LIRS-replacement ppt2](http://www.ece.eng.wayne.edu/~sjiang/Projects/LIRS/sig02.ppt)
