#pragma once

namespace ember::util {
struct dynamic_base : public QObject {
	Q_OBJECT
public:
	virtual ~dynamic_base() = default;
signals:
	void changed();
};
}