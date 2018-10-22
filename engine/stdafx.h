#ifndef __STDAFX_H__
#define __STDAFX_H__

#define NULL 0

#define ASSERT(expr) if(!(expr)) *(int*)0 = 0;
#include "GL/glew.h"

#include <SDL.h>
#include <SDL_opengl.h>
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "SDL_mixer.h"
#include "SDL_draw.h"
//#include <GL/gl.h>
//#include <GL/glu.h>

#include <stddef.h>
#include <stdio.h>
#include <string>
#include <list>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>

static inline void fatalError(std::string info)
{
	std::cout << info << std::endl;
	std::cout << "Enter Any Key To Exit" << std::endl;
	int tmp;
	std::cin >> tmp;
	SDL_Quit();
	exit(1);
}


#endif // !__STDAFX_H__
