using System;
using System.Collections.Generic;
using System.Text;

namespace DPSDK_CSharpDemo
{
    public enum Demo_Language_Type
    {
	    Language_Chinese	= 0,
	    Language_English	= 1,
    };

    class Translate
    {
        protected Demo_Language_Type m_iLanguage = Demo_Language_Type.Language_English;
        protected string m_strFileName = "English.ini";
        protected const string strTag = "String"; 
        protected IniFile m_LocalIniFile;

        public Translate(Demo_Language_Type iLanguageType)
        {
	        m_iLanguage = iLanguageType;
	        if (m_iLanguage == Demo_Language_Type.Language_English)
	        {
		        m_strFileName = "English.ini";
	        }
	        else
	        {
		        m_strFileName = "Chinese.ini";
	        }

            string strCurPath = Environment.CurrentDirectory;
            m_LocalIniFile = new IniFile(strCurPath + "/" + m_strFileName);
        }

        public string ConvertString(string strSrc)
        {
            string strValue = m_LocalIniFile.ReadString(strTag, strSrc, strSrc);
            return strValue;
            //return Encoding.UTF8.GetString(Encoding.Default.GetBytes(strValue));
        }
    }
}
