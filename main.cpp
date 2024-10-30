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

    DL_list_t list = DL_list_ctor(10, &last_err, "./logs/log.html");
    DL_list_log_file_start(list.log_file_ptr);
    ListLogDump(&list)


    return 0;
}