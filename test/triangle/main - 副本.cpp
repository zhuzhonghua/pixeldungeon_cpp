#include "GL/glew.h"
#include "SDL.h"
#include <SDL_opengl.h>
#include "SDL_image.h"

#include <stdio.h>
#include <string>
#include <iostream>
#include <vector>

void fatalError(const std::string& info)
{
	std::cout << info << std::endl;
	std::cout << "Enter Any Key To Exit" << std::endl;
	int tmp;
	std::cin >> tmp;
	SDL_Quit();
	exit(1);
}

SDL_Window* window;
SDL_GLContext context;

void init()
{
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	window = SDL_CreateWindow("Triangle", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 450, 450, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
	context = SDL_GL_CreateContext(window);

	SDL_GL_SetSwapInterval(1);
	glewInit();

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	std::printf("*** OpenGL Version: %s ***\n", glGetString(GL_VERSION));

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glClearColor(0.0, 0.0, 1.0, 1.0);
}

const char* VERT_SHADER_SRC1 = R"(
attribute vec2 vertexPosition;
void main(){
	gl_Position.xy = vertexPosition;
	gl_Position.z = 0.0;
	gl_Position.w = 1.0;
}
)";

const char* VERT_SHADER_SRC2 = R"(
attribute vec2 vertexPosition;
attribute vec2 aUV;
varying vec2 vUV;
void main(){
	gl_Position.xy = vertexPosition;
	gl_Position.z = 0.0;
	gl_Position.w = 1.0;

	vUV = aUV;
}
)";

const char* FRAG_SHADER_SRC1 = R"(
void main(){
	gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
}
)";

const char* FRAG_SHADER_SRC2 = R"(
varying vec2 vUV;
uniform sampler2D uTex;

void main(){
	gl_FragColor = texture2D(uTex, vUV);
}
)";

void compileShader(GLuint shaderID, const char* src)
{
	glShaderSource(shaderID, 1, &src, NULL);
	glCompileShader(shaderID);

	GLint success = 0;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
	if (success == GL_FALSE)
	{
		GLint maxLen = 0;
		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &maxLen);

		std::vector<char> errLog(maxLen);
		glGetShaderInfoLog(shaderID, maxLen, &maxLen, &errLog[0]);

		glDeleteShader(shaderID);

		std::printf("%s\n", &errLog[0]);
		fatalError("compile shader fail");
	}
}

void link(GLuint programID)
{
	glLinkProgram(programID);

	GLint success = 0;
	glGetProgramiv(programID, GL_LINK_STATUS, (int*)&success);
	if (success == GL_FALSE)
	{
		GLint maxLen = 0;
		glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &maxLen);

		std::vector<char> infoLog(maxLen);
		glGetProgramInfoLog(programID, maxLen, &maxLen, &infoLog[0]);

		glDeleteProgram(programID);

		std::printf("%s\n", &infoLog[0]);
		fatalError("link error");
	}
}

GLuint loadShaders()
{
	GLuint programID = glCreateProgram();

	GLuint vertShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	compileShader(vertShaderID, VERT_SHADER_SRC2);
	glAttachShader(programID, vertShaderID);

	compileShader(fragShaderID, FRAG_SHADER_SRC2);
	glAttachShader(programID, fragShaderID);

	link(programID);

	return programID;
}

GLuint vertexBuffer;
GLuint programID;
GLuint texID;

void initTriangle2()
{
	programID = loadShaders();

	float vertexData[] = {
		0,0,
		-1,0,
		-1,-1
	};

	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);
}

void drawTriangle2()
{
	glUseProgram(programID);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glDrawArrays(GL_TRIANGLES, 0, 3);
}

struct Position{
	float x;
	float y;
};

struct UV{
	float u;
	float v;
};

struct Vertex{
	Position pos;
	UV uv;

	void setPos(float x, float y) { pos.x = x; pos.y = y; }
	void setUV(float u, float v){ uv.u = u; uv.v = v; }
};

void loadImg()
{
	SDL_Surface* img = IMG_Load("water4.png");
	int width = img->w;
	int height = img->h;

	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);
	if (img->format->BytesPerPixel == 4)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, img->pixels);
	}
	else if (img->format->BytesPerPixel == 3)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img->pixels);
	}
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	SDL_FreeSurface(img);
}

void initTriangle3()
{
	programID = loadShaders();

	Vertex vertexData[6];
	vertexData[0].setPos(0, 0);
	vertexData[0].setUV(1, 1);

	vertexData[1].setPos(-1, 0);
	vertexData[1].setUV(0, 1);

	vertexData[2].setPos(-1, -1);
	vertexData[2].setUV(0, 0);

	vertexData[3].setPos(-1, -1);
	vertexData[3].setUV(0, 0);

	vertexData[4].setPos(0, -1);
	vertexData[4].setUV(1, 0);

	vertexData[5].setPos(0, 0);
	vertexData[5].setUV(1, 1);

	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

	loadImg();
}

void drawTriangle3()
{
	glUseProgram(programID);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texID);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, pos));
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void drawTriangle1()
{
	glBegin(GL_TRIANGLES);
	glVertex2f(0, 0);
	glVertex2f(-1,0);
	glVertex2f(-1, -1);
	glEnd();
}

int main(int argc, char *argv[])
{
	init();
	initTriangle3();

	bool exit = false;

	while (exit == false)
	{
		SDL_Event e;
		while (SDL_PollEvent(&e) != 0)
		{
			switch (e.type)
			{
			case SDL_QUIT:
				exit = true;
				break;
			}
		}

		glClearDepth(1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//drawTriangle1();
		//drawTriangle2();
		drawTriangle3();

		SDL_GL_SwapWindow(window);
		SDL_Delay(200);
	}
	
	return 0;
}