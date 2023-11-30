#ifndef PLUG_H_
#define PLUG_H_

#include <raylib.h>
#include <unistd.h>

#define N 32

typedef struct {
    size_t version;
    Music music;
} Plug;

typedef struct {
    float left;
    float right;
} Frame;

typedef void (*plug_init_t)(Plug* plug, const char* file_path);
typedef void (*plug_frame_t)(Plug* plug);
typedef void (*callback_t)(void* bufferData, unsigned int frames);

#endif  // PLUG_H_
