#include "Level0.hpp"
#include <alc\content\obj_loader.hpp>

Level0::Level0() { }

Level0::~Level0() { }

void Level0::on_create() {
	LevelBase::on_create();

	m_meshPivot = create_object<alc::object3d>("Mesh Pivot");

	m_meshRenderer = m_meshPivot->create_object<alc::mesh_renderer>("Mesh");
	m_meshRenderer->set_relative_position(glm::vec3(-0.5f));
	std::vector<std::pair<alc::mesh_ref, alc::material>> meshes;
	alc::tools::load_obj("cube.obj", meshes);
	if (meshes.size() > 0) {
		m_meshRenderer->set_mesh(meshes[0].first);
		//m_meshRenderer->set_material(meshes[0].second);
	}

	m_camera = create_object<alc::camera3d>("Camera");
	m_camera->set_position(glm::vec3(0.0f, 2.0f, 6.0f));
	m_camera->look_at(glm::vec3(0.0f)); // look at center of world
}

void Level0::on_update(alc::timestep ts) {
	LevelBase::on_update(ts);
	constexpr float speed = 1.0f;

	bool right = alc::keyboard::get_key(alc::keycode::arrow_right).held();
	bool left = alc::keyboard::get_key(alc::keycode::arrow_left).held();
	
	if (right != left) {
		float rotation = 0.0f;
		if (right) rotation = speed * ts;
		if (left) rotation = -speed * ts;

		auto rot = m_meshPivot->get_rotation();
		rot = glm::quat(glm::vec3(0.0f, rotation, 0.0f)) * rot;
		m_meshPivot->set_rotation(rot);
	}

}

void Level0::SetupLevel() { }
