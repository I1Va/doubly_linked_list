#ifndef DL_LIST_ERR_PROC_H
#define DL_LIST_ERR_PROC_H

#include <string.h>

enum DL_list_err_t {
    DL_ERR_OK         = 0ull,
    DL_ERR_FILE_OPEN  = 1ull << 0,
    DL_ERR_ALLOC      = 1ull << 1,
};

const size_t ERR_DESCR_MAX_SIZE = 128;

void DL_list_err_get_descr(enum DL_list_err_t err_code, char err_descr_str[]);

void DL_list_add_err(enum DL_list_err_t *dest, enum DL_list_err_t add);

#ifdef _DEBUG
    #define debug(str_, ...) fprintf_red(stderr, "{%s} [%s: %d]: descr{" str_ "}\n", __FILE_NAME__, __PRETTY_FUNCTION__, __LINE__, ##__VA_ARGS__);

    #define DEBUG_DL_LIST_ERROR(err_code, str_, ...) {                                                      \
        char DL_list_err_desr_str[ERR_DESCR_MAX_SIZE];                                                    \
        DL_list_err_get_descr(err_code, DL_list_err_desr_str);                                                      \
        fprintf_red(stderr, "{%s} [%s: %d]: err_descr: {%s}, message: {" str_ "}\n",              \
             __FILE_NAME__, __PRETTY_FUNCTION__, __LINE__, DL_list_err_desr_str, ##__VA_ARGS__);                    \
    }

    #define ON_DEBUG(...) __VA_ARGS__
#else
    #define debug(str_, ...) ;
    #define DEBUG_DL_LIST_ERROR(err_code, str_, ...) ;
    #define ON_DEBUG(...)
#endif // _DEBUG


#endif // DL_LIST_ERR_PROC_H