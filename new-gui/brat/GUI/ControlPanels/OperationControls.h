#if !defined GUI_CONTROL_PANELS_OPERATIONS_CONTROL_PANEL_H
#define GUI_CONTROL_PANELS_OPERATIONS_CONTROL_PANEL_H


#include "libbrathl/Product.h"

#include "ControlPanel.h"


class CProcessesTable;
class CDataExpressionsTreeWidget;
class CFieldsTreeWidget;
class CMapDisplayData;
class CBratFilters;


class COperationControls : public CDesktopControlsPanel
{
#if defined (__APPLE__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Winconsistent-missing-override"
#endif

	Q_OBJECT;

#if defined (__APPLE__)
#pragma clang diagnostic pop
#endif

	// types

	using base_t = CDesktopControlsPanel;

public:

    enum EMode
    {
        eQuick,
        eAdvanced
    };

	enum EPredefinedVariables
	{
		eSSH,
		eSWH,
		eWinds,
		eSigma0,
		eRange,

		EPredefinedVariables_size
	};


	//static members

    static const std::vector<std::string> smQuickPredefinedVariableNames;

	static bool FormulaMatchesQuickAlias( const std::string &description, EPredefinedVariables index );

	CField* QuickFindField( CProduct *product, EPredefinedVariables index, bool &alias_used, std::string &field_error_msg );


	static void SelectOperationDataset( COperation *operation, QComboBox *combo, bool block_signals );

	static bool AssignFilter( const CBratFilters &brat_filters, COperation *operation, const std::string &name );

	static bool RemoveFilter( COperation *operation, const std::string &name );

	static void RemoveOperationFormulas( COperation *operation );


	//...fill helpers

	static QList<QAction*> CreateDataComputationActions();
    static QList<QAction*> CreateDataSmoothingActions();


protected:

	// instance variables

    CStackedWidget *mStackWidget = nullptr;
	QWidget *mQuickOperationsPage = nullptr;
	QWidget *mAdvancedOperationsPage = nullptr;


	QWidget *mCommonGroup = nullptr;

	QToolButton *mOperationExportButton = nullptr;
	QAction *mExportOperationAction = nullptr;
	QAction *mEditExportAsciiAction = nullptr;
	QToolButton *mOperationStatisticsButton = nullptr;

	QToolButton *mSplitPlotsButton = nullptr;

	stack_button_type *mQuickPageButton = nullptr;
	stack_button_type *mAdvancedPageButton = nullptr;


	//...advanced

	QToolButton *mOperationFilterButton_Advanced = nullptr;
	QComboBox *mOperationsCombo = nullptr;
	QComboBox *mAdvancedDatasetsCombo = nullptr;
	QToolButton *mNewOperationButton = nullptr;
	QToolButton *mDeleteOperationButton = nullptr;
    QToolButton *mRenameOperationButton = nullptr;
    QToolButton *mDuplicateOperationButton = nullptr;

	QToolButton *mAdvancedExecuteButton = nullptr;
	QAction *mDelayExecutionAction = nullptr;
	QAction *mLaunchSchedulerAction = nullptr;
	CProcessesTable *mProcessesTable = nullptr;

	QGroupBox *mOperationExpressionsGroup = nullptr;

	QTimer mTimer;

	QToolButton *mShowAliasesButton = nullptr;

	QToolButton *mInsertFunction = nullptr;
	QToolButton *mInsertAlgorithm = nullptr;
	QToolButton *mInsertFormula = nullptr;
	QToolButton *mSaveAsFormula = nullptr;
	QToolButton *mCheckSyntaxButton = nullptr;
	QToolButton *mShowInfoButton = nullptr;
	QGroupBox *mExpressionGroup = nullptr;

	QToolButton *mDataComputation = nullptr;
	QActionGroup *mDataComputationGroup = nullptr;

    // NOTE: 'filter' (Brat v.3) was renamed to 'Smoothing' (Brat v.4)
    // In Brat v.4 'filter' is a reserved word to spatial and temporal filters
    QToolButton *mDataSmoothing = nullptr;
    QActionGroup *mDataSmoothingGroup = nullptr;

	CFieldsTreeWidget *mAdvancedFieldsTree = nullptr;
    CTextWidget *mAdvancedFieldDesc = nullptr;

	QToolButton *mSwitchToMapButton = nullptr;
	QToolButton *mSwitchToPlotButton = nullptr;
	CDataExpressionsTreeWidget *mDataExpressionsTree = nullptr;

	QgsCollapsibleGroupBox *mSamplingGroup = nullptr;
    //QComboBox *mAdvFilter = nullptr;
    QLabel *mXLonLabel = nullptr;
    QLabel *mYLatLabel = nullptr;
    QLineEdit *mXLonMinValue = nullptr;
    QLineEdit *mXLonMaxValue = nullptr;
    QLineEdit *mYLatMinValue = nullptr;
    QLineEdit *mYLatMaxValue = nullptr;
    QLineEdit *mXLonStep = nullptr;
    QLineEdit *mYLatStep = nullptr;
    QLineEdit *mXLonCutOff = nullptr;
    QLineEdit *mYLatCutOff = nullptr;
    QLabel *mXLonIntervalsLabel = nullptr;
    QLabel *mYLatIntervalsLabel = nullptr;
    QLabel *XiconWarning = nullptr;
    QLabel *YiconWarning = nullptr;


	//...quick

	QToolButton *mOperationFilterButton_Quick = nullptr;
	QComboBox *mQuickDatasetsCombo = nullptr;
	QToolButton *mAddVariable = nullptr;
	QToolButton *mClearVariables = nullptr;
	QToolButton *mDisplayMapButton = nullptr;
	QToolButton *mDisplayPlotButton = nullptr;
	QListWidget *mQuickVariablesList = nullptr;
    CTextWidget *mQuickFieldDesc = nullptr;
	bool mCanExecuteQuickOperation = false;


	//...domain variables

    CWorkspace *mWRoot = nullptr;
    CWorkspaceDataset *mWDataset = nullptr;
	CWorkspaceOperation *mWOperation = nullptr;
	CWorkspaceFormula *mWFormula = nullptr;
	CWorkspaceDisplay *mWDisplay = nullptr;

	const CBratFilters &mBratFilters;

	COperation *mCurrentOperation = nullptr;
	COperation *mQuickOperation = nullptr;
	const CDataset *mCurrentOriginalDataset = nullptr;
	CProduct *mProduct = nullptr;
    CFormula *mUserFormula = nullptr;
    CFormula *mXformula = nullptr;
    CFormula *mYformula = nullptr;

    CStringMap mMapFormulaString;


	// construction / destruction

    void Wire();
	QWidget* CreateCommonWidgets( QAbstractButton *b1, QAbstractButton *b2 );
	void CreateQuickOperationsPage();
	void CreateAdancedOperationsPage();

public:
	COperationControls( CProcessesTable *processes_table, CModel &model, CDesktopManagerBase *manager, QWidget *parent = nullptr, Qt::WindowFlags f = 0 );

	virtual ~COperationControls();


	// access 

    bool AdvancedMode() const;

    void SetAdvancedMode( bool advanced );


	// remaining operations

protected:

	//filtering

    void ResetFilterActions();
    void ResetFilterSelection();

	bool AssignAdvancedFilter( const std::string &name );
	bool RemoveAdvancedFilter( const std::string &name );
	bool AssignQuickFilter( const std::string &name );
	bool RemoveQuickFilter( const std::string &name );

	void SelectOperation( const std::string &name, bool select_map );	//meant for quick, designed (not tested) for all

	//quick

	CDataset* QuickDatasetSelected() const;
	COperation* QuickOperation() const;
	COperation* CreateEmptyQuickOperation();
	COperation* CreateQuickOperation( CMapTypeOp::ETypeOp type );
    bool IsQuickOperationSelected() const;
    void UpdateFieldsCheckState();

	//advanced

	void SelectDataComputationMode();
    void SelectDataSmoothingMode();
    std::string GetOpunit();
    /// Sampling methods ////////////////
    void UpdateSamplingGroup();
    void GetMinMax(CFormula *formula);
    bool ValidateData();
    bool VerifyMinMaxXY();
    bool VerifyMinMaxX();
    bool VerifyYMinMaxY();
    bool VerifyMinMaxAsDate( CFormula *formula, QLineEdit *LineEdit_Min, QLineEdit *LineEdit_Max );
    void GetValueAsDate( QLineEdit *mLineEdit_Axisvalue, double &seconds, double defValue, double min, double max );
    bool ComputeXYInterval();
    bool ComputeInterval(CFormula *formula, QLabel *IntervalsLabel, QLineEdit *StepLineEdit, QLabel *IconWarning );
    /////////////////////////////////////

    bool MapRequested() const;

	//both

	bool CheckQuickOperation( std::string& msg, bool basicControl, const CStringMap* aliases );		//CtrlOperation
	bool CheckAdvancedOperation( std::string& msg, bool basicControl, const CStringMap* aliases );	//CtrlOperation
	void LaunchDisplay( const std::string &display_name );
	bool Execute( bool sync );


	//remaining

	bool AssignDataset( const CDataset *dataset, bool changing_used_dataset );
	bool SelectDataset( const std::string &dataset_name );
	//bool SelectDataset( int dataset_index );
	bool SelectDataset( const CDataset *dataset );

    void FillDatasets_Advanced( int index );

	void SyncProcessFinished( int exit_code, QProcess::ExitStatus exitStatus, const COperation *operation );
	void AsyncProcessFinished( int exit_code, QProcess::ExitStatus exitStatus, const COperation *operation );

signals:
	void AsyncProcessExecution( bool executing );
	void SyncProcessExecution( bool executing );
    void OperationModified( const COperation *operation );


public slots:
	void HandleLaunchScheduler();

protected slots:

	//quick

	void HandleWorkspaceChanged_Quick();		//not really a slot; called by advanced

	void HandleSelectedDatasetChanged_Quick( int dataset_index );
	void HandleSelectedFieldChanged_Quick( int variable_index );
	void HandleVariableStateChanged_Quick( QListWidgetItem *item );
    void HandleDatasetsChanged_Quick(CDataset*);

	void HandleOperationFilterButton_Quick( QAction *a );
	void HandleOperationFilterButtonToggled_Quick( bool );

	void HandleQuickMap();
	void HandleQuickPlot();

	//remaining

	void HandlePageChanged( int index );

	//entry point
	void HandleWorkspaceChanged();

	void HandleSelectedOperationChanged( int operation_index );
	void HandleSelectedFieldChanged_Advanced();
	void HandleSelectedDatasetChanged_Advanced( int dataset_index );
    void HandleDatasetsChanged_Advanced( CDataset *dataset );

    void HandleFiltersChanged();
	void HandleFilterCompositionChanged( std::string filter_name );

	void HandleSwitchExpressionType();

	void HandleFormulaInserted( CFormula *formula );
	void HandleSelectedFormulaChanged( CFormula *formula );

	void HandleNewOperation();
	void HandleDeleteOperation();
    void HandleRenameOperation();
    void HandleDuplicateOperation();

	void HandleOperationFilterButton_Advanced( QAction *a );
	void HandleOperationFilterButtonToggled_Advanced( bool );

	void HandleExportOperation();
	void HandleEditExportAscii();
	void HandleOperationStatistics();

	bool HandleExecute();
	void HandleProcessFinished( int exit_code, QProcess::ExitStatus exitStatus, const COperation *operation, bool sync )
	{
		sync ?
			SyncProcessFinished( exit_code, exitStatus, operation )
			:
			AsyncProcessFinished( exit_code, exitStatus, operation );
	}
	void HandleDelayExecution();
	void SchedulerProcessError( QProcess::ProcessError );

	void HandleInsertFunction();
	void HandleInsertAlgorithm();
	void HandleInsertFormula();
	void HandleSaveAsFormula();
	void HandleDataComputation();
    void HandleDataSmoothing();

	void HandleShowAliases();
	void HandleCheckSyntax();
	void HandleShowInfo();
};





#endif	//GUI_CONTROL_PANELS_OPERATIONS_CONTROL_PANEL_H
