#pragma once
#include <QVersionNumber>

namespace ember::build {
namespace version {
constexpr uint64_t major = EMBER_VERSION_MAJOR;
constexpr uint64_t minor = EMBER_VERSION_MINOR;
constexpr uint64_t patch = EMBER_VERSION_PATCH;

inline QVersionNumber number() {
	return {major, minor, patch};
}
}  // namespace version

enum class target { internal_release, external_release, development, none };

constexpr inline target get_target() {
#ifdef EMBER_BUILD_TYPE_EXTERNAL_RELEASE
	return target::external_release;
#elif defined EMBER_BUILD_TYPE_INTERNAL_RELEASE
	return target::internal_release;
#elif defined EMBER_BUILD_TYPE_DEVELOPMENT
	return target::development;
#else
	static_assert(false, "No build target defined");
#endif
}
}  // namespace ember::build