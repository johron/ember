#pragma once

struct qt_fixture : public ::testing::Test {
	qt_fixture()
		: m_buffer(new char[255])
		, m_app(m_number, &m_buffer) {}
	~qt_fixture() {
		delete[] m_buffer;
	}

private:
	int m_number;
	char* m_buffer;
	QApplication m_app;
};
