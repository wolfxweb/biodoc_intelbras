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
        private void btnPtzOperateFunction_Click(object sender, EventArgs e)
        {
            if (textPtzChannelId.Text == "" || textPtzCruiseId.Text == "" || textPtzTrackId.Text == "")
            {
                AppData.ShowMsgInfo(AppData._CS("Please input correct params!"));
                return;
            }
            DPSDK_PTZOPERATE_FUNCTION_PARAM pPtzOperateFunctionParam = new DPSDK_PTZOPERATE_FUNCTION_PARAM();
            pPtzOperateFunctionParam.szChannelId = textPtzChannelId.Text;
            pPtzOperateFunctionParam.iPtzOFType = (PtzOperateFunction_e)Convert.ToInt32(comboBoxPtzOpType.Text.Substring(0, comboBoxPtzOpType.Text.IndexOf("-")));
            pPtzOperateFunctionParam.iCruiseId = Convert.ToInt32(textPtzCruiseId.Text);
            pPtzOperateFunctionParam.iTrackId = Convert.ToInt32(textPtzTrackId.Text);
            pPtzOperateFunctionParam.iSwitchMode = Convert.ToInt32(comboBoxPtzSwitch.Text.Substring(0, comboBoxPtzSwitch.Text.IndexOf("-")));
            pPtzOperateFunctionParam.iBorderType = Convert.ToInt32(comboBoxPtzBorderType.Text.Substring(0, comboBoxPtzBorderType.Text.IndexOf("-")));
            pPtzOperateFunctionParam.iAssisentType = Convert.ToInt32(comboBoxPtzAssisentType.Text.Substring(0, comboBoxPtzAssisentType.Text.IndexOf("-")));
            pPtzOperateFunctionParam.iMoveType = Convert.ToInt32(comboBoxPtzMoveType.Text.Substring(0, comboBoxPtzMoveType.Text.IndexOf("-")));
            pPtzOperateFunctionParam.iSwitchPtzMenu = Convert.ToInt32(comboBoxPtzSwitchMenu.Text.Substring(0, comboBoxPtzSwitchMenu.Text.IndexOf("-")));
            DPSDK_PTZOPERATE_RESULT pPtzOperateFunctionResult = new DPSDK_PTZOPERATE_RESULT();
            DPSDK_RES iRet = DPSDK_PtzOperateFunction(m_iSessionID, ref pPtzOperateFunctionParam, ref pPtzOperateFunctionResult);
            if (iRet == DPSDK_SUCCESS)
            {
                textPtzOpResult.Text = AppData._CS("PtzOperateFunction Success. iResult=%1 LockUserName=%2 LockUserLevel=%3.", pPtzOperateFunctionResult.iResult.ToString(), pPtzOperateFunctionResult.struLockUser.szLockUserName, pPtzOperateFunctionResult.struLockUser.iLockUserLevel.ToString());
            }
            else
            {
                textPtzOpResult.Text = AppData._CS("PtzOperateFunction failed. error code = %1", iRet.ToString());
            }
        }

        private void btnPtzOpCamera_Click(object sender, EventArgs e)
        {
            if (textPtzChannelId.Text == "" || textPtzOpCameraStep.Text == "")
            {
                AppData.ShowMsgInfo(AppData._CS("Please input correct params!"));
                return;
            }
            DPSDK_PTZOPERATE_CAMERA_PARAM pPtzOperateCamereParam = new DPSDK_PTZOPERATE_CAMERA_PARAM();
            pPtzOperateCamereParam.szChannelId = textPtzChannelId.Text;
            pPtzOperateCamereParam.iDirect = Convert.ToInt32(comboBoxPtzOpCameraDirect.Text.Substring(0, comboBoxPtzOpCameraDirect.Text.IndexOf("-")));
            pPtzOperateCamereParam.iCommand = Convert.ToInt32(comboBoxPtzOpCameraCommand.Text.Substring(0, comboBoxPtzOpCameraCommand.Text.IndexOf("-")));
            pPtzOperateCamereParam.iOperateType = Convert.ToInt32(comboBoxPtzOpCameraType.Text.Substring(0, comboBoxPtzOpCameraType.Text.IndexOf("-")));
            pPtzOperateCamereParam.iStep = Convert.ToInt32(textPtzOpCameraStep.Text);
            pPtzOperateCamereParam.szExtend = textPtzOpCameraExtend.Text;
            DPSDK_PTZOPERATE_RESULT pPtzOperateResult = new DPSDK_PTZOPERATE_RESULT();
            DPSDK_RES iRet = DPSDK_PtzOperateCamera(m_iSessionID, ref pPtzOperateCamereParam, ref pPtzOperateResult);
            if (iRet == DPSDK_SUCCESS)
            {
                textPtzOpResult.Text = AppData._CS("PtzOperateCamera Success. iResult=%1 LockUserName=%2 LockUserLevel=%3.", pPtzOperateResult.iResult.ToString(), pPtzOperateResult.struLockUser.szLockUserName, pPtzOperateResult.struLockUser.iLockUserLevel.ToString());
            }
            else
            {
                textPtzOpResult.Text = AppData._CS("PtzOperateCamera failed. error code = %1", iRet.ToString());
            }
        }

        private void btnPtzOpDirect_Click(object sender, EventArgs e)
        {
            if (textPtzChannelId.Text == "" || textPtzOpDirectStepY.Text == "" || textPtzOpDirectStepX.Text == "")
            {
                AppData.ShowMsgInfo(AppData._CS("Please input correct params!"));
                return;
            }
            DPSDK_PTZOPERATE_DIRECT_PARAM pPtzOperateDirectParam = new DPSDK_PTZOPERATE_DIRECT_PARAM();
            pPtzOperateDirectParam.szChannelId = textPtzChannelId.Text;
            pPtzOperateDirectParam.iStepY = Convert.ToInt32(textPtzOpDirectStepY.Text);
            pPtzOperateDirectParam.iStepX = Convert.ToInt32(textPtzOpDirectStepX.Text);
            pPtzOperateDirectParam.iDirect = Convert.ToInt32(comboBoxPtzOpDirect.Text.Substring(0, comboBoxPtzOpDirect.Text.IndexOf("-")));
            pPtzOperateDirectParam.iCommand = Convert.ToInt32(comboBoxPtzOpDirectCommand.Text.Substring(0, comboBoxPtzOpDirectCommand.Text.IndexOf("-")));
            pPtzOperateDirectParam.szExtend = textPtzOpDirectExtend.Text;
            DPSDK_PTZOPERATE_RESULT pPtzOperateResult = new DPSDK_PTZOPERATE_RESULT();
            DPSDK_RES iRet = DPSDK_PtzOperateDirect(m_iSessionID, ref pPtzOperateDirectParam, ref pPtzOperateResult);
            if (iRet == DPSDK_SUCCESS)
            {
                textPtzOpResult.Text = AppData._CS("PtzOperateDirect Success. iResult=%1 LockUserName=%2 LockUserLevel=%3.", pPtzOperateResult.iResult.ToString(), pPtzOperateResult.struLockUser.szLockUserName, pPtzOperateResult.struLockUser.iLockUserLevel.ToString());
            }
            else
            {
                textPtzOpResult.Text = AppData._CS("PtzOperateDirect failed. error code = %1", iRet.ToString());
            }
        }

        private void btnPtzOpFocus_Click(object sender, EventArgs e)
        {
            if (textPtzChannelId.Text == "" || textPtzOpFocus.Text == "" || textPtzOpFocusZoom.Text == "")
            {
                AppData.ShowMsgInfo(AppData._CS("Please input correct params!"));
                return;
            }
            DPSDK_PTZOPERATE_FOCUS_PARAM pPtzOperateFocusParam = new DPSDK_PTZOPERATE_FOCUS_PARAM();
            pPtzOperateFocusParam.szChannelId = textPtzChannelId.Text;
            pPtzOperateFocusParam.iOperateType = Convert.ToInt32(comboBoxPtzOpFocusType.Text.Substring(0, comboBoxPtzOpFocusType.Text.IndexOf("-")));
            pPtzOperateFocusParam.fFocus = (float)Convert.ToDouble(textPtzOpFocus.Text);
            pPtzOperateFocusParam.fZoom = (float)Convert.ToDouble(textPtzOpFocusZoom.Text);
            DPSDK_PTZOPERATE_RESULT pPtzOperateResult = new DPSDK_PTZOPERATE_RESULT();
            DPSDK_RES iRet = DPSDK_PtzOperateFocus(m_iSessionID, ref pPtzOperateFocusParam, ref pPtzOperateResult);
            if (iRet == DPSDK_SUCCESS)
            {
                textPtzOpResult.Text = AppData._CS("PtzOperateFocus Success. iResult=%1 LockUserName=%2 LockUserLevel=%3.", pPtzOperateResult.iResult.ToString(), pPtzOperateResult.struLockUser.szLockUserName, pPtzOperateResult.struLockUser.iLockUserLevel.ToString());
            }
            else
            {
                textPtzOpResult.Text = AppData._CS("PtzOperateFocus failed. error code = %1", iRet.ToString());
            }
        }

        private void btnPtzOpPresetPoint_Click(object sender, EventArgs e)
        {
            if (textPtzChannelId.Text == "")
            {
                AppData.ShowMsgInfo(AppData._CS("Please input correct params!"));
                return;
            }
            DPSDK_PTZOPERATE_PRESETPOINT_PARAM pPtzOperatePrePointParam = new DPSDK_PTZOPERATE_PRESETPOINT_PARAM();
            pPtzOperatePrePointParam.szChannelId = textPtzChannelId.Text;
            pPtzOperatePrePointParam.iOperateType = Convert.ToInt32(comboBoxPtzOpPresetType.Text.Substring(0, comboBoxPtzOpPresetType.Text.IndexOf("-")));
            pPtzOperatePrePointParam.szPointCode = textPtzOpPresetPointCode.Text;
            pPtzOperatePrePointParam.szPointName = textPtzOpPresetPointName.Text;
            pPtzOperatePrePointParam.szStartTime = textPtzOpPresetStartTime.Text;
            pPtzOperatePrePointParam.szEndTime = textPtzOpPresetEndTime.Text;
            pPtzOperatePrePointParam.fSpeed = 0;
            DPSDK_PTZOPERATE_RESULT pPtzOperateResult = new DPSDK_PTZOPERATE_RESULT();
            DPSDK_RES iRet = DPSDK_PtzOperatePresetPoint(m_iSessionID, ref pPtzOperatePrePointParam, ref pPtzOperateResult);
            if (iRet == DPSDK_SUCCESS)
            {
                textPtzOpResult.Text = AppData._CS("PtzOperatePresetPoint Success. iResult=%1 LockUserName=%2 LockUserLevel=%3.", pPtzOperateResult.iResult.ToString(), pPtzOperateResult.struLockUser.szLockUserName, pPtzOperateResult.struLockUser.iLockUserLevel.ToString());
            }
            else
            {
                textPtzOpResult.Text = AppData._CS("PtzOperatePresetPoint failed. error code = %1", iRet.ToString());
            }
        }

        private void btnPtzOpGetPreset_Click(object sender, EventArgs e)
        {
            if (textPtzChannelId.Text == "")
            {
                AppData.ShowMsgInfo(AppData._CS("Please input correct params!"));
                return;
            }
            uint uiBufLen = (uint)(Marshal.SizeOf(typeof(UInt32)) + Marshal.SizeOf(typeof(DPSDK_PTZ_PRESETPOINT_INFO)) * 100);
            IntPtr pPresetPointList = Marshal.AllocHGlobal((int)uiBufLen);
            DPSDK_RES iRet = DPSDK_PtzGetPresetPoints(m_iSessionID, textPtzChannelId.Text, pPresetPointList, uiBufLen);
            if (iRet == DPSDK_SUCCESS)
            {
                textPtzOpResult.Text = AppData._CS("PtzGetPresetPoints success.") + "\r\n";
                DPSDK_PTZ_PRESETPOINT_LIST stuPresetPointList = new DPSDK_PTZ_PRESETPOINT_LIST();
                stuPresetPointList = (DPSDK_PTZ_PRESETPOINT_LIST)Marshal.PtrToStructure(pPresetPointList, typeof(DPSDK_PTZ_PRESETPOINT_LIST));
                if (stuPresetPointList.uiTotal > 0)
                {
                    DPSDK_PTZ_PRESETPOINT_INFO[] pPresetPointInfo = new DPSDK_PTZ_PRESETPOINT_INFO[stuPresetPointList.uiTotal];
                    for (int i = 0; i < (int)stuPresetPointList.uiTotal; i++)
                    {
                        IntPtr ptr = new IntPtr(pPresetPointList.ToInt32() + Marshal.SizeOf(typeof(UInt32)) + Marshal.SizeOf(typeof(DPSDK_PTZ_PRESETPOINT_INFO)) * i);
                        pPresetPointInfo[i] = (DPSDK_PTZ_PRESETPOINT_INFO)Marshal.PtrToStructure(ptr, typeof(DPSDK_PTZ_PRESETPOINT_INFO));
                        textPtzOpResult.Text += "PointCode = " + pPresetPointInfo[i].szPointCode + ", PointName = " + Encoding.GetEncoding("utf-8").GetString(Encoding.Default.GetBytes(pPresetPointInfo[i].szPointName)) + ", PointType = " + pPresetPointInfo[i].iPointType.ToString() + "\r\n";
                    }
                }
            }
            else
            {
                textPtzOpResult.Text = AppData._CS("PtzGetPresetPoints failed. error code = %1", iRet.ToString());
            }
            Marshal.FreeHGlobal(pPresetPointList);
        }

        private void btnPtzOpSitPos_Click(object sender, EventArgs e)
        {
            if (textPtzChannelId.Text == "" || textPtzOpSitPosPointX.Text == "" || textPtzOpSitPosPointY.Text == "" || textPtzOpSitPosPointZ.Text == "")
            {
                AppData.ShowMsgInfo(AppData._CS("Please input correct params!"));
                return;
            }
            DPSDK_PTZOPERATE_SITPOSITION_PARAM pPtzOperateSitPositionParam = new DPSDK_PTZOPERATE_SITPOSITION_PARAM();
            pPtzOperateSitPositionParam.szChannelId = textPtzChannelId.Text;
            pPtzOperateSitPositionParam.fPointX = Convert.ToDouble(textPtzOpSitPosPointX.Text);
            pPtzOperateSitPositionParam.fPointY = Convert.ToDouble(textPtzOpSitPosPointY.Text);
            pPtzOperateSitPositionParam.fPointZ = Convert.ToDouble(textPtzOpSitPosPointZ.Text);
            pPtzOperateSitPositionParam.szExtend = textPtzOpSitPosExtend.Text;
            DPSDK_PTZOPERATE_RESULT pPtzOperateResult = new DPSDK_PTZOPERATE_RESULT();
            DPSDK_RES iRet = DPSDK_PtzSitPosition(m_iSessionID, ref pPtzOperateSitPositionParam, ref pPtzOperateResult);
            if (iRet == DPSDK_SUCCESS)
            {
                textPtzOpResult.Text = AppData._CS("PtzSitPosition Success. iResult=%1 LockUserName=%2 LockUserLevel=%3.", pPtzOperateResult.iResult.ToString(), pPtzOperateResult.struLockUser.szLockUserName, pPtzOperateResult.struLockUser.iLockUserLevel.ToString());
            }
            else
            {
                textPtzOpResult.Text = AppData._CS("PtzSitPosition failed. error code = %1", iRet.ToString());
            }
        }

        private void btnPtzOpArrangePtz_Click(object sender, EventArgs e)
        {
            if (textPtzChannelId.Text == "" || textPtzOpArrangeLockTime.Text == "")
            {
                AppData.ShowMsgInfo(AppData._CS("Please input correct params!"));
                return;
            }
            DPSDK_PTZOPERATE_ARRANGEPTZ_PARAM pPtzOperateArrangePtzParam = new DPSDK_PTZOPERATE_ARRANGEPTZ_PARAM();
            pPtzOperateArrangePtzParam.szChannelId = textPtzChannelId.Text;
            pPtzOperateArrangePtzParam.iOperateType = Convert.ToInt32(comboBoxPtzOpArrangeType.Text.Substring(0, comboBoxPtzOpArrangeType.Text.IndexOf("-")));
            pPtzOperateArrangePtzParam.uiLockTime = Convert.ToUInt32(textPtzOpArrangeLockTime.Text);
            pPtzOperateArrangePtzParam.szExtend = textPtzOpArrangeExtend.Text;
            DPSDK_PTZOPERATE_RESULT pPtzOperateResult = new DPSDK_PTZOPERATE_RESULT();
            DPSDK_RES iRet = DPSDK_PtzArrangePtz(m_iSessionID, ref pPtzOperateArrangePtzParam, ref pPtzOperateResult);
            if (iRet == DPSDK_SUCCESS)
            {
                textPtzOpResult.Text = AppData._CS("PtzArrangePtz success. iResult=%1 LockUserName=%2 LockUserLevel=%3.", pPtzOperateResult.iResult.ToString(), pPtzOperateResult.struLockUser.szLockUserName, pPtzOperateResult.struLockUser.iLockUserLevel.ToString());
            }
            else
            {
                textPtzOpResult.Text = AppData._CS("PtzArrangePtz failed. error code = %1", iRet.ToString());
            }
        }

        private void btnPtzOpAlarmOut_Click(object sender, EventArgs e)
        {
            if (textPtzChannelId.Text == "")
            {
                AppData.ShowMsgInfo(AppData._CS("Please input correct params!"));
                return;
            }
            DPSDK_PTZOPERATE_ALARMOUT_PARAM pAlarmOutParam = new DPSDK_PTZOPERATE_ALARMOUT_PARAM();
            pAlarmOutParam.szChannelId = textPtzChannelId.Text;
            pAlarmOutParam.iOperateType = Convert.ToInt32(comboBoxPtzOpAlarmOutType.Text.Substring(0, comboBoxPtzOpAlarmOutType.Text.IndexOf("-")));
            pAlarmOutParam.iCommand = Convert.ToInt32(comboBoxPtzOpAlarmOutCommand.Text.Substring(0, comboBoxPtzOpAlarmOutCommand.Text.IndexOf("-")));
            DPSDK_PTZOPERATE_RESULT pPtzOperateResult = new DPSDK_PTZOPERATE_RESULT();
            DPSDK_RES iRet = DPSDK_AlarmActionOut(m_iSessionID, ref pAlarmOutParam, ref pPtzOperateResult);
            if (iRet == DPSDK_SUCCESS)
            {
                textPtzOpResult.Text = AppData._CS("AlarmActionOut success. iResult=%1 LockUserName=%2 LockUserLevel=%3.", pPtzOperateResult.iResult.ToString(), pPtzOperateResult.struLockUser.szLockUserName, pPtzOperateResult.struLockUser.iLockUserLevel.ToString());
            }
            else
            {
                textPtzOpResult.Text = AppData._CS("AlarmActionOut failed. error code = %1", iRet.ToString());
            }
        }

        private void btnPtzOpClear_Click(object sender, EventArgs e)
        {
            textPtzOpResult.Text = "";
        }
    }
}