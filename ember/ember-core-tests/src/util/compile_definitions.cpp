TEST(compile_definitions, version_major) {
#ifndef EMBER_VERSION_MAJOR
	static_assert(false, "Major version not defined");
#endif
}

TEST(compile_definitions, version_minor) {
#ifndef EMBER_VERSION_MINOR
	static_assert(false, "Minor version not defined");
#endif
}

TEST(compile_definitions, version_patch) {
#ifndef EMBER_VERSION_PATCH
	static_assert(false, "Patch version not defined");
#endif
}