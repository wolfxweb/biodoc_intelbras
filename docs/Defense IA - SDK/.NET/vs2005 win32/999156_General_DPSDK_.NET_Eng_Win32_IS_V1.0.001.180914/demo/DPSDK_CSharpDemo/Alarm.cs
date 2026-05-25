using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using System.Collections;
using DPSDK_RES = System.Int32;

namespace DPSDK_CSharpDemo
{
    public partial class CSharpDemo : Form
    {
        private void AlarmEventNotify(IntPtr pNotify)
        {
            if (checkBoxAlarmForbiddenNotify.CheckState == CheckState.Unchecked)
            {
                DPSDK_ALARMEVENT_NOTIFY struNotify = (DPSDK_ALARMEVENT_NOTIFY)Marshal.PtrToStructure(pNotify, typeof(DPSDK_ALARMEVENT_NOTIFY));
                string strShowInfo = AppData._CS("AlarmEventNotify.") + "\r\n";
                strShowInfo += "AlarmCode=" + struNotify.szAlarmCode + " AlarmNodeCode=" + struNotify.szAlarmNodeCode + " AlarmTime=" + struNotify.szAlarmTime + " AlarmGrade=" + struNotify.iAlarmGrade.ToString() +
                    " AlarmStatus=" + struNotify.iAlarmStatus.ToString() + " AlarmObjType=" + struNotify.iAlarmObjType.ToString() + " AlarmType=" + struNotify.iAlarmType.ToString() + " AlarmCategory=" +
                    struNotify.iAlarmCategory.ToString() + " AlarmMessage=" + struNotify.szAlarmMessage + ".\r\n";
                ShowAlarmInfo(strShowInfo);
            }
        }

        private void AlarmConfirmNotify(IntPtr pNotify)
        {
            if (checkBoxAlarmForbiddenNotify.CheckState == CheckState.Unchecked)
            {
                DPSDK_ALARMCONFIRM_NOTIFY struNotify = (DPSDK_ALARMCONFIRM_NOTIFY)Marshal.PtrToStructure(pNotify, typeof(DPSDK_ALARMCONFIRM_NOTIFY));
                string strShowInfo = AppData._CS("AlarmConfirmNotify.") + "\r\n";
                strShowInfo += "AlarmCode=" + struNotify.szAlarmCode + " AlarmTime=" + struNotify.szAlarmTime + " HandleStatus=" + struNotify.iHandleStatus.ToString() + " HandleUser=" + struNotify.szHandleUser + " HandleMessage=" + struNotify.szHandleMessage;
                for (uint i = 0; i < struNotify.uiEmailReceiverListSize; i++)
                {
                    strShowInfo += "  Email[" + i.ToString() + "]=" + struNotify.struEmailReceiverList[i].szEmailAddr;
                }
                strShowInfo += ".\r\n";
                ShowAlarmInfo(strShowInfo);
            }
        }

        private void AlarmPictureNotify(IntPtr pNotify)
        {
            if (checkBoxAlarmForbiddenNotify.CheckState == CheckState.Unchecked)
            {
                DPSDK_ALARM_DETAILINFO_NOTIFY struNotify = (DPSDK_ALARM_DETAILINFO_NOTIFY)Marshal.PtrToStructure(pNotify, typeof(DPSDK_ALARM_DETAILINFO_NOTIFY));
                string strShowInfo = AppData._CS("AlarmPictureNotify.") + "\r\n";
                strShowInfo += "AlarmCode=" + struNotify.szAlarmCode + " AlarmTime=" + struNotify.szAlarmTime + " AlarmPicture=" + struNotify.szAlarmPicture + " PictureSize=" + struNotify.uiAlarmPictureSize.ToString() + ".\r\n";
                ShowAlarmInfo(strShowInfo);
            }
        }

        private void AlarmExportNotify(IntPtr pNotify)
        {
            if (checkBoxAlarmForbiddenNotify.CheckState == CheckState.Unchecked)
            {
                DPSDK_ALARMEXPORT_RESULT_NOTIFY struNotify = (DPSDK_ALARMEXPORT_RESULT_NOTIFY)Marshal.PtrToStructure(pNotify, typeof(DPSDK_ALARMEXPORT_RESULT_NOTIFY));
                string strShowInfo = AppData._CS("AlarmExportNotify.") + "\r\n";
                strShowInfo += "SessionId=" + struNotify.uiSessionId.ToString() + " DownloadPath=" + struNotify.szDownloadPath + ".\r\n";
                ShowAlarmInfo(strShowInfo);
            }
        }

        private void ShowAlarmInfo(string strInfo)
        {
            if(checkBoxAlarmStopRefresh.CheckState == CheckState.Unchecked)
            {
                string strShow = strInfo + "\r\n" + textAlarmInfo.Text;
                textAlarmInfo.Text = strShow;
            }
        }

        private void StringSplit(string src, string sep, ref ArrayList dest)
        {
            string str = src;
            string subStr = "";
            int start = 0, index = 0;

            do 
            {
                index = str.IndexOf(sep, start);
                if (index != -1)
                {
                    subStr = str.Substring(start, index - start);
                    if (subStr.Length > 0 && subStr != sep)
                    {
                        dest.Add(subStr);
                    }
                    start = index + sep.Length;
                }
            } while (index != -1);

            subStr = str.Substring(start);
            if (subStr.Length > 0 && subStr != sep)
            {
                dest.Add(subStr);
            }
        }

        private void btnAlarmConfirm_Click(object sender, EventArgs e)
        {
            string strEmailLists = textAlarmEmailList.Text;
            ArrayList arrEmailLists = new ArrayList();
            StringSplit(strEmailLists, "\r\n", ref arrEmailLists);
            int iEmailNumber = arrEmailLists.Count;
	        int iParamSize = 0;
            if (iEmailNumber > 1)
            {
                iParamSize = Marshal.SizeOf(typeof(DPSDK_CONFIRMALARM_PARAM)) - Marshal.SizeOf(typeof(IntPtr)) + Marshal.SizeOf(typeof(DPSDK_EMAILADDRESS)) * iEmailNumber;
            }
            else
            {
                iParamSize = Marshal.SizeOf(typeof(DPSDK_CONFIRMALARM_PARAM)) - Marshal.SizeOf(typeof(IntPtr)) + Marshal.SizeOf(typeof(DPSDK_EMAILADDRESS));
            }

            IntPtr pConfirmAlarmParam = Marshal.AllocHGlobal(iParamSize);
            DPSDK_CONFIRMALARM_PARAM struConfirmAlarmParam = new DPSDK_CONFIRMALARM_PARAM();
            struConfirmAlarmParam.iHandleStatus = Convert.ToInt32(comboBoxAlarmHandleStatus.Text.Substring(0, comboBoxAlarmHandleStatus.Text.IndexOf("-")));
            struConfirmAlarmParam.szAlarmCode = textAlarmAlarmCode.Text;
            struConfirmAlarmParam.szHandleUser = textAlarmHandleUser.Text;
            struConfirmAlarmParam.szHandleMessage = textAlarmHandleMsg.Text;
            struConfirmAlarmParam.uiEmailRevceiverNumber = (uint)iEmailNumber;
            Marshal.StructureToPtr(struConfirmAlarmParam, pConfirmAlarmParam, true);
            for (int i = 0; i < iEmailNumber; i++)
            {
                DPSDK_EMAILADDRESS struEmailAddress = new DPSDK_EMAILADDRESS();
                struEmailAddress.szEmailAddr = (string)arrEmailLists[i];
                IntPtr ptr = new IntPtr(pConfirmAlarmParam.ToInt32() + Marshal.SizeOf(typeof(DPSDK_CONFIRMALARM_PARAM)) - Marshal.SizeOf(typeof(IntPtr)) + i * Marshal.SizeOf(typeof(DPSDK_EMAILADDRESS)));
                Marshal.StructureToPtr(struEmailAddress, ptr, true);
            }

            DPSDK_RES iRet = DPSDK_ConfirmAlarm(m_iSessionID, pConfirmAlarmParam);
            if (iRet == DPSDK_SUCCESS)
            {
                ShowAlarmInfo(AppData._CS("AlarmConfirm Success."));
            }
            else
            {
                ShowAlarmInfo(AppData._CS("AlarmConfirm failed. error code = %1", iRet.ToString()));
            }
            Marshal.FreeHGlobal(pConfirmAlarmParam);
        }

        private void btnAlarmQuery_Click(object sender, EventArgs e)
        {
            if (textAlarmQueryPageSize.Text == "" || textAlarmQueryPageNum.Text == "")
            {
                AppData.ShowMsgInfo(AppData._CS("Please input correct params!"));
                return;
            }

            DPSDK_QUERYALARM_PARAM pQueryAlarmParam = new DPSDK_QUERYALARM_PARAM();
            pQueryAlarmParam.iPageNo = Convert.ToInt32(textAlarmQueryPageNum.Text);
            pQueryAlarmParam.iPageSize = Convert.ToInt32(textAlarmQueryPageSize.Text);
            pQueryAlarmParam.iSortType = Convert.ToInt32(comboBoxAlarmQuerySortType.Text.Substring(0, comboBoxAlarmQuerySortType.Text.IndexOf("-")));
            pQueryAlarmParam.iSortOrder = Convert.ToInt32(comboBoxAlarmQuerySortOrder.Text.Substring(0, comboBoxAlarmQuerySortOrder.Text.IndexOf("-")));
            pQueryAlarmParam.szAlarmCode = textAlarmQueryAlarmCode.Text;
            pQueryAlarmParam.szAlarmId = textAlarmQueryAlarmId.Text;
            //pQueryAlarmParam.szDeviceId = textAlarmQueryDeviceId.Text;
            pQueryAlarmParam.iChannelIdNum = 1;
            pQueryAlarmParam.pChannelIdList = Marshal.AllocHGlobal(Marshal.SizeOf(typeof(DPSDK_CHANNEL_ID)));
            DPSDK_CHANNEL_ID struChannelId = new DPSDK_CHANNEL_ID();
            struChannelId.szId = textAlarmQueryChannelId.Text;
            Marshal.StructureToPtr(struChannelId, pQueryAlarmParam.pChannelIdList, true);

            pQueryAlarmParam.iDeviceIdNum = 1;
            pQueryAlarmParam.pDeviceIdList = Marshal.AllocHGlobal(Marshal.SizeOf(typeof(DPSDK_DEVICE_ID)));
            DPSDK_DEVICE_ID struDeviceId = new DPSDK_DEVICE_ID();
            struDeviceId.szId = textAlarmQueryDeviceId.Text;
            Marshal.StructureToPtr(struDeviceId, pQueryAlarmParam.pDeviceIdList, true);

            pQueryAlarmParam.szOrgId = textAlarmQueryOrgId.Text;
            pQueryAlarmParam.szHandleUser = textAlarmQueryHandleUser.Text;
            pQueryAlarmParam.szBeginTime = textAlarmQueryBeginTime.Text;
            pQueryAlarmParam.szEndTime = textAlarmQueryEndTime.Text;
            pQueryAlarmParam.szHandleBeginTime = textAlarmQueryHandleBeginTime.Text;
            pQueryAlarmParam.szHandleEndTime = textAlarmQueryHandleEndTime.Text;

            // AlarmTypeList
            string strAlarmType = textAlarmQueryTypeList.Text;
            ArrayList arrAlarmType = new ArrayList();
            StringSplit(strAlarmType, ";", ref arrAlarmType);
            pQueryAlarmParam.uiAlarmTypeNumber = (uint)arrAlarmType.Count;
            if (arrAlarmType.Count > 0)
            {
                pQueryAlarmParam.pAlarmType = Marshal.AllocHGlobal(Marshal.SizeOf(typeof(int)) * arrAlarmType.Count);
                for (int i = 0; i < arrAlarmType.Count; i++)
                {
                    IntPtr ptr = new IntPtr(pQueryAlarmParam.pAlarmType.ToInt32() + Marshal.SizeOf(typeof(int)) * i);
                    Marshal.StructureToPtr(Convert.ToInt32((string)arrAlarmType[i]), ptr, true);
                }
            }

            // AlarmGradeList
            string strAlarmGrade = textAlarmQueryGradeList.Text;
            ArrayList arrAlarmGrade = new ArrayList();
            StringSplit(strAlarmGrade, ";", ref arrAlarmGrade);
            pQueryAlarmParam.uiAlarmGradeNumber = (uint)arrAlarmGrade.Count;
            if (arrAlarmGrade.Count > 0)
            {
                pQueryAlarmParam.pAlarmGrade = Marshal.AllocHGlobal(Marshal.SizeOf(typeof(int)) * arrAlarmGrade.Count);
                for (int i = 0; i < arrAlarmGrade.Count; i++)
                {
                    IntPtr ptr = new IntPtr(pQueryAlarmParam.pAlarmGrade.ToInt32() + Marshal.SizeOf(typeof(int)) * i);
                    Marshal.StructureToPtr(Convert.ToInt32((string)arrAlarmGrade[i]), ptr, true);
                }
            }

            // AlarmStatusList
            string strAlarmStatus = textAlarmQueryStatusList.Text;
            ArrayList arrAlarmStatus = new ArrayList();
            StringSplit(strAlarmStatus, ";", ref arrAlarmStatus);
            pQueryAlarmParam.uiAlarmStatusNumber = (uint)arrAlarmStatus.Count;
            if (arrAlarmStatus.Count > 0)
            {
                pQueryAlarmParam.pAlarmStatus = Marshal.AllocHGlobal(Marshal.SizeOf(typeof(int)) * arrAlarmStatus.Count);
                for (int i = 0; i < arrAlarmStatus.Count; i++)
                {
                    IntPtr ptr = new IntPtr(pQueryAlarmParam.pAlarmStatus.ToInt32() + Marshal.SizeOf(typeof(int)) * i);
                    Marshal.StructureToPtr(Convert.ToInt32((string)arrAlarmStatus[i]), ptr, true);
                }
            }

            // HandleStatusList
            string strAlarmHandleStatus = textAlarmQueryHandleStatusList.Text;
            ArrayList arrAlarmHandleStatus = new ArrayList();
            StringSplit(strAlarmHandleStatus, ";", ref arrAlarmHandleStatus);
            pQueryAlarmParam.uiHandleStatusNumber = (uint)arrAlarmHandleStatus.Count;
            if (arrAlarmHandleStatus.Count > 0)
            {
                pQueryAlarmParam.pHandleStatus = Marshal.AllocHGlobal(Marshal.SizeOf(typeof(int)) * arrAlarmHandleStatus.Count);
                for (int i = 0; i < arrAlarmHandleStatus.Count; i++)
                {
                    IntPtr ptr = new IntPtr(pQueryAlarmParam.pHandleStatus.ToInt32() + Marshal.SizeOf(typeof(int)) * i);
                    Marshal.StructureToPtr(Convert.ToInt32((string)arrAlarmHandleStatus[i]), ptr, true);
                }
            }

            uint uiBufLen = 0;
            if (pQueryAlarmParam.iPageSize > 1)
	        {
                //Since C++ struct byte alignment，it is not need to sub IntPtr size
                uiBufLen = (uint)(Marshal.SizeOf(typeof(DPSDK_ALARM_DETAILINFO_LIST))/* - Marshal.SizeOf(typeof(IntPtr))*/ + Marshal.SizeOf(typeof(DPSDK_ALARM_DETAILINFO)) * pQueryAlarmParam.iPageSize);
	        }
	        else
	        {
                uiBufLen = (uint)(Marshal.SizeOf(typeof(DPSDK_ALARM_DETAILINFO_LIST))/* - Marshal.SizeOf(typeof(IntPtr))*/ + Marshal.SizeOf(typeof(DPSDK_ALARM_DETAILINFO)));
	        }
            IntPtr pAlarmDetailInfoList = Marshal.AllocHGlobal((int)uiBufLen);

            DPSDK_RES iRet = DPSDK_QueryAlarm(m_iSessionID, ref pQueryAlarmParam, uiBufLen, pAlarmDetailInfoList);
            if (iRet == DPSDK_SUCCESS)
            {
                string strShowInfo = AppData._CS("AlarmQuery Success.");
                strShowInfo += "\r\n";
                DPSDK_ALARM_DETAILINFO_LIST struAlarmDetailInfoList = new DPSDK_ALARM_DETAILINFO_LIST();
                struAlarmDetailInfoList = (DPSDK_ALARM_DETAILINFO_LIST)Marshal.PtrToStructure(pAlarmDetailInfoList, typeof(DPSDK_ALARM_DETAILINFO_LIST));
                for (int i = 0; i < (int)struAlarmDetailInfoList.uiTotal; i++)
                {
                    DPSDK_ALARM_DETAILINFO struAlarmDetailInfo = new DPSDK_ALARM_DETAILINFO();
                    IntPtr ptr = new IntPtr(pAlarmDetailInfoList.ToInt32() + Marshal.SizeOf(typeof(DPSDK_ALARM_DETAILINFO_LIST)) + Marshal.SizeOf(typeof(DPSDK_ALARM_DETAILINFO)) * i);
                    struAlarmDetailInfo = (DPSDK_ALARM_DETAILINFO)Marshal.PtrToStructure(ptr, typeof(DPSDK_ALARM_DETAILINFO));

                    string strSingleInfo = "AlarmId=" + struAlarmDetailInfo.szAlarmId + " DeviceCode=" + struAlarmDetailInfo.szDeviceId + " DeviceName=" + struAlarmDetailInfo.szDeviceName + " ChannelCode=" + struAlarmDetailInfo.szChannelId +
                        " ChannelName=" + struAlarmDetailInfo.szChannelName + " HandleUser=" + struAlarmDetailInfo.szHandleUser + " HandleTime=" + struAlarmDetailInfo.szHandleTime + " HandleMessage=" + struAlarmDetailInfo.szHandleMessage +
                        " AlarmCode=" + struAlarmDetailInfo.szAlarmCode + " AlarmTime=" + struAlarmDetailInfo.szAlarmTime + " AlarmGrade=" + struAlarmDetailInfo.iAlarmGrade.ToString() + " AlarmType=" + struAlarmDetailInfo.iAlarmType.ToString() +
                        " AlarmStatus=" + struAlarmDetailInfo.iAlarmStatus.ToString() + " HandleStatus=" + struAlarmDetailInfo.iHandleStatus.ToString() + " AlarmPicture=" + struAlarmDetailInfo.szAlarmPicture +
                        " AlarmPictureSize=" + struAlarmDetailInfo.uiAlarmPictureSize.ToString();
                    for (int j = 0; j < (int)struAlarmDetailInfo.uiEmailReceiverListSize; j++)
                    {
                        strSingleInfo += " Email[" + j.ToString() + "]=" + struAlarmDetailInfo.struEmailReceiverList[j].szEmailAddr;
                    }
                    strSingleInfo += ".\r\n";
                    strShowInfo += strSingleInfo;
                }
                ShowAlarmInfo(strShowInfo);
            }
            else
            {
                ShowAlarmInfo(AppData._CS("AlarmQuery failed. error code = %1", iRet.ToString() + ".\r\n"));
            }

            Marshal.FreeHGlobal(pQueryAlarmParam.pChannelIdList);
            Marshal.FreeHGlobal(pQueryAlarmParam.pDeviceIdList);
            if (arrAlarmType.Count > 0)
            {
                Marshal.FreeHGlobal(pQueryAlarmParam.pAlarmType);
            }
            if (arrAlarmGrade.Count > 0)
            {
                Marshal.FreeHGlobal(pQueryAlarmParam.pAlarmGrade);
            }
            if (arrAlarmStatus.Count > 0)
            {
                Marshal.FreeHGlobal(pQueryAlarmParam.pAlarmStatus);
            }
            if (arrAlarmHandleStatus.Count > 0)
            {
                Marshal.FreeHGlobal(pQueryAlarmParam.pHandleStatus);
            }
            Marshal.FreeHGlobal(pAlarmDetailInfoList);
        }

        private void btnAlarmTypeQuery_Click(object sender, EventArgs e)
        {
            string szLanguage = textAlarmTypeQueryLanguage.Text;
            if (szLanguage != "en_US" && szLanguage != "zh_CN")
            {
                szLanguage = "en_US";
            }
            IntPtr pAlarmTypeXml = IntPtr.Zero;
            DPSDK_RES iRet = DPSDK_GetAlarmTypeGroupInfo(m_iSessionID, szLanguage, ref pAlarmTypeXml);
            if (iRet == DPSDK_SUCCESS)
            {
                string strRetMsg = AppData._CS("GetAlarmType Success.") + "\r\n";
                if (pAlarmTypeXml != IntPtr.Zero)
                {
                    byte[] strAlarmTypeXml = new byte[10240];
                    Marshal.Copy(pAlarmTypeXml, strAlarmTypeXml, 0, 10240);
                    strRetMsg += "\r\n" + Encoding.GetEncoding("utf-8").GetString(strAlarmTypeXml);
                }
                textAlarmInfo.Text = strRetMsg;
            }
            else
            {
                textAlarmInfo.Text = AppData._CS("GetAlarmType failed. error code = %1", iRet.ToString());
            }
            DPSDK_ReleaseDataBuffer(pAlarmTypeXml);
        }

        private void btnAlarmCountQuery_Click(object sender, EventArgs e)
        {
            DPSDK_QUERYALARMCOUNT_PARAM pQueryAlarmCountParam = new DPSDK_QUERYALARMCOUNT_PARAM();
            pQueryAlarmCountParam.szAlarmCode = textAlarmQueryAlarmCode.Text;
            pQueryAlarmCountParam.szAlarmId = textAlarmQueryAlarmId.Text;
            //pQueryAlarmCountParam.szDeviceId = textAlarmQueryDeviceId.Text;
            pQueryAlarmCountParam.iChannelIdNum = 1;
            pQueryAlarmCountParam.pChannelIdList = Marshal.AllocHGlobal(Marshal.SizeOf(typeof(DPSDK_CHANNEL_ID)));
            DPSDK_CHANNEL_ID struChannelId = new DPSDK_CHANNEL_ID();
            struChannelId.szId = textAlarmQueryChannelId.Text;
            Marshal.StructureToPtr(struChannelId, pQueryAlarmCountParam.pChannelIdList, true);

            pQueryAlarmCountParam.iDeviceIdNum = 1;
            pQueryAlarmCountParam.pDeviceIdList = Marshal.AllocHGlobal(Marshal.SizeOf(typeof(DPSDK_DEVICE_ID)));
            DPSDK_DEVICE_ID struDeviceId = new DPSDK_DEVICE_ID();
            struDeviceId.szId = textAlarmQueryDeviceId.Text;
            Marshal.StructureToPtr(struDeviceId, pQueryAlarmCountParam.pDeviceIdList, true);

            pQueryAlarmCountParam.szOrgId = textAlarmQueryOrgId.Text;
            pQueryAlarmCountParam.szHandleUser = textAlarmQueryHandleUser.Text;
            pQueryAlarmCountParam.szBeginTime = textAlarmQueryBeginTime.Text;
            pQueryAlarmCountParam.szEndTime = textAlarmQueryEndTime.Text;
            pQueryAlarmCountParam.szHandleBeginTime = textAlarmQueryHandleBeginTime.Text;
            pQueryAlarmCountParam.szHandleEndTime = textAlarmQueryHandleEndTime.Text;

            // AlarmTypeList
            string strAlarmType = textAlarmQueryTypeList.Text;
            ArrayList arrAlarmType = new ArrayList();
            StringSplit(strAlarmType, ";", ref arrAlarmType);
            pQueryAlarmCountParam.uiAlarmTypeNumber = (uint)arrAlarmType.Count;
            if (arrAlarmType.Count > 0)
            {
                pQueryAlarmCountParam.pAlarmType = Marshal.AllocHGlobal(Marshal.SizeOf(typeof(int)) * arrAlarmType.Count);
                for(int i = 0; i < arrAlarmType.Count; i++)
                {
                    IntPtr ptr = new IntPtr(pQueryAlarmCountParam.pAlarmType.ToInt32() + Marshal.SizeOf(typeof(int)) * i);
                    Marshal.StructureToPtr(Convert.ToInt32((string)arrAlarmType[i]), ptr, true);
                }
            }

            // AlarmGradeList
            string strAlarmGrade = textAlarmQueryGradeList.Text;
            ArrayList arrAlarmGrade = new ArrayList();
            StringSplit(strAlarmGrade, ";", ref arrAlarmGrade);
            pQueryAlarmCountParam.uiAlarmGradeNumber = (uint)arrAlarmGrade.Count;
            if (arrAlarmGrade.Count > 0)
            {
                pQueryAlarmCountParam.pAlarmGrade = Marshal.AllocHGlobal(Marshal.SizeOf(typeof(int)) * arrAlarmGrade.Count);
                for (int i = 0; i < arrAlarmGrade.Count; i++)
                {
                    IntPtr ptr = new IntPtr(pQueryAlarmCountParam.pAlarmGrade.ToInt32() + Marshal.SizeOf(typeof(int)) * i);
                    Marshal.StructureToPtr(Convert.ToInt32((string)arrAlarmGrade[i]), ptr, true);
                }
            }

            // AlarmStatusList
            string strAlarmStatus = textAlarmQueryStatusList.Text;
            ArrayList arrAlarmStatus = new ArrayList();
            StringSplit(strAlarmStatus, ";", ref arrAlarmStatus);
            pQueryAlarmCountParam.uiAlarmStatusNumber = (uint)arrAlarmStatus.Count;
            if (arrAlarmStatus.Count > 0)
            {
                pQueryAlarmCountParam.pAlarmStatus = Marshal.AllocHGlobal(Marshal.SizeOf(typeof(int)) * arrAlarmStatus.Count);
                for (int i = 0; i < arrAlarmStatus.Count; i++)
                {
                    IntPtr ptr = new IntPtr(pQueryAlarmCountParam.pAlarmStatus.ToInt32() + Marshal.SizeOf(typeof(int)) * i);
                    Marshal.StructureToPtr(Convert.ToInt32((string)arrAlarmStatus[i]), ptr, true);
                }
            }

            // HandleStatusList
            string strAlarmHandleStatus = textAlarmQueryHandleStatusList.Text;
            ArrayList arrAlarmHandleStatus = new ArrayList();
            StringSplit(strAlarmHandleStatus, ";", ref arrAlarmHandleStatus);
            pQueryAlarmCountParam.uiHandleStatusNumber = (uint)arrAlarmHandleStatus.Count;
            if (arrAlarmHandleStatus.Count > 0)
            {
                pQueryAlarmCountParam.pHandleStatus = Marshal.AllocHGlobal(Marshal.SizeOf(typeof(int)) * arrAlarmHandleStatus.Count);
                for (int i = 0; i < arrAlarmHandleStatus.Count; i++)
                {
                    IntPtr ptr = new IntPtr(pQueryAlarmCountParam.pHandleStatus.ToInt32() + Marshal.SizeOf(typeof(int)) * i);
                    Marshal.StructureToPtr(Convert.ToInt32((string)arrAlarmHandleStatus[i]), ptr, true);
                }
            }

            IntPtr pAlarmCount = IntPtr.Zero;
            DPSDK_RES iRet = DPSDK_QueryAlarmCount(m_iSessionID, ref pQueryAlarmCountParam, ref pAlarmCount);
            if (iRet == DPSDK_SUCCESS)
            {
                ShowAlarmInfo(AppData._CS("QueryAlarmCount Success,alarm count=%1.", pAlarmCount.ToString() + "\r\n"));
            }
            else
            {
                ShowAlarmInfo(AppData._CS("QueryAlarmCount failed. error code = %1", iRet.ToString() + ".\r\n"));
            }

            Marshal.FreeHGlobal(pQueryAlarmCountParam.pChannelIdList);
            Marshal.FreeHGlobal(pQueryAlarmCountParam.pDeviceIdList);
            if (arrAlarmType.Count > 0)
            {
                Marshal.FreeHGlobal(pQueryAlarmCountParam.pAlarmType);
            }
            if (arrAlarmGrade.Count > 0)
            {
                Marshal.FreeHGlobal(pQueryAlarmCountParam.pAlarmGrade);
            }
            if (arrAlarmStatus.Count > 0)
            {
                Marshal.FreeHGlobal(pQueryAlarmCountParam.pAlarmStatus);
            }
            if (arrAlarmHandleStatus.Count > 0)
            {
                Marshal.FreeHGlobal(pQueryAlarmCountParam.pHandleStatus);
            }
        }

        private void btnAlarmQueryPF_Click(object sender, EventArgs e)
        {
            uint uiBufLen = (uint)(Marshal.SizeOf(typeof(DPSDK_ALARMPROCESS_DETAILINFO_LIST)) - Marshal.SizeOf(typeof(IntPtr)) + 3 * Marshal.SizeOf(typeof(DPSDK_ALARMPROCESS_DETAILINFO)));//一共只取3条记录
            IntPtr pAlarmProcessInfoList = Marshal.AllocHGlobal((int)uiBufLen);
            DPSDK_RES iRet = DPSDK_QueryAlarmProcessFlow(m_iSessionID, textAlarmQueryPFAlarmCode.Text, uiBufLen, pAlarmProcessInfoList);
            if( iRet == DPSDK_SUCCESS)
	        {
                string strRetMsg = AppData._CS("AlarmQueryPF Success.") + "\r\n";
                DPSDK_ALARMPROCESS_DETAILINFO_LIST struAlarmProcessInfoList = new DPSDK_ALARMPROCESS_DETAILINFO_LIST();
                struAlarmProcessInfoList = (DPSDK_ALARMPROCESS_DETAILINFO_LIST)Marshal.PtrToStructure(pAlarmProcessInfoList, typeof(DPSDK_ALARMPROCESS_DETAILINFO_LIST));
                for (int i = 0; i < (int)struAlarmProcessInfoList.uiTotal && i < 3; i++)
                {
                    DPSDK_ALARMPROCESS_DETAILINFO struAlarmProcessDetailInfo = new DPSDK_ALARMPROCESS_DETAILINFO();
                    IntPtr ptr = new IntPtr(pAlarmProcessInfoList.ToInt32() + Marshal.SizeOf(typeof(DPSDK_ALARMPROCESS_DETAILINFO_LIST)) - Marshal.SizeOf(typeof(IntPtr)) + Marshal.SizeOf(typeof(DPSDK_ALARMPROCESS_DETAILINFO)) * i);
                    struAlarmProcessDetailInfo = (DPSDK_ALARMPROCESS_DETAILINFO)Marshal.PtrToStructure(ptr, typeof(DPSDK_ALARMPROCESS_DETAILINFO));
                    strRetMsg += "HandleUser = " + struAlarmProcessDetailInfo.szHandleUser + " HandleTime = " + struAlarmProcessDetailInfo.szHandleTime + " HandleMessage = " + struAlarmProcessDetailInfo.szHandleMessage +
                        " HandleStatus = " + struAlarmProcessDetailInfo.iHandleStatus.ToString() + ".\r\n";
                }
                ShowAlarmInfo(strRetMsg);
	        }
	        else
	        {
                ShowAlarmInfo(AppData._CS("AlarmQueryPF failed. error code = %1", iRet.ToString() + ".\r\n"));
	        }
            Marshal.FreeHGlobal(pAlarmProcessInfoList);
        }

        private void btnAlarmBlock_Click(object sender, EventArgs e)
        {
            if (textAlarmBlockType.Text == "" || textAlarmBlockDuration.Text == "")
            {
                AppData.ShowMsgInfo(AppData._CS("Please input correct params!"));
                return;
            }

            DPSDK_BLOCKALARM_PARAM pBlockAlarmParam = new DPSDK_BLOCKALARM_PARAM();
            pBlockAlarmParam.szAlarmCodeSource = textAlarmCodeSource.Text;
            pBlockAlarmParam.iAlarmType = Convert.ToInt32(textAlarmBlockType.Text);
            pBlockAlarmParam.iDuration = Convert.ToInt32(textAlarmBlockDuration.Text);
            DPSDK_RES iRet = DPSDK_BlockAlarm(m_iSessionID, ref pBlockAlarmParam);
            if (iRet == DPSDK_SUCCESS)
            {
                ShowAlarmInfo(AppData._CS("AlarmBlock Success.") + "\r\n");
            }
            else
            {
                ShowAlarmInfo(AppData._CS("AlarmBlock failed. error code = %1", iRet.ToString() + "\r\n"));
            }
        }

        private void btnAlarmExport_Click(object sender, EventArgs e)
        {
            DPSDK_ALARMEXPORT_PARAM pAlarmExportParam = new DPSDK_ALARMEXPORT_PARAM();
            pAlarmExportParam.iSortType = Convert.ToInt32(comboBoxAlarmExportSortType.Text.Substring(0, comboBoxAlarmExportSortType.Text.IndexOf("-")));
            pAlarmExportParam.iSortOrder = Convert.ToInt32(comboBoxAlarmExportSortOrder.Text.Substring(0, comboBoxAlarmExportSortOrder.Text.IndexOf("-")));
            pAlarmExportParam.szAlarmCode = textAlarmExportAlarmCode.Text;
            pAlarmExportParam.szAlarmId = textAlarmExportAlarmId.Text;
            //pAlarmExportParam.szDeviceId = textAlarmExportDeviceId.Text;
            pAlarmExportParam.iChannelIdNum = 1;
            pAlarmExportParam.pChannelIdList = Marshal.AllocHGlobal(Marshal.SizeOf(typeof(DPSDK_CHANNEL_ID)));
            DPSDK_CHANNEL_ID struChannelId = new DPSDK_CHANNEL_ID();
            struChannelId.szId = textAlarmExportChannelId.Text;
            Marshal.StructureToPtr(struChannelId, pAlarmExportParam.pChannelIdList, true);

            pAlarmExportParam.iDeviceIdNum = 1;
            pAlarmExportParam.pDeviceIdList = Marshal.AllocHGlobal(Marshal.SizeOf(typeof(DPSDK_DEVICE_ID)));
            DPSDK_DEVICE_ID struDeviceId = new DPSDK_DEVICE_ID();
            struDeviceId.szId = textAlarmExportDeviceId.Text;
            Marshal.StructureToPtr(struDeviceId, pAlarmExportParam.pDeviceIdList, true);

            pAlarmExportParam.szOrgId = textAlarmExportOrgId.Text;
            pAlarmExportParam.szHandleUser = textAlarmExportHandleUser.Text;
            pAlarmExportParam.szBeginTime = textAlarmExportBeginTime.Text;
            pAlarmExportParam.szEndTime = textAlarmExportEndTime.Text;
            pAlarmExportParam.szHandleBeginTime = textAlarmExportHandleBeginTime.Text;
            pAlarmExportParam.szHandleEndTime = textAlarmExportHandleEndTime.Text;
            pAlarmExportParam.szLanguage = textAlarmExportLanguage.Text;

            // AlarmTypeList
            string strAlarmType = textAlarmExportTypeList.Text;
            ArrayList arrAlarmType = new ArrayList();
            StringSplit(strAlarmType, ";", ref arrAlarmType);
            pAlarmExportParam.uiAlarmTypeNumber = (uint)arrAlarmType.Count;
            if (arrAlarmType.Count > 0)
            {
                pAlarmExportParam.pAlarmType = Marshal.AllocHGlobal(Marshal.SizeOf(typeof(int)) * arrAlarmType.Count);
                for (int i = 0; i < arrAlarmType.Count; i++)
                {
                    IntPtr ptr = new IntPtr(pAlarmExportParam.pAlarmType.ToInt32() + Marshal.SizeOf(typeof(int)) * i);
                    Marshal.StructureToPtr(Convert.ToInt32((string)arrAlarmType[i]), ptr, true);
                }
            }

            // AlarmGradeList
            string strAlarmGrade = textAlarmExportGradeList.Text;
            ArrayList arrAlarmGrade = new ArrayList();
            StringSplit(strAlarmGrade, ";", ref arrAlarmGrade);
            pAlarmExportParam.uiAlarmGradeNumber = (uint)arrAlarmGrade.Count;
            if (arrAlarmGrade.Count > 0)
            {
                pAlarmExportParam.pAlarmGrade = Marshal.AllocHGlobal(Marshal.SizeOf(typeof(int)) * arrAlarmGrade.Count);
                for (int i = 0; i < arrAlarmGrade.Count; i++)
                {
                    IntPtr ptr = new IntPtr(pAlarmExportParam.pAlarmGrade.ToInt32() + Marshal.SizeOf(typeof(int)) * i);
                    Marshal.StructureToPtr(Convert.ToInt32((string)arrAlarmGrade[i]), ptr, true);
                }
            }

            // AlarmStatusList
            string strAlarmStatus = textAlarmExportStatusList.Text;
            ArrayList arrAlarmStatus = new ArrayList();
            StringSplit(strAlarmStatus, ";", ref arrAlarmStatus);
            pAlarmExportParam.uiAlarmStatusNumber = (uint)arrAlarmStatus.Count;
            if (arrAlarmStatus.Count > 0)
            {
                pAlarmExportParam.pAlarmStatus = Marshal.AllocHGlobal(Marshal.SizeOf(typeof(int)) * arrAlarmStatus.Count);
                for (int i = 0; i < arrAlarmStatus.Count; i++)
                {
                    IntPtr ptr = new IntPtr(pAlarmExportParam.pAlarmStatus.ToInt32() + Marshal.SizeOf(typeof(int)) * i);
                    Marshal.StructureToPtr(Convert.ToInt32((string)arrAlarmStatus[i]), ptr, true);
                }
            }

            // HandleStatusList
            string strAlarmHandleStatus = textAlarmExportHandleStatusList.Text;
            ArrayList arrAlarmHandleStatus = new ArrayList();
            StringSplit(strAlarmHandleStatus, ";", ref arrAlarmHandleStatus);
            pAlarmExportParam.uiHandleStatusNumber = (uint)arrAlarmHandleStatus.Count;
            if (arrAlarmHandleStatus.Count > 0)
            {
                pAlarmExportParam.pHandleStatus = Marshal.AllocHGlobal(Marshal.SizeOf(typeof(int)) * arrAlarmHandleStatus.Count);
                for (int i = 0; i < arrAlarmHandleStatus.Count; i++)
                {
                    IntPtr ptr = new IntPtr(pAlarmExportParam.pHandleStatus.ToInt32() + Marshal.SizeOf(typeof(int)) * i);
                    Marshal.StructureToPtr(Convert.ToInt32((string)arrAlarmHandleStatus[i]), ptr, true);
                }
            }

            DPSDK_RES iRet = DPSDK_ExportAlarms(m_iSessionID, ref pAlarmExportParam, ++m_iAlarmExportSessionID);
            if (iRet == DPSDK_SUCCESS)
            {
                textAlarmInfo.Text = AppData._CS("AlarmExport Success.");
            }
            else
            {
                textAlarmInfo.Text = AppData._CS("AlarmExport failed. error code = %1", iRet.ToString());
            }

            Marshal.FreeHGlobal(pAlarmExportParam.pChannelIdList);
            Marshal.FreeHGlobal(pAlarmExportParam.pDeviceIdList);
            if (arrAlarmType.Count > 0)
            {
                Marshal.FreeHGlobal(pAlarmExportParam.pAlarmType);
            }
            if (arrAlarmGrade.Count > 0)
            {
                Marshal.FreeHGlobal(pAlarmExportParam.pAlarmGrade);
            }
            if (arrAlarmStatus.Count > 0)
            {
                Marshal.FreeHGlobal(pAlarmExportParam.pAlarmStatus);
            }
            if (arrAlarmHandleStatus.Count > 0)
            {
                Marshal.FreeHGlobal(pAlarmExportParam.pHandleStatus);
            }
        }

        private void btnAlarmClear_Click(object sender, EventArgs e)
        {
            textAlarmInfo.Text = "";
        }
    }
}