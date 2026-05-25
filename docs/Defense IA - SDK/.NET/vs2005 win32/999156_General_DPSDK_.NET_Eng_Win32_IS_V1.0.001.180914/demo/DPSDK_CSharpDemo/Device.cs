using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.IO;
using System.Runtime.InteropServices;
using System.Collections;
using DPSDK_RES = System.Int32;

namespace DPSDK_CSharpDemo
{
    public partial class CSharpDemo : Form
    {
        private void btnDeviceQueryOrg_Click(object sender, EventArgs e)
        {
            ClearOrgAndDevData();

	        int iChannelTypeCount = 0;
	        uint uiQueryLen = (uint)(Marshal.SizeOf(typeof(DPSDK_QUERY_ORG_INFO)) - Marshal.SizeOf(typeof(IntPtr)) + iChannelTypeCount * Marshal.SizeOf(typeof(int)));
            IntPtr pQueryOrgInfo = Marshal.AllocHGlobal((int)uiQueryLen);
            DPSDK_QUERY_ORG_INFO struQueryOrgInfo = new DPSDK_QUERY_ORG_INFO();
            struQueryOrgInfo.szOrgCode = textDeviceOrgCode.Text;
            Marshal.StructureToPtr(struQueryOrgInfo, pQueryOrgInfo, true);

            //for ( int i = 0; i < iChannelTypeCount; ++i )
            //{
            //    pQueryOrgInfo->iChannelTypeList[i] = vecChannelType[i];
            //}

            DPSDK_RES iRet = DPSDK_GetOrganization(m_iSessionID, pQueryOrgInfo, uiQueryLen, fDataCallback, IntPtr.Zero);
            Marshal.FreeHGlobal(pQueryOrgInfo);
	        if ( iRet == DPSDK_SUCCESS )
	        {
		        HandleGetAllDeviceInfo(true);

		        string strOrgXml = "";
                PacketOrgXml(ref m_struDepInfoAll, ref strOrgXml);
                textDeviceOrgData.Text = strOrgXml;
	        }
	        else
	        {
		        AppData.ShowMsgInfo(AppData._CS("Get Organization failed. error code = %1", iRet.ToString()));
	        }
        }

        private void btnDeviceQueryByLayered_Click(object sender, EventArgs e)
        {
            if (!LocalConfig.IsVSL())
            {
                btnDeviceQueryOrg_Click(sender, e);
                return;
            }
            ClearOrgAndDevData();

            string strOrgCode = textDeviceOrgCode.Text;
            DPSDK_RES iRet = HandleGeDeviceTreeByLayered(strOrgCode, 1);
            if (iRet == DPSDK_SUCCESS)
            {
                HandleGetAllDeviceInfo(false);

                string strOrgXml = "";
                PacketOrgXml(ref m_struDepInfoAll, ref strOrgXml);
                textDeviceOrgData.Text = strOrgXml;
            }
            else
            {
                AppData.ShowMsgInfo(AppData._CS("Get Device ByLayered failed. error code = %1", iRet.ToString()));
            }
        }

        private void checkBoxDeviceCompress_CheckedChanged(object sender, EventArgs e)
        {
            IntPtr iCompressType = (IntPtr)(checkBoxDeviceCompress.CheckState == CheckState.Checked ? DPSDK_COMPRESS_TYPE.COMPRESS_DEFAULT : DPSDK_COMPRESS_TYPE.COMPRESS_DISABLE);
            DPSDK_RES iRet = DPSDK_SetCompressType(iCompressType);
	        if (iRet == DPSDK_SUCCESS)
            {
                AppData.ShowMsgInfo("SetCompressType success.");
	        }
	        else
	        {
                AppData.ShowMsgInfo(AppData._CS("SetCompressType Failed. error code = %1", iRet.ToString()));
	        }
        }

        private void btnDeviceGetUserData_Click(object sender, EventArgs e)
        {
            textDeviceUserDataInfo.Text = "";
            IntPtr pUserData = IntPtr.Zero;
            DPSDK_RES iRet = DPSDK_GetUserData(m_iSessionID, textDeviceUserDataName.Text, ref pUserData);
            if (iRet == DPSDK_SUCCESS)
            {
                if (pUserData != IntPtr.Zero)
                {
                    byte[] strUserData = new byte[10240];
                    Marshal.Copy(pUserData, strUserData, 0, 10240);
                    textDeviceUserDataInfo.Text = Encoding.GetEncoding("utf-8").GetString(strUserData);
                    DPSDK_ReleaseDataBuffer(pUserData);
                }  
                AppData.ShowMsgInfo("Get User Data success.");
            }
            else
            {
                AppData.ShowMsgInfo(AppData._CS("Get User Data Failed. error code = %1", iRet.ToString()));
            }
        }

        private void btnDeviceSaveUserData_Click(object sender, EventArgs e)
        {
            DPSDK_RES iRet = DPSDK_SaveUserData(m_iSessionID, textDeviceUserDataName.Text, textDeviceUserDataInfo.Text, (uint)textDeviceUserDataInfo.Text.Length);
            if (iRet == DPSDK_SUCCESS)
            {
                AppData.ShowMsgInfo("Save User Data success.");
            }
            else
            {
                AppData.ShowMsgInfo(AppData._CS("Save User Data Failed. error code = %1", iRet.ToString()));
            }
        }

        private void btnDeviceDelUserData_Click(object sender, EventArgs e)
        {
            DPSDK_RES iRet = DPSDK_DelUserData(m_iSessionID, textDeviceUserDataName.Text);
            if (iRet == DPSDK_SUCCESS)
            {
                textDeviceUserDataName.Text = "";
                textDeviceUserDataInfo.Text = "";
                AppData.ShowMsgInfo("Del User Data success.");
            }
            else
            {
                AppData.ShowMsgInfo(AppData._CS("Del User Data Failed. error code = %1", iRet.ToString()));
            }
        }

        private void radioButtonDeviceOrg_CheckedChanged(object sender, EventArgs e)
        {
            if (radioButtonDeviceOrg.Checked)
            {
                DisplayOrgTree();
            }
        }

        private void radioButtonDeviceDevice_CheckedChanged(object sender, EventArgs e)
        {
            if (radioButtonDeviceDevice.Checked)
            {
                DisplayDeviceTree();
            }
        }

        private void radioButtonDeviceChannel_CheckedChanged(object sender, EventArgs e)
        {
            if (radioButtonDeviceChannel.Checked)
            {
                DisplayChannelTree();
            }
        }

        private void ClearOrgAndDevData()
        {
            m_struDevInfoAll.vecDevInfo.Clear();

            m_struDepInfoAll.vecChnlID.Clear();
            m_struDepInfoAll.vecDevID.Clear();
            m_struDepInfoAll.vecSubDepInfo.Clear();
            m_struDepInfoAll.depInfo.strCoding = "";
            m_struDepInfoAll.depInfo.strDepName = "";
            m_struDepInfoAll.depInfo.strSN = "";
            m_struDepInfoAll.depInfo.nDepSort = 0;
            m_struDepInfoAll.depInfo.nDepType = 0;
            m_struDepInfoAll.depInfo.isParent = 0;

            //m_mapIdName.clear();
            treeViewDeviceOrg.Nodes.Clear();
        }

        private DPSDK_RES HandleGeDeviceTreeByLayered([MarshalAs(UnmanagedType.LPStr)] string pParentId, int nNodeType)
        {
            uint uiPageNum = 0;
            uint uiPageSize = 100;
            DPSDK_PAGE_INFO pPageInfo = new DPSDK_PAGE_INFO();
            pPageInfo.uiPage = uiPageNum;
            pPageInfo.uiPageSize = uiPageSize;

            IntPtr uiTotal = IntPtr.Zero;
            DPSDK_GET_DEVICE_LAYERED_PARAM pParam = new DPSDK_GET_DEVICE_LAYERED_PARAM();
            pParam.iOrgType = 1;
            pParam.iShowDev = checkBoxDeviceContainDevice.CheckState == CheckState.Checked ? 1 : 0;
            pParam.iDeep = 2 + pParam.iShowDev;
            pParam.iNodeType = nNodeType;
            pParam.szID = pParentId;
            Dep_Info_All? depChild = FinOrgInfo(ref m_struDepInfoAll, pParentId);
            if (depChild == null)
            {
                depChild = m_struDepInfoAll;
            }

            DPSDK_RES iRet = DPSDK_SUCCESS;
            do
            {
                uiPageNum++;
                pPageInfo.uiPage = uiPageNum;
                iRet = DPSDK_GetDeviceByLayered(m_iSessionID, ref pParam, ref pPageInfo, ref uiTotal, fDataCallback, Marshal.StringToHGlobalAnsi(pParentId));
            } while ((uiPageNum * uiPageSize < (uint)uiTotal.ToInt32()) && iRet == DPSDK_SUCCESS);

            if (iRet == DPSDK_SUCCESS)
            {
                if (pParentId.Length == 0)
                {
                    iRet = HandleGeDeviceTreeByLayered(depChild.Value.depInfo.strCoding, nNodeType);
                }
                else
                {
                    for (int i = 0; i < depChild.Value.vecSubDepInfo.Count; i++)
                    {
                        if (((Dep_Info_All)depChild.Value.vecSubDepInfo[i]).depInfo.isParent == 1)
                        {
                            iRet = HandleGeDeviceTreeByLayered(((Dep_Info_All)depChild.Value.vecSubDepInfo[i]).depInfo.strCoding, nNodeType);
                        }
                    }
                }
            }
            return iRet;
        }

        private void fDataCallback(IntPtr iDataType, IntPtr pDataBuf, IntPtr uiBufSize, IntPtr pUserData)
        {
            if (iDataType.ToInt32() == DPSDK_DATA_ORG_INFO)
            {
                DPSDK_ORG_INFO pOrgInfo = (DPSDK_ORG_INFO)Marshal.PtrToStructure(pDataBuf, typeof(DPSDK_ORG_INFO));
                CopyOrgData(ref pOrgInfo, ref m_struDepInfoAll);
            }
            else if (iDataType.ToInt32() == DPSDK_DATA_ALL_ORG_INFO)
            {

            }
            else if (iDataType.ToInt32() == DPSDK_DATA_DEVICE_INFO)
            {
                DPSDK_DEV_ALL_INFO_LIST pDevAllInfoList = (DPSDK_DEV_ALL_INFO_LIST)Marshal.PtrToStructure(pDataBuf, typeof(DPSDK_DEV_ALL_INFO_LIST));
                CopyDevData(ref pDevAllInfoList, ref m_struDevInfoAll);
            }
            else if (iDataType.ToInt32() == DPSDK_DATA_DEVICE_LAYERED)
            {
                DPSDK_LAYERED_RESULT_LIST pSrcInfo = (DPSDK_LAYERED_RESULT_LIST)Marshal.PtrToStructure(pDataBuf, typeof(DPSDK_LAYERED_RESULT_LIST));
                string pParentId = Marshal.PtrToStringAnsi(pUserData);
                FinOrgAndCopyDevTree(pParentId, ref pSrcInfo, ref m_struDepInfoAll);
            }
        }            

        private Dep_Info_All? FinOrgInfo(ref Dep_Info_All depCur, [MarshalAs(UnmanagedType.LPStr)] string pParentId)
        {
            if (depCur.depInfo.strCoding == pParentId)
            {
                return depCur;
            }

            for (int i = 0; i < depCur.vecSubDepInfo.Count; i++)
            {
                Dep_Info_All dep = (Dep_Info_All)depCur.vecSubDepInfo[i];
                if (dep.depInfo.strCoding == pParentId)
                {
                    return dep;
                }

                Dep_Info_All? depChild = FinOrgInfo(ref dep, pParentId);
                if (depChild != null)
                {
                    return depChild;
                }
            }
            return null;
        }

        private void CopyOrgData(ref DPSDK_ORG_INFO pOrgInfo, ref Dep_Info_All struDep)
        {
            struDep.vecChnlID.Clear();
            struDep.vecDevID.Clear();
            struDep.vecSubDepInfo.Clear();
            struDep.depInfo.strCoding = pOrgInfo.struOrgBaseInfo.szOrgCode;
            struDep.depInfo.strDepName = pOrgInfo.struOrgBaseInfo.szOrgName;
            struDep.depInfo.strSN = pOrgInfo.struOrgBaseInfo.szOrgSN;
            struDep.depInfo.nDepSort = pOrgInfo.struOrgBaseInfo.iOrgSort;
            struDep.depInfo.nDepType = pOrgInfo.struOrgBaseInfo.iOrgType;

            if (pOrgInfo.iDevNum > 0)
            {
                DPSDK_ORG_SUB_DEV_INFO[] pDevList = new DPSDK_ORG_SUB_DEV_INFO[pOrgInfo.iDevNum];
                //pDevList[0] = (DPSDK_ORG_SUB_DEV_INFO)Marshal.PtrToStructure(pOrgInfo.pDevList, typeof(DPSDK_ORG_SUB_DEV_INFO));
                for (int i = 0; i < pOrgInfo.iDevNum; i++)
                {
                    pDevList[i] = (DPSDK_ORG_SUB_DEV_INFO)Marshal.PtrToStructure(new IntPtr(pOrgInfo.pDevList.ToInt32() + i * Marshal.SizeOf(typeof(DPSDK_ORG_SUB_DEV_INFO))), typeof(DPSDK_ORG_SUB_DEV_INFO));
                    struDep.vecDevID.Add(pDevList[i].szDeviceId);
                }
            }

            if (pOrgInfo.iChannelNum > 0)
            {
                DPSDK_ORG_SUB_CHANNEL_INFO[] pChannelList = new DPSDK_ORG_SUB_CHANNEL_INFO[pOrgInfo.iChannelNum];
                //pChannelList[0] = (DPSDK_ORG_SUB_CHANNEL_INFO)Marshal.PtrToStructure(pOrgInfo.pChannelList, typeof(DPSDK_ORG_SUB_CHANNEL_INFO));
                for (int i = 0; i < pOrgInfo.iChannelNum; i++)
                {
                    pChannelList[i] = (DPSDK_ORG_SUB_CHANNEL_INFO)Marshal.PtrToStructure(new IntPtr(pOrgInfo.pChannelList.ToInt32() + i * Marshal.SizeOf(typeof(DPSDK_ORG_SUB_CHANNEL_INFO))), typeof(DPSDK_ORG_SUB_CHANNEL_INFO));
                    struDep.vecChnlID.Add(pChannelList[i].szChannelId);
                }
            }

            if (pOrgInfo.iOrgNum > 0)
            {
                DPSDK_ORG_INFO[] pOrgList = new DPSDK_ORG_INFO[pOrgInfo.iOrgNum];
                //pOrgList[0] = (DPSDK_ORG_INFO)Marshal.PtrToStructure(pOrgInfo.pOrgList, typeof(DPSDK_ORG_INFO));
                for (int i = 0; i < pOrgInfo.iOrgNum; i++)
                {
                    pOrgList[i] = (DPSDK_ORG_INFO)Marshal.PtrToStructure(new IntPtr(pOrgInfo.pOrgList.ToInt32() + i * Marshal.SizeOf(typeof(DPSDK_ORG_INFO))), typeof(DPSDK_ORG_INFO));
                    Dep_Info_All struSubDep = new Dep_Info_All();
                    struSubDep.vecSubDepInfo = new ArrayList();
                    struSubDep.vecDevID = new ArrayList();
                    struSubDep.vecChnlID = new ArrayList();
                    CopyOrgData(ref pOrgList[i], ref struSubDep);
                    struDep.vecSubDepInfo.Add(struSubDep);
                }
            }
           
        }

        private int FinOrgAndCopyDevTree([MarshalAs(UnmanagedType.LPStr)] string pParentId, ref DPSDK_LAYERED_RESULT_LIST pSrcInfo, ref Dep_Info_All pDepInfo)
        {
            if (pDepInfo.depInfo.strCoding == pParentId)
            {
                CopyDevTreeByLayeredData(ref pSrcInfo, ref pDepInfo);
                return DPSDK_SUCCESS;
            }

            for (int i = 0; i < pDepInfo.vecSubDepInfo.Count; i++)
            {
                Dep_Info_All dep = (Dep_Info_All)pDepInfo.vecSubDepInfo[i];
                if (dep.depInfo.strCoding == pParentId)
                {
                    CopyDevTreeByLayeredData(ref pSrcInfo, ref dep);
                    pDepInfo.vecSubDepInfo[i] = dep;
                    return DPSDK_SUCCESS;
                }

                int iRet = FinOrgAndCopyDevTree(pParentId, ref pSrcInfo, ref dep);
                if (iRet == DPSDK_SUCCESS)
                {
                    pDepInfo.vecSubDepInfo[i] = dep;
                    return iRet;
                }
            }
            return -1;
        }

        private void CopyDevTreeByLayeredData(ref DPSDK_LAYERED_RESULT_LIST pSrcInfo, ref Dep_Info_All pDepInfo)
        {
            if (pSrcInfo.iResultNum > 0)
            {
                DPSDK_LAYERED_RESULT[] pResultList = new DPSDK_LAYERED_RESULT[pSrcInfo.iResultNum];
                pResultList[0] = (DPSDK_LAYERED_RESULT)Marshal.PtrToStructure(pSrcInfo.pResultList, typeof(DPSDK_LAYERED_RESULT));
                if (pResultList[0].szParentID.Length == 0 && pResultList[0].iNodeType == 1)
                {
                    pDepInfo.depInfo.strCoding = pResultList[0].szID;
                    pDepInfo.depInfo.strDepName = pResultList[0].szName;
                    pDepInfo.depInfo.isParent = pResultList[0].isParent;
                    pDepInfo.depInfo.nDepSort = pResultList[0].iSort;
                }
                else
                {
                    for (int i = 0; i < pSrcInfo.iResultNum; i++)
                    {
                        pResultList[i] = (DPSDK_LAYERED_RESULT)Marshal.PtrToStructure(new IntPtr(pSrcInfo.pResultList.ToInt32() + i * Marshal.SizeOf(typeof(DPSDK_LAYERED_RESULT))), typeof(DPSDK_LAYERED_RESULT));
                        if (pResultList[i].iNodeType == 1)
                        {
                            Dep_Info_All struDep = new Dep_Info_All();
                            struDep.vecSubDepInfo = new ArrayList();
                            struDep.vecDevID = new ArrayList();
                            struDep.vecChnlID = new ArrayList();
                            struDep.depInfo.strCoding = pResultList[i].szID;
                            struDep.depInfo.strDepName = pResultList[i].szName;
                            struDep.depInfo.nDepSort = pResultList[i].iSort;
                            struDep.depInfo.isParent = pResultList[i].isParent;
                            pDepInfo.vecSubDepInfo.Add(struDep);
                        }
                        else if (pResultList[i].iNodeType == 2)
                        {
                            pDepInfo.vecDevID.Add(pResultList[i].szID);
                        }
                        else if (pResultList[i].iNodeType == 3)
                        {
                            pDepInfo.vecChnlID.Add(pResultList[i].szID);
                        }
                    }
                }
            }
        }

        private void HandleGetAllDeviceInfo(bool bOnce)
        {
            m_struDevInfoAll.vecDevInfo.Clear();

            if (bOnce)
	        {
		        /*DPSDK_QUERY_DEV_INFO struInfo;
		        memset(&struInfo, 0, sizeof(struInfo));
		        DEMO_STRCPY(struInfo.szOrgCode, m_struDepInfoAll.depInfo.strCoding.c_str());*/
		        int iRet = DPSDK_GetDevice(m_iSessionID, IntPtr.Zero, 0, fDataCallback, IntPtr.Zero);
		        if (iRet == DPSDK_SUCCESS)
		        {
		        }
		        else
		        {
			        AppData.ShowMsgInfo(AppData._CS("Get Device Failed. error code = %1", iRet.ToString()));
		        }
	        }
	        else
            {
                HandleGetDeviceInfoByOrg(ref m_struDepInfoAll, true);
            }

            

            MapIdAndName();
            DisplayTree();
        }

        private void HandleGetDeviceInfoByOrg(ref Dep_Info_All depCur, bool bSubChild)
        {
            if (depCur.vecDevID.Count > 0)
            {
                uint uiQueryLen = (uint)(Marshal.SizeOf(typeof(DPSDK_QUERY_DEV_INFO)) - Marshal.SizeOf(typeof(IntPtr)) + depCur.vecDevID.Count * Marshal.SizeOf(typeof(DPSDK_DEVICE_ID)));
                IntPtr pQueryDevInfo = Marshal.AllocHGlobal((int)uiQueryLen);
                DPSDK_QUERY_DEV_INFO struQueryDevInfo = new DPSDK_QUERY_DEV_INFO();
                struQueryDevInfo.szOrgCode = depCur.depInfo.strCoding;
                Marshal.StructureToPtr(struQueryDevInfo, pQueryDevInfo, true);
                for (int i = 0; i < depCur.vecDevID.Count; i++)
                {
                    DPSDK_DEVICE_ID struDeviceID = new DPSDK_DEVICE_ID();
                    struDeviceID.szId = (string)depCur.vecDevID[i];
                    IntPtr ptr = new IntPtr(pQueryDevInfo.ToInt32() + Marshal.SizeOf(typeof(DPSDK_QUERY_DEV_INFO)) - Marshal.SizeOf(typeof(IntPtr)) + i * Marshal.SizeOf(typeof(DPSDK_DEVICE_ID)));
                    Marshal.StructureToPtr(struDeviceID, ptr, true);
                }
                DPSDK_RES iRet = DPSDK_GetDevice(m_iSessionID, pQueryDevInfo, uiQueryLen, fDataCallback, IntPtr.Zero);
                Marshal.FreeHGlobal(pQueryDevInfo);
            }

            if (bSubChild)
            {
                for (int i = 0; i < depCur.vecSubDepInfo.Count; i++)
                {
                    Dep_Info_All depChild = (Dep_Info_All)depCur.vecSubDepInfo[i];
                    HandleGetDeviceInfoByOrg(ref depChild, bSubChild);
                }
            }
        }

        private void CopyDevData(ref DPSDK_DEV_ALL_INFO_LIST pDevAllInfoList, ref Dev_Info_All pUserDevInfo)
        {
            for (int i = 0; i < pDevAllInfoList.iDevNum; i++)
            {
                Dev_Info devInfo = new Dev_Info();
                devInfo.vecEncChnlInfo = new ArrayList();
                devInfo.vecDecChnlInfo = new ArrayList();
                devInfo.vecAlarmInChnlInfo = new ArrayList();
                devInfo.vecAlarmOutChnlInfo = new ArrayList();
                devInfo.vecTvWallInChnlInfo = new ArrayList();
                devInfo.vecTvWallOutChnlInfo = new ArrayList();
                devInfo.vecDoorChnlInfo = new ArrayList();
                devInfo.vecVoiceChnlInfo = new ArrayList();
                devInfo.vecRoadGateChnlInfo = new ArrayList();
                devInfo.vecLEDChnlInfo = new ArrayList();
                devInfo.vecDispatcherChnlInfo = new ArrayList();
                devInfo.vecPosChnlInfo = new ArrayList();
                devInfo.vecVirtualChnlInfo = new ArrayList();

                IntPtr ptr = new IntPtr(pDevAllInfoList.pDevAllInfoList.ToInt32() + i * Marshal.SizeOf(typeof(DPSDK_DEV_ALL_INFO)));
                DPSDK_DEV_ALL_INFO pDevInfo = (DPSDK_DEV_ALL_INFO)Marshal.PtrToStructure(ptr, typeof(DPSDK_DEV_ALL_INFO));
                devInfo.struDevInfo = pDevInfo.struDevInfo;

                for (int j = 0; j < pDevInfo.iEncChnlNum; j++)
                {
                    IntPtr ptrChnl = new IntPtr(pDevAllInfoList.pDevAllInfoList.ToInt32() + i * Marshal.SizeOf(typeof(DPSDK_DEV_ALL_INFO)) + Marshal.SizeOf(typeof(DPSDK_DEV_INFO)) + 1 * Marshal.SizeOf(typeof(int)) + 0 * Marshal.SizeOf(typeof(IntPtr)));
                    IntPtr ptrEncChnlInfoList = (IntPtr)Marshal.PtrToStructure(ptrChnl, typeof(IntPtr));
                    DPSDK_ENC_CHANNEL_INFO struEncChnlInfo = (DPSDK_ENC_CHANNEL_INFO)Marshal.PtrToStructure(new IntPtr(ptrEncChnlInfoList.ToInt32() + j * Marshal.SizeOf(typeof(DPSDK_ENC_CHANNEL_INFO))), typeof(DPSDK_ENC_CHANNEL_INFO));
                    devInfo.vecEncChnlInfo.Add(struEncChnlInfo);
                }
                for (int j = 0; j < pDevInfo.iDecChnlNum; j++)
                {
                    IntPtr ptrChnl = new IntPtr(pDevAllInfoList.pDevAllInfoList.ToInt32() + i * Marshal.SizeOf(typeof(DPSDK_DEV_ALL_INFO)) + Marshal.SizeOf(typeof(DPSDK_DEV_INFO)) + 2 * Marshal.SizeOf(typeof(int)) + 1 * Marshal.SizeOf(typeof(IntPtr)));
                    IntPtr ptrDecChnlInfoList = (IntPtr)Marshal.PtrToStructure(ptrChnl, typeof(IntPtr));
                    DPSDK_DEC_CHANNEL_INFO struDecChnlInfo = (DPSDK_DEC_CHANNEL_INFO)Marshal.PtrToStructure(new IntPtr(ptrDecChnlInfoList.ToInt32() + j * Marshal.SizeOf(typeof(DPSDK_DEC_CHANNEL_INFO))), typeof(DPSDK_DEC_CHANNEL_INFO));
                    devInfo.vecDecChnlInfo.Add(struDecChnlInfo);
                }
                for (int j = 0; j < pDevInfo.iAlarmInChnlNum; j++)
                {
                    IntPtr ptrChnl = new IntPtr(pDevAllInfoList.pDevAllInfoList.ToInt32() + i * Marshal.SizeOf(typeof(DPSDK_DEV_ALL_INFO)) + Marshal.SizeOf(typeof(DPSDK_DEV_INFO)) + 3 * Marshal.SizeOf(typeof(int)) + 2 * Marshal.SizeOf(typeof(IntPtr)));
                    IntPtr ptrAlarmInChnlInfoList = (IntPtr)Marshal.PtrToStructure(ptrChnl, typeof(IntPtr));
                    DPSDK_ALARMIN_CHANNEL_INFO struAlarmInChnlInfo = (DPSDK_ALARMIN_CHANNEL_INFO)Marshal.PtrToStructure(new IntPtr(ptrAlarmInChnlInfoList.ToInt32() + j * Marshal.SizeOf(typeof(DPSDK_ALARMIN_CHANNEL_INFO))), typeof(DPSDK_ALARMIN_CHANNEL_INFO));
                    devInfo.vecAlarmInChnlInfo.Add(struAlarmInChnlInfo);
                }
                for (int j = 0; j < pDevInfo.iAlarmOutChnlNum; j++)
                {
                    IntPtr ptrChnl = new IntPtr(pDevAllInfoList.pDevAllInfoList.ToInt32() + i * Marshal.SizeOf(typeof(DPSDK_DEV_ALL_INFO)) + Marshal.SizeOf(typeof(DPSDK_DEV_INFO)) + 4 * Marshal.SizeOf(typeof(int)) + 3 * Marshal.SizeOf(typeof(IntPtr)));
                    IntPtr ptrAlarmOutChnlInfoList = (IntPtr)Marshal.PtrToStructure(ptrChnl, typeof(IntPtr));
                    DPSDK_ALARMOUT_CHANNEL_INFO struAlarmOutChnlInfo = (DPSDK_ALARMOUT_CHANNEL_INFO)Marshal.PtrToStructure(new IntPtr(ptrAlarmOutChnlInfoList.ToInt32() + j * Marshal.SizeOf(typeof(DPSDK_ALARMOUT_CHANNEL_INFO))), typeof(DPSDK_ALARMOUT_CHANNEL_INFO));
                    devInfo.vecAlarmOutChnlInfo.Add(struAlarmOutChnlInfo);
                }
                for (int j = 0; j < pDevInfo.iTvWallInChnlNum; j++)
                {
                    IntPtr ptrChnl = new IntPtr(pDevAllInfoList.pDevAllInfoList.ToInt32() + i * Marshal.SizeOf(typeof(DPSDK_DEV_ALL_INFO)) + Marshal.SizeOf(typeof(DPSDK_DEV_INFO)) + 5 * Marshal.SizeOf(typeof(int)) + 4 * Marshal.SizeOf(typeof(IntPtr)));
                    IntPtr ptrTvWallInChnlInfoList = (IntPtr)Marshal.PtrToStructure(ptrChnl, typeof(IntPtr));
                    DPSDK_TVWALLIN_CHANNEL_INFO struTvWallInChnlInfo = (DPSDK_TVWALLIN_CHANNEL_INFO)Marshal.PtrToStructure(new IntPtr(ptrTvWallInChnlInfoList.ToInt32() + j * Marshal.SizeOf(typeof(DPSDK_TVWALLIN_CHANNEL_INFO))), typeof(DPSDK_TVWALLIN_CHANNEL_INFO));
                    devInfo.vecTvWallInChnlInfo.Add(struTvWallInChnlInfo);
                }
                for (int j = 0; j < pDevInfo.iTvWallOutChnlNum; j++)
                {
                    IntPtr ptrChnl = new IntPtr(pDevAllInfoList.pDevAllInfoList.ToInt32() + i * Marshal.SizeOf(typeof(DPSDK_DEV_ALL_INFO)) + Marshal.SizeOf(typeof(DPSDK_DEV_INFO)) + 6 * Marshal.SizeOf(typeof(int)) + 5 * Marshal.SizeOf(typeof(IntPtr)));
                    IntPtr ptrTvWallOutChnlInfoList = (IntPtr)Marshal.PtrToStructure(ptrChnl, typeof(IntPtr));
                    DPSDK_TVWALLOUT_CHANNEL_INFO struTvWallOutChnlInfo = (DPSDK_TVWALLOUT_CHANNEL_INFO)Marshal.PtrToStructure(new IntPtr(ptrTvWallOutChnlInfoList.ToInt32() + j * Marshal.SizeOf(typeof(DPSDK_TVWALLOUT_CHANNEL_INFO))), typeof(DPSDK_TVWALLOUT_CHANNEL_INFO));
                    devInfo.vecTvWallOutChnlInfo.Add(struTvWallOutChnlInfo);
                }
                for (int j = 0; j < pDevInfo.iDoorChnlNum; j++)
                {
                    IntPtr ptrChnl = new IntPtr(pDevAllInfoList.pDevAllInfoList.ToInt32() + i * Marshal.SizeOf(typeof(DPSDK_DEV_ALL_INFO)) + Marshal.SizeOf(typeof(DPSDK_DEV_INFO)) + 7 * Marshal.SizeOf(typeof(int)) + 6 * Marshal.SizeOf(typeof(IntPtr)));
                    IntPtr ptrDoorChnlInfoList = (IntPtr)Marshal.PtrToStructure(ptrChnl, typeof(IntPtr));
                    DPSDK_DOOR_CHANNEL_INFO struDoorChnlInfo = (DPSDK_DOOR_CHANNEL_INFO)Marshal.PtrToStructure(new IntPtr(ptrDoorChnlInfoList.ToInt32() + j * Marshal.SizeOf(typeof(DPSDK_DOOR_CHANNEL_INFO))), typeof(DPSDK_DOOR_CHANNEL_INFO));
                    devInfo.vecDoorChnlInfo.Add(struDoorChnlInfo);
                }
                for (int j = 0; j < pDevInfo.iVoiceChnlNum; j++)
                {
                    IntPtr ptrChnl = new IntPtr(pDevAllInfoList.pDevAllInfoList.ToInt32() + i * Marshal.SizeOf(typeof(DPSDK_DEV_ALL_INFO)) + Marshal.SizeOf(typeof(DPSDK_DEV_INFO)) + 8 * Marshal.SizeOf(typeof(int)) + 7 * Marshal.SizeOf(typeof(IntPtr)));
                    IntPtr ptrVoiceChnlInfoList = (IntPtr)Marshal.PtrToStructure(ptrChnl, typeof(IntPtr));
                    DPSDK_VOICE_CHANNEL_INFO struVoiceChnlInfo = (DPSDK_VOICE_CHANNEL_INFO)Marshal.PtrToStructure(new IntPtr(ptrVoiceChnlInfoList.ToInt32() + j * Marshal.SizeOf(typeof(DPSDK_VOICE_CHANNEL_INFO))), typeof(DPSDK_VOICE_CHANNEL_INFO));
                    devInfo.vecVoiceChnlInfo.Add(struVoiceChnlInfo);
                }
                for (int j = 0; j < pDevInfo.iRoadGateChnlNum; j++)
                {
                    IntPtr ptrChnl = new IntPtr(pDevAllInfoList.pDevAllInfoList.ToInt32() + i * Marshal.SizeOf(typeof(DPSDK_DEV_ALL_INFO)) + Marshal.SizeOf(typeof(DPSDK_DEV_INFO)) + 9 * Marshal.SizeOf(typeof(int)) + 8 * Marshal.SizeOf(typeof(IntPtr)));
                    IntPtr ptrRoadGateChnlInfoList = (IntPtr)Marshal.PtrToStructure(ptrChnl, typeof(IntPtr));
                    DPSDK_ROADGATE_CHANNEL_INFO struRoadGateChnlInfo = (DPSDK_ROADGATE_CHANNEL_INFO)Marshal.PtrToStructure(new IntPtr(ptrRoadGateChnlInfoList.ToInt32() + j * Marshal.SizeOf(typeof(DPSDK_ROADGATE_CHANNEL_INFO))), typeof(DPSDK_ROADGATE_CHANNEL_INFO));
                    devInfo.vecRoadGateChnlInfo.Add(struRoadGateChnlInfo);
                }
                for (int j = 0; j < pDevInfo.iLEDChnlNum; j++)
                {
                    IntPtr ptrChnl = new IntPtr(pDevAllInfoList.pDevAllInfoList.ToInt32() + i * Marshal.SizeOf(typeof(DPSDK_DEV_ALL_INFO)) + Marshal.SizeOf(typeof(DPSDK_DEV_INFO)) + 10 * Marshal.SizeOf(typeof(int)) + 9 * Marshal.SizeOf(typeof(IntPtr)));
                    IntPtr ptrLEDChnlInfoList = (IntPtr)Marshal.PtrToStructure(ptrChnl, typeof(IntPtr));
                    DPSDK_LED_CHANNEL_INFO struLEDChnlInfo = (DPSDK_LED_CHANNEL_INFO)Marshal.PtrToStructure(new IntPtr(ptrLEDChnlInfoList.ToInt32() + j * Marshal.SizeOf(typeof(DPSDK_LED_CHANNEL_INFO))), typeof(DPSDK_LED_CHANNEL_INFO));
                    devInfo.vecLEDChnlInfo.Add(struLEDChnlInfo);
                }
                for (int j = 0; j < pDevInfo.iDispatcherChnlNum; j++)
                {
                    IntPtr ptrChnl = new IntPtr(pDevAllInfoList.pDevAllInfoList.ToInt32() + i * Marshal.SizeOf(typeof(DPSDK_DEV_ALL_INFO)) + Marshal.SizeOf(typeof(DPSDK_DEV_INFO)) + 11 * Marshal.SizeOf(typeof(int)) + 10 * Marshal.SizeOf(typeof(IntPtr)));
                    IntPtr ptrDispatcherChnlInfoList = (IntPtr)Marshal.PtrToStructure(ptrChnl, typeof(IntPtr));
                    DPSDK_DISPATCHER_CHANNEL_INFO struDispatcherChnlInfo = (DPSDK_DISPATCHER_CHANNEL_INFO)Marshal.PtrToStructure(new IntPtr(ptrDispatcherChnlInfoList.ToInt32() + j * Marshal.SizeOf(typeof(DPSDK_DISPATCHER_CHANNEL_INFO))), typeof(DPSDK_DISPATCHER_CHANNEL_INFO));
                    devInfo.vecDispatcherChnlInfo.Add(struDispatcherChnlInfo);
                }
                for (int j = 0; j < pDevInfo.iPosChnlNum; j++)
                {
                    IntPtr ptrChnl = new IntPtr(pDevAllInfoList.pDevAllInfoList.ToInt32() + i * Marshal.SizeOf(typeof(DPSDK_DEV_ALL_INFO)) + Marshal.SizeOf(typeof(DPSDK_DEV_INFO)) + 12 * Marshal.SizeOf(typeof(int)) + 11 * Marshal.SizeOf(typeof(IntPtr)));
                    IntPtr ptrPosChnlInfoList = (IntPtr)Marshal.PtrToStructure(ptrChnl, typeof(IntPtr));
                    DPSDK_POS_CHANNEL_INFO struPosChnlInfo = (DPSDK_POS_CHANNEL_INFO)Marshal.PtrToStructure(new IntPtr(ptrPosChnlInfoList.ToInt32() + j * Marshal.SizeOf(typeof(DPSDK_POS_CHANNEL_INFO))), typeof(DPSDK_POS_CHANNEL_INFO));
                    devInfo.vecPosChnlInfo.Add(struPosChnlInfo);
                }
                for (int j = 0; j < pDevInfo.iVirtualChnlNum; j++)
                {
                    IntPtr ptrChnl = new IntPtr(pDevAllInfoList.pDevAllInfoList.ToInt32() + i * Marshal.SizeOf(typeof(DPSDK_DEV_ALL_INFO)) + Marshal.SizeOf(typeof(DPSDK_DEV_INFO)) + 13 * Marshal.SizeOf(typeof(int)) + 12 * Marshal.SizeOf(typeof(IntPtr)));
                    IntPtr ptrVirtualChnlInfoList = (IntPtr)Marshal.PtrToStructure(ptrChnl, typeof(IntPtr));
                    DPSDK_VIRTUAL_CHANNEL_INFO struVirtualChnlInfo = (DPSDK_VIRTUAL_CHANNEL_INFO)Marshal.PtrToStructure(new IntPtr(ptrVirtualChnlInfoList.ToInt32() + j * Marshal.SizeOf(typeof(DPSDK_VIRTUAL_CHANNEL_INFO))), typeof(DPSDK_VIRTUAL_CHANNEL_INFO));
                    devInfo.vecVirtualChnlInfo.Add(struVirtualChnlInfo);
                }
                pUserDevInfo.vecDevInfo.Add(devInfo);
            }
        }

        private void MapIdAndName()
        {
            m_mapIdName.Clear();
            if (m_struDevInfoAll.vecDevInfo.Count > 0)
            {
                for (int i = 0; i < m_struDevInfoAll.vecDevInfo.Count; i++)
                {
                    Dev_Info struDevInfo = (Dev_Info)m_struDevInfoAll.vecDevInfo[i];
                    m_mapIdName.Add(struDevInfo.struDevInfo.szDeviceID, struDevInfo.struDevInfo.szDeviceName);

                    //Encode channel
                    for (int j = 0; j < struDevInfo.vecEncChnlInfo.Count; j++)
                    {
                        DPSDK_ENC_CHANNEL_INFO struEncChnlInfo = (DPSDK_ENC_CHANNEL_INFO)struDevInfo.vecEncChnlInfo[j];
                        m_mapIdName.Add(struEncChnlInfo.struChannelInfo.szChannelID, struEncChnlInfo.struChannelInfo.szChannelName);
                    }
                    //Decode channel
                    for (int j = 0; j < struDevInfo.vecDecChnlInfo.Count; j++)
                    {
                        DPSDK_DEC_CHANNEL_INFO struDecChnlInfo = (DPSDK_DEC_CHANNEL_INFO)struDevInfo.vecDecChnlInfo[j];
                        m_mapIdName.Add(struDecChnlInfo.struChannelInfo.szChannelID, struDecChnlInfo.struChannelInfo.szChannelName);
                    }
                    //Alarm input channel
                    for (int j = 0; j < struDevInfo.vecAlarmInChnlInfo.Count; j++)
                    {
                        DPSDK_ALARMIN_CHANNEL_INFO struAlarmInChnlInfo = (DPSDK_ALARMIN_CHANNEL_INFO)struDevInfo.vecAlarmInChnlInfo[j];
                        m_mapIdName.Add(struAlarmInChnlInfo.struChannelInfo.szChannelID, struAlarmInChnlInfo.struChannelInfo.szChannelName);
                    }
                    //Alarm output channel
                    for (int j = 0; j < struDevInfo.vecAlarmOutChnlInfo.Count; j++)
                    {
                        DPSDK_ALARMOUT_CHANNEL_INFO struAlarmOutChnlInfo = (DPSDK_ALARMOUT_CHANNEL_INFO)struDevInfo.vecAlarmOutChnlInfo[j];
                        m_mapIdName.Add(struAlarmOutChnlInfo.struChannelInfo.szChannelID, struAlarmOutChnlInfo.struChannelInfo.szChannelName);
                    }
                    //Big screen input channel
                    for (int j = 0; j < struDevInfo.vecTvWallInChnlInfo.Count; j++)
                    {
                        DPSDK_TVWALLIN_CHANNEL_INFO struTvWallInChnlInfo = (DPSDK_TVWALLIN_CHANNEL_INFO)struDevInfo.vecTvWallInChnlInfo[j];
                        m_mapIdName.Add(struTvWallInChnlInfo.struChannelInfo.szChannelID, struTvWallInChnlInfo.struChannelInfo.szChannelName);
                    }
                    //Big screen output channel
                    for (int j = 0; j < struDevInfo.vecTvWallOutChnlInfo.Count; j++)
                    {
                        DPSDK_TVWALLOUT_CHANNEL_INFO struTvWallOutChnlInfo = (DPSDK_TVWALLOUT_CHANNEL_INFO)struDevInfo.vecTvWallOutChnlInfo[j];
                        m_mapIdName.Add(struTvWallOutChnlInfo.struChannelInfo.szChannelID, struTvWallOutChnlInfo.struChannelInfo.szChannelName);
                    }
                    //Acess channel
                    for (int j = 0; j < struDevInfo.vecDoorChnlInfo.Count; j++)
                    {
                        DPSDK_DOOR_CHANNEL_INFO struDoorChnlInfo = (DPSDK_DOOR_CHANNEL_INFO)struDevInfo.vecDoorChnlInfo[j];
                        m_mapIdName.Add(struDoorChnlInfo.struChannelInfo.szChannelID, struDoorChnlInfo.struChannelInfo.szChannelName);
                    }
                    //Audio channel
                    for (int j = 0; j < struDevInfo.vecVoiceChnlInfo.Count; j++)
                    {
                        DPSDK_VOICE_CHANNEL_INFO struVoiceChnlInfo = (DPSDK_VOICE_CHANNEL_INFO)struDevInfo.vecVoiceChnlInfo[j];
                        m_mapIdName.Add(struVoiceChnlInfo.struChannelInfo.szChannelID, struVoiceChnlInfo.struChannelInfo.szChannelName);
                    }
                    //Road gate channel
                    for (int j = 0; j < struDevInfo.vecRoadGateChnlInfo.Count; j++)
                    {
                        DPSDK_ROADGATE_CHANNEL_INFO struRoadGateChnlInfo = (DPSDK_ROADGATE_CHANNEL_INFO)struDevInfo.vecRoadGateChnlInfo[j];
                        m_mapIdName.Add(struRoadGateChnlInfo.struChannelInfo.szChannelID, struRoadGateChnlInfo.struChannelInfo.szChannelName);
                    }
                    //LED channel
                    for (int j = 0; j < struDevInfo.vecLEDChnlInfo.Count; j++)
                    {
                        DPSDK_LED_CHANNEL_INFO struLEDChnlInfo = (DPSDK_LED_CHANNEL_INFO)struDevInfo.vecLEDChnlInfo[j];
                        m_mapIdName.Add(struLEDChnlInfo.struChannelInfo.szChannelID, struLEDChnlInfo.struChannelInfo.szChannelName);
                    }
                    //Dispatcher channel
                    for (int j = 0; j < struDevInfo.vecDispatcherChnlInfo.Count; j++)
                    {
                        DPSDK_DISPATCHER_CHANNEL_INFO struDispatcherChnlInfo = (DPSDK_DISPATCHER_CHANNEL_INFO)struDevInfo.vecDispatcherChnlInfo[j];
                        m_mapIdName.Add(struDispatcherChnlInfo.struChannelInfo.szChannelID, struDispatcherChnlInfo.struChannelInfo.szChannelName);
                    }
                    //POS channel
                    for (int j = 0; j < struDevInfo.vecPosChnlInfo.Count; j++)
                    {
                        DPSDK_POS_CHANNEL_INFO struPosChnlInfo = (DPSDK_POS_CHANNEL_INFO)struDevInfo.vecPosChnlInfo[j];
                        m_mapIdName.Add(struPosChnlInfo.struChannelInfo.szChannelID, struPosChnlInfo.struChannelInfo.szChannelName);
                    }
                    //Virtual channel
                    for (int j = 0; j < struDevInfo.vecVirtualChnlInfo.Count; j++)
                    {
                        DPSDK_VIRTUAL_CHANNEL_INFO struVirtualChnlInfo = (DPSDK_VIRTUAL_CHANNEL_INFO)struDevInfo.vecVirtualChnlInfo[j];
                        m_mapIdName.Add(struVirtualChnlInfo.struChannelInfo.szChannelID, struVirtualChnlInfo.struChannelInfo.szChannelName);
                    }
                }
            }
        }

        private string GetMapNameFromID(string strID)
        {
            if (m_mapIdName.ContainsKey(strID))
            {
                return m_mapIdName[strID];
            }

            return "";
        }

        private void DisplayOrgTree()
        {
            if (m_struDepInfoAll.depInfo.strCoding == "" || m_struDepInfoAll.depInfo.strCoding == null)
            {
                m_struDepInfoAll.depInfo.strCoding = textDeviceOrgCode.Text;
            }
            treeViewDeviceOrg.Nodes.Clear();

            TreeNode orgNode = new TreeNode(Encoding.GetEncoding("utf-8").GetString(Encoding.Default.GetBytes(m_struDepInfoAll.depInfo.strDepName)));
            treeViewDeviceOrg.Nodes.Add(orgNode);
            LoopOrgNode(ref orgNode, ref m_struDepInfoAll);

            //Add device ID
            for (int i = 0; i < m_struDepInfoAll.vecDevID.Count; i++)
            {
                string strInfo = GetMapNameFromID((string)m_struDepInfoAll.vecDevID[i]);
                if (strInfo == "")
                {
                    strInfo = (string)m_struDepInfoAll.vecDevID[i];
                }
                TreeNode devNode = new TreeNode(Encoding.GetEncoding("utf-8").GetString(Encoding.Default.GetBytes(strInfo)));
                orgNode.Nodes.Add(devNode);
            }
            //Add channel ID
            for ( int i = 0; i < m_struDepInfoAll.vecChnlID.Count; i++ )
            {
                string strInfo = GetMapNameFromID((string)m_struDepInfoAll.vecChnlID[i]);
                if (strInfo == "")
                {
                    strInfo = (string)m_struDepInfoAll.vecChnlID[i];
                }
                TreeNode chnlNode = new TreeNode(Encoding.GetEncoding("utf-8").GetString(Encoding.Default.GetBytes(strInfo)));
                orgNode.Nodes.Add(chnlNode);
            }
        }

        private void LoopOrgNode(ref TreeNode node, ref Dep_Info_All struDepInfoAll)
        {
            for (int i = 0; i < struDepInfoAll.vecSubDepInfo.Count; i++)
            {
                Dep_Info_All subDepInfo = (Dep_Info_All)struDepInfoAll.vecSubDepInfo[i];
//                 byte[] buffer1 = Encoding.Default.GetBytes(subDepInfo.depInfo.strDepName);
//                 byte[] buffer2 = Encoding.Convert(Encoding.UTF8, Encoding.Default, buffer1, 0, buffer1.Length);
//                 string strBuffer = Encoding.Default.GetString(buffer2, 0, buffer2.Length);
                TreeNode subOrgNode = new TreeNode(Encoding.GetEncoding("utf-8").GetString(Encoding.Default.GetBytes(subDepInfo.depInfo.strDepName))/*strBuffer*/);
                node.Nodes.Add(subOrgNode);
                LoopOrgNode(ref subOrgNode, ref subDepInfo);

                //Add device ID
                for (int j = 0; j < subDepInfo.vecDevID.Count; j++)
                {
                    string strInfo = GetMapNameFromID((string)subDepInfo.vecDevID[j]);
                    if (strInfo == "")
                    {
                        strInfo = (string)subDepInfo.vecDevID[j];
                    }
                    TreeNode devNode = new TreeNode(Encoding.GetEncoding("utf-8").GetString(Encoding.Default.GetBytes(strInfo)));
                    subOrgNode.Nodes.Add(devNode);
                }
                //Add channel ID
                for (int j = 0; j < subDepInfo.vecChnlID.Count; j++)
                {
                    string strInfo = GetMapNameFromID((string)subDepInfo.vecChnlID[j]);
                    if (strInfo == "")
                    {
                        strInfo = (string)subDepInfo.vecChnlID[j];
                    }
                    TreeNode chnlNode = new TreeNode(Encoding.GetEncoding("utf-8").GetString(Encoding.Default.GetBytes(strInfo)));
                    subOrgNode.Nodes.Add(chnlNode);
                }
            }
        }

        private void DisplayDeviceTree()
        {
            treeViewDeviceOrg.Nodes.Clear();
            if (m_struDevInfoAll.vecDevInfo.Count == 0)
            {
                return;
            }

            for (int i = 0; i < m_struDevInfoAll.vecDevInfo.Count; i++)
            {
                Dev_Info struDevInfo = (Dev_Info)m_struDevInfoAll.vecDevInfo[i];
                TreeNode devNode = new TreeNode(Encoding.GetEncoding("utf-8").GetString(Encoding.Default.GetBytes(struDevInfo.struDevInfo.szDeviceName)));
                treeViewDeviceOrg.Nodes.Add(devNode);

                //Encode channel
                for (int j = 0; j < struDevInfo.vecEncChnlInfo.Count; j++)
                {
                    DPSDK_ENC_CHANNEL_INFO struEncChnlInfo = (DPSDK_ENC_CHANNEL_INFO)struDevInfo.vecEncChnlInfo[j];
                    TreeNode chnlNode = new TreeNode(Encoding.GetEncoding("utf-8").GetString(Encoding.Default.GetBytes(struEncChnlInfo.struChannelInfo.szChannelName)));
                    devNode.Nodes.Add(chnlNode);
                }
                //Decode channel
                for (int j = 0; j < struDevInfo.vecDecChnlInfo.Count; j++)
                {
                    DPSDK_DEC_CHANNEL_INFO struDecChnlInfo = (DPSDK_DEC_CHANNEL_INFO)struDevInfo.vecDecChnlInfo[j];
                    TreeNode chnlNode = new TreeNode(Encoding.GetEncoding("utf-8").GetString(Encoding.Default.GetBytes(struDecChnlInfo.struChannelInfo.szChannelName)));
                    devNode.Nodes.Add(chnlNode);
                }
                //Alarm input channel
                for (int j = 0; j < struDevInfo.vecAlarmInChnlInfo.Count; j++)
                {
                    DPSDK_ALARMIN_CHANNEL_INFO struAlarmInChnlInfo = (DPSDK_ALARMIN_CHANNEL_INFO)struDevInfo.vecAlarmInChnlInfo[j];
                    TreeNode chnlNode = new TreeNode(Encoding.GetEncoding("utf-8").GetString(Encoding.Default.GetBytes(struAlarmInChnlInfo.struChannelInfo.szChannelName)));
                    devNode.Nodes.Add(chnlNode);
                }
                //Alarm output channel
                for (int j = 0; j < struDevInfo.vecAlarmOutChnlInfo.Count; j++)
                {
                    DPSDK_ALARMOUT_CHANNEL_INFO struAlarmOutChnlInfo = (DPSDK_ALARMOUT_CHANNEL_INFO)struDevInfo.vecAlarmOutChnlInfo[j];
                    TreeNode chnlNode = new TreeNode(Encoding.GetEncoding("utf-8").GetString(Encoding.Default.GetBytes(struAlarmOutChnlInfo.struChannelInfo.szChannelName)));
                    devNode.Nodes.Add(chnlNode);
                }
                //Big screen input channel
                for (int j = 0; j < struDevInfo.vecTvWallInChnlInfo.Count; j++)
                {
                    DPSDK_TVWALLIN_CHANNEL_INFO struTvWallInChnlInfo = (DPSDK_TVWALLIN_CHANNEL_INFO)struDevInfo.vecTvWallInChnlInfo[j];
                    TreeNode chnlNode = new TreeNode(Encoding.GetEncoding("utf-8").GetString(Encoding.Default.GetBytes(struTvWallInChnlInfo.struChannelInfo.szChannelName)));
                    devNode.Nodes.Add(chnlNode);
                }
                //Big screen output channel
                for (int j = 0; j < struDevInfo.vecTvWallOutChnlInfo.Count; j++)
                {
                    DPSDK_TVWALLOUT_CHANNEL_INFO struTvWallOutChnlInfo = (DPSDK_TVWALLOUT_CHANNEL_INFO)struDevInfo.vecTvWallOutChnlInfo[j];
                    TreeNode chnlNode = new TreeNode(Encoding.GetEncoding("utf-8").GetString(Encoding.Default.GetBytes(struTvWallOutChnlInfo.struChannelInfo.szChannelName)));
                    devNode.Nodes.Add(chnlNode);
                }
                //Acess channel
                for (int j = 0; j < struDevInfo.vecDoorChnlInfo.Count; j++)
                {
                    DPSDK_DOOR_CHANNEL_INFO struDoorChnlInfo = (DPSDK_DOOR_CHANNEL_INFO)struDevInfo.vecDoorChnlInfo[j];
                    TreeNode chnlNode = new TreeNode(Encoding.GetEncoding("utf-8").GetString(Encoding.Default.GetBytes(struDoorChnlInfo.struChannelInfo.szChannelName)));
                    devNode.Nodes.Add(chnlNode);
                }
                //Audio channel
                for (int j = 0; j < struDevInfo.vecVoiceChnlInfo.Count; j++)
                {
                    DPSDK_VOICE_CHANNEL_INFO struVoiceChnlInfo = (DPSDK_VOICE_CHANNEL_INFO)struDevInfo.vecVoiceChnlInfo[j];
                    TreeNode chnlNode = new TreeNode(Encoding.GetEncoding("utf-8").GetString(Encoding.Default.GetBytes(struVoiceChnlInfo.struChannelInfo.szChannelName)));
                    devNode.Nodes.Add(chnlNode);
                }
                //Road gate channel
                for (int j = 0; j < struDevInfo.vecRoadGateChnlInfo.Count; j++)
                {
                    DPSDK_ROADGATE_CHANNEL_INFO struRoadGateChnlInfo = (DPSDK_ROADGATE_CHANNEL_INFO)struDevInfo.vecRoadGateChnlInfo[j];
                    TreeNode chnlNode = new TreeNode(Encoding.GetEncoding("utf-8").GetString(Encoding.Default.GetBytes(struRoadGateChnlInfo.struChannelInfo.szChannelName)));
                    devNode.Nodes.Add(chnlNode);
                }
                //LED channel
                for (int j = 0; j < struDevInfo.vecLEDChnlInfo.Count; j++)
                {
                    DPSDK_LED_CHANNEL_INFO struLEDChnlInfo = (DPSDK_LED_CHANNEL_INFO)struDevInfo.vecLEDChnlInfo[j];
                    TreeNode chnlNode = new TreeNode(Encoding.GetEncoding("utf-8").GetString(Encoding.Default.GetBytes(struLEDChnlInfo.struChannelInfo.szChannelName)));
                    devNode.Nodes.Add(chnlNode);
                }
                //Dispatcher channel
                for (int j = 0; j < struDevInfo.vecDispatcherChnlInfo.Count; j++)
                {
                    DPSDK_DISPATCHER_CHANNEL_INFO struDispatcherChnlInfo = (DPSDK_DISPATCHER_CHANNEL_INFO)struDevInfo.vecDispatcherChnlInfo[j];
                    TreeNode chnlNode = new TreeNode(Encoding.GetEncoding("utf-8").GetString(Encoding.Default.GetBytes(struDispatcherChnlInfo.struChannelInfo.szChannelName)));
                    devNode.Nodes.Add(chnlNode);
                }
                //POS channel
                for (int j = 0; j < struDevInfo.vecPosChnlInfo.Count; j++)
                {
                    DPSDK_POS_CHANNEL_INFO struPosChnlInfo = (DPSDK_POS_CHANNEL_INFO)struDevInfo.vecPosChnlInfo[j];
                    TreeNode chnlNode = new TreeNode(Encoding.GetEncoding("utf-8").GetString(Encoding.Default.GetBytes(struPosChnlInfo.struChannelInfo.szChannelName)));
                    devNode.Nodes.Add(chnlNode);
                }
                //Virtual channel
                for (int j = 0; j < struDevInfo.vecVirtualChnlInfo.Count; j++)
                {
                    DPSDK_VIRTUAL_CHANNEL_INFO struVirtualChnlInfo = (DPSDK_VIRTUAL_CHANNEL_INFO)struDevInfo.vecVirtualChnlInfo[j];
                    TreeNode chnlNode = new TreeNode(Encoding.GetEncoding("utf-8").GetString(Encoding.Default.GetBytes(struVirtualChnlInfo.struChannelInfo.szChannelName)));
                    devNode.Nodes.Add(chnlNode);
                }
            }
        }

        private void DisplayChannelTree()
        {
            treeViewDeviceOrg.Nodes.Clear();
            if (m_struDevInfoAll.vecDevInfo.Count == 0)
            {
                return;
            }

            for (int i = 0; i < m_struDevInfoAll.vecDevInfo.Count; i++)
            {
                Dev_Info struDevInfo = (Dev_Info)m_struDevInfoAll.vecDevInfo[i];
                //Encode channel
                for (int j = 0; j < struDevInfo.vecEncChnlInfo.Count; j++)
                {
                    DPSDK_ENC_CHANNEL_INFO struEncChnlInfo = (DPSDK_ENC_CHANNEL_INFO)struDevInfo.vecEncChnlInfo[j];
                    TreeNode chnlNode = new TreeNode(Encoding.GetEncoding("utf-8").GetString(Encoding.Default.GetBytes(struEncChnlInfo.struChannelInfo.szChannelName)));
                    treeViewDeviceOrg.Nodes.Add(chnlNode);
                }
                //Decode channel
                for (int j = 0; j < struDevInfo.vecDecChnlInfo.Count; j++)
                {
                    DPSDK_DEC_CHANNEL_INFO struDecChnlInfo = (DPSDK_DEC_CHANNEL_INFO)struDevInfo.vecDecChnlInfo[j];
                    TreeNode chnlNode = new TreeNode(Encoding.GetEncoding("utf-8").GetString(Encoding.Default.GetBytes(struDecChnlInfo.struChannelInfo.szChannelName)));
                    treeViewDeviceOrg.Nodes.Add(chnlNode);
                }
                //Alarm input channel
                for (int j = 0; j < struDevInfo.vecAlarmInChnlInfo.Count; j++)
                {
                    DPSDK_ALARMIN_CHANNEL_INFO struAlarmInChnlInfo = (DPSDK_ALARMIN_CHANNEL_INFO)struDevInfo.vecAlarmInChnlInfo[j];
                    TreeNode chnlNode = new TreeNode(Encoding.GetEncoding("utf-8").GetString(Encoding.Default.GetBytes(struAlarmInChnlInfo.struChannelInfo.szChannelName)));
                    treeViewDeviceOrg.Nodes.Add(chnlNode);
                }
                //Alarm output channel
                for (int j = 0; j < struDevInfo.vecAlarmOutChnlInfo.Count; j++)
                {
                    DPSDK_ALARMOUT_CHANNEL_INFO struAlarmOutChnlInfo = (DPSDK_ALARMOUT_CHANNEL_INFO)struDevInfo.vecAlarmOutChnlInfo[j];
                    TreeNode chnlNode = new TreeNode(Encoding.GetEncoding("utf-8").GetString(Encoding.Default.GetBytes(struAlarmOutChnlInfo.struChannelInfo.szChannelName)));
                    treeViewDeviceOrg.Nodes.Add(chnlNode);
                }
                //Big screen input channel
                for (int j = 0; j < struDevInfo.vecTvWallInChnlInfo.Count; j++)
                {
                    DPSDK_TVWALLIN_CHANNEL_INFO struTvWallInChnlInfo = (DPSDK_TVWALLIN_CHANNEL_INFO)struDevInfo.vecTvWallInChnlInfo[j];
                    TreeNode chnlNode = new TreeNode(Encoding.GetEncoding("utf-8").GetString(Encoding.Default.GetBytes(struTvWallInChnlInfo.struChannelInfo.szChannelName)));
                    treeViewDeviceOrg.Nodes.Add(chnlNode);
                }
                //Big screen output channel
                for (int j = 0; j < struDevInfo.vecTvWallOutChnlInfo.Count; j++)
                {
                    DPSDK_TVWALLOUT_CHANNEL_INFO struTvWallOutChnlInfo = (DPSDK_TVWALLOUT_CHANNEL_INFO)struDevInfo.vecTvWallOutChnlInfo[j];
                    TreeNode chnlNode = new TreeNode(Encoding.GetEncoding("utf-8").GetString(Encoding.Default.GetBytes(struTvWallOutChnlInfo.struChannelInfo.szChannelName)));
                    treeViewDeviceOrg.Nodes.Add(chnlNode);
                }
                //Acess channel
                for (int j = 0; j < struDevInfo.vecDoorChnlInfo.Count; j++)
                {
                    DPSDK_DOOR_CHANNEL_INFO struDoorChnlInfo = (DPSDK_DOOR_CHANNEL_INFO)struDevInfo.vecDoorChnlInfo[j];
                    TreeNode chnlNode = new TreeNode(Encoding.GetEncoding("utf-8").GetString(Encoding.Default.GetBytes(struDoorChnlInfo.struChannelInfo.szChannelName)));
                    treeViewDeviceOrg.Nodes.Add(chnlNode);
                }
                //Audio channel
                for (int j = 0; j < struDevInfo.vecVoiceChnlInfo.Count; j++)
                {
                    DPSDK_VOICE_CHANNEL_INFO struVoiceChnlInfo = (DPSDK_VOICE_CHANNEL_INFO)struDevInfo.vecVoiceChnlInfo[j];
                    TreeNode chnlNode = new TreeNode(Encoding.GetEncoding("utf-8").GetString(Encoding.Default.GetBytes(struVoiceChnlInfo.struChannelInfo.szChannelName)));
                    treeViewDeviceOrg.Nodes.Add(chnlNode);
                }
                //Road gate channel
                for (int j = 0; j < struDevInfo.vecRoadGateChnlInfo.Count; j++)
                {
                    DPSDK_ROADGATE_CHANNEL_INFO struRoadGateChnlInfo = (DPSDK_ROADGATE_CHANNEL_INFO)struDevInfo.vecRoadGateChnlInfo[j];
                    TreeNode chnlNode = new TreeNode(Encoding.GetEncoding("utf-8").GetString(Encoding.Default.GetBytes(struRoadGateChnlInfo.struChannelInfo.szChannelName)));
                    treeViewDeviceOrg.Nodes.Add(chnlNode);
                }
                //LED channel
                for (int j = 0; j < struDevInfo.vecLEDChnlInfo.Count; j++)
                {
                    DPSDK_LED_CHANNEL_INFO struLEDChnlInfo = (DPSDK_LED_CHANNEL_INFO)struDevInfo.vecLEDChnlInfo[j];
                    TreeNode chnlNode = new TreeNode(Encoding.GetEncoding("utf-8").GetString(Encoding.Default.GetBytes(struLEDChnlInfo.struChannelInfo.szChannelName)));
                    treeViewDeviceOrg.Nodes.Add(chnlNode);
                }
                //Dispatcher channel
                for (int j = 0; j < struDevInfo.vecDispatcherChnlInfo.Count; j++)
                {
                    DPSDK_DISPATCHER_CHANNEL_INFO struDispatcherChnlInfo = (DPSDK_DISPATCHER_CHANNEL_INFO)struDevInfo.vecDispatcherChnlInfo[j];
                    TreeNode chnlNode = new TreeNode(Encoding.GetEncoding("utf-8").GetString(Encoding.Default.GetBytes(struDispatcherChnlInfo.struChannelInfo.szChannelName)));
                    treeViewDeviceOrg.Nodes.Add(chnlNode);
                }
                //POS channel
                for (int j = 0; j < struDevInfo.vecPosChnlInfo.Count; j++)
                {
                    DPSDK_POS_CHANNEL_INFO struPosChnlInfo = (DPSDK_POS_CHANNEL_INFO)struDevInfo.vecPosChnlInfo[j];
                    TreeNode chnlNode = new TreeNode(Encoding.GetEncoding("utf-8").GetString(Encoding.Default.GetBytes(struPosChnlInfo.struChannelInfo.szChannelName)));
                    treeViewDeviceOrg.Nodes.Add(chnlNode);
                }
                //Virtual channel
                for (int j = 0; j < struDevInfo.vecVirtualChnlInfo.Count; j++)
                {
                    DPSDK_VIRTUAL_CHANNEL_INFO struVirtualChnlInfo = (DPSDK_VIRTUAL_CHANNEL_INFO)struDevInfo.vecVirtualChnlInfo[j];
                    TreeNode chnlNode = new TreeNode(Encoding.GetEncoding("utf-8").GetString(Encoding.Default.GetBytes(struVirtualChnlInfo.struChannelInfo.szChannelName)));
                    treeViewDeviceOrg.Nodes.Add(chnlNode);
                }
            }
        }

        private void DisplayTree()
        {
            if (radioButtonDeviceOrg.Checked)
            {
                DisplayOrgTree();
            }
            else if (radioButtonDeviceDevice.Checked)
            {
                DisplayDeviceTree();
            }
            else if (radioButtonDeviceChannel.Checked)
            {
                DisplayChannelTree();
            }
        }

        private void PacketOrgXml(ref Dep_Info_All struDepInfoAll, ref string strXmlData)
        {
            strXmlData += "<Department ";

            strXmlData = strXmlData + XML_CODING + " = " + struDepInfoAll.depInfo.strCoding + " ";
            strXmlData = strXmlData + XML_NAME + " = " + Encoding.GetEncoding("utf-8").GetString(Encoding.Default.GetBytes(struDepInfoAll.depInfo.strDepName)) + " ";
            strXmlData = strXmlData + XML_SN + " = " + struDepInfoAll.depInfo.strSN + " ";

            strXmlData += ">\r\n";

            for (int i = 0; i < struDepInfoAll.vecSubDepInfo.Count; ++i)
            {
                Dep_Info_All struSubDepInfoAll = (Dep_Info_All)struDepInfoAll.vecSubDepInfo[i];
                PacketOrgXml(ref struSubDepInfoAll, ref strXmlData);
            }
            for (int i = 0; i < struDepInfoAll.vecDevID.Count; ++i)
            {
                strXmlData += "<Device>" + (string)struDepInfoAll.vecDevID[i] + "</Device>\r\n";
            }
            for (int i = 0; i < struDepInfoAll.vecChnlID.Count; ++i)
            {
                strXmlData += "<Channel>" + (string)struDepInfoAll.vecChnlID[i] + "</Channel>\r\n";
            }
            strXmlData += "</Department>\r\n";
        }
    }
}