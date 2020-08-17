// FlappyBird.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include "Windows.h"
#include "chrono"
#include "vector"
#include <ctime>
#include <string>

#define PIPE_DELAY 800

using namespace std;
int screenWidth = 120;
int screenHeight = 40;
bool game = true;
bool playagain = false;
CHAR_INFO *screen = new CHAR_INFO[screenWidth*screenHeight];
struct pipe;
vector<pipe> pipes(1);
int score = 0;
int highscore = 0;
int score_check = 0;

enum COLOUR
{
	FG_BLACK = 0x0000,
	FG_DARK_BLUE = 0x0001,
	FG_DARK_GREEN = 0x0002,
	FG_DARK_CYAN = 0x0003,
	FG_DARK_RED = 0x0004,
	FG_DARK_MAGENTA = 0x0005,
	FG_DARK_YELLOW = 0x0006,
	FG_GREY = 0x0007,
	FG_DARK_GREY = 0x0008,
	FG_BLUE = 0x0009,
	FG_GREEN = 0x000A,
	FG_CYAN = 0x000B,
	FG_RED = 0x000C,
	FG_MAGENTA = 0x000D,
	FG_YELLOW = 0x000E,
	FG_WHITE = 0x000F,
	BG_BLACK = 0x0000,
	BG_DARK_BLUE = 0x0010,
	BG_DARK_GREEN = 0x0020,
	BG_DARK_CYAN = 0x0030,
	BG_DARK_RED = 0x0040,
	BG_DARK_MAGENTA = 0x0050,
	BG_DARK_YELLOW = 0x0060,
	BG_GREY = 0x0070,
	BG_DARK_GREY = 0x0080,
	BG_BLUE = 0x0090,
	BG_GREEN = 0x00A0,
	BG_CYAN = 0x00B0,
	BG_RED = 0x00C0,
	BG_MAGENTA = 0x00D0,
	BG_YELLOW = 0x00E0,
	BG_WHITE = 0x00F0,
};

enum PIXEL_TYPE
{
	PIXEL_SOLID = 0x2588,
	PIXEL_THREEQUARTERS = 0x2593,
	PIXEL_HALF = 0x2592,
	PIXEL_QUARTER = 0x2591,
};


//Initialize Bird
float xpos = (float)(screenWidth / 3.0f);
float ypos = float(screenHeight) / 2.0f;
float velocity = 0.0f;
float lift = 30.0f;
float gravity = 100.0f;

//Initialize Pipe
struct pipe {
	float xpipe = screenWidth;
	float wpipe = 10.0;
	float gapheight = 10.0;
	float gap = (rand() % screenHeight);
	float speed = 20;
};

int t = 0;
int p = 0; //iterator for pipe vector


static const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
void setcursor(bool visible, DWORD size) // set bool visible = 0 - invisible, bool visible = 1 - visible
{
	if (size == 0)
	{
		size = 20;          // default cursor size Changing to numbers from 1 to 20, decreases cursor width
	}
	CONSOLE_CURSOR_INFO lpCursor;
	lpCursor.bVisible = visible;
	lpCursor.dwSize = size;
	SetConsoleCursorInfo(hOut, &lpCursor);
}

void fullscreen()
{
	keybd_event(VK_MENU, 0x38, 0, 0);
	keybd_event(VK_RETURN, 0x1c, 0, 0);
	keybd_event(VK_RETURN, 0x1c, KEYEVENTF_KEYUP, 0);
	keybd_event(VK_MENU, 0x38, KEYEVENTF_KEYUP, 0);
}

/*
void highlight() {
	//highlight pipe (debug)
	for (int i = 0; i < pipes.size(); i++) {
		for (int x = pipes.at(i).xpipe - pipes.at(i).wpipe; x < pipes.at(i).xpipe; x++) {
			for (int y = pipes.at(i).gap; y < screenHeight; y++) {
				screen[y * screenWidth + x].Attributes = FG_RED;
				screen[y * screenWidth + x].Char.UnicodeChar = PIXEL_SOLID;
			}
		}

		//draw top pipe
		for (int x = pipes.at(i).xpipe - pipes.at(i).wpipe; x < pipes.at(i).xpipe; x++) {
			for (int y = 0; y < pipes.at(i).gap - pipes.at(i).gapheight; y++) {
				screen[y * screenWidth + x].Attributes = FG_RED;
				screen[y * screenWidth + x].Char.UnicodeChar = PIXEL_SOLID;
			}
		}
	}
}
*/

/*
void debugCol() {
	if (GetAsyncKeyState(VK_UP) & 0x8000)
	{
		//// low bit is set.  Key just transitioned from released to pressed.
		ypos -= .1;
		gravity = 0;
	}

	if (GetAsyncKeyState(VK_DOWN) & 0x8000)
	{
		//// low bit is set.  Key just transitioned from released to pressed.
		ypos += .1;
		gravity = 0;
	}

	if (GetAsyncKeyState(VK_LEFT) & 0x8000)
	{
		//// low bit is set.  Key just transitioned from released to pressed.
		xpos -= .1;
		gravity = 0;
	}

	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
	{
		//// low bit is set.  Key just transitioned from released to pressed.
		xpos += .1;
		gravity = 0;
	}

	if (GetAsyncKeyState(VK_LSHIFT) & 0x8000)
	{
		//// low bit is set.  Key just transitioned from released to pressed.
		for (int i = 0; i < pipes.size(); i++) {
			pipes.at(i).speed = 20;
			t = 800;
		}
	}

	if (GetAsyncKeyState(VK_RSHIFT) & 0x8000)
	{
		//// low bit is set.  Key just transitioned from released to pressed.
		for (int i = 0; i < pipes.size(); i++) {
			pipes.at(i).speed = 0;
			t += 100000000000;
		}
	}
	
}
*/

int main()
{

	srand(time(0));
	setcursor(0, 1);

	memset(screen, 0, sizeof(CHAR_INFO) * screenWidth * screenHeight);
	HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(hConsole);
	DWORD dwBytesWritten = 0;
	SMALL_RECT rectWindow = { 0, 0, screenWidth, screenHeight };
	SetConsoleWindowInfo(hConsole, TRUE, &rectWindow);
	auto tp1 = chrono::system_clock::now();
	auto tp2 = chrono::system_clock::now();


	while (game) {

		//Internal system time difference so that speed is the same for all computer speeds
		tp2 = chrono::system_clock::now();
		chrono::duration<float> elapsedTime = tp2 - tp1;
		tp1 = tp2;
		float fElapsedTime = elapsedTime.count();

		//acceleration
		if (velocity < 1500.0f * fElapsedTime) {
			velocity += gravity * fElapsedTime;
			ypos += velocity * fElapsedTime;
		}
		else {
			ypos += (gravity / 9.0f) * fElapsedTime;
		}

		//Quit game (ESC)
		if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
			game = false;
			break;
		}

		//jump (SPACE)
		if (GetAsyncKeyState(VK_SPACE) & 0x8000)
		{
			//// low bit is set.  Key just transitioned from released to pressed.
			velocity = -lift;
		}

		//clear screen
		for (int x = 0; x < screenWidth; x++) {
			for (int y = 0; y < screenHeight; y++) {
				screen[y * screenWidth + x].Char.UnicodeChar = L' ';
			}
		}


		//draw all pipes on screen
		for (int i = 0; i <= p; i++) {
			while (pipes.at(i).gap < 10 || pipes.at(i).gap > screenHeight - 10) {
				pipes.at(i).gap = (rand() % screenHeight);
			}
			//draw bottom pipe
			for (int x = (pipes.at(i).xpipe - pipes.at(i).wpipe); x < pipes.at(i).xpipe; x++) {
				for (int y = pipes.at(i).gap; y < screenHeight; y++) {
					screen[y * screenWidth + x].Char.UnicodeChar = PIXEL_SOLID;
					screen[y * screenWidth + x].Attributes = FG_GREEN;
				}
			}

			//draw gap
			for (int x = pipes.at(i).xpipe - 3; x < pipes.at(i).xpipe - 3; x++) {
				for (int y = pipes.at(i).gap - pipes.at(i).gapheight; y < pipes.at(i).gap; y++) {
					screen[y * screenWidth + x].Char.UnicodeChar = L'/';
					screen[y * screenWidth + x].Attributes = FG_BLACK;
				}
			}

			//draw top pipe
			for (int x = (pipes.at(i).xpipe - pipes.at(i).wpipe); x < pipes.at(i).xpipe; x++) {
				for (int y = 0; y < (pipes.at(i).gap - pipes.at(i).gapheight); y++) {
					screen[y * screenWidth + x].Char.UnicodeChar = PIXEL_SOLID;
					screen[y * screenWidth + x].Attributes = FG_GREEN;
				}
			}
		}

		/*pipes*/
		t++;
		cout << "t: " << t << endl;
		if (t == 1000) {
			p++;
			pipe temp;
			pipes.push_back(temp);
			t = 0;
		}

		//move all pipes on screen
		for (int i = 0; i < pipes.size(); i++) {
			pipes.at(i).xpipe -= pipes.at(i).speed * fElapsedTime;
		}

		//erase pipe when it reaches the end
		for (int i = 0; i < pipes.size(); i++) {
			if (pipes.at(i).xpipe <= pipes.at(i).wpipe) {
				pipes.erase(pipes.begin() + i);
				p--;
			}
		}

		int xpos = (int)(screenWidth / 3.0f);

		//bird collision
		if (screen[(int)(ypos + 0) * screenWidth + (int)xpos].Char.UnicodeChar == PIXEL_SOLID
			|| screen[(int)(ypos + 0) * screenWidth + (int)xpos + 2].Char.UnicodeChar == PIXEL_SOLID) {
			//clear
			for (int x = 0; x < screenWidth; x++) {
				for (int y = 0; y < screenHeight; y++) {
					screen[y * screenWidth + x].Char.UnicodeChar = L' ';
				}
			}
			playagain = true;
		}

		//Score
		if (screen[(int)(ypos)* screenWidth + (int)xpos + 3].Char.UnicodeChar == L'/') {
			//score only increments by 1
			if (score_check != 1) {
				score++;
				score_check++;
			}
		}
		if (screen[(int)(ypos)* screenWidth + (int)xpos].Char.UnicodeChar == L'/') {
			score_check = 0;
		}


		if (score >= highscore) {
			highscore = score;
		}

		wstring wScore = L"Score: " + to_wstring(score) + L"   ";
		wstring whighscore = L"High Score: " + to_wstring(highscore) + L"   ";

		//print scores
		for (int x = 0; x < 11; x++) {
			for (int y = 0; y < 1; y++) {
				screen[(y + 20) * screenWidth + x].Attributes = FG_WHITE;
				screen[(y + 20) * screenWidth + x].Char.UnicodeChar = wScore[y * 11 + x];
			}
		}

		for (int x = 0; x < 17; x++) {
			for (int y = 0; y < 1; y++) {
				screen[(y + 21) * screenWidth + x].Attributes = FG_WHITE;
				screen[(y + 21) * screenWidth + x].Char.UnicodeChar = whighscore[y * 17 + x];
			}
		}

		//Ensure that bird stays on screen
		if (ypos >= screenHeight - 1) {
			ypos = screenHeight - 1;
		}

		if (ypos <= 0) {
			ypos = 0;
		}

		//draw bird
		screen[int(ypos) * screenWidth + xpos].Attributes = FG_YELLOW;
		screen[int(ypos) * screenWidth + xpos].Char.UnicodeChar = PIXEL_SOLID;
		screen[int(ypos) * screenWidth + xpos + 1].Attributes = FG_YELLOW;
		screen[int(ypos) * screenWidth + xpos + 1].Char.UnicodeChar = PIXEL_SOLID;
		screen[int(ypos) * screenWidth + xpos + 2].Attributes = FG_YELLOW;
		screen[int(ypos) * screenWidth + xpos + 2].Char.UnicodeChar = '>';

		if (playagain) {

			//clear and reset
			for (int x = 0; x < screenWidth; x++) {
				for (int y = 0; y < screenHeight; y++) {
					screen[y * screenWidth + x].Char.UnicodeChar = L' ';
				}
			}

			pipes.clear();
			pipe temp;
			pipes.insert(pipes.begin(), temp);
			t = 0;
			p = 0;
			score_check = 0;
			score = 0;
			velocity = 0.0f;
			ypos = screenHeight / 2.0f;
			xpos = (float)(screenWidth / 3.0f);
			string option;
			wstring gameover;
			gameover += L" ------------------------------------------------------------------------- ";
			gameover += L"|    *****      *     *       * ******       ****  *       ****** ****    |";
			gameover += L"|   *          * *    * *   * * *           *    *  *     * *     *   *   |";
			gameover += L"|   *  ****   *   *   *  * *  * *****       *    *   *   *  ****  ****    |";
			gameover += L"|   *  *  *  *******  *   *   * *           *    *    * *   *     * *     |";
			gameover += L"|    *****  *       * *       * ******       ****      *    ***** *   *   |";
			gameover += L" ------------------------------------------------------------------------- ";
			gameover += L"                                                                           ";
			gameover += L"                         By John Alcantara                                 ";
			gameover += L"                                                                           ";
			gameover += L"                         Play Again?: (Y/N)                                ";

			for (int x = 0; x < 17; x++) {
				for (int y = 0; y < 1; y++) {
					screen[(y + 15) * screenWidth + x].Attributes = FG_WHITE;
					screen[(y + 15) * screenWidth + x].Char.UnicodeChar = whighscore[y * 17 + x];
				}
			}

			for (int x = 0; x < 75; x++) {
				for (int y = 0; y < 11; y++) {
					screen[(y + 10) * screenWidth + (x + 30)].Attributes = FG_WHITE;
					screen[(y + 10) * screenWidth + (x + 30)].Char.UnicodeChar = gameover[y * 75 + x];
				}
			}

			if (GetAsyncKeyState((unsigned short)'Y') & 0x8000) {
				playagain = false;
			}

			if (GetAsyncKeyState((unsigned short)'N') & 0x8000) {
				game = false;
				break;
			}

			if (GetAsyncKeyState(VK_ESCAPE)) {
				break;
			}

			screen[screenWidth * screenHeight - 1].Char.UnicodeChar = '\0';
			WriteConsoleOutput(hConsole, screen, { (short)screenWidth, (short)screenHeight }, { 0,0 }, &rectWindow);
		}

		//Create Screen Buffer
		screen[screenWidth * screenHeight - 1].Char.UnicodeChar = '\0';
		WriteConsoleOutput(hConsole, screen, { (short)screenWidth, (short)screenHeight }, { 0,0 }, &rectWindow);
	}
	return 0;
}