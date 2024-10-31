#include <stdio.h>

#include "DL_list_err_proc.h"

void DL_list_add_err(enum DL_list_err_t *dest, enum DL_list_err_t add) {
    *dest = (DL_list_err_t)((unsigned long long)(*dest) | (unsigned long long) add);
}

void DL_list_err_get_descr(const enum DL_list_err_t err_code, char err_descr_str[]) {
    #define DESCR_(err_code, err)                 \
        {                                         \
            if (err_code & err) {                 \
                sprintf(err_descr_str, #err", "); \
            }                                     \
        }                                         \

    DESCR_(err_code, DL_ERR_ALLOC);
    DESCR_(err_code, DL_ERR_FILE_OPEN);
    DESCR_(err_code, DL_ERR_PUSH);
    DESCR_(err_code, DL_ERR_INSERT);
}
