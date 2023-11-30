#include <assert.h>
#include <complex.h>
#include <math.h>
#include <memory.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

#include <raylib.h>

#include <dlfcn.h>  // POSIX Dependency

#include "plug.h"

const char* libplug_file_name = "libplug.so";
void* libplug = NULL;
plug_init_t plug_init = NULL;
plug_frame_t plug_update = NULL;
Plug plug = { 0 };

bool reload_libplug(void) {
  if (libplug != NULL) {
    dlclose(libplug);
  }

  libplug = dlopen(libplug_file_name, RTLD_NOW);
  if (libplug == NULL) {
    fprintf(stderr, "ERROR: could not load %s: %s\n", libplug_file_name, dlerror());
    return false;
  }

  plug_init = dlsym(libplug, "plug_init");
  if (plug_init == NULL) {
    fprintf(stderr, "ERROR: could not find plug_init in %s: %s\n", libplug_file_name, dlerror());
    return false;
  }

  plug_update = dlsym(libplug, "plug_update");
  if (plug_update == NULL) {
    fprintf(stderr, "ERROR: could not find plug_update in %s: %s\n", libplug_file_name, dlerror());
    return false;
  }

  return true;
}

int main(int argc, char* argv[]) {
  if (!reload_libplug()) {
    return 1;
  }

  if (argc != 2) {
    fprintf(stderr, "Usage: %s <path_to_mp3_file>\n", argv[0]);
    return 1;
  }

  InitWindow(800, 600, "MuzViz");
  SetTargetFPS(60);

  InitAudioDevice();

  plug_init(&plug, argv[1]);

  while (!WindowShouldClose()) {
    if (IsKeyPressed(KEY_R)) {
      if (!reload_libplug()) {
        return 1;
      }
    }
    plug_update(&plug);
  }

  return 0;
}
