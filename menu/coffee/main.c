#include "coffee.h"

// stack and heap share the same area

/*******************************************

      +-----------------+
      |       VM        |
      +-----------------+
      |      Stack      |
      +-----------------+ <- stack pointer
      |                 |
      |                 |
      |    Objs/Beans   | <- virtual addresses
      |                 |
      |                 |
      |                 |
      +-----------------+ <- heap pointer
      |      Heap       |
      +-----------------+


********************************************/

#define MEM_SIZE 16000 // 16kb

void *memory;

#define MAX_COMMAND 1024

static cf_Bean* evallist(cf_VM *vm, cf_Bean *list);
static cf_Bean* evallist(cf_VM *vm, cf_Bean *list) {
  if (list == cf_null(vm)) return cf_null(vm);
  cf_Bean *i = list;
  int args = -1;
  while (i != cf_null(vm)) {
    cf_Bean *t = cf_car(vm, i);

    // printf("%p: %d\n", t, t->tag);
    if (t == cf_symbol(vm, "car")) printf("%p\n", t);
    if (t == cf_symbol(vm, "quote")) {
      t = cf_car(vm, cf_cdr(vm, i));
      i = cf_cdr(vm, i);
    }
    else if (t->tag == CF_TPAIR) t = evallist(vm, t);
    else if (t->tag == CF_TSYMBOL) t = cf_cdr(vm, t);

    // printf("%p\n", t);
    // printf("%p", t);
    cf_push(vm, t);
    // printf("teste\n");
    args++;
    i = cf_cdr(vm, i);
    if (t == cf_symbol(vm, "car")) printf("%p\n", i);
  }

  return cf_call(vm, args);
}

static void printtree(cf_VM *vm, cf_Bean *b);
static void printtree(cf_VM *vm, cf_Bean *b) {

  // cf_Bean *i = b;
  while (b != cf_null(vm)) {
    cf_Bean *i = cf_car(vm, b);
    switch(i->tag & TYPE_MASK) {
      case CF_TNULL: printf("null"); break;
      case CF_TTRUE: printf("true"); break;
      case CF_TNUMBER: printf("num(%g)", i->n); break;
      // case CF_TSTRING: printstring;
      case CF_TPAIR: {
        printtree(vm, cf_car(vm, b));
        printtree(vm, cf_cdr(vm, b));
        break;
      }
    }

    printf(" ");
    b = cf_cdr(vm, b);
  }
  printf("\n");
}

int main(int argc, char ** argv) {
  memory = malloc(MEM_SIZE);
  cf_VM *vm = cf_init(memory, MEM_SIZE);

  printf("vm: %ld, bean: %ld\n", sizeof(cf_VM), sizeof(cf_Bean));
  printf("%ld %ld\n", sizeof(void*), sizeof(cf_Ptr));

  char op;
  float arg0, arg1;

  // char *cc = "\0";
  // cc++;
  // printf("%ld, %d\n", strlen(cc), cc == NULL);



  // cf_Bean *s = cf_symbol(vm, "print");
  // cf_Bean *cd = cf_cdr(vm, s);
  // printf("%p -> %p\n", s, cd);

#if 0
  // op = '+';
  // arg0 = 1;
  // arg1 = 1;
  FILE *fp;
  fp = fopen("main.coffee", "rb");
  fseek(fp, 0, SEEK_END);
  int sz = ftell(fp);
  fseek(fp, 0, SEEK_SET);

  char buff[sz+1];
  fread(buff, 1, sz, fp);
  buff[sz] = '\0';

  fclose(fp);

  cf_Bean *b = cf_read(vm, buff);
  // evallist(vm, b);
  // // printf("%p\n", cf_call(cf_VM *vm, int args));
  // cf_pushcfunc(vm, cf_print);
  // cf_push(vm, cf_car(vm, b));

  // cf_call(vm, 1);
  while (b != cf_null(vm)) {
    cf_Bean *i = cf_car(vm, b);
    cf_Bean *res = evallist(vm, i);
  //   // printf("%p: %d, %g\n", res, res->tag, res->n);

    b = cf_cdr(vm, b);
  }



  // while (i != cf_null(vm)) {
  //   cf_Bean *t = cf_car(vm, i);
  //   printf("%p: %d\n", t, t->tag);
  //   // cf_Bean *res = evalist(vm, t);
  //   // printf("%p\n", res);
    
  //   i = cf_cdr(vm, i);
  // }

  cf_gc(vm);


  // cf_pushcfunc(vm, cf_print);
  // cf_push(vm, b);

  // cf_call(vm, 1);
#else
  int run = 1;
  while (run) {
    printf("> ");
    char command[MAX_COMMAND];
    gets(command);

    if (*command == 'e') break;

    cf_Bean *b = cf_read(vm, command);
    while (b != cf_null(vm)) {
      cf_Bean *i = cf_car(vm, b);
      // cf_Bean *res = evallist(vm, i);

      // printtree(vm, i);

      // cf_pushcfunc(vm, cf_print);
      // cf_pushstring(vm, "tree:");
      // cf_Bean *iter = i;
      // int args = 1;
      // while (iter != cf_null(vm)) {
      //   // cf_Bean *ca = cf_car(vm, iter);
      //   cf_push(vm, cf_car(vm, iter));

      //   // printf("%p: %d\n", ca, ca->tag);

      //   args++;
      //   iter = cf_cdr(vm, iter);
      // }

      // cf_call(vm, args);

      cf_Bean *el = NULL;
      cf_Bean *iter = i;

      // cf_foreach(vm, el, iter) {
      //   printf("%p\n", el);
      // }

      cf_Bean *res = evallist(vm, i);
      // printf("%p\n", b);
      // printf("%p: %d, %g\n", res, res->tag, res->n);

      b = cf_cdr(vm, b);
    }
      // cf_print(vm);
      // printf("nnn: %f\n", b->n);
    // if (b != cf_null(vm)) {
    //   cf_pushcfunc(vm, cf_print);
    //   cf_push(vm, b);


    //   cf_call(vm, 1);
    // }
    cf_gc(vm);
    cf_settop(vm, 0);
  }
  // printf("op: ");
  // scanf("%c %g %g", &op, &arg0, &arg1);
#endif

  cf_status(vm);


  int nn = 250;
  // // push many objects to the stack
  printf("\npush %d objects to the stack\n", nn);
  for (int i = 0; i < nn; i++) cf_pushnumber(vm, i*2);

  cf_status(vm);

  cf_deinit(vm);
  free(memory);
  return 0;
}
