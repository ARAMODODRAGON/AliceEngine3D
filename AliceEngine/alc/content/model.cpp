#include "model.hpp"
#include "../core/debug.hpp"
#define TINYOBJLOADER_IMPLEMENTATION
#include "../dependencies/tiny_obj_loader.h"
#include "content_manager.hpp"

namespace alc {

	model model::load(std::string file) {

		// variables
		tinyobj::attrib_t attributes;
		std::vector<tinyobj::shape_t> shapes;
		std::vector<tinyobj::material_t> materials;
		std::string errorMessage;

		// reserve some memory
		attributes.vertices.reserve(200);
		attributes.normals.reserve(200);
		attributes.texcoords.reserve(200);
		shapes.reserve(100);
		materials.reserve(10);

		// attempt to load using tiny_obj_loader
		if (!tinyobj::LoadObj(&attributes, &shapes, &materials, &errorMessage, file.c_str())) {
			// fail
			ALC_DEBUG_ERROR("tinyobj error: \"" + errorMessage + "\"");
			return model();
		}

		// now that we have successfully loaded our object, 
		// create a model to store all the meshes and related data
		model m;
		m.m_data.reset(new model::data_t());
		m.m_data->meshes.reserve(shapes.size());

		// create our variables to be used in assembling the meshes
		mesh::vertex vert;
		std::vector<mesh::vertex> verticies;

		// NOTE: big memory save could be to also include indicies as an array
		// requires some smart thinking that I can't do right now because im tired

		// now load each and every one into seperate vectors and create each mesh
		for (auto& shape : shapes) {
			verticies.reserve(shape.mesh.indices.size());
			verticies.clear();

			// start assembling the shape data into vectors which stores the verticies and indicies
			for (size_t i = 0; i < shape.mesh.indices.size(); i++) {
				auto [vertIndex, normIndex, texIndex] = shape.mesh.indices[i];
				if (vertIndex == -1) {
					ALC_DEBUG_WARNING("failed load vertex");
					continue;
				}

				// assign data
				// ignore index if it is -1

				if (vertIndex != -1) {
					vert.position.x = attributes.vertices[vertIndex];
					vert.position.y = attributes.vertices[vertIndex + 1];
					vert.position.z = attributes.vertices[vertIndex + 2];
				} else vert.position = glm::vec3(0.0f);

				if (normIndex != -1) {
					vert.normal.x = attributes.normals[normIndex];
					vert.normal.y = attributes.normals[normIndex + 1];
					vert.normal.z = attributes.normals[normIndex + 2];
				} else vert.normal = glm::vec3(0.0f);

				if (texIndex != -1) {
					vert.uv.x = attributes.texcoords[texIndex];
					vert.uv.y = attributes.texcoords[texIndex + 1];
				} else vert.uv = glm::vec2(0.0f);

				// add the vertex
				verticies.push_back(vert);

			} // indicies loop

			// finished assembling the verticies
			// now load as mesh
			auto mesh_ = mesh::create(verticies);

			// check if failed
			if (!mesh_) continue;

			// if successfully loaded add to meshes
			submesh subm;
			subm.m = mesh_;

			if (!shape.mesh.material_ids.size()) {
				ALC_DEBUG_ERROR("No material for associated mesh");
				continue;
			}
			if (shape.mesh.material_ids.size() > 1) {
				ALC_DEBUG_ERROR("Mutilple materials associated with mesh. Default to only use the first material");
				continue;
			}

			// quickly create material
			// NOTE: this should be further extended so materials are loaded 
			// and stored outside as a shared value
			auto& mat = materials[shape.mesh.material_ids[0]];
			subm.mat.diffuseTexture = content_manager::load_texture(mat.diffuse_texname);
			subm.mat.ambient = { mat.ambient[0], mat.ambient[1], mat.ambient[2] };
			subm.mat.diffuse = { mat.diffuse[0], mat.diffuse[1], mat.diffuse[2] };
			subm.mat.specular = { mat.specular[0], mat.specular[1], mat.specular[2] };
			subm.mat.shinyness = mat.shininess;
			subm.mat.transparency = mat.dissolve;

			// add mesh
			m.m_data->meshes.push_back(subm);

		} // shapes loop

		// return the created model
		return m;
	}

	bool model::unload(model& m) {
		// unload each mesh in the model, then free the model from memory
		for (size_t i = 0; i < m.m_data->meshes.size(); i++) {
			mesh::unload(m.m_data->meshes[i].m);
		}
		m.m_data->meshes.clear();
		m.m_data.reset();
		return false;
	}

	long model::get_shared_count() const {
		return m_data.use_count();
	}

}