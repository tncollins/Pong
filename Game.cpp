#include "Game.h"
#include <iostream>
#include <string>
const int thickness = 15;
const float paddleH = 100.0f;
Game::Game()
{
	mWindow = nullptr;
	mIsRunning = true;
	mPaddle1Pos.x = 10;
	mPaddle1Pos.y = 768/2;
	mPaddle2Pos.x = 1024 - 10-thickness;
	mPaddle2Pos.y = 768 / 2;

	mBallPos.x = 1024 / 2;
	mBallPos.y = 768 / 2;

	mBallVel.x = -200.0f;
	mBallVel.y = 235.0f;
}
bool Game::Initialize()
{
	int sdlResult = SDL_Init(SDL_INIT_VIDEO);

	if (sdlResult != 0)
	{
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}

	mWindow = SDL_CreateWindow(
		"Game Programming in C++(chapter 1)", //window title
		100, //Top left x-coordinate of window
		100, //Top left y-coordinate of window
		1024, //width of window
		768, //height f winodw
		0  //Flags (0 for no flags set)
	);

	mRenderer = SDL_CreateRenderer(
		mWindow,
		-1,//-1 unless multiple windows are present
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
	);

	if (!mWindow)
	{
		SDL_Log("Failed to create window: %s", SDL_GetError());
		return false;
	}

	if (!mRenderer)
	{
		SDL_Log("Failed to create renderer: %s", SDL_GetError());
		return false;
	}

	
	if (TTF_Init() == -1)
	{
		SDL_Log("Failed to create TTF: %s", SDL_GetError());
		return false;
	}
	mIsPaused = true;
	mLeftScore = 0;
	mRightScore = 0;
	mScore = to_string(mLeftScore) + " - " + to_string(mRightScore);
	return true;
}

void Game::Shutdown()
{
	SDL_DestroyWindow(mWindow);
	SDL_DestroyRenderer(mRenderer);
	SDL_Quit();
	TTF_Quit();
}

void Game::RunLoop()
{
	while (mIsRunning)
	{
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
}
void Game::ProcessInput()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			mIsRunning = false;
			break;
		}
	}

	//get state of keyboard
	const Uint8* state = SDL_GetKeyboardState(NULL);
	//if escape is pressed also end loop
	if (state[SDL_SCANCODE_ESCAPE])
	{
		mIsRunning = false;
	}

	if (state[SDL_SCANCODE_RETURN])
	{
		mIsPaused = !mIsPaused;
	}

	mPaddleDir = 0;
	mPaddle2Dir = 0;
	if (state[SDL_SCANCODE_W])
	{
		mPaddleDir -= 1;
	}

	if (state[SDL_SCANCODE_S])
	{
		mPaddleDir += 1;
	}

	if (state[SDL_SCANCODE_UP])
	{
		mPaddle2Dir -= 1;
	}

	if (state[SDL_SCANCODE_DOWN])
	{
		mPaddle2Dir += 1;
	}
}

void Game::UpdateGame()
{
	if (!mIsPaused)
	{
		while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16))
			;

		float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;

		if (deltaTime > 0.05f)
		{
			deltaTime = 0.05f;
		}

		mTicksCount = SDL_GetTicks();

		if (mPaddleDir != 0)
		{
			mPaddle1Pos.y += mPaddleDir * 300.0f*deltaTime;

			if (mPaddle1Pos.y < (paddleH / 2.0f + thickness))
			{
				mPaddle1Pos.y = paddleH / 2.0f + thickness;
			}
			else if (mPaddle1Pos.y > (768.0f - paddleH / 2.0f - thickness))
			{
				mPaddle1Pos.y = 768.0f - paddleH / 2.0f - thickness;
			}
		}

		if (mPaddle2Dir != 0)
		{
			mPaddle2Pos.y += mPaddle2Dir * 300.0f*deltaTime;

			if (mPaddle2Pos.y < (paddleH / 2.0f + thickness))
			{
				mPaddle2Pos.y = paddleH / 2.0f + thickness;
			}
			else if (mPaddle2Pos.y > (768.0f - paddleH / 2.0f - thickness))
			{
				mPaddle2Pos.y = 768.0f - paddleH / 2.0f - thickness;
			}
		}
		mBallPos.x += mBallVel.x*deltaTime;
		mBallPos.y += mBallVel.y*deltaTime;

		if (mBallPos.y <= thickness && mBallVel.y < 0.0f)
		{
			mBallVel.y *= -1;
		}

		if (mBallPos.y >= (768 - thickness) && mBallVel.y > 0.0f)
		{
			mBallVel.y *= -1;
		}


		float diff1 = mPaddle1Pos.y - mBallPos.y;
		if (diff1 < 0.0f)
		{
			diff1 *= -1;
		}
		float diff2 = mPaddle2Pos.y - mBallPos.y;

		if (diff2 < 0.0f)
		{
			diff2 *= -1;
		}
		if (diff1 <= paddleH / 2.0f && mBallPos.x <= 25.0f && mBallPos.x >= 20.0f && mBallVel.x < 0.0f)
		{
			mBallVel.x *= -1.0f;
		}

		if (diff2 <= paddleH / 2.0f && mBallPos.x >= 999.0f && mBallPos.x <= 1008.0f && mBallVel.x > 0.0f)
		{
			mBallVel.x *= -1.0f;
		}

		if (mBallPos.x > 1008.0f)
		{
			mLeftScore++;
			mScore = to_string(mLeftScore) + " - " + to_string(mRightScore);
			if (mLeftScore < 10)
			{
				reset();
			}
			else {
				//game over 
			}
		}

		if (mBallPos.x < 20.0f)
		{
			mRightScore++;
			mScore = to_string(mLeftScore) + " - " + to_string(mRightScore);
			if (mRightScore < 10)
			{
				reset();
			}
		}
	}
}

void Game::GenerateOutput()
{
	SDL_SetRenderDrawColor(
		mRenderer,
		0, //R
		0, //G
		255, //B
		255 //A
	);

	SDL_RenderClear(mRenderer);
	SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);
	SDL_Color color = {255,0,0};
	mfont = TTF_OpenFont("font.ttf", 50);
	surface = TTF_RenderText_Solid(mfont, mScore.c_str(), color);
	mtexture = SDL_CreateTextureFromSurface(mRenderer, surface);
	int height = 0;
	SDL_QueryTexture(mtexture,NULL,NULL,&height,&height);


	SDL_Rect wall{
		0,
		0,
		1024,
		thickness
	};
	SDL_Rect bWall{
		0,
		(768 - thickness),
		1024,
		thickness
	};
	SDL_Rect ball{
		static_cast<int>(mBallPos.x - thickness / 2),
		static_cast<int>(mBallPos.y - thickness / 2),
		thickness,
		thickness
	};
	
	SDL_Rect paddle1{
		static_cast<int>(mPaddle1Pos.x ),
		static_cast<int>(mPaddle1Pos.y -paddleH/2),
		thickness,
		static_cast<int>(paddleH)
	};
	SDL_Rect paddle2{
		static_cast<int>(mPaddle2Pos.x),
		static_cast<int>(mPaddle2Pos.y-paddleH/2),
		thickness,
		static_cast<int>(paddleH)
	};

	SDL_Rect score{
		1024/2-50,
		2*thickness,
		100,
		100
	};

	SDL_RenderFillRect(mRenderer, &wall);
	SDL_RenderFillRect(mRenderer, &bWall);
	SDL_RenderFillRect(mRenderer, &paddle1);
	SDL_RenderFillRect(mRenderer, &paddle2);
	SDL_RenderFillRect(mRenderer, &ball);
	SDL_RenderCopy(mRenderer, mtexture, NULL, &score);
	SDL_RenderPresent(mRenderer);

}

void Game::reset()
{
	mPaddle1Pos.y = 768 / 2;
	mPaddle2Pos.y = 768 / 2;

	mBallPos.x = 1024 / 2;
	mBallPos.y = 768 / 2;

	mBallVel.x = -200.0f;
	mBallVel.y = 235.0f;

	mIsPaused = true;
}