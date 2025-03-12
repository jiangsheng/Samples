#pragma once
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
	ref class CandidateList:public IEnumString
		{
		private:
			int current;
			int size;
			cli::array<String^>^ candidateWords;  
		public :
			CandidateList(cli::array<String^>^ candidateWords)
				{
				this->candidateWords=gcnew cli::array<String^>(1);
				ReplaceCandidateList(candidateWords);
				}

			void ReplaceCandidateList(cli::array<String^>^ newCandidateList)
				{

				lock  lockCandidateWords(candidateWords);
				Array::Clear(this->candidateWords, 0, this->size);

				if (this->candidateWords != nullptr)
					{
					this->candidateWords = newCandidateList;
					}
				this->current = 0;
				this->size = (this->candidateWords == nullptr) ?
					0 : this->candidateWords->Length;

				}
#pragma region IEnumString Members

			virtual void Clone([Out] IEnumString^% ppenum)=IEnumString::Clone
				{
				CandidateList^ candidateList = gcnew CandidateList(candidateWords);
				ppenum = candidateList;
				}

			virtual int Next(int celt, cli::array<String^>^ rgelt, IntPtr pceltFetched)=IEnumString::Next
				{
				lock lockCandidateWords(candidateWords);
					{
					if (celt < 0 || rgelt==nullptr)
						{
						return (int)NativeMethods::Win32Error::E_INVALID_ARG;
						}
					int index = 0;
					while ((this->current < this->size) && (celt > 0))
						{
						rgelt[index++] = this->candidateWords[this->current++];
						celt--;
						}
					if (pceltFetched != IntPtr::Zero)
						{
						Marshal::WriteInt32(pceltFetched, index);
						}
					if (celt != 0)
						{
						return 1;
						}
					return 0;
					}
				}

			virtual void Reset()=IEnumString::Reset
				{
				this->current = 0;
				}

			virtual int Skip(int celt)=IEnumString::Skip
				{
				this->current += celt;
				if (this->current >= this->size)
					{
					return 1;
					}
				return 0;

				}

#pragma endregion
		};
	}
