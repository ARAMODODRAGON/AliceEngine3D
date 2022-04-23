#include "obj_loader.hpp"
#define TINYOBJLOADER_IMPLEMENTATION
#include "../dependencies/tiny_obj_loader.h"

namespace alc {
	namespace tools {

		size_t load_obj(const std::string& filepath, std::vector<std::pair<mesh_ref, material>>& out) {
			std::string realFilepath = content_manager::get_full_path(filepath);

			size_t initSize = out.size();

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
			if (!tinyobj::LoadObj(&attributes, &shapes, &materials, &errorMessage, realFilepath.c_str())) {
				// fail
				ALC_DEBUG_ERROR("tinyobj error: \"" + errorMessage + "\"");
				return 0;
			}

			// create our variables to be used in assembling the meshes
			mesh::basic_vertex vert;
			std::vector<mesh::basic_vertex> verticies;
			//std::vector<uint32> indicies;

			// NOTE: big memory save could be to also include indicies as an array
			// requires some smart thinking that I can't do right now because im tired

			// now load each and every one into seperate vectors and create each mesh
			for (auto& shape : shapes) {
				verticies.reserve(shape.mesh.indices.size());
				verticies.clear();

				// start assembling the shape data into vectors which stores the verticies and indicies
				for (size_t i = 0; i < shape.mesh.indices.size(); i++) {
					int vertIndex = shape.mesh.indices[i].vertex_index;
					int normIndex = shape.mesh.indices[i].normal_index;
					int texIndex = shape.mesh.indices[i].texcoord_index;
					if (vertIndex == -1) {
						ALC_DEBUG_WARNING("failed load vertex");
						continue;
					}

					if (normIndex == -1 && attributes.normals.size())
						normIndex = vertIndex;

					if (texIndex == -1 && attributes.texcoords.size())
						texIndex = vertIndex;

					// assign data
					// ignore index if it is -1

					if (vertIndex != -1) {
						vert.position.x = attributes.vertices[3 * vertIndex];
						vert.position.y = attributes.vertices[3 * vertIndex + 1];
						vert.position.z = attributes.vertices[3 * vertIndex + 2];
					} else vert.position = glm::vec3(0.0f);

					if (normIndex != -1) {
						vert.normal.x = attributes.normals[3 * normIndex];
						vert.normal.y = attributes.normals[3 * normIndex + 1];
						vert.normal.z = attributes.normals[3 * normIndex + 2];
					} else vert.normal = glm::vec3(0.0f);

					if (texIndex != -1) {
						vert.uv.x = attributes.texcoords[2 * texIndex];
						vert.uv.y = attributes.texcoords[2 * texIndex + 1];
					} else vert.uv = glm::vec2(0.0f);

					// add the vertex
					verticies.push_back(vert);

				} // indicies loop

				// if there are no normals, generate them
				if (attributes.normals.size() == 0) {
					for (size_t i = 0; i < verticies.size(); i += 3) {
						glm::vec3 normal = glm::normalize(glm::cross(
							verticies[i + 1].position - verticies[i].position,
							verticies[i + 2].position - verticies[i].position
						));
						verticies[i + 0].normal = normal;
						verticies[i + 1].normal = normal;
						verticies[i + 2].normal = normal;
					}
				}

				// finished assembling the verticies
				// now load as mesh
				mesh_ref mesh_(new mesh(verticies.data(), verticies.size(), nullptr, 0, mesh::basic_vertex::basic_info()));

				// check if failed
				if (!mesh_->is_valid()) continue;

				// if successfully loaded add to meshes

				if (!shape.mesh.material_ids.size()) {
					ALC_DEBUG_ERROR("No material for associated mesh");
					continue;
				}

				material mat;
				if (materials.size()) {
					auto& shapemat = materials[shape.mesh.material_ids[0]];

					// quickly create material
					// NOTE: this should be further extended so materials are loaded 
					// and stored outside as a shared value
					mat.diffuseTexture = content_manager::create<texture>(shapemat.diffuse_texname);
					mat.ambient = { shapemat.ambient[0], shapemat.ambient[1], shapemat.ambient[2] };
					mat.diffuse = { shapemat.diffuse[0], shapemat.diffuse[1], shapemat.diffuse[2] };
					mat.specular = { shapemat.specular[0], shapemat.specular[1], shapemat.specular[2] };
					mat.shinyness = shapemat.shininess;
					mat.transparency = shapemat.dissolve;
				}

				// add mesh
				out.emplace_back(mesh_, mat);

			} // shapes loop
			return out.size() - initSize;
		} // load_obj

	}
}
