#ifndef ALC_CONTENT_MATERIAL_HPP
#define ALC_CONTENT_MATERIAL_HPP
#include "../common.hpp"
#include "texture.hpp"

namespace alc {

	// struct representing a material
	struct material final {
		texture diffuseTexture;


		float shinyness = 0.0f;
		float transparency = 0.0f;
		glm::vec3 ambient = glm::vec3(1.0f);
		glm::vec3 diffuse = glm::vec3(1.0f);
		glm::vec3 specular = glm::vec3(0.0f);
	};

}

#endif // !ALC_CONTENT_MATERIAL_HPP