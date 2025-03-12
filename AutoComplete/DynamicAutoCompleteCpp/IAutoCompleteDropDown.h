#pragma once
[ComImport]
[InterfaceType(ComInterfaceType::InterfaceIsIUnknown)]
[Guid("3CD141F4-3C6A-11d2-BCAA-00C04FD929DB")]
interface class  IAutoCompleteDropDown
	{
	[PreserveSig]
	Int32 GetDropDownStatus([Out] System::UInt32% pdwFlags, 
		[Out][MarshalAs(UnmanagedType::LPWStr)] StringBuilder^ ppwszString);
	[PreserveSig]
	Int32 ResetEnumerator();
	};
