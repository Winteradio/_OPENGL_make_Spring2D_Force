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

int num = 6;

float particle[6][3][3];
/// <summary> ��� ����
	/// ù��° [4] : ������ ����(1����[0], 2����[1], 3����[2], 4����[3])
	/// �ι�° [4] : Ư���� ����(��ǥ��[0], �ӵ���[1], ����[2], ����[3])
	///				ex) particle[3][0]~ �ȿ� ��� �ִ� �� : 4��° ���� ��ǥ���� ���� �迭
	/// ����° [3] : �ప�� (x��[0], y��[1], z��[2])
	/// </summary>

float T_Time = 0.0f;

class Physics{
public :
	float G = -10.0f; // �߷� ���
	float Ks = 1000.0f; // ź�����
	float Kd = 5.0f; // �׷� ���
	float Kr = 0.5f; // �ݹ߰��
	float mass = 10.0f; // ������ ����
	float L = 200.0f; // ���ö ���� ����
	int Time =10; // Time_step
	float e = 0.00001f; // e �浹�� ���� �ÿ� ���̴� ���

	float Dot(float V1[3], float V2[3]) { // ���� ����
		return V1[0] * V2[0] + V1[1] * V2[1] + V1[2] * V2[2];
	}

	float Distance(float V1[3]) { // �� �� ������ �Ÿ�
		return (float)pow((pow(V1[0], 2) + pow(V1[1], 2) + pow(V1[2],2)),0.5);
	}


	void G_Force(int i) { // �߷� ���
		particle[i][2][1] += G*mass;
	}

	void D_Force(int i) { // �׷� ���
		for (int j = 0; j <= 2; j++) {
			particle[i][2][j] += -Kd * particle[i][1][j];
		}
	}

	void S_Force(){// ź���� ���
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


	void E_Velocity(int i) { // Euler Velocity �ӵ� ���
			for (int j = 0; j <= 2; j++){
				particle[i][1][j] += ((float)Time / 1000) * particle[i][2][j]/mass;
			}
	}

	void Collision(int i) { // �浹 ���
		float X_Delta[3] = { 0,0,0 };
		float N_Delta[3] = { 0,0,0 };
		float V_Delta[3] = { 0,0,0 };
		for (int j = 0; j <= 2; j++) {
			if (sqrt(pow(particle[i][0][j], 2)) >= (float)GLUT_INIT_WINDOW_WIDTH/2) {
				if (particle[i][0][j] >= (float)GLUT_INIT_WINDOW_WIDTH / 2) {
					N_Delta[j] = -1;
				}
				else {
					N_Delta[j] = 1;
				}
				X_Delta[j] = particle[i][0][j] + N_Delta[j] * (float)GLUT_INIT_WINDOW_WIDTH / 2;
				for (int k = 0; k <= 2; k++) {
					V_Delta[k] = particle[i][0][k];
				}
			}
		}
		if (Dot(X_Delta, N_Delta) < e) {
			if (Dot(V_Delta, N_Delta) < 0) {
				for (int j = 0; j <= 2; j++) {
					if (sqrt(pow(particle[i][0][j], 2)) >= (float)GLUT_INIT_WINDOW_WIDTH / 2) {
						particle[i][1][j] = -Kr * particle[i][1][j];
					}
				}
			}

		}
	}

	void Contact(int i) { // ���� ���
		float X_Delta[3] = { 0,0,0 };
		float N_Delta[3] = { 0,0,0 };
		float V_Delta[3] = { 0,0,0 };
		for (int j = 0; j <= 2; j++) {
			if (sqrt(pow(particle[i][0][j], 2)) >= (float)GLUT_INIT_WINDOW_WIDTH / 2) {
				if (particle[i][0][j] >= (float)GLUT_INIT_WINDOW_WIDTH / 2) {
					N_Delta[j] = -1.0f;
				}
				else {
					N_Delta[j] = 1.0f;
				}
				X_Delta[j] = particle[i][0][j] + N_Delta[j] * (float)GLUT_INIT_WINDOW_WIDTH / 2;
				for (int k = 0; k <= 2; k++) {
					V_Delta[k] = particle[i][0][k];
				}

				cout << "Dot :" << Dot(X_Delta, N_Delta) << endl;
			}
		}
		if (sqrt(pow(Dot(X_Delta, N_Delta),2)) < e) {
			if (fabs(Dot(V_Delta, N_Delta)) < e) {
				for (int j = 0; j <= 2; j++) {
					if (sqrt(pow(particle[i][0][j], 2)) >= (float)GLUT_INIT_WINDOW_WIDTH / 2) {
						particle[i][2][j] = 0;
					}
				}
			}
		}
	}
};

class Reset { // �ʱ�ȭ
public :
	void reset() { // �� Ÿ�� ���Ǹ��� ���� �ʱ�ȭ
		for (int i = 0; i < num; i++) {
			for (int j = 0; j < 3; j++) {
				particle[i][2][j] = 0;
			}
		}
	}

	void start() { // ó���� ���ڵ��� ��ǥ�� �ִ� �Լ�
		for (int i = 0; i < num; i++) {
			for (int j = 0; j <= 1; j++) {
				for (int k = 0; k <= 1; k++) {
					particle[i][j][k] = ((float)(rand() % 100 - 50));
				}
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
		P.Contact(i);
		P.E_Velocity(i);
		P.Collision(i);
		for (int j = 0; j <= 2; j++) {
			particle[i][0][j] += particle[i][1][j] *(float)P.Time / 1000;
		}
	}
	R.reset();
	T_Time += (float)P.Time / 1000;
	glutPostRedisplay();
	glutTimerFunc(P.Time, Timer, 1);
}


void display() {
	glClear(GL_COLOR_BUFFER_BIT);

	glPointSize(5);
	glColor3f(0.0, 1.0, 0.0);
	glBegin(GL_POINTS);
	{
		for (int i = 0; i < num; i++) {
			glVertex2f(particle[i][0][0]/((float)(GLUT_INIT_WINDOW_WIDTH) / 2), particle[i][0][1]/ ((float)(GLUT_INIT_WINDOW_HEIGHT) / 2));
		}
	}
	glEnd();
	glColor3f(1.0, 1.0, 1.0);
	glLineWidth(1.0);
	glBegin(GL_LINES);
	{for (int i = 0; i < num-1; i++) {
		for (int j = i + 1; j < num; j++) {
			glVertex2f(particle[i][0][0]/ ((float)(GLUT_INIT_WINDOW_WIDTH) / 2), particle[i][0][1]/ ((float)(GLUT_INIT_WINDOW_HEIGHT) / 2));
			glVertex2f(particle[j][0][0]/ ((float)(GLUT_INIT_WINDOW_WIDTH) / 2), particle[j][0][1]/ ((float)(GLUT_INIT_WINDOW_HEIGHT) / 2));
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