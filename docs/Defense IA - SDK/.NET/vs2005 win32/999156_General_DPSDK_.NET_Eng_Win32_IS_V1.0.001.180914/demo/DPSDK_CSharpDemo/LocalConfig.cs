using System;
using System.Collections.Generic;
using System.Text;

namespace DPSDK_CSharpDemo
{
    struct Demo_Save_Login_Info_t 
    {
	    public string strIP;
        public string strPort;
        public string strUserName;
        public string strPWD;

        public void Init()
	    {
		    strIP = "172.25.7.51";
		    strPort = "8314";
		    strUserName = "system";
		    strPWD = "admin12345";
	    }
    };

    class LocalConfig
    {
        static string strFileName = "Localcfg.ini";
        static string strTagLastLoginInfo = "LastLoginInfo";
        static string strItemIP = "Ip";
        static string strItemPort = "Port";
        static string strItemUserName = "UserName";
        static string strItemPassword = "Password";

        static string strTagLanguage = "Language";
        static string strItemType = "Type";

        static string strPlatFormType = "PlatFormType";

        protected static IniFile m_LocalIniFile = new IniFile(Environment.CurrentDirectory + "/" + strFileName);

        public static Demo_Language_Type GetLanguage()
        {
            Demo_Language_Type qvValue = (Demo_Language_Type)m_LocalIniFile.ReadInt(strTagLanguage, strItemType, (int)Demo_Language_Type.Language_English);
            return qvValue;
        }

        public static bool CheckMenuEnable(string strMenuName)
        {
	        int qvValue = m_LocalIniFile.ReadInt("Menus", strMenuName, 0);
	        bool bRet = false;
	        if (qvValue != 0)
	        {
		        bRet = true;
	        }
	        return bRet;
        }

        public static Demo_Save_Login_Info_t GetLastLoginInfo()
        {
            Demo_Save_Login_Info_t struLoginInfo = new Demo_Save_Login_Info_t();
            struLoginInfo.Init();
	        struLoginInfo.strIP = m_LocalIniFile.ReadString(strTagLastLoginInfo, strItemIP, "");
	        struLoginInfo.strPort = m_LocalIniFile.ReadString(strTagLastLoginInfo, strItemPort, "");
	        struLoginInfo.strUserName = m_LocalIniFile.ReadString(strTagLastLoginInfo, strItemUserName, "");
	        struLoginInfo.strPWD = m_LocalIniFile.ReadString(strTagLastLoginInfo, strItemPassword, "");
            return struLoginInfo;
        }

        public static void SaveLastLoginInfo(Demo_Save_Login_Info_t struLoginInfo)
        {
	        m_LocalIniFile.WriteString(strTagLastLoginInfo, strItemIP, struLoginInfo.strIP);
	        m_LocalIniFile.WriteString(strTagLastLoginInfo, strItemPort, struLoginInfo.strPort);
	        m_LocalIniFile.WriteString(strTagLastLoginInfo, strItemUserName, struLoginInfo.strUserName);
            m_LocalIniFile.WriteString(strTagLastLoginInfo, strItemPassword, struLoginInfo.strPWD);
        }

        public static bool IsVSL()
        {
            int qvValue = m_LocalIniFile.ReadInt(strPlatFormType, strItemType, 0);
            return (qvValue == 0);
        }
    }
}
