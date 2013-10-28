// AutoCompleteDemo.cpp : main project file.

#include "stdafx.h"
#include "IAutoComplete2.h"
#include "AutocompleteBindingSource.h"
#include "FormTest.h"

using namespace AutoCompleteDemo;

[STAThreadAttribute]
int main(array<System::String ^> ^args)
{
	// Enabling Windows XP visual effects before any controls are created
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false); 

	// Create the main window and run it
	Application::Run(gcnew FormTest());
	return 0;
}
