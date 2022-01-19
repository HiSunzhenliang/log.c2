/**
 * MIT License
 *
 * Copyright (c) 2022 sunzhenliang
 * Copyright (c) 2020 rxi
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "log.h"

#define MAX_CALLBACKS 32
#define MAXLEN_FMTTIME 32

typedef struct {
    log_LogFn fn;
    void *udata;
    int level;
} Callback;

static void stdout_callback(log_Event *ev);

static struct {
    void *udata;
    log_LockFn lock;
    Callback callbacks[MAX_CALLBACKS];
} L = {.callbacks = {{stdout_callback, NULL, LOG_DISABLE}}};

typedef struct {
    char* filename;
    size_t n;
} backup;

static const char *level_strings[] = {
    "EMERGE", "ALERT", "CRITIC", "ERROR", "WARN", "NOTICE", "INFO", "DEBUG",
};

#ifdef LOG_USE_COLOR

#define NONE         "\033[m"
#define RED          "\033[0;32;31m"
#define LIGHT_RED    "\033[1;31m"
#define GREEN        "\033[0;32;32m"
#define LIGHT_GREEN  "\033[1;32m"
#define BLUE         "\033[0;32;34m"
#define LIGHT_BLUE   "\033[1;34m"
#define CYAN         "\033[0;36m"
#define LIGHT_CYAN   "\033[1;36m"
#define PURPLE       "\033[0;35m"
#define LIGHT_PURPLE "\033[1;35m"
#define BROWN        "\033[0;33m"
#define YELLOW       "\033[1;33m"
#define DARY_GRAY    "\033[1;30m"
#define LIGHT_GRAY   "\033[0;37m"
#define WHITE        "\033[1;37m"

static const char *level_colors[] = {
    LIGHT_RED, LIGHT_RED, LIGHT_RED, LIGHT_RED, YELLOW, CYAN, GREEN, BLUE,
};
#endif

static void stdout_callback(log_Event *ev) {
#ifdef LOG_USE_COLOR
    fprintf(ev->udata, "%s %s[%-6s]"NONE" "DARY_GRAY"[%s:%-3d]:"NONE" ",
            ev->fmttime, level_colors[ev->level], level_strings[ev->level], ev->file, ev->line);
#else
    fprintf(ev->udata, "%s [%-6s] [%s:%-3d]: ",
            ev->fmttime, level_strings[ev->level], ev->file, ev->line);
#endif
    vfprintf(ev->udata, ev->fmt, ev->ap);
    fprintf(ev->udata, "\n");
    fflush(ev->udata);
}

static void file_callback(log_Event *ev) {
    fprintf(ev->udata, "%s [%-6s] [%s:%-3d]: ",
            ev->fmttime, level_strings[ev->level], ev->file, ev->line);
    vfprintf(ev->udata, ev->fmt, ev->ap);
    fprintf(ev->udata, "\n");
    fflush(ev->udata);
}

static void lock(void) {
    if (L.lock) {
        L.lock(true, L.udata);
    }
}

static void unlock(void) {
    if (L.lock) {
        L.lock(false, L.udata);
    }
}

static void gen_fmttime(char fmt_time[]) {
    struct timeval _time;
    gettimeofday(&_time, NULL);
    time_t t = _time.tv_sec;
    struct tm *tm = localtime(&t);
    size_t len = strftime(fmt_time, MAXLEN_FMTTIME, "%Y-%m-%d_%H:%M:%S", tm);
    sprintf(&fmt_time[len], "_%03ld", _time.tv_usec / 1000);
}

static int clipping(int val, int max, int min) {
    int tmp = val > max ? max : val;
    return tmp < min ? min : tmp;
}

static FILE *try_open_log_file(FILE **file, char *path) {
    if (*file) {
        return *file;
    }
    *file = fopen(path, "a");
    if (!*file) {
        return NULL;
    }
    return *file;
}

static int backup_file(char *filename, char *bak_tail) {
    char new_path[FILENAME_MAX];
    snprintf(new_path, FILENAME_MAX, "%s_%s", filename, bak_tail);
    return rename(filename, new_path);
}

static void file_backup_callback(log_Event *ev) {
    backup *bp = ev->udata;

    static int log_line_num = 0;
    static FILE *fp;

    if (!try_open_log_file(&fp, bp->filename)) {
        return;
    }

    fprintf(fp, "%s [%-6s] [%s:%-3d]: ", ev->fmttime,
            log_level_string(ev->level), ev->file, ev->line);
    vfprintf(fp, ev->fmt, ev->ap);
    fprintf(fp, "\n");
    fflush(fp);
    log_line_num += 1;

    if (log_line_num >= bp->n) {
        fclose(fp);
        fp = NULL;
        int err = backup_file(bp->filename, ev->fmttime);
        if (!err) {
            log_line_num = 0;
        }
    }
}

const char *log_level_string(int level) { return level_strings[level]; }

void log_set_lock(log_LockFn fn, void *udata) {
    L.lock = fn;
    L.udata = udata;
}

void log_stdout_verbose(int level) {
    L.callbacks[0] = (Callback){stdout_callback, stdout, clipping(level, LOG_MAX, LOG_DISABLE)};
}

void log_stderr_verbose(int level) {
    L.callbacks[0] = (Callback){stdout_callback, stderr, clipping(level, LOG_MAX, LOG_DISABLE)};
}

int log_add_callback(log_LogFn fn, void *udata, int level) {
    for (int i = 0; i < MAX_CALLBACKS; i++) {
        if (!L.callbacks[i].fn) {
            L.callbacks[i] =
                (Callback){fn, udata, clipping(level, LOG_MAX, LOG_EMERGENCY)};
            return 0;
        }
    }
    return -1;
}

int log_add_fp(FILE *fp, int level) {
    return log_add_callback(file_callback, fp, clipping(level, LOG_MAX, LOG_DISABLE));
}

int log_add_file_backup(char *filename, size_t n, int level) {
    if (!filename || !n) {
        return -1;
    }
    backup bp = {.filename = filename, .n = n};
    return log_add_callback(file_backup_callback, &bp, clipping(level, LOG_MAX, LOG_DISABLE));
}

void log_log(int level, const char *file, int line, const char *fmt, ...) {
    char fmttime[MAXLEN_FMTTIME];
    gen_fmttime(fmttime);
    log_Event ev = {
        .fmttime = fmttime,
        .fmt = fmt,
        .file = file,
        .line = line,
        .level = level,
    };

    lock();

    for (int i = 0; i < MAX_CALLBACKS && L.callbacks[i].fn; i++) {
        Callback *cb = &L.callbacks[i];
        if (level <= cb->level) {
            ev.udata = cb->udata;
            va_start(ev.ap, fmt);
            cb->fn(&ev);
            va_end(ev.ap);
        }
    }

    unlock();
}
