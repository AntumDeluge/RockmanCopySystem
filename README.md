
# Rockman Copy System

by Benoît "BenoitRen" Renard

## Description

Rockman Copy System is a video game engine that aims to replicate the physics of the Rockman (Mega
Man in the West) games as accurately as possible. So far, Rockman, Rockman 2 and Rockman 3's physics
have been implemented.

It's written in C++ using [SDL 1.2 and SDL 2.0][SDL] and is available for Windows (95 and up) and
the PlayStation Portable. It's open source and subject to the terms of the
[Mozilla Public License, v. 2.0](https://mozilla.org/MPL/2.0/).

## Building

The [CMake Build System](https://cmake.org/) is used for configuring and compiling.

__Note:__ You will likely need the data files included in the Windows packages downloadable from the
[original author's homepage][home].

Execute CMake from source directory:

1. Open command prompt/terminal.
2. Change to source directory.
3. Execute `cmake .` to configure.
4. Execute `cmake --build .` to build.

Build in sub-directory:

1. Open command prompt/terminal.
2. Change to source directory.
3. Execute `cmake -B <dir>` to configure (replace "<dir>" with name of directory).
4. Execute `cmake --build <dir>` to build.

Build from external directory:

1. Open command prompt/terminal.
2. Change to desired build directory.
3. Execute `cmake -S <source-path> -B .` to configure (replace "<source-path>" with path to source
   code directory).
4. Execute `cmake --build .` to build.

## Links

- Homepage: https://www.benoitren.be/rockmancopysystem.html


[home]: https://www.benoitren.be/rockmancopysystem.html
[SDL]: https://libsdl.org/
