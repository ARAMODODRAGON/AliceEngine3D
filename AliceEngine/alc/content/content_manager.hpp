#ifndef ALC_CONTENT_CONTENT_MANAGER_HPP
#define ALC_CONTENT_CONTENT_MANAGER_HPP
#include "../common.hpp"
#include "mesh.hpp"
#include "shader.hpp"

namespace alc {

	class texture;

	class content_manager final {
		ALC_STATIC_CLASS(content_manager);
	public:



	private:
		static inline std::unordered_map<std::string, mesh> m_meshes;
	};

}

#endif // !ALC_CONTENT_CONTENT_MANAGER_HPP