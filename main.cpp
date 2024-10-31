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

    DL_list_push_back(&list, 1, &last_err);
    ListLogDump(&list);
    
    DL_list_insert(&list, 0, 1, &last_err);

    ListLogDump(&list)

    DL_list_insert(&list, 4, 52, &last_err);


    ListLogDump(&list)

    DL_list_dtor(&list);

    return 0;
}