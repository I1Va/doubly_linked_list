#include <stdio.h>

#include "DL_list_err_proc.h"
#include "DL_list_proc.h"

void DL_list_add_err(enum DL_list_err_t *dest, enum DL_list_err_t add) {
    *dest = (DL_list_err_t)((unsigned long long)(*dest) | (unsigned long long) add);
}

void DL_list_err_get_descr(const enum DL_list_err_t err_code, char err_descr_str[]) {
    bool error = false;
    #define DESCR_(err_code, err)                 \
        {                                         \
            if (err_code & err) {                 \
                sprintf(err_descr_str, #err", "); \
                error = true;                     \
            }                                     \
        }                                         \

    DESCR_(err_code, DL_ERR_ALLOC);
    DESCR_(err_code, DL_ERR_FILE_OPEN);
    DESCR_(err_code, DL_ERR_PUSH);
    DESCR_(err_code, DL_ERR_INSERT);
    DESCR_(err_code, DL_ERR_CYCLED);
    DESCR_(err_code, DL_ERR_INVALID_NODE);
    DESCR_(err_code, DL_ERR_INVALID_NODE_CONNECTION);
    DESCR_(err_code, DL_ERR_STACK);
    DESCR_(err_code, DL_ERR_SYSTEM);

    if (!error) {
        sprintf(err_descr_str, "ALL IS OK:)");
    }
    #undef DESCR_
}

DL_list_err_t DL_list_verify(const DL_list_t list) {
    DL_list_err_t errors = DL_ERR_OK;

    for (int i = 0; i < list.size; i++) {
        DL_list_elem_t node = list.data[i];
        if (node.addr >= list.size || node.addr < 0 || i != node.addr) {
            DL_list_add_err(&errors, DL_ERR_INVALID_NODE);
            DEBUG_DL_LIST_ERROR(DL_ERR_INVALID_NODE, "node[%d] has invalid addr: {%d}", i, node.addr)
        }
        if (node.next == -1 && node.prev == -1 && node.value == DL_LIST_POISON_VALUE) {
            continue;
        }
        if (list.data[node.prev].next != node.addr) {
            DL_list_add_err(&errors, DL_ERR_INVALID_NODE_CONNECTION);
            DEBUG_DL_LIST_ERROR(DL_ERR_INVALID_NODE, "nodes '%d' and '%d' connection invalid", node.prev, node.addr);
        }
        if (list.data[node.next].prev != node.addr) {
            DL_list_add_err(&errors, DL_ERR_INVALID_NODE_CONNECTION);
            DEBUG_DL_LIST_ERROR(DL_ERR_INVALID_NODE, "nodes '%d' and '%d' connection invalid", node.next, node.addr);
        }
    }

    DL_list_elem_t node = list.data[0];
    node = list.data[node.next];

    size_t iterations = 0;

    while (node.addr != 0) {
        if (iterations >= MAX_CYCLE_ITERATIONS) {
            DL_list_add_err(&errors, DL_ERR_CYCLED);
            DEBUG_DL_LIST_ERROR(DL_ERR_CYCLED, "")
            break;
        }
        if (node.next == -1) {
            DL_list_add_err(&errors, DL_ERR_INVALID_NODE);
            DEBUG_DL_LIST_ERROR(DL_ERR_INVALID_NODE,
                "reachable node has invalid parameters: addr: {%d}, next: {%d} prev: {%d}", node.addr, node.next, node.prev)
            break;
        }
        node = list.data[node.next];
        iterations++;
    }

    return errors;
}
