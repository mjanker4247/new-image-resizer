//
//  main.cpp
//  magictool
//
//  Created by Marco Janker on 25/01/19.
//  Copyright © 2019 Marco Janker. All rights reserved.
//

#include "resizer.hpp"

std::vector<char *> getFilesFromPath(char* path)
{
    DIR *dir; struct dirent *diread;
    vector<char *> files;

    if ((dir = opendir(path)) != nullptr) {
        while ((diread = readdir(dir)) != nullptr) {
            files.push_back(diread->d_name);
        }
        closedir (dir);
    } else {
        perror ("opendir");
    }
    return files;
}

CImg<unsigned char> resizeKeepAspectRatio(CImg<unsigned char> srcImage, const float dstWidth, const float dstHeight) {
	float scaleHeight;
	float scaleWidth;
	try {
		/*
		 * The size of the final image is determined by the scaling factor, and there are two factors
		 */
		scaleHeight = dstHeight / (float)srcImage.height();
		scaleWidth = dstWidth / (float)srcImage.width();
	} catch (const char* msg) {
		cerr << msg << endl;
	}
	/* 
	 * Now, we have two scaling ratios, which one produces the smaller image? 
	 * The one that has the smallest scaling factor.
	 */
	float scale = std::min(scaleHeight, scaleWidth);

	return srcImage.resize((int)(srcImage.width() * scale), (int)(srcImage.height() * scale),-100,-100,6);
}

#ifdef BOOST_WINDOWS_API
std::ostream &MessageBox(std::ostream &s) {
    std::ostringstream *st = dynamic_cast<std::ostringstream *>(&s);
    if (NULL != st)
        ::MessageBox(NULL, st->str().c_str(), "", MB_OK);
    return s;
}

void showMessage(const string& s) {
    std::ostringstream stm;
    stm  << "Number of arguments: " << s << MessageBox;
}
#endif

int ValidImage(std::uint8_t* ImageBytes) {
    const static std::vector<std::uint8_t> GIFBytesOne = { 0x47, 0x49, 0x46, 0x38, 0x37, 0x61 };
    const static std::vector<std::uint8_t> GIFBytesTwo = { 0x47, 0x49, 0x46, 0x38, 0x39, 0x61 };
    const static std::vector<std::uint8_t> PNGBytes = { 0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A };
    const static std::vector<std::uint8_t> BMPBytes = { 0x42, 0x4D };
    const static std::vector<std::uint8_t> JPGBytes = { 0xFF, 0xD8, 0xFF };
    const static std::vector<std::uint8_t> JPEGBytes = { 0x00, 0x00, 0x00, 0x0C, 0x6A, 0x50, 0x20, 0x20 };
    const static std::vector<std::uint8_t> TIFFMonoChrome = { 0x0C, 0xED };
    const static std::vector<std::uint8_t> TIFFOne = { 0x49, 0x20, 0x49 };
    const static std::vector<std::uint8_t> TIFFTwo = { 0x49, 0x49, 0x2A, 0x00 };
    const static std::vector<std::uint8_t> TIFFThree = { 0x4D, 0x4D, 0x00, 0x2A };
    const static std::vector<std::uint8_t> TIFFFour = { 0x4D, 0x4D, 0x00, 0x2B };
    const static std::vector<std::uint8_t> CompressedTGA = {0x0, 0x0, 0xA, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
    const static std::vector<std::uint8_t> DeCompressedTGA = {0x0, 0x0, 0x2, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};

    const static std::array<std::vector<std::uint8_t>, 13> All = {
        GIFBytesOne, GIFBytesTwo, PNGBytes, BMPBytes,
        JPGBytes, JPEGBytes, TIFFMonoChrome, TIFFOne,
        TIFFTwo, TIFFThree, TIFFFour, CompressedTGA,
        DeCompressedTGA
    };

    int I = 0;
    for (const auto& it : All)
    {
        if (std::equal(it.begin(), it.end(), ImageBytes))
            return I;
        ++I;
    }
    return -1;
}

bool isValidImage (const std::string filename) {

    std::fstream hFile(filename, std::ios::in | std::ios::binary);

    if (!hFile.is_open())
    {
        throw std::invalid_argument("File Not Found.");
    }

    std::uint8_t Header[18] = {0};
	try {
		hFile.read(reinterpret_cast<char*>(&Header), sizeof(Header));
		hFile.seekg(0, std::ios::beg);
	} catch (const char* msg) {
		cerr << msg << endl;
	}

    for (int count = 0; count < LAST; count++)
    {
        if ( ValidImage(Header) == (IMAGE_TYPE)count )
            return true;
    }
    return false;
}

string getOrderNumber(fs::path filePath) {
	try {
		//Check if a text document with name "overlay.txt" exists and use the containing number as order number
		filePath = filePath / orderNumberFile;
		if ( boost::filesystem::exists(filePath) ) {
			useOverlay = true;
			boost::filesystem::ifstream inFile(filePath);
			string s;
			std::getline(inFile,s); 
			return s;
		}

		// Extract order and position number from complete path to file
		static const std::regex e("(\\d{7}_\\d{3})");
		std::smatch what;
		if (std::regex_search(filePath.generic_string(), what, e)) {
			return what[1];
		}
	} catch (const char* msg) {
		cerr << msg << endl;
	}
    return NULL;
}

int high_low(int high,int low) {
    return (high - low) / 2;
}

float getTextSize (const char * text, int initialSize, float coveredSize, float toBeCovered, float tolerance) {
    int size = initialSize;
    float coveredArea = 0;
    int low = 0, high = 0;
    CImg<unsigned char> textbox;
	
	try {

		while ( (coveredArea < (toBeCovered - tolerance)) || (coveredArea > (toBeCovered + tolerance)) ) {
			// Create textbox
			textbox.draw_text(0,0,text,white,black,1,size);
			// get width and height of textbox
			textboxWidth = textbox.width();
			textboxHeight = textbox.height();
			textboxSize = textboxWidth * textboxHeight;
			coveredArea = static_cast<float>(textboxSize)/coveredSize*100;

			if (coveredArea > toBeCovered) {
				//printf("Lower than %u\n", size);
				high = size;
				size = high_low(high, low) + low;
			} else {
				//printf("Higher than %u\n", size);
				low = size;
				size += high_low(high, low);
			}
			// Empty textbox
			textbox.assign();
		}
	} catch (const char* msg) {
		cerr << msg << endl;
	}
    return size;
}
	
std::vector<fs::path> getFolderContent(std::string path) {
	
	fs::path mypath(path);
	fs::directory_iterator end_iter;

	std::vector<fs::path> result_set;

	if ( fs::exists(mypath) && fs::is_directory(mypath))
	{
		for( fs::directory_iterator dir_iter(mypath) ; dir_iter != end_iter ; ++dir_iter)
		{
			if (fs::is_regular_file(dir_iter->status()) )
			{
				result_set.push_back( *dir_iter );
			}
		}
	}
	return result_set;
}

void printVersion()
{
	std::cout <<  "Version: " << VERSION << std::endl;
}

int main(int argc, char * argv[]) {

	printVersion();
	
	if (argc < 2) {
		std::cout << "missing filename" << std::endl;
		exit(1);
	}

	// populate vector with content of folder
	std::vector<fs::path> dirListing = getFolderContent(argc>1? argv[1] : ".");

	for (std::vector<fs::path>::iterator it = dirListing.begin() ; it != dirListing.end(); ++it)
    	std::cout << ' ' << *it;
  	std::cout << '\n';

	#ifdef _WIN32
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
		width = csbi.srWindow.Right - csbi.srWindow.Left;
	#else
		struct winsize win;
		ioctl(0, TIOCGWINSZ, &win);
		width = win.ws_col;
	#endif
	
	unsigned int total = steps * (argc - 1);
	ProgressBar progressBar(total, width, '#', '-');
	
	// One argument could be a folder

	// Otherwise it is a picture?

	// Iterate over arguments
	for (++argv; *argv; ++argv) {
		
		char* originalFile = *argv;
		std::string strOriginalFile(originalFile);
		
		// skip argument if it is not an image
		if ( ! isValidImage(strOriginalFile) ) { cout << "Not an image!" << endl; continue; }
					
		// Create path objects
		// path to original file
		fs::path pathOriginalFile(originalFile);
		// path object to backup copy of original image file
		fs::path pathDestDirectory = pathOriginalFile.parent_path() / destDir;
		fs::path pathDestFile = pathOriginalFile.parent_path() / destDir / pathOriginalFile.filename();
		// path object to resized image file
		fs::path pathResizedFile = pathOriginalFile.parent_path() / (fs::path(pathOriginalFile.stem().string() + resizedAppendix ).string() + pathOriginalFile.extension().string());
		
		// read order number once
		if ( strOrderNumber.length() == 0 ) {
			strOrderNumber = getOrderNumber( pathOriginalFile.parent_path() );
			orderNumber = strOrderNumber.c_str();
			if (useOverlay) {
				cout << "Use text from \"overlay.txt\": " << orderNumber << endl;
			} else {
				cout << "Use text from filesystem: " << orderNumber << endl;
			}
		}
		
		// Step 1
		++progressBar;
		progressBar.display();
		
		if ((exists(pathOriginalFile)) ) {
			if (is_regular_file(pathOriginalFile)) {
				try {
					
					originalImage.load(originalFile);

					if (originalImage) {
						// Step 2
						++progressBar;
						progressBar.display();
						// resize original image
						std::future< CImg<unsigned char> > resultFromResize = std::async(std::launch::async, resizeKeepAspectRatio, originalImage,newimage_width,newimage_height);

						try {
							resizedImage = resultFromResize.get();
						} catch (const char* msg) {
							cerr << msg << endl;
						}

						// get size of resized image
						resizedImageSize = resizedImage.width() * resizedImage.height();
						// cout << "Calculating optimum text size." << endl;
						
						try {
							// Create textbox
							if ( textSize == 0 ) {
								// use a reasonable initial size value
								textSize = getTextSize(orderNumber, sqrt(resizedImageSize / 100 * coveragePercent) , static_cast<float>(resizedImageSize), coveragePercent, tolerance);
								textbox.draw_text(0, 0, orderNumber, white, black, 1, textSize);
								// create background image with size of textbox
								textbackground.assign(textbox.width() ,textbox.height() ,1,3,0);
								// fill it black
								textbackground.fill(backgroundColor);
								// draw text on background
								textbackground.draw_text(0,0,orderNumber,green,black,1,textSize);
							}
						} catch (const char* msg) {
							cerr << msg << endl;
						}
						// t.report();
						
						try {
							// draw text box in left top corner
							resizedImage.draw_image(posX,posY,textbackground);
						} catch (const char* msg) {
							cerr << msg << endl;
						}

						// Step 3
						++progressBar;
						progressBar.display();
						// save modified image
						//cout << "Saving resized image: " << filename << endl;
						try {
							resizedImage.save_jpeg( pathResizedFile.generic_string().c_str(), 90);
						} catch (const char* msg) {
							cerr << msg << endl;
						}
						
						// move original image to destination folder
						// Create directory
						if ( ! exists(pathDestDirectory)) {
							try {
								boost::filesystem::create_directory(pathDestDirectory);
							}
							catch (const fs::filesystem_error& ex)
							{
								std::fprintf(stderr,"Filesystem Error: %s",ex.what());
								system("pause");
							}
						}

						// Step 4
						// Move original image
						++progressBar;
						progressBar.display();
						//cout << "Moving original imgage to " << pathDestFile.parent_path() << endl;
						try {
							boost::filesystem::rename(pathOriginalFile,pathDestFile);
						}
						catch (const fs::filesystem_error& ex)
						{
							std::fprintf(stderr,"Filesystem Error: %s",ex.what());
							system("pause");
						}
					}
				} catch (CImgException) {
					std::cout << "Image " << pathOriginalFile << " not recognized" << std::endl;
					system("pause");
					originalImage.assign();
				}

			} else if (is_directory(pathOriginalFile)) {
				// is path p a directory?
				cout << pathOriginalFile << " is a directory.\n";
				system("pause");
			}
		}
		
		// Step 5
		++progressBar;
		progressBar.display();

	}
	std::this_thread::sleep_for(std::chrono::milliseconds(300));
	progressBar.done();
	
//    std::cout << "Text size: " << textSize << std::endl;
//    std::cout << "Width: " << textboxWidth << std::endl;
//    std::cout << "Height: " << textboxHeight << std::endl;
//    std::cout << "File size: " << file_size( originalFile ) << std::endl;
	// system("pause");
    return 0;
}
