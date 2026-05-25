using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;

namespace DPSDK_CSharpDemo
{
    class IniFile
    {
        private string m_FileName;

        public string FileName
        {
            get { return m_FileName; }
            set { m_FileName = value; }
        }

        [DllImport("kernel32.dll")]
        private static extern int GetPrivateProfileInt(
            string lpAppName,
            string lpKeyName,
            int nDefault,
            string lpFileName
            );

        //[DllImport("kernel32.dll")]
        //private static extern int GetPrivateProfileString(
        //    string lpAppName,
        //    string lpKeyName,
        //    string lpDefault,
        //    StringBuilder lpReturnedString,
        //    int nSize,
        //    string lpFileName
        //    );

        //[DllImport("kernel32.dll")]
        //private static extern int WritePrivateProfileString(
        //    string lpAppName,
        //    string lpKeyName,
        //    string lpString,
        //    string lpFileName
        //    );

        [DllImport("kernel32.dll")]
        private static extern int GetPrivateProfileString(
            byte[] section,
            byte[] key,
            byte[] def,
            byte[] value,
            int size,
            string lpFileName
            );

        [DllImport("kernel32.dll")]
        private static extern int WritePrivateProfileString(
            byte[] section,
            byte[] key,
            byte[] value,
            string lpFileName
            );

        /// <summary>
        /// constructor
        /// </summary>
        /// <param name="aFileName">IniÎÄ¼þÂ·¾¶</param>
        public IniFile(string aFileName)
        {
            this.m_FileName = aFileName;
        }

        /// <summary>
        /// constructor
        /// </summary>
        public IniFile()
        { }

        /// <summary>
        /// [extend]Read Int value
        /// </summary>
        /// <param name="section">Section</param>
        /// <param name="name">Key</param>
        /// <param name="def">Default value</param>
        /// <returns></returns>
        public int ReadInt(string section, string name, int def)
        {
            return GetPrivateProfileInt(section, name, def, this.m_FileName);
        }

        public static byte[] getBytes(string s)
        {
            return null == s ? null : Encoding.GetEncoding("UTF-8").GetBytes(s);
        }

        /// <summary>
        /// [extend]Read string value
        /// </summary>
        /// <param name="section">Section</param>
        /// <param name="name">Key</param>
        /// <param name="def">Default value</param>
        /// <returns></returns>
        public string ReadString(string section, string name, string def)
        {
            //StringBuilder vRetSb = new StringBuilder(2048);
            //GetPrivateProfileString(section, name, def, vRetSb, 2048, this.m_FileName);
            //return vRetSb.ToString();

            byte[] vRetSb = new byte[2048];
            int count = GetPrivateProfileString(getBytes(section), getBytes(name), getBytes(def), vRetSb, 2048, this.m_FileName);
            return Encoding.GetEncoding("UTF-8").GetString(vRetSb, 0, count).Trim();
        }

        /// <summary>
        /// [extend]Write Int value£¬if not exist section and key will auto create
        /// </summary>
        /// <param name="section">Section</param>
        /// <param name="name">Key</param>
        /// <param name="Ival">Value</param>
        public void WriteInt(string section, string name, int Ival)
        {

            WritePrivateProfileString(getBytes(section), getBytes(name), getBytes(Ival.ToString()), this.m_FileName);
        }

        /// <summary>
        /// [extend]Write String value£¬if not exist section and key will auto create
        /// </summary>
        /// <param name="section">Section</param>
        /// <param name="name">Key</param>
        /// <param name="strVal">Value</param>
        public void WriteString(string section, string name, string strVal)
        {
            WritePrivateProfileString(getBytes(section), getBytes(name), getBytes(strVal), this.m_FileName);
        }

        /// <summary>
        /// Delete Section
        /// </summary>
        /// <param name="section"></param>
        public void DeleteSection(string section)
        {
            WritePrivateProfileString(getBytes(section), null, null, this.m_FileName);
        }

        /// <summary>
        /// Delete All Section
        /// </summary>
        public void DeleteAllSection()
        {
            WritePrivateProfileString(null, null, null, this.m_FileName);
        }

    }
}
