# README

## On Windows

Install [msys2](https://www.msys2.org)

Install build environment and conan in MSYS2:
```
pacman -S --needed base-devel mingw-w64-x86_64-toolchain
pacman -S mingw-w64-x86_64-python-pip
pacman -D mingw-w64-x86_64-cmake
pip install conan
```

Create or update conan profile: 

```
conan profile new default --force --detect  # Generates default profile detecting GCC and sets old ABI
conan profile update settings.compiler.libcxx=libstdc++11 default  # Sets libcxx to C++11 ABI
````

### Out of source build instructions

Change to project folder and build

```
cd <path/to/workfolder>
conan install ./new-image-resizer -if build-win --build=missing
conan build ./new-image-resizer -bf build-win
````

## For Mac

### Out of source build instructions

```
cd <path/to/workfolder>
conan install ./new-image-resizer -if build-mac --build=missing
conan build ./new-image-resizer -bf build-mac
````

This command will generate a file 'conanbuildinfo.xcconfig' in the current folder.
Within Xcode create a new project for a CLI-Tool.
Remove demo code.
Right click on project and add files 
- 'conanbuildinfo.xcconfig'
- folder 'src'
- any other file/folder that you want to add

In build settings of target exclude architecture 'arm64' or 'x86_64'
In project info choose 'conanbuildinfo' for debug and release configuration

