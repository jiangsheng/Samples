﻿#pragma once

#using <mscorlib.dll>
#using <System.dll>
#using <System.Xml.dll>

using namespace System::Security::Permissions;
[assembly:SecurityPermissionAttribute(SecurityAction::RequestMinimum, SkipVerification=false)];
// 
// This source code was auto-generated by xsd, Version=2.0.50727.42.
// 
namespace AutoCompleteDemo {
    namespace DataTypes {
    using namespace System::Xml::Serialization;
    
    using namespace System;
    ref class Northwind;
    ref class NorthwindOrders;
    
    
    /// <remarks/>
    [System::CodeDom::Compiler::GeneratedCodeAttribute(L"xsd", L"2.0.50727.42"), 
    System::SerializableAttribute, 
    System::Diagnostics::DebuggerStepThroughAttribute, 
    System::ComponentModel::DesignerCategoryAttribute(L"code"), 
    System::Xml::Serialization::XmlTypeAttribute(AnonymousType=true, Namespace=L"http://tempuri.org/Northwind.xsd"), 
    System::Xml::Serialization::XmlRootAttribute(Namespace=L"http://tempuri.org/Northwind.xsd", IsNullable=false)]
    public ref class Northwind {
        
        private: cli::array< AutoCompleteDemo::DataTypes::NorthwindOrders^  >^  itemsField;
        
        /// <remarks/>
        public: [System::Xml::Serialization::XmlElementAttribute(L"Orders")]
        property cli::array< AutoCompleteDemo::DataTypes::NorthwindOrders^  >^  Items {
            cli::array< AutoCompleteDemo::DataTypes::NorthwindOrders^  >^  get();
            System::Void set(cli::array< AutoCompleteDemo::DataTypes::NorthwindOrders^  >^  value);
        }
    };
    
    /// <remarks/>
    [System::CodeDom::Compiler::GeneratedCodeAttribute(L"xsd", L"2.0.50727.42"), 
    System::SerializableAttribute, 
    System::Diagnostics::DebuggerStepThroughAttribute, 
    System::ComponentModel::DesignerCategoryAttribute(L"code"), 
    System::Xml::Serialization::XmlTypeAttribute(AnonymousType=true, Namespace=L"http://tempuri.org/Northwind.xsd")]
    public ref class NorthwindOrders {
        
        private: System::Int32 orderIDField;
        
        private: System::String^  customerIDField;
        
        private: System::Int32 employeeIDField;
        
        private: System::Boolean employeeIDFieldSpecified;
        
        private: System::DateTime orderDateField;
        
        private: System::Boolean orderDateFieldSpecified;
        
        private: System::DateTime requiredDateField;
        
        private: System::Boolean requiredDateFieldSpecified;
        
        private: System::DateTime shippedDateField;
        
        private: System::Boolean shippedDateFieldSpecified;
        
        private: System::Int32 shipViaField;
        
        private: System::Boolean shipViaFieldSpecified;
        
        private: System::Decimal freightField;
        
        private: System::Boolean freightFieldSpecified;
        
        private: System::String^  shipNameField;
        
        private: System::String^  shipAddressField;
        
        private: System::String^  shipCityField;
        
        private: System::String^  shipRegionField;
        
        private: System::String^  shipPostalCodeField;
        
        private: System::String^  shipCountryField;
        
        /// <remarks/>
        public: property System::Int32 OrderID {
            System::Int32 get();
            System::Void set(System::Int32 value);
        }
        
        /// <remarks/>
        public: property System::String^  CustomerID {
            System::String^  get();
            System::Void set(System::String^  value);
        }
        
        /// <remarks/>
        public: property System::Int32 EmployeeID {
            System::Int32 get();
            System::Void set(System::Int32 value);
        }
        
        /// <remarks/>
        public: [System::Xml::Serialization::XmlIgnoreAttribute]
        property System::Boolean EmployeeIDSpecified {
            System::Boolean get();
            System::Void set(System::Boolean value);
        }
        
        /// <remarks/>
        public: property System::DateTime OrderDate {
            System::DateTime get();
            System::Void set(System::DateTime value);
        }
        
        /// <remarks/>
        public: [System::Xml::Serialization::XmlIgnoreAttribute]
        property System::Boolean OrderDateSpecified {
            System::Boolean get();
            System::Void set(System::Boolean value);
        }
        
        /// <remarks/>
        public: property System::DateTime RequiredDate {
            System::DateTime get();
            System::Void set(System::DateTime value);
        }
        
        /// <remarks/>
        public: [System::Xml::Serialization::XmlIgnoreAttribute]
        property System::Boolean RequiredDateSpecified {
            System::Boolean get();
            System::Void set(System::Boolean value);
        }
        
        /// <remarks/>
        public: property System::DateTime ShippedDate {
            System::DateTime get();
            System::Void set(System::DateTime value);
        }
        
        /// <remarks/>
        public: [System::Xml::Serialization::XmlIgnoreAttribute]
        property System::Boolean ShippedDateSpecified {
            System::Boolean get();
            System::Void set(System::Boolean value);
        }
        
        /// <remarks/>
        public: property System::Int32 ShipVia {
            System::Int32 get();
            System::Void set(System::Int32 value);
        }
        
        /// <remarks/>
        public: [System::Xml::Serialization::XmlIgnoreAttribute]
        property System::Boolean ShipViaSpecified {
            System::Boolean get();
            System::Void set(System::Boolean value);
        }
        
        /// <remarks/>
        public: property System::Decimal Freight {
            System::Decimal get();
            System::Void set(System::Decimal value);
        }
        
        /// <remarks/>
        public: [System::Xml::Serialization::XmlIgnoreAttribute]
        property System::Boolean FreightSpecified {
            System::Boolean get();
            System::Void set(System::Boolean value);
        }
        
        /// <remarks/>
        public: property System::String^  ShipName {
            System::String^  get();
            System::Void set(System::String^  value);
        }
        
        /// <remarks/>
        public: property System::String^  ShipAddress {
            System::String^  get();
            System::Void set(System::String^  value);
        }
        
        /// <remarks/>
        public: property System::String^  ShipCity {
            System::String^  get();
            System::Void set(System::String^  value);
        }
        
        /// <remarks/>
        public: property System::String^  ShipRegion {
            System::String^  get();
            System::Void set(System::String^  value);
        }
        
        /// <remarks/>
        public: property System::String^  ShipPostalCode {
            System::String^  get();
            System::Void set(System::String^  value);
        }
        
        /// <remarks/>
        public: property System::String^  ShipCountry {
            System::String^  get();
            System::Void set(System::String^  value);
        }
    };
    }
}
namespace AutoCompleteDemo {
    namespace DataTypes {
    
    
    inline cli::array< AutoCompleteDemo::DataTypes::NorthwindOrders^  >^  Northwind::Items::get() {
        return this->itemsField;
    }
    inline System::Void Northwind::Items::set(cli::array< AutoCompleteDemo::DataTypes::NorthwindOrders^  >^  value) {
        this->itemsField = value;
    }
    
    
    inline System::Int32 NorthwindOrders::OrderID::get() {
        return this->orderIDField;
    }
    inline System::Void NorthwindOrders::OrderID::set(System::Int32 value) {
        this->orderIDField = value;
    }
    
    inline System::String^  NorthwindOrders::CustomerID::get() {
        return this->customerIDField;
    }
    inline System::Void NorthwindOrders::CustomerID::set(System::String^  value) {
        this->customerIDField = value;
    }
    
    inline System::Int32 NorthwindOrders::EmployeeID::get() {
        return this->employeeIDField;
    }
    inline System::Void NorthwindOrders::EmployeeID::set(System::Int32 value) {
        this->employeeIDField = value;
    }
    
    inline System::Boolean NorthwindOrders::EmployeeIDSpecified::get() {
        return this->employeeIDFieldSpecified;
    }
    inline System::Void NorthwindOrders::EmployeeIDSpecified::set(System::Boolean value) {
        this->employeeIDFieldSpecified = value;
    }
    
    inline System::DateTime NorthwindOrders::OrderDate::get() {
        return this->orderDateField;
    }
    inline System::Void NorthwindOrders::OrderDate::set(System::DateTime value) {
        this->orderDateField = value;
    }
    
    inline System::Boolean NorthwindOrders::OrderDateSpecified::get() {
        return this->orderDateFieldSpecified;
    }
    inline System::Void NorthwindOrders::OrderDateSpecified::set(System::Boolean value) {
        this->orderDateFieldSpecified = value;
    }
    
    inline System::DateTime NorthwindOrders::RequiredDate::get() {
        return this->requiredDateField;
    }
    inline System::Void NorthwindOrders::RequiredDate::set(System::DateTime value) {
        this->requiredDateField = value;
    }
    
    inline System::Boolean NorthwindOrders::RequiredDateSpecified::get() {
        return this->requiredDateFieldSpecified;
    }
    inline System::Void NorthwindOrders::RequiredDateSpecified::set(System::Boolean value) {
        this->requiredDateFieldSpecified = value;
    }
    
    inline System::DateTime NorthwindOrders::ShippedDate::get() {
        return this->shippedDateField;
    }
    inline System::Void NorthwindOrders::ShippedDate::set(System::DateTime value) {
        this->shippedDateField = value;
    }
    
    inline System::Boolean NorthwindOrders::ShippedDateSpecified::get() {
        return this->shippedDateFieldSpecified;
    }
    inline System::Void NorthwindOrders::ShippedDateSpecified::set(System::Boolean value) {
        this->shippedDateFieldSpecified = value;
    }
    
    inline System::Int32 NorthwindOrders::ShipVia::get() {
        return this->shipViaField;
    }
    inline System::Void NorthwindOrders::ShipVia::set(System::Int32 value) {
        this->shipViaField = value;
    }
    
    inline System::Boolean NorthwindOrders::ShipViaSpecified::get() {
        return this->shipViaFieldSpecified;
    }
    inline System::Void NorthwindOrders::ShipViaSpecified::set(System::Boolean value) {
        this->shipViaFieldSpecified = value;
    }
    
    inline System::Decimal NorthwindOrders::Freight::get() {
        return this->freightField;
    }
    inline System::Void NorthwindOrders::Freight::set(System::Decimal value) {
        this->freightField = value;
    }
    
    inline System::Boolean NorthwindOrders::FreightSpecified::get() {
        return this->freightFieldSpecified;
    }
    inline System::Void NorthwindOrders::FreightSpecified::set(System::Boolean value) {
        this->freightFieldSpecified = value;
    }
    
    inline System::String^  NorthwindOrders::ShipName::get() {
        return this->shipNameField;
    }
    inline System::Void NorthwindOrders::ShipName::set(System::String^  value) {
        this->shipNameField = value;
    }
    
    inline System::String^  NorthwindOrders::ShipAddress::get() {
        return this->shipAddressField;
    }
    inline System::Void NorthwindOrders::ShipAddress::set(System::String^  value) {
        this->shipAddressField = value;
    }
    
    inline System::String^  NorthwindOrders::ShipCity::get() {
        return this->shipCityField;
    }
    inline System::Void NorthwindOrders::ShipCity::set(System::String^  value) {
        this->shipCityField = value;
    }
    
    inline System::String^  NorthwindOrders::ShipRegion::get() {
        return this->shipRegionField;
    }
    inline System::Void NorthwindOrders::ShipRegion::set(System::String^  value) {
        this->shipRegionField = value;
    }
    
    inline System::String^  NorthwindOrders::ShipPostalCode::get() {
        return this->shipPostalCodeField;
    }
    inline System::Void NorthwindOrders::ShipPostalCode::set(System::String^  value) {
        this->shipPostalCodeField = value;
    }
    
    inline System::String^  NorthwindOrders::ShipCountry::get() {
        return this->shipCountryField;
    }
    inline System::Void NorthwindOrders::ShipCountry::set(System::String^  value) {
        this->shipCountryField = value;
    }
    }
}