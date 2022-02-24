#include "spriterenderer.hpp"

namespace alc {

	spriterenderer::spriterenderer()
		: m_enabled(true), m_parentEntity(nullptr), m_texture(nullptr), m_layer(0) { }

	spriterenderer::~spriterenderer() { }

	entity* spriterenderer::get_entity() const {
		return m_parentEntity;
	}

	bool spriterenderer::get_enabled() const {
		return m_enabled;
	}

	void spriterenderer::set_enabled(bool enabled) {
		if (m_enabled == enabled) return;
		if (enabled) gfx2d::scenegraph2d::__add_sprite(this, m_layer, m_texture);
		else		 gfx2d::scenegraph2d::__remove_sprite(this, m_layer, m_texture);
	}

	texture spriterenderer::get_texture() const {
		return m_texture;
	}

	void spriterenderer::set_texture(texture tex) {
		if (tex == m_texture) return;
		gfx2d::scenegraph2d::__remove_sprite(this, m_layer, m_texture);
		gfx2d::scenegraph2d::__add_sprite(this, m_layer, m_texture = tex);
	}

	uint32 spriterenderer::get_layer() const {
		return m_layer;
	}

	void spriterenderer::set_layer(uint32 layer) { 
		if (layer == m_layer) return;
		gfx2d::scenegraph2d::__remove_sprite(this, m_layer, m_texture);
		gfx2d::scenegraph2d::__add_sprite(this, m_layer = layer, m_texture);
	}


	void spriterenderer::on_create() {
		// enabled and running
		if (entity* e = dynamic_cast<entity*>(get_parent())) {
			m_parentEntity = e;
			gfx2d::scenegraph2d::__add_sprite(this, m_layer, m_texture);
		}
		// should be disabled
		else m_enabled = false;
	}

	void spriterenderer::on_destroy() {
		if (m_enabled) gfx2d::scenegraph2d::__remove_sprite(this, m_layer, m_texture);
	}


}