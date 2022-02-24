#ifndef ALC_CONTENT_MODEL_HPP
#define ALC_CONTENT_MODEL_HPP
#include "mesh.hpp"
#include "texture.hpp"
#include "shader.hpp"
#include "material.hpp"

namespace alc {

	// represents a model consiting of one or more meshes with set shaders and textures
	class model final {
	public:

		// loads a model from an obj file
		static model load(std::string file);

		// unloads a model
		static bool unload(model& m);


		model() = default;

		// gets the number of shared instances of this mesh
		long get_shared_count() const;

	private:
		struct submesh final {
			mesh m;
			material mat;
			submesh() = default;
		};
		struct data_t final {
			std::vector<submesh> meshes;
			//shader sha;
		};
		std::shared_ptr<data_t> m_data;
	};

}

#endif // !ALC_CONTENT_MODEL_HPP