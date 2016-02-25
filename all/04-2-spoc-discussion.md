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
虚拟页数量为M，访问的页序列为b(t)，t = 1,2,3...，b(t)表示在t时刻的访问页号，0< b(t) < M+1。在时刻t，物理页帧的集合为S(t)，其元素范围在[0..N]中，物理页帧的集合大小为N(N < M)。
如果b(t)属于S(t)，则表示命中，且物理页帧在t+1时刻的集合S(t + 1) = S(t)
如果b(t)不属于S(t)，则表示缺页，这时有一个块要被换出，换出的页为v(t)。
则物理页帧在t+1时刻的集合S(t + 1) = (S(t) - {v(t)}) + {b(t)}
设S(1) = {1, 2, ..., N} S'(1) = {1, 2, ..., N} 即S(1) = S'(1)
证明LRU算法在物理页帧集合S'的大小为N+k（0 < k < M-N）时的缺页率比在物理页帧集合S大小为N的缺失率要低。

证明：使用数学归纳法，假设在时间t-1时，s(t－1)包含于s'(t－1)中。证明s(t)包含于s'(t)。考虑四种情况
1.b(t)属于s(t-1), b(t)属于s'(t-1)
这种情况下s(t)和s‘(t)没有发生改变，包含关系依然存在。
2.b(t)不属于s(t-1), b(t)不属于s'(t-1)
这种情况下两个集合都会发生替换，替换掉栈底元素，将b(t)插入栈顶。
s'(t)的栈底元素被替换，而s(t)中仍然有这个元素存在的情况不会存在。
设s(t-1)的栈底为a，而s'(t-1)的栈底为b，则b不会在s(t-1)中存在。
可知a在s'(t-1)中也存在，那么在s'(t-1)中有b排在a的后面，则b的访问时间比a的访问时间长，
而如果b在s(t-1)中存在，则不会是栈底，那么就有a的访问时间比b长，
这样产生了一个矛盾。则这种情况不存在。
3.b(t)不属于s(t-1), b(t)属于s'(t-1) 
这种情况下s(t)会发生替换，替换掉栈底元素，将b(t)插入栈顶，
因为本来就有b(t)属于s'(t-1)，这样也有两个集合的包含关系成立。
4.b(t)属于s(t-1), b(t)不属于s'(t-1) 
根据归纳假设，这种情况不存在，不予考虑。
综上所述，命题成立。则LRU不存在belady现象。
```
<<<<<<< HEAD

=======
>>>>>>> 790bc39b948c60181c85f58654a11682d075cbe2

(2)（spoc）根据你的`学号 mod 4`的结果值，确定选择四种替换算法（0：LRU置换算法，1:改进的clock 页置换算法，2：工作集页置换算法，3：缺页率置换算法）中的一种来设计一个应用程序（可基于python, ruby, C, C++，LISP等）模拟实现，并给出测试。请参考如python代码或独自实现。
 - [页置换算法实现的参考实例](https://github.com/chyyuu/ucore_lab/blob/master/related_info/lab3/page-replacement-policy.py)
```
<<<<<<< HEAD
学号 mod 4 = 1
=======
学号 mod 4 = 1，实现改进的clock页置换算法。程序如下：
>>>>>>> 790bc39b948c60181c85f58654a11682d075cbe2

#include <iostream>
#include <vector>
using namespace std;

struct Page {
<<<<<<< HEAD
int used;
int modified;
int frame;
Page(int u, int m, int f) : used(u), modified(m), frame(f) {}
=======
    int used;
  	 int modified;
   	int frame;
   	Page(int u, int m, int f) : used(u), modified(m), frame(f) {}
>>>>>>> 790bc39b948c60181c85f58654a11682d075cbe2
};

vector<Page> pages;
int pointer;

void print() {
<<<<<<< HEAD
int i = pointer;
for (int j = 0; j < 4; j ++) {
cout << pages[i].used << ' ' << pages[i].modified << ' ' << pages[i].frame << endl;
i = (i + 1) % 4;    
}       
}    

void read(int f) {
bool flag = false;
for (int i = pages.size() - 1; i >= 0; i --)
if (f == pages[i].frame) {
flag = true;
pages[i].used = 1;
cout << "read " << f << " hit" << endl;
break;   
}  
if (!flag) {
cout << "read " << f << " miss" << endl;
while (pages[pointer].used || pages[pointer].modified) {
if (pages[pointer].used) {
pages[pointer].used = 0;
} else {
pages[pointer].modified = 0;   
}
pointer = (pointer + 1) % 4;
}
pages[pointer].used = 1;
pages[pointer].frame = f;
pointer = (pointer + 1) % 4;
}    
print();
}

void write(int f) {
bool flag = false;
for (int i = pages.size() - 1; i >= 0; i --)
if (f == pages[i].frame) {
flag = true;
pages[i].used = 1;
pages[i].modified = 1;
cout << "write " << f << " hit" << endl;   
}    
if (!flag) {
cout << "write " << f << " miss" << endl;
while (pages[pointer].used || pages[pointer].modified) {
if (pages[pointer].used) {
pages[pointer].used = 0;
} else {
pages[pointer].modified = 0;   
}
pointer = (pointer + 1) % 4;
}
pages[pointer].used = 1;
pages[pointer].modified = 1;
pages[pointer].frame = f;
pointer = (pointer + 1) % 4;
}
print();
} 

int main() {
Page p(0, 0, 1);
pages.push_back(p);
p.frame = 2;
pages.push_back(p);
p.frame = 3;
pages.push_back(p);
p.frame = 4;
pages.push_back(p);
pointer = 0;
read(3);
write(1);
read(4);
write(2);
read(5);
read(2);
write(1);
read(2);
read(3);
write(4);
system("pause");
return 0;   
}    
```
```
=======
    int i = pointer;
    for (int j = 0; j < 4; j ++) {
    	   cout << pages[i].used << ' ' << pages[i].modified << ' ' << pages[i].frame << endl;
     	  i = (i + 1) % 4;    
   	}       
}    

void read(int f) {
    bool flag = false;
    for (int i = pages.size() - 1; i >= 0; i --)
 	   	  if (f == pages[i].frame) {
 	   	      flag = true;
 	   	 	    pages[i].used = 1;
        	   cout << "read " << f << " hit" << endl;
         	  break;   
 	   	  }  
   	if (!flag) {
   	 	  cout << "read " << f << " miss" << endl;
	   	   while (pages[pointer].used || pages[pointer].modified) {
  	   		    if (pages[pointer].used) {
  			 	         pages[pointer].used = 0;
  			       } else {
  			 	         pages[pointer].modified = 0;   
  			       }
  			       pointer = (pointer + 1) % 4;
	        }
	        pages[pointer].used = 1;
    	    pages[pointer].frame = f;
    	    pointer = (pointer + 1) % 4;
	    }    
	    print();
	}

void write(int f) {
    bool flag = false;
    for (int i = pages.size() - 1; i >= 0; i --)
    	   if (f == pages[i].frame) {
    	 	     flag = true;
       		   pages[i].used = 1;
         	  pages[i].modified = 1;
          	 cout << "write " << f << " hit" << endl;   
    	   }    
	   if (!flag) {
	 	     cout << "write " << f << " miss" << endl;
   		   while (pages[pointer].used || pages[pointer].modified) {
  	   		    if (pages[pointer].used) {
  			 	         pages[pointer].used = 0;
  			       } else {
  			 	         pages[pointer].modified = 0;   
  			       }
  			       pointer = (pointer + 1) % 4;
	       }
	       pages[pointer].used = 1;
   	    pages[pointer].modified = 1;
   	    pages[pointer].frame = f;
   	    pointer = (pointer + 1) % 4;
	   }
	   print();
} 

int main() {
   	Page p(0, 0, 1);
   	pages.push_back(p);
   	p.frame = 2;
   	pages.push_back(p);
   	p.frame = 3;
   	pages.push_back(p);
   	p.frame = 4;
   	pages.push_back(p);
   	pointer = 0;
   	read(3);
   	write(1);
   	read(4);
   	write(2);
   	read(5);
   	read(2);
   	write(1);
   	read(2);
   	read(3);
   	write(4);
   	system("pause");
   	return 0;   
}    

测试结果如下：

>>>>>>> 790bc39b948c60181c85f58654a11682d075cbe2
read 3 hit
0 0 1
0 0 2
1 0 3
0 0 4
write 1 hit
1 1 1
0 0 2
1 0 3
0 0 4
read 4 hit
1 1 1
0 0 2
1 0 3
1 0 4
write 2 hit
1 1 1
1 1 2
1 0 3
1 0 4
read 5 miss
0 0 4
0 0 1
0 0 2
1 0 5
read 2 hit
0 0 4
0 0 1
1 0 2
1 0 5
write 1 hit
0 0 4
1 1 1
1 0 2
1 0 5
read 2 hit
0 0 4
1 1 1
1 0 2
1 0 5
read 3 miss
1 1 1
1 0 2
1 0 5
1 0 3
write 4 miss
0 0 5
0 0 3
0 0 1
1 1 4
<<<<<<< HEAD
=======

经验证，以上结果正确。
>>>>>>> 790bc39b948c60181c85f58654a11682d075cbe2
```
 
## 扩展思考题
（1）了解LIRS页置换算法的设计思路，尝试用高级语言实现其基本思路。此算法是江松博士（导师：张晓东博士）设计完成的，非常不错！

参考信息：

 - [LIRS conf paper](http://www.ece.eng.wayne.edu/~sjiang/pubs/papers/jiang02_LIRS.pdf)
 - [LIRS journal paper](http://www.ece.eng.wayne.edu/~sjiang/pubs/papers/jiang05_LIRS.pdf)
 - [LIRS-replacement ppt1](http://dragonstar.ict.ac.cn/course_09/XD_Zhang/(6)-LIRS-replacement.pdf)
 - [LIRS-replacement ppt2](http://www.ece.eng.wayne.edu/~sjiang/Projects/LIRS/sig02.ppt)
