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

Create conan profile: 

conan profile new default --force --detect  # Generates default profile detecting GCC and sets old ABI
conan profile update settings.compiler.libcxx=libstdc++11 default  # Sets libcxx to C++11 ABI

cd <path/to/source>
mkdir build
cd build
conan install .. --build=missing
conan build ..