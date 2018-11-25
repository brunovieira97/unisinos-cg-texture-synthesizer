# Texture Synthesizer

Texture synthesizer written in C++. This application will create a new texture image file based on a raw pre-synthesized file.

## Parameters

1. Path to the original texture image file
2. Path to the pre-synthesized image, which will guide the final result
3. Result file path + name

## Build and Run
MinGW's G++ specific libraries were compiled and used on this project. In order to build the runnable .exe, use the following command:
	
	g++ -Wall ./Source/*.cpp -I. -g -lglew32 -lglfw3 -lsoil2 -lopengl32 -lglu32 -lgdi32

## Made with
* C++
* SOIL

## Development Environment
* [MinGW](http://mingw.org/)
* [VS Code](https://code.visualstudio.com/)

## Authors
* [brunovieira97](https://www.github.com/brunovieira97) - Bruno Vieira
* [christianmlima](https://www.github.com/christianmlima) - Christian Lima