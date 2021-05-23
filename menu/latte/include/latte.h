#define LATTE_IMPLEMENTATION
#ifndef LATTE_H
#define LATTE_H

#define LATTE_VERSION "0.1.0"

#ifndef LA_API
    #if defined(_WIN32)
        #if defined(BUILD_SHARE)
	    #define LA_API __declspec(dllexport)
        #elif defined(USE_SHARED)
            #define LA_API __declspec(dllimport)
        #else
            #define LA_API
        #endif
    #else
        #define LA_API
    #endif
#endif

#define LA_VERSION LATTE_VERSION

#define LA_STR(x) #x
#define la_assert(s) if (!(s)) la_fatal("Assertion '%s' failed", LA_STR((s)))

enum {
    LA_TREG = 0,
    LA_TLINK,
    LA_TSYM,
    LA_TCHR,
    LA_TBLK,
    LA_TDIR,
    LA_TFIFO
};

enum {
    LA_READ_MODE    = (1 << 0),
    LA_WRITE_MODE   = (1 << 1),
    LA_REWRITE_MODE = (1 << 2)
};

enum {
    LA_BLOCK_SIZE = 0,
    LA_NAME_SIZE,

};

enum {
    LA_FILE_STREAM = 0,
    LA_FILE_STATIC
};


enum {
    LA_LOG = 1,
    LA_ERROR,
    LA_WARNING,
    LA_FATAL,

    LA_TRACE = 8
};

typedef struct Latte Latte;

typedef struct la_vfs_s la_vfs_t;
typedef struct la_node_s la_node_t;

typedef struct la_posix_header_s la_posix_header_t;

typedef struct la_header_s la_header_t;
typedef struct la_file_s la_file_t;
typedef struct la_dir_s la_dir_t;

struct la_header_s {
    char name[100];
    char linkname[100];
    int type;
    int mode;
    long size;
    unsigned mtime;
    unsigned int uid, gid;
    char uname[32], gname[32];
};

/**********************
 *        Core        *
 **********************/
LA_API int la_resolve_path(const char *path, char *out);

LA_API void la_get_basedir(char *out);
LA_API void la_set_basedir(const char *basedir);

LA_API int la_init(const char *basedir);
LA_API void la_deinit();

LA_API int la_header(const char *path, la_header_t *out);

LA_API int la_touch(const char *filename);
LA_API int la_rm(const char *filename);

LA_API int la_mkdir(const char *path);
LA_API int la_rmdir(const char *path);

LA_API int la_isfile(const char *filename);
LA_API int la_isdir(const char *path);

/**********************
 *        File        *
 **********************/

LA_API la_file_t* la_fopen(const char *filename, int mode);
LA_API int la_freopen(la_file_t *fp, const char *filename, int mode);
LA_API int la_fclose(la_file_t *fp);
LA_API int la_fclose_stream(la_file_t *fp);

LA_API int la_fheader(la_file_t *fp, la_header_t *out);
LA_API int la_fseek(la_file_t *fp, long offset);
LA_API long la_ftell(la_file_t *fp);

LA_API int la_fread(la_file_t *fp, char *out, int bytes);
LA_API int la_fwrite(la_file_t *fp, const char *buffer, int bytes);

LA_API int la_fappend(la_file_t *fp, const char *buffer, int bytes);
LA_API int la_finsert(la_file_t *fp, const char *buffer, int bytes);

/**********************
 *        Dir         *
 **********************/

LA_API la_dir_t* la_dopen(const char *path);
LA_API int la_dclose(la_dir_t *dp);

LA_API la_file_t* la_dfopen(la_dir_t *dir, const char *filename, int mode);
LA_API la_dir_t* la_ddopen(la_dir_t *dir, const char *path);

LA_API int la_drewind(la_dir_t *dp);
LA_API int la_dread(la_dir_t *dp, la_header_t *out);

/***********************
 *       Virtual       *
 ***********************/

LA_API la_vfs_t* la_vopen(const char *filename, int mode);
LA_API int la_vclose(la_vfs_t *vfs);

LA_API la_file_t* la_vfopen(la_vfs_t *drv, const char *filename);
LA_API la_dir_t* la_vdopen(la_vfs_t *drv, const char *path);

LA_API int la_isvfile(la_vfs_t *drv, const char *filename);
LA_API int la_vreplace(la_vfs_t *drv, const char *f1, const char *f2);

LA_API la_node_t* la_node_create(int type);
LA_API int la_node_destroy(la_node_t *node);

LA_API la_file_t* la_node_file(la_node_t *node);
LA_API la_dir_t* la_node_dir(la_node_t *node);

LA_API int la_node_exists(la_node_t *node, const char *name);
LA_API int la_node_search(la_node_t *node, const char *name);
LA_API int la_node_push_child(la_node_t *node, la_node_t *child);


/***********************
 *        Utils        *
 ***********************/
typedef void la_lib_t;

typedef struct la_tar_s la_tar_t;

LA_API la_lib_t* la_dlopen(const char *libpath);
LA_API void la_dlclose(la_lib_t *dl);

LA_API int la_dlcall(la_lib_t *dl, const char *fn);

/***********************
 *        Debug        *
 ***********************/

LA_API int la_log_output(void *out);
LA_API int la_log_(int mode, const char *file, int line, const char *function, const char *fmt, ...);

#define la_log(msg...) la_log_(LA_LOG, __FILE__, __LINE__, __PRETTY_FUNCTION__, msg)
#define la_warn(msg...) la_log_(LA_WARNING, __FILE__, __LINE__, __PRETTY_FUNCTION__, msg)
#define la_error(msg...) la_log_(LA_ERROR, __FILE__, __LINE__, __PRETTY_FUNCTION__, msg)
#define la_fatal(msg...) la_log_(LA_FATAL, __FILE__, __LINE__, __PRETTY_FUNCTION__, msg)
#define la_trace(msg...) la_log_(LA_TRACE, __FILE__, __LINE__, __PRETTY_FUNCTION__, msg)

#define la_traceerror(msg...) la_log_(LA_ERROR | LA_TRACE, __FILE__, __LINE__, __PRETTY_FUNCTION__, msg)
#define la_tracewarn(msg...) la_log_(LA_WARNING | LA_TRACE, __FILE__, __LINE__, __PRETTY_FUNCTION__, msg)
#define la_tracefatal(msg...) la_log_(LA_FATAL | LA_TRACE, __FILE__, __LINE__, __PRETTY_FUNCTION__, msg)
#endif /* LATTE _H */


#if defined(LATTE_IMPLEMENTATION)

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>

#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>

#ifdef _WIN32
#include <windows.h>
#endif

#define latte() (&_ctx)

/* POSIX header */
struct la_posix_header_s {
  struct {
    char name[100];       /* 0 */
    char mode[8];         /* 100 */
    char uid[8];          /* 108 */
    char gid[8];          /* 116 */
    char size[12];        /* 124 */
    char mtime[12];       /* 136 */
    char chksum[8];       /* 148 */
    char typeflag;        /* 156 */
    char linkname[100];   /* 157 */
    char magic[6];        /* 257 */
    char version[2];      /* 263 */
    char uname[32];       /* 265 */
    char gname[32];       /* 297 */
    char devmajor[8];     /* 329 */
    char devminor[8];     /* 337 */
    char prefix[155];     /* 345 */
    char _[12];           /* 500 */
  };                      /* 512 */
};

typedef char la_block_t[512];

struct la_file_s {
    char usage;
    la_header_t h;
    int pos, offset;
    int mode;
    union {
        FILE *stream;
        void *data;
    };
};

struct la_dir_s {
    la_header_t h;
    DIR *stream;
};

struct la_node_s {
    char name[100];
    int type;

    la_file_t fp;

    la_node_t *child;
    la_node_t *next;
};

struct la_vfs_s {
    la_file_t stream;
    la_node_t *root; 
};

struct Latte {
    char basedir[64];
    int basedir_len;
    FILE *out;
};

static Latte _ctx;

/**********************
 *        Core        *
 **********************/

int la_resolve_path(const char *path, char *out) {
    if (!out) return 0;
    if (path && path[0] == '/') {
        strcpy(out, path);
        return 1;
    }

    char base[64];
    la_get_basedir(base);
    int len = strlen(base);

    strcpy(out, base);
    if (base[len] != '/' && base[len] != '\\') strcat(out, "/");
    if (path) strcat(out, path);
    return 1;
}

void la_set_basedir(const char *basedir) {
    if (!basedir) return;
    strcpy(latte()->basedir, basedir);
}

void la_get_basedir(char *out) {
    if (!out) return;
    strcpy(out, latte()->basedir);
}

int la_init(const char *basedir) {
   la_set_basedir(basedir);
   return 1;
}

void la_deinit() {

}

int la_header(const char *path, la_header_t *out) {
    if (!path) return 0;
    if (!out) return 0;
    char file[100];

    la_resolve_path(path, file);

    struct stat s;
    if (stat(file, &s) != 0) return 0;

    out->gid = s.st_gid;
    out->uid = s.st_uid;
    strcpy(out->name, path);
    out->mode = s.st_mode & 0x8f;

    out->type = S_ISDIR(s.st_mode) ? LA_TDIR : LA_TREG;
    out->size = s.st_size;

    return 1;
}

int la_touch(const char *filename) {
    if (!filename) {
        fprintf(stderr, "filename cannot be NULL\n");
        return 0;
    }
    char file[100];
    la_resolve_path(filename, file);

    if (la_isfile(filename)) return 1;
    FILE *fp;
    fp = fopen(file, "wb");
    if (!fp) {
        fprintf(stderr, "failed to create %s\n", filename);
        return 0;
    }
    fclose(fp);

    return 1;
}

int la_rm(const char *filename) {
    if (!filename) return 0;

    return remove(filename) != -1;
}

int la_mkdir(const char *path) {
    if (!path) return 0;
    char dir[100];
    la_resolve_path(path, dir);
    int err;
#ifdef _WIN32
    err = _mkdir(dir);
#else
    err = mkdir(dir, 0733);
#endif
    return err == 0;
}

int la_rmdir(const char *path) {
    if (!path) return 0;
    if (!la_header(path, NULL)) {
        la_error("cannot remove '%s'", path);
        return 0;
    }
    la_dir_t *dp = la_dopen(path);
    la_header_t h;

    char dir[100];
    while (la_dread(dp, &h)) {
        /* if (h.type == LA_TREG) la_rm(h. */
        if (h.name[0] != '.') {
            sprintf(dir, "%s/%s", path, h.name);
            if (h.type == LA_TREG) la_rm(dir);
            else if (h.type == LA_TDIR) la_rmdir(dir);
        } 
    }

    la_dclose(dp);

    la_resolve_path(path, dir);
    int err = 0;

#ifdef _WIN32
    err = _rmdir(dir);
#else
    err = rmdir(dir);
#endif
    return err == 0;
}

int la_isfile(const char *filename) {
    la_header_t h;
    la_header(filename, &h);

    return h.type == LA_TREG;
}

int la_isdir(const char *path) {
    la_header_t h;
    la_header(path, &h);

    return h.type == LA_TDIR;
}

/**********************
 *        File        *
 **********************/

static int _get_mode(char *out, int mode) {
    if (!out) return 0;
    memset(out, 0, 3);

    if (mode & LA_REWRITE_MODE) out[0] = 'w';
    else if (mode & LA_READ_MODE) out[0] = 'r';
    else if (mode & LA_WRITE_MODE) out[0] = 'w';
    else la_fatal("invalid file open mode: %d\n", mode);

    out[1] = 'b';
    
    if (mode & LA_REWRITE_MODE || mode & LA_WRITE_MODE)
        if (mode & LA_READ_MODE) out[2] = '+';

    return 1;
}

static int _header(const char *filepath, la_header_t *out) {
    la_assert(filepath != NULL);
    if (!out) return 0;

    struct stat s;
    const char *file = filepath;

    if (stat(file, &s) != 0) {
        FILE *out = latte()->out;
        la_log_output(stderr);
        la_error("cannot open file %s\n", filepath);
        la_log_output(out);
        return 0;
    }

    out->gid = s.st_gid;
    out->uid = s.st_uid;
    strcpy(out->name, file);
    out->mode = s.st_mode & 0x8f;

    out->type = S_ISDIR(s.st_mode) ? LA_TDIR : LA_TREG;
    out->size = s.st_size;

    return 1;
}

static int _file_init(la_file_t *fp, const char *filename, int mode) {
    la_assert(fp != NULL);

    char m[3];
    _get_mode(m, mode);
    if (!fp->stream) fp->stream = fopen(filename, m);
    else fp->stream = freopen(filename, m, fp->stream);

    if (fp->stream == NULL) la_fatal("cannot open file %s", filename);

    _header(filename, &fp->h);

    fp->mode = mode;
    fp->pos = 0;
    fp->offset = 0;

    return 1;
}

la_file_t* la_fopen(const char *filename, int mode) {
   /*la_file_t *fp = (la_file_t*)malloc(sizeof(*fp)); 
   memset(fp, 0, sizeof(*fp));
    if (!fp) la_fatal("failed to malloc\n");
    if (filename == NULL) return NULL;

    char file[100];
    la_resolve_path(filename, file);
    if (mode & LA_WRITE_MODE) la_touch(file);
    return fp;*/
    la_file_t *fp = (la_file_t*)malloc(sizeof(*fp));
    if (!fp) la_fatal("failed to malloc\n");
    la_assert(filename != NULL);

    memset(fp, 0, sizeof(*fp));

    char file[100];
    la_resolve_path(filename, file);
    if (mode & LA_WRITE_MODE) la_touch(file);

    _file_init(fp, file, mode);

    strcpy(fp->h.name, filename);

    return fp;
}

int la_freopen(la_file_t *fp, const char *filename, int mode) {
    la_assert(fp != NULL);

    char file[100];
    la_resolve_path(filename, file);
    
    _file_init(fp, file, mode);
    strcpy(fp->h.name, filename);

    return 1;
}

int la_fclose(la_file_t *fp) {
    if (!fp) return 0;

    la_fclose_stream(fp);

    free(fp);
    return 1;
}

int la_fclose_stream(la_file_t *fp) {
    if (!fp) return 0;
    
    if (fp->stream) fclose(fp->stream);
    else return 0;

    return 1;
}

int la_fheader(la_file_t *fp, la_header_t *out) {
    la_assert(fp != NULL);
    if (!out) return 0;

    memcpy(out, &fp->h, sizeof(*out));
    return 1;
}

long la_fsize(la_file_t *fp) {
    return fp->h.size;
}

int la_fseek(la_file_t *fp, long offset) {
    la_assert(fp != NULL);
    fp->offset = offset;

    fseek(fp->stream, fp->pos + fp->offset, SEEK_SET);
    return 1;
}

long la_ftell(la_file_t *fp) {
    return fp->offset;
}

int la_fread(la_file_t *fp, char *out, int bytes) {
    la_assert(fp != NULL);
    if (!out) return 0;

    if (~fp->mode & LA_READ_MODE) {
        la_error("write-only file");
        return 0;
    }

    int off = fp->pos + fp->offset;
    fseek(fp->stream, off, SEEK_SET);

    return fread(out, bytes, 1, fp->stream) == 0;
}

int la_fwrite(la_file_t *fp, const char *text, int len) {
 la_assert(fp != NULL);
    if (!text) return 0;

    if (len < 0) len = strlen(text);

    if (~fp->mode & LA_REWRITE_MODE && ~fp->mode & LA_WRITE_MODE) {
        la_error("read-only file");
        return 0;
    }
    int off = fp->pos + fp->offset;

    fseek(fp->stream, off, SEEK_SET);
    int res = fwrite(text, len, 1, fp->stream);
    int p = ftell(fp->stream);

    fseek(fp->stream, 0, SEEK_END);
    fp->h.size = ftell(fp->stream);
    fseek(fp->stream, p, SEEK_SET);
    fp->offset = p - fp->pos;

    return res;
}

int la_frewrite(la_file_t *fp, const char *text, int len) {
    la_assert(fp != NULL);
    if (!text) return 0;

    if (len < 0) len = strlen(text);

    if (~fp->mode & LA_WRITE_MODE && ~fp->mode & LA_REWRITE_MODE) {
        la_error("read-only file");
        return 0;
    }
    int start_sz = fp->offset - fp->pos;

    char sbuff[start_sz];
    la_fread(fp, sbuff, start_sz);

    int mode = fp->mode;

    la_freopen(fp, NULL, LA_REWRITE_MODE);
    la_freopen(fp, NULL, mode);

    la_fwrite(fp, sbuff, start_sz);
    la_fwrite(fp, text, len);

    return 1;
}

int la_fappend(la_file_t *fp, const char *text, int len) {
    la_assert(fp != NULL);
    if (!text) return 0;

    if (~fp->mode & LA_WRITE_MODE && ~fp->mode & LA_REWRITE_MODE) {
        la_error("read-only file");
        return 0;
    }

    la_fseek(fp, fp->h.size);
    la_fwrite(fp, text, len);

    return 1;
}

/************************
 *      Directory       *
 ************************/

int la_dheader(la_dir_t *dir, la_header_t *out) {
    la_assert(dir != NULL);
    if (!out) return 0;

    memcpy(out, &dir->h, sizeof(*out));
    return 1;
}

la_dir_t* la_dopen(const char *path) {
    la_assert(path != NULL);
    char p[100];
    DIR *dp = NULL;

    p[0] = '\0';
    la_resolve_path(path, p);
    dp = opendir(p);
    if (!dp) {
        la_error("cannot open '%s'", path);
        return NULL;
    }

    la_dir_t *dir = (la_dir_t*)malloc(sizeof(*dir));
    dir->stream = dp;

    _header(p, &dir->h);
    char *c = p;
    printf("%c\n", c[0]);
    while (*c == '.' || *c == '/') c++;
    strcpy(dir->h.name, c);
    return dir;
}

int la_dclose(la_dir_t *dir) {
    if (!dir) return 0;
    if (dir->stream) closedir(dir->stream);

    free(dir);
    return 1;
}

la_file_t* la_dfopen(la_dir_t *dir, const char *filename, int mode) {
    if (!dir) return NULL;
    if (!filename) return NULL;

    char file[100] = "";
    strcat(file, dir->h.name);
    strcat(file, "/");
    strcat(file, filename);

    la_file_t *fp = la_fopen(file, mode);

    if (!fp) {
        la_error("failed to open: %s", file);
        return NULL;
    }
    return fp;
}

la_dir_t* la_ddopen(la_dir_t *dir, const char *path) {
    if (!dir) return NULL;
    if (!path) return NULL;

    char file[100] = "";
    strcat(file, dir->h.name);
    strcat(file, "/");
    strcat(file, path);

    la_dir_t *dp = la_dopen(file);

    if (!dp) {
        la_error("failed to open: %s", file);
    }
    return dp;
}

int la_dread(la_dir_t *dir, la_header_t *out) {
    if (!dir) {
        la_error("dir cannot be NULL");
        return 0;
    }
    if (!out) return 0;
    struct dirent *d = NULL;

    d = readdir(dir->stream);
    if (!d) return 0;

    char file[100];
    sprintf(file, "%s/%s", dir->h.name, d->d_name);
    _header(file, out);
    strcpy(out->name, d->d_name);
    return 1;
}

/*************************
 *        Virtual        *
 *************************/

int _posix_to_la_header(la_posix_header_t *posh, la_header_t *out) {
    if (!posh) {
        la_error("posix header cannot be NULL");
        return 0;
    }
    if (!out) return 0;

    strcpy(out->name, posh->name);
    strcpy(out->gname, posh->gname);
    strcpy(out->uname, posh->uname);
    out->size = strtol(posh->size, NULL, 8);
    out->mode = strtol(posh->mode, NULL, 8);
    out->gid = strtol(posh->gid, NULL, 10);
    out->uid = strtol(posh->uid, NULL, 10);
    
    return 1;
}

int _la_header_to_posix(la_header_t *h, la_posix_header_t *out) {

    return 1;
}

la_node_t* la_node_create(int type) {
    la_node_t *node = (la_node_t*)malloc(sizeof(*node));
    la_assert(node != NULL);

    memset(node, 0, sizeof(*node));
    node->type = type;

    return node;
}

int la_node_exists(la_node_t *node, const char *name) {
    if (!node) return 0;
    if (!name) return 0;

    la_node_t *aux = node;
    while (aux->next) {
        char *p = node->fp.h.name;
        if (*p == '.') p++;
        if (*p == '/') p++;
        if (!strcmp(p, name)) return 1;
        aux = aux->next;
    }

    return 0;
}

la_vfs_t* la_vopen(const char *vhd, int mode) {
    la_assert(vhd != NULL);
    la_vfs_t *drv = (la_vfs_t*)malloc(sizeof(*drv));
    if (!drv) la_fatal("cannot malloc for %s", vhd);
    memset(drv, 0, sizeof(*drv));
    drv->root = la_node_create(LA_TDIR);

    if (!la_freopen(&drv->stream, vhd, mode)) {
        la_error("cannot open virtual drive");
        free(drv);
        return NULL;
    }

    memset(drv->root, 0, sizeof(*drv->root));
    drv->root->type = 1;


    la_file_t *stream = &drv->stream;
    int off = 0;
    la_node_t *curr = drv->root;
    int sz = 0;
    while (off < stream->h.size-1024) { /* tar two empty 512b blocks */
        la_posix_header_t ph;
        la_header_t h;
        
        la_fread(stream, (char*)&ph, sizeof(ph));    
        _posix_to_la_header(&ph, &h);

        if (!h.name[0]) break;

        la_node_t *node = la_node_create(h.type);
        if (curr) curr->next = node;

        memcpy(&node->fp, stream, sizeof(*stream));
        node->fp.offset = 0;
        node->fp.mode = mode;
        memcpy(&node->fp.h, &h, sizeof(h));

        la_fseek(stream, off + 512);
        node->fp.pos = stream->offset;
        off = stream->offset;

        if (h.size > 0) off += (1 + (h.size / 512)) * 512;
        la_fseek(stream, off);

        curr = node;
    }
    la_fseek(stream, 0);

    return drv;
}

#if 0
la_vfs_t* la_vmount(const char *path, int mode) {
    la_assert(path != NULL);
    la_vfs_t *drv = (la_vfs_t*)malloc(sizeof(*drv));
    if (!drv) la_fatal("cannot alloc mem for %s", path);
    memset(drv, 0, sizeof(*drv));

    la_dir_t *dir = la_dopen(path);
    if (!dir) la_fatal("failed to open dir: %s", path);

    la_header_t h;
    while (la_dread(dir, &h)) { 
        if (h.type == LA_TREG) {
            la_file_t *fp = la_dfopen(dir, h.name, mode);
        }
    }
    return NULL;
}
#endif

int la_vclose(la_vfs_t *drv) {
    if (!drv) return 0;
    
    if (drv->stream.stream) fclose(drv->stream.stream);
    return 1;
}

int la_isvfile(la_vfs_t *drv, const char *filename) {
   la_file_t *fp = la_vfopen(drv, filename); 
   return fp != NULL;
}

int la_isvdir(la_vfs_t *drv, const char *path) {
    return 0;
}

la_file_t* la_vfopen(la_vfs_t *drv, const char *filename) {
    la_assert(drv != NULL);
    if (!filename) return NULL;

    la_node_t *iter = drv->root;

    while (iter) {
        char *p = iter->fp.h.name;
        if ( *p == '.' ) p++;
        if ( *p == '/' ) p++;
        if (!strcmp(filename, p)) return &iter->fp;
        
        iter = iter->next;
    }
    
    la_error("file not found: %s", filename);
    return NULL;
}

la_dir_t* la_vdopen(la_vfs_t *drv, const char *path) {
    return NULL;
}

/***********************
 *        Utils        *
 ***********************/

int la_log_output(void *out) {
    latte()->out = (FILE*)out;
    return 1;
}

int la_log_(int mode, const char *file, int line, const char *function, const char *fmt, ...) {

    if (!latte()->out) latte()->out = stdout;

    time_t t = time(NULL);
    struct tm *tm_now;

    va_list args;

    int m = (mode & 0x7);

    char smode[15] = "";

    switch (m) {
        case LA_LOG: sprintf((char*)smode, "[log]   "); break;
        case LA_ERROR: sprintf((char*)smode, "[error] "); break;
        case LA_WARNING: sprintf((char*)smode, "[warn]  "); break;
        case LA_FATAL: sprintf((char*)smode, "[fatal] "); break;
        default: strcpy((char*)smode, "");
    }
    if (m <= 1 && mode & LA_TRACE) strcpy((char*)smode, "[trace] ");

    tm_now = localtime(&t);
    char tm_buf[10];
    strftime((char*)tm_buf, sizeof(tm_buf), "%H:%M:%S", tm_now);
    fprintf(latte()->out, "%s %s%s:%d: ", tm_buf, smode, file, line); 
    if (mode & LA_TRACE) fprintf(latte()->out, "in function %s(): ", function);

    va_start(args, fmt);
    vfprintf(latte()->out, fmt, args);

    va_end(args);
    fprintf(latte()->out, "\n");
    if (mode == LA_FATAL) exit(-1);
    return 1;
}

#endif /* LATTE_IMPLEMENTATION */
