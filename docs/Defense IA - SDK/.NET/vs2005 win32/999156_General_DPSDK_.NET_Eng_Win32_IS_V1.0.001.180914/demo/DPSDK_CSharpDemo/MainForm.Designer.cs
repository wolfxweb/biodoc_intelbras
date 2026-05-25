using System;
using System.Runtime.InteropServices;
using System.Collections.Generic;
using System.Collections;

using DPSDK_RES = System.Int32;

namespace DPSDK_CSharpDemo
{
    partial class CSharpDemo
    {
        /// <summary>
        /// necessary designer
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// clear all resource
        /// </summary>
        /// <param name="disposing">release resource=true; other=false。</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        private IntPtr m_iSessionID;
        private IntPtr m_iPreviewMediaSessionID;
        private IntPtr m_iPlaybackMediaSessionID;
        private IntPtr m_iDownloadMediaSessionID;
        private int m_iConvertType;
        private Dev_Info_All m_struDevInfoAll;
        private Dep_Info_All m_struDepInfoAll;
        private Dictionary<string, string> m_mapIdName;
        private int m_iAlarmExportSessionID;
        public EventCallBack m_fEventCallbackFun;
        public DPSDK_EVENT_DOWNLOAD_CALLBACK m_fEventDownloadCallback;

        public const int DPSDK_SUCCESS                              = 0;        // Success
        public const int DPSDK_EVENT_SERVER_OFFLINE                 = 1;        // Service offline
        public const int DPSDK_EVENT_RELOGIN_SUCCESS                = 2;		// Service reconnection successfully
        public const int DPSDK_EVENT_ALARM_ALARMEVENT               = 3;        // Alarm event
        public const int DPSDK_EVENT_ALARM_CONFIRMALARM             = 4;		// Alarm confirmation
        public const int DPSDK_EVENT_ALARM_ALARMPICTURE             = 5;		// Alarm linked picture
        public const int DPSDK_EVENT_ALARM_EXPORTALARM              = 6;		// Alarm export
        public const int DPSDK_EVENT_DEVICE_STATUS                  = 7;		// Device status change
        public const int DPSDK_EVENT_CHANNEL_STATUS                 = 8;		// Channel status change
        public const int DPSDK_EVENT_ADD_ORG                        = 9;		// Add organization
        public const int DPSDK_EVENT_MODIFY_ORG                     = 10;		// Modify organization
        public const int DPSDK_EVENT_DELETE_ORG                     = 11;		// Delete organization		The style of data is xml <NodeIDs><NodeID>OrganizationID</NodeID></NodeIDs>
        public const int DPSDK_EVENT_MOVE_ORG                       = 12;		// Move organization
        public const int DPSDK_EVENT_ADD_DEVICE                     = 13;		// Add device
        public const int DPSDK_EVENT_MODIFY_DEVICE                  = 14;		// Modify device
        public const int DPSDK_EVENT_DELETE_DEVICE                  = 15;		// Delete device
        public const int DPSDK_EVENT_MOVE_DEVICE                    = 16;		// Move device
        public const int DPSDK_EVENT_ALERT_USER                     = 17;		// User alert
        public const int DPSDK_EVENT_MEDIA_SCREENSHOT               = 103;		// Call back screenshot. The corresponding structure is MEDIA_DISPLAY
        public const int DPSDK_EVENT_DOWNLOAD_PROGRESS              = 130;      // Download progress
        public const int DPSDK_EVENT_DOWNLOAD_CUT_FILE              = 131;      // Split is finished
        public const int DPSDK_EVENT_DOWNLOAD_FILE_STARTPLAYBACK    = 133;      // Start download by file
        public const int DPSDK_EVENT_DOWNLOAD_TIME_STARTPLAYBACK    = 134;      // Start download by time
        public const int DPSDK_EVENT_VTCALL_INVITE                  = 431;		// Visual intercom call invitation notice
        // Organization, device
	    public const int DPSDK_DATA_ORG_INFO                        = 1;		// Organizational data A detailed view of the structure DPSDK_ORG_INFO
	    public const int DPSDK_DATA_DEVICE_INFO                     = 2;		// Device data A detailed view of the structure DPSDK_DEV_ALL_INFO_LIST
	    public const int DPSDK_DATA_COLLECT_ORG_INFO                = 3;		// Collection tree A detailed view of the structure DPSDK_COLLECTION_ORG_INFO
	    public const int DPSDK_DATA_DEVICE_LAYERED                  = 4;		// Hierarchical acquisition of device tree A detailed view of the structure DPSDK_LAYERED_RESULT_LIST
	    public const int DPSDK_DATA_DEVICE_LIST_BY_ORG              = 5;		// Device data A detailed view of the structure DPSDK_DEV_ALL_INFO_LIST
        public const int DPSDK_DATA_ALL_ORG_INFO                    = 6;		// All organizational data A detailed view of the structure 见DPSDK_ALL_ORG_INFO
        public const string XML_CODING                              = "coding";
        public const string XML_NAME                                = "name";
        public const string XML_SN                                  = "sn";
        public const string DPSDK_DLL                               = "DPSDK.dll";
        public const int DPSDK_FILE_PATH_LEN                        = 1024;      // Length of file path          

        // Media Stream Callback Function
        public delegate IntPtr DPSDK_REALDATA_CALLBACK(IntPtr iMediaType, [MarshalAs(UnmanagedType.LPStr)] string pData, IntPtr iDataLen, IntPtr pUserParam);

        // Fish eye data callback
        public delegate void DPSDK_FISHEYE_CALLBACK(byte uszCorrectMode, UInt16 uRadius, UInt16 uCircleX, UInt16 uCircleY, uint uWidthRatio, uint uHeigthRatio, byte uszGain, byte uszDenoiseLevel, byte uszInstallStyle, IntPtr pUserParam);

        // Video plotting callback
        public delegate void DPSDK_DRAW_CALLBACK(IntPtr hDc, IntPtr pWnd, IntPtr pUserParam);

        // Data callback for the analysis of source data
        public delegate void DPSDK_DEMUXDEC_CALLBACK(IntPtr pUserParam, IntPtr iEncode);

        // Event callbacks
        public delegate void DPSDK_EVENT_CALLBACK(IntPtr iEventType, IntPtr iMediaSessionID, IntPtr pUserParam);

        // Replay the back wall callback
        public delegate IntPtr DPSDK_TVWALL_PLAYBACK_CALLBACK([MarshalAs(UnmanagedType.LPStr)] string pData, IntPtr iDataLen, IntPtr pUserParam);

        // Local Record Event Callback Funciton
        public delegate IntPtr DPSDK_EVENT_LOCALPLAY_CALLBACK(IntPtr iEventType, IntPtr iMediaSessionID, IntPtr pUserParam);

        /**
        * @brief	IVS Data Callback Funciton
        * @param	[IN] pData		Media Stream Data
        * @param	[IN] iDataLen	Data Length
        * @param	[IN] lRealLen	Real Length
        * @param	[IN] pReserved	Reserved Param
        * @param	[IN] pUserData		 User Data
        * @return	Returned value is 0 in case of success
        */
        public delegate IntPtr DPSDK_IVSDATA_CALLBACK([MarshalAs(UnmanagedType.LPStr)] string pData, IntPtr lType, IntPtr lDateLen, IntPtr lRealLen, IntPtr pReserved, IntPtr pUserData);

        // Record Event Callback Function
        public delegate void DPSDK_EVENT_DOWNLOAD_CALLBACK(IntPtr iEventType, IntPtr iMediaSessionID, IntPtr pData, IntPtr pUserParam);

        // Data sync callback. Used for upper level data copy.
        public delegate void DPSDK_DataCallback(IntPtr iDataType, IntPtr pDataBuf, IntPtr uiBufSize, IntPtr pUserData);

        // Event callback definition.
        public delegate void EventCallBack(IntPtr iEventType, IntPtr pEventBuf, IntPtr uiBufSize, IntPtr pUserData);

        // General begin
        [DllImport(DPSDK_DLL, CharSet = CharSet.Ansi)]
        private extern static DPSDK_RES DPSDK_Init();

        [DllImport(DPSDK_DLL, CharSet = CharSet.Ansi)]
        private extern static DPSDK_RES DPSDK_Uninit();

        [DllImport(DPSDK_DLL, CharSet = CharSet.Ansi)]
        private extern static DPSDK_RES DPSDK_SetLogInfo([MarshalAs(UnmanagedType.LPStr)] string szLogPath, IntPtr iLogLevel);

        [DllImport(DPSDK_DLL, CharSet = CharSet.Ansi)]
        private extern static DPSDK_RES DPSDK_Login(ref DPSDK_LOGIN_PARAM pLoginParam, ref IntPtr pSessionID);

        [DllImport(DPSDK_DLL, CharSet = CharSet.Ansi)]
        private extern static DPSDK_RES DPSDK_Logout(IntPtr iSessionID);

        [DllImport(DPSDK_DLL, CharSet = CharSet.Ansi)]
        private extern static DPSDK_RES DPSDK_GetClientVersion(IntPtr iSessionID, ref byte pBuf, uint uiBufLen);

        [DllImport(DPSDK_DLL, CharSet = CharSet.Ansi)]
        private extern static DPSDK_RES DPSDK_GetClientRoad(IntPtr iSessionID, ref byte pBuf, uint uiBufLen);

        [DllImport(DPSDK_DLL, CharSet = CharSet.Ansi)]
        private extern static DPSDK_RES DPSDK_GetVersion();

        [DllImport(DPSDK_DLL, CharSet = CharSet.Ansi)]
        private extern static DPSDK_RES DPSDK_GetEMapInfo(IntPtr iSessionID, ref DPSDK_SERVER_INFO pServerInfo);

        [DllImport(DPSDK_DLL, CharSet = CharSet.Ansi)]
        private extern static DPSDK_RES DPSDK_SyncTime(IntPtr iSessionID);

        [DllImport(DPSDK_DLL, CharSet = CharSet.Ansi)]
        private extern static DPSDK_RES DPSDK_SetHttpsMode(IntPtr iHttpsMode);

        [DllImport(DPSDK_DLL, CharSet = CharSet.Ansi)]
        private extern static DPSDK_RES DPSDK_ModifyUserPwd(IntPtr iSessionID, [MarshalAs(UnmanagedType.LPStr)] string pNewPwd);

        [DllImport(DPSDK_DLL, CharSet = CharSet.Ansi)]
        private extern static DPSDK_RES DPSDK_SetEventCallBack(IntPtr iSessionID, EventCallBack fEventCallBack, IntPtr pUserData);

        [DllImport(DPSDK_DLL, CharSet = CharSet.Ansi)]
        private extern static DPSDK_RES DPSDK_GetFtpInfo(IntPtr iSessionID, uint uiDataType, IntPtr pFtpServerInfoList, uint uiBufLen);

        [DllImport(DPSDK_DLL, CharSet = CharSet.Ansi)]
        private extern static DPSDK_RES DPSDK_GetMenuRight(IntPtr iSessionID, IntPtr pMenuRightList, uint uiBufLen, IntPtr pForbiddenMenuRightList, uint uiForbiddenLen);

        // Real preview begin
        [DllImport(DPSDK_DLL, CharSet = CharSet.Ansi)]
        private extern static DPSDK_RES DPSDK_StartRealPlay(IntPtr iSessionID, ref DPSDK_REALPLAY_PARAM pRealPlayParam, ref IntPtr pMediaSessionID);

        [DllImport(DPSDK_DLL, CharSet = CharSet.Ansi)]
        private extern static DPSDK_RES DPSDK_StopRealPlay(IntPtr iSessionID, IntPtr pMediaSessionID);

        [DllImport(DPSDK_DLL, CharSet = CharSet.Ansi)]
        private extern static DPSDK_RES DPSDK_GetPlayStreamMode(IntPtr iSessionID, IntPtr pMediaSessionID, ref IntPtr pStreamMode);

        [DllImport(DPSDK_DLL, CharSet = CharSet.Ansi)]
        private extern static DPSDK_RES DPSDK_SetPlayStreamMode(IntPtr iSessionID, IntPtr pMediaSessionID, uint uiStreamMode, uint uiDelayTime);

        [DllImport(DPSDK_DLL, CharSet = CharSet.Ansi)]
        private extern static DPSDK_RES DPSDK_SetDisplayRegion(IntPtr iSessionID, IntPtr pMediaSessionID, ref DPSDK_RECT pRECT, IntPtr hDestWnd, bool bEnable);

        [DllImport(DPSDK_DLL, CharSet = CharSet.Ansi)]
        private extern static DPSDK_RES DPSDK_OperateRealPlay(IntPtr iSessionID, IntPtr pMediaSessionID, [MarshalAs(UnmanagedType.LPStr)] string pCodeID, int iOperateType);

        [DllImport(DPSDK_DLL, CharSet = CharSet.Ansi)]
        private extern static DPSDK_RES DPSDK_GetPictureSize(IntPtr iSessionID, IntPtr pMediaSessionID, ref IntPtr pWidth, ref IntPtr pHeight);

        [DllImport(DPSDK_DLL, CharSet = CharSet.Ansi)]
        private extern static DPSDK_RES DPSDK_Get24BitPictureFile(IntPtr iSessionID, IntPtr pMediaSessionID, uint uiPicFormat, [MarshalAs(UnmanagedType.LPStr)] string pPath);

        [DllImport(DPSDK_DLL, CharSet = CharSet.Ansi)]
        private extern static DPSDK_RES DPSDK_GetPictureBuf(IntPtr iSessionID, IntPtr pMediaSessionID, ref byte pPicBuf, IntPtr iBufsize, ref IntPtr pPicSize, uint uiPicFormat);

        [DllImport(DPSDK_DLL, CharSet = CharSet.Ansi)]
        private extern static DPSDK_RES DPSDK_OpenSound(IntPtr iSessionID, IntPtr pMediaSessionID);

        [DllImport(DPSDK_DLL, CharSet = CharSet.Ansi)]
        private extern static DPSDK_RES DPSDK_CloseSound(IntPtr iSessionID, IntPtr pMediaSessionID);

        [DllImport(DPSDK_DLL, CharSet = CharSet.Ansi)]
        private extern static DPSDK_RES DPSDK_OpenSoundShare(IntPtr iSessionID, IntPtr pMediaSessionID);

        [DllImport(DPSDK_DLL, CharSet = CharSet.Ansi)]
        private extern static DPSDK_RES DPSDK_CloseSoundShare(IntPtr iSessionID, IntPtr pMediaSessionID);

        [DllImport(DPSDK_DLL, CharSet = CharSet.Ansi)]
        private extern static DPSDK_RES DPSDK_IsOpenSoundState(IntPtr iSessionID, IntPtr pMediaSessionID, ref IntPtr pIsOpenSound);

        [DllImport(DPSDK_DLL, CharSet = CharSet.Ansi)]
        private extern static DPSDK_RES DPSDK_GetVolume(IntPtr iSessionID, IntPtr pMediaSessionID, ref IntPtr pVolume);

        [DllImport(DPSDK_DLL, CharSet = CharSet.Ansi)]
        private extern static DPSDK_RES DPSDK_SetVolume(IntPtr iSessionID, IntPtr pMediaSessionID, uint uiVolume);

        [DllImport(DPSDK_DLL, CharSet = CharSet.Ansi)]
        private extern static DPSDK_RES DPSDK_StartRecord(IntPtr iSessionID, IntPtr pMediaSessionID, [MarshalAs(UnmanagedType.LPStr)] string pFile, uint uiSplitRecordLen);

        [DllImport(DPSDK_DLL, CharSet = CharSet.Ansi)]
        private extern static DPSDK_RES DPSDK_StopRecord(IntPtr iSessionID, IntPtr pMediaSessionID, IntPtr pRecordFile, uint uiBufLen);

        [DllImport(DPSDK_DLL, CharSet = CharSet.Ansi)]
        private extern static DPSDK_RES DPSDK_IsRecordState(IntPtr iSessionID, IntPtr pMediaSessionID, ref IntPtr pIsRecord);

        [DllImport(DPSDK_DLL, CharSet = CharSet.Ansi)]
        private extern static DPSDK_RES DPSDK_SetSplitRecordLen(IntPtr iSessionID, IntPtr pMediaSessionID, uint uiSplitRecordLen);

        // PTZ function begin
        [DllImport(DPSDK_DLL, CharSet = CharSet.Ansi)]
        private extern static DPSDK_RES DPSDK_PtzOperateFunction(IntPtr iSessionID, ref DPSDK_PTZOPERATE_FUNCTION_PARAM pPtzOperateFunctionParam, ref DPSDK_PTZOPERATE_RESULT pPtzOperateFunctionResult);

        [DllImport(DPSDK_DLL, CharSet = CharSet.Ansi)]
        private extern static DPSDK_RES DPSDK_PtzOperateCamera(IntPtr iSessionID, ref DPSDK_PTZOPERATE_CAMERA_PARAM pPtzOperateCamereParam, ref DPSDK_PTZOPERATE_RESULT pPtzOperateResult);

        [DllImport(DPSDK_DLL, CharSet = CharSet.Ansi)]
        private extern static DPSDK_RES DPSDK_PtzOperateDirect(IntPtr iSessionID, ref DPSDK_PTZOPERATE_DIRECT_PARAM pPtzOperateDirectParam, ref DPSDK_PTZOPERATE_RESULT pPtzOperateResult);

        [DllImport(DPSDK_DLL, CharSet = CharSet.Ansi)]
        private extern static DPSDK_RES DPSDK_PtzOperateFocus(IntPtr iSessionID, ref DPSDK_PTZOPERATE_FOCUS_PARAM pPtzOperateFocusParam, ref DPSDK_PTZOPERATE_RESULT pPtzOperateResult);

        [DllImport(DPSDK_DLL, CharSet = CharSet.Ansi)]
        private extern static DPSDK_RES DPSDK_PtzOperatePresetPoint(IntPtr iSessionID, ref DPSDK_PTZOPERATE_PRESETPOINT_PARAM pPtzOperatePrePointParam, ref DPSDK_PTZOPERATE_RESULT pPtzOperateResult);

        [DllImport(DPSDK_DLL, CharSet = CharSet.Ansi)]
        private extern static DPSDK_RES DPSDK_PtzGetPresetPoints(IntPtr iSessionID, [MarshalAs(UnmanagedType.LPStr)] string pChannelId, IntPtr pPresetPointList, uint uiBufLen);

        [DllImport(DPSDK_DLL, CharSet = CharSet.Ansi)]
        private extern static DPSDK_RES DPSDK_PtzSitPosition(IntPtr iSessionID, ref DPSDK_PTZOPERATE_SITPOSITION_PARAM pPtzOperateSitPositionParam, ref DPSDK_PTZOPERATE_RESULT pPtzOperateResult);

        [DllImport(DPSDK_DLL, CharSet = CharSet.Ansi)]
        private extern static DPSDK_RES DPSDK_PtzArrangePtz(IntPtr iSessionID, ref DPSDK_PTZOPERATE_ARRANGEPTZ_PARAM pPtzOperateArrangePtzParam, ref DPSDK_PTZOPERATE_RESULT pPtzOperateResult);

        [DllImport(DPSDK_DLL, CharSet = CharSet.Ansi)]
        private extern static DPSDK_RES DPSDK_AlarmActionOut(IntPtr iSessionID, ref DPSDK_PTZOPERATE_ALARMOUT_PARAM pAlarmOutParam, ref DPSDK_PTZOPERATE_RESULT pPtzOperateResult);

        // Alarm begin
        [DllImport(DPSDK_DLL, CharSet = CharSet.Ansi)]
        private extern static DPSDK_RES DPSDK_ConfirmAlarm(IntPtr iSessionID, IntPtr pConfirmAlarmParam);

        [DllImport(DPSDK_DLL, CharSet = CharSet.Ansi)]
        private extern static DPSDK_RES DPSDK_QueryAlarm(IntPtr iSessionID, ref DPSDK_QUERYALARM_PARAM pQueryAlarmParam, uint uiBufLen, IntPtr pAlarmDetailInfoList);

        [DllImport(DPSDK_DLL, CharSet = CharSet.Ansi)]
        private extern static DPSDK_RES DPSDK_GetAlarmTypeGroupInfo(IntPtr iSessionID, [MarshalAs(UnmanagedType.LPStr)] string pLanguage, ref IntPtr pInfoXml);

        [DllImport(DPSDK_DLL, CharSet = CharSet.Ansi)]
        private extern static DPSDK_RES DPSDK_ReleaseDataBuffer(IntPtr pBuffer);

        [DllImport(DPSDK_DLL, CharSet = CharSet.Ansi)]
        private extern static DPSDK_RES DPSDK_QueryAlarmCount(IntPtr iSessionID, ref DPSDK_QUERYALARMCOUNT_PARAM pQueryAlarmCountParam, ref IntPtr pAlarmCount);

        [DllImport(DPSDK_DLL, CharSet = CharSet.Ansi)]
        private extern static DPSDK_RES DPSDK_QueryAlarmProcessFlow(IntPtr iSessionID, [MarshalAs(UnmanagedType.LPStr)] string pAlarmCode, uint uiBufLen, IntPtr pAlarmProcessInfoList);

        [DllImport(DPSDK_DLL, CharSet = CharSet.Ansi)]
        private extern static DPSDK_RES DPSDK_BlockAlarm(IntPtr iSessionID, ref DPSDK_BLOCKALARM_PARAM pBlockAlarmParam);

        [DllImport(DPSDK_DLL, CharSet = CharSet.Ansi)]
        private extern static DPSDK_RES DPSDK_ExportAlarms(IntPtr iSessionID, ref DPSDK_ALARMEXPORT_PARAM pAlarmExportParam, int iSessionId);

        // Playback begin
        [DllImport(DPSDK_DLL, CharSet = CharSet.Ansi)]
        private extern static DPSDK_RES DPSDK_QueryRecord(IntPtr iSessionID, ref DPSDK_QUERY_RECORD_PARAM pQueryRecord, IntPtr pRecordList, uint uiBufLen);

        [DllImport(DPSDK_DLL, CharSet = CharSet.Ansi)]
        private extern static DPSDK_RES DPSDK_QueryRecordDate(IntPtr iSessionID, ref DPSDK_QUERY_RECORD_DATE_PARAM pQueryDateInfo, ref DPSDK_RECORD_DATE_INFO pRecordDate);

        [DllImport(DPSDK_DLL, CharSet = CharSet.Ansi)]
        private extern static DPSDK_RES DPSDK_GetRecordStatus(IntPtr iSessionID, [MarshalAs(UnmanagedType.LPStr)] string pChannelID, ref DPSDK_RECORD_STATUS_INFO pRecordInfo);

        [DllImport(DPSDK_DLL, CharSet = CharSet.Ansi)]
        private extern static DPSDK_RES DPSDK_LockRecordFile(IntPtr iSessionID, ref DPSDK_LOCK_RECORD_FILE_PARAM pLockFileInfo, ref DPSDK_LOCK_RECORD_FILE_RESULT pResult);

        [DllImport(DPSDK_DLL, CharSet = CharSet.Ansi)]
        private extern static DPSDK_RES DPSDK_UnlockRecordFile(IntPtr iSessionID, ref DPSDK_UNLOCK_RECORD_FILE_PARAM pUnlockFileInfo, ref DPSDK_LOCK_RECORD_FILE_RESULT pResult);

        [DllImport(DPSDK_DLL, CharSet = CharSet.Ansi)]
        private extern static DPSDK_RES DPSDK_QueryLockRecord(IntPtr iSessionID, ref DPSDK_QUERY_LOCK_RECORD_PARAM pQueryLockRecord, IntPtr pLockRecordList, uint uiBufLen);

        [DllImport(DPSDK_DLL, CharSet = CharSet.Ansi)]
        private extern static DPSDK_RES DPSDK_StartRemoteRecord(IntPtr iSessionID, ref DPSDK_PTZOPERATE_STARTREMOTERECORD_PARAM pStartRemoteRecordParam, ref DPSDK_PTZOPERATE_REMOTERECORD_RESULT pStartRemoteRecordResult);

        [DllImport(DPSDK_DLL, CharSet = CharSet.Ansi)]
        private extern static DPSDK_RES DPSDK_StopRemoteRecord(IntPtr iSessionID, ref DPSDK_PTZOPERATE_STOPREMOTERECORD_PARAM pStopRemoteRecordParam, ref DPSDK_PTZOPERATE_REMOTERECORD_RESULT pStopRemoteRecordResult);

        [DllImport(DPSDK_DLL, CharSet = CharSet.Ansi)]
        private extern static DPSDK_RES DPSDK_StopPlayback(IntPtr iSessionID, IntPtr pMediaSessionID);

        [DllImport(DPSDK_DLL, CharSet = CharSet.Ansi)]
        private extern static DPSDK_RES DPSDK_StartPlaybackByTime(IntPtr iSessionID, ref DPSDK_PLAYBACK_BY_TIME_PARAM pPlaybackParam, ref IntPtr pMediaSessionID);

        [DllImport(DPSDK_DLL, CharSet = CharSet.Ansi)]
        private extern static DPSDK_RES DPSDK_StartPlaybackByFile(IntPtr iSessionID, ref DPSDK_PLAYBACK_BY_FILE_PARAM pPlaybackParam, ref IntPtr pMediaSessionID);

        [DllImport(DPSDK_DLL, CharSet = CharSet.Ansi)]
        private extern static DPSDK_RES DPSDK_PlaybackPause(IntPtr iSessionID, IntPtr pMediaSessionID);

        [DllImport(DPSDK_DLL, CharSet = CharSet.Ansi)]
        private extern static DPSDK_RES DPSDK_PlaybackResume(IntPtr iSessionID, IntPtr pMediaSessionID);

        [DllImport(DPSDK_DLL, CharSet = CharSet.Ansi)]
        private extern static DPSDK_RES DPSDK_PlaybackFrameStep(IntPtr iSessionID, IntPtr pMediaSessionID);

        [DllImport(DPSDK_DLL, CharSet = CharSet.Ansi)]
        private extern static DPSDK_RES DPSDK_PlaybackSeek(IntPtr iSessionID, IntPtr pMediaSessionID, ref DPSDK_PLAYBACK_SEEK_PARAM pPlaybackSeekParam);

        [DllImport(DPSDK_DLL, CharSet = CharSet.Ansi)]
        private extern static DPSDK_RES DPSDK_GetPlayedTime(IntPtr iSessionID, IntPtr pMediaSessionID, ref IntPtr pTime);

        [DllImport(DPSDK_DLL, CharSet = CharSet.Ansi)]
        private extern static DPSDK_RES DPSDK_GetProviderType(IntPtr iSessionID, IntPtr pMediaSessionID, ref IntPtr pProviderType);

        [DllImport(DPSDK_DLL, CharSet = CharSet.Ansi)]
        private extern static DPSDK_RES DPSDK_SetPlaybackSpeed(IntPtr iSessionID, IntPtr pMediaSessionID, DPSDK_PLAYBACK_SPEED iSpeed);

        [DllImport(DPSDK_DLL, CharSet = CharSet.Ansi)]
        private extern static DPSDK_RES DPSDK_StartDownloadRecordByTime(IntPtr iSessionID, ref DPSDK_DOWNLOAD_BY_TIME_PARAM pDownloadByTimeParam, ref IntPtr pMediaSessionID);

        [DllImport(DPSDK_DLL, CharSet = CharSet.Ansi)]
        private extern static DPSDK_RES DPSDK_StartDownloadRecordByFile(IntPtr iSessionID, ref DPSDK_DOWNLOAD_BY_FILE_PARAM pDownloadByFileParam, ref IntPtr pMediaSessionID);

        [DllImport(DPSDK_DLL, CharSet = CharSet.Ansi)]
        private extern static DPSDK_RES DPSDK_StopDownloadRecord(IntPtr iSessionID, IntPtr pMediaSessionID);

        [DllImport(DPSDK_DLL, CharSet = CharSet.Ansi)]
        private extern static DPSDK_RES DPSDK_PauseDownloadRecord(IntPtr iSessionID, IntPtr pMediaSessionID);

        [DllImport(DPSDK_DLL, CharSet = CharSet.Ansi)]
        private extern static DPSDK_RES DPSDK_ResumeDownloadRecord(IntPtr iSessionID, IntPtr pMediaSessionID);

        [DllImport(DPSDK_DLL, CharSet = CharSet.Ansi)]
        private extern static DPSDK_RES DPSDK_GetDownloadRecordInfo(IntPtr iSessionID, IntPtr pMediaSessionID, IntPtr pDownloadInfo, uint uiBufLen);

        [DllImport(DPSDK_DLL, CharSet = CharSet.Ansi)]
        private extern static DPSDK_RES DPSDK_ConvertToBmpFile(IntPtr iSessionID, IntPtr pMediaSessionID, ref DPSDK_CONVERT_BMP pConvertBMP);

        [DllImport(DPSDK_DLL, CharSet = CharSet.Ansi)]
        private extern static DPSDK_RES DPSDK_ConvertToJpegFile(IntPtr iSessionID, IntPtr pMediaSessionID, ref DPSDK_CONVERT_JPEG pConvertJPEG);

        //Organization begin
        [DllImport(DPSDK_DLL, CharSet = CharSet.Ansi)]
        private extern static DPSDK_RES DPSDK_GetDeviceByLayered(IntPtr iSessionID, ref DPSDK_GET_DEVICE_LAYERED_PARAM pParam, ref DPSDK_PAGE_INFO pPageInfo, ref IntPtr pTotal, DPSDK_DataCallback fDataCallBack, IntPtr pUserData);

        [DllImport(DPSDK_DLL, CharSet = CharSet.Ansi)]
        private extern static DPSDK_RES DPSDK_GetOrganization(IntPtr iSessionID, IntPtr pParam, uint uiLen, DPSDK_DataCallback fDataCallBack, IntPtr pUserData);

        [DllImport(DPSDK_DLL, CharSet = CharSet.Ansi)]
        private extern static DPSDK_RES DPSDK_SaveUserData(IntPtr iSessionID, [MarshalAs(UnmanagedType.LPStr)] string pUserDataName, [MarshalAs(UnmanagedType.LPStr)] string pUserData, uint uiDataLen);

        [DllImport(DPSDK_DLL, CharSet = CharSet.Ansi)]
        private extern static DPSDK_RES DPSDK_GetUserData(IntPtr iSessionID, [MarshalAs(UnmanagedType.LPStr)] string pUserDataName, ref IntPtr pUserData);

        [DllImport(DPSDK_DLL, CharSet = CharSet.Ansi)]
        private extern static DPSDK_RES DPSDK_DelUserData(IntPtr iSessionID, [MarshalAs(UnmanagedType.LPStr)] string pUserDataName);

        [DllImport(DPSDK_DLL, CharSet = CharSet.Ansi)]
        private extern static DPSDK_RES DPSDK_SetCompressType(IntPtr iCompressType);

        [DllImport(DPSDK_DLL, CharSet = CharSet.Ansi)]
        private extern static DPSDK_RES DPSDK_GetDevice(IntPtr iSessionID, IntPtr pQueryDevInfo, uint uiQueryLen, DPSDK_DataCallback fDataCallBack, IntPtr pUserData);

        // Log rank The higher the level is, the less the content of the output is
        public enum DPSDK_LOG_LEVEL_TYPE
        {
            LOG_LEVEL_DEBUG                 = 2,		        // debugging Do not print normally for debugging and use
            LOG_LEVEL_INFO                  = 4,		        // information
            LOG_LEVEL_WARN                  = 5,		        // notice
            LOG_LEVEL_ERR                   = 6,		        // error
        }

        // Https model
        public enum DPSDK_HTTPS_MODE
        {
	        DPSDK_HTTPS_UNENABLE	        = 0,				// Not enable https
	        DPSDK_HTTPS_ONEWAY_AUTH         = 1,				// Enable https one-way authentication
	        DPSDK_HTTPS_TWOWAY_AUTH         = 2					// Enable https two-way authentication
        }

        public enum DPSDK_FTP_DATA_TYPE
        {
	        FTP_UNKNOW			            = 0,	            // Unknown
	        FTP_ALARM			            = 1,	            // Alarm
	        FTP_TAG				            = 2,	            // Tag
	        FTP_DOOR			            = 3	                // Acess
        }

        // Decoding type
        public enum DPSDK_DECODE_TYPE
        {
            DPSDK_DECODE_SW                 = 0,				// CPU decoding
            DPSDK_DECODE_HW                 = 1,				// GPU decoding
            DPSDK_DECODE_HW_FAST            = 2					// GPU decoding
        }

        public enum DPSDK_VIDEO_LOCK_TYPE
        {
            DPSDK_VIDEO_CMD_LOCK            = 0,				// Lock the current camera
            DPSDK_VIDEO_CMD_UNLOCK_ONE      = 1				    // Unlock the current camera
        }

        // Capture image format
        public enum DPSDK_PIC_FORMAT
        {
	        DPSDK_PIC_FORMAT_BMP            = 0,			    // BMP type
	        DPSDK_PIC_FORMAT_JPEG           = 1,			    // JPEG type
	        DPSDK_PIC_FORMAT_BMP24	        = 6			        // BMP24 type
        }

        // Operating parameters of cloud platform
        public enum PtzOperateFunction_e
	    {
		    PtzOF_Show_PtzMenu				= 0,		        // Display "Cloud platform menu"
		    PtzOF_Move_PtzMenu				= 1,		        // Control "Menu direction of the cloud platform"
		    PtzOF_Confirm_PtzMenuItem		= 2,		        // Determine "Cloud platform menu item"
		    PtzOF_Set_LineScannBorder		= 3,		        // Set up "Line scavenging boundary"
		    PtzOF_Switch_LineScanBorder		= 4,		        // Switch "Line scan"
		    PtzOF_Switch_AutoRotate			= 5,		        // Switch "Horizontal rotation"
		    PtzOF_Switch_Light				= 6,		        // Switch "lighting"
		    PtzOF_Switch_RainBrush			= 7,		        // Switch "Wiper"
		    PtzOF_Switch_InfraredLight		= 8,		        // Switch "infrared light"
		    PtzOF_Switch_AssisentPoint		= 9,		        // Switch "Auxiliary point"
		    PtzOF_Switch_Cruise				= 10,		        // Switch "Cruise function"
		    PtzOF_Switch_Track				= 11,		        // Switch "Cruising"
		    PtzOF_Switch_SetTrack			= 12		        // Switch "Track setting"
	    }

        // Video source
        public enum DPSDK_SOURCE_TYPE
	    {
		    DPSDK_SOURCE_TYPE_ALL           = 1,                //All video, including platform video and device video
	        DPSDK_SOURCE_TYPE_DEVICE        = 2,                //Device video
	        DPSDK_SOURCE_TYPE_CENTER        = 3,				//Platform video
            DPSDK_SOURCE_TYPE_3RD_CLOUD     = 4,				//3rd platform video
	    }

        // Stream type
        public enum DPSDK_STREAM_TYPE
        {
	        STREAM_UNKNOW_STREAM            = 0,                // Unknown
	        STREAM_MAIN_STREAM              = 1,                // Main stream
	        STREAM_SUB_STREAM               = 2,                // Auxiliary code stream
	        STREAM_THIRD_STREAM             = 3,                // Three bit stream
	        STREAM_LOCAL_SIGNAL_STREAM      = 5	                // Local signal
        }

        // Record type
        public enum DPSDK_RECORD_TYPE
        {
	        DPSDK_RECORD_TYPE_ALL           = 0,
	        DPSDK_RECORD_TYPE_MANUAL        = 1,                // Manual record
	        DPSDK_RECORD_TYPE_ALARM         = 2,                // Alarm record
	        DPSDK_RECORD_TYPE_MOTION_DETECT = 3,                // Dynamic detection
	        DPSDK_RECORD_TYPE_VIDEO_LOST    = 4,				// Video loss
	        DPSDK_RECORD_TYPE_VIDEO_SHELTER = 5,                // Video occlusion
	        DPSDK_RECORD_TYPE_TIMER         = 6,                // Timing video
	        DPSDK_RECORD_TYPE_ALLDAY        = 7,                // All-weather video
	        DPSDK_RECORD_TYPE_FILE_RECORD   = 8,				// File video conversion
	        DPSDK_RECORD_TYPE_NORMAL        = 9,                // Ordinary video

	        DPSDK_RECORD_TYPE_CARD          = 25,               // Card number video There is no this in the protocol library for the time being
	        DPSDK_RECORD_TYPE_ALARM_BEGIN   = 10,				// Alarm start Definition in the match protocol stack 10~300 -m -f -cSpecial alarm
	        DPSDK_RECORD_TYPE_ALARM_END     = 1000				// End of intelligent alarm Definition in the match protocol stack 300~1000Intelligent alarm
        }

        // Playback speed
        public enum DPSDK_PLAYBACK_SPEED
        {
	        DPSDK_PB_NORMAL			        = 1024,
	        DPSDK_PB_NORMAL_FAST2	        = DPSDK_PB_NORMAL * 2,
	        DPSDK_PB_NORMAL_FAST4	        = DPSDK_PB_NORMAL * 4,
	        DPSDK_PB_NORMAL_FAST8	        = DPSDK_PB_NORMAL * 8,
	        DPSDK_PB_NORMAL_FAST16	        = DPSDK_PB_NORMAL * 16,
	        DPSDK_PB_NORMAL_SLOW2	        = DPSDK_PB_NORMAL / 2,
	        DPSDK_PB_NORMAL_SLOW4	        = DPSDK_PB_NORMAL / 4,
	        DPSDK_PB_NORMAL_SLOW8	        = DPSDK_PB_NORMAL / 8,
	        DPSDK_PB_NORMAL_SLOW16	        = DPSDK_PB_NORMAL / 16
        }

        public enum DPSDK_RECORD_FILE_NAME_RULE
        {
	        DPSDK_NAME_RULE_TIME_CHANNELID      = 0,
	        DPSDK_NAME_RULE_TIME_CHANNELNAME    = 1,
	        DPSDK_NAME_RULE_CHANNELID_TIME      = 2,
	        DPSDK_NAME_RULE_CHANNELNAME_TIME    = 3
        }

        public enum DPSDK_DOWNLOAD_RECORD_FILE_FORMAT
        {
	        DPSDK_FILE_FORMAT_NORMAL        = 0,			    // Original stream
	        DPSDK_FILE_FORMAT_AVI           = 1,				// avi format
	        DPSDK_FILE_FORMAT_MP4           = 2,				// mp4 format
	        DPSDK_FILE_FORMAT_FLV           = 3,				// flv format
	        DPSDK_FILE_FORMAT_ASF           = 4				    // asf format
        }

        // Compression method
        public enum DPSDK_COMPRESS_TYPE
        {
	        COMPRESS_DISABLE			    = 0,				// No use of compression
	        COMPRESS_DEFAULT			    = 1				    // Using the default compression method
        }

        // Login information
        [StructLayout(LayoutKind.Sequential)]
        public struct DPSDK_LOGIN_PARAM
        {
            public byte                 bDomainUser;			// Whether or not domain login
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 128)]
            public string               szUserName;             // User name
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 128)]
            public string               szPWD;                  // Cleartext password, the login type is0(basic account), can not be empty
            public DPSDK_IP             struIP;                 // Login server IP
            public uint                 uiPort;                 // Login server port
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 128)]
            public string               szMACAddress;           // AC address
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 128)]
            public string               szIMEI;                 // Check code for mobile client landing platform
            public uint                 uiClientType;           // Client type: Reference DPSDK_CLIENT_TYPE
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 32)]
            public string               szReserve;              // Reserved field
            public DPSDK_IP             struClientIP;           // Client IP
        }

        [StructLayout(LayoutKind.Sequential)]
        public struct DPSDK_IP
        {
            public int                  uiIPType;               // IP type, refer to DPSDK_IP_TYPE
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 64)]
            public string               szIP;                   // IP address
        }

        [StructLayout(LayoutKind.Sequential)]
        public struct DPSDK_SERVER_INFO
        {
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 64)]
            public string               szIP;                   // server IP
            public uint                 uiPort;                 // server port
        }

        // FTP server list information
        [StructLayout(LayoutKind.Sequential)]
        public struct DPSDK_FTP_SERVER_INFO_LIST
        {
            public uint			        uiTotal;				// The total number of result
	        public IntPtr	            struFtpServerInfo;		// FTP server information
        }

        // FTP server information
        [StructLayout(LayoutKind.Sequential)]
        public struct DPSDK_FTP_SERVER_INFO
        {
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 256)]
            public string               szUrl;                  // FTP absolute path ftps://192.168.1.1:21
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 128)]
            public string               szUserName;             // User name
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 128)]
            public string               szPWD;                  // Password
            public uint                 uiDataType;             // Data type Refer: DPSDK_FTP_DATA_TYPE
        }

        // The list of menu right
        [StructLayout(LayoutKind.Sequential)]
        public struct DPSDK_MENU_RIGHT_LIST
        {
	        public uint                 uiTotal;				// The total number of result
	        public IntPtr               struMenuRight;			// The list of menu right
        }

        // Menu information
        [StructLayout(LayoutKind.Sequential)]
        public struct DPSDK_MENU_RIGHT
        {
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 64)]
            public string               szMenuRight;            // Menu right information
        }

        // Device status change notification
        [StructLayout(LayoutKind.Sequential)]
        public struct DPSDK_DEV_STATUS_NOTIFY
        {
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 56)]
            public string               szDeviceID;             // Device ID
	        public int                  iStatus;				// Device status see DPSDK_DEV_STATUS
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 256)]
            public string               szOfflineReason;        // Off-line reason
        }

        // Channel state change notification
        [StructLayout(LayoutKind.Sequential)]
        public struct DPSDK_CHANNEL_STATUS_NOTIFY
        {
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 64)]
            public string               szChannelID;            // ChannelID
	        public int                  iStatus;				// Channel status see DPSDK_DEV_STATUS
        }

        // Organization of basic data
        [StructLayout(LayoutKind.Sequential)]
        public struct DPSDK_ORG_BASE_INFO
        {
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 96)]
            public string               szOrgCode;              // organization Code
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 256)]
            public string               szOrgName;              // Organization name
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 56)]
            public string               szOrgSN;                // organization SN code
	        public int	                iOrgType;				// Organization node type
	        public int	                iOrgSort;				// Organization sort
	        public Int64                tModifyTime;			// Modify time

	        //
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 96)]
            public string               szParentCode;           // Organization parent node ID

	        //Cloud SDK parameter
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 256)]
            public string               szParentName;           // Organization parent node name
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 96)]
            public string               szPlatformCode;         // GB ID
	        public int	                iGroupNumber;			// Number of department under this node
	        public int	                iDevivceNumber;			// Number of device under this node
	        public int                  iChannelNum;			// Number of channel under this node
        }

        // Mobile organization notification
        [StructLayout(LayoutKind.Sequential)]
        public struct DPSDK_MOVE_ORG_NOTIFY
        {
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 96)]
            public string               szOldOrgCode;           // Old organization node Code
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 96)]
            public string               szOldParentOrgCode;     // Old organization father node Code
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 96)]
            public string               szNewOrgCode;           // New organization nodeCode
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 96)]
            public string               szNewParentOrgCode;     // New organization father node Code
        }

        // Increase the device notification
        [StructLayout(LayoutKind.Sequential)]
        public struct DPSDK_ADD_DEVICE_NOTIFY
        {
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 96)]
            public string               szOrgCode;              // Organization code
	        public DPSDK_DEV_ALL_INFO   struDevAllInfo;			// Device data
        }

        // Modification of device notification
        [StructLayout(LayoutKind.Sequential)]
        public struct DPSDK_MODIFY_DEVICE_NOTIFY
        {
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 96)]
            public string               szOldOrgCode;           // Old organization
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 96)]
            public string               szNewOrgCode;           // New organization
	        public DPSDK_DEV_ALL_INFO   struDevAllInfo;			// Device data
        }

        // Delete device notification Support batch
        [StructLayout(LayoutKind.Sequential)]
        public struct DPSDK_DELETE_DEVICE_NOTIFY
        {
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 96)]
            public string               szOrgCode;              // organization code
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 56)]
            public string               szDeviceID;             // Device ID
        }

        // Mobile device notification Support batch
        [StructLayout(LayoutKind.Sequential)]
        public struct DPSDK_MOVE_DEVICE_NOTIFY
        {
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 56)]
            public string               szDeviceID;             // Device ID
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 96)]
            public string               szOldOrgCode;           // The original organization of the equipment code
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 96)]
            public string               szNewOrgCode;           // New organization of equipment code
        }

        // Unicast video parameters
        [StructLayout(LayoutKind.Sequential)]
        public struct DPSDK_REALPLAY_PARAM
        {
            public DPSDK_MEDIA_BASE_PARAM struMediaBaseParam;   // Basic video parameters
            public DPSDK_MEDIA_CALLBACK struMediaCallBack;      // Video callback structure

            //Transcoding parameter
            public int		            iUsedVcs;               // Whether the tag needs to pass throughVCSTranscoding.0It means that there is no need for transcoding;1It means that transcoding is required
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 20)]
	        public string		        szVideoCode;	        // Video coding format, reference video coding format to define strings
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 20)]
	        public string		        szResolution;           // Code stream resolution, reference stream resolution definition string
            public int                  iFps;                   // Frame rate
            public int                  iBps;                   // Bit stream code stream
        }

        // Basic video parameters
        [StructLayout(LayoutKind.Sequential)]
        public struct DPSDK_MEDIA_BASE_PARAM
        {
            public IntPtr               pHWnd;                  // Window handle
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 56)]
            public string               szCodeId;               // Channel ID Or equipment ID
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 56)]
            public string               szDeviceCode;           // Device code for request media key
            public int                  iStreamType;            // Code stream type 1=Main stream, 2=Auxiliary code stream
            public int                  iDataType;              // Video type:1=video, 2=audio frequency, 3=Audio and video
            public int                  iDecodeType;            // Decode type See DPSDK_DECODE_TYPE Definition
            public int                  iStreamMode;            // Playback mode See DPSDK_STREAM_MODE Definition
            public uint                 uiDelayTime;            // Play delay time, when IstreamMode is  DPSDK_STREAM_CUSTOM_MODETime, it is  effective Company MS
        }

        // DPSDK_MEDIA_CALLBACK
        [StructLayout(LayoutKind.Sequential)]
        public struct DPSDK_MEDIA_CALLBACK
        {
            public DPSDK_REALDATA_CALLBACK fRealDataCallBack;   // Bitstream callback
            public IntPtr               pRealUserData;          // Code stream callback user data

            public DPSDK_FISHEYE_CALLBACK fFishEyeCallBack;     // Fish eye data callback
            public IntPtr               pFishEyeUserData;       // Fish eye data callback user data

            public DPSDK_DRAW_CALLBACK  fDrawCallBack;          // Video plotting callback
            public IntPtr               pDrawUserData;          // Video plotting callback user data

            public DPSDK_DEMUXDEC_CALLBACK fDemuxDecCallBack;   // Data callback for the analysis of source data
            public IntPtr               pDemuxDecUserData;      // Data callback to user data analyzed by source data

            public DPSDK_EVENT_CALLBACK fEventCallBack;         // Event callbacks
            public IntPtr               pEventUserData;         // Event callback user data

            public DPSDK_TVWALL_PLAYBACK_CALLBACK fTVWallPlaybackCallBack; //Replay the back wall callback
            public IntPtr               pTVWallPlaybackUserData;// Playback the upper wall callback user data

            public DPSDK_EVENT_LOCALPLAY_CALLBACK fEventLocalCallBack;// Local Record Event Callback
            public IntPtr               pEventLocalUserData;    // Local Record Event Callback user data

            public DPSDK_IVSDATA_CALLBACK fIVSDataCallBack;
            public IntPtr               pIVSUserData;		    // Local Record Event Callback user data
        }

        [StructLayout(LayoutKind.Sequential)]
        public struct DPSDK_RECT
        {
            public Int64                left;
            public Int64                top;
            public Int64                right;
            public Int64                bottom;
        }

        // Video file list
        [StructLayout(LayoutKind.Sequential)]
        public struct DPSDK_FILE_STORE_LIST
        {
            public UInt64               uiTotal;                // Total number of video files
            public IntPtr               pFileList;              // Video file list
        }

        // Video file path
        [StructLayout(LayoutKind.Sequential)]
        public struct DPSDK_FILE_STORE_INFO
        {
            public uint                 uiStoreLen;             // Video length
            public Int64                lBeginTime;		        // Video start time
            public Int64                lEndTime;		        // Video end time
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = DPSDK_FILE_PATH_LEN)]
            public string               szFile;	                // Full path of video files
        }

        // Operating parameters of cloud platform
        [StructLayout(LayoutKind.Sequential)]
        public struct DPSDK_PTZOPERATE_FUNCTION_PARAM
        {
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 64)]
            public string               szChannelId;            // Channel ID
            public PtzOperateFunction_e iPtzOFType;             // Operating function type of cloud platform
            public int                  iCruiseId;              // (This variable is only in Switch_CruiseOperation when effective) Cruise ID
            public int                  iTrackId;               // (This variable is only in Switch_Track、Switch_SetTrackEffective operation during operation ID
            public int                  iSwitchMode;            // (This variable is only in SwitchEffective operation)0-Close，1-open
            public int                  iBorderType;            // (This variable is only in Set_LineScannBorderEffective operation)16-Left boundary17-.Right boundary
            public int                  iAssisentType;          // (This variable is only in Switch_AssisentPointEffective operation)23-Backlight compensation,24-Number doubled,27- Color turn black,35-Shutter time,41-Brightness,42-Image flip,43-The name of the preset point is hidden,80-Restore factory settings
            public int                  iMoveType;              // (This variable is only in Move_PtzMenuEffective operation)25-Upward movement,26-Move down,27-Left shift,28-Right
            public int                  iSwitchPtzMenu;         // (This variable is only in Show_PtzMenuEffective operation)22=Open the platform menu,23=Close the cloud table menu
        }

        // Function operation result of cloud platform 
        [StructLayout(LayoutKind.Sequential)]
        public struct DPSDK_PTZOPERATE_RESULT
        {
            public DPSDK_PTZ_LOCKUSER   struLockUser;
            public int                  iResult;				// Operation results:0-failure,1-success
        };

        // Lock holder information
        [StructLayout(LayoutKind.Sequential)]
        public struct DPSDK_PTZ_LOCKUSER
        {
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 128)]
            public string               szLockUserName;         // User name for lockin cloud
	        public int		            iLockUserLevel;         // Lock user level of cloud
        }

        // Operating cloud platform camera parameters
        [StructLayout(LayoutKind.Sequential)]
        public struct DPSDK_PTZOPERATE_CAMERA_PARAM
        {
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 64)]
            public string               szChannelId;            // Channel ID
	        public int				    iDirect;                // Direction:1-increase,2-decrease
	        public int				    iCommand;               // Order:0-stop it，1-open
	        public int				    iStep;                  // Step 
	        public int				    iOperateType;           // Operation type:1-variable,2-zoom,3-aperture
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 256)]
	        public string				szExtend;		        // Extended data
            public double               fSpeed;                 // The speed of PTZ,normalized 0~1，0 represent default speed, used when vsl zoomed
	        public int				    iDuration;              // Duration,unit:ms
        }

        // Cloud platform direction control parameters
        [StructLayout(LayoutKind.Sequential)]
        public struct DPSDK_PTZOPERATE_DIRECT_PARAM
        {
	        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 64)]
            public string               szChannelId;            // Channel ID
	        public int				    iStepY;                 // Vertical direction step
	        public int				    iStepX;                 // Horizontal direction step
	        public int				    iDirect;                // Direction:1-On,2-Under the,3-Left,4-Right,5-Upper left,6-Lower left,7-On the right,8-lower right
	        public int				    iCommand;               // Order:0-Stop it, 1-open
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 256)]
            public string               szExtend;               // Extended data
	        public int                  iDuration;              // The duration of PTZ turned, unit:ms
        }

        // Electric focusing control parameters
        [StructLayout(LayoutKind.Sequential)]
        public struct DPSDK_PTZOPERATE_FOCUS_PARAM
        {
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 64)]
            public string               szChannelId;            // Channel ID
	        public float				fFocus;                 // Focal length 
            public float                fZoom;                  // Multiple
            public int                  iOperateType;           // Operation type:0-Reset,1-Continuous focusing,2-Autofocus
        }

        // Control preset point parameters
        [StructLayout(LayoutKind.Sequential)]
        public struct DPSDK_PTZOPERATE_PRESETPOINT_PARAM
        {
	        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 64)]
            public string               szChannelId;            // Channel ID
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 50)]
	        public string				szPointCode;	        // Preset point coding
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 50)]
	        public string				szPointName;	        // Preset point name
	        public int				iOperateType;               // Operation type:1-Location, 2-Set up, 3-delete, 4-Update working time
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 15)]
	        public string				szStartTime;			// Start time(time stamp)
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 15)]
	        public string				szEndTime;				// End time(time stamp)
	        public double			    fSpeed;                 // The speed of PTZ turning, normalized 0~1，0 represent default speed
        }

        // Get a list of preset points
        [StructLayout(LayoutKind.Sequential)]
        public struct DPSDK_PTZ_PRESETPOINT_LIST
        {
	        public uint			        uiTotal;                // Total
            public IntPtr               struPresetPointInfo;    // Preset point list
        }

        // Get the preset point information
        [StructLayout(LayoutKind.Sequential)]
        public struct DPSDK_PTZ_PRESETPOINT_INFO
        {
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 50)]
	        public string				szPointName;	        // Preset point name
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 50)]
	        public string				szPointCode;	        // Preset point encoding, from 1 start
	        public int				    iPointType;             // Preset point type,0=Ordinary preset point,1=Preset points that have been set for intelligent rules
        }

        // Three dimensional positioning parameters
        [StructLayout(LayoutKind.Sequential)]
        public struct DPSDK_PTZOPERATE_SITPOSITION_PARAM
        {
	        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 64)]
            public string               szChannelId;            // Channel ID
	        public double			    fPointX;                // Horizontal coordinates:-8192 - Eight thousand one hundred and ninety-two
	        public double			    fPointY;                // Vertical coordinates:-8192 - Eight thousand one hundred and ninety-two
	        public double			    fPointZ;                // Variable number:-4 - 4
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 256)]
	        public string				szExtend;               // Extended data
        }

        // Lock the unlocking parameters
        [StructLayout(LayoutKind.Sequential)]
        public struct DPSDK_PTZOPERATE_ARRANGEPTZ_PARAM
        {
	        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 64)]
            public string               szChannelId;            // Channel ID
	        public uint			        uiLockTime;             // Lock time, unit second,texpression has been locked until the release or Bei Qiang Wins 
	        public int                  iOperateType;           // Operation type:0-Unknown, 1-Lock the current camera, 2-Unlock the current camera, 3-Unlock all the cameras locked by the user, 4-Lock all the cameras, 5-Query lock state
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 256)]
            public string               szExtend;               // Extended data
        }

        // Alarm output control parameters
        [StructLayout(LayoutKind.Sequential)]
        public struct DPSDK_PTZOPERATE_ALARMOUT_PARAM
        {
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 64)]
            public string               szChannelId;            // Channel ID
	        public int 					iOperateType;           // Control type:1-Status control,2-Pattern control
	        public int 					iCommand;               // Control commands: state control,1-Open,0-Shut down; mode control:0-Close，1-Automatically,2-Manual
        }

        // Alarm confirmation parameter
        [StructLayout(LayoutKind.Sequential)]
        public struct DPSDK_CONFIRMALARM_PARAM
        {
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 50)]
            public string               szAlarmCode;            // Alarm code
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 64)]
            public string               szHandleUser;           // Handling human username
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 766)]
            public string               szHandleMessage;        // Handling opinions
	        public uint			        uiEmailRevceiverNumber; // Alarm processing mailbox number
	        public int				    iHandleStatus;          // Processing state (Reference resources AlarmDealWith_e)
            public IntPtr               struEmailReceiverList;  // Alarm processing notification mailbox list
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 1024)]
            public string               szAlarmComment;
        }

        // E-mail address
        [StructLayout(LayoutKind.Sequential)]
        public struct DPSDK_EMAILADDRESS
        {
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 320)]
            public string               szEmailAddr;            // E-mail address
        }

        // Alarm query parameters
        [StructLayout(LayoutKind.Sequential)]
        public struct DPSDK_QUERYALARM_PARAM
        {
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 15)]
            public string               szBeginTime;            // Start time of alarm yyyymmddhhmmss
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 15)]
            public string               szEndTime;              // The end time of the alarm yyyymmddhhmmss
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 15)]
            public string               szHandleBeginTime;      // Alarm processing start time yyyymmddhhmmss
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 15)]
            public string               szHandleEndTime;        // Alarm processing end time yyyymmddhhmmss
            //[MarshalAs(UnmanagedType.ByValTStr, SizeConst = 56)]
            //public string               szDeviceId;             // Device ID
            public IntPtr               pDeviceIdList;
            public int                  iDeviceIdNum;
            public IntPtr               pChannelIdList;
            public int                  iChannelIdNum;
	        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 20)]
            public string               szOrgId;                // Organization node ID
	        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 20)]
            public string               szAlarmId;              // Alarm ID
	        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 50)]
            public string               szAlarmCode;            // Alarm code (specifying this condition to ignore other conditions）
	        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 64)]
            public string               szHandleUser;           // Alarm processing person
	        public int				    iPageSize;              // Number of alerts per page
            public int                  iPageNo;                // Query page number (from 1Start）
            public int                  iSortType;              // Sort fields (1=Alarm time,2=Alarm type,3=Alarm level,4=report Police officer,5=Processing state）
            public int                  iSortOrder;             // Sort direction (0=Ascending order,1=Descending order）
	        public IntPtr			    pAlarmType;             // Alarm type (Reference resources Alarm_type_e)
	        public uint			        uiAlarmTypeNumber;      // Number of alarm types
            public IntPtr               pAlarmGrade;            // Alarm level (Reference resources AlarmLevel_e)
            public uint                 uiAlarmGradeNumber;     // The number of alarm levels
            public IntPtr               pAlarmStatus;           // Alarm state (Reference resources AlarmState_e)
            public uint                 uiAlarmStatusNumber;    // The number of alarm states
            public IntPtr               pHandleStatus;          // Alarm processing state (Reference resources AlarmDealWith_e)
            public uint                 uiHandleStatusNumber;   // The number of state of the alarm processing
        }

        // Channel code
        [StructLayout(LayoutKind.Sequential)]
        public struct DPSDK_CHANNEL_ID
        {
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 64)]
            public string               szId;                   // Channel ID
        };

        // Alarm record list
        [StructLayout(LayoutKind.Sequential)]
        public struct DPSDK_ALARM_DETAILINFO_LIST
        {
            public Int64                tQueryTime;				// The service time of this query,unit seconds
	        public uint                 uiTotal;				// Total number of alarm records
	        public IntPtr               struAlarmInfoList;      // Alarm record
        }

        // Alarm information
        [StructLayout(LayoutKind.Sequential)]
        public struct DPSDK_ALARM_DETAILINFO
        {
	        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 20)]
            public string               szAlarmId;              // Alarm ID
	        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 56)]
            public string               szDeviceId;             // Device ID
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 256)]
            public string               szDeviceName;           // Device name
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 64)]
            public string               szChannelId;            // Channel ID
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 256)]
            public string               szChannelName;          // Channel name
	        public int				    iAlarmGrade;            // Alarm level(Reference resources AlarmLevel_e)
	        public int				    iAlarmType;             // Alarm type(Reference resources Alarm_type_e)
	        public int				    iAlarmStatus;           // Alarm state(Reference resources AlarmState_e)
	        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 64)]
            public string               szHandleUser;           // Alarm processing person
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 15)]
            public string               szHandleTime;           // Alarm processing time yyyymmddhhmmss
	        public int				    iHandleStatus;          // Alarm processing state(Reference resources AlarmDealWith_e)
	        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 766)]
            public string               szHandleMessage;        // Handling opinions
	        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 50)]
            public string               szAlarmCode;            // Alarm code
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 15)]
            public string               szAlarmTime;            // Alarm time yyyymmddhhmmss
	        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 256*16)]
            public string               szAlarmPicture;         // Alarm snapshot path
	        public uint			        uiAlarmPictureSize;     // Alarm snapshot size
	        public uint			        uiEmailReceiverListSize;// The actual number of notification mailbox lists is not greater than that of the alarm DPSDK_EMAILRECEIVERLIST_SIZE
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 10)]
            public DPSDK_EMAILADDRESS[] struEmailReceiverList;  // Alarm processing notification mailbox list(Most return DPSDK_EMAILRECEIVERLIST_SIZEA mail address)
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 200)]
            public DPSDK_ALARM_MEMO_INFO[] struMemoList;        // Alarm memo list
	        public int                  iMemoNum;               // Alarm memo number
	        public Int64                tQueryTime;
        };

        [StructLayout(LayoutKind.Sequential)]
        public struct DPSDK_ALARM_MEMO_INFO
        {
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 256)]
            public string               szMemo;                 // Alarm memo data
        }

        // Alarm total query parameters
        [StructLayout(LayoutKind.Sequential)]
        public struct DPSDK_QUERYALARMCOUNT_PARAM
        {
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 15)]
            public string               szBeginTime;            // Start time of alarm yyyymmddhhmmss
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 15)]
            public string               szEndTime;              // The end time of the alarm yyyymmddhhmmss
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 15)]
            public string               szHandleBeginTime;      // Alarm processing start time yyyymmddhhmmss
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 15)]
            public string               szHandleEndTime;        // Alarm processing end time yyyymmddhhmmss
            //[MarshalAs(UnmanagedType.ByValTStr, SizeConst = 56)]
            //public string               szDeviceId;             // Device ID
            public IntPtr               pDeviceIdList;
            public int                  iDeviceIdNum;
            public IntPtr               pChannelIdList;
            public int                  iChannelIdNum;
	        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 20)]
            public string               szOrgId;                // Organization node ID
	        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 20)]
            public string               szAlarmId;              // Alarm ID
	        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 50)]
            public string               szAlarmCode;            // Alarm code (specifying this condition to ignore other conditions)
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 64)]
            public string               szHandleUser;           // Alarm processing person
            public IntPtr               pAlarmType;             // Alarm type (Reference resources Alarm_type_e)
            public uint                 uiAlarmTypeNumber;      // Number of alarm types
            public IntPtr               pAlarmGrade;            // Alarm level (Reference resources AlarmLevel_e)
            public uint                 uiAlarmGradeNumber;     // The number of alarm levels
            public IntPtr               pAlarmStatus;           // Alarm state (Reference resources AlarmState_e)
            public uint                 uiAlarmStatusNumber;    // The number of alarm states
            public IntPtr               pHandleStatus;          // Alarm processing state (Reference resources AlarmDealWith_e)
            public uint                 uiHandleStatusNumber;   // The number of state of the alarm processing
        }

        // Alarm processing record list
        [StructLayout(LayoutKind.Sequential)]
        public struct DPSDK_ALARMPROCESS_DETAILINFO_LIST
        {
	        public uint                 uiTotal;                // Total number of alarm processing information
	        public IntPtr	            struAlarmProcessInfoList;// Alarm processing information
        }

        // Alarm processing record
        [StructLayout(LayoutKind.Sequential)]
        public struct DPSDK_ALARMPROCESS_DETAILINFO
        {
	        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 64)]
            public string               szHandleUser;           // Alarm processing person
	        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 15)]
            public string               szHandleTime;           // Alarm processing time yyyymmddhhmmss
	        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 766)]
            public string               szHandleMessage;        // Warning handling opinion
            public int				    iHandleStatus;          // Alarm processing state(Reference resources AlarmDealWith_e)
        }

        // Shielded alarm parameter
        [StructLayout(LayoutKind.Sequential)]
        public struct DPSDK_BLOCKALARM_PARAM
        {
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 100)]
            public string               szAlarmCodeSource;      // Shielded alarm source (device alarm for device code, channel alarm as channel Code, system alarm for service code)
	        public int				    iAlarmType;             // Shielding alarm type (Reference resources Alarm_type_e)
	        public int				    iDuration;              // The length of the shielding time (unit: Second)
        }

        // Alarm export parameters
        [StructLayout(LayoutKind.Sequential)]
        public struct DPSDK_ALARMEXPORT_PARAM
        {
	        public int				    iSortType;              // Sort fields (1=Alarm time,2=Alarm type,3=Alarm level,4=AlarmTake care of people,5=Processing state)
	        public int				    iSortOrder;             // Sort direction (0=Ascending order,1=Descending order)
	        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 20)]
            public string               szAlarmId;              // Alarm ID
	        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 50)]
            public string               szAlarmCode;            // Alarm code
	        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 20)]
            public string               szOrgId;                // Organization node ID
	        //[MarshalAs(UnmanagedType.ByValTStr, SizeConst = 56)]
            //public string               szDeviceId;             // Device ID
            public IntPtr               pDeviceIdList;
            public int                  iDeviceIdNum;
            public IntPtr               pChannelIdList;
            public int                  iChannelIdNum;
	        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 15)]
            public string               szBeginTime;            // Start time of alarm yyyymmddhhmmss
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 15)]
            public string               szEndTime;              // The end time of the alarm yyyymmddhhmmss
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 15)]
            public string               szHandleBeginTime;      // Alarm processing start time yyyymmddhhmmss
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 15)]
            public string               szHandleEndTime;        // Alarm processing end time yyyymmddhhmmss
	        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 64)]
            public string               szHandleUser;           // Alarm processing person
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 20)]
            public string               szLanguage;             // Language
            public IntPtr               pAlarmType;             // Alarm type(Reference resources Alarm_type_e)
            public uint                 uiAlarmTypeNumber;      // Number of alarm types
            public IntPtr               pAlarmGrade;            // Alarm level(Reference resources AlarmLevel_e)
            public uint                 uiAlarmGradeNumber;     // The number of alarm levels
            public IntPtr               pAlarmStatus;           // Alarm state(Reference resources AlarmState_e)
            public uint                 uiAlarmStatusNumber;    // The number of alarm states
            public IntPtr               pHandleStatus;          // Alarm processing state(Reference resources AlarmDealWith_e)
            public uint                 uiHandleStatusNumber;   // The number of state of the alarm processing
        }

        // Alarm event (notice)
        [StructLayout(LayoutKind.Sequential)]
        public struct DPSDK_ALARMEVENT_NOTIFY
        {
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 50)]
            public string               szAlarmCode;			// Alarm code
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 90)]
            public string               szAlarmNodeCode;        // Alarm source code
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 15)]
            public string               szAlarmTime;            // Alarm time yyyymmddhhmmss
	        public int					iAlarmGrade;			// Alarm level (Reference resources AlarmLevel_e)
	        public int					iAlarmStatus;			// Alarm state (Reference resources AlarmState_e)
	        public int					iAlarmObjType;			// Alarm object type (Reference resources AlarmObject_e)
	        public int					iAlarmType;				// Alarm type (Reference resources Alarm_type_e)
	        public int					iAlarmCategory;			// Type of alarm (Reference resources AlarmCategory_e)
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 256)]
            public string               szAlarmMessage;         // Alarm extension information (for example,GPSThe extended information of the alarm includes the latitude and longitude, the heightEtc
	        public uint				    uiAlarmLinkVedioListSize;// Alarm video linkage information list number(Not greater than DPSDK_ALARM_LINKVEDIOINFOLIST_SIZE)
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 16)]
            public DPSDK_ALARMLINKVEDIO_INFO[] struAlarmLinkVedioList;// Alarm video linkage information list(Most returnDPSDK_ALARM_LINKVEDIOINFOLIST_SIZEVideo linkage information)
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 256)]
            public string               szAlarmPicture;         // Alarm smart snapshot path
	        public uint				    uiAlarmPictureSize;		// Alarm smart snapshot size
	        public int					iScreenNum;				// The number of Screen
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 200)]
            public DPSDK_ALARM_MEMO_INFO[] struMemoList;        // Alarm memo list
	        public int                  iMemoNum;               // Alarm memo number
        }

        // Alarm linkage video information
        [StructLayout(LayoutKind.Sequential)]
        public struct DPSDK_ALARMLINKVEDIO_INFO
        {
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 64)]
            public string               szLinkVedioId;          // Linkage video channel ID
	        public int			        iStreamType;			// Code stream type
	        public int			        iScreenId;				// Screen ID
        }

        // Alarm confirmation (notice)
        [StructLayout(LayoutKind.Sequential)]
        public struct DPSDK_ALARMCONFIRM_NOTIFY
        {
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 50)]
            public string               szAlarmCode;            // Alarm code
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 15)]
            public string               szAlarmTime;            // Alarm time yyyymmddhhmmss
	        public int				    iHandleStatus;			// Processing state
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 766)]
            public string               szHandleMessage;        // Handling opinions
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 64)]
            public string               szHandleUser;           // Handling human user name
	        public uint			        uiEmailReceiverListSize;// The actual number of notification mailbox lists is not greater than that of the alarm. DPSDK_EMAILRECEIVERLIST_SIZE）
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 10)]
            public DPSDK_EMAILADDRESS[] struEmailReceiverList;  // Alarm processing notification mailbox list(Most return PSDK_EMAILRECEIVERLIST_SIZEA mail address)
            public int				    iAlarmGrade;			// Alarm level (Reference resources AlarmLevel_e)
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 256)]
            public string               szAlarmPicture;         // Alarm smart snapshot path
	        public uint			        uiAlarmPictureSize;		// Alarm smart snapshot size
	        public int				    iAlarmStatus;			// Alarm state (Reference resources AlarmState_e)
	        public int				    iAlarmType;				// Alarm type (Reference resources Alarm_type_e)
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 56)]
            public string               szDeviceId;             // Device ID
	        public int				    iChannelSeq;			// Channel number
	        public int				    iUnitType;				// Unit type
	        public int				    iAlarmObjType;			// Alarm object type (Reference resources AlarmObject_e)
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 90)]
            public string               szAlarmNodeCode;        // Alarm source code
	        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 200)]
            public DPSDK_ALARM_MEMO_INFO[] struMemoList;        // Alarm memo list
	        public int                  iMemoNum;				// Alarm memo number
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 1024)]
            public string               szAlarmComment;
        }

        // Alarm information (notice)
        [StructLayout(LayoutKind.Sequential)]
        public struct DPSDK_ALARM_DETAILINFO_NOTIFY
        {
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 50)]
            public string               szAlarmCode;            // Alarm code
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 15)]
            public string               szAlarmTime;            // Alarm time yyyymmddhhmmss
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 256*16)]
            public string               szAlarmPicture;         // Alarm snapshot path
	        public uint			        uiAlarmPictureSize;		// Alarm snapshot size
        }

        // Alarm export results (notice)
        [StructLayout(LayoutKind.Sequential)]
        public struct DPSDK_ALARMEXPORT_RESULT_NOTIFY
        {
	        public uint				    uiSessionId;			// Session marking
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 256)]
            public string               szDownloadPath;         // Downloading path
        }

        // Query record information
        [StructLayout(LayoutKind.Sequential)]
        public struct DPSDK_QUERY_RECORD_PARAM
        {
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 64)]
            public string               szCameraId;             // Channel ID
	        public DPSDK_STREAM_TYPE    iStreamType;            // Code stream type
	        public DPSDK_SOURCE_TYPE    iSourceType;            // Video source type
	        public DPSDK_RECORD_TYPE    iRecordType;            // Video type
            public Int64                tBeginTime;             // Start time
            public Int64                tEndTime;               // End time
        }

        // Record information
        [StructLayout(LayoutKind.Sequential)]
        public struct DPSDK_RECORD_INFO_LIST
        {
	        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 64)]
            public string               szCameraId;             // Channel ID
	        public uint	                iRetCount;              // Return the number of records, that is record number of recorded video records
	        public IntPtr	            struSingleRecord;       // Video recording information
        }

        [StructLayout(LayoutKind.Sequential)]
        public struct DPSDK_SINGLE_RECORD_INFO
        {
	        public DPSDK_SOURCE_TYPE	iSourceType;            // Video source
	        public DPSDK_RECORD_TYPE	iRecordType;            // Video type. See RecordType_e
	        public Int64		        iStartTime;             // Start time
	        public Int64		        iEndTime;               // End time
			[MarshalAs(UnmanagedType.ByValTStr, SizeConst = 256)]
            public string               szName;                 // The name of the video (different manufacturers are different in the identification of the documents)			
	        public Int64		        iLength;                // File length, unit KB
	        public DPSDK_STREAM_TYPE	iStreamType;            // Code stream type

			// Here's the information needed for the center video
	        public Int64		        iPlanId;                // Video plan ID
	        public int		            iSSId;                  // Storage service ID
			[MarshalAs(UnmanagedType.ByValTStr, SizeConst = 32)]
            public string               szDiskId;               // Disk ID
	        public int		            iFileHandle;            // File handle
			[MarshalAs(UnmanagedType.ByValTStr, SizeConst = 64)]
            public string               szChannelCode;          // Channel coding
	        public byte		            bRecordHidden;          // Video hiding state True: concealment ；False Visible
	        public byte		            bForgotten;             // Do you forget to forget the video

			// The informations of alarm video what are add
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 64)]
            public string               szAlarmChannelId;       // Video camera ID
	        public byte                 bLocked;                // Whether or not to be locked,Device video will not be locked
        }

        // Query record data parameter
        [StructLayout(LayoutKind.Sequential)]
        public struct DPSDK_QUERY_RECORD_DATE_PARAM
        {
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 64)]
            public string               szCameraId;             // Channel ID
	        public DPSDK_SOURCE_TYPE    iSourceType;            // Video source type
	        public int                  iYear;                  // Year
	        public int                  iMonth;                 // Month
        }

        // Record data
        [StructLayout(LayoutKind.Sequential)]
        public struct DPSDK_RECORD_DATE_INFO
        {
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 31)]
            public int[]                RecordDays;             // The record is video taped. 0 start for the first day
        }

        // Channel video information
        [StructLayout(LayoutKind.Sequential)]
        public struct DPSDK_RECORD_STATUS_INFO
        {
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 64)]
            public string               szChannelCode;          // Channel coding	
            public int                  iChannelSeq;            // Channel number
            public int                  iRecordStatus;          // Video status SeeDPSDK_RECORD_STATUSDefinition
            public int                  iFlow;                  // Average flow rate (Kbps）
            public int                  iStreamType;            // Code stream type See DPSDK_STREAM_TYPEDefinition
            public int                  iUsedCapacity;          // Used storage capacity
        }

        // Lock record file
        [StructLayout(LayoutKind.Sequential)]
        public struct DPSDK_LOCK_RECORD_FILE_PARAM
        {
	        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 64)]
            public string               szCameraId;			    // Camera ID
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 256)]
            public string               szFilename;             // The name of the video (different manufacturers are different in the identification of the documents)
        }

        // Locking or unlocking the results of video files
        [StructLayout(LayoutKind.Sequential)]
        public struct DPSDK_LOCK_RECORD_FILE_RESULT
        {
            public int                  iLockNum;               // Lock number
        }

        // Unlock record file
        [StructLayout(LayoutKind.Sequential)]
        public struct DPSDK_UNLOCK_RECORD_FILE_PARAM
        {
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 64)]
            public string               szCameraId;			    // Camera ID
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 256)]
            public string               szFilename;             // The name of the video (different manufacturers are different in the identification of the documents)
	        public byte	                bForce;                 // Whether or not compulsory
        }

        // Query record lock information
        [StructLayout(LayoutKind.Sequential)]
        public struct DPSDK_QUERY_LOCK_RECORD_PARAM
        {
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 64)]
            public string               szUserId;			    // User ID
	        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 64)]
            public string               szCameraId;			    // Camera ID
	        public DPSDK_SOURCE_TYPE    iSourceType;            // Video source, platform video or device video
	        public Int64                tStartTime;             // The start time of lock
	        public Int64                tEndTime;               // The unlock time of lock
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 128)]
            public string               szReason;			    // The reason of lock
        }

        [StructLayout(LayoutKind.Sequential)]
        public struct DPSDK_RECORD_LOCK_INFO_LIST
        {
	        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 64)]
            public string               szCameraID;			    // Camera ID
	        public int                  iRetCount;              // The number of returned
	        public IntPtr	            struSingleRecordLockInfo;// Used to save the informations of video lock which are query, the size of it depend on iReqCount
        }

        [StructLayout(LayoutKind.Sequential)]
        public struct DPSDK_SINGLE_RECORD_LOCK_INFO
        {
	        public int                  iLockId;                // Video lock ID
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 64)]
            public string               szUserId;			    // Lock user
	        public DPSDK_SOURCE_TYPE    iSourceType;            // Video source, platform video or device video
	        public Int64                tOperateTime;           // Operate time
	        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 64)]
            public string               szCameraId;			    // Camera ID
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 64)]
            public string               szUserIp;			    // User IP
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 128)]
            public string               szUserName;			    // User name
	        public Int64                tStartTime;             // The start time of lock
	        public Int64                tEndTime;               // The end time of lock
	        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 128)]
            public string               szReason;			    // The reason of lock
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 256)]
            public string               szFileName;			    // The name of record file
        }

        // Open the manual video parameters
        [StructLayout(LayoutKind.Sequential)]
        public struct DPSDK_PTZOPERATE_STARTREMOTERECORD_PARAM
        {
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 64)]
            public string               szChannelId;            // Camera ID
	        public int					iStreamType;            // Code stream type (code stream type:1-Main stream, 2-Auxiliary code stream)
	        public int					iRecordDuration;        // Video length(default 12*3600s)
        }

        // Open/Stop the result of manual video
        [StructLayout(LayoutKind.Sequential)]
        public struct DPSDK_PTZOPERATE_REMOTERECORD_RESULT
        {
	        public int					iPlanId;                // Plan ID
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 15)]
            public string               szNow;                  // Current time(time stamp)
        }

        // Turn off the manual video parameters
        [StructLayout(LayoutKind.Sequential)]
        public struct DPSDK_PTZOPERATE_STOPREMOTERECORD_PARAM
        {
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 64)]
            public string               szChannelId;            // Camera ID
            public int                  iStreamType;            // Code stream type (code stream type:1-Main stream, 2-Auxiliary code stream)
	        public byte					bForce;                 // Is it forced to close
        }

        // Playback by time parameter
        [StructLayout(LayoutKind.Sequential)]
        public struct DPSDK_PLAYBACK_BY_TIME_PARAM
        {
	        public DPSDK_MEDIA_CALLBACK	struMediaCallBack;      // Video callback function

	        public IntPtr               pHWnd;                  // Window handle
	        public int		            iDirection;             // Playback direction See DPSDK_PLAY_DIRECTION Definition

            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 64)]
            public string               szCodeId;               // Channel id or device id
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 56)]
            public string               szDeviceCode;           // Device code for request media key
	        public Int64		        tBeginTime;             // Start time(time stamp)
	        public Int64		        tPlayTime;              // Start playing time
	        public Int64		        tEndTime;               // End time(time stamp)
	        public int		            iRecordSource;          // Video source, see see DPSDK_SOURCE_TYPE
	        public int		            iStreamType;            // Code stream type, see see DPSDK_STREAM_TYPE
	        public int		            iRecordType;            // Video type, see see DPSDK_RECORD_TYPE
        }

        // Playback by file parameter
        [StructLayout(LayoutKind.Sequential)]
        public struct DPSDK_PLAYBACK_BY_FILE_PARAM
        {
	        public DPSDK_MEDIA_CALLBACK	struMediaCallBack;      // Video callback function
	
	        public IntPtr               pHWnd;                  // Window handle
	        public int		            iDirection;             // Playback direction See DPSDK_PLAY_DIRECTION Definition

	        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 64)]
            public string               szCodeId;               // Channel ID
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 56)]
            public string               szDeviceCode;           // Device code for request media key
	        public Int64		        tBeginTime;             // Start time
	        public Int64		        tEndTime;               // End time
	        public int		            iRecordSource;          // Video source, see DPSDK_SOURCE_TYPE
	        public UInt64	            uSSId;                  // Storage service (IDReturn to the query)
	        public UInt64	            uFileHandle;            // File handle(Return to the query)
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 32)]
            public string               szDiskId;               // Disk ID(Return to the query)
	        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 256)]
            public string               szFilename;             // The name of the video (different manufacturers are different in the identification of the documents)
        }

        // Seek playback
        [StructLayout(LayoutKind.Sequential)]
        public struct DPSDK_PLAYBACK_SEEK_PARAM
        {
            public Int64                tBeginTime;             // Start time
            public Int64                tEndTime;               // End time
	        public DPSDK_PLAYBACK_SPEED	iSpeed;                 // Playback speed
	        public int		            iDirection;             // Playback direction See DPSDK_PLAY_DIRECTION Definition
            public int                  iNotCheckIFrameTimeOffset;// Check IFrameTimeOffset, 0 check, 1 not check
        }

        // Download parameters by time
        [StructLayout(LayoutKind.Sequential)]
        public struct DPSDK_DOWNLOAD_BY_TIME_PARAM
        {
	        public DPSDK_EVENT_DOWNLOAD_CALLBACK    fEventCallBack;// Event callbacks
	        public IntPtr               pEventUserData;         // Event callback user data

            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 64)]
            public string               szChannelId;            // Channel ID
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 56)]
            public string               szDeviceCode;           // Device code for request media key
	        public DPSDK_SOURCE_TYPE	iSourceType;			// Video source
	        public DPSDK_STREAM_TYPE	iStreamType;			// Code stream type
	        public DPSDK_RECORD_TYPE	iRecordType;			// Video type
	        public Int64			    tBeginTime;				// start time
	        public Int64			    tEndTime;				// End time

            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 256)]
            public string               szChannelName;          // Channel name
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = DPSDK_FILE_PATH_LEN)]
            public string               szDownloadPath;         // Downloading path
	        public DPSDK_RECORD_FILE_NAME_RULE iNameRule;       // Download file naming rules
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = DPSDK_FILE_PATH_LEN)]
            public string               szDownloadFileName;     // Download the name of the file, if it is empty, use INameRuleThe defined rules are generated, not empty,Neglecting INameRule, szDownloadPath, szChannelNamefield
	        public int			        iSplitFileSize;         // Division of file size, unit MB，0Non segmentation
	        public DPSDK_DOWNLOAD_RECORD_FILE_FORMAT iFileFormat;// Download file format
        }

        // Download parameters by file
        [StructLayout(LayoutKind.Sequential)]
        public struct DPSDK_DOWNLOAD_BY_FILE_PARAM
        {
	        public DPSDK_EVENT_DOWNLOAD_CALLBACK	fEventCallBack;// Event callbacks
	        public IntPtr               pEventUserData;         // Event callback user data

	        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 64)]
            public string               szChannelId;            // Channel ID
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 56)]
            public string               szDeviceCode;           // Device code for request media key
	        public DPSDK_SOURCE_TYPE	iSourceType;            // Video source
	        public Int64			    tBeginTime;				// Start time
	        public Int64			    tEndTime;				// End time
	        public UInt64		        uSSId;					// Storage service (IDReturn to the query)
	        public UInt64		        uFileHandle;			// File handle(Return to the query)
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 32)]
            public string               szDiskId;               // Disk ID(Return to the query)
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 256)]
            public string               szFilename;            // The name of the video (different manufacturers are different in the identification of the documents)

	        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 256)]
            public string               szChannelName;          // Channel name
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = DPSDK_FILE_PATH_LEN)]
            public string               szDownloadPath;         // Downloading path
	        public DPSDK_RECORD_FILE_NAME_RULE iNameRule;		// Download file naming rules
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = DPSDK_FILE_PATH_LEN)]
            public string               szDownloadFileName;     // Download the name of the file, if it is empty, use INameRuleThe defined rules are generated, not empty,Neglecting INameRule, szDownloadPath, szChannelNamefield
	        public int			        iSplitFileSize;			// Division of file size, unit MB，0Non segmentation
	        public DPSDK_DOWNLOAD_RECORD_FILE_FORMAT iFileFormat;// Download file format
        }

        [StructLayout(LayoutKind.Sequential, Pack = 8)]
        public struct DPSDK_DOWNLOAD_RECORD_INFO
        {
	        public int			        iDownloadID;
	        public int			        iFileID;
	        public int			        iDownloadMode;
	        public int			        iRecordSource;
	        public int			        iRecordType;
	        public int			        iStreamType;
	        public UInt64		        uiCurFileSize;
	        public UInt64		        uiPrevFileSize;
	        public Int64			    tBeginTime;
	        public Int64			    tEndTime;
	        public int			        iDownloadState;
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 64)]
            public string               szChannelId;
	        public UInt64		        uiFileHandle;
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 32)]
            public string               szDiskId;
	        public int			        iDownloadStatus;
	        public int			        iFileCount;
	        public IntPtr			    szDownloadFileName;
        }

        // Picture turn BMP format
        [StructLayout(LayoutKind.Sequential)]
        public struct DPSDK_CONVERT_BMP
        {
	        public IntPtr			    pBuf;			        // Image data pointer
            public int                  lSize;			        // Image data size
            public int                  lWidth;			        // Image width
            public int                  lHeight;		        // Image height
            public int                  lType;			        // Image type
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 256)]
            public string               szFileName;             // File name to be saved.It is best to BMP as a file extension
        }

        // Picture turn jpeg format
        [StructLayout(LayoutKind.Sequential)]
        public struct DPSDK_CONVERT_JPEG
        {
	        public IntPtr			    pBuf;			        // Image data pointer
            public int                  lSize;			        // Image data size
            public int                  lWidth;			        // Image width
            public int                  lHeight;		        // Image height
            public int                  lType;			        // Image type
	        public int			        iQuality;		        // Image compression quality,region[0, 100]
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = DPSDK_FILE_PATH_LEN)]
            public string               szFileName;             // File name to be saved.It is best to jpg as a file extension
        }

        //Event callback parameter structure
        [StructLayout(LayoutKind.Sequential)]
        public struct DPSDK_EVENT_PARAM
        {
            public int                  iSessionID;				// Conversation ID
            public IntPtr               pBuf;					// Message structure
            public uint                 uiBufLen;				// Message structure length
        }

        // Screen shots callback structure
        [StructLayout(LayoutKind.Sequential)]
        public struct DPSDK_SCREENSHOT
        {
            public Int32                lPort;		            // Channel code
            public IntPtr               pBuf;		            // The data of returned image
            public Int32                lSize;		            // User data
            public Int32                lWidth;		            // Image width, unit:pixel
            public Int32                lHeight;	            // Image height
            public Int32                lStamp;		            // The information of time scale, unit:ms
            public Int32                lType;		            // Data type,T_RGB32,T_UYVY
        }

        // Hierarchical acquisition of device tree request parameters
        [StructLayout(LayoutKind.Sequential)]
        public struct DPSDK_GET_DEVICE_LAYERED_PARAM
        {
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 96)]
            public string               szID;                   // Node ID，Represent one code of orgCode,deviceCode,channelCode
	        public int                  iNodeType;				// DPSDK_NODE_TYPE definition 1:Organization,2:Equipment,3:passageway
	        public int                  iOrgType;				// 1: Basic organization
	        public int                  iShowDev;				// 0: No device nodes are needed,1: Need device node
	        public int                  iDeep;					// 2: organization+Equipment,3Organization+equipment+passageway

	        public int                  iCategoryNum;			// Device large class list length
            public IntPtr               pCategoryList;          // Device large list

	        public int                  iChannelTypeNum;		// Channel type list length
            public IntPtr               pChannelTypeList;       // Channel type set that needs to be querying See DPSDK_DEV_UNIT_TYPE Definition

            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 256)]
            public string               szKeyWord;              // Search keywords
        }

        // Paging information
        [StructLayout(LayoutKind.Sequential)]
        public struct DPSDK_PAGE_INFO
        {
	        public uint                 uiPage;                 // The current paging, from 1 start
	        public uint                 uiPageSize;             // Page size
        }

        [StructLayout(LayoutKind.Sequential)]
        public struct Dev_Info_All
        {
            public ArrayList            vecDevInfo;
        }

        [StructLayout(LayoutKind.Sequential)]
        public struct Dev_Info
        {
            public DPSDK_DEV_INFO       struDevInfo;
            public ArrayList            vecEncChnlInfo;
            public ArrayList            vecDecChnlInfo;
            public ArrayList            vecAlarmInChnlInfo;
            public ArrayList            vecAlarmOutChnlInfo;
            public ArrayList            vecTvWallInChnlInfo;
            public ArrayList            vecTvWallOutChnlInfo;
            public ArrayList            vecDoorChnlInfo;
            public ArrayList            vecVoiceChnlInfo;
            public ArrayList            vecRoadGateChnlInfo;
            public ArrayList            vecLEDChnlInfo;
            public ArrayList            vecDispatcherChnlInfo;
            public ArrayList            vecPosChnlInfo;
            public ArrayList            vecVirtualChnlInfo;
        }

        // Basic equipment information
        [StructLayout(LayoutKind.Sequential)]
        public struct DPSDK_DEV_INFO
        {
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 56)]
            public string               szDeviceID;             // Device ID
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 256)]
            public string               szDeviceName;           // Device name
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 128)]
            public string               szUserName;             // Device login user
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 128)]
            public string               szUserPwd;              // Device login password
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 64)]
            public string               szIP;                   // Device additionIP
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 56)]
            public string               szDeviceSn;             // Device serial number
	        public UInt16               ushPort;				// Device add port
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 64)]
            public string               szDevIP;                // Device real ip
	        public UInt16               ushDevPort;				// Device real port
	        public int                  iManFac;				// Manufacturer
	        public int                  iStatus;				// Device status	See DPSDK_DEV_STATUS Definition
	        public int                  iDevType;				// Device type	see DPSDK_DEV_TYPE Definition
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 96)]
            public string               szOrgCode;              // The organization code
	        public int                  iDomainID;				// Domain ID
	        public int                  iDevModel;				// Device model

           [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 64)]
	        public string szSipId;
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 128)]
	        public string szSipPwd;
	        public int iSipIdNum;
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 20)]
            public DPSDK_CHANNEL_ID[] vthRelatedConfirmVTOSipId;
	        public int iUnitEnable;
	        public int iBuildingEnable;
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 64)]
	        public string szSoftwareVersion;
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 64)]
	        public string szHardwareVersion;
        }

        // Channel basic information
        [StructLayout(LayoutKind.Sequential)]
        public struct DPSDK_BASE_CHANNEL_INFO
        {
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 64)]
            public string               szChannelID;            // Channel ID
	        public int                  iChannelSeq;			// Channel serial number, begin at 0
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 256)]
            public string               szChannelName;          // Channel name
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 56)]
            public string               szChnlSN;               // Channel SN code
	        public int                  iChannelType;			// Channel type		see DPSDK_CHANNEL_TYPE  Only coded channels are currently available classification
	        public int                  iStatus;				// Channel status	see DPSDK_DEV_STATUS
	        public int                  iDomainID;				// Domain ID
        };

        // Channel extent information
        [StructLayout(LayoutKind.Sequential)]
        public struct DPSDK_CHANNEL_EXT_INFO
        {
	        public int                  iRoadway;				// Roadway 
	        public float                fCartMaxSpeed;			// Max speed of cart
            public float                fCartMinSpeed;			// Min speed of cart
            public float                fDollyMaxSpeed;			// Max speed of dolly
            public float                fDollyMinSpeed;			// Min speed of dolly
            public float                fDirection;				// Direction
        }

        // Coded channel information
        [StructLayout(LayoutKind.Sequential)]
        public struct DPSDK_ENC_CHANNEL_INFO
        {
	        public DPSDK_BASE_CHANNEL_INFO struChannelInfo;
	        public DPSDK_CHANNEL_EXT_INFO struChnExtInfo;		// Channel extent information

	        public int                  iCameraType;			// Camera type See DPSDK_CAMERA_TYPE Definition
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 50)]
            public string               szLatitude;             // Latitude
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 50)]
            public string               szLongitude;            // Longitude
	        public int                  iCameraFunction;		// 0 No support function 1 Support fish eye 2 Support electric focusing
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 64)]
            public string               szMulticastIP;          // Multicast IP
	        public UInt16 ushMulticastPort;						// Multicast port
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 64)]
            public string               szNVR_IPCIP;            // NVR Front end IPC IP
	        public int                  iChannelRemoteType;		// Remote channel type See DPSDK_CHANNEL_REMOTE_TYPE Definition
	        public int                  iFaceFunction;			// Face function, 0= not support, 1= snap, 2= identify
	        public int                  iIntelliState;			// Intelligent status 0 off-ling,1 on-ling
            public int                  iTargetDetection;		// Intelligent status 0= not support

	        // The type of unit in which the channel belongs
	        public int                  iTrackID;				// Flow type
	        public int                  iStreamType;			// Code stream type See DPSDK_STREAM_TYPE Definition
	        public byte                 bZeroEncode;			// Does it support 0 Channel multi picture coding
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 64)]
            public string               szKeyCode;              // Key code
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 64)]
            public string               szDB33Code;             // DB code
	        public int                  iPCFlag;				// 1=Area,2=In-Out,3=Out Door
        }

        // Decoding channel information
        [StructLayout(LayoutKind.Sequential)]
        public struct DPSDK_DEC_CHANNEL_INFO
        {
	        public DPSDK_BASE_CHANNEL_INFO struChannelInfo;
	        public int                  iMaxSpliteNum;			// Maximum division number equipment related
	
	        // The type of unit in which the channel belongs
	        public int                  iDecodeMode;			// Decoding mode See DPSDK_DECODE_MODE Definition
	        public byte                 bConbineStatus;			// Does it support conbine
        };

        // Alarm input channel information
        [StructLayout(LayoutKind.Sequential)]
        public struct DPSDK_ALARMIN_CHANNEL_INFO
        {
	        public DPSDK_BASE_CHANNEL_INFO struChannelInfo;
            public int                  iAlarmType;				// Alarm type
            public int                  iAlarmLevel;			// Alarm level
        }

        // Alarm output channel information
        [StructLayout(LayoutKind.Sequential)]
        public struct DPSDK_ALARMOUT_CHANNEL_INFO
        {
	        public DPSDK_BASE_CHANNEL_INFO struChannelInfo;
            public int                  iAlarmType;				// Alarm type
        }

        // Large screen input channel data
        [StructLayout(LayoutKind.Sequential)]
        public struct DPSDK_TVWALLIN_CHANNEL_INFO
        {
	        public DPSDK_BASE_CHANNEL_INFO struChannelInfo;
            public int                  iCameraType;			// Camera type See DPSDK_CAMERA_TYPEDefinition
            public int                  iChannelRemoteType;		// Remote channel type See DPSDK_CHANNEL_REMOTE_TYPEDefinition
        }

        // Large screen output channel data
        [StructLayout(LayoutKind.Sequential)]
        public struct DPSDK_TVWALLOUT_CHANNEL_INFO
        {
	        public DPSDK_BASE_CHANNEL_INFO struChannelInfo;
	        public int                  iDecodeMode;			// Decoding mode See DPSDK_DECODE_MODEDefinition
        };

        // Access data
        [StructLayout(LayoutKind.Sequential)]
        public struct DPSDK_DOOR_CHANNEL_INFO
        {
	        public DPSDK_BASE_CHANNEL_INFO struChannelInfo;

	        // Unit attributes of a channel
	        public int                  iThirdControl;			// Whether third party control is allowed 0 no 1 yes
        }

        // Voice Channel Data
        [StructLayout(LayoutKind.Sequential)]
        public struct DPSDK_VOICE_CHANNEL_INFO
        {
	        public DPSDK_BASE_CHANNEL_INFO struChannelInfo;
	
	        // Channel Cell Attribute
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 64)]
            public string               szVoiceIP;              // Voice Service Address
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 64)]
            public string               szClientIP;             // Voice Client Address
	        public UInt16               ushVoicePort;			// Voice Service Port
            public UInt16               ushStatusPort;			// Voice Status Port
        }

        // Channel gate data
        [StructLayout(LayoutKind.Sequential)]
        public struct DPSDK_ROADGATE_CHANNEL_INFO
        {
	        public DPSDK_BASE_CHANNEL_INFO struChannelInfo;
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 20)]
            public string               szSluiceType;           // Channel gate type
        }

        // LED Channel data
        [StructLayout(LayoutKind.Sequential)]
        public struct DPSDK_LED_CHANNEL_INFO
        {
	        public DPSDK_BASE_CHANNEL_INFO struChannelInfo;
	        public int iFreeParkingSpace;						// Residual parking space
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 256)]
            public string               szLEDChnlDesc;          // Description information
        }

        // Dispatcher channel data
        [StructLayout(LayoutKind.Sequential)]
        public struct DPSDK_DISPATCHER_CHANNEL_INFO
        {
	        public DPSDK_BASE_CHANNEL_INFO struChannelInfo;
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 20)]
            public string               szCallNum;              // Phone number
        }

        // POS Channel data
        [StructLayout(LayoutKind.Sequential)]
        public struct DPSDK_POS_CHANNEL_INFO
        {
	        public DPSDK_BASE_CHANNEL_INFO struChannelInfo;
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 64)]
            public string               szLinkChnl;             // POS Channel binding video source
        }

        // Virtual Channel Data
        [StructLayout(LayoutKind.Sequential)]
        public struct DPSDK_VIRTUAL_CHANNEL_INFO
        {
	        public DPSDK_BASE_CHANNEL_INFO struChannelInfo;
        }

        [StructLayout(LayoutKind.Sequential)]
        public struct Dep_Info_All
        {
	        public Dep_Info			    depInfo;			    // Org data
            public ArrayList            vecSubDepInfo;          // Sub org list
            public ArrayList            vecDevID;			    // Sub device list
            public ArrayList            vecChnlID;			    // Sub channel list
        }

        // Department information
        [StructLayout(LayoutKind.Sequential)]
        public struct Dep_Info
        {
            public string		        strCoding;				// Node code
	        public string		        strDepName;				// Node name
	        public string		        strSN;					// 
	        public int				    nDepType;				// Org node type
	        public int				    nDepSort;				// Org sort
	        public byte                 isParent;
        }

        // Organization device condition
        [StructLayout(LayoutKind.Sequential)]
        public struct DPSDK_QUERY_DEV_INFO
        {
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 96)]
            public string               szOrgCode;              // Organization code
	        public uint                 uiCategoriesCount;		// The number of device categories which are need to query
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 128)]
            public int[]                iCategoriesList;        // The device categories function which are need to query see DPSDK_DEV_UNIT_TYPE
	        public IntPtr               struDevIdList;			// The device id list which are need to query
        }

        [StructLayout(LayoutKind.Sequential)]
        public struct DPSDK_QUERY_ORG_INFO
        {
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 96)]
            public string               szOrgCode;				// Oorganization code is the length which is the default query root organization
            public IntPtr               iChannelTypeList;		// Channel type set that needs to be querying see DPSDK_DEV_UNIT_TYPEDefinition
        }

        // Channel code
        [StructLayout(LayoutKind.Sequential)]
        public struct DPSDK_DEVICE_ID
        {
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 56)]
            public string               szId;                   // Device id
        }

        // Obtain Layered List of Device Tree Returned Result
        [StructLayout(LayoutKind.Sequential)]
        public struct DPSDK_LAYERED_RESULT_LIST
        {
	        public int                  iResultNum;				// List Length
	        public IntPtr               pResultList;			// Result List
        }

        [StructLayout(LayoutKind.Sequential)]
        public struct DPSDK_ORG_SUB_DEV_INFO
        {
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 56)]
            public string	szDeviceId;	                        // Device ID
            public int      iSort;	
        }

        [StructLayout(LayoutKind.Sequential)]
        public struct DPSDK_ORG_SUB_CHANNEL_INFO
        {
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 64)]
            public string	szChannelId;		                // Channel ID
            public int      iSort;								// Sort
        }

        [StructLayout(LayoutKind.Sequential)]
        public struct DPSDK_ORG_INFO
        {
            public DPSDK_ORG_BASE_INFO struOrgBaseInfo;			// Organization Info
            public int iDevNum;									// Number of Child Device
            public IntPtr pDevList;			                    // List of Child Device
            public int iChannelNum;								// Number of Sub-channel
            public IntPtr pChannelList;	                        // List of Sub-channel
            public int iOrgNum;									// Number of Sub-organization
            public IntPtr pOrgList;								// List of Sub-organization
        }

        // Gradation gets the result of the device tree
        [StructLayout(LayoutKind.Sequential)]
        public struct DPSDK_LAYERED_RESULT
        {
	        public int                  iNodeType;				// See DPSDK_NODE_TYPE Definition 1:Organization,2:Equipment,3:passageway

            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 96)]
            public string               szID;                   // Node ID
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 256)]
            public string               szName;                 // Node name
	        public byte                 isParent;				// Whether it is a parent node
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 96)]
            public string               szParentID;             // Parent node ID
	        public int                  iSort;					// Sort value
	        public int                  iStatus;				// Channel state see DPSDK_DEV_STATUS Definition

	        public int                  iType1;					// iNodeType For equipment, it represents a large class of equipment.INodeTypeUnit type for channel time
	        public int                  iType2;					// iNodeType A small class of devices is represented when the device is used.INodeTypeExpress channel type for channel
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 56)]
            public string               szSN;                   // SN code
	        // Equipment information
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 64)]
            public string               szIP;                   // Device IP
	        // Channel information
	        public int                  iChannelSeq;			// Channel code
	        public int                  iDomainID;				// Domain  ID
        }

        // Device list
        [StructLayout(LayoutKind.Sequential)]
        public struct DPSDK_DEV_ALL_INFO_LIST
        {
	        public int                  iDevNum;				// Number of devices
	        public IntPtr               pDevAllInfoList;		// Device list data
        }

        // Device data
        [StructLayout(LayoutKind.Sequential)]
        public struct DPSDK_DEV_ALL_INFO
        {
	        public DPSDK_DEV_INFO       struDevInfo;			// Device data 
        	
	        // Coding channel
	        public int                  iEncChnlNum;			// Number of coded channels 
            public IntPtr               pEncChnlInfoList;		// Code channel list 
	        // Decoding channel
            public int                  iDecChnlNum;			// Decode channel number 
            public IntPtr               pDecChnlInfoList;		// Decode channel list 
	        // Alarm input channel
            public int                  iAlarmInChnlNum;		// Number of alarm input 
            public IntPtr               pAlarmInChnlInfoList;	// Alarm input list 
	        // Alarm output channel
            public int                  iAlarmOutChnlNum;		// Alarm output number 
            public IntPtr               pAlarmOutChnlInfoList;	// Alarm output list 
	        // Large screen input channel
            public int                  iTvWallInChnlNum;		// Large screen input channel number 
            public IntPtr               pTvWallInChnlInfoList;	// Large screen input channel list 

            public int                  iTvWallOutChnlNum;		// Large screen output channel number 
            public IntPtr               pTvWallOutChnlInfoList;	// Large screen output channel list 
	        // Entrance guard channel
            public int                  iDoorChnlNum;			// Number of access channels 
            public IntPtr               pDoorChnlInfoList;		// List of access channels 
	        // Voice channel
            public int                  iVoiceChnlNum;			// Voice channel number 
            public IntPtr               pVoiceChnlInfoList;		// Voice channel list 
	        // Channel gate 
            public int                  iRoadGateChnlNum;		// Number of channel gates 
            public IntPtr               pRoadGateChnlInfoList;	// List of channel gates 
	        // LED channel
            public int                  iLEDChnlNum;			// LED Number of channels 
            public IntPtr               pLEDChnlInfoList;		// LED Channel list 
	        // Dispatcher channel
            public int                  iDispatcherChnlNum;		// Number of channels for the dispatcher 
            public IntPtr               pDispatcherChnlInfoList;// Scheduler list 
	        // POS channel
            public int                  iPosChnlNum;			// POS Number of channels 
            public IntPtr               pPosChnlInfoList;		// POS Channel list 
	        // Virtual channel
            public int                  iVirtualChnlNum;		// Number of virtual channels  
            public IntPtr               pVirtualChnlInfoList;	// Virtual channel list

            // Event detetor channel
            //public int                  iDetetorChnlNum;		// Number of evnet detetor channels  
            //public IntPtr               pDetetorChnlInfoList;	// Event detetor channel list
        }

        #region Windows Form designer general code

        /// <summary>
        /// designer support nessary method
        /// Use code editer edit this context
        /// </summary>
        private void InitializeComponent()
        {
            this.tabControl1 = new System.Windows.Forms.TabControl();
            this.General = new System.Windows.Forms.TabPage();
            this.groupBox_ClientInfo = new System.Windows.Forms.GroupBox();
            this.btnClientVersion = new System.Windows.Forms.Button();
            this.btnClientRoad = new System.Windows.Forms.Button();
            this.btnQueryEmap = new System.Windows.Forms.Button();
            this.btnQueryMenuRight = new System.Windows.Forms.Button();
            this.btnSyncTime = new System.Windows.Forms.Button();
            this.btnQueryFTP = new System.Windows.Forms.Button();
            this.groupBox_LoginOP = new System.Windows.Forms.GroupBox();
            this.checkBoxDoMain = new System.Windows.Forms.CheckBox();
            this.checkBoxHttps = new System.Windows.Forms.CheckBox();
            this.groupBox_ModPWD = new System.Windows.Forms.GroupBox();
            this.textNewPassword = new System.Windows.Forms.TextBox();
            this.btnModifyPassword = new System.Windows.Forms.Button();
            this.label_NewPWD = new System.Windows.Forms.Label();
            this.textResults = new System.Windows.Forms.TextBox();
            this.label_Res = new System.Windows.Forms.Label();
            this.btnLogout = new System.Windows.Forms.Button();
            this.btnLogin = new System.Windows.Forms.Button();
            this.textPassword = new System.Windows.Forms.TextBox();
            this.label_PWD = new System.Windows.Forms.Label();
            this.textUserName = new System.Windows.Forms.TextBox();
            this.label_Name = new System.Windows.Forms.Label();
            this.textServerPort = new System.Windows.Forms.TextBox();
            this.label_Port = new System.Windows.Forms.Label();
            this.textServerIP = new System.Windows.Forms.TextBox();
            this.label_IP = new System.Windows.Forms.Label();
            this.Device = new System.Windows.Forms.TabPage();
            this.groupBox_UserData = new System.Windows.Forms.GroupBox();
            this.label_UserDataInfo = new System.Windows.Forms.Label();
            this.textDeviceUserDataInfo = new System.Windows.Forms.TextBox();
            this.btnDeviceDelUserData = new System.Windows.Forms.Button();
            this.label_UserDataName = new System.Windows.Forms.Label();
            this.textDeviceUserDataName = new System.Windows.Forms.TextBox();
            this.btnDeviceGetUserData = new System.Windows.Forms.Button();
            this.btnDeviceSaveUserData = new System.Windows.Forms.Button();
            this.button50 = new System.Windows.Forms.Button();
            this.treeViewDeviceOrg = new System.Windows.Forms.TreeView();
            this.radioButtonDeviceChannel = new System.Windows.Forms.RadioButton();
            this.radioButtonDeviceDevice = new System.Windows.Forms.RadioButton();
            this.radioButtonDeviceOrg = new System.Windows.Forms.RadioButton();
            this.textDeviceOrgData = new System.Windows.Forms.TextBox();
            this.groupBox_OrgParam = new System.Windows.Forms.GroupBox();
            this.checkBox20 = new System.Windows.Forms.CheckBox();
            this.checkBox19 = new System.Windows.Forms.CheckBox();
            this.checkBox18 = new System.Windows.Forms.CheckBox();
            this.checkBox17 = new System.Windows.Forms.CheckBox();
            this.checkBox16 = new System.Windows.Forms.CheckBox();
            this.checkBox15 = new System.Windows.Forms.CheckBox();
            this.checkBox14 = new System.Windows.Forms.CheckBox();
            this.checkBox13 = new System.Windows.Forms.CheckBox();
            this.checkBox12 = new System.Windows.Forms.CheckBox();
            this.checkBox11 = new System.Windows.Forms.CheckBox();
            this.checkBox10 = new System.Windows.Forms.CheckBox();
            this.checkBox9 = new System.Windows.Forms.CheckBox();
            this.checkBox8 = new System.Windows.Forms.CheckBox();
            this.btnDeviceQueryByLayered = new System.Windows.Forms.Button();
            this.btnDeviceQueryOrg = new System.Windows.Forms.Button();
            this.checkBoxDeviceCompress = new System.Windows.Forms.CheckBox();
            this.checkBoxDeviceContainDevice = new System.Windows.Forms.CheckBox();
            this.checkBox5 = new System.Windows.Forms.CheckBox();
            this.textDeviceOrgCode = new System.Windows.Forms.TextBox();
            this.label_Org = new System.Windows.Forms.Label();
            this.Alarm = new System.Windows.Forms.TabPage();
            this.tabAlarm = new System.Windows.Forms.TabControl();
            this.tabAlarmQuery = new System.Windows.Forms.TabPage();
            this.btnAlarmCountQuery = new System.Windows.Forms.Button();
            this.textAlarmTypeQueryLanguage = new System.Windows.Forms.TextBox();
            this.btnAlarmTypeQuery = new System.Windows.Forms.Button();
            this.label_QueryLan = new System.Windows.Forms.Label();
            this.btnAlarmQuery = new System.Windows.Forms.Button();
            this.label_QuerySO = new System.Windows.Forms.Label();
            this.label_QueryABT = new System.Windows.Forms.Label();
            this.textAlarmQueryBeginTime = new System.Windows.Forms.TextBox();
            this.label_QueryAET = new System.Windows.Forms.Label();
            this.textAlarmQueryEndTime = new System.Windows.Forms.TextBox();
            this.label_QueryHBT = new System.Windows.Forms.Label();
            this.textAlarmQueryHandleBeginTime = new System.Windows.Forms.TextBox();
            this.label_QueryHET = new System.Windows.Forms.Label();
            this.textAlarmQueryHandleEndTime = new System.Windows.Forms.TextBox();
            this.label_QueryDevID = new System.Windows.Forms.Label();
            this.textAlarmQueryDeviceId = new System.Windows.Forms.TextBox();
            this.label_QueryChnlID = new System.Windows.Forms.Label();
            this.textAlarmQueryChannelId = new System.Windows.Forms.TextBox();
            this.label_QueryOrgID = new System.Windows.Forms.Label();
            this.textAlarmQueryOrgId = new System.Windows.Forms.TextBox();
            this.label_QueryAlarmID = new System.Windows.Forms.Label();
            this.textAlarmQueryAlarmId = new System.Windows.Forms.TextBox();
            this.label_QueryAlarmCode = new System.Windows.Forms.Label();
            this.textAlarmQueryAlarmCode = new System.Windows.Forms.TextBox();
            this.label_QueryHU = new System.Windows.Forms.Label();
            this.textAlarmQueryHandleUser = new System.Windows.Forms.TextBox();
            this.label_QueryST = new System.Windows.Forms.Label();
            this.comboBoxAlarmQuerySortType = new System.Windows.Forms.ComboBox();
            this.label_QueryPZ = new System.Windows.Forms.Label();
            this.textAlarmQueryPageSize = new System.Windows.Forms.TextBox();
            this.label_QueryPN = new System.Windows.Forms.Label();
            this.textAlarmQueryPageNum = new System.Windows.Forms.TextBox();
            this.comboBoxAlarmQuerySortOrder = new System.Windows.Forms.ComboBox();
            this.label_QueryATL = new System.Windows.Forms.Label();
            this.textAlarmQueryTypeList = new System.Windows.Forms.TextBox();
            this.label_QueryASL = new System.Windows.Forms.Label();
            this.textAlarmQueryStatusList = new System.Windows.Forms.TextBox();
            this.label_QueryAGL = new System.Windows.Forms.Label();
            this.textAlarmQueryGradeList = new System.Windows.Forms.TextBox();
            this.label_QueryHSL = new System.Windows.Forms.Label();
            this.textAlarmQueryHandleStatusList = new System.Windows.Forms.TextBox();
            this.tabAlarmExport = new System.Windows.Forms.TabPage();
            this.btnAlarmExport = new System.Windows.Forms.Button();
            this.label_ExportHBT = new System.Windows.Forms.Label();
            this.label_ExportABT = new System.Windows.Forms.Label();
            this.textAlarmExportBeginTime = new System.Windows.Forms.TextBox();
            this.label_ExportAET = new System.Windows.Forms.Label();
            this.textAlarmExportHandleStatusList = new System.Windows.Forms.TextBox();
            this.textAlarmExportEndTime = new System.Windows.Forms.TextBox();
            this.label_ExportHSL = new System.Windows.Forms.Label();
            this.textAlarmExportHandleBeginTime = new System.Windows.Forms.TextBox();
            this.textAlarmExportGradeList = new System.Windows.Forms.TextBox();
            this.label_ExportHET = new System.Windows.Forms.Label();
            this.label_ExportAGL = new System.Windows.Forms.Label();
            this.textAlarmExportHandleEndTime = new System.Windows.Forms.TextBox();
            this.textAlarmExportStatusList = new System.Windows.Forms.TextBox();
            this.label_ExportDevID = new System.Windows.Forms.Label();
            this.label_ExportASL = new System.Windows.Forms.Label();
            this.textAlarmExportDeviceId = new System.Windows.Forms.TextBox();
            this.textAlarmExportTypeList = new System.Windows.Forms.TextBox();
            this.label_ExportChnlID = new System.Windows.Forms.Label();
            this.label_ExportATL = new System.Windows.Forms.Label();
            this.textAlarmExportChannelId = new System.Windows.Forms.TextBox();
            this.comboBoxAlarmExportSortOrder = new System.Windows.Forms.ComboBox();
            this.label_ExportOrgID = new System.Windows.Forms.Label();
            this.label_ExportSO = new System.Windows.Forms.Label();
            this.textAlarmExportOrgId = new System.Windows.Forms.TextBox();
            this.textAlarmExportLanguage = new System.Windows.Forms.TextBox();
            this.label_ExportAlarmID = new System.Windows.Forms.Label();
            this.label_ExportLan = new System.Windows.Forms.Label();
            this.textAlarmExportAlarmId = new System.Windows.Forms.TextBox();
            this.comboBoxAlarmExportSortType = new System.Windows.Forms.ComboBox();
            this.label_ExportAlarmCode = new System.Windows.Forms.Label();
            this.label_ExportST = new System.Windows.Forms.Label();
            this.textAlarmExportAlarmCode = new System.Windows.Forms.TextBox();
            this.textAlarmExportHandleUser = new System.Windows.Forms.TextBox();
            this.label_ExportHU = new System.Windows.Forms.Label();
            this.tabAlarmConfirm = new System.Windows.Forms.TabPage();
            this.textAlarmQueryPFAlarmCode = new System.Windows.Forms.TextBox();
            this.btnAlarmQueryPF = new System.Windows.Forms.Button();
            this.textAlarmCodeSource = new System.Windows.Forms.TextBox();
            this.textAlarmBlockType = new System.Windows.Forms.TextBox();
            this.label_ConfirmDS = new System.Windows.Forms.Label();
            this.textAlarmBlockDuration = new System.Windows.Forms.TextBox();
            this.label_ConfirmAT = new System.Windows.Forms.Label();
            this.btnAlarmBlock = new System.Windows.Forms.Button();
            this.label_ConfirmACS = new System.Windows.Forms.Label();
            this.label_ConfirmAPF_AC = new System.Windows.Forms.Label();
            this.btnAlarmConfirm = new System.Windows.Forms.Button();
            this.label_ConfirmAC = new System.Windows.Forms.Label();
            this.textAlarmAlarmCode = new System.Windows.Forms.TextBox();
            this.label_ConfirmHU = new System.Windows.Forms.Label();
            this.textAlarmHandleUser = new System.Windows.Forms.TextBox();
            this.textAlarmEmailList = new System.Windows.Forms.TextBox();
            this.label_ConfirmHM = new System.Windows.Forms.Label();
            this.label_ConfirmEL = new System.Windows.Forms.Label();
            this.textAlarmHandleMsg = new System.Windows.Forms.TextBox();
            this.comboBoxAlarmHandleStatus = new System.Windows.Forms.ComboBox();
            this.label_ConfirmHS = new System.Windows.Forms.Label();
            this.btnAlarmClear = new System.Windows.Forms.Button();
            this.checkBoxAlarmStopRefresh = new System.Windows.Forms.CheckBox();
            this.checkBoxAlarmForbiddenNotify = new System.Windows.Forms.CheckBox();
            this.textAlarmInfo = new System.Windows.Forms.TextBox();
            this.Preview = new System.Windows.Forms.TabPage();
            this.pictureBoxPreviewVideo = new System.Windows.Forms.PictureBox();
            this.textPreviewResult = new System.Windows.Forms.TextBox();
            this.label_PreHR = new System.Windows.Forms.Label();
            this.tabPreview = new System.Windows.Forms.TabControl();
            this.PreviewVideo = new System.Windows.Forms.TabPage();
            this.btnPreviewLockVideo = new System.Windows.Forms.Button();
            this.groupDisRegion = new System.Windows.Forms.GroupBox();
            this.btnPreviewSetRegion = new System.Windows.Forms.Button();
            this.checkBoxPreviewEnable = new System.Windows.Forms.CheckBox();
            this.textPreviewRight = new System.Windows.Forms.TextBox();
            this.label_PreRight = new System.Windows.Forms.Label();
            this.textPreviewBottom = new System.Windows.Forms.TextBox();
            this.label_PreBottom = new System.Windows.Forms.Label();
            this.textPreviewLeft = new System.Windows.Forms.TextBox();
            this.label_PreLeft = new System.Windows.Forms.Label();
            this.textPreviewTop = new System.Windows.Forms.TextBox();
            this.label_PreTop = new System.Windows.Forms.Label();
            this.groupStreamMode = new System.Windows.Forms.GroupBox();
            this.btnPreviewSetStreamMode = new System.Windows.Forms.Button();
            this.btnPreviewGetStreamMode = new System.Windows.Forms.Button();
            this.textPreviewDelayTime = new System.Windows.Forms.TextBox();
            this.textPreviewStreamMode = new System.Windows.Forms.TextBox();
            this.label_PreDelT = new System.Windows.Forms.Label();
            this.label_PreSM = new System.Windows.Forms.Label();
            this.groupVideoPlay = new System.Windows.Forms.GroupBox();
            this.btnPreviewStopVideo = new System.Windows.Forms.Button();
            this.btnPreviewStartVideo = new System.Windows.Forms.Button();
            this.groupMP = new System.Windows.Forms.GroupBox();
            this.textPreviewStartIndex = new System.Windows.Forms.TextBox();
            this.textPreviewTrackID = new System.Windows.Forms.TextBox();
            this.textPreviewScreenNum = new System.Windows.Forms.TextBox();
            this.label_PreSI = new System.Windows.Forms.Label();
            this.label_PreTrackID = new System.Windows.Forms.Label();
            this.label_PreSN = new System.Windows.Forms.Label();
            this.groupVBP = new System.Windows.Forms.GroupBox();
            this.textPreviewDataType = new System.Windows.Forms.TextBox();
            this.textPreviewStreamType = new System.Windows.Forms.TextBox();
            this.textPreviewDeviceID = new System.Windows.Forms.TextBox();
            this.checkBoxPreviewDecode = new System.Windows.Forms.CheckBox();
            this.label_PreDT = new System.Windows.Forms.Label();
            this.label_PreST = new System.Windows.Forms.Label();
            this.label_PreDevID = new System.Windows.Forms.Label();
            this.radioPreviewMultiviewPlay = new System.Windows.Forms.RadioButton();
            this.radioPreviewSinglePlay = new System.Windows.Forms.RadioButton();
            this.PreviewPicture = new System.Windows.Forms.TabPage();
            this.groupGetPic = new System.Windows.Forms.GroupBox();
            this.btnPreviewGetPicToBuffer = new System.Windows.Forms.Button();
            this.btnPreviewGetPicToFile = new System.Windows.Forms.Button();
            this.btnPreviewGetPicSize = new System.Windows.Forms.Button();
            this.pictureBoxPreviewPicture = new System.Windows.Forms.PictureBox();
            this.comboBoxPreviewPicture = new System.Windows.Forms.ComboBox();
            this.textPreviewPicPath = new System.Windows.Forms.TextBox();
            this.label_PrePicPath = new System.Windows.Forms.Label();
            this.PreviewSound = new System.Windows.Forms.TabPage();
            this.labelPreviewVolume = new System.Windows.Forms.Label();
            this.btnPreviewGetVolum = new System.Windows.Forms.Button();
            this.btnPreviewSetVolume = new System.Windows.Forms.Button();
            this.trackBarPreviewVolume = new System.Windows.Forms.TrackBar();
            this.btnPreviewIsOpenSound = new System.Windows.Forms.Button();
            this.btnPreviewCloseShareSound = new System.Windows.Forms.Button();
            this.btnPreviewOpenShareSound = new System.Windows.Forms.Button();
            this.btnPreviewCloseSound = new System.Windows.Forms.Button();
            this.btnPreviewOpenSound = new System.Windows.Forms.Button();
            this.PreviewRecord = new System.Windows.Forms.TabPage();
            this.btnPreviewIsRecordState = new System.Windows.Forms.Button();
            this.btnPreviewStopRecord = new System.Windows.Forms.Button();
            this.btnPreviewStartRecord = new System.Windows.Forms.Button();
            this.btnPreviewSetSplitLen = new System.Windows.Forms.Button();
            this.textPreviewSplitLen = new System.Windows.Forms.TextBox();
            this.label_PreSplitLen = new System.Windows.Forms.Label();
            this.textPreviewRecordPath = new System.Windows.Forms.TextBox();
            this.label_PreRecPath = new System.Windows.Forms.Label();
            this.DeviceFunction = new System.Windows.Forms.TabPage();
            this.tabControl_PTZ = new System.Windows.Forms.TabControl();
            this.tabPage_PTZOpFun = new System.Windows.Forms.TabPage();
            this.groupBox_PTZFun = new System.Windows.Forms.GroupBox();
            this.btnPtzOperateFunction = new System.Windows.Forms.Button();
            this.comboBoxPtzSwitchMenu = new System.Windows.Forms.ComboBox();
            this.label_PTZSM = new System.Windows.Forms.Label();
            this.comboBoxPtzMoveType = new System.Windows.Forms.ComboBox();
            this.label_PTZMT = new System.Windows.Forms.Label();
            this.comboBoxPtzAssisentType = new System.Windows.Forms.ComboBox();
            this.label_PTZAT = new System.Windows.Forms.Label();
            this.comboBoxPtzBorderType = new System.Windows.Forms.ComboBox();
            this.label_PTZBT = new System.Windows.Forms.Label();
            this.comboBoxPtzSwitch = new System.Windows.Forms.ComboBox();
            this.label_PTZSwith = new System.Windows.Forms.Label();
            this.textPtzTrackId = new System.Windows.Forms.TextBox();
            this.label_PTZTraclID = new System.Windows.Forms.Label();
            this.textPtzCruiseId = new System.Windows.Forms.TextBox();
            this.label_PTZCruID = new System.Windows.Forms.Label();
            this.comboBoxPtzOpType = new System.Windows.Forms.ComboBox();
            this.label_PTZOpType = new System.Windows.Forms.Label();
            this.tabPageOPCamera = new System.Windows.Forms.TabPage();
            this.groupBoxPTZDir = new System.Windows.Forms.GroupBox();
            this.btnPtzOpDirect = new System.Windows.Forms.Button();
            this.textPtzOpDirectExtend = new System.Windows.Forms.TextBox();
            this.label_PTZDorEx = new System.Windows.Forms.Label();
            this.textPtzOpDirectStepY = new System.Windows.Forms.TextBox();
            this.label_PTZStepY = new System.Windows.Forms.Label();
            this.textPtzOpDirectStepX = new System.Windows.Forms.TextBox();
            this.label_PTZStepX = new System.Windows.Forms.Label();
            this.comboBoxPtzOpDirectCommand = new System.Windows.Forms.ComboBox();
            this.label_PTZDirCom = new System.Windows.Forms.Label();
            this.comboBoxPtzOpDirect = new System.Windows.Forms.ComboBox();
            this.label_PTZDir = new System.Windows.Forms.Label();
            this.groupBox_PTZOpCam = new System.Windows.Forms.GroupBox();
            this.btnPtzOpCamera = new System.Windows.Forms.Button();
            this.textPtzOpCameraExtend = new System.Windows.Forms.TextBox();
            this.label_PTZCamEx = new System.Windows.Forms.Label();
            this.textPtzOpCameraStep = new System.Windows.Forms.TextBox();
            this.label_PTZStep = new System.Windows.Forms.Label();
            this.comboBoxPtzOpCameraType = new System.Windows.Forms.ComboBox();
            this.label_PTZOpT = new System.Windows.Forms.Label();
            this.comboBoxPtzOpCameraCommand = new System.Windows.Forms.ComboBox();
            this.label_PTZCom = new System.Windows.Forms.Label();
            this.comboBoxPtzOpCameraDirect = new System.Windows.Forms.ComboBox();
            this.label_PTZCamDir = new System.Windows.Forms.Label();
            this.groupBox_PTZFocus = new System.Windows.Forms.GroupBox();
            this.btnPtzOpFocus = new System.Windows.Forms.Button();
            this.textPtzOpFocusZoom = new System.Windows.Forms.TextBox();
            this.label_PTZForZoom = new System.Windows.Forms.Label();
            this.textPtzOpFocus = new System.Windows.Forms.TextBox();
            this.label_PTZFor = new System.Windows.Forms.Label();
            this.comboBoxPtzOpFocusType = new System.Windows.Forms.ComboBox();
            this.label_PTZForOpType = new System.Windows.Forms.Label();
            this.tabPagePreset = new System.Windows.Forms.TabPage();
            this.groupBox_PTZPreset = new System.Windows.Forms.GroupBox();
            this.btnPtzOpGetPreset = new System.Windows.Forms.Button();
            this.btnPtzOpPresetPoint = new System.Windows.Forms.Button();
            this.textPtzOpPresetEndTime = new System.Windows.Forms.TextBox();
            this.label_PTZPointET = new System.Windows.Forms.Label();
            this.textPtzOpPresetStartTime = new System.Windows.Forms.TextBox();
            this.label_PTZPointST = new System.Windows.Forms.Label();
            this.textPtzOpPresetPointName = new System.Windows.Forms.TextBox();
            this.label_PTZPointN = new System.Windows.Forms.Label();
            this.textPtzOpPresetPointCode = new System.Windows.Forms.TextBox();
            this.label_PTZPointC = new System.Windows.Forms.Label();
            this.comboBoxPtzOpPresetType = new System.Windows.Forms.ComboBox();
            this.label_PTZPreOpT = new System.Windows.Forms.Label();
            this.tabPagePTZOther = new System.Windows.Forms.TabPage();
            this.groupBox_PTZSit = new System.Windows.Forms.GroupBox();
            this.btnPtzOpSitPos = new System.Windows.Forms.Button();
            this.textPtzOpSitPosExtend = new System.Windows.Forms.TextBox();
            this.label_PTZSitEx = new System.Windows.Forms.Label();
            this.textPtzOpSitPosPointZ = new System.Windows.Forms.TextBox();
            this.label_PTZSitPZ = new System.Windows.Forms.Label();
            this.textPtzOpSitPosPointY = new System.Windows.Forms.TextBox();
            this.label_PTZSitPY = new System.Windows.Forms.Label();
            this.textPtzOpSitPosPointX = new System.Windows.Forms.TextBox();
            this.label_PTZSitPX = new System.Windows.Forms.Label();
            this.groupBox_PTZArrange = new System.Windows.Forms.GroupBox();
            this.btnPtzOpArrangePtz = new System.Windows.Forms.Button();
            this.textPtzOpArrangeExtend = new System.Windows.Forms.TextBox();
            this.label_PTZArrEx = new System.Windows.Forms.Label();
            this.textPtzOpArrangeLockTime = new System.Windows.Forms.TextBox();
            this.label_PTZArrLT = new System.Windows.Forms.Label();
            this.label_PTZArrOT = new System.Windows.Forms.Label();
            this.comboBoxPtzOpArrangeType = new System.Windows.Forms.ComboBox();
            this.groupBox_PTZAlarm = new System.Windows.Forms.GroupBox();
            this.btnPtzOpAlarmOut = new System.Windows.Forms.Button();
            this.comboBoxPtzOpAlarmOutCommand = new System.Windows.Forms.ComboBox();
            this.label_PTZAlarmCom = new System.Windows.Forms.Label();
            this.label_PTZAlarmOT = new System.Windows.Forms.Label();
            this.comboBoxPtzOpAlarmOutType = new System.Windows.Forms.ComboBox();
            this.btnPtzOpClear = new System.Windows.Forms.Button();
            this.textPtzOpResult = new System.Windows.Forms.TextBox();
            this.textPtzChannelId = new System.Windows.Forms.TextBox();
            this.label_PTZChnlCode = new System.Windows.Forms.Label();
            this.Playback = new System.Windows.Forms.TabPage();
            this.textPlaybackResult = new System.Windows.Forms.TextBox();
            this.tabControl3 = new System.Windows.Forms.TabControl();
            this.PlaybackRecord = new System.Windows.Forms.TabPage();
            this.comboBoxPlaybackQueryRecordType = new System.Windows.Forms.ComboBox();
            this.label59 = new System.Windows.Forms.Label();
            this.groupBox22 = new System.Windows.Forms.GroupBox();
            this.btnPlaybackStartRemoteRecord = new System.Windows.Forms.Button();
            this.btnPlaybackStopRemoteRecord = new System.Windows.Forms.Button();
            this.label85 = new System.Windows.Forms.Label();
            this.textPlaybackDuration = new System.Windows.Forms.TextBox();
            this.label81 = new System.Windows.Forms.Label();
            this.groupBox21 = new System.Windows.Forms.GroupBox();
            this.checkBoxPlaybackForce = new System.Windows.Forms.CheckBox();
            this.textPlaybackUserID = new System.Windows.Forms.TextBox();
            this.label84 = new System.Windows.Forms.Label();
            this.btnPlaybackQueryLockRecord = new System.Windows.Forms.Button();
            this.btnPlaybackUnlockRecordFile = new System.Windows.Forms.Button();
            this.btnPlaybackLockRecordFile = new System.Windows.Forms.Button();
            this.groupBox20 = new System.Windows.Forms.GroupBox();
            this.btnPlaybackGetRecordStatus = new System.Windows.Forms.Button();
            this.btnPlaybackQueryRecordDate = new System.Windows.Forms.Button();
            this.btnPlaybackQueryRecord = new System.Windows.Forms.Button();
            this.comboBoxPlaybackStreamType = new System.Windows.Forms.ComboBox();
            this.comboBoxPlaybackSourceType = new System.Windows.Forms.ComboBox();
            this.label82 = new System.Windows.Forms.Label();
            this.label83 = new System.Windows.Forms.Label();
            this.textPlaybackDeviceID = new System.Windows.Forms.TextBox();
            this.listViewPlaybackRecordInfo = new System.Windows.Forms.ListView();
            this.Seq = new System.Windows.Forms.ColumnHeader();
            this.SourceType = new System.Windows.Forms.ColumnHeader();
            this.RecordType = new System.Windows.Forms.ColumnHeader();
            this.StartTime = new System.Windows.Forms.ColumnHeader();
            this.EndTime = new System.Windows.Forms.ColumnHeader();
            this.FileName = new System.Windows.Forms.ColumnHeader();
            this.FileSize = new System.Windows.Forms.ColumnHeader();
            this.StreamType = new System.Windows.Forms.ColumnHeader();
            this.bLock = new System.Windows.Forms.ColumnHeader();
            this.SSId = new System.Windows.Forms.ColumnHeader();
            this.FileHandle = new System.Windows.Forms.ColumnHeader();
            this.DiskId = new System.Windows.Forms.ColumnHeader();
            this.datePlaybackEndTime = new System.Windows.Forms.DateTimePicker();
            this.label79 = new System.Windows.Forms.Label();
            this.datePlaybackStartTime = new System.Windows.Forms.DateTimePicker();
            this.label78 = new System.Windows.Forms.Label();
            this.PlaybackPlayback = new System.Windows.Forms.TabPage();
            this.pictureBoxPlaybackVideo = new System.Windows.Forms.PictureBox();
            this.groupBox19 = new System.Windows.Forms.GroupBox();
            this.btnPlaybackSetPlaybackSpeed = new System.Windows.Forms.Button();
            this.btnPlaybackGetProviderType = new System.Windows.Forms.Button();
            this.btnPlaybackGetPlayedTime = new System.Windows.Forms.Button();
            this.groupBox18 = new System.Windows.Forms.GroupBox();
            this.btnPlaybackSeek = new System.Windows.Forms.Button();
            this.btnPlaybackFrameStep = new System.Windows.Forms.Button();
            this.btnPlaybackPausePlayback = new System.Windows.Forms.Button();
            this.btnPlaybackFile = new System.Windows.Forms.Button();
            this.btnPlaybackByTime = new System.Windows.Forms.Button();
            this.comboBoxPlaybackDirection = new System.Windows.Forms.ComboBox();
            this.label77 = new System.Windows.Forms.Label();
            this.comboBoxPlaybackSpeed = new System.Windows.Forms.ComboBox();
            this.label76 = new System.Windows.Forms.Label();
            this.PlaybackDownload = new System.Windows.Forms.TabPage();
            this.groupBox17 = new System.Windows.Forms.GroupBox();
            this.comboBoxPlaybackRecordType = new System.Windows.Forms.ComboBox();
            this.comboBoxPlaybackNameRule = new System.Windows.Forms.ComboBox();
            this.comboBoxPlaybackFileFormat = new System.Windows.Forms.ComboBox();
            this.label73 = new System.Windows.Forms.Label();
            this.label74 = new System.Windows.Forms.Label();
            this.label75 = new System.Windows.Forms.Label();
            this.textPlaybackDiskID = new System.Windows.Forms.TextBox();
            this.label72 = new System.Windows.Forms.Label();
            this.textPlaybackChannelID = new System.Windows.Forms.TextBox();
            this.label67 = new System.Windows.Forms.Label();
            this.textPlaybackChannelName = new System.Windows.Forms.TextBox();
            this.label68 = new System.Windows.Forms.Label();
            this.textPlaybackDownloadPath = new System.Windows.Forms.TextBox();
            this.label69 = new System.Windows.Forms.Label();
            this.textPlaybackDownloadFileName = new System.Windows.Forms.TextBox();
            this.label70 = new System.Windows.Forms.Label();
            this.textPlaybackSplitSize = new System.Windows.Forms.TextBox();
            this.label71 = new System.Windows.Forms.Label();
            this.btnPlaybackGetRecordInfo = new System.Windows.Forms.Button();
            this.btnPlaybackDownloadRecordByTime = new System.Windows.Forms.Button();
            this.btnPlaybackPauseDownloadRecord = new System.Windows.Forms.Button();
            this.btnPlaybackDownloadRecordByFile = new System.Windows.Forms.Button();
            this.PlaybackSound = new System.Windows.Forms.TabPage();
            this.labelPlaybackVolume = new System.Windows.Forms.Label();
            this.btnPlaybackGetVolum = new System.Windows.Forms.Button();
            this.btnPlaybackSetVolume = new System.Windows.Forms.Button();
            this.trackBarPlaybackVolume = new System.Windows.Forms.TrackBar();
            this.btnPlaybackIsOpenSound = new System.Windows.Forms.Button();
            this.btnPlaybackCloseShareSound = new System.Windows.Forms.Button();
            this.btnPlaybackOpenShareSound = new System.Windows.Forms.Button();
            this.btnPlaybackCloseSound = new System.Windows.Forms.Button();
            this.btnPlaybackOpenSound = new System.Windows.Forms.Button();
            this.PlaybackPicture = new System.Windows.Forms.TabPage();
            this.groupBox16 = new System.Windows.Forms.GroupBox();
            this.pictureBoxPlaybackPicture = new System.Windows.Forms.PictureBox();
            this.textPlaybackType = new System.Windows.Forms.TextBox();
            this.label66 = new System.Windows.Forms.Label();
            this.textPlaybackSize = new System.Windows.Forms.TextBox();
            this.label65 = new System.Windows.Forms.Label();
            this.textPlaybackQuality = new System.Windows.Forms.TextBox();
            this.label64 = new System.Windows.Forms.Label();
            this.textPlaybackHeight = new System.Windows.Forms.TextBox();
            this.label63 = new System.Windows.Forms.Label();
            this.textPlaybackWidth = new System.Windows.Forms.TextBox();
            this.label62 = new System.Windows.Forms.Label();
            this.btnPlaybackConvertToBmpFile = new System.Windows.Forms.Button();
            this.btnPlaybackGet24BitPictureFile = new System.Windows.Forms.Button();
            this.btnPlaybackConvertToJpegFile = new System.Windows.Forms.Button();
            this.btnPlaybackGetPictureBuf = new System.Windows.Forms.Button();
            this.textPlaybackPicPath = new System.Windows.Forms.TextBox();
            this.label61 = new System.Windows.Forms.Label();
            this.comboBoxPlaybackPicFormat = new System.Windows.Forms.ComboBox();
            this.label60 = new System.Windows.Forms.Label();
            this.btnInit = new System.Windows.Forms.Button();
            this.btnUnInit = new System.Windows.Forms.Button();
            this.textBox24 = new System.Windows.Forms.TextBox();
            this.label88 = new System.Windows.Forms.Label();
            this.textBox25 = new System.Windows.Forms.TextBox();
            this.label89 = new System.Windows.Forms.Label();
            this.textBox26 = new System.Windows.Forms.TextBox();
            this.label90 = new System.Windows.Forms.Label();
            this.textBox27 = new System.Windows.Forms.TextBox();
            this.label91 = new System.Windows.Forms.Label();
            this.tabControl1.SuspendLayout();
            this.General.SuspendLayout();
            this.groupBox_ClientInfo.SuspendLayout();
            this.groupBox_LoginOP.SuspendLayout();
            this.groupBox_ModPWD.SuspendLayout();
            this.Device.SuspendLayout();
            this.groupBox_UserData.SuspendLayout();
            this.groupBox_OrgParam.SuspendLayout();
            this.Alarm.SuspendLayout();
            this.tabAlarm.SuspendLayout();
            this.tabAlarmQuery.SuspendLayout();
            this.tabAlarmExport.SuspendLayout();
            this.tabAlarmConfirm.SuspendLayout();
            this.Preview.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBoxPreviewVideo)).BeginInit();
            this.tabPreview.SuspendLayout();
            this.PreviewVideo.SuspendLayout();
            this.groupDisRegion.SuspendLayout();
            this.groupStreamMode.SuspendLayout();
            this.groupVideoPlay.SuspendLayout();
            this.groupMP.SuspendLayout();
            this.groupVBP.SuspendLayout();
            this.PreviewPicture.SuspendLayout();
            this.groupGetPic.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBoxPreviewPicture)).BeginInit();
            this.PreviewSound.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.trackBarPreviewVolume)).BeginInit();
            this.PreviewRecord.SuspendLayout();
            this.DeviceFunction.SuspendLayout();
            this.tabControl_PTZ.SuspendLayout();
            this.tabPage_PTZOpFun.SuspendLayout();
            this.groupBox_PTZFun.SuspendLayout();
            this.tabPageOPCamera.SuspendLayout();
            this.groupBoxPTZDir.SuspendLayout();
            this.groupBox_PTZOpCam.SuspendLayout();
            this.groupBox_PTZFocus.SuspendLayout();
            this.tabPagePreset.SuspendLayout();
            this.groupBox_PTZPreset.SuspendLayout();
            this.tabPagePTZOther.SuspendLayout();
            this.groupBox_PTZSit.SuspendLayout();
            this.groupBox_PTZArrange.SuspendLayout();
            this.groupBox_PTZAlarm.SuspendLayout();
            this.Playback.SuspendLayout();
            this.tabControl3.SuspendLayout();
            this.PlaybackRecord.SuspendLayout();
            this.groupBox22.SuspendLayout();
            this.groupBox21.SuspendLayout();
            this.groupBox20.SuspendLayout();
            this.PlaybackPlayback.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBoxPlaybackVideo)).BeginInit();
            this.groupBox19.SuspendLayout();
            this.groupBox18.SuspendLayout();
            this.PlaybackDownload.SuspendLayout();
            this.groupBox17.SuspendLayout();
            this.PlaybackSound.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.trackBarPlaybackVolume)).BeginInit();
            this.PlaybackPicture.SuspendLayout();
            this.groupBox16.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBoxPlaybackPicture)).BeginInit();
            this.SuspendLayout();
            // 
            // tabControl1
            // 
            this.tabControl1.Alignment = System.Windows.Forms.TabAlignment.Left;
            this.tabControl1.Controls.Add(this.General);
            this.tabControl1.Controls.Add(this.Device);
            this.tabControl1.Controls.Add(this.Alarm);
            this.tabControl1.Controls.Add(this.Preview);
            this.tabControl1.Controls.Add(this.DeviceFunction);
            this.tabControl1.Controls.Add(this.Playback);
            this.tabControl1.DrawMode = System.Windows.Forms.TabDrawMode.OwnerDrawFixed;
            this.tabControl1.ItemSize = new System.Drawing.Size(30, 120);
            this.tabControl1.Location = new System.Drawing.Point(12, 31);
            this.tabControl1.Multiline = true;
            this.tabControl1.Name = "tabControl1";
            this.tabControl1.SelectedIndex = 0;
            this.tabControl1.Size = new System.Drawing.Size(1045, 649);
            this.tabControl1.SizeMode = System.Windows.Forms.TabSizeMode.Fixed;
            this.tabControl1.TabIndex = 0;
            this.tabControl1.DrawItem += new System.Windows.Forms.DrawItemEventHandler(this.tabControl1_DrawItem);
            // 
            // General
            // 
            this.General.Controls.Add(this.groupBox_ClientInfo);
            this.General.Controls.Add(this.groupBox_LoginOP);
            this.General.Controls.Add(this.groupBox_ModPWD);
            this.General.Controls.Add(this.textResults);
            this.General.Controls.Add(this.label_Res);
            this.General.Controls.Add(this.btnLogout);
            this.General.Controls.Add(this.btnLogin);
            this.General.Controls.Add(this.textPassword);
            this.General.Controls.Add(this.label_PWD);
            this.General.Controls.Add(this.textUserName);
            this.General.Controls.Add(this.label_Name);
            this.General.Controls.Add(this.textServerPort);
            this.General.Controls.Add(this.label_Port);
            this.General.Controls.Add(this.textServerIP);
            this.General.Controls.Add(this.label_IP);
            this.General.Location = new System.Drawing.Point(124, 4);
            this.General.Name = "General";
            this.General.Padding = new System.Windows.Forms.Padding(3);
            this.General.Size = new System.Drawing.Size(917, 641);
            this.General.TabIndex = 0;
            this.General.Text = "General";
            this.General.UseVisualStyleBackColor = true;
            // 
            // groupBox_ClientInfo
            // 
            this.groupBox_ClientInfo.Controls.Add(this.btnClientVersion);
            this.groupBox_ClientInfo.Controls.Add(this.btnClientRoad);
            this.groupBox_ClientInfo.Controls.Add(this.btnQueryEmap);
            this.groupBox_ClientInfo.Controls.Add(this.btnQueryMenuRight);
            this.groupBox_ClientInfo.Controls.Add(this.btnSyncTime);
            this.groupBox_ClientInfo.Controls.Add(this.btnQueryFTP);
            this.groupBox_ClientInfo.Location = new System.Drawing.Point(9, 210);
            this.groupBox_ClientInfo.Name = "groupBox_ClientInfo";
            this.groupBox_ClientInfo.Size = new System.Drawing.Size(881, 58);
            this.groupBox_ClientInfo.TabIndex = 22;
            this.groupBox_ClientInfo.TabStop = false;
            this.groupBox_ClientInfo.Text = "Client Info";
            // 
            // btnClientVersion
            // 
            this.btnClientVersion.Location = new System.Drawing.Point(12, 20);
            this.btnClientVersion.Name = "btnClientVersion";
            this.btnClientVersion.Size = new System.Drawing.Size(108, 23);
            this.btnClientVersion.TabIndex = 11;
            this.btnClientVersion.Text = "Client Version";
            this.btnClientVersion.UseVisualStyleBackColor = true;
            this.btnClientVersion.Click += new System.EventHandler(this.btnClientVersion_Click);
            // 
            // btnClientRoad
            // 
            this.btnClientRoad.Location = new System.Drawing.Point(142, 20);
            this.btnClientRoad.Name = "btnClientRoad";
            this.btnClientRoad.Size = new System.Drawing.Size(102, 23);
            this.btnClientRoad.TabIndex = 12;
            this.btnClientRoad.Text = "Client Road";
            this.btnClientRoad.UseVisualStyleBackColor = true;
            this.btnClientRoad.Click += new System.EventHandler(this.btnClientRoad_Click);
            // 
            // btnQueryEmap
            // 
            this.btnQueryEmap.Location = new System.Drawing.Point(396, 20);
            this.btnQueryEmap.Name = "btnQueryEmap";
            this.btnQueryEmap.Size = new System.Drawing.Size(75, 23);
            this.btnQueryEmap.TabIndex = 13;
            this.btnQueryEmap.Text = "Query Emap";
            this.btnQueryEmap.UseVisualStyleBackColor = true;
            this.btnQueryEmap.Visible = false;
            this.btnQueryEmap.Click += new System.EventHandler(this.btnQueryEmap_Click);
            // 
            // btnQueryMenuRight
            // 
            this.btnQueryMenuRight.Location = new System.Drawing.Point(267, 20);
            this.btnQueryMenuRight.Name = "btnQueryMenuRight";
            this.btnQueryMenuRight.Size = new System.Drawing.Size(109, 23);
            this.btnQueryMenuRight.TabIndex = 15;
            this.btnQueryMenuRight.Text = "Query Menu Right";
            this.btnQueryMenuRight.UseVisualStyleBackColor = true;
            this.btnQueryMenuRight.Click += new System.EventHandler(this.btnQueryMenuRight_Click);
            // 
            // btnSyncTime
            // 
            this.btnSyncTime.Location = new System.Drawing.Point(592, 20);
            this.btnSyncTime.Name = "btnSyncTime";
            this.btnSyncTime.Size = new System.Drawing.Size(75, 23);
            this.btnSyncTime.TabIndex = 16;
            this.btnSyncTime.Text = "Sync Time";
            this.btnSyncTime.UseVisualStyleBackColor = true;
            this.btnSyncTime.Visible = false;
            this.btnSyncTime.Click += new System.EventHandler(this.btnSyncTime_Click);
            // 
            // btnQueryFTP
            // 
            this.btnQueryFTP.Location = new System.Drawing.Point(493, 20);
            this.btnQueryFTP.Name = "btnQueryFTP";
            this.btnQueryFTP.Size = new System.Drawing.Size(75, 23);
            this.btnQueryFTP.TabIndex = 14;
            this.btnQueryFTP.Text = "Query FTP";
            this.btnQueryFTP.UseVisualStyleBackColor = true;
            this.btnQueryFTP.Visible = false;
            this.btnQueryFTP.Click += new System.EventHandler(this.btnQueryFTP_Click);
            // 
            // groupBox_LoginOP
            // 
            this.groupBox_LoginOP.Controls.Add(this.checkBoxDoMain);
            this.groupBox_LoginOP.Controls.Add(this.checkBoxHttps);
            this.groupBox_LoginOP.Location = new System.Drawing.Point(9, 59);
            this.groupBox_LoginOP.Name = "groupBox_LoginOP";
            this.groupBox_LoginOP.Size = new System.Drawing.Size(881, 53);
            this.groupBox_LoginOP.TabIndex = 21;
            this.groupBox_LoginOP.TabStop = false;
            this.groupBox_LoginOP.Text = "Login Option";
            // 
            // checkBoxDoMain
            // 
            this.checkBoxDoMain.AutoSize = true;
            this.checkBoxDoMain.Location = new System.Drawing.Point(117, 20);
            this.checkBoxDoMain.Name = "checkBoxDoMain";
            this.checkBoxDoMain.Size = new System.Drawing.Size(60, 16);
            this.checkBoxDoMain.TabIndex = 0;
            this.checkBoxDoMain.Text = "doMain";
            this.checkBoxDoMain.UseVisualStyleBackColor = true;
            this.checkBoxDoMain.Visible = false;
            // 
            // checkBoxHttps
            // 
            this.checkBoxHttps.AutoSize = true;
            this.checkBoxHttps.Location = new System.Drawing.Point(14, 20);
            this.checkBoxHttps.Name = "checkBoxHttps";
            this.checkBoxHttps.Size = new System.Drawing.Size(54, 16);
            this.checkBoxHttps.TabIndex = 17;
            this.checkBoxHttps.Text = "https";
            this.checkBoxHttps.UseVisualStyleBackColor = true;
            this.checkBoxHttps.CheckedChanged += new System.EventHandler(this.checkBoxHttps_CheckedChanged);
            // 
            // groupBox_ModPWD
            // 
            this.groupBox_ModPWD.Controls.Add(this.textNewPassword);
            this.groupBox_ModPWD.Controls.Add(this.btnModifyPassword);
            this.groupBox_ModPWD.Controls.Add(this.label_NewPWD);
            this.groupBox_ModPWD.Location = new System.Drawing.Point(9, 132);
            this.groupBox_ModPWD.Name = "groupBox_ModPWD";
            this.groupBox_ModPWD.Size = new System.Drawing.Size(881, 59);
            this.groupBox_ModPWD.TabIndex = 20;
            this.groupBox_ModPWD.TabStop = false;
            this.groupBox_ModPWD.Text = "Modify Password";
            // 
            // textNewPassword
            // 
            this.textNewPassword.Location = new System.Drawing.Point(97, 23);
            this.textNewPassword.Name = "textNewPassword";
            this.textNewPassword.Size = new System.Drawing.Size(100, 21);
            this.textNewPassword.TabIndex = 2;
            // 
            // btnModifyPassword
            // 
            this.btnModifyPassword.Location = new System.Drawing.Point(267, 23);
            this.btnModifyPassword.Name = "btnModifyPassword";
            this.btnModifyPassword.Size = new System.Drawing.Size(107, 23);
            this.btnModifyPassword.TabIndex = 1;
            this.btnModifyPassword.Text = "Modify Password";
            this.btnModifyPassword.UseVisualStyleBackColor = true;
            this.btnModifyPassword.Click += new System.EventHandler(this.btnModifyPassword_Click);
            // 
            // label_NewPWD
            // 
            this.label_NewPWD.AutoSize = true;
            this.label_NewPWD.Location = new System.Drawing.Point(10, 26);
            this.label_NewPWD.Name = "label_NewPWD";
            this.label_NewPWD.Size = new System.Drawing.Size(83, 12);
            this.label_NewPWD.TabIndex = 0;
            this.label_NewPWD.Text = "New Password:";
            // 
            // textResults
            // 
            this.textResults.Location = new System.Drawing.Point(7, 305);
            this.textResults.Multiline = true;
            this.textResults.Name = "textResults";
            this.textResults.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.textResults.Size = new System.Drawing.Size(883, 248);
            this.textResults.TabIndex = 19;
            // 
            // label_Res
            // 
            this.label_Res.AutoSize = true;
            this.label_Res.Location = new System.Drawing.Point(7, 286);
            this.label_Res.Name = "label_Res";
            this.label_Res.Size = new System.Drawing.Size(95, 12);
            this.label_Res.TabIndex = 18;
            this.label_Res.Text = "Handle Results:";
            // 
            // btnLogout
            // 
            this.btnLogout.Location = new System.Drawing.Point(711, 15);
            this.btnLogout.Name = "btnLogout";
            this.btnLogout.Size = new System.Drawing.Size(75, 23);
            this.btnLogout.TabIndex = 10;
            this.btnLogout.Text = "LogOut";
            this.btnLogout.UseVisualStyleBackColor = true;
            this.btnLogout.Click += new System.EventHandler(this.btnLogout_Click);
            // 
            // btnLogin
            // 
            this.btnLogin.Location = new System.Drawing.Point(601, 15);
            this.btnLogin.Name = "btnLogin";
            this.btnLogin.Size = new System.Drawing.Size(75, 23);
            this.btnLogin.TabIndex = 9;
            this.btnLogin.Text = "Login";
            this.btnLogin.UseVisualStyleBackColor = true;
            this.btnLogin.Click += new System.EventHandler(this.btnLogin_Click);
            // 
            // textPassword
            // 
            this.textPassword.Location = new System.Drawing.Point(503, 15);
            this.textPassword.Name = "textPassword";
            this.textPassword.Size = new System.Drawing.Size(57, 21);
            this.textPassword.TabIndex = 8;
            this.textPassword.Text = "admin123";
            // 
            // label_PWD
            // 
            this.label_PWD.AutoSize = true;
            this.label_PWD.Location = new System.Drawing.Point(437, 18);
            this.label_PWD.Name = "label_PWD";
            this.label_PWD.Size = new System.Drawing.Size(59, 12);
            this.label_PWD.TabIndex = 7;
            this.label_PWD.Text = "Password:";
            // 
            // textUserName
            // 
            this.textUserName.Location = new System.Drawing.Point(356, 15);
            this.textUserName.Name = "textUserName";
            this.textUserName.Size = new System.Drawing.Size(54, 21);
            this.textUserName.TabIndex = 6;
            this.textUserName.Text = "system";
            // 
            // label_Name
            // 
            this.label_Name.AutoSize = true;
            this.label_Name.Location = new System.Drawing.Point(290, 18);
            this.label_Name.Name = "label_Name";
            this.label_Name.Size = new System.Drawing.Size(59, 12);
            this.label_Name.TabIndex = 5;
            this.label_Name.Text = "UserName:";
            // 
            // textServerPort
            // 
            this.textServerPort.Location = new System.Drawing.Point(223, 16);
            this.textServerPort.Name = "textServerPort";
            this.textServerPort.Size = new System.Drawing.Size(40, 21);
            this.textServerPort.TabIndex = 4;
            this.textServerPort.Text = "8314";
            // 
            // label_Port
            // 
            this.label_Port.AutoSize = true;
            this.label_Port.Location = new System.Drawing.Point(182, 18);
            this.label_Port.Name = "label_Port";
            this.label_Port.Size = new System.Drawing.Size(35, 12);
            this.label_Port.TabIndex = 3;
            this.label_Port.Text = "Port:";
            // 
            // textServerIP
            // 
            this.textServerIP.Location = new System.Drawing.Point(79, 16);
            this.textServerIP.Name = "textServerIP";
            this.textServerIP.Size = new System.Drawing.Size(87, 21);
            this.textServerIP.TabIndex = 2;
            this.textServerIP.Text = "172.25.13.186";
            // 
            // label_IP
            // 
            this.label_IP.AutoSize = true;
            this.label_IP.Location = new System.Drawing.Point(12, 18);
            this.label_IP.Name = "label_IP";
            this.label_IP.Size = new System.Drawing.Size(65, 12);
            this.label_IP.TabIndex = 1;
            this.label_IP.Text = "Server IP:";
            // 
            // Device
            // 
            this.Device.Controls.Add(this.groupBox_UserData);
            this.Device.Controls.Add(this.treeViewDeviceOrg);
            this.Device.Controls.Add(this.radioButtonDeviceChannel);
            this.Device.Controls.Add(this.radioButtonDeviceDevice);
            this.Device.Controls.Add(this.radioButtonDeviceOrg);
            this.Device.Controls.Add(this.textDeviceOrgData);
            this.Device.Controls.Add(this.groupBox_OrgParam);
            this.Device.Location = new System.Drawing.Point(124, 4);
            this.Device.Name = "Device";
            this.Device.Padding = new System.Windows.Forms.Padding(3);
            this.Device.Size = new System.Drawing.Size(917, 641);
            this.Device.TabIndex = 1;
            this.Device.Text = "Device";
            this.Device.UseVisualStyleBackColor = true;
            // 
            // groupBox_UserData
            // 
            this.groupBox_UserData.Controls.Add(this.label_UserDataInfo);
            this.groupBox_UserData.Controls.Add(this.textDeviceUserDataInfo);
            this.groupBox_UserData.Controls.Add(this.btnDeviceDelUserData);
            this.groupBox_UserData.Controls.Add(this.label_UserDataName);
            this.groupBox_UserData.Controls.Add(this.textDeviceUserDataName);
            this.groupBox_UserData.Controls.Add(this.btnDeviceGetUserData);
            this.groupBox_UserData.Controls.Add(this.btnDeviceSaveUserData);
            this.groupBox_UserData.Controls.Add(this.button50);
            this.groupBox_UserData.Location = new System.Drawing.Point(7, 121);
            this.groupBox_UserData.Name = "groupBox_UserData";
            this.groupBox_UserData.Size = new System.Drawing.Size(904, 69);
            this.groupBox_UserData.TabIndex = 16;
            this.groupBox_UserData.TabStop = false;
            this.groupBox_UserData.Text = "User Data";
            // 
            // label_UserDataInfo
            // 
            this.label_UserDataInfo.AutoSize = true;
            this.label_UserDataInfo.Location = new System.Drawing.Point(342, 19);
            this.label_UserDataInfo.Name = "label_UserDataInfo";
            this.label_UserDataInfo.Size = new System.Drawing.Size(95, 12);
            this.label_UserDataInfo.TabIndex = 11;
            this.label_UserDataInfo.Text = "User Data Info:";
            // 
            // textDeviceUserDataInfo
            // 
            this.textDeviceUserDataInfo.Location = new System.Drawing.Point(437, 15);
            this.textDeviceUserDataInfo.Name = "textDeviceUserDataInfo";
            this.textDeviceUserDataInfo.Size = new System.Drawing.Size(325, 21);
            this.textDeviceUserDataInfo.TabIndex = 12;
            // 
            // btnDeviceDelUserData
            // 
            this.btnDeviceDelUserData.Location = new System.Drawing.Point(260, 41);
            this.btnDeviceDelUserData.Name = "btnDeviceDelUserData";
            this.btnDeviceDelUserData.Size = new System.Drawing.Size(96, 23);
            this.btnDeviceDelUserData.TabIndex = 9;
            this.btnDeviceDelUserData.Text = "Del User Data";
            this.btnDeviceDelUserData.UseVisualStyleBackColor = true;
            this.btnDeviceDelUserData.Click += new System.EventHandler(this.btnDeviceDelUserData_Click);
            // 
            // label_UserDataName
            // 
            this.label_UserDataName.AutoSize = true;
            this.label_UserDataName.Location = new System.Drawing.Point(7, 19);
            this.label_UserDataName.Name = "label_UserDataName";
            this.label_UserDataName.Size = new System.Drawing.Size(95, 12);
            this.label_UserDataName.TabIndex = 2;
            this.label_UserDataName.Text = "User Data Name:";
            // 
            // textDeviceUserDataName
            // 
            this.textDeviceUserDataName.Location = new System.Drawing.Point(102, 15);
            this.textDeviceUserDataName.Name = "textDeviceUserDataName";
            this.textDeviceUserDataName.Size = new System.Drawing.Size(225, 21);
            this.textDeviceUserDataName.TabIndex = 3;
            // 
            // btnDeviceGetUserData
            // 
            this.btnDeviceGetUserData.Location = new System.Drawing.Point(9, 40);
            this.btnDeviceGetUserData.Name = "btnDeviceGetUserData";
            this.btnDeviceGetUserData.Size = new System.Drawing.Size(96, 23);
            this.btnDeviceGetUserData.TabIndex = 7;
            this.btnDeviceGetUserData.Text = "Get User Data";
            this.btnDeviceGetUserData.UseVisualStyleBackColor = true;
            this.btnDeviceGetUserData.Click += new System.EventHandler(this.btnDeviceGetUserData_Click);
            // 
            // btnDeviceSaveUserData
            // 
            this.btnDeviceSaveUserData.Location = new System.Drawing.Point(131, 41);
            this.btnDeviceSaveUserData.Name = "btnDeviceSaveUserData";
            this.btnDeviceSaveUserData.Size = new System.Drawing.Size(100, 23);
            this.btnDeviceSaveUserData.TabIndex = 8;
            this.btnDeviceSaveUserData.Text = "Save User Data";
            this.btnDeviceSaveUserData.UseVisualStyleBackColor = true;
            this.btnDeviceSaveUserData.Click += new System.EventHandler(this.btnDeviceSaveUserData_Click);
            // 
            // button50
            // 
            this.button50.Location = new System.Drawing.Point(385, 41);
            this.button50.Name = "button50";
            this.button50.Size = new System.Drawing.Size(96, 23);
            this.button50.TabIndex = 10;
            this.button50.Text = "Query Map Org";
            this.button50.UseVisualStyleBackColor = true;
            this.button50.Visible = false;
            // 
            // treeViewDeviceOrg
            // 
            this.treeViewDeviceOrg.Location = new System.Drawing.Point(712, 215);
            this.treeViewDeviceOrg.Name = "treeViewDeviceOrg";
            this.treeViewDeviceOrg.Size = new System.Drawing.Size(202, 418);
            this.treeViewDeviceOrg.TabIndex = 15;
            // 
            // radioButtonDeviceChannel
            // 
            this.radioButtonDeviceChannel.AutoSize = true;
            this.radioButtonDeviceChannel.Location = new System.Drawing.Point(841, 196);
            this.radioButtonDeviceChannel.Name = "radioButtonDeviceChannel";
            this.radioButtonDeviceChannel.Size = new System.Drawing.Size(65, 16);
            this.radioButtonDeviceChannel.TabIndex = 14;
            this.radioButtonDeviceChannel.Text = "Channel";
            this.radioButtonDeviceChannel.UseVisualStyleBackColor = true;
            this.radioButtonDeviceChannel.CheckedChanged += new System.EventHandler(this.radioButtonDeviceChannel_CheckedChanged);
            // 
            // radioButtonDeviceDevice
            // 
            this.radioButtonDeviceDevice.AutoSize = true;
            this.radioButtonDeviceDevice.Location = new System.Drawing.Point(768, 196);
            this.radioButtonDeviceDevice.Name = "radioButtonDeviceDevice";
            this.radioButtonDeviceDevice.Size = new System.Drawing.Size(59, 16);
            this.radioButtonDeviceDevice.TabIndex = 13;
            this.radioButtonDeviceDevice.Text = "Device";
            this.radioButtonDeviceDevice.UseVisualStyleBackColor = true;
            this.radioButtonDeviceDevice.CheckedChanged += new System.EventHandler(this.radioButtonDeviceDevice_CheckedChanged);
            // 
            // radioButtonDeviceOrg
            // 
            this.radioButtonDeviceOrg.AutoSize = true;
            this.radioButtonDeviceOrg.Checked = true;
            this.radioButtonDeviceOrg.Location = new System.Drawing.Point(712, 196);
            this.radioButtonDeviceOrg.Name = "radioButtonDeviceOrg";
            this.radioButtonDeviceOrg.Size = new System.Drawing.Size(41, 16);
            this.radioButtonDeviceOrg.TabIndex = 12;
            this.radioButtonDeviceOrg.TabStop = true;
            this.radioButtonDeviceOrg.Text = "Org";
            this.radioButtonDeviceOrg.UseVisualStyleBackColor = true;
            this.radioButtonDeviceOrg.CheckedChanged += new System.EventHandler(this.radioButtonDeviceOrg_CheckedChanged);
            // 
            // textDeviceOrgData
            // 
            this.textDeviceOrgData.Location = new System.Drawing.Point(7, 196);
            this.textDeviceOrgData.Multiline = true;
            this.textDeviceOrgData.Name = "textDeviceOrgData";
            this.textDeviceOrgData.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.textDeviceOrgData.Size = new System.Drawing.Size(698, 437);
            this.textDeviceOrgData.TabIndex = 11;
            // 
            // groupBox_OrgParam
            // 
            this.groupBox_OrgParam.Controls.Add(this.checkBox20);
            this.groupBox_OrgParam.Controls.Add(this.checkBox19);
            this.groupBox_OrgParam.Controls.Add(this.checkBox18);
            this.groupBox_OrgParam.Controls.Add(this.checkBox17);
            this.groupBox_OrgParam.Controls.Add(this.checkBox16);
            this.groupBox_OrgParam.Controls.Add(this.checkBox15);
            this.groupBox_OrgParam.Controls.Add(this.checkBox14);
            this.groupBox_OrgParam.Controls.Add(this.checkBox13);
            this.groupBox_OrgParam.Controls.Add(this.checkBox12);
            this.groupBox_OrgParam.Controls.Add(this.checkBox11);
            this.groupBox_OrgParam.Controls.Add(this.checkBox10);
            this.groupBox_OrgParam.Controls.Add(this.checkBox9);
            this.groupBox_OrgParam.Controls.Add(this.checkBox8);
            this.groupBox_OrgParam.Controls.Add(this.btnDeviceQueryByLayered);
            this.groupBox_OrgParam.Controls.Add(this.btnDeviceQueryOrg);
            this.groupBox_OrgParam.Controls.Add(this.checkBoxDeviceCompress);
            this.groupBox_OrgParam.Controls.Add(this.checkBoxDeviceContainDevice);
            this.groupBox_OrgParam.Controls.Add(this.checkBox5);
            this.groupBox_OrgParam.Controls.Add(this.textDeviceOrgCode);
            this.groupBox_OrgParam.Controls.Add(this.label_Org);
            this.groupBox_OrgParam.Location = new System.Drawing.Point(7, 6);
            this.groupBox_OrgParam.Name = "groupBox_OrgParam";
            this.groupBox_OrgParam.Size = new System.Drawing.Size(904, 107);
            this.groupBox_OrgParam.TabIndex = 0;
            this.groupBox_OrgParam.TabStop = false;
            this.groupBox_OrgParam.Text = "Query Organization Param";
            // 
            // checkBox20
            // 
            this.checkBox20.AutoSize = true;
            this.checkBox20.Location = new System.Drawing.Point(9, 85);
            this.checkBox20.Name = "checkBox20";
            this.checkBox20.Size = new System.Drawing.Size(114, 16);
            this.checkBox20.TabIndex = 19;
            this.checkBox20.Text = "Virtual Channel";
            this.checkBox20.UseVisualStyleBackColor = true;
            this.checkBox20.Visible = false;
            // 
            // checkBox19
            // 
            this.checkBox19.AutoSize = true;
            this.checkBox19.Location = new System.Drawing.Point(679, 65);
            this.checkBox19.Name = "checkBox19";
            this.checkBox19.Size = new System.Drawing.Size(90, 16);
            this.checkBox19.TabIndex = 18;
            this.checkBox19.Text = "LED Channel";
            this.checkBox19.UseVisualStyleBackColor = true;
            this.checkBox19.Visible = false;
            // 
            // checkBox18
            // 
            this.checkBox18.AutoSize = true;
            this.checkBox18.Location = new System.Drawing.Point(531, 64);
            this.checkBox18.Name = "checkBox18";
            this.checkBox18.Size = new System.Drawing.Size(120, 16);
            this.checkBox18.TabIndex = 17;
            this.checkBox18.Text = "RoadGate Channel";
            this.checkBox18.UseVisualStyleBackColor = true;
            this.checkBox18.Visible = false;
            // 
            // checkBox17
            // 
            this.checkBox17.AutoSize = true;
            this.checkBox17.Location = new System.Drawing.Point(414, 65);
            this.checkBox17.Name = "checkBox17";
            this.checkBox17.Size = new System.Drawing.Size(90, 16);
            this.checkBox17.TabIndex = 16;
            this.checkBox17.Text = "Pos Channel";
            this.checkBox17.UseVisualStyleBackColor = true;
            this.checkBox17.Visible = false;
            // 
            // checkBox16
            // 
            this.checkBox16.AutoSize = true;
            this.checkBox16.Location = new System.Drawing.Point(285, 65);
            this.checkBox16.Name = "checkBox16";
            this.checkBox16.Size = new System.Drawing.Size(102, 16);
            this.checkBox16.TabIndex = 15;
            this.checkBox16.Text = "Voice Channel";
            this.checkBox16.UseVisualStyleBackColor = true;
            this.checkBox16.Visible = false;
            // 
            // checkBox15
            // 
            this.checkBox15.AutoSize = true;
            this.checkBox15.Location = new System.Drawing.Point(162, 64);
            this.checkBox15.Name = "checkBox15";
            this.checkBox15.Size = new System.Drawing.Size(96, 16);
            this.checkBox15.TabIndex = 14;
            this.checkBox15.Text = "Door Channel";
            this.checkBox15.UseVisualStyleBackColor = true;
            this.checkBox15.Visible = false;
            // 
            // checkBox14
            // 
            this.checkBox14.AutoSize = true;
            this.checkBox14.Location = new System.Drawing.Point(9, 65);
            this.checkBox14.Name = "checkBox14";
            this.checkBox14.Size = new System.Drawing.Size(126, 16);
            this.checkBox14.TabIndex = 13;
            this.checkBox14.Text = "TvWallOut Channel";
            this.checkBox14.UseVisualStyleBackColor = true;
            this.checkBox14.Visible = false;
            // 
            // checkBox13
            // 
            this.checkBox13.AutoSize = true;
            this.checkBox13.Location = new System.Drawing.Point(650, 45);
            this.checkBox13.Name = "checkBox13";
            this.checkBox13.Size = new System.Drawing.Size(120, 16);
            this.checkBox13.TabIndex = 12;
            this.checkBox13.Text = "TvWallIn Channel";
            this.checkBox13.UseVisualStyleBackColor = true;
            this.checkBox13.Visible = false;
            // 
            // checkBox12
            // 
            this.checkBox12.AutoSize = true;
            this.checkBox12.Location = new System.Drawing.Point(495, 45);
            this.checkBox12.Name = "checkBox12";
            this.checkBox12.Size = new System.Drawing.Size(120, 16);
            this.checkBox12.TabIndex = 11;
            this.checkBox12.Text = "AlarmOut Channel";
            this.checkBox12.UseVisualStyleBackColor = true;
            this.checkBox12.Visible = false;
            // 
            // checkBox11
            // 
            this.checkBox11.AutoSize = true;
            this.checkBox11.Location = new System.Drawing.Point(347, 45);
            this.checkBox11.Name = "checkBox11";
            this.checkBox11.Size = new System.Drawing.Size(114, 16);
            this.checkBox11.TabIndex = 10;
            this.checkBox11.Text = "AlarmIn Channel";
            this.checkBox11.UseVisualStyleBackColor = true;
            this.checkBox11.Visible = false;
            // 
            // checkBox10
            // 
            this.checkBox10.AutoSize = true;
            this.checkBox10.Location = new System.Drawing.Point(230, 45);
            this.checkBox10.Name = "checkBox10";
            this.checkBox10.Size = new System.Drawing.Size(90, 16);
            this.checkBox10.TabIndex = 9;
            this.checkBox10.Text = "Dec Channel";
            this.checkBox10.UseVisualStyleBackColor = true;
            this.checkBox10.Visible = false;
            // 
            // checkBox9
            // 
            this.checkBox9.AutoSize = true;
            this.checkBox9.Location = new System.Drawing.Point(115, 45);
            this.checkBox9.Name = "checkBox9";
            this.checkBox9.Size = new System.Drawing.Size(90, 16);
            this.checkBox9.TabIndex = 8;
            this.checkBox9.Text = "Enc Channel";
            this.checkBox9.UseVisualStyleBackColor = true;
            this.checkBox9.Visible = false;
            // 
            // checkBox8
            // 
            this.checkBox8.AutoSize = true;
            this.checkBox8.Location = new System.Drawing.Point(9, 45);
            this.checkBox8.Name = "checkBox8";
            this.checkBox8.Size = new System.Drawing.Size(78, 16);
            this.checkBox8.TabIndex = 7;
            this.checkBox8.Text = "All Check";
            this.checkBox8.UseVisualStyleBackColor = true;
            this.checkBox8.Visible = false;
            // 
            // btnDeviceQueryByLayered
            // 
            this.btnDeviceQueryByLayered.Location = new System.Drawing.Point(513, 16);
            this.btnDeviceQueryByLayered.Name = "btnDeviceQueryByLayered";
            this.btnDeviceQueryByLayered.Size = new System.Drawing.Size(111, 23);
            this.btnDeviceQueryByLayered.TabIndex = 6;
            this.btnDeviceQueryByLayered.Text = "Query By Layered";
            this.btnDeviceQueryByLayered.UseVisualStyleBackColor = true;
            this.btnDeviceQueryByLayered.Click += new System.EventHandler(this.btnDeviceQueryByLayered_Click);
            // 
            // btnDeviceQueryOrg
            // 
            this.btnDeviceQueryOrg.Location = new System.Drawing.Point(414, 16);
            this.btnDeviceQueryOrg.Name = "btnDeviceQueryOrg";
            this.btnDeviceQueryOrg.Size = new System.Drawing.Size(75, 23);
            this.btnDeviceQueryOrg.TabIndex = 5;
            this.btnDeviceQueryOrg.Text = "Query Org";
            this.btnDeviceQueryOrg.UseVisualStyleBackColor = true;
            this.btnDeviceQueryOrg.Visible = false;
            this.btnDeviceQueryOrg.Click += new System.EventHandler(this.btnDeviceQueryOrg_Click);
            // 
            // checkBoxDeviceCompress
            // 
            this.checkBoxDeviceCompress.AutoSize = true;
            this.checkBoxDeviceCompress.Checked = true;
            this.checkBoxDeviceCompress.CheckState = System.Windows.Forms.CheckState.Checked;
            this.checkBoxDeviceCompress.Location = new System.Drawing.Point(327, 20);
            this.checkBoxDeviceCompress.Name = "checkBoxDeviceCompress";
            this.checkBoxDeviceCompress.Size = new System.Drawing.Size(72, 16);
            this.checkBoxDeviceCompress.TabIndex = 4;
            this.checkBoxDeviceCompress.Text = "Compress";
            this.checkBoxDeviceCompress.UseVisualStyleBackColor = true;
            this.checkBoxDeviceCompress.CheckedChanged += new System.EventHandler(this.checkBoxDeviceCompress_CheckedChanged);
            // 
            // checkBoxDeviceContainDevice
            // 
            this.checkBoxDeviceContainDevice.AutoSize = true;
            this.checkBoxDeviceContainDevice.Checked = true;
            this.checkBoxDeviceContainDevice.CheckState = System.Windows.Forms.CheckState.Checked;
            this.checkBoxDeviceContainDevice.Location = new System.Drawing.Point(187, 20);
            this.checkBoxDeviceContainDevice.Name = "checkBoxDeviceContainDevice";
            this.checkBoxDeviceContainDevice.Size = new System.Drawing.Size(108, 16);
            this.checkBoxDeviceContainDevice.TabIndex = 3;
            this.checkBoxDeviceContainDevice.Text = "Contain Device";
            this.checkBoxDeviceContainDevice.UseVisualStyleBackColor = true;
            // 
            // checkBox5
            // 
            this.checkBox5.AutoSize = true;
            this.checkBox5.Location = new System.Drawing.Point(650, 20);
            this.checkBox5.Name = "checkBox5";
            this.checkBox5.Size = new System.Drawing.Size(132, 16);
            this.checkBox5.TabIndex = 2;
            this.checkBox5.Text = "Query All Sub Code";
            this.checkBox5.UseVisualStyleBackColor = true;
            this.checkBox5.Visible = false;
            // 
            // textDeviceOrgCode
            // 
            this.textDeviceOrgCode.Location = new System.Drawing.Point(69, 18);
            this.textDeviceOrgCode.Name = "textDeviceOrgCode";
            this.textDeviceOrgCode.Size = new System.Drawing.Size(100, 21);
            this.textDeviceOrgCode.TabIndex = 1;
            // 
            // label_Org
            // 
            this.label_Org.AutoSize = true;
            this.label_Org.Location = new System.Drawing.Point(7, 21);
            this.label_Org.Name = "label_Org";
            this.label_Org.Size = new System.Drawing.Size(59, 12);
            this.label_Org.TabIndex = 0;
            this.label_Org.Text = "Org Code:";
            // 
            // Alarm
            // 
            this.Alarm.Controls.Add(this.tabAlarm);
            this.Alarm.Controls.Add(this.btnAlarmClear);
            this.Alarm.Controls.Add(this.checkBoxAlarmStopRefresh);
            this.Alarm.Controls.Add(this.checkBoxAlarmForbiddenNotify);
            this.Alarm.Controls.Add(this.textAlarmInfo);
            this.Alarm.Location = new System.Drawing.Point(124, 4);
            this.Alarm.Name = "Alarm";
            this.Alarm.Size = new System.Drawing.Size(917, 641);
            this.Alarm.TabIndex = 2;
            this.Alarm.Text = "Alarm";
            this.Alarm.UseVisualStyleBackColor = true;
            // 
            // tabAlarm
            // 
            this.tabAlarm.Controls.Add(this.tabAlarmQuery);
            this.tabAlarm.Controls.Add(this.tabAlarmExport);
            this.tabAlarm.Controls.Add(this.tabAlarmConfirm);
            this.tabAlarm.Location = new System.Drawing.Point(17, 13);
            this.tabAlarm.Name = "tabAlarm";
            this.tabAlarm.SelectedIndex = 0;
            this.tabAlarm.Size = new System.Drawing.Size(887, 263);
            this.tabAlarm.TabIndex = 112;
            this.tabAlarm.Tag = "";
            // 
            // tabAlarmQuery
            // 
            this.tabAlarmQuery.Controls.Add(this.btnAlarmCountQuery);
            this.tabAlarmQuery.Controls.Add(this.textAlarmTypeQueryLanguage);
            this.tabAlarmQuery.Controls.Add(this.btnAlarmTypeQuery);
            this.tabAlarmQuery.Controls.Add(this.label_QueryLan);
            this.tabAlarmQuery.Controls.Add(this.btnAlarmQuery);
            this.tabAlarmQuery.Controls.Add(this.label_QuerySO);
            this.tabAlarmQuery.Controls.Add(this.label_QueryABT);
            this.tabAlarmQuery.Controls.Add(this.textAlarmQueryBeginTime);
            this.tabAlarmQuery.Controls.Add(this.label_QueryAET);
            this.tabAlarmQuery.Controls.Add(this.textAlarmQueryEndTime);
            this.tabAlarmQuery.Controls.Add(this.label_QueryHBT);
            this.tabAlarmQuery.Controls.Add(this.textAlarmQueryHandleBeginTime);
            this.tabAlarmQuery.Controls.Add(this.label_QueryHET);
            this.tabAlarmQuery.Controls.Add(this.textAlarmQueryHandleEndTime);
            this.tabAlarmQuery.Controls.Add(this.label_QueryDevID);
            this.tabAlarmQuery.Controls.Add(this.textAlarmQueryDeviceId);
            this.tabAlarmQuery.Controls.Add(this.label_QueryChnlID);
            this.tabAlarmQuery.Controls.Add(this.textAlarmQueryChannelId);
            this.tabAlarmQuery.Controls.Add(this.label_QueryOrgID);
            this.tabAlarmQuery.Controls.Add(this.textAlarmQueryOrgId);
            this.tabAlarmQuery.Controls.Add(this.label_QueryAlarmID);
            this.tabAlarmQuery.Controls.Add(this.textAlarmQueryAlarmId);
            this.tabAlarmQuery.Controls.Add(this.label_QueryAlarmCode);
            this.tabAlarmQuery.Controls.Add(this.textAlarmQueryAlarmCode);
            this.tabAlarmQuery.Controls.Add(this.label_QueryHU);
            this.tabAlarmQuery.Controls.Add(this.textAlarmQueryHandleUser);
            this.tabAlarmQuery.Controls.Add(this.label_QueryST);
            this.tabAlarmQuery.Controls.Add(this.comboBoxAlarmQuerySortType);
            this.tabAlarmQuery.Controls.Add(this.label_QueryPZ);
            this.tabAlarmQuery.Controls.Add(this.textAlarmQueryPageSize);
            this.tabAlarmQuery.Controls.Add(this.label_QueryPN);
            this.tabAlarmQuery.Controls.Add(this.textAlarmQueryPageNum);
            this.tabAlarmQuery.Controls.Add(this.comboBoxAlarmQuerySortOrder);
            this.tabAlarmQuery.Controls.Add(this.label_QueryATL);
            this.tabAlarmQuery.Controls.Add(this.textAlarmQueryTypeList);
            this.tabAlarmQuery.Controls.Add(this.label_QueryASL);
            this.tabAlarmQuery.Controls.Add(this.textAlarmQueryStatusList);
            this.tabAlarmQuery.Controls.Add(this.label_QueryAGL);
            this.tabAlarmQuery.Controls.Add(this.textAlarmQueryGradeList);
            this.tabAlarmQuery.Controls.Add(this.label_QueryHSL);
            this.tabAlarmQuery.Controls.Add(this.textAlarmQueryHandleStatusList);
            this.tabAlarmQuery.Location = new System.Drawing.Point(4, 22);
            this.tabAlarmQuery.Name = "tabAlarmQuery";
            this.tabAlarmQuery.Padding = new System.Windows.Forms.Padding(3);
            this.tabAlarmQuery.Size = new System.Drawing.Size(879, 237);
            this.tabAlarmQuery.TabIndex = 0;
            this.tabAlarmQuery.Text = "Query";
            this.tabAlarmQuery.UseVisualStyleBackColor = true;
            // 
            // btnAlarmCountQuery
            // 
            this.btnAlarmCountQuery.Location = new System.Drawing.Point(8, 120);
            this.btnAlarmCountQuery.Name = "btnAlarmCountQuery";
            this.btnAlarmCountQuery.Size = new System.Drawing.Size(114, 30);
            this.btnAlarmCountQuery.TabIndex = 55;
            this.btnAlarmCountQuery.Text = "AlarmCountQuery";
            this.btnAlarmCountQuery.UseVisualStyleBackColor = true;
            this.btnAlarmCountQuery.Click += new System.EventHandler(this.btnAlarmCountQuery_Click);
            // 
            // textAlarmTypeQueryLanguage
            // 
            this.textAlarmTypeQueryLanguage.Location = new System.Drawing.Point(10, 208);
            this.textAlarmTypeQueryLanguage.Name = "textAlarmTypeQueryLanguage";
            this.textAlarmTypeQueryLanguage.Size = new System.Drawing.Size(53, 21);
            this.textAlarmTypeQueryLanguage.TabIndex = 111;
            this.textAlarmTypeQueryLanguage.Text = "en_Us";
            // 
            // btnAlarmTypeQuery
            // 
            this.btnAlarmTypeQuery.Location = new System.Drawing.Point(81, 202);
            this.btnAlarmTypeQuery.Name = "btnAlarmTypeQuery";
            this.btnAlarmTypeQuery.Size = new System.Drawing.Size(114, 30);
            this.btnAlarmTypeQuery.TabIndex = 54;
            this.btnAlarmTypeQuery.Text = "AlarmTypeQuery";
            this.btnAlarmTypeQuery.UseVisualStyleBackColor = true;
            this.btnAlarmTypeQuery.Click += new System.EventHandler(this.btnAlarmTypeQuery_Click);
            // 
            // label_QueryLan
            // 
            this.label_QueryLan.AutoSize = true;
            this.label_QueryLan.Location = new System.Drawing.Point(10, 192);
            this.label_QueryLan.Name = "label_QueryLan";
            this.label_QueryLan.Size = new System.Drawing.Size(53, 12);
            this.label_QueryLan.TabIndex = 110;
            this.label_QueryLan.Text = "Language";
            // 
            // btnAlarmQuery
            // 
            this.btnAlarmQuery.Location = new System.Drawing.Point(143, 120);
            this.btnAlarmQuery.Name = "btnAlarmQuery";
            this.btnAlarmQuery.Size = new System.Drawing.Size(98, 30);
            this.btnAlarmQuery.TabIndex = 53;
            this.btnAlarmQuery.Text = "AlarmQuery";
            this.btnAlarmQuery.UseVisualStyleBackColor = true;
            this.btnAlarmQuery.Click += new System.EventHandler(this.btnAlarmQuery_Click);
            // 
            // label_QuerySO
            // 
            this.label_QuerySO.AutoSize = true;
            this.label_QuerySO.Location = new System.Drawing.Point(548, 67);
            this.label_QuerySO.Name = "label_QuerySO";
            this.label_QuerySO.Size = new System.Drawing.Size(59, 12);
            this.label_QuerySO.TabIndex = 43;
            this.label_QuerySO.Text = "SortOrder";
            // 
            // label_QueryABT
            // 
            this.label_QueryABT.AutoSize = true;
            this.label_QueryABT.Location = new System.Drawing.Point(10, 24);
            this.label_QueryABT.Name = "label_QueryABT";
            this.label_QueryABT.Size = new System.Drawing.Size(89, 12);
            this.label_QueryABT.TabIndex = 15;
            this.label_QueryABT.Text = "AlarmBeginTime";
            // 
            // textAlarmQueryBeginTime
            // 
            this.textAlarmQueryBeginTime.Location = new System.Drawing.Point(10, 40);
            this.textAlarmQueryBeginTime.Name = "textAlarmQueryBeginTime";
            this.textAlarmQueryBeginTime.Size = new System.Drawing.Size(89, 21);
            this.textAlarmQueryBeginTime.TabIndex = 16;
            // 
            // label_QueryAET
            // 
            this.label_QueryAET.AutoSize = true;
            this.label_QueryAET.Location = new System.Drawing.Point(10, 67);
            this.label_QueryAET.Name = "label_QueryAET";
            this.label_QueryAET.Size = new System.Drawing.Size(77, 12);
            this.label_QueryAET.TabIndex = 17;
            this.label_QueryAET.Text = "AlarmEndTime";
            // 
            // textAlarmQueryEndTime
            // 
            this.textAlarmQueryEndTime.Location = new System.Drawing.Point(10, 83);
            this.textAlarmQueryEndTime.Name = "textAlarmQueryEndTime";
            this.textAlarmQueryEndTime.Size = new System.Drawing.Size(89, 21);
            this.textAlarmQueryEndTime.TabIndex = 18;
            // 
            // label_QueryHBT
            // 
            this.label_QueryHBT.AutoSize = true;
            this.label_QueryHBT.Location = new System.Drawing.Point(106, 24);
            this.label_QueryHBT.Name = "label_QueryHBT";
            this.label_QueryHBT.Size = new System.Drawing.Size(95, 12);
            this.label_QueryHBT.TabIndex = 19;
            this.label_QueryHBT.Text = "HandleBeginTime";
            // 
            // textAlarmQueryHandleBeginTime
            // 
            this.textAlarmQueryHandleBeginTime.Location = new System.Drawing.Point(106, 40);
            this.textAlarmQueryHandleBeginTime.Name = "textAlarmQueryHandleBeginTime";
            this.textAlarmQueryHandleBeginTime.Size = new System.Drawing.Size(89, 21);
            this.textAlarmQueryHandleBeginTime.TabIndex = 20;
            // 
            // label_QueryHET
            // 
            this.label_QueryHET.AutoSize = true;
            this.label_QueryHET.Location = new System.Drawing.Point(106, 67);
            this.label_QueryHET.Name = "label_QueryHET";
            this.label_QueryHET.Size = new System.Drawing.Size(83, 12);
            this.label_QueryHET.TabIndex = 21;
            this.label_QueryHET.Text = "HandleEndTime";
            // 
            // textAlarmQueryHandleEndTime
            // 
            this.textAlarmQueryHandleEndTime.Location = new System.Drawing.Point(106, 83);
            this.textAlarmQueryHandleEndTime.Name = "textAlarmQueryHandleEndTime";
            this.textAlarmQueryHandleEndTime.Size = new System.Drawing.Size(89, 21);
            this.textAlarmQueryHandleEndTime.TabIndex = 22;
            // 
            // label_QueryDevID
            // 
            this.label_QueryDevID.AutoSize = true;
            this.label_QueryDevID.Location = new System.Drawing.Point(202, 24);
            this.label_QueryDevID.Name = "label_QueryDevID";
            this.label_QueryDevID.Size = new System.Drawing.Size(53, 12);
            this.label_QueryDevID.TabIndex = 23;
            this.label_QueryDevID.Text = "DeviceId";
            // 
            // textAlarmQueryDeviceId
            // 
            this.textAlarmQueryDeviceId.Location = new System.Drawing.Point(202, 40);
            this.textAlarmQueryDeviceId.Name = "textAlarmQueryDeviceId";
            this.textAlarmQueryDeviceId.Size = new System.Drawing.Size(80, 21);
            this.textAlarmQueryDeviceId.TabIndex = 24;
            // 
            // label_QueryChnlID
            // 
            this.label_QueryChnlID.AutoSize = true;
            this.label_QueryChnlID.Location = new System.Drawing.Point(202, 67);
            this.label_QueryChnlID.Name = "label_QueryChnlID";
            this.label_QueryChnlID.Size = new System.Drawing.Size(59, 12);
            this.label_QueryChnlID.TabIndex = 25;
            this.label_QueryChnlID.Text = "ChannelId";
            // 
            // textAlarmQueryChannelId
            // 
            this.textAlarmQueryChannelId.Location = new System.Drawing.Point(202, 83);
            this.textAlarmQueryChannelId.Name = "textAlarmQueryChannelId";
            this.textAlarmQueryChannelId.Size = new System.Drawing.Size(80, 21);
            this.textAlarmQueryChannelId.TabIndex = 26;
            // 
            // label_QueryOrgID
            // 
            this.label_QueryOrgID.AutoSize = true;
            this.label_QueryOrgID.Location = new System.Drawing.Point(289, 24);
            this.label_QueryOrgID.Name = "label_QueryOrgID";
            this.label_QueryOrgID.Size = new System.Drawing.Size(35, 12);
            this.label_QueryOrgID.TabIndex = 27;
            this.label_QueryOrgID.Text = "OrgId";
            // 
            // textAlarmQueryOrgId
            // 
            this.textAlarmQueryOrgId.Location = new System.Drawing.Point(289, 40);
            this.textAlarmQueryOrgId.Name = "textAlarmQueryOrgId";
            this.textAlarmQueryOrgId.Size = new System.Drawing.Size(59, 21);
            this.textAlarmQueryOrgId.TabIndex = 28;
            // 
            // label_QueryAlarmID
            // 
            this.label_QueryAlarmID.AutoSize = true;
            this.label_QueryAlarmID.Location = new System.Drawing.Point(289, 67);
            this.label_QueryAlarmID.Name = "label_QueryAlarmID";
            this.label_QueryAlarmID.Size = new System.Drawing.Size(47, 12);
            this.label_QueryAlarmID.TabIndex = 29;
            this.label_QueryAlarmID.Text = "AlarmId";
            // 
            // textAlarmQueryAlarmId
            // 
            this.textAlarmQueryAlarmId.Location = new System.Drawing.Point(289, 83);
            this.textAlarmQueryAlarmId.Name = "textAlarmQueryAlarmId";
            this.textAlarmQueryAlarmId.Size = new System.Drawing.Size(59, 21);
            this.textAlarmQueryAlarmId.TabIndex = 30;
            // 
            // label_QueryAlarmCode
            // 
            this.label_QueryAlarmCode.AutoSize = true;
            this.label_QueryAlarmCode.Location = new System.Drawing.Point(356, 24);
            this.label_QueryAlarmCode.Name = "label_QueryAlarmCode";
            this.label_QueryAlarmCode.Size = new System.Drawing.Size(59, 12);
            this.label_QueryAlarmCode.TabIndex = 31;
            this.label_QueryAlarmCode.Text = "AlarmCode";
            // 
            // textAlarmQueryAlarmCode
            // 
            this.textAlarmQueryAlarmCode.Location = new System.Drawing.Point(356, 40);
            this.textAlarmQueryAlarmCode.Name = "textAlarmQueryAlarmCode";
            this.textAlarmQueryAlarmCode.Size = new System.Drawing.Size(89, 21);
            this.textAlarmQueryAlarmCode.TabIndex = 32;
            // 
            // label_QueryHU
            // 
            this.label_QueryHU.AutoSize = true;
            this.label_QueryHU.Location = new System.Drawing.Point(356, 67);
            this.label_QueryHU.Name = "label_QueryHU";
            this.label_QueryHU.Size = new System.Drawing.Size(65, 12);
            this.label_QueryHU.TabIndex = 33;
            this.label_QueryHU.Text = "HandleUser";
            // 
            // textAlarmQueryHandleUser
            // 
            this.textAlarmQueryHandleUser.Location = new System.Drawing.Point(356, 83);
            this.textAlarmQueryHandleUser.Name = "textAlarmQueryHandleUser";
            this.textAlarmQueryHandleUser.Size = new System.Drawing.Size(89, 21);
            this.textAlarmQueryHandleUser.TabIndex = 34;
            // 
            // label_QueryST
            // 
            this.label_QueryST.AutoSize = true;
            this.label_QueryST.Location = new System.Drawing.Point(452, 67);
            this.label_QueryST.Name = "label_QueryST";
            this.label_QueryST.Size = new System.Drawing.Size(53, 12);
            this.label_QueryST.TabIndex = 37;
            this.label_QueryST.Text = "SortType";
            // 
            // comboBoxAlarmQuerySortType
            // 
            this.comboBoxAlarmQuerySortType.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBoxAlarmQuerySortType.FormattingEnabled = true;
            this.comboBoxAlarmQuerySortType.Items.AddRange(new object[] {
            "1-Alarm time",
            "2-Alarm type",
            "3-Alarm level",
            "4-HandleUser",
            "5-Processing state"});
            this.comboBoxAlarmQuerySortType.Location = new System.Drawing.Point(452, 84);
            this.comboBoxAlarmQuerySortType.Name = "comboBoxAlarmQuerySortType";
            this.comboBoxAlarmQuerySortType.Size = new System.Drawing.Size(88, 20);
            this.comboBoxAlarmQuerySortType.TabIndex = 38;
            // 
            // label_QueryPZ
            // 
            this.label_QueryPZ.AutoSize = true;
            this.label_QueryPZ.Location = new System.Drawing.Point(452, 24);
            this.label_QueryPZ.Name = "label_QueryPZ";
            this.label_QueryPZ.Size = new System.Drawing.Size(53, 12);
            this.label_QueryPZ.TabIndex = 39;
            this.label_QueryPZ.Text = "PageSize";
            // 
            // textAlarmQueryPageSize
            // 
            this.textAlarmQueryPageSize.Location = new System.Drawing.Point(454, 39);
            this.textAlarmQueryPageSize.Name = "textAlarmQueryPageSize";
            this.textAlarmQueryPageSize.Size = new System.Drawing.Size(86, 21);
            this.textAlarmQueryPageSize.TabIndex = 40;
            this.textAlarmQueryPageSize.Text = "10";
            // 
            // label_QueryPN
            // 
            this.label_QueryPN.AutoSize = true;
            this.label_QueryPN.Location = new System.Drawing.Point(548, 24);
            this.label_QueryPN.Name = "label_QueryPN";
            this.label_QueryPN.Size = new System.Drawing.Size(47, 12);
            this.label_QueryPN.TabIndex = 41;
            this.label_QueryPN.Text = "PageNum";
            // 
            // textAlarmQueryPageNum
            // 
            this.textAlarmQueryPageNum.Location = new System.Drawing.Point(550, 39);
            this.textAlarmQueryPageNum.Name = "textAlarmQueryPageNum";
            this.textAlarmQueryPageNum.Size = new System.Drawing.Size(76, 21);
            this.textAlarmQueryPageNum.TabIndex = 42;
            this.textAlarmQueryPageNum.Text = "1";
            // 
            // comboBoxAlarmQuerySortOrder
            // 
            this.comboBoxAlarmQuerySortOrder.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBoxAlarmQuerySortOrder.FormattingEnabled = true;
            this.comboBoxAlarmQuerySortOrder.Items.AddRange(new object[] {
            "0-Ascending order",
            "1-Descending order"});
            this.comboBoxAlarmQuerySortOrder.Location = new System.Drawing.Point(548, 84);
            this.comboBoxAlarmQuerySortOrder.Name = "comboBoxAlarmQuerySortOrder";
            this.comboBoxAlarmQuerySortOrder.Size = new System.Drawing.Size(78, 20);
            this.comboBoxAlarmQuerySortOrder.TabIndex = 44;
            // 
            // label_QueryATL
            // 
            this.label_QueryATL.AutoSize = true;
            this.label_QueryATL.Location = new System.Drawing.Point(636, 24);
            this.label_QueryATL.Name = "label_QueryATL";
            this.label_QueryATL.Size = new System.Drawing.Size(83, 12);
            this.label_QueryATL.TabIndex = 45;
            this.label_QueryATL.Text = "AlarmTypeList";
            // 
            // textAlarmQueryTypeList
            // 
            this.textAlarmQueryTypeList.Location = new System.Drawing.Point(636, 40);
            this.textAlarmQueryTypeList.Name = "textAlarmQueryTypeList";
            this.textAlarmQueryTypeList.Size = new System.Drawing.Size(89, 21);
            this.textAlarmQueryTypeList.TabIndex = 46;
            // 
            // label_QueryASL
            // 
            this.label_QueryASL.AutoSize = true;
            this.label_QueryASL.Location = new System.Drawing.Point(636, 67);
            this.label_QueryASL.Name = "label_QueryASL";
            this.label_QueryASL.Size = new System.Drawing.Size(95, 12);
            this.label_QueryASL.TabIndex = 47;
            this.label_QueryASL.Text = "AlarmStatusList";
            // 
            // textAlarmQueryStatusList
            // 
            this.textAlarmQueryStatusList.Location = new System.Drawing.Point(636, 83);
            this.textAlarmQueryStatusList.Name = "textAlarmQueryStatusList";
            this.textAlarmQueryStatusList.Size = new System.Drawing.Size(89, 21);
            this.textAlarmQueryStatusList.TabIndex = 48;
            // 
            // label_QueryAGL
            // 
            this.label_QueryAGL.AutoSize = true;
            this.label_QueryAGL.Location = new System.Drawing.Point(733, 24);
            this.label_QueryAGL.Name = "label_QueryAGL";
            this.label_QueryAGL.Size = new System.Drawing.Size(89, 12);
            this.label_QueryAGL.TabIndex = 49;
            this.label_QueryAGL.Text = "AlarmGradeList";
            // 
            // textAlarmQueryGradeList
            // 
            this.textAlarmQueryGradeList.Location = new System.Drawing.Point(733, 40);
            this.textAlarmQueryGradeList.Name = "textAlarmQueryGradeList";
            this.textAlarmQueryGradeList.Size = new System.Drawing.Size(89, 21);
            this.textAlarmQueryGradeList.TabIndex = 50;
            // 
            // label_QueryHSL
            // 
            this.label_QueryHSL.AutoSize = true;
            this.label_QueryHSL.Location = new System.Drawing.Point(733, 67);
            this.label_QueryHSL.Name = "label_QueryHSL";
            this.label_QueryHSL.Size = new System.Drawing.Size(101, 12);
            this.label_QueryHSL.TabIndex = 51;
            this.label_QueryHSL.Text = "HandleStatusList";
            // 
            // textAlarmQueryHandleStatusList
            // 
            this.textAlarmQueryHandleStatusList.Location = new System.Drawing.Point(733, 83);
            this.textAlarmQueryHandleStatusList.Name = "textAlarmQueryHandleStatusList";
            this.textAlarmQueryHandleStatusList.Size = new System.Drawing.Size(89, 21);
            this.textAlarmQueryHandleStatusList.TabIndex = 52;
            this.textAlarmQueryHandleStatusList.Text = "5";
            // 
            // tabAlarmExport
            // 
            this.tabAlarmExport.Controls.Add(this.btnAlarmExport);
            this.tabAlarmExport.Controls.Add(this.label_ExportHBT);
            this.tabAlarmExport.Controls.Add(this.label_ExportABT);
            this.tabAlarmExport.Controls.Add(this.textAlarmExportBeginTime);
            this.tabAlarmExport.Controls.Add(this.label_ExportAET);
            this.tabAlarmExport.Controls.Add(this.textAlarmExportHandleStatusList);
            this.tabAlarmExport.Controls.Add(this.textAlarmExportEndTime);
            this.tabAlarmExport.Controls.Add(this.label_ExportHSL);
            this.tabAlarmExport.Controls.Add(this.textAlarmExportHandleBeginTime);
            this.tabAlarmExport.Controls.Add(this.textAlarmExportGradeList);
            this.tabAlarmExport.Controls.Add(this.label_ExportHET);
            this.tabAlarmExport.Controls.Add(this.label_ExportAGL);
            this.tabAlarmExport.Controls.Add(this.textAlarmExportHandleEndTime);
            this.tabAlarmExport.Controls.Add(this.textAlarmExportStatusList);
            this.tabAlarmExport.Controls.Add(this.label_ExportDevID);
            this.tabAlarmExport.Controls.Add(this.label_ExportASL);
            this.tabAlarmExport.Controls.Add(this.textAlarmExportDeviceId);
            this.tabAlarmExport.Controls.Add(this.textAlarmExportTypeList);
            this.tabAlarmExport.Controls.Add(this.label_ExportChnlID);
            this.tabAlarmExport.Controls.Add(this.label_ExportATL);
            this.tabAlarmExport.Controls.Add(this.textAlarmExportChannelId);
            this.tabAlarmExport.Controls.Add(this.comboBoxAlarmExportSortOrder);
            this.tabAlarmExport.Controls.Add(this.label_ExportOrgID);
            this.tabAlarmExport.Controls.Add(this.label_ExportSO);
            this.tabAlarmExport.Controls.Add(this.textAlarmExportOrgId);
            this.tabAlarmExport.Controls.Add(this.textAlarmExportLanguage);
            this.tabAlarmExport.Controls.Add(this.label_ExportAlarmID);
            this.tabAlarmExport.Controls.Add(this.label_ExportLan);
            this.tabAlarmExport.Controls.Add(this.textAlarmExportAlarmId);
            this.tabAlarmExport.Controls.Add(this.comboBoxAlarmExportSortType);
            this.tabAlarmExport.Controls.Add(this.label_ExportAlarmCode);
            this.tabAlarmExport.Controls.Add(this.label_ExportST);
            this.tabAlarmExport.Controls.Add(this.textAlarmExportAlarmCode);
            this.tabAlarmExport.Controls.Add(this.textAlarmExportHandleUser);
            this.tabAlarmExport.Controls.Add(this.label_ExportHU);
            this.tabAlarmExport.Location = new System.Drawing.Point(4, 22);
            this.tabAlarmExport.Name = "tabAlarmExport";
            this.tabAlarmExport.Padding = new System.Windows.Forms.Padding(3);
            this.tabAlarmExport.Size = new System.Drawing.Size(879, 237);
            this.tabAlarmExport.TabIndex = 1;
            this.tabAlarmExport.Text = "Export";
            this.tabAlarmExport.UseVisualStyleBackColor = true;
            // 
            // btnAlarmExport
            // 
            this.btnAlarmExport.Location = new System.Drawing.Point(3, 115);
            this.btnAlarmExport.Name = "btnAlarmExport";
            this.btnAlarmExport.Size = new System.Drawing.Size(106, 30);
            this.btnAlarmExport.TabIndex = 105;
            this.btnAlarmExport.Text = "AlarmExport";
            this.btnAlarmExport.UseVisualStyleBackColor = true;
            this.btnAlarmExport.Click += new System.EventHandler(this.btnAlarmExport_Click);
            // 
            // label_ExportHBT
            // 
            this.label_ExportHBT.AutoSize = true;
            this.label_ExportHBT.Location = new System.Drawing.Point(101, 20);
            this.label_ExportHBT.Name = "label_ExportHBT";
            this.label_ExportHBT.Size = new System.Drawing.Size(95, 12);
            this.label_ExportHBT.TabIndex = 75;
            this.label_ExportHBT.Text = "HandleBeginTime";
            // 
            // label_ExportABT
            // 
            this.label_ExportABT.AutoSize = true;
            this.label_ExportABT.Location = new System.Drawing.Point(5, 20);
            this.label_ExportABT.Name = "label_ExportABT";
            this.label_ExportABT.Size = new System.Drawing.Size(89, 12);
            this.label_ExportABT.TabIndex = 71;
            this.label_ExportABT.Text = "AlarmBeginTime";
            // 
            // textAlarmExportBeginTime
            // 
            this.textAlarmExportBeginTime.Location = new System.Drawing.Point(5, 36);
            this.textAlarmExportBeginTime.Name = "textAlarmExportBeginTime";
            this.textAlarmExportBeginTime.Size = new System.Drawing.Size(89, 21);
            this.textAlarmExportBeginTime.TabIndex = 72;
            // 
            // label_ExportAET
            // 
            this.label_ExportAET.AutoSize = true;
            this.label_ExportAET.Location = new System.Drawing.Point(5, 63);
            this.label_ExportAET.Name = "label_ExportAET";
            this.label_ExportAET.Size = new System.Drawing.Size(77, 12);
            this.label_ExportAET.TabIndex = 73;
            this.label_ExportAET.Text = "AlarmEndTime";
            // 
            // textAlarmExportHandleStatusList
            // 
            this.textAlarmExportHandleStatusList.Location = new System.Drawing.Point(728, 79);
            this.textAlarmExportHandleStatusList.Name = "textAlarmExportHandleStatusList";
            this.textAlarmExportHandleStatusList.Size = new System.Drawing.Size(89, 21);
            this.textAlarmExportHandleStatusList.TabIndex = 104;
            // 
            // textAlarmExportEndTime
            // 
            this.textAlarmExportEndTime.Location = new System.Drawing.Point(5, 79);
            this.textAlarmExportEndTime.Name = "textAlarmExportEndTime";
            this.textAlarmExportEndTime.Size = new System.Drawing.Size(89, 21);
            this.textAlarmExportEndTime.TabIndex = 74;
            // 
            // label_ExportHSL
            // 
            this.label_ExportHSL.AutoSize = true;
            this.label_ExportHSL.Location = new System.Drawing.Point(728, 63);
            this.label_ExportHSL.Name = "label_ExportHSL";
            this.label_ExportHSL.Size = new System.Drawing.Size(101, 12);
            this.label_ExportHSL.TabIndex = 103;
            this.label_ExportHSL.Text = "HandleStatusList";
            // 
            // textAlarmExportHandleBeginTime
            // 
            this.textAlarmExportHandleBeginTime.Location = new System.Drawing.Point(101, 36);
            this.textAlarmExportHandleBeginTime.Name = "textAlarmExportHandleBeginTime";
            this.textAlarmExportHandleBeginTime.Size = new System.Drawing.Size(89, 21);
            this.textAlarmExportHandleBeginTime.TabIndex = 76;
            // 
            // textAlarmExportGradeList
            // 
            this.textAlarmExportGradeList.Location = new System.Drawing.Point(728, 36);
            this.textAlarmExportGradeList.Name = "textAlarmExportGradeList";
            this.textAlarmExportGradeList.Size = new System.Drawing.Size(89, 21);
            this.textAlarmExportGradeList.TabIndex = 102;
            // 
            // label_ExportHET
            // 
            this.label_ExportHET.AutoSize = true;
            this.label_ExportHET.Location = new System.Drawing.Point(101, 63);
            this.label_ExportHET.Name = "label_ExportHET";
            this.label_ExportHET.Size = new System.Drawing.Size(83, 12);
            this.label_ExportHET.TabIndex = 77;
            this.label_ExportHET.Text = "HandleEndTime";
            // 
            // label_ExportAGL
            // 
            this.label_ExportAGL.AutoSize = true;
            this.label_ExportAGL.Location = new System.Drawing.Point(728, 20);
            this.label_ExportAGL.Name = "label_ExportAGL";
            this.label_ExportAGL.Size = new System.Drawing.Size(89, 12);
            this.label_ExportAGL.TabIndex = 101;
            this.label_ExportAGL.Text = "AlarmGradeList";
            // 
            // textAlarmExportHandleEndTime
            // 
            this.textAlarmExportHandleEndTime.Location = new System.Drawing.Point(101, 79);
            this.textAlarmExportHandleEndTime.Name = "textAlarmExportHandleEndTime";
            this.textAlarmExportHandleEndTime.Size = new System.Drawing.Size(89, 21);
            this.textAlarmExportHandleEndTime.TabIndex = 78;
            // 
            // textAlarmExportStatusList
            // 
            this.textAlarmExportStatusList.Location = new System.Drawing.Point(631, 79);
            this.textAlarmExportStatusList.Name = "textAlarmExportStatusList";
            this.textAlarmExportStatusList.Size = new System.Drawing.Size(89, 21);
            this.textAlarmExportStatusList.TabIndex = 100;
            // 
            // label_ExportDevID
            // 
            this.label_ExportDevID.AutoSize = true;
            this.label_ExportDevID.Location = new System.Drawing.Point(197, 20);
            this.label_ExportDevID.Name = "label_ExportDevID";
            this.label_ExportDevID.Size = new System.Drawing.Size(53, 12);
            this.label_ExportDevID.TabIndex = 79;
            this.label_ExportDevID.Text = "DeviceId";
            // 
            // label_ExportASL
            // 
            this.label_ExportASL.AutoSize = true;
            this.label_ExportASL.Location = new System.Drawing.Point(631, 63);
            this.label_ExportASL.Name = "label_ExportASL";
            this.label_ExportASL.Size = new System.Drawing.Size(95, 12);
            this.label_ExportASL.TabIndex = 99;
            this.label_ExportASL.Text = "AlarmStatusList";
            // 
            // textAlarmExportDeviceId
            // 
            this.textAlarmExportDeviceId.Location = new System.Drawing.Point(197, 36);
            this.textAlarmExportDeviceId.Name = "textAlarmExportDeviceId";
            this.textAlarmExportDeviceId.Size = new System.Drawing.Size(80, 21);
            this.textAlarmExportDeviceId.TabIndex = 80;
            // 
            // textAlarmExportTypeList
            // 
            this.textAlarmExportTypeList.Location = new System.Drawing.Point(631, 36);
            this.textAlarmExportTypeList.Name = "textAlarmExportTypeList";
            this.textAlarmExportTypeList.Size = new System.Drawing.Size(89, 21);
            this.textAlarmExportTypeList.TabIndex = 98;
            // 
            // label_ExportChnlID
            // 
            this.label_ExportChnlID.AutoSize = true;
            this.label_ExportChnlID.Location = new System.Drawing.Point(197, 63);
            this.label_ExportChnlID.Name = "label_ExportChnlID";
            this.label_ExportChnlID.Size = new System.Drawing.Size(59, 12);
            this.label_ExportChnlID.TabIndex = 81;
            this.label_ExportChnlID.Text = "ChannelId";
            // 
            // label_ExportATL
            // 
            this.label_ExportATL.AutoSize = true;
            this.label_ExportATL.Location = new System.Drawing.Point(631, 20);
            this.label_ExportATL.Name = "label_ExportATL";
            this.label_ExportATL.Size = new System.Drawing.Size(83, 12);
            this.label_ExportATL.TabIndex = 97;
            this.label_ExportATL.Text = "AlarmTypeList";
            // 
            // textAlarmExportChannelId
            // 
            this.textAlarmExportChannelId.Location = new System.Drawing.Point(197, 79);
            this.textAlarmExportChannelId.Name = "textAlarmExportChannelId";
            this.textAlarmExportChannelId.Size = new System.Drawing.Size(80, 21);
            this.textAlarmExportChannelId.TabIndex = 82;
            // 
            // comboBoxAlarmExportSortOrder
            // 
            this.comboBoxAlarmExportSortOrder.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBoxAlarmExportSortOrder.FormattingEnabled = true;
            this.comboBoxAlarmExportSortOrder.Items.AddRange(new object[] {
            "0-Ascending order",
            "1-Descending order"});
            this.comboBoxAlarmExportSortOrder.Location = new System.Drawing.Point(545, 79);
            this.comboBoxAlarmExportSortOrder.Name = "comboBoxAlarmExportSortOrder";
            this.comboBoxAlarmExportSortOrder.Size = new System.Drawing.Size(78, 20);
            this.comboBoxAlarmExportSortOrder.TabIndex = 96;
            // 
            // label_ExportOrgID
            // 
            this.label_ExportOrgID.AutoSize = true;
            this.label_ExportOrgID.Location = new System.Drawing.Point(284, 20);
            this.label_ExportOrgID.Name = "label_ExportOrgID";
            this.label_ExportOrgID.Size = new System.Drawing.Size(35, 12);
            this.label_ExportOrgID.TabIndex = 83;
            this.label_ExportOrgID.Text = "OrgId";
            // 
            // label_ExportSO
            // 
            this.label_ExportSO.AutoSize = true;
            this.label_ExportSO.Location = new System.Drawing.Point(545, 62);
            this.label_ExportSO.Name = "label_ExportSO";
            this.label_ExportSO.Size = new System.Drawing.Size(59, 12);
            this.label_ExportSO.TabIndex = 95;
            this.label_ExportSO.Text = "SortOrder";
            // 
            // textAlarmExportOrgId
            // 
            this.textAlarmExportOrgId.Location = new System.Drawing.Point(284, 36);
            this.textAlarmExportOrgId.Name = "textAlarmExportOrgId";
            this.textAlarmExportOrgId.Size = new System.Drawing.Size(59, 21);
            this.textAlarmExportOrgId.TabIndex = 84;
            // 
            // textAlarmExportLanguage
            // 
            this.textAlarmExportLanguage.Location = new System.Drawing.Point(450, 36);
            this.textAlarmExportLanguage.Name = "textAlarmExportLanguage";
            this.textAlarmExportLanguage.Size = new System.Drawing.Size(88, 21);
            this.textAlarmExportLanguage.TabIndex = 94;
            this.textAlarmExportLanguage.Text = "en_Us";
            // 
            // label_ExportAlarmID
            // 
            this.label_ExportAlarmID.AutoSize = true;
            this.label_ExportAlarmID.Location = new System.Drawing.Point(284, 63);
            this.label_ExportAlarmID.Name = "label_ExportAlarmID";
            this.label_ExportAlarmID.Size = new System.Drawing.Size(47, 12);
            this.label_ExportAlarmID.TabIndex = 85;
            this.label_ExportAlarmID.Text = "AlarmId";
            // 
            // label_ExportLan
            // 
            this.label_ExportLan.AutoSize = true;
            this.label_ExportLan.Location = new System.Drawing.Point(450, 20);
            this.label_ExportLan.Name = "label_ExportLan";
            this.label_ExportLan.Size = new System.Drawing.Size(53, 12);
            this.label_ExportLan.TabIndex = 93;
            this.label_ExportLan.Text = "Language";
            // 
            // textAlarmExportAlarmId
            // 
            this.textAlarmExportAlarmId.Location = new System.Drawing.Point(284, 79);
            this.textAlarmExportAlarmId.Name = "textAlarmExportAlarmId";
            this.textAlarmExportAlarmId.Size = new System.Drawing.Size(59, 21);
            this.textAlarmExportAlarmId.TabIndex = 86;
            // 
            // comboBoxAlarmExportSortType
            // 
            this.comboBoxAlarmExportSortType.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBoxAlarmExportSortType.FormattingEnabled = true;
            this.comboBoxAlarmExportSortType.Items.AddRange(new object[] {
            "1-Alarm time",
            "2-Alarm type",
            "3-Alarm level",
            "4-HandleUser",
            "5-Processing state"});
            this.comboBoxAlarmExportSortType.Location = new System.Drawing.Point(450, 79);
            this.comboBoxAlarmExportSortType.Name = "comboBoxAlarmExportSortType";
            this.comboBoxAlarmExportSortType.Size = new System.Drawing.Size(88, 20);
            this.comboBoxAlarmExportSortType.TabIndex = 92;
            // 
            // label_ExportAlarmCode
            // 
            this.label_ExportAlarmCode.AutoSize = true;
            this.label_ExportAlarmCode.Location = new System.Drawing.Point(351, 20);
            this.label_ExportAlarmCode.Name = "label_ExportAlarmCode";
            this.label_ExportAlarmCode.Size = new System.Drawing.Size(59, 12);
            this.label_ExportAlarmCode.TabIndex = 87;
            this.label_ExportAlarmCode.Text = "AlarmCode";
            // 
            // label_ExportST
            // 
            this.label_ExportST.AutoSize = true;
            this.label_ExportST.Location = new System.Drawing.Point(450, 62);
            this.label_ExportST.Name = "label_ExportST";
            this.label_ExportST.Size = new System.Drawing.Size(53, 12);
            this.label_ExportST.TabIndex = 91;
            this.label_ExportST.Text = "SortType";
            // 
            // textAlarmExportAlarmCode
            // 
            this.textAlarmExportAlarmCode.Location = new System.Drawing.Point(351, 36);
            this.textAlarmExportAlarmCode.Name = "textAlarmExportAlarmCode";
            this.textAlarmExportAlarmCode.Size = new System.Drawing.Size(89, 21);
            this.textAlarmExportAlarmCode.TabIndex = 88;
            // 
            // textAlarmExportHandleUser
            // 
            this.textAlarmExportHandleUser.Location = new System.Drawing.Point(351, 79);
            this.textAlarmExportHandleUser.Name = "textAlarmExportHandleUser";
            this.textAlarmExportHandleUser.Size = new System.Drawing.Size(89, 21);
            this.textAlarmExportHandleUser.TabIndex = 90;
            // 
            // label_ExportHU
            // 
            this.label_ExportHU.AutoSize = true;
            this.label_ExportHU.Location = new System.Drawing.Point(351, 63);
            this.label_ExportHU.Name = "label_ExportHU";
            this.label_ExportHU.Size = new System.Drawing.Size(65, 12);
            this.label_ExportHU.TabIndex = 89;
            this.label_ExportHU.Text = "HandleUser";
            // 
            // tabAlarmConfirm
            // 
            this.tabAlarmConfirm.Controls.Add(this.textAlarmQueryPFAlarmCode);
            this.tabAlarmConfirm.Controls.Add(this.btnAlarmQueryPF);
            this.tabAlarmConfirm.Controls.Add(this.textAlarmCodeSource);
            this.tabAlarmConfirm.Controls.Add(this.textAlarmBlockType);
            this.tabAlarmConfirm.Controls.Add(this.label_ConfirmDS);
            this.tabAlarmConfirm.Controls.Add(this.textAlarmBlockDuration);
            this.tabAlarmConfirm.Controls.Add(this.label_ConfirmAT);
            this.tabAlarmConfirm.Controls.Add(this.btnAlarmBlock);
            this.tabAlarmConfirm.Controls.Add(this.label_ConfirmACS);
            this.tabAlarmConfirm.Controls.Add(this.label_ConfirmAPF_AC);
            this.tabAlarmConfirm.Controls.Add(this.btnAlarmConfirm);
            this.tabAlarmConfirm.Controls.Add(this.label_ConfirmAC);
            this.tabAlarmConfirm.Controls.Add(this.textAlarmAlarmCode);
            this.tabAlarmConfirm.Controls.Add(this.label_ConfirmHU);
            this.tabAlarmConfirm.Controls.Add(this.textAlarmHandleUser);
            this.tabAlarmConfirm.Controls.Add(this.textAlarmEmailList);
            this.tabAlarmConfirm.Controls.Add(this.label_ConfirmHM);
            this.tabAlarmConfirm.Controls.Add(this.label_ConfirmEL);
            this.tabAlarmConfirm.Controls.Add(this.textAlarmHandleMsg);
            this.tabAlarmConfirm.Controls.Add(this.comboBoxAlarmHandleStatus);
            this.tabAlarmConfirm.Controls.Add(this.label_ConfirmHS);
            this.tabAlarmConfirm.Location = new System.Drawing.Point(4, 22);
            this.tabAlarmConfirm.Name = "tabAlarmConfirm";
            this.tabAlarmConfirm.Padding = new System.Windows.Forms.Padding(3);
            this.tabAlarmConfirm.Size = new System.Drawing.Size(879, 237);
            this.tabAlarmConfirm.TabIndex = 3;
            this.tabAlarmConfirm.Text = "AlarmConfirm";
            this.tabAlarmConfirm.UseVisualStyleBackColor = true;
            // 
            // textAlarmQueryPFAlarmCode
            // 
            this.textAlarmQueryPFAlarmCode.Location = new System.Drawing.Point(6, 146);
            this.textAlarmQueryPFAlarmCode.Name = "textAlarmQueryPFAlarmCode";
            this.textAlarmQueryPFAlarmCode.Size = new System.Drawing.Size(120, 21);
            this.textAlarmQueryPFAlarmCode.TabIndex = 67;
            // 
            // btnAlarmQueryPF
            // 
            this.btnAlarmQueryPF.Location = new System.Drawing.Point(137, 137);
            this.btnAlarmQueryPF.Name = "btnAlarmQueryPF";
            this.btnAlarmQueryPF.Size = new System.Drawing.Size(129, 30);
            this.btnAlarmQueryPF.TabIndex = 68;
            this.btnAlarmQueryPF.Text = "AlarmProcessFlow";
            this.btnAlarmQueryPF.UseVisualStyleBackColor = true;
            this.btnAlarmQueryPF.Click += new System.EventHandler(this.btnAlarmQueryPF_Click);
            // 
            // textAlarmCodeSource
            // 
            this.textAlarmCodeSource.Location = new System.Drawing.Point(353, 146);
            this.textAlarmCodeSource.Name = "textAlarmCodeSource";
            this.textAlarmCodeSource.Size = new System.Drawing.Size(120, 21);
            this.textAlarmCodeSource.TabIndex = 70;
            // 
            // textAlarmBlockType
            // 
            this.textAlarmBlockType.Location = new System.Drawing.Point(481, 146);
            this.textAlarmBlockType.Name = "textAlarmBlockType";
            this.textAlarmBlockType.Size = new System.Drawing.Size(59, 21);
            this.textAlarmBlockType.TabIndex = 72;
            // 
            // label_ConfirmDS
            // 
            this.label_ConfirmDS.AutoSize = true;
            this.label_ConfirmDS.Location = new System.Drawing.Point(547, 120);
            this.label_ConfirmDS.Name = "label_ConfirmDS";
            this.label_ConfirmDS.Size = new System.Drawing.Size(71, 12);
            this.label_ConfirmDS.TabIndex = 73;
            this.label_ConfirmDS.Text = "Duration(s)";
            // 
            // textAlarmBlockDuration
            // 
            this.textAlarmBlockDuration.Location = new System.Drawing.Point(547, 146);
            this.textAlarmBlockDuration.Name = "textAlarmBlockDuration";
            this.textAlarmBlockDuration.Size = new System.Drawing.Size(68, 21);
            this.textAlarmBlockDuration.TabIndex = 74;
            // 
            // label_ConfirmAT
            // 
            this.label_ConfirmAT.AutoSize = true;
            this.label_ConfirmAT.Location = new System.Drawing.Point(481, 120);
            this.label_ConfirmAT.Name = "label_ConfirmAT";
            this.label_ConfirmAT.Size = new System.Drawing.Size(59, 12);
            this.label_ConfirmAT.TabIndex = 71;
            this.label_ConfirmAT.Text = "AlarmType";
            // 
            // btnAlarmBlock
            // 
            this.btnAlarmBlock.Location = new System.Drawing.Point(622, 137);
            this.btnAlarmBlock.Name = "btnAlarmBlock";
            this.btnAlarmBlock.Size = new System.Drawing.Size(79, 30);
            this.btnAlarmBlock.TabIndex = 75;
            this.btnAlarmBlock.Text = "AlarmBlock";
            this.btnAlarmBlock.UseVisualStyleBackColor = true;
            this.btnAlarmBlock.Click += new System.EventHandler(this.btnAlarmBlock_Click);
            // 
            // label_ConfirmACS
            // 
            this.label_ConfirmACS.AutoSize = true;
            this.label_ConfirmACS.Location = new System.Drawing.Point(353, 120);
            this.label_ConfirmACS.Name = "label_ConfirmACS";
            this.label_ConfirmACS.Size = new System.Drawing.Size(95, 12);
            this.label_ConfirmACS.TabIndex = 69;
            this.label_ConfirmACS.Text = "AlarmCodeSource";
            // 
            // label_ConfirmAPF_AC
            // 
            this.label_ConfirmAPF_AC.AutoSize = true;
            this.label_ConfirmAPF_AC.Location = new System.Drawing.Point(6, 120);
            this.label_ConfirmAPF_AC.Name = "label_ConfirmAPF_AC";
            this.label_ConfirmAPF_AC.Size = new System.Drawing.Size(59, 12);
            this.label_ConfirmAPF_AC.TabIndex = 66;
            this.label_ConfirmAPF_AC.Text = "AlarmCode";
            // 
            // btnAlarmConfirm
            // 
            this.btnAlarmConfirm.Location = new System.Drawing.Point(778, 27);
            this.btnAlarmConfirm.Name = "btnAlarmConfirm";
            this.btnAlarmConfirm.Size = new System.Drawing.Size(90, 30);
            this.btnAlarmConfirm.TabIndex = 14;
            this.btnAlarmConfirm.Text = "AlarmConfirm";
            this.btnAlarmConfirm.UseVisualStyleBackColor = true;
            this.btnAlarmConfirm.Click += new System.EventHandler(this.btnAlarmConfirm_Click);
            // 
            // label_ConfirmAC
            // 
            this.label_ConfirmAC.AutoSize = true;
            this.label_ConfirmAC.Location = new System.Drawing.Point(6, 17);
            this.label_ConfirmAC.Name = "label_ConfirmAC";
            this.label_ConfirmAC.Size = new System.Drawing.Size(59, 12);
            this.label_ConfirmAC.TabIndex = 4;
            this.label_ConfirmAC.Text = "AlarmCode";
            // 
            // textAlarmAlarmCode
            // 
            this.textAlarmAlarmCode.Location = new System.Drawing.Point(6, 33);
            this.textAlarmAlarmCode.Name = "textAlarmAlarmCode";
            this.textAlarmAlarmCode.Size = new System.Drawing.Size(120, 21);
            this.textAlarmAlarmCode.TabIndex = 5;
            // 
            // label_ConfirmHU
            // 
            this.label_ConfirmHU.AutoSize = true;
            this.label_ConfirmHU.Location = new System.Drawing.Point(135, 17);
            this.label_ConfirmHU.Name = "label_ConfirmHU";
            this.label_ConfirmHU.Size = new System.Drawing.Size(65, 12);
            this.label_ConfirmHU.TabIndex = 6;
            this.label_ConfirmHU.Text = "HandleUser";
            // 
            // textAlarmHandleUser
            // 
            this.textAlarmHandleUser.Location = new System.Drawing.Point(135, 33);
            this.textAlarmHandleUser.Name = "textAlarmHandleUser";
            this.textAlarmHandleUser.Size = new System.Drawing.Size(87, 21);
            this.textAlarmHandleUser.TabIndex = 7;
            // 
            // textAlarmEmailList
            // 
            this.textAlarmEmailList.Location = new System.Drawing.Point(524, 17);
            this.textAlarmEmailList.Multiline = true;
            this.textAlarmEmailList.Name = "textAlarmEmailList";
            this.textAlarmEmailList.Size = new System.Drawing.Size(235, 47);
            this.textAlarmEmailList.TabIndex = 13;
            // 
            // label_ConfirmHM
            // 
            this.label_ConfirmHM.AutoSize = true;
            this.label_ConfirmHM.Location = new System.Drawing.Point(233, 17);
            this.label_ConfirmHM.Name = "label_ConfirmHM";
            this.label_ConfirmHM.Size = new System.Drawing.Size(59, 12);
            this.label_ConfirmHM.TabIndex = 8;
            this.label_ConfirmHM.Text = "HandleMsg";
            // 
            // label_ConfirmEL
            // 
            this.label_ConfirmEL.AutoSize = true;
            this.label_ConfirmEL.Location = new System.Drawing.Point(458, 17);
            this.label_ConfirmEL.Name = "label_ConfirmEL";
            this.label_ConfirmEL.Size = new System.Drawing.Size(59, 12);
            this.label_ConfirmEL.TabIndex = 12;
            this.label_ConfirmEL.Text = "EmailList";
            // 
            // textAlarmHandleMsg
            // 
            this.textAlarmHandleMsg.Location = new System.Drawing.Point(233, 33);
            this.textAlarmHandleMsg.Name = "textAlarmHandleMsg";
            this.textAlarmHandleMsg.Size = new System.Drawing.Size(111, 21);
            this.textAlarmHandleMsg.TabIndex = 9;
            // 
            // comboBoxAlarmHandleStatus
            // 
            this.comboBoxAlarmHandleStatus.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBoxAlarmHandleStatus.FormattingEnabled = true;
            this.comboBoxAlarmHandleStatus.Items.AddRange(new object[] {
            "1-In the process of processing",
            "2-Resolved",
            "3-False positives",
            "4-Ignore",
            "5-Unsolved"});
            this.comboBoxAlarmHandleStatus.Location = new System.Drawing.Point(355, 33);
            this.comboBoxAlarmHandleStatus.Name = "comboBoxAlarmHandleStatus";
            this.comboBoxAlarmHandleStatus.Size = new System.Drawing.Size(106, 20);
            this.comboBoxAlarmHandleStatus.TabIndex = 11;
            // 
            // label_ConfirmHS
            // 
            this.label_ConfirmHS.AutoSize = true;
            this.label_ConfirmHS.Location = new System.Drawing.Point(353, 17);
            this.label_ConfirmHS.Name = "label_ConfirmHS";
            this.label_ConfirmHS.Size = new System.Drawing.Size(77, 12);
            this.label_ConfirmHS.TabIndex = 10;
            this.label_ConfirmHS.Text = "HandleStatus";
            // 
            // btnAlarmClear
            // 
            this.btnAlarmClear.Location = new System.Drawing.Point(799, 487);
            this.btnAlarmClear.Name = "btnAlarmClear";
            this.btnAlarmClear.Size = new System.Drawing.Size(75, 30);
            this.btnAlarmClear.TabIndex = 109;
            this.btnAlarmClear.Text = "Clear";
            this.btnAlarmClear.UseVisualStyleBackColor = true;
            this.btnAlarmClear.Click += new System.EventHandler(this.btnAlarmClear_Click);
            // 
            // checkBoxAlarmStopRefresh
            // 
            this.checkBoxAlarmStopRefresh.AutoSize = true;
            this.checkBoxAlarmStopRefresh.Location = new System.Drawing.Point(802, 456);
            this.checkBoxAlarmStopRefresh.Name = "checkBoxAlarmStopRefresh";
            this.checkBoxAlarmStopRefresh.Size = new System.Drawing.Size(96, 16);
            this.checkBoxAlarmStopRefresh.TabIndex = 108;
            this.checkBoxAlarmStopRefresh.Text = "Stop refresh";
            this.checkBoxAlarmStopRefresh.UseVisualStyleBackColor = true;
            // 
            // checkBoxAlarmForbiddenNotify
            // 
            this.checkBoxAlarmForbiddenNotify.AutoSize = true;
            this.checkBoxAlarmForbiddenNotify.Location = new System.Drawing.Point(802, 425);
            this.checkBoxAlarmForbiddenNotify.Name = "checkBoxAlarmForbiddenNotify";
            this.checkBoxAlarmForbiddenNotify.Size = new System.Drawing.Size(102, 16);
            this.checkBoxAlarmForbiddenNotify.TabIndex = 107;
            this.checkBoxAlarmForbiddenNotify.Text = "Shielded push";
            this.checkBoxAlarmForbiddenNotify.UseVisualStyleBackColor = true;
            // 
            // textAlarmInfo
            // 
            this.textAlarmInfo.Location = new System.Drawing.Point(17, 298);
            this.textAlarmInfo.Multiline = true;
            this.textAlarmInfo.Name = "textAlarmInfo";
            this.textAlarmInfo.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.textAlarmInfo.Size = new System.Drawing.Size(772, 335);
            this.textAlarmInfo.TabIndex = 106;
            // 
            // Preview
            // 
            this.Preview.Controls.Add(this.pictureBoxPreviewVideo);
            this.Preview.Controls.Add(this.textPreviewResult);
            this.Preview.Controls.Add(this.label_PreHR);
            this.Preview.Controls.Add(this.tabPreview);
            this.Preview.Location = new System.Drawing.Point(124, 4);
            this.Preview.Name = "Preview";
            this.Preview.Size = new System.Drawing.Size(917, 641);
            this.Preview.TabIndex = 3;
            this.Preview.Text = "Preview";
            this.Preview.UseVisualStyleBackColor = true;
            // 
            // pictureBoxPreviewVideo
            // 
            this.pictureBoxPreviewVideo.Location = new System.Drawing.Point(4, 298);
            this.pictureBoxPreviewVideo.Name = "pictureBoxPreviewVideo";
            this.pictureBoxPreviewVideo.Size = new System.Drawing.Size(910, 340);
            this.pictureBoxPreviewVideo.TabIndex = 3;
            this.pictureBoxPreviewVideo.TabStop = false;
            // 
            // textPreviewResult
            // 
            this.textPreviewResult.Location = new System.Drawing.Point(4, 235);
            this.textPreviewResult.Multiline = true;
            this.textPreviewResult.Name = "textPreviewResult";
            this.textPreviewResult.Size = new System.Drawing.Size(906, 57);
            this.textPreviewResult.TabIndex = 2;
            // 
            // label_PreHR
            // 
            this.label_PreHR.AutoSize = true;
            this.label_PreHR.Location = new System.Drawing.Point(8, 219);
            this.label_PreHR.Name = "label_PreHR";
            this.label_PreHR.Size = new System.Drawing.Size(77, 12);
            this.label_PreHR.TabIndex = 1;
            this.label_PreHR.Text = "HandleResult";
            // 
            // tabPreview
            // 
            this.tabPreview.Controls.Add(this.PreviewVideo);
            this.tabPreview.Controls.Add(this.PreviewPicture);
            this.tabPreview.Controls.Add(this.PreviewSound);
            this.tabPreview.Controls.Add(this.PreviewRecord);
            this.tabPreview.Location = new System.Drawing.Point(4, 4);
            this.tabPreview.Name = "tabPreview";
            this.tabPreview.SelectedIndex = 0;
            this.tabPreview.Size = new System.Drawing.Size(910, 208);
            this.tabPreview.TabIndex = 0;
            // 
            // PreviewVideo
            // 
            this.PreviewVideo.Controls.Add(this.btnPreviewLockVideo);
            this.PreviewVideo.Controls.Add(this.groupDisRegion);
            this.PreviewVideo.Controls.Add(this.groupStreamMode);
            this.PreviewVideo.Controls.Add(this.groupVideoPlay);
            this.PreviewVideo.Location = new System.Drawing.Point(4, 22);
            this.PreviewVideo.Name = "PreviewVideo";
            this.PreviewVideo.Padding = new System.Windows.Forms.Padding(3);
            this.PreviewVideo.Size = new System.Drawing.Size(902, 182);
            this.PreviewVideo.TabIndex = 0;
            this.PreviewVideo.Text = "Video";
            this.PreviewVideo.UseVisualStyleBackColor = true;
            // 
            // btnPreviewLockVideo
            // 
            this.btnPreviewLockVideo.Location = new System.Drawing.Point(673, 129);
            this.btnPreviewLockVideo.Name = "btnPreviewLockVideo";
            this.btnPreviewLockVideo.Size = new System.Drawing.Size(110, 23);
            this.btnPreviewLockVideo.TabIndex = 3;
            this.btnPreviewLockVideo.Text = "LockVideo";
            this.btnPreviewLockVideo.UseVisualStyleBackColor = true;
            this.btnPreviewLockVideo.Click += new System.EventHandler(this.btnPreviewLockVideo_Click);
            // 
            // groupDisRegion
            // 
            this.groupDisRegion.Controls.Add(this.btnPreviewSetRegion);
            this.groupDisRegion.Controls.Add(this.checkBoxPreviewEnable);
            this.groupDisRegion.Controls.Add(this.textPreviewRight);
            this.groupDisRegion.Controls.Add(this.label_PreRight);
            this.groupDisRegion.Controls.Add(this.textPreviewBottom);
            this.groupDisRegion.Controls.Add(this.label_PreBottom);
            this.groupDisRegion.Controls.Add(this.textPreviewLeft);
            this.groupDisRegion.Controls.Add(this.label_PreLeft);
            this.groupDisRegion.Controls.Add(this.textPreviewTop);
            this.groupDisRegion.Controls.Add(this.label_PreTop);
            this.groupDisRegion.Location = new System.Drawing.Point(636, 7);
            this.groupDisRegion.Name = "groupDisRegion";
            this.groupDisRegion.Size = new System.Drawing.Size(194, 112);
            this.groupDisRegion.TabIndex = 2;
            this.groupDisRegion.TabStop = false;
            this.groupDisRegion.Text = "DisplayRegion";
            // 
            // btnPreviewSetRegion
            // 
            this.btnPreviewSetRegion.Location = new System.Drawing.Point(95, 77);
            this.btnPreviewSetRegion.Name = "btnPreviewSetRegion";
            this.btnPreviewSetRegion.Size = new System.Drawing.Size(75, 23);
            this.btnPreviewSetRegion.TabIndex = 9;
            this.btnPreviewSetRegion.Text = "SetRegion";
            this.btnPreviewSetRegion.UseVisualStyleBackColor = true;
            this.btnPreviewSetRegion.Click += new System.EventHandler(this.btnPreviewSetRegion_Click);
            // 
            // checkBoxPreviewEnable
            // 
            this.checkBoxPreviewEnable.AutoSize = true;
            this.checkBoxPreviewEnable.Location = new System.Drawing.Point(10, 82);
            this.checkBoxPreviewEnable.Name = "checkBoxPreviewEnable";
            this.checkBoxPreviewEnable.Size = new System.Drawing.Size(60, 16);
            this.checkBoxPreviewEnable.TabIndex = 8;
            this.checkBoxPreviewEnable.Text = "Enable";
            this.checkBoxPreviewEnable.UseVisualStyleBackColor = true;
            // 
            // textPreviewRight
            // 
            this.textPreviewRight.Location = new System.Drawing.Point(134, 51);
            this.textPreviewRight.Name = "textPreviewRight";
            this.textPreviewRight.Size = new System.Drawing.Size(40, 21);
            this.textPreviewRight.TabIndex = 7;
            // 
            // label_PreRight
            // 
            this.label_PreRight.AutoSize = true;
            this.label_PreRight.Location = new System.Drawing.Point(88, 54);
            this.label_PreRight.Name = "label_PreRight";
            this.label_PreRight.Size = new System.Drawing.Size(35, 12);
            this.label_PreRight.TabIndex = 6;
            this.label_PreRight.Text = "Right";
            // 
            // textPreviewBottom
            // 
            this.textPreviewBottom.Location = new System.Drawing.Point(134, 21);
            this.textPreviewBottom.Name = "textPreviewBottom";
            this.textPreviewBottom.Size = new System.Drawing.Size(40, 21);
            this.textPreviewBottom.TabIndex = 5;
            // 
            // label_PreBottom
            // 
            this.label_PreBottom.AutoSize = true;
            this.label_PreBottom.Location = new System.Drawing.Point(87, 24);
            this.label_PreBottom.Name = "label_PreBottom";
            this.label_PreBottom.Size = new System.Drawing.Size(41, 12);
            this.label_PreBottom.TabIndex = 4;
            this.label_PreBottom.Text = "Bottom";
            // 
            // textPreviewLeft
            // 
            this.textPreviewLeft.Location = new System.Drawing.Point(40, 51);
            this.textPreviewLeft.Name = "textPreviewLeft";
            this.textPreviewLeft.Size = new System.Drawing.Size(40, 21);
            this.textPreviewLeft.TabIndex = 3;
            // 
            // label_PreLeft
            // 
            this.label_PreLeft.AutoSize = true;
            this.label_PreLeft.Location = new System.Drawing.Point(8, 54);
            this.label_PreLeft.Name = "label_PreLeft";
            this.label_PreLeft.Size = new System.Drawing.Size(29, 12);
            this.label_PreLeft.TabIndex = 2;
            this.label_PreLeft.Text = "Left";
            // 
            // textPreviewTop
            // 
            this.textPreviewTop.Location = new System.Drawing.Point(40, 21);
            this.textPreviewTop.Name = "textPreviewTop";
            this.textPreviewTop.Size = new System.Drawing.Size(40, 21);
            this.textPreviewTop.TabIndex = 1;
            // 
            // label_PreTop
            // 
            this.label_PreTop.AutoSize = true;
            this.label_PreTop.Location = new System.Drawing.Point(7, 24);
            this.label_PreTop.Name = "label_PreTop";
            this.label_PreTop.Size = new System.Drawing.Size(23, 12);
            this.label_PreTop.TabIndex = 0;
            this.label_PreTop.Text = "Top";
            // 
            // groupStreamMode
            // 
            this.groupStreamMode.Controls.Add(this.btnPreviewSetStreamMode);
            this.groupStreamMode.Controls.Add(this.btnPreviewGetStreamMode);
            this.groupStreamMode.Controls.Add(this.textPreviewDelayTime);
            this.groupStreamMode.Controls.Add(this.textPreviewStreamMode);
            this.groupStreamMode.Controls.Add(this.label_PreDelT);
            this.groupStreamMode.Controls.Add(this.label_PreSM);
            this.groupStreamMode.Location = new System.Drawing.Point(457, 7);
            this.groupStreamMode.Name = "groupStreamMode";
            this.groupStreamMode.Size = new System.Drawing.Size(157, 146);
            this.groupStreamMode.TabIndex = 1;
            this.groupStreamMode.TabStop = false;
            this.groupStreamMode.Text = "StreamMode";
            // 
            // btnPreviewSetStreamMode
            // 
            this.btnPreviewSetStreamMode.Location = new System.Drawing.Point(27, 107);
            this.btnPreviewSetStreamMode.Name = "btnPreviewSetStreamMode";
            this.btnPreviewSetStreamMode.Size = new System.Drawing.Size(104, 23);
            this.btnPreviewSetStreamMode.TabIndex = 5;
            this.btnPreviewSetStreamMode.Text = "SetStream Mode";
            this.btnPreviewSetStreamMode.UseVisualStyleBackColor = true;
            this.btnPreviewSetStreamMode.Click += new System.EventHandler(this.btnPreviewSetStreamMode_Click);
            // 
            // btnPreviewGetStreamMode
            // 
            this.btnPreviewGetStreamMode.Location = new System.Drawing.Point(27, 76);
            this.btnPreviewGetStreamMode.Name = "btnPreviewGetStreamMode";
            this.btnPreviewGetStreamMode.Size = new System.Drawing.Size(104, 23);
            this.btnPreviewGetStreamMode.TabIndex = 4;
            this.btnPreviewGetStreamMode.Text = "GetStream Mode";
            this.btnPreviewGetStreamMode.UseVisualStyleBackColor = true;
            this.btnPreviewGetStreamMode.Click += new System.EventHandler(this.btnPreviewGetStreamMode_Click);
            // 
            // textPreviewDelayTime
            // 
            this.textPreviewDelayTime.Location = new System.Drawing.Point(78, 46);
            this.textPreviewDelayTime.Name = "textPreviewDelayTime";
            this.textPreviewDelayTime.Size = new System.Drawing.Size(73, 21);
            this.textPreviewDelayTime.TabIndex = 3;
            this.textPreviewDelayTime.Text = "0";
            // 
            // textPreviewStreamMode
            // 
            this.textPreviewStreamMode.Location = new System.Drawing.Point(78, 20);
            this.textPreviewStreamMode.Name = "textPreviewStreamMode";
            this.textPreviewStreamMode.Size = new System.Drawing.Size(73, 21);
            this.textPreviewStreamMode.TabIndex = 2;
            this.textPreviewStreamMode.Text = "2";
            // 
            // label_PreDelT
            // 
            this.label_PreDelT.AutoSize = true;
            this.label_PreDelT.Location = new System.Drawing.Point(7, 50);
            this.label_PreDelT.Name = "label_PreDelT";
            this.label_PreDelT.Size = new System.Drawing.Size(65, 12);
            this.label_PreDelT.TabIndex = 1;
            this.label_PreDelT.Text = "DelayTime:";
            // 
            // label_PreSM
            // 
            this.label_PreSM.AutoSize = true;
            this.label_PreSM.Location = new System.Drawing.Point(7, 24);
            this.label_PreSM.Name = "label_PreSM";
            this.label_PreSM.Size = new System.Drawing.Size(71, 12);
            this.label_PreSM.TabIndex = 0;
            this.label_PreSM.Text = "StreamMode:";
            // 
            // groupVideoPlay
            // 
            this.groupVideoPlay.Controls.Add(this.btnPreviewStopVideo);
            this.groupVideoPlay.Controls.Add(this.btnPreviewStartVideo);
            this.groupVideoPlay.Controls.Add(this.groupMP);
            this.groupVideoPlay.Controls.Add(this.groupVBP);
            this.groupVideoPlay.Controls.Add(this.radioPreviewMultiviewPlay);
            this.groupVideoPlay.Controls.Add(this.radioPreviewSinglePlay);
            this.groupVideoPlay.Location = new System.Drawing.Point(7, 7);
            this.groupVideoPlay.Name = "groupVideoPlay";
            this.groupVideoPlay.Size = new System.Drawing.Size(431, 170);
            this.groupVideoPlay.TabIndex = 0;
            this.groupVideoPlay.TabStop = false;
            this.groupVideoPlay.Text = "VideoPlay";
            // 
            // btnPreviewStopVideo
            // 
            this.btnPreviewStopVideo.Location = new System.Drawing.Point(332, 137);
            this.btnPreviewStopVideo.Name = "btnPreviewStopVideo";
            this.btnPreviewStopVideo.Size = new System.Drawing.Size(75, 23);
            this.btnPreviewStopVideo.TabIndex = 5;
            this.btnPreviewStopVideo.Text = "StopVideo";
            this.btnPreviewStopVideo.UseVisualStyleBackColor = true;
            this.btnPreviewStopVideo.Click += new System.EventHandler(this.btnPreviewStopVideo_Click);
            // 
            // btnPreviewStartVideo
            // 
            this.btnPreviewStartVideo.Location = new System.Drawing.Point(239, 137);
            this.btnPreviewStartVideo.Name = "btnPreviewStartVideo";
            this.btnPreviewStartVideo.Size = new System.Drawing.Size(75, 23);
            this.btnPreviewStartVideo.TabIndex = 4;
            this.btnPreviewStartVideo.Text = "StartVideo";
            this.btnPreviewStartVideo.UseVisualStyleBackColor = true;
            this.btnPreviewStartVideo.Click += new System.EventHandler(this.btnPreviewStartVideo_Click);
            // 
            // groupMP
            // 
            this.groupMP.Controls.Add(this.textPreviewStartIndex);
            this.groupMP.Controls.Add(this.textPreviewTrackID);
            this.groupMP.Controls.Add(this.textPreviewScreenNum);
            this.groupMP.Controls.Add(this.label_PreSI);
            this.groupMP.Controls.Add(this.label_PreTrackID);
            this.groupMP.Controls.Add(this.label_PreSN);
            this.groupMP.Location = new System.Drawing.Point(224, 41);
            this.groupMP.Name = "groupMP";
            this.groupMP.Size = new System.Drawing.Size(201, 90);
            this.groupMP.TabIndex = 3;
            this.groupMP.TabStop = false;
            this.groupMP.Text = "MultiviewParam";
            // 
            // textPreviewStartIndex
            // 
            this.textPreviewStartIndex.Enabled = false;
            this.textPreviewStartIndex.Location = new System.Drawing.Point(78, 57);
            this.textPreviewStartIndex.Name = "textPreviewStartIndex";
            this.textPreviewStartIndex.Size = new System.Drawing.Size(115, 21);
            this.textPreviewStartIndex.TabIndex = 12;
            this.textPreviewStartIndex.Text = "1";
            // 
            // textPreviewTrackID
            // 
            this.textPreviewTrackID.Enabled = false;
            this.textPreviewTrackID.Location = new System.Drawing.Point(78, 35);
            this.textPreviewTrackID.Name = "textPreviewTrackID";
            this.textPreviewTrackID.Size = new System.Drawing.Size(115, 21);
            this.textPreviewTrackID.TabIndex = 11;
            this.textPreviewTrackID.Text = "701";
            // 
            // textPreviewScreenNum
            // 
            this.textPreviewScreenNum.Enabled = false;
            this.textPreviewScreenNum.Location = new System.Drawing.Point(78, 13);
            this.textPreviewScreenNum.Name = "textPreviewScreenNum";
            this.textPreviewScreenNum.Size = new System.Drawing.Size(115, 21);
            this.textPreviewScreenNum.TabIndex = 10;
            this.textPreviewScreenNum.Text = "4";
            // 
            // label_PreSI
            // 
            this.label_PreSI.AutoSize = true;
            this.label_PreSI.Location = new System.Drawing.Point(7, 59);
            this.label_PreSI.Name = "label_PreSI";
            this.label_PreSI.Size = new System.Drawing.Size(71, 12);
            this.label_PreSI.TabIndex = 9;
            this.label_PreSI.Text = "StartIndex:";
            // 
            // label_PreTrackID
            // 
            this.label_PreTrackID.AutoSize = true;
            this.label_PreTrackID.Location = new System.Drawing.Point(7, 37);
            this.label_PreTrackID.Name = "label_PreTrackID";
            this.label_PreTrackID.Size = new System.Drawing.Size(53, 12);
            this.label_PreTrackID.TabIndex = 8;
            this.label_PreTrackID.Text = "TrackID:";
            // 
            // label_PreSN
            // 
            this.label_PreSN.AutoSize = true;
            this.label_PreSN.Location = new System.Drawing.Point(7, 15);
            this.label_PreSN.Name = "label_PreSN";
            this.label_PreSN.Size = new System.Drawing.Size(65, 12);
            this.label_PreSN.TabIndex = 7;
            this.label_PreSN.Text = "ScreenNum:";
            // 
            // groupVBP
            // 
            this.groupVBP.Controls.Add(this.textPreviewDataType);
            this.groupVBP.Controls.Add(this.textPreviewStreamType);
            this.groupVBP.Controls.Add(this.textPreviewDeviceID);
            this.groupVBP.Controls.Add(this.checkBoxPreviewDecode);
            this.groupVBP.Controls.Add(this.label_PreDT);
            this.groupVBP.Controls.Add(this.label_PreST);
            this.groupVBP.Controls.Add(this.label_PreDevID);
            this.groupVBP.Location = new System.Drawing.Point(7, 41);
            this.groupVBP.Name = "groupVBP";
            this.groupVBP.Size = new System.Drawing.Size(200, 111);
            this.groupVBP.TabIndex = 2;
            this.groupVBP.TabStop = false;
            this.groupVBP.Text = "VideoBaseParam";
            // 
            // textPreviewDataType
            // 
            this.textPreviewDataType.Location = new System.Drawing.Point(87, 63);
            this.textPreviewDataType.Name = "textPreviewDataType";
            this.textPreviewDataType.Size = new System.Drawing.Size(109, 21);
            this.textPreviewDataType.TabIndex = 6;
            this.textPreviewDataType.Text = "1";
            // 
            // textPreviewStreamType
            // 
            this.textPreviewStreamType.Location = new System.Drawing.Point(87, 41);
            this.textPreviewStreamType.Name = "textPreviewStreamType";
            this.textPreviewStreamType.Size = new System.Drawing.Size(109, 21);
            this.textPreviewStreamType.TabIndex = 5;
            this.textPreviewStreamType.Text = "1";
            // 
            // textPreviewDeviceID
            // 
            this.textPreviewDeviceID.Location = new System.Drawing.Point(88, 19);
            this.textPreviewDeviceID.Name = "textPreviewDeviceID";
            this.textPreviewDeviceID.Size = new System.Drawing.Size(108, 21);
            this.textPreviewDeviceID.TabIndex = 4;
            this.textPreviewDeviceID.Text = "149874676B1A6078J8GPT68";
            // 
            // checkBoxPreviewDecode
            // 
            this.checkBoxPreviewDecode.AutoSize = true;
            this.checkBoxPreviewDecode.Location = new System.Drawing.Point(12, 89);
            this.checkBoxPreviewDecode.Name = "checkBoxPreviewDecode";
            this.checkBoxPreviewDecode.Size = new System.Drawing.Size(72, 16);
            this.checkBoxPreviewDecode.TabIndex = 3;
            this.checkBoxPreviewDecode.Text = "IsDecode";
            this.checkBoxPreviewDecode.UseVisualStyleBackColor = true;
            // 
            // label_PreDT
            // 
            this.label_PreDT.AutoSize = true;
            this.label_PreDT.Location = new System.Drawing.Point(10, 65);
            this.label_PreDT.Name = "label_PreDT";
            this.label_PreDT.Size = new System.Drawing.Size(59, 12);
            this.label_PreDT.TabIndex = 2;
            this.label_PreDT.Text = "DataType:";
            // 
            // label_PreST
            // 
            this.label_PreST.AutoSize = true;
            this.label_PreST.Location = new System.Drawing.Point(10, 43);
            this.label_PreST.Name = "label_PreST";
            this.label_PreST.Size = new System.Drawing.Size(71, 12);
            this.label_PreST.TabIndex = 1;
            this.label_PreST.Text = "StreamType:";
            // 
            // label_PreDevID
            // 
            this.label_PreDevID.AutoSize = true;
            this.label_PreDevID.Location = new System.Drawing.Point(10, 21);
            this.label_PreDevID.Name = "label_PreDevID";
            this.label_PreDevID.Size = new System.Drawing.Size(59, 12);
            this.label_PreDevID.TabIndex = 0;
            this.label_PreDevID.Text = "DeviceID:";
            // 
            // radioPreviewMultiviewPlay
            // 
            this.radioPreviewMultiviewPlay.AutoSize = true;
            this.radioPreviewMultiviewPlay.Location = new System.Drawing.Point(251, 20);
            this.radioPreviewMultiviewPlay.Name = "radioPreviewMultiviewPlay";
            this.radioPreviewMultiviewPlay.Size = new System.Drawing.Size(101, 16);
            this.radioPreviewMultiviewPlay.TabIndex = 1;
            this.radioPreviewMultiviewPlay.Text = "MultiviewPlay";
            this.radioPreviewMultiviewPlay.UseVisualStyleBackColor = true;
            this.radioPreviewMultiviewPlay.CheckedChanged += new System.EventHandler(this.radioPreviewMultiviewPlay_CheckedChanged);
            // 
            // radioPreviewSinglePlay
            // 
            this.radioPreviewSinglePlay.AutoSize = true;
            this.radioPreviewSinglePlay.Checked = true;
            this.radioPreviewSinglePlay.Location = new System.Drawing.Point(65, 20);
            this.radioPreviewSinglePlay.Name = "radioPreviewSinglePlay";
            this.radioPreviewSinglePlay.Size = new System.Drawing.Size(83, 16);
            this.radioPreviewSinglePlay.TabIndex = 0;
            this.radioPreviewSinglePlay.TabStop = true;
            this.radioPreviewSinglePlay.Text = "SinglePlay";
            this.radioPreviewSinglePlay.UseVisualStyleBackColor = true;
            this.radioPreviewSinglePlay.CheckedChanged += new System.EventHandler(this.radioPreviewSinglePlay_CheckedChanged);
            // 
            // PreviewPicture
            // 
            this.PreviewPicture.Controls.Add(this.groupGetPic);
            this.PreviewPicture.Location = new System.Drawing.Point(4, 22);
            this.PreviewPicture.Name = "PreviewPicture";
            this.PreviewPicture.Padding = new System.Windows.Forms.Padding(3);
            this.PreviewPicture.Size = new System.Drawing.Size(902, 182);
            this.PreviewPicture.TabIndex = 1;
            this.PreviewPicture.Text = "Picture";
            this.PreviewPicture.UseVisualStyleBackColor = true;
            // 
            // groupGetPic
            // 
            this.groupGetPic.Controls.Add(this.btnPreviewGetPicToBuffer);
            this.groupGetPic.Controls.Add(this.btnPreviewGetPicToFile);
            this.groupGetPic.Controls.Add(this.btnPreviewGetPicSize);
            this.groupGetPic.Controls.Add(this.pictureBoxPreviewPicture);
            this.groupGetPic.Controls.Add(this.comboBoxPreviewPicture);
            this.groupGetPic.Controls.Add(this.textPreviewPicPath);
            this.groupGetPic.Controls.Add(this.label_PrePicPath);
            this.groupGetPic.Location = new System.Drawing.Point(7, 7);
            this.groupGetPic.Name = "groupGetPic";
            this.groupGetPic.Size = new System.Drawing.Size(397, 169);
            this.groupGetPic.TabIndex = 0;
            this.groupGetPic.TabStop = false;
            this.groupGetPic.Text = "GetPicture";
            // 
            // btnPreviewGetPicToBuffer
            // 
            this.btnPreviewGetPicToBuffer.Location = new System.Drawing.Point(124, 76);
            this.btnPreviewGetPicToBuffer.Name = "btnPreviewGetPicToBuffer";
            this.btnPreviewGetPicToBuffer.Size = new System.Drawing.Size(106, 36);
            this.btnPreviewGetPicToBuffer.TabIndex = 6;
            this.btnPreviewGetPicToBuffer.Text = "GetPicture ToBuffer";
            this.btnPreviewGetPicToBuffer.UseVisualStyleBackColor = true;
            this.btnPreviewGetPicToBuffer.Click += new System.EventHandler(this.btnPreviewGetPicToBuffer_Click);
            // 
            // btnPreviewGetPicToFile
            // 
            this.btnPreviewGetPicToFile.Location = new System.Drawing.Point(6, 76);
            this.btnPreviewGetPicToFile.Name = "btnPreviewGetPicToFile";
            this.btnPreviewGetPicToFile.Size = new System.Drawing.Size(103, 36);
            this.btnPreviewGetPicToFile.TabIndex = 5;
            this.btnPreviewGetPicToFile.Text = "GetPicture ToFile";
            this.btnPreviewGetPicToFile.UseVisualStyleBackColor = true;
            this.btnPreviewGetPicToFile.Click += new System.EventHandler(this.btnPreviewGetPicToFile_Click);
            // 
            // btnPreviewGetPicSize
            // 
            this.btnPreviewGetPicSize.Location = new System.Drawing.Point(124, 46);
            this.btnPreviewGetPicSize.Name = "btnPreviewGetPicSize";
            this.btnPreviewGetPicSize.Size = new System.Drawing.Size(106, 23);
            this.btnPreviewGetPicSize.TabIndex = 4;
            this.btnPreviewGetPicSize.Text = "GetPicture Size";
            this.btnPreviewGetPicSize.UseVisualStyleBackColor = true;
            this.btnPreviewGetPicSize.Click += new System.EventHandler(this.btnPreviewGetPicSize_Click);
            // 
            // pictureBoxPreviewPicture
            // 
            this.pictureBoxPreviewPicture.Location = new System.Drawing.Point(244, 16);
            this.pictureBoxPreviewPicture.Name = "pictureBoxPreviewPicture";
            this.pictureBoxPreviewPicture.Size = new System.Drawing.Size(145, 145);
            this.pictureBoxPreviewPicture.TabIndex = 3;
            this.pictureBoxPreviewPicture.TabStop = false;
            // 
            // comboBoxPreviewPicture
            // 
            this.comboBoxPreviewPicture.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBoxPreviewPicture.FormattingEnabled = true;
            this.comboBoxPreviewPicture.Items.AddRange(new object[] {
            "BMP",
            "JPEG"});
            this.comboBoxPreviewPicture.Location = new System.Drawing.Point(7, 48);
            this.comboBoxPreviewPicture.Name = "comboBoxPreviewPicture";
            this.comboBoxPreviewPicture.Size = new System.Drawing.Size(97, 20);
            this.comboBoxPreviewPicture.TabIndex = 2;
            // 
            // textPreviewPicPath
            // 
            this.textPreviewPicPath.Location = new System.Drawing.Point(91, 18);
            this.textPreviewPicPath.Name = "textPreviewPicPath";
            this.textPreviewPicPath.Size = new System.Drawing.Size(139, 21);
            this.textPreviewPicPath.TabIndex = 1;
            // 
            // label_PrePicPath
            // 
            this.label_PrePicPath.AutoSize = true;
            this.label_PrePicPath.Location = new System.Drawing.Point(7, 21);
            this.label_PrePicPath.Name = "label_PrePicPath";
            this.label_PrePicPath.Size = new System.Drawing.Size(77, 12);
            this.label_PrePicPath.TabIndex = 0;
            this.label_PrePicPath.Text = "PicturePath:";
            // 
            // PreviewSound
            // 
            this.PreviewSound.Controls.Add(this.labelPreviewVolume);
            this.PreviewSound.Controls.Add(this.btnPreviewGetVolum);
            this.PreviewSound.Controls.Add(this.btnPreviewSetVolume);
            this.PreviewSound.Controls.Add(this.trackBarPreviewVolume);
            this.PreviewSound.Controls.Add(this.btnPreviewIsOpenSound);
            this.PreviewSound.Controls.Add(this.btnPreviewCloseShareSound);
            this.PreviewSound.Controls.Add(this.btnPreviewOpenShareSound);
            this.PreviewSound.Controls.Add(this.btnPreviewCloseSound);
            this.PreviewSound.Controls.Add(this.btnPreviewOpenSound);
            this.PreviewSound.Location = new System.Drawing.Point(4, 22);
            this.PreviewSound.Name = "PreviewSound";
            this.PreviewSound.Size = new System.Drawing.Size(902, 182);
            this.PreviewSound.TabIndex = 2;
            this.PreviewSound.Text = "Sound";
            this.PreviewSound.UseVisualStyleBackColor = true;
            // 
            // labelPreviewVolume
            // 
            this.labelPreviewVolume.AutoSize = true;
            this.labelPreviewVolume.Location = new System.Drawing.Point(427, 102);
            this.labelPreviewVolume.Name = "labelPreviewVolume";
            this.labelPreviewVolume.Size = new System.Drawing.Size(11, 12);
            this.labelPreviewVolume.TabIndex = 8;
            this.labelPreviewVolume.Text = "0";
            // 
            // btnPreviewGetVolum
            // 
            this.btnPreviewGetVolum.Location = new System.Drawing.Point(591, 87);
            this.btnPreviewGetVolum.Name = "btnPreviewGetVolum";
            this.btnPreviewGetVolum.Size = new System.Drawing.Size(105, 45);
            this.btnPreviewGetVolum.TabIndex = 7;
            this.btnPreviewGetVolum.Text = "GetVolume";
            this.btnPreviewGetVolum.UseVisualStyleBackColor = true;
            this.btnPreviewGetVolum.Click += new System.EventHandler(this.btnPreviewGetVolum_Click);
            // 
            // btnPreviewSetVolume
            // 
            this.btnPreviewSetVolume.Location = new System.Drawing.Point(466, 87);
            this.btnPreviewSetVolume.Name = "btnPreviewSetVolume";
            this.btnPreviewSetVolume.Size = new System.Drawing.Size(105, 45);
            this.btnPreviewSetVolume.TabIndex = 6;
            this.btnPreviewSetVolume.Text = "SetVolume";
            this.btnPreviewSetVolume.UseVisualStyleBackColor = true;
            this.btnPreviewSetVolume.Click += new System.EventHandler(this.btnPreviewSetVolume_Click);
            // 
            // trackBarPreviewVolume
            // 
            this.trackBarPreviewVolume.Location = new System.Drawing.Point(18, 87);
            this.trackBarPreviewVolume.Maximum = 65535;
            this.trackBarPreviewVolume.Name = "trackBarPreviewVolume";
            this.trackBarPreviewVolume.Size = new System.Drawing.Size(402, 40);
            this.trackBarPreviewVolume.TabIndex = 5;
            this.trackBarPreviewVolume.ValueChanged += new System.EventHandler(this.trackBarPreviewVolume_ValueChanged);
            // 
            // btnPreviewIsOpenSound
            // 
            this.btnPreviewIsOpenSound.Location = new System.Drawing.Point(510, 15);
            this.btnPreviewIsOpenSound.Name = "btnPreviewIsOpenSound";
            this.btnPreviewIsOpenSound.Size = new System.Drawing.Size(97, 36);
            this.btnPreviewIsOpenSound.TabIndex = 4;
            this.btnPreviewIsOpenSound.Text = "IsOpenSound";
            this.btnPreviewIsOpenSound.UseVisualStyleBackColor = true;
            this.btnPreviewIsOpenSound.Click += new System.EventHandler(this.btnPreviewIsOpenSound_Click);
            // 
            // btnPreviewCloseShareSound
            // 
            this.btnPreviewCloseShareSound.Location = new System.Drawing.Point(387, 15);
            this.btnPreviewCloseShareSound.Name = "btnPreviewCloseShareSound";
            this.btnPreviewCloseShareSound.Size = new System.Drawing.Size(97, 36);
            this.btnPreviewCloseShareSound.TabIndex = 3;
            this.btnPreviewCloseShareSound.Text = "CloseShare Sound";
            this.btnPreviewCloseShareSound.UseVisualStyleBackColor = true;
            this.btnPreviewCloseShareSound.Click += new System.EventHandler(this.btnPreviewCloseShareSound_Click);
            // 
            // btnPreviewOpenShareSound
            // 
            this.btnPreviewOpenShareSound.Location = new System.Drawing.Point(264, 15);
            this.btnPreviewOpenShareSound.Name = "btnPreviewOpenShareSound";
            this.btnPreviewOpenShareSound.Size = new System.Drawing.Size(97, 36);
            this.btnPreviewOpenShareSound.TabIndex = 2;
            this.btnPreviewOpenShareSound.Text = "OpenShare Sound";
            this.btnPreviewOpenShareSound.UseVisualStyleBackColor = true;
            this.btnPreviewOpenShareSound.Click += new System.EventHandler(this.btnPreviewOpenShareSound_Click);
            // 
            // btnPreviewCloseSound
            // 
            this.btnPreviewCloseSound.Location = new System.Drawing.Point(141, 15);
            this.btnPreviewCloseSound.Name = "btnPreviewCloseSound";
            this.btnPreviewCloseSound.Size = new System.Drawing.Size(97, 36);
            this.btnPreviewCloseSound.TabIndex = 1;
            this.btnPreviewCloseSound.Text = "CloseSound";
            this.btnPreviewCloseSound.UseVisualStyleBackColor = true;
            this.btnPreviewCloseSound.Click += new System.EventHandler(this.btnPreviewCloseSound_Click);
            // 
            // btnPreviewOpenSound
            // 
            this.btnPreviewOpenSound.Location = new System.Drawing.Point(18, 15);
            this.btnPreviewOpenSound.Name = "btnPreviewOpenSound";
            this.btnPreviewOpenSound.Size = new System.Drawing.Size(97, 36);
            this.btnPreviewOpenSound.TabIndex = 0;
            this.btnPreviewOpenSound.Text = "OpenSound";
            this.btnPreviewOpenSound.UseVisualStyleBackColor = true;
            this.btnPreviewOpenSound.Click += new System.EventHandler(this.btnPreviewOpenSound_Click);
            // 
            // PreviewRecord
            // 
            this.PreviewRecord.Controls.Add(this.btnPreviewIsRecordState);
            this.PreviewRecord.Controls.Add(this.btnPreviewStopRecord);
            this.PreviewRecord.Controls.Add(this.btnPreviewStartRecord);
            this.PreviewRecord.Controls.Add(this.btnPreviewSetSplitLen);
            this.PreviewRecord.Controls.Add(this.textPreviewSplitLen);
            this.PreviewRecord.Controls.Add(this.label_PreSplitLen);
            this.PreviewRecord.Controls.Add(this.textPreviewRecordPath);
            this.PreviewRecord.Controls.Add(this.label_PreRecPath);
            this.PreviewRecord.Location = new System.Drawing.Point(4, 22);
            this.PreviewRecord.Name = "PreviewRecord";
            this.PreviewRecord.Size = new System.Drawing.Size(902, 182);
            this.PreviewRecord.TabIndex = 3;
            this.PreviewRecord.Text = "Record";
            this.PreviewRecord.UseVisualStyleBackColor = true;
            // 
            // btnPreviewIsRecordState
            // 
            this.btnPreviewIsRecordState.Location = new System.Drawing.Point(291, 102);
            this.btnPreviewIsRecordState.Name = "btnPreviewIsRecordState";
            this.btnPreviewIsRecordState.Size = new System.Drawing.Size(101, 39);
            this.btnPreviewIsRecordState.TabIndex = 7;
            this.btnPreviewIsRecordState.Text = "IsRecordState";
            this.btnPreviewIsRecordState.UseVisualStyleBackColor = true;
            this.btnPreviewIsRecordState.Click += new System.EventHandler(this.btnPreviewIsRecordState_Click);
            // 
            // btnPreviewStopRecord
            // 
            this.btnPreviewStopRecord.Location = new System.Drawing.Point(161, 102);
            this.btnPreviewStopRecord.Name = "btnPreviewStopRecord";
            this.btnPreviewStopRecord.Size = new System.Drawing.Size(101, 39);
            this.btnPreviewStopRecord.TabIndex = 6;
            this.btnPreviewStopRecord.Text = "StopRecord";
            this.btnPreviewStopRecord.UseVisualStyleBackColor = true;
            this.btnPreviewStopRecord.Click += new System.EventHandler(this.btnPreviewStopRecord_Click);
            // 
            // btnPreviewStartRecord
            // 
            this.btnPreviewStartRecord.Location = new System.Drawing.Point(31, 102);
            this.btnPreviewStartRecord.Name = "btnPreviewStartRecord";
            this.btnPreviewStartRecord.Size = new System.Drawing.Size(101, 39);
            this.btnPreviewStartRecord.TabIndex = 5;
            this.btnPreviewStartRecord.Text = "StartRecord";
            this.btnPreviewStartRecord.UseVisualStyleBackColor = true;
            this.btnPreviewStartRecord.Click += new System.EventHandler(this.btnPreviewStartRecord_Click);
            // 
            // btnPreviewSetSplitLen
            // 
            this.btnPreviewSetSplitLen.Location = new System.Drawing.Point(259, 53);
            this.btnPreviewSetSplitLen.Name = "btnPreviewSetSplitLen";
            this.btnPreviewSetSplitLen.Size = new System.Drawing.Size(100, 23);
            this.btnPreviewSetSplitLen.TabIndex = 4;
            this.btnPreviewSetSplitLen.Text = "SetSplitLen";
            this.btnPreviewSetSplitLen.UseVisualStyleBackColor = true;
            this.btnPreviewSetSplitLen.Click += new System.EventHandler(this.btnPreviewSetSplitLen_Click);
            // 
            // textPreviewSplitLen
            // 
            this.textPreviewSplitLen.Location = new System.Drawing.Point(106, 56);
            this.textPreviewSplitLen.Name = "textPreviewSplitLen";
            this.textPreviewSplitLen.Size = new System.Drawing.Size(131, 21);
            this.textPreviewSplitLen.TabIndex = 3;
            this.textPreviewSplitLen.Text = "100";
            // 
            // label_PreSplitLen
            // 
            this.label_PreSplitLen.AutoSize = true;
            this.label_PreSplitLen.Location = new System.Drawing.Point(31, 58);
            this.label_PreSplitLen.Name = "label_PreSplitLen";
            this.label_PreSplitLen.Size = new System.Drawing.Size(59, 12);
            this.label_PreSplitLen.TabIndex = 2;
            this.label_PreSplitLen.Text = "SplitLen:";
            // 
            // textPreviewRecordPath
            // 
            this.textPreviewRecordPath.Location = new System.Drawing.Point(106, 21);
            this.textPreviewRecordPath.Name = "textPreviewRecordPath";
            this.textPreviewRecordPath.Size = new System.Drawing.Size(273, 21);
            this.textPreviewRecordPath.TabIndex = 1;
            this.textPreviewRecordPath.Text = "D:\\test";
            // 
            // label_PreRecPath
            // 
            this.label_PreRecPath.AutoSize = true;
            this.label_PreRecPath.Location = new System.Drawing.Point(29, 24);
            this.label_PreRecPath.Name = "label_PreRecPath";
            this.label_PreRecPath.Size = new System.Drawing.Size(71, 12);
            this.label_PreRecPath.TabIndex = 0;
            this.label_PreRecPath.Text = "RecordPath:";
            // 
            // DeviceFunction
            // 
            this.DeviceFunction.Controls.Add(this.tabControl_PTZ);
            this.DeviceFunction.Controls.Add(this.btnPtzOpClear);
            this.DeviceFunction.Controls.Add(this.textPtzOpResult);
            this.DeviceFunction.Controls.Add(this.textPtzChannelId);
            this.DeviceFunction.Controls.Add(this.label_PTZChnlCode);
            this.DeviceFunction.Location = new System.Drawing.Point(124, 4);
            this.DeviceFunction.Name = "DeviceFunction";
            this.DeviceFunction.Size = new System.Drawing.Size(917, 641);
            this.DeviceFunction.TabIndex = 4;
            this.DeviceFunction.Text = "DeviceFunction";
            this.DeviceFunction.UseVisualStyleBackColor = true;
            // 
            // tabControl_PTZ
            // 
            this.tabControl_PTZ.Controls.Add(this.tabPage_PTZOpFun);
            this.tabControl_PTZ.Controls.Add(this.tabPageOPCamera);
            this.tabControl_PTZ.Controls.Add(this.tabPagePreset);
            this.tabControl_PTZ.Controls.Add(this.tabPagePTZOther);
            this.tabControl_PTZ.Location = new System.Drawing.Point(12, 47);
            this.tabControl_PTZ.Name = "tabControl_PTZ";
            this.tabControl_PTZ.SelectedIndex = 0;
            this.tabControl_PTZ.Size = new System.Drawing.Size(898, 364);
            this.tabControl_PTZ.TabIndex = 12;
            // 
            // tabPage_PTZOpFun
            // 
            this.tabPage_PTZOpFun.Controls.Add(this.groupBox_PTZFun);
            this.tabPage_PTZOpFun.Location = new System.Drawing.Point(4, 22);
            this.tabPage_PTZOpFun.Name = "tabPage_PTZOpFun";
            this.tabPage_PTZOpFun.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage_PTZOpFun.Size = new System.Drawing.Size(890, 338);
            this.tabPage_PTZOpFun.TabIndex = 0;
            this.tabPage_PTZOpFun.Text = "Function";
            this.tabPage_PTZOpFun.UseVisualStyleBackColor = true;
            // 
            // groupBox_PTZFun
            // 
            this.groupBox_PTZFun.Controls.Add(this.btnPtzOperateFunction);
            this.groupBox_PTZFun.Controls.Add(this.comboBoxPtzSwitchMenu);
            this.groupBox_PTZFun.Controls.Add(this.label_PTZSM);
            this.groupBox_PTZFun.Controls.Add(this.comboBoxPtzMoveType);
            this.groupBox_PTZFun.Controls.Add(this.label_PTZMT);
            this.groupBox_PTZFun.Controls.Add(this.comboBoxPtzAssisentType);
            this.groupBox_PTZFun.Controls.Add(this.label_PTZAT);
            this.groupBox_PTZFun.Controls.Add(this.comboBoxPtzBorderType);
            this.groupBox_PTZFun.Controls.Add(this.label_PTZBT);
            this.groupBox_PTZFun.Controls.Add(this.comboBoxPtzSwitch);
            this.groupBox_PTZFun.Controls.Add(this.label_PTZSwith);
            this.groupBox_PTZFun.Controls.Add(this.textPtzTrackId);
            this.groupBox_PTZFun.Controls.Add(this.label_PTZTraclID);
            this.groupBox_PTZFun.Controls.Add(this.textPtzCruiseId);
            this.groupBox_PTZFun.Controls.Add(this.label_PTZCruID);
            this.groupBox_PTZFun.Controls.Add(this.comboBoxPtzOpType);
            this.groupBox_PTZFun.Controls.Add(this.label_PTZOpType);
            this.groupBox_PTZFun.Location = new System.Drawing.Point(16, 21);
            this.groupBox_PTZFun.Name = "groupBox_PTZFun";
            this.groupBox_PTZFun.Size = new System.Drawing.Size(772, 162);
            this.groupBox_PTZFun.TabIndex = 2;
            this.groupBox_PTZFun.TabStop = false;
            this.groupBox_PTZFun.Text = "PtzOperateFunction";
            // 
            // btnPtzOperateFunction
            // 
            this.btnPtzOperateFunction.Location = new System.Drawing.Point(10, 124);
            this.btnPtzOperateFunction.Name = "btnPtzOperateFunction";
            this.btnPtzOperateFunction.Size = new System.Drawing.Size(107, 27);
            this.btnPtzOperateFunction.TabIndex = 16;
            this.btnPtzOperateFunction.Text = "OperateFunction";
            this.btnPtzOperateFunction.UseVisualStyleBackColor = true;
            this.btnPtzOperateFunction.Click += new System.EventHandler(this.btnPtzOperateFunction_Click);
            // 
            // comboBoxPtzSwitchMenu
            // 
            this.comboBoxPtzSwitchMenu.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBoxPtzSwitchMenu.FormattingEnabled = true;
            this.comboBoxPtzSwitchMenu.Items.AddRange(new object[] {
            "22-Open the platform menu",
            "23-Close the platform menu"});
            this.comboBoxPtzSwitchMenu.Location = new System.Drawing.Point(421, 88);
            this.comboBoxPtzSwitchMenu.Name = "comboBoxPtzSwitchMenu";
            this.comboBoxPtzSwitchMenu.Size = new System.Drawing.Size(165, 20);
            this.comboBoxPtzSwitchMenu.TabIndex = 15;
            // 
            // label_PTZSM
            // 
            this.label_PTZSM.AutoSize = true;
            this.label_PTZSM.Location = new System.Drawing.Point(419, 72);
            this.label_PTZSM.Name = "label_PTZSM";
            this.label_PTZSM.Size = new System.Drawing.Size(65, 12);
            this.label_PTZSM.TabIndex = 14;
            this.label_PTZSM.Text = "SwitchMenu";
            // 
            // comboBoxPtzMoveType
            // 
            this.comboBoxPtzMoveType.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBoxPtzMoveType.FormattingEnabled = true;
            this.comboBoxPtzMoveType.Items.AddRange(new object[] {
            "25-Upward movement",
            "26-Move down",
            "27-Left shift",
            "28-Right shift"});
            this.comboBoxPtzMoveType.Location = new System.Drawing.Point(185, 88);
            this.comboBoxPtzMoveType.Name = "comboBoxPtzMoveType";
            this.comboBoxPtzMoveType.Size = new System.Drawing.Size(215, 20);
            this.comboBoxPtzMoveType.TabIndex = 13;
            // 
            // label_PTZMT
            // 
            this.label_PTZMT.AutoSize = true;
            this.label_PTZMT.Location = new System.Drawing.Point(183, 72);
            this.label_PTZMT.Name = "label_PTZMT";
            this.label_PTZMT.Size = new System.Drawing.Size(53, 12);
            this.label_PTZMT.TabIndex = 12;
            this.label_PTZMT.Text = "MoveType";
            // 
            // comboBoxPtzAssisentType
            // 
            this.comboBoxPtzAssisentType.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBoxPtzAssisentType.FormattingEnabled = true;
            this.comboBoxPtzAssisentType.Items.AddRange(new object[] {
            "23-Backlight compensation",
            "24-Number doubled",
            "27-Color turn black",
            "35-Shutter time",
            "41-Brightness",
            "42-Image flip",
            "43-Hidding preset point\'s name",
            "80-Restore factory settings"});
            this.comboBoxPtzAssisentType.Location = new System.Drawing.Point(10, 88);
            this.comboBoxPtzAssisentType.Name = "comboBoxPtzAssisentType";
            this.comboBoxPtzAssisentType.Size = new System.Drawing.Size(153, 20);
            this.comboBoxPtzAssisentType.TabIndex = 11;
            // 
            // label_PTZAT
            // 
            this.label_PTZAT.AutoSize = true;
            this.label_PTZAT.Location = new System.Drawing.Point(8, 72);
            this.label_PTZAT.Name = "label_PTZAT";
            this.label_PTZAT.Size = new System.Drawing.Size(77, 12);
            this.label_PTZAT.TabIndex = 10;
            this.label_PTZAT.Text = "AssisentType";
            // 
            // comboBoxPtzBorderType
            // 
            this.comboBoxPtzBorderType.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBoxPtzBorderType.FormattingEnabled = true;
            this.comboBoxPtzBorderType.Items.AddRange(new object[] {
            "16-Left boundary",
            "17-Right boundary"});
            this.comboBoxPtzBorderType.Location = new System.Drawing.Point(605, 37);
            this.comboBoxPtzBorderType.Name = "comboBoxPtzBorderType";
            this.comboBoxPtzBorderType.Size = new System.Drawing.Size(152, 20);
            this.comboBoxPtzBorderType.TabIndex = 9;
            // 
            // label_PTZBT
            // 
            this.label_PTZBT.AutoSize = true;
            this.label_PTZBT.Location = new System.Drawing.Point(603, 21);
            this.label_PTZBT.Name = "label_PTZBT";
            this.label_PTZBT.Size = new System.Drawing.Size(65, 12);
            this.label_PTZBT.TabIndex = 8;
            this.label_PTZBT.Text = "BorderType";
            // 
            // comboBoxPtzSwitch
            // 
            this.comboBoxPtzSwitch.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBoxPtzSwitch.FormattingEnabled = true;
            this.comboBoxPtzSwitch.Items.AddRange(new object[] {
            "0-Close",
            "1-Open"});
            this.comboBoxPtzSwitch.Location = new System.Drawing.Point(421, 38);
            this.comboBoxPtzSwitch.Name = "comboBoxPtzSwitch";
            this.comboBoxPtzSwitch.Size = new System.Drawing.Size(165, 20);
            this.comboBoxPtzSwitch.TabIndex = 7;
            // 
            // label_PTZSwith
            // 
            this.label_PTZSwith.AutoSize = true;
            this.label_PTZSwith.Location = new System.Drawing.Point(429, 21);
            this.label_PTZSwith.Name = "label_PTZSwith";
            this.label_PTZSwith.Size = new System.Drawing.Size(41, 12);
            this.label_PTZSwith.TabIndex = 6;
            this.label_PTZSwith.Text = "Switch";
            // 
            // textPtzTrackId
            // 
            this.textPtzTrackId.Location = new System.Drawing.Point(302, 37);
            this.textPtzTrackId.Name = "textPtzTrackId";
            this.textPtzTrackId.Size = new System.Drawing.Size(98, 21);
            this.textPtzTrackId.TabIndex = 5;
            // 
            // label_PTZTraclID
            // 
            this.label_PTZTraclID.AutoSize = true;
            this.label_PTZTraclID.Location = new System.Drawing.Point(301, 21);
            this.label_PTZTraclID.Name = "label_PTZTraclID";
            this.label_PTZTraclID.Size = new System.Drawing.Size(47, 12);
            this.label_PTZTraclID.TabIndex = 4;
            this.label_PTZTraclID.Text = "TrackId";
            // 
            // textPtzCruiseId
            // 
            this.textPtzCruiseId.Location = new System.Drawing.Point(185, 37);
            this.textPtzCruiseId.Name = "textPtzCruiseId";
            this.textPtzCruiseId.Size = new System.Drawing.Size(96, 21);
            this.textPtzCruiseId.TabIndex = 3;
            // 
            // label_PTZCruID
            // 
            this.label_PTZCruID.AutoSize = true;
            this.label_PTZCruID.Location = new System.Drawing.Point(185, 21);
            this.label_PTZCruID.Name = "label_PTZCruID";
            this.label_PTZCruID.Size = new System.Drawing.Size(53, 12);
            this.label_PTZCruID.TabIndex = 2;
            this.label_PTZCruID.Text = "CruiseId";
            // 
            // comboBoxPtzOpType
            // 
            this.comboBoxPtzOpType.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBoxPtzOpType.FormattingEnabled = true;
            this.comboBoxPtzOpType.Items.AddRange(new object[] {
            "0-Show_PtzMenu",
            "1-Move_PtzMenu",
            "2-Confirm_PtzMenuItem",
            "3-Set_LineScannBorder",
            "4-Switch_LineScanBorder",
            "5-Switch_AutoRotate",
            "6-Switch_Light",
            "7-Switch_RainBrush",
            "8-Switch_InfraredLight",
            "9-Switch_AssisentPoint",
            "10-Switch_Cruise",
            "11-Switch_Track",
            "12-Switch_SetTrack"});
            this.comboBoxPtzOpType.Location = new System.Drawing.Point(10, 38);
            this.comboBoxPtzOpType.Name = "comboBoxPtzOpType";
            this.comboBoxPtzOpType.Size = new System.Drawing.Size(153, 20);
            this.comboBoxPtzOpType.TabIndex = 1;
            // 
            // label_PTZOpType
            // 
            this.label_PTZOpType.AutoSize = true;
            this.label_PTZOpType.Location = new System.Drawing.Point(8, 21);
            this.label_PTZOpType.Name = "label_PTZOpType";
            this.label_PTZOpType.Size = new System.Drawing.Size(71, 12);
            this.label_PTZOpType.TabIndex = 0;
            this.label_PTZOpType.Text = "OperateType";
            // 
            // tabPageOPCamera
            // 
            this.tabPageOPCamera.Controls.Add(this.groupBoxPTZDir);
            this.tabPageOPCamera.Controls.Add(this.groupBox_PTZOpCam);
            this.tabPageOPCamera.Controls.Add(this.groupBox_PTZFocus);
            this.tabPageOPCamera.Location = new System.Drawing.Point(4, 22);
            this.tabPageOPCamera.Name = "tabPageOPCamera";
            this.tabPageOPCamera.Padding = new System.Windows.Forms.Padding(3);
            this.tabPageOPCamera.Size = new System.Drawing.Size(890, 338);
            this.tabPageOPCamera.TabIndex = 1;
            this.tabPageOPCamera.Text = "Camera";
            this.tabPageOPCamera.UseVisualStyleBackColor = true;
            // 
            // groupBoxPTZDir
            // 
            this.groupBoxPTZDir.Controls.Add(this.btnPtzOpDirect);
            this.groupBoxPTZDir.Controls.Add(this.textPtzOpDirectExtend);
            this.groupBoxPTZDir.Controls.Add(this.label_PTZDorEx);
            this.groupBoxPTZDir.Controls.Add(this.textPtzOpDirectStepY);
            this.groupBoxPTZDir.Controls.Add(this.label_PTZStepY);
            this.groupBoxPTZDir.Controls.Add(this.textPtzOpDirectStepX);
            this.groupBoxPTZDir.Controls.Add(this.label_PTZStepX);
            this.groupBoxPTZDir.Controls.Add(this.comboBoxPtzOpDirectCommand);
            this.groupBoxPTZDir.Controls.Add(this.label_PTZDirCom);
            this.groupBoxPTZDir.Controls.Add(this.comboBoxPtzOpDirect);
            this.groupBoxPTZDir.Controls.Add(this.label_PTZDir);
            this.groupBoxPTZDir.Location = new System.Drawing.Point(15, 99);
            this.groupBoxPTZDir.Name = "groupBoxPTZDir";
            this.groupBoxPTZDir.Size = new System.Drawing.Size(817, 64);
            this.groupBoxPTZDir.TabIndex = 4;
            this.groupBoxPTZDir.TabStop = false;
            this.groupBoxPTZDir.Text = "PtzOperateDirect";
            // 
            // btnPtzOpDirect
            // 
            this.btnPtzOpDirect.Location = new System.Drawing.Point(682, 33);
            this.btnPtzOpDirect.Name = "btnPtzOpDirect";
            this.btnPtzOpDirect.Size = new System.Drawing.Size(108, 23);
            this.btnPtzOpDirect.TabIndex = 10;
            this.btnPtzOpDirect.Text = "OperateDirect";
            this.btnPtzOpDirect.UseVisualStyleBackColor = true;
            this.btnPtzOpDirect.Click += new System.EventHandler(this.btnPtzOpDirect_Click);
            // 
            // textPtzOpDirectExtend
            // 
            this.textPtzOpDirectExtend.Location = new System.Drawing.Point(395, 35);
            this.textPtzOpDirectExtend.Name = "textPtzOpDirectExtend";
            this.textPtzOpDirectExtend.Size = new System.Drawing.Size(213, 21);
            this.textPtzOpDirectExtend.TabIndex = 9;
            // 
            // label_PTZDorEx
            // 
            this.label_PTZDorEx.AutoSize = true;
            this.label_PTZDorEx.Location = new System.Drawing.Point(393, 21);
            this.label_PTZDorEx.Name = "label_PTZDorEx";
            this.label_PTZDorEx.Size = new System.Drawing.Size(41, 12);
            this.label_PTZDorEx.TabIndex = 8;
            this.label_PTZDorEx.Text = "Extend";
            // 
            // textPtzOpDirectStepY
            // 
            this.textPtzOpDirectStepY.Location = new System.Drawing.Point(326, 36);
            this.textPtzOpDirectStepY.Name = "textPtzOpDirectStepY";
            this.textPtzOpDirectStepY.Size = new System.Drawing.Size(55, 21);
            this.textPtzOpDirectStepY.TabIndex = 7;
            // 
            // label_PTZStepY
            // 
            this.label_PTZStepY.AutoSize = true;
            this.label_PTZStepY.Location = new System.Drawing.Point(324, 21);
            this.label_PTZStepY.Name = "label_PTZStepY";
            this.label_PTZStepY.Size = new System.Drawing.Size(35, 12);
            this.label_PTZStepY.TabIndex = 6;
            this.label_PTZStepY.Text = "StepY";
            // 
            // textPtzOpDirectStepX
            // 
            this.textPtzOpDirectStepX.Location = new System.Drawing.Point(254, 36);
            this.textPtzOpDirectStepX.Name = "textPtzOpDirectStepX";
            this.textPtzOpDirectStepX.Size = new System.Drawing.Size(60, 21);
            this.textPtzOpDirectStepX.TabIndex = 5;
            // 
            // label_PTZStepX
            // 
            this.label_PTZStepX.AutoSize = true;
            this.label_PTZStepX.Location = new System.Drawing.Point(252, 21);
            this.label_PTZStepX.Name = "label_PTZStepX";
            this.label_PTZStepX.Size = new System.Drawing.Size(35, 12);
            this.label_PTZStepX.TabIndex = 4;
            this.label_PTZStepX.Text = "StepX";
            // 
            // comboBoxPtzOpDirectCommand
            // 
            this.comboBoxPtzOpDirectCommand.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBoxPtzOpDirectCommand.FormattingEnabled = true;
            this.comboBoxPtzOpDirectCommand.Items.AddRange(new object[] {
            "0-Stop",
            "1-Open"});
            this.comboBoxPtzOpDirectCommand.Location = new System.Drawing.Point(132, 37);
            this.comboBoxPtzOpDirectCommand.Name = "comboBoxPtzOpDirectCommand";
            this.comboBoxPtzOpDirectCommand.Size = new System.Drawing.Size(104, 20);
            this.comboBoxPtzOpDirectCommand.TabIndex = 3;
            // 
            // label_PTZDirCom
            // 
            this.label_PTZDirCom.AutoSize = true;
            this.label_PTZDirCom.Location = new System.Drawing.Point(132, 21);
            this.label_PTZDirCom.Name = "label_PTZDirCom";
            this.label_PTZDirCom.Size = new System.Drawing.Size(47, 12);
            this.label_PTZDirCom.TabIndex = 2;
            this.label_PTZDirCom.Text = "Command";
            // 
            // comboBoxPtzOpDirect
            // 
            this.comboBoxPtzOpDirect.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBoxPtzOpDirect.FormattingEnabled = true;
            this.comboBoxPtzOpDirect.Items.AddRange(new object[] {
            "1-Upper",
            "2-Under",
            "3-Left",
            "4-Right",
            "5-Upper Left",
            "6-Under Left",
            "7-Upper Right",
            "8-Under Right"});
            this.comboBoxPtzOpDirect.Location = new System.Drawing.Point(9, 37);
            this.comboBoxPtzOpDirect.Name = "comboBoxPtzOpDirect";
            this.comboBoxPtzOpDirect.Size = new System.Drawing.Size(116, 20);
            this.comboBoxPtzOpDirect.TabIndex = 1;
            // 
            // label_PTZDir
            // 
            this.label_PTZDir.AutoSize = true;
            this.label_PTZDir.Location = new System.Drawing.Point(7, 21);
            this.label_PTZDir.Name = "label_PTZDir";
            this.label_PTZDir.Size = new System.Drawing.Size(41, 12);
            this.label_PTZDir.TabIndex = 0;
            this.label_PTZDir.Text = "Direct";
            // 
            // groupBox_PTZOpCam
            // 
            this.groupBox_PTZOpCam.Controls.Add(this.btnPtzOpCamera);
            this.groupBox_PTZOpCam.Controls.Add(this.textPtzOpCameraExtend);
            this.groupBox_PTZOpCam.Controls.Add(this.label_PTZCamEx);
            this.groupBox_PTZOpCam.Controls.Add(this.textPtzOpCameraStep);
            this.groupBox_PTZOpCam.Controls.Add(this.label_PTZStep);
            this.groupBox_PTZOpCam.Controls.Add(this.comboBoxPtzOpCameraType);
            this.groupBox_PTZOpCam.Controls.Add(this.label_PTZOpT);
            this.groupBox_PTZOpCam.Controls.Add(this.comboBoxPtzOpCameraCommand);
            this.groupBox_PTZOpCam.Controls.Add(this.label_PTZCom);
            this.groupBox_PTZOpCam.Controls.Add(this.comboBoxPtzOpCameraDirect);
            this.groupBox_PTZOpCam.Controls.Add(this.label_PTZCamDir);
            this.groupBox_PTZOpCam.Location = new System.Drawing.Point(15, 17);
            this.groupBox_PTZOpCam.Name = "groupBox_PTZOpCam";
            this.groupBox_PTZOpCam.Size = new System.Drawing.Size(817, 64);
            this.groupBox_PTZOpCam.TabIndex = 3;
            this.groupBox_PTZOpCam.TabStop = false;
            this.groupBox_PTZOpCam.Text = "PtzOperateCamera";
            // 
            // btnPtzOpCamera
            // 
            this.btnPtzOpCamera.Location = new System.Drawing.Point(682, 34);
            this.btnPtzOpCamera.Name = "btnPtzOpCamera";
            this.btnPtzOpCamera.Size = new System.Drawing.Size(108, 23);
            this.btnPtzOpCamera.TabIndex = 10;
            this.btnPtzOpCamera.Text = "OperateCamera";
            this.btnPtzOpCamera.UseVisualStyleBackColor = true;
            this.btnPtzOpCamera.Click += new System.EventHandler(this.btnPtzOpCamera_Click);
            // 
            // textPtzOpCameraExtend
            // 
            this.textPtzOpCameraExtend.Location = new System.Drawing.Point(468, 36);
            this.textPtzOpCameraExtend.Name = "textPtzOpCameraExtend";
            this.textPtzOpCameraExtend.Size = new System.Drawing.Size(140, 21);
            this.textPtzOpCameraExtend.TabIndex = 9;
            // 
            // label_PTZCamEx
            // 
            this.label_PTZCamEx.AutoSize = true;
            this.label_PTZCamEx.Location = new System.Drawing.Point(466, 21);
            this.label_PTZCamEx.Name = "label_PTZCamEx";
            this.label_PTZCamEx.Size = new System.Drawing.Size(41, 12);
            this.label_PTZCamEx.TabIndex = 8;
            this.label_PTZCamEx.Text = "Extend";
            // 
            // textPtzOpCameraStep
            // 
            this.textPtzOpCameraStep.Location = new System.Drawing.Point(395, 36);
            this.textPtzOpCameraStep.Name = "textPtzOpCameraStep";
            this.textPtzOpCameraStep.Size = new System.Drawing.Size(63, 21);
            this.textPtzOpCameraStep.TabIndex = 7;
            // 
            // label_PTZStep
            // 
            this.label_PTZStep.AutoSize = true;
            this.label_PTZStep.Location = new System.Drawing.Point(395, 21);
            this.label_PTZStep.Name = "label_PTZStep";
            this.label_PTZStep.Size = new System.Drawing.Size(29, 12);
            this.label_PTZStep.TabIndex = 6;
            this.label_PTZStep.Text = "Step";
            // 
            // comboBoxPtzOpCameraType
            // 
            this.comboBoxPtzOpCameraType.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBoxPtzOpCameraType.FormattingEnabled = true;
            this.comboBoxPtzOpCameraType.Items.AddRange(new object[] {
            "1-Variable",
            "2-Zoom",
            "3-Aperture"});
            this.comboBoxPtzOpCameraType.Location = new System.Drawing.Point(254, 37);
            this.comboBoxPtzOpCameraType.Name = "comboBoxPtzOpCameraType";
            this.comboBoxPtzOpCameraType.Size = new System.Drawing.Size(127, 20);
            this.comboBoxPtzOpCameraType.TabIndex = 5;
            // 
            // label_PTZOpT
            // 
            this.label_PTZOpT.AutoSize = true;
            this.label_PTZOpT.Location = new System.Drawing.Point(254, 21);
            this.label_PTZOpT.Name = "label_PTZOpT";
            this.label_PTZOpT.Size = new System.Drawing.Size(71, 12);
            this.label_PTZOpT.TabIndex = 4;
            this.label_PTZOpT.Text = "OperateType";
            // 
            // comboBoxPtzOpCameraCommand
            // 
            this.comboBoxPtzOpCameraCommand.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBoxPtzOpCameraCommand.FormattingEnabled = true;
            this.comboBoxPtzOpCameraCommand.Items.AddRange(new object[] {
            "0-Stop",
            "1-Open"});
            this.comboBoxPtzOpCameraCommand.Location = new System.Drawing.Point(134, 37);
            this.comboBoxPtzOpCameraCommand.Name = "comboBoxPtzOpCameraCommand";
            this.comboBoxPtzOpCameraCommand.Size = new System.Drawing.Size(102, 20);
            this.comboBoxPtzOpCameraCommand.TabIndex = 3;
            // 
            // label_PTZCom
            // 
            this.label_PTZCom.AutoSize = true;
            this.label_PTZCom.Location = new System.Drawing.Point(132, 21);
            this.label_PTZCom.Name = "label_PTZCom";
            this.label_PTZCom.Size = new System.Drawing.Size(47, 12);
            this.label_PTZCom.TabIndex = 2;
            this.label_PTZCom.Text = "Command";
            // 
            // comboBoxPtzOpCameraDirect
            // 
            this.comboBoxPtzOpCameraDirect.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBoxPtzOpCameraDirect.FormattingEnabled = true;
            this.comboBoxPtzOpCameraDirect.Items.AddRange(new object[] {
            "1-Increase",
            "2-Decrease"});
            this.comboBoxPtzOpCameraDirect.Location = new System.Drawing.Point(11, 37);
            this.comboBoxPtzOpCameraDirect.Name = "comboBoxPtzOpCameraDirect";
            this.comboBoxPtzOpCameraDirect.Size = new System.Drawing.Size(114, 20);
            this.comboBoxPtzOpCameraDirect.TabIndex = 1;
            // 
            // label_PTZCamDir
            // 
            this.label_PTZCamDir.AutoSize = true;
            this.label_PTZCamDir.Location = new System.Drawing.Point(9, 21);
            this.label_PTZCamDir.Name = "label_PTZCamDir";
            this.label_PTZCamDir.Size = new System.Drawing.Size(41, 12);
            this.label_PTZCamDir.TabIndex = 0;
            this.label_PTZCamDir.Text = "Direct";
            // 
            // groupBox_PTZFocus
            // 
            this.groupBox_PTZFocus.Controls.Add(this.btnPtzOpFocus);
            this.groupBox_PTZFocus.Controls.Add(this.textPtzOpFocusZoom);
            this.groupBox_PTZFocus.Controls.Add(this.label_PTZForZoom);
            this.groupBox_PTZFocus.Controls.Add(this.textPtzOpFocus);
            this.groupBox_PTZFocus.Controls.Add(this.label_PTZFor);
            this.groupBox_PTZFocus.Controls.Add(this.comboBoxPtzOpFocusType);
            this.groupBox_PTZFocus.Controls.Add(this.label_PTZForOpType);
            this.groupBox_PTZFocus.Location = new System.Drawing.Point(15, 178);
            this.groupBox_PTZFocus.Name = "groupBox_PTZFocus";
            this.groupBox_PTZFocus.Size = new System.Drawing.Size(458, 65);
            this.groupBox_PTZFocus.TabIndex = 5;
            this.groupBox_PTZFocus.TabStop = false;
            this.groupBox_PTZFocus.Text = "PtzOperateFocus";
            // 
            // btnPtzOpFocus
            // 
            this.btnPtzOpFocus.Location = new System.Drawing.Point(348, 37);
            this.btnPtzOpFocus.Name = "btnPtzOpFocus";
            this.btnPtzOpFocus.Size = new System.Drawing.Size(86, 21);
            this.btnPtzOpFocus.TabIndex = 6;
            this.btnPtzOpFocus.Text = "OperateFocus";
            this.btnPtzOpFocus.UseVisualStyleBackColor = true;
            this.btnPtzOpFocus.Click += new System.EventHandler(this.btnPtzOpFocus_Click);
            // 
            // textPtzOpFocusZoom
            // 
            this.textPtzOpFocusZoom.Location = new System.Drawing.Point(219, 35);
            this.textPtzOpFocusZoom.Name = "textPtzOpFocusZoom";
            this.textPtzOpFocusZoom.Size = new System.Drawing.Size(68, 21);
            this.textPtzOpFocusZoom.TabIndex = 5;
            // 
            // label_PTZForZoom
            // 
            this.label_PTZForZoom.AutoSize = true;
            this.label_PTZForZoom.Location = new System.Drawing.Point(219, 20);
            this.label_PTZForZoom.Name = "label_PTZForZoom";
            this.label_PTZForZoom.Size = new System.Drawing.Size(29, 12);
            this.label_PTZForZoom.TabIndex = 4;
            this.label_PTZForZoom.Text = "Zoom";
            // 
            // textPtzOpFocus
            // 
            this.textPtzOpFocus.Location = new System.Drawing.Point(134, 35);
            this.textPtzOpFocus.Name = "textPtzOpFocus";
            this.textPtzOpFocus.Size = new System.Drawing.Size(71, 21);
            this.textPtzOpFocus.TabIndex = 3;
            // 
            // label_PTZFor
            // 
            this.label_PTZFor.AutoSize = true;
            this.label_PTZFor.Location = new System.Drawing.Point(134, 20);
            this.label_PTZFor.Name = "label_PTZFor";
            this.label_PTZFor.Size = new System.Drawing.Size(35, 12);
            this.label_PTZFor.TabIndex = 2;
            this.label_PTZFor.Text = "Focus";
            // 
            // comboBoxPtzOpFocusType
            // 
            this.comboBoxPtzOpFocusType.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBoxPtzOpFocusType.FormattingEnabled = true;
            this.comboBoxPtzOpFocusType.Items.AddRange(new object[] {
            "0-Reset",
            "1-Continuous focusing",
            "2-Autofocusing"});
            this.comboBoxPtzOpFocusType.Location = new System.Drawing.Point(7, 37);
            this.comboBoxPtzOpFocusType.Name = "comboBoxPtzOpFocusType";
            this.comboBoxPtzOpFocusType.Size = new System.Drawing.Size(117, 20);
            this.comboBoxPtzOpFocusType.TabIndex = 1;
            // 
            // label_PTZForOpType
            // 
            this.label_PTZForOpType.AutoSize = true;
            this.label_PTZForOpType.Location = new System.Drawing.Point(7, 21);
            this.label_PTZForOpType.Name = "label_PTZForOpType";
            this.label_PTZForOpType.Size = new System.Drawing.Size(71, 12);
            this.label_PTZForOpType.TabIndex = 0;
            this.label_PTZForOpType.Text = "OperateType";
            // 
            // tabPagePreset
            // 
            this.tabPagePreset.Controls.Add(this.groupBox_PTZPreset);
            this.tabPagePreset.Location = new System.Drawing.Point(4, 22);
            this.tabPagePreset.Name = "tabPagePreset";
            this.tabPagePreset.Padding = new System.Windows.Forms.Padding(3);
            this.tabPagePreset.Size = new System.Drawing.Size(890, 338);
            this.tabPagePreset.TabIndex = 2;
            this.tabPagePreset.Text = "Preset";
            this.tabPagePreset.UseVisualStyleBackColor = true;
            // 
            // groupBox_PTZPreset
            // 
            this.groupBox_PTZPreset.Controls.Add(this.btnPtzOpGetPreset);
            this.groupBox_PTZPreset.Controls.Add(this.btnPtzOpPresetPoint);
            this.groupBox_PTZPreset.Controls.Add(this.textPtzOpPresetEndTime);
            this.groupBox_PTZPreset.Controls.Add(this.label_PTZPointET);
            this.groupBox_PTZPreset.Controls.Add(this.textPtzOpPresetStartTime);
            this.groupBox_PTZPreset.Controls.Add(this.label_PTZPointST);
            this.groupBox_PTZPreset.Controls.Add(this.textPtzOpPresetPointName);
            this.groupBox_PTZPreset.Controls.Add(this.label_PTZPointN);
            this.groupBox_PTZPreset.Controls.Add(this.textPtzOpPresetPointCode);
            this.groupBox_PTZPreset.Controls.Add(this.label_PTZPointC);
            this.groupBox_PTZPreset.Controls.Add(this.comboBoxPtzOpPresetType);
            this.groupBox_PTZPreset.Controls.Add(this.label_PTZPreOpT);
            this.groupBox_PTZPreset.Location = new System.Drawing.Point(9, 22);
            this.groupBox_PTZPreset.Name = "groupBox_PTZPreset";
            this.groupBox_PTZPreset.Size = new System.Drawing.Size(561, 131);
            this.groupBox_PTZPreset.TabIndex = 6;
            this.groupBox_PTZPreset.TabStop = false;
            this.groupBox_PTZPreset.Text = "PtzOperatePresetPoint";
            // 
            // btnPtzOpGetPreset
            // 
            this.btnPtzOpGetPreset.Location = new System.Drawing.Point(163, 90);
            this.btnPtzOpGetPreset.Name = "btnPtzOpGetPreset";
            this.btnPtzOpGetPreset.Size = new System.Drawing.Size(96, 23);
            this.btnPtzOpGetPreset.TabIndex = 13;
            this.btnPtzOpGetPreset.Text = "Get Preset";
            this.btnPtzOpGetPreset.UseVisualStyleBackColor = true;
            this.btnPtzOpGetPreset.Click += new System.EventHandler(this.btnPtzOpGetPreset_Click);
            // 
            // btnPtzOpPresetPoint
            // 
            this.btnPtzOpPresetPoint.Location = new System.Drawing.Point(7, 90);
            this.btnPtzOpPresetPoint.Name = "btnPtzOpPresetPoint";
            this.btnPtzOpPresetPoint.Size = new System.Drawing.Size(96, 23);
            this.btnPtzOpPresetPoint.TabIndex = 12;
            this.btnPtzOpPresetPoint.Text = "PresetPoint";
            this.btnPtzOpPresetPoint.UseVisualStyleBackColor = true;
            this.btnPtzOpPresetPoint.Click += new System.EventHandler(this.btnPtzOpPresetPoint_Click);
            // 
            // textPtzOpPresetEndTime
            // 
            this.textPtzOpPresetEndTime.Location = new System.Drawing.Point(435, 34);
            this.textPtzOpPresetEndTime.Name = "textPtzOpPresetEndTime";
            this.textPtzOpPresetEndTime.Size = new System.Drawing.Size(100, 21);
            this.textPtzOpPresetEndTime.TabIndex = 11;
            // 
            // label_PTZPointET
            // 
            this.label_PTZPointET.AutoSize = true;
            this.label_PTZPointET.Location = new System.Drawing.Point(435, 21);
            this.label_PTZPointET.Name = "label_PTZPointET";
            this.label_PTZPointET.Size = new System.Drawing.Size(47, 12);
            this.label_PTZPointET.TabIndex = 10;
            this.label_PTZPointET.Text = "EndTime";
            // 
            // textPtzOpPresetStartTime
            // 
            this.textPtzOpPresetStartTime.Location = new System.Drawing.Point(321, 34);
            this.textPtzOpPresetStartTime.Name = "textPtzOpPresetStartTime";
            this.textPtzOpPresetStartTime.Size = new System.Drawing.Size(100, 21);
            this.textPtzOpPresetStartTime.TabIndex = 9;
            // 
            // label_PTZPointST
            // 
            this.label_PTZPointST.AutoSize = true;
            this.label_PTZPointST.Location = new System.Drawing.Point(319, 21);
            this.label_PTZPointST.Name = "label_PTZPointST";
            this.label_PTZPointST.Size = new System.Drawing.Size(59, 12);
            this.label_PTZPointST.TabIndex = 8;
            this.label_PTZPointST.Text = "StartTime";
            // 
            // textPtzOpPresetPointName
            // 
            this.textPtzOpPresetPointName.Location = new System.Drawing.Point(238, 35);
            this.textPtzOpPresetPointName.Name = "textPtzOpPresetPointName";
            this.textPtzOpPresetPointName.Size = new System.Drawing.Size(69, 21);
            this.textPtzOpPresetPointName.TabIndex = 7;
            // 
            // label_PTZPointN
            // 
            this.label_PTZPointN.AutoSize = true;
            this.label_PTZPointN.Location = new System.Drawing.Point(237, 21);
            this.label_PTZPointN.Name = "label_PTZPointN";
            this.label_PTZPointN.Size = new System.Drawing.Size(59, 12);
            this.label_PTZPointN.TabIndex = 6;
            this.label_PTZPointN.Text = "PointName";
            // 
            // textPtzOpPresetPointCode
            // 
            this.textPtzOpPresetPointCode.Location = new System.Drawing.Point(163, 35);
            this.textPtzOpPresetPointCode.Name = "textPtzOpPresetPointCode";
            this.textPtzOpPresetPointCode.Size = new System.Drawing.Size(58, 21);
            this.textPtzOpPresetPointCode.TabIndex = 5;
            // 
            // label_PTZPointC
            // 
            this.label_PTZPointC.AutoSize = true;
            this.label_PTZPointC.Location = new System.Drawing.Point(162, 21);
            this.label_PTZPointC.Name = "label_PTZPointC";
            this.label_PTZPointC.Size = new System.Drawing.Size(59, 12);
            this.label_PTZPointC.TabIndex = 4;
            this.label_PTZPointC.Text = "PointCode";
            // 
            // comboBoxPtzOpPresetType
            // 
            this.comboBoxPtzOpPresetType.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBoxPtzOpPresetType.FormattingEnabled = true;
            this.comboBoxPtzOpPresetType.Items.AddRange(new object[] {
            "1-Location",
            "2-Set up",
            "3-Delete",
            "4-Update working time"});
            this.comboBoxPtzOpPresetType.Location = new System.Drawing.Point(7, 37);
            this.comboBoxPtzOpPresetType.Name = "comboBoxPtzOpPresetType";
            this.comboBoxPtzOpPresetType.Size = new System.Drawing.Size(135, 20);
            this.comboBoxPtzOpPresetType.TabIndex = 3;
            // 
            // label_PTZPreOpT
            // 
            this.label_PTZPreOpT.AutoSize = true;
            this.label_PTZPreOpT.Location = new System.Drawing.Point(7, 21);
            this.label_PTZPreOpT.Name = "label_PTZPreOpT";
            this.label_PTZPreOpT.Size = new System.Drawing.Size(71, 12);
            this.label_PTZPreOpT.TabIndex = 2;
            this.label_PTZPreOpT.Text = "OperateType";
            // 
            // tabPagePTZOther
            // 
            this.tabPagePTZOther.Controls.Add(this.groupBox_PTZSit);
            this.tabPagePTZOther.Controls.Add(this.groupBox_PTZArrange);
            this.tabPagePTZOther.Controls.Add(this.groupBox_PTZAlarm);
            this.tabPagePTZOther.Location = new System.Drawing.Point(4, 22);
            this.tabPagePTZOther.Name = "tabPagePTZOther";
            this.tabPagePTZOther.Padding = new System.Windows.Forms.Padding(3);
            this.tabPagePTZOther.Size = new System.Drawing.Size(890, 338);
            this.tabPagePTZOther.TabIndex = 3;
            this.tabPagePTZOther.Text = "Other";
            this.tabPagePTZOther.UseVisualStyleBackColor = true;
            // 
            // groupBox_PTZSit
            // 
            this.groupBox_PTZSit.Controls.Add(this.btnPtzOpSitPos);
            this.groupBox_PTZSit.Controls.Add(this.textPtzOpSitPosExtend);
            this.groupBox_PTZSit.Controls.Add(this.label_PTZSitEx);
            this.groupBox_PTZSit.Controls.Add(this.textPtzOpSitPosPointZ);
            this.groupBox_PTZSit.Controls.Add(this.label_PTZSitPZ);
            this.groupBox_PTZSit.Controls.Add(this.textPtzOpSitPosPointY);
            this.groupBox_PTZSit.Controls.Add(this.label_PTZSitPY);
            this.groupBox_PTZSit.Controls.Add(this.textPtzOpSitPosPointX);
            this.groupBox_PTZSit.Controls.Add(this.label_PTZSitPX);
            this.groupBox_PTZSit.Location = new System.Drawing.Point(15, 16);
            this.groupBox_PTZSit.Name = "groupBox_PTZSit";
            this.groupBox_PTZSit.Size = new System.Drawing.Size(430, 67);
            this.groupBox_PTZSit.TabIndex = 7;
            this.groupBox_PTZSit.TabStop = false;
            this.groupBox_PTZSit.Text = "PtzSitPosition";
            // 
            // btnPtzOpSitPos
            // 
            this.btnPtzOpSitPos.Location = new System.Drawing.Point(333, 34);
            this.btnPtzOpSitPos.Name = "btnPtzOpSitPos";
            this.btnPtzOpSitPos.Size = new System.Drawing.Size(80, 23);
            this.btnPtzOpSitPos.TabIndex = 8;
            this.btnPtzOpSitPos.Text = "SitPosition";
            this.btnPtzOpSitPos.UseVisualStyleBackColor = true;
            this.btnPtzOpSitPos.Click += new System.EventHandler(this.btnPtzOpSitPos_Click);
            // 
            // textPtzOpSitPosExtend
            // 
            this.textPtzOpSitPosExtend.Location = new System.Drawing.Point(175, 36);
            this.textPtzOpSitPosExtend.Name = "textPtzOpSitPosExtend";
            this.textPtzOpSitPosExtend.Size = new System.Drawing.Size(115, 21);
            this.textPtzOpSitPosExtend.TabIndex = 7;
            // 
            // label_PTZSitEx
            // 
            this.label_PTZSitEx.AutoSize = true;
            this.label_PTZSitEx.Location = new System.Drawing.Point(173, 21);
            this.label_PTZSitEx.Name = "label_PTZSitEx";
            this.label_PTZSitEx.Size = new System.Drawing.Size(41, 12);
            this.label_PTZSitEx.TabIndex = 6;
            this.label_PTZSitEx.Text = "Extend";
            // 
            // textPtzOpSitPosPointZ
            // 
            this.textPtzOpSitPosPointZ.Location = new System.Drawing.Point(120, 36);
            this.textPtzOpSitPosPointZ.Name = "textPtzOpSitPosPointZ";
            this.textPtzOpSitPosPointZ.Size = new System.Drawing.Size(41, 21);
            this.textPtzOpSitPosPointZ.TabIndex = 5;
            // 
            // label_PTZSitPZ
            // 
            this.label_PTZSitPZ.AutoSize = true;
            this.label_PTZSitPZ.Location = new System.Drawing.Point(118, 21);
            this.label_PTZSitPZ.Name = "label_PTZSitPZ";
            this.label_PTZSitPZ.Size = new System.Drawing.Size(41, 12);
            this.label_PTZSitPZ.TabIndex = 4;
            this.label_PTZSitPZ.Text = "PointZ";
            // 
            // textPtzOpSitPosPointY
            // 
            this.textPtzOpSitPosPointY.Location = new System.Drawing.Point(65, 36);
            this.textPtzOpSitPosPointY.Name = "textPtzOpSitPosPointY";
            this.textPtzOpSitPosPointY.Size = new System.Drawing.Size(41, 21);
            this.textPtzOpSitPosPointY.TabIndex = 3;
            // 
            // label_PTZSitPY
            // 
            this.label_PTZSitPY.AutoSize = true;
            this.label_PTZSitPY.Location = new System.Drawing.Point(63, 21);
            this.label_PTZSitPY.Name = "label_PTZSitPY";
            this.label_PTZSitPY.Size = new System.Drawing.Size(41, 12);
            this.label_PTZSitPY.TabIndex = 2;
            this.label_PTZSitPY.Text = "PointY";
            // 
            // textPtzOpSitPosPointX
            // 
            this.textPtzOpSitPosPointX.Location = new System.Drawing.Point(9, 36);
            this.textPtzOpSitPosPointX.Name = "textPtzOpSitPosPointX";
            this.textPtzOpSitPosPointX.Size = new System.Drawing.Size(41, 21);
            this.textPtzOpSitPosPointX.TabIndex = 1;
            // 
            // label_PTZSitPX
            // 
            this.label_PTZSitPX.AutoSize = true;
            this.label_PTZSitPX.Location = new System.Drawing.Point(7, 21);
            this.label_PTZSitPX.Name = "label_PTZSitPX";
            this.label_PTZSitPX.Size = new System.Drawing.Size(41, 12);
            this.label_PTZSitPX.TabIndex = 0;
            this.label_PTZSitPX.Text = "PointX";
            // 
            // groupBox_PTZArrange
            // 
            this.groupBox_PTZArrange.Controls.Add(this.btnPtzOpArrangePtz);
            this.groupBox_PTZArrange.Controls.Add(this.textPtzOpArrangeExtend);
            this.groupBox_PTZArrange.Controls.Add(this.label_PTZArrEx);
            this.groupBox_PTZArrange.Controls.Add(this.textPtzOpArrangeLockTime);
            this.groupBox_PTZArrange.Controls.Add(this.label_PTZArrLT);
            this.groupBox_PTZArrange.Controls.Add(this.label_PTZArrOT);
            this.groupBox_PTZArrange.Controls.Add(this.comboBoxPtzOpArrangeType);
            this.groupBox_PTZArrange.Location = new System.Drawing.Point(15, 100);
            this.groupBox_PTZArrange.Name = "groupBox_PTZArrange";
            this.groupBox_PTZArrange.Size = new System.Drawing.Size(430, 67);
            this.groupBox_PTZArrange.TabIndex = 8;
            this.groupBox_PTZArrange.TabStop = false;
            this.groupBox_PTZArrange.Text = "PtzArrangePtz";
            // 
            // btnPtzOpArrangePtz
            // 
            this.btnPtzOpArrangePtz.Location = new System.Drawing.Point(333, 33);
            this.btnPtzOpArrangePtz.Name = "btnPtzOpArrangePtz";
            this.btnPtzOpArrangePtz.Size = new System.Drawing.Size(84, 23);
            this.btnPtzOpArrangePtz.TabIndex = 6;
            this.btnPtzOpArrangePtz.Text = "ArrangePtz";
            this.btnPtzOpArrangePtz.UseVisualStyleBackColor = true;
            this.btnPtzOpArrangePtz.Click += new System.EventHandler(this.btnPtzOpArrangePtz_Click);
            // 
            // textPtzOpArrangeExtend
            // 
            this.textPtzOpArrangeExtend.Location = new System.Drawing.Point(221, 36);
            this.textPtzOpArrangeExtend.Name = "textPtzOpArrangeExtend";
            this.textPtzOpArrangeExtend.Size = new System.Drawing.Size(93, 21);
            this.textPtzOpArrangeExtend.TabIndex = 5;
            // 
            // label_PTZArrEx
            // 
            this.label_PTZArrEx.AutoSize = true;
            this.label_PTZArrEx.Location = new System.Drawing.Point(219, 21);
            this.label_PTZArrEx.Name = "label_PTZArrEx";
            this.label_PTZArrEx.Size = new System.Drawing.Size(41, 12);
            this.label_PTZArrEx.TabIndex = 4;
            this.label_PTZArrEx.Text = "Extend";
            // 
            // textPtzOpArrangeLockTime
            // 
            this.textPtzOpArrangeLockTime.Location = new System.Drawing.Point(158, 36);
            this.textPtzOpArrangeLockTime.Name = "textPtzOpArrangeLockTime";
            this.textPtzOpArrangeLockTime.Size = new System.Drawing.Size(48, 21);
            this.textPtzOpArrangeLockTime.TabIndex = 3;
            // 
            // label_PTZArrLT
            // 
            this.label_PTZArrLT.AutoSize = true;
            this.label_PTZArrLT.Location = new System.Drawing.Point(158, 21);
            this.label_PTZArrLT.Name = "label_PTZArrLT";
            this.label_PTZArrLT.Size = new System.Drawing.Size(53, 12);
            this.label_PTZArrLT.TabIndex = 2;
            this.label_PTZArrLT.Text = "LockTime";
            // 
            // label_PTZArrOT
            // 
            this.label_PTZArrOT.AutoSize = true;
            this.label_PTZArrOT.Location = new System.Drawing.Point(9, 21);
            this.label_PTZArrOT.Name = "label_PTZArrOT";
            this.label_PTZArrOT.Size = new System.Drawing.Size(71, 12);
            this.label_PTZArrOT.TabIndex = 1;
            this.label_PTZArrOT.Text = "OperateType";
            // 
            // comboBoxPtzOpArrangeType
            // 
            this.comboBoxPtzOpArrangeType.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBoxPtzOpArrangeType.FormattingEnabled = true;
            this.comboBoxPtzOpArrangeType.Items.AddRange(new object[] {
            "0-Unknow",
            "1-Lock current camera",
            "2-Unlock current camera",
            "3-Unlock all camera",
            "4-Lock all camera",
            "5-Query lock status"});
            this.comboBoxPtzOpArrangeType.Location = new System.Drawing.Point(10, 37);
            this.comboBoxPtzOpArrangeType.Name = "comboBoxPtzOpArrangeType";
            this.comboBoxPtzOpArrangeType.Size = new System.Drawing.Size(142, 20);
            this.comboBoxPtzOpArrangeType.TabIndex = 0;
            // 
            // groupBox_PTZAlarm
            // 
            this.groupBox_PTZAlarm.Controls.Add(this.btnPtzOpAlarmOut);
            this.groupBox_PTZAlarm.Controls.Add(this.comboBoxPtzOpAlarmOutCommand);
            this.groupBox_PTZAlarm.Controls.Add(this.label_PTZAlarmCom);
            this.groupBox_PTZAlarm.Controls.Add(this.label_PTZAlarmOT);
            this.groupBox_PTZAlarm.Controls.Add(this.comboBoxPtzOpAlarmOutType);
            this.groupBox_PTZAlarm.Location = new System.Drawing.Point(451, 16);
            this.groupBox_PTZAlarm.Name = "groupBox_PTZAlarm";
            this.groupBox_PTZAlarm.Size = new System.Drawing.Size(433, 67);
            this.groupBox_PTZAlarm.TabIndex = 9;
            this.groupBox_PTZAlarm.TabStop = false;
            this.groupBox_PTZAlarm.Text = "AlarmActionOut";
            // 
            // btnPtzOpAlarmOut
            // 
            this.btnPtzOpAlarmOut.Location = new System.Drawing.Point(312, 33);
            this.btnPtzOpAlarmOut.Name = "btnPtzOpAlarmOut";
            this.btnPtzOpAlarmOut.Size = new System.Drawing.Size(75, 23);
            this.btnPtzOpAlarmOut.TabIndex = 6;
            this.btnPtzOpAlarmOut.Text = "AlarmOut";
            this.btnPtzOpAlarmOut.UseVisualStyleBackColor = true;
            this.btnPtzOpAlarmOut.Click += new System.EventHandler(this.btnPtzOpAlarmOut_Click);
            // 
            // comboBoxPtzOpAlarmOutCommand
            // 
            this.comboBoxPtzOpAlarmOutCommand.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBoxPtzOpAlarmOutCommand.FormattingEnabled = true;
            this.comboBoxPtzOpAlarmOutCommand.Items.AddRange(new object[] {
            "0-Close",
            "1-Open/Automatically",
            "2-Manual"});
            this.comboBoxPtzOpAlarmOutCommand.Location = new System.Drawing.Point(150, 36);
            this.comboBoxPtzOpAlarmOutCommand.Name = "comboBoxPtzOpAlarmOutCommand";
            this.comboBoxPtzOpAlarmOutCommand.Size = new System.Drawing.Size(140, 20);
            this.comboBoxPtzOpAlarmOutCommand.TabIndex = 5;
            // 
            // label_PTZAlarmCom
            // 
            this.label_PTZAlarmCom.AutoSize = true;
            this.label_PTZAlarmCom.Location = new System.Drawing.Point(148, 20);
            this.label_PTZAlarmCom.Name = "label_PTZAlarmCom";
            this.label_PTZAlarmCom.Size = new System.Drawing.Size(47, 12);
            this.label_PTZAlarmCom.TabIndex = 4;
            this.label_PTZAlarmCom.Text = "Command";
            // 
            // label_PTZAlarmOT
            // 
            this.label_PTZAlarmOT.AutoSize = true;
            this.label_PTZAlarmOT.Location = new System.Drawing.Point(11, 20);
            this.label_PTZAlarmOT.Name = "label_PTZAlarmOT";
            this.label_PTZAlarmOT.Size = new System.Drawing.Size(71, 12);
            this.label_PTZAlarmOT.TabIndex = 3;
            this.label_PTZAlarmOT.Text = "OperateType";
            // 
            // comboBoxPtzOpAlarmOutType
            // 
            this.comboBoxPtzOpAlarmOutType.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBoxPtzOpAlarmOutType.FormattingEnabled = true;
            this.comboBoxPtzOpAlarmOutType.Items.AddRange(new object[] {
            "1-Status control",
            "2-Pattern control"});
            this.comboBoxPtzOpAlarmOutType.Location = new System.Drawing.Point(12, 36);
            this.comboBoxPtzOpAlarmOutType.Name = "comboBoxPtzOpAlarmOutType";
            this.comboBoxPtzOpAlarmOutType.Size = new System.Drawing.Size(127, 20);
            this.comboBoxPtzOpAlarmOutType.TabIndex = 2;
            // 
            // btnPtzOpClear
            // 
            this.btnPtzOpClear.Location = new System.Drawing.Point(807, 525);
            this.btnPtzOpClear.Name = "btnPtzOpClear";
            this.btnPtzOpClear.Size = new System.Drawing.Size(88, 23);
            this.btnPtzOpClear.TabIndex = 11;
            this.btnPtzOpClear.Text = "Clear";
            this.btnPtzOpClear.UseVisualStyleBackColor = true;
            this.btnPtzOpClear.Click += new System.EventHandler(this.btnPtzOpClear_Click);
            // 
            // textPtzOpResult
            // 
            this.textPtzOpResult.Location = new System.Drawing.Point(11, 434);
            this.textPtzOpResult.Multiline = true;
            this.textPtzOpResult.Name = "textPtzOpResult";
            this.textPtzOpResult.ReadOnly = true;
            this.textPtzOpResult.Size = new System.Drawing.Size(778, 199);
            this.textPtzOpResult.TabIndex = 10;
            // 
            // textPtzChannelId
            // 
            this.textPtzChannelId.Location = new System.Drawing.Point(96, 7);
            this.textPtzChannelId.Name = "textPtzChannelId";
            this.textPtzChannelId.Size = new System.Drawing.Size(171, 21);
            this.textPtzChannelId.TabIndex = 1;
            // 
            // label_PTZChnlCode
            // 
            this.label_PTZChnlCode.AutoSize = true;
            this.label_PTZChnlCode.Location = new System.Drawing.Point(9, 10);
            this.label_PTZChnlCode.Name = "label_PTZChnlCode";
            this.label_PTZChnlCode.Size = new System.Drawing.Size(59, 12);
            this.label_PTZChnlCode.TabIndex = 0;
            this.label_PTZChnlCode.Text = "ChannelId";
            // 
            // Playback
            // 
            this.Playback.Controls.Add(this.textPlaybackResult);
            this.Playback.Controls.Add(this.tabControl3);
            this.Playback.Location = new System.Drawing.Point(124, 4);
            this.Playback.Name = "Playback";
            this.Playback.Size = new System.Drawing.Size(917, 641);
            this.Playback.TabIndex = 5;
            this.Playback.Text = "Playback";
            this.Playback.UseVisualStyleBackColor = true;
            // 
            // textPlaybackResult
            // 
            this.textPlaybackResult.Location = new System.Drawing.Point(4, 444);
            this.textPlaybackResult.Multiline = true;
            this.textPlaybackResult.Name = "textPlaybackResult";
            this.textPlaybackResult.ReadOnly = true;
            this.textPlaybackResult.Size = new System.Drawing.Size(906, 86);
            this.textPlaybackResult.TabIndex = 1;
            // 
            // tabControl3
            // 
            this.tabControl3.Controls.Add(this.PlaybackRecord);
            this.tabControl3.Controls.Add(this.PlaybackPlayback);
            this.tabControl3.Controls.Add(this.PlaybackDownload);
            this.tabControl3.Controls.Add(this.PlaybackSound);
            this.tabControl3.Controls.Add(this.PlaybackPicture);
            this.tabControl3.Location = new System.Drawing.Point(4, 4);
            this.tabControl3.Name = "tabControl3";
            this.tabControl3.SelectedIndex = 0;
            this.tabControl3.Size = new System.Drawing.Size(910, 433);
            this.tabControl3.TabIndex = 0;
            // 
            // PlaybackRecord
            // 
            this.PlaybackRecord.Controls.Add(this.comboBoxPlaybackQueryRecordType);
            this.PlaybackRecord.Controls.Add(this.label59);
            this.PlaybackRecord.Controls.Add(this.groupBox22);
            this.PlaybackRecord.Controls.Add(this.label81);
            this.PlaybackRecord.Controls.Add(this.groupBox21);
            this.PlaybackRecord.Controls.Add(this.groupBox20);
            this.PlaybackRecord.Controls.Add(this.comboBoxPlaybackStreamType);
            this.PlaybackRecord.Controls.Add(this.comboBoxPlaybackSourceType);
            this.PlaybackRecord.Controls.Add(this.label82);
            this.PlaybackRecord.Controls.Add(this.label83);
            this.PlaybackRecord.Controls.Add(this.textPlaybackDeviceID);
            this.PlaybackRecord.Controls.Add(this.listViewPlaybackRecordInfo);
            this.PlaybackRecord.Controls.Add(this.datePlaybackEndTime);
            this.PlaybackRecord.Controls.Add(this.label79);
            this.PlaybackRecord.Controls.Add(this.datePlaybackStartTime);
            this.PlaybackRecord.Controls.Add(this.label78);
            this.PlaybackRecord.Location = new System.Drawing.Point(4, 22);
            this.PlaybackRecord.Name = "PlaybackRecord";
            this.PlaybackRecord.Padding = new System.Windows.Forms.Padding(3);
            this.PlaybackRecord.Size = new System.Drawing.Size(902, 407);
            this.PlaybackRecord.TabIndex = 0;
            this.PlaybackRecord.Text = "Record";
            this.PlaybackRecord.UseVisualStyleBackColor = true;
            // 
            // comboBoxPlaybackQueryRecordType
            // 
            this.comboBoxPlaybackQueryRecordType.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBoxPlaybackQueryRecordType.FormattingEnabled = true;
            this.comboBoxPlaybackQueryRecordType.Items.AddRange(new object[] {
            "0-All",
            "1-Manual",
            "2-Alarm"});
            this.comboBoxPlaybackQueryRecordType.Location = new System.Drawing.Point(107, 142);
            this.comboBoxPlaybackQueryRecordType.Name = "comboBoxPlaybackQueryRecordType";
            this.comboBoxPlaybackQueryRecordType.Size = new System.Drawing.Size(91, 20);
            this.comboBoxPlaybackQueryRecordType.TabIndex = 41;
            // 
            // label59
            // 
            this.label59.AutoSize = true;
            this.label59.Location = new System.Drawing.Point(33, 146);
            this.label59.Name = "label59";
            this.label59.Size = new System.Drawing.Size(71, 12);
            this.label59.TabIndex = 40;
            this.label59.Text = "RecordType:";
            // 
            // groupBox22
            // 
            this.groupBox22.Controls.Add(this.btnPlaybackStartRemoteRecord);
            this.groupBox22.Controls.Add(this.btnPlaybackStopRemoteRecord);
            this.groupBox22.Controls.Add(this.label85);
            this.groupBox22.Controls.Add(this.textPlaybackDuration);
            this.groupBox22.Location = new System.Drawing.Point(634, 50);
            this.groupBox22.Name = "groupBox22";
            this.groupBox22.Size = new System.Drawing.Size(252, 117);
            this.groupBox22.TabIndex = 39;
            this.groupBox22.TabStop = false;
            this.groupBox22.Text = "Record";
            // 
            // btnPlaybackStartRemoteRecord
            // 
            this.btnPlaybackStartRemoteRecord.Location = new System.Drawing.Point(6, 67);
            this.btnPlaybackStartRemoteRecord.Name = "btnPlaybackStartRemoteRecord";
            this.btnPlaybackStartRemoteRecord.Size = new System.Drawing.Size(117, 23);
            this.btnPlaybackStartRemoteRecord.TabIndex = 42;
            this.btnPlaybackStartRemoteRecord.Text = "StartRemoteRecord";
            this.btnPlaybackStartRemoteRecord.UseVisualStyleBackColor = true;
            this.btnPlaybackStartRemoteRecord.Click += new System.EventHandler(this.btnPlaybackStartRemoteRecord_Click);
            // 
            // btnPlaybackStopRemoteRecord
            // 
            this.btnPlaybackStopRemoteRecord.Location = new System.Drawing.Point(129, 67);
            this.btnPlaybackStopRemoteRecord.Name = "btnPlaybackStopRemoteRecord";
            this.btnPlaybackStopRemoteRecord.Size = new System.Drawing.Size(117, 23);
            this.btnPlaybackStopRemoteRecord.TabIndex = 41;
            this.btnPlaybackStopRemoteRecord.Text = "StopRemoteRecord";
            this.btnPlaybackStopRemoteRecord.UseVisualStyleBackColor = true;
            this.btnPlaybackStopRemoteRecord.Click += new System.EventHandler(this.btnPlaybackStopRemoteRecord_Click);
            // 
            // label85
            // 
            this.label85.AutoSize = true;
            this.label85.Location = new System.Drawing.Point(21, 27);
            this.label85.Name = "label85";
            this.label85.Size = new System.Drawing.Size(59, 12);
            this.label85.TabIndex = 40;
            this.label85.Text = "Duration:";
            // 
            // textPlaybackDuration
            // 
            this.textPlaybackDuration.Location = new System.Drawing.Point(92, 23);
            this.textPlaybackDuration.Name = "textPlaybackDuration";
            this.textPlaybackDuration.Size = new System.Drawing.Size(136, 21);
            this.textPlaybackDuration.TabIndex = 39;
            // 
            // label81
            // 
            this.label81.AutoSize = true;
            this.label81.Location = new System.Drawing.Point(33, 59);
            this.label81.Name = "label81";
            this.label81.Size = new System.Drawing.Size(65, 12);
            this.label81.TabIndex = 38;
            this.label81.Text = "Device ID:";
            // 
            // groupBox21
            // 
            this.groupBox21.Controls.Add(this.checkBoxPlaybackForce);
            this.groupBox21.Controls.Add(this.textPlaybackUserID);
            this.groupBox21.Controls.Add(this.label84);
            this.groupBox21.Controls.Add(this.btnPlaybackQueryLockRecord);
            this.groupBox21.Controls.Add(this.btnPlaybackUnlockRecordFile);
            this.groupBox21.Controls.Add(this.btnPlaybackLockRecordFile);
            this.groupBox21.Location = new System.Drawing.Point(409, 50);
            this.groupBox21.Name = "groupBox21";
            this.groupBox21.Size = new System.Drawing.Size(206, 117);
            this.groupBox21.TabIndex = 37;
            this.groupBox21.TabStop = false;
            this.groupBox21.Text = "Lock";
            // 
            // checkBoxPlaybackForce
            // 
            this.checkBoxPlaybackForce.AutoSize = true;
            this.checkBoxPlaybackForce.Location = new System.Drawing.Point(13, 86);
            this.checkBoxPlaybackForce.Name = "checkBoxPlaybackForce";
            this.checkBoxPlaybackForce.Size = new System.Drawing.Size(66, 16);
            this.checkBoxPlaybackForce.TabIndex = 41;
            this.checkBoxPlaybackForce.Text = "IsForce";
            this.checkBoxPlaybackForce.UseVisualStyleBackColor = true;
            // 
            // textPlaybackUserID
            // 
            this.textPlaybackUserID.Location = new System.Drawing.Point(11, 50);
            this.textPlaybackUserID.Name = "textPlaybackUserID";
            this.textPlaybackUserID.Size = new System.Drawing.Size(69, 21);
            this.textPlaybackUserID.TabIndex = 40;
            // 
            // label84
            // 
            this.label84.AutoSize = true;
            this.label84.Location = new System.Drawing.Point(13, 24);
            this.label84.Name = "label84";
            this.label84.Size = new System.Drawing.Size(53, 12);
            this.label84.TabIndex = 39;
            this.label84.Text = "User ID:";
            // 
            // btnPlaybackQueryLockRecord
            // 
            this.btnPlaybackQueryLockRecord.Location = new System.Drawing.Point(86, 82);
            this.btnPlaybackQueryLockRecord.Name = "btnPlaybackQueryLockRecord";
            this.btnPlaybackQueryLockRecord.Size = new System.Drawing.Size(111, 23);
            this.btnPlaybackQueryLockRecord.TabIndex = 13;
            this.btnPlaybackQueryLockRecord.Text = "QueryLockRecord";
            this.btnPlaybackQueryLockRecord.UseVisualStyleBackColor = true;
            this.btnPlaybackQueryLockRecord.Click += new System.EventHandler(this.btnPlaybackQueryLockRecord_Click);
            // 
            // btnPlaybackUnlockRecordFile
            // 
            this.btnPlaybackUnlockRecordFile.Location = new System.Drawing.Point(86, 50);
            this.btnPlaybackUnlockRecordFile.Name = "btnPlaybackUnlockRecordFile";
            this.btnPlaybackUnlockRecordFile.Size = new System.Drawing.Size(111, 23);
            this.btnPlaybackUnlockRecordFile.TabIndex = 12;
            this.btnPlaybackUnlockRecordFile.Text = "UnlockRecordFile";
            this.btnPlaybackUnlockRecordFile.UseVisualStyleBackColor = true;
            this.btnPlaybackUnlockRecordFile.Click += new System.EventHandler(this.btnPlaybackUnlockRecordFile_Click);
            // 
            // btnPlaybackLockRecordFile
            // 
            this.btnPlaybackLockRecordFile.Location = new System.Drawing.Point(86, 19);
            this.btnPlaybackLockRecordFile.Name = "btnPlaybackLockRecordFile";
            this.btnPlaybackLockRecordFile.Size = new System.Drawing.Size(111, 23);
            this.btnPlaybackLockRecordFile.TabIndex = 11;
            this.btnPlaybackLockRecordFile.Text = "LockRecordFile";
            this.btnPlaybackLockRecordFile.UseVisualStyleBackColor = true;
            this.btnPlaybackLockRecordFile.Click += new System.EventHandler(this.btnPlaybackLockRecordFile_Click);
            // 
            // groupBox20
            // 
            this.groupBox20.Controls.Add(this.btnPlaybackGetRecordStatus);
            this.groupBox20.Controls.Add(this.btnPlaybackQueryRecordDate);
            this.groupBox20.Controls.Add(this.btnPlaybackQueryRecord);
            this.groupBox20.Location = new System.Drawing.Point(254, 50);
            this.groupBox20.Name = "groupBox20";
            this.groupBox20.Size = new System.Drawing.Size(135, 117);
            this.groupBox20.TabIndex = 36;
            this.groupBox20.TabStop = false;
            this.groupBox20.Text = "Query";
            // 
            // btnPlaybackGetRecordStatus
            // 
            this.btnPlaybackGetRecordStatus.Location = new System.Drawing.Point(12, 82);
            this.btnPlaybackGetRecordStatus.Name = "btnPlaybackGetRecordStatus";
            this.btnPlaybackGetRecordStatus.Size = new System.Drawing.Size(111, 23);
            this.btnPlaybackGetRecordStatus.TabIndex = 10;
            this.btnPlaybackGetRecordStatus.Text = "GetRecordStatus";
            this.btnPlaybackGetRecordStatus.UseVisualStyleBackColor = true;
            this.btnPlaybackGetRecordStatus.Click += new System.EventHandler(this.btnPlaybackGetRecordStatus_Click);
            // 
            // btnPlaybackQueryRecordDate
            // 
            this.btnPlaybackQueryRecordDate.Location = new System.Drawing.Point(12, 50);
            this.btnPlaybackQueryRecordDate.Name = "btnPlaybackQueryRecordDate";
            this.btnPlaybackQueryRecordDate.Size = new System.Drawing.Size(111, 23);
            this.btnPlaybackQueryRecordDate.TabIndex = 9;
            this.btnPlaybackQueryRecordDate.Text = "QueryRecordDate";
            this.btnPlaybackQueryRecordDate.UseVisualStyleBackColor = true;
            this.btnPlaybackQueryRecordDate.Click += new System.EventHandler(this.btnPlaybackQueryRecordDate_Click);
            // 
            // btnPlaybackQueryRecord
            // 
            this.btnPlaybackQueryRecord.Location = new System.Drawing.Point(12, 19);
            this.btnPlaybackQueryRecord.Name = "btnPlaybackQueryRecord";
            this.btnPlaybackQueryRecord.Size = new System.Drawing.Size(111, 23);
            this.btnPlaybackQueryRecord.TabIndex = 8;
            this.btnPlaybackQueryRecord.Text = "Query Record";
            this.btnPlaybackQueryRecord.UseVisualStyleBackColor = true;
            this.btnPlaybackQueryRecord.Click += new System.EventHandler(this.btnPlaybackQueryRecord_Click);
            // 
            // comboBoxPlaybackStreamType
            // 
            this.comboBoxPlaybackStreamType.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBoxPlaybackStreamType.FormattingEnabled = true;
            this.comboBoxPlaybackStreamType.Items.AddRange(new object[] {
            "1-Main stream",
            "2-Auxiliary code stream"});
            this.comboBoxPlaybackStreamType.Location = new System.Drawing.Point(107, 114);
            this.comboBoxPlaybackStreamType.Name = "comboBoxPlaybackStreamType";
            this.comboBoxPlaybackStreamType.Size = new System.Drawing.Size(91, 20);
            this.comboBoxPlaybackStreamType.TabIndex = 34;
            // 
            // comboBoxPlaybackSourceType
            // 
            this.comboBoxPlaybackSourceType.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBoxPlaybackSourceType.FormattingEnabled = true;
            this.comboBoxPlaybackSourceType.Items.AddRange(new object[] {
            "1-All",
            "2-Device",
            "3-Center"});
            this.comboBoxPlaybackSourceType.Location = new System.Drawing.Point(106, 85);
            this.comboBoxPlaybackSourceType.Name = "comboBoxPlaybackSourceType";
            this.comboBoxPlaybackSourceType.Size = new System.Drawing.Size(92, 20);
            this.comboBoxPlaybackSourceType.TabIndex = 33;
            // 
            // label82
            // 
            this.label82.AutoSize = true;
            this.label82.Location = new System.Drawing.Point(33, 117);
            this.label82.Name = "label82";
            this.label82.Size = new System.Drawing.Size(71, 12);
            this.label82.TabIndex = 32;
            this.label82.Text = "StreamType:";
            // 
            // label83
            // 
            this.label83.AutoSize = true;
            this.label83.Location = new System.Drawing.Point(33, 88);
            this.label83.Name = "label83";
            this.label83.Size = new System.Drawing.Size(71, 12);
            this.label83.TabIndex = 31;
            this.label83.Text = "SourceType:";
            // 
            // textPlaybackDeviceID
            // 
            this.textPlaybackDeviceID.Location = new System.Drawing.Point(106, 56);
            this.textPlaybackDeviceID.Name = "textPlaybackDeviceID";
            this.textPlaybackDeviceID.Size = new System.Drawing.Size(92, 21);
            this.textPlaybackDeviceID.TabIndex = 19;
            this.textPlaybackDeviceID.Text = "178769508B1A9J82899BVPK";
            // 
            // listViewPlaybackRecordInfo
            // 
            this.listViewPlaybackRecordInfo.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.Seq,
            this.SourceType,
            this.RecordType,
            this.StartTime,
            this.EndTime,
            this.FileName,
            this.FileSize,
            this.StreamType,
            this.bLock,
            this.SSId,
            this.FileHandle,
            this.DiskId});
            this.listViewPlaybackRecordInfo.FullRowSelect = true;
            this.listViewPlaybackRecordInfo.GridLines = true;
            this.listViewPlaybackRecordInfo.Location = new System.Drawing.Point(6, 182);
            this.listViewPlaybackRecordInfo.Name = "listViewPlaybackRecordInfo";
            this.listViewPlaybackRecordInfo.Size = new System.Drawing.Size(890, 222);
            this.listViewPlaybackRecordInfo.TabIndex = 4;
            this.listViewPlaybackRecordInfo.UseCompatibleStateImageBehavior = false;
            this.listViewPlaybackRecordInfo.View = System.Windows.Forms.View.Details;
            // 
            // Seq
            // 
            this.Seq.Text = "Seq";
            // 
            // SourceType
            // 
            this.SourceType.Text = "SourceType";
            this.SourceType.Width = 80;
            // 
            // RecordType
            // 
            this.RecordType.Text = "RecordType";
            this.RecordType.Width = 100;
            // 
            // StartTime
            // 
            this.StartTime.Text = "StartTime";
            this.StartTime.Width = 99;
            // 
            // EndTime
            // 
            this.EndTime.Text = "EndTime";
            this.EndTime.Width = 105;
            // 
            // FileName
            // 
            this.FileName.Text = "FileName";
            this.FileName.Width = 85;
            // 
            // FileSize
            // 
            this.FileSize.Text = "FileSize";
            this.FileSize.Width = 81;
            // 
            // StreamType
            // 
            this.StreamType.Text = "StreamType";
            this.StreamType.Width = 87;
            // 
            // bLock
            // 
            this.bLock.Text = "bLock";
            // 
            // SSId
            // 
            this.SSId.Text = "SSId";
            // 
            // FileHandle
            // 
            this.FileHandle.Text = "FileHandle";
            // 
            // DiskId
            // 
            this.DiskId.Text = "DiskId";
            // 
            // datePlaybackEndTime
            // 
            this.datePlaybackEndTime.CustomFormat = "yyyy-MM-dd HH:mm:ss";
            this.datePlaybackEndTime.Format = System.Windows.Forms.DateTimePickerFormat.Custom;
            this.datePlaybackEndTime.Location = new System.Drawing.Point(431, 16);
            this.datePlaybackEndTime.Name = "datePlaybackEndTime";
            this.datePlaybackEndTime.ShowUpDown = true;
            this.datePlaybackEndTime.Size = new System.Drawing.Size(200, 21);
            this.datePlaybackEndTime.TabIndex = 3;
            this.datePlaybackEndTime.TabStop = false;
            this.datePlaybackEndTime.Value = new System.DateTime(2018, 4, 21, 23, 59, 59, 0);
            // 
            // label79
            // 
            this.label79.AutoSize = true;
            this.label79.Location = new System.Drawing.Point(360, 21);
            this.label79.Name = "label79";
            this.label79.Size = new System.Drawing.Size(53, 12);
            this.label79.TabIndex = 2;
            this.label79.Text = "EndTime:";
            // 
            // datePlaybackStartTime
            // 
            this.datePlaybackStartTime.CustomFormat = "yyyy-MM-dd HH:mm:ss";
            this.datePlaybackStartTime.Format = System.Windows.Forms.DateTimePickerFormat.Custom;
            this.datePlaybackStartTime.Location = new System.Drawing.Point(104, 16);
            this.datePlaybackStartTime.Name = "datePlaybackStartTime";
            this.datePlaybackStartTime.ShowUpDown = true;
            this.datePlaybackStartTime.Size = new System.Drawing.Size(200, 21);
            this.datePlaybackStartTime.TabIndex = 1;
            this.datePlaybackStartTime.Value = new System.DateTime(2018, 4, 21, 0, 0, 0, 0);
            // 
            // label78
            // 
            this.label78.AutoSize = true;
            this.label78.Location = new System.Drawing.Point(33, 21);
            this.label78.Name = "label78";
            this.label78.Size = new System.Drawing.Size(65, 12);
            this.label78.TabIndex = 0;
            this.label78.Text = "StartTime:";
            // 
            // PlaybackPlayback
            // 
            this.PlaybackPlayback.Controls.Add(this.pictureBoxPlaybackVideo);
            this.PlaybackPlayback.Controls.Add(this.groupBox19);
            this.PlaybackPlayback.Controls.Add(this.groupBox18);
            this.PlaybackPlayback.Controls.Add(this.comboBoxPlaybackDirection);
            this.PlaybackPlayback.Controls.Add(this.label77);
            this.PlaybackPlayback.Controls.Add(this.comboBoxPlaybackSpeed);
            this.PlaybackPlayback.Controls.Add(this.label76);
            this.PlaybackPlayback.Location = new System.Drawing.Point(4, 22);
            this.PlaybackPlayback.Name = "PlaybackPlayback";
            this.PlaybackPlayback.Padding = new System.Windows.Forms.Padding(3);
            this.PlaybackPlayback.Size = new System.Drawing.Size(902, 407);
            this.PlaybackPlayback.TabIndex = 1;
            this.PlaybackPlayback.Text = "Playback";
            this.PlaybackPlayback.UseVisualStyleBackColor = true;
            // 
            // pictureBoxPlaybackVideo
            // 
            this.pictureBoxPlaybackVideo.Location = new System.Drawing.Point(225, 52);
            this.pictureBoxPlaybackVideo.Name = "pictureBoxPlaybackVideo";
            this.pictureBoxPlaybackVideo.Size = new System.Drawing.Size(667, 344);
            this.pictureBoxPlaybackVideo.TabIndex = 36;
            this.pictureBoxPlaybackVideo.TabStop = false;
            // 
            // groupBox19
            // 
            this.groupBox19.Controls.Add(this.btnPlaybackSetPlaybackSpeed);
            this.groupBox19.Controls.Add(this.btnPlaybackGetProviderType);
            this.groupBox19.Controls.Add(this.btnPlaybackGetPlayedTime);
            this.groupBox19.Location = new System.Drawing.Point(44, 273);
            this.groupBox19.Name = "groupBox19";
            this.groupBox19.Size = new System.Drawing.Size(159, 117);
            this.groupBox19.TabIndex = 35;
            this.groupBox19.TabStop = false;
            // 
            // btnPlaybackSetPlaybackSpeed
            // 
            this.btnPlaybackSetPlaybackSpeed.Location = new System.Drawing.Point(15, 82);
            this.btnPlaybackSetPlaybackSpeed.Name = "btnPlaybackSetPlaybackSpeed";
            this.btnPlaybackSetPlaybackSpeed.Size = new System.Drawing.Size(129, 23);
            this.btnPlaybackSetPlaybackSpeed.TabIndex = 10;
            this.btnPlaybackSetPlaybackSpeed.Text = "SetPlaybackSpeed";
            this.btnPlaybackSetPlaybackSpeed.UseVisualStyleBackColor = true;
            this.btnPlaybackSetPlaybackSpeed.Click += new System.EventHandler(this.btnPlaybackSetPlaybackSpeed_Click);
            // 
            // btnPlaybackGetProviderType
            // 
            this.btnPlaybackGetProviderType.Location = new System.Drawing.Point(15, 50);
            this.btnPlaybackGetProviderType.Name = "btnPlaybackGetProviderType";
            this.btnPlaybackGetProviderType.Size = new System.Drawing.Size(129, 23);
            this.btnPlaybackGetProviderType.TabIndex = 9;
            this.btnPlaybackGetProviderType.Text = "GetProviderType";
            this.btnPlaybackGetProviderType.UseVisualStyleBackColor = true;
            this.btnPlaybackGetProviderType.Click += new System.EventHandler(this.btnPlaybackGetProviderType_Click);
            // 
            // btnPlaybackGetPlayedTime
            // 
            this.btnPlaybackGetPlayedTime.Location = new System.Drawing.Point(15, 19);
            this.btnPlaybackGetPlayedTime.Name = "btnPlaybackGetPlayedTime";
            this.btnPlaybackGetPlayedTime.Size = new System.Drawing.Size(129, 23);
            this.btnPlaybackGetPlayedTime.TabIndex = 8;
            this.btnPlaybackGetPlayedTime.Text = "GetPlayedTime";
            this.btnPlaybackGetPlayedTime.UseVisualStyleBackColor = true;
            this.btnPlaybackGetPlayedTime.Click += new System.EventHandler(this.btnPlaybackGetPlayedTime_Click);
            // 
            // groupBox18
            // 
            this.groupBox18.Controls.Add(this.btnPlaybackSeek);
            this.groupBox18.Controls.Add(this.btnPlaybackFrameStep);
            this.groupBox18.Controls.Add(this.btnPlaybackPausePlayback);
            this.groupBox18.Controls.Add(this.btnPlaybackFile);
            this.groupBox18.Controls.Add(this.btnPlaybackByTime);
            this.groupBox18.Location = new System.Drawing.Point(44, 75);
            this.groupBox18.Name = "groupBox18";
            this.groupBox18.Size = new System.Drawing.Size(159, 185);
            this.groupBox18.TabIndex = 34;
            this.groupBox18.TabStop = false;
            this.groupBox18.Text = "Play";
            // 
            // btnPlaybackSeek
            // 
            this.btnPlaybackSeek.Location = new System.Drawing.Point(15, 147);
            this.btnPlaybackSeek.Name = "btnPlaybackSeek";
            this.btnPlaybackSeek.Size = new System.Drawing.Size(129, 23);
            this.btnPlaybackSeek.TabIndex = 7;
            this.btnPlaybackSeek.Text = "PlaybackSeek";
            this.btnPlaybackSeek.UseVisualStyleBackColor = true;
            this.btnPlaybackSeek.Click += new System.EventHandler(this.btnPlaybackSeek_Click);
            // 
            // btnPlaybackFrameStep
            // 
            this.btnPlaybackFrameStep.Location = new System.Drawing.Point(15, 115);
            this.btnPlaybackFrameStep.Name = "btnPlaybackFrameStep";
            this.btnPlaybackFrameStep.Size = new System.Drawing.Size(129, 23);
            this.btnPlaybackFrameStep.TabIndex = 6;
            this.btnPlaybackFrameStep.Text = "PlaybackFrameStep";
            this.btnPlaybackFrameStep.UseVisualStyleBackColor = true;
            this.btnPlaybackFrameStep.Click += new System.EventHandler(this.btnPlaybackFrameStep_Click);
            // 
            // btnPlaybackPausePlayback
            // 
            this.btnPlaybackPausePlayback.Location = new System.Drawing.Point(15, 84);
            this.btnPlaybackPausePlayback.Name = "btnPlaybackPausePlayback";
            this.btnPlaybackPausePlayback.Size = new System.Drawing.Size(129, 23);
            this.btnPlaybackPausePlayback.TabIndex = 5;
            this.btnPlaybackPausePlayback.Text = "PausePlayback";
            this.btnPlaybackPausePlayback.UseVisualStyleBackColor = true;
            this.btnPlaybackPausePlayback.Click += new System.EventHandler(this.btnPlaybackPausePlayback_Click);
            // 
            // btnPlaybackFile
            // 
            this.btnPlaybackFile.Location = new System.Drawing.Point(15, 51);
            this.btnPlaybackFile.Name = "btnPlaybackFile";
            this.btnPlaybackFile.Size = new System.Drawing.Size(129, 23);
            this.btnPlaybackFile.TabIndex = 4;
            this.btnPlaybackFile.Text = "PlaybackByFile";
            this.btnPlaybackFile.UseVisualStyleBackColor = true;
            this.btnPlaybackFile.Click += new System.EventHandler(this.btnPlaybackFile_Click);
            // 
            // btnPlaybackByTime
            // 
            this.btnPlaybackByTime.Location = new System.Drawing.Point(15, 20);
            this.btnPlaybackByTime.Name = "btnPlaybackByTime";
            this.btnPlaybackByTime.Size = new System.Drawing.Size(129, 23);
            this.btnPlaybackByTime.TabIndex = 3;
            this.btnPlaybackByTime.Text = "PlaybackByTime";
            this.btnPlaybackByTime.UseVisualStyleBackColor = true;
            this.btnPlaybackByTime.Click += new System.EventHandler(this.btnPlaybackByTime_Click);
            // 
            // comboBoxPlaybackDirection
            // 
            this.comboBoxPlaybackDirection.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBoxPlaybackDirection.FormattingEnabled = true;
            this.comboBoxPlaybackDirection.Items.AddRange(new object[] {
            "0-Direction",
            "1-Back Direction"});
            this.comboBoxPlaybackDirection.Location = new System.Drawing.Point(401, 23);
            this.comboBoxPlaybackDirection.Name = "comboBoxPlaybackDirection";
            this.comboBoxPlaybackDirection.Size = new System.Drawing.Size(109, 20);
            this.comboBoxPlaybackDirection.TabIndex = 33;
            // 
            // label77
            // 
            this.label77.AutoSize = true;
            this.label77.Location = new System.Drawing.Point(320, 26);
            this.label77.Name = "label77";
            this.label77.Size = new System.Drawing.Size(65, 12);
            this.label77.TabIndex = 32;
            this.label77.Text = "Direction:";
            // 
            // comboBoxPlaybackSpeed
            // 
            this.comboBoxPlaybackSpeed.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBoxPlaybackSpeed.FormattingEnabled = true;
            this.comboBoxPlaybackSpeed.Items.AddRange(new object[] {
            "1",
            "2",
            "4",
            "8",
            "16",
            "1/2",
            "1/4",
            "1/8",
            "1/16"});
            this.comboBoxPlaybackSpeed.Location = new System.Drawing.Point(95, 23);
            this.comboBoxPlaybackSpeed.Name = "comboBoxPlaybackSpeed";
            this.comboBoxPlaybackSpeed.Size = new System.Drawing.Size(109, 20);
            this.comboBoxPlaybackSpeed.TabIndex = 31;
            // 
            // label76
            // 
            this.label76.AutoSize = true;
            this.label76.Location = new System.Drawing.Point(39, 26);
            this.label76.Name = "label76";
            this.label76.Size = new System.Drawing.Size(41, 12);
            this.label76.TabIndex = 30;
            this.label76.Text = "Speed:";
            // 
            // PlaybackDownload
            // 
            this.PlaybackDownload.Controls.Add(this.groupBox17);
            this.PlaybackDownload.Location = new System.Drawing.Point(4, 22);
            this.PlaybackDownload.Name = "PlaybackDownload";
            this.PlaybackDownload.Size = new System.Drawing.Size(902, 407);
            this.PlaybackDownload.TabIndex = 2;
            this.PlaybackDownload.Text = "Download";
            this.PlaybackDownload.UseVisualStyleBackColor = true;
            // 
            // groupBox17
            // 
            this.groupBox17.Controls.Add(this.comboBoxPlaybackRecordType);
            this.groupBox17.Controls.Add(this.comboBoxPlaybackNameRule);
            this.groupBox17.Controls.Add(this.comboBoxPlaybackFileFormat);
            this.groupBox17.Controls.Add(this.label73);
            this.groupBox17.Controls.Add(this.label74);
            this.groupBox17.Controls.Add(this.label75);
            this.groupBox17.Controls.Add(this.textPlaybackDiskID);
            this.groupBox17.Controls.Add(this.label72);
            this.groupBox17.Controls.Add(this.textPlaybackChannelID);
            this.groupBox17.Controls.Add(this.label67);
            this.groupBox17.Controls.Add(this.textPlaybackChannelName);
            this.groupBox17.Controls.Add(this.label68);
            this.groupBox17.Controls.Add(this.textPlaybackDownloadPath);
            this.groupBox17.Controls.Add(this.label69);
            this.groupBox17.Controls.Add(this.textPlaybackDownloadFileName);
            this.groupBox17.Controls.Add(this.label70);
            this.groupBox17.Controls.Add(this.textPlaybackSplitSize);
            this.groupBox17.Controls.Add(this.label71);
            this.groupBox17.Controls.Add(this.btnPlaybackGetRecordInfo);
            this.groupBox17.Controls.Add(this.btnPlaybackDownloadRecordByTime);
            this.groupBox17.Controls.Add(this.btnPlaybackPauseDownloadRecord);
            this.groupBox17.Controls.Add(this.btnPlaybackDownloadRecordByFile);
            this.groupBox17.Location = new System.Drawing.Point(15, 11);
            this.groupBox17.Name = "groupBox17";
            this.groupBox17.Size = new System.Drawing.Size(502, 363);
            this.groupBox17.TabIndex = 0;
            this.groupBox17.TabStop = false;
            this.groupBox17.Text = "Download";
            // 
            // comboBoxPlaybackRecordType
            // 
            this.comboBoxPlaybackRecordType.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBoxPlaybackRecordType.FormattingEnabled = true;
            this.comboBoxPlaybackRecordType.Items.AddRange(new object[] {
            "0-All",
            "1-Manual",
            "2-Alarm"});
            this.comboBoxPlaybackRecordType.Location = new System.Drawing.Point(348, 199);
            this.comboBoxPlaybackRecordType.Name = "comboBoxPlaybackRecordType";
            this.comboBoxPlaybackRecordType.Size = new System.Drawing.Size(138, 20);
            this.comboBoxPlaybackRecordType.TabIndex = 31;
            // 
            // comboBoxPlaybackNameRule
            // 
            this.comboBoxPlaybackNameRule.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBoxPlaybackNameRule.FormattingEnabled = true;
            this.comboBoxPlaybackNameRule.Items.AddRange(new object[] {
            "0-ChannelID",
            "1-ChannelName",
            "2-ChannelID-Time",
            "3-ChannelName-Time"});
            this.comboBoxPlaybackNameRule.Location = new System.Drawing.Point(348, 161);
            this.comboBoxPlaybackNameRule.Name = "comboBoxPlaybackNameRule";
            this.comboBoxPlaybackNameRule.Size = new System.Drawing.Size(138, 20);
            this.comboBoxPlaybackNameRule.TabIndex = 30;
            // 
            // comboBoxPlaybackFileFormat
            // 
            this.comboBoxPlaybackFileFormat.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBoxPlaybackFileFormat.FormattingEnabled = true;
            this.comboBoxPlaybackFileFormat.Items.AddRange(new object[] {
            "0-Normal",
            "1-AVI",
            "2-MP4",
            "3-FLV",
            "4-ASF"});
            this.comboBoxPlaybackFileFormat.Location = new System.Drawing.Point(348, 123);
            this.comboBoxPlaybackFileFormat.Name = "comboBoxPlaybackFileFormat";
            this.comboBoxPlaybackFileFormat.Size = new System.Drawing.Size(138, 20);
            this.comboBoxPlaybackFileFormat.TabIndex = 29;
            // 
            // label73
            // 
            this.label73.AutoSize = true;
            this.label73.Location = new System.Drawing.Point(274, 203);
            this.label73.Name = "label73";
            this.label73.Size = new System.Drawing.Size(71, 12);
            this.label73.TabIndex = 28;
            this.label73.Text = "RecordType:";
            // 
            // label74
            // 
            this.label74.AutoSize = true;
            this.label74.Location = new System.Drawing.Point(274, 164);
            this.label74.Name = "label74";
            this.label74.Size = new System.Drawing.Size(59, 12);
            this.label74.TabIndex = 27;
            this.label74.Text = "NameRule:";
            // 
            // label75
            // 
            this.label75.AutoSize = true;
            this.label75.Location = new System.Drawing.Point(274, 127);
            this.label75.Name = "label75";
            this.label75.Size = new System.Drawing.Size(71, 12);
            this.label75.TabIndex = 26;
            this.label75.Text = "FileFormat:";
            // 
            // textPlaybackDiskID
            // 
            this.textPlaybackDiskID.Location = new System.Drawing.Point(128, 312);
            this.textPlaybackDiskID.Name = "textPlaybackDiskID";
            this.textPlaybackDiskID.Size = new System.Drawing.Size(125, 21);
            this.textPlaybackDiskID.TabIndex = 25;
            // 
            // label72
            // 
            this.label72.AutoSize = true;
            this.label72.Location = new System.Drawing.Point(20, 317);
            this.label72.Name = "label72";
            this.label72.Size = new System.Drawing.Size(47, 12);
            this.label72.TabIndex = 24;
            this.label72.Text = "DiskID:";
            // 
            // textPlaybackChannelID
            // 
            this.textPlaybackChannelID.Location = new System.Drawing.Point(128, 275);
            this.textPlaybackChannelID.Name = "textPlaybackChannelID";
            this.textPlaybackChannelID.Size = new System.Drawing.Size(125, 21);
            this.textPlaybackChannelID.TabIndex = 23;
            this.textPlaybackChannelID.Text = "178769508B1A9J82899BVPK";
            // 
            // label67
            // 
            this.label67.AutoSize = true;
            this.label67.Location = new System.Drawing.Point(20, 280);
            this.label67.Name = "label67";
            this.label67.Size = new System.Drawing.Size(65, 12);
            this.label67.TabIndex = 22;
            this.label67.Text = "ChannelID:";
            // 
            // textPlaybackChannelName
            // 
            this.textPlaybackChannelName.Location = new System.Drawing.Point(128, 237);
            this.textPlaybackChannelName.Name = "textPlaybackChannelName";
            this.textPlaybackChannelName.Size = new System.Drawing.Size(125, 21);
            this.textPlaybackChannelName.TabIndex = 21;
            // 
            // label68
            // 
            this.label68.AutoSize = true;
            this.label68.Location = new System.Drawing.Point(20, 241);
            this.label68.Name = "label68";
            this.label68.Size = new System.Drawing.Size(77, 12);
            this.label68.TabIndex = 20;
            this.label68.Text = "ChannelName:";
            // 
            // textPlaybackDownloadPath
            // 
            this.textPlaybackDownloadPath.Location = new System.Drawing.Point(128, 199);
            this.textPlaybackDownloadPath.Name = "textPlaybackDownloadPath";
            this.textPlaybackDownloadPath.Size = new System.Drawing.Size(125, 21);
            this.textPlaybackDownloadPath.TabIndex = 19;
            this.textPlaybackDownloadPath.Text = "D:\\";
            // 
            // label69
            // 
            this.label69.AutoSize = true;
            this.label69.Location = new System.Drawing.Point(20, 203);
            this.label69.Name = "label69";
            this.label69.Size = new System.Drawing.Size(83, 12);
            this.label69.TabIndex = 18;
            this.label69.Text = "DownloadPath:";
            // 
            // textPlaybackDownloadFileName
            // 
            this.textPlaybackDownloadFileName.Location = new System.Drawing.Point(128, 161);
            this.textPlaybackDownloadFileName.Name = "textPlaybackDownloadFileName";
            this.textPlaybackDownloadFileName.Size = new System.Drawing.Size(125, 21);
            this.textPlaybackDownloadFileName.TabIndex = 17;
            // 
            // label70
            // 
            this.label70.AutoSize = true;
            this.label70.Location = new System.Drawing.Point(20, 164);
            this.label70.Name = "label70";
            this.label70.Size = new System.Drawing.Size(107, 12);
            this.label70.TabIndex = 16;
            this.label70.Text = "DownloadFileName:";
            // 
            // textPlaybackSplitSize
            // 
            this.textPlaybackSplitSize.Location = new System.Drawing.Point(128, 123);
            this.textPlaybackSplitSize.Name = "textPlaybackSplitSize";
            this.textPlaybackSplitSize.Size = new System.Drawing.Size(125, 21);
            this.textPlaybackSplitSize.TabIndex = 15;
            this.textPlaybackSplitSize.Text = "0";
            // 
            // label71
            // 
            this.label71.AutoSize = true;
            this.label71.Location = new System.Drawing.Point(20, 127);
            this.label71.Name = "label71";
            this.label71.Size = new System.Drawing.Size(65, 12);
            this.label71.TabIndex = 14;
            this.label71.Text = "SplitSize:";
            // 
            // btnPlaybackGetRecordInfo
            // 
            this.btnPlaybackGetRecordInfo.Location = new System.Drawing.Point(266, 55);
            this.btnPlaybackGetRecordInfo.Name = "btnPlaybackGetRecordInfo";
            this.btnPlaybackGetRecordInfo.Size = new System.Drawing.Size(143, 23);
            this.btnPlaybackGetRecordInfo.TabIndex = 7;
            this.btnPlaybackGetRecordInfo.Text = "GetRecordInfo";
            this.btnPlaybackGetRecordInfo.UseVisualStyleBackColor = true;
            this.btnPlaybackGetRecordInfo.Click += new System.EventHandler(this.btnPlaybackGetRecordInfo_Click);
            // 
            // btnPlaybackDownloadRecordByTime
            // 
            this.btnPlaybackDownloadRecordByTime.Location = new System.Drawing.Point(22, 55);
            this.btnPlaybackDownloadRecordByTime.Name = "btnPlaybackDownloadRecordByTime";
            this.btnPlaybackDownloadRecordByTime.Size = new System.Drawing.Size(143, 23);
            this.btnPlaybackDownloadRecordByTime.TabIndex = 6;
            this.btnPlaybackDownloadRecordByTime.Text = "DownloadRecordByTime";
            this.btnPlaybackDownloadRecordByTime.UseVisualStyleBackColor = true;
            this.btnPlaybackDownloadRecordByTime.Click += new System.EventHandler(this.btnPlaybackDownloadRecordByTime_Click);
            // 
            // btnPlaybackPauseDownloadRecord
            // 
            this.btnPlaybackPauseDownloadRecord.Location = new System.Drawing.Point(266, 20);
            this.btnPlaybackPauseDownloadRecord.Name = "btnPlaybackPauseDownloadRecord";
            this.btnPlaybackPauseDownloadRecord.Size = new System.Drawing.Size(143, 23);
            this.btnPlaybackPauseDownloadRecord.TabIndex = 5;
            this.btnPlaybackPauseDownloadRecord.Text = "PauseDownloadRecord";
            this.btnPlaybackPauseDownloadRecord.UseVisualStyleBackColor = true;
            this.btnPlaybackPauseDownloadRecord.Click += new System.EventHandler(this.btnPlaybackPauseDownloadRecord_Click);
            // 
            // btnPlaybackDownloadRecordByFile
            // 
            this.btnPlaybackDownloadRecordByFile.Location = new System.Drawing.Point(22, 20);
            this.btnPlaybackDownloadRecordByFile.Name = "btnPlaybackDownloadRecordByFile";
            this.btnPlaybackDownloadRecordByFile.Size = new System.Drawing.Size(143, 23);
            this.btnPlaybackDownloadRecordByFile.TabIndex = 4;
            this.btnPlaybackDownloadRecordByFile.Text = "DownloadRecordByFile";
            this.btnPlaybackDownloadRecordByFile.UseVisualStyleBackColor = true;
            this.btnPlaybackDownloadRecordByFile.Click += new System.EventHandler(this.btnPlaybackDownloadRecordByFile_Click);
            // 
            // PlaybackSound
            // 
            this.PlaybackSound.Controls.Add(this.labelPlaybackVolume);
            this.PlaybackSound.Controls.Add(this.btnPlaybackGetVolum);
            this.PlaybackSound.Controls.Add(this.btnPlaybackSetVolume);
            this.PlaybackSound.Controls.Add(this.trackBarPlaybackVolume);
            this.PlaybackSound.Controls.Add(this.btnPlaybackIsOpenSound);
            this.PlaybackSound.Controls.Add(this.btnPlaybackCloseShareSound);
            this.PlaybackSound.Controls.Add(this.btnPlaybackOpenShareSound);
            this.PlaybackSound.Controls.Add(this.btnPlaybackCloseSound);
            this.PlaybackSound.Controls.Add(this.btnPlaybackOpenSound);
            this.PlaybackSound.Location = new System.Drawing.Point(4, 22);
            this.PlaybackSound.Name = "PlaybackSound";
            this.PlaybackSound.Size = new System.Drawing.Size(902, 407);
            this.PlaybackSound.TabIndex = 3;
            this.PlaybackSound.Text = "Sound";
            this.PlaybackSound.UseVisualStyleBackColor = true;
            // 
            // labelPlaybackVolume
            // 
            this.labelPlaybackVolume.AutoSize = true;
            this.labelPlaybackVolume.Location = new System.Drawing.Point(442, 104);
            this.labelPlaybackVolume.Name = "labelPlaybackVolume";
            this.labelPlaybackVolume.Size = new System.Drawing.Size(11, 12);
            this.labelPlaybackVolume.TabIndex = 17;
            this.labelPlaybackVolume.Text = "0";
            // 
            // btnPlaybackGetVolum
            // 
            this.btnPlaybackGetVolum.Location = new System.Drawing.Point(607, 89);
            this.btnPlaybackGetVolum.Name = "btnPlaybackGetVolum";
            this.btnPlaybackGetVolum.Size = new System.Drawing.Size(105, 45);
            this.btnPlaybackGetVolum.TabIndex = 16;
            this.btnPlaybackGetVolum.Text = "GetVolume";
            this.btnPlaybackGetVolum.UseVisualStyleBackColor = true;
            this.btnPlaybackGetVolum.Click += new System.EventHandler(this.btnPlaybackGetVolum_Click);
            // 
            // btnPlaybackSetVolume
            // 
            this.btnPlaybackSetVolume.Location = new System.Drawing.Point(482, 89);
            this.btnPlaybackSetVolume.Name = "btnPlaybackSetVolume";
            this.btnPlaybackSetVolume.Size = new System.Drawing.Size(105, 45);
            this.btnPlaybackSetVolume.TabIndex = 15;
            this.btnPlaybackSetVolume.Text = "SetVolume";
            this.btnPlaybackSetVolume.UseVisualStyleBackColor = true;
            this.btnPlaybackSetVolume.Click += new System.EventHandler(this.btnPlaybackSetVolume_Click);
            // 
            // trackBarPlaybackVolume
            // 
            this.trackBarPlaybackVolume.Location = new System.Drawing.Point(33, 89);
            this.trackBarPlaybackVolume.Maximum = 65535;
            this.trackBarPlaybackVolume.Name = "trackBarPlaybackVolume";
            this.trackBarPlaybackVolume.Size = new System.Drawing.Size(402, 40);
            this.trackBarPlaybackVolume.TabIndex = 14;
            this.trackBarPlaybackVolume.ValueChanged += new System.EventHandler(this.trackBarPlaybackVolume_ValueChanged);
            // 
            // btnPlaybackIsOpenSound
            // 
            this.btnPlaybackIsOpenSound.Location = new System.Drawing.Point(525, 17);
            this.btnPlaybackIsOpenSound.Name = "btnPlaybackIsOpenSound";
            this.btnPlaybackIsOpenSound.Size = new System.Drawing.Size(97, 36);
            this.btnPlaybackIsOpenSound.TabIndex = 13;
            this.btnPlaybackIsOpenSound.Text = "IsOpenSound";
            this.btnPlaybackIsOpenSound.UseVisualStyleBackColor = true;
            this.btnPlaybackIsOpenSound.Click += new System.EventHandler(this.btnPlaybackIsOpenSound_Click);
            // 
            // btnPlaybackCloseShareSound
            // 
            this.btnPlaybackCloseShareSound.Location = new System.Drawing.Point(402, 17);
            this.btnPlaybackCloseShareSound.Name = "btnPlaybackCloseShareSound";
            this.btnPlaybackCloseShareSound.Size = new System.Drawing.Size(97, 36);
            this.btnPlaybackCloseShareSound.TabIndex = 12;
            this.btnPlaybackCloseShareSound.Text = "CloseShare Sound";
            this.btnPlaybackCloseShareSound.UseVisualStyleBackColor = true;
            this.btnPlaybackCloseShareSound.Click += new System.EventHandler(this.btnPlaybackCloseShareSound_Click);
            // 
            // btnPlaybackOpenShareSound
            // 
            this.btnPlaybackOpenShareSound.Location = new System.Drawing.Point(279, 17);
            this.btnPlaybackOpenShareSound.Name = "btnPlaybackOpenShareSound";
            this.btnPlaybackOpenShareSound.Size = new System.Drawing.Size(97, 36);
            this.btnPlaybackOpenShareSound.TabIndex = 11;
            this.btnPlaybackOpenShareSound.Text = "OpenShare Sound";
            this.btnPlaybackOpenShareSound.UseVisualStyleBackColor = true;
            this.btnPlaybackOpenShareSound.Click += new System.EventHandler(this.btnPlaybackOpenShareSound_Click);
            // 
            // btnPlaybackCloseSound
            // 
            this.btnPlaybackCloseSound.Location = new System.Drawing.Point(156, 17);
            this.btnPlaybackCloseSound.Name = "btnPlaybackCloseSound";
            this.btnPlaybackCloseSound.Size = new System.Drawing.Size(97, 36);
            this.btnPlaybackCloseSound.TabIndex = 10;
            this.btnPlaybackCloseSound.Text = "CloseSound";
            this.btnPlaybackCloseSound.UseVisualStyleBackColor = true;
            this.btnPlaybackCloseSound.Click += new System.EventHandler(this.btnPlaybackCloseSound_Click);
            // 
            // btnPlaybackOpenSound
            // 
            this.btnPlaybackOpenSound.Location = new System.Drawing.Point(33, 17);
            this.btnPlaybackOpenSound.Name = "btnPlaybackOpenSound";
            this.btnPlaybackOpenSound.Size = new System.Drawing.Size(97, 36);
            this.btnPlaybackOpenSound.TabIndex = 9;
            this.btnPlaybackOpenSound.Text = "OpenSound";
            this.btnPlaybackOpenSound.UseVisualStyleBackColor = true;
            this.btnPlaybackOpenSound.Click += new System.EventHandler(this.btnPlaybackOpenSound_Click);
            // 
            // PlaybackPicture
            // 
            this.PlaybackPicture.Controls.Add(this.groupBox16);
            this.PlaybackPicture.Controls.Add(this.textPlaybackPicPath);
            this.PlaybackPicture.Controls.Add(this.label61);
            this.PlaybackPicture.Controls.Add(this.comboBoxPlaybackPicFormat);
            this.PlaybackPicture.Controls.Add(this.label60);
            this.PlaybackPicture.Location = new System.Drawing.Point(4, 22);
            this.PlaybackPicture.Name = "PlaybackPicture";
            this.PlaybackPicture.Size = new System.Drawing.Size(902, 407);
            this.PlaybackPicture.TabIndex = 4;
            this.PlaybackPicture.Text = "Picture";
            this.PlaybackPicture.UseVisualStyleBackColor = true;
            // 
            // groupBox16
            // 
            this.groupBox16.Controls.Add(this.pictureBoxPlaybackPicture);
            this.groupBox16.Controls.Add(this.textPlaybackType);
            this.groupBox16.Controls.Add(this.label66);
            this.groupBox16.Controls.Add(this.textPlaybackSize);
            this.groupBox16.Controls.Add(this.label65);
            this.groupBox16.Controls.Add(this.textPlaybackQuality);
            this.groupBox16.Controls.Add(this.label64);
            this.groupBox16.Controls.Add(this.textPlaybackHeight);
            this.groupBox16.Controls.Add(this.label63);
            this.groupBox16.Controls.Add(this.textPlaybackWidth);
            this.groupBox16.Controls.Add(this.label62);
            this.groupBox16.Controls.Add(this.btnPlaybackConvertToBmpFile);
            this.groupBox16.Controls.Add(this.btnPlaybackGet24BitPictureFile);
            this.groupBox16.Controls.Add(this.btnPlaybackConvertToJpegFile);
            this.groupBox16.Controls.Add(this.btnPlaybackGetPictureBuf);
            this.groupBox16.Location = new System.Drawing.Point(16, 73);
            this.groupBox16.Name = "groupBox16";
            this.groupBox16.Size = new System.Drawing.Size(466, 318);
            this.groupBox16.TabIndex = 4;
            this.groupBox16.TabStop = false;
            this.groupBox16.Text = "Picture";
            // 
            // pictureBoxPlaybackPicture
            // 
            this.pictureBoxPlaybackPicture.Location = new System.Drawing.Point(262, 119);
            this.pictureBoxPlaybackPicture.Name = "pictureBoxPlaybackPicture";
            this.pictureBoxPlaybackPicture.Size = new System.Drawing.Size(180, 180);
            this.pictureBoxPlaybackPicture.TabIndex = 14;
            this.pictureBoxPlaybackPicture.TabStop = false;
            // 
            // textPlaybackType
            // 
            this.textPlaybackType.Location = new System.Drawing.Point(115, 273);
            this.textPlaybackType.Name = "textPlaybackType";
            this.textPlaybackType.Size = new System.Drawing.Size(125, 21);
            this.textPlaybackType.TabIndex = 13;
            // 
            // label66
            // 
            this.label66.AutoSize = true;
            this.label66.Location = new System.Drawing.Point(59, 278);
            this.label66.Name = "label66";
            this.label66.Size = new System.Drawing.Size(35, 12);
            this.label66.TabIndex = 12;
            this.label66.Text = "Type:";
            // 
            // textPlaybackSize
            // 
            this.textPlaybackSize.Location = new System.Drawing.Point(115, 235);
            this.textPlaybackSize.Name = "textPlaybackSize";
            this.textPlaybackSize.Size = new System.Drawing.Size(125, 21);
            this.textPlaybackSize.TabIndex = 11;
            // 
            // label65
            // 
            this.label65.AutoSize = true;
            this.label65.Location = new System.Drawing.Point(59, 239);
            this.label65.Name = "label65";
            this.label65.Size = new System.Drawing.Size(35, 12);
            this.label65.TabIndex = 10;
            this.label65.Text = "Size:";
            // 
            // textPlaybackQuality
            // 
            this.textPlaybackQuality.Location = new System.Drawing.Point(115, 197);
            this.textPlaybackQuality.Name = "textPlaybackQuality";
            this.textPlaybackQuality.Size = new System.Drawing.Size(125, 21);
            this.textPlaybackQuality.TabIndex = 9;
            // 
            // label64
            // 
            this.label64.AutoSize = true;
            this.label64.Location = new System.Drawing.Point(59, 201);
            this.label64.Name = "label64";
            this.label64.Size = new System.Drawing.Size(53, 12);
            this.label64.TabIndex = 8;
            this.label64.Text = "Quality:";
            // 
            // textPlaybackHeight
            // 
            this.textPlaybackHeight.Location = new System.Drawing.Point(115, 159);
            this.textPlaybackHeight.Name = "textPlaybackHeight";
            this.textPlaybackHeight.Size = new System.Drawing.Size(125, 21);
            this.textPlaybackHeight.TabIndex = 7;
            // 
            // label63
            // 
            this.label63.AutoSize = true;
            this.label63.Location = new System.Drawing.Point(59, 162);
            this.label63.Name = "label63";
            this.label63.Size = new System.Drawing.Size(47, 12);
            this.label63.TabIndex = 6;
            this.label63.Text = "Height:";
            // 
            // textPlaybackWidth
            // 
            this.textPlaybackWidth.Location = new System.Drawing.Point(115, 121);
            this.textPlaybackWidth.Name = "textPlaybackWidth";
            this.textPlaybackWidth.Size = new System.Drawing.Size(125, 21);
            this.textPlaybackWidth.TabIndex = 5;
            // 
            // label62
            // 
            this.label62.AutoSize = true;
            this.label62.Location = new System.Drawing.Point(59, 125);
            this.label62.Name = "label62";
            this.label62.Size = new System.Drawing.Size(41, 12);
            this.label62.TabIndex = 4;
            this.label62.Text = "Width:";
            // 
            // btnPlaybackConvertToBmpFile
            // 
            this.btnPlaybackConvertToBmpFile.Location = new System.Drawing.Point(247, 81);
            this.btnPlaybackConvertToBmpFile.Name = "btnPlaybackConvertToBmpFile";
            this.btnPlaybackConvertToBmpFile.Size = new System.Drawing.Size(129, 23);
            this.btnPlaybackConvertToBmpFile.TabIndex = 3;
            this.btnPlaybackConvertToBmpFile.Text = "ConvertToBmpFile";
            this.btnPlaybackConvertToBmpFile.UseVisualStyleBackColor = true;
            this.btnPlaybackConvertToBmpFile.Click += new System.EventHandler(this.btnPlaybackConvertToBmpFile_Click);
            // 
            // btnPlaybackGet24BitPictureFile
            // 
            this.btnPlaybackGet24BitPictureFile.Location = new System.Drawing.Point(59, 81);
            this.btnPlaybackGet24BitPictureFile.Name = "btnPlaybackGet24BitPictureFile";
            this.btnPlaybackGet24BitPictureFile.Size = new System.Drawing.Size(129, 23);
            this.btnPlaybackGet24BitPictureFile.TabIndex = 2;
            this.btnPlaybackGet24BitPictureFile.Text = "Get24BitPictureFile";
            this.btnPlaybackGet24BitPictureFile.UseVisualStyleBackColor = true;
            this.btnPlaybackGet24BitPictureFile.Click += new System.EventHandler(this.btnPlaybackGet24BitPictureFile_Click);
            // 
            // btnPlaybackConvertToJpegFile
            // 
            this.btnPlaybackConvertToJpegFile.Location = new System.Drawing.Point(247, 33);
            this.btnPlaybackConvertToJpegFile.Name = "btnPlaybackConvertToJpegFile";
            this.btnPlaybackConvertToJpegFile.Size = new System.Drawing.Size(129, 23);
            this.btnPlaybackConvertToJpegFile.TabIndex = 1;
            this.btnPlaybackConvertToJpegFile.Text = "ConvertToJpegFile";
            this.btnPlaybackConvertToJpegFile.UseVisualStyleBackColor = true;
            this.btnPlaybackConvertToJpegFile.Click += new System.EventHandler(this.btnPlaybackConvertToJpegFile_Click);
            // 
            // btnPlaybackGetPictureBuf
            // 
            this.btnPlaybackGetPictureBuf.Location = new System.Drawing.Point(59, 33);
            this.btnPlaybackGetPictureBuf.Name = "btnPlaybackGetPictureBuf";
            this.btnPlaybackGetPictureBuf.Size = new System.Drawing.Size(129, 23);
            this.btnPlaybackGetPictureBuf.TabIndex = 0;
            this.btnPlaybackGetPictureBuf.Text = "GetPictureBuf";
            this.btnPlaybackGetPictureBuf.UseVisualStyleBackColor = true;
            this.btnPlaybackGetPictureBuf.Click += new System.EventHandler(this.btnPlaybackGetPictureBuf_Click);
            // 
            // textPlaybackPicPath
            // 
            this.textPlaybackPicPath.Location = new System.Drawing.Point(317, 22);
            this.textPlaybackPicPath.Name = "textPlaybackPicPath";
            this.textPlaybackPicPath.Size = new System.Drawing.Size(123, 21);
            this.textPlaybackPicPath.TabIndex = 3;
            this.textPlaybackPicPath.Text = "D:\\";
            // 
            // label61
            // 
            this.label61.AutoSize = true;
            this.label61.Location = new System.Drawing.Point(275, 26);
            this.label61.Name = "label61";
            this.label61.Size = new System.Drawing.Size(35, 12);
            this.label61.TabIndex = 2;
            this.label61.Text = "Path:";
            // 
            // comboBoxPlaybackPicFormat
            // 
            this.comboBoxPlaybackPicFormat.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBoxPlaybackPicFormat.FormattingEnabled = true;
            this.comboBoxPlaybackPicFormat.Items.AddRange(new object[] {
            "0-BMP",
            "1-JPEG"});
            this.comboBoxPlaybackPicFormat.Location = new System.Drawing.Point(112, 23);
            this.comboBoxPlaybackPicFormat.Name = "comboBoxPlaybackPicFormat";
            this.comboBoxPlaybackPicFormat.Size = new System.Drawing.Size(121, 20);
            this.comboBoxPlaybackPicFormat.TabIndex = 1;
            // 
            // label60
            // 
            this.label60.AutoSize = true;
            this.label60.Location = new System.Drawing.Point(40, 25);
            this.label60.Name = "label60";
            this.label60.Size = new System.Drawing.Size(65, 12);
            this.label60.TabIndex = 0;
            this.label60.Text = "PicFormat:";
            // 
            // btnInit
            // 
            this.btnInit.Location = new System.Drawing.Point(14, 4);
            this.btnInit.Name = "btnInit";
            this.btnInit.Size = new System.Drawing.Size(92, 23);
            this.btnInit.TabIndex = 1;
            this.btnInit.Text = "Init System";
            this.btnInit.UseVisualStyleBackColor = true;
            this.btnInit.Click += new System.EventHandler(this.btnInit_Click);
            // 
            // btnUnInit
            // 
            this.btnUnInit.Location = new System.Drawing.Point(121, 4);
            this.btnUnInit.Name = "btnUnInit";
            this.btnUnInit.Size = new System.Drawing.Size(92, 23);
            this.btnUnInit.TabIndex = 2;
            this.btnUnInit.Text = "UnInit System";
            this.btnUnInit.UseVisualStyleBackColor = true;
            this.btnUnInit.Click += new System.EventHandler(this.btnUnInit_Click);
            // 
            // textBox24
            // 
            this.textBox24.Location = new System.Drawing.Point(144, 35);
            this.textBox24.Name = "textBox24";
            this.textBox24.Size = new System.Drawing.Size(87, 21);
            this.textBox24.TabIndex = 7;
            // 
            // label88
            // 
            this.label88.AutoSize = true;
            this.label88.Location = new System.Drawing.Point(144, 19);
            this.label88.Name = "label88";
            this.label88.Size = new System.Drawing.Size(65, 12);
            this.label88.TabIndex = 6;
            this.label88.Text = "HandleUser";
            // 
            // textBox25
            // 
            this.textBox25.Location = new System.Drawing.Point(15, 35);
            this.textBox25.Name = "textBox25";
            this.textBox25.Size = new System.Drawing.Size(120, 21);
            this.textBox25.TabIndex = 5;
            // 
            // label89
            // 
            this.label89.AutoSize = true;
            this.label89.Location = new System.Drawing.Point(15, 19);
            this.label89.Name = "label89";
            this.label89.Size = new System.Drawing.Size(59, 12);
            this.label89.TabIndex = 4;
            this.label89.Text = "AlarmCode";
            // 
            // textBox26
            // 
            this.textBox26.Location = new System.Drawing.Point(144, 35);
            this.textBox26.Name = "textBox26";
            this.textBox26.Size = new System.Drawing.Size(87, 21);
            this.textBox26.TabIndex = 7;
            // 
            // label90
            // 
            this.label90.AutoSize = true;
            this.label90.Location = new System.Drawing.Point(144, 19);
            this.label90.Name = "label90";
            this.label90.Size = new System.Drawing.Size(65, 12);
            this.label90.TabIndex = 6;
            this.label90.Text = "HandleUser";
            // 
            // textBox27
            // 
            this.textBox27.Location = new System.Drawing.Point(15, 35);
            this.textBox27.Name = "textBox27";
            this.textBox27.Size = new System.Drawing.Size(120, 21);
            this.textBox27.TabIndex = 5;
            // 
            // label91
            // 
            this.label91.AutoSize = true;
            this.label91.Location = new System.Drawing.Point(15, 19);
            this.label91.Name = "label91";
            this.label91.Size = new System.Drawing.Size(59, 12);
            this.label91.TabIndex = 4;
            this.label91.Text = "AlarmCode";
            // 
            // CSharpDemo
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1058, 680);
            this.Controls.Add(this.btnUnInit);
            this.Controls.Add(this.btnInit);
            this.Controls.Add(this.tabControl1);
            this.Name = "CSharpDemo";
            this.Text = "CSharpDemo";
            this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.CSharpDemo_FormClosed);
            this.tabControl1.ResumeLayout(false);
            this.General.ResumeLayout(false);
            this.General.PerformLayout();
            this.groupBox_ClientInfo.ResumeLayout(false);
            this.groupBox_LoginOP.ResumeLayout(false);
            this.groupBox_LoginOP.PerformLayout();
            this.groupBox_ModPWD.ResumeLayout(false);
            this.groupBox_ModPWD.PerformLayout();
            this.Device.ResumeLayout(false);
            this.Device.PerformLayout();
            this.groupBox_UserData.ResumeLayout(false);
            this.groupBox_UserData.PerformLayout();
            this.groupBox_OrgParam.ResumeLayout(false);
            this.groupBox_OrgParam.PerformLayout();
            this.Alarm.ResumeLayout(false);
            this.Alarm.PerformLayout();
            this.tabAlarm.ResumeLayout(false);
            this.tabAlarmQuery.ResumeLayout(false);
            this.tabAlarmQuery.PerformLayout();
            this.tabAlarmExport.ResumeLayout(false);
            this.tabAlarmExport.PerformLayout();
            this.tabAlarmConfirm.ResumeLayout(false);
            this.tabAlarmConfirm.PerformLayout();
            this.Preview.ResumeLayout(false);
            this.Preview.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBoxPreviewVideo)).EndInit();
            this.tabPreview.ResumeLayout(false);
            this.PreviewVideo.ResumeLayout(false);
            this.groupDisRegion.ResumeLayout(false);
            this.groupDisRegion.PerformLayout();
            this.groupStreamMode.ResumeLayout(false);
            this.groupStreamMode.PerformLayout();
            this.groupVideoPlay.ResumeLayout(false);
            this.groupVideoPlay.PerformLayout();
            this.groupMP.ResumeLayout(false);
            this.groupMP.PerformLayout();
            this.groupVBP.ResumeLayout(false);
            this.groupVBP.PerformLayout();
            this.PreviewPicture.ResumeLayout(false);
            this.groupGetPic.ResumeLayout(false);
            this.groupGetPic.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBoxPreviewPicture)).EndInit();
            this.PreviewSound.ResumeLayout(false);
            this.PreviewSound.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.trackBarPreviewVolume)).EndInit();
            this.PreviewRecord.ResumeLayout(false);
            this.PreviewRecord.PerformLayout();
            this.DeviceFunction.ResumeLayout(false);
            this.DeviceFunction.PerformLayout();
            this.tabControl_PTZ.ResumeLayout(false);
            this.tabPage_PTZOpFun.ResumeLayout(false);
            this.groupBox_PTZFun.ResumeLayout(false);
            this.groupBox_PTZFun.PerformLayout();
            this.tabPageOPCamera.ResumeLayout(false);
            this.groupBoxPTZDir.ResumeLayout(false);
            this.groupBoxPTZDir.PerformLayout();
            this.groupBox_PTZOpCam.ResumeLayout(false);
            this.groupBox_PTZOpCam.PerformLayout();
            this.groupBox_PTZFocus.ResumeLayout(false);
            this.groupBox_PTZFocus.PerformLayout();
            this.tabPagePreset.ResumeLayout(false);
            this.groupBox_PTZPreset.ResumeLayout(false);
            this.groupBox_PTZPreset.PerformLayout();
            this.tabPagePTZOther.ResumeLayout(false);
            this.groupBox_PTZSit.ResumeLayout(false);
            this.groupBox_PTZSit.PerformLayout();
            this.groupBox_PTZArrange.ResumeLayout(false);
            this.groupBox_PTZArrange.PerformLayout();
            this.groupBox_PTZAlarm.ResumeLayout(false);
            this.groupBox_PTZAlarm.PerformLayout();
            this.Playback.ResumeLayout(false);
            this.Playback.PerformLayout();
            this.tabControl3.ResumeLayout(false);
            this.PlaybackRecord.ResumeLayout(false);
            this.PlaybackRecord.PerformLayout();
            this.groupBox22.ResumeLayout(false);
            this.groupBox22.PerformLayout();
            this.groupBox21.ResumeLayout(false);
            this.groupBox21.PerformLayout();
            this.groupBox20.ResumeLayout(false);
            this.PlaybackPlayback.ResumeLayout(false);
            this.PlaybackPlayback.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBoxPlaybackVideo)).EndInit();
            this.groupBox19.ResumeLayout(false);
            this.groupBox18.ResumeLayout(false);
            this.PlaybackDownload.ResumeLayout(false);
            this.groupBox17.ResumeLayout(false);
            this.groupBox17.PerformLayout();
            this.PlaybackSound.ResumeLayout(false);
            this.PlaybackSound.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.trackBarPlaybackVolume)).EndInit();
            this.PlaybackPicture.ResumeLayout(false);
            this.PlaybackPicture.PerformLayout();
            this.groupBox16.ResumeLayout(false);
            this.groupBox16.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBoxPlaybackPicture)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.TabControl tabControl1;
        private System.Windows.Forms.TabPage General;
        private System.Windows.Forms.TabPage Device;
        private System.Windows.Forms.TabPage Alarm;
        private System.Windows.Forms.TabPage Preview;
        private System.Windows.Forms.TabPage DeviceFunction;
        private System.Windows.Forms.TabPage Playback;
        private System.Windows.Forms.Button btnInit;
        private System.Windows.Forms.Button btnUnInit;
        private System.Windows.Forms.Label label_IP;
        private System.Windows.Forms.CheckBox checkBoxDoMain;
        private System.Windows.Forms.TextBox textServerIP;
        private System.Windows.Forms.TextBox textServerPort;
        private System.Windows.Forms.Label label_Port;
        private System.Windows.Forms.TextBox textPassword;
        private System.Windows.Forms.Label label_PWD;
        private System.Windows.Forms.TextBox textUserName;
        private System.Windows.Forms.Label label_Name;
        private System.Windows.Forms.Button btnLogout;
        private System.Windows.Forms.Button btnLogin;
        private System.Windows.Forms.Button btnQueryEmap;
        private System.Windows.Forms.Button btnClientRoad;
        private System.Windows.Forms.Button btnClientVersion;
        private System.Windows.Forms.CheckBox checkBoxHttps;
        private System.Windows.Forms.Button btnSyncTime;
        private System.Windows.Forms.Button btnQueryMenuRight;
        private System.Windows.Forms.Button btnQueryFTP;
        private System.Windows.Forms.TextBox textResults;
        private System.Windows.Forms.Label label_Res;
        private System.Windows.Forms.GroupBox groupBox_ModPWD;
        private System.Windows.Forms.TextBox textNewPassword;
        private System.Windows.Forms.Button btnModifyPassword;
        private System.Windows.Forms.Label label_NewPWD;
        private System.Windows.Forms.TabControl tabPreview;
        private System.Windows.Forms.TabPage PreviewVideo;
        private System.Windows.Forms.GroupBox groupVideoPlay;
        private System.Windows.Forms.RadioButton radioPreviewSinglePlay;
        private System.Windows.Forms.TabPage PreviewPicture;
        private System.Windows.Forms.TabPage PreviewSound;
        private System.Windows.Forms.TabPage PreviewRecord;
        private System.Windows.Forms.RadioButton radioPreviewMultiviewPlay;
        private System.Windows.Forms.GroupBox groupVBP;
        private System.Windows.Forms.Label label_PreDevID;
        private System.Windows.Forms.TextBox textPreviewDeviceID;
        private System.Windows.Forms.CheckBox checkBoxPreviewDecode;
        private System.Windows.Forms.Label label_PreDT;
        private System.Windows.Forms.Label label_PreST;
        private System.Windows.Forms.TextBox textPreviewDataType;
        private System.Windows.Forms.TextBox textPreviewStreamType;
        private System.Windows.Forms.GroupBox groupMP;
        private System.Windows.Forms.TextBox textPreviewStartIndex;
        private System.Windows.Forms.TextBox textPreviewTrackID;
        private System.Windows.Forms.TextBox textPreviewScreenNum;
        private System.Windows.Forms.Label label_PreSI;
        private System.Windows.Forms.Label label_PreTrackID;
        private System.Windows.Forms.Label label_PreSN;
        private System.Windows.Forms.Button btnPreviewStartVideo;
        private System.Windows.Forms.GroupBox groupStreamMode;
        private System.Windows.Forms.TextBox textPreviewDelayTime;
        private System.Windows.Forms.TextBox textPreviewStreamMode;
        private System.Windows.Forms.Label label_PreDelT;
        private System.Windows.Forms.Label label_PreSM;
        private System.Windows.Forms.Button btnPreviewSetStreamMode;
        private System.Windows.Forms.Button btnPreviewGetStreamMode;
        private System.Windows.Forms.GroupBox groupDisRegion;
        private System.Windows.Forms.TextBox textPreviewLeft;
        private System.Windows.Forms.Label label_PreLeft;
        private System.Windows.Forms.TextBox textPreviewTop;
        private System.Windows.Forms.Label label_PreTop;
        private System.Windows.Forms.Button btnPreviewLockVideo;
        private System.Windows.Forms.Button btnPreviewSetRegion;
        private System.Windows.Forms.CheckBox checkBoxPreviewEnable;
        private System.Windows.Forms.TextBox textPreviewRight;
        private System.Windows.Forms.Label label_PreRight;
        private System.Windows.Forms.TextBox textPreviewBottom;
        private System.Windows.Forms.Label label_PreBottom;
        private System.Windows.Forms.PictureBox pictureBoxPreviewVideo;
        private System.Windows.Forms.TextBox textPreviewResult;
        private System.Windows.Forms.Label label_PreHR;
        private System.Windows.Forms.GroupBox groupGetPic;
        private System.Windows.Forms.ComboBox comboBoxPreviewPicture;
        private System.Windows.Forms.TextBox textPreviewPicPath;
        private System.Windows.Forms.Label label_PrePicPath;
        private System.Windows.Forms.Button btnPreviewGetPicSize;
        private System.Windows.Forms.PictureBox pictureBoxPreviewPicture;
        private System.Windows.Forms.Button btnPreviewGetPicToBuffer;
        private System.Windows.Forms.Button btnPreviewGetPicToFile;
        private System.Windows.Forms.Button btnPreviewIsOpenSound;
        private System.Windows.Forms.Button btnPreviewCloseShareSound;
        private System.Windows.Forms.Button btnPreviewOpenShareSound;
        private System.Windows.Forms.Button btnPreviewCloseSound;
        private System.Windows.Forms.Button btnPreviewOpenSound;
        private System.Windows.Forms.TrackBar trackBarPreviewVolume;
        private System.Windows.Forms.Button btnPreviewGetVolum;
        private System.Windows.Forms.Button btnPreviewSetVolume;
        private System.Windows.Forms.Label labelPreviewVolume;
        private System.Windows.Forms.Button btnPreviewIsRecordState;
        private System.Windows.Forms.Button btnPreviewStopRecord;
        private System.Windows.Forms.Button btnPreviewStartRecord;
        private System.Windows.Forms.Button btnPreviewSetSplitLen;
        private System.Windows.Forms.TextBox textPreviewSplitLen;
        private System.Windows.Forms.Label label_PreSplitLen;
        private System.Windows.Forms.TextBox textPreviewRecordPath;
        private System.Windows.Forms.Label label_PreRecPath;
        private System.Windows.Forms.Button btnPreviewStopVideo;
        private System.Windows.Forms.GroupBox groupBox_PTZFun;
        private System.Windows.Forms.TextBox textPtzChannelId;
        private System.Windows.Forms.Label label_PTZChnlCode;
        private System.Windows.Forms.TextBox textPtzTrackId;
        private System.Windows.Forms.Label label_PTZTraclID;
        private System.Windows.Forms.TextBox textPtzCruiseId;
        private System.Windows.Forms.Label label_PTZCruID;
        private System.Windows.Forms.ComboBox comboBoxPtzOpType;
        private System.Windows.Forms.Label label_PTZOpType;
        private System.Windows.Forms.ComboBox comboBoxPtzSwitch;
        private System.Windows.Forms.Label label_PTZSwith;
        private System.Windows.Forms.Button btnPtzOperateFunction;
        private System.Windows.Forms.ComboBox comboBoxPtzSwitchMenu;
        private System.Windows.Forms.Label label_PTZSM;
        private System.Windows.Forms.ComboBox comboBoxPtzMoveType;
        private System.Windows.Forms.Label label_PTZMT;
        private System.Windows.Forms.ComboBox comboBoxPtzAssisentType;
        private System.Windows.Forms.Label label_PTZAT;
        private System.Windows.Forms.ComboBox comboBoxPtzBorderType;
        private System.Windows.Forms.Label label_PTZBT;
        private System.Windows.Forms.GroupBox groupBox_PTZOpCam;
        private System.Windows.Forms.ComboBox comboBoxPtzOpCameraType;
        private System.Windows.Forms.Label label_PTZOpT;
        private System.Windows.Forms.ComboBox comboBoxPtzOpCameraCommand;
        private System.Windows.Forms.Label label_PTZCom;
        private System.Windows.Forms.ComboBox comboBoxPtzOpCameraDirect;
        private System.Windows.Forms.Label label_PTZCamDir;
        private System.Windows.Forms.Button btnPtzOpCamera;
        private System.Windows.Forms.TextBox textPtzOpCameraExtend;
        private System.Windows.Forms.Label label_PTZCamEx;
        private System.Windows.Forms.TextBox textPtzOpCameraStep;
        private System.Windows.Forms.Label label_PTZStep;
        private System.Windows.Forms.GroupBox groupBoxPTZDir;
        private System.Windows.Forms.TextBox textPtzOpDirectStepY;
        private System.Windows.Forms.Label label_PTZStepY;
        private System.Windows.Forms.TextBox textPtzOpDirectStepX;
        private System.Windows.Forms.Label label_PTZStepX;
        private System.Windows.Forms.ComboBox comboBoxPtzOpDirectCommand;
        private System.Windows.Forms.Label label_PTZDirCom;
        private System.Windows.Forms.ComboBox comboBoxPtzOpDirect;
        private System.Windows.Forms.Label label_PTZDir;
        private System.Windows.Forms.GroupBox groupBox_PTZFocus;
        private System.Windows.Forms.Button btnPtzOpDirect;
        private System.Windows.Forms.TextBox textPtzOpDirectExtend;
        private System.Windows.Forms.Label label_PTZDorEx;
        private System.Windows.Forms.Button btnPtzOpFocus;
        private System.Windows.Forms.TextBox textPtzOpFocusZoom;
        private System.Windows.Forms.Label label_PTZForZoom;
        private System.Windows.Forms.TextBox textPtzOpFocus;
        private System.Windows.Forms.Label label_PTZFor;
        private System.Windows.Forms.ComboBox comboBoxPtzOpFocusType;
        private System.Windows.Forms.Label label_PTZForOpType;
        private System.Windows.Forms.GroupBox groupBox_PTZPreset;
        private System.Windows.Forms.Label label_PTZPointET;
        private System.Windows.Forms.TextBox textPtzOpPresetStartTime;
        private System.Windows.Forms.Label label_PTZPointST;
        private System.Windows.Forms.TextBox textPtzOpPresetPointName;
        private System.Windows.Forms.Label label_PTZPointN;
        private System.Windows.Forms.TextBox textPtzOpPresetPointCode;
        private System.Windows.Forms.Label label_PTZPointC;
        private System.Windows.Forms.ComboBox comboBoxPtzOpPresetType;
        private System.Windows.Forms.Label label_PTZPreOpT;
        private System.Windows.Forms.Button btnPtzOpGetPreset;
        private System.Windows.Forms.Button btnPtzOpPresetPoint;
        private System.Windows.Forms.TextBox textPtzOpPresetEndTime;
        private System.Windows.Forms.GroupBox groupBox_PTZSit;
        private System.Windows.Forms.TextBox textPtzOpSitPosPointX;
        private System.Windows.Forms.Label label_PTZSitPX;
        private System.Windows.Forms.Label label_PTZSitEx;
        private System.Windows.Forms.TextBox textPtzOpSitPosPointZ;
        private System.Windows.Forms.Label label_PTZSitPZ;
        private System.Windows.Forms.TextBox textPtzOpSitPosPointY;
        private System.Windows.Forms.Label label_PTZSitPY;
        private System.Windows.Forms.GroupBox groupBox_PTZArrange;
        private System.Windows.Forms.Button btnPtzOpArrangePtz;
        private System.Windows.Forms.TextBox textPtzOpArrangeExtend;
        private System.Windows.Forms.Label label_PTZArrEx;
        private System.Windows.Forms.TextBox textPtzOpArrangeLockTime;
        private System.Windows.Forms.Label label_PTZArrLT;
        private System.Windows.Forms.Label label_PTZArrOT;
        private System.Windows.Forms.ComboBox comboBoxPtzOpArrangeType;
        private System.Windows.Forms.Button btnPtzOpSitPos;
        private System.Windows.Forms.TextBox textPtzOpSitPosExtend;
        private System.Windows.Forms.GroupBox groupBox_PTZAlarm;
        private System.Windows.Forms.Label label_PTZAlarmOT;
        private System.Windows.Forms.ComboBox comboBoxPtzOpAlarmOutType;
        private System.Windows.Forms.Button btnPtzOpAlarmOut;
        private System.Windows.Forms.ComboBox comboBoxPtzOpAlarmOutCommand;
        private System.Windows.Forms.Label label_PTZAlarmCom;
        private System.Windows.Forms.Button btnPtzOpClear;
        private System.Windows.Forms.TextBox textPtzOpResult;
        private System.Windows.Forms.TabControl tabControl3;
        private System.Windows.Forms.TabPage PlaybackRecord;
        private System.Windows.Forms.TabPage PlaybackPlayback;
        private System.Windows.Forms.TabPage PlaybackDownload;
        private System.Windows.Forms.TabPage PlaybackSound;
        private System.Windows.Forms.TabPage PlaybackPicture;
        private System.Windows.Forms.TextBox textPlaybackResult;
        private System.Windows.Forms.Label labelPlaybackVolume;
        private System.Windows.Forms.Button btnPlaybackGetVolum;
        private System.Windows.Forms.Button btnPlaybackSetVolume;
        private System.Windows.Forms.TrackBar trackBarPlaybackVolume;
        private System.Windows.Forms.Button btnPlaybackIsOpenSound;
        private System.Windows.Forms.Button btnPlaybackCloseShareSound;
        private System.Windows.Forms.Button btnPlaybackOpenShareSound;
        private System.Windows.Forms.Button btnPlaybackCloseSound;
        private System.Windows.Forms.Button btnPlaybackOpenSound;
        private System.Windows.Forms.Label label61;
        private System.Windows.Forms.ComboBox comboBoxPlaybackPicFormat;
        private System.Windows.Forms.Label label60;
        private System.Windows.Forms.GroupBox groupBox16;
        private System.Windows.Forms.TextBox textPlaybackPicPath;
        private System.Windows.Forms.Button btnPlaybackConvertToBmpFile;
        private System.Windows.Forms.Button btnPlaybackGet24BitPictureFile;
        private System.Windows.Forms.Button btnPlaybackConvertToJpegFile;
        private System.Windows.Forms.Button btnPlaybackGetPictureBuf;
        private System.Windows.Forms.TextBox textPlaybackType;
        private System.Windows.Forms.Label label66;
        private System.Windows.Forms.TextBox textPlaybackSize;
        private System.Windows.Forms.Label label65;
        private System.Windows.Forms.TextBox textPlaybackQuality;
        private System.Windows.Forms.Label label64;
        private System.Windows.Forms.TextBox textPlaybackHeight;
        private System.Windows.Forms.Label label63;
        private System.Windows.Forms.TextBox textPlaybackWidth;
        private System.Windows.Forms.Label label62;
        private System.Windows.Forms.GroupBox groupBox17;
        private System.Windows.Forms.Button btnPlaybackGetRecordInfo;
        private System.Windows.Forms.Button btnPlaybackDownloadRecordByTime;
        private System.Windows.Forms.Button btnPlaybackPauseDownloadRecord;
        private System.Windows.Forms.Button btnPlaybackDownloadRecordByFile;
        private System.Windows.Forms.TextBox textPlaybackDiskID;
        private System.Windows.Forms.Label label72;
        private System.Windows.Forms.TextBox textPlaybackChannelID;
        private System.Windows.Forms.Label label67;
        private System.Windows.Forms.TextBox textPlaybackChannelName;
        private System.Windows.Forms.Label label68;
        private System.Windows.Forms.TextBox textPlaybackDownloadPath;
        private System.Windows.Forms.Label label69;
        private System.Windows.Forms.TextBox textPlaybackDownloadFileName;
        private System.Windows.Forms.Label label70;
        private System.Windows.Forms.TextBox textPlaybackSplitSize;
        private System.Windows.Forms.Label label71;
        private System.Windows.Forms.Label label73;
        private System.Windows.Forms.Label label74;
        private System.Windows.Forms.Label label75;
        private System.Windows.Forms.ComboBox comboBoxPlaybackRecordType;
        private System.Windows.Forms.ComboBox comboBoxPlaybackNameRule;
        private System.Windows.Forms.ComboBox comboBoxPlaybackFileFormat;
        private System.Windows.Forms.GroupBox groupBox18;
        private System.Windows.Forms.ComboBox comboBoxPlaybackDirection;
        private System.Windows.Forms.Label label77;
        private System.Windows.Forms.ComboBox comboBoxPlaybackSpeed;
        private System.Windows.Forms.Label label76;
        private System.Windows.Forms.GroupBox groupBox19;
        private System.Windows.Forms.Button btnPlaybackSetPlaybackSpeed;
        private System.Windows.Forms.Button btnPlaybackGetProviderType;
        private System.Windows.Forms.Button btnPlaybackGetPlayedTime;
        private System.Windows.Forms.Button btnPlaybackSeek;
        private System.Windows.Forms.Button btnPlaybackFrameStep;
        private System.Windows.Forms.Button btnPlaybackPausePlayback;
        private System.Windows.Forms.Button btnPlaybackFile;
        private System.Windows.Forms.Button btnPlaybackByTime;
        private System.Windows.Forms.DateTimePicker datePlaybackStartTime;
        private System.Windows.Forms.Label label78;
        private System.Windows.Forms.DateTimePicker datePlaybackEndTime;
        private System.Windows.Forms.Label label79;
        private System.Windows.Forms.ListView listViewPlaybackRecordInfo;
        private System.Windows.Forms.ColumnHeader Seq;
        private System.Windows.Forms.ColumnHeader SourceType;
        private System.Windows.Forms.ColumnHeader RecordType;
        private System.Windows.Forms.ColumnHeader StartTime;
        private System.Windows.Forms.ColumnHeader EndTime;
        private System.Windows.Forms.ColumnHeader FileName;
        private System.Windows.Forms.ColumnHeader FileSize;
        private System.Windows.Forms.ColumnHeader StreamType;
        private System.Windows.Forms.ColumnHeader bLock;
        private System.Windows.Forms.TextBox textPlaybackDeviceID;
        private System.Windows.Forms.GroupBox groupBox20;
        private System.Windows.Forms.Button btnPlaybackGetRecordStatus;
        private System.Windows.Forms.Button btnPlaybackQueryRecordDate;
        private System.Windows.Forms.Button btnPlaybackQueryRecord;
        private System.Windows.Forms.ComboBox comboBoxPlaybackStreamType;
        private System.Windows.Forms.ComboBox comboBoxPlaybackSourceType;
        private System.Windows.Forms.Label label82;
        private System.Windows.Forms.Label label83;
        private System.Windows.Forms.GroupBox groupBox21;
        private System.Windows.Forms.Button btnPlaybackQueryLockRecord;
        private System.Windows.Forms.Button btnPlaybackUnlockRecordFile;
        private System.Windows.Forms.Button btnPlaybackLockRecordFile;
        private System.Windows.Forms.Label label81;
        private System.Windows.Forms.Label label84;
        private System.Windows.Forms.CheckBox checkBoxPlaybackForce;
        private System.Windows.Forms.TextBox textPlaybackUserID;
        private System.Windows.Forms.GroupBox groupBox22;
        private System.Windows.Forms.Button btnPlaybackStartRemoteRecord;
        private System.Windows.Forms.Button btnPlaybackStopRemoteRecord;
        private System.Windows.Forms.Label label85;
        private System.Windows.Forms.TextBox textPlaybackDuration;
        private System.Windows.Forms.ComboBox comboBoxAlarmHandleStatus;
        private System.Windows.Forms.Label label_ConfirmHS;
        private System.Windows.Forms.TextBox textAlarmHandleMsg;
        private System.Windows.Forms.Label label_ConfirmHM;
        private System.Windows.Forms.TextBox textAlarmHandleUser;
        private System.Windows.Forms.Label label_ConfirmHU;
        private System.Windows.Forms.TextBox textAlarmAlarmCode;
        private System.Windows.Forms.Label label_ConfirmAC;
        private System.Windows.Forms.TextBox textBox24;
        private System.Windows.Forms.Label label88;
        private System.Windows.Forms.TextBox textBox25;
        private System.Windows.Forms.Label label89;
        private System.Windows.Forms.TextBox textBox26;
        private System.Windows.Forms.Label label90;
        private System.Windows.Forms.TextBox textBox27;
        private System.Windows.Forms.Label label91;
        private System.Windows.Forms.TextBox textAlarmQueryEndTime;
        private System.Windows.Forms.Label label_QueryAET;
        private System.Windows.Forms.TextBox textAlarmQueryBeginTime;
        private System.Windows.Forms.Label label_QueryABT;
        private System.Windows.Forms.Button btnAlarmConfirm;
        private System.Windows.Forms.TextBox textAlarmEmailList;
        private System.Windows.Forms.Label label_ConfirmEL;
        private System.Windows.Forms.TextBox textAlarmQueryHandleUser;
        private System.Windows.Forms.Label label_QueryHU;
        private System.Windows.Forms.TextBox textAlarmQueryAlarmCode;
        private System.Windows.Forms.Label label_QueryAlarmCode;
        private System.Windows.Forms.TextBox textAlarmQueryAlarmId;
        private System.Windows.Forms.Label label_QueryAlarmID;
        private System.Windows.Forms.TextBox textAlarmQueryOrgId;
        private System.Windows.Forms.Label label_QueryOrgID;
        private System.Windows.Forms.TextBox textAlarmQueryChannelId;
        private System.Windows.Forms.Label label_QueryChnlID;
        private System.Windows.Forms.TextBox textAlarmQueryDeviceId;
        private System.Windows.Forms.Label label_QueryDevID;
        private System.Windows.Forms.TextBox textAlarmQueryHandleEndTime;
        private System.Windows.Forms.Label label_QueryHET;
        private System.Windows.Forms.TextBox textAlarmQueryHandleBeginTime;
        private System.Windows.Forms.Label label_QueryHBT;
        private System.Windows.Forms.TextBox textAlarmQueryStatusList;
        private System.Windows.Forms.Label label_QueryASL;
        private System.Windows.Forms.TextBox textAlarmQueryTypeList;
        private System.Windows.Forms.Label label_QueryATL;
        private System.Windows.Forms.ComboBox comboBoxAlarmQuerySortOrder;
        private System.Windows.Forms.Label label_QuerySO;
        private System.Windows.Forms.TextBox textAlarmQueryPageNum;
        private System.Windows.Forms.Label label_QueryPN;
        private System.Windows.Forms.TextBox textAlarmQueryPageSize;
        private System.Windows.Forms.Label label_QueryPZ;
        private System.Windows.Forms.ComboBox comboBoxAlarmQuerySortType;
        private System.Windows.Forms.Label label_QueryST;
        private System.Windows.Forms.Button btnAlarmCountQuery;
        private System.Windows.Forms.Button btnAlarmTypeQuery;
        private System.Windows.Forms.Button btnAlarmQuery;
        private System.Windows.Forms.TextBox textAlarmQueryHandleStatusList;
        private System.Windows.Forms.Label label_QueryHSL;
        private System.Windows.Forms.TextBox textAlarmQueryGradeList;
        private System.Windows.Forms.Label label_QueryAGL;
        private System.Windows.Forms.ComboBox comboBoxAlarmExportSortType;
        private System.Windows.Forms.Label label_ExportST;
        private System.Windows.Forms.TextBox textAlarmExportHandleUser;
        private System.Windows.Forms.Label label_ExportHU;
        private System.Windows.Forms.TextBox textAlarmExportAlarmCode;
        private System.Windows.Forms.Label label_ExportAlarmCode;
        private System.Windows.Forms.TextBox textAlarmExportAlarmId;
        private System.Windows.Forms.Label label_ExportAlarmID;
        private System.Windows.Forms.TextBox textAlarmExportOrgId;
        private System.Windows.Forms.Label label_ExportOrgID;
        private System.Windows.Forms.TextBox textAlarmExportChannelId;
        private System.Windows.Forms.Label label_ExportChnlID;
        private System.Windows.Forms.TextBox textAlarmExportDeviceId;
        private System.Windows.Forms.Label label_ExportDevID;
        private System.Windows.Forms.TextBox textAlarmExportHandleEndTime;
        private System.Windows.Forms.Label label_ExportHET;
        private System.Windows.Forms.TextBox textAlarmExportHandleBeginTime;
        private System.Windows.Forms.Label label_ExportHBT;
        private System.Windows.Forms.TextBox textAlarmExportEndTime;
        private System.Windows.Forms.Label label_ExportAET;
        private System.Windows.Forms.TextBox textAlarmExportBeginTime;
        private System.Windows.Forms.Label label_ExportABT;
        private System.Windows.Forms.Button btnAlarmExport;
        private System.Windows.Forms.TextBox textAlarmExportHandleStatusList;
        private System.Windows.Forms.Label label_ExportHSL;
        private System.Windows.Forms.TextBox textAlarmExportGradeList;
        private System.Windows.Forms.Label label_ExportAGL;
        private System.Windows.Forms.TextBox textAlarmExportStatusList;
        private System.Windows.Forms.Label label_ExportASL;
        private System.Windows.Forms.TextBox textAlarmExportTypeList;
        private System.Windows.Forms.Label label_ExportATL;
        private System.Windows.Forms.ComboBox comboBoxAlarmExportSortOrder;
        private System.Windows.Forms.Label label_ExportSO;
        private System.Windows.Forms.TextBox textAlarmExportLanguage;
        private System.Windows.Forms.Label label_ExportLan;
        private System.Windows.Forms.CheckBox checkBoxAlarmForbiddenNotify;
        private System.Windows.Forms.TextBox textAlarmInfo;
        private System.Windows.Forms.Button btnAlarmClear;
        private System.Windows.Forms.CheckBox checkBoxAlarmStopRefresh;
        private System.Windows.Forms.GroupBox groupBox_OrgParam;
        private System.Windows.Forms.Label label_Org;
        private System.Windows.Forms.Button btnDeviceQueryByLayered;
        private System.Windows.Forms.Button btnDeviceQueryOrg;
        private System.Windows.Forms.CheckBox checkBoxDeviceCompress;
        private System.Windows.Forms.CheckBox checkBoxDeviceContainDevice;
        private System.Windows.Forms.CheckBox checkBox5;
        private System.Windows.Forms.TextBox textDeviceOrgCode;
        private System.Windows.Forms.CheckBox checkBox14;
        private System.Windows.Forms.CheckBox checkBox13;
        private System.Windows.Forms.CheckBox checkBox12;
        private System.Windows.Forms.CheckBox checkBox11;
        private System.Windows.Forms.CheckBox checkBox10;
        private System.Windows.Forms.CheckBox checkBox9;
        private System.Windows.Forms.CheckBox checkBox8;
        private System.Windows.Forms.CheckBox checkBox15;
        private System.Windows.Forms.CheckBox checkBox19;
        private System.Windows.Forms.CheckBox checkBox18;
        private System.Windows.Forms.CheckBox checkBox17;
        private System.Windows.Forms.CheckBox checkBox16;
        private System.Windows.Forms.CheckBox checkBox20;
        private System.Windows.Forms.Button button50;
        private System.Windows.Forms.Button btnDeviceDelUserData;
        private System.Windows.Forms.Button btnDeviceSaveUserData;
        private System.Windows.Forms.Button btnDeviceGetUserData;
        private System.Windows.Forms.TextBox textDeviceUserDataName;
        private System.Windows.Forms.Label label_UserDataName;
        private System.Windows.Forms.TextBox textDeviceOrgData;
        private System.Windows.Forms.TreeView treeViewDeviceOrg;
        private System.Windows.Forms.RadioButton radioButtonDeviceChannel;
        private System.Windows.Forms.RadioButton radioButtonDeviceDevice;
        private System.Windows.Forms.RadioButton radioButtonDeviceOrg;
        private System.Windows.Forms.PictureBox pictureBoxPlaybackVideo;
        private System.Windows.Forms.PictureBox pictureBoxPlaybackPicture;
        private System.Windows.Forms.ComboBox comboBoxPlaybackQueryRecordType;
        private System.Windows.Forms.Label label59;
        private System.Windows.Forms.GroupBox groupBox_UserData;
        private System.Windows.Forms.Label label_UserDataInfo;
        private System.Windows.Forms.TextBox textDeviceUserDataInfo;
        private System.Windows.Forms.TextBox textAlarmTypeQueryLanguage;
        private System.Windows.Forms.Label label_QueryLan;
        private System.Windows.Forms.GroupBox groupBox_LoginOP;
        private System.Windows.Forms.GroupBox groupBox_ClientInfo;
        private System.Windows.Forms.TabControl tabAlarm;
        private System.Windows.Forms.TabPage tabAlarmQuery;
        private System.Windows.Forms.TabPage tabAlarmExport;
        private System.Windows.Forms.TabPage tabAlarmConfirm;
        private System.Windows.Forms.TextBox textAlarmQueryPFAlarmCode;
        private System.Windows.Forms.Button btnAlarmQueryPF;
        private System.Windows.Forms.TextBox textAlarmCodeSource;
        private System.Windows.Forms.TextBox textAlarmBlockType;
        private System.Windows.Forms.Label label_ConfirmDS;
        private System.Windows.Forms.TextBox textAlarmBlockDuration;
        private System.Windows.Forms.Label label_ConfirmAT;
        private System.Windows.Forms.Button btnAlarmBlock;
        private System.Windows.Forms.Label label_ConfirmACS;
        private System.Windows.Forms.Label label_ConfirmAPF_AC;
        private System.Windows.Forms.ColumnHeader SSId;
        private System.Windows.Forms.ColumnHeader FileHandle;
        private System.Windows.Forms.ColumnHeader DiskId;
        private System.Windows.Forms.TabControl tabControl_PTZ;
        private System.Windows.Forms.TabPage tabPage_PTZOpFun;
        private System.Windows.Forms.TabPage tabPageOPCamera;
        private System.Windows.Forms.TabPage tabPagePreset;
        private System.Windows.Forms.TabPage tabPagePTZOther;
    }
}

