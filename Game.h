#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <iostream>
using namespace std;
struct Vector2
{
	float x;
	float y;
};
class Game
{
public:
	Game();
	//Initialize the game
	bool Initialize();
	//runs game loop
	void RunLoop();
	//shutdown the game
	void Shutdown();

private:
	//helper functions for game loop
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();
	void reset();
	//window created by SDL
	SDL_Window* mWindow;

	//game shoud continue to run
	bool mIsRunning;
	//Pause game
	bool mIsPaused;
	SDL_Renderer* mRenderer;

	Vector2 mPaddle1Pos;
	Vector2 mPaddle2Pos;
	Vector2 mBallPos;
	Vector2 mBallVel;
	Uint32 mTicksCount;
	int mLeftScore;
	int mRightScore;
	int mPaddleDir;
	int mPaddle2Dir;

	TTF_Font* mfont;
	SDL_Surface* surface;
	SDL_Texture* mtexture;
	string mScore;
};