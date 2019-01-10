## Windows

Note:  There is also a Visual Studio 2017 Solution available.

### Using Windows Subsystem for Linux (WSL)

- ensure that you have GLEW and GLUT installed

```sh
$ sudo apt install libglew-dev libglut-dev
```

- build the project:

```sh
$ make -f Makefile.Unix
```

- run the application:

```sh
$ ./main
```

### Using Visual Studio Command Prompt

- GLEW and GLUT are included in the 'include' and 'lib' subfolders

```batch
> nmake /F Nmakefile.Windows
```

- run the application:

```batch
> main
```

## MacOS

Disclaimer:  The recommended platform is Windows.  Using any other platform may mean that technical support is not available.  I've taken a few hours to make it as seamless as possible on other platforms, but you are using it at your own risk.

- ensure that you have GLEW installed (GLUT is installed by default)

e.g. using Homebrew:

```sh
$ brew install glew
```

- build the project:

```sh
$ make -f Makefile.MacOS
```

- run the application:

```sh
$ ./main
```

## Linux or other UNIX-based operating system

Note:  Driver support in Linux isn't the best.  Feel free to try it out, but if you are getting poor performance, it may be because some of the functionality is happening in software.

Disclaimer:  The recommended platform is Windows.  Using any other platform may mean that technical support is not available.  I've taken a few hours to make it as seamless as possible on other platforms, but you are using it at your own risk.

- ensure that you have GLEW and GLUT installed

e.g. on Ubuntu:
```sh
$ sudo apt install libglew-dev libglut-dev
```

- build the project:

```sh
$ make -f Makefile.Unix
```

- run the application:

```sh
$ ./main
```
