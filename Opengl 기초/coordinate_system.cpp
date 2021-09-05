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

int num = 7;

float particle[7][3][3];
/// <summary> 행렬 설명
	/// 첫번째 [4] : 점들의 개수(1번점[0], 2번점[1], 3번점[2], 4번점[3])
	/// 두번째 [4] : 특성의 개수(좌표값[0], 속도값[1], 힘값[2], 무게[3])
	///				ex) particle[3][0]~ 안에 들어 있는 값 : 4번째 점의 좌표값에 대한 배열
	/// 세번째 [3] : 축값들 (x축[0], y축[1], z축[2])
	/// </summary>

float T_Time = 0.0f;

class Physics{
public :
	float G = -0.0f; // 중력 상수
	float Ks = 100.0f; // 탄성계수
	float Kd = 20.0f; // 항력 계수
	float Kr = 0.5f; // 반발계수
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


	void G_Force(int i) { // 중력
		particle[i][2][1] += G*mass;
	}

	void D_Force(int i) { // 항력
		for (int j = 0; j <= 2; j++) {
			particle[i][2][j] += -Kd * particle[i][1][j];
		}
	}

	void S_Force(){// 탄성력
		float X_Delta[3];
		float V_Delta[3];
		for (int i = 0; i < num-1; i++) {
			for (int j = i+1; j < num ; j++) {
				for (int t = 0; t <= 2; t++) {
					X_Delta[t] = particle[i][0][t] - particle[j][0][t];
					V_Delta[t] = particle[i][1][t] - particle[j][1][t];
				}
				for (int t = 0; t <= 2; t++) {
					particle[i][2][t] += -(Ks * (Distance(X_Delta) - L) + Kd * Dot(V_Delta, X_Delta) / Distance(X_Delta)) * X_Delta[t] / Distance(X_Delta);
					particle[j][2][t] += -particle[i][2][t];
				}
			}
		}
	}

	void N_Force(int i,int j) { // 
		if (sqrt(pow(particle[i][0][j], 2)) >= 1.0) {
			particle[i][1][j] = -Kr*particle[i][1][j];
		}
	}

	void E_Velocity(int i) { // Euler Velocity
			for (int j = 0; j <= 2; j++){
				particle[i][1][j] += (time / 1000) * particle[i][2][j]/mass;
			}
	}
};

class MMath {
public :

};

class Reset {
public :
	void reset() {
		for (int i = 0; i < num; i++) {
			for (int j = 0; j < 3; j++) {
				particle[i][2][j] = 0;
			}
		}
	}

	void start() {
		particle[0][0][0] = 0.5;
		particle[0][0][1] = 0.5;
		for (int i = 1; i < num; i++) {
			for (int j = 0; j <= 1; j++) {
				particle[i][0][j] = ((float)(rand() % 100) - 50) / 100;
			}
		}
	}
};

Physics P = Physics();
Reset R = Reset();

void Timer(int value) {
	P.S_Force();
	for (int i = 0; i < num; i++) {
		P.G_Force(i);
		P.D_Force(i);
		P.E_Velocity(i);
		for (int j = 0; j <= 2; j++) {
			P.N_Force(i, j);
		}
		for (int j = 0; j <= 2; j++) {
			particle[i][0][j] += particle[i][1][j] * P.time / 1000;
		}
	}
	R.reset();
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
	R.start();
	glutInit(&argc, argv);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(120, 50);
	glutCreateWindow("OpenGL");
	glutDisplayFunc(display);
	glClearColor(0.0, 0.0, 0.0, 0);
	glutTimerFunc(0, Timer, 1);
	glutMainLoop();
}