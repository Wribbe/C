#include <stdlib.h>
#include <stdio.h>

#include "portaudio.h"

#define UNUSED(x) (void)x

typedef struct {
    float left_phase;
    float right_phase;
} paTestData;

/* This routine will be called by the PortAudio engine when audio is needed.
 * It may be called at interrupt level on some machines, so don't do anything
 * that could mess up the system, like calling malloc() or free().
 */

static int patestCallback( const void * inputBuffer,
                           void * outputBuffer,
                           unsigned long framesPerBuffer,
                           const PaStreamCallbackTimeInfo * timeInfo,
                           PaStreamCallbackFlags statusFlags,
                           void * userData ) {
    /* Cast data passed through stream to our structure. */

    UNUSED(timeInfo);
    UNUSED(statusFlags);

    paTestData * data = (paTestData*)userData;
    float *out = (float*)outputBuffer;
    unsigned int i;
    (void) inputBuffer; /* Prevent unused variable warning. */

    for(i=0; i<framesPerBuffer; i++) {
        *out++ = data->left_phase;
        *out++ = data->right_phase;
        /* Genrate simple sawtooth phaser that ranges between -1.0 and 1.0. */
        data->left_phase += 0.01f;
        /* When signal reaches top, drop back down. */
        if(data->left_phase >= 1.0f) {
            data->left_phase -= 2.0f;
        }
        /* Higher pitch so we can distinguish left and right. */
        data->right_phase += 0.03f;
        if(data->right_phase >= 1.0f) {
            data->right_phase -= 2.0f;
        }
    }
    return 0;
}

#define SAMPLE_RATE (44100)
#define NUM_SECONDS (2)
static paTestData data;

int main(void) {
    PaError err = Pa_Initialize();
    if(err != paNoError) {
        goto error;
    }

    PaStream * stream;

    err = Pa_OpenDefaultStream(&stream,
                               0, /* no input channels. */
                               2, /* stereo output. */
                               paFloat32, /* 32 bit floating point output. */
                               SAMPLE_RATE,
                               256, /* frame per buffer, number of samples that
                                       PortAudio will request from the
                                       callback. Many apps may want to use
                                       paFramesPerBufferUnspecified, which
                                       tells portAudio to pick the best
                                       possibly changing buffer size.*/
                               patestCallback, /* callback function. */
                               &data); /* Pointer passed to callback function. */

    if (err != paNoError ) {
        goto error;
    }

    err = Pa_StartStream(stream);
    if (err != paNoError) {
        goto error;
    }

    Pa_Sleep(NUM_SECONDS*1000);

    err = Pa_StopStream(stream);
    if(err != paNoError) {
        goto error;
    }

    err = Pa_Terminate();
    if(err != paNoError) {
        goto error;
    }
    return err;
error:
    printf("Portaudio error: %s\n", Pa_GetErrorText(err));
    Pa_Terminate();
    return err;
}
