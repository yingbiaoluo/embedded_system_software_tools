Linux进程调度
==========

## 一、进程
进程：通常把进程定义为程序执行的一个实例。
*A process is usually defined as an instance of a program in execution.*<br>
进程的状态：

## 二、进程调度

## 三、进程间通信：

### 3.1 管道（pipes）
管道是进程之间的一个单向数据流：一个进程写入管道的所有数据都由内核定向到另一个进程，另一个进程由此就可以从管道中读取数据。<br>
A *pipe* is a one-way flow of data between processes: all data written by a process to the pipe is routed by the kernel to another process, which can thus read it.

### 3.2 FIFO

<br>
<br>
<br>

附～～～～～面经～～～～～：
> 抓住面试官想问的点
>
> 有些同学被问到一些自己会的基础知识的时候会特别激动，想抓住这个机会表现自己，就会事无巨细地回答一波。我个人认为，如果是基础知识的话，其实不用回答得特别详细，说出一些面试官想问的关键要点就可以了。有时候不一定能判断出来面试官想问的要点，这也不要紧，就说一些自己认为是关键的要点，然后等着面试管继续问follow-up就可以了。这里举一个简单的例子，**如果面试官问进程与线程的区别，那么简单地说线程是调度的最小单位，同一个进程的线程共享地址空间，容易有线程安全问题；进程是多数资源分配的最小单位，所以进程的地址空间都是独立的，资源安全问题相对较少。回答到这个份上就差不多够了**，然后等面试官继续问follow-up，而不需要去解释为什么线程会有安全问题等。之所以建议这么做，是因为对于有些公司，面试时间是有限制的（例如Google, hulu等），所以面试时间是很宝贵的，你应该用这珍贵的时间去展示自己的优势，而不是说一些绝大部分人都懂的trivial的知识。当然了，有经验/不nice的面试官可能会打断你，问他自己感兴趣想问的东西，但如果你运气不好恰好面试官没啥经验或者不喜欢打断人，那这样浪费宝贵的时间是很可惜的。
>
> 来自GitHub：[写在20年初的校招面试心得与自学CS经验及找工作分享](https://github.com/conanhujinming/tips_for_interview/blob/master/README-zh_CN.md#编程部分心得)
