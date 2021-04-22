#pragma once
#include <QByteArray>
#include <QFile>
#include <QIODevice>
#include <QJsonDocument>

namespace ember::util::file::detail {
template <class data_t>
struct file {};

template <>
struct file<QByteArray> {
	static bool write(const QByteArray& data, QString filename) {
		QFile file(filename);
		if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
			throw std::runtime_error("unable to open file: " + filename.toStdString());
		}
		if (file.write(data) != data.size()) {
			throw std::runtime_error("unable to write all data to file: " + filename.toStdString());
			return false;
		}
		return true;
	}

	static QByteArray read(QString filename) {
		QFile file(filename);
		if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
			return file.readAll();
		} else {
			throw std::runtime_error("unable to open file: " + filename.toStdString());
		}
	}
};

template <>
struct file<QJsonDocument> {
	static bool write(const QJsonDocument& data, QString filename) {
		return write(data, filename, QJsonDocument::Indented);
	}
	static bool write(const QJsonDocument& data, QString filename, QJsonDocument::JsonFormat format) {
		return file<QByteArray>::write(QJsonDocument(data).toJson(format), filename);
	}
	static QJsonDocument read(QString filename) {
		QJsonParseError parse_result;
		const auto document = QJsonDocument::fromJson(file<QByteArray>::read(filename), &parse_result);
		if (parse_result.error != QJsonParseError::NoError) {
			throw std::runtime_error(parse_result.errorString().toStdString());
		}
		return document;
	}
};
}  // namespace ember::util::file::detail

namespace ember::util::file {
template <class data_t, class... arg_t>
bool write(data_t&& data, QString filename, arg_t&&... args) {
	return detail::file<data_t>::write(data, filename, std::forward<arg_t>(args)...);
}

template <class data_t>
data_t read(QString filename) {
	return detail::file<data_t>::read(filename);
}
}  // namespace ember::util::file