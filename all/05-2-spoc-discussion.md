#lec10 进程／线程控制spoc练习

查看kern/process/proc.h中可以看到proc_struct的内容：

struct proc_struct {
    enum proc_state state;                      // Process state
    int pid;                                    // Process ID
    int runs;                                   // the running times of Proces
    uintptr_t kstack;                           // Process kernel stack
    volatile bool need_resched;                 // bool value: need to be rescheduled to release CPU?
    struct proc_struct *parent;                 // the parent process
    struct mm_struct *mm;                       // Process's memory management field
    struct context context;                     // Switch here to run process
    struct trapframe *tf;                       // Trap frame for current interrupt
    uintptr_t cr3;                              // CR3 register: the base addr of Page Directroy Table(PDT)
    uint32_t flags;                             // Process flag
    char name[PROC_NAME_LEN + 1];               // Process name
    list_entry_t list_link;                     // Process link list 
    list_entry_t hash_link;                     // Process hash list
};
包含了当前进程状态，进程ID，内核堆栈，内存管理的数据结构等等。 通过hash列表进行分组，每组里面按照双向列表进行组织。 proc.c下的函数基本上
都用到了它，比如proc_run,proc_init,find_proc等函数。 setup_kstack,copy_thread,do_fork需要修改它。 因为为了创建进程，切换进程，退出进程时
需要修改链表结构和链表内的内容 。
