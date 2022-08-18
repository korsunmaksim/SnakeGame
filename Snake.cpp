#include<iostream>
#include<string>
#include<algorithm>
#include<thread>
#include<conio.h>
#include<fstream>
#include<vector>
char border = static_cast<char>(219);


enum Direction
{
	STOP,
	UP,
	DOWN,
	LEFT,
	RIGHT
};

class Area
{
public:
	int height, width;
	Area(int Height, int Width)
	{
		this->height = Height;
		this->width = Width;
	}
};


class Fruit
{
public:
	int X;
	int Y;
	char symb = 'F';
	Fruit(const Area& area)
	{
		this->X = rand() % (area.width - 1);
		this->Y = rand() % (area.height - 1);
	}

};



class Snake
{
public:
	char head = static_cast<char>(1);
	int X, Y;
	int nTail = 0;
	int* tailX;
	int* tailY;
	bool gameOver;
	Direction dir;
	int score = 0;
	Snake(const Area& area)
	{
		this->X = area.width / 2 - 1;
		this->Y = area.height / 2 - 1;
		this->gameOver = false;
		this->dir = STOP;
		tailX = new int[20];
		tailY = new int[10];
	}

};



void Draw(const Area& area, const Snake& snake, const Fruit& fruit)
{
	system("cls");
	for (size_t i = 0; i <= area.width; i++)
	{
		std::cout << border;
	}
	std::cout << std::endl;
	for (size_t i = 0; i < area.height; i++)
	{
		for (size_t j = 0; j < area.width; j++)
		{
			if (j == 0 || j == area.width - 1) std::cout << border;
			if (j == snake.X && i == snake.Y) std::cout << snake.head;
			else if (j == fruit.X && i == fruit.Y) std::cout << fruit.symb;
			else
			{
				bool printSpace = true;
				for (size_t l = 0; l < snake.nTail; l++)
				{
					if (snake.tailX[l] == j && snake.tailY[l] == i)
					{
						std::cout << 'o';
						printSpace = false;
					}
				}
				if (printSpace) std::cout << " ";
			}
		}
		std::cout << std::endl;
	}
	for (size_t i = 0; i <= area.width; i++)
	{
		std::cout << border;
	}
	std::cout << std::endl;
	std::cout << "Score:" << snake.score;
}

void Input(char k, Snake& snake)
{
	switch (k)
	{
	case 'w':
		snake.dir = UP;
		break;
	case 's':
		snake.dir = DOWN;
		break;
	case 'a':
		snake.dir = LEFT;
		break;
	case 'd':
		snake.dir = RIGHT;
		break;
	case 'x':
		snake.gameOver = true;
		break;
	}
}

void Logic(Snake& snake, Area& area, Fruit& fruit)
{
	int prevX = snake.tailX[0];
	int prevY = snake.tailY[0];
	int currentX;
	int currentY;
	snake.tailX[0] = snake.X;
	snake.tailY[0] = snake.Y;
	for (size_t i = 1; i < snake.nTail; i++)
	{
		currentX = snake.tailX[i];
		snake.tailX[i] = prevX;
		prevX = currentX;
		currentY = snake.tailY[i];
		snake.tailY[i] = prevY;
		prevY = currentY;
	}
	switch (snake.dir)
	{
	case UP:
		snake.Y--;
		break;
	case DOWN:
		snake.Y++;
		break;
	case LEFT:
		snake.X--;
		break;
	case RIGHT:
		snake.X++;
		break;
	}
	for (size_t i = 0; i < snake.nTail; i++)
	{
		if (snake.tailX[i] == snake.X && snake.tailY[i] == snake.Y)
			snake.gameOver = true;
	}
	if (snake.X >= area.width - 1) snake.X = 0;
	else if (snake.X < 0) snake.X = area.width - 2;
	if (snake.Y >= area.height) snake.Y = 0;
	else if (snake.Y < 0) snake.Y = area.height - 1;
	if (snake.X == fruit.X && snake.Y == fruit.Y)
	{
		snake.score++;
		snake.nTail++;
		fruit.X = rand() % (area.width - 1);
		fruit.Y = rand() % (area.height - 1);
	}

}

void GameStart(Area& area, Snake& snake, Fruit& fruit)
{
	while (!snake.gameOver)
	{
		Draw(area, snake, fruit);
		if (_kbhit)
		{
			char c = _getch();
			Input(c, snake);
			Logic(snake, area, fruit);
			while (true)
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(40));
				Draw(area, snake, fruit);
				Input(c, snake);
				Logic(snake, area, fruit);
				if (_kbhit())break;
			}
		}
	}
	std::cout << std::endl;
}

void CheckRecord(Snake& snake)
{
	std::string fileName = "Results.txt";
	std::vector<int>results;
	std::ofstream fOut;
	std::ifstream fIn;
	fOut.open(fileName, std::ofstream::app);
	fOut << snake.score << std::endl;
	fOut.close();
	fIn.open(fileName);
	int tmp;
	while (!fIn.eof())
	{
		fIn >> tmp;
		results.push_back(tmp);
	}
	fIn.close();
	std::cout << "\n\n";

	if (auto lastResult = results.end() - 2 == std::max_element(results.begin(), results.end()))
	{
		std::cout << "New Record !!!!! :    " << snake.score;
	}
	else
	{
		std::cout << "Result:" << snake.score;
	}
	std::cout << "\n";
}

int main()
{
	Area area(20,30);
	Snake snake(area);
	Fruit fruit(area);
	GameStart(area, snake, fruit);
	CheckRecord(snake);
}

