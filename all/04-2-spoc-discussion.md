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

> 采用数学归纳法证明。

> 初始条件显然满足，现在假设 t - 1 时满足 S(t - 1) ⊂ S'(t - 1) 。

> 考虑 t 时刻的情况，若访问的页面 b(t) ∈ S(t - 1) 且 b(t) ∈ S'(t - 1) ，则访问之后的 S(t) S'(t) 均不发生变化，成立。

> 若访问的页面 b(t) ∉ S(t - 1) 但是 b(t) ∈ S'(t - 1) ，那么按照 LRU 替换掉 S(t - 1) 中最不常使用的页面换成 b(t) 后仍满足 S(t) ⊂ S'(t) 。成立。

> 若 b(t) ∉ S(t - 1) 而且 b(t) ∉  S'(t - 1)，设 c 是 S 中替换掉的页面，若 c 也是 S' 中替换掉的页面的话，结论成立，因为刚好替换掉了相同的页面换了同一个页面；若 c 不是 S' 替换掉的页面，则 S' 替换掉的页面 d ∉ S(t - 1) ，因为 c d 均在 S'(t - 1) 中，替换 d 说明 d 比 c 更不常使用，若 d 在 S 中则应该替换掉 d 而不是 c 。所以原命题依然成立。

> 综上所述，命题得证。

(2)（spoc）根据你的`学号 mod 4`的结果值，确定选择四种替换算法（0：LRU置换算法，1:改进的clock 页置换算法，2：工作集页置换算法，3：缺页率置换算法）中的一种来设计一个应用程序（可基于python, ruby, C, C++，LISP等）模拟实现，并给出测试。请参考如python代码或独自实现。
 - [页置换算法实现的参考实例](https://github.com/chyyuu/ucore_lab/blob/master/related_info/lab3/page-replacement-policy.py)

代碼如下：
```
# LRU simulation in python

class Page:
    index = int()
    mod = int()

    def __init__(self, index, mod):
        self.index = index
        self.mod = mod

MAX_LEN = 5
        
def visitPage(page_list, index):
    found = False
    for i in range(len(page_list)):
        if(page_list[i].index == index):
            found = True
            for i in range(len(page_list)):
                if page_list[i].index != index:
                    page_list[i].mod += 1
                else:
                    page_list[i].mod = 0
            break

    if not found:
        # get the index of the oldest page
        m = -1
        ind = -1
        for i in range(len(page_list)):
            if page_list[i].mod > m:
                m = page_list[i].mod
                ind = i

        if len(page_list) >= MAX_LEN:
            page_list.pop(ind)
        page_list.append(Page(index, 0))
        for i in range(len(page_list)):
            if page_list[i].index != index:
                page_list[i].mod += 1
        
def showList(page_list):
    for page in page_list:
        print 'index: %d; \t mod: %d ' %(page.index, page.mod)
    print '\n'

                
def test():
    page_list = list()
    for i in range(1, 6):
        visitPage(page_list, i)
        showList(page_list)

    visitPage(page_list, 3)
    showList(page_list)
    visitPage(page_list, 1)
    showList(page_list)
    visitPage(page_list, 6)
    showList(page_list)


if __name__ == '__main__':
    test()
```
 
## 扩展思考题
（1）了解LIRS页置换算法的设计思路，尝试用高级语言实现其基本思路。此算法是江松博士（导师：张晓东博士）设计完成的，非常不错！

参考信息：

 - [LIRS conf paper](http://www.ece.eng.wayne.edu/~sjiang/pubs/papers/jiang02_LIRS.pdf)
 - [LIRS journal paper](http://www.ece.eng.wayne.edu/~sjiang/pubs/papers/jiang05_LIRS.pdf)
 - [LIRS-replacement ppt1](http://dragonstar.ict.ac.cn/course_09/XD_Zhang/(6)-LIRS-replacement.pdf)
 - [LIRS-replacement ppt2](http://www.ece.eng.wayne.edu/~sjiang/Projects/LIRS/sig02.ppt)
