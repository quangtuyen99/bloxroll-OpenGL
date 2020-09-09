#pragma once
void importMap();
void initOffset();
void initMap();
void initTextLevel();
void initMenu();
void initPlay();
void initBlox();

void keyboardMenu(unsigned char, int, int);
void keyboardSL(unsigned char, int, int);
void keyboardPlay(unsigned char, int, int);
void keyboardSkip(unsigned char, int, int);
void specialMenu(int, int, int);
void specialSL(int, int, int);
void specialPlay(int, int, int);

void tLogoIn(int);
void tTextAppear(int);
void tMenu(int);
void tZoomIn(int);
void tZoomOut(int);
void tPath(int);
void tBloxFall(int);
void tBloxRoll(int);
void tChangeView(int);
void tWin(int);
void tWin2(int);
void tRedirect(int);
void tExtra(int);

void displayMenu();
void displaySL();
void displayPlay();
void drawBlox();
void drawPathSL(float, float, int);
void drawPath(float, float, float);
void drawPathExtra(float, float, float);
void drawGrid();

void doneCheck();
void rollFuncUpStart();
void rollFuncDownStart();
void rollFuncLeftStart();
void rollFuncRightStart();
void rollFuncUpDone();
void rollFuncDownDone();
void rollFuncLeftDone();
void rollFuncRightDone();
