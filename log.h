/**
 * Copyright (c) 2022 sunzhenliang
 * Copyright (c) 2021 sunzhenliang zhaolei wanglihe
 * Copyright (c) 2020 rxi
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the MIT license. See `log.c` for details.
 */
#ifndef LOG_H
#define LOG_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>

#define LOG_VERSION "0.2.0"

typedef struct {
    va_list ap;
    const char *fmt;
    const char *file;
    char *fmttime;
    void *udata;
    int line;
    int level;
} log_Event;

typedef void (*log_LogFn)(log_Event *ev);
typedef void (*log_LockFn)(bool lock, void *udata);

enum {
    LOG_DISABLE = -1,
    LOG_EMERGENCY = 0,
    LOG_ALERT,
    LOG_CRITICAL,
    LOG_ERROR,
    LOG_WARN,
    LOG_NOTICE,
    LOG_INFO,
    LOG_DEBUG,
    LOG_MAX,
};

#define log_emergency(...) log_log(LOG_EMERGENCY, __FILE__, __LINE__, __VA_ARGS__)
#define log_alert(...)     log_log(LOG_ALERT, __FILE__, __LINE__, __VA_ARGS__)
#define log_critical(...)  log_log(LOG_CRITICAL, __FILE__, __LINE__, __VA_ARGS__)
#define log_error(...)     log_log(LOG_ERROR, __FILE__, __LINE__, __VA_ARGS__)
#define log_warn(...)      log_log(LOG_WARN, __FILE__, __LINE__, __VA_ARGS__)
#define log_notice(...)    log_log(LOG_NOTICE, __FILE__, __LINE__, __VA_ARGS__)
#define log_info(...)      log_log(LOG_INFO, __FILE__, __LINE__, __VA_ARGS__)
#define log_debug(...)     log_log(LOG_DEBUG, __FILE__, __LINE__, __VA_ARGS__)

const char *log_level_string(int level);
void log_set_lock(log_LockFn fn, void *udata);
void log_stdout_verbose(int level);
void log_stderr_verbose(int level);
int log_add_callback(log_LogFn fn, void *udata, int level);
int log_add_fp(FILE *fp, int level);
int log_add_file_backup(char *filename, size_t n, int level);

void log_log(int level, const char *file, int line, const char *fmt, ...);

#ifdef __cplusplus
}
#endif

#endif
