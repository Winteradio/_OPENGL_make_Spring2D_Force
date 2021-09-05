#include "GL/freeglut.h"
#include <windows.h>
#include <math.h>
#include <cmath>
#include <cstdlib>
#include <stdlib.h>
#include <iostream>
#include <random>

//사용할 라이브러리를 지정해줍니다.
#pragma comment(lib, "freeglut.lib")
#pragma comment(lib, "glew32s.lib")

using namespace std;

int num = 4;

float particle[4][4][3];
/// <summary> 행렬 설명
	/// 첫번째 [4] : 점들의 개수(1번점[0], 2번점[1], 3번점[2], 4번점[3])
	/// 두번째 [4] : 특성의 개수(좌표값[0], 속도값[1], 힘값[2], 무게[3])
	///				ex) particle[3][0]~ 안에 들어 있는 값 : 4번째 점의 좌표값에 대한 배열
	/// 세번째 [3] : 축값들 (x축[0], y축[1], z축[2])
	/// </summary>

float T_Time = 0.0f;

class Physics{
public :
	float G = -15.0f; // 중력 상수
	float Ks = 2.0f; // 탄성계수
	float Kd = 2.0f; // 항력 계수
	float mass = 10.0f; // 점들의 무게
	float L = 0.5f; // 용수철 원래 길이
	int Time =10; // Time_step
	float time = 10.0f;

	float Dot(float V1[2], float V2[2]) {
		return V1[0] * V2[0] + V1[1] * V2[1];
	}

	float Distance(float V1[2]) {
		return (float)pow((pow(V1[0], 2) + pow(V1[1], 2)),0.5);
	}


	float G_Force() { // 중력
		return G;
	}

	float D_Force() { // 항력
		return -Kd * V;
	}

	void S_Force() { // 탄성력
		float X_Delta[2];
		float V_Delta[2];
		for (int i = 0; i < num-1; i++) {
			for (int j = i +1 ; j < num ; j++) {
				for (int t = 0; t <= 1; t++) {
					X_Delta[t] = particle[i][0][t] - particle[j][0][t];
					V_Delta[t] = particle[i][1][t] - particle[j][1][t];
				}
				for (int t = 0; t <= 1; t++) {
					particle[i][2][t] += -(Ks * (Distance(X_Delta) - L) + Kd * Dot(V_Delta, X_Delta) / Distance(X_Delta)) * X_Delta[t] / Distance(X_Delta);
					particle[j][2][t] = -particle[i][2][t];
				}
			}
		}
	}

	void N_Force(int i,int j) { // 
		particle[i][1][j] = -particle[i][1][j];
		particle[i][2][j] = -particle[i][2][j];
	}

	void E_Velocity() { // Euler Velocity
		for (int i = 0; i < num; i++) {
			for (int j = 0; j <= 1; j++){
				particle[i][1][j] += (time / 1000) * particle[i][2][j]/mass;
			}
		}
	}
};

class Reset {
public :
	void reset() {
		Physics P = Physics();
		for (int i = 0; i < num; i++) {
			for (int j = 0; j < 3; j++) {
				if (j == 1) {
					particle[i][2][j] = P.G;
				}
				particle[i][2][j] = 0;
			}
		}
	}

	void start() {
		for (int i = 0; i < num; i++) {
			for (int j = 0; j <= 1; j++) {
				for (int k = 0; k <= 1; k++) {
					particle[i][j][k] = ((float)(rand() % GLUT_WINDOW_WIDTH) - GLUT_WINDOW_WIDTH*0.5) / GLUT_WINDOW_WIDTH;
				}
			}
		}
	}
};

void Timer(int value) {
	Physics P = Physics();
	Reset R = Reset();
	for (int i = 0; i < num ; i++) {
		for (int j = 0; j <= 1; j++) {
			P.D_Force(particle[i][1][j]);
			P.S_Force();
			if (sqrt(pow(particle[i][0][j], 2)) > 0.8) {
				P.N_Force(i, j);
			}
			P.E_Velocity();
			particle[i][0][j] += particle[i][1][j] * P.time / 1000;
			R.reset();
		}
	}
	T_Time += P.time / 1000;
	glutPostRedisplay();
	glutTimerFunc(P.Time, Timer, 1);
}


void display() {
	glClear(GL_COLOR_BUFFER_BIT);

	glPointSize(5);
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_POINTS);
	{
		for (int i = 0; i < num; i++) {
			glVertex2f(particle[i][0][0], particle[i][0][1]);
		}
	}
	glEnd();
	glColor3f(1.0, 1.0, 1.0);
	glLineWidth(1.0);
	glBegin(GL_LINES);
	{for (int i = 0; i < num-1; i++) {
		for (int j = i + 1; j < num; j++) {
			glVertex2f(particle[i][0][0], particle[i][0][1]);
			glVertex2f(particle[j][0][0], particle[j][0][1]);
		}
	}}
	glEnd();
	glFlush();
}

int main(int argc, char** argv) {
	Reset R = Reset();
	R.start();
	glutInit(&argc, argv);
	glutInitWindowSize(300, 500);
	glutInitWindowPosition(120, 50);
	glutCreateWindow("OpenGL");
	glutDisplayFunc(display);
	glClearColor(0.0, 0.0, 0.0, 0);
	glutTimerFunc(0, Timer, 1);
	glutMainLoop();
}