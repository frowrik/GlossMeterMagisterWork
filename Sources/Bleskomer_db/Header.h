// include libs

#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "opengl32.lib")

#define	WinnAME "Blicomer"

#define	 IMGUI_DEFINE_MATH_OPERATORS
#define	 IMGUI_DEFINE_PLACEMENT_NEW


#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "glfw3.h"
#include "mainx.h"
#include "imgui_tabs.h"

#include "SerialCom.h"

// https://www.codeproject.com/Tips/349002/Select-a-USB-Serial-Device-via-its-VID-PID