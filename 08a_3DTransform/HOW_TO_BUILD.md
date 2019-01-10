## Windows

Note:  There is also a Visual Studio 2017 Solution available.

### Using Windows Subsystem for Linux (WSL)

- ensure that you have GLEW and GLUT installed

$ sudo apt install libglew-dev libglut-dev

- build the project:

$ make -f Makefile.Unix

- run the application:

$ ./main

### Using Visual Studio Command Prompt

- GLEW and GLUT are included in the 'include' and 'lib' subfolders

> nmake /F Nmakefile.Windows

- run the application:

> main

## MacOS

Disclaimer:  The recommended platform is Windows.  Using any other platform may mean that technical support is not available.  I've taken a few hours to make it as seamless as possible on other platforms, but you are using it at your own risk.

- ensure that you have GLEW installed (GLUT is installed by default)

e.g. using Homebrew:

$ brew install glew

- build the project:

$ make -f Makefile.MacOS

- run the application:

$ ./main

## Linux or other UNIX-based operating system

Note:  Driver support in Linux isn't the best.  Feel free to try it out, but if you are getting poor performance, it may be because some of the functionality is happening in software.

Disclaimer:  The recommended platform is Windows.  Using any other platform may mean that technical support is not available.  I've taken a few hours to make it as seamless as possible on other platforms, but you are using it at your own risk.

- ensure that you have GLEW and GLUT installed

e.g. on Ubuntu:
$ sudo apt install libglew-dev libglut-dev

- build the project:

$ make -f Makefile.Unix

- run the application:

$ ./main
