#ifndef MOCHA_H
#define MOCHA_H

// #include "definitions.h"

#include <stdio.h>
#include <stdlib.h>

#define TRACEERR(...) int _;(void)_
#define ERRLOG(...) int _;(void)_

#define TRACELOG(...) int _;(void)_
#define LOG(...) int _;(void)_

#define MO_API extern
#define MOCHA_VALUE float
#define MO_TYPE_MASK 0x10

#define MAX_AUDIO_DATA 64

// #include "miniaudio.h"

#ifndef AUDIO_DEVICE_FORMAT
  #define AUDIO_DEVICE_FORMAT ma_format_f32
#endif

#ifndef AUDIO_DEVICE_CHANNELS
  #define AUDIO_DEVICE_CHANNELS 2
#endif

#ifndef AUDIO_DEVICE_SAMPLE_RATE
  #define AUDIO_DEVICE_SAMPLE_RATE 44100
#endif

#ifndef MAX_AUDIO_BUFFER_CHANNELS
  #define MAX_AUDIO_BUFFER_CHANNELS 255
#endif


typedef enum {
  MO_AUDIO_STREAM = 0,
  MO_AUDIO_STATIC
} MO_AUDIO_USAGE_;


typedef struct Mocha Mocha;

typedef struct mo_audio_s mo_audio_t;
typedef struct mo_wave_s mo_wave_t;
typedef unsigned int mo_uint32;

struct mo_data_s {
  int usage;
  union {
    void *data;
    void *fp;
  };
  mo_uint32 size;
};

/*======================
 * Audio Module
 *======================*/
MO_API int mo_init(int flags);
MO_API int mo_deinit();

MO_API int mo_start_device();
MO_API int mo_stop_device();

MO_API mo_audio_t* mo_audio(void *data, mo_uint32 size, int usage);
MO_API mo_audio_t* mo_audio_load(const char *filename, int usage);
MO_API int mo_audio_destroy(mo_audio_t *audio);

MO_API float mo_volume(mo_audio_t *audio, float volume);
MO_API int mo_play(mo_audio_t *audio);
MO_API int mo_pause(mo_audio_t *audio);
MO_API int mo_stop(mo_audio_t *audio);

MO_API int mo_is_playing(mo_audio_t *audio);

#endif // TICO_AUDIO_H
