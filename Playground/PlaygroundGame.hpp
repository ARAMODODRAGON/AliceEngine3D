#ifndef PL_PLAYGROUNDGAME_HPP
#define PL_PLAYGROUNDGAME_HPP
#include <ALC\ALC.hpp>

class PlaygroundGame final : public ALC::Game {
public:

	PlaygroundGame();
	~PlaygroundGame();

	void Init() override;

	void Exit() override;

	void Step(ALC::Timestep t) override;

	void PreDraw() override;
	void Draw() override;
	void PostDraw() override;

private:
};


#endif // !PL_PLAYGROUNDGAME_HPP