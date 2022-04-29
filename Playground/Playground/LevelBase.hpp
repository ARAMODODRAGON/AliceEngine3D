#ifndef PLAYGROUND_LEVEL_BASE_HPP
#define PLAYGROUND_LEVEL_BASE_HPP
#include <alc\alc.hpp>

class LevelBase : public alc::object {
public:

	LevelBase();
	virtual ~LevelBase() = 0;

	// event
	virtual void on_create() override;
	virtual void on_update(alc::timestep ts) override;

	// called on start to setup level
	// can be called to reset level at any point
	virtual void SetupLevel() = 0;
};

#endif // !PLAYGROUND_LEVEL_BASE_HPP