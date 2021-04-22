#pragma once
#include <mutex>
#include <QFontDatabase>

namespace ember::icon {
struct font_factory {
	template <class font_t>
	static QFont get() {
		static std::once_flag the_flag;
		static QFont the_font;
		std::call_once(the_flag, [&]() {
			const auto result = QFontDatabase::addApplicationFont(font_t::name);
			if (result < 0) {
				throw std::runtime_error("failed to add application font");
			}

			const auto families = QFontDatabase::applicationFontFamilies(result);
			if (families.empty()) {
				throw std::runtime_error("application font contains no families");
			}

			the_font = QFont(families.at(0));
		});

		return the_font;
	}
};
}  // namespace ember::icon
