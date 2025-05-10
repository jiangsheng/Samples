; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CPicViewView
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "PicView.h"
LastPage=0

ClassCount=5
Class1=CPicViewApp
Class2=CPicViewDoc
Class3=CPicViewView
Class4=CMainFrame

ResourceCount=2
Resource1=IDR_MAINFRAME
Class5=CAboutDlg
Resource2=IDD_ABOUTBOX

[CLS:CPicViewApp]
Type=0
HeaderFile=PicView.h
ImplementationFile=PicView.cpp
Filter=N
BaseClass=CWinApp
VirtualFilter=AC
LastObject=ID_FILE_OPEN

[CLS:CPicViewDoc]
Type=0
HeaderFile=PicViewDoc.h
ImplementationFile=PicViewDoc.cpp
Filter=N
LastObject=CPicViewDoc
BaseClass=CDocument
VirtualFilter=DC

[CLS:CPicViewView]
Type=0
HeaderFile=PicViewView.h
ImplementationFile=PicViewView.cpp
Filter=C
BaseClass=CListView
VirtualFilter=VWC
LastObject=CPicViewView


[CLS:CMainFrame]
Type=0
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
Filter=T
LastObject=ID_FILE_OPEN




[CLS:CAboutDlg]
Type=0
HeaderFile=PicView.cpp
ImplementationFile=PicView.cpp
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
Command3=ID_APP_EXIT
Command4=ID_VIEW_TOOLBAR
Command5=ID_VIEW_STATUS_BAR
Command6=ID_APP_ABOUT
CommandCount=6

[ACL:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_EDIT_UNDO
Command5=ID_EDIT_CUT
Command6=ID_EDIT_COPY
Command7=ID_EDIT_PASTE
Command8=ID_EDIT_UNDO
Command9=ID_EDIT_CUT
Command10=ID_EDIT_COPY
Command11=ID_EDIT_PASTE
Command12=ID_NEXT_PANE
Command13=ID_PREV_PANE
CommandCount=13

[TB:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_APP_ABOUT
CommandCount=3

