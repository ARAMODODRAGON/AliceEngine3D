#ifndef ALC_GRAPHICS_2D_SCENEGRAPH2D_HPP
#define ALC_GRAPHICS_2D_SCENEGRAPH2D_HPP
#include "../../content/shader.hpp"
#include "../../content/texture.hpp"
#include "../../objects/singleton.hpp"
#include "../../objects/entity.hpp"

namespace alc {

	class scenegraph2d final : public singleton {
		ALC_STATIC_CLASS(scenegraph2d);
	public:

		// layer handling

		// add layer and return its index
		// if no set index then it will be inserted at the end
		// layers with larger index will appear in front of other layers
		// returns set index of layer
		static uint32 add_layer(uint32 setindex = -1);

		// removes a layer
		static bool remove_layer(uint32 index);

		// get layer shader
		static shader get_layer_shader(uint32 index);

		// set layer shader
		static bool set_layer_shader(uint32 index, shader sha);


		// camera handling

		// creates a camera and returns that camera id
		// requires the associated matricies to render the scene
		static uint32 add_camera(glm::mat4* transform, glm::mat4* view);

		// removes a camera
		static bool remove_camera(uint32 id);


		// sprite handling

		// creates a sprite on the set layer with the associated entity
		// will insert to first layer if set layer doesnt exist
		static uint32 create_sprite(entity* obj, uint32 layer = 0);

		// removes a sprite
		static bool remove_sprite(uint32 sprite);

		// gets a sprites texture
		static texture get_sprite_texture(uint32 sprite);

		// sets a sprites texture
		static bool set_sprite_texture(uint32 sprite, texture tex);

		// gets sprite layer
		static uint32 get_sprite_layer(uint32 sprite);

		// sets sprite layer
		static bool set_sprite_layer(uint32 sprite, uint32 layer);


	private:

		struct sprite_inst final {
			uint32 id;
			uint32 layerIndex;
			texture tex;
			entity* obj;
		};

		struct camera_inst final {
			uint32 id;
			glm::mat4* transform;
			glm::mat4* view;
		};

		struct layer_inst final {
			std::vector<uint32> sprites;
			shader sha;
		};

		static inline std::vector<layer_inst> s_layers;
		static inline std::unordered_map<uint32, camera_inst> s_cameras;
		static inline std::unordered_map<uint32, sprite_inst> s_sprites;

	public:
		static void __init(std::vector<std::string>& layershaders);
		static void __exit();
		static void __render();
	};

}

#endif // !ALC_GRAPHICS_2D_SCENEGRAPH2D_HPP