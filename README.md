# README

## On Windows

Install conan. See https://www.conan.io

Create conan profile: 

conan profile new default --detect  # Generates default profile detecting GCC and sets old ABI
conan profile update settings.compiler.libcxx=libstdc++11 default  # Sets libcxx to C++11 ABI

cd <path/to/source>
mkdir build
cd build
conan install .. --build=missing
conan build ..