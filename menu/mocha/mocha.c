#include "mocha.h"

#define STB_VORBIS_HEADER_ONLY
#include "stb_vorbis.h"

#define DR_WAV_IMPLEMENTATION
#include "dr_wav.h"

#define DR_FLAC_IMPLEMENTATION
#include "dr_flac.h"

#define DR_MP3_IMPLEMENTATION
#include "dr_mp3.h"

#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"

#undef STB_VORBIS_HEADER_ONLY
#include "stb_vorbis.h"

#define mo_true 1
#define mo_false 0

struct mo_buffer_s {
    unsigned short id;
    ma_decoder decoder;
    float volume, pitch;

    int playing, paused;
    int loop, loaded;
    mo_uint32 offset;

    // mo_data_t data;
    int usage;
    union { void *data, *fp; };
    mo_uint32 size;
};

typedef unsigned int mo_sound_t;

struct mo_audio_t {
  ma_decoder decoder;
  unsigned char id;

  float volume;
  float pitch;

  int playing;
  int paused;
  int loop;
  int loaded;

//   MO_AUDIO_USAGE usage;

//   unsigned char *data;
//   size_t dataSize;

  mo_audio_t *data;

  size_t currentReadPos;
};

#define INDEXMASK 0xff
#define GROUPMASK 0xff00
#define USAGEMASK 0xf0000

typedef unsigned int mo_audio_instance_t;

struct mo_audio_s {
    ma_decoder decoder;
    mo_uint32 flags;

    float volume, pitch;

    int playing, paused;
    int loop, loaded;

    int usage;
    mo_uint32 size;
    mo_uint32 offset;
    union { void *data, *fp; };
};

struct Mocha {
  struct {
    ma_context ctx;
    ma_device device;
    ma_mutex lock;

    int isReady;
  } system;

  mo_audio_t data[MAX_AUDIO_DATA];
  mo_audio_t channels[MAX_AUDIO_BUFFER_CHANNELS];

  struct {
    mo_audio_t buffer[MAX_AUDIO_BUFFER_CHANNELS];
  } multiChannel;
};

static Mocha _mocha_ctx;
#define mocha() (&_mocha_ctx)

static char* _readfile(const char *filename, size_t *size) {
    FILE *fp;
    char *buffer;
    int sz = 0;

    fp = fopen(filename, "rb");

    fseek(fp, 0, SEEK_END);
    sz = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    buffer = malloc(sz+1);
    fread(buffer, 1, sz, fp);
    if (size) *size = sz;

    fclose(fp);

    return buffer;
}

// static mo_Audio audio = {0};

static ma_uint32 _read_and_mix_pcm_frames(mo_audio_t *audioBuffer, float *pOutputF32, ma_uint32 frameCount) {

  float temp[4096];
  ma_uint32 tempCapInFrames = ma_countof(temp) / AUDIO_DEVICE_CHANNELS;
  ma_uint32 totalFramesRead = 0;
  ma_decoder *pDecoder = &audioBuffer->decoder;
  float volume = audioBuffer->volume;
  float size = audioBuffer->size * ma_get_bytes_per_frame(AUDIO_DEVICE_FORMAT, AUDIO_DEVICE_CHANNELS);

  while (totalFramesRead < frameCount) {
    ma_uint32 iSample;
    ma_uint32 framesReadThisIteration;
    ma_uint32 totalFramesRemaining = frameCount - totalFramesRead;
    ma_uint32 framesToReadThisIteration = tempCapInFrames;
    if (framesToReadThisIteration > totalFramesRemaining) {
      framesToReadThisIteration = totalFramesRemaining;
    }

    if (audioBuffer->usage == MO_AUDIO_STREAM) {
      framesReadThisIteration = (ma_uint32)ma_decoder_read_pcm_frames(pDecoder, temp, framesToReadThisIteration);
    } else {
      framesReadThisIteration = framesToReadThisIteration;
      mo_uint32 aux = framesToReadThisIteration * ma_get_bytes_per_frame(AUDIO_DEVICE_FORMAT, AUDIO_DEVICE_CHANNELS);
      memcpy(temp, audioBuffer->data + audioBuffer->offset, aux);
      if (audioBuffer->offset > size) framesReadThisIteration = 0;
      audioBuffer->offset += aux;
    }


    if (framesReadThisIteration == 0) {
      break;
    }

    for (iSample = 0; iSample < framesReadThisIteration * AUDIO_DEVICE_CHANNELS; ++iSample) {
      pOutputF32[totalFramesRead * AUDIO_DEVICE_CHANNELS + iSample] += temp[iSample] * volume;
    }

    totalFramesRead += framesReadThisIteration;

    if (framesReadThisIteration < framesToReadThisIteration) {
      break;
    }
  }

  return totalFramesRead;
}

static void _data_callback(ma_device *pDevice, void *pOutput, const void *pInput, ma_uint32 frameCount) {
  float *fOutput = (float *)pOutput;
  Mocha *mo = pDevice->pUserData;

  ma_mutex_lock(&mo->system.lock);
  int i;
  for (i = 0; i < MAX_AUDIO_BUFFER_CHANNELS; i++) {
    mo_audio_t *audioBuffer = &mo->multiChannel.buffer[i];
    if (audioBuffer->playing && audioBuffer->loaded && !audioBuffer->paused) {
      ma_uint32 framesRead = _read_and_mix_pcm_frames(audioBuffer, fOutput, frameCount);
      if (framesRead < frameCount) {
        if (audioBuffer->loop) {
          ma_decoder_seek_to_pcm_frame(&audioBuffer->decoder, 0);
          audioBuffer->offset = 0;
        } else {
          audioBuffer->playing = mo_false;
        }
      }
    }
  }
  ma_mutex_unlock(&mo->system.lock);

  (void)pInput;
}

int mo_init(int flags) {
    Mocha *mo = mocha();
    ma_context_config ctxConfig = ma_context_config_init();
    ma_result result = ma_context_init(NULL, 0, &ctxConfig, &mo->system.ctx);
    if (result != MA_SUCCESS) {
        fprintf(stderr, "Failed to init audio context");
        // dog_log(1, "[mocha] failed to init mocha");
        return 0;
    }

    ma_device_config devConfig = ma_device_config_init(ma_device_type_playback);
    devConfig.playback.pDeviceID = NULL;
    devConfig.playback.format = AUDIO_DEVICE_FORMAT;
    devConfig.playback.channels = AUDIO_DEVICE_CHANNELS;
    devConfig.sampleRate = AUDIO_DEVICE_SAMPLE_RATE;
    devConfig.pUserData = mo;
    devConfig.dataCallback = _data_callback;

  result = ma_device_init(&mo->system.ctx, &devConfig, &mo->system.device);
  if (result != MA_SUCCESS) {
    fprintf(stderr, "Failed to init device");
    // dog_log(1, "[mocha] failed to init mocha device");
    ma_context_uninit(&mo->system.ctx);
    // return -1;
    return 0;
  }

  result = mo_start_device(mo);
  if (result != MA_SUCCESS) {
    fprintf(stderr, "Failed to start device");
    // dog_log(1, "[mocha] failed to start mocha device");
    ma_context_uninit(&mo->system.ctx);
    ma_device_uninit(&mo->system.device);
    return 0;
  }

  if (ma_mutex_init(&mo->system.ctx, &mo->system.lock) != MA_SUCCESS) {
    fprintf(stderr, "Failed to start mutex");
    // dog_log(1, "[mocha] failed to init mocha mutex");
    ma_device_uninit(&mo->system.device);
    ma_context_uninit(&mo->system.ctx);
    // return -1;
    return 0;
  }

  int i;
  for (i = 0; i < MAX_AUDIO_BUFFER_CHANNELS; i++) {
    mo_audio_t *buffer = &mo->multiChannel.buffer[i];
    buffer->playing = mo_false;
    buffer->volume = 1.f;
    buffer->pitch = 1.f;
    buffer->loaded = mo_false;
    buffer->paused = mo_false;
    buffer->loop = mo_false;
    buffer->flags = i;
  }

  mo->system.isReady = mo_true;

  // LOG("Audio Module initiated");
  return 1;
}

int mo_start_device() {
    Mocha *mo = mocha();
    return ma_device_start(&mo->system.device);
}

int mo_stop_device() {
    Mocha *mo = mocha();
    return ma_device_stop(&mo->system.device);
}

int mo_deinit() {
    Mocha *mo = mocha();
    if (mo->system.isReady) {
        ma_mutex_uninit(&mo->system.lock);
        ma_device_uninit(&mo->system.device);
        ma_context_uninit(&mo->system.ctx);
    } else {
        fprintf(stderr, "Audio Module could not be closed, not initialized");
    }
    return 1;
}

#if 0
int mocha_get_id(mo_audio_t *buffer) {
    return buffer->id;
}

mo_audio_t *mocha_get_from_id(unsigned int id) {
    Mocha *mo = mocha();
    return &mo->multiChannel.buffer[id];
}
#endif

mo_audio_t *mo_audio(void *data, mo_uint32 size, int usage) {
    Mocha *mo = mocha();
    int index = 0;
    int i;
    for (i = 0; i < MAX_AUDIO_BUFFER_CHANNELS; i++) {
        if (!mo->multiChannel.buffer[i].loaded) {
            index = i;
            break;
        }
    }

    mo_audio_t *buff = &mo->multiChannel.buffer[index];
    ma_decoder_config decoderConfig = ma_decoder_config_init(AUDIO_DEVICE_FORMAT, AUDIO_DEVICE_CHANNELS, AUDIO_DEVICE_SAMPLE_RATE);
    ma_result result = 0;
    buff->offset = 0;
    if (usage == MO_AUDIO_STREAM) {
        mo_audio_t *aData = NULL;
        aData = malloc(sizeof(*aData));
        aData->data = (unsigned char*)data;
        aData->usage = usage;
        aData->size = size;
        result = ma_decoder_init_memory(aData->data, aData->size, &decoderConfig, &buff->decoder);
        buff->data = aData;
        // mocha_resources_add_sound(filename, aData);
    } else {
        ma_uint64 pFrameCountOut;
        void *ppData;
        result = ma_decode_memory(data, size, &decoderConfig, &pFrameCountOut, &ppData);
        buff->data = ppData;
        buff->size = pFrameCountOut;
        free(data);
    }

    if (result != MA_SUCCESS) {
        fprintf(stderr, "Failed to load sound\n");
        return NULL;
     }
    buff->loaded = mo_true;
    buff->playing = mo_false;
    buff->paused = mo_true;
    //   audioBuffer->usage = usage;
    buff->usage = usage;
    buff->loop = mo_false;

    return buff;
}

mo_audio_t* mo_audio_load(const char *filename, int usage) {
    Mocha *mo = mocha();
    int index = 0;
    int i;
    for (i = 0; i < MAX_AUDIO_BUFFER_CHANNELS; i++) {
        if (!mo->multiChannel.buffer[i].loaded) {
            index = i;
            break;
        }
    }
    mo_audio_t *audioBuffer = &mo->multiChannel.buffer[index];
    ma_decoder_config decoderConfig = ma_decoder_config_init(AUDIO_DEVICE_FORMAT, AUDIO_DEVICE_CHANNELS, AUDIO_DEVICE_SAMPLE_RATE);
    ma_result result = 0;
    audioBuffer->offset = 0;

    if (usage == MO_AUDIO_STREAM) {
        // mo_AudioData *aData = mocha_resources_get_sound(filename);
  	mo_audio_t *aData = NULL;
        if (aData && aData->usage == usage) {
            audioBuffer->data = aData;
        } else {
            aData = malloc(sizeof(*aData));
            aData->data = (unsigned char*)_readfile(filename, &aData->size);
            aData->usage = usage;
        }

        result = ma_decoder_init_memory(aData->data, aData->size, &decoderConfig, &audioBuffer->decoder);
        audioBuffer->data = aData;
        // mocha_resources_add_sound(filename, aData);
    } else {
        size_t size;
        char *data = _readfile(filename, &size);
        ma_uint64 pFrameCountOut;
        void *ppData;
        result = ma_decode_memory(data, size, &decoderConfig, &pFrameCountOut, &ppData);
        audioBuffer->data = ppData;
    }

    if (result != MA_SUCCESS) {
        fprintf(stderr, "Failed to load sound '%s'", filename);
        return NULL;
     }
    audioBuffer->loaded = mo_true;
    audioBuffer->playing = mo_false;
    audioBuffer->paused = mo_true;
    //   audioBuffer->usage = usage;
    audioBuffer->loop = mo_false;

    return audioBuffer;
}

int mo_audio_destroy(mo_audio_t *audio) {
    if (!audio) return 0;   

    return 1;
}

int mo_play(mo_audio_t *audioBuffer) {
  if (audioBuffer) {
    audioBuffer->playing = mo_true;
    audioBuffer->paused = mo_false;
    return 1;
  }
  return 0;
}

int mo_stop(mo_audio_t *audioBuffer) {
  if (audioBuffer) {
    audioBuffer->playing = mo_false;
    ma_decoder_seek_to_pcm_frame(&audioBuffer->decoder, 0);
  }
  return 1;
}

int mo_pause(mo_audio_t *audioBuffer) {
  if (audioBuffer) {
    audioBuffer->paused = mo_true;
    audioBuffer->playing = mo_false;
  }
  return 1;
}

float mo_volume(mo_audio_t *audioBuffer, float volume) {
    if (audioBuffer) audioBuffer->volume = volume;
    else ma_device_set_master_volume(&mocha()->system.device, volume);
    return volume;
}

int mo_is_playing(mo_audio_t *audioBuffer) {
  return audioBuffer->playing;
}

int mo_is_paused(mo_audio_t *audioBuffer) {
  return audioBuffer->paused;
}

int mo_unload(mo_audio_t *audioBuffer) {
  if (audioBuffer) {
    audioBuffer->loaded = mo_false;
    audioBuffer->data = NULL;
    ma_decoder_uninit(&audioBuffer->decoder);
  }
  return 1;
}

