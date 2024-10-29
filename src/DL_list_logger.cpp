#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <time.h>

#include <stdarg.h>
#include "DL_list_logger.h"
#include "general.h"


ON_DEBUG
(

void DL_list_fprintf_border(FILE* stream, const char bord_char, const size_t bord_sz, bool new_line) {
    for (size_t i = 0; i < bord_sz; i++) {
        fprintf(stream, WHT);
        fputc(bord_char, stream);
    }
    if (new_line) {
        fputc('\n', stream);
    }
}

void DL_list_fprintf_title(FILE *stream, const char tittle[], const char bord_char, const size_t bord_sz) {
    assert(tittle != NULL);
    size_t tittle_sz = strlen(tittle);
    if (bord_sz < tittle_sz) {
        return;
    }
    size_t len = bord_sz - tittle_sz;
    DL_list_fprintf_border(stream, bord_char, len / 2, false);
    fprintf_red(stream, "%s", tittle);
    DL_list_fprintf_border(stream, bord_char, (len + 1) / 2, true);
}

const char *DL_list_get_log_descr(enum DL_list_log_type_t log_type) {
    // #define DL_DESCR_(log_type) case log_type: return #log_type;

    switch (log_type) {
        case DL_LOG_ANALYS: return "DL_LOG_ANALYS";
        case DL_LOG_DEBUG: return "DL_LOG_DEBUG";
        case DL_LOG_ERROR: return "DL_LOG_ERROR";
        default: return "STRANGE LOG_TYPE";
    }
    // #undef DL_DESCR_
}

void DL_list_log_print_time(FILE *log_output_file_ptr) {
    time_t cur_time;
    time(&cur_time);
    struct tm *now = localtime(&cur_time);

    char date_str[date_nmemb] = {};
    char time_str[time_nmemb] = {};
    strftime(date_str, date_nmemb, "%m/%d/%Y", now);
    strftime(time_str, time_nmemb, "%T", now);

    double milliseconds = ((double) clock()) / CLOCKS_PER_SEC;
    fprintf(log_output_file_ptr, "Date: %s Time: %s Ms: {%f}\n", date_str, time_str, milliseconds);
}

void DL_list_print_log_func_info(FILE *log_output_file_ptr, const char file_name[], const char func_name[], const int line_idx) {
    fprintf(log_output_file_ptr, "file : {%s}; func: {%s}; line: {%d}\n", file_name, func_name, line_idx);
}

void DL_list_print_log_type(FILE *log_output_file_ptr, enum DL_list_log_type_t log_type) {
    fprintf(log_output_file_ptr, "log_type: {%s}\n", DL_list_get_log_descr(log_type));
}

void DL_list_log_var_print(FILE *log_output_file_ptr, enum DL_list_log_type_t log_type, const char file_name[], const char func_name[], const int line_idx, const char fmt[], ...) {
    DL_list_fprintf_title(log_output_file_ptr, "LOG_VAR", '-', BORDER_SZ);
    DL_list_print_log_type(log_output_file_ptr, log_type);
    DL_list_log_print_time(log_output_file_ptr);
    DL_list_print_log_func_info(log_output_file_ptr, file_name, func_name, line_idx);

    if (line_idx) {}

    va_list args;
    va_start(args, fmt);
    vfprintf(log_output_file_ptr, fmt, args);

    va_end(args);
    fprintf(log_output_file_ptr, "\n");

    DL_list_fprintf_border(log_output_file_ptr, '-', BORDER_SZ, true);
}


void DL_list_log_dump(DL_list_t *list, const char file_name[], const char func_name[], const int line_idx) {
    if (list == NULL) {
        return;
    }

    if (list->log_file_ptr == NULL) {
        return;
    }

    DL_list_fprintf_title(list->log_file_ptr, "DL_LIST DUMP", '-', BORDER_SZ);
    DL_list_print_log_type(list->log_file_ptr, DL_LOG_DEBUG);
    DL_list_log_print_time(list->log_file_ptr);
    DL_list_print_log_func_info(list->log_file_ptr, file_name, func_name, line_idx);

    fprintf_red(list->log_file_ptr, "list [%p] at %s:%d\n", list, file_name, line_idx);

    DL_list_fprintf_border(list->log_file_ptr, '-', BORDER_SZ, true);
}

)
