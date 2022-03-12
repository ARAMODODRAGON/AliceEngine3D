#ifndef ALC_GRAPHICS_3D_MESH_COMPONENT_HPP
#define ALC_GRAPHICS_3D_MESH_COMPONENT_HPP
#include "../objects/world_object.hpp"
#include "../content/content_manager.hpp"
#include "../datatypes/bounds3d.hpp"

namespace alc {

	class mesh_component : public world_component {
	public:

		mesh_component();
		~mesh_component();
		void on_create() override;

		// checks if this is enabled
		// is disabled by default if there is no attached world_object
		bool get_enabled() const;

		// sets if enabled
		// cannot set if there is no attached world_object
		void set_enabled(bool enabled);

		// checks if this mesh component is static
		bool get_static() const;

		// sets this mesh to be static or not
		// the current transform of the attached object is stored 
		// and calling this function is required to refresh the transform
		void set_static(bool stat);

		// calculates and returns the bounds3d
		//bounds3d get_bounds() const;

		// get the mesh
		mesh get_mesh() const;

		// set the mesh
		void set_mesh(const mesh& m);

		// get the material
		material get_material() const;

		// set the material
		void set_material(const material& m);

		// get the shader
		shader get_shader() const;

		// set the shader
		void set_shader(const shader& s);

		// returns the transformation matrix 
		glm::mat4 get_transform() const;

	private:
		mesh m_mesh;
		material m_material;
		shader m_shader;

		bool m_isStatic : 1;
		bool m_isEnabled : 1;
		glm::mat4 m_lockedTransform;
	};

}

#endif // !ALC_GRAPHICS_3D_MESH_COMPONENT_HPP