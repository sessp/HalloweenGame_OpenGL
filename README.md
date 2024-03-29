# Halloween themed Game
Halloween themed OpenGL program using C++. 

Program has the following features:
- A scene with multiple models including a ghosdt, grave, doors, skeleton, sign, house, fence, tree and oil lamp.
- Ability to zoom in and out using '1' and '3'.
- Ability to change between orthographic and perspective projection using 'P'.
- Ability to move, jump and interact with the world. 'WASD', 'SPACE' and 'F' respectively. Also a sprint feature with the 'SHIFT' key.
- Ability to rotate up and down with 'E' and 'Q'.
- A full bright feature, when 'O' is pressed. 
- Multiple animations for some of the models.
- An oil lamp, with light properties. The lighting in the scene adapts to the lamp. 
- Ability to increase brightness of lamp using 'K' and decrease using 'L'. 


## Compiling and Running
Libraries included are GLFW, GLAD, GLM and ASSIMP. Program was tested and developed on Linux Ubuntu. 
Code is located in 
```
OpenGL->src->3.openGL_tutorial->sample_2->sample2.cpp
```
Compiling:
1. From "OpenGL" Go into "build".
2. run ```cmake```
3. run ```make```
Running:
1. In "OpenGL" go to "Build" and then "bin" then "3.openGL_tutorial" and then just execute the program.  All the executables 
are located in the bin folder. 

# References
Note this program was developed with help using [https://learnopengl.com](https://learnopengl.com). 
All of the code I created is located in
```
OpenGL->src->3.openGL_tutorial->sample_2->sample2.cpp
```
Other resources I created can be found in the resources folder and shader folders. 
Everything else can be assumed to be either resources provided from learnopengl or resources provided to us by Curtin University, and I did not produce them. 

## Windows building
All relevant libraries are found in /libs and all DLLs found in /dlls (pre-)compiled for Windows. 
The CMake script knows where to find the libraries so just run CMake script and generate project of choice.
Note that you still have to manually copy the required .DLL files from the /dlls folder to your binary folder for the binaries to run.

Keep in mind the supplied libraries were generated with a specific compiler version which may or may not work on your system (generating a large batch of link errors). In that case it's advised to build the libraries yourself from the source.

## Linux building
First make sure you have CMake, Git, and GCC by typing as root (sudo) `apt-get install g++ cmake git` and then get the required packages:
Using root (sudo) and type `apt-get install libsoil-dev libglm-dev libassimp-dev libglew-dev libglfw3-dev libxinerama-dev libxcursor-dev  libxi-dev` .
Next, run CMake (preferably CMake-gui). The source directory is LearnOpenGL and specify the build directory as LearnOpenGL/build. Creating the build directory within LearnOpenGL is important for linking to the resource files (it also will be ignored by Git). Hit configure and specify your compiler files (Unix Makefiles are recommended), resolve any missing directories or libraries, and then hit generate. Navigate to the build directory (`cd LearnOpenGL/build`) and type `make` in the terminal. This should generate the executables in the respective chapter folders.

Note that CodeBlocks or other IDEs may have issues running the programs due to problems finding the shader and resource files, however it should still be able to generate the exectuables. To work around this problem it is possible to set an environment variable to tell the tutorials where the resource files can be found. The environment variable is named LOGL_ROOT_PATH and may be set to the path to the root of the LearnOpenGL directory tree. For example:

    `export LOGL_ROOT_PATH=/home/user/tutorials/LearnOpenGL`

Running `ls $LOGL_ROOT_PATH` should list, among other things, this README file and the resources direcory.

## Mac OS X building
Building on Mac OS X is fairly simple (thanks [@hyperknot](https://github.com/hyperknot)):
```
brew install cmake assimp glm glfw
mkdir build
cd build
cmake ../.
make -j8
```

## Glitter
Polytonic created a project called [Glitter](https://github.com/Polytonic/Glitter) that is a dead-simple boilerplate for OpenGL. 
Everything you need to run a single LearnOpenGL Project (including all libraries) and just that; nothing more. 
Perfect if you want to follow along with the tutorials, without the hassle of having to manually compile and link all third party libraries!
