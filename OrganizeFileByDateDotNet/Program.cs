using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;

namespace OrganizeFileByDateDotNet
{
    internal class Program
    {
        static void Main(string[] args)
        {
            string inputFolder = args.Length > 0 ? args[0] : Directory.GetCurrentDirectory();
            string outputFolder = args.Length >1 ? args[1] : Directory.GetCurrentDirectory();
            if (args.Length > 2)
            {
                Console.WriteLine($"too many arguments! expected [inputfolder] [outputfolder]");
                return;
            }

            var exePath = System.Reflection.Assembly.GetEntryAssembly().Location;
            DirectoryInfo inputDirectoryInfo=new DirectoryInfo(inputFolder);
            foreach (var fileInfo in inputDirectoryInfo.GetFiles())
            {
                if (string.Compare(fileInfo.FullName, exePath, StringComparison.InvariantCultureIgnoreCase) == 0)
                    continue;
                var addedPath=fileInfo.LastWriteTime.ToString("yyyy-MM-dd");
                var targetPath=Path.Combine(outputFolder, addedPath);
                if (!Directory.Exists(targetPath))
                {
                    Directory.CreateDirectory(targetPath);
                }
                targetPath = Path.Combine(targetPath, fileInfo.Name);
                File.Move(fileInfo.FullName, targetPath);
            }
        }
    }
}
