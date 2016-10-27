using SelectCustomPlaces.NativeMethods;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace SelectCustomPlaces
{

   
}
class Program
{
    static void Main(string[] args)
    {
        IFileDialog fileDialog= (IFileDialog)
            Activator.CreateInstance(Type.GetTypeFromCLSID(new Guid("DC1C5A9C-E88A-4dde-A5A1-60F82A20AEF7")));
        IShellItem customPlace = null;
        Win32.SHCreateItemFromParsingName(
            System.IO.Directory.GetParent(System.IO.Directory.GetCurrentDirectory()).FullName, IntPtr.Zero,
             typeof(IShellItem).GUID, out customPlace);
        
        fileDialog.AddPlace(customPlace,FDAP.FDAP_TOP);
        fileDialog.SetFolder(customPlace);
        fileDialog.Show(IntPtr.Zero);
    }
}