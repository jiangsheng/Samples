using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using System.Runtime.InteropServices.ComTypes;
using System.Diagnostics;

namespace DynamicAutoCompleteCSharp
{
    public partial class FormAutoComplete : Form
    {
        object autoComplete ;
        CandidateList candidateList;
        int stringLength = 10;
        int randomStrings = 10000;
        Random rnd = new Random((int)System.DateTime.Now.Ticks);
        DynamicAutoCompleteCSharp.NativeMethods.IAutoComplete iAutoComplete;
        DynamicAutoCompleteCSharp.NativeMethods.IAutoComplete2 iAutoComplete2;
        DynamicAutoCompleteCSharp.NativeMethods.IAutoCompleteDropDown iAutoCompleteDropDown; 
        public FormAutoComplete()
        {
            InitializeComponent();
            string[] data = new string[randomStrings];
            GenerateCandidates(data, null);
            candidateList= new CandidateList(data);
        }
        void GenerateCandidates(string[] data,string prefix)
			{
				bool addPrefix=!String.IsNullOrEmpty(prefix);
				StringBuilder text=new StringBuilder();
				for (int i = 0; i < randomStrings; i++)
				{
                    text.Remove(0, text.Length);
					if(addPrefix)
						text.Append(prefix);
                    text.Append(GenerateRandomText(rnd, 'a', 'z', stringLength));
                    data[i] = text.ToString();
					
				}
			}
        private static string GenerateRandomText(Random rnd,char minValue, char maxValue, int stringLength)
        {
            
            System.Text.StringBuilder randomText = new
            System.Text.StringBuilder(stringLength);

            //the range that we are allowed to go above the min value
            int randomRange = maxValue - minValue;

            double rndValue;

            for (int i = 0; i < stringLength; i++)
            {
                rndValue = rnd.NextDouble();

                randomText.Append((char)(minValue + rndValue * randomRange));
            }
            return randomText.ToString();
        }
        private void FormAutoComplete_Load(object sender, EventArgs e)
        {
            InitializeAutoComplete();
            InitializeEditControl();
        }

        private void InitializeEditControl()
        {
            AutoCompleteMode autoCompleteMode = textBox1.AutoCompleteMode;
            textBox1.AutoCompleteMode = AutoCompleteMode.None;
            iAutoComplete2.SetOptions((uint)autoCompleteMode);
            IEnumString iEnumString = candidateList;
            iAutoComplete2.Init(
                new HandleRef(textBox1, textBox1.Handle),
                iEnumString, String.Empty
                , String.Empty);
        }

        private void InitializeAutoComplete()
        {
            Type typeAautoComplete = Type.GetTypeFromCLSID(new Guid("{00BB2763-6A77-11D0-A535-00C04FD7D062}"));
            autoComplete = Activator.CreateInstance(typeAautoComplete);
            iAutoComplete = (DynamicAutoCompleteCSharp.NativeMethods.IAutoComplete)autoComplete;
            iAutoComplete2 = (DynamicAutoCompleteCSharp.NativeMethods.IAutoComplete2)autoComplete;
            iAutoCompleteDropDown = (DynamicAutoCompleteCSharp.NativeMethods.IAutoCompleteDropDown)autoComplete;
        }

        private void textBox1_TextChanged(object sender, EventArgs e)
        {
            if (string.IsNullOrEmpty(textBox1.Text)) return;
            uint status = 0;
            StringBuilder text;
            try
            {
                iAutoCompleteDropDown.GetDropDownStatus(out status, out text);
            }
            catch (Exception)
            {
                return;
            }

            if (text == null) text = new StringBuilder(textBox1.Text);
            string[] data = new string[randomStrings];
            GenerateCandidates(data, text.ToString());
            candidateList.ReplaceCandidateList(data);
            iAutoCompleteDropDown.ResetEnumerator();
        }
    }
}
