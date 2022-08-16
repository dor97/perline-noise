#include <SFML/Graphics.hpp>
#include"perlinNoise.h"
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void keyPressed(int& nOctavesCount, bool& updateA, bool& updateB, float& fScalingBais, char* chose, int& ind, int nOutPutSize, int nOutPutWidth, int nOutPutHight, float* fNoiseSeed1D, float* fNoiseSeed2D)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))	//num of octaves
	{
		++nOctavesCount;
		if (nOctavesCount == 9)
			nOctavesCount = 1;
		updateA = true; updateB = true;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z) && chose[ind] == 'a')
	{
		for (int i = 0; i < nOutPutSize; ++i)	//fill a 1 D noise array with random values
			fNoiseSeed1D[i] = (float)rand() / (float)RAND_MAX;
		updateA = true;	//need to update the 1 D noise seed array
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z) && (chose[ind] == 'b' || chose[ind] == 'c'))
	{
		for (int i = 0; i < nOutPutWidth * nOutPutHight; ++i)	//fill a 2 D noise array with random values
			fNoiseSeed2D[i] = (float)rand() / (float)RAND_MAX;
		updateB = true;	//need to update the 2 D noise seed array
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		fScalingBais -= 0.2;	//decreasing the amount we divide every octave
		if (fScalingBais == 0)
			fScalingBais = 0.2;
		updateA = true; updateB = true;	//need to update the noise seed array
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		fScalingBais += 0.2;	//increasing the amount we divide every octave
		updateA = true; updateB = true;	//need to update the noise seed array
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
		ind = (ind + 1) % 3;	//moving to the next perlin noise option
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//set a rectangle color according to the color from the perlin noise array
void setColor(sf::RectangleShape& rect, int color)
{
	if (color < 100)
		rect.setFillColor(sf::Color::Color(color, color, color + 60, 255));
	else
		if (color < 200)
		{
			color -= 100;
			rect.setFillColor(sf::Color::Color(color, color + 60, color, 255));
		}
		else
		{
			color -= 45;
			rect.setFillColor(sf::Color::Color(color, color, color, 255));
		}
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int main()
{
	constexpr int width = 768, height = 768;
	sf::RenderWindow window(sf::VideoMode(width, height), "perlinNoise");
	window.setFramerateLimit(60);
	float* fNoiseSeed1D = nullptr, *fPerLinNoise1D = nullptr;
	int nOutPutWidth = 256, nOutPutHight = 256;
	float* fNoiseSeed2D = nullptr, *fPerLinNoise2D = nullptr;
	int nOutPutSize = 256;
	int nOctavesCount = 1;
	float fScalingBais = 2.0f;
	bool updateA = true, updateB = true;
	srand(clock());
	char chose[3] = { 'a', 'b', 'c'};
	int ind = 0;
	perlinNoise p;

	fNoiseSeed1D = new float[nOutPutSize];
	fPerLinNoise1D = new float[nOutPutSize];

	fNoiseSeed2D = new float[nOutPutWidth * nOutPutHight];
	fPerLinNoise2D = new float[nOutPutWidth * nOutPutHight];

	if(chose[ind] == 'a')
		for (int i = 0; i < nOutPutSize; ++i)
			fNoiseSeed1D[i] = (float)rand() / (float)RAND_MAX;
	if (chose[ind] == 'b' || chose[ind] == 'c')
		for (int i = 0; i < nOutPutWidth * nOutPutHight; ++i)
			fNoiseSeed2D[i] = (float)rand() / (float)RAND_MAX;

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed) window.close();

			if (event.type == sf::Event::KeyPressed)
			{
				keyPressed(nOctavesCount, updateA, updateB, fScalingBais, chose, ind, nOutPutSize, nOutPutWidth, nOutPutHight, fNoiseSeed1D, fNoiseSeed2D);
			}

		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) window.close();
		window.clear(sf::Color::Color(100, 100, 100, 255));

		if (chose[ind] == 'a')	//1 D perline noise
		{
			if (!fPerLinNoise1D || updateA)
			{
				p.perLinNoise(nOutPutSize, fNoiseSeed1D, fScalingBais, nOctavesCount, fPerLinNoise1D);
				updateA = false;
			}
			for (int i = 0; i < nOutPutSize; ++i)
			{
				sf::RectangleShape rect(sf::Vector2f(3, fPerLinNoise1D[i] * height / 2));
				rect.setPosition(i * 3, height / 2 - fPerLinNoise1D[i] * height / 2);
				rect.setFillColor(sf::Color::Green);
				window.draw(rect);
				
			}
		}
		if (chose[ind] == 'b')	// 2 D black and white perline noise
		{
			if (!fPerLinNoise2D || updateB)
			{
				p.perLinNoise2D(nOutPutWidth, nOutPutHight, fNoiseSeed2D, fScalingBais, nOctavesCount, fPerLinNoise2D);
				updateB = false;
			}
			for (int x = 0; x < nOutPutWidth; ++x)
				for (int y = 0; y < nOutPutHight; ++y)
				{
					sf::RectangleShape ract(sf::Vector2f(3, 3));
					ract.setPosition(x * 3, y * 3);
					int color = (int)(fPerLinNoise2D[y * nOutPutWidth + x] * 255);
					ract.setFillColor(sf::Color::Color(color, color, color, 255));
					window.draw(ract);
				}
		}
		if (chose[ind] == 'c')	// 2 D colored perline noise
		{
			if (!fPerLinNoise2D || updateB)
			{
				p.perLinNoise2D(nOutPutWidth, nOutPutHight, fNoiseSeed2D, fScalingBais, nOctavesCount, fPerLinNoise2D);
				updateB = false;
			}
			for (int x = 0; x < nOutPutWidth; ++x)
				for (int y = 0; y < nOutPutHight; ++y)
				{
					sf::RectangleShape rect(sf::Vector2f(3, 3));
					rect.setPosition(x * 3, y * 3);
					int color = (int)(fPerLinNoise2D[y * nOutPutWidth + x] * 300);
					setColor(rect, color);
					window.draw(rect);
				}
		}

		window.display();
	}

	delete[] fNoiseSeed1D;
	delete[] fPerLinNoise1D;
	delete[] fNoiseSeed2D;
	delete[] fPerLinNoise2D;

	return 0;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------