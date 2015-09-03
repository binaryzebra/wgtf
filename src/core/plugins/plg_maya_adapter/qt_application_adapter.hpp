#ifndef QT_APPLICATION_ADAPTER_HPP
#define QT_APPLICATION_ADAPTER_HPP

#include "core_dependency_system/i_interface.hpp"

#include "qt_application.hpp"
#include "i_application_adapter.hpp"
#include "i_application_listener.hpp"
#include <QtCore/QObject>

class QtApplicationAdapter : public QObject, public QtApplication, public Implements< IApplicationAdapter >
{
	Q_OBJECT
public:
	QtApplicationAdapter();
	virtual ~QtApplicationAdapter();

	int startApplication() override;
	void addListener( IApplicationListener * listener ) override;
	void removeListener( IApplicationListener * listener ) override;
public slots:
	void applicationStateChanged( Qt::ApplicationState state );
	void applicationStopped();
private:
	std::vector< IApplicationListener * >	listeners_;
};

#endif//QT_APPLICATION_ADAPTER_HPP