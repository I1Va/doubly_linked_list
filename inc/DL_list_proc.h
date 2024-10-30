#ifndef DL_PROC_H
#define DL_PROC_H

#include <string.h>
#include <stdio.h>

#include "general.h"
#include "DL_list_err_proc.h"

typedef int DL_list_elem_value_t;

struct DL_list_elem_t {
    int next;
    int prev;

    DL_list_elem_value_t value;
};

const char LOG_IMG_DIR_NAME[] = "imgs";
const char LOG_GRAPHVIZ_CODE_DIR_NAME[] = "graphviz_code_dir";
const size_t MAX_DIGITS_N = 32;
const size_t MAX_LOG_FILE_PATH_SZ = 128;
const size_t MAX_SYSTEM_COMMAND_SIZE = 128;

struct DL_list_t {



    int size;

    int head;
    int tail;
    DL_list_elem_t *data;
    ON_DEBUG(FILE *log_file_ptr;)
    ON_DEBUG(char log_file_path[MAX_LOG_FILE_PATH_SZ];)


};

const DL_list_elem_value_t DL_LIST_POISON_VALUE = 0xBADBAD;

bool DL_list_ctor(DL_list_t *list, const int size ON_DEBUG(, const char log_path[]));

#endif // DL_PROC_H