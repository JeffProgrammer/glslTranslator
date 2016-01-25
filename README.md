# glslTranslator
A GLSL 120 to GLSL 330 translation layer that lets you write GLSL 120 while targeting both a legacy and a core profile. This is so that you can support older hardware and newer hardware.

The inspiration for writing this is that in my own proprietary game and game engine, I wish to support Mac OSX 10.6.8 still in 2016 which targets OpenGL 2.1, all of the 3.0 extensions, and GLSL 120. Because of this, apple did not create a core profile in that version of OSX, but the hardware and the OS most certainly supports a majority of OpenGL 3.X. I didn't want to write shaders twice, so I decided to use the lowest common denominator and just translate the shaders on the fly.

## Build

Tested on Xcode 7.0.1 using Apple LLVM 7.0 C++ compiler.

## License

3 Clause BSD.