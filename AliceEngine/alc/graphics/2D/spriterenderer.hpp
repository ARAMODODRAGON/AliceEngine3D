#ifndef ALC_GRAPHICS_2D_SPRITERENDERER_HPP
#define ALC_GRAPHICS_2D_SPRITERENDERER_HPP
#include "../../objects/entity.hpp"
#include "scenegraph2d.hpp"

namespace alc {

	// used to render a sprite in the world
	// requires the object or its parent it's attached to to be an entity
	class spriterenderer final : alc::component {
	public:

		spriterenderer();
		~spriterenderer();

		// returns the parent if its an entity
		entity* get_entity() const;

		// returns if this sprite is enabled for rendering
		bool get_enabled() const;

		// sets this sprite to enabled or disabled
		void set_enabled(bool enabled);

		// returns the texture
		texture get_texture() const;

		// sets the texture
		void set_texture(texture tex);

		// gets the layer
		uint32 get_layer() const;

		// sets the layer
		void set_layer(uint32 layer);

	private:
		void on_create() override;
		void on_destroy() override;

		bool m_enabled : 1;
		entity* m_parentEntity;

		texture m_texture;
		uint32 m_layer;
	};

}

#endif // !ALC_GRAPHICS_2D_SPRITERENDERER_HPP