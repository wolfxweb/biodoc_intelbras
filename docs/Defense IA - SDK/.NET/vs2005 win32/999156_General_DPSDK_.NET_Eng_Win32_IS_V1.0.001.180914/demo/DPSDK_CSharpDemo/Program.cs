using System;
using System.Collections.Generic;
using System.Windows.Forms;

namespace DPSDK_CSharpDemo
{
    static class Program
    {
        /// <summary>
        /// Main app
        /// </summary>
        [STAThread]
        static void Main()
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            Application.Run(new CSharpDemo());
        }
    }
}