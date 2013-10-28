#pragma once
using namespace System;
using namespace System::Collections;
using namespace System::Collections::Generic;
using namespace System::ComponentModel;
using namespace System::Reflection;
using namespace System::Windows::Forms;
#include "SortComparer.h"
namespace AutoCompleteDemo 
{
	generic<typename T> 
	where T:ref class
	ref class BindingListView:BindingList<T>, IBindingListView,IRaiseItemChangedEvents,ITypedList
	{
	public:
		BindingListView(void);
		BindingListView(List<T>^ list);
		void CommonConstruct();
	protected :
		bool m_Sorted ;
		bool m_Filtered ;
		String^ m_FilterString ;
		ListSortDirection m_SortDirection;
		PropertyDescriptor^ m_SortProperty ;
		ListSortDescriptionCollection^ m_SortDescriptions;
		List<T>^ m_OriginalCollection;
		virtual property bool SupportsSearchingCore {
			 bool get() override { return true; }
		}
		virtual  int FindCore (PropertyDescriptor^ propertyDescriptor,Object^ key) override;
		virtual property bool SupportsSortingCore
		{
		  bool get() override  { return true; }
		}
		virtual property bool IsSortedCore
		{
		  bool get() override  { return m_Sorted; }
		}

		virtual property ListSortDirection SortDirectionCore
		{
			ListSortDirection get() override  { return m_SortDirection; }
		}

		virtual property PropertyDescriptor^ SortPropertyCore
		{
			PropertyDescriptor^  get() override  { return m_SortProperty; }
		}
		virtual void ApplySortCore(PropertyDescriptor^ propertyDescriptor,
		  ListSortDirection direction) override
		{
			m_SortDirection = direction;
			m_SortProperty = propertyDescriptor;
			SortComparer<T>^ comparer =
			 gcnew SortComparer<T>(propertyDescriptor,direction);
			ApplySortInternal(comparer);
		}	
		virtual void ApplySortInternal(SortComparer<T>^ comparer)
		{
			if (m_OriginalCollection->Count == 0)
			{
				m_OriginalCollection->AddRange(this);
			}
			List<T>^ listRef = dynamic_cast<List<T>^>(Items);
			if (listRef == nullptr)	return;
			listRef->Sort(comparer);
			m_Sorted = true;
			OnListChanged(gcnew ListChangedEventArgs(ListChangedType::Reset, -1));
		}
		virtual void RemoveSortCore()override
		{
			if (!m_Sorted)
				return;  
			Clear();
			for each (T item in m_OriginalCollection)
			{
				Add(item);
			}
			m_OriginalCollection->Clear();
			m_SortProperty = nullptr;
			m_SortDescriptions = nullptr;
			m_Sorted = false;
		}

		virtual void ApplySort(ListSortDescriptionCollection^ sorts)
			=IBindingListView::ApplySort
		{
			m_SortProperty = nullptr;
			m_SortDescriptions = sorts;
			SortComparer<T>^ comparer = gcnew SortComparer<T>(sorts);
			ApplySortInternal(comparer);
		}

		property String^ Filter
		{
			virtual String^ get()=IBindingListView::Filter::get
			{
				return m_FilterString;
			}
			virtual void set(String^ value)=IBindingListView::Filter::set
			{
				m_FilterString = value;
				m_Filtered = true;
				UpdateFilter();
			}
		}

		virtual void RemoveFilter()=IBindingListView::RemoveFilter
		{
		   if (!m_Filtered)
			  return;
		   m_FilterString = nullptr;
		   m_Filtered = false;
		   m_Sorted = false;
		   m_SortDescriptions = nullptr;
		   m_SortProperty = nullptr;
		   Clear();
		   for each (T item in m_OriginalCollection)
		   {
			 Add(item);
		   }
		   m_OriginalCollection->Clear();
		}
		property ListSortDescriptionCollection^ SortDescriptions
		{
			virtual ListSortDescriptionCollection^ get()=IBindingListView::SortDescriptions::get
			{
				return m_SortDescriptions;
			}
		}

		property bool SupportsAdvancedSorting
		{
			virtual bool get()=IBindingListView::SupportsAdvancedSorting::get
			{
				return true;
			}
		}

		property bool SupportsFiltering
		{
			virtual bool get()=IBindingListView::SupportsFiltering::get
			{
				return true;
			}
		}

		virtual void UpdateFilter()
		{
			int equalsPos = m_FilterString->IndexOf('=');
			// Get property name
			String^ propName = m_FilterString->Substring(0,equalsPos)->Trim();
			// Get filter criteria
			String^ criteria = m_FilterString->Substring(equalsPos+1,
			 m_FilterString->Length - equalsPos - 1)->Trim();
			// Strip leading and trailing quotes
			criteria = criteria->Substring(1, criteria->Length - 2);
			// Get a property descriptor for the filter property
			PropertyDescriptor^ propDesc =
			  TypeDescriptor::GetProperties(T::typeid)[propName];
			if (m_OriginalCollection->Count == 0)
			{
				m_OriginalCollection->AddRange(this);
			}
			List<T>^ currentCollection = gcnew List<T>(this);
			Clear();
			for each (T item in currentCollection)
			{
				Object^ value = propDesc->GetValue(item);
				if (value->ToString() == criteria)
				{
				   Add(item);      
				}
			}
		}

		property bool AllowNew
		{
			virtual bool get()=IBindingList::AllowNew::get
			{
				return CheckReadOnly();
			}
		}

		property bool AllowRemove
		{
			virtual bool get()=IBindingList::AllowRemove::get
			{
				return CheckReadOnly();
			}
		}

		private:
			bool CheckReadOnly()
		{
			if (m_Sorted || m_Filtered)
			{
			   return false;
			}
			else
			{
			  return true;
			}
		}

		protected:
			virtual void InsertItem(int index, T item)override
		{
			for each (PropertyDescriptor^ propDesc in
			   TypeDescriptor::GetProperties(item))
			{
				  if (propDesc->SupportsChangeEvents)
				  {
					  propDesc->AddValueChanged(item, 
						  gcnew EventHandler(this,&BindingListView<T>::OnItemChanged));
				  }
			}
			BindingList::InsertItem(index, item);
		}

		virtual void RemoveItem(int index)override
		{
			T item = Items[index];
			PropertyDescriptorCollection^ propDescs =
				TypeDescriptor::GetProperties(item);
			for each (PropertyDescriptor^ propDesc in propDescs)
			{
				if (propDesc->SupportsChangeEvents)
				{
					propDesc->RemoveValueChanged(item,
						gcnew EventHandler(this,&BindingListView<T>::OnItemChanged));
				}
			}
			BindingList::RemoveItem(index);
		}

		void OnItemChanged(Object^ sender, EventArgs^ args)
		{
			int index = Items->IndexOf((T)sender);
			OnListChanged(gcnew ListChangedEventArgs(
				ListChangedType::ItemChanged, index));
		}

		property bool RaisesItemChangedEvents
		{
			virtual bool get()=IRaiseItemChangedEvents::RaisesItemChangedEvents::get
			{
			   return true; 
			}
		 
		}
		typedef cli::array<PropertyDescriptor^>^ ListAccessors;
		[NonSerialized()]
		PropertyDescriptorCollection^ propertyDescriptors;
		public:

		virtual PropertyDescriptorCollection^ GetItemProperties(
			ListAccessors listAccessors)
		{
			PropertyDescriptorCollection^ propertyDescriptorCollection = nullptr;

            if (nullptr == listAccessors)
            {
                // Return properties in sort order.
                propertyDescriptorCollection = propertyDescriptors;
            }
            else
            {
                // Return child list shape.
				propertyDescriptorCollection = ListBindingHelper::GetListItemProperties(
					listAccessors[0]->PropertyType);
            }

            return propertyDescriptorCollection;
		}
		virtual String^ GetListName(ListAccessors listAccessors)
		{
			return T::typeid->Name;
		}
	};
};