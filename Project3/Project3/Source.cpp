#include<SFML/Graphics.hpp>
#include<iostream>
#include<SFML\Window.hpp>
#include<string>
#include <ctime>
#include <windows.h>
#include <time.h>

#include "Menu1.h"
#include <opencv2/core/core.hpp>  
#include <opencv2/highgui/highgui.hpp>  
#include <opencv2/imgproc/imgproc.hpp>  
#include "backgroundSubstractor.h"

class timer {
private:
	unsigned long begTime;
public:
	void start() {
		begTime = clock();
	}

	unsigned long elapsedTime() {
		return ((unsigned long)clock() - begTime) / CLOCKS_PER_SEC;
	}

	bool isTimeout(unsigned long seconds) {
		return seconds >= elapsedTime();
	}
};


int main() {
	int DimensionX = 1920;
	int DimensionY = 1080;
	


	//timers
	timer backgTimer, playTimer, preTimer,t;

	unsigned long backSeconds = 5;
	unsigned long playSeconds = 15;
	unsigned long startSeconds = 3;
	//float movespeed = 10000.0f;
	bool MenuStat = true;
	bool ViewStat = true;
	
	//making the menu
	Menu menu(DimensionX, DimensionY);
	//Menu menua();

	backgroundSubstractor bg;
	
	cv::Mat	src = cv::imread("5.jpg", 1);
	cv::Mat	dst = cv::imread("9.jpg", 1);
	cv::imshow("bla", src);


	
	sf::Texture bTexture;
	sf::Sprite bImage;


	sf::Vector2i screenDimensions(DimensionX, DimensionY);
	sf::RenderWindow window1(sf::VideoMode(screenDimensions.x, screenDimensions.y), "my second");
	window1.setVisible(false);
	sf::RenderWindow window(sf::VideoMode(screenDimensions.x, screenDimensions.y), "my first", sf::Style::None);

	cv::Mat imageback, image;
	cv::VideoCapture capBackG(0);
	cv::VideoCapture capPlay(0);


	

	if (!bTexture.loadFromFile("1.jpg"))
		std::cout << "Couln't load image" << std::endl;

	bImage.setTexture(bTexture);

	//	bImage.setScale(sf::Vector2f(1.0f,1.0f));
	sf::View Gameview;
	sf::View Menu;
	Gameview.setViewport(sf::FloatRect(0, 0, 1.0f, 1.0f));
	Gameview.setSize(screenDimensions.x, screenDimensions.y);
	Menu.setViewport(sf::FloatRect(0, 0, 1.0f, 1.0f));
	Menu.setSize(screenDimensions.x, screenDimensions.y);


	sf::Vector2f position(screenDimensions.x / 2, screenDimensions.y / 2);
	bImage.setOrigin(bImage.getGlobalBounds().width / 2, bImage.getGlobalBounds().height / 2);
	bImage.setPosition(position);

	//sf::Clock clock;
	//	Gameview.zoom(2.0f);
	
	while (window.isOpen())
	{
		//clock.restart();
		sf::Event Event;
		while (window.pollEvent(Event))
		{
			switch (Event.type)
			{
				window.close();
				break;
			case sf::Event::KeyPressed:
				if (Event.key.code == sf::Keyboard::Escape) {
					ViewStat = true;
					MenuStat = true;
				}
				break;
			case sf::Event::KeyReleased:
				switch (Event.key.code)
				{
				case sf::Keyboard::Up:
					menu.MoveUp();
					break;
				case sf::Keyboard::Down:
					menu.MoveDown();
					break;


				case sf::Keyboard::Return:
					switch (menu.GetPressedItem())
					{
					case 0:
					{
						preTimer.start();
						std::cout << "timer started . . ." << std::endl;
						while (true)
						{
							if (preTimer.elapsedTime() <= startSeconds)
							{
								break;//countdown thingy;
							}
						}
						backgTimer.start();
						std::cout << "Background timer has started... GET the fuck out" << std::endl;
						while (true)
						{
							if(backgTimer.elapsedTime()<=backSeconds)
							{
								cv::Mat cameraFrame;
								capBackG.read(cameraFrame);
								cv::flip(cameraFrame, cameraFrame, 1);
								cv::imshow("cam", cameraFrame);
								if (cv::waitKey(30) >= 0)
									break;
							}
							else {
								capBackG >> imageback;
								flip(imageback, imageback, 1);
								imwrite("Background.jpg", imageback);
							imshow("Webcam", imageback);
								break;
							}


						}
						playTimer.start();
						while (true) {
							if (playTimer.elapsedTime() <= playSeconds) {
								cv::Mat cameraFrame;
								capPlay.read(cameraFrame);
								cv::flip(cameraFrame, cameraFrame, 1);
								cv::imshow("cam", cameraFrame);
								if (cv::waitKey(30) >= 0)
									break;
							}
							else {
								capPlay >> image;
								flip(image, image, 1);
								imwrite("test1.jpg", image);
								imshow("Webcam", image);
								break;
							}
						}
						
						cv::imshow("111", bg.backgroundSub(src, dst));
						ViewStat = false;
						MenuStat = false;
						window.setView(Gameview);
						
						break; }
					case 1:
						std::cout << "option is pressed" << std::endl;
						break;
					case 2:
						window.close();
						break;

					default:
						break;
					}
				}


			}
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) {
			
			Gameview.zoom(1.0002f);
			
			
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::X)) {

			Gameview.zoom(0.9999f);
			
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::G))
		{
			if (ViewStat == false) {
				t.start();
				while (t.elapsedTime() <= startSeconds) {
					Gameview.zoom(0.999f);
					Gameview.setCenter(position);
					window.setView(Gameview);
					window.draw(bImage);
					window.display();
					window.clear();
				}
			}
		}


		Gameview.setCenter(position);
		Menu.setCenter(position);
		if (ViewStat == true)
			window.setView(Menu);
		else
		{
			window.setView(Gameview);
			window.draw(bImage);
		}


		if (MenuStat == true) {
			menu.draw(window);
		}
		else
			window.draw(bImage);

		window.display();
		window.clear();
	}
}

	




