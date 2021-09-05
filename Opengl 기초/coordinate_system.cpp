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

int num = 4;

float particle[4][4][3];
/// <summary> ��� ����
	/// ù��° [4] : ������ ����(1����[0], 2����[1], 3����[2], 4����[3])
	/// �ι�° [4] : Ư���� ����(��ǥ��[0], �ӵ���[1], ����[2], ����[3])
	///				ex) particle[3][0]~ �ȿ� ��� �ִ� �� : 4��° ���� ��ǥ���� ���� �迭
	/// ����° [3] : �ప�� (x��[0], y��[1], z��[2])
	/// </summary>

float T_Time = 0.0f;

class Physics{
public :
	float G = -15.0f; // �߷� ���
	float Ks = 2.0f; // ź�����
	float Kd = 2.0f; // �׷� ���
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


	float G_Force() { // �߷�
		return G;
	}

	float D_Force() { // �׷�
		return -Kd * V;
	}

	void S_Force() { // ź����
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