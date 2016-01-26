# glslTranslator
A GLSL 120 to GLSL 330 translation layer that lets you write GLSL 120 while targeting both a legacy and a core profile. This is so that you can support older hardware and newer hardware.

The inspiration for writing this is that I wish to support Mac OSX 10.6.8 still in 2016 which targets OpenGL 2.1, all of the 3.0 extensions, and GLSL 120. Because of this, apple did not create a core profile in that version of OSX, but the hardware and the OS most certainly supports a majority of OpenGL 3.X. I didn't want to write shaders twice, so I decided to use the lowest common denominator and just translate the shaders on the fly.

## What's supported in the procedural translation?

* #version 120 -> #version 330 core
* attribute -> in (vertex shader)
* varying -> out (vertex shader)
* varying -> in (fragment shader)
* texture2D -> texture
* texture3D -> texture
* gl_FragColor -> Generated output variable

## Build

Run Cmake and build. A tester program is provided for the library.

Tested on the following tools:
- Xcode 7.0.1 using Apple LLVM 7.0 C++ compiler (x86_64)
- MSVC 2015 32-bit VS140

## License

3 Clause BSD.