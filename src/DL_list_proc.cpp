#include <cassert>
#include <cstddef>
#include <stdlib.h>

#include "DL_list_proc.h"
#include "DL_list_err_proc.h"
#include "general.h"

const size_t MAX_CYCLE_ITERATIONS = 1ul << 17;

bool DL_list_ctor(DL_list_t *list, const int size ON_DEBUG(, const char log_path[] = "")) {
    list->size = size;

    list->data = (DL_list_elem_t *) calloc(size + 1, sizeof(DL_list_elem_t));
    if (list->data == NULL) {
        DEBUG_DL_LIST_ERROR(DL_ERR_ALLOC, "")
        CLEAR_MEMORY(exit_mark);
    }

    list->data[0].next = 0;
    list->data[0].prev = 0;
    list->data[0].addr = 0;
    list->data[0].value = DL_LIST_POISON_VALUE;

    for (int i = 1; i < size; i++) {
        list->data[i].next = -1;
        list->data[i].prev = -1;
        list->data[i].addr  = i;
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

int DL_list_get_free_cell_addr(DL_list_t *list) { // FIXME: slow: O(N)
    for (int i = 0; i < list->size; i++) {
        if (list->data[i].next == -1 && list->data[i].prev == -1 && list->data[i].value == DL_LIST_POISON_VALUE) {
            return i;
        }
    }
    return -1;
}


DL_list_elem_value_t *DL_list_front(DL_list_t *list) {
    DL_list_elem_t *front_node = &list->data[list->data[0].next];
    if (front_node->value == DL_LIST_POISON_VALUE) {
        return NULL;
    }
    return &front_node->value;
}

DL_list_elem_value_t *DL_list_back(DL_list_t *list) {
    DL_list_elem_t *back_node = &list->data[list->data[0].prev];
    if (back_node->value == DL_LIST_POISON_VALUE) {
        return NULL;
    }
    return &back_node->value;
}

DL_list_elem_value_t *DL_list_next_elem(DL_list_t *list, const int addr) {
    if (addr >= list->size || addr < 0) {
        return NULL;
    }
    DL_list_elem_t *next_node = &list->data[list->data[addr].next];
    if (next_node->value == DL_LIST_POISON_VALUE) {
        return NULL;
    }
    return &next_node->value;
}

DL_list_elem_value_t *DL_list_prev_elem(DL_list_t *list, const int addr) {
    if (addr >= list->size || addr < 0) {
        return NULL;
    }
    DL_list_elem_t *prev_node = &list->data[list->data[addr].prev];
    if (prev_node->value == DL_LIST_POISON_VALUE) {
        return NULL;
    }
    return &prev_node->value;
}

int DL_list_push_back(DL_list_t *list, const DL_list_elem_value_t value) {
    assert(list != NULL);
    // FIXME: если буфер переполняется, то просто возвращаем ошибку, не делаем recalloc

    int new_node_addr = DL_list_get_free_cell_addr(list);
    if (new_node_addr == -1) {
        DEBUG_DL_LIST_ERROR(DL_ERR_PUSH, "DL_list_get_free_cell_addr: {-1}")
        return -1;
    }

    DL_list_elem_t *new_node = &list->data[new_node_addr];
    DL_list_elem_t *zero_node = &list->data[0];
    DL_list_elem_t *old_last_node = &list->data[zero_node->prev];

    new_node->value = value;
    new_node->prev = old_last_node->addr;
    new_node->next = zero_node->addr;
    new_node->addr = new_node_addr;


    old_last_node->next = new_node->addr;

    zero_node->prev = new_node->addr;

    return new_node_addr;
}

void DL_list_reset_node(DL_list_elem_t *node) {
    assert(node != NULL);

    node->next = -1;
    node->prev = -1;
    node->value = DL_LIST_POISON_VALUE;
}

int DL_list_push_front(DL_list_t *list, const DL_list_elem_value_t value) {
    assert(list != NULL);

    int new_node_addr = DL_list_get_free_cell_addr(list);
    if (new_node_addr == -1) {
        DEBUG_DL_LIST_ERROR(DL_ERR_PUSH, "DL_list_get_free_cell_addr: {-1}")
        return -1;
    }

    DL_list_elem_t *new_node = &list->data[new_node_addr];
    DL_list_elem_t *zero_node = &list->data[0];
    DL_list_elem_t *old_first_node = &list->data[zero_node->next];

    new_node->value = value;
    new_node->next = old_first_node->addr;
    new_node->prev = zero_node->addr;
    new_node->addr = new_node_addr;


    old_first_node->prev = new_node->addr;

    zero_node->next = new_node->addr;

    return new_node_addr;
}

int DL_list_insert_back(DL_list_t *list, const int addr, const DL_list_elem_value_t value) {
    assert(list != NULL);

    int new_node_addr = DL_list_get_free_cell_addr(list);
    if (new_node_addr == -1) {
        DEBUG_DL_LIST_ERROR(DL_ERR_PUSH, "DL_list_get_free_cell_addr: {-1}")
        return -1;
    }

    if (addr >= list->size || addr < 0) {
        DEBUG_DL_LIST_ERROR(DL_ERR_INSERT, "invalid addr: {%d}", addr);
        return -1;
    }

    DL_list_elem_t *new_node = &list->data[new_node_addr];
    new_node->value = value;
    new_node->addr = new_node_addr;

    DL_list_elem_t *mid_node = &list->data[addr];
    DL_list_elem_t *right_node = &list->data[mid_node->next];


    new_node->next = right_node->addr;
    new_node->prev = mid_node->addr;

    mid_node->next = new_node->addr;
    right_node->prev = new_node->addr;

    return new_node_addr;
}

int DL_list_insert_front(DL_list_t *list, const int addr, const DL_list_elem_value_t value) {
    assert(list != NULL);

    int new_node_addr = DL_list_get_free_cell_addr(list);
    if (new_node_addr == -1) {
        DEBUG_DL_LIST_ERROR(DL_ERR_PUSH, "DL_list_get_free_cell_addr: {-1}")
        return -1;
    }

    if (addr >= list->size || addr < 0) {
        DEBUG_DL_LIST_ERROR(DL_ERR_INSERT, "invalid addr: {%d}", addr);
        return -1;
    }

    DL_list_elem_t *new_node = &list->data[new_node_addr];
    new_node->value = value;
    new_node->addr = new_node_addr;

    DL_list_elem_t *mid_node = &list->data[addr];
    DL_list_elem_t *left_node = &list->data[mid_node->prev];

    new_node->next = mid_node->addr;
    new_node->prev = left_node->addr;

    mid_node->prev = new_node->addr;
    left_node->next = new_node->addr;

    return new_node_addr;
}

bool DL_list_pop(DL_list_t *list, const int addr) {
    assert(list != NULL);

    if (addr >= list->size || addr < 0) {
        DEBUG_DL_LIST_ERROR(DL_ERR_INSERT, "invalid addr: {%d}", addr);
        return false;
    }

    DL_list_elem_t *mid_node = &list->data[addr];
    DL_list_elem_t *left_node = &list->data[mid_node->prev];
    DL_list_elem_t *right_node = &list->data[mid_node->next];

    DL_list_reset_node(mid_node);

    left_node->next = right_node->addr;
    right_node->prev = left_node->addr;

    return true;
}

int DL_list_find(DL_list_t *list, const DL_list_elem_value_t value) {
    assert(list != NULL);

    DL_list_elem_t node = list->data[0];
    node = list->data[node.next];

    size_t iterations = 0;

    while (node.addr != 0) {
        if (iterations >= MAX_CYCLE_ITERATIONS) {
            DEBUG_DL_LIST_ERROR(DL_ERR_CYCLED, "")
            return -1;
        }
        if (node.value == value) {
            return node.addr;
        }
        if (node.next == -1) {
            DEBUG_DL_LIST_ERROR(DL_ERR_INVALID_NODE,
                "reachable node has invalid parameters: addr: {%d}, next: {%d} prev: {%d}", node.addr, node.next, node.prev)
        }
        node = list->data[node.next];
        iterations++;
    }
    return -1;
}

void DL_list_clear(DL_list_t *list) {
    assert(list != NULL);

    for (int i = 1; i < list->size; i++) {
        DL_list_reset_node(&list->data[i]);
    }
    list->data[0].next = 0;
    list->data[0].prev = 0;

}
