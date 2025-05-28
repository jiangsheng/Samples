using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace HtmlEncodingConverter
{
    internal enum MLDETECTCP
    {  // Default setting will be used. 
        MLDETECTCP_NONE = 0,

        // Input stream consists of 7-bit data. 
        MLDETECTCP_7BIT = 1,

        // Input stream consists of 8-bit data. 
        MLDETECTCP_8BIT = 2,

        // Input stream consists of double-byte data. 
        MLDETECTCP_DBCS = 4,

        // Input stream is an HTML page. 
        MLDETECTCP_HTML = 8,

        //Not currently supported. 
        MLDETECTCP_NUMBER = 16
    }
    public struct DetectEncodingInfo
    {
        public UInt32 nLangID;
        public UInt32 nCodePage;
        public Int32 nDocPercent;
        public Int32 nConfidence;
    };

    [ComImport, Guid("275c23e2-3747-11d0-9fea-00aa003f8646")]
    public class MultiLanguage
    {

    }
    // i am not going to have the correct signature for all methods since i don't call them
    // just the one i am interested in
    [Guid("DCCFC164-2B38-11D2-B7EC-00C04F8F5D9A"),
        InterfaceType(ComInterfaceType.InterfaceIsIUnknown)]
    public interface IMultiLanguage2
    {
        void GetNumberOfCodePageInfo();
        void GetCodePageInfo();
        void GetFamilyCodePage();
        void EnumCodePages();
        void GetCharsetInfo();
        void IsConvertible();
        void ConvertString();
        void ConvertStringToUnicode();
        void ConvertStringFromUnicode();
        void ConvertStringReset();
        void GetRfc1766FromLcid();
        void GetLcidFromRfc1766();
        void EnumRfc1766();
        void GetRfc1766Info();
        void CreateConvertCharset();
        void ConvertStringInIStream();
        void ConvertStringToUnicodeEx();
        void ConvertStringFromUnicodeEx();
        void DetectCodepageInIStream();
        void DetectInputCodepage(
            [In] UInt32 dwFlag,
            [In] UInt32 dwPrefWinCodePage,
            [In] IntPtr pSrcStr,
            [In, Out] ref Int32 pcSrcSize,
            [Out] out DetectEncodingInfo lpEncoding,
            [In, Out] ref Int32 pnScores);
        void ValidateCodePage();
        void GetCodePageDescription();
        void IsCodePageInstallable();
        void SetMimeDBSource();
        void GetNumberOfScripts();
        void EnumScripts();
        void ValidateCodePageEx();
    }
}
