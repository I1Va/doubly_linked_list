#include <stdlib.h>

#include "DL_list_proc.h"
#include "DL_list_err_proc.h"
#include "general.h"

struct DL_list_t ctor(const size_t size, DL_list_err_t *last_err
    ON_DEBUG(, const char log_path[] = ""))
{
    DL_list_t list = {};

    list.size = size;
    list.head = -1;
    list.tail = -1;

    list.data = (DL_list_elem_t *) calloc(size, sizeof(DL_list_elem_t));
    if (list.data == NULL) {
        DL_list_add_err(last_err, DL_ERR_ALLOC);
        DEBUG_DL_LIST_ERROR(*last_err, "")
        CLEAR_MEMORY(exit_mark);
    }
    for (size_t i = 0; i < size; i++) {
        list.data[i].next = -1;
        list.data[i].prev = -1;
        list.data[i].value = DL_LIST_POISON_VALUE;
    }

    ON_DEBUG
    (
        list.log_file_ptr = fopen(log_path, "a");
        if (list.log_file_ptr == NULL) {
            DL_list_add_err(last_err, DL_ERR_FILE_OPEN);
            DEBUG_DL_LIST_ERROR(*last_err, "")
            CLEAR_MEMORY(exit_mark);
        }
        setbuf(list.log_file_ptr, NULL); // disable buffering
    )
    exit_mark:

    if (list.data != NULL) {
        FREE(list.data);
    }
    return list;
}
