#include "StdAfx.h"
#include "Orders.h"
using namespace AutoCompleteDemo::NorthwindTableAdapters;
using namespace System::Data::OleDb;
using namespace System::IO;
namespace AutoCompleteDemo {
	
	OrderList::OrderList()
	{
	}
	void OrderList::RemoveFilter()
	{
		if (!m_Filtered)
			  return;
		m_FilterString = nullptr;
		m_Filtered = false;
		m_Sorted = false;
		m_SortDescriptions = nullptr;
		m_SortProperty = nullptr;
		Clear();
		OrderList^ newList=OrderFactory::GetOrders(nullptr);
		for each (Order^ item in newList)
		{
			Add(item);
		}
	}

	void OrderList::UpdateFilter()
	{
		Clear();
		OrderList^ newList=OrderFactory::GetOrders(Filter);
		for each (Order^ item in newList)
		{
			Add(item);
		}
	}
	OrderList^ OrderFactory::GetOrders(String^ filter)
	{
		OrderList^ result=gcnew OrderList();
		OrdersTableAdapter^ ordersTableAdapter;
		Northwind::OrdersDataTable^ ordersDataTable;
		try
		{
			ordersTableAdapter=gcnew OrdersTableAdapter();
			OleDbConnection^ connection=ordersTableAdapter->Connection;
			connection->ConnectionString=String::Format(
				L"Provider=Microsoft.Jet.OLEDB.4.0;Data Source={0};Persist Security Info=True",
				Path::Combine(
				Path::GetDirectoryName(
				System::Reflection::Assembly::GetExecutingAssembly()->Location),"Nwind.mdb"));
				;
			if(String::IsNullOrEmpty(filter))
				ordersDataTable=ordersTableAdapter->GetData();
			else
				ordersDataTable=ordersTableAdapter->GetDataByFilter(filter);
			
			for each(Northwind::OrdersRow^ row in ordersDataTable)
			{
				Order^ order=gcnew Order();

				order->CustomerID=row->CustomerID;
				order->EmployeeIDSpecified=!row->IsEmployeeIDNull();
				if(order->EmployeeIDSpecified)
					order->EmployeeID=row->EmployeeID;

				order->Freight=row->Freight;
				order->OrderDateSpecified=!row->IsOrderDateNull();
				if(order->OrderDateSpecified)
					order->OrderDate=row->OrderDate;
				order->OrderID=row->OrderID;
				order->RequiredDateSpecified=!row->IsRequiredDateNull();
				if(order->RequiredDateSpecified)
					order->RequiredDate=row->RequiredDate;
				order->ShipAddress=row->ShipAddress;
				order->ShipCity=row->ShipCity;
				order->ShipCountry=row->ShipCountry;
				order->ShipName=row->ShipName;
				order->ShippedDateSpecified=!row->IsShippedDateNull();
				if(order->ShippedDateSpecified)
					order->RequiredDate=row->ShippedDate;
				order->ShipPostalCode=row->ShipPostalCode;
				order->ShipRegion=row->ShipRegion;
				order->ShipViaSpecified=!row->IsShipViaNull();
				if(order->ShipViaSpecified)
					order->ShipVia=row->ShipVia;				
				result->Add(order);
			}
		}
		finally{
			delete ordersDataTable;
			delete ordersTableAdapter;

		}
		return result;
	}
}