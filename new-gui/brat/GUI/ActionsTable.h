#ifndef ACTIONS_TABLE_H
#define ACTIONS_TABLE_H


class CApplicationSettings;



//////////////////////////////////////////////////////////////////////////////////
//					Application Global QAction Tags
//////////////////////////////////////////////////////////////////////////////////


enum EActionTag : int
{	
	eAction_Separator,

	eAction_RecentFile1, eAction_RecentFile2, eAction_RecentFile3, 
	eAction_RecentFile4, eAction_RecentFile5, eAction_RecentFile6, 
	eAction_RecentFile7, eAction_RecentFile8, eAction_RecentFile9, 
	eAction_RecentFile10,
	eFirstRecentFileAction = eAction_RecentFile1,
	eLastRecentFileAction = eAction_RecentFile10,

	eAction_Exit,
	eAction_Open,
	eAction_New,
	eAction_Save,
	eAction_Save_As,
	eAction_Import_Workspace,
	eAction_Rename_Workspace,
	eAction_Delete_Workspace,
	eAction_About,
	eAction_Close,
	eAction_Close_All,
	eAction_Tile,
	eAction_Cascade,
	eAction_Cut,
	eAction_Copy,
	eAction_Paste,
	eAction_Next,
	eAction_Previous,
	eAction_Options,
	eAction_Undo,
	eAction_Redo,
	eAction_Delete,
	eAction_Select_All,
	eAction_Test,
	eAction_One_Click,
	eAction_Launch_Scheduler,
	eAction_Workspace_Tree,
	eAction_Save_Map_Image,
	eAction_Refresh_Map,
	eAction_Graphic_Settings,
	eAction_Full_Screen,
	eAction_Re_center,
	eAction_Zoom_In,
    eAction_Zoom_Out,
	eAction_Operations,
	eAction_Dataset,
	eAction_Filter, eLastAutomaticAction = eAction_Filter,

	eAction_DisplayEditor2D,
	eAction_DisplayEditor3D,
	eAction_DisplayEditorLog,
	eAction_DisplayEditorDock,

	eAction_MapEditorDistance,
	eAction_MapEditorArea,
	eAction_MapEditorMean,
	eAction_MapEditorStDev,
	eAction_MapEditorLinearRegression,

	eActionGroup_Projections,
	eAction_Projection1,
	eAction_Projection2,
	eAction_Projection3,

	eAction_ApplicationPaths_page,
	eAction_StartupOptions_page,
	eAction_ApplicationStyles_page,


	//add new items before this line

	EActionTags_size
};





//////////////////////////////////////////////////////////////////////////////////
//					Actions Table Entry Type - CActionInfo
//							Actions Table Management
//////////////////////////////////////////////////////////////////////////////////



struct CActionInfo : non_copyable
{
	///////////////////////////
	// Table Entry Section
	///////////////////////////

	// instance data

public:
	EActionTag mTag;
	std::string mName;
	std::string mTip;
	std::string mIconPath;
	std::string mOnIconPath;
	std::string mShortcut;
    std::wstring mWShortcut;
    QString mQShortcut;
    QList<QAction*> mActions;


	// construction / destruction

protected:
	// For internal processing only
	//
	CActionInfo( const QAction *a ) :
        mTag( EActionTags_size ), mName( q2a( a->text() ) ), mTip( q2a( a->toolTip() ) ), mShortcut( q2a( a->shortcut() ) ), mQShortcut( a->shortcut() ), mWShortcut( q2w( a->toolTip() ) )
	{}

public:
	CActionInfo( EActionTag ActionTag, const std::string &Name, const std::string &Tip, const std::string &IconPath, 
		const std::string &OnIconPath = "", const std::string &Shortcut = "" ) :
        mTag( ActionTag ), mName( Name ), mTip( Tip ), mIconPath( IconPath ), mOnIconPath( OnIconPath ), mShortcut( Shortcut ), mQShortcut( Shortcut.c_str() ), mWShortcut( q2w( Shortcut.c_str() ) )
	{}

	virtual ~CActionInfo()
	{}


	// access - testers

	bool IsAutomatic() const
	{
		return mTag != eAction_Separator && mTag <= eLastAutomaticAction;
	}


	// operators

    bool operator == ( const CActionInfo &o ) const;


    static std::string FormatTip( const std::string &tip )
    {
        size_t delimiter = tip.find("\n");
        size_t split_pos = (delimiter == std::string::npos) ?  0 : delimiter;

        std::string name        = tip.substr(0, split_pos);
        std::string description = tip.substr( split_pos );

        return "<html><body><p><b>" + name + "</b></p><p>" + description + "</p></body></html>";
    }


	///////////////////////////////////
	//	Table Management Section
	//	(all static member functions)
	///////////////////////////////////

public:
	// static members: query process hidden Actions Table


	// Calls SetActionProperties
	//
	static QAction* CreateAction( QObject *parent, EActionTag tag );
	

	// Groups actions under a new menu button. Besides that, actions are left untouched.
	//	A menu button is a special kind of action, that nevertheless can figure in the actions 
	//	table, so the properties of the new action/button are assigned based on the table entry.
	//	Being (so far) a more limited type of action, SetActionProperties is not used.
	//
	static QToolButton* CreateMenuButton( EActionTag button_tag, const QList< QAction* > &actions );


	// Calls CreateAction
	//
	static QActionGroup* CreateActionsGroup( QWidget *group_parent, const std::vector< EActionTag > &tags, bool exclusive );


	// For the Qt dock widgets actions, automatically created to view/hide them
	// Calls SetActionProperties
	//
	static QAction* SetDockActionProperties( QDockWidget *dock, EActionTag tag );


	// Intended for automatically created (designer) actions, such as brat's main menu 
	//	bar. The implementation does not impose any restriction, but there is no reason 
	//	for "a" to be an action defined in the source code. As in all other cases, the
	//	entry that matches "a" must exist in the table; see operator == for the	properties 
	//	used to consider a successful equality match between a table entry and "a".
	//
	// RecentFileActions are not considered and return EActionTags_size as failure result.
	// Separators return the separator tag and are not affected.
	//
	static EActionTag UpdateActionProperties( QAction *a );


	static void UpdateActionsState( std::initializer_list< EActionTag > tags, bool enable );

private:

	static const CActionInfo& ActionInfo( EActionTag tag );

	
	// Assigns 
	//	- tool-top
	//	- icon
	//	- checkable property if "on" icon is also specified in table
	//	- "on" icon, if specified in table
	//
	// Updates 
	//	- ActionsTable entry identified by "tag", adding "a" to its 
	//	physical actions list (which is not intended for runtime production 
	//	use)
	//
	// Returns
	//	- the same received action "a"
	//
    static QAction* SetActionProperties( QAction *a, EActionTag tag );


	// RecentFileAction not considered. EActionTags_size is returned with "not found" meaning.
	//
	static EActionTag FindEntry( const QAction *a );


	//not used
	static QToolButton* CreateMenuButton( QObject *parent, EActionTag button_tag, const std::vector< EActionTag > &tags );


	// diagnostic

	static bool ActionsTableIntegrityChecked;


	static bool CheckActionsTableIntegrity();
};





//////////////////////////////////////////////////////////////////////////////////
//						Most Recent Files QActions
//////////////////////////////////////////////////////////////////////////////////

class CRecentFilesProcessor : public QObject
{
	Q_OBJECT


	// DATA

	static const size_t smMaxRecentFiles = eLastRecentFileAction - eFirstRecentFileAction + 1;

	QMenu *mRecentFilesMenu = nullptr;
	QAction *mRecentFileActions[ smMaxRecentFiles ];
	QStringList mRecentFiles;
	const QString mSettingsSectionName;

	// CONSTRUCTION / DESTRUCTION

	void SetupMenu( QMainWindow *parent, QAction *ActionAfter );
public:
	CRecentFilesProcessor( QMainWindow *parent, const std::string MenuName, QMenu *MenuParent, QAction *ActionAfter, const QString &SettingsSectionName );

	CRecentFilesProcessor( QMainWindow *parent, QMenu *RecentFiles_menu, QAction *ActionAfter, const QString &SettingsSectionName );

	virtual ~CRecentFilesProcessor()
	{}


	// ACCESS

	size_t size() const							//can be used by LoadLastFileAtStartUp logic
	{
		return (size_t)mRecentFiles.size();
	}
	QString path( const size_t i ) const		//can be used by LoadLastFileAtStartUp logic
	{
		assert__( i < size() );

		return mRecentFileActions[ i ]->data().toString();
	}


	// UPDATE: to be called when file is successfully opened

	void setCurrentFile( const QString &curFile )
	{
		mRecentFiles.removeAll( curFile );
		mRecentFiles.prepend( curFile );
		UpdateRecentFileActions();
	}


	// PERSISTENCE  

	//	... using Qt settings logic

	void ReadSettings( const QSettings &settings );

	void WriteSettings( QSettings &settings );

	// ... using BRAT v3 logic

	virtual bool ReadSettings();

	virtual bool WriteSettings();


protected:
	// IMPLEMENTATION details

	template< class CONTAINER >
	bool LoadRecentFilesGeneric( CONTAINER &paths );

	template< class CONTAINER >
	bool SaveRecentFilesGeneric( const CONTAINER &paths );

	void UpdateRecentFileActions();

signals:

	void triggered( QAction *action );
	void updatedRecentFiles();

private slots:

	void openRecentFile()
	{
		QAction *action = qobject_cast< QAction*>( sender() );
		if ( action )
			emit triggered( action );
	}
};




#endif // ACTIONS_TABLE_H