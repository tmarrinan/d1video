D1Video
=======

DXT1-based Converter and Player for Ultra High Resolution Videos (4K - 16K)

* *max performance test: 15360x4320 at 24 fps*


## Installation##

#### Dependencies: ####

* MS Visual Studio 2013 or higher (Windows)
* FreeGLUT (Windows / Linux) / GLUT (Mac OS X - built in)
* FreeType
* libjpeg
* libpng

### Linux / Mac OS X ###

#### Build ####

* Open Terminal
    * `cd <d1video_directory>`
    * `make`

### Windows ###

* Open MS Visual Studio

## Run ##

##### D1V Converter (convert a stack of frames to d1v video) #####

* Open Terminal
    * `cd <d1video_directory>`
    * `./bin/d1vconverter [options]`
* Options:
    * `-i, --input <DIRECTORY>` directory with image set sequence
    * `-o, --output <FILE>` file to save output D1V video [Default: output.d1v]
    * `-r, --framerate <INTEGER>` frames per second to play back video [Default: 24]

##### D1V Player (play a d1v video) #####

* Open Terminal
    * `cd <d1video_directory>`
    * `./bin/d1vplayer [options]`
* Options:
    * `-i, --input <FILE>` dxt1 compressed video to show
    * `-n, --no-gui` open player without graphic ui
* Controls:
    * `space`: play/pause
    * `left-arrow`: rewind to beginning
    * `l`: toggle loop
    * `ctrl + f`: full screen (Windows / Linux)
    * `cmd + f`: full screen (Max OS X)
    * `esc`: exit full screen
