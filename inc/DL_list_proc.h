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

struct DL_list_t {
    ON_DEBUG(FILE *log_file_ptr;)

    int size;

    int head;
    int tail;
    DL_list_elem_t *data;


};

const DL_list_elem_value_t DL_LIST_POISON_VALUE = 0xBADBAD;

struct DL_list_t DL_list_ctor(const int size, DL_list_err_t *last_err
    ON_DEBUG(, const char log_path[]));

#endif // DL_PROC_H