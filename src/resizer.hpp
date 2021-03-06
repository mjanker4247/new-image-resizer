#define VERSION 4

// Debug
#define USEDEBUG 0
#ifdef USEDEBUG
#define Debug(x) std::cout << x << std::endl
#else
#define Debug(x)
#endif

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/ioctl.h>
#endif

#define cimg_display 0
#define cimg_use_jpeg
#define cimg_use_png
#include "CImg.h"
using namespace cimg_library;

#include <cstring>
#include <cstdio>
#include <cstdint>
#include <cstdint>
#include <vector>
#include <array>
#include <thread>
#include <future>
#include <chrono>
#include <algorithm>
#include <regex>
#include <iostream>
#include <fstream>
#include <filesystem>
namespace fs = std::filesystem;
#include <regex>

#include "ProgressBar.hpp"

using namespace std;

// Declare colors
const unsigned char white[] = {255, 255, 255}, black[] = {0, 0, 0},
                    red[] = {255, 0, 0}, yellow[] = {200, 155, 0},
                    green[] = {0, 255, 0}, purple[] = {175, 32, 186},
                    blue[] = {0, 0, 255}, grey[] = {127, 127, 127};

enum IMAGE_TYPE {
  GIF = 0,
  PNG,
  BMP,
  JPG,
  JPEG,
  TIFF,
  TGA,
  LAST // used for looping
};

// Declare variables
/*
 * This values can be stored in a config file
 */
int newimage_width = 800;
int newimage_height = 600;
const float coveragePercent = 2.0;
const float tolerance = 0.1;
// Black background color
const int backgroundColor = 255;
const int posX = 0;
const int posY = 0;
char resizedAppendix[] = "_resized";
char destDir[] = "Originale";
string orderNumberFile = "overlay.txt";

const char *orderNumber;
int textboxWidth;
int textboxHeight;
int textboxSize;
int resizedImageSize;
int textSize = 0;
string strOrderNumber;
int width;
std::vector<fs::path> files;

bool useOverlay = false;
CImg<unsigned char> textbackground;
CImg<unsigned char> textbox;

bool isValidImage(const string file);
string readOrderNumberFromFile(const string &file);
string readOrderNumberFromFile(std::filesystem::path filePath);
int high_low(int high, int low);
CImg<unsigned char> getTextbox(const char *text, int initialSize,
                               float coveredSize, float toBeCovered,
                               float tolerance);
void printVersion();
std::vector<fs::path> getFilesFromDirectory(char *path);
std::vector<fs::path> getAllPictures(std::vector<char *>);
std::vector<fs::path> getAllPictures(std::vector<fs::path>);
vector<string> readLinesFromFile(string file);
void resizeAndStore(fs::path file);
