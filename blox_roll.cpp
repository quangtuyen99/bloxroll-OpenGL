#pragma warning(disable:4996)
#include<stdio.h>
#include <GL/glut.h>
#include "main.h"
#define N 15 // Độ dài mảng

// Khai báo cấu trúc điểm 2d
typedef struct Point {
	int x, y;
} Point;

// Khai báo cấu trúc điểm 3d
typedef struct Point3f {
	float x, y, z;
} Point3f;


void (*RollFunc[4])();
void (*tFunc)(int); 
void (*tWinFunc)(int);

void tNULL(int value) { }
void tNULL2(int value) { tFunc = &tBloxRoll; }


Point3f* pPnt;
Point3f A, B, C, D, E, F, G, H;
// Tọa độ vẽ khối hộp theo các mặt tương ứng
Point3f CLeft, CRight, CBack, CFront, CTop, CBottom;
// p-điểm
Point3f p[80], pExtra[6], tmpP;
Point3f ColorPath[] = { {0.3f, 0.4f, 0.5f}, {0.3f, 0.4f, 0.5f}, {0.2f, 0.3f, 0.4f}, {0.3f, 0.2f, 0.4f}, {0.2f, 0.1f, 0.4f}, {0.4f, 0.2f, 0.3f}, {0.2f, 0.1f, 0.1f} };// Màu của từng mặt
Point3f Axis[] = { {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f} };
Point3f Color[2] = { {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 5.0f} };
Point3f* ptrColor;
Point3f ColorWin[2] = { {0.0f, 1.0f, 0.2f}, {1.0f, 0.8f, 0.5f} };

Point Gate[15][6]; // Tọa độ đọc được, sẽ được mở thêm trong bản đồ

float AngleOffset[] = { -5.0f, -5.0f, 5.0f, 5.0f };
float angle;
float x, y, z, tmpF;

/*
numLv - biến đếm level
pSum - Tổng số điểm
canPrev, canNext - Biến kiểm tra có thể bấm mũi tên trái qua phải nữa hay không trong menu
*/
int pFirst, pLast, pDone, hasBlox, pSum, tSpawnPath, level, level2, numLv, stWin;
int canPrev, canNext, tP, tN;
/*
StartX, StartY - Tọa độ bắt đầu
t2 - biến góc nhìn
*/
int i, j, k, ii, ij, q, n, Choice, Choice2, t, t2 = 20, mt, StartX, StartY, Win;
/*
PosX, PosY - Biến hỗ trợ cho xử lý di chuyển khối hộp
*/

int PosX, PosY, Enter;
float ViewX, ViewY, TranX, TranZ, LookAt, OL;

// khai báo chuỗi xuất hiện trong game
char textLogo[] = "Blox Roll";
char textMenu[2][6] = { "Play", "Level" };
char textWin[2][5] = { "Win", "Lose" };

// Chiều dài chuỗi
int lenWin[2] = { 3, 4 };
float startWin[2] = { -130.0f, -180.0f };
int lenMenu[2] = { 4, 5 };

//Tọa độ 3d
//alpha - giá trị màu alpha
float alpha, cx, cy, cz;

// Numextra - số được thêm tương ứng với mỗi level
int loop[20], numExtra[15], offsetExtra[6], markExtra[6];
float alphaExtra[6];

// bản đồ và bản đồ mở rộng mảng 3 chiều biến đầu là level, biến 2 và 3 là đọc từ mảng 2 chiều từ file
int Map[15][N][N], extraMap[15][N][N];

int offsetText[9], offset[20];
int tIO = 0, isNext, isSwitching = 0;
float offsetMap = -0.44f, oMD;
char txtLv[20][2];
float oTxtLv[20];
int lTxtLv[20];

// Hàm vẽ
void reshape(int w, int h) { }

//Hàm main
int main(int argc, char** argv) {
	importMap(); // Đọc map từ file txt
	initOffset();
	initTextLevel();
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowPosition(200, 50);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Blox Roll");
	glEnable(GL_LINE_SMOOTH); //vẽ các đường với bộ lọc chính xác. Nếu không, hãy vẽ các đường bí danh.
	glEnable(GL_BLEND); // trộn các giá trị màu của phân đoạn đã tính toán với các giá trị trong bộ đệm màu. 
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // Có 2 tham số được truyền vào sfactor và dfactor
														//sfactor: Chỉ định cách tính các hệ số trộn nguồn alpha, xanh lục, xanh lam và đỏ.
														//dfactor: Chỉ định cách tính các yếu tố hòa trộn đích đến đỏ, xanh lá cây, xanh lam và alpha.
	glClearColor(0.2, 0.5, 0.6, 1);
	initMenu(); // Khởi tạo menu
	glutReshapeFunc(reshape);
	glutMainLoop();
	return 0;
}

// Khởi tạo hình hộp chữ nhật
void initBlox() {
	x = 1.0f; y = 2.0f; z = 1.0f;
	A.x = 0.0f; A.y = 0.0f; A.z = 0.0f;
	B.x = 1.0f; B.y = 0.0f; B.z = 0.0f;
	C.x = 1.0f; C.y = 0.0f; C.z = 1.0f;
	D.x = 0.0f; D.y = 0.0f; D.z = 1.0f;
	E.x = 0.0f; E.y = 2.0f; E.z = 0.0f;
	F.x = 1.0f; F.y = 2.0f; F.z = 0.0f;
	G.x = 1.0f; G.y = 2.0f; G.z = 1.0f;
	H.x = 0.0f; H.y = 2.0f; H.z = 1.0f;
	CLeft.x = 0.0f; CLeft.y = 0.0f; CLeft.z = 1.0f;
	CRight.x = 0.0f; CRight.y = 1.0f; CRight.z = 0.0f;
	CBack.x = 0.0f; CBack.y = 1.0f; CBack.z = 1.0f;
	CFront.x = 1.0f; CFront.y = 0.0f; CFront.z = 1.0f;
	CTop.x = 1.0f; CTop.y = 0.0f; CTop.z = 0.0f;
	CBottom.x = 1.0f; CBottom.y = 1.0f; CBottom.z = 0.0f;
}

// Đọc map từ file txt
void importMap() {
	// Khai báo biến file
	FILE* f;
	char s[10];
	level = 0;
	do {
		sprintf_s(s, "Map%d.txt", level + 1);
		// Mở file
		f = fopen(s, "r");
		if (f == NULL) break;
		q = 0;
		// Trong file là mảng 2 chiều với kích thước 15x15
		for (i = 0; i < 15; i++) {
			for (j = 0; j < 15; j++) {
				
				fscanf(f, "%d", &n);// Đọc từ file
				Map[level][i][j] = n;

				if (n == 3 || n == 4 || n == 5) { // Nếu số đọc được là 3 4 hoặc 5 
					// 3 là tạo ô mở rộng
					// 4 là tạo ô mở rộng nhưng phải đứng lên
					// 5 tương tự 4 nhưng bật tắt được
					extraMap[level][i][j] = q; // Gán giá trị mảng tại vị trí bằng q
					q++;
				}
			}
		}

		fscanf(f, "%d", &numExtra[level]); // Đọc số được thêm với level tương ứng
		for (k = 0; k < numExtra[level]; k++) {  // Duyệt mảng 
			fscanf(f, "%d%d", &i, &j); // Đọc theo từng cặp số
			Gate[level][k].x = i; // Gán theo tọa độ x
			Gate[level][k].y = j; // Gán theo tọa độ y
		}
		level++; // Tăng biến cấp
	} while (1);

	numLv = level; // Đặt lại biến numLv
}


// Khởi tạo bản đồ
void initMap() {
	// Duyệt mảng 2 chiều 15x15
	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {

			if (Map[level][i][j]) { // Nếu tại level, vị trí i,j
				if (Map[level][i][j] == -1) { // Nếu vị trí đó có giá trị -1

					Map[level][i][j] = 1; // Đặt lại giá trị 1

					// Tọa độ bắt đầu
					StartX = j; 
					StartY = i;

					// Đặt giá trị xử lý di chuyển
					PosX = 2 * j;
					PosY = 2 * i;

					// Tọa độ theo 3d
					cx = j;
					cz = i;
					cy = 16.0f;
				}
				
				// Đặt giá trị 3d p tại vị trí pSum
				p[pSum].x = j;
				p[pSum].y = -16.0f;
				p[pSum].z = i;
				pSum++; // Tăng biến pSum
			}
		}
	}

	for (i = 0; i < numExtra[level]; i++) {
		alphaExtra[i] = 0.0f;
		offsetExtra[i] = 0;
		markExtra[i] = 0;
	}
}

// Khởi tạo menu
void initMenu() {
	glDisable(GL_DEPTH_TEST);
	glViewport(0, 0, 800, 600);
	alpha = 0.0f;
	t = -1000;
	Enter = 0;
	Choice2 = 0; // Biến chọn
	glutDisplayFunc(displayMenu);
	glutTimerFunc(0, tLogoIn, 0);
}

// Khởi tạo map
void initPlay() {
	glEnable(GL_DEPTH_TEST);
	glViewport(0, 0, 800, 600);
	pFirst = pLast = pDone = hasBlox = pSum = tSpawnPath = 0;
	Choice = -1;
	mt = -1;
	ViewX = 30.0f; ViewY = -20.0f;
	TranX = 0.0f; TranZ = 0.0f;
	LookAt = 0.0f;
	angle = 0.0f;
	Win = -1;
	stWin = 0;

	initMap(); // Khởi tạo bản đồ
	initBlox(); // Khởi tạo khối hộp

	tFunc = &tPath;
	tWinFunc = &tWin;

	glutKeyboardFunc(keyboardSkip); // Nếu muốn pass xây dựng map, có thể bấm esc để skip qua
	glutTimerFunc(25, tPath, 0);
}

// Khởi tạo di chuyển khối hộp
void initOffset() {
	RollFunc[0] = &rollFuncUpDone;
	RollFunc[1] = &rollFuncRightDone;
	RollFunc[2] = &rollFuncDownDone;
	RollFunc[3] = &rollFuncLeftDone;
	for (i = 0; i < 11; i++) {
		offset[i] = i * 5;
		loop[i] = i + 1;
	}
	for (i = 11; i < 20; i++) {
		offset[i] = 100 - i * 5;
		loop[i] = i + 1;
	}
	loop[19] = 0;
	for (i = 0; i < 9; i++)
		offsetText[i] = i;
}

// Khởi tạo text của level
void initTextLevel() {
	for (i = 0; i < numLv; i++) {
		if (i < 9) {
			txtLv[i][0] = i + 49;
			oTxtLv[i] = 0.103f;
			lTxtLv[i] = 1;
		}
		else {
			txtLv[i][0] = (i + 1) / 10 + 48;
			txtLv[i][1] = (i + 1) % 10 + 48;
			oTxtLv[i] = 0.041f;
			lTxtLv[i] = 2;
		}
	}
}

// Hàm vẽ menu khởi tạo
void displayMenu() {
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();
	gluPerspective(70.0f, 4.0f / 3, 0.1f, 2000.0f);
	glPushMatrix();
	glLineWidth(2.0f);
	glColor3f(0.5f, 1.0f, 1.0f);
	glTranslatef(-380.0f, 200.0f, -1000.0f);
	for (i = 0; i < 9; i++) {
		glTranslatef(t, offset[offsetText[i]], 0);
		glutStrokeCharacter(GLUT_STROKE_ROMAN, textLogo[i]);
		glTranslatef(-t, -offset[offsetText[i]], 0);
		glTranslatef(15.0f, 0.0f, 0.0f);
	}
	if (alpha > 0.01f) {
		ptrColor = &Color[Choice2];
		glColor4f(ptrColor->x, ptrColor->y, ptrColor->z, alpha);
		glTranslatef(-400.0f, -420.0f, -400.0f);
		for (i = 0; i < lenMenu[0]; i++) {
			glutStrokeCharacter(GLUT_STROKE_ROMAN, textMenu[0][i]);
		}

		ptrColor = &Color[1 - Choice2];
		glColor4f(ptrColor->x, ptrColor->y, ptrColor->z, alpha);
		glTranslatef(-260.0f, -260.0f, 0.0f);
		for (i = 0; i < lenMenu[1]; i++) {
			glutStrokeCharacter(GLUT_STROKE_ROMAN, textMenu[1][i]);
		}

		if (Choice2 == 1)
			glTranslatef(-400.0f, 50.0f, 0.0f);
		else
			glTranslatef(-400.0f, 310.0f, 0.0f);

		glBegin(GL_TRIANGLES);
		glColor4f(0.0f, 1.0f, 0.0f, alpha);
		glVertex3f(0.0f, 60.0f, 0.0f);
		glColor4f(0.0f, 0.0f, 1.0f, alpha);
		glVertex3f(0.0f, -60.0f, 0.0f);
		glColor4f(0.0f, 1.0f, 1.0f, alpha);
		glVertex3f(60.0f, 0.0f, 0.0f);
		glEnd();
	}
	glPopMatrix();
	glutSwapBuffers();
}

// Hàm thay đổi biến choice mỗi khi bấm 1 mũi tên thì Choice2 sẽ thay đổi từ -1 thành 0 và ngược lại
void specialMenu(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_UP:
	case GLUT_KEY_LEFT:
	case GLUT_KEY_DOWN:
	case GLUT_KEY_RIGHT:
		Choice2 = 1 - Choice2;
		break;
	}
}

// Hàm bàn phím Menu 
void keyboardMenu(unsigned char key, int x, int y) {
	switch (key) {
	case 13: // Nếu bấm Enter
		switch (Choice2) {
		case 0: // Play
			level = 0;
			initPlay();
			glutDisplayFunc(displayPlay);
			break;
		case 1: // Level
			level = level2 = 0;
			canPrev = 0;
			if (numLv > 1) {
				canNext = 1;
				tN = 0;
			}
			glutDisplayFunc(displaySL);
			glutKeyboardFunc(keyboardSL);
			glutSpecialFunc(specialSL);
			break;
		}
		Enter = 1;
		break;
	}
}

// Hàm vẽ màn chơi
void displayPlay() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	gluPerspective(35.0f, 4.0f / 3, 0.1f, 1000.0f);
	gluLookAt(LookAt, 0.0, 2.0, 0.0, 0.0, 1.0, 0.0, 1.0, 0.0);
	glTranslatef(-4.0f, 4.0f, -27.0f);
	glRotatef(ViewX, 1.0f, 0.0f, 0.0f);
	glRotatef(ViewY, 0.0f, 1.0f, 0.0f);
	glTranslatef(TranX, 1.0f, TranZ);
	drawGrid();
	for (k = 0; k < numExtra[level]; k++) {
		if (alphaExtra[k] > 0.0f) drawPathExtra(Gate[level][k].x, alphaExtra[k], Gate[level][k].y);
	}
	if (hasBlox == 1) {
		glTranslatef(cx, cy, cz);
		if (Choice != -1) {
			if (Choice == 1)
				glTranslatef(x, 0.0f, 0.0f);
			else if (Choice == 2)
				glTranslatef(0.0f, 0.0f, z);
			glRotatef(angle, Axis[Choice].x, Axis[Choice].y, Axis[Choice].z);
		}
		drawBlox();
	}
	if (stWin > 0) { // Vẽ win
		glDisable(GL_DEPTH_TEST);
		glColor3f(ColorWin[Win].x, ColorWin[Win].y, ColorWin[Win].z);
		glLineWidth(2.0f + stWin * 0.2f);
		for (i = 0; i < lenWin[Win]; i++) {
			glLoadIdentity();
			gluPerspective(70.0f, 4.0f / 3, 0.1f, 2000.0f);
			glTranslatef(startWin[Win] + 100.0f * i, 0.0f, -700.0f);
			glRotatef(90.0f - 4.5f * stWin, 0.0f, 1.0f, 0.0f);
			glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, textWin[Win][i]);
		}
		glEnable(GL_DEPTH_TEST);
	}
	glutSwapBuffers();
}

// hàm vẽ menu Level
void displaySL() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	//glPushMatrix();
	glViewport(0, 0, 800, 800);
	glTranslatef(-0.15f, -0.8f, 0.0f);
	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_QUADS);
	glVertex2f(0.0f, 0.0f);
	glVertex2f(0.3f, 0.0f);
	glVertex2f(0.3f, 0.3f);
	glVertex2f(0.0f, 0.3f);
	glEnd();
	if (canPrev) {
		glBegin(GL_TRIANGLES);
		glVertex2f(-0.14f + tP * 0.005f, 0.15f);
		glVertex2f(-0.06f, 0.23f - tP * 0.005f);
		glVertex2f(-0.06f, 0.07f + tP * 0.005f);
		glEnd();
	}
	if (canNext) {
		glBegin(GL_TRIANGLES);
		glVertex2f(0.44f - tN * 0.005f, 0.15f);
		glVertex2f(0.36f, 0.23f - tN * 0.005f);
		glVertex2f(0.36f, 0.07f + tN * 0.005f);
		glEnd();
	}
	glColor3f(0.0f, 0.0f, 0.0f);
	glTranslatef(tIO * 0.0025f * lTxtLv[level] + oTxtLv[level], 0.110f + tIO * 0.00235f, 0.0f);
	glLineWidth(4.0f - tIO * 0.16f);
	glScalef(0.001f - 0.000055f * tIO, 0.001f - 0.000055f * tIO, 1);
	for (i = 0; i < lTxtLv[level]; i++)
		glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, txtLv[level][i]);
	glLoadIdentity();
	glLineWidth(4.0f);
	glColor3f(1.0f, 1.0f, 1.0f);
	glViewport(0, 0, 800, 800);
	gluOrtho2D(-1.0f, 1.0f, -1.0f, 1.0f);
	glTranslatef(0.0f, -0.2f, 0.0f);
	glBegin(GL_LINES);
	glVertex2f(-0.505f, -0.2f);
	glVertex2f(-0.505f, 0.6f);
	glEnd();
	glBegin(GL_LINES);
	glVertex2f(0.507f, -0.2f);
	glVertex2f(0.507f, 0.6f);
	glEnd();
	glBegin(GL_LINES);
	glVertex2f(-0.505f, -0.2f);
	glVertex2f(0.507f, -0.2f);
	glEnd();
	glBegin(GL_LINES);
	glVertex2f(-0.505f, 0.6f);
	glVertex2f(0.507f, 0.6f);
	glEnd();
	glViewport(200, 0, 400, 800);
	glFrustum(-0.5f, 0.5f, -1.0f, 1.0f, 1.0f, 400.0f);
	glTranslatef(offsetMap, 1.1f, -2.3f);
	glRotatef(-40.0f, 1.0f, 0.0f, 0.0f);
	glRotatef(-20.0f, 0.0f, 0.0f, 1.0f);
	glLineWidth(1.0f);
	for (i = 0; i < 15; i++) {
		for (j = 0; j < 15; j++) {
			if (Map[level2][i][j]) drawPathSL(0.1f * j, -0.1f * i, Map[level2][i][j]);
		}
	}
	glTranslatef(1.8f, 0.655f, 0.0f);
	if (level2 < numLv - 1) for (i = 0; i < 15; i++) {
		for (j = 0; j < 15; j++) {
			if (Map[level2 + 1][i][j]) drawPathSL(0.1f * j, -0.1f * i, Map[level2 + 1][i][j]);
		}
	}
	glTranslatef(-3.6f, -1.31f, 0.0f);
	if (level2 > 0) for (i = 0; i < 15; i++) {
		for (j = 0; j < 15; j++) {
			if (Map[level2 - 1][i][j]) drawPathSL(0.1f * j, -0.1f * i, Map[level2 - 1][i][j]);
		}
	}
	//glPopMatrix();
	glutSwapBuffers();
}

// Hàm kiểm tra bàn phím khi đang trong menu
void keyboardSL(unsigned char key, int x, int y) {
	if (!isSwitching) switch (key) {
	case 13: // Nếu bấm enter
		initPlay();
		glutDisplayFunc(displayPlay);
		glutSpecialFunc(NULL);
		break;
	}
}

// Hàm di chuyển giữa các level menu
void specialSL(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_LEFT: if (canPrev && !isSwitching) { // Nếu bấm mũi tên trái
		oMD = 0.0638f; isNext = 0;
		if (!canNext) tN = 15; canNext = 1;
		isSwitching = 1;
		glutTimerFunc(0, tZoomOut, 0);
	}
					  break;
	case GLUT_KEY_RIGHT: if (canNext && !isSwitching) { // Nếu bấm mũi tên phải
		oMD = -0.0638f; isNext = 1;
		if (!canPrev) tP = 15; canPrev = 1;
		isSwitching = 1;
		glutTimerFunc(0, tZoomOut, 0);
	}
					   break;
	}
}

// Hàm vẽ bản đồ trong menu level
void drawPathSL(float x, float y, int z) {
	glPushMatrix();
	glTranslatef(x, y, 0.0f);
	switch (z) {
	case -1: glColor3f(0.8f, 0.35f, 0.35f); break;
	case 2: glColor3f(0.2f, 0.7f, 0.2f); break;
	case 6: glColor3f(0.2f, 0.1f, 0.2f); break;
	default: glColor3f(0.0f, 1.0f, 0.5f); break;
	}

	glBegin(GL_QUADS);
	glVertex2f(0.01f, 0.01f);
	glVertex2f(0.09f, 0.01f);
	glVertex2f(0.09f, 0.09f);
	glVertex2f(0.01f, 0.09f);
	glEnd();

	switch (z) {
	case 3: glColor3f(0.2f, 0.6f, 0.2f); glBegin(GL_QUADS);
		glVertex3f(0.028f, 0.028f, 0.001f); glVertex3f(0.072f, 0.028f, 0.001f);
		glVertex3f(0.072f, 0.072f, 0.001f); glVertex3f(0.028f, 0.072f, 0.001f); glEnd(); break;
	case 4: glColor3f(0.1f, 0.3f, 0.6f); glBegin(GL_TRIANGLES);
		glVertex3f(0.026f, 0.026f, 0.001f); glVertex3f(0.074f, 0.026f, 0.001f);
		glVertex3f(0.050f, 0.074f, 0.001f); glEnd(); break;
	case 5: glLineWidth(2.0f); glColor3f(0.2f, 0.6f, 0.3f); glBegin(GL_LINES);
		glVertex3f(0.028f, 0.028f, 0.001f); glVertex3f(0.072f, 0.072f, 0.001f);
		glVertex3f(0.028f, 0.072f, 0.001f); glVertex3f(0.072f, 0.028f, 0.001f); glEnd(); break;
	}

	glColor3f(0.3f, 0.8f, 0.8f);
	glBegin(GL_LINES);
	glVertex2f(0.0f, 0.0f);
	glVertex2f(0.1f, 0.0f);
	glEnd();
	glBegin(GL_LINES);
	glVertex2f(0.0f, 0.1f);
	glVertex2f(0.1f, 0.1f);
	glBegin(GL_LINES);
	glVertex2f(0.0f, 0.0f);
	glVertex2f(0.0f, 0.1f);
	glEnd();
	glBegin(GL_LINES);
	glVertex2f(0.1f, 0.0f);
	glVertex2f(0.1f, 0.1f);
	glEnd();
	glPopMatrix();
}



// Hàm zoom mũi tên và level
void tZoomIn(int value) {
	if (tIO > 0) {
		offsetMap += oMD;
		tIO--;
		if (canPrev && tP > 0) tP--;
		else
			if (canNext && tN > 0) tN--;
		glutPostRedisplay();
		glutTimerFunc(15, tZoomIn, 0);
	}
	else {
		if (isNext) level2++;
		else level2--;
		isSwitching = 0;
		offsetMap = -0.44f;
		glutPostRedisplay();
	}
}

// Hàm Zoom mũi tên và level
void tZoomOut(int value) {
	if (tIO < 15) {
		offsetMap += oMD;
		tIO++;
		if (isNext) {
			tN++;
			if (tP > 0) tP--;
		}
		else {
			tP++;
			if (tN > 0) tN--;
		}
		glutPostRedisplay();
		glutTimerFunc(15, tZoomOut, 0);
	}
	else {
		if (oMD < 0) {
			level++;
			if (level == numLv - 1) canNext = 0;
		}
		else {
			level--;
			if (level == 0) canPrev = 0;
		}
		glutTimerFunc(15, tZoomIn, 0);
	}
}



// Hàm thay đổi góc nhìn
void tChangeView(int value) {
	if (t2 < 20) {
		t2++;
		ViewX += 3 * mt;
		ViewY += 1 * mt;
		TranX -= 0.15f * mt;
		TranZ -= 0.2f * mt;
		glutTimerFunc(15, tChangeView, 0);
		glutPostRedisplay();
	}
}

// Hàm kiểm tra bàn phím khi đang chơi
void keyboardPlay(unsigned char key, int x, int y) {
	if (t2 == 20) {
		switch (key) {
		case 32: // Nếu bấm space sẽ thay đổi góc nhìn
			t2 = 0;
			mt = -mt;
			glutTimerFunc(0, tChangeView, 0);
			break;
		case 27: // Nếu bấm esc sẽ thoát ra
			tFunc = tNULL;
			tWinFunc = tNULL;
			glutKeyboardFunc(NULL);
			glutSpecialFunc(NULL);
			for (k = 0; k < numExtra[level]; k++)
				Map[level][Gate[level][k].y][Gate[level][k].x] = 0;
			Map[level][StartY][StartX] = -1;
			initMenu(); // Khởi tạo lại menu
			break;
		}
	}
}

// Hàm skip qua xây map
void keyboardSkip(unsigned char key, int x, int y) {
	if (t2 == 20) {
		switch (key) {
		case 32: // Nếu bấm space, thay đổi góc nhìn
			t2 = 0;
			mt = -mt;
			glutTimerFunc(0, tChangeView, 0);
			break;
		case 27: // Nếu bấm esc sẽ skip qua xây map
			tFunc = &tNULL2;
			glutSpecialFunc(specialPlay);
			glutKeyboardFunc(keyboardPlay);
			pLast = pSum;
			for (i = pFirst; i < pLast; i++) {
				p[i].y = 0.0f;
			}
			hasBlox = 1;
			cy = 0.0f;
			glutPostRedisplay();
			break;
		}
	}
}





// Hàm vẽ menu
void tMenu(int value) {
	for (i = 0; i < 9; i++)
		offsetText[i] = loop[offsetText[i]];
	glutPostRedisplay();
	if (Enter == 0) glutTimerFunc(25, tMenu, 0);
}

// Hàm vẽ text trong menu
void tTextAppear(int value) {
	if (alpha < 0.95f) {
		alpha += 0.1f;
		for (i = 0; i < 9; i++)
			offsetText[i] = loop[offsetText[i]];
		glutPostRedisplay();
		glutTimerFunc(25, tTextAppear, 0);
	}
	else {
		glutSpecialFunc(specialMenu);
		glutKeyboardFunc(keyboardMenu);
		glutTimerFunc(25, tMenu, 0);
	}
}
// Hàm vẽ bản đồ khi đang trong menu level
void tLogoIn(int value) {
	if (t < 45) {
		t += 15;
		for (i = 0; i < 9; i++)
			offsetText[i] = loop[offsetText[i]];
		glutPostRedisplay();
		glutTimerFunc(25, tLogoIn, 0);
	}
	else glutTimerFunc(25, tTextAppear, 0);
}



// Hàm vẽ khối hộp
void drawBlox() {
	glColor3f(CBack.x, CBack.y, CBack.z);
	glBegin(GL_QUADS);
	//glNormal3f(0.0f, 0.0f, -1.0f);
	glVertex3f(A.x, A.y, A.z);
	glVertex3f(B.x, B.y, B.z);
	glVertex3f(F.x, F.y, F.z);
	glVertex3f(E.x, E.y, E.z);
	glEnd();
	glColor3f(CFront.x, CFront.y, CFront.z);
	glBegin(GL_QUADS);
	//glNormal3f(0.0f, 0.0f, 1.0f);
	glVertex3f(D.x, D.y, D.z);
	glVertex3f(C.x, C.y, C.z);
	glVertex3f(G.x, G.y, G.z);
	glVertex3f(H.x, H.y, H.z);
	glEnd();
	glColor3f(CLeft.x, CLeft.y, CLeft.z);
	glBegin(GL_QUADS);
	//glNormal3f(-1.0f, 0.0f, 0.0f);
	glVertex3f(A.x, A.y, A.z);
	glVertex3f(D.x, D.y, D.z);
	glVertex3f(H.x, H.y, H.z);
	glVertex3f(E.x, E.y, E.z);
	glEnd();
	glColor3f(CRight.x, CRight.y, CRight.z);
	glBegin(GL_QUADS);
	//glNormal3f(1.0f, 0.0f, 0.0f);
	glVertex3f(B.x, B.y, B.z);
	glVertex3f(C.x, C.y, C.z);
	glVertex3f(G.x, G.y, G.z);
	glVertex3f(F.x, F.y, F.z);
	glEnd();
	glColor3f(CBottom.x, CBottom.y, CBottom.z);
	glBegin(GL_QUADS);
	//glNormal3f(0.0f, -1.0f, 0.0f);
	glVertex3f(A.x, A.y, A.z);
	glVertex3f(B.x, B.y, B.z);
	glVertex3f(C.x, C.y, C.z);
	glVertex3f(D.x, D.y, D.z);
	glEnd();
	glColor3f(CTop.x, CTop.y, CTop.z);
	glBegin(GL_QUADS);
	//glNormal3f(0.0f, 1.0f, 0.0f);
	glVertex3f(E.x, E.y, E.z);
	glVertex3f(F.x, F.y, F.z);
	glVertex3f(G.x, G.y, G.z);
	glVertex3f(H.x, H.y, H.z);
	glEnd();
}

// Hàm vẽ bản đồ
void drawPath(float x, float y, float z) {
	glPushMatrix();
	pPnt = &ColorPath[Map[level][(int)z][(int)x]];
	glColor3f(pPnt->x, pPnt->y, pPnt->z);
	glTranslatef(x, y, z);
	glBegin(GL_QUADS);
	glVertex3f(0.04f, 0.0f, 0.04f);
	glVertex3f(0.96f, 0.0f, 0.04f);
	glVertex3f(0.96f, 0.0f, 0.96f);
	glVertex3f(0.04f, 0.0f, 0.96f);
	glEnd();
	switch (Map[level][(int)z][(int)x]) {
	case 3: glColor3f(0.4f, 0.9f, 0.4f); glBegin(GL_QUADS);
		glVertex3f(0.28f, 0.01f, 0.28f); glVertex3f(0.72f, 0.01f, 0.28f);
		glVertex3f(0.72f, 0.01f, 0.72f); glVertex3f(0.28f, 0.01f, 0.72f); glEnd(); break;
	case 4: glColor3f(0.22f, 0.80f, 0.80f); glBegin(GL_TRIANGLES);
		glVertex3f(0.26f, 0.01f, 0.26f); glVertex3f(0.74f, 0.01f, 0.26f);
		glVertex3f(0.50f, 0.01f, 0.74f); glEnd(); break;
	case 5: glLineWidth(2.0f); glColor3f(0.2f, 1.0f, 0.4f); glBegin(GL_LINES);
		glVertex3f(0.28f, 0.01f, 0.28f); glVertex3f(0.72f, 0.01f, 0.72f);
		glVertex3f(0.28f, 0.01f, 0.72f); glVertex3f(0.72f, 0.01f, 0.28f); glEnd(); break;
	}
	glColor3f(0.3f, 0.8f, 0.8f);
	glBegin(GL_LINES);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(1.0f, 0.0f, 0.0f);
	glEnd();
	glBegin(GL_LINES);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 1.0f);
	glEnd();
	glBegin(GL_LINES);
	glVertex3f(1.0f, 0.0f, 0.0f);
	glVertex3f(1.0f, 0.0f, 1.0f);
	glEnd();
	glBegin(GL_LINES);
	glVertex3f(0.0f, 0.0f, 1.0f);
	glVertex3f(1.0f, 0.0f, 1.0f);
	glEnd();
	glPopMatrix();
}

// Hàm vẽ bản đồ mở rộng
void drawPathExtra(float x, float y, float z) {
	glPushMatrix();
	glColor4f(0.3f, 0.4f, 0.5f, y);
	glTranslatef(x, 0.0f, z);
	glBegin(GL_QUADS);
	glVertex3f(0.04f, 0.0f, 0.04f);
	glVertex3f(0.96f, 0.0f, 0.04f);
	glVertex3f(0.96f, 0.0f, 0.96f);
	glVertex3f(0.04f, 0.0f, 0.96f);
	glEnd();
	glColor4f(0.3f, 0.8f, 0.8f, y);
	glBegin(GL_LINES);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(1.0f, 0.0f, 0.0f);
	glEnd();
	glBegin(GL_LINES);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 1.0f);
	glEnd();
	glBegin(GL_LINES);
	glVertex3f(1.0f, 0.0f, 0.0f);
	glVertex3f(1.0f, 0.0f, 1.0f);
	glEnd();
	glBegin(GL_LINES);
	glVertex3f(0.0f, 0.0f, 1.0f);
	glVertex3f(1.0f, 0.0f, 1.0f);
	glEnd();
	glPopMatrix();
}

// Hàm vẽ các khối vuông
void drawGrid() {
	glLineWidth(2);
	for (i = 0; i < pLast; i++)
		drawPath(p[i].x, p[i].y, p[i].z);
}



// Hàm xử lí các mũi tên khi chơi
void specialPlay(int key, int xx, int yy) {
	if (Choice == -1)
		switch (key) {
		case GLUT_KEY_UP:
			rollFuncUpStart();//Mũi tên lên
			break;
		case GLUT_KEY_DOWN:
			rollFuncDownStart();//Mũi tên xuống
			break;
		case GLUT_KEY_LEFT:
			rollFuncLeftStart();//Mũi tên trái
			break;
		case GLUT_KEY_RIGHT:
			rollFuncRightStart();//Mũi tên phải
			break;
		}
}
// Hàm xây dựng bản đồ
void tPath(int value) {
	if (pFirst < pSum) {
		for (i = pFirst; i < pLast; i++) {
			if (p[i].y < -0.1f)
				p[i].y += 0.3f;
			else {
				pFirst++;
				p[i].y = 0;
			}
		}
		tSpawnPath++;
		if (tSpawnPath % 15 == 0)
			if (pLast < pSum)
				pLast++;
		glutPostRedisplay();
		glutTimerFunc(15, tFunc, 0);
	}
	else {
		hasBlox = 1;
		tFunc = &tBloxFall;
		glutTimerFunc(15, tFunc, 0);
	}
}

// Hàm vẽ khối hộp đang rơi trong lv9
void tBloxFall(int value) {
	if (cy > 0) {
		cy -= 0.5f;
		glutPostRedisplay();
		glutTimerFunc(15, tFunc, 0);
	}
	else {
		cy = 0.0f;
		tFunc = &tBloxRoll;
		glutKeyboardFunc(keyboardPlay);
		glutSpecialFunc(specialPlay);
	}
}

// Hàm lăn khối hộp
void tBloxRoll(int value) {
	if (n < 18) {
		LookAt += OL;
		angle += AngleOffset[Choice];
		n++;
		if (n == 18) {
			angle = 0.0f;
			RollFunc[Choice]();
			Choice = -1;
		}
		else glutTimerFunc(15, tFunc, 0);
		glutPostRedisplay();
	}
}

// Hàm win
void tWin(int value) {
	if (E.y > 0) {
		E.y = F.y = G.y = H.y = E.y - 0.08f;
		glutPostRedisplay();
		glutTimerFunc(25, tWinFunc, 0);
	}
	else {
		hasBlox = 0;
		glutPostRedisplay();
		tWinFunc = &tWin2;
		glutTimerFunc(0, tWinFunc, 0);
	}
}

void tWin2(int value) {
	if (stWin < 20) {
		stWin++;
		glutPostRedisplay();
		glutTimerFunc(25, tWinFunc, 0);
	}
	else {
		tWinFunc = &tRedirect;
		glutTimerFunc(500, tWinFunc, 0);
	}
}

// Hiệu ứng win
void tRedirect(int value) {
	tFunc = tNULL;
	glutKeyboardFunc(NULL);
	Map[level][StartY][StartX] = -1;
	for (k = 0; k < numExtra[level]; k++)
		Map[level][Gate[level][k].y][Gate[level][k].x] = 0;
	if (Win == 0) level++;
	if (level < numLv) initPlay();
	else initMenu();
}


void tExtra(int value) {
	ij = 0;
	for (ii = 0; ii < numExtra[level]; ii++) {
		if (offsetExtra[ii] != 0) {
			alphaExtra[ii] += 0.1f * offsetExtra[ii];
			if (alphaExtra[ii] < 0.95f && alphaExtra[ii] > 0.05f) ij = 1;
			else {
				offsetExtra[ii] = 0;
				markExtra[ii] = 1 - markExtra[ii];
				Map[level][Gate[level][ii].y][Gate[level][ii].x] = 1 - Map[level][Gate[level][ii].y][Gate[level][ii].x];
			}
		}
	}
	if (ij == 1) glutTimerFunc(25, tExtra, 0);
	glutPostRedisplay();
}

// Hàm kiểm tra có hoàn thành thao tác di chuyển chưa
void doneCheck() {
	j = PosX >> 1;
	i = PosY >> 1;
	if (PosX & 1) {
		if (Map[level][i][j] == 3) {
			if (!markExtra[extraMap[level][i][j]]) {
				offsetExtra[extraMap[level][i][j]] = 1;
				glutTimerFunc(0, tExtra, 0);
			}
		}
		else if (Map[level][i][j + 1] == 3) {
			if (!markExtra[extraMap[level][i][j + 1]]) {
				offsetExtra[extraMap[level][i][j + 1]] = 1;
				glutTimerFunc(0, tExtra, 0);
			}
		}
	}
	else if (PosY & 1) {
		if (Map[level][i][j] == 3) {
			if (!markExtra[extraMap[level][i][j]]) {
				offsetExtra[extraMap[level][i][j]] = 1;
				glutTimerFunc(0, tExtra, 0);
			}
		}
		else if (Map[level][i + 1][j] == 3) {
			if (!markExtra[extraMap[level][i + 1][j]]) {
				offsetExtra[extraMap[level][i + 1][j]] = 1;
				glutTimerFunc(0, tExtra, 0);
			}
		}
	}
	else {
		switch (Map[level][i][j]) {
		case 3: case 4: if (!markExtra[extraMap[level][i][j]]) { offsetExtra[extraMap[level][i][j]] = 1; glutTimerFunc(0, tExtra, 0); } break;
		case 5: if (!markExtra[extraMap[level][i][j]]) offsetExtra[extraMap[level][i][j]] = 1; else offsetExtra[extraMap[level][i][j]] = -1;
			glutTimerFunc(0, tExtra, 0); break;
		case 6: Win = 1; glutSpecialFunc(NULL); break;
		}
	}
}

// Xử lý khi bấm mũi tên lên
void rollFuncUpStart() {
	if (PosX & 1) {
		if (Map[level][(PosY >> 1) - 1][PosX >> 1] && Map[level][(PosY >> 1) - 1][(PosX >> 1) + 1]) {
			PosY -= 2;
			Choice = 0;
			n = 0;
			glutTimerFunc(0, tBloxRoll, 0);
		}
	}
	else {
		if (PosY & 1) {
			if (Map[level][(PosY >> 1) - 1][PosX >> 1]) {
				PosY -= 3;
				if (Map[level][PosY >> 1][PosX >> 1] == 2) {
					Win = 0;
					glutSpecialFunc(NULL);
				}
				Choice = 0;
				n = 0;
				glutTimerFunc(0, tBloxRoll, 0);
			}
		}
		else {
			if (Map[level][(PosY >> 1) - 1][PosX >> 1] && Map[level][(PosY >> 1) - 2][PosX >> 1]) {
				PosY -= 3;
				Choice = 0;
				n = 0;
				glutTimerFunc(0, tBloxRoll, 0);
			}
		}
	}
	OL = 0.0f;
}

// Đặt lại các giá trị khi bấm mũi tên lên
void rollFuncUpDone() {
	doneCheck();
	cz -= y;
	D.z = H.z = C.z = G.z = y;
	E.y = F.y = G.y = H.y = z;
	tmpF = y;
	y = z;
	z = tmpF;
	tmpP = CBack;
	CBack = CTop;
	CTop = CFront;
	CFront = CBottom;
	CBottom = tmpP;
	if (Win == 0) {
		A.y = B.y = C.y = D.y = -0.01f;
		glutTimerFunc(200, tWin, 0);
	}
	else if (Win == 1) glutTimerFunc(200, tWin, 0);
}

// Hàm bấm mũi tên xuống
void rollFuncDownStart() {
	if (PosX & 1) {
		if (Map[level][(PosY >> 1) + 1][PosX >> 1] && Map[level][(PosY >> 1) + 1][(PosX >> 1) + 1]) {
			PosY += 2;
			Choice = 2;
			n = 0;
			A.z = B.z = F.z = E.z = -z;
			D.z = C.z = G.z = H.z = 0;
			glutTimerFunc(0, tBloxRoll, 0);
		}
	}
	else {
		if (PosY & 1) {
			if (Map[level][(PosY >> 1) + 2][PosX >> 1]) {
				PosY += 3;
				if (Map[level][PosY >> 1][PosX >> 1] == 2) {
					Win = 0;
					glutSpecialFunc(NULL);
				}
				Choice = 2;
				n = 0;
				A.z = B.z = F.z = E.z = -z;
				D.z = C.z = G.z = H.z = 0;
				glutTimerFunc(0, tBloxRoll, 0);
			}
		}
		else {
			if (Map[level][(PosY >> 1) + 1][PosX >> 1] && Map[level][(PosY >> 1) + 2][PosX >> 1]) {
				PosY += 3;
				Choice = 2;
				n = 0;
				A.z = B.z = F.z = E.z = -z;
				D.z = C.z = G.z = H.z = 0;
				glutTimerFunc(0, tBloxRoll, 0);
			}
		}
	}
	OL = 0.0f;
}

// Đặt lại các giá trị khi bấm mũi tên xuống
void rollFuncDownDone() {
	doneCheck();
	cz += z;
	A.z = B.z = F.z = E.z = 0;
	D.z = C.z = G.z = H.z = y;
	E.y = F.y = G.y = H.y = z;
	tmpF = y;
	y = z;
	z = tmpF;
	tmpP = CFront;
	CFront = CTop;
	CTop = CBack;
	CBack = CBottom;
	CBottom = tmpP;
	if (Win == 0) {
		A.y = B.y = C.y = D.y = -0.01f;
		glutTimerFunc(200, tWin, 0);
	}
	else if (Win == 1) glutTimerFunc(200, tWin, 0);
}

// Bấm mũi tên trái
void rollFuncLeftStart() {
	if (PosY & 1) {
		if (Map[level][PosY >> 1][(PosX >> 1) - 1] && Map[level][(PosY >> 1) + 1][(PosX >> 1) - 1]) {
			PosX -= 2;
			Choice = 3;
			n = 0;
			glutTimerFunc(0, tBloxRoll, 0);
		}
	}
	else {
		if (PosX & 1) {
			if (Map[level][PosY >> 1][(PosX >> 1) - 1]) {
				PosX -= 3;
				if (Map[level][PosY >> 1][PosX >> 1] == 2) {
					Win = 0;
					glutSpecialFunc(NULL);
				}
				Choice = 3;
				n = 0;
				glutTimerFunc(0, tBloxRoll, 0);
			}
		}
		else {
			if (Map[level][PosY >> 1][(PosX >> 1) - 1] && Map[level][PosY >> 1][(PosX >> 1) - 2]) {
				PosX -= 3;
				Choice = 3;
				n = 0;
				glutTimerFunc(0, tBloxRoll, 0);
			}
		}
	}
	OL = 0.001f;
}

// Đặt lại các giá trị khi bấm mũi tên trái
void rollFuncLeftDone() {
	doneCheck();
	cx -= y;
	B.x = C.x = G.x = F.x = y;
	E.y = F.y = G.y = H.y = x;
	tmpF = y;
	y = x;
	x = tmpF;
	tmpP = CLeft;
	CLeft = CTop;
	CTop = CRight;
	CRight = CBottom;
	CBottom = tmpP;
	if (Win == 0) {
		A.y = B.y = C.y = D.y = -0.01f;
		glutTimerFunc(200, tWin, 0);
	}
	else if (Win == 1) glutTimerFunc(200, tWin, 0);
}

// Bám mũi tên phải
void rollFuncRightStart() {
	if (PosY & 1) {
		if (Map[level][PosY >> 1][(PosX >> 1) + 1] && Map[level][(PosY >> 1) + 1][(PosX >> 1) + 1]) {
			PosX += 2;
			Choice = 1;
			n = 0;
			A.x = D.x = E.x = H.x = -x;
			B.x = C.x = G.x = F.x = 0;
			glutTimerFunc(0, tBloxRoll, 0);
		}
	}
	else {
		if (PosX & 1) {
			if (Map[level][PosY >> 1][(PosX >> 1) + 2]) {
				PosX += 3;
				if (Map[level][PosY >> 1][PosX >> 1] == 2) {
					Win = 0;
					glutSpecialFunc(NULL);
				}
				Choice = 1;
				n = 0;
				A.x = D.x = E.x = H.x = -x;
				B.x = C.x = G.x = F.x = 0;
				glutTimerFunc(0, tBloxRoll, 0);
			}
		}
		else {
			if (Map[level][PosY >> 1][(PosX >> 1) + 1] && Map[level][PosY >> 1][(PosX >> 1) + 2]) {
				PosX += 3;
				Choice = 1;
				n = 0;
				A.x = D.x = E.x = H.x = -x;
				B.x = C.x = G.x = F.x = 0;
				glutTimerFunc(0, tBloxRoll, 0);
			}
		}
	}
	OL = -0.001f;
}
// Đặt lại các giá trị khi bấm mũi tên phải
void rollFuncRightDone() {
	doneCheck();
	cx += x;
	A.x = D.x = E.x = H.x = 0;
	B.x = C.x = G.x = F.x = y;
	E.y = F.y = G.y = H.y = x;
	tmpF = y;
	y = x;
	x = tmpF;
	tmpP = CRight;
	CRight = CTop;
	CTop = CLeft;
	CLeft = CBottom;
	CBottom = tmpP;
	if (Win == 0) {
		A.y = B.y = C.y = D.y = -0.01f;
		glutTimerFunc(200, tWin, 0);
	}
	else if (Win == 1) glutTimerFunc(200, tWin, 0);
}

