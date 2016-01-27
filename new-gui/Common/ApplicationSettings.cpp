// #include "stdafx.h"

// While we can't include stdafx.h, this pragma must be here
//
#if defined (__APPLE__)
#pragma clang diagnostic ignored "-Wdeprecated-register"
#endif

#include <QStyleFactory>

#include "new-gui/Common/+UtilsIO.h"
#include "new-gui/Common/QtUtilsIO.h"

#include "ApplicationSettings.h"


////////////////////////////////////////////////////////////////////////////////////////////
//
//									CApplicationSettings
//
////////////////////////////////////////////////////////////////////////////////////////////


bool CApplicationSettings::SaveConfig()
{
	return
	WriteVersionSignature() &&
	WriteSection( GROUP_COMMON, 
	 
		k_v( ENTRY_USER_MANUAL,				m_userManual ),
		k_v( ENTRY_USER_MANUAL_VIEWER,		m_userManualViewer ),

		k_v( ENTRY_APP_STYLE,				mAppStyle ),
		k_v( ENTRY_APPLICATION_STYLESHEETS, mCustomAppStyleSheet ),
		k_v( ENTRY_USE_DEFAULT_STYLE,		mUseDefaultStyle ),
		k_v( ENTRY_CUSTOM_STYLESHEET,		mCustomAppStyleSheet ),
		k_v( ENTRY_NO_STYLESHEET,			mNoStyleSheet )
	);
}


bool CApplicationSettings::LoadConfig()
{
	return
	( mSettings.status() == QSettings::NoError )	//just in case check; LoadConfig is typically called only once and before any other settings use
	&&
	ReadSection( GROUP_COMMON, 

		k_v( ENTRY_USER_MANUAL,				&m_userManual ),
		k_v( ENTRY_USER_MANUAL_VIEWER,		&m_userManualViewer ),

		k_v( ENTRY_APP_STYLE,				&mAppStyle ),
		k_v( ENTRY_APPLICATION_STYLESHEETS, &mCustomAppStyleSheet ),
		k_v( ENTRY_USE_DEFAULT_STYLE,		&mUseDefaultStyle ),
		k_v( ENTRY_CUSTOM_STYLESHEET,		&mCustomAppStyleSheet ),
		k_v( ENTRY_NO_STYLESHEET,			&mNoStyleSheet )
	);
}





//////////////////////////////////////////////////////////////////////////////////////////
//								Application Styles
//////////////////////////////////////////////////////////////////////////////////////////


//static
std::string CApplicationSettings::getNameOfStyle( const QStyle *s )
{
    return q2t<std::string>( s->objectName().toLower() );
}

//static
std::string CApplicationSettings::getNameOfStyle( QStyle *s, bool del )
{
    std::string result = getNameOfStyle( s );
    if ( del )
        delete s;
    return result;
}




static const std::vector< std::string >& buildStyleNamesList()
{
	static std::vector< std::string > styles;
	if ( styles.empty() )
	{
		QStringList l = QStyleFactory::keys();
		for ( int i = 0; i < l.size(); ++i )
		{
			const std::string key = q2t<std::string>( QString( l.at( i ) ).toLower() );
			styles.push_back( key );
			if ( 0 )
			{
                assert__( CApplicationSettings::getNameOfStyle( QStyleFactory::create( key.c_str() ), true ) == key );
			}
		}
	}
	return styles;
}

//static 
const std::vector< std::string >& CApplicationSettings::getStyles()
{
	static std::vector< std::string > styles = buildStyleNamesList();
	return styles;
}


//static 
size_t CApplicationSettings::getStyleIndex( const QString &qname )
{
	const std::vector< std::string > &v = getStyles();
	const size_t size = v.size();
	const std::string name = q2t<std::string>( qname );
	for ( size_t i = 0; i < size; ++i )
		if ( name == v[i] )
			return i;

	return (size_t)-1;
}


//static 
const std::vector< std::string >& CApplicationSettings::getStyleSheets( bool resources )
{
	static const std::vector< std::string > style_sheets =
	{
		"DarkStyle",
		"DarkOrangeStyle",
		"Dark-2015",
	};

	static const std::vector< std::string > res =
	{
		":/StyleSheets/StyleSheets/QTDark.stylesheet",						//with recommended style QCleanlooksStyle
		":/StyleSheets/StyleSheets/DarkOrange/darkorange.stylesheet",		//with recommended style QPlastiqueStyle
		":/StyleSheets/StyleSheets/Dark-2015/Dark-2015.stylesheet",		//with recommended style QPlastiqueStyle ?
	};

	assert__( EApplicationStylesSheets_size == style_sheets.size() );
	assert__( EApplicationStylesSheets_size == res.size() );

	return resources ? res : style_sheets;
}

//#include "new-gui/brat/BratApplication.h"

bool CApplicationSettings::setApplicationStyle( QApplication &app, QString default_style )
{
    Q_UNUSED( app );

	//validate options
	 
	if ( getStyleIndex( mAppStyle ) == (size_t)-1 )
		mAppStyle = default_style;

	//if ( mCustomAppStyleSheet < 0 || mCustomAppStyleSheet >= getStyleSheets( true ).size() )
	//	mCustomAppStyleSheet = smDefaultCustomAppStyleSheet;

	//use options

	const QString &name = mUseDefaultStyle ? default_style : mAppStyle;					assert__( !name.isEmpty() );
	QStyle *style = QStyleFactory::create( name );										assert__( !style->objectName().toLower().compare( name ) );
	QApplication::setStyle( style );												//assert__( !getCurrentStyleName().compare( name ) );
	mAppStyle = name;					//update options style field with actually used style

	//if ( mNoStyleSheet )
	//	app.setStyleSheet( nullptr );	//this is necessary because without this the "old" sheet continues to be used and the style assignment above is not enough
	//else
	//{
 //       QString resource = t2q( getStyleSheets( true )[mCustomAppStyleSheet] );
 //       QFile styleFile( resource );
 //       if ( !styleFile.open( QFile::ReadOnly ) )
 //       {
 //           if ( &options == &getAppOptions() )
 //           {
 //               setStyleSheet( nullptr );
 //               mNoStyleSheet = true;
 //           }
 //           else
 //               mCustomAppStyleSheet = getAppOptions().m_CustomAppStyleSheet;
 //           return false;
 //       }
 //       QString sheet( styleFile.readAll() );
 //       app.setStyleSheet( sheet );		//apparently this changes the style name for an empty string
	//}
	return true;
}