using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
namespace SelectCustomPlaces.NativeMethods
{
    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Unicode)]
    public struct COMDLG_FILTERSPEC
    {
        public string pszName;
        public string pszSpec;
    }
    
    public enum FDE_OVERWRITE_RESPONSE
    {
        FDEOR_DEFAULT = 0,
        FDEOR_ACCEPT = 0x1,
        FDEOR_REFUSE = 0x2
    }
    public enum FDE_SHAREVIOLATION_RESPONSE
    {
        FDESVR_DEFAULT = 0,
        FDESVR_ACCEPT = 0x1,
        FDESVR_REFUSE = 0x2
    }
    public enum FDAP
    {
        FDAP_BOTTOM = 0,
        FDAP_TOP = 0x1
    }
    enum FILEOPENDIALOGOPTIONS
    {
        FOS_OVERWRITEPROMPT = 0x2,
        FOS_STRICTFILETYPES = 0x4,
        FOS_NOCHANGEDIR = 0x8,
        FOS_PICKFOLDERS = 0x20,
        FOS_FORCEFILESYSTEM = 0x40,
        FOS_ALLNONSTORAGEITEMS = 0x80,
        FOS_NOVALIDATE = 0x100,
        FOS_ALLOWMULTISELECT = 0x200,
        FOS_PATHMUSTEXIST = 0x800,
        FOS_FILEMUSTEXIST = 0x1000,
        FOS_CREATEPROMPT = 0x2000,
        FOS_SHAREAWARE = 0x4000,
        FOS_NOREADONLYRETURN = 0x8000,
        FOS_NOTESTFILECREATE = 0x10000,
        FOS_HIDEMRUPLACES = 0x20000,
        FOS_HIDEPINNEDPLACES = 0x40000,
        FOS_NODEREFERENCELINKS = 0x100000,
        FOS_DONTADDTORECENT = 0x2000000,
        FOS_FORCESHOWHIDDEN = 0x10000000,
        FOS_DEFAULTNOMINIMODE = 0x20000000,
        FOS_FORCEPREVIEWPANEON = 0x40000000
    }
    [ComImport]
    [InterfaceType(ComInterfaceType.InterfaceIsIUnknown)]
    [Guid("42f85136-db7e-439c-85f1-e4075d135fc8")]
    public interface IFileDialog
    {
        [PreserveSig]
        HResult Show(IntPtr hwndParent);
        void SetFileTypes(uint cFileTypes,
            [MarshalAs(UnmanagedType.LPArray, SizeParamIndex = 0)]
            COMDLG_FILTERSPEC[] rgFilterSpec);
        void SetFileTypeIndex(uint iFileType);
        uint GetFileTypeIndex();
        HResult Advise(IFileDialogEvents pfde);
        void Unadvise(ushort dwCookie);
        void SetOptions(ushort fos);
        ushort GetOptions();
        void SetDefaultFolder(IShellItem psi);
        void SetFolder(IShellItem psi);
        IShellItem GetFolder();
        IShellItem GetCurrentSelection();
        void SetFileName(string pszName);
        string GetFileName();
        void SetTitle(string pszTitle);
        void SetOkButtonLabel(string pszText);
        void SetFileNameLabel(string pszLabel);
        IShellItem GetResult();
        void AddPlace(IShellItem psi, FDAP fdap);
        void SetDefaultExtension(string pszDefaultExtension);
        void Close(int hr);
        void SetClientGuid(Guid guid);
        void ClearClientData();
        void SetFilter(IShellItemFilter pFilter);
    }
    [ComImport]
    [InterfaceType(ComInterfaceType.InterfaceIsIUnknown)]
    [Guid("973510db-7d7f-452b-8975-74a85828d354")]
    public interface IFileDialogEvents
    {
        [PreserveSig]
        HResult OnFileOk(IFileDialog pfd);
        [PreserveSig]
        HResult OnFolderChanging(IFileDialog pfd, IShellItem psiFolder);
        [PreserveSig]
        HResult OnFolderChange(IFileDialog pfd);
        [PreserveSig]
        HResult OnSelectionChange(IFileDialog pfd);
        [PreserveSig]
        HResult OnShareViolation(IFileDialog pfd, IShellItem psi,
            out FDE_SHAREVIOLATION_RESPONSE pResponse);
        [PreserveSig]
        HResult OnTypeChange(IFileDialog pfd);
        [PreserveSig]
        HResult OnOverwrite(IFileDialog pfd, IShellItem psi, out FDE_OVERWRITE_RESPONSE pResponse);
    };
    [ComImport]
    [InterfaceType(ComInterfaceType.InterfaceIsIUnknown)]
    [Guid("2659B475-EEB8-48b7-8F07-B378810F48CF")]
    public interface IShellItemFilter
    {
        int IncludeItem(IShellItem psi);
        int GetEnumFlagsForItem(IShellItem psi, out SHCONTF pgrfFlags);
    };

    public enum HResult
    {
        DRAGDROP_S_CANCEL = 0x00040101,
        DRAGDROP_S_DROP = 0x00040100,
        DRAGDROP_S_USEDEFAULTCURSORS = 0x00040102,
        DATA_S_SAMEFORMATETC = 0x00040130,
        S_OK = 0,
        S_FALSE = 1,
        E_NOINTERFACE = unchecked((int)0x80004002),
        E_NOTIMPL = unchecked((int)0x80004001),
        OLE_E_ADVISENOTSUPPORTED = unchecked((int)80040003),
        MK_E_NOOBJECT = unchecked((int)0x800401E5),

    }
    [ComImport]
    [InterfaceType(ComInterfaceType.InterfaceIsIUnknown)]
    [Guid("43826d1e-e718-42ee-bc55-a1e261c37bfe")]
    public interface IShellItem
    {
        IntPtr BindToHandler(IntPtr pbc,
            [MarshalAs(UnmanagedType.LPStruct)]Guid bhid,
            [MarshalAs(UnmanagedType.LPStruct)]Guid riid);

        [PreserveSig]
        HResult GetParent(out IShellItem ppsi);

        IntPtr GetDisplayName(SIGDN sigdnName);

        SFGAO GetAttributes(SFGAO sfgaoMask);

        int Compare(IShellItem psi, SICHINT hint);
    }; public enum SFGAO : uint
    {
        CANCOPY = 0x00000001,
        CANMOVE = 0x00000002,
        CANLINK = 0x00000004,
        STORAGE = 0x00000008,
        CANRENAME = 0x00000010,
        CANDELETE = 0x00000020,
        HASPROPSHEET = 0x00000040,
        DROPTARGET = 0x00000100,
        CAPABILITYMASK = 0x00000177,
        ENCRYPTED = 0x00002000,
        ISSLOW = 0x00004000,
        GHOSTED = 0x00008000,
        LINK = 0x00010000,
        SHARE = 0x00020000,
        READONLY = 0x00040000,
        HIDDEN = 0x00080000,
        DISPLAYATTRMASK = 0x000FC000,
        STREAM = 0x00400000,
        STORAGEANCESTOR = 0x00800000,
        VALIDATE = 0x01000000,
        REMOVABLE = 0x02000000,
        COMPRESSED = 0x04000000,
        BROWSABLE = 0x08000000,
        FILESYSANCESTOR = 0x10000000,
        FOLDER = 0x20000000,
        FILESYSTEM = 0x40000000,
        HASSUBFOLDER = 0x80000000,
        CONTENTSMASK = 0x80000000,
        STORAGECAPMASK = 0x70C50008,
    }

    public enum SHCONTF
    {
        FOLDERS = 0x0020,
        NONFOLDERS = 0x0040,
        INCLUDEHIDDEN = 0x0080,
        INIT_ON_FIRST_NEXT = 0x0100,
        NETPRINTERSRCH = 0x0200,
        SHAREABLE = 0x0400,
        STORAGE = 0x0800
    }
    public enum SIGDN : uint
    {
        NORMALDISPLAY = 0,
        PARENTRELATIVEPARSING = 0x80018001,
        PARENTRELATIVEFORADDRESSBAR = 0x8001c001,
        DESKTOPABSOLUTEPARSING = 0x80028000,
        PARENTRELATIVEEDITING = 0x80031001,
        DESKTOPABSOLUTEEDITING = 0x8004c000,
        FILESYSPATH = 0x80058000,
        URL = 0x80068000
    }

    public enum SICHINT : uint
    {
        DISPLAY = 0x00000000,
        CANONICAL = 0x10000000,
        ALLFIELDS = 0x80000000
    }
    public class Win32
    {
        [DllImport("shell32.dll", CharSet = CharSet.Unicode, PreserveSig = false)]
        public static extern void SHCreateItemFromParsingName(
    [In][MarshalAs(UnmanagedType.LPWStr)] string pszPath,
    [In] IntPtr pbc,
    [In][MarshalAs(UnmanagedType.LPStruct)] Guid riid,
    [Out][MarshalAs(UnmanagedType.Interface, IidParameterIndex = 2)] out IShellItem ppv);
    }
}
