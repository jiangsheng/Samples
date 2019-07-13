using Microsoft.WindowsAPICodePack.Shell;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ComparePaths
{
    class Program
    {
        static void Main(string[] args)
        {
            string path1 = "C:\\PROGRA~1";
            string path2 = "C:\\Program Files";
            ShellFileSystemFolder folder1 = ShellFileSystemFolder.FromFolderPath(path1);
            ShellFileSystemFolder folder2 = ShellFileSystemFolder.FromFolderPath(path2);
            Console.WriteLine(string.Format("{0} and {1} are {2}",
                path1,
                path2,
                folder1.Equals(folder2) ?"the same ":"not the same"));
        }
    }
}
