#include <SDL.h>
#include <stdio.h>
#include <math.h>
#include <limits.h>

// the "t" in "sin(wt)"
double t = 0;

// plays a sine wave signal sin(wt)
// where w = 2 * pi * f
// where f = 440Hz
void MyAudioCallback(void* userdata, Uint8* byteStream, int byteLength)
{
    // we asked for a stream of signed 16-bit integers,
    // so we can safely perform this cast.
    Sint16 *sampleStream = (Sint16*) byteStream;

    // calculate the number of samples to write
    int numberOfSamples = byteLength / sizeof(Sint16);

    // calculate how much to increase t after each sample
    double secondsPerSample = 1.0 / 48000;

    // fill the stream with a buffer full of sine wave
    for (int i = 0; i < numberOfSamples; i++)
    {
        // sample current value of sine wave
         double y = sin(2 * 3.14159265359 * 440 * t);

        // quantize to Sint16
         sampleStream[i] = (Sint16) (y * SHRT_MAX);

         // move time forward
         t += secondsPerSample;
    }
}

int main(int argc, char *argv[])
{
    // initialize SDL with only audio support
    if (SDL_Init(SDL_INIT_AUDIO)) {
        printf("SDL_Init: %s\n", SDL_GetError());
        return -1;
    }

    // describe the kind of audio format we want
    SDL_AudioSpec desiredAudioFormat;
    SDL_zero(desiredAudioFormat);                 // initialize all fields to zero
    desiredAudioFormat.freq = 48000;              // the frequency is 48,000 samples/second
    desiredAudioFormat.format = AUDIO_S16;        // each sample is a 16 bit signed integer
    desiredAudioFormat.channels = 1;              // mono audio
    desiredAudioFormat.samples = 4096;            // 4096 samples will be submitted at a time
    desiredAudioFormat.callback = MyAudioCallback;// call MyAudioCallback whenever it needs more samples to play
    
    // open audio device
    SDL_AudioDeviceID audioDeviceID = SDL_OpenAudioDevice(
        NULL,                // I don't care what device to use
        0,                   // I'm not using this device for audio capture
        &desiredAudioFormat, // the format I want audio in
        NULL,                // force format to be the desired one
        0);                  // no special audio conversion flags
    if (!audioDeviceID)
    {
        printf("SDL_OpenAudioDevice: %s\n", SDL_GetError());
        return -1;
    }

    // start playing audio by un-pausing it
    SDL_PauseAudioDevice(audioDeviceID, 0);

    while (1)
    {
        // wait for one event.
        SDL_Event e;
        SDL_WaitEvent(&e);

        // quit if it's a quit event. otherwise keep waiting.
        if (e.type == SDL_QUIT)
        {
            break;
        }
    }
    
    // throw everything away
    SDL_Quit();

    return 0;
}
