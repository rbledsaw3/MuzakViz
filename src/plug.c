#include "plug.h"
#include <assert.h>
#include <complex.h>
#include <math.h>
#include <memory.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

#include <raylib.h>

float in[N];
float complex out[N];
Plug plug = { 0 };

void fft(float in[], float complex out[], size_t n, size_t stride) {
  assert(n > 0);

  if (n == 1) {
    out[0] = in[0] + I * in[0];
    return;
  }

  fft(in, out, n / 2, stride * 2);
  fft(in + stride, out + n / 2, n / 2, stride * 2);

  for (size_t k = 0; k < n / 2; ++k) {
    float t = (float) k / n;
    float complex v = cexp(-2 * PI * t * I) * out[k + n / 2];
    float complex e = out[k];
    out[k] = e + v;
    out[k + n / 2] = e - v;
  }
}

float amp(float complex z) {
  float a = fabsf(crealf(z));
  float b = fabsf(cimagf(z));
  if (a < b)
    return b;
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
}

void plug_init(Plug* plug, const char* file_path) {
  plug->music = LoadMusicStream(file_path);
  SetMusicVolume(plug->music, 0.5f);
  PlayMusicStream(plug->music);
  AttachAudioStreamProcessor(plug->music.stream, callback);
}

void plug_update(Plug* plug) {
  UpdateMusicStream(plug->music);

  if (IsKeyPressed(KEY_SPACE)) {
    if (IsMusicStreamPlaying(plug->music)) {
      PauseMusicStream(plug->music);
    } else {
      ResumeMusicStream(plug->music);
    }
  }

  int w = GetRenderWidth();
  int h = GetRenderHeight();

  BeginDrawing();
  ClearBackground(CLITERAL(Color) { 0x18, 0x18, 0x18, 0xFF });

  fft(in, out, N, 1);

  float max_amp = 0.0f;
  for (size_t i = 0; i < N; ++i) {
    float a = amp(out[i]);
    if (max_amp < a) {
      max_amp = a;
    }
  }
  float cell_width = (float) w / N;
  for (size_t i = 0; i < N; ++i) {
    float t = amp(out[i]) / max_amp;
    DrawRectangleGradientV(i * cell_width, h * .75 - h / 2 * t, cell_width, h / 2 * t, MAROON, RED);
  }
  EndDrawing();
}
