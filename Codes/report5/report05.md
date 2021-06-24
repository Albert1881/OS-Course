 * 
1. What is deadlock?什么是死锁.
  Answer:

  死锁是指两个或两个以上的进程在执行过程中，由于竞争资源或者由于彼此通信而造成的一种阻塞的现象，若无外力作用，它们都将无法推进下去。此时称系统处于死锁状态或系统产生了死锁，这些永远在互相等待的进程称为死锁进程

  死锁是一种状态,指的是操作系统中,一组进程中任何一个进程都在等待组中另外一个或多个进程(包括自身)所占用的资源(比如发送信息,将某个锁释放等等,打印机等外设等等),因而整组进程都无法执行的状态.线程也可以发生死锁,但是此时所占用的资源的定义与进程不同,主要为各种锁(因为进程内所有线程的文件等资源是公用的).一般来说,我们所说的是进程的死锁.线程死锁会单独称为`Thread deadlock`

2. What are the requirements of deadlock?

    1. Mutual exclusion

       Only one thread at a time can use a resource.

    2. Hold and wait

       Thread holding at least one resource is waiting to acquire additional resources held by other threads

    3. No preemption

       Resources are released only voluntarily by the thread holding the resource, after thread is finished with it

    4. Circular wait

       There exists a set {P1 , …, Pn } of waiting processes

       P1 is waiting for a resource that is held by P2 

       P2 is waiting for a resource that is held by P3 

       …

       Pn is waiting for a resource that is held by P1

    1. 资源不可以被共用,只能被单独进程所使用.在被进程使用时,其他申请的进程必须等待.
    2. 申请到资源使用权限的进程可以无限期等待,同时持有资源.(进程不主动给出资源).
    3. 进程申请到的资源不会被强制收回.(OS不会主动将资源索回,同时也要求).
    4. 有一组进程,其中进程之间互相持有所需要的资源.
    + 注解:这些条件是deadlock发生的必要不充分条件.

3. What’s different between deadlock prevention and deadlock avoidance?

    1. deadlock prevention 通过破坏死锁产生的四个必要条件之一来防止死锁的发生。好处在于不需要检测流程请求等,避免了检测的成本.但是缺点在于系统的效率会下降.。

       1) 破坏互斥条件

       如果允许系统资源都能共享使用，则系统不会进入死锁状态。但有些资源根本不能同时访问，如打印机等临界资源只能互斥使用。所以，破坏互斥条件而预防死锁的方法不太可行，而且在有的场合应该保护这种互斥性。

       2) 破坏不剥夺条件

       当一个已保持了某些不可剥夺资源的进程，请求新的资源而得不到满足时，它必须释放已经保持的所有资源，待以后需要时再重新申请。这意味着，一个进程已占有的资源会被暂时释放，或者说是被剥夺了，或从而破坏了不可剥夺条件。

       该策略实现起来比较复杂，释放已获得的资源可能造成前一阶段工作的失效，反复地申请和释放资源会增加系统开销，降低系统吞吐量。这种方法常用于状态易于保存和恢复的资源，如CPU的寄存器及内存资源，一般不能用于打印机之类的资源。

       3) 破坏请求和保持条件

       釆用预先静态分配方法，即进程在运行前一次申请完它所需要的全部资源，在它的资源未满足前，不把它投入运行。一旦投入运行后，这些资源就一直归它所有，也不再提出其他资源请求，这样就可以保证系统不会发生死锁。

       这种方式实现简单，但缺点也显而易见，系统资源被严重浪费，其中有些资源可能仅在运行初期或运行快结束时才使用，甚至根本不使用。而且还会导致“饥饿”现象，当由于个别资源长期被其他进程占用时，将致使等待该资源的进程迟迟不能开始运行。

       4) 破坏循环等待条件

       为了破坏循环等待条件，可釆用顺序资源分配法。首先给系统中的资源编号，规定每个进程，必须按编号递增的顺序请求资源，同类资源一次申请完。也就是说，只要进程提出申请分配资源Ri，则该进程在以后的资源申请中，只能申请编号大于Ri的资源。

    2. deadlock avoidence 系统对进程发出每一个系统能够满足的资源申请进行动态检查，并根据检查结果决定是否分配资源。如果分配后系统可能发生死锁,则不予分配,否则予以分配。这是一种保证系统不进入死锁状态的动态策略。优点时只在进程申请时才会检测，不会限制系统效率。缺点是需要进程预先告知所需的资源请求，而且也要明确系统已有的资源，也不可以出现资源被进程申请后没有被释放就直接退出的情况。

    3. 区别

       1）deadlock prevention 在运行时发生,防止四个条件出现，而deadlock avoidence 是在资源申请的时候进行

       2）deadlock prevention不需要对系统资源的情况有了解,而deadlock avoidence 需要对系统资源,进程申请资源都很明确

       3）deadlock prevention会阻止死锁的四个条件发生,但是 deadlock avoidence 之后有可能会将死锁的四个条件凑齐,但是不会发生死锁

       4）deadlock prevention是操作系统对用户程序限制的（限制其申请资源），deadlock avoidence 是操作系统对进程和进程之间的（对用户程序不加限制）。

       

4. How to prevent deadlock? Give at least two examples.

    1) 破坏互斥条件

    如果允许系统资源都能共享使用，则系统不会进入死锁状态。

    2) 破坏不剥夺条件

    当一个已保持了某些不可剥夺资源的进程，请求新的资源而得不到满足时，它必须释放已经保持的所有资源，待以后需要时再重新申请。这意味着，一个进程已占有的资源会被暂时释放，或者说是被剥夺了，或从而破坏了不可剥夺条件。这种方法常用于状态易于保存和恢复的资源，如CPU的寄存器及内存资源，一般不能用于打印机之类的资源。

    3) 破坏请求和保持条件

    釆用预先静态分配方法，即进程在运行前一次申请完它所需要的全部资源，在它的资源未满足前，不把它投入运行。一旦投入运行后，这些资源就一直归它所有，也不再提出其他资源请求，这样就可以保证系统不会发生死锁。

    4) 破坏循环等待条件

    为了破坏循环等待条件，可釆用顺序资源分配法。首先给系统中的资源编号，规定每个进程，必须按编号递增的顺序请求资源，同类资源一次申请完。也就是说，只要进程提出申请分配资源Ri，则该进程在以后的资源申请中，只能申请编号大于Ri的资源。

    

5. Which way does recent UNIX OS choose to deal with deadlock problem, why?
    1. UNIX OS 使用鸵鸟算法，具体来讲就是忽略死锁的存在，因而将死锁转交给进程自己处理。
    2. 因为死锁发生的概率较低，而且避免死锁的代价比较高。因此将检测死锁的职责转交给上层的应用程序(比如数据库等)比较合适。

6. What data structures you use in your implementation（of Banker's algorithm）? Where and why you use them? Are they optimal for your purpose?
   1. 使用了string,vector,unordered_map.
   2. 使用string为了方便接受读入的命令名称,使用vector为了存储资源的现有量,以及进程们的可用量与分配量,使用unordered_map是为了将进程和进程的可用量之间关联起来.
   3. string用在了读取命令处,方便使用.vector既使用在了资源的现有量处,也使用在了unordered_map的value中,存储进程们的资源可用值和已分配值.
   4. 我认为这是最好的选择,首先string比较方便管理,在只需要一次使用的时候比较方便,vector可以动态声明长度,适合资源种类依照输入变化的情况,unordered_map可以将进程的pid和其对应的资源vector联系起来,都是比较适合的数据结构.