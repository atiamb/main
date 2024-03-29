/*
* This file is part of BRAT
*
* BRAT is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* as published by the Free Software Foundation; either version 2
* of the License, or (at your option) any later version.
*
* BRAT is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/
#if !defined GUI_DESKTOP_MANAGER_H
#define GUI_DESKTOP_MANAGER_H

#include <QMainWindow>
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QGridLayout>

#include "new-gui/Common/QtUtils.h"
#include "ApplicationPaths.h"



class CBratSettings;

class CTabbedDock;
class CGlobeWidget;
class CMapWidget;


//#define TABBED_MANAGER

#if defined TABBED_MANAGER
using desktop_manager_base_t = QTabWidget;
#else
using desktop_manager_base_t = QWidget;
#endif


/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
//						Desktop Managers Base Classes
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////


class CDesktopManagerBase : public desktop_manager_base_t, non_copyable
{
#if defined (__APPLE__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Winconsistent-missing-override"
#endif

    Q_OBJECT

#if defined (__APPLE__)
#pragma clang diagnostic pop
#endif
	
	// types

	using base_t = desktop_manager_base_t;

protected:

	//data

public:
	const CApplicationPaths &mPaths;

protected:
	QGroupBox *mMapBox = nullptr;
	CMapWidget *mMap = nullptr;
	CTabbedDock *mMapDock = nullptr;

protected:

	// construction / destruction

#if defined TABBED_MANAGER
	CDesktopManagerBase( const CApplicationPaths &paths, QMainWindow *const parent );
#else
	CDesktopManagerBase( const CBratSettings &settings, QMainWindow *const parent, Qt::WindowFlags f = 0 );
#endif

	static void SetChildWindowTitle( QWidget *child, const QWidget *widget = nullptr )
	{
		QString title = widget ? widget->windowTitle() : "";
		if ( !title.isEmpty() )
			child->setWindowTitle( title + "[*]" );
	}

public:
	virtual ~CDesktopManagerBase()
	{
		//delete mMap;
	}


	// access

	CMapWidget* Map() { return mMap; }


	CTabbedDock* MapDock() { return mMapDock; }


	// abstract interface

	virtual QList<QWidget*> SubWindowList() = 0;

public:
	virtual QWidget* AddSubWindow( QWidget *widget, Qt::WindowFlags flags = 0 ) = 0;

	virtual QWidget* AddSubWindow( std::function< QWidget*() > f, Qt::WindowFlags flags = 0 );


	virtual void SubWindowClosed( QWidget *emitter ) = 0;

	//emit AllSubWindowsClosed only if there was something to close
	//
	virtual void CloseAllSubWindows() = 0;


protected:

	void SetMapTitle( const QString &title )
	{
		mMapBox->setTitle( title );
	}


signals:

	void subWindowActivated( QWidget *window );

	void AllSubWindowsClosed();


protected slots:

	void HandleMapWindowTitleChanged( const QString &title );
};




template< class SUB_WINDOW >
class CAbstractDesktopManager : public CDesktopManagerBase
{
	// types

	using base_t = CDesktopManagerBase;

protected:

	using desktop_child_t = SUB_WINDOW;

	//data

protected:

	// construction / destruction

#if defined TABBED_MANAGER
	CAbstractDesktopManager( const CBratSettings &settings, QMainWindow *const parent )
		: base_t( settings, parent )
#else
	CAbstractDesktopManager( const CBratSettings &settings, QMainWindow *const parent, Qt::WindowFlags f = 0 )
		: base_t( settings, parent, f )
#endif
	{}


public:
	virtual ~CAbstractDesktopManager()
	{}
};




/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
//								SDI Desktop Manager
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////
//									SDI SubWindow
/////////////////////////////////////////////////////////////////////////////////////

class CSubWindow : public QDialog
{
#if defined (__APPLE__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Winconsistent-missing-override"
#endif

    Q_OBJECT

#if defined (__APPLE__)
#pragma clang diagnostic pop
#endif
		
	using base_t = QDialog;

	//data

	QWidget *mWidget = nullptr;

public:
	//explicit 
	CSubWindow( QWidget *widget, QWidget *parent = nullptr, Qt::WindowFlags f = 0 );

	virtual ~CSubWindow()
	{}


	//access

	QWidget* Widget() { return mWidget;	}

protected:

	virtual void closeEvent( QCloseEvent *event ) override
	{
		if ( mWidget->close() )
		{
			emit closed( this );
			event->accept();
		}
		else
			event->ignore();
	}

	virtual void reject() override
	{}

signals:

	// qt moc does not recognize typedefs in receiver, so use known type as argument
	//
	void closed( QWidget *emitter );		
};



/////////////////////////////////////////////////////////////////////////////////////
//									Manager
/////////////////////////////////////////////////////////////////////////////////////

class CDesktopManagerSDI : public CAbstractDesktopManager< CSubWindow >
{
#if defined (__APPLE__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Winconsistent-missing-override"
#endif

    Q_OBJECT

#if defined (__APPLE__)
#pragma clang diagnostic pop
#endif

	using base_t = CAbstractDesktopManager;

protected:

	//data

	QSplitter *mSplitter = nullptr;
	CGlobeWidget *mGlobeView = nullptr;
	QList<QWidget*> mSubWindows;

protected:

public:
	//ctor/dtor

	CDesktopManagerSDI( const CBratSettings &settings, QMainWindow *parent );

	virtual ~CDesktopManagerSDI()
	{}

public:
	virtual QList<QWidget*> SubWindowList() override
	{
		return mSubWindows;
	}

	virtual desktop_child_t* AddSubWindow( QWidget *widget, Qt::WindowFlags flags = 0 ) override;


	// see base class comment
	//
	virtual void CloseAllSubWindows() override
	{	
		if ( !mSubWindows.empty() )
		{
			//this also works on windows at least
			//for ( auto child : mSubWindows )
			//	child->close();
			foreach( auto child, mSubWindows )
				child->close();

			if ( mSubWindows.isEmpty() )
				emit AllSubWindowsClosed();
		}
	}

protected:
	QWidget* CenterOnWidget( QWidget *const w, const QWidget *const parent );

	QWidget* AddTab( QWidget *tab_widget, const QString &title );

protected slots:

	void closeAllSubWindows()
	{
		CloseAllSubWindows();
	}

    void SubWindowClosed( QWidget *emitter ) override
	{
		desktop_child_t *child = qobject_cast< desktop_child_t* >( emitter );

		mSubWindows.removeOne( child );
		if (mSubWindows.isEmpty())
			emit AllSubWindowsClosed();
	}
};





/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
//								MDI Desktop Manager
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////




//using desktop_manager_base_t = QMdiArea;
//using desktop_manager_child_t = QMdiSubWindow;

class CDesktopManagerMDI : public CAbstractDesktopManager< QMdiSubWindow >
{
#if defined (__APPLE__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Winconsistent-missing-override"
#endif

    Q_OBJECT

#if defined (__APPLE__)
#pragma clang diagnostic pop
#endif

	using desktop_child_t = QMdiSubWindow;

	using base_t = CAbstractDesktopManager< desktop_child_t >;

protected:

	//data

private:
	QMdiArea *mMdiArea = nullptr;

	//construction / destruction

protected:

	void AddMDIArea( QWidget *parent );
public:

	CDesktopManagerMDI( const CBratSettings &settings, QMainWindow *parent );

	virtual ~CDesktopManagerMDI()
	{}

	// access / operations

public:
    desktop_child_t* AddSubWindow( QWidget *widget, Qt::WindowFlags flags = 0 ) override
	{
        desktop_child_t *child = mMdiArea->addSubWindow( widget, flags );
        SetChildWindowTitle( child, widget );

		connect( child, SIGNAL( closed( QWidget* ) ), this, SLOT( SubWindowClosed( QWidget* ) ) );

		return child;
	}


	virtual QList<QWidget*> SubWindowList() override
	{
		QList<QWidget*> list;
        auto mdi_list = mMdiArea->subWindowList();
		for ( auto pchild : mdi_list )
			list << pchild;
		return list;
	}

	// see base class comment
	//
	virtual void CloseAllSubWindows() override
	{
		if ( !mMdiArea->subWindowList().isEmpty() )
		{
			mMdiArea->closeAllSubWindows();
			if ( mMdiArea->subWindowList().isEmpty() )
				emit AllSubWindowsClosed();
		}
	}

protected slots:
    void SubWindowClosed( QWidget *emitter ) override
	{
        Q_UNUSED( emitter );

		if (mMdiArea->subWindowList().isEmpty())
			emit AllSubWindowsClosed();
	}
};




#endif	//GUI_DESKTOP_MANAGER_H
