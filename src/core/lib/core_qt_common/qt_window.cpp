#include "qt_window.hpp"

#include "core_ui_framework/i_action.hpp"
#include "core_ui_framework/i_view.hpp"
#include "i_qt_framework.hpp"
#include "qt_context_menu.hpp"
#include "qt_dock_region.hpp"
#include "qt_menu_bar.hpp"
#include "qt_tab_region.hpp"
#include "qt_tool_bar.hpp"

#include <QApplication>
#include <QDockWidget>
#include <QElapsedTimer>
#include <QEvent>
#include <QMainWindow>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QTabWidget>
#include <QToolBar>
#include <QUiLoader>
#include <QWindow>
#include <cassert>
#include <chrono>
#include <thread>

namespace
{
	template< typename T >
	std::vector< T * > getChildren( const QObject & parent )
	{
		std::vector< T * > children;
		foreach ( auto child, parent.children() )
		{
			T * childT = qobject_cast< T * >( child );
			if (childT != nullptr)
			{
				children.push_back( childT );
			}
			auto grandChildren = getChildren< T >( *child );
			children.insert( 
				children.end(), grandChildren.begin(), grandChildren.end() );
		}
		return children;
	}
}

QtWindow::QtWindow( IQtFramework & qtFramework, QIODevice & source )
	: qtFramework_( qtFramework )
	, mainWindow_( nullptr )
{
	QUiLoader loader;

	// Load optional plugins that may have custom widgets
	auto& pluginPath = qtFramework.getPluginPath();
	if( !pluginPath.empty() )
		loader.addPluginPath( pluginPath.c_str() );
	
	auto qMainWindow = qobject_cast< QMainWindow * >( loader.load( &source ) );

	if (qMainWindow == nullptr)
	{
		return;
	}

	mainWindow_.reset( qMainWindow );

	auto idProperty = mainWindow_->property( "id" );
	if (idProperty.isValid())
	{
		id_ = idProperty.toString().toUtf8().operator const char *();
	}

	auto menuBars = getChildren< QMenuBar >( *mainWindow_ );
	for (auto & menuBar : menuBars)
	{
		if (menuBar->property( "path" ).isValid())
		{
			menus_.emplace_back( new QtMenuBar( *menuBar ) );
		}
	}

	auto toolBars = getChildren< QToolBar >( *mainWindow_ );
	for (auto & toolBar : toolBars)
	{
		if (toolBar->property( "path" ).isValid())
		{
			menus_.emplace_back( new QtToolBar( *toolBar ) );
		}
	}

	auto contextMenus = getChildren< QMenu >( *mainWindow_ );
	for (auto & contextMenu : contextMenus)
	{
		if (contextMenu->property( "path" ).isValid())
		{
			menus_.emplace_back( new QtContextMenu( *contextMenu ) );
		}
	}

	auto dockWidgets = getChildren< QDockWidget >( *mainWindow_ );
	for (auto & dockWidget : dockWidgets)
	{
		if ( dockWidget->property( "layoutTags" ).isValid() )
		{
			regions_.emplace_back( new QtDockRegion( qtFramework_, *mainWindow_, *dockWidget ) );
		}
	}

	auto tabWidgets = getChildren< QTabWidget >( *mainWindow_ );
	for (auto & tabWidget : tabWidgets)
	{
		if ( tabWidget->property( "layoutTags" ).isValid() )
		{
			regions_.emplace_back( new QtTabRegion( qtFramework_, *tabWidget ) );
		}
	}
	modalityFlag_ = mainWindow_->windowModality();
	mainWindow_->installEventFilter( this );
}

QtWindow::~QtWindow()
{
	mainWindow_ = nullptr;
}

const char * QtWindow::id() const
{
	return id_.c_str();
}

const char * QtWindow::title() const
{
	if (mainWindow_.get() == nullptr)
	{
		return "";
	}

	return mainWindow_->windowTitle().toUtf8().constData();
}

void QtWindow::update()
{
	for (auto & menu : menus_)
	{
		menu->update();
	}
}

void QtWindow::close()
{
	if (mainWindow_.get() == nullptr)
	{
		return;
	}

	mainWindow_->close();
}

void QtWindow::show( bool wait /* = false */)
{
	if (mainWindow_.get() == nullptr)
	{
		return;
	}
	mainWindow_->setWindowModality( modalityFlag_ );
	if (wait)
	{
		waitForWindowExposed();
	}
	mainWindow_->show();
}

void QtWindow::showMaximized( bool wait /* = false */)
{
	if (mainWindow_.get() == nullptr)
	{
		return;
	}
	mainWindow_->setWindowModality( modalityFlag_ );
	if (wait)
	{
		waitForWindowExposed();
	}
	mainWindow_->showMaximized();
}

void QtWindow::showModal()
{
	if (mainWindow_.get() == nullptr)
	{
		return;
	}
	mainWindow_->setWindowModality( Qt::ApplicationModal );
	mainWindow_->show();
}

void QtWindow::hide()
{
	if (mainWindow_.get() == nullptr)
	{
		return;
	}

	mainWindow_->hide();
}

const Menus & QtWindow::menus() const
{
	return menus_;
}

const Regions & QtWindow::regions() const
{
	return regions_;
}

QMainWindow * QtWindow::window() const
{
	return mainWindow_.get();
}

void QtWindow::waitForWindowExposed()
{
	if (mainWindow_.get() == nullptr)
	{
		return;
	}
	enum { TimeOutMs = 10 };
	QElapsedTimer timer;
	const int timeout = 1000;
	if (!mainWindow_->windowHandle())
	{
		mainWindow_->createWinId();
	}
	auto window = mainWindow_->windowHandle();
	timer.start();
	while (!window->isExposed()) 
	{
		const int remaining = timeout - int(timer.elapsed());
		if (remaining <= 0)
		{
			break;
		}
		QCoreApplication::processEvents(QEventLoop::AllEvents, remaining);
		QCoreApplication::sendPostedEvents(0, QEvent::DeferredDelete);
		std::this_thread::sleep_for( std::chrono::milliseconds( uint32_t( TimeOutMs ) ) );
	}
}

bool QtWindow::eventFilter( QObject * obj, QEvent * event )
{
	if (obj == mainWindow_.get())
	{
		if (event->type() == QEvent::Close)
		{
			this->notifyCloseEvent();
			return true;
		}
	}
	return QObject::eventFilter( obj, event );
}
