using MSHTML;
using System.Diagnostics;
using System.Runtime.InteropServices;

namespace WinFormsHTMLSegments
{
    internal class ComHelper
    {
        public static object? QueryService(object source, Guid serviceID, Guid requestedIID)
        {
            // cast the source object to a service-provider
            UCOMIServiceProvider? sp =
                source as UCOMIServiceProvider;

            // if the source object isn't a service provider return null
            if (sp == null)
            {
                return null;
            }

            // query for the requested interface
            object? obj = null;
            try
            {
                sp.QueryService(ref serviceID, ref requestedIID, out obj);
            }
            catch (COMException e)
            {
                Debug.Assert(false, "QueryService failure: " + e.Message);
            }

            // return interface (returns null if interface not found)
            return obj;
        }
    }
}