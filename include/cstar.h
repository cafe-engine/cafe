/******************************************************************************
 *
 *   This is free and unencumbered software released into the public domain.
 * Anyone is free to copy, modify, publish, use, compile, sell, or
 * distribute this software, either in source code form or as a compiled
 * binary, for any purpose, commercial or non-commercial, and by any
 * means.
 *
 * In jurisdictions that recognize copyright laws, the author or authors
 * of this software dedicate any and all copyright interest in the
 * software to the public domain. We make this dedication for the benefit
 * of the public at large and to the detriment of our heirs and
 * successors. We intend this dedication to be an overt act of
 * relinquishment in perpetuity of all present and future rights to this
 * software under copyright law.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * For more information, please refer to <http://unlicense.org/>
 *
 *
 *******************************************************************************/

#ifndef CSTAR_H
#define CSTAR_H

#define CST_VERSION "0.1.0"

#ifndef CST_API
    #if defined(_WIN32)
        #if defined(BUILD_SHARE)
	    #define CST_API __declspec(dllexport)
        #elif defined(USE_SHARED)
            #define CST_API __declspec(dllimport)
        #else
            #define CST_API
        #endif
    #else
        #define CST_API
    #endif
#endif

#define CST_STR(x) #x
#define cst_assert(s) if (!(s)) cst_fatal("Assertion '%s' failed", CST_STR((s)))

enum {
    CST_LOG = 1,
    CST_ERROR,
    CST_WARNING,
    CST_FATAL,

    CST_TRACE = 8
};


CST_API int cst_set_output(void *out);
CST_API int cst_log_(int mode, const char *file, int line, const char *function, const char *fmt, ...);

#define cst_log(msg...) cst_log_(CST_LOG, __FILE__, __LINE__, __PRETTY_FUNCTION__, msg)
#define cst_warn(msg...) cst_log_(CST_WARNING, __FILE__, __LINE__, __PRETTY_FUNCTION__, msg)
#define cst_error(msg...) cst_log_(CST_ERROR, __FILE__, __LINE__, __PRETTY_FUNCTION__, msg)
#define cst_fatal(msg...) cst_log_(CST_FATAL, __FILE__, __LINE__, __PRETTY_FUNCTION__, msg)
#define cst_trace(msg...) cst_log_(CST_TRACE, __FILE__, __LINE__, __PRETTY_FUNCTION__, msg)

#define cst_traceerror(msg...) cst_log_(CST_ERROR | CST_TRACE, __FILE__, __LINE__, __PRETTY_FUNCTION__, msg)
#define cst_tracewarn(msg...) cst_log_(CST_WARNING | CST_TRACE, __FILE__, __LINE__, __PRETTY_FUNCTION__, msg)
#define cst_tracefatal(msg...) cst_log_(CST_FATAL | CST_TRACE, __FILE__, __LINE__, __PRETTY_FUNCTION__, msg)

#endif /* CSTAR_H */

#if defined(CSTAR_IMPLEMENTATION)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdarg.h>

struct cst_t {
    FILE *out;
};

static struct cst_t ctx = {NULL};

int cst_set_output(void *out) {
    ctx.out = out;
    return 1;
}

int cst_log_(int mode, const char *file, int line, const char *function, const char *fmt, ...) {

    if (!ctx.out) ctx.out = stdout;

    time_t t = time(NULL);
    struct tm *tm_now;

    va_list args;

    int m = (mode & 0x7);

    char smode[15] = "";

    switch (m) {
        case CST_LOG: sprintf((char*)smode, "[log]   "); break;
        case CST_ERROR: sprintf((char*)smode, "[error] "); break;
        case CST_WARNING: sprintf((char*)smode, "[warn]  "); break;
        case CST_FATAL: sprintf((char*)smode, "[fatal] "); break;
        default: strcpy((char*)smode, "");
    }
    if (m <= 1 && mode & CST_TRACE) strcpy((char*)smode, "[trace] ");

    tm_now = localtime(&t);
    char tm_buf[10];
    strftime((char*)tm_buf, sizeof(tm_buf), "%H:%M:%S", tm_now);
    fprintf(ctx.out, "%s %s%s:%d: ", tm_buf, smode, file, line); 
    if (mode & CST_TRACE) fprintf(ctx.out, "in function %s(): ", function);

    va_start(args, fmt);
    vfprintf(ctx.out, fmt, args);

    va_end(args);
    fprintf(ctx.out, "\n");
    if (mode == CST_FATAL) exit(-1);
    return 1;
}

#if defined(CSTAR_CLI)
int main(int argc, char ** argv) {

    printf("CStar CLI >\n");
    int running = 1;
    cst_log("testandow, serase foi?");
    cst_error("opa opa, tudo bom?");
    cst_trace("kek");
    cst_assert(running != 1);
    while (running) {
        printf("> ");
        char c;
        scanf("%c", &c);
        if (c == 'e') break;
    }
    return 0;
}
#endif /* CSTAR_CLI */

#endif /* CSTAR_IMPLEMENTATION */
