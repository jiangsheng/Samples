; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CDlgDocOutline
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "IEAutomation.h"
ODLFile=IEAutomation.odl
LastPage=0

ClassCount=10
Class1=CIEAutomationApp
Class2=CIEAutomationDoc
Class3=CAboutDlg
Class4=CMainFrame

ResourceCount=12
Resource1=IDR_MAINFRAME
Resource2=IDR_IEAUTOTYPE
Class5=CChildFrame
Class6=CIEAutomationView
Resource3=IDD_ABOUTBOX
Resource4=IDR_FAVORITES_POPUP
Resource5=IDR_FONT_POPUP
Resource6=IDD_DLG_DOC_OUTLINE (English (U.S.))
Resource7=IDD_DLG_SCRIPT (English (U.S.))
Resource8=IDD_DLG_AUTHENTICATE (English (U.S.))
Class7=CDlgInputText
Resource9=IDD_DLG_INPUT_TEXT (English (U.S.))
Class8=CDlgAuthenticate
Class9=CDOMExternal
Resource10=IDD_ABOUTBOX (English (U.S.))
Class10=CDlgDocOutline
Resource11=IDR_IEAUTOTYPE (English (U.S.))
Resource12=IDR_MAINFRAME (English (U.S.))

[CLS:CIEAutomationApp]
Type=0
HeaderFile=IEAutomation.h
ImplementationFile=IEAutomation.cpp
Filter=N

[CLS:CIEAutomationDoc]
Type=0
HeaderFile=IEAutomationDoc.h
ImplementationFile=IEAutomationDoc.cpp
Filter=N

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


[CLS:CAboutDlg]
Type=0
HeaderFile=IEAutomation.cpp
ImplementationFile=IEAutomation.cpp
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

[MNU:IDR_IEAUTOTYPE]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_CLOSE
Command4=ID_FILE_SAVE
Command5=ID_FILE_SAVE_AS
Command6=ID_FILE_PRINT
Command7=ID_FILE_PRINT_SETUP
Command8=ID_FILE_MRU_FILE1
Command9=ID_APP_EXIT
Command10=ID_EDIT_UNDO
Command11=ID_EDIT_CUT
Command12=ID_EDIT_COPY
Command13=ID_EDIT_PASTE
Command14=ID_VIEW_TOOLBAR
Command15=ID_VIEW_STATUS_BAR
Command16=ID_WINDOW_NEW
Command17=ID_WINDOW_CASCADE
Command18=ID_WINDOW_TILE_HORZ
Command19=ID_WINDOW_ARRANGE
Command20=ID_APP_ABOUT
CommandCount=20

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

[CLS:CIEAutomationView]
Type=0
HeaderFile=IEAutomationView.h
ImplementationFile=IEAutomationView.cpp
BaseClass=CHtmlView
Filter=C
LastObject=CIEAutomationView
VirtualFilter=7VWC

[MNU:IDR_FONT_POPUP]
Type=1
Class=?
Command1=ID_VIEW_FONTS_LARGEST
Command2=ID_VIEW_FONTS_LARGE
Command3=ID_VIEW_FONTS_MEDIUM
Command4=ID_VIEW_FONTS_SMALL
Command5=ID_VIEW_FONTS_SMALLEST
CommandCount=5

[MNU:IDR_FAVORITES_POPUP]
Type=1
Class=?
Command1=ID_BOGUS
CommandCount=1

[MNU:IDR_MAINFRAME (English (U.S.))]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_PRINT_SETUP
Command4=ID_FILE_MRU_FILE1
Command5=ID_APP_EXIT
Command6=ID_VIEW_TOOLBAR
Command7=ID_VIEW_STATUS_BAR
Command8=ID_APP_ABOUT
CommandCount=8

[MNU:IDR_IEAUTOTYPE (English (U.S.))]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_PRINT_SETUP
Command4=ID_FILE_OFFLINE
Command5=ID_FILE_MRU_FILE1
Command6=ID_APP_EXIT
Command7=ID_VIEW_TOOLBAR
Command8=ID_VIEW_STATUS_BAR
Command9=ID_VIEW_SOURCE
Command10=ID_EDIT_FIND
Command11=ID_VIEW_OPTION
Command12=ID_GO_BACK
Command13=ID_GO_FORWARD
Command14=ID_GO_START_PAGE
Command15=ID_GO_SEARCH_THE_WEB
Command16=ID_FAVORITES_ADD
Command17=ID_FAVORITES_MANAGE
Command18=ID_SCRIPT_EDIT
Command19=ID_SCRIPT_RUN
Command20=ID_WINDOW_NEW
Command21=ID_WINDOW_CASCADE
Command22=ID_WINDOW_TILE_HORZ
Command23=ID_WINDOW_ARRANGE
Command24=ID_APP_ABOUT
CommandCount=24

[TB:IDR_MAINFRAME (English (U.S.))]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_EDIT_CUT
Command5=ID_EDIT_COPY
Command6=ID_EDIT_PASTE
Command7=ID_FILE_PRINT
Command8=ID_APP_ABOUT
CommandCount=8

[DLG:IDD_ABOUTBOX (English (U.S.))]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[CLS:CDlgAuthenticate]
Type=0
HeaderFile=DlgAuthenticate.h
ImplementationFile=DlgAuthenticate.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_EDIT_USERNAME
VirtualFilter=dWC

[CLS:CDOMExternal]
Type=0
HeaderFile=DOMExternal.h
ImplementationFile=DOMExternal.cpp
BaseClass=CCmdTargetPlus
Filter=N
VirtualFilter=C
LastObject=CDOMExternal

[CLS:CDlgDocOutline]
Type=0
HeaderFile=DlgDocOutline.h
ImplementationFile=DlgDocOutline.cpp
BaseClass=CDialog
Filter=D
LastObject=CDlgDocOutline
VirtualFilter=dWC

[DLG:IDD_DLG_DOC_OUTLINE (English (U.S.))]
Type=1
Class=CDlgDocOutline
ControlCount=4
Control1=IDOK,button,1342242817
Control2=IDC_TREE,SysTreeView32,1350637591
Control3=IDC_BUTTON_EDIT_OUTHTML,button,1342242816
Control4=IDC_BUTTON_EDIT_FRAME_SOURCE,button,1342242816

[DLG:IDD_DLG_INPUT_TEXT (English (U.S.))]
Type=1
Class=CDlgInputText
ControlCount=3
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_EDIT,edit,1352728644

[DLG:IDD_DLG_SCRIPT (English (U.S.))]
Type=1
Class=?
ControlCount=3
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_EDIT_SCRIPT,edit,1352732740

[DLG:IDD_DLG_AUTHENTICATE (English (U.S.))]
Type=1
Class=CDlgAuthenticate
ControlCount=6
Control1=IDC_STATIC,static,1342308352
Control2=IDC_EDIT_USERNAME,edit,1350631552
Control3=IDC_STATIC,static,1342308352
Control4=IDC_EDIT_PASSWORD,edit,1350631584
Control5=IDOK,button,1342242817
Control6=IDCANCEL,button,1342242816

[CLS:CDlgInputText]
Type=0
HeaderFile=DlgInputText.h
ImplementationFile=DlgInputText.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_EDIT
VirtualFilter=dWC

