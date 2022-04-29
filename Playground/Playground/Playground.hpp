#ifndef PLAYGROUND_PLAYGROUND_HPP
#define PLAYGROUND_PLAYGROUND_HPP
#include <alc\alc.hpp>
#include "LevelBase.hpp"

class Playground final : public alc::game {
public:

	Playground();
	~Playground();

	void on_create() override;
	void on_destroy() override;
	void on_update(alc::timestep ts) override;

private:
	LevelBase* m_currentLevel;
};

#endif // !PLAYGROUND_PLAYGROUND_HPP