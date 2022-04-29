#ifndef ALC_GRAPHICS_2D_SPRITERENDERER_HPP
#define ALC_GRAPHICS_2D_SPRITERENDERER_HPP
#include "../objects/object3d.hpp"
#include "scenegraph2d.hpp"

namespace alc {

	// used to render a sprite in the world
	// requires the object or its parent it's attached to to be an world_object
	class sprite_renderer final : public object3d {
	public:

		sprite_renderer();
		~sprite_renderer();

		// returns if this sprite is enabled for rendering
		bool get_enabled() const;

		// sets this sprite to enabled or disabled
		void set_enabled(bool enabled);

		// returns the texture
		texture_ref get_texture() const;

		// sets the texture
		void set_texture(texture_ref tex);

		// gets the layer
		uint32 get_layer() const;

		// sets the layer
		void set_layer(uint32 layer);

	private:
		void on_create() override;
		void on_destroy() override;

		bool m_enabled : 1;

		texture_ref m_texture;
		uint32 m_layer;
	};

}

#endif // !ALC_GRAPHICS_2D_SPRITERENDERER_HPP