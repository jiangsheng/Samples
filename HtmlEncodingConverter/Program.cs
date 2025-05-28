using Config.Net;
using OpenCCNET;
using System.IO;
using System.Runtime.InteropServices;
using System.Text;
using static System.Formats.Asn1.AsnWriter;

namespace HtmlEncodingConverter
{
    internal class Program
    {
        static void Main(string[] args)
        {
            ZhConverter.Initialize();
            System.Text.Encoding.RegisterProvider(CodePagesEncodingProvider.Instance);

            IMySettings settings = LoadAndConfirmSettings();
            if (!Directory.Exists(settings.InputFolder))
            {
                Console.WriteLine($"Input folder does not exist: {settings.InputFolder}");
                return;
            }
            var sourceEncoding= System.Text.Encoding.GetEncoding(settings.FromEncoding);
            var targetEncoding = System.Text.Encoding.GetEncoding(settings.ToEncoding);
            var files = Directory.GetFiles(settings.InputFolder, $"*.{settings.FileExtension}", SearchOption.TopDirectoryOnly);
            foreach (var file in files)
            {
                EncodingResult? encodingResult = DetectEncoding(file, settings);
                if (encodingResult == null)
                {
                    Console.WriteLine($"Could not detect encoding for file: {file}");
                    continue;
                }
                var detectedEncoding = encodingResult.DetectedEncoding.HeaderName;
                Console.WriteLine($"Detected Encoding: {detectedEncoding} for file {file}");
                if (string.Compare(settings.ToEncoding, detectedEncoding, StringComparison.Ordinal) == 0)
                {
                    Console.WriteLine($"File {file} is already in the desired encoding ({settings.ToEncoding}). Skipping.");
                    continue;
                }
                if (string.Compare(settings.FromEncoding, detectedEncoding, StringComparison.Ordinal) != 0)
                {
                    Console.WriteLine($"File {file} is in {detectedEncoding} encoding, which is not the expected {settings.FromEncoding}. Skipping.");
                    continue;
                }
                string convertedHtml = ConvertHtmlEncoding(encodingResult.DetectedText, settings);
                string toString = ConvertEncoding(sourceEncoding, targetEncoding, convertedHtml);

                string outputFilePath = Path.Combine(settings.OutputFolder, Path.GetFileName(file));
                File.WriteAllText(outputFilePath, toString, targetEncoding);
                Console.WriteLine($"Converted {file} from {settings.FromEncoding} to {settings.ToEncoding} and saved to {outputFilePath}");
            }
        }

        private static string ConvertEncoding(Encoding sourceEncoding, Encoding targetEncoding, string convertedHtml)
        {
            switch (sourceEncoding.HeaderName)
            {
                case "gb2312":
                    switch (targetEncoding.HeaderName)
                    {
                        case "big5":
                            return ZhConverter.HansToHant(convertedHtml);

                    }
                    break;
                 case "big5":
                    switch (targetEncoding.HeaderName)
                    {
                        case "gb2312":
                            return ZhConverter.HantToHans(convertedHtml);
                    }
                    break;
            }
            byte[] fromBytes = sourceEncoding.GetBytes(convertedHtml);
            byte[] toBytes = Encoding.Convert(sourceEncoding, targetEncoding, fromBytes);

            string toString = targetEncoding.GetString(toBytes);
            return toString;
        }

        private static IMySettings LoadAndConfirmSettings()
        {
            string jsonFilePath = Path.Combine(
                            Environment.GetFolderPath(Environment.SpecialFolder.ApplicationData),
                            "HtmlEncodingConverter");
            if (!Directory.Exists(jsonFilePath))
            {
                Directory.CreateDirectory(jsonFilePath);
            }
            jsonFilePath = Path.Combine(jsonFilePath, "settings.json");
            IMySettings settings = new ConfigurationBuilder<IMySettings>()
                .UseAppConfig()
                .UseJsonFile(jsonFilePath)
                .UseCommandLineArgs()
                .Build();
            settings.InputFolder =
                PromptForTextValue("Enter Path for the input files (Press Enter for default):[{0}]", settings.InputFolder);
            settings.OutputFolder = PromptForTextValue("Enter Path for the output folder(Press Enter for default):[{0}]", settings.OutputFolder);
            settings.FileExtension = PromptForTextValue("Enter the file extension to process (Press Enter for default):[{0}]", settings.FileExtension);
            settings.FromEncoding = PromptForTextValue("Enter the source encoding (Press Enter for default):[{0}]", settings.FromEncoding);
            settings.ToEncoding = PromptForTextValue("Enter the target encoding (Press Enter for default):[{0}]", settings.ToEncoding);

            if (!Directory.Exists(settings.OutputFolder))
            {
                Console.WriteLine($"Output folder does not exist: {settings.OutputFolder}. Creating it.");
                Directory.CreateDirectory(settings.OutputFolder);
            }
            if (string.IsNullOrWhiteSpace(settings.FileExtension))
            {
                settings.FileExtension = "html";
            }
            if (string.IsNullOrWhiteSpace(settings.FromEncoding))
            {
                settings.FromEncoding = "utf-8";
            }
            if (string.IsNullOrWhiteSpace(settings.ToEncoding))
            {
                settings.ToEncoding = "utf-8";
            }
            return settings;
        }

        private static string ConvertHtmlEncoding(string detectedText, IMySettings settings)
        {
            HtmlAgilityPack.HtmlDocument doc = new HtmlAgilityPack.HtmlDocument();
            doc.LoadHtml(detectedText);
            var encodingNode = doc.DocumentNode.SelectSingleNode("//meta[@http-equiv='Content-Type']");
            if (encodingNode != null)
            {
                encodingNode.SetAttributeValue("content", $"text/html; charset={settings.ToEncoding}");
            }
            return doc.DocumentNode.OuterHtml;
        }

        private static EncodingResult? DetectEncoding(string file, IMySettings settings)
        {
            var fileBytes = File.ReadAllBytes(file);
            try
            {
                if (fileBytes.Length < 256)
                {
                    Array.Resize(ref fileBytes, 256);
                }
                IMultiLanguage2 lang = (IMultiLanguage2)new MultiLanguage();
                int len = fileBytes.Length;
                DetectEncodingInfo info;
                int scores = 1;

                // bytes to IntPtr
                GCHandle handle = GCHandle.Alloc(fileBytes, GCHandleType.Pinned);
                IntPtr pbytes = Marshal.UnsafeAddrOfPinnedArrayElement(fileBytes, 0);
                MLDETECTCP options = MLDETECTCP.MLDETECTCP_HTML;
                try
                {
                    lang.DetectInputCodepage((uint)options, 0, pbytes, ref len, out info, ref scores);
                }
                finally
                {
                    if (handle.IsAllocated)
                        handle.Free();
                }
                var detectedEncoding= Encoding.GetEncoding((int)info.nCodePage);
                if (detectedEncoding == null)
                {
                    Console.WriteLine($"Could not find encoding for code page {info.nCodePage} in file {file}");
                    return null;
                }
                string detectedText = detectedEncoding.GetString(fileBytes, 0, len);
                return new EncodingResult
                {
                    DetectedEncoding = detectedEncoding,
                    DetectedText = detectedText
                };
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Error detecting encoding for file {file}: {ex.Message}");
                return null;
            }           

        }      
        static string PromptForTextValue(string prompt, string defaultValue)
        {
            Console.WriteLine(string.Format(prompt, defaultValue));
            var result = Console.ReadLine();
            if (string.IsNullOrWhiteSpace(result))
            {
                return defaultValue;
            }
            return result;
        }
    }

}
