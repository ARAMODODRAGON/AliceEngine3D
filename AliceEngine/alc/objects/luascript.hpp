#ifndef ALC_OBJECT_LUASCRIPT_HPP
#define ALC_OBJECT_LUASCRIPT_HPP
#include "object.hpp"

namespace alc {

	class luascript final : public iscript {
	public:

		luascript();
		~luascript();

		bool load(const std::string& filepath);
		void message(object* self, const std::string& function) override;

	private:

	};

}

#endif // !ALC_OBJECT_LUASCRIPT_HPP