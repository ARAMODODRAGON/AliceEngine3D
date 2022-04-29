#include "sprite_renderer.hpp"

namespace alc {

	sprite_renderer::sprite_renderer()
		: m_enabled(true), m_texture(nullptr), m_layer(0) { }

	sprite_renderer::~sprite_renderer() { }

	bool sprite_renderer::get_enabled() const {
		return m_enabled;
	}

	void sprite_renderer::set_enabled(bool enabled) {
		if (m_enabled == enabled) return;
		//if (enabled) gfx2d::scenegraph2d::__add_sprite(this, m_layer, m_texture);
		//else		 gfx2d::scenegraph2d::__remove_sprite(this, m_layer, m_texture);
	}

	texture_ref sprite_renderer::get_texture() const {
		return m_texture;
	}

	void sprite_renderer::set_texture(texture_ref tex) {
		if (tex == m_texture) return;
		//gfx2d::scenegraph2d::__remove_sprite(this, m_layer, m_texture);
		//gfx2d::scenegraph2d::__add_sprite(this, m_layer, m_texture = tex);
	}

	uint32 sprite_renderer::get_layer() const {
		return m_layer;
	}

	void sprite_renderer::set_layer(uint32 layer) { 
		if (layer == m_layer) return;
		//gfx2d::scenegraph2d::__remove_sprite(this, m_layer, m_texture);
		//gfx2d::scenegraph2d::__add_sprite(this, m_layer = layer, m_texture);
	}


	void sprite_renderer::on_create() {
		// should be disabled
	}

	void sprite_renderer::on_destroy() {
		//if (m_enabled) gfx2d::scenegraph2d::__remove_sprite(this, m_layer, m_texture);
	}


}