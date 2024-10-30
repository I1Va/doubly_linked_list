#include <cassert>
#include <stdlib.h>

#include "DL_list_proc.h"
#include "DL_list_err_proc.h"
#include "general.h"

bool DL_list_ctor(DL_list_t *list, const int size ON_DEBUG(, const char log_path[] = "")) {
    list->size = size;
    list->head = -1;
    list->tail = -1;

    list->data = (DL_list_elem_t *) calloc(size + 1, sizeof(DL_list_elem_t));
    if (list->data == NULL) {
        DEBUG_DL_LIST_ERROR(DL_ERR_FILE_OPEN, "")
        CLEAR_MEMORY(exit_mark);
    }
    for (int i = 0; i < size; i++) {
        list->data[i].next = -1;
        list->data[i].prev = -1;
        list->data[i].value = DL_LIST_POISON_VALUE;
    }

    ON_DEBUG
    (
        strcpy(list->log_file_path, log_path);
        list->log_file_ptr = fopen(log_path, "a");
        if (list->log_file_ptr == NULL) {
            DEBUG_DL_LIST_ERROR(DL_ERR_FILE_OPEN, "")
            CLEAR_MEMORY(exit_mark);
        }
        setbuf(list->log_file_ptr, NULL); // disable buffering
    )

    return true;

    exit_mark:

    if (list->data != NULL) {
        FREE(list->data);
    }
    return false;
}


void DL_list_dtor(DL_list_t *list) {
    assert(list != NULL);

    if (list->data != 0) {
        FREE(list->data);
    }
}

void DL_list_push_back(DL_list_t *list, const DL_list_elem_value_t value) {
    assert(list != NULL);

    if (list->head == -1) {
        list->data[1].value = value;
        list->data[1].prev = 0;
        list->head = 1;
        list->tail = 1;
    } else { //FIXME: Сделать
        assert(list->head > 0);
        DL_list_elem_t new_node = list->data[(list->head + 1) % list->size];
        new_node.value = value;
        new_node.prev = list->head;

        list->head = (list->head + 1) % list->size;
    }
}