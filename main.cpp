#include "DL_list_err_proc.h"
#include "DL_list_logger.h"
#include "DL_list_proc.h"
#include "general.h"
#include <cstdio>
#include <cstdlib>

const char logs_dir[] = "./logs";

int main() {
    create_logs_dir(logs_dir);

    DL_list_err_t last_err = DL_ERR_OK;

    DL_list_t list = {};
    DL_list_ctor(&list, 10, "./logs/log.html");
    DL_list_log_file_start(list.log_file_ptr);

    DL_list_push_back(&list, 1);
    DL_list_push_back(&list, 1);
    DL_list_push_back(&list, 2);
    DL_list_push_back(&list, 3);
    DL_list_push_back(&list, 5);
    printf("prev[addr=4]: %d\n", *DL_list_prev_elem(&list, 4));
    printf("front: %d\n", *DL_list_front(&list));
    printf("back: %d\n", *DL_list_back(&list));
    DL_list_push_front(&list, 228);
    DL_list_push_front(&list, 52);
    DL_list_insert_back(&list, 4, 2006);
    DL_list_pop(&list, 3);

    printf("addr of '421':{%d}", DL_list_find(&list, 228));
    DL_list_clear(&list);
    // DL_list_insert_front(&list, 8, 7500);
    // DL_list_push_back(&list, 8, &last_err);
    // DL_list_push_back(&list, 13, &last_err);
    // DL_list_push_back(&list, 21, &last_err);
    // DL_list_push_back(&list, 34, &last_err);
    // DL_list_push_back(&list, 52, &last_err);

    ListLogDump(&list);

    // DL_list_insert(&list, 4, 52, &last_err);


    DL_list_dtor(&list);

    return 0;
}