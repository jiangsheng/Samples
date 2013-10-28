#pragma once

namespace AutoCompleteDemo {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for FormTest
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class FormTest : public System::Windows::Forms::Form
	{
	public:
		FormTest(void);
	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~FormTest();
	private: System::Windows::Forms::BindingSource^  bindingSourceAutoComplete;
	protected: 
	private: System::Windows::Forms::TextBox^  textBoxDemo;
	private: System::Data::DataSet^  dataSetDemo;
	private: System::Data::DataTable^  dataTableAutoCompleteSource;
	private: System::Data::DataColumn^  dataColumn1;
	private: AutoCompleteDemo::AutocompleteBindingSource^  autocompleteBindingSource1;

	private: System::ComponentModel::IContainer^  components;

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void);
#pragma endregion
		//BindingEnumString^ bindingEnumString;
		//void Bind();
		void PopulateRandomData();
		private: System::Void FormTest_Load(System::Object^  sender, System::EventArgs^  e) ;
		private: System::Void textBoxDemo_TextChanged(System::Object^  sender, System::EventArgs^  e) ;
	};
}

