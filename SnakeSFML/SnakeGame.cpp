#include <SFML/Graphics.hpp>
#include<iostream>
#include <time.h>

using namespace sf;

enum Direction
{
	UP,
	LEFT,
	RIGHT,
	DOWN
};

int borderSize = 25;
Direction dir = Direction::RIGHT;
int amount = 1;
bool game = true;

struct Area
{
	int M, N;
	Area()
	{
		M = 20;
		N = 30;
	}
	Area(int M, int N)
	{
		this->M = M;
		this->N = N;
	}
};

struct Snake {

	int x, y;
};

struct Fruit

{

	int x, y;

};

Snake s[100];
Fruit f;
Area area;


void Logic() {

	//Changing position of a tails pieces
	for (int i = amount; i > 0; i--) {

		s[i].x = s[i - 1].x;

		s[i].y = s[i - 1].y;

	}
	//Changing direction
	if (dir == UP)

		s[0].y += 1;

	if (dir == DOWN)

		s[0].y -= 1;

	if (dir == RIGHT)

		s[0].x += 1;

	if (dir == LEFT)

		s[0].x -= 1;

	//Catching a fruit
	if ((s[0].x == f.x) && (s[0].y == f.y))
	{
		amount++;
		f.x = rand() % area.N;
		f.y = rand() % area.M;
	}

	//Moving beyond borders
	if (s[0].x > area.N - 1)
		s[0].x = 1;
	if (s[0].x < 0)
		s[0].x = area.N - 1;
	if (s[0].y > area.M - 1)
		s[0].y = 0;
	if (s[0].y < 0)
		s[0].y = area.M - 1;

	for (int i = 1; i < amount; i++)
	{
		if ((s[0].x == s[i].x) && (s[0].y == s[i].y))
			game = false;
	}

}

int main()
{
	srand(time(0));
	RenderWindow window(VideoMode((area.N * borderSize), (area.M * borderSize)), "Snake!");
	Texture br, sn, ap, sn1;
	Font font;
	font.loadFromFile("C:/Users/winmax/Desktop/C++/SnakeSFML/CyrilicOld.TTF");
	Text text("", font, 60);
	text.setStyle(Text::Bold);
	br.loadFromFile("border.png");
	sn.loadFromFile("snake.png");
	ap.loadFromFile("apple.png");
	sn1.loadFromFile("snake2.png");
	Sprite borders(br), snake(sn), apple(ap), snake2(sn1);

	f.x = rand() % (area.M - 25);
	f.y = rand() % (area.N - 25);

	Clock clock;

	double timer = 0, delay = 0.1;

	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();
		}

		float time = clock.getElapsedTime().asSeconds();
		clock.restart();
		timer += time;
		if (Keyboard::isKeyPressed(Keyboard::Left))
			dir = LEFT;
		if (Keyboard::isKeyPressed(Keyboard::Right))
			dir = RIGHT;
		if (Keyboard::isKeyPressed(Keyboard::Down))
			dir = UP;
		if (Keyboard::isKeyPressed(Keyboard::Up))
			dir = DOWN;


		if (timer > delay && game)   //delay 
		{
			timer = 0;
			Logic();
		}
		window.clear();
		for (int i = 0; i < area.N; i++)  //drawing borders
		{
			for (int j = 0; j < area.M; j++)
			{
				borders.setPosition(i * borderSize, j * borderSize);
				window.draw(borders);
			}
		}
		for (int i = 0; i < amount; i++)
		{
			if (i != 0)             //if not a head,we choose a first part of a .png with position (0,0)
				snake.setTextureRect(IntRect(0, 0, borderSize, borderSize));
			else                    //else we have a head,so we choose part of a .png which matches our direction
				snake.setTextureRect(IntRect(borderSize * dir, borderSize, borderSize, borderSize));

			if (!game && i == 1)    // if game ended
				snake.setTextureRect(IntRect(dir * borderSize, borderSize * 2, borderSize, borderSize));

			snake.setPosition(s[i].x * borderSize, s[i].y * borderSize);
			window.draw(snake);
		}
		apple.setPosition(f.x * borderSize, f.y * borderSize);
		window.draw(apple);
		if (!game)
		{
			text.setString("GAME OVER \n    Score: " + std::to_string(amount-1) + "\n ");
			text.setPosition((area.N / 2) * borderSize - 155, ((area.M / 2) * borderSize) - 125);
			window.draw(text);
		}
		window.display();
	}
	return 0;
}


