# Simple9SliceScale
This is a simple implementation of [9-slice scaling](https://en.wikipedia.org/wiki/9-slice_scaling) using OpenGL. The rendered textures can be used as background images for 2D scalable rectangular shapes.

Feel free to modify and use it in your project.

## Build
Use [CMake](https://cmake.org/download/) to generate project / build files. All dependencies are handled in the *CMakeLists.txt* files.
Run the following command in the project directory (where the CMakeLists.txt is located):
```bash
cmake -B build
```
### Build the Visual Studio solution on Windows
Inside the *build* directory, open the *Simple9SliceScale.sln* solution and build the *INSTALL* project in *Release* mode. The output executable should be placed in the *build/out/bin* directory. Double click on the *9SliceScale.exe* to run the example

### Build and run the project on Linux
Navigate to the *build* directory and run the following command:
```bash
make install
```
When the build finishes, navigate to the *build/out/bin* folder and execute the following commands to run the example:
```bash
export LD_LIBRARY_PATH=../lib/
./9SliceScale
```
(Tested on Ubuntu 20.04)

## ImGui example window:
![Example](example.gif)
