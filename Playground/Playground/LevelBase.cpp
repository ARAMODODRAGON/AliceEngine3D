#include "LevelBase.hpp"

LevelBase::LevelBase() { 
	// enable update
	set_update(true);
}

LevelBase::~LevelBase() { }

void LevelBase::on_create() {
	SetupLevel();
}

void LevelBase::on_update(alc::timestep ts) {
	using kb = alc::keyboard;
	using kc = alc::keycode;

	// get key presses
	auto ctrl = kb::get_key(kc::right_ctrl);
	auto rKey = kb::get_key(kc::key_r);

	// check if both are pressed (but not nessisarily at the same time) 
	if ((ctrl && rKey.pressed()) || (ctrl.pressed() && rKey))
		SetupLevel();

}
