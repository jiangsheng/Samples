; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CWDSSampleView
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "WDSSample.h"
LastPage=0

ClassCount=7
Class1=CWDSSampleApp
Class2=CWDSSampleDoc
Class3=CWDSSampleView
Class4=CMainFrame

ResourceCount=4
Resource1=IDR_WDSSAMTYPE
Resource2=IDD_ABOUTBOX
Resource3=IDR_MAINFRAME
Class5=CChildFrame
Class6=CAboutDlg
Class7=CSearchOptionDlg
Resource4=IDD_DIALOG_SEARCH_OPTIONS

[CLS:CWDSSampleApp]
Type=0
HeaderFile=WDSSample.h
ImplementationFile=WDSSample.cpp
Filter=N

[CLS:CWDSSampleDoc]
Type=0
HeaderFile=WDSSampleDoc.h
ImplementationFile=WDSSampleDoc.cpp
Filter=N

[CLS:CWDSSampleView]
Type=0
HeaderFile=WDSSampleView.h
ImplementationFile=WDSSampleView.cpp
Filter=C
BaseClass=CListView
VirtualFilter=VWC
LastObject=CWDSSampleView


[CLS:CMainFrame]
Type=0
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
Filter=T


[CLS:CChildFrame]
Type=0
HeaderFile=ChildFrm.h
ImplementationFile=ChildFrm.cpp
Filter=M
BaseClass=CMDIChildWnd
VirtualFilter=mfWC
LastObject=CChildFrame


[CLS:CAboutDlg]
Type=0
HeaderFile=WDSSample.cpp
ImplementationFile=WDSSample.cpp
Filter=D
LastObject=CAboutDlg

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[MNU:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_PRINT_SETUP
Command4=ID_FILE_MRU_FILE1
Command5=ID_APP_EXIT
Command6=ID_VIEW_TOOLBAR
Command7=ID_VIEW_STATUS_BAR
Command8=ID_APP_ABOUT
CommandCount=8

[TB:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_EDIT_CUT
Command5=ID_EDIT_COPY
Command6=ID_EDIT_PASTE
Command7=ID_FILE_PRINT
Command8=ID_APP_ABOUT
CommandCount=8

[MNU:IDR_WDSSAMTYPE]
Type=1
Class=CWDSSampleView
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_CLOSE
Command4=ID_FILE_SAVE
Command5=ID_FILE_SAVE_AS
Command6=ID_FILE_PRINT
Command7=ID_FILE_PRINT_PREVIEW
Command8=ID_FILE_PRINT_SETUP
Command9=ID_FILE_MRU_FILE1
Command10=ID_APP_EXIT
Command11=ID_EDIT_UNDO
Command12=ID_EDIT_CUT
Command13=ID_EDIT_COPY
Command14=ID_EDIT_PASTE
Command15=ID_VIEW_TOOLBAR
Command16=ID_VIEW_STATUS_BAR
Command17=ID_WINDOW_NEW
Command18=ID_WINDOW_CASCADE
Command19=ID_WINDOW_TILE_HORZ
Command20=ID_WINDOW_ARRANGE
Command21=ID_APP_ABOUT
CommandCount=21

[ACL:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_FILE_PRINT
Command5=ID_EDIT_UNDO
Command6=ID_EDIT_CUT
Command7=ID_EDIT_COPY
Command8=ID_EDIT_PASTE
Command9=ID_EDIT_UNDO
Command10=ID_EDIT_CUT
Command11=ID_EDIT_COPY
Command12=ID_EDIT_PASTE
Command13=ID_NEXT_PANE
Command14=ID_PREV_PANE
CommandCount=14

[DLG:IDD_DIALOG_SEARCH_OPTIONS]
Type=1
Class=CSearchOptionDlg
ControlCount=4
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=IDC_EDIT1,edit,1350631552

[CLS:CSearchOptionDlg]
Type=0
HeaderFile=SearchOptionDlg.h
ImplementationFile=SearchOptionDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=CSearchOptionDlg
VirtualFilter=dWC

