using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;

namespace DPSDK_CSharpDemo
{
    class AppData
    {
        public enum Title_Info_e
	    {
		    TITLE_PROMPT = 0,
		    TITLE_ERROR = 1,
	    };

        static protected Translate s_Translate;

        public static void Init()
        {
            s_Translate = new Translate(LocalConfig.GetLanguage());
        }

        public static string _CS(string str)
        {
            return s_Translate.ConvertString(str);
        }

        public static string _CS(string str, string strValue1)
        {
            str = str.Replace("=", "%3D");
            string strRes = _CS(str);
            strRes = strRes.Replace("%1", strValue1);
            strRes = strRes.Replace("%3D", "=");
            return strRes;
        }

        public static string _CS(string str, string strValue1, string strValue2)
        {
            str = str.Replace("=", "%3D");
            string strRes = _CS(str);
            strRes = strRes.Replace("%1", strValue1);
            strRes = strRes.Replace("%2", strValue2);
            strRes = strRes.Replace("%3D", "=");
            return strRes;
        }

        public static string _CS(string str, string strValue1, string strValue2, string strValue3)
        {
            str = str.Replace("=", "%3D");
            string strRes = _CS(str);
            strRes = strRes.Replace("%1", strValue1);
            strRes = strRes.Replace("%2", strValue2);
            strRes = strRes.Replace("%3", strValue3);
            strRes = strRes.Replace("%3D", "=");
            return strRes;
        }

        public static void ShowMsgInfo(string strInfo)
        {
            ShowMsgInfo(strInfo, Title_Info_e.TITLE_PROMPT);
        }

        public static void ShowMsgInfo(string strInfo, Title_Info_e iTitle)
        {
            string strTitle = _CS("Error");
            if (iTitle != Title_Info_e.TITLE_ERROR)
	        {
		        strTitle = _CS("Prompt");
	        }
            MessageBox.Show(_CS(strInfo), strTitle, MessageBoxButtons.OK);
        }
    }
}
