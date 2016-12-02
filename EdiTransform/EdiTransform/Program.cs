using OopFactory.X12.Hipaa.Claims;
using OopFactory.X12.Hipaa.Claims.Services;
using OopFactory.X12.Parsing;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;
using System.Xml;
using OopFactory.X12.Parsing.Model;

namespace EdiTransform
{
    class Program
    {
        static void Main(string[] args)
        {
            MergeEdiTest();
            //UpdateEdiTest();
        }

        private static void MergeEdiTest()
        {
            using (Stream stream1 = Assembly.GetExecutingAssembly().GetManifestResourceStream("EdiTransform.InstitutionalClaim4010.txt"))
            {
                using (Stream stream2 = Assembly.GetExecutingAssembly().GetManifestResourceStream("EdiTransform.InstitutionalClaim5010.txt"))
                {
                    var parser = new X12Parser();
                    var interchange1 = parser.ParseMultiple(stream1).ToList().FirstOrDefault();
                    var interchange2 = parser.ParseMultiple(stream2).ToList().FirstOrDefault();

                    foreach (var transaction in interchange2.FunctionGroups.ToArray().First().Transactions)
                    {
                        interchange1.FunctionGroups.ToArray().First().Transactions.Add(transaction);
                    }
                    string newX12 = interchange1.SerializeToX12(true);
                    Debug.WriteLine("After passing through interchange object:");
                    Debug.WriteLine(newX12);
                }
            }
        }

        private static void UpdateEdiTest()
        {
            using (Stream stream = Assembly.GetExecutingAssembly().GetManifestResourceStream("EdiTransform.ProfessionalClaim1.txt"))
            {
                var parser = new X12Parser();
                var interchanges = parser.ParseMultiple(stream);
                var service = new ProfessionalClaimToHcfa1500FormTransformation("");
                ClaimDocument doc = service.Transform837ToClaimDocument(interchanges.First());
                var claim = doc.Claims.First();
                claim.ServiceLines[0].Procedure.ProcedureCode = "87070";
                claim.ServiceLines[1].Procedure.ProcedureCode = "99213";
                claim.BillingProvider.TaxId = "12345";
                XmlDocument newdoc = TransformClaimDocumentTo837(interchanges.First(), claim);
                string x12 = parser.TransformToX12(newdoc.OuterXml);
                var modifiedInterchange = parser.ParseMultiple(x12).First();
                string newX12 = modifiedInterchange.SerializeToX12(true);
                Debug.WriteLine("After passing through interchange object:");
                Debug.WriteLine(newX12);
            }
        }

        //reverse engineer from https://raw.githubusercontent.com/mmooney/MMDB.DataService/master/ThirdParty/x12parser/trunk/src/OopFactory.X12.Hipaa/Claims/Services/Xsl/X12-837-To-ClaimDocument.xslt
        private static XmlDocument TransformClaimDocumentTo837(Interchange interchange, Claim claim)
        {
            XmlDocument document = new XmlDocument() { PreserveWhitespace = true } ;
            document.LoadXml(interchange.Serialize());
            var claimLoop2300 = document.SelectNodes("//Loop");
            for (int i = 0; i < claimLoop2300.Count; i++)
            {
                TransformLoopTo837(claimLoop2300[i],claim);
            }
            return document;
        }

        private static void TransformLoopTo837(XmlNode loop, Claim claim)
        {
            var loopId = ((XmlElement)loop).GetAttribute("LoopId");
            switch (loopId)
            {
                case "2300":
                    TransformLoop2300To837(loop, claim);break;
                case "2010BA":
                    TransformLoop2010BATo837(loop, claim.Subscriber);break;
            }                
        }

        private static void TransformLoop2010BATo837(XmlNode loop2010BA, ClaimMember subscriber)
        {
            var gender = loop2010BA.SelectSingleNode("DMG/DMG03");
            switch (subscriber.Gender)
            {
                case OopFactory.X12.Hipaa.Common.GenderEnum.Unknown:
                    gender.InnerText = "";
                    break;
                case OopFactory.X12.Hipaa.Common.GenderEnum.Male:
                    gender.InnerText = "M";
                    break;
                case OopFactory.X12.Hipaa.Common.GenderEnum.Female:
                    gender.InnerText= "F";
                    break;
                default:
                    break;
            }
            var dob= loop2010BA.SelectSingleNode("DMG/DMG02");
            if (subscriber.DateOfBirth.HasValue)
                dob.InnerText= subscriber.DateOfBirth.Value.ToString("yyyyMMdd");
        }

        private static void TransformLoop2300To837(XmlNode loop2300, Claim claim)
        {
            var parentLoopId = loop2300.SelectSingleNode("../@LoopId");
            var parentLoopIdValue= (parentLoopId ==null)?string.Empty:parentLoopId.Value;
            XmlNode transactionCode;
            if (parentLoopIdValue == "2000B")
            {
                transactionCode = loop2300.SelectSingleNode("../../../ST/ST02");
            }
            else
                transactionCode = loop2300.SelectSingleNode("../../../../ST/ST02");
            transactionCode.InnerText= claim.TransactionCode;
            var claimNumber = loop2300.SelectSingleNode("REF[REF01 = 'D9'] / REF02");
            claimNumber.InnerText = claim.ClaimNumber;
            var patientControlNumber= loop2300.SelectSingleNode("CLM/CLM01");
            patientControlNumber.InnerText = claim.PatientControlNumber;
            var totalClaimChargeAmount= loop2300.SelectSingleNode("CLM/CLM02");
            totalClaimChargeAmount.InnerText = claim.TotalClaimChargeAmount.ToString();
            if (!string.IsNullOrWhiteSpace(claim.ProviderSignatureOnFile))
            {
                var providerSignatureOnFile = loop2300.SelectSingleNode("CLM/CLM06");
                providerSignatureOnFile.InnerText = claim.ProviderSignatureOnFile;
            }
            if (!string.IsNullOrWhiteSpace(claim.MedicalRecordNumber))
            {
                var medicalRecordNumber = loop2300.SelectSingleNode("REF[REF01 = 'EA'] / REF02");
                medicalRecordNumber.InnerText = claim.MedicalRecordNumber;
            }
            if(!string.IsNullOrWhiteSpace(claim.PriorAuthorizationNumber))
            {
                var priorAuthorizationNumber = loop2300.SelectSingleNode("REF[REF01 = 'G1'] / REF02");
                priorAuthorizationNumber.Value = claim.PriorAuthorizationNumber;
            }
            
            if (parentLoopIdValue == "2000B")
            {
                TransformBillingProviderHLoopTo837(loop2300.SelectNodes("../../."), claim);
                TransformSubscriberHLoopTo837(loop2300.SelectNodes("../."), claim);
            }
            else if (parentLoopIdValue == "2000C")
            {
                TransformBillingProviderHLoopTo837(loop2300.SelectNodes("../../../."), claim);
                TransformSubscriberHLoopTo837(loop2300.SelectNodes("../../."), claim);
                TransformPatientHLoopTo837(loop2300.SelectNodes("../."), claim);
            }
            TransformPWKTo837(loop2300.SelectNodes("PWK"), claim);
            TransformDTPTo837(loop2300.SelectNodes("DTP"), claim);
            TransformAMTTo837(loop2300.SelectNodes("AMT"), claim);
            TransformREFTo837(loop2300.SelectNodes("REF"), claim);
            TransformNTETo837(loop2300.SelectNodes("NTE"), claim);
            TransformHITo837(loop2300.SelectNodes("HI"), claim);
            TransformLoopTo837(loop2300.SelectNodes("Loop"), claim);
        }

        private static void TransformLoopTo837(XmlNodeList xmlNodeList, Claim claim)
        {
            foreach (XmlNode item in xmlNodeList)
            {
                TransformLoopTo837(item, claim);
            }
        }

        private static void TransformNTETo837(XmlNodeList xmlNodeList, Claim claim)
        {
            
        }

        private static void TransformREFTo837(XmlNodeList xmlNodeList, Claim claim)
        {
            
        }

        private static void TransformAMTTo837(XmlNodeList xmlNodeList, Claim claim)
        {
            
        }

        private static void TransformDTPTo837(XmlNodeList xmlNodeList, Claim claim)
        {
            
        }

        private static void TransformPWKTo837(XmlNodeList xmlNodeList, Claim claim)
        {
            
        }

        private static void TransformPatientHLoopTo837(XmlNodeList xmlNodeList, Claim claim)
        {
            
        }

        private static void TransformSubscriberHLoopTo837(XmlNodeList xmlNodeList, Claim claim)
        {
            foreach (XmlNode item in xmlNodeList)
            {
                TransformSubscriberHLoopTo837(item,claim);
            }
        }

        private static void TransformSubscriberHLoopTo837(XmlNode hloop, Claim claim)
        {
            TransformSBRTo837(hloop.SelectSingleNode("SBR"),  claim.SubscriberInformation);
        }

        private static void TransformSBRTo837(XmlNode xmlNode,
            OopFactory.X12.Hipaa.Common.SubscriberInformation subscriberInformation)
        {
            var payerResponsibilitySequenceNumberCode = xmlNode.SelectSingleNode("SBR01");
            payerResponsibilitySequenceNumberCode.InnerText = 
                subscriberInformation.PayerResponsibilitySequenceNumberCode;
        }
        class TransformBillingProviderHLoopTo837State {
            public XmlNode FirstType87 { get; set; }
            public XmlNode FirstType85 { get; set; }
        }        
        private static void TransformBillingProviderHLoopTo837(XmlNodeList xmlNodeList, Claim claim)
        {
            TransformBillingProviderHLoopTo837State state = new TransformBillingProviderHLoopTo837State();
            foreach (XmlNode item in xmlNodeList)
            {
                TransformBillingProviderHLoopTo837(item, claim, state);
            }
            if (claim.PayToProvider != null )             
            {
                if (state.FirstType87 != null)
                {
                    TransformPayToProvider(state.FirstType87, claim);
                }
                else if (state.FirstType85 != null)
                {
                    TransformPayToProvider(state.FirstType85, claim);
                }
            }
        }

        private static void TransformPayToProvider(XmlNode billingProvider, Claim claim)
        {
            XmlNode name = billingProvider.SelectSingleNode("REF");
            XmlNode taxId = null;
            if (name != null)
            {
                XmlNode nameId = name.SelectSingleNode("REF02");
                XmlNode nameQualifier = name.SelectSingleNode("REF01");
                
                if (nameId != null && nameQualifier != null)
                {
                    switch (nameQualifier.InnerText)
                    {
                        case "FI": taxId =nameId; break;
                        case "24": taxId = nameId; ; break;
                    }
                }
            }
            if (taxId != null)
                taxId.InnerText = claim.PayToProvider.TaxId;
            else
            {
                taxId = GetReferenceId(billingProvider, "EI");
                if (taxId == null)
                    taxId = GetReferenceId(billingProvider, "TJ");
            }
            if (taxId != null)
                taxId.InnerText = claim.PayToProvider.TaxId;
        }
        private static XmlNode GetReferenceId(XmlNode billingProvider, string referenceIdType)
        {
            return billingProvider.SelectSingleNode(string.Format("//REF/REF01[text() ='{0}']/../REF02", referenceIdType));
        }

        private static void TransformBillingProviderHLoopTo837(XmlNode xmlNode, Claim claim, TransformBillingProviderHLoopTo837State state)
        {
            var billingProviderLoop= xmlNode.SelectSingleNode("Loop");
            TransformBillingProviderLoopTo837(billingProviderLoop, claim,state);
        }

        private static void TransformBillingProviderLoopTo837(XmlNode billingProviderLoop, Claim claim, TransformBillingProviderHLoopTo837State state)
        {
            var nameTypeNode = billingProviderLoop.SelectSingleNode("NM1/NM101");
            if (nameTypeNode != null)
            {
                switch (nameTypeNode.InnerText)
                {
                    case "87":if (state.FirstType87 == null) state.FirstType87 = billingProviderLoop;break;
                    case "85": if (state.FirstType85 == null) state.FirstType85 = billingProviderLoop; break;
                }
            }


        }

        private static void TransformHITo837(XmlNodeList xmlNodeList, Claim claim)
        {
            
        }
    }
}
