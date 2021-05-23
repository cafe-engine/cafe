#ifndef COFFEE_H
#define COFFEE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define COFFEE_VERSION "0.1.0"
#define CF_VERSION COFFEE_VERSION

// #define OBJ_MAX 16000

#define STR(x) #x
#define CF_ASSERT(expr, error_msg) if(!(expr)) { \
  fprintf(stderr, "[coffee] assertion '%s' failed, %s\n", STR(expr), error_msg); \
  exit(1); \
}

#define CF_API

#define cf_foreach(vm, el, iter) for ((el) = cf_car((vm), (iter)); (iter) != cf_null((vm)); (iter) = cf_cdr((vm), (iter)), (el) = cf_car((vm), (iter)))

#define MARK_MASK 0x80
#define TYPE_MASK 0x7f

#define STACK_MAX 256

typedef struct cf_VM cf_VM;
typedef struct cf_Bean cf_Bean;
typedef cf_Bean*(*cf_CFunc)(cf_VM*);

typedef float cf_Number;
typedef double cf_Double;

typedef unsigned short cf_Short;
typedef unsigned short cf_VPtr;
typedef int cf_Int;
typedef unsigned int cf_UInt;
typedef long int cf_LInt;
typedef void* cf_Ptr;
typedef char* cf_String;
typedef char cf_Bool;
typedef unsigned char cf_Byte;

// co_short_t st;
// co_byte_t byte;
// co_string_t str;

typedef unsigned int cf_vptr_t;
typedef unsigned short cf_uint16;
typedef unsigned int cf_uint32;
typedef unsigned char cf_uint8;

typedef enum {
  OP_RET = 0, // ret
  OP_REQ,     // req
  OP_PRINT,   // prn
  OP_ADD,     // +
  OP_SUB,     // -
  OP_MUL,     // *
  OP_DIV      // /
} OPCODE;

typedef struct cf_OpCode {
  const char *name;
  cf_Byte args;
  cf_CFunc fn;
} cf_OpCode;

CF_API cf_Bean *cf_ret(cf_VM *vm);
CF_API cf_Bean *cf_req(cf_VM *vm);
// CF_API cf_Bean *cf_print(cf_VM *vm);
CF_API cf_Bean *cf_print(cf_VM *vm);
CF_API cf_Bean *cf_add(cf_VM *vm);
CF_API cf_Bean *cf_sub(cf_VM *vm);
CF_API cf_Bean *cf_mul(cf_VM *vm);
CF_API cf_Bean *cf_div(cf_VM *vm);

CF_API cf_Bean *cf_quote(cf_VM *vm);

static cf_OpCode opcodes[] = {
  {"ret", 1, NULL},
  {"req", 1, NULL},
  {"prn", -1, cf_print},
  {"+", 2, cf_add},
  {"-", 2, cf_sub},
  {"*", 2, cf_mul},
  {"/", 2, cf_div}
};

// typedef union cf_Value {
//   cf_Byte header;
//   cf_Byte b;
//   cf_Number n;
//   cf_String str;
//   cf_Ptr ptr;
//   cf_CFunc fn;
//   cf_Bean *bn;
// } cf_Value;

// typedef union cf_Value {
//   cf_Byte b;
//   struct {
//     cf_Byte p, q, r, s;
//   };
//   cf_Byte str[4];
//   cf_Number n;
//   cf_UInt vptr;


//   // cf_String
// } cf_Value;

// co_value_t val;

typedef union cf_Value {
  cf_Byte b;
  cf_VPtr vptr;
  cf_UInt tag;
  cf_Number n;
  cf_UInt i;
  cf_Byte str[4];
  struct {
    cf_VPtr vptr0;
    cf_VPtr vptr1;
  };
  // struct
} cf_Value;

struct cf_Bean {
  union {
    struct {
      cf_Byte tag;
      cf_VPtr next;
      union {
        cf_Byte byte;
        cf_Number n;
        cf_UInt i;
        cf_Byte str[4];
        struct {
          cf_VPtr car;
          cf_VPtr cdr;
        };
      };
    };

    cf_Double ln;
    cf_LInt li;
  };
};

// co_bean_t bean;

// struct cf_Bean {

//   union {
//     struct {

//       union {
//         cf_Byte tag;
//         cf_Value car;
//         struct {
//           cf_VPtr _;
//           cf_VPtr vptr;
//         }
//       };

//       union {
//         cf_Byte b;
//         cf_Number n;
//         cf_Value cdr;
//       };
//     };

//     // cf_String str;
//     cf_Bean *bn;
//     cf_Ptr ptr;
//     cf_CFunc fn;
//     cf_Double ln;
//   };
//   // union {
//   //   cf_Byte tag;
//   //   struct {
//   //     cf_Value car;
//   //     cf_Value cdr;
//   //   };
//   //   cf_Number n;
//   //   cf_String str;
//   //   cf_Ptr ptr;
//   //   cf_CFunc fn;
//   //   cf_Double ln;
//   // };
//   // union {
//   //   struct {
//   //     union {
//   //       cf_Value tag;
//   //       cf_Value car;
//   //     };
//   //     cf_Value cdr;
//   //   };
//   //   cf_CFunc fn;
//   //   cf_Ptr ptr;
//   //   cf_Double n;
//   //   cf_String str;
//   // };
// };

// struct cf_Bean {
//   cf_Byte header; // 1bit mark + 7bit type
//   cf_Byte s[3];
//   // union {
//   //   cf_Byte b;
//   //   cf_Number n;
//   //   cf_String str;
//   //   cf_Ptr ptr;
//   //   cf_CFunc fn;
//   //   struct {
//   //     cf_Bean *car;
//   //     cf_Bean *cdr;
//   //   };
//   // };
//   union {
//     struct {
//       cf_Value car;
//       cf_Value cdr;
//     };
//     cf_Ptr ptr;
//     cf_CFunc fn;
//     cf_Double n;
//     cf_String str;
//     // void **pptr;
//     // cf_Byte data[8];
//     // cf_Short d[4];
//     // cf_LInt ptr;
//     // void* ptr;
//   };

//   // cf_Bean *next;
// };

typedef enum {
  CF_TFREE = 0,
  CF_TNULL, CF_TTRUE,
  CF_TNUMBER, CF_TSTRING,
  CF_TPAIR, CF_TTABLE,
  CF_TFUNC, CF_TCFUNC,
  CF_TPTR,

  CF_TSYMBOL,
  CF_TMAX
} CF_TYPE;

struct cf_HashTable {
  cf_Bean *nodes[10];
};

struct cf_VM {
  cf_Bean *sNull;
  cf_Bean *sTrue;

  unsigned int sp; // stack pointer
  unsigned int hp; // heap pointer

  unsigned int fp;

  // cf_Bean stack[STACK_MAX];
  union {
    cf_Short *stack;
    cf_Bean *memory;
  };
  int mem_size;

  // cf_Bean *first;
  // cf_Bean *gc;
  cf_Bean *symbol_list;
  // cf_Bean *sym_table[10];

  // symbols

  // cf_Byte *code;
};

// cf_Bean sNull;
// cf_Bean sTrue;

#if defined(__cplusplus)
extern "C" {
#endif

#define tonumber(bean) (bean)->n
#define tostring(bean) (bean)->str
#define tobool(bean) 
#define tocfunc(vm, b) (cf_cdr(vm, b)->fn);

// Coffee* co_init(void *ptr, int size);
// int cof_deinit(Coffee *vm);
// cfe_mark(Coffee *vm, co_bean_t *bean);
// int co_sweep(Coffee *vm);
// 

CF_API cf_VM* cf_init(void *ptr, int size);
CF_API void cf_deinit(cf_VM *vm);

CF_API void cf_mark(cf_VM *vm, cf_Bean *bean);
CF_API void cf_sweep(cf_VM *vm);
CF_API void cf_gc(cf_VM *vm);

CF_API void cf_error(cf_VM *vm, const cf_String error);
CF_API cf_Bean* cf_status(cf_VM *vm);

CF_API int cf_top(cf_VM *vm);
CF_API void cf_settop(cf_VM *vm, int index);

CF_API cf_Bean* cf_call(cf_VM *vm, int args);


CF_API cf_Bean* cf_set(cf_VM *vm);
CF_API cf_Bean* cf_get(cf_VM *vm, int index);
#define cf_getnumber(vm, index) tonumber(cf_get((vm), (index)))

CF_API void cf_push(cf_VM *vm, cf_Bean *bean);
#define cf_pushnumber(vm, n) cf_push((vm), cf_number((vm), (n)))
#define cf_pushstring(vm, str) cf_push((vm), cf_string((vm), (str)))
#define cf_pushsymbol(vm, str) cf_push((vm), cf_symbol((vm), (str)))
#define cf_pushbool(vm, b) cf_push((vm), cf_bool((vm), (b)))
#define cf_pushcfunc(vm, cfn) cf_push((vm), cf_cfunc((vm), (cfn)))
#define cf_pushptr(vm, ptr) cf_push((vm), cf_ptr((vm), (ptr)))
#define cf_pushnull(vm) cf_push((vm), cf_null((vm)))

CF_API cf_Bean* cf_pop(cf_VM *vm);
#define cf_popnumber(vm) tonumber(cf_pop((vm)))

// pop a value from the stack and set as a global variable
CF_API cf_Bean* cf_setglobal(cf_VM *vm, const cf_String name);

// Check for a free object or malloc new
// Threw an error if is out of memory (hp < sp)
CF_API cf_Bean* cf_create(cf_VM *vm);

// Alloc a new object at heap (hp++)
CF_API cf_Bean* cf_malloc(cf_VM *vm);

// return the global null object
CF_API cf_Bean* cf_null(cf_VM *vm);
// return the global true object
CF_API cf_Bean* cf_true(cf_VM *vm);
// return a new number
CF_API cf_Bean* cf_number(cf_VM *vm, cf_Number n);
// return a new string
CF_API cf_Bean* cf_string(cf_VM *vm, cf_String str);
// return global true or null
CF_API cf_Bean* cf_bool(cf_VM *vm, cf_Bool b);
// return a new pair or 'a' and 'b'
CF_API cf_Bean* cf_pair(cf_VM *vm, cf_Bean *a, cf_Bean *b);
#define cf_cons(vm, a, b) cf_pair(vm, a, b)
// return a new table
CF_API cf_Bean* cf_table(cf_VM *vm, int sz);

CF_API cf_Bean* cf_list(cf_VM *vm, int sz);
// return a new cfunc
CF_API cf_Bean* cf_cfunc(cf_VM *vm, cf_CFunc fn);
// return a new ptr
CF_API cf_Bean* cf_ptr(cf_VM *vm, cf_Ptr ptr);
// return a symbol (check in symbol list first)
CF_API cf_Bean* cf_symbol(cf_VM *vm, const cf_String str);


// return car (object at b->car position)
CF_API cf_Bean* cf_car(cf_VM *vm, cf_Bean *b);
// return car (object at b->cdr position)
CF_API cf_Bean* cf_cdr(cf_VM *vm, cf_Bean *b);

#define cf_tobean(vm, index) cf_optbean((vm), (index), cf_null((vm)))
#define cf_tonumber(vm, index) cf_optnumber((vm), (index), -1)
#define cf_tostring(vm, index) cf_optstring((vm), (index), NULL)
#define cf_tosymbol(vm, index) cf_optsymbol((vm), (index), cf_null((vm)))
#define cf_tobool(vm, index) cf_optbool((vm), (index), -1)
#define cf_tocfunc(vm, index) cf_optcfunc((vm), (index), NULL)
#define cf_toptr(vm, index) cf_optptr((vm), (index), NULL)

CF_API cf_Bean* cf_optbean(cf_VM *vm, int index, cf_Bean *opt);
CF_API cf_Number cf_optnumber(cf_VM *vm, int index, cf_Number opt);
CF_API const cf_String cf_optstring(cf_VM *vm, int index, cf_String opt);
CF_API cf_Bean* cf_optsymbol(cf_VM *vm, int index, cf_Bean *bn);
CF_API cf_Bool cf_optbool(cf_VM *vm, int index, cf_Bool opt);

CF_API cf_CFunc cf_optcfunc(cf_VM *vm, int index, cf_CFunc opt);
CF_API cf_Ptr cf_optptr(cf_VM *vm, int index, cf_Ptr opt);

// REPL

CF_API cf_Bean* cf_read(cf_VM *vm, const char *str);
CF_API cf_Bean* cf_eval(cf_VM *vm);

#if defined(__cplusplus)
}
#endif

#endif