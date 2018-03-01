# OS课程练习
这里的习题用于清华大学计算机系2018年春季课程练习。


这里包括从互联网上搜集的操作系统课程相关习题和答案，包括部分考研试题，版权属于各出题单位或个人。由清华大学OS课的老师和助教撰写的习题和答案的文档版权属于陈渝、向勇，并采用 Creative Commons Attribution/Share-Alike (CC-BY-SA) License.

OS习题集采用gitbook的方式展现，可进行在线交互式答题。

下面是gitbook提供的试题编写的例子：
## mcqx test1

{%mcq ans="o1"%}
{%title%} This is a question?
{%o1%} First option
{%o2%} Second option
{%o3%} Third option
{%o4%} Fourth option
{%endmcq%}

## mcqx test2

{%mcq ans="o1", count=2%}
{%title%} This is a question?
{%o1%} First option
{%o2%} Second option
{%o3%} Third option
{%o4%} Fourth option
{%hint%} This is a hint.
{%endmcq%}

## mcqx test3

{%mcq ans="o1", count=2%}
{%title%} This is a question?
{%o1%} First option
{%o2%} Second option
{%o3%} Third option
{%o4%} Fourth option
{%o5%} Fourth option
{%o6%} Fourth option
{%o7%} Fourth option
{%o8%} Fourth option
{%endmcq%}

## mcqx test4

{%mcq ans="o1", random=true%}
{%title%} This is a question?
{%o1%} First option
{%o2%} Second option
{%o3%} Third option
{%o4%} Fourth option
{%endmcq%}

## mcqx test5


## spoiler test6

This is a spoiler: {%s%}Hello World.{%ends%}

## fbq test7

{%fbq%}Testing. Please type $$hello## (hello) and $$world## (world).{%endfbq%}

## quiz test8

<quiz name="Gitbook Quiz">
    <question multiple>
        <p>What is gitbook used for?</p>
        <answer correct>To read books</answer>
        <answer>To book hotel named git</answer>
        <answer correct>To write and publish beautiful books</answer>
        <explanation>GitBook.com lets you write, publish and manage your books online as a service.</explanation>
    </question>
    <question>
        <p>Is it quiz?</p>
        <answer correct>Yes</answer>
        <answer>No</answer>
    </question>
    <question>
        <p>This is multiple dropdown quiz, in each dropdown select a correct number corresponding to the dropdown's order</p>
        <answer>
            <option correct>First</option>
            <option>Second</option>
            <option>Third</option>
            <option>Fourth</option>
        </answer>
        <answer>
            <option>First</option>
            <option correct>Second</option>
            <option>Third</option>
            <option>Fourth</option>
        </answer>
        <answer>
            <option>First</option>
            <option>Second</option>
            <option correct>Third</option>
            <option>Fourth</option>
        </answer>
        <answer>
            <option>First</option>
            <option>Second</option>
            <option>Third</option>
            <option correct>Fourth</option>
        </answer>
    </question>
</quiz>

