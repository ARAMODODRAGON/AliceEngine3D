#ifndef ALC_CONTENT_SHADER_HPP
#define ALC_CONTENT_SHADER_HPP
#include "../common.hpp"

namespace alc {

	// object representing a shader
	// file uses a single file format where #type *x* indicators are used to identify the types of shaders in a file
	// types include:
	// 'vertex', 'fragment', 'geometry', and 'compute' shader types
	class shader {
	public:

		// loads a shader file
		static shader load(const std::string& filepath);

		// loads a shader from source
		static shader loadsource(const std::string& source);

		// deletes the shader
		static bool unload(shader& shader_);


		// invalid shader
		shader(std::nullptr_t = nullptr);

		// checks if this is a valid shader
		bool is_valid() const;

		// checks if this is a valid shader
		operator bool() const;

		// returns the id
		operator uint32() const;

		// returns the id
		uint32 get_id() const;

		// compares this to another shader
		bool operator==(const shader& other) const;

		// compares this to another shader
		bool operator!=(const shader& other) const;

		// gets the uniform id
		uint32 get_uniform(const std::string& str) const;

		// gets the number of shared instances of this shader
		long get_shared_count() const;

	private:
		struct data_t {
			data_t(uint32 id) : id(id) { }
			~data_t();
			uint32 id;
		};
		std::shared_ptr<data_t> m_data;
	};

}

#endif // !ALC_CONTENT_SHADER_HPP