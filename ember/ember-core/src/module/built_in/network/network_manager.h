#pragma once
#include <QNetworkAccessManager>

namespace ember::network {
	struct manager : public module::mandatory {
		QNetworkAccessManager m_access_manager;
	};
}