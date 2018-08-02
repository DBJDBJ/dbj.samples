// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

// stdafx.cpp : source file that includes just the standard includes
// $safeprojectname$.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

/*

1. Create stdafx.h and stdafx.cpp in the root folder of the project.
2. Go to project properties -> precompiled headers. Change to "use".
3. Go to stdafx.cpp, right-click properties -> precompiled headers. Change to "create".
4. Go to project properties -> advanced; change "Force include files" to stdafx.h;%(ForcedIncludeFiles)

Don't change any CPP file; keep your header files as they are. Build as-is. 
This means even: #include  "../atdafx.h" , will compile OK
*/

#include "stdafx.h"

#ifdef DBJ_WIN
#pragma message ("\nWARNING: This is DBJ_WIN dbj++ build\n")
#endif