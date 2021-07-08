#include "Scene0.hpp"

Scene0::Scene0() { }

Scene0::~Scene0() { }

void Scene0::Init() {
	m_cam.SetCameraSize(ALC::vec2(1280.0f, 720.0f));
}

void Scene0::Exit() { }

void Scene0::Step(ALC::Timestep ts) { }

void Scene0::PreDraw() { }

void Scene0::Draw() {
	using SB = ALC::SpriteBatch;

	SB::Begin(m_cam);

	SB::End();
}

void Scene0::PostDraw() { }
