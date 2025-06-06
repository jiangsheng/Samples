#pragma once

        [ComImport]
		[InterfaceType(ComInterfaceType::InterfaceIsIUnknown)]
        [Guid("EAC04BC0-3791-11D2-BB95-0060977B464C")]
         interface  class  IAutoComplete2 /*: IAutoComplete */
        {
            /// <summary>
            /// Initializes the autocomplete object.
            /// </summary>
            [PreserveSig]
            Int32 Init(
                HandleRef hwndEdit,						// Handle to the window for the system edit control that is to 
                // have autocompletion enabled. 
			IEnumString^ punkACL,							// Pointer to the IUnknown interface of the string list object that 
                // is responsible for generating candidates for the completed 
                // string. The object must expose an IEnumString interface. 
                //IntPtr pwszRegKeyPath,
                [MarshalAs(UnmanagedType::LPWStr)]
			String^ pwszRegKeyPath,					// Pointer to an optional null-terminated Unicode string that gives
                // the registry path, including the value name, where the format 
                // string is stored as a REG_SZ value. The autocomplete object 
                // first looks for the path under HKEY_CURRENT_USER . If it fails,
                // it then tries HKEY_LOCAL_MACHINE . For a discussion of the 
                // format string, see the definition of pwszQuickComplete. 
                //IntPtr pwszQuickComplete);
				[MarshalAs(UnmanagedType::LPWStr)]
			String^ pwszQuickComplete) abstract
;				// Pointer to an optional string that specifies the format to be
            // used if the user enters some text and presses CTRL+ENTER. Set
            // this parameter to NULL to disable quick completion. Otherwise, 
            // the autocomplete object treats pwszQuickComplete as a sprintf 
            // format string, and the text in the edit box as its associated 
            // argument, to produce a new string. For example, set 
            // pwszQuickComplete to "http://www. %s.com/". When a user enters
            // "MyURL" into the edit box and presses CTRL+ENTER, the text in 
            // the edit box is updated to "http://www.MyURL.com/". 

            /// <summary>
            /// Enables or disables autocompletion.
            /// </summary>
            [PreserveSig]
            Int32 Enable(
                Int32 fEnable) abstract
;							// Value that is set to TRUE to enable autocompletion, or to 
            // FALSE to disable it. 


            /// <summary>
            /// Sets the current autocomplete options.
            /// </summary>
            [PreserveSig]
            Int32 SetOptions(
                UInt32 dwFlag) abstract
;			// Flags that allow an application to specify autocomplete options. 

            /// <summary>
            /// Retrieves the current autocomplete options.
            /// </summary>
            [PreserveSig]
            Int32 GetOptions(
                [Out] UInt32 % pdwFlag) abstract
;	// that indicate the options that are currently set. 

        };