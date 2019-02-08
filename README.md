# sgl

Cross platform 2D software rendering graphics library, inspired by [graphics.h](https://web.stanford.edu/class/archive/cs/cs106b/cs106b.1126/materials/cppdoc/graphics.html), [minifb](https://github.com/emoon/minifb), [SDL 1.2](https://www.libsdl.org/) and [QuickCG](http://lodev.org/cgtutor/).

Designed to be a drop-in and use with little hassle sort of deal. I don't know if anyone will really find a use for this besides me. But it's very fun to work on. It's not explicitly a game development library, just a rendering library. It's most likely less efficient, slower, more buggy and has less features, support, compatibility and portability than SDL - Just so you know, if you didn't already guess.


## Features

- Windows, OS X and Linux (X11)
- Keyboard and mouse events
- Primitive shapes
- BMP loading (Uncompressed 8, 24 & 32 bpp) (1 & 4 bpp, RLE compression and OS/2 not supported yet)
- Save surfaces to BMP file
- Text rendering (adapted from [dhepper/font8x8](https://github.com/dhepper/font8x8))
- Joystick handling (adapted from [ThemsAllTook/libstem_gamepad](https://github.com/ThemsAllTook/libstem_gamepad))
- GIF loading (adapted from [hidefromkgb/gif_load](https://github.com/hidefromkgb/gif_load)) and saving (adapted from [lecram/gifenc](https://github.com/lecram/gifenc))
- Message box & dialog support (adapted from [AndrewBelt/osdialog](https://github.com/AndrewBelt/osdialog))
- Optional OpenGL & Metal backends (Just for rendering to screen)
- Optional extras (AA primitives, BDF font rendering, Freetype rendering, image loader (adapted from  [stb_image](https://github.com/nothings/stb)))

See below screenshots for TODO list and the examples folder for some idea of how to use. ___Still a WIP, API subject to change a lot___.

OS X will be the primary platform for all stuff, then after that is working to a level I like I will finish the windows port and after that the linux port. It's too much effort changing something on OS X then doing it on other platforms all the time. __At the moment Windows & Linux won't build__.


## Building

No external libraries are used unless you want to use the alternate backends (enable them by defining ```GRAPHICS_ENABLE_(OPENGL/METAL)```). I don't know how to make CMake files yet so backends will have to be defined manually. To enable AA primitives define ```GRAPHICS_ENABLE_AA```, for BDF rendering ```GRAPHICS_ENABLE_BDF``` and for image loading using stb_image ```GRAPHICS_ENABLE_STB_IMAGE```.

On OS X you'll have to link Cocoa framework ```-framework Cocoa``` and include the ```-x objective-c -fno-objc-arc``` flags. If you're using OpenGL backend ```-framework OpenGL``` and ```-framework Metal -framework MetalKit``` for Metal. Also, if you're using XCode you'll also have to disable modules in the build settings, becuase modules imports curses with stdio for some reason.

NOTE: On OS X 10.14, something changed and CoreGraphics isn't working like it used to. So if you're using 10.14 Metal is now the default rending backend. See above.

On Linux you'll have to link libX11 and libm ```-lX11 -lm```, and if you're using the OpenGL backend include ```-ld -lGL```. **NOTE**: X11 can't automatically strech stuff being rendered like ```StretchDIBits``` and ```CGContextDrawImage``` can - so resizing the window is disabled (_until I can find a solution_) unless you're using the OpenGL backend.

On Windows (Visual Studio) you'll have to add ```/utf-8``` to the command line options or unicode decoding won't work properly. I don't know why, but it doesn't.

Tested on:
- OS X 10.12, 10.13 & 10.14 (clang)
- Windows 7 x64 (MSVC)
-  Ubuntu 19 x86_64 (Linux 4.13.0-16) (Inside VM only) (clang)


## Screenshots

<p align="center">
  <img src="https://raw.githubusercontent.com/takeiteasy/graphics.h/master/screenshots/screenshot_osx.png">
</p>

<p align="center">
  <img src="https://raw.githubusercontent.com/takeiteasy/graphics.h/master/screenshots/screenshot_win.png">
</p>

<p align="center">
  <img src="https://raw.githubusercontent.com/takeiteasy/graphics.h/master/screenshots/screenshot_nix.png">
</p>


## TODO

### Important

- Cursor clipping for Linux [and OS X](https://stackoverflow.com/a/40922095)
- Sort out joysticks
  - General clean up
  - Remove redundant stuff
  - Make joystick_t a private struct in s ource
  - Fix crashes
  - Linux joysticks totally untested
  - Move outside of os specific code block
- Message box support (~OS X~/Windows/Linux)
- Multiple Windows (~OS X~/Windows/Linux)
- Window icon support (~OS X~/Windows/Linux)
- Cursor loading from surface (~OS X~/Windows/Linux)
- Set cursor position function (~OS X~/Windows/Linux)
- Make a CMake build file
- Port new stuff to Windows & Linux

### Less important

- Come back to surface transformation
- Different scaling functions
- Add fill option for ```ellipse_rotated()``` and other functions
- Add line width option to primitives
- 1 & 4 bpp, RLE compressed and OS/2 BMP support

### At some point

- Fix color pallete for GIF save
- Keyboard scancodes
- Make thread safe (surface locks)
- Vulkan/DirectX/~~Metal~~/SIXEL backends
- Wayland window code
- Documentation & comments
- libtcc interactive player (like [CToy](https://github.com/anael-seghezzi/CToy))
- More examples/tests
- More extras (~gif load/save~, ~FreeType fonts~, C++ OOP wrapper)
- Optional OpenCL for processing
- Pixel shader support
- Audio playback (wav files)
- Other language bindings (Either Wren/Lua/Ruby probably)

### Investigate

- Why CoreGraphics rendering is so slow
- Resizing for X11
- Getting CoreGraphcs working on OS X in 10.14 again


## License

```Copyright (c) 2013, George Watson
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
* Redistributions of source code must retain the above copyright
notice, this list of conditions and the following disclaimer.
* Redistributions in binary form must reproduce the above copyright
notice, this list of conditions and the following disclaimer in the
documentation and/or other materials provided with the distribution.
* Neither the name of the <organization> nor the
names of its contributors may be used to endorse or promote products
derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL RUSTY SHACKLEFORD BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.```
