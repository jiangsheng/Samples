#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Diagnostics;


namespace AutoCompleteDemo {

	using namespace System;
	using namespace System::Runtime::InteropServices;
	using namespace System::Windows::Forms;
	/// <summary>
	/// Summary for AutocompleteBindingSource
	/// </summary>
	public ref class AutocompleteBindingSource :  public System::ComponentModel::Component,public System::Runtime::InteropServices::ComTypes::IEnumString,public ISupportInitialize
	{
	public:
		AutocompleteBindingSource(void);
		AutocompleteBindingSource(System::ComponentModel::IContainer ^container);
		void CommonConstruct();

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~AutocompleteBindingSource()
		{
			if (components)
			{
				delete components;
			}
			if(autoComplete2 !=nullptr)
			{
				Marshal::ReleaseComObject(autoComplete2 );
				autoComplete2 =nullptr;
			}
		}

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			components = gcnew System::ComponentModel::Container();
		}
#pragma endregion
	
#pragma region IEnumString methods
public:
		virtual void Clone(System::Runtime::InteropServices::ComTypes::IEnumString^% ppenum);		
		virtual int Next(int celt,
			array<String^>^ rgelt, IntPtr pceltFetched);
		virtual void Reset();		
		virtual int Skip(int celt) ;
#pragma endregion		
		
#pragma region ISupportInitialize methods
public:
		virtual Void BeginInit();
		virtual Void EndInit();
#pragma endregion		
#pragma region Properties
public:
		property String^ DisplayMember;
		property BindingSource^ BindingSource;
		property TextBox^ ControlToBind;
#pragma endregion	
#pragma region Methods
public :
		bool Bind();
#pragma endregion	
private:
		//bool Bind(HandleRef edit, System::UInt32 options);
		int current;
		int size;
		static const Guid CLSID_AutoComplete =Guid("{00BB2763-6A77-11D0-A535-00C04FD7D062}"); 
		IAutoComplete2^ autoComplete2;
		private: System::Void ControlToBind_HandleCreated(System::Object^  sender, System::EventArgs^  e) ;

	};
}
