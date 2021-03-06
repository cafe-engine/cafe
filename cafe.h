#ifndef CAFE_H
#define CAFE_H

#define CAFE_VER "0.1.0"
#define CAFE_API

#define TEA_TNUM float

typedef struct ca_data_s ca_data_t;

struct ca_data_s {
    unsigned int size;
    void *data;
};

CAFE_API int cafe_init(int argc, char **argv);
CAFE_API int cafe_deinit();

CAFE_API int cafe_loop();

// define these functions
CAFE_API int cafe_load();
CAFE_API int cafe_update(float dt);
CAFE_API int cafe_draw();

#endif /* CAFE_H */
