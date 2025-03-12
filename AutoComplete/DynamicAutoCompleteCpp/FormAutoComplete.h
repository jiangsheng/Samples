#pragma once
#include "CandidateList.h"
#include "NativeMethods.h"
namespace DynamicAutoCompleteCpp {

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
	using namespace System::Diagnostics;
	/// <summary>
	/// Summary for Form1
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class FormAutoComplete : public System::Windows::Forms::Form
	{
		Object^ autoComplete ;
		CandidateList^ candidateList;
		int stringLength ;
		int randomStrings;
		Random^ rnd;
		NativeMethods::IAutoComplete^ iAutoComplete;
		NativeMethods::IAutoComplete2^ iAutoComplete2;
		NativeMethods::IAutoCompleteDropDown^ iAutoCompleteDropDown; 

	public:
		FormAutoComplete(void);
		void GenerateCandidates(cli::array<String^> ^data, String ^prefix);
	protected:
		~FormAutoComplete();
	private: System::Windows::Forms::TextBox^  textBox1;
	protected: 

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		void InitializeComponent(void);
#pragma endregion
		static String^ GenerateRandomText(Random ^rnd, Char minValue, Char maxValue, int stringLength);
		System::Void FormAutoComplete_Load(System::Object ^sender, System::EventArgs ^e);
		void InitializeEditControl();
		void InitializeAutoComplete();
		System::Void textBox1_TextChanged(System::Object ^sender, System::EventArgs ^e);
	};
}

