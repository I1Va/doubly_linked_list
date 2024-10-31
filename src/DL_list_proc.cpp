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
        DEBUG_DL_LIST_ERROR(DL_ERR_ALLOC, "")
        CLEAR_MEMORY(exit_mark);
    }

    for (int i = 0; i < size; i++) {
        list->data[i].next = -1;
        list->data[i].prev = -1;
        list->data[i].idx  = -1;
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

void DL_list_push_back(DL_list_t *list, const DL_list_elem_value_t value, DL_list_err_t *return_err) {
    assert(list != NULL);
    if (list->head == -1) {
        list->data[0].value = value;
        list->data[0].next = -1;
        list->data[0].prev = -1;
        list->data[0].idx = 0;
        list->head = 0;
        list->tail = 0;
    } else { //FIXME: Сделать
        assert(list->head >= 0);
        list->data[list->head].next = (list->head + 1) % list->size;
        if ((list->head + 1) % list->size == list->tail) {
            DL_list_add_err(return_err, DL_ERR_PUSH);
            DEBUG_DL_LIST_ERROR(DL_ERR_PUSH, "")
            return;
        }

        DL_list_elem_t new_node = list->data[(list->head + 1) % list->size];
        new_node.value = value;
        new_node.prev = list->head;
        new_node.idx = list->data[new_node.prev].idx + 1;
        new_node.next = -1;
        list->head = (list->head + 1) % list->size;
        list->data[list->head] = new_node;
    }
}

int DL_list_get_free_cell_addr(DL_list_t *list) {
    for (int i = 0; i < list->size; i++) {
        if (list->data[i].next == -1 && list->data[i].prev == -1) {
            return i;
        }
    }
    return -1;
}

int DL_list_get_addr_from_idx(DL_list_t *list, const int idx) {
    for (int i = 0; i < list->size; i++) {
        if (list->data[i].idx == idx) {
            return i;
        }
    }
    return -1;
}

void DL_list_update_idx(DL_list_t *list, const int insertred_idx) {
    for (int i = 0; i < list->size; i++) {
        if (list->data[i].idx >= insertred_idx) {
            list->data[i].idx++;
        }
    }
}

void DL_list_insert(DL_list_t *list, const int idx, const DL_list_elem_value_t value, DL_list_err_t *return_err) {
    if (idx == 0) {
        DL_list_elem_t new_node = {};
        new_node.next = -1;
        new_node.prev = -1;
        new_node.value = value;

        list->data[0] = new_node;
    } else {
        int cur_node_addr = DL_list_get_addr_from_idx(list, idx);
        if (cur_node_addr == -1) {
            DL_list_add_err(return_err, DL_ERR_INSERT);
            DEBUG_DL_LIST_ERROR(DL_ERR_INSERT, "cur_node_addr: %d", cur_node_addr)
            return;
        }

        DL_list_elem_t *cur_node = &list->data[cur_node_addr];

        DL_list_elem_t *prev_node = &list->data[cur_node->prev];
        int prev_node_addr = cur_node->prev;

        int new_node_addr = DL_list_get_free_cell_addr(list);
        if (new_node_addr == -1) {
            DL_list_add_err(return_err, DL_ERR_INSERT);
            DEBUG_DL_LIST_ERROR(DL_ERR_INSERT, "")
            return;
        }
        DL_list_update_idx(list, idx);

        prev_node->next = new_node_addr; // FIXME: O(N)

        cur_node->prev = prev_node->next;

        DL_list_elem_t new_node = {};
        new_node.prev = prev_node_addr;
        new_node.next = cur_node_addr;
        new_node.value = value;
        new_node.idx = idx;

        list->data[new_node_addr] = new_node;
    }
}