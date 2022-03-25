#include <iostream>
#include <alc\alc.hpp>
#include <alc\core\alice_events.hpp>
#include <alc\objects\singleton.hpp>
#include <alc\jobs\job_queue.hpp>
#include <alc\objects\world_object.hpp>
#include <alc\graphics3d\mesh_component.hpp>
#include <alc\graphics3d\camera3d.hpp>
#include <alc\content\obj_loader.hpp>

class DunesTerrain : public alc::world_object {
public:

	DunesTerrain() {
		alc::alice_events::onUpdate += alc::make_function<&DunesTerrain::on_update>(this);
	}
	~DunesTerrain() {
		alc::alice_events::onUpdate -= alc::make_function<&DunesTerrain::on_update>(this);
	}

	struct DesertTerrainVertex {
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 uv;
	};
	const alc::uint32 X_SIZE = 20;
	const alc::uint32 Z_SIZE = 20;
	const float MIN_X_RANGE = -30.0f;
	const float MAX_X_RANGE = 30.0f;
	const float MIN_Z_RANGE = -30.0f;
	const float MAX_Z_RANGE = 30.0f;

	void create_mesh() {
		//alc::mesh_info info;
		//info.vertexSize = sizeof(DesertTerrainVertex);
		//info.attributes.emplace_back(alc::attribute_type::type_float, 3, (void*)offsetof(DesertTerrainVertex, position));
		//info.attributes.emplace_back(alc::attribute_type::type_float, 3, (void*)offsetof(DesertTerrainVertex, normal));
		//info.attributes.emplace_back(alc::attribute_type::type_float, 2, (void*)offsetof(DesertTerrainVertex, uv));
		//
		//// define and reserve our vectors
		//std::vector<DesertTerrainVertex> verticies;
		//verticies.reserve(X_SIZE * Z_SIZE);
		//std::vector<alc::uint32> indices;
		//indices.reserve((X_SIZE - 1) * (Z_SIZE - 1) * 6);
		//
		//// vertex
		//DesertTerrainVertex vert;
		//vert.normal = glm::vec3(0.0f, 1.0f, 0.0f);
		//
		//// define verticies
		//for (size_t x = 0; x < X_SIZE; x++) {
		//	const float xScale = float(x) / (X_SIZE - 1);
		//
		//	for (size_t z = 0; z < Z_SIZE; z++) {
		//		const float zScale = float(z) / (Z_SIZE - 1);
		//
		//		vert.position.x = glm::mix(MIN_X_RANGE, MAX_X_RANGE, xScale);
		//		vert.position.y = 0.0f;
		//		vert.position.z = glm::mix(MIN_Z_RANGE, MAX_Z_RANGE, zScale);
		//		vert.uv.x = xScale;
		//		vert.uv.y = zScale;
		//
		//		verticies.push_back(vert);
		//	}
		//}
		//
		//// define indices
		//for (size_t x = 0; x < (X_SIZE - 1); x++) {
		//	for (size_t z = 0; z < (Z_SIZE - 1); z++) {
		//		// define indices for the four corners
		//		const size_t topLeft = x + (z * Z_SIZE);
		//		const size_t topRight = (x + 1) + (z * Z_SIZE);
		//		const size_t bottomLeft = x + ((z + 1) * Z_SIZE);
		//		const size_t bottomRight = (x + 1) + ((z + 1) * Z_SIZE);
		//
		//		// upper left triangle
		//		indices.emplace_back(bottomLeft);
		//		indices.emplace_back(topRight);
		//		indices.emplace_back(topLeft);
		//		// lower right triangle
		//		indices.emplace_back(bottomLeft);
		//		indices.emplace_back(bottomRight);
		//		indices.emplace_back(topRight);
		//	}
		//}
		// 
		//m_desertMesh.reset(new alc::mesh(verticies.data(), verticies.size(), indices.data(), indices.size(), info));

		std::vector<std::pair<alc::mesh_ref, alc::material>> meshes;
		if (alc::tools::load_obj("cube.obj", meshes)) {
			m_desertMesh = meshes[0].first;
			m_material = meshes[0].second;
		}

	}

	void on_create() override {
		create_mesh();
		m_meshComponent = create_component<alc::mesh_component>();
		m_meshComponent->set_mesh(m_desertMesh);
	}

	void on_update(alc::timestep ts) {
		if (alc::keyboard::get_key(alc::keycode::key_f)) {
			set_position(get_position() + glm::vec3(0.0f, ts, 0.0f));
		}
	}

	void on_destroy() override { }

private:
	alc::mesh_ref m_desertMesh;
	alc::material m_material;
	alc::mesh_component* m_meshComponent;
};

class Level final : public alc::object {
public:

	Level()
		: m_yaw(180.0f), m_pitch(0.0f), m_zoom(1.0f) {
		alc::alice_events::onUpdate += alc::make_function<&Level::on_update>(this);
	}
	~Level() {
		alc::alice_events::onUpdate -= alc::make_function<&Level::on_update>(this);
	}

	void on_create() override {

		// create camera
		m_cameraBoom = create_object<alc::world_object>("Camera Boom");
		m_cameraBoom->set_position(glm::vec3(0.0f)); // position at center of the world
		m_camera = m_cameraBoom->create_object<alc::camera3d>("Camera");
		m_camera->set_relative_position(glm::vec3(0.0f, 1.0f, 3.0f));
		m_camera->look_at(glm::vec3(0.0f));

		// create terrain
		m_terrain = create_object<DunesTerrain>("Dune Terrain");
	}

	void on_destroy() override { }

	void on_update(alc::timestep ts) {
		const bool left = alc::keyboard::get_key(alc::keycode::arrow_left);
		const bool right = alc::keyboard::get_key(alc::keycode::arrow_right);
		const bool up = alc::keyboard::get_key(alc::keycode::arrow_up);
		const bool down = alc::keyboard::get_key(alc::keycode::arrow_down);
		constexpr float speed = 4.0f;
		constexpr float zoomspeed = 20.0f;

		auto rotation = m_cameraBoom->get_rotation();

		if (left != right) {
			if (left) rotation = glm::rotate(rotation, speed * ts, glm::vec3(0.0f, 1.0f, 0.0f));
			if (right) rotation = glm::rotate(rotation, -speed * ts, glm::vec3(0.0f, 1.0f, 0.0f));
		}

		m_cameraBoom->set_rotation(rotation);

		float length = glm::length(m_camera->get_relative_position());
		glm::vec3 dir = glm::normalize(m_camera->get_relative_position());

		if (up != down) {
			if (up) length -= zoomspeed * ts;
			if (down) length += zoomspeed * ts;
			length = glm::clamp(length, 1.0f, 100.0f);
		}

		m_camera->set_relative_position(dir * length);
	}

private:

	float m_yaw, m_pitch, m_zoom;
	alc::world_object* m_cameraBoom;
	alc::camera3d* m_camera;
	DunesTerrain* m_terrain;

};

class Playground : public alc::game {
public:

	void on_create() override {
		alc::world::create<Level>();
	}

	void on_destroy() override {

	}

	void on_update(alc::timestep ts) override {

	}

};

static const alc::engine_settings GetSettings() {
	alc::engine_settings set;

	set.window.titlebar = "Playground";
	set.window.size = glm::uvec2(1280u, 720u);

	set.gameBinding = alc::bind_game<Playground>();

	set.renderer2d.enabled = false;
	//set.renderer2d.layerInfo = {
	//	alc::bind_layer("Layer 0"),
	//	alc::bind_layer("Layer 1"),
	//	alc::bind_layer("Layer 2")
	//};

	//set.jobs.enabled = true;
	//set.jobs.maxthreads = 4;

	return set;
}

int main(int argc, char* argv[]) {

	// get settings
	const alc::engine_settings set = GetSettings();

	// start the engine
	alc::engine::start(&set);

	return 0;
}
