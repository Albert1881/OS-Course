Uniprogramming means one program sits in the main memory at a time. That is only one program runs on the computer at a time. These types of operating systems in which one program runs at a time are known as Uniprogramming operating systems.

The shortcoming is that it is very inefficient (because only one program can exist in memory at a time, and if you need to run more than one program, you need to read and write memory repeatedly). And it is very weak in functionality, because there is no way to really run more than one program at a time.

Not efficient; Not powerful.



Multiprogramming means multiple programs reside in main memory (RAM) at a time. OS which handles multiple programs at a time is known as a multiprogramming operating system. One processor or CPU can only run one process at a time. OS uses context switching in the main memory for running multiple programs.

Memory allocation becomes more complex. with memory isolation between processes, paging, and so on, resulting in internal and external fragmentation. Need to take into account memory isolation between processes, paging, and so on. Result in internal and external fragmentation.

Pros

1. Make data more “logical”, easy to share 
2. We can do much better on “protection”
3. Reduce fragmentation

Cons 

1. Segment size is not fixed, more complicated
2. One process need allocate memory many times
3. Fragmentation is smaller but still a problem

Paging is a storage mechanism that allows OS to retrieve processes from the secondary storage into the main memory in the form of pages. In the Paging method, the main memory is divided into small fixed-size blocks of physical memory, which is called frames. The size of a frame should be kept the same as that of a page to have maximum utilization of the main memory and to avoid external fragmentation. Paging is used for faster access to data, and it is a logical concept.

Pros

1. Simple to implement
2. Reduce external fragmentation
3. Demand paging technique (learn latter)

Cons 

1. Page table requires extra memory space
2. Internal fragmentation problem



Internal fragmentation happens when the memory is split into mounted sized blocks. Whenever a method request for the memory, the mounted sized block is allotted to the method. just in case the memory allotted to the method is somewhat larger than the memory requested, then the distinction between allotted and requested memory is that the Internal fragmentation. 

内部碎片：当内存被分割成固定大小的块时，内部碎片就会发生。每当一个方法请求内存时，固定大小的块就分配给该方法。如果分配给方法的内存比请求的内存稍大一些，那么分配的和请求的内存之间的区别是内部碎片。

方法：

1. 采用分页的方式，分页使得内碎片只会在进程的最后一页出现。
2. 采用动态分区的方式，使用First Fit，Next Fit，Best Fit，Worst Fit动态分区分配算法。使得空间正好适合进程。
3. 设定阈值，限制每次分配空间时内碎片的量。

原因：

1. 分页的方式可以使得内碎片只在进程的最后一页产生，限制了内碎片的大小

2. 动态分区可以使内碎片不会产生，每次进程申请的内存与分配的相等。 FF尽量使用低地址，BF尽量使用刚好的块，WF尽量使用最大的块。从不同方面使得内存利用效率变高。 

3. 设定阈值可以使每次分配时的冗余内存得到控制。

   

   External fragmentation happens when there’s a sufficient quantity of area within the memory to satisfy the memory request of a method. however the process’s memory request cannot be fulfilled because the memory offered is during a non-contiguous manner. Either you apply first-fit or best-fit memory allocation strategy it’ll cause external fragmentation. 

1. 使用分页的方式,使得内碎片只会在进程的最后一页出现;使用动态分区,使得空间正好适合进程(当然,这种方式会使得外碎片产生);设定阈值,限制每次分配空间时内碎片的量;使用Best Fit的方式,尽量分配最适合的块给进程.
2. 分页的方式可以使得内碎片只在进程的最后一页产生,相对来说限制了内碎片的大小和出现位置,较为方便管理.(虽然每次都会出现,但是有了限制).
3. 动态分区则完全使内碎片不存在,因为每次分配的空间都正好适合进程.
4. 阈值可以使每次分配时的冗余空间得到控制.
5. Best Fit的方式和阈值配合,可以使每次分配的冗余空间尽量小.

有多种分配内存算法，FF，BF，WF，同时设定阈值，在分配时不产生小于阈值的块

 FF尽量使用低地址，BF尽量使用刚好的块，WF尽量使用最大的块。从不同方面使得内存利用效率变高。

 

外部碎片：当内存分配器在分配的内存部分之间留下未使用的内存块部分时，就会发生外部碎片。

方法：

1. 使用分页的方式；
2. 使用 Worst Fit 分配算法；
3. 使用压缩碎片的方式；

原因：

1. 分页时，进程以页为单位申请内存，因此不会存在外碎片；
2. Worst Fit 分配算法，将空闲分区要按大小从大到小进行排序，自表头开始查找到第一个满足要求的自由分区分配。该算法保留小的空闲区，尽量减少小的外碎片产生；
3. 压缩手段可以将不连续的空闲内存聚合到一起成为一块大空间，使得进程之间的外部碎片减少；