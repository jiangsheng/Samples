// See https://aka.ms/new-console-template for more information
using System.Xml;

if (args.Length == 0)
{
    Console.WriteLine("Usage:RemoveNodeFromXml Path FilePattern NodeName");
    return;
}
if (args.Length != 3)
{
    Console.WriteLine("Usage:RemoveNodeFromXml Path FilePattern NodeName");
    return;
}
var path = args[0];
var pattern= args[1];
var nodeName= args[2];
if (!Directory.Exists(path))
{
    Console.WriteLine("Invalid Path Parameter");
    return;
}

var files = Directory.EnumerateFiles(path, pattern).ToList();
if(files.Count==0)
{
    Console.WriteLine("No file matches the given pattern");
    return;
}
foreach (var file  in files)
{
    System.Xml.XmlDocument doc = new System.Xml.XmlDocument();
    try
    {
        Console.WriteLine(String.Format("Processing {0}",file));
        doc.Load(file);
        var matchedNodes = doc.SelectNodes(String.Format("//{0}", nodeName));
        if (matchedNodes != null)
        {
            if (matchedNodes.Count > 0)
            {
                foreach (XmlNode node in matchedNodes)
                {
                    node.ParentNode.RemoveChild(node);
                }
                doc.Save(file);
            }
        }
    }
    catch (Exception ex)
    {
        Console.WriteLine(ex.Message);
    }    
}