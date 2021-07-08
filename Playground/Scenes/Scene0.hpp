#ifndef PL_SCENE0_HPP
#define PL_SCENE0_HPP
#include <ALC\ALC.hpp>

class Scene0 final : public ALC::IScene {
public:

	Scene0();
	~Scene0();

	void Init() override;
	void Exit() override;
	void Step(ALC::Timestep ts) override;
	void PreDraw() override;
	void Draw() override;
	void PostDraw() override;

private:
	ALC::Camera2D m_cam;
	ALC::ObjectFactory m_factory;
};

#endif // !PL_SCENE0_HPP