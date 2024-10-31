#ifndef DL_LIST_ERR_PROC_H
#define DL_LIST_ERR_PROC_H

#include <string.h>

#include "DL_list_proc.h";

const size_t MAX_CYCLE_ITERATIONS = 1ul << 17;

enum DL_list_err_t {
    DL_ERR_OK                       = 0ull,
    DL_ERR_FILE_OPEN                = 1ull << 0,
    DL_ERR_ALLOC                    = 1ull << 1,
    DL_ERR_PUSH                     = 1ull << 2,
    DL_ERR_INSERT                   = 1ull << 3,
    DL_ERR_CYCLED                   = 1ull << 4,
    DL_ERR_INVALID_NODE             = 1ull << 5,
    DL_ERR_INVALID_NODE_CONNECTION  = 1ull << 6,


};

const size_t ERR_DESCR_MAX_SIZE = 128;

void DL_list_err_get_descr(enum DL_list_err_t err_code, char err_descr_str[]);

void DL_list_add_err(enum DL_list_err_t *dest, enum DL_list_err_t add);

DL_list_err_t DL_list_verify(const DL_list_t list);

#ifdef _DEBUG
    #define debug(str_, ...) fprintf_red(stderr, "{%s} [%s: %d]: descr{" str_ "}\n", __FILE_NAME__, __PRETTY_FUNCTION__, __LINE__, ##__VA_ARGS__);

    #define DEBUG_DL_LIST_ERROR(err_code, str_, ...) {                                                      \
        char DL_list_err_desr_str[ERR_DESCR_MAX_SIZE];                                                    \
        DL_list_err_get_descr(err_code, DL_list_err_desr_str); \
        printf("DESCR: '%s'\n", DL_list_err_desr_str);                                                      \
        fprintf_red(stderr, "{%s} [%s: %d]: err_descr: {%s}, message: {" str_ "}\n",              \
             __FILE_NAME__, __PRETTY_FUNCTION__, __LINE__, DL_list_err_desr_str, ##__VA_ARGS__);                    \
        fprintf(stderr, WHT); \
    }

    #define ON_DEBUG(...) __VA_ARGS__
#else
    #define debug(str_, ...) ;
    #define DEBUG_DL_LIST_ERROR(err_code, str_, ...) ;
    #define ON_DEBUG(...)
#endif // _DEBUG


#endif // DL_LIST_ERR_PROC_H