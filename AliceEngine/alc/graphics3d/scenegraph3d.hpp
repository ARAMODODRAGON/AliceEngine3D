#ifndef ALC_GRAPHICS_3D_SCENEGRAPH3D_HPP
#define ALC_GRAPHICS_3D_SCENEGRAPH3D_HPP
#include "../common.hpp"
#include "../core/engine.hpp"
#include "mesh_component.hpp"

namespace alc {

	class camera3d;

	namespace gfx3d {

		// handles rendering 
		class scenegraph3d final {
			ALC_STATIC_CLASS(scenegraph3d);

			struct mesh_group final {
				mesh_ref usedMesh;
				std::vector<mesh_component*> dynamicMeshes;
				mesh_group(mesh_ref m) : usedMesh(m) { }
			};

			struct shader_group final {
				shader_ref usedShader;
				std::vector<mesh_group> dynamicMeshGroups;
				shader_group(shader_ref shad) : usedShader(shad) { }
			};

			static inline std::vector<camera3d*> s_cameras;
			static inline std::vector<shader_group> s_shaderGroups;
			static inline shader_ref s_defaultShader;

			static shader_group* get_shader_group(shader_ref s);
			static mesh_group* get_mesh_group(shader_group* group, mesh_ref m);
			 
		public:
			static void __init(const engine_settings* set);
			static void __exit();
			static void __draw();
			static void __add(mesh_component* mc);
			static void __remove(const mesh_component* mc);
			static void __add(camera3d* cam);
			static void __remove(const camera3d* cam);
		};

	}

}

#endif // !ALC_GRAPHICS_3D_SCENEGRAPH3D_HPP