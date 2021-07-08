#include "PlaygroundGame.hpp"

PlaygroundGame::PlaygroundGame() { }

PlaygroundGame::~PlaygroundGame() { }

void PlaygroundGame::Init() { }

void PlaygroundGame::Exit() { }

void PlaygroundGame::Step(ALC::Timestep t) { }

void PlaygroundGame::PreDraw() {
	GetWindow()->ClearScreen(ALC_COLOR_BLACK);
}

void PlaygroundGame::Draw() { }

void PlaygroundGame::PostDraw() {
	GetWindow()->SwapBuffers();
}
