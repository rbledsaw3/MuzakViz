# MuzakViz

A music visualizer written in C using raylib based on Tsoding's streams.

## Dependencies

* Raylib (I built mine as a shared library; also be sure to build with flac support turned on if using .flac)
* GLFW (just use the default that comes with raylib; I had issues when linking to the latest GLFW release)

## Usage

```
./muzakviz path_to_music_file.mp3
```

Supports .mp3, .wav, .ogg, .xm, .mod, .flac (if raylib was built with .flac support)

## TODO

* Seems like there's some frame weirdness that needs investigating
* Playlist support

## Acknowledgements

* [Tsoding on YouTube](https://youtube.com/@TsodingDaily)
* [Tsoding on Twitch](https://twitch.tv/tsoding)
