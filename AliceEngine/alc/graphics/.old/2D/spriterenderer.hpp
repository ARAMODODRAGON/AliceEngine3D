#ifndef ALC_GRAPHICS_2D_SPRITERENDERER_HPP
#define ALC_GRAPHICS_2D_SPRITERENDERER_HPP
#include "../../objects/entity.hpp"
#include "../../content/content_manager.hpp"

namespace alc {

	class spriterenderer final : component {
	public:

		spriterenderer();
		~spriterenderer();

		// returns the first parent that is an entity
		entity* get_parent_entity();

		// gets if enabled
		// will always be disabled if no parent object is an entity
		bool get_enabled() const;

		// sets if enabled
		// will not work if no parent object is an entity
		void set_enabled(bool enabled);

		// gets the texture
		texture get_texture() const;

		// sets the texture
		void set_texture(texture tex);

		// gets the layer index
		uint32 get_layer() const;

		// sets the layer index
		void set_layer(uint32 index);

	private:
		uint32 m_spriteID;
		entity* m_parentEntity;
		void on_create() override;
		void on_destroy() override;
	};

}

#endif // !ALC_GRAPHICS_2D_SPRITERENDERER_HPP