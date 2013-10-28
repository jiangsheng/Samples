#pragma once
using namespace System;
using namespace System::Collections;
using namespace System::Collections::Generic;
using namespace System::ComponentModel;
using namespace System::Reflection;
namespace AutoCompleteDemo {
	generic<typename T> 
	where T:ref class
	ref class SortComparer: System::Collections::Generic::IComparer<T>
	{
	private:
		ListSortDescriptionCollection^ m_SortCollection ;
		PropertyDescriptor^ m_PropDesc ;
		ListSortDirection m_Direction;
		void CommonConstruct()
		{
			m_SortCollection = nullptr;
			m_PropDesc = nullptr;
			m_Direction =ListSortDirection::Ascending;

		}
	public:
		SortComparer(PropertyDescriptor^ propDesc, ListSortDirection direction)
		{
			m_PropDesc = propDesc;
			m_Direction = direction;
		}

		SortComparer(ListSortDescriptionCollection^ sortCollection)
		{
			m_SortCollection = sortCollection;
		}
	
		virtual int Compare(T x, T y) 
		{
			if (m_PropDesc != nullptr) // Simple sort
			{
				Object^ xValue = m_PropDesc->GetValue(x);
				Object^ yValue = m_PropDesc->GetValue(y);
				return CompareValues(xValue, yValue, m_Direction);
			}
			else if (m_SortCollection != nullptr &&
				m_SortCollection->Count > 0)
			{
				return RecursiveCompareInternal(x,y, 0);
			}
			else return 0;
		}
	private:
		int CompareValues(Object^ xValue, Object^ yValue,	ListSortDirection direction)
		{

			int retValue = 0;
			IComparable^ xValueAsIComparable= dynamic_cast<IComparable^>(xValue);
			IComparable^ yValueAsIComparable= dynamic_cast<IComparable^>(yValue);
			if (xValueAsIComparable!=nullptr) // Can ask the x value
			{
				retValue = xValueAsIComparable->CompareTo(yValue);
			}
			else if (yValueAsIComparable!=nullptr) // Can ask the x value
			{
				retValue = yValueAsIComparable->CompareTo(xValue);
			} 
			// not comparable, compare String representations
			else if (!xValue->Equals(yValue))
			{
				retValue = xValue->ToString()->CompareTo(yValue->ToString());
			}
			if (direction == ListSortDirection::Ascending)
			{
				return retValue;
			}
			else
			{
				return retValue * -1;
			}
		}

		int RecursiveCompareInternal(T x, T y, int index)
		{
			if (index >= m_SortCollection->Count)
				return 0; 

			ListSortDescription^ listSortDesc = m_SortCollection[index];
			Object^ xValue = listSortDesc->PropertyDescriptor->GetValue(x);
			Object^ yValue = listSortDesc->PropertyDescriptor->GetValue(y);

			int retValue = CompareValues(xValue,yValue,listSortDesc->SortDirection);
			if (retValue == 0)
			{
				return RecursiveCompareInternal(x,y,++index);
			}
			else
			{
				return retValue;
			}
		}
	};
}