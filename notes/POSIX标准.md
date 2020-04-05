# POSXI标准

POSIX标准原文：https://pubs.opengroup.org/onlinepubs/9699919799/nframe.html  
GNU C库：https://www.gnu.org/software/libc/manual/html_node/index.html#SEC_Contents

POSIX表示**可移植操作系统接口(Portable Operating System Interface of UNIX，缩写为 POSIX)** ，POSIX标准定义了操作系统应该为应用程序提供的接口标准。

POSIX标准意在期望获得源代码级别的软件可移植性。换句话说，为一个POSIX兼容的操作系统编写的程序，应该可以在任何其它的POSIX操作系统（即使是来自另一个厂商）上编译执行。

**简单总结：**

完成同一功能，不同内核提供的系统调用（也就是一个函数）是不同的，例如创建进程，linux下是fork函数，windows下是creatprocess函数。好，我现在在linux下写一个程序，用到fork函数，那么这个程序该怎么往windows上移植？我需要把源代码里的fork通通改成creatprocess，然后重新编译...

posix标准的出现就是为了解决这个问题。linux和windows都要实现基本的posix标准，linux把fork函数封装成posix_fork（随便说的），windows把creatprocess函数也封装成posix_fork，都声明在unistd.h里。这样，程序员编写普通应用时候，只用包含unistd.h，调用posix_fork函数，程序就在源代码级别可移植了。
(来自[CSDN博客](https://blog.csdn.net/weixin_40039738/article/details/81867577?depth_1-utm_source=distribute.pc_relevant.none-task-blog-BlogCommendFromBaidu-1&utm_source=distribute.pc_relevant.none-task-blog-BlogCommendFromBaidu-1))

C/C++的标准库是指ANSI C/C++定义的标准库，这个是构成标准C/C++语言的基础。简称libc/libc++，常见的libc/libc++的实现有：UNIX C/C++，GNU C/C++，MicroSoft Visual C/C++  

在标准C/C++库基础上，兼容POSIX协议的C/C++库就是POSIX标准的C/C++库，这些C/C++库可以直接操作符合POSIX标准的操作系统调用。
常见的POSIX标准C/C++库有：<br>
2.1 BSD，这个库是AOSP使用的标准C库。  
2.2 Bionic，这个是BSD的衍生库，用于NDK开发  
2.3 UNIX C/C++，GUN C/C++，MicroSoft Visual C/C++


[wiki链接](https://zh.wikipedia.org/zh-hans/可移植操作系统接口)
