#include <SFML/Graphics.hpp>
#include <Windows.h>
#include <stdlib.h>
#include <unordered_set>

bool inMainMenu = true;
bool easyMode = false;
bool expertMode = false;
bool medMode = false;
bool setupGameplay = false;

sf::Texture *textureDefault;
sf::Texture *texturePressed;

sf::Texture *textureFlag;
sf::Texture *textureFlagPressed;
sf::Sprite *flagSprite;
sf::Texture *textureWhite;

sf::RectangleShape *gameplayBackButton;
sf::Font *gameplayFont;
sf::Text *gameplayBackButtonText;

bool lostGame = false;
bool wonGame = false;
int squaresCountdown = 0;

void setupGameplayAndBoard(float boardXpos, float boardYpos, float width, int numSquaresWidth, int numSquaresHeight, int numMines);
bool checkWithinBounds(int boardIndex, int direction, int numSquaresWidth, int numSquaresHeight);
bool checkIfMineAround(int boardIndex, int direction, int numSquaresWidth, int numSquaresHeight);
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
	std::unordered_set<int> *borderSquares;

	int widthSquares;
	int heightSquares;
	float startingPosX;
	float startingPosY;
	float lenOfSquare;
};

void recurseBlankSquares(int index);


Board *board;



//The loop that runs at the main menu
void mainMenuLoop(sf::RenderWindow &window, sf::Event  &event, sf::Clock *clock)
{
	//button
	sf::RectangleShape easyButton(sf::Vector2f(200, 100));
	easyButton.setFillColor(sf::Color::Blue);
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


	//////logic to make text go center of button///////
	sf::Text text;
	text.setFont(font);

	text.setString("EASY");

	text.setCharacterSize(48); // in pixels, not points!

	text.setFillColor(sf::Color::Red);

	text.setStyle(sf::Text::Bold | sf::Text::Underlined);

	sf::FloatRect textBounds(text.getLocalBounds());
	sf::Vector2f squareSize(easyButton.getSize());

	//line below thanks to Mario from StackOverflow!
	text.setOrigin((textBounds.width - squareSize.x) / 2 + textBounds.left, (textBounds.height - squareSize.y) / 2 + textBounds.top);


	easyButton.setPosition(sf::Vector2f(200.f, 100.f));
	text.setPosition(easyButton.getPosition());




	



	//////logic to make text go center of button///////
	sf::Text text2;
	text2.setFont(font);

	text2.setString("EXPERT");

	text2.setCharacterSize(48); // in pixels, not points!

	text2.setFillColor(sf::Color::Red);

	text2.setStyle(sf::Text::Bold | sf::Text::Underlined);

	sf::FloatRect textBounds2(text2.getLocalBounds());
	sf::Vector2f squareSize2(easyButton.getSize());

	//line below thanks to Mario from StackOverflow!
	text2.setOrigin((textBounds2.width - squareSize2.x) / 2 + textBounds2.left, (textBounds2.height - squareSize2.y) / 2 + textBounds2.top);


	expertButton.setPosition(sf::Vector2f(200.f, 300.f));
	text2.setPosition(expertButton.getPosition());



	//////logic to make text go center of button///////
	sf::Text text3;
	text3.setFont(font);

	text3.setString("MEDIUM");

	text3.setCharacterSize(48); // in pixels, not points!

	text3.setFillColor(sf::Color::Red);

	text3.setStyle(sf::Text::Bold | sf::Text::Underlined);

	sf::FloatRect textBounds3(text3.getLocalBounds());
	sf::Vector2f squareSize3(easyButton.getSize());

	//line below thanks to Mario from StackOverflow!
	text3.setOrigin((textBounds3.width - squareSize3.x) / 2 + textBounds3.left, (textBounds3.height - squareSize3.y) / 2 + textBounds3.top);


	mediumButton.setPosition(sf::Vector2f(200.f, 500.f));
	text3.setPosition(mediumButton.getPosition());




	while (window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
			window.close();
		if (event.type == sf::Event::MouseButtonReleased)
		{
			sf::FloatRect fr = easyButton.getGlobalBounds();
			sf::FloatRect fr2 = expertButton.getGlobalBounds();
			sf::FloatRect fr3 = mediumButton.getGlobalBounds();

			sf::Vector2i mouseCoords = sf::Mouse::getPosition(window);
			sf::Vector2f worldPos = window.mapPixelToCoords(mouseCoords);
			if (fr.contains(worldPos.x, worldPos.y))
			{
				inMainMenu = false;
				//easyMode = true;
				setupGameplay = true;
				easyMode = true;
				clock->restart();
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
	



	window.clear();
	window.draw(easyButton);
	window.draw(text);
	window.draw(expertButton);
	window.draw(text2);
	window.draw(mediumButton);
	window.draw(text3);
	window.display();



}






int main()
{
	FreeConsole();
	

	sf::RenderWindow window(sf::VideoMode(1280, 720), "minesweeper");
	
	/*sf::CircleShape shape(100.f);
	sf::RectangleShape square(sf::Vector2f(100, 100));
	square.setPosition(200.f, 0.f);
	shape.setFillColor(sf::Color::Blue);
	//square.setFillColor(sf::Color::Green);
	square.setOutlineColor(sf::Color::Blue);
	square.setOutlineThickness(-1.f);*/

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



	bool goodMode = false;
	bool validClick = false;
	bool validRelease = true;

	bool clickedOnBackButton = false;



	bool leftClickOn = false;
	bool rightClickOn = false;

	GameSquare *squareOfInterest = nullptr;
	int indexOfInterest = -1;


	sf::Clock clock;
	while (window.isOpen())
	{
		sf::Event event;


		if (inMainMenu)
		{
			mainMenuLoop(window, event, &clock);
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
			/*window.clear();
			for (int i = 0; i < 81; i++)
			{
				window.draw(board->gameBoardArr[i].square);
			}
			window.draw(*gameplayBackButton);
			window.draw(*gameplayBackButtonText);
			window.display();*/
		}
		else if (lostGame)
		{

			while (window.pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
					window.close();
				if (event.type == sf::Event::MouseButtonReleased)
				{


					if (validRelease)
					{

						if (leftClickOn)
						{
							if (squareOfInterest != nullptr)
								if (!squareOfInterest->isFlagged)
								{
									//squareOfInterest->square.setFillColor(sf::Color::Transparent);
									//squareOfInterest->square.setTexture(textureWhite);

									drawAndUpdate(*squareOfInterest, indexOfInterest);
								}

								else
								{
									squareOfInterest->square.setFillColor(sf::Color::White);
									squareOfInterest->square.setTexture(textureFlag);
								}

							squareOfInterest = nullptr;
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
								}
								else
								{
									squareOfInterest->isFlagged = false;
									squareOfInterest->square.setFillColor(sf::Color::Blue);
									squareOfInterest->square.setTexture(textureDefault);
								}
							}

							squareOfInterest = nullptr;


						}

						//sf::FloatRect fr = square.getGlobalBounds();
						//sf::Vector2i mouseCoords = sf::Mouse::getPosition(window);
						//sf::Vector2f worldPos = window.mapPixelToCoords(mouseCoords);
						//if (fr.contains(worldPos.x, worldPos.y) && validClick)
							//square.setFillColor(sf::Color::White);
					}


					validClick = true;

				}

			}
			/*window.clear();
			for (int i = 0; i < 81; i++)
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
			window.display();*/


		}
		else
		{
			//in this else statement, we are in the gameplay loop

			while (window.pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
					window.close();
				if (event.type == sf::Event::MouseButtonReleased)
				{


					if (validRelease)
					{

						if (leftClickOn)
						{
							if(squareOfInterest != nullptr)
								if (!squareOfInterest->isFlagged)
								{
									//squareOfInterest->square.setFillColor(sf::Color::Transparent);
									//squareOfInterest->square.setTexture(textureWhite);

									drawAndUpdate(*squareOfInterest, indexOfInterest);
								}
									
								else
								{
									squareOfInterest->square.setFillColor(sf::Color::White);
									squareOfInterest->square.setTexture(textureFlag);
								}

							


							squareOfInterest = nullptr;

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
								}
								else
								{
									squareOfInterest->isFlagged = false;
									squareOfInterest->square.setFillColor(sf::Color::Blue);
									squareOfInterest->square.setTexture(textureDefault);
								}
							}
								
							squareOfInterest = nullptr;


						}



						//sf::FloatRect fr = square.getGlobalBounds();
						//sf::Vector2i mouseCoords = sf::Mouse::getPosition(window);
						//sf::Vector2f worldPos = window.mapPixelToCoords(mouseCoords);
						//if (fr.contains(worldPos.x, worldPos.y) && validClick)
							//square.setFillColor(sf::Color::White);
					}


					validClick = true;

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
				validRelease = false;

			if (!leftClickOn && !rightClickOn)
				validRelease = true;


			if (leftClickOn && !rightClickOn && validRelease)
			{
				sf::Vector2i mouseCoords = sf::Mouse::getPosition(window);
				sf::Vector2f worldPos = window.mapPixelToCoords(mouseCoords);

				//make sure left click either on the back button or a square

				sf::FloatRect backFr = gameplayBackButton->getGlobalBounds();

				if (backFr.contains(worldPos.x, worldPos.y))
				{

				}
				//find out if click is on the grid at all
				else if (squareOfInterest == nullptr && worldPos.x >= board->startingPosX && worldPos.x <= board->startingPosX + board->widthSquares * board->lenOfSquare && worldPos.y <= board->startingPosY + board->heightSquares * board->lenOfSquare && worldPos.y >= board->startingPosY)
				{
					int numSquares = board->widthSquares * board->heightSquares;
					
					int i = 0;
					while (i < numSquares)
					{
						if (worldPos.y > board->gameBoardArr[i].square.getPosition().y + board->lenOfSquare)
						{
							i += board->widthSquares;
							continue;
						}
						sf::FloatRect gb = board->gameBoardArr[i].square.getGlobalBounds();
						if (gb.contains(worldPos.x, worldPos.y) && !board->gameBoardArr[i].isClicked)
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


				//find out if user clicked on a square
				/*sf::FloatRect fr = board->gameBoardArr[80].square.getGlobalBounds();

	
				if (backFr.contains(worldPos.x, worldPos.y))
				{

				}
				else if (fr.contains(worldPos.x,worldPos.y))
				{
					squareOfInterest = &board->gameBoardArr[80].square;
					texture.loadFromFile("Textures/texture-2.png");
					board->gameBoardArr[80].square.setTexture(&texture);
				}
				else
				{
					validRelease = false;
				}*/
			}


			if (rightClickOn && !leftClickOn && validRelease)
			{
				sf::Vector2i mouseCoords = sf::Mouse::getPosition(window);
				sf::Vector2f worldPos = window.mapPixelToCoords(mouseCoords);

				if (squareOfInterest == nullptr && worldPos.x >= board->startingPosX && worldPos.x <= board->startingPosX + board->widthSquares * board->lenOfSquare && worldPos.y <= board->startingPosY + board->heightSquares * board->lenOfSquare && worldPos.y >= board->startingPosY)
				{
					int numSquares = board->widthSquares * board->heightSquares;

					int i = 0;
					while (i < numSquares)
					{
						if (worldPos.y > board->gameBoardArr[i].square.getPosition().y + board->lenOfSquare)
						{
							i += board->widthSquares;
							continue;
						}
						sf::FloatRect gb = board->gameBoardArr[i].square.getGlobalBounds();
						if (gb.contains(worldPos.x, worldPos.y) && !board->gameBoardArr[i].isClicked)
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









			/*if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
			{
				sf::FloatRect fr = square.getGlobalBounds();
				sf::Vector2i mouseCoords = sf::Mouse::getPosition(window);
				sf::Vector2f worldPos = window.mapPixelToCoords(mouseCoords);
				if (fr.contains(worldPos.x, worldPos.y))
				{
					texture.loadFromFile("Textures/texture-2.png");
					square.setTexture(&texture);
				}
				else
				{
					texture.loadFromFile("Textures/texture-3.png");
					square.setTexture(&texture);
					validClick = false;
				}

			}*/





			/*sf::FloatRect recBounds = square.getGlobalBounds();
			sf::Vector2f vec = square.getPosition();
			sf::Vector2f circleVec = sf::Vector2f(vec.x + recBounds.width / 2, vec.y + recBounds.height / 2);
			//shape.setPosition(circleVec);


			sf::Font font;
			if (!font.loadFromFile("Fonts/Roboto-Black.ttf"))
			{
				// error...
				square.setFillColor(sf::Color::Cyan);
			}

			sf::Text text;

			// select the font
			text.setFont(font); // font is a sf::Font

			// set the string to display
			text.setString("3");

			// set the character size
			text.setCharacterSize(48); // in pixels, not points!

			// set the color
			text.setFillColor(sf::Color::Red);

			// set the text style
			text.setStyle(sf::Text::Bold | sf::Text::Underlined);

			sf::FloatRect textBounds(text.getLocalBounds());
			sf::Vector2f squareSize(square.getSize());

			//line below thanks to Mario from StackOverflow!
			text.setOrigin((textBounds.width - squareSize.x) / 2 + textBounds.left, (textBounds.height - squareSize.y) / 2 + textBounds.top);

			text.setPosition(square.getPosition());



			shape.setPosition(sf::Vector2f(0.f, 0.f));


			sf::Time elapsed = clock.restart();

			square.move(sf::Vector2f(20.f * elapsed.asSeconds(), 0.f));

			window.clear();

			window.draw(square);
			window.draw(text);
			window.draw(shape);

			window.display();*/


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
		square.isClicked = true;
		//square.square.setFillColor(sf::Color::White);

	}
}





void setupGameplayAndBoard(float boardXpos, float boardYpos, float width, int numSquaresWidth, int numSquaresHeight, int numMines)
{
	//create back button and its text
	gameplayBackButton = new sf::RectangleShape(sf::Vector2f(200, 100));


	gameplayFont = new sf::Font;
	if (!gameplayFont->loadFromFile("Fonts/Roboto-Black.ttf"))
	{
		// error...

	}

	gameplayBackButtonText = new sf::Text;
	// select the font
	gameplayBackButtonText->setFont(*gameplayFont); // font is a sf::Font

	gameplayBackButtonText->setString("BACK");

	gameplayBackButtonText->setCharacterSize(48); // in pixels, not points!

	gameplayBackButtonText->setFillColor(sf::Color::Red);

	//gameplayBackButtonText->setStyle(sf::Text::Bold | sf::Text::Underlined);
	gameplayBackButtonText->setStyle(sf::Text::Bold | sf::Text::Underlined);

	sf::FloatRect textBounds(gameplayBackButtonText->getLocalBounds());
	sf::Vector2f squareSize(gameplayBackButton->getSize());

	gameplayBackButtonText->setOrigin((textBounds.width - squareSize.x) / 2 + textBounds.left, (textBounds.height - squareSize.y) / 2 + textBounds.top);
	gameplayBackButtonText->setPosition(gameplayBackButton->getPosition());


	//creating the board
	//first, fill board with blank squares
	board = new Board;
	int numSquares = numSquaresWidth * numSquaresHeight;
	board->gameBoardArr = new GameSquare[numSquares];
	float currentXPos = boardXpos;
	float currentYPos = boardYpos;
	float side = width / numSquaresWidth;


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





	//calculate the border squares
	//board->borderSquares = new std::unordered_set<int>;
	//for (int i = 0; i < )

	srand(time(0));

	board->mineSet = new std::unordered_set<int>;
	//std::unordered_set<int> intSet;
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
	
	/*//go through the mine squares and calculate the flag squares that surround them. Check to make sure within bounds.
	for (auto it = intSet.begin(); it != intSet.end(); it++)
	{
		int index = *it;
		for (int i = 0; i <= 7; i++)
		{
			if (checkWithinBounds(index, i, numSquaresWidth, numSquaresHeight))
			{
				switch (i)
				{
				case 0:
					if (!board->gameBoardArr[index - numSquaresWidth].isMine)
					board->gameBoardArr[index - numSquaresWidth].
				}
			}
		}
	}*/



	//topleft/bottom not detected?????
	
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
					board->gameBoardArr[i].num.setFillColor(sf::Color::Green);
					break;
				case 3:
					board->gameBoardArr[i].num.setFillColor(sf::Color::Red);
					break;
				case 4:
					board->gameBoardArr[i].num.setFillColor(sf::Color::Red);
					break;
				case 5:
					board->gameBoardArr[i].num.setFillColor(sf::Color::Black);
					break;
				case 6:
					board->gameBoardArr[i].num.setFillColor(sf::Color::Cyan);
					break;
				case 7:
					board->gameBoardArr[i].num.setFillColor(sf::Color::Black);
					break;
				case 8:
					board->gameBoardArr[i].num.setFillColor(sf::Color::Black);
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
	bool top = false;
	bool left = false;
	bool right = false;
	bool bottom = false;

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

		//top = true;
	}
		
	if (boardIndex % numSquaresWidth == numSquaresWidth - 1)
	{
		canGoTopRight = false;
		canGoRight = false;
		canGoBRight = false;

		//right = true;
	}
		
	if (boardIndex >= numSquaresWidth * (numSquaresHeight - 1))
	{
		canGoBLeft = false;
		canGoDown = false;
		canGoBRight = false;
		//bottom = true;
	}

	if (boardIndex % numSquaresWidth == 0)
	{
		canGoTopLeft = false;
		canGoLeft = false;
		canGoBLeft = false;
		//left = true;
	}

	/*if (top & right)
	{
		canGoUp = false;
		canGoTopLeft = false;
		canGoRight = false;
		canGoBRight = false;
	}
	else if (top & left)
	{
		canGoUp = false;
		canGoTopRight = false;
		canGoLeft = false;
		canGoBLeft = false;
	}
	else if (bottom & right)
	{
		canGoDown = false;
		canGoBLeft = false;
		canGoTopRight = false;
		canGoRight = false;
	}
	else if (bottom & left)
	{
		canGoDown = false;
		canGoTopLeft = false;
		canGoLeft = false;
		canGoBRight = false;
	}*/


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

	/*if (boardIndex < numSquaresWidth)
		// check if top left
		if (boardIndex == 0)

		//check if top right
		else if (boardIndex == numSquaresWidth - 1)


		else

	else if (boardIndex % numSquaresWidth == numSquaresWidth - 1)

		//check if top right
		if (boardIndex == numSquaresWidth - 1)


		//check if bottom right
        else if (boardIndex == numSquaresWidth * numSquaresHeight - 1)

	    
		else

    
	else if (boardIndex >= numSquaresWidth * (numSquaresHeight - 1))

		//check if bottom left
		if (boardIndex == numSquaresWidth * (numSquaresHeight - 1))

		else if (boardIndex == numSquaresWidth * numSquaresHeight - 1)*/

}


void recurseBlankSquares(int index)
{
	board->gameBoardArr[index].square.setFillColor(sf::Color::White);
	board->gameBoardArr[index].square.setTexture(textureWhite);
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