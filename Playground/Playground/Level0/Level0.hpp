#ifndef PLAYGROUND_LEVEL0_HPP
#define PLAYGROUND_LEVEL0_HPP
#include "../LevelBase.hpp"
#include <alc\graphics3d\mesh_renderer.hpp>
#include <alc\graphics3d\camera3d.hpp>

class Level0 final : public LevelBase {
public:

	Level0();
	~Level0();

	// events
	void on_create() override;
	void on_update(alc::timestep ts) override;
	void SetupLevel() override;

private:
	alc::object3d* m_meshPivot;
	alc::mesh_renderer* m_meshRenderer;
	alc::camera3d* m_camera;
};

#endif // !PLAYGROUND_LEVEL0_HPP