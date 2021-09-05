#include "GL/freeglut.h"
#include <windows.h>
#include <math.h>
#include <cmath>
#include <cstdlib>
#include <stdlib.h>
#include <iostream>
#include <random>

//����� ���̺귯���� �������ݴϴ�.
#pragma comment(lib, "freeglut.lib")
#pragma comment(lib, "glew32s.lib")

using namespace std;

int num = 7;

float particle[7][3][3];
/// <summary> ��� ����
	/// ù��° [4] : ������ ����(1����[0], 2����[1], 3����[2], 4����[3])
	/// �ι�° [4] : Ư���� ����(��ǥ��[0], �ӵ���[1], ����[2], ����[3])
	///				ex) particle[3][0]~ �ȿ� ��� �ִ� �� : 4��° ���� ��ǥ���� ���� �迭
	/// ����° [3] : �ప�� (x��[0], y��[1], z��[2])
	/// </summary>

float T_Time = 0.0f;

class Physics{
public :
	float G = -0.0f; // �߷� ���
	float Ks = 100.0f; // ź�����
	float Kd = 20.0f; // �׷� ���
	float Kr = 0.5f; // �ݹ߰��
	float mass = 10.0f; // ������ ����
	float L = 0.5f; // ���ö ���� ����
	int Time =10; // Time_step
	float time = 10.0f;

	float Dot(float V1[2], float V2[2]) {
		return V1[0] * V2[0] + V1[1] * V2[1];
	}

	float Distance(float V1[2]) {
		return (float)pow((pow(V1[0], 2) + pow(V1[1], 2)),0.5);
	}


	void G_Force(int i) { // �߷�
		particle[i][2][1] += G*mass;
	}

	void D_Force(int i) { // �׷�
		for (int j = 0; j <= 2; j++) {
			particle[i][2][j] += -Kd * particle[i][1][j];
		}
	}

	void S_Force(){// ź����
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