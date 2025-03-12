#include "stdafx.h"
#include "FormAutoComplete.h" 
namespace DynamicAutoCompleteCpp
{
	FormAutoComplete::FormAutoComplete()
	{
		stringLength = 10;
		randomStrings =10000;
		rnd = gcnew Random((int)System::DateTime::Now.Ticks);
		InitializeComponent();
		cli::array<String^>^ data = gcnew cli::array<String^>(randomStrings);
		Debug::WriteLine("FormAutoComplete");
		GenerateCandidates(data,nullptr);
		candidateList= gcnew CandidateList(data);

		//
		//TODO: Add the constructor code here
		//

	}
	void FormAutoComplete::GenerateCandidates(cli::array<String^> ^data, String ^prefix)
	{
		bool addPrefix = !String::IsNullOrEmpty(prefix);
		StringBuilder ^text = gcnew StringBuilder();
		for (int i = 0; i < randomStrings; i++)
		{
			text->Remove(0, text->Length);
			if (addPrefix)
				text->Append(prefix);
			text->Append(GenerateRandomText(rnd, 'a', 'z', stringLength));
			data[i] = text->ToString();
		}
	}
	System::Void DynamicAutoCompleteCpp::FormAutoComplete::textBox1_TextChanged(System::Object ^sender, System::EventArgs ^e)
	{
		Debug::WriteLine("textBox1_TextChanged");
		if (String::IsNullOrEmpty(textBox1->Text))
			return;
		UInt32 status = 0;
		StringBuilder ^text = nullptr;
		try
		{
			iAutoCompleteDropDown->GetDropDownStatus(status, text);
		}
		catch (Exception ^ex)
		{
			Debug::WriteLine(ex->Message);
			return;
		}
		if (text == nullptr)
			text = gcnew StringBuilder(textBox1->Text);
		cli::array<String^> ^data = gcnew cli::array<String^>(randomStrings);
		GenerateCandidates(data, text->ToString());
		candidateList->ReplaceCandidateList(data);
		iAutoCompleteDropDown->ResetEnumerator();
	}

	void DynamicAutoCompleteCpp::FormAutoComplete::InitializeAutoComplete()
	{
		Type ^typeAautoComplete = Type::GetTypeFromCLSID(Guid("{00BB2763-6A77-11D0-A535-00C04FD7D062}"));
		autoComplete = Activator::CreateInstance(typeAautoComplete);
		iAutoComplete = (DynamicAutoCompleteCpp::NativeMethods::IAutoComplete^)autoComplete;
		iAutoComplete2 = (DynamicAutoCompleteCpp::NativeMethods::IAutoComplete2^)autoComplete;
		iAutoCompleteDropDown = (DynamicAutoCompleteCpp::NativeMethods::IAutoCompleteDropDown^)autoComplete;
	}

	void DynamicAutoCompleteCpp::FormAutoComplete::InitializeEditControl()
	{
		AutoCompleteMode autoCompleteMode = textBox1->AutoCompleteMode;
		textBox1->AutoCompleteMode = AutoCompleteMode::None;
		iAutoComplete2->SetOptions((UInt32)autoCompleteMode);
		IEnumString ^iEnumString = candidateList;
		iAutoComplete2->Init(HandleRef(textBox1, textBox1->Handle), iEnumString, String::Empty, String::Empty);
	}

	System::Void DynamicAutoCompleteCpp::FormAutoComplete::FormAutoComplete_Load(System::Object ^sender, System::EventArgs ^e)
	{
		InitializeAutoComplete();
		InitializeEditControl();
	}

	String^ DynamicAutoCompleteCpp::FormAutoComplete::GenerateRandomText(Random ^rnd, Char minValue, Char maxValue, int stringLength)
	{
		System::Text::StringBuilder ^randomText = gcnew System::Text::StringBuilder(stringLength);
		// the range that we are allowed to go above the min value
		int randomRange = maxValue - minValue;
		double rndValue;
		for (int i = 0; i < stringLength; i++)
		{
			rndValue = rnd->NextDouble();
			randomText->Append((Char)(minValue + rndValue * randomRange));
		}
		return randomText->ToString();
	}

	void DynamicAutoCompleteCpp::FormAutoComplete::InitializeComponent(void)
	{
		this->textBox1 = (gcnew System::Windows::Forms::TextBox());
		this->SuspendLayout();
		// 
		// textBox1
		// 
		this->textBox1->AutoCompleteMode = System::Windows::Forms::AutoCompleteMode::Suggest;
		this->textBox1->Location = System::Drawing::Point(-4, -1);
		this->textBox1->Name = L"textBox1";
		this->textBox1->Size = System::Drawing::Size(295, 20);
		this->textBox1->TabIndex = 1;
		this->textBox1->TextChanged += gcnew System::EventHandler(this, & FormAutoComplete::textBox1_TextChanged);
		// 
		// Form1
		// 
		this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
		this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
		this->ClientSize = System::Drawing::Size(292, 23);
		this->Controls->Add(this->textBox1);
		this->Name = L"Form1";
		this->Text = L"Form1";
		this->Load += gcnew System::EventHandler(this, & FormAutoComplete::FormAutoComplete_Load);
		this->ResumeLayout(false);
		this->PerformLayout();
	}

	FormAutoComplete::~FormAutoComplete()
	{
		if (components)
			delete components;
		if(iAutoComplete!=nullptr)
		{
			Marshal::ReleaseComObject(iAutoComplete);
			iAutoComplete=nullptr;
		}
		if(iAutoComplete2!=nullptr)
		{
			Marshal::ReleaseComObject(iAutoComplete2);
			iAutoComplete2=nullptr;
		}
		if(iAutoCompleteDropDown!=nullptr)
		{
			Marshal::ReleaseComObject(iAutoCompleteDropDown);
			iAutoCompleteDropDown=nullptr;
		}
	}

}