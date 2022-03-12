#include "scenegraph2d.hpp"
#include "detail\basic_sprite_shader.hpp"
#include "spriterenderer.hpp"
#include "../core/engine.hpp"

namespace alc {
	namespace gfx2d {

		void scenegraph2d::__init(const engine_settings* set) {
			s_set = set;

			// load basic shader
			s_spriteShader = content_manager::load_shader_source(c_basicSpriteShader);

			// create layers
			s_layers.resize(set->renderer2d.layerInfo.size() ? set->renderer2d.layerInfo.size() : 1);

			for (size_t i = 0; i < set->renderer2d.layerInfo.size(); i++) {
				// set layer name
				s_layers[i].name = set->renderer2d.layerInfo[i].name;

				// possibly load shader
				if (set->renderer2d.layerInfo[i].spriteShader != "") {
					s_layers[i].spriteShader =
						content_manager::load_shader(set->renderer2d.layerInfo[i].spriteShader);

					if (!s_layers[i].spriteShader) s_layers[i].spriteShader = s_spriteShader;
				}
			}

			// TODO: load mesh to batch onto

		}

		void scenegraph2d::__exit() {

		}

		void scenegraph2d::__draw() {
			// TODO
		}

		void scenegraph2d::__add_camera(camera2d* cam) {
			s_cameras.push_back(cam);
		}

		void scenegraph2d::__remove_camera(const camera2d* cam) {
			s_cameras.remove_if([cam](auto other) { return other == cam; });
		}

		bool scenegraph2d::__add_sprite(spriterenderer* spr, uint32 layerID, texture tex) {
			if (spr == nullptr) return false;
			if (layerID >= s_layers.size()) {
				ALC_DEBUG_WARNING("Invalid layer ID: " + VTOS(layerID));
				return false;
			}
			// get layer
			auto& layer = s_layers[layerID];

			// add sprite into layer based on the order of the texture
			for (auto it = layer.sprites.begin(); it != layer.sprites.end(); it++) {
				// check if should insert next to other sprites with the same texture
				if ((*it)->get_texture().get_id() == tex.get_id()) {
					layer.sprites.insert(it, spr);
					// exit
					return true;
				}
			}

			// insert at end
			layer.sprites.push_back(spr);
			return true;
		}

		void scenegraph2d::__remove_sprite(const spriterenderer* spr, uint32 layerID, texture tex) {
			// get layer
			auto& layer = s_layers[layerID];
			// remove
			layer.sprites.remove_if([spr](auto other) { return spr == other; });
		}

	}
}

