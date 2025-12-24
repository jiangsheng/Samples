using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace OutlookSenderStatistics
{
    public class MailCountBySender
    {
        public string? Sender { get; set; }
        public int MailCount { get; set; }
        public MailCountBySender(string sender, int mailCount)
        {
            Sender = sender;
            MailCount = mailCount;
        }
        public MailCountBySender() { }
    }
}
