#ifndef ALC_CONTENT_MESH_HPP
#define ALC_CONTENT_MESH_HPP

namespace alc {

	class mesh final {
	public:
		class submesh final {
			friend class mesh;
		public:

			submesh();

		private:
			uint32 m_vao, m_vbo, m_ebo;
		public:
			void __bind();
		};

		// loads mesh from a file
		static mesh load(const std::string& filepath);

		// deletes the mesh
		static bool unload(mesh& m);


		mesh(std::nullptr_t = nullptr);

		// checks if this is a valid mesh
		bool is_valid() const;

		// checks if this is a valid mesh
		operator bool() const;

		// compares this to another mesh
		bool operator==(const mesh& other) const;

		// compares this to another mesh
		bool operator!=(const mesh& other) const;

	private:
		struct mesh_data final {
			std::vector<submesh> m_submeshes;
		};
		std::shared_ptr<mesh_data> m_meshData;
	};

}

#endif // !ALC_CONTENT_MESH_HPP