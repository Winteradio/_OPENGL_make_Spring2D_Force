#include "GL/freeglut.h"
#include <windows.h>
#include <math.h>
#include <stdlib.h>
#include <iostream>
#include <list>
#include <string>

	//사용할 라이브러리를 지정해줍니다.
#pragma comment(lib, "freeglut.lib")
#pragma comment(lib, "glew32s.lib")

	using namespace std;

	// 점들의 좌표
	float P_X;
	float P_Y;

	// 점들의 초기, 나중 속도
	float V_X;
	float V_Y;

	// 중력 가속도
	float G = -2;

	// 스피링 원래 길이
	float L = 0.5;

	// 감쇠계수
	double T = 0.8;

	// 점들의 개수
	int num = 3;

	int neg = 1;

	float w = 1;

	float inf[3][4];

	float loc[2][2];

	GLuint Time = 10;

	/// 초기의 점들의 좌표와 속도를 무작위로 입력한다
	void Reset() {
		for (int i = 0; i < num; i++) {
			P_X = rand() % 1 - 1;
			P_Y = rand() % 1 - 1;
			V_X = rand() % 3 - 3;
			V_Y = rand() % 3 - 3;
			inf[i][0] = P_X;
			inf[i][1] = P_Y;
			inf[i][2] = V_X;
			inf[i][3] = V_Y;
		}
	}

	float move(float x1, float y1, float x2, float y2) {
		return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
	}


	/// 선과 점들을 그리는 Display 함수이다.
	void display() {
		glClear(GL_COLOR_BUFFER_BIT);

		glPointSize(10);
		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_POINTS);
		{
			for (int i = 0; i < num; i++) {
				glVertex2f(inf[i][0], inf[i][1]);
			}
		}
		glEnd();
		glColor3f(1.0, 1.0, 1.0);
		glLineWidth(2.0);
		glBegin(GL_LINES);
		{for (int i = 0; i < num; i++) {
			glVertex2f(inf[i][0], inf[i][1]);
			if (i < num - 1) {
				glVertex2f(inf[i + 1][0], inf[i + 1][1]);
			}
			else if (i == num - 1) {
				glVertex2f(inf[0][0], inf[0][1]);
			}
		}}
		glEnd();
		glFlush();
	}

	/// 주어진 Time의 간격에 따라 점의 위치를 조절하며 스프링의 이벤트를 줄과 점에 입력한다
	void TimerFunc(int value) {
		for (int i = 0; i < num; i++) {
			for (int j = 1; j < num; j++) {
				if (i + j == num) {
					j = -i;
				}
				if (move(inf[i][0], inf[i][1], inf[i + j][0], inf[i + j][1]) > L) {
					neg = -neg;
				}
				else if (move(inf[i][0], inf[i][1], inf[i + j][0], inf[i + j][1]) == L) {
					neg = 0;
				}
				inf[i][0] = inf[i][0] + neg * inf[i][2] / w * cos(Time/1000 * w) * cos(atan2(inf[i][0]-inf[i+j][0], inf[i][1] - inf [i+j][1]) / 180);
				inf[i][1] = inf[i][1] + neg * inf[i][3] / w * cos(Time/1000 * w) * sin(atan2(inf[i][0] - inf[i + j][0], inf[i][1] - inf[i + j][1]) / 180) + 1/2 * G * pow(Time/1000,2);

			}
		}
		glutPostRedisplay();
		glutTimerFunc(Time, TimerFunc, 1);
	}
	/// 뒷배경색 함수
	void backdisplay() {
		glClearColor(0.0, 0.0, 0.0, 0); /// 배경색
	}

	/// 총괄적으로 Display와 Time, Reset 함수 등을 진행시키는 Main 함수
	int main(int argc, char** argv) {
		Reset();
		glutInit(&argc, argv);
		glutInitWindowSize(300, 500);
		glutInitWindowPosition(120, 50);
		glutCreateWindow("OpenGL");
		glutDisplayFunc(display);
		backdisplay();
		glutTimerFunc(0, TimerFunc, 1);
		glutMainLoop();
	}