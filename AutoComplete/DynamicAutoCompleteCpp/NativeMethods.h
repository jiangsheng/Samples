#pragma once
namespace DynamicAutoCompleteCpp {
	using namespace System;
	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Collections::Generic;
	using namespace System::Text;
	using namespace System::Runtime::InteropServices::ComTypes;
	using namespace System::Collections::Specialized;
	using namespace System::Runtime::InteropServices;
	using namespace msclr;
	private ref class NativeMethods
		{
		internal:
			
			#include "IAutoComplete.h"
			#include "IAutoComplete2.h"
			#include "IAutoCompleteDropDown.h"
			#include "win32error.h"
		};
	}