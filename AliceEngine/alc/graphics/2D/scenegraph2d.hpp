#ifndef ALC_GRAPHICS_2D_SCENEGRAPH2D_HPP
#define ALC_GRAPHICS_2D_SCENEGRAPH2D_HPP
#include "../../common.hpp"
#include "../../content/content_manager.hpp"

namespace alc {

	class spriterenderer;
	class camera2d;

	namespace gfx2d {


		// renders all 2d objects in the scene
		class scenegraph2d final {
			ALC_STATIC_CLASS(scenegraph2d);
		public:


		private:

			struct layer final {
				std::list<spriterenderer> sprites;
				shader sh;
				// TODO: add renderer2ds as a seperate thing to render in this layer
			};

			static inline std::vector<layer> s_layers;
			static inline std::vector<camera2d*> s_cameras;

			//static inline mesh s_batchMesh;

		public:
			static void __init(struct engine_settings* set);
			static void __exit();
			static void __add_camera(camera2d* cam);
			static void __remove_camera(const camera2d* cam);
			static void __add_sprite(spriterenderer* spr, uint32 layer, texture tex);
			static void __remove_sprite(const spriterenderer* spr, uint32 layer, texture tex);
		};

	}

}

#endif // !ALC_GRAPHICS_2D_SCENEGRAPH2D_HPP