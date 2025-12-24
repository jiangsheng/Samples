using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace OutlookSenderStatistics
{
    public class InboxSelection
    {
        public bool IsSelected { get; set; }
        public string? Folder { get; set; }
        public string? EntryId{ get; set; }
    }
}
