#include "StdAfx.h"
#include "BindingListView.h"
namespace AutoCompleteDemo {
	generic<typename T> 
	where T:ref class
	BindingListView<T>::BindingListView(void)
	{
		CommonConstruct();
	}
	generic<typename T> 
	where T:ref class
	BindingListView<T>::BindingListView(List<T>^ list)
	: BindingList<T>(list)
	{
		CommonConstruct();
	}
	generic<typename T> 
	where T:ref class
	void BindingListView<T>::CommonConstruct()
	{
		m_Sorted = false;
		m_Filtered = false;
		m_FilterString = nullptr;
		m_SortProperty = nullptr;
		m_SortDirection =ListSortDirection::Ascending;
		m_SortDescriptions =gcnew ListSortDescriptionCollection();
		m_OriginalCollection = gcnew List<T>();
		// Get the 'shape' of the list. 
        // Only get the public properties marked with Browsable = true.
		cli::array<Attribute^>^ attributes=gcnew cli::array<Attribute^>(1);
		attributes[0]=gcnew BrowsableAttribute(true);
		PropertyDescriptorCollection^ propertyDescriptorCollection = TypeDescriptor::GetProperties(
			T::typeid, attributes
		);

        // Sort the properties.
        propertyDescriptors = propertyDescriptorCollection->Sort();
	}
	generic<typename T> 
	where T:ref class
	int BindingListView<T>::FindCore (PropertyDescriptor^ propertyDescriptor,Object^ key) 
	{
		// Simple iteration:
		for (int i = 0; i < Count; i++)
		{
			T item = this[i];
			if (propertyDescriptor->GetValue(item)->Equals(key))
			{
				return i;
			}
		}
		return -1; // Not found

		// Alternative search implementation
		// using List.FindIndex:
		//Predicate<T> pred = delegate(T item)
		//{
		// if (property.GetValue(item).Equals(key))
		// return true;
		// else
		// return false;
		//};
		//List<T> list = Items as List<T>;
		//if (list == null)
		// return -1;
		//return list.FindIndex(pred);

	}
}