#include "util/file_util.h"
#include "editor/editor_main.h"

using namespace ember::util::file::detail;

static const QString test_file = "test_file.txt";
static const auto test_json = R"({
        "FirstName": "John",
        "LastName": "Doe",
        "Age": 43,
        "Address": {
            "Street": "Downing Street 10",
            "City": "London",
            "Country": "Great Britain"
        },
        "Phone numbers": [
            "+44 1234567",
            "+44 2345678"
            ]
    })";


/*
TEST(file_util, write_text_file) {
	QByteArray bytes{"some bytes"};
	const auto result = ember::util::file::write(test_file, bytes);
	EXPECT_TRUE(result);
	QFile::remove(test_file);
}

TEST(file_util, read_text_file) {
	QByteArray bytes{"some bytes"};
	EXPECT_TRUE(ember::util::file::write(test_file, bytes));
	const auto read_data = ember::util::file::read<QByteArray>(test_file);
	EXPECT_EQ(bytes, read_data);
	QFile::remove(test_file);
}

TEST(file_util, write_json) {
	QJsonParseError parse_error;
	const auto original = QJsonDocument::fromJson(test_json, &parse_error);
	EXPECT_EQ(parse_error.error, QJsonParseError::NoError);
	EXPECT_TRUE(file::write(original, test_file, QJsonDocument::Compact));
	QFile::remove(test_file);
}

TEST(file_util, read_json) {
	QJsonParseError parse_error;
	const auto original = QJsonDocument::fromJson(test_json, &parse_error);
	EXPECT_EQ(parse_error.error, QJsonParseError::NoError);
	EXPECT_TRUE(file::write(original, test_file, QJsonDocument::Indented));
	const auto read_data = file::read<QJsonDocument>(test_file);
	EXPECT_EQ(original, read_data);
}
*/