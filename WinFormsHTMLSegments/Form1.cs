using MSHTML;
using System.ComponentModel.Design;

namespace WinFormsHTMLSegments
{
    public partial class Form1 : Form
    {
        private static readonly Guid EDIT_SERVICES_SID = new Guid(0x3050f7f9, 0x98b5, 0x11cf, 0xbb, 0x82, 0x00, 0xaa, 0x00, 0xbd, 0xce, 0x0b);
        public Form1()
        {
            InitializeComponent();
        }
        private void Form1_Load(object sender, EventArgs e)
        {
            webBrowser1.Url = new Uri("about:blank");
        }

        private void LoadExamplePage()
        {
            webBrowser1.DocumentText =
            "<html>\r\n<head>\r\n<title>Lorem ipsum</title>" +
            "<meta http-equiv=\"X-UA-Compatible\" content=\"IE=7\">\r\n</head>\r\n" +
            "<body><div id=\"div1\">Lorem ipsum dolor sit amet esse officia ad ut." +
            "Et anim ex do elit aute duis dolore occaecat reprehenderit pariatur ea ipsum. " +
            "Ea occaecat in aliqua elit culpa elit amet ad excepteur aliquip elit nisi sunt." +
            "Lorem laboris eu in fugiat. Sint quis enim sint anim nisi amet reprehenderit dolore ut in.</div>" +
            "<div id=\"div2\">Dolore et ut sunt consectetur do incididunt." +
            "Voluptate occaecat fugiat in ea id cillum sed. " +
            "Aute ex ut ex dolore dolor dolore sit reprehenderit sed qui laborum." +
            "Eu magna fugiat dolor eiusmod. " +
            "Ullamco anim ex ullamco sint lorem sed sit anim esse voluptate in consectetur cupidatat.</div>" +
            "<div id=\"div3\">Occaecat deserunt dolor aliqua consequat dolor in irure reprehenderit dolore sed cupidatat eu. " +
            "Consectetur cupidatat enim nulla minim esse fugiat dolore dolore adipiscing." +
            "Ut officia officia exercitation exercitation sed ut. " +
            "Officia lorem voluptate in quis esse. Cupidatat ut fugiat sint anim.</div>" +
            "</body>\r\n</html>";
        }
        void HighlightPage(HtmlDocument doc)
        {
            object domDocument = doc.DomDocument;
            var doc2 = domDocument as IHTMLDocument2;
            var doc4 = domDocument as IHTMLDocument4;
            if (doc2 == null) return;
            if (doc2.designMode != "on")
            {
                var div1 = doc.GetElementById("div1").DomElement
                    as IHTMLElement;
                var div3 = doc.GetElementById("div3").DomElement
                    as IHTMLElement;
                IMarkupServices markupServices =
                    domDocument as IMarkupServices;
                IMarkupPointer[] divEnds = new IMarkupPointer[4];
                for (int i = 0; i < 4; i++)
                {
                    IMarkupPointer tempPointer;
                    markupServices.CreateMarkupPointer(out tempPointer);
                    divEnds[i] = tempPointer;
                }
                divEnds[0].MoveAdjacentToElement(div1,
                    _ELEMENT_ADJACENCY.ELEM_ADJ_AfterBegin);
                divEnds[1].MoveAdjacentToElement(div1,
                    _ELEMENT_ADJACENCY.ELEM_ADJ_BeforeEnd);
                divEnds[2].MoveAdjacentToElement(div3,
                    _ELEMENT_ADJACENCY.ELEM_ADJ_AfterBegin);
                divEnds[3].MoveAdjacentToElement(div3,
                    _ELEMENT_ADJACENCY.ELEM_ADJ_BeforeEnd);

                IDisplayServices displayServices =
                    domDocument as IDisplayServices;
                IDisplayPointer[] displayPointers = new IDisplayPointer[4];
                for (int i = 0; i < 4; i++)
                {
                    IDisplayPointer displayPointer;
                    displayServices.CreateDisplayPointer(out displayPointer);
                    displayPointer.MoveToMarkupPointer(divEnds[i], null);
                    displayPointers[i] = displayPointer;
                }
                IHTMLRenderStyle renderStyle = doc4.createRenderStyle(null);
                renderStyle.defaultTextSelection = "false";
                renderStyle.textDecoration = "underline";
                renderStyle.textUnderlineStyle = "wave";
                renderStyle.textDecorationColor = "red";
                renderStyle.textBackgroundColor = "transparent";
                renderStyle.textColor = "transparent";

                IHighlightRenderingServices highlightRenderingServices
                    = domDocument as IHighlightRenderingServices;
                IHighlightSegment highlightSegment;
                highlightRenderingServices.AddSegment(
                    displayPointers[0], displayPointers[1], 
                    renderStyle, out highlightSegment);
                highlightRenderingServices.AddSegment(
                    displayPointers[2], displayPointers[3],
                    renderStyle, out highlightSegment);
                //can turn design mode on here and wait for the document to switch mode.
                //however in this example
                //the url is about:blank
                //and turn on designer mode will wipe our changes
                /*doc2.designMode = "on";
                IHTMLEditServices editServices =
                   ComHelper.QueryService(domDocument, 
                EDIT_SERVICES_SID, typeof(IHTMLEditServices).GUID)
                   as IHTMLEditServices;
                IMarkupContainer markupContainer =
                    domDocument as IMarkupContainer;
                ISelectionServices selectionServices;
                editServices.GetSelectionServices(
                    markupContainer, out selectionServices);*/
            }
        }
        private void webBrowser1_DocumentCompleted(
            object sender, WebBrowserDocumentCompletedEventArgs e)
        {
            if (webBrowser1.ReadyState == WebBrowserReadyState.Complete)
            {
                var doc = webBrowser1.Document;
                if (doc == null) return;
                //about:blank
                if (doc.Title == string.Empty)
                {
                    LoadExamplePage();
                }
                else if (doc.Title == "Lorem ipsum")
                {
                    HighlightPage(doc);
                }
            }
        }

    }
}
