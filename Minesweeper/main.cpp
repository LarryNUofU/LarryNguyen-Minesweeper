#include <SFML/Graphics.hpp>
#include <Windows.h>
#include <stdlib.h>
#include <unordered_set>
#include <cstdio>
#include <ctime>

#include "GameStats.h"

bool inMainMenu = true;
bool easyMode = false;
bool expertMode = false;
bool medMode = false;
bool setupGameplay = false;
bool restartGame = false;

//the following are variables for SFML textures, fonts, etc.
sf::Texture *textureDefault;
sf::Texture *texturePressed;

sf::Texture *textureFlag;
sf::Texture *textureFlagPressed;

sf::Texture *textureWhite;

sf::RectangleShape *gameplayBackButton;
sf::Font *gameplayFont;
sf::Text *gameplayBackButtonText;

sf::RectangleShape *gameplayRestartButton;
sf::Text *gameplayRestartButtonText;

//text for the timer
sf::Text timerText;

bool lostGame = false;
bool wonGame = false;
bool canUpdateStats = true;
bool shouldLoadStats = true;

int squaresCountdown = 0;

bool cleanupMemory = false;

void setupGameplayAndBoard(float boardXpos, float boardYpos, float width, int numSquaresWidth, int numSquaresHeight, int numMines);
bool checkWithinBounds(int boardIndex, int direction, int numSquaresWidth, int numSquaresHeight);
bool checkIfMineAround(int boardIndex, int direction, int numSquaresWidth, int numSquaresHeight);

void centerTextInButton(sf::Text &text, sf::Font &font, sf::RectangleShape &button, std::string &str);

//Represents the stats of games won, etc.
GameStats stats;


//represents a single square of a board
struct GameSquare
{
	sf::Text num;
	sf::RectangleShape square;
	bool isClicked;
	bool isFlagged;
	bool isMine;
	bool isBlank;
	bool isNumber;
};


void drawAndUpdate(GameSquare &square, int index);


struct Board
{
	GameSquare *gameBoardArr;
	std::unordered_set<int> *mineSet;
	//std::unordered_set<int> *borderSquares;

	int widthSquares;
	int heightSquares;
	float startingPosX;
	float startingPosY;
	float lenOfSquare;
};


void recurseBlankSquares(int index);

Board *board;

void memoryCleanup();
void drawStats(sf::RenderWindow &window, sf::Font &font);
std::string durationToTime(double time);

//The loop that runs at the main menu
void mainMenuLoop(sf::RenderWindow &window, sf::Event  &event)
{
	
	if (cleanupMemory)
	{
		memoryCleanup();
	}
	
	//button
	sf::RectangleShape easyButton(sf::Vector2f(200, 100));
	easyButton.setFillColor(sf::Color::Blue);
	easyButton.setOutlineThickness(0.1f);
	sf::Font font;
	if (!font.loadFromFile("Fonts/Roboto-Black.ttf"))
	{
		// error...
		easyButton.setFillColor(sf::Color::Cyan);
	}

	//button
	sf::RectangleShape expertButton(sf::Vector2f(200, 100));
	expertButton.setFillColor(sf::Color::Blue);

	//button
	sf::RectangleShape mediumButton(sf::Vector2f(200, 100));
	mediumButton.setFillColor(sf::Color::Blue);

	std::string str;

	//////logic to make text go center of button///////
	sf::Text text;
	str.assign("EASY");
	centerTextInButton(text, font, easyButton, str);
	easyButton.setPosition(sf::Vector2f(200.f, 100.f));
	text.setPosition(easyButton.getPosition());

	sf::Text text2;
	str.assign("MEDIUM");
	centerTextInButton(text2, font, mediumButton, str);
	mediumButton.setPosition(sf::Vector2f(200.f, 300.f));
	text2.setPosition(mediumButton.getPosition());

	sf::Text text3;
	str.assign("EXPERT");
	centerTextInButton(text3, font, expertButton, str);
	expertButton.setPosition(sf::Vector2f(200.f, 500.f));
	expertButton.setOutlineThickness(0.1f);
	text3.setPosition(expertButton.getPosition());


	sf::FloatRect fr = easyButton.getGlobalBounds();
	sf::FloatRect fr2 = expertButton.getGlobalBounds();
	sf::FloatRect fr3 = mediumButton.getGlobalBounds();

	sf::Vector2i mouseCoords = sf::Mouse::getPosition(window);
	sf::Vector2f worldPos = window.mapPixelToCoords(mouseCoords);
	while (window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
			window.close();
		if (event.type == sf::Event::MouseButtonReleased)
		{

			if (fr.contains(worldPos.x, worldPos.y))
			{
				inMainMenu = false;
				setupGameplay = true;
				easyMode = true;
			}
			else if (fr2.contains(worldPos.x, worldPos.y))
			{
				inMainMenu = false;
				setupGameplay = true;
				expertMode = true;
			}
			else if (fr3.contains(worldPos.x, worldPos.y))
			{
				inMainMenu = false;
				setupGameplay = true;
				medMode = true;
			}
				
		}

	}
	

	if (fr.contains(worldPos.x, worldPos.y))
		easyButton.setTexture(texturePressed);

	if (fr2.contains(worldPos.x, worldPos.y))
		expertButton.setTexture(texturePressed);


	if (fr3.contains(worldPos.x, worldPos.y))
		mediumButton.setTexture(texturePressed);


	//draw the SF objects for the frame to display
	window.clear();
	window.draw(easyButton);
	window.draw(text);
	window.draw(mediumButton);
	window.draw(text2);
	window.draw(expertButton);
	window.draw(text3);

	//Draws the stats on the same screen
	drawStats(window, font);

	window.display();

	canUpdateStats = true;


}

void drawStats(sf::RenderWindow &window, sf::Font &font) {
	if (stats.saveFileExists() && shouldLoadStats)
	{
		stats.loadStats();
		shouldLoadStats = false;
	}

	int temp = stats.easyGamesWon;
	//render stats using SF text
	sf::Text text;
	text.setFont(font);
	text.setFillColor(sf::Color::White);
	text.setString("Games won: " + std::to_string(stats.easyGamesWon));
	text.setPosition(sf::Vector2f(410.f, 100.f));
	window.draw(text);
	text.setString("Best time: " + (stats.fastestEasyWon != DBL_MAX ? durationToTime(stats.fastestEasyWon) : "N/A"));
	text.setPosition(sf::Vector2f(410.f, 130.f));
	window.draw(text);
	

	text.setString("Games won: " + std::to_string(stats.medGamesWon));
	text.setPosition(sf::Vector2f(410.f, 300.f));
	window.draw(text);
	text.setString("Best time: " + (stats.fastestMedWon != DBL_MAX ? durationToTime(stats.fastestMedWon) : "N/A"));
	text.setPosition(sf::Vector2f(410.f, 330.f));
	window.draw(text);


	text.setString("Games won: " + std::to_string(stats.expertGamesWon));
	text.setPosition(sf::Vector2f(410.f, 500.f));
	window.draw(text);
	text.setString("Best time: " + (stats.fastestExpertWon != DBL_MAX ? durationToTime(stats.fastestExpertWon) : "N/A"));
	text.setPosition(sf::Vector2f(410.f, 530.f));
	window.draw(text);
}

void memoryCleanup() {

	delete gameplayBackButton;
	delete gameplayBackButtonText;
	delete gameplayFont;
	delete[] board->gameBoardArr;
	delete board->mineSet;
	delete board;

	delete gameplayRestartButton;
	delete gameplayRestartButtonText;
	cleanupMemory = false;
}


void lostOrWonLoop(sf::RenderWindow &window, sf::Event  &event, bool &leftClickOn, bool &rightClickOn, bool &validRelease, bool &clickedOnBackButton, bool &clickedOnRestartButton, double &duration)
{

	sf::Text endText;
	endText.setFont(*gameplayFont);

	if (lostGame)
		endText.setString("GAME OVER");
	else
		endText.setString("YOU WIN!");

	endText.setCharacterSize(30); // in pixels, not points!

	endText.setFillColor(sf::Color::Red);

	endText.setStyle(sf::Text::Bold | sf::Text::Underlined);


	endText.setPosition(sf::Vector2f(300.f, 10.f));

	//update the stats if won
	if (!lostGame && canUpdateStats)
	{
		stats.saveStats((easyMode ? 0 : medMode ? 1 : 2), duration);
		canUpdateStats = false;
		shouldLoadStats = true;
	}

	while (window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
			window.close();
		if (event.type == sf::Event::MouseButtonReleased)
		{


			if (validRelease)
			{

				if (clickedOnBackButton)
				{	
					inMainMenu = true;
					clickedOnBackButton = false;
					lostGame = false;
					easyMode = false;
					medMode = false;
					expertMode = false;

					cleanupMemory = true;
					
				}
				else if (clickedOnRestartButton)
				{
					clickedOnRestartButton = false;
					lostGame = false;
					restartGame = true;
					
				}

			}




		}

	}

	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		leftClickOn = true;
	else
		leftClickOn = false;
	if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
		rightClickOn = true;
	else
		rightClickOn = false;


	if (rightClickOn && leftClickOn)
	{
		validRelease = false;
		clickedOnBackButton = false;
		clickedOnRestartButton = false;
	}


	if (!leftClickOn && !rightClickOn)
		validRelease = true;

	//Calculate mouse coordinates on screen
	sf::Vector2i mouseCoords = sf::Mouse::getPosition(window);
	sf::Vector2f worldPos = window.mapPixelToCoords(mouseCoords);

	//bound calculations of the back button
	sf::FloatRect backFr = gameplayBackButton->getGlobalBounds();
	if (leftClickOn && !rightClickOn && validRelease)
	{
		//make sure left click either on the back button or a square
		if (backFr.contains(worldPos.x, worldPos.y))
		{
			clickedOnBackButton = true;
		}
	}
	if (backFr.contains(worldPos.x, worldPos.y))
		gameplayBackButtonText->setFillColor(sf::Color::Blue);
	else
	{
		gameplayBackButtonText->setFillColor(sf::Color::Red);
	}

	//bound calculations of the restart button
	sf::FloatRect restartFr = gameplayRestartButton->getGlobalBounds();
	if (leftClickOn && !rightClickOn && validRelease)
	{
		
		if (restartFr.contains(worldPos.x, worldPos.y))
		{
			clickedOnRestartButton = true;
		}
	}
	if (restartFr.contains(worldPos.x, worldPos.y))
		gameplayRestartButtonText->setFillColor(sf::Color::Blue);
	else
	{
		gameplayRestartButtonText->setFillColor(sf::Color::Red);
	}




	window.clear();
	for (int i = 0; i < board->heightSquares * board->widthSquares; i++)
	{
		window.draw(board->gameBoardArr[i].square);

		if (board->gameBoardArr[i].isNumber && board->gameBoardArr[i].isClicked)
		{
			window.draw(board->gameBoardArr[i].num);
		}
		if (lostGame && board->gameBoardArr[i].isMine)
		{
			board->gameBoardArr[i].square.setFillColor(sf::Color::Red);
		}

	}

	window.draw(*gameplayBackButton);
	window.draw(*gameplayBackButtonText);
	window.draw(endText);
	window.draw(*gameplayRestartButton);
	window.draw(*gameplayRestartButtonText);
	window.draw(timerText);
	window.display();
	
}



bool mouseInsideBoard(float mouseX, float mouseY)
{
	return mouseX >= board->startingPosX && mouseX <= board->startingPosX + board->widthSquares * board->lenOfSquare && mouseY <= board->startingPosY + board->heightSquares * board->lenOfSquare && mouseY >= board->startingPosY;
}


void setSquareOfInterest(GameSquare *&squareOfInterest, int &indexOfInterest, float mouseX, float mouseY)
{
	int numSquares = board->widthSquares * board->heightSquares;

	int i = 0;
	while (i < numSquares)
	{
		if (mouseY > board->gameBoardArr[i].square.getPosition().y + board->lenOfSquare)
		{
			i += board->widthSquares;
			continue;
		}
		sf::FloatRect gb = board->gameBoardArr[i].square.getGlobalBounds();
		if (gb.contains(mouseX, mouseY) && !board->gameBoardArr[i].isClicked)
		{
			squareOfInterest = &board->gameBoardArr[i];
			indexOfInterest = i;
			//texture.loadFromFile("Textures/texture-2.png");
			board->gameBoardArr[i].square.setTexture(texturePressed);
			break;
		}

		i++;
	}
}

std::string durationToTime(double duration) {
	int seconds = (int)duration;
	int minutes = seconds / 60;
	seconds = seconds % 60;
	std::string str;
	if (minutes < 10)
		str.append("0");
	str.append(std::to_string(minutes));
	str.append(":");
	if (seconds < 10)
		str.append("0");
	str.append(std::to_string(seconds));

	return str;
}

int main()
{
	FreeConsole();
	

	sf::RenderWindow window(sf::VideoMode(1280, 720), "minesweeper");
	

	sf::Texture texture;
	if (!texture.loadFromFile("Textures/texture-3.png"))
	{
		//error
	}
	//square.setTexture(&texture);

	textureDefault = &texture;

	sf::Texture texture2;
	if (!texture2.loadFromFile("Textures/texture-2.png"))
	{
		//error
	}
	texturePressed = &texture2;


	sf::Texture texture3;
	if (!texture3.loadFromFile("Textures/texture-3-flag.png"))
	{
		//error
	}
	textureFlag = &texture3;


	sf::Texture texture4;
	if (!texture4.loadFromFile("Textures/texture-2-flag.png"))
	{
		//error
	}
	textureFlagPressed = &texture4;


	sf::Texture texture5;
	if (!texture5.loadFromFile("Textures/test3.png"))
	{
		//error
	}
	textureWhite = &texture5;


	bool validRelease = true;
	bool clickedOnBackButton = false;
	bool leftClickOn = false;
	bool rightClickOn = false;
	bool clickedOnRestartButton = false;
	GameSquare *squareOfInterest = nullptr;
	int indexOfInterest = -1;

	bool shouldStartTimer = true;
	std::clock_t start = std::clock();
	double duration;


	while (window.isOpen())
	{
		sf::Event event;


		if (inMainMenu)
		{
			mainMenuLoop(window, event);
		}
		else if (setupGameplay)
		{
			if (easyMode)
				setupGameplayAndBoard(433.f, 153.f, 414.f, 9, 9, 9);
			else if (expertMode)
				setupGameplayAndBoard(120.f, 130.f, 1040.f, 30, 16, 99);
			else
			{
				setupGameplayAndBoard(320.f, 50.f, 640.f, 16, 16, 30);
			}
				

			setupGameplay = false;
			shouldStartTimer = true;
		}
		else if (restartGame)
		{
			memoryCleanup();
			setupGameplay = true;
			restartGame = false;
		}
		else if (lostGame || squaresCountdown == 0)
		{	
			lostOrWonLoop(window, event, leftClickOn, rightClickOn, validRelease, clickedOnBackButton, clickedOnRestartButton, duration);
		}
		else
		{
			//in this else statement, we are in the gameplay loop

			timerText.setFont(*gameplayFont); // font is a sf::Font
			timerText.setCharacterSize(48); // in pixels, not points!
			timerText.setFillColor(sf::Color::Yellow);
			timerText.setPosition(sf::Vector2f(578.f, 0.f));
			timerText.setString("00:00");

			if (!shouldStartTimer)
			{
				duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;
				/*int seconds = (int)duration;
				int minutes = seconds / 60;
				seconds = seconds % 60;
				std::string str;
				if (minutes < 10)
					str.append("0");
				str.append(std::to_string(minutes));
				str.append(":");
				if (seconds < 10)
					str.append("0");
				str.append(std::to_string(seconds));*/
				std::string str = durationToTime(duration);

				timerText.setString(str);
			}

			
			while (window.pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
					window.close();
				if (event.type == sf::Event::MouseButtonReleased)
				{


					if (validRelease)
					{
						if (clickedOnBackButton)
						{
							inMainMenu = true;
							clickedOnBackButton = false;
							lostGame = false;
							easyMode = false;
							medMode = false;
							expertMode = false;

							cleanupMemory = true;

						}
						else if (leftClickOn)
						{
							if (squareOfInterest != nullptr)
							{
								if (shouldStartTimer)
								{
									start = std::clock();
									shouldStartTimer = false;
								}



								if (!squareOfInterest->isFlagged)
								{
									drawAndUpdate(*squareOfInterest, indexOfInterest);
								}
								else
								{
									squareOfInterest->square.setFillColor(sf::Color::White);
									squareOfInterest->square.setTexture(textureFlag);
								}

								squareOfInterest = nullptr;
							}
						}
						else if (rightClickOn)
						{
							if (squareOfInterest != nullptr)
							{
								if (!squareOfInterest->isFlagged)
								{
									squareOfInterest->isFlagged = true;
									squareOfInterest->square.setFillColor(sf::Color::White);
									squareOfInterest->square.setTexture(textureFlag);

									if (squareOfInterest->isMine)
										squaresCountdown--;
									else
										squaresCountdown++;

								}
								else
								{
									squareOfInterest->isFlagged = false;
									squareOfInterest->square.setFillColor(sf::Color::Blue);
									squareOfInterest->square.setTexture(textureDefault);

									if (squareOfInterest->isMine)
										squaresCountdown++;
									else
										squaresCountdown--;

								}
							}
								
							squareOfInterest = nullptr;

						}


					}


				}

			}


			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
				leftClickOn = true;
			else
				leftClickOn = false;
			if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
				rightClickOn = true;
			else
				rightClickOn = false;


			if (rightClickOn && leftClickOn)
			{
				validRelease = false;
				clickedOnBackButton = false;
			}

			if (!leftClickOn && !rightClickOn)
				validRelease = true;



			sf::Vector2i mouseCoords = sf::Mouse::getPosition(window);
			sf::Vector2f worldPos = window.mapPixelToCoords(mouseCoords);
			sf::FloatRect backFr = gameplayBackButton->getGlobalBounds();

			if (leftClickOn && !rightClickOn && validRelease)
			{


				if (backFr.contains(worldPos.x, worldPos.y))
				{
					clickedOnBackButton = true;
				}
				//find out if click is on the grid at all
				else if (squareOfInterest == nullptr && mouseInsideBoard(worldPos.x, worldPos.y))
				{
					setSquareOfInterest(squareOfInterest, indexOfInterest, worldPos.x, worldPos.y);
				}


				if (squareOfInterest != nullptr)
				{
					sf::FloatRect gb = squareOfInterest->square.getGlobalBounds();
					if (!gb.contains(worldPos.x, worldPos.y))
					{
						validRelease = false;
						indexOfInterest = -1;
					}
				}
				else if (!clickedOnBackButton)
				{
					validRelease = false;
				}

			}



			if (rightClickOn && !leftClickOn && validRelease)
			{

				if (squareOfInterest == nullptr && mouseInsideBoard(worldPos.x, worldPos.y))
				{
					setSquareOfInterest(squareOfInterest, indexOfInterest, worldPos.x, worldPos.y);
				}


				if (squareOfInterest != nullptr)
				{
					sf::FloatRect gb = squareOfInterest->square.getGlobalBounds();
					if (!gb.contains(worldPos.x, worldPos.y))
					{
						validRelease = false;
						indexOfInterest = -1;
					}
				}
				else
				{
					validRelease = false;
				}

			}


			if (backFr.contains(worldPos.x, worldPos.y))
				gameplayBackButtonText->setFillColor(sf::Color::Blue);
			else
				gameplayBackButtonText->setFillColor(sf::Color::Red);


			if (!validRelease && squareOfInterest != nullptr)
			{
				if (squareOfInterest->isFlagged)
				{
					
					squareOfInterest->square.setFillColor(sf::Color::White);
					squareOfInterest->square.setTexture(textureFlag);
				}
				else
				{
					//texture.loadFromFile("Textures/texture-3.png");
					squareOfInterest->square.setTexture(textureDefault);
				}

				squareOfInterest = nullptr;
				indexOfInterest = -1;
			}


			window.clear();
			for (int i = 0; i < board->heightSquares * board->widthSquares; i++)
			{
				window.draw(board->gameBoardArr[i].square);

				if (board->gameBoardArr[i].isNumber && board->gameBoardArr[i].isClicked)
				{
					window.draw(board->gameBoardArr[i].num);
				}


				if (!board->gameBoardArr[i].isClicked && board->gameBoardArr[i].isFlagged)
				{

				}

			}



			window.draw(*gameplayBackButton);
			window.draw(*gameplayBackButtonText);
			window.draw(timerText);
			window.display();

		}

	}

	return 0;

}


void drawAndUpdate(GameSquare &square, int index)
{
	if (square.isBlank)
	{
		//square.isClicked = true;
		recurseBlankSquares(index);
	}
	else if (square.isMine)
	{
		square.isClicked = true;
		square.square.setFillColor(sf::Color::Red);
		lostGame = true;
	}
	else
	{
		//is a number square
		square.square.setFillColor(sf::Color::White);
		square.square.setTexture(textureWhite);
		//square.square.setOutlineThickness(-0.503f);
		square.isClicked = true;
		//square.square.setFillColor(sf::Color::White);
		squaresCountdown--;
	}
}


void centerTextInButton(sf::Text &text, sf::Font &font, sf::RectangleShape &button, std::string &str)
{
	//////logic to make text go center of button///////
	text.setFont(font);

	text.setString(str.c_str());

	text.setCharacterSize(48); // in pixels, not points!

	text.setFillColor(sf::Color::Red);

	text.setStyle(sf::Text::Bold);

	sf::FloatRect textBounds(text.getLocalBounds());
	sf::Vector2f squareSize(button.getSize());

	//line below thanks to Mario from StackOverflow!
	text.setOrigin((textBounds.width - squareSize.x) / 2 + textBounds.left, (textBounds.height - squareSize.y) / 2 + textBounds.top);
}

void setupGameplayAndBoard(float boardXpos, float boardYpos, float width, int numSquaresWidth, int numSquaresHeight, int numMines)
{
	gameplayFont = new sf::Font;
	if (!gameplayFont->loadFromFile("Fonts/Roboto-Black.ttf"))
	{
		// error...

	}

	std::string str;

	//create back button and its text
	gameplayBackButton = new sf::RectangleShape(sf::Vector2f(200, 100));
	gameplayBackButtonText = new sf::Text;
	str.assign("BACK");
	centerTextInButton(*gameplayBackButtonText, *gameplayFont, *gameplayBackButton, str);
	gameplayBackButtonText->setPosition(gameplayBackButton->getPosition());

	//creating restart button and its text
	gameplayRestartButton = new sf::RectangleShape(sf::Vector2f(230, 100));
	gameplayRestartButtonText = new sf::Text;
	str.assign("RESTART");
	centerTextInButton(*gameplayRestartButtonText, *gameplayFont, *gameplayRestartButton, str);
	gameplayRestartButtonText->setPosition(gameplayRestartButton->getPosition());

	gameplayRestartButton->setPosition(sf::Vector2f(1050.f, 0.f));
	gameplayRestartButtonText->setPosition(gameplayRestartButton->getPosition());


	//creating the board
	//first, fill board with blank squares
	board = new Board;
	int numSquares = numSquaresWidth * numSquaresHeight;
	board->gameBoardArr = new GameSquare[numSquares];
	float currentXPos = boardXpos;
	float currentYPos = boardYpos;
	float side = width / numSquaresWidth;

	squaresCountdown = numSquares;


	board->heightSquares = numSquaresHeight;
	board->widthSquares = numSquaresWidth;
	board->lenOfSquare = side;
	board->startingPosX = boardXpos;
	board->startingPosY = boardYpos;

	for (int i = 0; i < numSquares; i++)
	{
		GameSquare gs;
		gs.isBlank = true;
		gs.isMine = false;
		gs.isNumber = false;
		gs.isClicked = false;
		gs.isFlagged = false;


		float side = width / numSquaresWidth;
		//set the appropriate coordinate of the square
		sf::RectangleShape square(sf::Vector2f(side, side));
		square.setPosition(currentXPos, currentYPos);


		//set font of the text and position
		gs.num.setFont(*gameplayFont);
		gameplayBackButtonText->setString("BACK");

		gs.num.setCharacterSize(28); // in pixels, not points!

		gs.num.setString("0");

		gs.num.setFillColor(sf::Color::Red);

		//gs.num.setStyle(sf::Text::Bold | sf::Text::Underlined);
		gs.num.setStyle(sf::Text::Bold);

		sf::FloatRect textBounds(gs.num.getLocalBounds());
		sf::Vector2f squareSize(square.getSize());

		gs.num.setOrigin((textBounds.width - squareSize.x) / 2 + textBounds.left, (textBounds.height - squareSize.y) / 2 + textBounds.top);
		gs.num.setPosition(square.getPosition());


		//set visuals of the square
		square.setFillColor(sf::Color::Blue);


		square.setOutlineColor(sf::Color::Black);



		square.setOutlineThickness(-1.f);
		square.setTexture(textureDefault);


		//set the distance parameters for the next square's position.
		if (i % numSquaresWidth == numSquaresWidth - 1)
		{
			currentXPos = boardXpos;
			currentYPos += side;
		}
		else
		{
			currentXPos += side;
		}
		gs.square = square;
		board->gameBoardArr[i] = gs;
	}


	//sample the random number generator
	srand(time(0));

	board->mineSet = new std::unordered_set<int>;
	//Choose random positions on the board for mines
	while (numMines > 0)
	{
		
		int randNum = rand() % numSquares;

		if (board->mineSet->find(randNum) == board->mineSet->end())
		{
			board->gameBoardArr[randNum].isBlank = false;
			board->gameBoardArr[randNum].isMine = true;
			board->mineSet->insert(randNum);
			numMines -= 1;
		}
	}
	

	
	//for each square, calculate a number for the square if it is needed and its color
	for (int i = 0; i < numSquares; i++)
	{
		if (!board->gameBoardArr[i].isMine)
		{
			int minesAroundSquare = 0;
			for (int j = 0; j <= 7; j++)
			{
				if (checkIfMineAround(i, j, numSquaresWidth, numSquaresHeight))
					minesAroundSquare++;
			}

			//set the square object to be a number square if more than 0 mines around
			if (minesAroundSquare > 0)
			{
				board->gameBoardArr[i].isBlank = false;
				board->gameBoardArr[i].isNumber = true;

				board->gameBoardArr[i].num.setString(std::to_string(minesAroundSquare));

				switch (minesAroundSquare)
				{
				case 1:
					board->gameBoardArr[i].num.setFillColor(sf::Color::Blue);
					break;
				case 2:
					board->gameBoardArr[i].num.setFillColor(sf::Color(0, 128, 0, 255));
					break;
				case 3:
					board->gameBoardArr[i].num.setFillColor(sf::Color::Red);
					break;
				case 4:
					board->gameBoardArr[i].num.setFillColor(sf::Color(128, 0, 128, 255));
					break;
				case 5:
					board->gameBoardArr[i].num.setFillColor(sf::Color(128, 0, 0, 255));
					break;
				case 6:
					board->gameBoardArr[i].num.setFillColor(sf::Color(64, 224, 208, 255));
					break;
				case 7:
					board->gameBoardArr[i].num.setFillColor(sf::Color::Black);
					break;
				case 8:
					board->gameBoardArr[i].num.setFillColor(sf::Color(128, 128, 128, 255));
					break;
				}
			}
		}
	}


}


bool checkIfMineAround(int boardIndex, int direction, int numSquaresWidth, int numSquaresHeight)
{
	if (checkWithinBounds(boardIndex, direction, numSquaresWidth, numSquaresHeight))
	{
		switch (direction)
		{
		case 0:
			if (board->gameBoardArr[boardIndex - numSquaresWidth - 1].isMine)
				return true;
			else
				return false;
		case 1:
			if (board->gameBoardArr[boardIndex - numSquaresWidth].isMine)
				return true;
			else
				return false;
		case 2:
			if (board->gameBoardArr[boardIndex - numSquaresWidth + 1].isMine)
				return true;
			else
				return false;
		case 3:
			if (board->gameBoardArr[boardIndex - 1].isMine)
				return true;
			else
				return false;
		case 4:
			if (board->gameBoardArr[boardIndex + 1].isMine)
				return true;
			else
				return false;
		case 5:
			if (board->gameBoardArr[boardIndex + numSquaresWidth - 1].isMine)
				return true;
			else
				return false;

		case 6:
			if (board->gameBoardArr[boardIndex + numSquaresWidth].isMine)
				return true;
			else
				return false;

		case 7:
			if (board->gameBoardArr[boardIndex + numSquaresWidth + 1].isMine)
				return true;
			else
				return false;
		}
	}
	else
	{
		return false;
	}
}



//boardIndex assumed to be a valid index
//direction:
//0 - top left, 1 - top, 2 - top right, 3 - left, 4 - right, 5 - bottom left, 6 - bottom, 7 - bottom right
//
//
bool checkWithinBounds(int boardIndex, int direction, int numSquaresWidth, int numSquaresHeight) 
{
	bool canGoUp = true;
	bool canGoLeft = true;
	bool canGoRight = true;
	bool canGoDown = true;
	bool canGoTopLeft = true;
	bool canGoTopRight = true;
	bool canGoBLeft = true;
	bool canGoBRight = true;

	if (boardIndex < numSquaresWidth)
	{
		canGoUp = false;
		canGoTopLeft = false;
		canGoTopRight = false;
	}
	if (boardIndex % numSquaresWidth == numSquaresWidth - 1)
	{
		canGoTopRight = false;
		canGoRight = false;
		canGoBRight = false;
	}
		
	if (boardIndex >= numSquaresWidth * (numSquaresHeight - 1))
	{
		canGoBLeft = false;
		canGoDown = false;
		canGoBRight = false;
	}

	if (boardIndex % numSquaresWidth == 0)
	{
		canGoTopLeft = false;
		canGoLeft = false;
		canGoBLeft = false;
	}


	switch (direction)
	{
	case 0:
		if (canGoTopLeft)
			return true;
		else
			return false;
	case 1:
		if (canGoUp)
			return true;
		else
			return false;
	case 2:
		if (canGoTopRight)
			return true;
		else
			return false;
	case 3:
		if (canGoLeft)
			return true;
		else
			return false;
	case 4:
		if (canGoRight)
			return true;
		else
			return false;
	case 5:
		if (canGoBLeft)
			return true;
		else
			return false;
	case 6:
		if (canGoDown)
			return true;
		else
			return false;
	case 7:
		if (canGoBRight)
			return true;
		else
			return false;
	}

}


void recurseBlankSquares(int index)
{
	squaresCountdown--;
	board->gameBoardArr[index].square.setFillColor(sf::Color::White);
	board->gameBoardArr[index].square.setTexture(textureWhite);
	//board->gameBoardArr[index].square.setOutlineThickness(-0.503f);
	board->gameBoardArr[index].isClicked = true;


	for (int i = 0; i <= 7; i++)
	{
		switch (i)
		{
		case 0:
			if (checkWithinBounds(index, i, board->widthSquares, board->heightSquares))
			{
				if (board->gameBoardArr[index - board->widthSquares - 1].isNumber && !board->gameBoardArr[index - board->widthSquares - 1].isClicked)
					drawAndUpdate(board->gameBoardArr[index - board->widthSquares - 1], index - board->widthSquares - 1);
				if (board->gameBoardArr[index - board->widthSquares - 1].isBlank && !board->gameBoardArr[index - board->widthSquares - 1].isClicked)
					recurseBlankSquares(index - board->widthSquares - 1);
			}
			break;
		case 1:
			if (checkWithinBounds(index, i, board->widthSquares, board->heightSquares))
			{
				if (board->gameBoardArr[index - board->widthSquares].isNumber && !board->gameBoardArr[index - board->widthSquares].isClicked)
					drawAndUpdate(board->gameBoardArr[index - board->widthSquares], index - board->widthSquares);
				if (board->gameBoardArr[index - board->widthSquares].isBlank && !board->gameBoardArr[index - board->widthSquares].isClicked)
					recurseBlankSquares(index - board->widthSquares);
			}
			break;
		case 2:
			if (checkWithinBounds(index, i, board->widthSquares, board->heightSquares))
			{
				if (board->gameBoardArr[index - board->widthSquares + 1].isNumber && !board->gameBoardArr[index - board->widthSquares + 1].isClicked)
					drawAndUpdate(board->gameBoardArr[index - board->widthSquares + 1], index - board->widthSquares + 1);
				if (board->gameBoardArr[index - board->widthSquares + 1].isBlank && !board->gameBoardArr[index - board->widthSquares + 1].isClicked)
					recurseBlankSquares(index - board->widthSquares + 1);
			}
			break;
		case 3:
			if (checkWithinBounds(index, i, board->widthSquares, board->heightSquares))
			{
				if (board->gameBoardArr[index - 1].isNumber && !board->gameBoardArr[index - 1].isClicked)
					drawAndUpdate(board->gameBoardArr[index - 1], index - 1);
				if (board->gameBoardArr[index - 1].isBlank && !board->gameBoardArr[index - 1].isClicked)
					recurseBlankSquares(index - 1);
			}
			break;
		case 4:
			if (checkWithinBounds(index, i, board->widthSquares, board->heightSquares))
			{
				if (board->gameBoardArr[index + 1].isNumber && !board->gameBoardArr[index + 1].isClicked)
					drawAndUpdate(board->gameBoardArr[index + 1], index + 1);
				if (board->gameBoardArr[index + 1].isBlank && !board->gameBoardArr[index + 1].isClicked)
					recurseBlankSquares(index + 1);
			}
			break;
		case 5:
			if (checkWithinBounds(index, i, board->widthSquares, board->heightSquares))
			{
				if (board->gameBoardArr[index + board->widthSquares - 1].isNumber && !board->gameBoardArr[index + board->widthSquares - 1].isClicked)
					drawAndUpdate(board->gameBoardArr[index + board->widthSquares - 1], index + board->widthSquares - 1);
				if (board->gameBoardArr[index + board->widthSquares - 1].isBlank && !board->gameBoardArr[index + board->widthSquares - 1].isClicked)
					recurseBlankSquares(index + board->widthSquares - 1);
			}
			break;
		case 6:
			if (checkWithinBounds(index, i, board->widthSquares, board->heightSquares))
			{
				if (board->gameBoardArr[index + board->widthSquares].isNumber && !board->gameBoardArr[index + board->widthSquares].isClicked)
					drawAndUpdate(board->gameBoardArr[index + board->widthSquares], index + board->widthSquares);
				if (board->gameBoardArr[index + board->widthSquares].isBlank && !board->gameBoardArr[index + board->widthSquares].isClicked)
					recurseBlankSquares(index + board->widthSquares);
			}
			break;
		case 7:
			if (checkWithinBounds(index, i, board->widthSquares, board->heightSquares))
			{
				if (board->gameBoardArr[index + board->widthSquares + 1].isNumber && !board->gameBoardArr[index + board->widthSquares + 1].isClicked)
					drawAndUpdate(board->gameBoardArr[index + board->widthSquares + 1], index + board->widthSquares + 1);
				if (board->gameBoardArr[index + board->widthSquares + 1].isBlank && !board->gameBoardArr[index + board->widthSquares + 1].isClicked)
					recurseBlankSquares(index + board->widthSquares + 1);
			}
			break;
		}

	}

}