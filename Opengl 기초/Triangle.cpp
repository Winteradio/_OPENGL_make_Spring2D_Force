#include "GL/freeglut.h"
#include <iostream>

/// 위글
/// wglCreateContext() : 랜더링 컨텍스트를 생성한다.
/// wglDeleteContext() : 랜더링 컨텍스트를 삭제한다.
/// wglMakeCurrent() : 현재 랜더링 컨텍스트를 설정한다.
/// wglGetProcAddress() : 확장함수 포인터를 리턴한다.
/// wglUseFontBitmaps() : 현재 선택된 GDI 폰트에 대해 오픈GL 디스플레이 리스트를 만든다.
/// wglUseFontOutlines() : 현재의 외각선 글꼴과 각 글리프들에 대한 디스플레이 리스트를 만든다.
/// 
/// GLUT
/// glutTimerFunc() : 지정된 시간후에 지정된 함수를 한번 호출한다. 다시 호출하려면 다시 이 함수를 호출해야한다.
/// 
/// 픽셀형식
/// ChoosePixelFormat() : 가장 근접한 픽셀형식을 결정한다.
/// SetPixelFormat() : 픽셀형식을 디바이스 컨텍스트에 설정헌다.
/// 
/// 화면출력
/// ChangeDisplaySetting() : 전체화면으로 바꾼다.
/// SwapBuffers() : 후면버펴를 전면버퍼로 바꾼다.
/// glClear() : 화면과 버퍼를 지운다.
/// glClearColor() : 화면 배경 색상을 설정한다.
/// 
/// 그리기
/// glBegin() : 기본도형 그리기를 시작한다.
/// glEnd() : 기본도형 그리기를 마친다.
/// glVertex[2,3,4][d,f,i,s][v]() : 정점을 지정한다.
/// glColor[3,4][b,d,f,i,s,f]() : 색을 지정한다.
/// 
/// 점
/// glPointSize() : 점의 크기를 변경한다.
/// 
/// 선
/// glLineWidth() : 선의 굵기를 변경한다.
/// glLineStipple() : 점선 패턴을 설정한다.
/// 
/// 다각형
/// glPolygonMode() : 전면, 후면의 다각형 그리기 방식(점,선,면 그리기)을 설정한다.
/// glCullFace() : 면을 제거한다.
/// glFrontFace() : 전면을 변경한다.
/// glEdgeFlag() : 변을 숨긴다. 정점 지정 중에 사용한다.
/// glPolygonStipple() : 다각형 패턴설정
/// 
/// 기타 도형 그리기
/// glRectf() : 사각형을 그린다
/// auxSolidCube() : 정육면체를 그린다.
/// auxSolidSphere() : 구를 그린다.
/// auxSolidTorus() : 토러스(원환)을 그린다.
/// 
/// 스테이트머신
/// 
/// 행렬
/// 
/// 스택
/// 
/// 조망변환
/// 
/// 모델링변환
/// 
/// 투영
/// 
/// 셰이딩
/// 
/// 조명
/// 
/// 깊이
/// 
/// 비트맵
/// 
/// 텍스쳐
/// 
/// 다중 텍스쳐링
/// 
/// 디스플레이 리스트
/// 
/// 정점배열


#pragma comment(lib, "freeglut.lib")
#pragma comment(lib, "glew32s.lib")

using namespace std;
    /// glFlush : gl 명령어를 받는 즉시 명령어 하나하나를 즉시 처리할 경우 GPU는 매번 파이프라인 프로세서아 교신해야 하기 때무넹 시간적 손해 발생.
    ///           때문에 일정한 분량의 명령어를 쌓아두었다가 한 번에 파이프라인 프로세서에 전달한다. 
    ///           이를 기다리길 원치 않을 때 glFlush 함수를 호출하면 된다.

    /// glViewport(x,y,width,hight) : viewport를 설정하는 함수이다. GL이 사용하는 화면 좌표계는 윈도우의 좌하단을 원점으로 하는 것을 주의해야한다.

GLfloat current_angle = 0.0f; // 삼각형의 현재회전각도

void Keyboard(unsigned char key, int x, int y) {
    cout << "다음 키가 눌러졌습니다. W" << key << "W ASCII:" << int(key) << endl;

    // ESC 키가 눌러졌다면 프로그램 종료
    if (key == 27) {
        exit(0);
    }
    // key는 숫자 번호에 따라 번호가 달라지는 것 같다.
    /// 방향키
    /// 왼쪽 : 37
    /// 위로 : 38
    /// 오른족 : 39
    /// 아래쪽 : 40
}

void drawBitmapText(char* str, float x, float y, float z) {
    glRasterPos3f(x, y, z); // 문자열이 그려질 위치지정

    while (*str) {
        //GLUT_BITMAP_TIEMS_ROMAN_24 폰트를 사용하여 문자열을 그린다.
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *str);
        str++;
    }
}

void display() {
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // GlClear에서 컬러 버퍼 지운후 윈도우를 채울 색을 지정>> 지금은 검은색
    glClear(GL_COLOR_BUFFER_BIT); // 컬러 버퍼를 지운다.

    glMatrixMode(GL_MODELVIEW); // 이후 계산은 modelview matrix에 영항을 주게 된다
    // glMatrixMode() : 
    /////////////// GL_MODELVIEW :  화면에 형상을 그리면 GL_MODELVIEW 행렬을 곱해서 실제적 위치를 정한다.
    ///////////////                 이는 glTanslatef, glRotatef, glScalef 등을 통해 물체 위치를 변환하는 것이며, '모델링 변환'이라 부른다.
    /////////////// GL_PROJECTION : 화면에 어떻게 나타나게 할 것인지를 결정한다.

    glLoadIdentity(); // modelview matrix를 초기화

    // Z축을 중시믕로 설정된 current_angle로 회전한다.
    glRotatef(current_angle, 0.0f, 0.0f, 1.0f);
    // glRotatef() : 지정된 축에 대해서 angle만큼의 회전행렬을 곱한다.
    /////////////// current_angle : GLfloat angle : 회전각도
    /////////////// 뒤에 좌표계 : GLfloat x,y,z : 회전축 


    //중앙이 원점에 오도록 한변의 길이가 1인 빨간색 정사각형을 그린다.
    glColor3f(1.0f, 0.0f, 0.0f); //빨간색 지정
    glBegin(GL_TRIANGLES); // 4점이 하나의 사각형을 구성한다. 반시계 방향으로 4점의 vertex를 지정해줘야한다.
    // glBegin() : 그리기를 시작하는 함수
    /////////////// GL_POINT : 점을 그린다.
    /////////////// GL_LINES : 선을 그린다.
    /////////////// GL_POLYGON : 다각형을 그린다.
    /////////////// GL_TRIANGLES : 삼각형을 그린다.
    /////////////// GL_QUADS : 사각형을 그린다.
    /////////////// GL_LINE_STRIP : 겹친 선을 그린다.
    /////////////// GL_LINE_LOOP : 겹치지 않고 연결된 선을 그린다.
    /////////////// GL_TRIANGLE_STRIP : 삼각형이 연결된 도형을 그린다.
    /////////////// GL_TRIANGEL_FAN : 한점을 줌시으로 삼각형이 연결된 도형을 그린다.
    /////////////// GL_QUA_STRIP : 사각형이 이어진 도형을 그린다.
    glColor3f(1.0f, 0.0f, 0.0f); // 빨간색 지정
    glVertex3f(-0.5f, -0.5f,0.0f); // 왼쪽 아래 vertex 
    glColor3f(0.0f, 1.0f, 0.0f); // 녹색 지정
    glVertex3f(0.5f, -0.5f,0.0f); // 오른쪽 아래 vertex
    glColor3f(0.0f, 0.0f, 1.0f); // 파란색 지정
    glVertex3f(0.5f, 0.5f,0.0f); // 위쪽 vertex
    glEnd();

    glColor3f(1.0f, 1.0f, 1.0f); // 흰색 지정
    drawBitmapText("- 0.5, -0.5", -0.5f, -0.5f, 0.0f);// 지정한 좌표에 문자열 출력
    drawBitmapText("0.5,-0.5", 0.5f, -0.5f, 0.0f);
    drawBitmapText("0.0,0.5", 0.5f, 0.5f, 0.0f);

    // display 함수 호출될 때마다 회전각도 증가시킴
    current_angle += 0.5;
    cout << current_angle << endl;

    // 더블 버퍼링을 하고 있다면, 프론트 버퍼와 백버퍼 2개가 사용된다. 현재 화면에 보여지는 것은 프론트 버퍼에 있는 내용이다.
    // 백 버퍼는 다음 장면을 위해 렌더링을 하고 있는 곳이다. 백퍼터의 렌더링이 완료되면 두 개의 버퍼를 교환(swap)한다.
    // 화면에 업데이트된 프론트 버퍼에 있는 내용이 출력된다.

    // 싱글 버퍼라면 버퍼에 있는 것을 화면에 출력한다.
    glutSwapBuffers();
}

void reshape(GLsizei width, GLsizei height) {
    if (height == 0) { // 0으로 나누는 것을 방지
        height = 1;
    }

    GLfloat aspect = (GLfloat)width / (GLfloat)height;

    // 변경된 윈도우 크기로 viewport를 설정한다.
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION); // 뒤에 오는 계산들은 Projection matrix에 영향을 주도록 설정
    glLoadIdentity(); //projection matrix 초기화
    if (width >= height) {
        // aspect >=1 이면, height를 [-1,1]로 설정하고 width는 [-1*aspect,1*spect]로 설정한다.
        // left,right,top,bottom
        gluOrtho2D(-1.0 * aspect, 1.0 * aspect, -1.0, 1.0);
    }
    else {
        // aspect <1 이면, width를 [-1,1]로 설정하고 heigth를 [-1/aspect,1/aspect]로 설정한다.
        gluOrtho2D(-1.0, 1.0, -1.0 / aspect, 1.0 / aspect);
    }

    // 뒤에 오는 계산들은 Modelview Matrix에 영향을 주도록 설정한다.
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity(); // Modelview Matrix 초기화
}

void timer(int value) {
    glutPostRedisplay(); // 윈도우를 다시 그리도록 요청
    glutTimerFunc(30, timer, 0); // 다음 타이머 이벤트는 30밀리세컨트 후 호출됨.
}

int main(int argc, char** argv) {
    glutInit(&argc, argv); // GLUT 초기화

    glutInitDisplayMode(GLUT_DOUBLE); // 더블 버퍼를 사용하도록 설정
    glutInitWindowSize(500, 500); // 윈도우의 width와 height 설정
    glutInitWindowPosition(100, 100); // 윈도우의 위치 (x,y)
    glutCreateWindow("OpenGl Example"); // 윈도우 생성

    // 디스플레이 콜백 함수 등록, display 함수는 윈도우 처음 생성할 때와 화면 다시 그릴 필요 있을 때 호출된다.
    glutDisplayFunc(display);
    // 키보드 콜백 함수 등록, 키보드가 눌러지면 호출된다.
    glutKeyboardFunc(Keyboard);
    // reshape 콜백 함수 등록, reshape함수는 윈도우 처음 생성할 때와 윈도우 크기 변경시 호출된다.
    glutReshapeFunc(reshape);
    // 타이머 콜백 함수 등록, 처음에는 바로 호출됨
    glutTimerFunc(0, timer, 0);

    // Glut event proceeing loop에 진입한다.
    // 이 함수는 리턴되지 않기 때문에 다음줄에 잇는 코드가 실행되지 앟는다.
    glutMainLoop();

    return 0;
}
