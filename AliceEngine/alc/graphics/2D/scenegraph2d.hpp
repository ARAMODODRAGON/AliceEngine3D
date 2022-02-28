#ifndef ALC_GRAPHICS_2D_SCENEGRAPH2D_HPP
#define ALC_GRAPHICS_2D_SCENEGRAPH2D_HPP
#include "../../common.hpp"
#include "../../content/content_manager.hpp"

namespace alc {

	class spriterenderer;
	class camera2d;
	struct engine_settings;

	namespace gfx2d {

		// info for each layer of the scenegraph
		struct layerinfo {
			std::string name;
			std::string spriteShader;
			//std::string postProcessingShader;
		};

		// renders all 2d objects in the scene
		class scenegraph2d final {
			ALC_STATIC_CLASS(scenegraph2d);
		public:


		private:

			struct layer final {
				std::string name;
				std::list<spriterenderer*> sprites;
				shader spriteShader;
				// TODO: add renderer2ds as a seperate thing to render in this layer
			};

			static inline std::vector<layer> s_layers;
			static inline std::list<camera2d*> s_cameras;

			static inline const engine_settings* s_set = nullptr;
			static inline shader s_spriteShader;
			//static inline mesh s_batchMesh;

		public:
			static void __init(const engine_settings* set);
			static void __exit();
			static void __draw();
			static void __add_camera(camera2d* cam);
			static void __remove_camera(const camera2d* cam);
			static bool __add_sprite(spriterenderer* spr, uint32 layerID, texture tex);
			static void __remove_sprite(const spriterenderer* spr, uint32 layerID, texture tex);
		};

	}

	inline gfx2d::layerinfo bind_layer(const std::string& name, const std::string& spriteShader = "") {
		gfx2d::layerinfo info;
		info.name = name;
		info.spriteShader = spriteShader;
		return info;
	}

}

#endif // !ALC_GRAPHICS_2D_SCENEGRAPH2D_HPP