# ASCII-Video-Player
I couldn't find ASCII Video Converter and player, so I made my own. 


## Some Information
- I made this because i couldn't find anything like that anywhere. So here you are :3
- It can be used only in linux terminal.
- Also, I used [this repository](https://github.com/TheZoraiz/ascii-image-converter) to convert images to ASCII, as it difficult to code this for me. It has Apache 2.0 Licence, so I thought that I can use it. 


## Dependencies

- [git](https://github.com/git/git) - to clone repository
- g++ (part of [gcc (GNU Compiler Collection)](https://github.com/gcc-mirror/gcc)) - to compile code
- [mpv](https://github.com/mpv-player/mpv) - to play the audio
- [ascii-image-converter](https://github.com/TheZoraiz/ascii-image-converter) - to convert from photo-like frames to ascii-like frames
- [FFmpeg](https://github.com/FFmpeg/FFmpeg) - to split video into frames
- [unzip](https://github.com/LuaDist/unzip) - to unzip the release archive.

Everything can be installed with github, but it is more preferable to use [apt](https://github.com/Debian/apt).

## Installation 

### Through cloning

1. Cloning repository:
```bash
git clone https://github.com/Kolya080808/ASCII-Video-Player.git; cd ASCII-Video-Player
```
2. Installing dependencies, converting video into frames and compiling code:
```bash
chmod +x make-ascii.sh; ./make-ascii.sh <video filename>
```
3. Running the video:
```bash
./run <video filename> <delay (1/fps, for me it is 1/30 = 0.03333...>
```

### Through releases page (it is only a bit faster):

1. Download the zip
2. Unzip it:
```bash
unzip release.zip
```
3. Installing dependencies, converting video into frames:
```bash
chmod +x make-ascii.sh; ./make-ascii.sh <video filename>
```
4. Running the video:
```bash
./run <video filename> <delay (1/fps, for me it is 1/30 = 0.03333...>
```


That's all. Thanks for visiting this repository! 
