#include "util/settings_storage.h"

using namespace ember::util;
static const QString test_file("test_settings.ini");

TEST(settings_storage, save_to_file) {
	QFile::remove(test_file);
	{
		auto settings = settings_storage(test_file);
		for (int i = 0; i < 10; ++i) {
			settings.set(QString::number(i), i);
		}
	}

	{ 
		auto settings = settings_storage(test_file);
		EXPECT_EQ(settings.size(), 10);
		for (int i = 0; i < 10; ++i) {
			EXPECT_EQ(settings.get<int>(QString::number(i)), i);
		}
	}
}