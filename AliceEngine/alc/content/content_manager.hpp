#ifndef ALC_CONTENT_CONTENT_MANAGER_HPP
#define ALC_CONTENT_CONTENT_MANAGER_HPP
#include "../common.hpp"
#include "shader.hpp"
#include "texture.hpp"
#include "mesh.hpp"
#include "../datatypes/timestep.hpp"
#include "../reflection/typehash.hpp"
#include "../core/debug.hpp"

namespace alc {

	namespace detail {
		enum class ignore_manager { };
	}

	// allows any type of content to be referenced 
	// and is automatically managed by the content_manager
	template<typename Ty>
	class content_ref final {
	public:

		// default constructor
		content_ref(std::nullptr_t = nullptr);

		// constructs with a new ptr
		explicit content_ref(Ty* ptr, detail::ignore_manager i = static_cast<detail::ignore_manager>(0));

		~content_ref();

		// copy & move
		content_ref(const content_ref& other);
		content_ref& operator=(const content_ref& other);
		content_ref(content_ref&& other);
		content_ref& operator=(content_ref&& other);

		// swaps this with another
		void swap(content_ref& x);

		// removes the internal reference to the ptr
		void reset();

		// removes and replaces the internal reference to the ptr
		void reset(Ty* ptr);

		// forcebly deletes the ptr and alerts all 
		// associated references to no longer reference the ptr
		void force_delete();

		// returns the ptr stored by this 
		Ty* get() const;

		// returns the equivalent of *get()
		Ty& operator*() const;

		// access to the internal ptr
		Ty* operator->() const;

		// compares the internal ptr 
		bool operator==(const content_ref& other) const;

		// compares the internal ptr 
		bool operator!=(const content_ref& other) const;

		// checks if this is valid
		operator bool() const;

		// returns the number of references to this 
		size_t use_count() const;

	private:
		struct data_t final {
			Ty* ptr;
			size_t refCount;
			bool ignore;
		};
		data_t* m_data;
	public:
		void __force_delete();
	};

	using texture_ref = content_ref<texture>;
	using shader_ref = content_ref<shader>;
	using mesh_ref = content_ref<mesh>;


	// struct representing a material
	struct material final {
		texture_ref diffuseTexture;


		float shinyness = 0.0f;
		float transparency = 0.0f;
		glm::vec3 ambient = glm::vec3(1.0f);
		glm::vec3 diffuse = glm::vec3(1.0f);
		glm::vec3 specular = glm::vec3(0.0f);
	};

	// loads and manages all content in the game
	// generally preferred to use if you have content used across multiple objects
	class content_manager final {
		ALC_STATIC_CLASS(content_manager);
	public:

		// returns the full path of the given content filepath
		static std::string get_full_path(const std::string& contentPath);

		// creates content of type and manages that content
		template<typename Ty, typename... Args>
		static content_ref<Ty> create(const std::string& identifier, Args&&... args);

		// gets content of type if it is managed by the content manager
		template<typename Ty>
		static content_ref<Ty> get(const std::string& identifier);

	private:
		struct imanaged_content {
			virtual ~imanaged_content() = 0 { }
			virtual void remove_unreferenced() = 0;
		};
		template<typename ContentTy>
		struct managed_content final : imanaged_content {
			using refTy = content_ref<ContentTy>;
			managed_content();
			~managed_content();
			void remove_unreferenced() override;
			std::unordered_map<std::string, refTy> map;
			std::list<refTy> list;
		};
		static inline std::unordered_map<typehash, std::unique_ptr<imanaged_content>> s_content;
		static inline int32 s_removalRate;
		static inline int32 s_framecounter;
		static inline std::string s_resourcePath;
		template<typename Ty> static managed_content<Ty>* __getcontent();
	public:
		static void __init(uint32 rate, const std::string& resourcePath);
		static void __exit();
		static void __step();
		template<typename Ty>
		static void __add(content_ref<Ty>& ref);
		template<typename Ty>
		static void __remove(content_ref<Ty>& ref);
	};

	// template implementations

	template<typename Ty>
	inline content_ref<Ty>::content_ref(std::nullptr_t)
		: m_data(nullptr) { }

	template<typename Ty>
	inline content_ref<Ty>::content_ref(Ty* ptr, detail::ignore_manager i) {
		m_data = new data_t();
		m_data->ptr = ptr;
		m_data->refCount = 1;
		m_data->ignore = i == static_cast<detail::ignore_manager>(1);
		if (!m_data->ignore) content_manager::__add(*this);
	}

	template<typename Ty>
	inline content_ref<Ty>::~content_ref() {
		if (m_data) {
			m_data->refCount--;
			if (m_data->refCount == 0) {
				if (m_data->ptr) delete m_data->ptr;
				delete m_data;
				if (!m_data->ignore) content_manager::__remove(*this);
			}
			m_data = nullptr;
		}
	}


	template<typename Ty>
	inline content_ref<Ty>::content_ref(const content_ref& other) : content_ref<Ty>() {
		if (other.m_data && other.m_data->ptr) {
			m_data = other.m_data;
			m_data->refCount++;
		}
	}

	template<typename Ty>
	inline content_ref<Ty>& content_ref<Ty>::operator=(const content_ref& other) {
		content_ref<Ty>(other).swap(*this);
		return *this;
	}

	template<typename Ty>
	inline content_ref<Ty>::content_ref(content_ref&& other) : content_ref<Ty>() {
		other.swap(*this);
	}

	template<typename Ty>
	inline content_ref<Ty>& content_ref<Ty>::operator=(content_ref&& other) {
		other.swap(*this);
		return *this;
	}


	template<typename Ty>
	inline void content_ref<Ty>::swap(content_ref& other) {
		data_t* data = m_data;
		m_data = other.m_data;
		other.m_data = data;
	}

	template<typename Ty>
	inline void content_ref<Ty>::reset() {
		content_ref<Ty>().swap(*this);
	}

	template<typename Ty>
	inline void content_ref<Ty>::reset(Ty* ptr) {
		content_ref<Ty>(ptr).swap(*this);
	}

	template<typename Ty>
	inline void content_ref<Ty>::force_delete() {
		__force_delete();
	}

	template<typename Ty>
	inline Ty* content_ref<Ty>::get() const {
		if (m_data) return m_data->ptr;
		return nullptr;
	}

	template<typename Ty>
	inline Ty& content_ref<Ty>::operator*() const {
		return *get();
	}

	template<typename Ty>
	inline Ty* content_ref<Ty>::operator->() const {
		return get();
	}

	template<typename Ty>
	inline bool content_ref<Ty>::operator==(const content_ref& other) const {
		return m_data == other.m_data;
	}

	template<typename Ty>
	inline bool content_ref<Ty>::operator!=(const content_ref& other) const {
		return m_data != other.m_data;
	}

	template<typename Ty>
	inline content_ref<Ty>::operator bool() const {
		return m_data;
	}

	template<typename Ty>
	inline size_t content_ref<Ty>::use_count() const {
		if (m_data) return m_data->refCount;
		return 0;
	}

	template<typename Ty>
	inline void content_ref<Ty>::__force_delete() {
		if (m_data && m_data->ptr) {
			// specifically delete the ptr data instead of deleting everything altogether
			delete m_data->ptr; m_data->ptr = nullptr;
			m_data->ignore = true;
		}
		if (m_data && !m_data->ignore) content_manager::__remove(*this);
		content_ref<Ty>().swap(*this);
	}

	template<typename Ty>
	inline content_manager::managed_content<Ty>* content_manager::__getcontent() {
		if (auto it = s_content.find(get_typehash<Ty>()); it != s_content.end())
			return reinterpret_cast<managed_content<Ty>*>(it->second.get());
		auto* manager = new managed_content<Ty>();
		s_content.emplace(get_typehash<Ty>(), manager);
		return manager;
	}

	template<typename Ty, typename ...Args>
	inline content_ref<Ty> content_manager::create(const std::string& identifier, Args && ...args) {
		auto* manager = __getcontent<Ty>();
		if (auto it = manager->map.find(identifier); it != manager->map.end()) {
			ALC_DEBUG_ERROR("Could not create content with preexisting identifier " + identifier);
			return content_ref<Ty>();
		}
		content_ref<Ty> content(new Ty(std::forward(args)...), static_cast<detail::ignore_manager>(1));
		manager->map.emplace(identifier, content);
		return content;
	}

	template<typename Ty>
	inline content_ref<Ty> content_manager::get(const std::string& identifier) {
		auto* manager = __getcontent<Ty>();
		if (auto it = manager->map.find(identifier); it != manager->map.end())
			return it->second;
		return content_ref<Ty>();
	}

	template<typename Ty>
	inline void content_manager::__add(content_ref<Ty>& ref) {
		auto* manager = __getcontent<Ty>();
		manager->list.push_back(ref);
	}

	template<typename Ty>
	inline void content_manager::__remove(content_ref<Ty>& ref) {
		auto* manager = __getcontent<Ty>();
		for (auto it = manager->list.begin(); it != manager->list.end(); it++) {
			if (*it == ref) {
				manager->list.erase(it);
				return;
			}
		}
	}


	template<typename ContentTy>
	inline content_manager::managed_content<ContentTy>::managed_content() { }

	template<typename ContentTy>
	inline content_manager::managed_content<ContentTy>::~managed_content() {
		for (auto [str, con] : map) {
			con.__force_delete();
		}
		map.clear();
		for (auto con : list) {
			con.__force_delete();
		}
		list.clear();
	}

	template<typename ContentTy>
	inline void content_manager::managed_content<ContentTy>::remove_unreferenced() {
		for (auto it = map.begin(); it != map.end(); it++) {
			if (it->second.use_count() == 1) {
				it->second.reset();
				it = map.erase(it);
			}
		}
		if (list.size()) {
			for (auto it = list.begin(); it != list.end(); it++) {
				if (it->use_count() == 1) {
					it->reset();
					it = list.erase(it);
				}
			}
		}
	}

}

#endif // !ALC_CONTENT_CONTENT_MANAGER_HPP