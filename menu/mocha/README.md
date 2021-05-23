# Mocha
a tiny c lib for handle audio

```c
#include "mocha.h"

int main(int argc, char ** argv) {

    mo_init(0);

    mo_audio_t *audio = mo_audio_load("music.wav", MO_AUDIO_STREAM);

    while (mo_is_playing(audio));

    mo_deinit();

    return 0;
}
```
