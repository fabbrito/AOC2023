# Advent of Code 2023 - using C++

This is my solutions repo for Advent of Code 2023, all written in C++.

There are two included projects - one called _Utils_, and one called _Challenges_. [Premake](https://github.com/premake/premake-core) is used to generate project files.

_Utils_ builds into a static library and is meant to contain common code intended for use in multiple applications.
_Challenges_ builds into an executable and links the _Utils_ static library, as well as provides an include path to _Utils_'s code.

## Getting Started
1. Clone this repository
2. Open the `Scripts/` directory and run the appropriate `Setup` script to generate projects files (set to Visual Studio 2022 for Windows and gmake2 for Linux)

Note: no macOS setup script is currently provided.

## Included
- Premake binaries for Win/Mac/Linux (`v5.0-beta2`)

## License
- UNLICENSE for this repository (see `UNLICENSE.txt` for more details)
- Premake is licensed under BSD 3-Clause (see included LICENSE.txt file for more details)