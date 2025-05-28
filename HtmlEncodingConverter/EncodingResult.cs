using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace HtmlEncodingConverter
{
    internal class EncodingResult
    {
        public Encoding DetectedEncoding  { get; set; }
        public string DetectedText { get; set; }

    }
}
