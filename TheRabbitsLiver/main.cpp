// -----------------------------------
// main.cpp
// 2020. 11. 11
// Code by derisan (derisan@naver.com)
// -----------------------------------

#include <iostream>

#include <GL/glew.h>
#include <GL/freeglut.h>

#include "gfw.h"

void DisplayFunc();
void ReshapeFunc(int w, int h);
void KeyboardFunc(unsigned char i, int x, int y);
void TimerFunc(int value);
void Shutdown();

Gfw gfw;

int main(int argc, char** argv)
{
	if (!gfw.Init(&argc, argv, 1600, 900))
	{
		std::cout << "Failed to initialize gfw" << std::endl;
		return -1;
	}

	glutDisplayFunc(DisplayFunc);
	glutReshapeFunc(ReshapeFunc);
	glutKeyboardFunc(KeyboardFunc);
	glutTimerFunc(16, TimerFunc, 1);

	glutMainLoop();

	return 0;
}

void DisplayFunc()
{

}

void ReshapeFunc(int w, int h)
{
	//glViewport(0, 0, w, h);
}

unsigned char key;
void KeyboardFunc(unsigned char i, int x, int y)
{
	key = i;
}

void TimerFunc(int value)
{
	glutTimerFunc(16, TimerFunc, 1);
	gfw.Run(key);
	if (gfw.GetShouldClose())
		Shutdown();
	key = NULL;
}

void Shutdown()
{
	gfw.Shutdown();
	glutLeaveMainLoop();
}