#include "Playground.hpp"
#include "Level0\Level0.hpp"

Playground::Playground() : m_currentLevel(nullptr) { }

Playground::~Playground() { }

void Playground::on_create() { 
	constexpr alc::uint32 loadIndex = 0;
	switch (loadIndex) {
		case 0: m_currentLevel = alc::world::create<Level0>(); break;
		default: break;
	}
}

void Playground::on_destroy() { }

void Playground::on_update(alc::timestep ts) { }
