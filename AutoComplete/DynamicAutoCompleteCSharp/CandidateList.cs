using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices.ComTypes;
using System.Collections.Specialized;
using System.Runtime.InteropServices;

namespace DynamicAutoCompleteCSharp
{
    class CandidateList:IEnumString
    {
        private int current;
        private int size;
        
        string[] candidateWords;  
        public CandidateList(string[] data)
        {
            this.candidateWords=new string[1];
            ReplaceCandidateList(data);
        }
        public void ReplaceCandidateList(string[] newCandidateList)
        {
            lock (candidateWords)
            {
                Array.Clear(this.candidateWords, 0, this.size);
                if (this.candidateWords != null)
                {
                    this.candidateWords = newCandidateList;
                }
                this.current = 0;
                this.size = (this.candidateWords == null) ?
                    0 : this.candidateWords.Length;
            }
        }
        #region IEnumString Members

        public void Clone(out IEnumString ppenum)
        {
            CandidateList candidateList = new CandidateList(candidateWords);          
            ppenum = candidateList;
        }

        public int Next(int celt, string[] rgelt, IntPtr pceltFetched)
        {
            lock (candidateWords)
            {
                if (celt < 0 || rgelt==null)
                {
                    return (int)NativeMethods.Win32Error.E_INVALID_ARG;
                }
                int index = 0;
                while ((this.current < this.size) && (celt > 0))
                {
                    rgelt[index++] = this.candidateWords[this.current++];
                    celt--;
                }
                if (pceltFetched != IntPtr.Zero)
                {
                    Marshal.WriteInt32(pceltFetched, index);
                }
                if (celt != 0)
                {
                    return 1;
                }
                return 0;
            }
        }

        public void Reset()
        {
            this.current = 0;
        }

        public int Skip(int celt)
        {
            this.current += celt;
            if (this.current >= this.size)
            {
                return 1;
            }
            return 0;

        }

        #endregion
    }
}
