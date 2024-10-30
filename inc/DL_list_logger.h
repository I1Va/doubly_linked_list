#ifndef DL_LIST_LOGGER_H
#define DL_LIST_LOGGER_H


enum DL_list_log_type_t {
    DL_LOG_ANALYS = 0,
    DL_LOG_DEBUG = 1,
    DL_LOG_ERROR = 2,
};
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include <stdarg.h>

#include "general.h"
#include "DL_list_proc.h"

const size_t BORDER_SZ = 100;
const size_t date_nmemb = 16;
const size_t time_nmemb = 16;
const size_t LOG_WIDTH_VAL = 75;

void create_logs_dir(const char log_dir[]);

void DL_list_log_file_start(FILE *stream);

void DL_list_fprintf_border(FILE* stream, const char bord_char, const size_t bord_sz, bool new_line);

void DL_list_fprintf_title(FILE *stream, const char tittle[], const char bord_char, const size_t bord_sz);

const char *DL_list_get_log_descr(enum DL_list_log_type_t log_type);

void DL_list_log_print_time(FILE *log_output_file_ptr);

void DL_list_print_log_func_info(FILE *log_output_file_ptr, const char file_name[], const char func_name[], const int line_idx);

void DL_list_print_log_type(FILE *log_output_file_ptr, enum DL_list_log_type_t log_type);

void DL_list_log_var_print(FILE *log_output_file_ptr, enum DL_list_log_type_t log_type, const char file_name[], const char func_name[], const int line_idx, const char fmt[], ...);

void DL_list_log_dump(DL_list_t *list, const char file_name[], const char func_name[], const int line_idx);

#define ListLogVar(log_output_file_ptr, log_type, fmt, ...) log_var_print(log_output_file_ptr, log_type, __FILE_NAME__, __PRETTY_FUNCTION__, __LINE__, fmt, ##__VA_ARGS__);

#define ListLogDump(list) DL_list_log_dump(list, __FILE_NAME__, __PRETTY_FUNCTION__, __LINE__);

#endif // DL_LIST_LOGGER_H