#include "scenegraph3d.hpp"
#include <glew.h>
#include "detail\basic_mesh_shader.hpp"
#include "camera3d.hpp"
#include <glm/gtc/type_ptr.hpp>

namespace alc {
	namespace gfx3d {

		scenegraph3d::shader_group* scenegraph3d::get_shader_group(shader_ref s) {
			// return the default shader group
			if (!s || !s->is_valid()) return &(s_shaderGroups.front());
			// find the shader group matching this one
			for (size_t i = 1; i < s_shaderGroups.size(); i++) {
				if (s_shaderGroups[i].usedShader == s) {
					return &(s_shaderGroups[i]);
				}
			}
			// failed, make new group
			s_shaderGroups.emplace_back(s);
			return &(s_shaderGroups.back());
		}

		scenegraph3d::mesh_group* scenegraph3d::get_mesh_group(shader_group* group, mesh_ref m) {
			if (!m || !m->is_valid()) return nullptr;
			// find mesh group
			for (size_t i = 0; i < group->dynamicMeshGroups.size(); i++) {
				if (group->dynamicMeshGroups[i].usedMesh == m) {
					return &(group->dynamicMeshGroups[i]);
				}
			}
			// failed, make new group
			group->dynamicMeshGroups.emplace_back(m);
			return &(group->dynamicMeshGroups.back());
		}

		void scenegraph3d::__init(const engine_settings* set) {
			// load default shader
			if (set->renderer3d.defaultShader != "")
				s_defaultShader.reset(new shader(set->renderer3d.defaultShader));
			if (!s_defaultShader || !s_defaultShader->is_valid())
				s_defaultShader.reset(new shader(c_basic_mesh_shader, shader::IS_SOURCE));

			s_shaderGroups.emplace_back(s_defaultShader);
		}

		void scenegraph3d::__exit() {
			s_cameras.clear();
			s_shaderGroups.clear();
			if (s_defaultShader) s_defaultShader.force_delete();
			s_defaultShader = nullptr;
		}

		void scenegraph3d::__draw() {
			// start with main loop for cameras
			for (auto* cam : s_cameras) {
				// get view and projection
				const glm::mat4 view = cam->get_view();
				const glm::mat4 projection = cam->get_projection();

				/////// TODO: set some camera related rendering options

				// loop through shader groups
				for (auto& shaderGroup : s_shaderGroups) {
					// bind shader
					glUseProgram(shaderGroup.usedShader);

					// get common shader uniforms
					uint32 uView = shaderGroup.usedShader->get_uniform("u_view");
					uint32 uProjection = shaderGroup.usedShader->get_uniform("u_projection");
					uint32 uModel = shaderGroup.usedShader->get_uniform("u_model");

					glUniformMatrix4fv(uView, 1, GL_FALSE, glm::value_ptr(view));
					glUniformMatrix4fv(uProjection, 1, GL_FALSE, glm::value_ptr(projection));

					// loop through meshes
					for (auto& meshGroups : shaderGroup.dynamicMeshGroups) {
						bool useElements = meshGroups.usedMesh->get_ebo() != -1;

						// bind vao/vbo/ebo
						glBindVertexArray(meshGroups.usedMesh->get_vao());
						if (useElements)
							glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshGroups.usedMesh->get_ebo());
						else
							glBindBuffer(GL_ARRAY_BUFFER, meshGroups.usedMesh->get_vbo());

						// loop through and draw all meshes
						for (auto* meshComponent : meshGroups.dynamicMeshes) {
							const glm::mat4 model = meshComponent->get_transform();

							// set all required uniforms
							glUniformMatrix4fv(uModel, 1, GL_FALSE, glm::value_ptr(model));

							// draw
							if (useElements)
								glDrawElements(GL_TRIANGLES, meshGroups.usedMesh->get_indicies_size(),
											   GL_UNSIGNED_INT, nullptr);
							else
								glDrawArrays(GL_TRIANGLES, 0, meshGroups.usedMesh->get_vertex_size());

						} // mesh components
					} // mesh groups
				} // shader groups
			} // cameras

			  // set these to 0 to prevent some confusion
			glUseProgram(0);
			glBindVertexArray(0);
		}

		void scenegraph3d::__add(mesh_component* mc) {
			if (!mc  || !mc->get_mesh() || !mc->get_mesh()->is_valid()) return;

			// get matching groups
			shader_group* shaderGroup = get_shader_group(mc->get_shader());
			mesh_group* meshGroup = get_mesh_group(shaderGroup, mc->get_mesh());

			// add mesh_component into the mesh_group ordered by material
			for (auto it = meshGroup->dynamicMeshes.begin(); it != meshGroup->dynamicMeshes.end(); it++) {
				// TEMPORARY: check for texture instead of the entire material
				if ((*it)->get_material().diffuseTexture == mc->get_material().diffuseTexture) {
					meshGroup->dynamicMeshes.insert(it, mc);
					return;
				}
			}
			// failed to add, push to end
			meshGroup->dynamicMeshes.push_back(mc);
		}

		void scenegraph3d::__remove(const mesh_component* mc) {
			if (!mc || ~mc->get_mesh() || !mc->get_mesh()->is_valid()) return;

			// get matching groups
			shader_group* shaderGroup = get_shader_group(mc->get_shader());
			mesh_group* meshGroup = get_mesh_group(shaderGroup, mc->get_mesh());

			// find and remove mesh_component
			for (auto it = meshGroup->dynamicMeshes.begin(); it != meshGroup->dynamicMeshes.end(); it++) {
				if ((*it) == mc) {
					meshGroup->dynamicMeshes.erase(it);
					return;
				}
			}
		}

		void scenegraph3d::__add(camera3d* cam) {
			s_cameras.push_back(cam);
		}

		void scenegraph3d::__remove(const camera3d* cam) {
			for (auto it = s_cameras.begin(); it != s_cameras.end(); it++) {
				if ((*it) == cam) {
					s_cameras.erase(it);
					return;
				}
			}
		}

	}
}
