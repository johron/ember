#pragma once

namespace ember::util {
namespace resource {
inline auto missing_texture() {
	static const auto missing = QPixmap(":/editor/missing.png");
	return missing;

}
}  // namespace resource

inline auto make_pixmap(const QString& path) {
	QPixmap image(path);
	return image.isNull() ? resource::missing_texture() : image;
}

inline auto make_pixmap(const QString& path, QSize size) {
	return make_pixmap(path).scaled(size, Qt::KeepAspectRatio, Qt::SmoothTransformation);
}
}  // namespace ember::util