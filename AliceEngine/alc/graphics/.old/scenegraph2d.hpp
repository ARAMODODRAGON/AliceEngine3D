#ifndef ALC_GRAPHICS_SCENEGRAPH2D_HPP
#define ALC_GRAPHICS_SCENEGRAPH2D_HPP
#include "../../common.hpp"
#include "../../content/content_manager.hpp"

namespace alc {

	namespace graphics {

		// structure to represent an instance of a sprite in the world
		struct sprite_instance final {
			uint32 c_id; // treat as constant
			uint32 layer;
			glm::mat3 transform;
			shader shad;
			texture tex;
		};

		// structure to represent a 2d camera instance in the world
		struct camera2d_instance final {
			uint32 id;
		};
	}

	// class that manages all the 2d rendering of the engine
	// handles both world and UI sprites
	class scenegraph2d final {
		ALC_STATIC_CLASS(scenegraph2d);
	public:


	private:

		struct shadergroup final {
			shader shad;
			std::vector<std::shared_ptr<graphics::sprite_instance>> sprites;
			bool isDirty = true;
		};

		struct layer final {
			std::list<shadergroup> shadergroups;
		};

		static inline std::vector<layer> layers;
		static inline std::vector<std::shared_ptr<graphics::sprite_instance>> sprites;
		static inline std::vector<std::shared_ptr<graphics::camera2d_instance>> cameras;

	public:
		static void __init();
		static void __exit();
	};

}

#endif // !ALC_GRAPHICS_SCENEGRAPH2D_HPP