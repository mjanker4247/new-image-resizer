#define VERSION 4

//Debug
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
#include <iostream>
#include <fstream>
#include <cstdint>
#include <vector>
#include <array>
#include <thread>
#include <future>
#include <chrono>
#include <algorithm> 

// Boost
#include <boost/regex.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
//#include <boost/timer/timer.hpp>
namespace fs = boost::filesystem;
//namespace po = boost::program_options;


#include "ProgressBar.hpp"
using namespace std;


// Declare colors
const unsigned char
	white[]  = { 255, 255, 255 },
	black[] = { 0, 0, 0 },
	red[] = { 255, 0, 0 },
	yellow[] = { 200, 155, 0 },
	green[] = { 0, 255, 0 },
	purple[] = { 175, 32, 186 },
	blue[] = { 0, 0, 255 },
	grey[] = { 127, 127, 127 };

enum IMAGE_TYPE {
	GIF = 0, PNG, BMP, JPG, JPEG, TIFF, TGA,
	LAST // used for looping
};

// Declare variables
int newimage_width = 800;
int newimage_height = 600;
const float coveragePercent = 2.0;
const float tolerance = 0.1;
const int backgroundColor = 255;
const int posX = 0;
const int posY = 0;
const char * orderNumber;
int textboxWidth;
int textboxHeight;
int textboxSize;
int resizedImageSize;
int textSize = 0;
string strOrderNumber;
int width;
// Define number of steps to be reported in progress
int steps = 5;
bool isFolder = false;
bool isFile = false;

char resizedAppendix[] = "_resized";
char destDir[] = "Originale";
string orderNumberFile = "overlay.txt";
bool useOverlay = false;
CImg<unsigned char> originalImage;
CImg<unsigned char> resizedImage;
CImg<unsigned char> textbackground;
CImg<unsigned char> textbox;