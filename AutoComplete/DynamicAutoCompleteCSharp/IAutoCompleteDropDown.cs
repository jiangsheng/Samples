using System;
using System.Runtime.InteropServices;
using System.Text;

namespace DynamicAutoCompleteCSharp
{
    internal partial class NativeMethods
    {
        [ComImport]
        [InterfaceType(ComInterfaceType.InterfaceIsIUnknown)]
        [Guid("3CD141F4-3C6A-11d2-BCAA-00C04FD929DB")]
        public interface IAutoCompleteDropDown
        {
            [PreserveSig]
            Int32 GetDropDownStatus(out uint pdwFlags,
                [MarshalAs(UnmanagedType.LPWStr)]  out StringBuilder ppwszString);
            [PreserveSig]
            Int32 ResetEnumerator();
        }
    }
}
