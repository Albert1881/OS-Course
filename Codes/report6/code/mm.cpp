#include <bits/stdc++.h>
#include <unistd.h>

using namespace std;

#define PROCESS_NAME_LEN 32      //进程名最大长度
#define MIN_SLICE 10          //内碎片最大大小
#define DEFAULT_MEM_SIZE 1024 //总内存大小
#define DEFAULT_MEM_START 0      //内存开始分配时的起始地址

typedef pair<int, string> My_algo;

int mem_size = DEFAULT_MEM_SIZE;
bool flag = 0; //当内存以及被分配了之后，不允许更改总内存大小的flag
static int pid = 0;
My_algo algo;

struct free_block { //空闲数据块
    int size;
    int start_addr;
    struct free_block *next;
};

struct allocated_block { //已分配的数据块
    int pid;
    int size;
    int start_addr;
    int *data;
    struct allocated_block *next;
};

free_block *free_block_head;                  //空闲数据块首指针
allocated_block *allocated_block_head = NULL; //分配块首指针

allocated_block *find_process(int id);       //寻找pid为id的分配块
free_block *init_free_block(int mem_size); //空闲块初始化
allocated_block *init_allocated_block(int mem_size);

void display_menu();                   //显示选项菜单
void set_mem_size();                   //设置内存大小
int allocate_mem(allocated_block *ab); //为制定块分配内存
void rearrange();                       // 对块进行重新分配
int create_new_process();               //创建新的进程
int free_mem(allocated_block *ab);       //释放分配块
void swap(int *p, int *q);               //交换地址
int dispose(allocated_block *ab);       //释放分配块结构体
void display_mem_usage();               //显示内存情况
void kill_process();                   //杀死对应进程并释放其空间与结构体
void Usemy_algo(int id);               //使用对应的分配算法

int First_Fit(allocated_block *ab);

int Worst_Fit(allocated_block *ab);

int Best_Fit(allocated_block *ab);

int Buddy_System(allocated_block *ab);

int next_power(int size);

void buddy_rearrange();

int allocate(free_block *pre, free_block *fb, allocated_block *ab);

int buddy_allocate(free_block *pre, free_block *fb, allocated_block *ab);

int buddy_free_mem(allocated_block *ab);

//主函数
int main() {
    int op;
    pid = 0;
    free_block_head = init_free_block(mem_size); //初始化一个可以使用的内存块，类似与操作系统可用的总存储空间
    for (;;) {
        sleep(1);
        display_menu();
        fflush(stdin);
        scanf("%d", &op);
        switch (op) {
            case 1: {
                set_mem_size();
                break;
            }
            case 2: {
                printf("Choose an algorithm\n");
                printf(" 1: Best Fit\n 2: Worst Fit\n 3: First Fit\n 4: Buddy System\n");
                int alg;
                scanf("%d", &alg);
                Usemy_algo(alg);
                break;
            }
            case 3: {
                create_new_process();
                break;
            }
            case 4: {
                kill_process();
                break;
            }
            case 5: {
                display_mem_usage();
                break;
            }
            case 233: {
                puts("bye....");
                sleep(1);
                return 0;
            }
            defaut:
                break;
        }
    }
}

allocated_block *find_process(int id) { //循环遍历分配块链表，寻找pid=id的进程所对应的块
    allocated_block *ab = allocated_block_head;
    while (ab != NULL) {
        if (ab->pid == id) {
            return ab;
        }
        ab = ab->next;
    }
    return NULL;
}

free_block *init_free_block(int mem_size) { //初始化空闲块，这里的mem_size表示允许的最大虚拟内存大小
    free_block *p;
    p = (free_block *) malloc(sizeof(free_block));
    if (p == NULL) {
        puts("No memory left");
        return NULL;
    }
    p->size = mem_size;
    p->start_addr = DEFAULT_MEM_START;
    p->next = NULL;
    return p;
}

allocated_block *init_allocated_block(int mem_size) {
    allocated_block *p;
    p = (allocated_block *) malloc(sizeof(allocated_block));
    if (p == NULL) {
        puts("No memory left");
        return NULL;
    }
    p->pid = ++pid;
    p->size = mem_size;
    p->start_addr = DEFAULT_MEM_START;
    p->next = NULL;
    return p;
}

void display_menu() {
    puts("\n\n******************menu*******************");
    printf("1) Set memory size (default = %d)\n", DEFAULT_MEM_SIZE);
    printf("2) Set memory allocation algorithm\n");
    printf("3) Create a new process\n");
    printf("4) Kill a process\n");
    printf("5) Display memory usage\n");
    printf("233) Exit\n");
}

void set_mem_size() { //更改最大内存大小
    if (flag == 1) {
        printf("Flag has been set. Memmory size can't be set.\n");
        return;
    } else if (allocated_block_head != NULL) {
        printf("Block has been allocated. Memmory size can't be set.\n");
        return;
    }
    int temp_mem_size = DEFAULT_MEM_SIZE;
    printf("Please set memory size\n");
    scanf("%d", &temp_mem_size);
    if (temp_mem_size <= 0) {
        printf("The memory size must be greater than zero. Memmory size can't be set.\n");
        return;
    }
    mem_size = temp_mem_size;
    if (free_block_head == NULL) {
        free_block_head = init_free_block(mem_size);
    } else {
        free_block_head->size = mem_size;
    }
    flag = 1;
    printf("Memmory size is set successfully.\n");
}

void Usemy_algo(int id) {
    switch (id) {
        case 1:
            algo = make_pair(1, "Best Fit");
            break;
        case 2:
            algo = make_pair(2, "Worst Fit");
            break;
        case 3:
            algo = make_pair(3, "First Fit");
            break;
        case 4:
            algo = make_pair(4, "Buddy System");
            break;
        default:
            printf("Algorithm number must between 1-4.\n");
            break;
    }
}

int allocate_mem(allocated_block *ab) { //为块分配内存，真正的操作系统会在这里进行置换等操作
    if (free_block_head == NULL) {
        return -1;
    }

    if (algo.first == NULL) {
        printf("Please set algorithm\n");
        return -1;
    }
    switch (algo.first) {
        case 1:
            Best_Fit(ab);
            break;
        case 2:
            Worst_Fit(ab);
            break;
        case 3:
            First_Fit(ab);
            break;
        case 4:
            Buddy_System(ab);
            break;
        default:
            printf("Algorithm number must between 1-4.\n");
            return -1;
    }
    return 1;
}

int First_Fit(allocated_block *ab) {
    rearrange();

    struct free_block *pre = free_block_head;
    struct free_block *fb = pre->next;
    if (pre->size >= ab->size) {
        allocate(NULL, pre, ab);
        return 1;
    }
    while (fb != NULL) {
        if (fb->size >= ab->size) {
            allocate(pre, fb, ab);
            return 1;
        }
        pre = pre->next;
        fb = pre->next;
    }
    return -1;
}

int Worst_Fit(allocated_block *ab) {
    struct free_block *pre = free_block_head;
    struct free_block *fb = pre->next;
    struct free_block *pwf_fb = NULL;
    struct free_block *wf_fb = NULL;

    if (pre->size >= ab->size) {
        wf_fb = pre;
    }
    while (fb != NULL) {
        if (fb->size >= ab->size && fb->size > wf_fb->size) {
            pwf_fb = pre;
            wf_fb = fb;
        }
        pre = pre->next;
        fb = pre->next;
    }
    if (wf_fb == NULL) {
        return -1;
    } else {
        return allocate(pwf_fb, wf_fb, ab);
    }
}

int Best_Fit(allocated_block *ab) {
    struct free_block *pre = free_block_head;
    struct free_block *fb = pre->next;
    struct free_block *pbf_fb = NULL;
    struct free_block *bf_fb = NULL;

    if (pre->size >= ab->size) {
        bf_fb = pre;
    }
    while (fb != NULL) {
        if (fb->size >= ab->size && fb->size < bf_fb->size) {
            pbf_fb = pre;
            bf_fb = fb;
        }
        pre = pre->next;
        fb = pre->next;
    }
    if (bf_fb == NULL) {
        return -1;
    } else {
        return allocate(pbf_fb, bf_fb, ab);
    }
}

int Buddy_System(allocated_block *ab) {
    buddy_rearrange();
    if (ab->size < MIN_SLICE) {
        ab->size = next_power(MIN_SLICE);
    } else {
        ab->size = next_power(ab->size);
    }
    struct free_block *pre = free_block_head;
    struct free_block *fb = pre->next;
    if (pre->size >= ab->size) {
        return buddy_allocate(NULL, pre, ab);
    }
    while (fb != NULL) {
        if (fb->size >= ab->size) {
            return buddy_allocate(pre, fb, ab);
        }
        pre = pre->next;
        fb = pre->next;
    }
    return -1;
}

int buddy_allocate(free_block *pre, free_block *fb, allocated_block *ab) {
    while (fb->size > ab->size) {
        int size = fb->size;
        fb->size = next_power(fb->size / 2);
        int buddy_size = size - fb->size;
        if (buddy_size < MIN_SLICE) {
            printf("Buddy allocate fail.\n");
            return -1;
        }
        free_block *buddy_block = init_free_block(buddy_size);
        buddy_block->start_addr = fb->start_addr + fb->size;
        buddy_block->next = fb->next;
        fb->next = buddy_block;
    }
    if (fb->size == ab->size) {
        ab->start_addr = fb->start_addr;
        if (pre == NULL) {
            free_block_head = fb->next;
        } else {
            pre->next = fb->next;
        }

        if (allocated_block_head == NULL) {
            allocated_block_head = ab;
        } else {
            allocated_block *temp = allocated_block_head;
            while (temp->next != NULL) {
                temp = temp->next;
            }
            temp->next = ab;
        }
        free(fb);
        return 1;
    }
    return -1;
}


int next_power(int size) {
    /* depend on the fact that size < 2^32 */
    size -= 1;
    size |= (size >> 1);
    size |= (size >> 2);
    size |= (size >> 4);
    size |= (size >> 8);
    size |= (size >> 16);
    return size + 1;
}

int allocate(free_block *pre, free_block *fb, allocated_block *ab) {
    if (fb->size < ab->size) {
        return -1;
    }
    ab->start_addr = fb->start_addr;
    if (fb->size - ab->size < MIN_SLICE) {
        ab->size = fb->size;
        if (pre != NULL) {
            pre->next = fb->next;
        } else {
            free_block_head = fb->next;
        }
        free(fb);
    } else {
        fb->start_addr += ab->size;
        fb->size -= ab->size;
    }

    if (allocated_block_head == NULL) {
        allocated_block_head = ab;
    } else {
        allocated_block *temp = allocated_block_head;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = ab;
    }
    return 1;
}

int create_new_process() { //创建新进程
    int mem_sz = 0;
    printf("Please input memory size\n");
    scanf("%d", &mem_sz);
    // Write your code here
    if (mem_sz <= 0 || mem_sz > mem_size) {
        printf("Memory size must between 0 and %d. Can't create process.\n", mem_size);
        return -1;
    }
    if (free_block_head == NULL) {
        printf("No free memory. Can't create process.\n");
        return -1;
    }
    allocated_block *ab = init_allocated_block(mem_sz);
    int result = allocate_mem(ab);
    if (result == -1) {
        free(ab);
    }
    return result;
}

void swap(int *p, int *q) {
    int tmp = *p;
    *p = *q;
    *q = tmp;
    return;
}

void rearrange() { //将块按照地址大小进行排序
    free_block *tmp, *tmpx;
    puts("Rearrange begins...");
    puts("Rearrange by address...");
    tmp = free_block_head;
    while (tmp != NULL) {
        tmpx = tmp->next;
        while (tmpx != NULL) {
            if (tmpx->start_addr < tmp->start_addr) {
                swap(&tmp->start_addr, &tmpx->start_addr);
                swap(&tmp->size, &tmpx->size);
            }
            tmpx = tmpx->next;
        }
        tmp = tmp->next;
    }
    usleep(500);
    puts("Rearrange Done.");
}

void buddy_rearrange() { //将块按照地址大小进行排序
    free_block *tmp, *tmpx;
    puts("Rearrange begins...");
    puts("Rearrange by address...");
    tmp = free_block_head;
    while (tmp != NULL) {
        tmpx = tmp->next;
        while (tmpx != NULL) {
            if (tmpx->size < tmp->size || (tmpx->size == tmp->size && tmpx->start_addr < tmp->start_addr)) {
                swap(&tmp->start_addr, &tmpx->start_addr);
                swap(&tmp->size, &tmpx->size);
            }
            tmpx = tmpx->next;
        }
        tmp = tmp->next;
    }
    usleep(500);
    puts("Rearrange Done.");
}

int free_mem(allocated_block *ab) { //释放某一块的内存
    free_block *nfb = init_free_block(ab->size);
    nfb->start_addr = ab->start_addr;

    if (free_block_head == NULL) {
        free_block_head = nfb;
        return 1;
    }
    nfb->next = free_block_head;
    free_block_head = nfb;

    rearrange();
    free_block *pre = free_block_head;
    free_block *fb = pre->next;

    while (fb != NULL) {
        if (pre->start_addr + pre->size == fb->start_addr) {
            pre->size += fb->size;
            pre->next = fb->next;
            free(fb);
        } else {
            pre = pre->next;
        }
        fb = pre->next;
    }
    return 1;
}


int buddy_free_mem(allocated_block *ab) {
    free_block *nfb = init_free_block(ab->size);
    nfb->start_addr = ab->start_addr;

    if (free_block_head == NULL) {
        free_block_head = nfb;
        return 1;
    }
    nfb->next = free_block_head;
    free_block_head = nfb;

    rearrange();
    free_block *fb = free_block_head;

    while (fb->next != NULL) {
        int index = (fb->start_addr - DEFAULT_MEM_START) / fb->size;
        if (index % 2 == 0) {
            free_block *temp = fb->next;
            if (fb->start_addr + fb->size == temp->start_addr && fb->size == temp->size) {
                fb->size += temp->size;
                fb->next = temp->next;
                free(temp);
                fb = free_block_head;
                continue;
            }
        }
        fb = fb->next;
    }
    return 1;
}

int dispose(allocated_block *fab) { //释放结构体所占的内存
    allocated_block *pre, *ab;
    if (fab == allocated_block_head) {
        allocated_block_head = allocated_block_head->next;
        free(fab);
        return 1;
    }
    pre = allocated_block_head;
    ab = allocated_block_head->next;
    while (ab != fab) {
        pre = ab;
        ab = ab->next;
    }
    pre->next = ab->next;
    free(ab);
    return 2;
}

void display_mem_usage() {
    rearrange();
    free_block *fb = free_block_head;
    allocated_block *ab = allocated_block_head;
    puts("*********************Free Memory*********************");
    printf("%20s %20s\n", "start_addr", "size");
    int cnt = 0;
    while (fb != NULL) {
        cnt++;
        printf("%20d %20d\n", fb->start_addr, fb->size);
        fb = fb->next;
    }
    if (!cnt)
        puts("No Free Memory");
    else
        printf("Totaly %d free blocks\n", cnt);
    puts("");
    puts("*******************Used Memory*********************");
    printf("%10s %10s %20s\n", "PID", "start_addr", "size");
    cnt = 0;
    while (ab != NULL) {
        cnt++;
        printf("%10d %10d %20d\n", ab->pid, ab->start_addr, ab->size);
        ab = ab->next;
    }
    if (!cnt)
        puts("No allocated block");
    else
        printf("Totaly %d allocated blocks\n", cnt);
    return;
}

void kill_process() { //杀死某个进程
    allocated_block *ab;
    int pid;
    puts("Please input the pid of Killed process");
    scanf("%d", &pid);
    ab = find_process(pid);
    if (ab != NULL) {
        if (algo.first != 4) {
            free_mem(ab);
        } else {
            buddy_free_mem(ab);
        }

        dispose(ab);
    }
}
