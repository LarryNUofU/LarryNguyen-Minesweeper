#include <SFML/Graphics.hpp>
#include <Windows.h>

bool inMainMenu = true;
bool easyMode = false;
bool medMode = false;
bool setupGameplay = false;


sf::RectangleShape *gameplayBackButton;
sf::Text *gameplayBackButtonText;



void setupGameplayAndBoard(float boardXpos, float boardYpos, float width, int numSquaresWidth, int numSquaresHeight, int numMines);


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
	int *mineArray;
	int *borderSquares;

	int widthSquares;
	int heightSquares;
	float startingPosX;
	float startingPosY;
	float lenOfSquare;
};

void RecurseBlankSquares(int num, Board &board);


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


	bool goodMode = false;
	bool validClick = true;
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
				setupGameplayAndBoard(433.f, 70.f, 414.f, 9, 9, 8);
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
					sf::FloatRect fr = square.getGlobalBounds();
					sf::Vector2i mouseCoords = sf::Mouse::getPosition(window);
					sf::Vector2f worldPos = window.mapPixelToCoords(mouseCoords);
					if (fr.contains(worldPos.x, worldPos.y) && validClick)
						square.setFillColor(sf::Color::White);

					validClick = true;
				}

			}


			if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && validClick)
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

			}




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


	sf::Font font;
	if (!font.loadFromFile("Fonts/Roboto-Black.ttf"))
	{
		// error...

	}

	sf::Text text;
	// select the font
	text.setFont(font); // font is a sf::Font

	// set the string to display
	text.setString("BACK");

	// set the character size
	text.setCharacterSize(48); // in pixels, not points!

	// set the color
	text.setFillColor(sf::Color::Red);

	// set the text style
	text.setStyle(sf::Text::Bold | sf::Text::Underlined);

	sf::FloatRect textBounds(text.getLocalBounds());
	sf::Vector2f squareSize(gameplayBackButton->getSize());

	//line below thanks to Mario from StackOverflow!
	text.setOrigin((textBounds.width - squareSize.x) / 2 + textBounds.left, (textBounds.height - squareSize.y) / 2 + textBounds.top);
	text.setPosition(gameplayBackButton->getPosition());


	//creating the board








}