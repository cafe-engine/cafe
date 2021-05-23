/*
 * =====================================================================================
 *
 *       Filename:  main.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  02/05/2021 19:12:13
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include "mocha.h"

int main(int argc, char ** argv) {
    mo_init(0);
    
    mo_audio_t *audio = mo_audio_load("music.wav", 0);
    mo_play(audio);

    while (mo_is_playing(audio));

    mo_deinit();

    return 0;
}
