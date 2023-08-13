#include <stdio.h>
#include <complex.h>
#include <assert.h>
#include <stdint.h>
#include <math.h>
#include <memory.h>
#include <unistd.h>

#include <raylib.h>

#include <dlfcn.h> // POSIX Dependency

#include "plug.h"

#define N 64
float in[N] = {0};
float complex out[N] = {0};
float pi;
float max_amp;

typedef struct {
    float left;
    float right;
} Frame;

void fft(float in[], float complex out[], size_t n, size_t stride) {
    assert(n > 0);

    if (n == 1) {
        out[0] = in[0] + I*in[0];
        return;
    }

    fft(in, out, n/2, stride*2);
    fft(in + stride, out + n/2, n/2, stride*2);

    for (size_t k = 0; k < n/2; ++k) {
        float t = (float) k/n;
        float complex v = cexp(-2*pi*t*I) * out[k + n/2];
        float complex e = out[k];
        out[k]       = e + v;
        out[k + n/2] = e - v;
    }
}

float amp (float complex z) {
    float a = fabsf(crealf(z));
    float b = fabsf(cimagf(z));
    if (a < b) return b;
    return a;
}

void callback(void* bufferData, unsigned int frames) {

    if (frames <= N) {
        return;
    }
    
    Frame* fs = bufferData;

    for (size_t i = 0; i < frames; ++i) {
        in[i] = fs[i].left;
    }
    
    fft(in, out, N, 1);
    
    max_amp = 0.0f;
    for (size_t i = 0; i < N; ++i) {
        float a = amp(out[i]);
        if (max_amp < a ) {
            max_amp = a;
        }
    }
}

plug_hello_t plug_hello = NULL;

int main(int argc, char* argv[]) {

    const char* libplug_file_name = "libplug.so";
    void* libplug = dlopen(libplug_file_name, RTLD_NOW);
    if (libplug == NULL) {
        fprintf(stderr, "ERROR: could not load %s: %s\n", libplug_file_name, dlerror());
        return 1;
    }

    plug_hello = dlsym(libplug, "plug_hello");
    if (plug_hello == NULL) {
        fprintf(stderr, "ERROR: could not find plug_hello in %s: %s\n", libplug_file_name, dlerror());
        return 1;
    }

    plug_hello();

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <path_to_mp3_file>\n", argv[0]);
        return 1;
    }

    pi = atan2f(1, 1) * 4;

    InitWindow(800, 600, "MuzViz");
    SetTargetFPS(60);

    InitAudioDevice();
    const char* file_path = argv[1];

    Music music = LoadMusicStream(file_path);

    PlayMusicStream(music);
    AttachAudioStreamProcessor(music.stream, callback);
    SetMasterVolume(0.5f);

    while (!WindowShouldClose()){
        UpdateMusicStream(music);

        if(IsKeyPressed(KEY_SPACE)){
            if(IsMusicStreamPlaying(music)){
                PauseMusicStream(music);
            } else {
                ResumeMusicStream(music);
            }
        }
        
        int w = GetRenderWidth();
        int h = GetRenderHeight();

        BeginDrawing();
        ClearBackground(CLITERAL(Color) {0x18, 0x18, 0x18, 0xFF});
        float cell_width = (float) w/N;
        for (size_t i = 0; i < N; ++i) {
            float t = amp(out[i])/max_amp;
            DrawRectangleGradientV (i*cell_width, h*.6 - h/2*t, cell_width, h/2*t, RED, MAROON);        
        }
        EndDrawing();
    }
    return 0;
}
