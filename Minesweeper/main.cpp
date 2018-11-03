#include <SFML/Graphics.hpp>
#include <Windows.h>
#include <stdlib.h>
#include <unordered_set>

bool inMainMenu = true;
bool easyMode = true;
bool medMode = false;
bool setupGameplay = false;

sf::Texture *textureDefault;


sf::RectangleShape *gameplayBackButton;
sf::Font *gameplayFont;
sf::Text *gameplayBackButtonText;



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

void drawAndUpdate(GameSquare &square);



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

void RecurseBlankSquares(int num, Board &board);


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


	while (window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
			window.close();
		if (event.type == sf::Event::MouseButtonReleased)
		{
			sf::FloatRect fr = easyButton.getGlobalBounds();
			sf::Vector2i mouseCoords = sf::Mouse::getPosition(window);
			sf::Vector2f worldPos = window.mapPixelToCoords(mouseCoords);
			if (fr.contains(worldPos.x, worldPos.y))
			{
				inMainMenu = false;
				//easyMode = true;
				setupGameplay = true;
				clock->restart();
			}
				
		}

	}
	



	window.clear();
	window.draw(easyButton);
	window.draw(text);
	window.display();



}






int main()
{
	FreeConsole();
	

	sf::RenderWindow window(sf::VideoMode(1280, 720), "SFML works!");
	
	sf::CircleShape shape(100.f);
	sf::RectangleShape square(sf::Vector2f(100, 100));
	square.setPosition(200.f, 0.f);
	shape.setFillColor(sf::Color::Blue);
	//square.setFillColor(sf::Color::Green);
	square.setOutlineColor(sf::Color::Blue);
	square.setOutlineThickness(-1.f);

	sf::Texture texture;
	if (!texture.loadFromFile("Textures/texture-3.png"))
	{
		//error
	}
	square.setTexture(&texture);

	textureDefault = &texture;



	bool goodMode = false;
	bool validClick = false;
	bool validRelease = true;

	bool clickedOnBackButton = false;



	bool leftClickOn = false;
	bool rightClickOn = false;

	sf::RectangleShape *squareOfInterest = nullptr;


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
				setupGameplayAndBoard(433.f, 153.f, 414.f, 9, 9, 8);


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

						}
						else if (rightClickOn)
						{

						}
						sf::FloatRect fr = square.getGlobalBounds();
						sf::Vector2i mouseCoords = sf::Mouse::getPosition(window);
						sf::Vector2f worldPos = window.mapPixelToCoords(mouseCoords);
						if (fr.contains(worldPos.x, worldPos.y) && validClick)
							square.setFillColor(sf::Color::White);
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

				//make sure left click either on a square or the back button
				sf::FloatRect backFr = gameplayBackButton->getGlobalBounds();

				sf::FloatRect fr = square.getGlobalBounds();

				sf::Vector2i mouseCoords = sf::Mouse::getPosition(window);
				sf::Vector2f worldPos = window.mapPixelToCoords(mouseCoords);

				
				if (backFr.contains(worldPos.x, worldPos.y))
				{

				}
				else if (fr.contains(worldPos.x, worldPos.y))
				{
					
					
					
					//if (fr.contains(worldPos.x, worldPos.y))
					//{
						squareOfInterest = &square;
						texture.loadFromFile("Textures/texture-2.png");
						square.setTexture(&texture);
					//}
					//else
					//{
					//	texture.loadFromFile("Textures/texture-3.png");
					//	square.setTexture(&texture);
					//}
				}
				else
				{
					validRelease = false;
				}
			}


			if (rightClickOn && !leftClickOn && validRelease)
			{

			}



			if (!validRelease && squareOfInterest != nullptr)
			{
				texture.loadFromFile("Textures/texture-3.png");
				square.setTexture(&texture);
				squareOfInterest = nullptr;
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





			sf::FloatRect recBounds = square.getGlobalBounds();
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

			window.display();
		}

	}

	return 0;

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

		gs.num.setCharacterSize(48); // in pixels, not points!

		gs.num.setFillColor(sf::Color::Red);

		gs.num.setStyle(sf::Text::Bold | sf::Text::Underlined);

		sf::FloatRect textBounds(gs.num.getLocalBounds());
		sf::Vector2f squareSize(gs.square.getSize());

		gs.num.setOrigin((textBounds.width - squareSize.x) / 2 + textBounds.left, (textBounds.height - squareSize.y) / 2 + textBounds.top);
		gs.num.setPosition(gs.square.getPosition());


		//set visuals of the square
		square.setFillColor(sf::Color::Blue);
		square.setOutlineColor(sf::Color::Blue);
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

	bool canGoUp = false;
	bool canGoLeft = false;
	bool canGoRight = false;
	bool canGoDown = false;
	bool canGoTopLeft = false;
	bool canGoTopRight = false;
	bool canGoBLeft = false;
	bool canGoBRight = false;

	if (boardIndex < numSquaresWidth)
	{
		canGoDown = true;
		canGoLeft = true;
		canGoRight = true;
		canGoBLeft = true;
		canGoBRight = true;
		top = true;
	}
		
	if (boardIndex % numSquaresWidth == numSquaresWidth - 1)
	{
		canGoUp = true;
		canGoDown = true;
		canGoLeft = true;
		canGoBLeft = true;
		canGoTopLeft = true;
		right = true;
	}
		
	if (boardIndex >= numSquaresWidth * (numSquaresHeight - 1))
	{
		canGoRight = true;
		canGoUp = true;
		canGoLeft = true;
		canGoTopLeft = true;
		canGoTopRight = true;
		bottom = true;
	}

	if (boardIndex % numSquaresWidth == 0)
	{
		canGoUp = true;
		canGoRight = true;
		canGoDown = true;
		canGoTopRight = true;
		canGoBLeft = true;
		left = true;
	}

	if (top & right)
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