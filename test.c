#include "list.h"
#include "defs.h"
#include <stdio.h>

struct page {
    int test;
    list_entry_t page_link;
};

#define le2page(le, member)  to_struct((le), struct page, member)

#define to_struct(ptr, type, member)                               \
((type *)((char *)(ptr) - offsetof(type, member)))

#define offsetof(type, member)                                      \
        ((size_t)(&((type *)0)->member))


typedef struct {
    list_entry_t free_list;
    unsigned int nr_free;
}free_area_t;

int main(){
             
    free_area_t free_area;
    struct page pg;
    free_area.free_list.next = &pg.page_link;
        
    pg.test = 1;
    pg.page_link.next = &free_area.free_list;
    list_entry_t* le = &free_area.free_list;
    while ( (le = list_next(le)) != &free_area.free_list ) {
        struct page* p = le2page(le, page_link);
        printf ( "%d\n", p->test );
    }
    return 0;
}
