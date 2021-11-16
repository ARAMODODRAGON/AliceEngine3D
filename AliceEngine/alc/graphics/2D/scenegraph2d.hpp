#ifndef ALC_GRAPHICS_2D_SCENEGRAPH2D_HPP
#define ALC_GRAPHICS_2D_SCENEGRAPH2D_HPP
#include "../../content/shader.hpp"
#include "../../content/texture.hpp"
#include "../../objects/singleton.hpp"

namespace alc {

	class spriterenderer;
	class camera2d;

	class scenegraph2d final : public singleton {
		ALC_SINGLETON_GETTER(scenegraph2d);
	public:

		scenegraph2d();
		~scenegraph2d();

		// returns a copy of the default shader
		shader default_shader() const;

		// gets the number of layers 
		size_t layer_size() const;

		// gets the layer shader at index
		shader get_layer_shader(size_t index);

		// sets the layer shader at index
		void set_layer_shader(size_t index, shader newshader);

		// adds a new layer as the last index
		// returns the index
		size_t add_new_layer();

		// removes the specified layer 
		// removes last index if not specified
		// cannot remove layer if there is only one
		bool remove_layer(size_t index = -1);

		// gets the number of camera2d's
		size_t camera2d_size() const;

		// gets the camera2d at index
		camera2d* get_camera2d(size_t index);

		// gets the number of spriterenderer's
		size_t spriterenderer_size() const;

		// returns the spriterenderer at index
		spriterenderer* get_spriterenderer(size_t index);

	private:

		struct layer {
			std::vector<spriterenderer*> sprites;
			shader shad;
		};

		std::vector<layer> m_layers;

		std::vector<spriterenderer*> m_sprites;
		std::vector<camera2d*> m_cameras;

	public:
		void __add_spriterenderer(spriterenderer* spr);
		void __remove_spriterenderer(spriterenderer* spr);
		void __set_spritelayer(spriterenderer* spr, size_t oldLayer, size_t newLayer);
		void __add_camera(camera2d* cam);
		void __remove_camera(camera2d* cam);
	};

}

#endif // !ALC_GRAPHICS_2D_SCENEGRAPH2D_HPP