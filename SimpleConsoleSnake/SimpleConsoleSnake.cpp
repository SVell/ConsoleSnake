// SimpleConsoleSnake.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <Windows.h>
#include <list>
#include <thread>

using namespace std;

int nScreenWidth = 120;
int nScreenHeight = 30;

struct sSnakeSegment
{
	int x;
	int y;
};

int main()
{
	while(1)
	{
		// Create Screen Buffer
		wchar_t* screen = new wchar_t[nScreenWidth * nScreenHeight];
		HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
		SetConsoleActiveScreenBuffer(hConsole);
		DWORD dwBytesWritten = 0;

		list<sSnakeSegment> snake = { {60,15},{61,15},{62,15},{63,15},{64,15},{65,15},{66,15},{67,15},{68,15},{69,15} };
		int nFoodX = 30;
		int nFoodY = 15;
		int nScore = 0;
		int nSnakeDirection = 3;
		bool isDead = false;

		bool keyLeft = false;
		bool keyRight = false;
		bool keyLeftOld = false;
		bool keyRightOld = false;


		// Game loop
		while (!isDead)
		{
			// Timing & Input
			//this_thread::sleep_for(200ms);

			
			// Get Input
			auto t1 = chrono::system_clock::now();
			while((chrono::system_clock::now() - t1) < ((nSnakeDirection % 2 == 1) ? 120ms : 200ms))
			{
				keyLeft = (0x8000 & GetAsyncKeyState((unsigned char)('\x25'))) != 0;
				keyRight = (0x8000 & GetAsyncKeyState((unsigned char)('\x27'))) != 0;

				
			}

			if (keyRight && !keyRightOld)
			{
				nSnakeDirection++;
				if (nSnakeDirection == 4)
					nSnakeDirection = 0;
			}

			if (keyLeft && !keyLeftOld)
			{
				nSnakeDirection--;
				if (nSnakeDirection == -1)
					nSnakeDirection = 3;
			}

			// Game Logic
			// Move Head
			switch (nSnakeDirection)
			{
			case 0: // Up
				snake.push_front({ snake.front().x,snake.front().y - 1 });
				break;
			case 1: // Right
				snake.push_front({ snake.front().x + 1,snake.front().y });
				break;
			case 2: // Down
				snake.push_front({ snake.front().x,snake.front().y + 1 });
				break;
			case 3: // Left
				snake.push_front({ snake.front().x - 1,snake.front().y });
				break;
			}

			// Collision Detection
			if (snake.front().x < 0 || snake.front().x > nScreenWidth)
			{
				isDead = true;
			}
			if (snake.front().y < 3 || snake.front().y > nScreenHeight)
			{
				isDead = true;
			}
			if (snake.front().x == nFoodX && snake.front().y == nFoodY)
			{
				nScore++;
				while (screen[nFoodY * nScreenWidth + nFoodX] != L' ')
				{
					nFoodX = rand() % nScreenWidth;
					nFoodY = (rand() % (nScreenHeight - 3)) + 3;
				}

				for (int i = 0; i < 5; i++)
				{
					snake.push_back({ snake.back().x,snake.back().y });
				}
			}

			// Collision Detection Snake vs Snake
			for (list<sSnakeSegment>::iterator i = snake.begin(); i != snake.end(); i++)
			{
				if (i != snake.begin() && i->x == snake.front().x && i->y == snake.front().y)
				{
					isDead = true;
				}
			}


			// Move Tail
			snake.pop_back();



			// Display to Player
			// Clear Screen
			for (int i = 0; i < nScreenHeight * nScreenWidth; i++)
			{
				screen[i] = L' ';
			}

			// Draw Stats & Border
			for (int i = 0; i < nScreenWidth; i++)
			{
				screen[i] = L'=';
				screen[2 * nScreenWidth + i] = L'=';
			}
			wsprintf(&screen[nScreenWidth + 5], L"SellBro Snake Game                                    SCORE: %d", nScore);

			// Draw Snake Body
			for (auto s : snake)
				screen[s.y * nScreenWidth + s.x] = isDead ? L'+' : L'O';

			// Draw Snake Head
			screen[snake.front().y * nScreenWidth + snake.front().x] = isDead ? L'X' : L'@';

			// Draw Food
			screen[nFoodY * nScreenWidth + nFoodX] = L'%';

			if (isDead)
				wsprintf(&screen[15 * nScreenWidth + 40], L"PRESS 'SPACE' TO PLAY AGAIN");


			screen[nScreenWidth * nScreenHeight - 1] = '\0';
			WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth * nScreenHeight, { 0,0 }, &dwBytesWritten);
		}
		
		while ((0x8000 & GetAsyncKeyState((unsigned char)('\x20'))) == 0);
	}
	return 0;
}
