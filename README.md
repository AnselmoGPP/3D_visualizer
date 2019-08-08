# Visualizer class

This is a library for 3D visualization using OpenGL. It contains a class called "visualizerClass" that takes care of everything you need for drawing points, lines and cubes. After building using the provided CMakeLists.txt, this library compiles in Ubuntu (make) and Windows (MVS) .

External libraries used:

- GLEW 2.1.0
- GLFW 3.3
- GLM 0.9.9.5
- ImGui 1.72b

Things to take into account:

- Folder hans_visual/CMakeLists includes CMakeLists templates for linking your applications using this library in Ubuntu (make) or Windows (MVS). Differences in the CMakeLists involve different directories for the library files (.a, .lib) and the linking of additional libraries in Ubuntu.

- The folder called Example contains a project that serves as an example of how to use this library in your code.

- You can specify in your app's CMakeLists which files to use for Debug or Release modes (you have to had compiled the visualizer in Debug or Release). Then, use MVS or your Ubuntu compiler to compile in Debug or Release. Also, use your compiler to compile for a certain architecture (x86, x64).

- More: https://sciencesoftcode.wordpress.com/2019/04/04/building-on-ubuntu/
