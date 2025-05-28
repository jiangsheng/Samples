namespace HtmlEncodingConverter
{
    public interface IMySettings
    {
        string InputFolder { get; set; }
        string OutputFolder { get; set; }
        string FileExtension { get; set; }
        string FromEncoding { get; set; }
        string ToEncoding { get; set; }
    }
}