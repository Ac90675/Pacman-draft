#include "Pacman.h"

#include <sstream>

Pacman::Pacman(int argc, char* argv[]) : Game(argc, argv), _cPacmanSpeed(0.1f), _cPacmanFrameTime(250)
{
	_pacmanCurrentFrameTime = 0; 
	_pacmanFrame = 0;
	_frameCount = 0;
	_paused = false;
	_startsc = true;
	_pKeyDown = false;

	//Initialise important Game aspects
	Graphics::Initialise(argc, argv, this, 1024, 768, false, 25, 25, "Pacman", 60);
	Input::Initialise();

	// Start the Game Loop - This calls Update and Draw in game loop
	Graphics::StartGameLoop();
}

Pacman::~Pacman()
{
	delete _pacmanTexture;
	delete _pacmanSourceRect;
	delete _munchieBlueTexture;
	delete _munchieInvertedTexture;
	delete _munchieRect;
}

void Pacman::LoadContent()
{
	// Load Pacman
	_pacmanTexture = new Texture2D();
	_pacmanTexture->Load("Textures/Pacman.tga", false);
	_pacmanPosition = new Vector2(350.0f, 350.0f);
	_pacmanSourceRect = new Rect(0.0f, 0.0f, 32, 32);
	_pacmanDirection = 0;

	// Load Munchie
	_munchieBlueTexture = new Texture2D();
	_munchieBlueTexture->Load("Textures/Munchie.tga", true);
	_munchieInvertedTexture = new Texture2D();
	_munchieInvertedTexture->Load("Textures/MunchieInverted.tga", true);
	_munchieRect = new Rect(100.0f, 450.0f, 12, 12);

	// Set string position
	_stringPosition = new Vector2(10.0f, 25.0f);

	// Set Menu Paramters
	_menuBackground = new Texture2D();
	_menuBackground->Load("Textures/Transparency.png", false);
	_menuRectangle = new Rect(0.0f, 0.0f, Graphics::GetViewportWidth(),
	Graphics::GetViewportHeight());
	_menuStringPosition = new Vector2(Graphics::GetViewportWidth() / 2.0f,
		Graphics::GetViewportHeight() / 2.0f);
}

void Pacman::Update(int elapsedTime)
{
	// Gets the current state of the keyboard
	Input::KeyboardState* keyboardState = Input::Keyboard::GetState();

	if (_startsc)
	{

		if (keyboardState->IsKeyDown(Input::Keys::SPACE))
		{
			_startsc = !_startsc;
		}
	}

	Pause(keyboardState, elapsedTime);
	
}

void Pacman::Pause(Input::KeyboardState* keyboardState, int elapsedTime)
{
	if (keyboardState->IsKeyDown(Input::Keys::P))
	{
		if (!_pKeyDown)
		{
			_paused = !_paused;
			_pKeyDown = true;
		}
	}

	if (keyboardState->IsKeyUp(Input::Keys::P))
	{
		_pKeyDown = false;
	}

	if (_pacmanCurrentFrameTime > _cPacmanFrameTime)
	{
		_pacmanFrame++;

		if (_pacmanFrame >= 2)
			_pacmanFrame = 0;
		_pacmanCurrentFrameTime = 0;
	}

	if (!_paused)
	{
		// Checks if D key is pressed
		if (keyboardState->IsKeyDown(Input::Keys::D))
		{
			_pacmanCurrentFrameTime += elapsedTime; //For checking how many frames have elapsed
			_pacmanPosition->X += _cPacmanSpeed * elapsedTime; //Moves Pacman across X axis
			_pacmanSourceRect->Y = 0.0f;
		}
		else if (keyboardState->IsKeyDown(Input::Keys::W))
		{
			_pacmanPosition->Y -= _cPacmanSpeed * elapsedTime; //Moves Pacman across X axis
			_pacmanSourceRect->Y = 96.0f;
		}
		else if (keyboardState->IsKeyDown(Input::Keys::S))
		{
			_pacmanPosition->Y += _cPacmanSpeed * elapsedTime; //Moves Pacman across X axis
			_pacmanSourceRect->Y = 32.0f;
			_pacmanDirection = 1;
		}
		if (keyboardState->IsKeyDown(Input::Keys::A))
		{
			_pacmanPosition->X -= _cPacmanSpeed * elapsedTime; //Moves Pacman across X axis
			_pacmanSourceRect->Y = 64.0f;
		}
		// Checks if pacman is trying to disappear
		if (_pacmanPosition->X > Graphics::GetViewportHeight())

		{  // Pacman hit right wall - reset his position
			_pacmanPosition->X = -_pacmanSourceRect->Width;
		}

	}
}

void Pacman::Draw(int elapsedTime)
{
	// Allows us to easily create a string
	std::stringstream stream;
	stream << "Pacman X: " << _pacmanPosition->X << " Y: " << _pacmanPosition->Y;
	SpriteBatch::BeginDraw(); // Starts Drawing
	
	if (_startsc)
	{
		std::stringstream menuStream;
		menuStream << "Start!";

		SpriteBatch::Draw(_menuBackground, _menuRectangle, nullptr);
		SpriteBatch::DrawString(menuStream.str().c_str(), _menuStringPosition,
			Color::Red);
	}

	if (_paused)
	{
		// Draws String
		SpriteBatch::DrawString(stream.str().c_str(), _stringPosition, Color::Green);
		{
			std::stringstream menuStream;
			menuStream << "PAUSED!";

			SpriteBatch::Draw(_menuBackground, _menuRectangle, nullptr);
			SpriteBatch::DrawString(menuStream.str().c_str(), _menuStringPosition,
				Color::Red);
		}
	}
	else
	{
		SpriteBatch::Draw(_pacmanTexture, _pacmanPosition, _pacmanSourceRect); // Draws Pacman

		if (_frameCount < 30)
		{
			// Draws Red Munchie
			SpriteBatch::Draw(_munchieInvertedTexture, _munchieRect, nullptr, Vector2::Zero, 1.0f, 0.0f, Color::White, SpriteEffect::NONE);

			_frameCount++;
		}
		else
		{
			// Draw Blue Munchie
			SpriteBatch::Draw(_munchieBlueTexture, _munchieRect, nullptr, Vector2::Zero, 1.0f, 0.0f, Color::White, SpriteEffect::NONE);

			_frameCount++;

			if (_frameCount >= 60)
				_frameCount = 0;
		}
		// Code for start screen
		SpriteBatch::DrawString(stream.str().c_str(), _stringPosition, Color::Green);
	}

	SpriteBatch::EndDraw();
}