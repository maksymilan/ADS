Texture Packing is to pack multiple rectangle shaped textures into one large texture.  The resulting texture must have a given width and a minimum height.

This project requires you to design an approximation algorithm that runs in polynomial time.  You must generate test cases of different sizes (from 10 to 10,000) with different distributions of widths and heights.  A thorough analysis on all the factors that might affect the approximation ratio of your proposed algorithm is expected.

Grading Policy:
Programming: Implement the approximation algorithm (6 pts.).  Write a test of performance program (1 pts.).  All the codes must be sufficiently commented.

Testing: Generate the test cases and give the run time table (2 pts.).  Plot the run times vs. input sizes for illustration (2 pts.).  Write analysis and comments (5 pts.).  The analysis must be based on your testing results.

Documentation: Chapter 1 (1 pt.), Chapter 2 (2 pts.), and finally a complete report (1 point for overall style of documentation).

Bonus: Compare your algorithm to any of the known strip algorithms.

Note: Anyone who does excellent job on answering the Bonus question will gain an extra point.

近似算法的思路：
我目前准备实现一个离线和一个在线算法，主要想法是这样
离线算法：先对所有的矩形调整为水平放置的状态（输入数据保证矩形长宽均小于给定的容器宽度），然后按照宽度进行排序，先放置最宽的，然后在剩下的矩形里选择能填充的并且填充之后剩余空间最小的进行填充，直到填不下为止。依次循环，直到所有矩形都被放进去
在线算法：选择一个矩形进行填充，然后下一个矩形如果能放下就放入，如果不能放下就放在这个矩形的顶上（最简单的放法）

实现了三种方法，第一种就是LM按照输入顺序随机放，第二种是将所有矩形变化为竖着放置，然后按照高度从高到低排放，第三种是将所有矩形横着放置，按照宽度从高到低进行排放
对于第二种方法，如果宽度足够大，那么高度最高就是随机数生成的最大值，

最后输出三种算法的文件，每一个文件分别对应一种算法，文件内分三列，分别是矩形的个数，输出的结果，所用的时间