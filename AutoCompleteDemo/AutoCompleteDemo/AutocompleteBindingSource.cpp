#include "StdAfx.h"
#include "IAutoComplete2.h"
#include "AutocompleteBindingSource.h"

namespace AutoCompleteDemo {
	using namespace System::ComponentModel;
	using namespace System::Reflection;
	AutocompleteBindingSource::AutocompleteBindingSource()
	{
		CommonConstruct();
		InitializeComponent();
	}
	AutocompleteBindingSource::AutocompleteBindingSource(System::ComponentModel::IContainer ^container)
	{
		/// <summary>
		/// Required for Windows.Forms Class Composition Designer support
		/// </summary>
		CommonConstruct();
		container->Add(this);
		InitializeComponent();
	}
	void AutocompleteBindingSource::CommonConstruct()
	{
		this->current=0;
		this->size=0;
		Type^ autoCompleteType = Type::GetTypeFromCLSID(CLSID_AutoComplete);
		try{
			autoComplete2 =(IAutoComplete2^)(Activator::CreateInstance(autoCompleteType));
		}
		catch(Exception^ e)
		{
			Marshal::ReleaseComObject(autoComplete2);
			autoComplete2 = nullptr;
		}
	}
	void AutocompleteBindingSource::BeginInit()
	{
	}
	void AutocompleteBindingSource::EndInit()
	{
		if(BindingSource!=nullptr)
			this->size=BindingSource->Count;
		if(ControlToBind!=nullptr)
		{
			ControlToBind->HandleCreated+=gcnew EventHandler(this,&AutocompleteBindingSource::ControlToBind_HandleCreated);			
			Bind();
		}
			
	}
	void AutocompleteBindingSource::Clone([Out]System::Runtime::InteropServices::ComTypes::IEnumString^% ppenum)
	{
		AutocompleteBindingSource^ newEnum =gcnew AutocompleteBindingSource(this->Container);
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(newEnum))->BeginInit();

		newEnum ->DisplayMember=this->DisplayMember;
		newEnum ->BindingSource=newEnum -> BindingSource;
		newEnum ->ControlToBind=this->ControlToBind;
		newEnum->current=this->current;
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(newEnum))->EndInit();

		ppenum = newEnum;
		
	}
	int AutocompleteBindingSource::Next(
		int celt, [Out, MarshalAs(UnmanagedType::LPArray, ArraySubType=UnmanagedType::LPWStr, SizeParamIndex=0)] array<String^>^ rgelt, IntPtr pceltFetched)
	{
		if (celt < 0)		{
			return E_INVALIDARG;
		}
		int index = 0;
		while ((this->current < this->size) && (celt > 0))
		{
			Object^ item=this->BindingSource->default[this->current];

			bool useDisplayMember=false;

			if(!String::IsNullOrEmpty(DisplayMember))
			{
				ITypedList^ typedList=dynamic_cast<ITypedList^>(this->BindingSource->DataSource);
				if(typedList!=nullptr)
				{
					PropertyDescriptorCollection^ propertyDescriptorCollection=
						typedList->GetItemProperties (nullptr);
					if(propertyDescriptorCollection!=nullptr)
					{
						PropertyDescriptor^ propertyDescriptor=
							propertyDescriptorCollection->default[DisplayMember];
						if(nullptr!=propertyDescriptor)
						{
							rgelt[index] =propertyDescriptor->GetValue(item)->ToString();
							System::Diagnostics::Debug::WriteLine(rgelt[index]);
							useDisplayMember=true;
						}
					}
				}
				else
				{
					ICustomTypeDescriptor^ customTypeDescriptor=dynamic_cast<ICustomTypeDescriptor^>(item);
					if(customTypeDescriptor!=nullptr)
					{
						PropertyDescriptorCollection^ propertyDescriptorCollection=
							customTypeDescriptor->GetProperties();
						if(propertyDescriptorCollection!=nullptr)
						{
							PropertyDescriptor^ propertyDescriptor=propertyDescriptorCollection->default[DisplayMember];
							if(propertyDescriptor!=nullptr)
							{
								rgelt[index] = propertyDescriptor->GetValue(item)->ToString();
								System::Diagnostics::Debug::WriteLine(rgelt[index]);
								useDisplayMember=true;
							}
						}
					}
				}
			}

			if(!useDisplayMember)
			{
				if(item!=nullptr)
				{
					rgelt[index] = item->ToString();
					System::Diagnostics::Debug::WriteLine(rgelt[index]);
				}
			}
			this->current++;
			index++;
			celt--;
		}
		if ((pceltFetched != IntPtr::Zero))
		{
			Marshal::WriteInt32(pceltFetched, index);
		}
		if ((celt != 0))
		{
			return 1;
		}
		return 0;

	}
	void AutocompleteBindingSource::Reset()
	{
		this->current = 0;
		if(BindingSource!=nullptr)
			this->size=BindingSource->Count;
		if (nullptr==this->autoComplete2)
			return ;
		
	}
	int AutocompleteBindingSource::Skip(int celt)
	{
		this->current = (this->current + celt);
		if ((this->current >= this->size))
		{
			return 1;
		}
		return 0;
	}
	bool AutocompleteBindingSource::Bind()
	{
		if (nullptr==this->autoComplete2)
			return false;
		try
		{
			this->autoComplete2->SetOptions((int)ControlToBind->AutoCompleteMode);
			this->autoComplete2->Init(
				HandleRef(ControlToBind,ControlToBind->Handle),
				this,
				String::Empty
				,String::Empty);
			return true;
		}
		catch(Exception^e) 
		{
			return false;
		}

	}
	System::Void AutocompleteBindingSource::ControlToBind_HandleCreated(System::Object ^sender, System::EventArgs ^e)
	{
		Bind();			
	}
}
