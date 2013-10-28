#include "stdafx.h"
#include "IAutoComplete2.h"
#include "AutocompleteBindingSource.h"
#include "FormTest.h"
#include "datatypes\northwind.h"
#include "Orders.h"
namespace AutoCompleteDemo {
		
	using namespace System::IO;
	using namespace System::Text;
	using namespace System::Security::Cryptography;
	FormTest::FormTest(void)
	{
		InitializeComponent();
		//
		//TODO: Add the constructor code here
		//
//			bindingEnumString=gcnew BindingEnumString(this->bindingSourceAutoComplete);
		//bindingEnumString->DisplayMember="Text";
	}
	FormTest::~FormTest()
	{
		if (components)
		{
			delete components;
		}
	}
	#pragma region Windows Form Designer generated code
	void FormTest::InitializeComponent()
	{
		this->components = (gcnew System::ComponentModel::Container());
		this->bindingSourceAutoComplete = (gcnew System::Windows::Forms::BindingSource(this->components));
		this->dataSetDemo = (gcnew System::Data::DataSet());
		this->dataTableAutoCompleteSource = (gcnew System::Data::DataTable());
		this->dataColumn1 = (gcnew System::Data::DataColumn());
		this->textBoxDemo = (gcnew System::Windows::Forms::TextBox());
		this->autocompleteBindingSource1 = (gcnew AutoCompleteDemo::AutocompleteBindingSource(this->components));
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->bindingSourceAutoComplete))->BeginInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataSetDemo))->BeginInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataTableAutoCompleteSource))->BeginInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->autocompleteBindingSource1))->BeginInit();
		this->SuspendLayout();
		// 
		// bindingSourceAutoComplete
		// 
		this->bindingSourceAutoComplete->DataSource = AutoCompleteDemo::DataTypes::NorthwindOrders::typeid;
		// 
		// dataSetDemo
		// 
		this->dataSetDemo->DataSetName = L"NewDataSet";
		this->dataSetDemo->Tables->AddRange(gcnew cli::array< System::Data::DataTable^  >(1) {this->dataTableAutoCompleteSource});
		// 
		// dataTableAutoCompleteSource
		// 
		this->dataTableAutoCompleteSource->Columns->AddRange(gcnew cli::array< System::Data::DataColumn^  >(1) {this->dataColumn1});
		cli::array< System::String^ >^ __mcTemp__1 = gcnew cli::array< System::String^  >(1) {L"Text"};
		this->dataTableAutoCompleteSource->Constraints->AddRange(gcnew cli::array< System::Data::Constraint^  >(1) {(gcnew System::Data::UniqueConstraint(L"Constraint1", 
			__mcTemp__1, true))});
		this->dataTableAutoCompleteSource->PrimaryKey = gcnew cli::array< System::Data::DataColumn^  >(1) {this->dataColumn1};
		this->dataTableAutoCompleteSource->TableName = L"AutoCompleteSource";
		// 
		// dataColumn1
		// 
		this->dataColumn1->AllowDBNull = false;
		this->dataColumn1->ColumnName = L"Text";
		// 
		// textBoxDemo
		// 
		this->textBoxDemo->AutoCompleteMode = System::Windows::Forms::AutoCompleteMode::Suggest;
		this->textBoxDemo->Location = System::Drawing::Point(3, 12);
		this->textBoxDemo->Name = L"textBoxDemo";
		this->textBoxDemo->Size = System::Drawing::Size(277, 20);
		this->textBoxDemo->TabIndex = 0;
		this->textBoxDemo->TextChanged += gcnew System::EventHandler(this, &FormTest::textBoxDemo_TextChanged);
		// 
		// autocompleteBindingSource1
		// 
		this->autocompleteBindingSource1->BindingSource = this->bindingSourceAutoComplete;
		this->autocompleteBindingSource1->ControlToBind = this->textBoxDemo;
		this->autocompleteBindingSource1->DisplayMember = L"Text";
		// 
		// FormTest
		// 
		this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
		this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
		this->ClientSize = System::Drawing::Size(292, 273);
		this->Controls->Add(this->textBoxDemo);
		this->Name = L"FormTest";
		this->Text = L"FormTest";
		this->Load += gcnew System::EventHandler(this, &FormTest::FormTest_Load);
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->bindingSourceAutoComplete))->EndInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataSetDemo))->EndInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataTableAutoCompleteSource))->EndInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->autocompleteBindingSource1))->EndInit();
		this->ResumeLayout(false);
		this->PerformLayout();

	}
	#pragma endregion
	System::Void FormTest::FormTest_Load(System::Object^  sender, System::EventArgs^  e) 
	{
		OrderList^ orderList=OrderFactory::GetOrders(nullptr);
		bindingSourceAutoComplete->DataSource=orderList;
		//PopulateRandomData();			
	}
	void FormTest::PopulateRandomData()
	{
		DataTable^ dataTable=dataSetDemo->Tables[0];

		RNGCryptoServiceProvider^ cryptoProvider =
			gcnew RNGCryptoServiceProvider();

		array<Byte>^ randomBytes = gcnew array<Byte>(15);
		for(int i=0;i<1000;i++)
		{
			cryptoProvider->GetNonZeroBytes(randomBytes);
			DataRow^ dataRow=dataTable->NewRow();
			dataRow->default[0]=System::Convert::ToBase64String(randomBytes);
			String^ s=dataRow->default["Text"]->ToString();
			dataTable->Rows->Add(dataRow);
		}
	}

	System::Void FormTest::textBoxDemo_TextChanged(System::Object^  sender, System::EventArgs^  e) 
	{
		static bool inThisFunction=false;
		if(!inThisFunction)
		{
			inThisFunction=true;
			if(String::IsNullOrEmpty(textBoxDemo->Text))
			{
				bindingSourceAutoComplete->Filter=nullptr;
			}
			else
			{
				/*System::String^ addText=textBoxDemo->Text+"og/NextElement";
				dataSetDemo->Tables[0]->DefaultView->Sort="Text";
				if(dataSetDemo->Tables[0]->DefaultView->FindRows(addText)->Length==0)
				{
					System::Data::DataRow^ row=dataSetDemo->Tables[0]->NewRow();
					row->default[0]=addText;
					dataSetDemo->Tables[0]->Rows->Add(row);
				}*/
				bindingSourceAutoComplete->Filter=textBoxDemo->Text;
				/*bindingSourceAutoComplete->Filter=
					String::Format("{0} LIKE '{1}%'"
					,dataSetDemo->Tables[0]->Columns[0]->Caption
					,textBoxDemo->Text);*/
			}
			
			if(textBoxDemo->SelectionStart>0)
			{
				autocompleteBindingSource1->Reset();
				autocompleteBindingSource1->Bind();
				String^ text=textBoxDemo->Text;
				int selectionStart=textBoxDemo->SelectionStart;
				int selectionLength=textBoxDemo->SelectionLength;
				textBoxDemo->SelectionStart=0;
				textBoxDemo->SelectionLength=0;
				textBoxDemo->SelectAll();			
				System::Windows::Forms::SendKeys::SendWait("{BACKSPACE}");
				textBoxDemo->Text=text;
				textBoxDemo->SelectionStart=selectionStart-1;
				textBoxDemo->SelectionLength=selectionLength+1;
				System::Windows::Forms::SendKeys::SendWait(textBoxDemo->SelectedText);
			}
			inThisFunction=false;
		}
	}
}