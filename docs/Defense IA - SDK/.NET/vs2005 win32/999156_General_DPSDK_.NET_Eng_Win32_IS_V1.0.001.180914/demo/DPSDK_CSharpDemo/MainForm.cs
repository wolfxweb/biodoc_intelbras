using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Collections;

namespace DPSDK_CSharpDemo
{
    public partial class CSharpDemo : Form
    {
        public CSharpDemo()
        {
            AppData.Init();
            m_iSessionID = IntPtr.Zero;
            m_iPreviewMediaSessionID = IntPtr.Zero;
            m_iPlaybackMediaSessionID = IntPtr.Zero;
            m_iDownloadMediaSessionID = IntPtr.Zero;
            m_iConvertType = -1;
            m_struDevInfoAll = new Dev_Info_All();
            m_struDevInfoAll.vecDevInfo = new ArrayList();
            m_struDepInfoAll = new Dep_Info_All();
            m_struDepInfoAll.vecSubDepInfo = new ArrayList();
            m_struDepInfoAll.vecDevID = new ArrayList();
            m_struDepInfoAll.vecChnlID = new ArrayList();
            m_mapIdName = new Dictionary<string, string>();
            m_iAlarmExportSessionID = 0;

            m_fEventCallbackFun = fEventCallBack;
            m_fEventDownloadCallback = fEventDownloadCallback;

            InitializeComponent();

            CheckForIllegalCrossThreadCalls = false;//Support multithreading

            this.tabControl1.ItemSize = new System.Drawing.Size(30, 120);

            //General
            Demo_Save_Login_Info_t struLoginInfo = LocalConfig.GetLastLoginInfo();
            this.textServerIP.Text = struLoginInfo.strIP;
            this.textServerPort.Text = struLoginInfo.strPort;
            this.textUserName.Text = struLoginInfo.strUserName;
            this.textPassword.Text = struLoginInfo.strPWD;

            //Real preview
            this.comboBoxPreviewPicture.SelectedIndex = this.comboBoxPreviewPicture.Items.IndexOf("BMP");

            //Alarm
            this.comboBoxAlarmHandleStatus.SelectedIndex = this.comboBoxAlarmHandleStatus.Items.IndexOf("1-In the process of processing");
            this.comboBoxAlarmQuerySortType.SelectedIndex = this.comboBoxAlarmQuerySortType.Items.IndexOf("1-Alarm time");
            this.comboBoxAlarmQuerySortOrder.SelectedIndex = this.comboBoxAlarmQuerySortOrder.Items.IndexOf("0-Ascending order");
            this.comboBoxAlarmExportSortType.SelectedIndex = this.comboBoxAlarmExportSortType.Items.IndexOf("1-Alarm time");
            this.comboBoxAlarmExportSortOrder.SelectedIndex = this.comboBoxAlarmExportSortOrder.Items.IndexOf("0-Ascending order");

            //PTZ function
            this.comboBoxPtzOpType.SelectedIndex = this.comboBoxPtzOpType.Items.IndexOf("0-Show_PtzMenu");
            this.comboBoxPtzSwitch.SelectedIndex = this.comboBoxPtzSwitch.Items.IndexOf("0-Close");
            this.comboBoxPtzBorderType.SelectedIndex = this.comboBoxPtzBorderType.Items.IndexOf("16-Left boundary");
            this.comboBoxPtzAssisentType.SelectedIndex = this.comboBoxPtzAssisentType.Items.IndexOf("23-Backlight compensation");
            this.comboBoxPtzMoveType.SelectedIndex = this.comboBoxPtzMoveType.Items.IndexOf("25-Upward movement");
            this.comboBoxPtzSwitchMenu.SelectedIndex = this.comboBoxPtzSwitchMenu.Items.IndexOf("22-Open the platform menu");
            this.comboBoxPtzOpCameraDirect.SelectedIndex = this.comboBoxPtzOpCameraDirect.Items.IndexOf("1-Increase");
            this.comboBoxPtzOpCameraCommand.SelectedIndex = this.comboBoxPtzOpCameraCommand.Items.IndexOf("0-Stop");
            this.comboBoxPtzOpCameraType.SelectedIndex = this.comboBoxPtzOpCameraType.Items.IndexOf("1-Variable");
            this.comboBoxPtzOpDirect.SelectedIndex = this.comboBoxPtzOpDirect.Items.IndexOf("1-Upper");
            this.comboBoxPtzOpDirectCommand.SelectedIndex = this.comboBoxPtzOpDirectCommand.Items.IndexOf("0-Stop");
            this.comboBoxPtzOpFocusType.SelectedIndex = this.comboBoxPtzOpFocusType.Items.IndexOf("0-Reset");
            this.comboBoxPtzOpPresetType.SelectedIndex = this.comboBoxPtzOpPresetType.Items.IndexOf("1-Location");
            this.comboBoxPtzOpArrangeType.SelectedIndex = this.comboBoxPtzOpArrangeType.Items.IndexOf("0-Unknow");
            this.comboBoxPtzOpAlarmOutType.SelectedIndex = this.comboBoxPtzOpAlarmOutType.Items.IndexOf("1-Status control");
            this.comboBoxPtzOpAlarmOutCommand.SelectedIndex = this.comboBoxPtzOpAlarmOutCommand.Items.IndexOf("0-Close");

            //Playback
            this.comboBoxPlaybackSourceType.SelectedIndex = this.comboBoxPlaybackSourceType.Items.IndexOf("1-All");
            this.comboBoxPlaybackStreamType.SelectedIndex = this.comboBoxPlaybackStreamType.Items.IndexOf("1-Main stream");
            this.comboBoxPlaybackQueryRecordType.SelectedIndex = this.comboBoxPlaybackQueryRecordType.Items.IndexOf("0-All");
            this.comboBoxPlaybackSpeed.SelectedIndex = this.comboBoxPlaybackSpeed.Items.IndexOf("1");
            this.comboBoxPlaybackDirection.SelectedIndex = this.comboBoxPlaybackDirection.Items.IndexOf("0-Direction");
            this.comboBoxPlaybackFileFormat.SelectedIndex = this.comboBoxPlaybackFileFormat.Items.IndexOf("1-AVI");
            this.comboBoxPlaybackNameRule.SelectedIndex = this.comboBoxPlaybackNameRule.Items.IndexOf("0-ChannelID");
            this.comboBoxPlaybackRecordType.SelectedIndex = this.comboBoxPlaybackRecordType.Items.IndexOf("0-All");
            this.comboBoxPlaybackPicFormat.SelectedIndex = this.comboBoxPlaybackPicFormat.Items.IndexOf("0-BMP");
            InitLanguage();
        }

        private void InitLanguage()
        {
            //tab
            this.General.Text = AppData._CS("General");
            this.Device.Text = AppData._CS("DeviceTree");
            this.Preview.Text = AppData._CS("Preview");
            this.DeviceFunction.Text = AppData._CS("DeviceFunction");
            this.Playback.Text = AppData._CS("Playback");
            this.Alarm.Text = AppData._CS("Alarm");

            //
            this.btnInit.Text = AppData._CS("Init System");
            this.btnUnInit.Text = AppData._CS("UnInit System");

            //General
            this.label_IP.Text = AppData._CS("Server IP");
            this.label_Port.Text = AppData._CS("Port");
            this.label_Name.Text = AppData._CS("UserName");
            this.label_PWD.Text = AppData._CS("Password");
            this.btnLogin.Text = AppData._CS("Login");
            this.btnLogout.Text = AppData._CS("Logout");
            this.groupBox_LoginOP.Text = AppData._CS("Login Option");
            this.checkBoxHttps.Text = AppData._CS("HTTPS Mode");
            this.checkBoxDoMain.Text = AppData._CS("doMain");
            this.groupBox_ModPWD.Text = AppData._CS("Modify Password");
            this.label_NewPWD.Text = AppData._CS("New Password");
            this.btnModifyPassword.Text = AppData._CS("Modify Password");
            this.groupBox_ClientInfo.Text = AppData._CS("Client Info");
            this.btnClientVersion.Text = AppData._CS("Client Version");
            this.btnClientRoad.Text = AppData._CS("Client Road");
            this.btnQueryMenuRight.Text = AppData._CS("Query Menu Right");
            this.btnQueryEmap.Text = AppData._CS("Query Emap");
            this.btnQueryFTP.Text = AppData._CS("Query FTP");
            this.btnSyncTime.Text = AppData._CS("Sync Time");
            this.label_Res.Text = AppData._CS("Handle Result");

            //Device
            this.groupBox_OrgParam.Text = AppData._CS("Query Organization Param");
            this.label_Org.Text = AppData._CS("Org Code");
            this.checkBoxDeviceContainDevice.Text = AppData._CS("Contain Device");
            this.checkBoxDeviceCompress.Text = AppData._CS("Compress");
            this.btnDeviceQueryByLayered.Text = AppData._CS("Query Org");
            this.groupBox_UserData.Text = AppData._CS("User Data");
            this.label_UserDataName.Text = AppData._CS("User Data Name");
            this.label_UserDataInfo.Text = AppData._CS("User Data Info");
            this.btnDeviceGetUserData.Text = AppData._CS("Get User Data");
            this.btnDeviceSaveUserData.Text = AppData._CS("Save User Data");
            this.btnDeviceDelUserData.Text = AppData._CS("Del User Data");
            this.radioButtonDeviceOrg.Text = AppData._CS("Org");
            this.radioButtonDeviceDevice.Text = AppData._CS("Device");
            this.radioButtonDeviceChannel.Text = AppData._CS("Channel");

            //Alarm
            this.tabAlarmQuery.Text = AppData._CS("Query");
            this.tabAlarmExport.Text = AppData._CS("Export");
            this.tabAlarmConfirm.Text = AppData._CS("AlarmConfirm");
            this.label_QueryABT.Text = AppData._CS("AlarmBeginTime");
            this.label_QueryAET.Text = AppData._CS("AlarmEndTime");
            this.label_QueryHBT.Text = AppData._CS("HandleBeginTime");
            this.label_QueryHET.Text = AppData._CS("HandleEndTime");
            this.label_QueryDevID.Text = AppData._CS("DeviceCode");
            this.label_QueryChnlID.Text = AppData._CS("ChannelCode");
            this.label_QueryOrgID.Text = AppData._CS("OrgCode");
            this.label_QueryAlarmID.Text = AppData._CS("AlarmId");
            this.label_QueryAlarmCode.Text = AppData._CS("AlarmCode");
            this.label_QueryHU.Text = AppData._CS("HandleUser");
            this.label_QueryPZ.Text = AppData._CS("PageSize");
            this.label_QueryPN.Text = AppData._CS("PageNum");
            this.label_QueryST.Text = AppData._CS("SortType");
            this.label_QuerySO.Text = AppData._CS("SortOrder");
            this.label_QueryATL.Text = AppData._CS("AlarmTypeList");
            this.label_QueryAGL.Text = AppData._CS("AlarmGradeList");
            this.label_QueryASL.Text = AppData._CS("AlarmStatusList");
            this.label_QueryHSL.Text = AppData._CS("HandleStatusList");
            this.label_QueryLan.Text = AppData._CS("Language");
            this.btnAlarmQuery.Text = AppData._CS("AlarmQuery");
            this.btnAlarmCountQuery.Text = AppData._CS("AlarmCountQuery");
            this.btnAlarmTypeQuery.Text = AppData._CS("AlarmTypeQuery");
            this.comboBoxAlarmQuerySortOrder.Items[0] = AppData._CS("0-Ascending order");
            this.comboBoxAlarmQuerySortOrder.Items[1] = AppData._CS("1-Descending order");
            this.comboBoxAlarmQuerySortType.Items[0] = AppData._CS("1-Alarm time");
            this.comboBoxAlarmQuerySortType.Items[1] = AppData._CS("2-Alarm type");
            this.comboBoxAlarmQuerySortType.Items[2] = AppData._CS("3-Alarm level");
            this.comboBoxAlarmQuerySortType.Items[3] = AppData._CS("4-HandleUser");
            this.comboBoxAlarmQuerySortType.Items[4] = AppData._CS("5-Processing state");
            this.checkBoxAlarmForbiddenNotify.Text = AppData._CS("Shielded push");
            this.checkBoxAlarmStopRefresh.Text = AppData._CS("Stop refresh");
            this.btnAlarmClear.Text = AppData._CS("Clear");
            this.label_ExportABT.Text = AppData._CS("AlarmBeginTime");
            this.label_ExportAET.Text = AppData._CS("AlarmEndTime");
            this.label_ExportHBT.Text = AppData._CS("HandleBeginTime");
            this.label_ExportHET.Text = AppData._CS("HandleEndTime");
            this.label_ExportDevID.Text = AppData._CS("DeviceCode");
            this.label_ExportChnlID.Text = AppData._CS("ChannelCode");
            this.label_ExportOrgID.Text = AppData._CS("OrgCode");
            this.label_ExportAlarmID.Text = AppData._CS("AlarmId");
            this.label_ExportAlarmCode.Text = AppData._CS("AlarmCode");
            this.label_ExportHU.Text = AppData._CS("HandleUser");
            this.label_ExportST.Text = AppData._CS("SortType");
            this.label_ExportSO.Text = AppData._CS("SortOrder");
            this.label_ExportATL.Text = AppData._CS("AlarmTypeList");
            this.label_ExportAGL.Text = AppData._CS("AlarmGradeList");
            this.label_ExportASL.Text = AppData._CS("AlarmStatusList");
            this.label_ExportHSL.Text = AppData._CS("HandleStatusList");
            this.label_ExportLan.Text = AppData._CS("Language");
            this.btnAlarmExport.Text = AppData._CS("AlarmExport");
            this.comboBoxAlarmExportSortOrder.Items[0] = AppData._CS("0-Ascending order");
            this.comboBoxAlarmExportSortOrder.Items[1] = AppData._CS("1-Descending order");
            this.comboBoxAlarmExportSortType.Items[0] = AppData._CS("1-Alarm time");
            this.comboBoxAlarmExportSortType.Items[1] = AppData._CS("2-Alarm type");
            this.comboBoxAlarmExportSortType.Items[2] = AppData._CS("3-Alarm level");
            this.comboBoxAlarmExportSortType.Items[3] = AppData._CS("4-HandleUser");
            this.comboBoxAlarmExportSortType.Items[4] = AppData._CS("5-Processing state");
            this.label_ConfirmAC.Text = AppData._CS("AlarmCode");
            this.label_ConfirmHU.Text = AppData._CS("HandleUser");
            this.label_ConfirmHM.Text = AppData._CS("HandleMsg");
            this.label_ConfirmHS.Text = AppData._CS("HandleStatus");
            this.label_ConfirmEL.Text = AppData._CS("EmailList");
            this.btnAlarmConfirm.Text = AppData._CS("AlarmConfirm");
            this.label_ConfirmAPF_AC.Text = AppData._CS("AlarmCode");
            this.btnAlarmQueryPF.Text = AppData._CS("AlarmProcessFlow");
            this.label_ConfirmACS.Text = AppData._CS("AlarmSourceCode");
            this.label_ConfirmAT.Text = AppData._CS("AlarmType");
            this.label_ConfirmDS.Text = AppData._CS("Duration(s)");
            this.btnAlarmBlock.Text = AppData._CS("AlarmBlock");
            this.comboBoxAlarmHandleStatus.Items[0] = AppData._CS("1-In the process of processing");
            this.comboBoxAlarmHandleStatus.Items[1] = AppData._CS("2-Resolved");
            this.comboBoxAlarmHandleStatus.Items[2] = AppData._CS("3-False positives");
            this.comboBoxAlarmHandleStatus.Items[3] = AppData._CS("4-Ignore");
            this.comboBoxAlarmHandleStatus.Items[4] = AppData._CS("5-Unsolved");

            //Preview
            this.PreviewVideo.Text = AppData._CS("Video");
            this.PreviewPicture.Text = AppData._CS("Picture");
            this.PreviewSound.Text = AppData._CS("Sound");
            this.PreviewRecord.Text = AppData._CS("Record");
            this.groupVideoPlay.Text = AppData._CS("VideoPlay");
            this.radioPreviewSinglePlay.Text = AppData._CS("SinglePlay");
            this.groupVBP.Text = AppData._CS("VideoBaseParam");
            this.label_PreDevID.Text = AppData._CS("ChannelCode");
            this.label_PreST.Text = AppData._CS("StreamType");
            this.label_PreDT.Text = AppData._CS("DataType");
            this.checkBoxPreviewDecode.Text = AppData._CS("IsDecode");
            this.radioPreviewMultiviewPlay.Text = AppData._CS("MulitviewPlay");
            this.groupMP.Text = AppData._CS("MulitviewParam");
            this.label_PreSN.Text = AppData._CS("ScreenNum");
            this.label_PreTrackID.Text = AppData._CS("TrackID");
            this.label_PreSI.Text = AppData._CS("StartIndex");
            this.btnPreviewStartVideo.Text = AppData._CS("StartVideo");
            this.btnPreviewStopVideo.Text = AppData._CS("StopVideo");
            this.groupStreamMode.Text = AppData._CS("StreamMode");
            this.label_PreSM.Text = AppData._CS("StreamMode");
            this.label_PreDelT.Text = AppData._CS("DelayTime");
            this.btnPreviewGetStreamMode.Text = AppData._CS("GetStreamMode");
            this.btnPreviewSetStreamMode.Text = AppData._CS("SetStreamMode");
            this.groupDisRegion.Text = AppData._CS("DisplayRegion");
            this.label_PreTop.Text = AppData._CS("Top");
            this.label_PreBottom.Text = AppData._CS("Bottom");
            this.label_PreLeft.Text = AppData._CS("Left");
            this.label_PreRight.Text = AppData._CS("Right");
            this.checkBoxPreviewEnable.Text = AppData._CS("Enable");
            this.btnPreviewSetRegion.Text = AppData._CS("SetRegion");
            this.btnPreviewLockVideo.Text = AppData._CS("LockVideo");
            this.label_PreHR.Text = AppData._CS("Handle Result");

            this.groupGetPic.Text = AppData._CS("GetPicture");
            this.label_PrePicPath.Text = AppData._CS("PicturePath");
            this.btnPreviewGetPicSize.Text = AppData._CS("GetPictureSize");
            this.btnPreviewGetPicToBuffer.Text = AppData._CS("GetPictureToBuffer");
            this.btnPreviewGetPicToFile.Text = AppData._CS("GetPictureToFile");
            //ui.labelPictureBuf.Text = AppData._CS("Thumbnail");

            this.btnPreviewOpenSound.Text = AppData._CS("OpenSound");
            this.btnPreviewCloseSound.Text = AppData._CS("CloseSound");
            this.btnPreviewOpenShareSound.Text = AppData._CS("Open Share Sound");
            this.btnPreviewCloseShareSound.Text = AppData._CS("Close Share Sound");
            this.btnPreviewIsOpenSound.Text = AppData._CS("IsOpenSound");
            this.btnPreviewSetVolume.Text = AppData._CS("SetVolume");
            this.btnPreviewGetVolum.Text = AppData._CS("GetVolume");

            this.label_PreRecPath.Text = AppData._CS("RecordPath");
            this.label_PreSplitLen.Text = AppData._CS("SplitLen");
            this.btnPreviewSetSplitLen.Text = AppData._CS("SplitLen");
            this.btnPreviewStartRecord.Text = AppData._CS("StartRecord");
            this.btnPreviewStopRecord.Text = AppData._CS("StopRecord");
            this.btnPreviewIsRecordState.Text = AppData._CS("IsRecordState");

            //DeviceFunction
            this.label_PTZChnlCode.Text = AppData._CS("ChannelCode");
            this.tabPage_PTZOpFun.Text = AppData._CS("Function");
            this.tabPageOPCamera.Text = AppData._CS("Camera");
            this.tabPagePreset.Text = AppData._CS("Preset");
            this.tabPagePTZOther.Text = AppData._CS("Other");
            this.groupBox_PTZFun.Text = AppData._CS("PtzOperateFunction");
            this.label_PTZOpType.Text = AppData._CS("OperateType");
            this.label_PTZCruID.Text = AppData._CS("CruiseID");
            this.label_PTZTraclID.Text = AppData._CS("TrackID");
            this.label_PTZSwith.Text = AppData._CS("Switch");
            this.label_PTZBT.Text = AppData._CS("BorderType");
            this.label_PTZAT.Text = AppData._CS("AssisentType");
            this.label_PTZMT.Text = AppData._CS("MoveType");
            this.label_PTZSM.Text = AppData._CS("SwitchMenu");
            this.btnPtzOperateFunction.Text = AppData._CS("OperateFunction");

            this.comboBoxPtzOpType.Items[0] = AppData._CS("0-Show_PtzMenu");
            this.comboBoxPtzOpType.Items[1] = AppData._CS("1-Move_PtzMenu");
            this.comboBoxPtzOpType.Items[2] = AppData._CS("2-Confirm_PtzMenuItem");
            this.comboBoxPtzOpType.Items[3] = AppData._CS("3-Set_LineScannBorder");
            this.comboBoxPtzOpType.Items[4] = AppData._CS("4-Switch_LineScanBorder");
            this.comboBoxPtzOpType.Items[5] = AppData._CS("5-Switch_AutoRotate");
            this.comboBoxPtzOpType.Items[6] = AppData._CS("6-Switch_Light");
            this.comboBoxPtzOpType.Items[7] = AppData._CS("7-Switch_RainBrush");
            this.comboBoxPtzOpType.Items[8] = AppData._CS("8-Switch_InfraredLight");
            this.comboBoxPtzOpType.Items[9] = AppData._CS("9-Switch_AssisentPoint");
            this.comboBoxPtzOpType.Items[10] = AppData._CS("10-Switch_Cruise");
            this.comboBoxPtzOpType.Items[11] = AppData._CS("11-Switch_Track");
            this.comboBoxPtzOpType.Items[12] = AppData._CS("12-Switch_SetTrack");
            this.comboBoxPtzSwitch.Items[0] = AppData._CS("0-Close");
            this.comboBoxPtzSwitch.Items[1] = AppData._CS("1-Open");
            this.comboBoxPtzBorderType.Items[0] = AppData._CS("16-Left boundary");
            this.comboBoxPtzBorderType.Items[1] = AppData._CS("17-Right boundary");
            this.comboBoxPtzAssisentType.Items[0] = AppData._CS("23-Backlight compensation");
            this.comboBoxPtzAssisentType.Items[1] = AppData._CS("24-Number doubled");
            this.comboBoxPtzAssisentType.Items[2] = AppData._CS("27-Color turn black");
            this.comboBoxPtzAssisentType.Items[3] = AppData._CS("35-Shutter time");
            this.comboBoxPtzAssisentType.Items[4] = AppData._CS("41-Brightness");
            this.comboBoxPtzAssisentType.Items[5] = AppData._CS("42-Image flip");
            this.comboBoxPtzAssisentType.Items[6] = AppData._CS("43-Hidding preset point's name");
            this.comboBoxPtzAssisentType.Items[7] = AppData._CS("80-Restore factory settings");
            this.comboBoxPtzMoveType.Items[0] = AppData._CS("25-Upward movement");
            this.comboBoxPtzMoveType.Items[1] = AppData._CS("26-Move down");
            this.comboBoxPtzMoveType.Items[2] = AppData._CS("27-Left shift");
            this.comboBoxPtzMoveType.Items[3] = AppData._CS("28-Right shift");
            this.comboBoxPtzSwitchMenu.Items[0] = AppData._CS("22-Open the platform menu");
            this.comboBoxPtzSwitchMenu.Items[1] = AppData._CS("23-Close the platform menu");
            this.groupBox_PTZOpCam.Text = AppData._CS("PtzOperateCamera");
            this.label_PTZCamDir.Text = AppData._CS("Direct");
            this.label_PTZCom.Text = AppData._CS("Command");
            this.label_PTZOpT.Text = AppData._CS("OperateType");
            this.label_PTZStep.Text = AppData._CS("Step");
            this.label_PTZCamEx.Text = AppData._CS("Extend");
            this.btnPtzOpCamera.Text = AppData._CS("OperateCamera");
            this.groupBoxPTZDir.Text = AppData._CS("PtzOperateDirect");
            this.label_PTZDir.Text = AppData._CS("Direct");
            this.label_PTZDirCom.Text = AppData._CS("Command");
            this.label_PTZStepX.Text = AppData._CS("StepX");
            this.label_PTZStepY.Text = AppData._CS("StepY");
            this.label_PTZDorEx.Text = AppData._CS("Extend");
            this.btnPtzOpDirect.Text = AppData._CS("OperateDirect");
            this.groupBox_PTZFocus.Text = AppData._CS("PtzOperateFocus");
            this.label_PTZForOpType.Text = AppData._CS("OperateType");
            this.label_PTZFor.Text = AppData._CS("Focus");
            this.label_PTZForZoom.Text = AppData._CS("Zoom");
            this.btnPtzOpFocus.Text = AppData._CS("OperateFocus");
            this.comboBoxPtzOpCameraDirect.Items[0] = AppData._CS("1-Increase");
            this.comboBoxPtzOpCameraDirect.Items[1] = AppData._CS("2-Decrease");
            this.comboBoxPtzOpCameraCommand.Items[0] = AppData._CS("0-Stop");
            this.comboBoxPtzOpCameraCommand.Items[1] = AppData._CS("1-Open");
            this.comboBoxPtzOpCameraType.Items[0] = AppData._CS("1-Variable");
            this.comboBoxPtzOpCameraType.Items[1] = AppData._CS("2-Zoom");
            this.comboBoxPtzOpCameraType.Items[2] = AppData._CS("3-Aperture");
            this.comboBoxPtzOpDirect.Items[0] = AppData._CS("1-Upper");
	        this.comboBoxPtzOpDirect.Items[1] = AppData._CS("2-Under");
	        this.comboBoxPtzOpDirect.Items[2] = AppData._CS("3-Left");
	        this.comboBoxPtzOpDirect.Items[3] = AppData._CS("4-Right");
	        this.comboBoxPtzOpDirect.Items[4] = AppData._CS("5-Upper Left");
	        this.comboBoxPtzOpDirect.Items[5] = AppData._CS("6-Under Left");
	        this.comboBoxPtzOpDirect.Items[6] = AppData._CS("7-Upper Right");
	        this.comboBoxPtzOpDirect.Items[7] = AppData._CS("8-Under Right");
	        this.comboBoxPtzOpDirectCommand.Items[0] = AppData._CS("0-Stop");
            this.comboBoxPtzOpDirectCommand.Items[1] = AppData._CS("1-Open");
            this.comboBoxPtzOpFocusType.Items[0] = AppData._CS("0-Reset");
            this.comboBoxPtzOpFocusType.Items[1] = AppData._CS("1-Continuous focusing");
            this.comboBoxPtzOpFocusType.Items[2] = AppData._CS("2-Autofocusing");
            this.groupBox_PTZPreset.Text = AppData._CS("PtzOperatePresetPoint");
            this.label_PTZPreOpT.Text = AppData._CS("OperateType");
            this.label_PTZPointC.Text = AppData._CS("PointCode");
            this.label_PTZPointN.Text = AppData._CS("PointName");
            this.label_PTZPointST.Text = AppData._CS("StartTime");
            this.label_PTZPointET.Text = AppData._CS("EndTime");
            this.btnPtzOpGetPreset.Text = AppData._CS("Get Preset");
            this.btnPtzOpPresetPoint.Text = AppData._CS("PresetPoint");
            this.btnPtzOpClear.Text = AppData._CS("Clear");
            this.comboBoxPtzOpPresetType.Items[0] = AppData._CS("1-Location");
	        this.comboBoxPtzOpPresetType.Items[1] = AppData._CS("2-Set up");
	        this.comboBoxPtzOpPresetType.Items[2] = AppData._CS("3-Delete");
	        this.comboBoxPtzOpPresetType.Items[3] = AppData._CS("4-Update working time");
            this.groupBox_PTZSit.Text = AppData._CS("PtzSitPosition");
            this.label_PTZSitPX.Text = AppData._CS("PointX");
            this.label_PTZSitPY.Text = AppData._CS("PointY");
            this.label_PTZSitPZ.Text = AppData._CS("PointZ");
            this.label_PTZSitEx.Text = AppData._CS("Extend");
            this.btnPtzOpSitPos.Text = AppData._CS("SitPosition");
            this.groupBox_PTZAlarm.Text = AppData._CS("AlarmActionOut");
            this.label_PTZAlarmOT.Text = AppData._CS("OperateType");
            this.label_PTZAlarmCom.Text = AppData._CS("Command");
            this.btnPtzOpAlarmOut.Text = AppData._CS("AlarmOut");
            this.comboBoxPtzOpAlarmOutType.Items[0] = AppData._CS("1-Status control");
            this.comboBoxPtzOpAlarmOutType.Items[1] = AppData._CS("2-Pattern control");
            this.comboBoxPtzOpAlarmOutCommand.Items[0] = AppData._CS("0-Close");
            this.comboBoxPtzOpAlarmOutCommand.Items[1] = AppData._CS("1-Open/Automatically");
            this.comboBoxPtzOpAlarmOutCommand.Items[2] = AppData._CS("2-Manual");
            this.groupBox_PTZArrange.Text = AppData._CS("PtzArrangePtz");
            this.label_PTZArrOT.Text = AppData._CS("OperateType");
            this.label_PTZArrLT.Text = AppData._CS("LockTime");
            this.label_PTZArrEx.Text = AppData._CS("Extend");
            this.btnPtzOpArrangePtz.Text = AppData._CS("ArrangePtz");
            this.comboBoxPtzOpArrangeType.Items[0] = AppData._CS("0-Unknow");
            this.comboBoxPtzOpArrangeType.Items[1] = AppData._CS("1-Lock current camera");
            this.comboBoxPtzOpArrangeType.Items[2] = AppData._CS("2-Unlock current camera");
            this.comboBoxPtzOpArrangeType.Items[3] = AppData._CS("3-Unlock all camera");
            this.comboBoxPtzOpArrangeType.Items[4] = AppData._CS("4-Lock all camera");
            this.comboBoxPtzOpArrangeType.Items[5] = AppData._CS("5-Query lock status");
        }

        private int GetPicBuffSize(int iPicFormat, int iVideoHeight, int iVideoWidth)
        {
            int iBmpFileHeaderSize = 14;
            int iBmpInfoHeaderSize = 40;

            int uiBuffSize = 0;
            switch (iPicFormat)
            {
                case (int)DPSDK_PIC_FORMAT.DPSDK_PIC_FORMAT_BMP:
                    uiBuffSize = iBmpFileHeaderSize + iBmpInfoHeaderSize + iVideoWidth * iVideoHeight * 4;
                    break;
                case (int)DPSDK_PIC_FORMAT.DPSDK_PIC_FORMAT_JPEG:
                    uiBuffSize = iVideoWidth * iVideoHeight * 3 / 2;
                    break;
                default:
                    break;
            }
            return uiBuffSize;
        }

        private void tabControl1_DrawItem(object sender, DrawItemEventArgs e)
        {
            Graphics g = e.Graphics;
            Font font = new Font("Î¢ÈíÑÅºÚ", 10.0f);
            SolidBrush brush = new SolidBrush(Color.Black);
            RectangleF tRectangleF = this.tabControl1.GetTabRect(e.Index);
            StringFormat sf = new StringFormat();
            sf.LineAlignment = StringAlignment.Center;
            sf.Alignment = StringAlignment.Near;
            g.DrawString(this.tabControl1.TabPages[e.Index].Text, font, brush, tRectangleF, sf);
        }

        private void btnInit_Click(object sender, EventArgs e)
        {
            DPSDK_SetLogInfo("log\\DPSDK_LOG", (IntPtr)DPSDK_LOG_LEVEL_TYPE.LOG_LEVEL_INFO);
            if (DPSDK_Init() == DPSDK_SUCCESS)
            {
                AppData.ShowMsgInfo("Init system success");
            }
            else
            {
                AppData.ShowMsgInfo("Init system failed");
            }
        }

        private void btnUnInit_Click(object sender, EventArgs e)
        {
            if (DPSDK_Uninit() == DPSDK_SUCCESS)
            {
                AppData.ShowMsgInfo("Uninit system success");
            }
            else
            {
                AppData.ShowMsgInfo("Uninit system failed");
            }
        }

        private void CSharpDemo_FormClosed(object sender, FormClosedEventArgs e)
        {
            DPSDK_Uninit();
        }


        

                
    }
}