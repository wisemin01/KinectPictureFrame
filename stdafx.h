#pragma once

using namespace std;

#pragma comment(lib, "kinect20.lib")
#pragma comment(lib, "kernel32.lib")

/*
#pragma comment(lib, "user32.lib")
gdi32.lib
winspool.lib
comdlg32.lib
advapi32.lib
shell32.lib
ole32.lib
oleaut32.lib
uuid.lib
odbc32.lib
odbccp32.lib
*/

// Windows Header Files
#include <Shlobj.h>

// Kinect Header files
#include <Kinect.h>

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <list>
#include <map>


const int WINSIZEX		 = 1920;
const int WINSIZEY		 = 1080;

const int DEPTH_WIDTH	 = 512;
const int DEPTH_HEIGHT	 = 424;

#define g_device	DXUTGetD3D9Device()

#include "ImageManager.h"
#include "InputManager.h"
#include "TextManager.h"