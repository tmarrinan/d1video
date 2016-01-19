D1Video
=======

DXT1-based Converter and Player for Ultra High Resolution Videos (4K - 16K)


## Installation##

### Mac OS X ###

#### Dependencies: ####

* SDL2
* OpenGL
* libjpeg
* libpng

#### Build ####

* Open Terminal
    * `cd <d1video_directory>`
    * `make`

#### Run ####

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
* Controls:
    * `space`: play/pause
    * `left-arrow`: rewind to beginning
    * `ctrl + f`: full screen (Windows / Linux)
    * `cmd + f`: full screen (Max OS X)
    * `esc`: exit full screen
