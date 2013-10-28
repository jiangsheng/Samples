#pragma once

#include "datatypes\northwind.h"
#include "BindingListView.h"
namespace AutoCompleteDemo {
	ref class OrderFactory;
	ref class Order:AutoCompleteDemo::DataTypes::NorthwindOrders
	{
	public:
		property String^ Text
		{
			String^ get()
			{
				return String::Format("{0} {1} {2} {3} {4}",
					ShipAddress,ShipCity,ShipRegion,ShipCountry,ShipPostalCode);
			}
		}
	};
	
	ref class OrderList:BindingListView<
		Order^>
		 
	{
	public:
		OrderList();		
		virtual void RemoveFilter() override;
		virtual void UpdateFilter() override;
	};

	ref class OrderFactory
	{
	public: 
		static OrderList^ GetOrders(String^ filter);
	};
}