using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using DPSDK_RES = System.Int32;

namespace DPSDK_CSharpDemo
{
    public partial class CSharpDemo : Form
    {
        private void fEventCallBack(IntPtr iEventType, IntPtr pEventBuf, IntPtr uiBufSize, IntPtr pUserData)
        {
            string strNotify = "";
            switch (iEventType.ToInt32())
            {
                case DPSDK_EVENT_SERVER_OFFLINE:
                    strNotify = "\r\nServer offline";
                    ShowEventNotify(strNotify);
                    break;
                case DPSDK_EVENT_RELOGIN_SUCCESS:
                    strNotify = "\r\nServer relogin success";
                    ShowEventNotify(strNotify);
                    break;
                case DPSDK_EVENT_ALARM_ALARMEVENT:
                    AlarmEventNotify(pEventBuf);
                    break;
                case DPSDK_EVENT_ALARM_CONFIRMALARM:
                    AlarmConfirmNotify(pEventBuf);
                    break;
                case DPSDK_EVENT_ALARM_ALARMPICTURE:
                    AlarmPictureNotify(pEventBuf);
                    break;
                case DPSDK_EVENT_ALARM_EXPORTALARM:
                    AlarmExportNotify(pEventBuf);
                    break;
                case DPSDK_EVENT_DEVICE_STATUS:
                    DPSDK_DEV_STATUS_NOTIFY struDevStatusNotify = new DPSDK_DEV_STATUS_NOTIFY();
                    struDevStatusNotify = (DPSDK_DEV_STATUS_NOTIFY)Marshal.PtrToStructure(pEventBuf, typeof(DPSDK_DEV_STATUS_NOTIFY));
                    strNotify = "\r\nDevice status modify. deviceCode = " + struDevStatusNotify.szDeviceID + ", device status = " + struDevStatusNotify.iStatus.ToString();
                    ShowEventNotify(strNotify);
                    break;
                case DPSDK_EVENT_CHANNEL_STATUS:
                    DPSDK_CHANNEL_STATUS_NOTIFY struChannelStatusNotify = new DPSDK_CHANNEL_STATUS_NOTIFY();
                    struChannelStatusNotify = (DPSDK_CHANNEL_STATUS_NOTIFY)Marshal.PtrToStructure(pEventBuf, typeof(DPSDK_CHANNEL_STATUS_NOTIFY));
                    strNotify = "\r\nChannel status modify. channelCode = " + struChannelStatusNotify.szChannelID + ", channel status = " + struChannelStatusNotify.iStatus.ToString();
                    ShowEventNotify(strNotify);
                    break;
                case DPSDK_EVENT_ADD_ORG:
                    DPSDK_ORG_BASE_INFO struOrgBaseInfo = new DPSDK_ORG_BASE_INFO();
                    struOrgBaseInfo = (DPSDK_ORG_BASE_INFO)Marshal.PtrToStructure(pEventBuf, typeof(DPSDK_ORG_BASE_INFO));
                    string strOrgName = Encoding.GetEncoding("utf-8").GetString(Encoding.Default.GetBytes(struOrgBaseInfo.szOrgName));
                    strNotify = "\r\nAdd org. org code = " + struOrgBaseInfo.szOrgCode + ", org name = " + strOrgName;
                    ShowEventNotify(strNotify);
                    break;
                case DPSDK_EVENT_MODIFY_ORG:
                    DPSDK_ORG_BASE_INFO struModifyOrgBaseInfo = new DPSDK_ORG_BASE_INFO();
                    struModifyOrgBaseInfo = (DPSDK_ORG_BASE_INFO)Marshal.PtrToStructure(pEventBuf, typeof(DPSDK_ORG_BASE_INFO));
                    string strModifyOrgName = Encoding.GetEncoding("utf-8").GetString(Encoding.Default.GetBytes(struModifyOrgBaseInfo.szOrgName));
                    strNotify = "\r\nModify org. org code = " + struModifyOrgBaseInfo.szOrgCode + ", org name = " + strModifyOrgName;
                    ShowEventNotify(strNotify);
                    break;
                case DPSDK_EVENT_DELETE_ORG:
                    strNotify = "\r\nDelete org, xml = " + Marshal.PtrToStringAnsi(pEventBuf);
                    ShowEventNotify(strNotify);
                    break;
                case DPSDK_EVENT_MOVE_ORG:
                    DPSDK_MOVE_ORG_NOTIFY struMoveOrgNotify = new DPSDK_MOVE_ORG_NOTIFY();
                    struMoveOrgNotify = (DPSDK_MOVE_ORG_NOTIFY)Marshal.PtrToStructure(pEventBuf, typeof(DPSDK_MOVE_ORG_NOTIFY));
                    strNotify = "\r\nMove org, old org code = " + struMoveOrgNotify.szOldOrgCode + ", new org code = " + struMoveOrgNotify.szNewOrgCode;
                    ShowEventNotify(strNotify);
                    break;
                case DPSDK_EVENT_ALERT_USER:
                    strNotify = "\r\nalert user.";
                    ShowEventNotify(strNotify);
                    break;
                case DPSDK_EVENT_ADD_DEVICE:
                    DPSDK_ADD_DEVICE_NOTIFY struAddDevcieNotify = new DPSDK_ADD_DEVICE_NOTIFY();
                    struAddDevcieNotify = (DPSDK_ADD_DEVICE_NOTIFY)Marshal.PtrToStructure(pEventBuf, typeof(DPSDK_ADD_DEVICE_NOTIFY));
                    string strAddDevcieDeviceName = Encoding.GetEncoding("utf-8").GetString(Encoding.Default.GetBytes(struAddDevcieNotify.struDevAllInfo.struDevInfo.szDeviceName));
                    strNotify = "\r\nAdd device, org code  = " + struAddDevcieNotify.szOrgCode + ", device ID = " + struAddDevcieNotify.struDevAllInfo.struDevInfo.szDeviceID + ", device name = " + strAddDevcieDeviceName;
                    ShowEventNotify(strNotify);
                    break;
                case DPSDK_EVENT_MODIFY_DEVICE:
                    DPSDK_MODIFY_DEVICE_NOTIFY struModifyDevcieNotify = new DPSDK_MODIFY_DEVICE_NOTIFY();
                    struModifyDevcieNotify = (DPSDK_MODIFY_DEVICE_NOTIFY)Marshal.PtrToStructure(pEventBuf, typeof(DPSDK_MODIFY_DEVICE_NOTIFY));
                    string strModifyDevcieDeviceName = Encoding.GetEncoding("utf-8").GetString(Encoding.Default.GetBytes(struModifyDevcieNotify.struDevAllInfo.struDevInfo.szDeviceName));
                    strNotify = "\r\nModify device, old org code  = " + struModifyDevcieNotify.szOldOrgCode + ", new org code = " + struModifyDevcieNotify.szNewOrgCode + ", device ID = " + struModifyDevcieNotify.struDevAllInfo.struDevInfo.szDeviceID + ", device name = " + strModifyDevcieDeviceName;
                    ShowEventNotify(strNotify);
                    break;
                case DPSDK_EVENT_DELETE_DEVICE:
                    DPSDK_DELETE_DEVICE_NOTIFY struDeleteDeviceNotify = new DPSDK_DELETE_DEVICE_NOTIFY();
                    struDeleteDeviceNotify = (DPSDK_DELETE_DEVICE_NOTIFY)Marshal.PtrToStructure(pEventBuf, typeof(DPSDK_DELETE_DEVICE_NOTIFY));
                    strNotify = "\r\nDelete device, org code  = " + struDeleteDeviceNotify.szOrgCode + ", device ID = " + struDeleteDeviceNotify.szDeviceID;
                    ShowEventNotify(strNotify);
                    break;
                case DPSDK_EVENT_MOVE_DEVICE:
                    int iCount = (int)uiBufSize / Marshal.SizeOf(typeof(DPSDK_MOVE_DEVICE_NOTIFY));
                    for (int i = 0; i < iCount; i++)
                    {
                        IntPtr ptr = new IntPtr(pEventBuf.ToInt32() + i * Marshal.SizeOf((typeof(DPSDK_MOVE_DEVICE_NOTIFY))));
                        DPSDK_MOVE_DEVICE_NOTIFY struMoveDeviceNotify = new DPSDK_MOVE_DEVICE_NOTIFY();
                        struMoveDeviceNotify = (DPSDK_MOVE_DEVICE_NOTIFY)Marshal.PtrToStructure(ptr, typeof(DPSDK_MOVE_DEVICE_NOTIFY));
                        strNotify = "\r\nMove device, old org code  = " + struMoveDeviceNotify.szOldOrgCode + ", new org code = " + struMoveDeviceNotify.szNewOrgCode + ", device ID = " + struMoveDeviceNotify.szDeviceID;
                        ShowEventNotify(strNotify);
                    }
                    break;
                case DPSDK_EVENT_VTCALL_INVITE:

                    break;
                case DPSDK_EVENT_MEDIA_SCREENSHOT:
                    if (m_iConvertType == 0 || m_iConvertType == 1)
                    {
                        MediaEventNotify(pEventBuf);
                    }
                    break;
                default:
                    break;
            }
        }

        private void ShowEventNotify(string strNotify)
        {
            textResults.Text += strNotify;
        }

        private void btnLogin_Click(object sender, EventArgs e)
        {
            DPSDK_LOGIN_PARAM pLoginParam = new DPSDK_LOGIN_PARAM();
            pLoginParam.bDomainUser = (byte)(checkBoxDoMain.CheckState == CheckState.Checked ? 1 : 0);
            pLoginParam.szUserName = textUserName.Text;
            pLoginParam.szPWD = textPassword.Text;
            pLoginParam.struIP.szIP = textServerIP.Text;
            pLoginParam.uiPort = Convert.ToUInt32(textServerPort.Text);
            pLoginParam.uiClientType = 1;
            DPSDK_RES iRet = DPSDK_Login(ref pLoginParam, ref m_iSessionID);
            if (iRet == DPSDK_SUCCESS)
            {
                textResults.Text = AppData._CS("Login success");
                m_fEventCallbackFun = fEventCallBack;
                DPSDK_SetEventCallBack(m_iSessionID, m_fEventCallbackFun, /*IntPtr.Zero*/new IntPtr(1));

                Demo_Save_Login_Info_t struLoginInfo = new Demo_Save_Login_Info_t();
		        struLoginInfo.strIP = textServerIP.Text;
		        struLoginInfo.strPort = textServerPort.Text;
		        struLoginInfo.strUserName = textUserName.Text;
		        struLoginInfo.strPWD = textPassword.Text;
		        LocalConfig.SaveLastLoginInfo(struLoginInfo);
            }
            else
            {
                textResults.Text = AppData._CS("Login failed. error code = %1", iRet.ToString());
            }
        }

        private void btnLogout_Click(object sender, EventArgs e)
        {
            if (m_iSessionID != IntPtr.Zero)
            {
                DPSDK_Logout(m_iSessionID);
                m_iSessionID = IntPtr.Zero;
            }
            textResults.Text = AppData._CS("Logout success");
        }

        private void btnClientVersion_Click(object sender, EventArgs e)
        {
            byte[] pBuf = new byte[256];
            DPSDK_RES iRet = DPSDK_GetClientVersion(m_iSessionID, ref pBuf[0], 256);
            if (iRet == DPSDK_SUCCESS)
            {
                textResults.Text = AppData._CS("Get client version success, version = %1", System.Text.Encoding.Default.GetString(pBuf));
            }
            else
            {
                textResults.Text = AppData._CS("Get client version failed, error code = %1", iRet.ToString());
            }
        }

        private void btnClientRoad_Click(object sender, EventArgs e)
        {
            byte[] pBuf = new byte[256];
            DPSDK_RES iRet = DPSDK_GetClientRoad(m_iSessionID, ref pBuf[0], 256);
	        if (iRet == DPSDK_SUCCESS)
	        {
                textResults.Text = AppData._CS("Get client road success, road = %1", System.Text.Encoding.Default.GetString(pBuf));
	        }
	        else
	        {
                textResults.Text = AppData._CS("Get client road failed, error code = %1", iRet.ToString());
	        }
        }

        private void btnQueryEmap_Click(object sender, EventArgs e)
        {
            DPSDK_SERVER_INFO pServerInfo = new DPSDK_SERVER_INFO();
            DPSDK_RES iRet = DPSDK_GetEMapInfo(m_iSessionID, ref pServerInfo);
            if (iRet == DPSDK_SUCCESS)
            {
                textResults.Text = AppData._CS("Query e-map success. e-map server ip = %1, port = %2", pServerInfo.szIP, pServerInfo.uiPort.ToString());
            }
            else
            {
                textResults.Text = AppData._CS("Query e-map failed, error code = %1", iRet.ToString());
            }
        }

        private void btnQueryFTP_Click(object sender, EventArgs e)
        {
            uint uiBufLen = (uint)(Marshal.SizeOf(typeof(DPSDK_FTP_SERVER_INFO_LIST)) - Marshal.SizeOf(typeof(IntPtr)) + 3 * Marshal.SizeOf(typeof(DPSDK_FTP_SERVER_INFO)));
            IntPtr pFtpServerInfoList = Marshal.AllocHGlobal((int)uiBufLen);
            DPSDK_RES iRet = DPSDK_GetFtpInfo(m_iSessionID, (uint)DPSDK_FTP_DATA_TYPE.FTP_UNKNOW, pFtpServerInfoList, uiBufLen);
            if (iRet == DPSDK_SUCCESS)
            {
                DPSDK_FTP_SERVER_INFO_LIST struFtpServerInfoList = new DPSDK_FTP_SERVER_INFO_LIST();
                struFtpServerInfoList = (DPSDK_FTP_SERVER_INFO_LIST)Marshal.PtrToStructure(pFtpServerInfoList, typeof(DPSDK_FTP_SERVER_INFO_LIST));
                string strResult = AppData._CS("Query ftp info success. Ftp count is %1", struFtpServerInfoList.uiTotal.ToString());
                for (int i = 0; i < (int)struFtpServerInfoList.uiTotal; i++)
                {
                    IntPtr ptr = new IntPtr(pFtpServerInfoList.ToInt32() + Marshal.SizeOf(typeof(DPSDK_FTP_SERVER_INFO_LIST)) - Marshal.SizeOf(typeof(IntPtr)) + i * Marshal.SizeOf(typeof(DPSDK_FTP_SERVER_INFO)));
                    DPSDK_FTP_SERVER_INFO struFtpServerInfo = new DPSDK_FTP_SERVER_INFO();
                    struFtpServerInfo = (DPSDK_FTP_SERVER_INFO)Marshal.PtrToStructure(ptr, typeof(DPSDK_FTP_SERVER_INFO));
                    strResult += "\r\nFtp datatype is " + struFtpServerInfo.uiDataType.ToString() + ", ftp url = " + struFtpServerInfo.szUrl + ", ftp name = " + struFtpServerInfo.szUserName + ", ftp password = " + struFtpServerInfo.szPWD;
                }
                textResults.Text = strResult;
            }
            else
            {
                textResults.Text = AppData._CS("Query ftp info failed, error code = %1", iRet.ToString());
            }
            Marshal.FreeHGlobal(pFtpServerInfoList);
        }

        private void btnQueryMenuRight_Click(object sender, EventArgs e)
        {
            uint uiBufLen = (uint)(Marshal.SizeOf(typeof(DPSDK_MENU_RIGHT_LIST)) - Marshal.SizeOf(typeof(IntPtr)) + 100 * Marshal.SizeOf(typeof(DPSDK_MENU_RIGHT)));
            IntPtr pMenuRightList = Marshal.AllocHGlobal((int)uiBufLen);
            uint uiForbiddenLen = (uint)(Marshal.SizeOf(typeof(DPSDK_MENU_RIGHT_LIST)) - Marshal.SizeOf(typeof(IntPtr)) + 100 * Marshal.SizeOf(typeof(DPSDK_MENU_RIGHT)));
            IntPtr pForbiddenMenuRightList = Marshal.AllocHGlobal((int)uiForbiddenLen);
            DPSDK_RES iRet = DPSDK_GetMenuRight(m_iSessionID, pMenuRightList, uiBufLen, pForbiddenMenuRightList, uiForbiddenLen);
            if (iRet == DPSDK_SUCCESS)
            {
                DPSDK_MENU_RIGHT_LIST struMenuRightList = new DPSDK_MENU_RIGHT_LIST();
                struMenuRightList = (DPSDK_MENU_RIGHT_LIST)Marshal.PtrToStructure(pMenuRightList, typeof(DPSDK_MENU_RIGHT_LIST));
                string strResult = AppData._CS("Query menu right success. Menu right count = %1 menuRights =", struMenuRightList.uiTotal.ToString());
                for (int i = 0; i < (int)struMenuRightList.uiTotal; i++)
                {
                    IntPtr ptr = new IntPtr(pMenuRightList.ToInt32() + Marshal.SizeOf(typeof(DPSDK_MENU_RIGHT_LIST)) - Marshal.SizeOf(typeof(IntPtr)) + i * Marshal.SizeOf(typeof(DPSDK_MENU_RIGHT)));
                    DPSDK_MENU_RIGHT struMenuRight = new DPSDK_MENU_RIGHT();
                    struMenuRight = (DPSDK_MENU_RIGHT)Marshal.PtrToStructure(ptr, typeof(DPSDK_MENU_RIGHT));
                    if (i == (int)struMenuRightList.uiTotal - 1)
                    {
                        strResult += struMenuRight.szMenuRight;
                    }
                    else
                    {
                        strResult += struMenuRight.szMenuRight + ", ";
                    }
                }
                textResults.Text = strResult;
            }
            else
            {
                textResults.Text = AppData._CS("Query menu right failed. error code = %1", iRet.ToString());
            }
            Marshal.FreeHGlobal(pMenuRightList);
            Marshal.FreeHGlobal(pForbiddenMenuRightList);
        }

        private void btnSyncTime_Click(object sender, EventArgs e)
        {
            DPSDK_RES iRet = DPSDK_SyncTime(m_iSessionID);
            if (iRet == DPSDK_SUCCESS)
            {
                textResults.Text = AppData._CS("Sync time success");
            }
            else
            {
                textResults.Text = AppData._CS("Sync time failed. error code = %1", iRet.ToString());
            }
        }

        private void checkBoxHttps_CheckedChanged(object sender, EventArgs e)
        {
            IntPtr iHttpsMode = (IntPtr)(checkBoxHttps.CheckState == CheckState.Checked ? DPSDK_HTTPS_MODE.DPSDK_HTTPS_ONEWAY_AUTH : DPSDK_HTTPS_MODE.DPSDK_HTTPS_UNENABLE);
            DPSDK_SetHttpsMode(iHttpsMode);
        }

        private void btnModifyPassword_Click(object sender, EventArgs e)
        {
            DPSDK_RES iRet = DPSDK_ModifyUserPwd(m_iSessionID, textNewPassword.Text);
            if (iRet == DPSDK_SUCCESS)
            {
                textResults.Text = AppData._CS("Modify password success");
            }
            else
            {
                textResults.Text = AppData._CS("Modify password failed. error code = %1", iRet.ToString());
            }
        }
    }
}