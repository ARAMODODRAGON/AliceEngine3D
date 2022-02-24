#ifndef ALC_GRAPHICS_3D_SCENEGRAPH3D_HPP
#define ALC_GRAPHICS_3D_SCENEGRAPH3D_HPP
#include "../../common.hpp"
#include "../../datatypes/bounds3d.hpp"

namespace alc {

	namespace gfx3d {

		// determines how a model will render
		enum class rendermode3d : uint8 {
			// renders individually from other instances of the same mesh
			single,

			// rendering is grouped with other matching meshes given they also use this setting
			grouped,

			// renders individually from other instances of the same mesh
			// renders using depth order and transparency enabled 
			transparent
		};

		//struct renderer3d_groupedinstances;

		// an instance of an object in the scene
		struct renderer3d_singleinstance final {
			uint32 id = -1;
			rendermode3d rendermode = rendermode3d::single;

			//renderer3d_groupedinstances* group = nullptr;
		};

		// an instance of a grouped renderer
		//struct renderer3d_groupedinstances final {
		//	uint32 id = -1;
		//	std::vector<std::shared_ptr<renderer3d_singleinstance>> instances;
		//};

		// handles rendering 
		class scenegraph3d final {
			ALC_STATIC_CLASS(scenegraph3d);
		public:




		private:

			struct oct_node final {
				std::vector<std::unique_ptr<oct_node>> subnodes;
				bounds3d bounds;

				std::vector<std::shared_ptr<renderer3d_singleinstance>> singleInstances;
				//std::vector<std::shared_ptr<renderer3d_singleinstance>> transparentInstances;
				//std::vector<std::shared_ptr<renderer3d_groupedinstances>> groupedInstances;
			};

			static inline std::vector<std::unique_ptr<oct_node>> s_nodes;
		public:
			static void __init();
			static void __exit();
		};

	}

}

#endif // !ALC_GRAPHICS_3D_SCENEGRAPH3D_HPP