#ifndef DPSDK_DPSDK_H
#define DPSDK_DPSDK_H

#include "DPSDKDefine.h"

/**
 * @brief	Set log information.
 * @param	[IN] pLogPath	Log root directory
 * @param	[IN] iLogLevel	Log level. For details, see DPSDK_LOG_LEVEL_TYPE definition.
 * @return	Return 0 if succeeded. Return error code if failed.
 * @note	This interface shall be called before initialization.
 */
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_SetLogInfo(const DPSDK_CHAR* pLogPath,
													 DPSDK_INT32 iLogLevel);

/**
 * @brief	Enable decoding process.
 * @param	[IN] iProcessNum Process number enabled
 * @param	[IN] iChannelNum Decoding channel number of single process
 * @return	Return 0 if succeeded. Return error code if failed.
 * @note	This interface shall be called before initialization.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_StartDecodeProcess(DPSDK_INT32 iProcessNum, DPSDK_INT32 iChannelNum);
/**
 * @brief	System initialization.
 * @return	Return 0 if succeeded. Return error code if failed.
 */
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_Init();

/**
 * @brief	System deinitialization.
 * @return	Return 0 if succeeded. Return error code if failed.
 */
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_Uninit();

/**
 * @brief	Get SDK version number.
 * @return	Return 0 if succeeded. Return error code if failed.
 */
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetVersion();

/**
* @brief	Set platform data compression mode.
* @param	[IN] iCompressType			Compression mode. For details, see DPSDK_COMPRESS_TYPE definition.
* @return	Return 0 if succeeded. Return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_SetCompressType(DPSDK_INT32 iCompressType);

/**
 * @brief	Event callback definition.
 * @param	[IN] iEventType	Event type
 * @param	[IN] pEventBuf	Event buffer
 * @param	[IN] uiBufSize	Event buffer size
 * @param	[IN] pUserData	User data
 * @return
 */
typedef DPSDK_VOID (DPSDK_CALL * EventCallBack)(DPSDK_INT32 iEventType, 
                                                DPSDK_VOID* pEventBuf, 
                                                DPSDK_UINT32 uiBufSize, 
                                                DPSDK_VOID* pUserData); 

/**
* @brief	Data sync callback. Used for upper level data copy.
* @param	[IN] iDataType	Data type. For details, see DPSDK_DATA_TYPE definition.
* @param	[IN] pDataBuf	Data buffer. For types, see the corresponding description of DPSDK_DATA_TYPE.
* @param	[IN] uiBufSize	Data size
* @param	[IN] pUserData	User data
* @return
*/
typedef DPSDK_VOID(DPSDK_CALL * DPSDK_DataCallback)(DPSDK_INT32 iDataType, 
													DPSDK_VOID* pDataBuf, 
													DPSDK_UINT32 uiBufSize, 
													DPSDK_VOID* pUserData);

/**
 * @brief	Set event callback.
 * @param	[IN] iSessionID		User session ID
 * @param	[IN] fEventCallBack	Event callback function
 * @param	[IN] pUserData		User data
 * @return	Return 0 if succeeded. Return error code if failed.
 */
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_SetEventCallBack(DPSDK_INT32 iSessionID,
														   EventCallBack fEventCallBack = NULL,
														   DPSDK_VOID* pUserData = NULL);

/**
 * @brief	User login.
 * @param	[IN] pLoginParam	Login parameter
 * @param	[OUT] pSessionID	User session ID
 * @return	Return 0 if succeeded. Return error code if failed.
 */
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_Login(DPSDK_LOGIN_PARAM* pLoginParam,
												DPSDK_INT32* pSessionID);

/**
 * @brief	User logout.
 * @param	[IN] iSessionID	User session ID
 * @return	Return 0 if succeeded. Return error code if failed.
 */
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_Logout(DPSDK_INT32 iSessionID);

/**
 * @brief	Get version information of application client.
 * @param	[IN] iSessionID	User session ID
 * @param	[OUT] pBuf		Data buffer
 * @param	[IN] uiBufLen	Data buffer length
 * @return	Return 0 if succeeded. Return error code if failed.
 */
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetClientVersion(DPSDK_INT32 iSessionID,
														   DPSDK_CHAR* pBuf,
														   DPSDK_UINT32 uiBufLen);

/**
* @brief	Get address information of application client.
* @param	[IN] iSessionID	User session ID
* @param	[OUT] pBuf		Data buffer
* @param	[IN] uiBufLen	Data buffer length
* @return	Return 0 if succeeded. Return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetClientRoad(DPSDK_INT32 iSessionID,
														DPSDK_CHAR* pBuf,
														DPSDK_UINT32 uiBufLen);
/**
 * @brief	Get electronic map information.
 * @param	[IN] iSessionID		User session ID
 * @param	[OUT] pServerInfo	Map connection information
 * @return	Return 0 if succeeded. Return error code if failed.
 */
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetEMapInfo(DPSDK_INT32 iSessionID,
													  DPSDK_SERVER_INFO* pServerInfo);

/**
* @brief	Get URL address of IR heat map according to the channel.
* @param	[IN] iSessionID		User session ID
* @param	[IN] pChannelCode	Channel code
* @param	[OUT] pURL			IR heat map URL
* @param	[IN] iMaxLen		pURL max memory length
* @return	Return 0 if succeeded. Return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetRadiometryURL(DPSDK_INT32 iSessionID,
														   DPSDK_CHAR* pChannelCode,
														   DPSDK_CHAR* pURL,
														   DPSDK_INT32 iMaxLen);

/**
* @brief	Get the temp data of a selected point in the IR heat map according to the channel.
* @param	[IN] iSessionID		User session ID
* @param	[OUT] pQueryParam	Request for parameters
* @param	[OUT] pPointInfo	Temperature information
* @return	Return 0 if succeeded. Return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetRadiometryPointInfo(DPSDK_INT32 iSessionID,
																 DPSDK_RADIO_POINT_PARAM* pQueryParam,
																 DPSDK_RADIO_POINT_INFO* pPointInfo);

/**
 * @brief	Get FTP information.
 * @param	[IN] iSessionID				User session ID
 * @param	[IN] uiDataType				The searched FTP type: 0-unknown <all>, 1-alarm, 2-label, 3-access control. For details, see DPSDK_FTP_DATA_TYPE definition.
 * @param	[OUT] pFtpServerInfoList	FTP service list
 * @param	[IN] uiBufLen				Buffer length		
 * @return	Return 0 if succeeded. Return error code if failed.
 * @note	uiBufLen = sizeof(DPSDK_FTP_SERVER_INFO_LIST) + (num-1)*sizeof(DPSDK_FTP_SERVER_INFO)
 *			Num refers to the number of FTP services you need to query. Num >= 1.
 */
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetFtpInfo(DPSDK_INT32 iSessionID,
													 DPSDK_UINT32 uiDataType,
													 DPSDK_FTP_SERVER_INFO_LIST* pFtpServerInfoList,
													 DPSDK_UINT32 uiBufLen);

/**
 * @brief	Modify user password.
 * @param	[IN] iSessionID		User session ID
 * @param	[IN] pNewPwd		User new password
 * @return	Return 0 if succeeded. Return error code if failed.
 * @note	The length of pNewPwd shall be no longer than DPSDK_PWD_LEN.
 */
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_ModifyUserPwd(DPSDK_INT32 iSessionID,
														DPSDK_CHAR* pNewPwd);

/**
* @brief	Get menu right information. 
* @param	[IN] iSessionID				User session ID
* @param	[OUT] pMenuRightList		Menu right list
* @param	[IN] uiBufLen				Buffer length
* @param	[OUT] pForbiddenMenuList	Forbidden Menu right list
* @param	[IN] uiForbiddenMenuBufLen	Forbidden Menu Buffer length
* @return	Return 0 if succeeded. Return error code if failed.
* @note	uiBufLen = sizeof(DPSDK_MENU_RIGHT_LIST) + (num-1)*sizeof(DPSDK_MENU_RIGHT)
*			Num refers to the number of menu rights you need to query. Num >= 1.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetMenuRight(DPSDK_INT32 iSessionID,
													   DPSDK_MENU_RIGHT_LIST* pMenuRightList,
													   DPSDK_UINT32 uiBufLen,
													   DPSDK_MENU_RIGHT_LIST* pForbiddenMenuList,
													   DPSDK_UINT32 uiForbiddenMenuBufLen);

/**
* @brief	Get face menu information.
* @param	[IN] iSessionID				User session ID
* @param	[OUT] pMenuList				Right menu list
* @param	[IN] uiBufLen				Buffer length
* @return	Return 0 if succeeded. Return error code if failed.
* @note	uiBufLen = sizeof(DPSDK_FACE_MENU_LIST) + (num-1)*sizeof(DPSDK_FACE_MENU)
*			Num refers to the number of menu rights you need to query. Num >= 1.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetFaceMenuList(DPSDK_INT32 iSessionID,
														  DPSDK_FACE_MENU_LIST* pMenuList,
														  DPSDK_UINT32 uiBufLen);

/**
* @brief	Get vehicle big data menu information. 
* @param	[IN] iSessionID				User session ID
* @param	[OUT] pMenuList				Right menu list
* @param	[IN] uiBufLen				Buffer length
* @return	Return 0 if succeeded. Return error code if failed.
* @note	uiBufLen = sizeof(DPSDK_VEHICLE_MENU_LIST) + (num-1)*sizeof(DPSDK_VEHICLE_MENU)
*			Num refers to the number of menu rights you need to query. Num >= 1.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetVehicleMenuList(DPSDK_INT32 iSessionID,
															 DPSDK_VEHICLE_MENU_LIST* pMenuList,
															 DPSDK_UINT32 uiBufLen);

/**
* @brief	Get all sub service menu information. 
* @param	[IN] iSessionID				User session ID
* @param	[OUT] pMenuList				Right menu list
* @param	[IN] uiBufLen				Buffer length
* @return	Return 0 if succeeded. Return error code if failed.
* @note	uiBufLen = sizeof(DPSDK_SERVICE_MENU_LIST) + (num-1)*sizeof(DPSDK_SERVICE_MENU)
*			Num refers to the number of menu rights you need to query. Num >= 1.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetServiceMenuList(DPSDK_INT32 iSessionID,
															 DPSDK_SERVICE_MENU_LIST* pMenuList,
															 DPSDK_UINT32 uiBufLen);

/**
* @brief	Get menu information of electronic map.
* @param	[IN] iSessionID				User session ID
* @param	[OUT] pMenuList				Right menu list
* @param	[IN] uiBufLen				Buffer length
* @return	Return 0 if succeeded. Return error code if failed.
* @note	uiBufLen = sizeof(DPSDK_EMAP_MENU_LIST) + (num-1)*sizeof(DPSDK_EMAP_MENU)
*			Num refers to the number of menu rights you need to query. Num >= 1.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetEmapMenuList(DPSDK_INT32 iSessionID,
														  DPSDK_EMAP_MENU_LIST* pMenuList,
														  DPSDK_UINT32 uiBufLen);

/**
* @brief	Network-wide timing.
* @param	[IN] iSessionID				User session ID
* @return	Return 0 if succeeded. Return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_SyncTime(DPSDK_INT32 iSessionID);

/**
* @brief	Get REST handle.
* @param	[IN] iSessionID			User session ID
* @return	Return REST handle if succeeded. Return NULL if failed.
* @note		Temporary interface which needs to be removed later. 
*/
DPSDK_EXPORT DPSDK_LPVOID DPSDK_CALL DPSDK_GetRestHandle(DPSDK_INT32 iSessionID);

/**
* @brief	Set to enable https or not.
* @param	[IN] iHttpsMode			https mode, 0=disable, 1=enable one-way authentication. For details, see DPSDK_HTTPS_MODE definition.
* @return	
*/
DPSDK_EXPORT DPSDK_VOID DPSDK_CALL DPSDK_SetHttpsMode(DPSDK_INT32 iHttpsMode);

/**
* @brief	Set SSL info of MQ.
* @param	[IN] pSSLInfo			SSL info of MQ.
* @return
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_SetSSLInfo(DPSDK_MQ_SSL_INFO* pSSLInfo);

/**
* @brief	Get user token.
* @param	[IN] iSessionID			User session ID
* @param	[OUT] pTokenBuffer		Token buffer
* @param	[IN] iBufLen			Buffer length
* @return	Return 0 if succeeded. Return error code if failed.
* @note		Temporary interface which needs to be removed later.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetUserToken(DPSDK_INT32 iSessionID,
													   DPSDK_CHAR* pTokenBuffer, 
													   DPSDK_UINT32 iBufLen);

/**
* @brief	Get user login data.
* @param	[IN] iSessionID			User session ID
* @param	[OUT] pUserLoginData	User login data
* @return	Return 0 if succeeded. Return error code if failed.
* @note		Temporary interface which needs to be removed later.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetUserLoginData(DPSDK_INT32 iSessionID, 
														   DPSDK_USER_LOGIN_DATA* pUserLoginData);

/**
* @brief	Universal Json data transmission interface.
* @param	[IN] iSessionID			User session ID
* @param	[IN] pDeviceID			Device ID
* @param	[IN] pJsonData			Json data
* @param	[IN] pRspJsonData		Json returned data
* @return	Return 0 if succeeded. Return error code if failed.
* @note		pRspJsonData shall be released by DPSDK_ReleaseDataBuffer.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_JsonTransportData(DPSDK_INT32 iSessionID,
															DPSDK_CHAR* pDeviceID,
															DPSDK_CHAR* pJsonData,
															DPSDK_CHAR** pRspJsonData);

//////////////////////////////////////////////////////////////////////////
/*
 *	Video Preview Interface
 */

/**
 * @brief	Start unicast video preview.
 * @param	[IN] iSessionID			User session ID
 * @param	[IN] pRealPlayParam		Video preview parameters. For details, see DPSDK_REALPLAY_PARAM structure.
 * @param	[OUT] pMediaSessionID	 Media session ID
 * @return	Return 0 if succeeded. Return error code if failed.
 */
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_StartRealPlay(DPSDK_INT32 iSessionID, 
														DPSDK_REALPLAY_PARAM* pRealPlayParam,
														DPSDK_INT32* pMediaSessionID);

/**
 * @brief	Start multicast video preview.
 * @param	[IN] iSessionID			User session ID
 * @param	[IN] pMulitcastParam	Video preview parameters. For details, see DPSDK_MULITCAST_REALPLAY_PARAM structure.
 * @param	[OUT] pMediaSessionID	Media session ID
 * @return	Return 0 if succeeded. Return error code if failed.
 */
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_StartMulitcastRealPlay(DPSDK_INT32 iSessionID, 
																DPSDK_MULITCAST_REALPLAY_PARAM* pMulitcastParam,
																DPSDK_INT32* pMediaSessionID);

/**
 * @brief	Start multi-image video preview.
 * @param	[IN] iSessionID			User session ID
 * @param	[IN] pMulitviewParam	Video preview parameters. For details, see DPSDK_MULITVIEW_REALPLAY_PARAM structure.
 * @param	[OUT] pMediaSessionID	Media session ID
 * @return	Return 0 if succeeded. Return error code if failed.
 */
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_StartMulitviewRealPlay(DPSDK_INT32 iSessionID, 
																DPSDK_MULITVIEW_REALPLAY_PARAM* pMulitviewParam,
																DPSDK_INT32* pMediaSessionID);

/**
 * @brief	Close video preview.
 * @param	[IN] iSessionID			User session ID
 * @param	[IN] iMediaSessionID	Media session ID
 * @return	Return 0 if succeeded. Return error code if failed.
 */
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_StopRealPlay(DPSDK_INT32 iSessionID,
 													   DPSDK_INT32 iMediaSessionID);



/** 
* @brief	Get play mode.
* @param	[IN] iSessionID			User session ID
* @param	[IN] iMediaSessionID	Media Session ID
* @param	[OUT] pStreamMode		Play mode. For details, see DPSDK_STREAM_MODE.
* @return	Return 0 if succeeded. Return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetPlayStreamMode(DPSDK_INT32 iSessionID,
															DPSDK_INT32 iMediaSessionID,
															DPSDK_UINT32* pStreamMode);

/** 
* @brief	Set play mode.
* @param	[IN] iSessionID			User session ID
* @param	[IN] iMediaSessionID	Media Session ID
* @param	[IN] uiStreamMode		Play mode. For details, see DPSDK_STREAM_MODE Enum.
* @param	[IN] uiDelayTime		Timeout period. Effective when uiStreamMode is set to STREAM_CUSTOM_MODE.
* @return	Return 0 if succeeded. Return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_SetPlayStreamMode(DPSDK_INT32 iSessionID,
															DPSDK_INT32 iMediaSessionID,
															DPSDK_UINT32 uiStreamMode,
															DPSDK_UINT32 uiDelayTime);

/**
 * @brief	Operate (lock, unlock) videos.
 * @param	[IN] iSessionID			User session ID
 * @param	[IN] iMediaSessionID	Media Session ID
 * @param	[IN] pCodeID			Video channel ID
 * @param	[IN] iOperateType		Operation Type. For details, see DPSDK_VIDEO_LOCK_TYPE.
 * @return	Return 0 if succeeded. Return error code if failed.
 */
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_OperateRealPlay(DPSDK_INT32 iSessionID,
														  DPSDK_INT32 iMediaSessionID,
														  DPSDK_CHAR* pCodeID, 
														  DPSDK_INT32 iOperateType);

/**
* @brief	Get the URL of platform snapshot images.
* @param	[IN] iSessionID			User session ID
* @param	[IN] pChannelID			Channel ID
* @param	[OUT] pPicURL			Snapshot URL
* @param	[IN] uiPicURLSize		Memory size of pPicURL
* @return	Return 0 if succeeded. Return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_PlatformSnapshotURL(DPSDK_INT32 iSessionID,
														      DPSDK_CHAR* pChannelID,
														      DPSDK_CHAR* pPicURL,
															  DPSDK_UINT32 uiBufSize);

//////////////////////////////////////////////////////////////////////////
/*
 *	Video Playback Interface
 */

 /**
 * @brief	Query record information in the channel.
 * @param	[IN] iSessionID			User session ID
 * @param	[IN] pChannelID			Channel ID
 * @param	[OUT] pRecordInfo		Channel record information
 * @return	Return 0 if succeeded. Return error code if failed.
 */
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetRecordStatus(DPSDK_INT32 iSessionID, DPSDK_CHAR* pChannelID, DPSDK_RECORD_STATUS_INFO* pRecordInfo);

/**
* @brief	Add record tag.
* @param	[IN] iSessionID			User session ID
* @param	[IN] pAddRecordTagInfo	Add record tag parameters. For details, see DPSDK_ADD_RECORD_TAG_PARAM structure.
* @param	[OUT] pTagId			Tag ID
* @return	Return 0 if succeeded. Return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_AddRecordTag(DPSDK_INT32 iSessionID, DPSDK_ADD_RECORD_TAG_PARAM* pAddRecordTagInfo, DPSDK_INT32* pTagId);

/**
* @brief	Delete record tag.
* @param	[IN] iSessionID			User session ID
* @param	[IN] pTagId	Record tag ID. The number is specified by iCount.
* @param	[IN] iCount	Record tag Number
* @return	Return 0 if succeeded. Return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_DelRecordTag(DPSDK_INT32 iSessionID, DPSDK_UINT32* pTagId, DPSDK_INT32 iCount);

/**
* @brief	Query record tag.
* @param	[IN] iSessionID				User session ID
* @param	[IN] pQueryRecordTag		Conditions to query the record tag. For details, see DPSDK_QUERY_RECORD_TAG_PARAM structure.
* @param	[OUT] pRecordTagList		Query results. For detailed parameters, see DPSDK_RECORD_TAG_INFO_LIST.
* @param	[IN] uiBufLen			        Buffer length
* @return	Return 0 if succeeded. Return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_QueryRecordTag(DPSDK_INT32 iSessionID, DPSDK_QUERY_RECORD_TAG_PARAM* pQueryRecordTag, DPSDK_RECORD_TAG_INFO_LIST* pRecordTagList, DPSDK_UINT32 uiBufLen);

/**
* @brief	Lock the record.
* @param	[IN] iSessionID				User session ID
* @param	[IN] pLockRecordInfo		Locking record parameters. For details, see DPSDK_LOCK_RECORD_PARAM structure.
* @param	[OUT] pLockId				Lock ID
* @return	Return 0 if succeeded. Return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_LockRecord(DPSDK_INT32 iSessionID, DPSDK_LOCK_RECORD_PARAM* pLockRecordInfo, DPSDK_INT32* pLockId);

/**
* @brief	Unlock the record.
* @param	[IN] iSessionID				User session ID
* @param	[IN] pLockId				Unlock ID. The number is specified by iCount.
* @param	[IN] iCount					Unlock number
* @return	Return 0 if succeeded. Return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_UnlockRecord(DPSDK_INT32 iSessionID, DPSDK_UINT32* pLockId, DPSDK_INT32 iCount);

/**
* @brief	Query record lock information.
* @param	[IN] iSessionID				User session ID
* @param	[IN] pQueryLockRecord		Query the record lock conditions. For details, see DPSDK_QUERY_LOCK_RECORD_PARAM structure.
* @param	[OUT] pLockRecordList		Query results. For details, see DPSDK_RECORD_LOCK_INFO_LIST.
* @param	[IN] uiBufLen				Buffer length
* @return	Return 0 if succeeded. Return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_QueryLockRecord(DPSDK_INT32 iSessionID, DPSDK_QUERY_LOCK_RECORD_PARAM* pQueryLockRecord, DPSDK_RECORD_LOCK_INFO_LIST* pLockRecordList, DPSDK_UINT32 uiBufLen);

/**
* @brief	Lock record file.
* @param	[IN] iSessionID				User session ID
* @param	[IN] pLockFileInfo			Parameters to Lock Record Files
* @param	[OUT] pResult				Lock result
* @return	Return 0 if succeeded. Return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_LockRecordFile(DPSDK_INT32 iSessionID, DPSDK_LOCK_RECORD_FILE_PARAM* pLockFileInfo, DPSDK_LOCK_RECORD_FILE_RESULT* pResult);

/**
* @brief	Unlock record file.
* @param	[IN] iSessionID				User session ID
* @param	[IN] pUnlockFileInfo		Parameters to unlock record files
* @param	[OUT] pResult				Unlock result
* @return	Return 0 if succeeded. Return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_UnlockRecordFile(DPSDK_INT32 iSessionID, DPSDK_UNLOCK_RECORD_FILE_PARAM* pUnlockFileInfo, DPSDK_LOCK_RECORD_FILE_RESULT* pResult);

/**
* @brief	Query record Date.
* @param	[IN] iSessionID				User session ID
* @param	[IN] pQueryDateInfo			Conditions to query record date.For details, see DPSDK_QUERY_RECORD_DATE_PARAM structure.
* @param	[OUT] pRecordDate			Query results. For detailed parameters, see DPSDK_RECORD_DATE_INFO.
* @return	Return 0 if succeeded. Return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_QueryRecordDate(DPSDK_INT32 iSessionID, DPSDK_QUERY_RECORD_DATE_PARAM* pQueryDateInfo, DPSDK_RECORD_DATE_INFO* pRecordDate);

/**
* @brief	Query records.
* @param	[IN] iSessionID				User session ID
* @param	[IN] pQueryRecord			Conditions to query the record. For details, see DPSDK_QUERY_RECORD_PARAM structure.
* @param	[OUT] pRecordList			Query results. For detailed parameters, see DPSDK_RECORD_INFO_LIST.
* @param	[IN] uiBufLen				Buffer length
* @return	Return 0 if succeeded. Return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_QueryRecord(DPSDK_INT32 iSessionID, DPSDK_QUERY_RECORD_PARAM* pQueryRecord, DPSDK_RECORD_INFO_LIST* pRecordList, DPSDK_UINT32 uiBufLen);

/**
* @brief	Query alarm records
* @param	[IN] iSessionID				User session ID
* @param	[IN] pQueryAlarmRecord		Conditions to query alarm records.For details, see DPSDK_QUERY_ALARM_RECORD_PARAM structure.
* @param	[OUT] pRecordList			Query results. For detailed parameters, see DPSDK_RECORD_PARAM.
* @param	[IN] uiBufLen				Buffer length
* @return	Return 0 if succeeded. Return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_QueryAlarmRecord(DPSDK_INT32 iSessionID, DPSDK_QUERY_ALARM_RECORD_PARAM* pQueryAlarmRecord, DPSDK_RECORD_INFO_LIST* pRecordList, DPSDK_UINT32 uiBufLen);

 /**
 * @brief	Start playback by time
 * @param	[IN] iSessionID			User session ID
 * @param	[IN] pPlaybackParam		Playback by time parameters.For details, see DPSDK_PLAYBACK_PARAM structure.
 * @param	[OUT] pMediaSessionID	Media session ID
 * @return	Return 0 if succeeded. Return error code if failed.
 */
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_StartPlaybackByTime(DPSDK_INT32 iSessionID,
															  DPSDK_PLAYBACK_BY_TIME_PARAM* pPlaybackParam,
															  DPSDK_INT32* pMediaSessionID);
/**
* @brief	Start playback by record.
* @param	[IN] iSessionID			User session ID
* @param	[IN] pPlaybackParam		Playback by record parameters. For details, see DPSDK_PLAYBACK_PARAM structure.
* @param	[OUT] pMediaSessionID	Media session ID
* @return	Return 0 if succeeded. Return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_StartPlaybackByFile(DPSDK_INT32 iSessionID,
															  DPSDK_PLAYBACK_BY_FILE_PARAM* pPlaybackParam,
															  DPSDK_INT32* pMediaSessionID);

/** 
* @brief	Stop playing.
* @param	[IN] iSessionID			User session ID
* @param	[IN] iMediaSessionID	Media session ID
* @return	Return 0 if succeeded. Return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_StopPlayback(DPSDK_INT32 iSessionID,
													   DPSDK_INT32 iMediaSessionID);

/**
* @brief	Seek play.
* @param	[IN] iSessionID			User session ID
* @param	[IN] iMediaSessionID	Media session ID
* @param	[IN] pPlaybackSeekParam Seek playback parameters. For details, see DPSDK_PLAYBACK_SEEK_PARAM structure.
* @return	Return 0 if succeeded. Return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_PlaybackSeek(DPSDK_INT32 iSessionID,
													   DPSDK_INT32 iMediaSessionID, 
													   DPSDK_PLAYBACK_SEEK_PARAM* pPlaybackSeekParam);

/**
* @brief	Set the play speed.
* @param	[IN] iSessionID			User session ID
* @param	[IN] iMediaSessionID	Media session ID
* @param	[IN] eSpeed				Playback speed
* @return	Return 0 if succeeded. Return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_SetPlaybackSpeed(DPSDK_INT32 iSessionID,
														   DPSDK_INT32 iMediaSessionID, 
														   DPSDK_PLAYBACK_SPEED iSpeed);

/** 
* @brief	Pause the playback.
* @param	[IN] iSessionID			User session ID
* @param	[IN] iMediaSessionID	Media session ID
* @return	Return 0 if succeeded. Return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_PlaybackPause(DPSDK_INT32 iSessionID,
														DPSDK_INT32 iMediaSessionID);

/**
* @brief	Restore playback
* @param	[IN] iSessionID			User session ID
* @param	[IN] iMediaSessionID	Media session ID
* @return	Return 0 if succeeded. Return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_PlaybackResume(DPSDK_INT32 iSessionID,
														 DPSDK_INT32 iMediaSessionID);

/** 
* @brief	Single frame play.
* @param	[IN] iSessionID			User session ID
* @param	[IN] iMediaSessionID	Media session ID
* @return	Return 0 if succeeded. Return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_PlaybackFrameStep(DPSDK_INT32 iSessionID,
															DPSDK_INT32 iMediaSessionID);

/** 
* @brief	Get the current play time.
* @param	[IN] iSessionID			User session ID
* @param	[IN] iMediaSessionID	Media session ID
* @return	Return 0 if succeeded. Return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetPlayedTime(DPSDK_INT32 iSessionID,
														DPSDK_INT32 iMediaSessionID, 
														DPSDK_TIMET* pTime);

/**
* @brief	Get the stream provider type.
* @param	[IN] iSessionID			User session ID
* @param	[IN] iMediaSessionID	Media session ID
* @param	[OUT] pProviderType	Provider type	
* @return	Return 0 if succeeded. Return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetProviderType(DPSDK_INT32 iSessionID,
														  DPSDK_INT32 iMediaSessionID, 
														  DPSDK_INT32 *pProviderType);

/**
* @brief	Set stream callback of wall playback.
* @param	[IN] iSessionID			User session ID
* @param	[IN] iMediaSessionID	Media session ID
* @param	[IN] pFun				Callback function address
* @param	[IN] pUserData			Callback function user parameters
* @return	Return 0 if succeeded. Return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_SetTVWallPlaybackCallBack(DPSDK_INT32 iSessionID,
																	DPSDK_INT32 iMediaSessionID, 
																	DPSDK_TVWALL_PLAYBACK_CALLBACK pFun, 
																	DPSDK_VOID* pUserData);

/**
* @brief	Enable sync playback
* @param	[IN] iSessionID			User session ID
* @param	[IN] iMediaSessionID	Media session ID
* @param	[OUT] pSyncPlayHandle	handle of sync playback
* @return	Return 0 if succeeded. Return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_OpenSyncPlayback(DPSDK_INT32 iSessionID,
														   DPSDK_INT32 iMediaSessionID,
														   DPSDK_LPVOID *pSyncPlayHandle);

/**
* @brief	Add to sync playback
* @param	[IN] iSessionID			User session ID
* @param	[IN] iMediaSessionID	Media session ID
* @param	[IN] SyncPlayHandle		Handle of sync playback
* @return	Return 0 if succeeded. Return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_AddToSyncPlayback(DPSDK_INT32 iSessionID,
															DPSDK_INT32 iMediaSessionID,
															DPSDK_LPVOID SyncPlayHandle);

/**
* @brief	Delete sync playback
* @param	[IN] iSessionID			User session ID
* @param	[IN] iMediaSessionID	Media session ID
* @param	[IN] SyncPlayHandle		Handle of sync playback
* @return	Return 0 if succeeded. Return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_DelFromSyncPlayback(DPSDK_INT32 iSessionID,
															  DPSDK_INT32 iMediaSessionID,
															  DPSDK_LPVOID SyncPlayHandle);

/**
* @brief	Set the direction of sync playback.
* @param	[IN] iSessionID			User session ID
* @param	[IN] iMediaSessionID	Media session ID
* @param	[IN] SyncPlayHandle		Handle of sync playback
* @param	[IN] iDirection			Direction of sync playback: 0-forward; 1-backword.
* @return	Return 0 if succeeded. Return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_SetSyncPlaybackDirection(DPSDK_INT32 iSessionID,
																   DPSDK_INT32 iMediaSessionID,
																   DPSDK_LPVOID SyncPlayHandle, 
																   DPSDK_INT32 iDirection);

/**
* @brief	Set the speed of sync playback.
* @param	[IN] iSessionID			User session ID
* @param	[IN] iMediaSessionID	Media session ID
* @param	[IN] SyncPlayHandle		Handle of sync playback
* @param	[IN] fSpeed				Speed of sync playback
* @return	Return 0 if succeeded. Return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_SetSyncPlaybackSpeed(DPSDK_INT32 iSessionID,
															   DPSDK_INT32 iMediaSessionID,
															   DPSDK_LPVOID SyncPlayHandle, 
															   DPSDK_FLOAT fSpeed);

/**
* @brief	Pause sync playback
* @param	[IN] iSessionID			User session ID
* @param	[IN] iMediaSessionID	Media session ID
* @param	[IN] SyncPlayHandle		Handle of sync playback
* @return	Return 0 if succeeded. Return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_SyncPlaybackPause(DPSDK_INT32 iSessionID,
															DPSDK_INT32 iMediaSessionID,
															DPSDK_LPVOID SyncPlayHandle);

/**
* @brief	Restore sync playback
* @param	[IN] iSessionID			User session ID
* @param	[IN] iMediaSessionID	Media session ID
* @param	[IN] SyncPlayHandle		Handle of sync playback
* @param	[IN] bPause				Pause tag, true: pause; false: playbak resumed.
* @return	Return 0 if succeeded. Return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_SyncPlaybackResume(DPSDK_INT32 iSessionID,
															 DPSDK_INT32 iMediaSessionID,
															 DPSDK_LPVOID SyncPlayHandle);

/**
* @brief	Play by single frame
* @param	[IN] iSessionID			User session ID
* @param	[IN] iMediaSessionID	Media session ID
* @param	[IN] SyncPlayHandle		Handle of sync playback
* @return	Return 0 if succeeded. Return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_SyncPlayblackFrameStep(DPSDK_INT32 iSessionID,
																 DPSDK_INT32 iMediaSessionID,
																 DPSDK_LPVOID SyncPlayHandle);

/**
* @brief	Close sync playback.
* @param	[IN] iSessionID			User session ID
* @param	[IN] iMediaSessionID	Media session ID
* @param	[IN] SyncPlayHandle		Handle of sync playback
* @return	Return 0 if succeeded. Return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_CloseSyncPlayback(DPSDK_INT32 iSessionID,
															DPSDK_INT32 iMediaSessionID,
															DPSDK_LPVOID SyncPlayHandle);

/**
* @brief	Query if the playback stream is smart stream or not.
* @param	[IN] iSessionID			User session ID
* @param	[IN] iMediaSessionID	Media session ID
* @param	[OUT] pbSmartStream		Smart stream type
* @return	Return 0 if succeeded. Return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_QueryPlayblakSmartStream(DPSDK_INT32 iSessionID,
																   DPSDK_INT32 iMediaSessionID,
																   DPSDK_BOOL *pbSmartStream);

//////////////////////////////////////////////////////////////////////////
/*
 *	Local Playback Interface
 */

/**
* @brief	playback by local file
* @param	[IN] pPlaybackParam		Playback parameter, see DPSDK_PLAYBACK_BY_LOCALFILE_PARAM structure
* @param	[OUT] pMediaSessionID	Media session ID
* @return	Return 0 if succeeded. Return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_StartPlaybackByLocalFile(DPSDK_PLAYBACK_BY_LOCALFILE_PARAM* pPlaybackParam,
																   DPSDK_INT32* pMediaSessionID);

/**
* @brief	playback by local file
* @param	[IN] pPlaybackParam		Playback parameter, see DPSDK_PLAYBACK_BY_LOCALFILE_PARAM structure
* @param	[IN] iMediaSessionID	Media session ID, created by upper level and guaranteed SN and is over or equal to 0
* @return	Return 0 if succeeded. Return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_StartPlaybackByLocalFileEx(DPSDK_PLAYBACK_BY_LOCALFILE_PARAM* pPlaybackParam,
																     DPSDK_INT32 iMediaSessionID);

/**
* @brief	set play direction
* @param	[IN] iMediaSessionID	Media session ID
* @param	[IN] iDirection			Direction, 0=forward; 1=back
* @return	Return 0 if succeeded. Return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_SetLocalPlaybackDirection(DPSDK_INT32 iMediaSessionID, 
																	DPSDK_INT32 iDirection);

/**
* @brief	set play position
* @param	[IN] iMediaSessionID	Media session ID
* @param	[IN] fPlayPos			File relative position, range 0-100%
* @return	Return 0 if succeeded. Return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_SetLocalPlaybackPos(DPSDK_INT32 iMediaSessionID, 
															  DPSDK_FLOAT fPlayPos);

/**
* @brief	get play position
* @param	[IN] iMediaSessionID	Media session ID
* @param	[OUT] pPlayPos			File relative position, range 0-100%
* @return	Return 0 if succeeded. Return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetLocalPlaybackPos(DPSDK_INT32 iMediaSessionID, 
															  DPSDK_FLOAT* pPlayPos);

/**
* @brief	get play start and end time
* @param	[IN] iMediaSessionID	Media session ID
* @param	[OUT] pStartTime		Start time
* @param	[OUT] pEndTime			End time
* @return	Return 0 if succeeded. Return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetLocalPlaybackStartEndTime(DPSDK_INT32 iMediaSessionID, 
																       DPSDK_UINT32 *pStartTime,
																       DPSDK_UINT32 *pEndTime);

//////////////////////////////////////////////////////////////////////////
/*
 *	Voice Intercom Interface
 */

 /**
 * @brief	Start intercom
 * @param	[IN] iSessionID			User session ID
 * @param	[IN] pStartTalkParam	Start talk parameters
 * @param	[OUT] pStartTalkRes		Return the correct parameter when it does not match.
 * @param	[OUT] pMediaSessionID	Media session ID
 * @return	Return 0 if succeeded. Return error code if failed.
 */
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_StartTalk(DPSDK_INT32 iSessionID,
													DPSDK_START_TALK_PARAM* pStartTalkParam,
													DPSDK_START_TALK_RESULT* pStartTalkRes,
													DPSDK_INT32* pMediaSessionID);

/**
* @brief	Close intercom
* @param	[IN] iSessionID			User session ID
* @param	[IN] iMediaSessionID	Media session ID
* @return	Return 0 if succeeded. Return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_StopTalk(DPSDK_INT32 iSessionID,
												   DPSDK_INT32 iMediaSessionID);

//////////////////////////////////////////////////////////////////////////
/*
 *	Record Interface
 */

/**	
* @brief	Start to take records.
* @param	[IN] iSessionID			User session ID
* @param	[IN] iMediaSessionID	Media session ID
* @param	[IN] pFile				Include record names of all the channels.
* @param	[IN] uiSplitRecordLen	Split record length
* @return	Return 0 if succeeded. Return error code if failed.
* @remark	The imported record name does not have suffix.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_StartRecord(DPSDK_INT32 iSessionID,
													  DPSDK_INT32 iMediaSessionID,
													  DPSDK_CHAR* pFile,
													  DPSDK_UINT32 uiSplitRecordLen);

/**	
* @brief	Stop recording
* @param	[IN] iSessionID			User session ID
* @param	[IN] iMediaSessionID	Media session ID
* @param	[OUT] pRecordFile		Record file list. Record results are not needed when importing NULL.
* @param	[IN] uiBufLen			Buffer length
* @return	Return 0 if succeeded. Return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_StopRecord(DPSDK_INT32 iSessionID,
													 DPSDK_INT32 iMediaSessionID,
													 DPSDK_FILE_STORE_LIST* pRecordFile,
													 DPSDK_UINT32 uiBufLen);

/** 
* @brief	Check if the device is taking a record.
* @param	[IN] iSessionID			User session ID
* @param	[IN] iMediaSessionID	Media session ID
* @param	[OUT] pIsRecord			True: taking a record. False: not taking a record.
* @return	Return 0 if succeeded. Return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_IsRecordState(DPSDK_INT32 iSessionID,
														DPSDK_INT32 iMediaSessionID,
														DPSDK_BOOL* pIsRecord);

/** 
* @brief	Set split record length.
* @param	[IN] iSessionID			User session ID
* @param	[IN] iMediaSessionID	Media session ID
* @param	[IN] uiSplitRecordLen   Record split length. Unit: M
* @return	Return 0 if succeeded. Return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_SetSplitRecordLen(DPSDK_INT32 iSessionID,
															DPSDK_INT32 iMediaSessionID,
															DPSDK_UINT32 uiSplitRecordLen);

//////////////////////////////////////////////////////////////////////////
/*
*	Download Interface
*/

/**
* @brief	Download by time
* @param	[IN] iSessionID				User session ID
* @param	[IN] pDownloadByTimeParam   For detailed meanings, see DPSDK_DOWNLOAD_BY_TIME_PARAM.
* @param	[OUT] pMediaSessionID		Media session ID
* @return	Return 0 if succeeded. Return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_StartDownloadRecordByTime(DPSDK_INT32 iSessionID,
																	DPSDK_DOWNLOAD_BY_TIME_PARAM* pDownloadByTimeParam,
																	DPSDK_INT32* pMediaSessionID);

/**
* @brief	Download by file
* @param	[IN] iSessionID				User session ID
* @param	[IN] pDownloadByFileParam   For detailed meanings, see DPSDK_DOWNLOAD_BY_FILE_PARAM.
* @param	[OUT] pMediaSessionID		Media session ID
* @return	Return 0 if succeeded. Return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_StartDownloadRecordByFile(DPSDK_INT32 iSessionID, 
																	DPSDK_DOWNLOAD_BY_FILE_PARAM* pDownloadByFileParam, 
																	DPSDK_INT32* pMediaSessionID);

/**
* @brief	Stop download
* @param	[IN] iSessionID				User session ID
* @param	[IN] iMediaSessionID		Media session ID
* @return	Return 0 if succeeded. Return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_StopDownloadRecord(DPSDK_INT32 iSessionID, DPSDK_INT32 iMediaSessionID);

/**
* @brief	Pause download
* @param	[IN] iSessionID				User session ID
* @param	[IN] iMediaSessionID		Media session ID
* @return	Return 0 if succeeded. Return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_PauseDownloadRecord(DPSDK_INT32 iSessionID, DPSDK_INT32 iMediaSessionID);

/**
* @brief	Restore download
* @param	[IN] iSessionID				User session ID
* @param	[IN] iMediaSessionID		Media session ID
* @return	Return 0 if succeeded. Return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_ResumeDownloadRecord(DPSDK_INT32 iSessionID, DPSDK_INT32 iMediaSessionID);

/**
* @brief	Get record download information
* @param	[IN] iSessionID				User session ID
* @param	[IN] iMediaSessionID		Media session ID
* @param	[OUT] pDownloadInfo			Record download information. For detailed meanings, see DPSDK_DOWNLOAD_RECORD_INFO.
* @return	Return 0 if succeeded. Return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetDownloadRecordInfo(DPSDK_INT32 iSessionID, DPSDK_INT32 iMediaSessionID, DPSDK_DOWNLOAD_RECORD_INFO* pDownloadInfo, DPSDK_UINT32 uiBufLen);

/**
* @brief	Set download max task number
* @param	[IN] iSessionID				User session ID
* @param	[IN] iMaxTaskNum			Max task number
* @return	Return 0 if succeeded. Return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_SetDownloadMaxTaskNum(DPSDK_INT32 iSessionID, DPSDK_INT32 iMaxTaskNum);
//////////////////////////////////////////////////////////////////////////
/*
 *	Snapshot Interface
 */

/** 
* @brief	Snapshot and keep the picture into the file.
* @param	[IN] iSessionID			User session ID
* @param	[IN] iMediaSessionID	Media session ID
* @param	[IN] uiPicFormat		Picture format. For details, see DPSDK_PIC_FORMAT.
* @param	[IN] pPath				Picture storage path
* @return	Return 0 if succeeded. Return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_Get24BitPictureFile(DPSDK_INT32 iSessionID,
															  DPSDK_INT32 iMediaSessionID,
															  DPSDK_UINT32 uiPicFormat,
															  DPSDK_CHAR* pPath);

/**
* @brief	Snapshot and save the picture data in the buffer address.
* @param	[IN] iSessionID			User session ID
* @param	[IN] iMediaSessionID	Media session ID
* @param	[OUT] pPicBuf			The buffer addresses to store the picture data are distributed by the user and its size shall be no smaller than the picture size.
									The recommended size for BMP pictures: DPSDK_BITMAP_FILE_HEADER_LEN + DPSDK_BITMAP_INFO_HEADER_LEN + iVideoWidth * iVideoHeight * 4
*									The recommended size for JPEG pictures: iVideoWidth * iVideoHeight * 3 / 2
* @param	[IN] iBufsize			Buffer size
* @param	[OUT] pPicSize			Actually snapped picture size 
* @param	[IN] uiPicFormat		Picture format. For details, see DPSDK_PIC_FORMAT.
* @return	Return 0 if succeeded. Return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetPictureBuf(DPSDK_INT32 iSessionID,
														DPSDK_INT32 iMediaSessionID,
														DPSDK_CHAR* pPicBuf, 
														DPSDK_INT32 iBufsize,
														DPSDK_INT32* pPicSize, 
														DPSDK_UINT32 uiPicFormat);

/**
* @brief	Transfer picture data to pictures in BMP format.
* @param	[IN] iSessionID			User session ID
* @param	[IN] iMediaSessionID	Media session ID
* @param	[IN] pConvertBMP		Picture data needed to be transferred. For details, see DPSDK_CONVERT_BMP.
* @return	Return 0 if succeeded. Return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_ConvertToBmpFile(DPSDK_INT32 iSessionID,
														   DPSDK_INT32 iMediaSessionID,
														   DPSDK_CONVERT_BMP* pConvertBMP);

/**
* @brief	Transfer picture data to pictures in JPEG format.
* @param	[IN] iSessionID			User session ID
* @param	[IN] iMediaSessionID	Media session ID
* @param	[IN] pConvertJPEG	Picture data needed to be transferred. For details, see DPSDK_CONVERT_JPEG.
* @return	Return 0 if succeeded. Return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_ConvertToJpegFile(DPSDK_INT32 iSessionID,
															DPSDK_INT32 iMediaSessionID,
															DPSDK_CONVERT_JPEG* pConvertJPEG);

//////////////////////////////////////////////////////////////////////////
/*
 *	Fisheye Interface
 */

/** 
* @brief	Enable Fisheye
* @param	[IN] iSessionID			User Session ID
* @param	[IN] iMediaSessionID	Media Session ID
* @param	[IN] uiFishType			Fisheye enable type, refer to DPSDK_FISH_TYPE definition
* @param	[IN] pPtzChannelParam	Smart track initialization channel parameter
* @return	Return 0 if succeeded, return error code if failed. 
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_StartFisheyeEx(DPSDK_INT32 iSessionID,
														 DPSDK_INT32 iMediaSessionID,
														 DPSDK_UINT32 uiFishType, 
														 DPSDK_MHFPTZ_INIT_PARAM* pPtzChannelParam);

/** 
* @brief	Disable Fisheye
* @param	[IN] iSessionID			User Session ID
* @param	[IN] iMediaSessionID	Media Session ID
* @return	Return 0 if succeeded, return error code if failed. 
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_CloseFisheye(DPSDK_INT32 iSessionID,
													   DPSDK_INT32 iMediaSessionID);

/**
* @brief	Fisheye parameter initialization
* @param	[IN] iSessionID			User Session ID
* @param	[IN] iMediaSessionID	Media Session ID
* @param	[OUT] pOptParam			Fisheye Parameter
* @return	Return 0 if succeeded, return error code if failed. 
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_InitFisheyeOptParam(DPSDK_INT32 iSessionID,
															  DPSDK_INT32 iMediaSessionID,
															  DPSDK_FISH_OPTPARAM* pOptParam);

/**
* @brief	Fisheye Parameter Initialization
* @param	[IN] iSessionID			User Session ID
* @param	[IN] iMediaSessionID	Media Session ID
* @param	[OUT] pOptParam			Fisheye Parameter
* @return	Return 0 if succeeded, return error code if failed. 
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_ReleaseFisheyeOptParamBuffer(DPSDK_FISH_OPTPARAM* pOptParam);

/** 
* @brief	Update Fisheye Parameter
* @param	[IN] iSessionID			User Session ID
* @param	[IN] iMediaSessionID	Media Session ID
* @param	[IN] pOptParam			Fisheye Parameter
* @return	Return 0 if succeeded, return error code if failed. 
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_UpdateFisheyeOptParam(DPSDK_INT32 iSessionID,
																DPSDK_INT32 iMediaSessionID,
																DPSDK_FISH_UPDATE_PARAM* pOptParam);

/** 
* @brief	Acquire Fisheye Parameter
* @param	[IN] iSessionID			User Session ID
* @param	[IN] iMediaSessionID	Media Session ID
* @param	[OUT] pOptParam			Fisheye Parameter
* @return	Return 0 if succeeded, return error code if failed. 
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetFisheyeInfo(DPSDK_INT32 iSessionID,
														 DPSDK_INT32 iMediaSessionID,
														 DPSDK_FISH_OPTPARAM* pOptParam);

/**
* @brief	Set Fisheye Parameter
* @param	[IN] iSessionID			User Session ID
* @param	[IN] iMediaSessionID	Media Session ID
* @param	[IN] pOptParam			Fisheye Parameter
* @return	Return 0 if succeeded, return error code if failed. 
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_SetFisheyeInfo(DPSDK_INT32 iSessionID,
														 DPSDK_INT32 iMediaSessionID,
														 DPSDK_FISH_OPTPARAM* pOptParam);

/**
* @brief	Acquire Fisheye Status
* @param	[IN] iSessionID			User Session ID
* @param	[IN] iMediaSessionID	Media Session ID
* @param	[OUT] pIsOpened			If fisheye is enabled, true=enable, false=disable
* @return	Return 0 if succeeded, return error code if failed. 
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetFisheyeState(DPSDK_INT32 iSessionID,
														  DPSDK_INT32 iMediaSessionID,
														  DPSDK_BOOL* pIsOpened);

/**
* @brief	Set and acquire fisheye parameter
* @param	[IN] iSessionID			User session ID
* @param	[IN] iMediaSessionID	Media session ID
* @param	[IN OUT] pEptzParam		Fisheye Parameter
* @param	[IN] bSecondRegion		Fill in 1 when operating the second window under floating mode (It is 0 by default)
* @return	Return 0 if succeeded, return error code if failed. 
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetFisheyeEptzUpdateParam(DPSDK_INT32 iSessionID,
																	DPSDK_INT32 iMediaSessionID,
																	DPSDK_FISH_EPTZPARAM* pEptzParam, 
																	DPSDK_BOOL bSecondRegion = false);

/** 
* @brief	Enable or disable second fisheye window in the floating mode
* @param	[IN] iSessionID			User Session ID
* @param	[IN] iMediaSessionID	Media Session ID
* @param	[IN] hDestWnd			Window Handle
* @param	[IN] pOptParam			Fisheye Parameter
* @param	[IN] bEnable			Enable or diable the second window under floating mode
* @return	Return 0 if succeeded, return error code if failed. 
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_ShowFisheyeSecondRegion(DPSDK_INT32 iSessionID,
																  DPSDK_INT32 iMediaSessionID,
																  HCWND hDestWnd, 
																  DPSDK_FISH_OPTPARAM* pOptParam, 
																  DPSDK_BOOL bEnable);

/**
* @brief	Control fisheye device to zoom and move/acquire smart track info
* @param	[IN] iSessionID			User Session ID
* @param	[IN] iMediaSessionID	Media Session ID
* @param	[IN OUT] pFishBasePtzInfo  E-PTZ zoom and move option
* @return	Return 0 if succeeded, return error code if failed. 
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_ControlFishEye(DPSDK_INT32 iSessionID,
														 DPSDK_INT32 iMediaSessionID,
														 DPSDK_FISH_EPTZPARAM* pFishBasePtzInfo);

/**
* @brief	Acquire fisheye PTZ parameter
* @param	[IN] iSessionID			User Session ID
* @param	[IN] iMediaSessionID	Media Session ID
* @param	[IN OUT] pEptzFrameInfo Fisheye PTZ related parameter storage structure
* @param	[IN] bSecondRegion		Fill in 1 when operating the second window under floating mode(It is 0 by default)
* @return	Return 0 if succeeded, return error code if failed. 
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetFishEyePtzInfo(DPSDK_INT32 iSessionID,
															DPSDK_INT32 iMediaSessionID,
															DPSDK_FISH_EPTZPARAM* pEptzFrameInfo,
															DPSDK_BOOL bSecondRegion);

/**
* @brief	Set Fisheye Parameter
* @param	[IN] iSessionID			User Session ID
* @param	[IN] iMediaSessionID	Media Session ID
* @param	[IN] pFishParams		Fisheye Parameter
* @return	Return 0 if succeeded, return error code if failed. 

*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_SetFisheyeParams(DPSDK_INT32 iSessionID,
														   DPSDK_INT32 iMediaSessionID,
														   DPSDK_FISH_PARAMS* pFishParams);

/**
* @brief	Acquire Fisheye Parameter
* @param	[IN] iSessionID			User Session ID
* @param	[IN] iMediaSessionID	Media Session ID
* @param	[IN OUT] pFishOptParamBase  Fisheye Parameter
* @return	Return 0 if succeeded, return error code if failed. 
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetFisheyeParams(DPSDK_INT32 iSessionID,
														   DPSDK_INT32 iMediaSessionID,
														   DPSDK_FISH_OPTPARAM* pFishOptParamBase);

//////////////////////////////////////////////////////////////////////////
/*
 *	        Video Basic Inteface
 */

/** 
* @brief	Enable Hardware Acceleration
* @param	[IN] iSessionID			User Session ID
* @param	[IN] iMediaSessionID	Media Session ID
* @param	[IN] iDecodeType		Decode type, refer to DPSDK_DECODE_TYPE definition
* @return	Return 0 if succeeded, return error code if failed. 
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_SetDecode(DPSDK_INT32 iSessionID,
													DPSDK_INT32 iMediaSessionID,
													DPSDK_INT32 iDecodeType);

/** 
* @brief	If it is to enable internal adjustment strategy of HD image
* @param	[IN] iSessionID			User Session ID
* @param	[IN] iMediaSessionID	Media Session ID
* @param	[IN] bEnable			Function Sign true enable false disable
* @return	Return 0 if succeeded, return error code if failed. 
* @remark	The interface is only used for network playback
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_EnableLargePicAdjustment(DPSDK_INT32 iSessionID,
																   DPSDK_INT32 iMediaSessionID,
																   DPSDK_BOOL bEnable);
/** 
* @brief	Acquire original picture size
* @param	[IN] iSessionID			User Session ID
* @param	[IN] iMediaSessionID	Media Session ID
* @param	[OUT] pWidth			Width
* @param	[OUT] pHeight			Height
* @return	Return 0 if succeeded, return error code if failed. 
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetPictureSize(DPSDK_INT32 iSessionID,
 														 DPSDK_INT32 iMediaSessionID,
														 DPSDK_INT32* pWidth, 
														 DPSDK_INT32* pHeight);

/** 
* @brief	Acquire Volume
* @param	[IN] iSessionID			User Session ID
* @param	[IN] iMediaSessionID	Media Session ID
* @param	[OUT] pVolume			Volume Value
* @return	Return 0 if succeeded, return error code if failed. 
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetVolume(DPSDK_INT32 iSessionID,
													DPSDK_INT32 iMediaSessionID,
													DPSDK_UINT32* pVolume);

/** 
* @brief	Set Volume
* @param	[IN] iSessionID			User Session ID
* @param	[IN] iMediaSessionID	Media Session ID
* @param	[IN] uiVolume			Volume Value
* @return	Return 0 if succeeded, return error code if failed. 
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_SetVolume(DPSDK_INT32 iSessionID,
													DPSDK_INT32 iMediaSessionID,
													DPSDK_UINT32 uiVolume);

/**
* @brief	Open audio via exclusive mode
* @param	[IN] iSessionID			User Session ID
* @param	[IN] iMediaSessionID	Media Session ID
* @return	Return 0 if succeeded, return error code if failed. 
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_OpenSound(DPSDK_INT32 iSessionID,
													DPSDK_INT32 iMediaSessionID);

/**
* @brief	Close audio via exclusive mode
* @param	[IN] iSessionID			User Session ID
* @param	[IN] iMediaSessionID	Media Session ID
* @return	Return 0 if succeeded, return error code if failed. 
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_CloseSound(DPSDK_INT32 iSessionID,
													 DPSDK_INT32 iMediaSessionID);

/** 
* @brief	Open audio via sharing mode
* @param	[IN] iSessionID			User Session ID
* @param	[IN] iMediaSessionID	Media Session ID
* @return	Return 0 if succeeded, return error code if failed. 
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_OpenSoundShare(DPSDK_INT32 iSessionID,
														 DPSDK_INT32 iMediaSessionID);

/** 
* @brief	Close sharing mode audio
* @param	[IN] iSessionID			User Session ID
* @param	[IN] iMediaSessionID	Media Session ID
* @return	Return 0 if succeeded, return error code if failed. 
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_CloseSoundShare(DPSDK_INT32 iSessionID,
														  DPSDK_INT32 iMediaSessionID);

/**
* @brief	Audio Enable Status
* @param	[IN] iSessionID			User Session ID
* @param	[IN] iMediaSessionID	Media Session ID
* @param	[OUT] pIsOpenSound		true=Enable, false=Disable
* @return	Return 0 if succeeded, return error code if failed. 
* @remark	It is true for enabling both exclusive and sharing audio
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_IsOpenSoundState(DPSDK_INT32 iSessionID,
														   DPSDK_INT32 iMediaSessionID,
														   DPSDK_BOOL* pIsOpenSound);

/**
* @brief	get video packet loss rate
* @param	[IN] iSessionID			User Session ID
* @param	[IN] iMediaSessionID	Media Session ID
* @param	[OUT] pLossPacketRate	Loss rate
* @return	Return 0 if succeeded, return error code if failed. 
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetLossPacketRate(DPSDK_INT32 iSessionID,
															DPSDK_INT32 iMediaSessionID,
															DPSDK_DOUBLE*  pLossPacketRate);


/**
* @brief	Set Video Parameter
* @param	[IN] iSessionID			User Session ID
* @param	[IN] iMediaSessionID	Media Session ID
* @param	[IN] iBrightness		Brightness	Default 64: Range 0-128
* @param	[IN] iContrast			Contrast	Default 64: Range 0-128
* @param	[IN] iSaturation		Saturation	Default 64: Range 0-128
* @param	[IN] iHue				Hue	        Default 64: Range 0-128
* @return	Return 0 if succeeded, return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_SetColor(DPSDK_INT32 iSessionID,
												   DPSDK_INT32 iMediaSessionID,
												   DPSDK_INT32 iBrightness, 
												   DPSDK_INT32 iContrast, 
												   DPSDK_INT32 iSaturation, 
												   DPSDK_INT32 iHue);

/**
* @brief	Acquire Video Parameter
* @param	[IN] iSessionID			User Session ID
* @param	[IN] iMediaSessionID	Media Session ID
* @param	[OUT] pBrightness		Brightness 	Default 64: Range 0-128
* @param	[OUT] pContrast			Contrast	Default 64£∫Range 0-128
* @param	[OUT] pSaturation		Saturation	Default 64£∫Range 0-128
* @param	[OUT] pHue				Hue 	    Default 64£∫Range 0-128
* @return	Return 0 if succeeded, return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetColor(DPSDK_INT32 iSessionID,
												   DPSDK_INT32 iMediaSessionID,
												   DPSDK_INT32* pBrightness, 
												   DPSDK_INT32* pContrast, 
												   DPSDK_INT32* pSaturation, 
												   DPSDK_INT32* pHue);

/** 
* @brief	Set or add display area, it can display with regional zoom
* @param	[IN] iSessionID			User Session ID
* @param	[IN] iMediaSessionID	Media Session ID
* @param	[IN] pRECT				Regional display area 
* @param	[IN] hDestWnd			Display window handle
* @param	[IN] bEnable			Open or close display area 	true Open	false Close
* @return	Return 0 if succeeded, return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_SetDisplayRegion(DPSDK_INT32 iSessionID,
														   DPSDK_INT32 iMediaSessionID,
														   DPSDK_RECT* pRECT, 
														   HCWND hDestWnd, 
														   DPSDK_BOOL bEnable);

/** 
* @brief	Enable video enhance algorithm function, it needs to include IvseDll.dll library	
* @param	[IN] iSessionID			User Session ID
* @param	[IN] iMediaSessionID	Media Session ID
* @return	Return 0 if succeeded, return error code if failed.
* @remark	It is only valid to Dahua stream, the algorithm takes longer time, it is not recommeded to use for HD video
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_StartIVSE(DPSDK_INT32 iSessionID,
													DPSDK_INT32 iMediaSessionID);

/**
* @brief	Stop video enhance algorithm function
* @param	[IN] iSessionID			User Session ID
* @param	[IN] iMediaSessionID    Media Session ID
* @return	Return 0 if succeeded, return error code if failed.
* @remark	It is only valid to Dahua stream
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_StopIVSE(DPSDK_INT32 iSessionID,
												   DPSDK_INT32 iMediaSessionID);

/** 
* @brief	Set video enhance parameter, it can call to combine multiple IVSE library several times	
* @param	[IN] iSessionID			User Session ID
* @param	[IN] iMediaSessionID	Media Session ID
* @param	[IN] struIVSEInfo		Video Enhance Parameter
* @param	[IN] bEnable			Enable Switch
* @return	Return 0 if succeeded, return error code if failed.
* @remark	It is only valid to Intelbras stream
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_SetIVSE(DPSDK_INT32 iSessionID,
												  DPSDK_INT32 iMediaSessionID,
												  DPSDK_IVSE_INFO* pIVSEInfo, 
												  DPSDK_BOOL bEnable);

/** 
* @brief	Acquire current stream data length
* @param	[IN] iSessionID			User Session ID
* @param	[IN] iMediaSessionID	Media Session ID
* @param	[OUT] pFrameDataLen	Stream Data Length
* @return	Return 0 if succeeded, return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetFrameDataLen(DPSDK_INT32 iSessionID,
														  DPSDK_INT32 iMediaSessionID,
														  DPSDK_UINT32* pFrameDataLen);

/** 
* @brief	Acquire current frame time
* @param	[IN] iSessionID			User Session ID
* @param	[IN] iMediaSessionID	Media Session ID
* @param	[OUT] pFrameTime		Frame Time
* @return	Return 0 if succeeded, return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetFrameTime(DPSDK_INT32 iSessionID,
													   DPSDK_INT32 iMediaSessionID,
													   DPSDK_TIMET* pFrameTime);

/** 
* @brief	Acquire video size from stream
* @param	[IN] iSessionID			User Session ID
* @param	[IN] iMediaSessionID	Media Session ID
* @param	[OUT] pWidth			Width 
* @param	[OUT] pHeight			Height
* @return	Return 0 if succeeded, return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetVideoFrameSize(DPSDK_INT32 iSessionID,
															DPSDK_INT32 iMediaSessionID,
															DPSDK_INT32* pWidth, 
															DPSDK_INT32* pHeight);

/** 
* @brief	Acquire current frame serial number
* @param	[IN] iSessionID			User Session ID
* @param	[IN] iMediaSessionID	Media Session ID
* @param	[OUT] pFrameNum			Frame Serial Number
* @return	Return 0 if succeeded, return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetFrameNumber(DPSDK_INT32 iSessionID,
														 DPSDK_INT32 iMediaSessionID,
														 DPSDK_INT32* pFrameNum);

/**
* @brief	Acquire remaining data size of source buffer zone
* @param	[IN] iSessionID			User Session ID
* @param	[IN] iMediaSessionID	Media Session ID
* @param	[OUT] pBufferRemain		Remaining data size of buffer zone, unit is BYTE
* @return	Return 0 if succeeded, return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetSourceBufferRemain(DPSDK_INT32 iSessionID,
																DPSDK_INT32 iMediaSessionID,
																DPSDK_INT32* pBufferRemain);

/**
* @brief	Acquire the size of designated buffer zone
* @param	[IN] iSessionID			User Session ID
* @param	[IN] iMediaSessionID	Media Session ID
* @param	[IN] uiVaxBufType		Buffer type, refer to DPSDK_VAX_BUF_TYPE enumeration definition
* @param	[OUT] pBufferSize		Buffer zone size
* @return	Return 0 if succeeded, return error code if failed.	
* @remark	It returns to buffer zone value according to different parameters successdfully, source buffer zone returns to byte, buffer zone returns to frame number after decoding		
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetBufferValue(DPSDK_INT32 iSessionID,
														 DPSDK_INT32 iMediaSessionID,
														 DPSDK_UINT32 uiVaxBufType,
														 DPSDK_INT32* pBufferSize);

//////////////////////////////////////////////////////////////////////////
/*
 *	Intelligent Interface
 */

/**	
* @brief	Set if it is to display intelligent info 
* @param	[IN] iSessionID			User Session ID
* @param	[IN] iMediaSessionID	Media Session ID
* @param	[IN] enumType			Type, refer to DPSDK_IVS_VISIBLE definition
* @param	[IN] bVisible Display sign  true  display  false    not display
* @return	Return 0 if succeeded, return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_SetIvsShowFlag(DPSDK_INT32 iSessionID,
														 DPSDK_INT32 iMediaSessionID,
														 DPSDK_UINT32 uiType, 
														 DPSDK_BOOL bVisible);
/**
* @brief	Acquire target category ID according to location
* @param	[IN] iSessionID			User Session ID
* @param	[IN] iMediaSessionID	Media Session ID
* @param	[IN] iPortX				Spot X Value
* @param	[IN] iPortY				Spot Y Value
* @param	[OUT] pClassID			Target Category id
* @return	Return 0 if succeeded, return error code if failed.
* @remark	Applied to smart tracking
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetIvsClassId(DPSDK_INT32 iSessionID,
	DPSDK_INT32 iMediaSessionID,
	DPSDK_INT32 iPortX,
	DPSDK_INT32 iPortY,
	DPSDK_INT32* pClassID);
/**	
* @brief	Acquire ID of designated target according to location 
* @param	[IN] iSessionID			User Session ID
* @param	[IN] iMediaSessionID	Media Session ID
* @param	[IN] iPortX				Spot X Value
* @param	[IN] iPortY				Spot Y Value
* @param	[OUT] pObjectID			Designated Target id
* @return	Return 0 if succeeded, return error code if failed.
* @remark	Applied to smart tracking 
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetIvsObjectId(DPSDK_INT32 iSessionID,
														 DPSDK_INT32 iMediaSessionID,
														 DPSDK_INT32 iPortX, 
														 DPSDK_INT32 iPortY,
														 DPSDK_INT32* pObjectID);
/**	
* @brief	Set designated target id
* @param	[IN] iSessionID			User Session ID
* @param	[IN] iMediaSessionID	Media Session ID
* @param	[IN] iClassID			Category id
* @param	[IN] pObjectID			Designated target id
* @param	[IN] bAttach		    If it is to draw the shape and color of tracking object
* @return	Return 0 if succeeded, return error code if failed.
* @remark	Applied to smart tracking
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_SetIvsObjectId(DPSDK_INT32 iSessionID,
														 DPSDK_INT32 iMediaSessionID,
														 DPSDK_INT32 iClassID,
														 DPSDK_INT32 iObjectID,
														 DPSDK_BOOL bAttach);
/**	
* @brief	Acquire enter and leave number
* @param	[IN] iSessionID			User Session ID
* @param	[IN] iMediaSessionID	Media Session ID
* @param	[OUT] pInValue			Enter Number
* @param	[OUT] pOutValue			Leave Number
* @return	Return 0 if succeeded, return error code if failed.
* @remark	Applied to people counting 
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetIvsPCInOutValue(DPSDK_INT32 iSessionID,
															 DPSDK_INT32 iMediaSessionID,
															 DPSDK_INT32* pInValue, 
															 DPSDK_INT32* pOutValue);
//////////////////////////////////////////////////////////////////////////
/*
 *	Split Interface
 */

/** 
* @brief	Splicing algorithm, used for display of 4K screen
* @param	[IN] iSessionID			User Session ID
* @param	[IN] iMediaSessionID	Media Session ID
* @param	[IN] iMode				Mode, 0 is basic mode, 1 is 1+3 mode, 2 is 1+5 mode etc. refer to DPSDK_SPLIT_TRECE_TYPE
* @return	Return 0 if succeeded, return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_SplitProc(DPSDK_INT32 iSessionID,
													DPSDK_INT32 iMediaSessionID,
													DPSDK_UINT32 iMode);

/** 
* @brief	Splicing algorithm,it needs amplified rectangle location for update
* @param	[IN] iSessionID			User Session ID
* @param	[IN] iMediaSessionID	Media Session ID
* @param	[IN] pAreaRect			Initial address of rectangle coordinate array
									If nMode is 0, then it should be NULL;
									If nMode is 1 or 4, then the size of the array is 3;
									If nMode is 2, then the size of the array is 5;
									If nMode is 5, then the size of array is 6 
* @return	Return 0 if succeeded, return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_SplitProcUpdate(DPSDK_INT32 iSessionID,
														  DPSDK_INT32 iMediaSessionID,
														  DPSDK_DISPLAY_RECT* pAreaRect);

//////////////////////////////////////////////////////////////////////////
/*
 *	IOS ANDROID Application
 */
#if (defined IOS) || (defined ANDROID)
/** 
* @brief	
* @param	[IN] iSessionID			User Session ID
* @param	[IN] iMediaSessionID	Media Session ID
* @param	[IN] iWidth				Width
* @param	[IN] iHeight			Height
* @return	Return 0 if succeeded, return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_OnSurfaceViewChanged(DPSDK_INT32 iSessionID,
															   DPSDK_INT32 iMediaSessionID,
															   DPSDK_INT32 iWidth, 
															   DPSDK_INT32 iHeight);

/** 
* @brief	Set Scaling
* @param	[IN] iSessionID			User Session ID
* @param	[IN] iMediaSessionID	Media Session ID
* @param	[IN] fScale				Scale
* @return	Return 0 if succeeded, return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_SetScale(DPSDK_INT32 iSessionID,
												   DPSDK_INT32 iMediaSessionID,
												   DPSDK_FLOAT fScale);

/** 
* @brief	Acquire Scaling
* @param	[IN] iSessionID			User Session ID
* @param	[IN] iMediaSessionID	Media Session ID
* @param	[OUT] pScale			Scale
* @return	Return 0 if succeeded, return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetScale(DPSDK_INT32 iSessionID,
												   DPSDK_INT32 iMediaSessionID,
												   DPSDK_FLOAT* pScale);

/** 
* @brief	Refresh 
* @param	[IN] iSessionID			User Session ID
* @param	[IN] iMediaSessionID	Media Session ID
* @return	Return 0 if succeeded, return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_RefreshPlay(DPSDK_INT32 iSessionID,
													  DPSDK_INT32 iMediaSessionID);

/** 
* @brief	Acquire
* @param	[IN] iSessionID			User Session ID
* @param	[IN] iMediaSessionID	Media Session ID
* @param	[OUT] pTranslateX		
* @return	Return 0 if succeeded, return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetTranslateX(DPSDK_INT32 iSessionID,
														DPSDK_INT32 iMediaSessionID,
														DPSDK_FLOAT* pTranslateX);

/** 
* @brief	
* @param	[IN] iSessionID			User Session ID
* @param	[IN] iMediaSessionID	Media Session ID
* @param	[OUT] pTranslateY		
* @return	Return 0 if succeeded, return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetTranslateY(DPSDK_INT32 iSessionID,
														DPSDK_INT32 iMediaSessionID,
														DPSDK_FLOAT* pTranslateY);

/** 
* @brief	
* @param	[IN] iSessionID			User Session ID
* @param	[IN] iMediaSessionID	Media Session ID
* @param	[IN] fTranslateX				
* @param	[IN] fTranslateY			
* @return	Return 0 if succeeded, return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_Translate(DPSDK_INT32 iSessionID,
													DPSDK_INT32 iMediaSessionID,
													DPSDK_FLOAT fTranslateX, 
													DPSDK_FLOAT fTranslateY);

/** 
* @brief	
* @param	[IN] iSessionID			User Session ID
* @param	[IN] iMediaSessionID	Media Session ID
* @return	Return 0 if succeeded, return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_SetIdentity(DPSDK_INT32 iSessionID,
													  DPSDK_INT32 iMediaSessionID);
#endif

//////////////////////////////////////////////////////////////////////////
/*
 *	ANPR Interface
 */

/** 
* @brief	Start ANPR Image Monitoring
* @param	[IN] iSessionID				User Session ID
* @param	[IN] pBayPicParam			ANPR Image Parameter
* @param	[OUT] pMonitorSessionID		Monitoring Session ID
* @return	Return 0 if succeeded, return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_StartBayonetPicture(DPSDK_INT32 iSessionID,
															  DPSDK_PICTURE_MONITOR* pBayPicParam,
															  DPSDK_INT32* pMonitorSessionID);

/** 
* @brief	Stop ANPR Image Monitoring
* @param	[IN] iSessionID				User Session ID
* @param	[IN] iMonitorSessionID		Monitoring Session ID
* @return	Return 0 if succeeded, return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_StopBayonetPicture(DPSDK_INT32 iSessionID,
															 DPSDK_INT32 iMonitorSessionID);

/**
* @brief	Acquire ANPR Image Monitoring Dictionary Data
* @param	[IN] iSessionID				User Session ID
* @param	[IN] pLanguage				Language, e.g.£∫zh_CN
* @param	[IN] iDictionaryType		Dictionary type,refer to DPSDK_BAYONET_DICTIONARY_TYPE definition
* @param	[OUT] pDicListXml			Dictionary data list xml
* @return	Return 0 if succeeded, return error code if failed.
* @note		pDicListXml needs to be released by DPSDK_ReleaseDataBuffer interface
*
*			pDicListXml format shown as follows
*			<List>
*				<Item>
*					<id>id</id>							--Item ID
*					<itemCode>name</itemCode>			--Item Coding
*					<itemName>name</itemName>			--Item Name
*					<itemPar1>name</itemPar1>			--Parameter 1
*					<itemPar2>name</itemPar2>			--Parameter 2
*					<itemType>name</itemType>			--Type coding, ID of dictionary type
*					<itemTypeName>name</itemTypeName>	--Type name, name of dictionary type
*					<memo>name</memo>					--Item Description
*					<sort>name</sort>					--Sorting Code
*					<stat>name</stat>					--Status
*				</Item>
*			</List>
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetBayonetDictionary(DPSDK_INT32 iSessionID,
															   DPSDK_CHAR* pLanguage,
															   DPSDK_INT32 iDictionaryType,
															   DPSDK_CHAR** pDicListXml);

/**
* @brief	Acquire Section List Data
* @param	[IN] iSessionID						User Session ID
* @param	[IN] fDataCallBack					Data synchronization callback function, refer to DPSDK_DATA_ITC_SECTION_LIST for data type
* @param	[IN] pUserData						User Data
* @return	Return 0 if succeeded, return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetAllItcSections(DPSDK_INT32 iSessionID,
															DPSDK_DataCallback fDataCallBack,
															DPSDK_VOID* pUserData);

/**
* @brief	Add car control
* @param	[IN] iSessionID						User Session ID
* @param	[IN] pParam							Car control parameter
* @return	Return 0 if succeeded, return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_AddCarControl(DPSDK_INT32 iSessionID,
														DPSDK_CAR_CTRL_PARAM* pParam);

//////////////////////////////////////////////////////////////////////////
/*
 *	Alarm Interface 
 */

/**
* @brief	Alarm Confirmation
* @param	[IN] iSessionID				User Session ID
* @param	[IN] pConfirmAlarmParam		Alarm Confirmation Parameter
* @return	Return 0 if succeeded, return error code if failed. 
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_ConfirmAlarm(DPSDK_INT32 iSessionID, 
									  DPSDK_CONFIRMALARM_PARAM* pConfirmAlarmParam);

/**
* @brief	Alarm Query
* @param	[IN] iSessionID				User Session ID
* @param	[IN] pQueryAlarmParam		Alarm Query Parameter
* @param	[IN] uiBufLen				Alarm List Cache Size
* @param	[OUT] pAlarmDetailInfoList  Alarm List
* @return	Return 0 if succeeded, return error code if failed.
* @note		uiBufLen = sizeof(DPSDK_ALARM_DETAILINFO_LIST) + (num-1)*sizeof(DPSDK_ALARM_DETAILINFO)
*			num is the number of alarm which needs to be queried£¨value >= 1
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_QueryAlarm(DPSDK_INT32 iSessionID,
									DPSDK_QUERYALARM_PARAM* pQueryAlarmParam, 
									DPSDK_UINT32 uiBufLen, 
									DPSDK_ALARM_DETAILINFO_LIST* pAlarmDetailInfoList);

/**
* @brief	Alarm Amount Query
* @param	[IN] iSessionID				User Session ID
* @param	[IN] pQueryAlarmCountParam	        Alarm Amount Query Parameter
* @param	[OUT] pAlarmCount			Alarm Amount
* @return	Return 0 if succeeded, return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_QueryAlarmCount(DPSDK_INT32 iSessionID,
									DPSDK_QUERYALARMCOUNT_PARAM* pQueryAlarmCountParam,
									DPSDK_UINT32* pAlarmCount);

/**	
* @brief	Alarm Treatment Flow Query 
* @param	[IN] iSessionID				User Session ID
* @param	[IN] pAlarmCode				Alarm SN
* @param	[IN] uiBufLen				Alarm Treatment Flow List Cache Size
* @param	[OUT] pAlarmProcessInfoList	Alarm Treatment Flow List
* @return	Return 0 if succeeded, return error code if failed.
* @note		uiBufLen = sizeof(DPSDK_ALARMPROCESS_DETAILINFO_LIST) + (num-1)*sizeof(DPSDK_ALARMPROCESS_DETAILINFO)
*			num is the number of alarm treatment flow which needs to be queried, value >= 1
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_QueryAlarmProcessFlow(DPSDK_INT32 iSessionID,
											   DPSDK_CHAR* pAlarmCode, 
											   DPSDK_UINT32 uiBufLen, 
											   DPSDK_ALARMPROCESS_DETAILINFO_LIST* pAlarmProcessInfoList);

/**	
* @brief	Shield Alarm
* @param	[IN] iSessionID				User Session ID
* @param	[IN] pBlockAlarmParam		Shield Alarm Parameter
* @return	Return 0 if succeeded, return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_BlockAlarm(DPSDK_INT32 iSessionID,
									DPSDK_BLOCKALARM_PARAM* pBlockAlarmParam);

/**	
* @brief	Get Alarm Car Black list Info
* @param	[IN] iSessionID				User Session ID
* @param	[IN] pBlockAlarmParam		Get Alarm Car Black list Info Parameter
* @param	[OUT] pBlockAlarmResult		Get Alarm Car Black list Info Result
* @return	Return 0 if succeeded, return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetAlarmCarBlacklistInfo(DPSDK_INT32 iSessionID,
																   DPSDK_CAR_ALARM_PARAM* pBlockAlarmParam,
																   DPSDK_CAR_BLOCKALARM_RESULT* pBlockAlarmResult);

/**	
* @brief	Get Alarm Car Peccancy Info
* @param	[IN] iSessionID				User Session ID
* @param	[IN] pCarAlarmParam		Get Alarm Car Peccancy Info Parameter
* @param	[OUT] pCarAlarmResult		Get Alarm Car Peccancy Info Result
* @return	Return 0 if succeeded, return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetAlarmCarPeccancyInfo(DPSDK_INT32 iSessionID,
																   DPSDK_CAR_ALARM_PARAM* pCarAlarmParam,
																   DPSDK_CAR_PECCANCYALARM_RESULT* pCarAlarmResult);

/**	
* @brief	Get Alarm FaceRecognition Info
* @param	[IN] iSessionID				User Session ID
* @param	[IN] pFaceAlarmParam		Get Alarm FaceRecognition Info Parameter
* @param	[OUT] pFaceAlarmResult		Get Alarm FaceRecognition Info Result
* @return	Return 0 if succeeded, return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetAlarmFaceRecognitionInfo(DPSDK_INT32 iSessionID,
																  DPSDK_FACE_ALARM_PARAM* pFaceAlarmParam,
																  DPSDK_FACE_ALARM_RESULT* pFaceAlarmResult);

/**	
* @brief	Acquire Alarm Type Grouping Info
* @param	[IN] iSessionID					User Session ID
* @param	[IN] pLanguage					Language
* @param	[OUT] pInfoXml					Alarm Type Grouping Info Xml Flow
* @return	Return 0 if succeeded, return error code if failed.
* @note		pInfoXml needs to be released by DPSDK_ReleaseDataBuffer interface

*			pInfoXml format shown as follows
*			<AlarmTypeGroupInfos>
*				<AlarmTypeGroupInfo>
*					<GroupId>id</GroupId>
*					<GroupName>name</GroupName>
*					<AlarmTypeInfo>
*						<AlarmType>type</AlarmType>
*						<AlarmTypeName>name</AlarmTypeName>
*						<Other>other</Other>
*					</AlarmTypeInfo>
*				</AlarmTypeGroupInfo>
*			</AlarmTypeGroupInfos>
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetAlarmTypeGroupInfo(DPSDK_INT32 iSessionID,
																DPSDK_CHAR* pLanguage, 
																DPSDK_CHAR** pInfoXml);

/**
* @brief	Alarm Export
* @param	[IN] iSessionID				User Session ID
* @param	[IN] pAlarmExportParam		Alarm Export Parameter
* @param	[IN] iSessionId				Alarm Export Session ID
* @return	Return 0 if succeeded, return error code if failed.
* @note		Alarm export session ID is managed by caller, after the function is successfully executed, The server asynchronously returns an alarm export data of the same session ID.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_ExportAlarms(DPSDK_INT32 iSessionID,
									  DPSDK_ALARMEXPORT_PARAM* pAlarmExportParam,
									  DPSDK_INT32 iSessionId);

//////////////////////////////////////////////////////////////////////////
/*
*	Alarm Process Interface
*/

/**
* @brief	Ask for handle alarm
* @param	[IN] iSessionID				User Session ID
* @param	[IN] pAskForHandleParam		Handle Alarm Parameter
* @return	Return 0 if succeeded, return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_AskForHandle(DPSDK_INT32 iSessionID,
													   DPSDK_ASK_FOR_HANDLE_PARAM* pAskForHandleParam);

/**
* @brief	Ask for duty
* @param	[IN] iSessionID				User Session ID
* @return	Return 0 if succeeded, return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_Duty(DPSDK_INT32 iSessionID);

/**
* @brief	Stop duty
* @param	[IN] iSessionID				User Session ID
* @return	Return 0 if succeeded, return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_StopDuty(DPSDK_INT32 iSessionID);

/**
* @brief	Get pend alarm list
* @param	[IN] iSessionID				User Session ID
* @param	[IN] fDataCallBack			Data synchronization callback function, refer to DPSDK_DATA_PEND_ALARM_LIST for data type
* @param	[IN] pUserData				User Data
* @return	Return 0 if succeeded, return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetPendAlarmList(DPSDK_INT32 iSessionID,
														   DPSDK_DataCallback fDataCallBack,
														   DPSDK_VOID* pUserData);

/**
* @brief	Get processed alarm list
* @param	[IN] iSessionID				User Session ID
* @param	[IN] fDataCallBack			Data synchronization callback function, refer to DPSDK_DATA_PROCESSED_ALARM_LIST for data type
* @param	[IN] pUserData				User Data
* @return	Return 0 if succeeded, return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetProcessedAlarmList(DPSDK_INT32 iSessionID,
																DPSDK_DataCallback fDataCallBack,
																DPSDK_VOID* pUserData);

/**
* @brief	Get alarm template list
* @param	[IN] iSessionID				User Session ID
* @param	[IN] fDataCallBack			Data synchronization callback function, refer to DPSDK_DATA_ALARM_TEMPLATE_LIST for data type
* @param	[IN] pUserData				User Data
* @return	Return 0 if succeeded, return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetAlarmTemplateList(DPSDK_INT32 iSessionID,
															   DPSDK_DataCallback fDataCallBack,
															   DPSDK_VOID* pUserData); 

/**
* @brief	Get alarm template
* @param	[IN] iSessionID				User Session ID
* @param	[IN] pGetParam				Query Parameter
* @param	[IN] fDataCallBack			Data synchronization callback function, refer to DPSDK_DATA_ALARM_TEMPLATE_INFO for data type
* @param	[IN] pUserData				User Data
* @return	Return 0 if succeeded, return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetAlarmTemplate(DPSDK_INT32 iSessionID,
														   DPSDK_GET_TEMPLATE_PARAM* pGetParam,
														   DPSDK_DataCallback fDataCallBack,
														   DPSDK_VOID* pUserData);

/**
* @brief	Get Channel FaceAlarm Types
* @param	[IN] iSessionID				User Session ID
* @param	[IN] pChannelID				ChannelID
* @param	[IN] fDataCallBack			Data synchronization callback function, refer to DPSDK_CHANNEL_FACEALARM_TYPE_LIST for data type
* @param	[IN] pUserData				User Data
* @return	Return 0 if succeeded, return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetChannelFaceAlarmTypes(DPSDK_INT32 iSessionID,
																   DPSDK_CHAR* pChannelID,
																   DPSDK_DataCallback fDataCallBack,
														           DPSDK_VOID* pUserData);


/**
* @brief	Alarm on file
* @param	[IN] iSessionID				User Session ID
* @param	[IN] pOnFileParam			On File Parameter
* @return	Return 0 if succeeded, return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_AlarmOnFile(DPSDK_INT32 iSessionID,
													  DPSDK_ALARM_ONFILE_PARAM* pOnFileParam);

/**
* @brief	Get handle alarm list
* @param	[IN] iSessionID				User Session ID
* @param	[IN] pGetParam				Get handle alarm list parameter
* @param	[IN] pPageInfo				Page Info
* @param	[OUT] uiTotal				Total number of records
* @param	[OUT] pHandleAlarmList		Handle alarm list
* @param	[IN] uiBufLen				pHandleAlarmList Memory Size
* @return	Return 0 if succeeded, return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetHandleAlarmList(DPSDK_INT32 iSessionID,
															 DPSDK_GET_HANDLE_ALARM_PARAM* pGetParam,
															 DPSDK_PAGE_INFO* pPageInfo,
															 DPSDK_UINT32* pTotal, 
															 DPSDK_HANDLE_ALARM_LIST* pHandleAlarmList,
															 DPSDK_UINT32 uiBufLen);

/**
* @brief	Get handle alarm list
* @param	[IN] iSessionID				User Session ID
* @param	[IN] pExportParam			Export handle alarm parameter
* @return	Return 0 if succeeded, return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_ExportHandleAlarm(DPSDK_INT32 iSessionID,
															DPSDK_EXPORT_HANDLE_ALARM_PARAM* pExportParam);

/**
* @brief	Get resource binding info
* @param	[IN] iSessionID				User Session ID
* @param	[IN] pGetParam				Query Parameter
* @param	[IN] fDataCallBack			Data synchronization callback function, refer to DPSDK_DATA_RESOURCE_BINDING_INFO for data type
* @param	[IN] pUserData				User Data
* @return	Return 0 if succeeded, return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetResourceBinding(DPSDK_INT32 iSessionID,
															 DPSDK_GET_RESOURCE_BINDING_PARAM* pGetParam,
															 DPSDK_DataCallback fDataCallBack,
															 DPSDK_VOID* pUserData);


/**
* @brief	Acquire all organization trees (exclude device, channel)
* @param	[IN] iSessionID				User Session ID
* @param	[IN] iOrgType				Organization type, default is 1, basic organization
* @param	[IN] fDataCallBack			Data synchronization callback function, refer to DPSDK_DATA_ALL_ORG_INFO for data type
* @param	[IN] pUserData				User Data
* @return	Return 0 if succeeded, return error code if failed.
**/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetAllOrg(DPSDK_INT32 iSessionID,
													DPSDK_INT32 iOrgType,
													DPSDK_DataCallback fDataCallBack,
													DPSDK_VOID* pUserData);

/**
* @brief	Acquire Organization Data according to condition
* @param	[IN] iSessionID				Organization Session ID
* @param	[IN] pQueryOrgInfo			Organization Query Condition, refer to DPSDK_QUERY_ORG_INFO definition
* @param	[IN] uiQueryLen				Organization Query Condition Length
* @param	[IN] fDataCallBack			Data Synchronization Callback Function, refer to DPSDK_DATA_ORG_INFO for data type.
* @param	[IN] pUserData				User Data
* @return	Return 0 if succeeded, return error code if failed.
**/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetOrganization(DPSDK_INT32 iSessionID,
														  DPSDK_QUERY_ORG_INFO* pQueryOrgInfo,
														  DPSDK_UINT32 uiQueryLen,
														  DPSDK_DataCallback fDataCallBack,
														  DPSDK_VOID* pUserData);

/**
* @brief	Acquire face channel data according to condition
* @param	[IN] iSessionID				User Session ID
* @param	[IN] pQueryParam			Organization Query Condition, refer to DPSDK_QUERY_FACE_CHANNEL_PARAM definition 
* @param	[IN] pPageInfo				Page Info
* @param	[OUT] uiTotal				Total number of records
* @param	[OUT] pFaceChannelList		Face Channel List
* @param	[IN] uiBufLen				pFaceChannelList Memory Size
* @return	Return 0 if succeeded, return error code if failed.
**/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetFaceChannels(DPSDK_INT32 iSessionID,
														  DPSDK_QUERY_FACE_CHANNEL_PARAM* pQueryParam,
														  DPSDK_PAGE_INFO* pPageInfo,
														  DPSDK_UINT32* pTotal,
														  DPSDK_FACE_CHANNEL_LIST* pFaceChannelList,
														  DPSDK_UINT32 uiBufLen);

/**
* @brief	Config face event rule
* @param	[IN] iSessionID				User Session ID
* @param	[IN] pConfigParam			Config Face Enent Parameter 
* @return	Return 0 if succeeded, return error code if failed.
**/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_ConfigFaceEvent(DPSDK_INT32 iSessionID,
														  DPSDK_CONFIG_FACE_EVENT_PARAM* pConfigParam);

/**
* @brief	Acquire device tree via hierarchy
* @param	[IN] iSessionID				User Session ID
* @param	[IN] pParam					Hierarchy Query Condition
* @param	[IN] pPageInfo				Page Info
* @param	[OUT] uiTotal				Total number of records
* @param	[IN] fDataCallBack			Data synchronization callback function, refer to DPSDK_DATA_DEVICE_LAYERED for data type
* @param	[IN] pUserData				User Data
* @return	Return 0 if succeeded, return error code if failed.
**/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetDeviceByLayered(DPSDK_INT32 iSessionID,
															 DPSDK_GET_DEVICE_LAYERED_PARAM* pParam,
															 DPSDK_PAGE_INFO* pPageInfo,
															 DPSDK_UINT32* pTotal,
															 DPSDK_DataCallback fDataCallBack,
															 DPSDK_VOID* pUserData);

/**
* @brief	Acquire organization data at the same level (Discarded)
* @param	[IN] iSessionID				User Session ID
* @param	[IN] pOrgID					Organization ID, it means root organization when it is NULL
* @param	[OUT] pOrgInfo				Organization Info
* @return	Return 0 if succeeded, return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetOrgInfo(DPSDK_INT32 iSessionID,
													 DPSDK_CHAR* pOrgID,
													 DPSDK_ORG_BASE_INFO* pOrgInfo); 

/**
* @brief	Create Organization (Discard) 
* @param	[IN] iSessionID				User Session ID
* @param	[IN] pOrgInfo				Organization Info
* @param	[OUT] pOrgID				organization ID
* @return	Return 0 if succeeded, return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_CreateOrg(DPSDK_INT32 iSessionID,
													DPSDK_ORG_CREATE_PARAM* pOrgInfo,
													DPSDK_CHAR* pOrgID);

/**
* @brief	Acquire list data of subordinate organization (Discard)
* @param	[IN] iSessionID				User Session ID
* @param	[IN] pParentID				Father Node ID
* @param	[OUT] pOrgList				Organization List
* @param	[IN] uiBufLen				Cache Size 
* @return	Return 0 if succeeded, return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetOrgByLayered(DPSDK_INT32 iSessionID,
														  DPSDK_CHAR* pParentID,
														  DPSDK_ORG_LIST* pOrgList,
														  DPSDK_UINT32 uiBufLen);

/**
* @brief	Add Collection Channel
* @param	[IN] iSessionID			    User Session ID
* @param	[IN] pCollectList			Channel List
* @param	[IN] uiBufLen				Cache Size
* @return	Return 0 if succeeded, return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_AddCollectionChannel(DPSDK_INT32 iSessionID,
															   DPSDK_COLLECTION_LIST* pCollectList,
															   DPSDK_UINT32 uiBufLen);

/**
* @brief	Delete Collection Channel
* @param	[IN] iSessionID				User Session ID
* @param	[IN] pChannelID				Channel ID
* @param	[IN] pOrgCode				Organization ID
* @return	Return 0 if succeeded, return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_DelCollectionChannel(DPSDK_INT32 iSessionID,
															   DPSDK_CHAR* pChannelID,
															   DPSDK_CHAR* pOrgCode);

/**
* @brief	Query Collection Channel List
* @param	[IN] iSessionID				User Session ID
* @param	[IN] pParam					Request Parameter
* @param	[IN] pPageInfo				Page Info
* @param	[OUT] uiTotal				Total Record Number 
* @param	[OUT] pChannelList			Collect Channel List
* @param	[IN] uiBufLen				Memory Size 
* @return	Return 0 if succeeded, return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetCollectionChannelList(DPSDK_INT32 iSessionID,
																   DPSDK_COLLECTION_DETAIL_PARAM* pParam,
																   DPSDK_PAGE_INFO* pPageInfo,
																   DPSDK_UINT32* pTotal,
																   DPSDK_COLLECTION_DETAIL_LIST* pChannelList,
																   DPSDK_UINT32 uiBufLen);

/**
* @brief	Acquire all collection organization data
* @param	[IN] iSessionID				User Session ID
* @param	[IN] fDataCallBack			Data sync callback function, refer to DPSDK_DATA_COLLECT_ORG_INFO for data type
* @param	[IN] pUserData				User Data
* @return	Return 0 if succeeded, return error code if failed.
**/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetAllCollectionOrg(DPSDK_INT32 iSessionID,
															  DPSDK_DataCallback fDataCallBack,
															  DPSDK_VOID* pUserData);

/**
* @brief	Save all collection organization data
* @param	[IN] iSessionID				User Session ID
* @param	[IN] pCollectOrgInfo		Collect Organization Info
* @return	Return 0 if succeeded, return error code if failed.
**/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_SaveAllCollectionOrg(DPSDK_INT32 iSessionID,
															   DPSDK_COLLECTION_ORG_INFO* pCollectOrgInfo);

/**
* @brief	Acquire user organization data according to condition
* @param	[IN] iSessionID				User Session ID
* @param	[IN] pQueryUserOrgInfo		User organization query condition, refer to DPSDK_QUERY_USER_ORG_INFO definition
* @param	[OUT] pUserOrgXml			Query result xml flow
* @return	Return 0 if succeeded, return error code if failed.
* @note		pUserOrgXml needs to be released by DPSDK_ReleaseDataBuffer interface
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetUserOrganization(DPSDK_INT32 iSessionID,
															  DPSDK_QUERY_USER_ORG_INFO* pQueryUserOrgInfo,
															  DPSDK_CHAR** pUserOrgXml);

/**
* @brief	Release Data Cache
* @param	[IN] pBuffer				Cache that needs to be released
* @return	Return 0 if succeeded, return error code if failed.
* @note		pBuffer is the cache space which is internally applied by sdk
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_ReleaseDataBuffer(DPSDK_CHAR* pBuffer);

/**
* @brief	Save User Private Data
* @param	[IN] iSessionID				User Session ID
* @param	[IN] pUserDataName			User Data Name
* @param	[IN] pUserData				User Data
* @param	[IN] uiDataLen				User Data Length
* @return	Return 0 if succeeded, return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_SaveUserData(DPSDK_INT32 iSessionID,
													   DPSDK_CHAR* pUserDataName,
													   DPSDK_CHAR* pUserData,
													   DPSDK_UINT32 uiDataLen);

/**
* @brief	Acquire User Private Data
* @param	[IN] iSessionID				User Session ID
* @param	[IN] pUserDataName			User Data Name
* @param	[OUT] pUserData				User Data
* @return	Return 0 if succeeded, return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetUserData(DPSDK_INT32 iSessionID,
													  DPSDK_CHAR* pUserDataName,
													  DPSDK_CHAR** pUserData);

/**
* @brief	Delete User Private Data
* @param	[IN] iSessionID				User Session ID
* @param	[IN] pUserDataName			User Data Name
* @return	Return 0 if succeeded, return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_DelUserData(DPSDK_INT32 iSessionID,
													  DPSDK_CHAR* pUserDataName);

/**
* @brief	Acquire Device Data
* @param	[IN] iSessionID				User Session ID
* @param	[IN] pQueryDevInfo			Query Condition,
* @param	[IN] uiQueryLen				Query Condition Length,
* @param	[IN] fDataCallBack			Data sync callback function, refer to DPSDK_DATA_DEVICE_INFO for data type
* @param	[OUT] pUserData				User Data
* @return	Return 0 if succeeded, return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetDevice(DPSDK_INT32 iSessionID,
													DPSDK_QUERY_DEV_INFO* pQueryDevInfo,
													DPSDK_UINT32 uiQueryLen,
													DPSDK_DataCallback fDataCallBack,
													DPSDK_VOID* pUserData);

/**
* @brief	Acquire Device Type
* @param	[IN] iSessionID				User Session ID
* @param	[IN] pLocale			 	International Voice 
* @param	[IN] fDataCallBack			Data sync callback function, refer to DPSDK_DATA_GET_DEVICE_TYPE for data type
* @param	[OUT] pUserData				User Data
* @return	Return 0 if succeeded, return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetDeviceType(DPSDK_INT32 iSessionID,
														DPSDK_CHAR* pLocale,
														DPSDK_DataCallback fDataCallBack,
														DPSDK_VOID* pUserData);

/**
* @brief	Acquire Map Organization
* @param	[IN] iSessionID				User Session ID
* @param	[OUT] pMapOrgXml			Query Result xml Flow
* @return	Return 0 if succeeded, return error code if failed.
* @note		pMapOrgXml needs to be released by DPSDK_ReleaseDataBuffer interface
*
*			pMapOrgXml format shown as follows
*			<MapOrganizations>
*				<MapOrganization>
*					<Path>path</Path>
*					<Id>id</Id>
*					<Name>name</Name>
*					<ParentId>parentid</ParentId>
					<Enable>enable</Enable>
*				</MapOrganization>
*			</MapOrganizations>
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetMapOrganization(DPSDK_INT32 iSessionID,
															 DPSDK_CHAR** pMapOrgXml);


/**
* @brief	Get media key
* @param	[IN] iSessionID				User Session ID
* @param	[IN] pDeviceID			 	Device code
* @param	[OUT] pKeyInfo				Key info
* @return	Return 0 if succeeded, return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetMediaKey(DPSDK_INT32 iSessionID,
													  DPSDK_CHAR* pDeviceCode,
													  DPSDK_MEDIA_KEY_INFO* pKeyInfo);

/**
* @brief	Get media key
* @param	[IN] iSessionID				User Session ID
* @param	[IN] pParam			 		Query parameter
* @param	[IN] fDataCallBack			Data sync callback function, refer to DPSDK_DATA_GET_HIS_MEDIA_KEY for data type
* @param	[OUT] pUserData				User Data
* @return	Return 0 if succeeded, return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_QueryHisMediaKey(DPSDK_INT32 iSessionID,
														   DPSDK_QUERY_MEDIA_KEY_PARAM* pParam,
														   DPSDK_DataCallback fDataCallBack,
														   DPSDK_VOID* pUserData);

//////////////////////////////////////////////////////////////////////////
/*
*	User Interface
*/

/**
* @brief	Acquire User Authority List
* @param	[IN] iSessionID			User Session ID
* @param	[OUT] pUserRightList	User Authority List
* @return	Return 0 if succeeded, return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetUserRight(DPSDK_INT32 iSessionID,
													   DPSDK_USER_RIGHT_LIST* pUserRightList);
/**
* @brief	Acquire User Grouped Data
* @param	[IN] iSessionID				User Session ID
* @param	[IN] fDataCallBack			Data sync callback function, refer to DPSDK_DATA_USER_GROUP_LIST
* @param	[IN] pUserData				User Data
* @return	Return 0 if succeeded, return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetUserGroup(DPSDK_INT32 iSessionID,
													   DPSDK_DataCallback fDataCallBack,
													   DPSDK_VOID* pUserData);

/**
* @brief	Acquire User Basic Info
* @param	[IN] iSessionID				User Session ID
* @param	[IN] pQueryParam£¨			Query Parameter
* @param	[IN] pPageInfo				Page Info
* @param	[OUT] uiTotal				Total Record Number 
* @param	[IN] fDataCallBack			Data sync callback function, refer to DPSDK_DATA_USER_BASE_INFO_LIST
* @param	[IN] pUserData				User Data
* @return	Return 0 if succeeded, return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetUserBaseInfo(DPSDK_INT32 iSessionID,
													  DPSDK_QUERY_USER_BASE_INFO* pQueryParam,
													  DPSDK_PAGE_INFO* pPageInfo,
													  DPSDK_UINT32* uiTotal,
													  DPSDK_DataCallback fDataCallBack,
													  DPSDK_VOID* pUserData);

/**
* @brief	Acquire User Detail Info
* @param	[IN] iSessionID				User Session ID
* @param	[IN] pQueryParam£¨			Query Parameter
* @param	[IN] fDataCallBack			Data sync callback function, refer to DPSDK_DATA_USER_DETAIL_INFO for data type
* @param	[IN] pUserData				User Data
* @return	Return 0 if succeeded, return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetUserDetailInfo(DPSDK_INT32 iSessionID,
															DPSDK_QUERY_USER_DETAIL_INFO* pQueryParam,
															DPSDK_DataCallback fDataCallBack,
															DPSDK_VOID* pUserData);

/**
* @brief	Modify User
* @param	[IN] iSessionID				User Session ID
* @param	[IN] pSaveUserInfo			User Info
* @return	Return 0 if succeeded, return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_SaveUser(DPSDK_INT32 iSessionID,
												   DPSDK_EDIT_USER_INFO* pSaveUserInfo);

/**
* @brief	Add User
* @param	[IN] iSessionID				User Session ID
* @param	[IN] pAddUserInfo			User Info
* @param	[OUT] pUserId				User ID
* @return	Return 0 if succeeded, return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_AddUser(DPSDK_INT32 iSessionID,
												  DPSDK_EDIT_USER_INFO* pAddUserInfo,
												  DPSDK_CHAR* pUserId);

/**
* @brief	Delete User
* @param	[IN] iSessionID				User Session ID
* @param	[IN] iUserNum				User Number
* @param	[IN] pUserIdList			User ID List
* @return	Return 0 if succeeded, return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_DeleteUser(DPSDK_INT32 iSessionID,
													 DPSDK_INT32 iUserNum,
													 DPSDK_USER_ID* pUserIdList);

/**
* @brief	Acquire Client Name
* @param	[IN] iSessionID				User Session ID
* @param	[OUT] pClientName			Client Name
* @return	Return 0 if succeeded, return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetClientName(DPSDK_INT32 iSessionID,
														DPSDK_CLIENT_NAME* pClientName);

/**
* @brief	Get app users
* @param	[IN] iSessionID				User Session ID
* @param	[IN] pGetParam				Get Parameter
* @param	[IN] fDataCallBack			Data sync callback function, refer to DPSDK_DATA_USER_GET_APP_USERS for data type
* @param	[IN] pUserData				User Data
* @return	Return 0 if succeeded, return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetAppUsers(DPSDK_INT32 iSessionID,
													  DPSDK_GET_APP_USERS_PARAM* pGetParam,
													  DPSDK_DataCallback fDataCallback,
													  DPSDK_VOID* pUserData);

/**
* @brief	Del app users
* @param	[IN] iSessionID				User Session ID
* @param	[IN] pDeleteParam			Delete Parameter
* @return	Return 0 if succeeded, return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_DeleteAppUsers(DPSDK_INT32 iSessionID,
													    DPSDK_DELETE_APP_USER_PARAM* pDeleteParam);

/**
* @brief	Update app users
* @param	[IN] iSessionID				User Session ID
* @param	[IN] pUpdateParam			Update Parameter
* @return	Return 0 if succeeded, return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_UpdateAppUser(DPSDK_INT32 iSessionID,
														 DPSDK_UPDATE_APP_USER_PARAM* pUpdateParam);

/**
* @brief	Get user email info
* @param	[IN] iSessionID				User Session ID
* @param	[IN] fDataCallBack			Data sync callback function, refer to DPSDK_DATA_USER_GET_EMAIL_INFO for data type
* @param	[IN] pUserData				User Data
* @return	Return 0 if succeeded, return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetUserEmailInfo(DPSDK_INT32 iSessionID,
														   DPSDK_DataCallback fDataCallback,
														   DPSDK_VOID* pUserData);

/**
* @brief	Get right user. 
* @param	[IN] iSessionID				User session ID
* @param	[IN] pResourceId		    Resource Id
* @param	[IN] fDataCallBack			Data sync call function, data type see DPSDK_USER_LIST
* @param	[IN] pUserData				User data
* @return	Return 0 if succeeded. Return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetRightUsers(DPSDK_INT32 iSessionID,
														DPSDK_CHAR* pResourceId,
														DPSDK_DataCallback fDataCallBack,
														DPSDK_VOID* pUserData);

/**
* @brief	Get user email info
* @param	[IN] iSessionID				User Session ID
* @param	[IN] pSendParam				Send Email Parameter
* @return	Return 0 if succeeded, return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_SendEmailMessage(DPSDK_INT32 iSessionID,
														   DPSDK_SEND_EMAIL_PARAM* pSendParam);

//////////////////////////////////////////////////////////////////////////
/*
*	Alarm Preplan 
*/

/**
* @brief	Acquire Time Module Basic Info
* @param	[IN] iSessionID				User Session ID
* @param	[IN] pPageInfo				Page Info
* @param	[OUT] iTotal				Total Pages
* @param	[OUT] pTemplateInfoList		Time Module Info List
* @return	Return 0 if succeeded, return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetTimeTemplateBaseInfoList(DPSDK_INT32 iSessionID,
																	  DPSDK_PAGE_INFO* pPageInfo,
																	  DPSDK_INT32* iTotal,
																	  DPSDK_TIME_TEMPLATE_BASE_INFO_LIST* pTemplateInfoList);

/**
* @brief	Acquire Time Template Detail Info
* @param	[IN] iSessionID				User Session ID
* @param	[IN] iTemplateId			Time Template id
* @param	[OUT] pTemplateDetailInfo	Time Template Info
* @return	Return 0 if succeeded, return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetTimeTemplateDetailInfo(DPSDK_INT32 iSessionID,
																	DPSDK_INT32 iTemplateId,
																	DPSDK_TIME_TEMPLATE_DETAIL_INFO* pTemplateDetailInfo);

/**
* @brief	Modify Time Template
* @param	[IN] iSessionID			 	User Session ID
* @param	[IN] pTemplateDetailInfo	Time Template Info
* @return	Return 0 if succeeded, return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_SaveTimeTemplate(DPSDK_INT32 iSessionID,
														   DPSDK_TIME_TEMPLATE_DETAIL_INFO* pTemplateDetailInfo);

/**
* @brief	Add Time Template
* @param	[IN] iSessionID				User Session ID
* @param	[IN] pTemplateDetailInfo	Time Template Info
* @param	[OUT] iTemplateId			Time Template id
* @return	Return 0 if succeeded, return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_AddTimeTemplate(DPSDK_INT32 iSessionID,
														  DPSDK_TIME_TEMPLATE_DETAIL_INFO* pTemplateDetailInfo,
														  DPSDK_INT32* iTemplateId);


/**
* @brief	Delete Time Template
* @param	[IN] iSessionID				User Session ID
* @param	[IN] iTemplateId			Time Template id
* @param	[IN] fDataCallBack			Data sync callback function, refer to DPSDK_DATA_TIME_TEMPLATE_INFO for data type
* @param	[IN] pUserData				User Data
* @return	Return 0 if succeeded, return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_DeleteTimeTemplate(DPSDK_INT32 iSessionID,
															DPSDK_INT32 iTemplateId,
															DPSDK_DataCallback fDataCallback,
															DPSDK_VOID* pUserData);

/**
* @brief	Acquire Alarm Preplan List
* @param	[IN] iSessionID				User Session ID
* @param	[IN] pPageInfo				Page Info
* @param	[OUT] iTotal				Total Pages
* @param	[IN] fDataCallBack			Data sync callback function, refer to DPSDK_DATA_ALARM_PLAN_LIST for data type
* @param	[IN] pUserData				User Data
* @return	Return 0 if succeeded, return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetAlarmPlanList(DPSDK_INT32 iSessionID,
															DPSDK_PAGE_INFO* pPageInfo,
															DPSDK_INT32* iTotal,
															DPSDK_DataCallback fDataCallBack,
															DPSDK_VOID* pUserData);

/**
* @brief	Save Alarm Plan
* @param	[IN] iSessionID				User Session ID
* @param	[IN] pAlarmPlanInfo			Alarm Preplan Info
* @return	Return 0 if succeeded, return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_SaveAlarmPlan(DPSDK_INT32 iSessionID,
														DPSDK_ALARM_PLAN_DETAIL_INFO* pAlarmPlanInfo);

/**
* @brief	Add New Alarm Plan
* @param	[IN] iSessionID				User Session ID
* @param	[IN] pAlarmPlanInfoList		Alarm Preplan Info
* @return	Return 0 if succeeded, return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_AddAlarmPlan(DPSDK_INT32 iSessionID,
													   DPSDK_ALARM_PLAN_DETAIL_INFO* pAlarmPlanInfo);



/**
* @brief	Acquire Alarm Plan
* @param	[IN] iSessionID				User Session ID
* @param	[IN] pChannelId				Channel ID
* @param	[IN] fDataCallBack			Data sync callback function, refer to DPSDK_DATA_PLAN_DETAIL_INFO for data type
* @param	[IN] pUserData				User Data
* @return	Return 0 if succeeded, return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetAlarmPlan(DPSDK_INT32 iSessionID,
															DPSDK_CHAR* pChannelId,
															DPSDK_DataCallback fDataCallBack,
															DPSDK_VOID* pUserData);

/**
* @brief	Delete Alarm Plan
* @param	[IN] iSessionID				User Session ID
* @param	[IN] pChannelId				Channel ID
* @return	Return 0 if succeeded, return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_DeleteAlarmPlan(DPSDK_INT32 iSessionID,
														  DPSDK_CHAR* pChannelId);

/**
* @brief	Copy Alarm Plan
* @param	[IN] iSessionID				User Session ID
* @param	[IN] pCopyParam				Copy Request Parameter
* @return	Return 0 if succeeded, return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_CopyAlarmPlan(DPSDK_INT32 iSessionID,
														DPSDK_ALARM_PLAN_COPY_PARAM* pCopyParam);

/**
* @brief	Acquire User Email List
* @param	[IN] iSessionID				User Session ID
* @param	[IN] pPageInfo				Page Info
* @param	[OUT] iTotal				Total Pages
* @param	[OUT] pUserMailList			User Email List
* @param	[IN] uiBufLen				Cache Size
* @return	Return 0 if succeeded, return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetUserMailList(DPSDK_INT32 iSessionID,
														  DPSDK_PAGE_INFO* pPageInfo,
														  DPSDK_INT32* iTotal,
														  DPSDK_USER_MAIL_LIST* pUserMailList,
														  DPSDK_UINT32 uiBufLen);

/**
* @brief	Acquire automatically-added email list
* @param	[IN] iSessionID				User Session ID
* @param	[IN] pPageInfo				Page Info
* @param	[OUT] iTotal				Total Pages
* @param	[OUT] pMailList				Email List
* @param	[IN] uiBufLen				Cache Size
* @return	Return 0 if succeeded, return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetMailList(DPSDK_INT32 iSessionID,
													  DPSDK_PAGE_INFO* pPageInfo,
													  DPSDK_INT32* iTotal,
													  DPSDK_MAIL_LIST* pMailList,
													  DPSDK_UINT32 uiBufLen);



//////////////////////////////////////////////////////////////////////////
/*
*	System Service and Resource Interface 
*/

/**
* @brief	Acquire manual backup file
* @param	[IN] iSessionID				User Session ID
* @param	[IN] pPWD					Cleartext Password
* @param	[OUT] pResult				Result
* @return	Return 0 if succeeded, return error code if failed.
* @note		pBackupFile needs to be released by DPSDK_ReleaseDataBuffer interface
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetManualBackUpFile(DPSDK_INT32 iSessionID,
															  DPSDK_CHAR* pPWD,
															  DPSDK_MANUAL_BACKUP_RESULT* pResult);

/**
* @brief	Auto Backup
* @param	[IN] iSessionID				User Session ID
* @param	[IN] pParam					Request Parameter
* @return	Return 0 if succeeded, return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_AutoBackUp(DPSDK_INT32 iSessionID,
													 DPSDK_AUTO_BACKUP_PARAM* pParam); 

/**
* @brief	Acquire Auto Recovery Config
* @param	[IN] iSessionID				User Session ID
* @param	[OUT] pResultInfo			Acquire auto return config info
* @return	Return 0 if succeeded, return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetAutoBackUp(DPSDK_INT32 iSessionID,
														DPSDK_AUTO_BACKUP_PARAM* pResultInfo);

/**
* @brief	Manual Restoration
* @param	[IN] iSessionID				User Session ID
* @param	[IN] pPWD					Cleartext Password
* @param	[IN] pBackupFile			Backup File Flow
* @param	[IN] uiFileLength			File Size
* @param	[IN] pUserPWD				User Cleartext Password
* @return	Return 0 if succeeded, return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_ManualRestore(DPSDK_INT32 iSessionID,
														DPSDK_CHAR* pPWD,
														DPSDK_CHAR* pBackupFile,
														DPSDK_UINT32 uiFileLength, 
														DPSDK_CHAR* pUserPWD);

/**
* @brief	Auto Backup File List
* @param	[IN] iSessionID				User Session ID
* @param	[IN] pPageInfo				Page Info
* @param	[OUT] iTotal				Total Pages
* @param	[OUT] pFileList				Request Parameter
* @return	Return 0 if succeeded, return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetAutoBackUpFileList(DPSDK_INT32 iSessionID,
																DPSDK_PAGE_INFO* pPageInfo,
																DPSDK_INT32* iTotal,
																DPSDK_AUTO_BACKUP_FILE_LIST* pFileList); 

/**
* @brief	Auto Backup File Restoration
* @param	[IN] iSessionID				User Session ID
* @param	[IN] pSN					Backup File SN Code
* @param	[IN] pPWD					Cleartext Password
* @param	[IN] pUserPWD				User Cleartext Password
* @return	Return 0 if succeeded, return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_AutoFileRestore(DPSDK_INT32 iSessionID,
														  DPSDK_CHAR* pSN,
														  DPSDK_CHAR* pPWD,
														  DPSDK_CHAR* pUserPWD);

/**
* @brief	Acquire Video Related List
* @param	[IN] iSessionID				User Session ID
* @param	[IN] pPageInfo				Page Info
* @param	[OUT] iTotal				Total Pages
* @param	[OUT] pVideoRelationList	Video Related List
* @return	Return 0 if succeeded, return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetVideoRelationList(DPSDK_INT32 iSessionID,
															   DPSDK_PAGE_INFO* pPageInfo,
															   DPSDK_INT32* iTotal,
															   DPSDK_VIDEO_RELATION_LIST* pVideoRelationList); 

/**
* @brief	Save Video Relevance
* @param	[IN] iSessionID				User Session ID
* @param	[IN] pSaveParam				Request Parameter
* @return	Return 0 if succeeded, return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_SaveVideoRelation(DPSDK_INT32 iSessionID,
															DPSDK_SAVE_VIDEO_RELATION_PARAM* pSaveParam); 

/**
* @brief	Acquire Relevance Relation Detail
* @param	[IN] iSessionID				User Session ID
* @param	[IN] pSourceId				Channel ID
* @param	[OUT] pRelationNodeList		Relevance Channel List
* @return	Return 0 if succeeded, return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetVideoRelationById(DPSDK_INT32 iSessionID,
																DPSDK_CHAR* pSourceId,
																DPSDK_VIDEO_RELATION_NODE_LIST* pRelationNodeList);

/**
* @brief	Delete Relevance Relation
* @param	[IN] iSessionID				User Session ID
* @param	[IN] pSourceId				Channel ID
* @return	Return 0 if succeeded, return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_DeleteVideoRelationById(DPSDK_INT32 iSessionID,
																  DPSDK_CHAR* pSourceId);

/**
* @brief	Acquire parameter config according to id
* @param	[IN] iSessionID				User Session ID
* @param	[IN] iId					Parameter Config id
* @param	[OUT] pConfigInfo			Parameter Config Info
* @return	Return 0 if succeeded, return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetConfigById(DPSDK_INT32 iSessionID,
														DPSDK_INT32 iId,
														DPSDK_SERVER_CONFIG_INFO* pConfigInfo);

/**
* @brief	Get Config Types
* @param	[IN] iSessionID				User Session ID
* @param	[IN] fDataCallBack			Data sync callback function, refer to DPSDK_DATA_GET_CONFIG_TYPE for data type
* @param	[IN] pUserData				User Data
* @return	Return 0 if succeeded, return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetConfigTypes(DPSDK_INT32 iSessionID,
														 DPSDK_DataCallback fDataCallBack,
														 DPSDK_VOID* pUserData);

/**
* @brief	Save Parameter Config
* @param	[IN] iSessionID				User Session ID
* @param	[IN] pConfigInfo			Parameter Config Info
* @return	Return 0 if succeeded, return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_SaveConfig(DPSDK_INT32 iSessionID,
													 DPSDK_SERVER_CONFIG_INFO* pConfigInfo);

/**
* @brief	Save https certificate
* @param	[IN] iSessionID				User Session ID
* @param	[IN] pHttpsConfigInfo		https Certificate Info
* @return	Return 0 if succeeded, return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_SaveConfigHttps(DPSDK_INT32 iSessionID,
														  DPSDK_HTTPS_CONFIG_INFO* pHttpsConfigInfo); 

/**
* @brief	Acquire Parameter Config Info
* @param	[IN] iSessionID				User Session ID
* @param	[OUT] pStorageInfo			Parameter Config Info
* @return	Return 0 if succeeded, return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetStorageConfig(DPSDK_INT32 iSessionID,
														   DPSDK_STORAGE_CONFIG_INFO* pStorageInfo);

/**
* @brief	Save Parameter Config info
* @param	[IN] iSessionID				User Session ID
* @param	[IN] pStorageInfo			Parameter Config Info
* @return	Return 0 if succeeded, return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_SaveStorageConfig(DPSDK_INT32 iSessionID,
															DPSDK_STORAGE_CONFIG_INFO* pStorageInfo);

/**
* @brief	Get Pos End info
* @param	[IN] iSessionID				User Session ID
* @param	[OUT] pPosEndInfo			Pos end Info
* @return	Return 0 if succeeded, return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetPosEnd(DPSDK_INT32 iSessionID,
													DPSDK_POS_END_INFO* pPosEndInfo);

/**
* @brief	Save Pos End info
* @param	[IN] iSessionID				User Session ID
* @param	[IN] pPosEndInfo			Pos end Info
* @return	Return 0 if succeeded, return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_SavePosEnd(DPSDK_INT32 iSessionID,
													 DPSDK_POS_END_INFO* pPosEndInfo);

/**
* @brief	Get Weather info
* @param	[IN] iSessionID				User Session ID
* @param	[OUT] pWeather				Weather Info
* @return	Return 0 if succeeded, return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetWeather(DPSDK_INT32 iSessionID,
													 DPSDK_WEATHER_INFO* pWeather);

/**
* @brief	Save Weather info
* @param	[IN] iSessionID				User Session ID
* @param	[IN] pWeather				Weather Info
* @return	Return 0 if succeeded, return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_SaveWeather(DPSDK_INT32 iSessionID,
													  DPSDK_WEATHER_INFO* pWeather);

/**
* @brief	FTP Config Info
* @param	[IN] iSessionID				User Session ID
* @param	[OUT] pFtpInfo				Ftp Config Info
* @return	Return 0 if succeeded, return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetFtpConfig(DPSDK_INT32 iSessionID,
													   DPSDK_FTP_CONFIG_INFO* pFtpInfo);

/**
* @brief	FTP Config Info
* @param	[IN] iSessionID				User Session ID
* @param	[IN] pFtpInfo				Ftp Config Info
* @return	Return 0 if succeeded, return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_SaveFtpConfig(DPSDK_INT32 iSessionID,
														DPSDK_FTP_CONFIG_INFO* pFtpInfo);

/**
* @brief	Timing Config Info
* @param	[IN] iSessionID				User Session ID
* @param	[OUT] pTimeSyncInfo			Timing Config Info
* @return	Return 0 if succeeded, return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetTimeSyncConfig(DPSDK_INT32 iSessionID,
															DPSDK_TIME_SYNC_CONFIG_INFO* pTimeSyncInfo);

/**
* @brief	Timing Config Info
* @param	[IN] iSessionID				User Session ID
* @param	[IN] pTimeSyncInfo			Timing Config Info
* @return	Return 0 if succeeded, return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_SaveTimeSyncConfig(DPSDK_INT32 iSessionID,
															 DPSDK_TIME_SYNC_CONFIG_INFO* pTimeSyncInfo);

/**
* @brief	Email Config Info
* @param	[IN] iSessionID				User Session ID
* @param	[OUT] pMailInfo				Email Config info
* @return	Return 0 if succeeded, return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetMailConfig(DPSDK_INT32 iSessionID,
														DPSDK_MAIL_CONFIG_INFO* pMailInfo);

/**
* @brief	Email Config Info
* @param	[IN] iSessionID				User Session ID
* @param	[IN] pMailInfo				Email Config Info
* @return	Return 0 if succeeded, return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_SaveMailConfig(DPSDK_INT32 iSessionID,
														 DPSDK_MAIL_CONFIG_INFO* pMailInfo);

/**
* @brief	Verify Email 
* @param	[IN] iSessionID				User Session ID
* @param	[IN] pMailInfo				Email Config Info
* @param	[OUT] bResult				Verification Result, true: verification passed, false: verification failed
* @return	Return 0 if succeeded, return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_TestMailConfig(DPSDK_INT32 iSessionID,
														DPSDK_MAIL_CONFIG_INFO* pMailInfo,
														DPSDK_BOOL* bResult);

/**
* @brief	User Status Query
* @param	[IN] iSessionID				User Session ID
* @param	[IN] pPageInfo				Page Info
* @param	[OUT] uiTotal				Total Record Number
* @param	[OUT] pUserStatusList		User Status List
* @return	Return 0 if succeeded, return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetUserStatusList(DPSDK_INT32 iSessionID,
															DPSDK_CHAR* pKeyWord,
														    DPSDK_PAGE_INFO* pPageInfo,
														    DPSDK_UINT32* uiTotal,
														    DPSDK_USER_STATUS_LIST* pUserStatusList);

/**
* @brief	Device Status Query
* @param	[IN] iSessionID				User Session ID
* @param	[IN] pQueryParam			Query Parameter
* @param	[IN] pPageInfo				Page Info
* @param	[OUT] uiTotal				Total Record Number
* @param	[OUT] pDeviceStatusList		Device Status List
* @return	Return 0 if succeeded, return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetDeviceStatusList(DPSDK_INT32 iSessionID,
															DPSDK_QUERY_DEVICE_STATUS_PARAM* pQueryParam,
															DPSDK_PAGE_INFO* pPageInfo,
															DPSDK_UINT32* uiTotal,
															DPSDK_DEVICE_STATUS_LIST* pDeviceStatusList);

/**
* @brief	Device Health Report Query
* @param	[IN] iSessionID				User Session ID
* @param	[IN] pQueryParam			Query Parameter
* @param	[IN] pPageInfo				Page Info
* @param	[OUT] uiTotal				Total Record Number
* @param	[OUT] pHealthReportList		Device Health Report List
* @return	Return 0 if succeeded, return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetDeviceHealthReportList(DPSDK_INT32 iSessionID,
																	DPSDK_QUERY_DEVICE_HEALTH_REPORT_PARAM* pQueryParam,
																	DPSDK_PAGE_INFO* pPageInfo,
																	DPSDK_UINT32* uiTotal,
																	DPSDK_DEVICE_HEALTH_REPORT_LIST* pHealthReportList);

/**
* @brief	User Operation Log Query
* @param	[IN] iSessionID				User Session ID
* @param	[IN] pQueryParam			Query Parameter
* @param	[IN] pPageInfo				Page Info
* @param	[OUT] uiTotal				Total Record Number
* @param	[OUT] pLogList				Log List
* @return	Return 0 if succeeded, return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetUserLogList(DPSDK_INT32 iSessionID,
														 DPSDK_QUERY_LOG_PARAM* pQueryParam,
														 DPSDK_PAGE_INFO* pPageInfo,
														 DPSDK_UINT32* uiTotal,
														 DPSDK_LOG_LIST* pLogList);

/**
* @brief	System Log Query
* @param	[IN] iSessionID				User Session ID
* @param	[IN] pQueryParam			Query Parameter
* @param	[IN] pPageInfo				Page Info
* @param	[OUT] uiTotal				Total Record Number
* @param	[OUT] pLogList				Log List
* @return	Return 0 if succeeded, return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetSystemLogList(DPSDK_INT32 iSessionID,
														  DPSDK_QUERY_LOG_PARAM* pQueryParam,
														  DPSDK_PAGE_INFO* pPageInfo,
														  DPSDK_UINT32* uiTotal,
														  DPSDK_LOG_LIST* pLogList);

/**
* @brief	Export User Operation Log
* @param	[IN] iSessionID				User Session ID
* @param	[IN] pExportParam			Export Parameter
* @param	[OUT] pFileUrl				File URL
* @param	[IN] uiBufLen				Cache Size
* @return	Return 0 if succeeded, return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_ExportUserLog(DPSDK_INT32 iSessionID,
														DPSDK_EXPORT_LOG_PARAM* pExportParam,
														DPSDK_CHAR* pFileUrl,
														DPSDK_UINT32 uiBufLen);

/**
* @brief	Export System Log
* @param	[IN] iSessionID				User Session ID
* @param	[IN] pExportParam			Export Parameter
* @param	[OUT] pFileUrl				File URL
* @param	[IN] uiBufLen				Cache Size
* @return	Return 0 if succeeded, return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_ExportSystemLog(DPSDK_INT32 iSessionID,
														  DPSDK_EXPORT_LOG_PARAM* pExportParam,
														  DPSDK_CHAR* pFileUrl,
														  DPSDK_UINT32 uiBufLen); 

/**
* @brief	Acquire Service Port
* @param	[IN] iSessionID				User Session ID
* @param	[IN] pServerName			Service Name
* @param	[OUT] pServerPort			Service Port
* @return	Return 0 if succeeded, return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetServerPort(DPSDK_INT32 iSessionID,
														DPSDK_CHAR* pServerName,
														DPSDK_INT32* pServerPort);

/**
* @brief	Acquire Service List
* @param	[IN] iSessionID				User Session ID
* @param	[OUT] pServerList			Service Status List
* @param	[IN] uiBufLen				Cache Size
* @return	Return 0 if succeeded, return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetServerList(DPSDK_INT32 iSessionID,
													DPSDK_SERVER_LIST* pServerList,
												    DPSDK_UINT32 uiBufLen);
/**
* @brief	Modify Service Status
* @param	[IN] iSessionID				User Session ID
* @param	[IN] pChangeStatusParam		Modify service status request parameter
* @return	Return 0 if succeeded, return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_ChangeServerStatus(DPSDK_INT32 iSessionID,
															 DPSDK_CHANGE_SERVER_STATUS_PARAM* pChangeStatusParam);

/**
* @brief	Modify service IP, port, internal and external network mapping (modify service port according to service name)
* @param	[IN] iSessionID				User Session ID
* @param	[IN] pServerName			Service Name
* @param	[OUT] pServerPort			Service Port
* @return	Return 0 if succeeded, return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_ConfigServerPort(DPSDK_INT32 iSessionID,
														 DPSDK_CHAR* pServerName,
													     DPSDK_INT32 iServerPort);

/**
* @brief	Modify service IP, port, internal and external network mapping
* @param	[IN] iSessionID				User Session ID
* @param	[IN] pConfigServerInfo		Service Config Info
* @return	Return 0 if succeeded, return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_SetServerConfig(DPSDK_INT32 iSessionID,
														  DPSDK_CONFIG_SERVER_INFO* pConfigServerInfo);

/**
* @brief	Acquire service IP, port, internal and external network mapping
* @param	[IN] iSessionID				User Session ID
* @param	[OUT] pConfigServerInfo		Service Config Info
* @return	Return 0 if succeeded, return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetServerConfig(DPSDK_INT32 iSessionID,
														  DPSDK_CONFIG_SERVER_INFO* pConfigServerInfo);

/**
* @brief	Acquire Service Status
* @param	[IN] iSessionID				User Session ID
* @param	[OUT] pFunctionList			Function List
* @param	[IN] uiBufLen				Cache Size
* @return	Return 0 if succeeded, return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetServerFunction(DPSDK_INT32 iSessionID,
															DPSDK_SERVER_FUNCTION_LIST* pFunctionList, 
															DPSDK_UINT32 uiBufLen);

/**
* @brief	Modify Service Status
* @param	[IN] iSessionID				User Session ID
* @param	[IN] pFunctionList			Function List
* @param	[IN] uiBufLen				Cache Size
* @return	Return 0 if succeeded, return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_SetServerFunction(DPSDK_INT32 iSessionID,
															DPSDK_SERVER_FUNCTION_LIST* pFunctionList,
															DPSDK_UINT32 uiBufLen);

/**
* @brief	Acquire Visual Intercom Device Unit Enable Status
* @param	[IN] iSessionID				User Session ID
* @param	[OUT] pStatus				Enable Status, "Status,1-Enable  0-Disable"
* @return	Return 0 if succeeded, return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetVideoUnitEnable(DPSDK_INT32 iSessionID,
															DPSDK_INT32* pStatus);

/**
* @brief	Update Visual Intercom Device Unit Enable Status
* @param	[IN] iSessionID				User Session ID
* @param	[IN] iStatus				Enable status, "Status, 1-Enable  0-Disable"
* @return	Return 0 if succeeded, return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_UpdateVideoUnitEnable(DPSDK_INT32 iSessionID,
																DPSDK_INT32 iStatus);

/**
* @brief	Acquire Visual Intercom Device Building Enable Status
* @param	[IN] iSessionID				User Session ID
* @param	[OUT] pStatus				Enable Status, "Status, 1-Enable  0-Disable"
* @return	Return 0 if succeeded, return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetVideoIntercomBuildingEnable(DPSDK_INT32 iSessionID,
																	DPSDK_INT32* pStatus);

/**
* @brief	Update Visual Intercom Device Building Enable Status
* @param	[IN] iSessionID				User Status ID
* @param	[IN] iStatus				Enable Status, "Status, 1-Enable  0-Disable"
* @return	Return 0 if succeeded, return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_UpdateVideoIntercomBuildingEnable(DPSDK_INT32 iSessionID,
																			DPSDK_INT32 iStatus);

//////////////////////////////////////////////////////////////////////////
/*
*	Device Management Interface
*/

/**
* @brief	Search Device
* @param	[IN] iSessionID				User Session ID
* @param	[IN] pSearchParam			Search Parameter
* @param	[IN] pPageInfo				Page Info
* @param	[OUT] uiTotal				Total Record Number
* @param	[OUT] pSearchDeviceList		        Search Device List
* @param	[IN] uiBufLen				Cache Size
* @return	Return 0 if succeeded, return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_SearchDevices(DPSDK_INT32 iSessionID,
														DPSDK_SEARCH_DEVICE_PARAM* pSearchParam,
														DPSDK_PAGE_INFO* pPageInfo,
														DPSDK_UINT32* uiTotal,
														DPSDK_SEARCH_DEVICE_LIST* pSearchDeviceList,
														DPSDK_UINT32 uiBufLen);

/**
* @brief	Device Add Management
* @param	[IN] iSessionID				User Session ID
* @param	[IN] pSaveDeviceInfo		Device Info
*@param	    [OUT] pReslutList		    Device add management return info list
* @param	[IN] uiBufLen				Cache Size
* @return	Return 0 if succeeded, return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_SaveDevices(DPSDK_INT32 iSessionID,
													  DPSDK_SAVE_DEVICE_INFO* pSaveDeviceInfo, 
													  DPSDK_SAVE_DEVICE_LIST* pReslutList,
													  DPSDK_UINT32 uiBufLen);

/**
* @brief	Add Device
* @param	[IN] iSessionID				User Session ID
* @param	[IN] pAddDeviceInfo 		Add Device Info
* @param	[OUT] pResult	 			Return Result
* @return	Return 0 if succeeded, return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_AddDevices(DPSDK_INT32 iSessionID, 
													 DPSDK_ADD_DEVICE_INFO* pAddDeviceInfo,
													 DPSDK_ADD_DEVICE_RESULT* pResult);

/**
* @brief	Batch Add Device
* @param	[IN] iSessionID				User Session ID
* @param	[IN] pAddDeviceList 		Add Device List Info
* @param	[IN] fDataCallBack			Data sync callback function, refer to DPSDK_ADD_DEVICE_RESULT_LIST for data type
* @param	[IN] pUserData				User Data
* @return	Return 0 if succeeded, return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_AddDeviceList(DPSDK_INT32 iSessionID, 
														DPSDK_ADD_DEVICE_LIST* pAddDeviceList,
														DPSDK_DataCallback fDataCallBack,
														DPSDK_VOID* pUserData);

/**
* @brief	Update Device
* @param	[IN] iSessionID				User Session ID
* @param	[IN] pDeviceInfo 			Device Info
* @param	[IN] fDataCallBack			Data sync callback function, refer to DPSDK_DELETE_DEVICE_FAIL_INFO for data type
* @param	[IN] pUserData				User Data
* @return	Return 0 if succeeded, return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_UpdateDevice(DPSDK_INT32 iSessionID,
													   DPSDK_UPDATE_DEVICE_INFO* pDeviceInfo,
													   DPSDK_DataCallback fDataCallBack,
													   DPSDK_VOID* pUserData);


/**
* @brief	Query Device Info
* @param	[IN] iSessionID				User Session ID
* @param	[IN] pQueryDevParam 		Query parameter
* @param	[IN] fDataCallBack			Data sync callback function, refer to DPSDK_DATA_QUERY_DEVICE_INFO for data type
* @param	[IN] pUserData				User Data
* @return	Return 0 if succeeded, return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_QueryDevice(DPSDK_INT32 iSessionID,
														DPSDK_QUERY_DEV_PARAM* pQueryDevParam,
														DPSDK_DataCallback fDataCallBack,
														DPSDK_VOID* pUserData);

/**
* @brief	Modify Device Password
* @param	[IN] iSessionID				User Session ID
* @param	[IN] pDeviceCode 			Device Coding
* @param	[IN] pLoginPWD 				Device Password
* @param	[IN] pOldPWD 				Original Device Password
* @return	Return 0 if succeeded, return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_ModifyDevicePWD(DPSDK_INT32 iSessionID,
														  DPSDK_CHAR* pDeviceCode, 
														  DPSDK_CHAR* pLoginPWD,
														  DPSDK_CHAR* pOldPWD);

/**
* @brief	Modify Device Organization
* @param	[IN] iSessionID				User Session ID
* @param	[IN] pDeviceList 			Device Coding List
* @param	[IN] iDeviceNum 			Device Coding Number
* @param	[IN] pOrgCode 			`	Organization Coding
* @param	[IN] pOrgName 			`	Organization Name
* @return	Return 0 if succeeded, return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_ModifyDeviceOrg(DPSDK_INT32 iSessionID,
														  DPSDK_DEVICE_ID* pDeviceList,
														  DPSDK_INT32 iDeviceNum,
														  DPSDK_CHAR* pOrgCode, 
														  DPSDK_CHAR* pOrgName);

/**
* @brief	Device Initialization
* @param	[IN] iSessionID				User Session ID
* @param	[IN] pInitDevInfo 			Device Modify Info
* @return	Return 0 if succeeded, return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_InitDevice(DPSDK_INT32 iSessionID,
													 DPSDK_INIT_DEVICE_INFO* pInitDevInfo);

/**
* @brief	Device Import
* @param	[IN] iSessionID				User Session ID
* @param	[IN] pLoginName 			User Name
* @param	[IN] pLoginPWD 				Device Password
* @return	Return 0 if succeeded, return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_ImportDevice(DPSDK_INT32 iSessionID,
													   DPSDK_CHAR* pLoginName,
													   DPSDK_CHAR* pLoginPWD);

/**
* @brief	Device Export
* @param	[IN] iSessionID				User Session ID
* @param	[IN] iDeviceCategory 		Device Category
* @param	[IN] pPageInfo				Page Info
* @param	[OUT] uiTotal				Total Record Number
* @param	[OUT] pExportDevList 		Device Export List
* @return	Return 0 if succeeded, return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_ExportDevice(DPSDK_INT32 iSessionID,
													   DPSDK_INT32 iDeviceCategory,
													   DPSDK_PAGE_INFO* pPageInfo,
													   DPSDK_UINT32* uiTotal,
													   DPSDK_EXPORT_DEVICE_LIST* pExportDevList);

/**
* @brief	Delete Device
* @param	[IN] iSessionID				User Session ID
* @param	[IN] pDeviceList 			Device List
* @param	[IN] uiBufLen 				Cache Size
* @param	[IN] fDataCallBack			Data sync callback function, refer to DPSDK_DATA_DELETE_DEVICE_INFO for data type
* @param	[IN] pUserData				User Data
* @return	Return 0 if succeeded, return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_DeleteDevice(DPSDK_INT32 iSessionID,
													   DPSDK_DEVICE_LIST* pDeviceList,
													   DPSDK_INT32 uiBufLen,
													   DPSDK_DataCallback fDataCallBack,
													   DPSDK_VOID* pUserData);

DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_DeleteUnAuthDevices(DPSDK_INT32 iSessionID,
															  DPSDK_DEVICE_LIST* pDeviceList,
															  DPSDK_INT32 uiBufLen);

DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetUnAuthDevices(DPSDK_INT32 iSessionID,
													   DPSDK_DataCallback fDataCallBack,
													   DPSDK_VOID* pUserData);

DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_SaveUnAuthDevices(DPSDK_INT32 iSessionID,
													   DPSDK_DEVICE_LIST* pDeviceList,
													   DPSDK_INT32 uiBufLen,
													   DPSDK_DataCallback fDataCallBack,
													   DPSDK_VOID* pUserData);

/**
* @brief	Acquire Dictionary Type
* @param	[IN] iSessionID				User Session ID
* @param	[IN] pTypeList			 	Query Type
* @param	[IN] iTypeNum			 	Query Type Number
* @param	[IN] pLocale			 	International Voice 
* @param	[OUT] pDictionaryList 		Dictionary List
* @param	[IN] uiBufLen		 		Cache Size
* @return	Return 0 if succeeded, return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetDeviceDictionary(DPSDK_INT32 iSessionID,
															  DPSDK_INT32* pTypeList,
															  DPSDK_INT32 iTypeNum,
															  DPSDK_CHAR* pLocale,
															  DPSDK_DEV_DICTIONARY_LIST* pDictionaryList,
															  DPSDK_UINT32 uiBufLen);

/**
* @brief	Export device info file, used to generate authorization file
* @param	[IN] iSessionID				User Session ID
* @param	[IN] iDeviceIdNum 			Device Number
* @param	[IN] pDeviceIdList	 		Device ID List
* @param	[OUT] pDevFileData			File Data
* @param	[OUT] pDevFileLen			File Length
* @return	Return 0 if succeeded, return error code if failed.
* @note		pDevFileData needs to be released by DPSDK_ReleaseDataBuffer interface
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_ExportDeviceFile(DPSDK_INT32 iSessionID,
														   DPSDK_INT32 iDeviceIdNum,
														   DPSDK_CHAR(*pDeviceIdList)[DPSDK_DEVICE_ID_LEN],
														   DPSDK_CHAR** pDevFileData,
														   DPSDK_INT32* pDevFileLen);

/**
* @brief	Import Device Authorization File
* @param	[IN] iSessionID				User Session ID
* @param	[IN] pDeviceId 				Device ID
* @param	[IN] pSecurityCode			Security Code
* @param	[IN] iSecurityCodeLen		Security Code Length
* @return	Return 0 if succeeded, return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_ImportDeviceSecurity(DPSDK_INT32 iSessionID,
															   DPSDK_CHAR* pDeviceId,
															   DPSDK_CHAR* pSecurityCode,
															   DPSDK_INT32 iSecurityCodeLen);

/**
* @brief	Acquire Device Reset Password Info
* @param	[IN] iSessionID				User Session ID
* @param	[IN] pDeviceId 				Device ID
* @param	[OUT] pPreSetEmail 			Reserved Email Address
* @param	[OUT] pPreSetPhone 			Reserved Mobile Number
* @param	[OUT] pQRCode				QR Code Info
* @param	[OUT] pQRCodeLen			QR Code Length
* @return	Return 0 if succeeded, return error code if failed.
* @note		pQRCode needs to be released by DPSDK_ReleaseDataBuffer interface
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetDeviceResetInfo(DPSDK_INT32 iSessionID,
															 DPSDK_CHAR* pDeviceId,
															 DPSDK_CHAR* pPreSetEmail,
															 DPSDK_CHAR* pPreSetPhone,
															 DPSDK_CHAR** pQRCode,
															 DPSDK_INT32* pQRCodeLen);

/**
* @brief	Reset Device Password
* @param	[IN] iSessionID				User Session ID
* @param	[IN] pDeviceId 				Device ID
* @param	[IN] pSecurityCode 			Security Code
* @param	[IN] iSecurityCodeLen 		Security Code Length
* @return	Return 0 if succeeded, return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_ResetDevice(DPSDK_INT32 iSessionID,
													  DPSDK_CHAR* pDeviceId,
													  DPSDK_CHAR* pSecurityCode,
													  DPSDK_INT32 iSecurityCodeLen);

/**
* @brief	Query if device IP is available
* @param	[IN] iSessionID				User Session ID
* @param	[IN] pDeviceIP 				Device IP
* @param	[OUT] bValid 				If it is available  true: available£ªfalse:Unavailable
* @return	Return 0 if succeeded, return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_IsDeviceIPValid(DPSDK_INT32 iSessionID,
														  DPSDK_CHAR* pDeviceIP,
														  DPSDK_BOOL* bValid); 

/**
* @brief	Modify Device IP
* @param	[IN] iSessionID				User Session ID
* @param	[IN] pDeviceInfo 			Device Info
* @return	Return 0 if succeeded, return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_ModifyDeviceIP(DPSDK_INT32 iSessionID,
														 DPSDK_MODIFY_DEVICE_IP_INFO* pDeviceInfo);

/**
* @brief	Query the reason of device offline
* @param	[IN] iSessionID				User Session ID
* @param	[IN] pParam		 			Request Parameter
* @param	[OUT] pResultList		 	Result List
* @param	[IN] uiBufLen		 		Cache Size
* @return	Return 0 if succeeded, return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetDeviceOfflineReason(DPSDK_INT32 iSessionID,
																 DPSDK_GET_DEVICE_OFFLINE_REASON_PARAM* pParam, 
																 DPSDK_GET_DEVICE_OFFLINE_REASON_RESULT_LIST* pResultList, 
																 DPSDK_UINT32 uiBufLen);

/**
* @brief	Acquire device search soso channel info status (If search has been completed)
* @param	[IN] iSessionID				User Session ID
* @param	[OUT] pSearchResultList		Search Result List
* @param	[IN] uiBufLen		 		Cache Size
* @return	Return 0 if succeeded, return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_SearchDeviceChannel(DPSDK_INT32 iSessionID,
															DPSDK_SOSO_CHANNEL_LIST* pSearchResultList,
															DPSDK_UINT32 uiBufLen);

/**
* @brief	Acquire all device codes which have not been searched
* @param	[IN] iSessionID				User Session ID
* @param	[IN] fDataCallBack		    Data sync callback function, refer to DPSDK_DATA__GET_UNSEARCH_DEVICE for data type
* @param	[IN] pUserData				User Data
* @return	Return 0 if succeeded, return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetUnSearchedDevice(DPSDK_INT32 iSessionID,
															  DPSDK_DataCallback fDataCallBack,
															  DPSDK_VOID* pUserData);

/**
* @brief	Add Organization
* @param	[IN] pOrgParam				Add Organization Parameter
* @param	[OUT] pOrgCode 				Newly added organization code
* @return	Return 0 if succeeded, return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_AddOrg(DPSDK_INT32 iSessionID,
												 DPSDK_ORG_EDIT_PARAM* pOrgParam,
												 DPSDK_CHAR* pOrgCode);

/**
* @brief	Modify Organization
* @param	[IN] pOrgCode	 			Organization Code
* @param	[IN] pOrgParam				Modify Organization Parameter
* @return	Return 0 if succeeded, return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_ModifyOrg(DPSDK_INT32 iSessionID,
													DPSDK_CHAR* pOrgCode,
													DPSDK_ORG_EDIT_PARAM* pOrgParam);

/**
* @brief	Delete Organization
* @param	[IN] pOrgCode				Organization Code
* @return	Return 0 if succeeded, return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_DeleteOrg(DPSDK_INT32 iSessionID,
													DPSDK_CHAR* pOrgCode);

/**
* @brief	Mobile Organization
* @param	[IN] pMoveParam				Organization Mobile Parameter
* @return	Return 0 if succeeded, return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_MoveOrg(DPSDK_INT32 iSessionID,
												  DPSDK_ORG_MOVE_PARAM* pMoveParam);


/**
* @brief	Search Channel Info
* @param	[IN] pSearchChnParam			Search Channel Request Parameter
* @param	[IN] fDataCallBack				Data sync callback function, refer to DPSDK_DATA_SEARCH_CHANNELS_INFO for data type
* @param	[IN] pUserData					User Data
* @return	Return 0 if succeeded, return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_SearchChannels(DPSDK_INT32 iSessionID,
														DPSDK_SEARCH_CHANNEL_INFO* pSearchChnParam,
														DPSDK_DataCallback fDataCallBack,
														DPSDK_VOID* pUserData);

/**
* @brief	barrier Control
* @param	[IN] pParam					Barrier Control Parameter
* @return	Return 0 if succeeded, return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_ControlSluice(DPSDK_INT32 iSessionID,
														DPSDK_ITC_PARAM* pParam);


//////////////////////////////////////////////////////////////////////////
/*
*	LICENSE Interface 
*/
/**
* @brief	Acquire License List
* @param	[IN] iSessionID				User Session ID
* @param	[OUT] pLicenseList			License List
* @param	[IN] uiBufLen			    Cache Size
* @return	Return 0 if succeeded, return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetLicenseList(DPSDK_INT32 iSessionID,
														 DPSDK_LICENSE_LIST* pLicenseList,
														 DPSDK_UINT32 uiBufLen);

/**
* @brief	Acquire License Component
* @param	[IN] iSessionID				User Session ID
* @param	[OUT] pComponentStatus		Whether to display a License Component£¨1: display, 0:not display
* @return	Return 0 if succeeded, return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetLicenseComponent(DPSDK_INT32 iSessionID,
															  DPSDK_INT32* pComponentStatus);

/**
* @brief	Export License
* @param	[IN] iSessionID				User Session ID
* @param	[IN] pLicenseList			License List
* @param	[IN] uiBufLen			    Cache Size
* @param	[OUT] pLicenseFile			License File
* @param	[OUT] pFileLen				File Length
* @return	Return 0 if succeeded, return error code if failed.
* @note		pLicenseFile needs to be released by DPSDK_ReleaseDataBuffer interface
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_ExportLicense(DPSDK_INT32 iSessionID,
														DPSDK_LICENSE_LIST* pLicenseList,
														DPSDK_UINT32 uiBufLen,
														DPSDK_CHAR** pLicenseFile,
														DPSDK_INT32* pFileLen);

/**
* @brief	Import License
* @param	[IN] iSessionID				User Session ID
* @param	[IN] pLicenseFile			License File
* @param	[IN] iFileLen				File Length
* @param	[OUT] pLicenseList			License List
* @param	[IN] uiBufLen			    Cache Size
* @return	Return 0 if succeeded, return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_ImportLicense(DPSDK_INT32 iSessionID,
														DPSDK_CHAR* pLicenseFile,
														DPSDK_INT32 iFileLen,
														DPSDK_LICENSE_LIST* pLicenseList,
														DPSDK_UINT32 uiBufLen);

//////////////////////////////////////////////////////////////////////////
/*
*	Record Schedule Interface
*/

/**
* @brief	Search record schedule list
* @param	[IN] iSessionID				User Session ID
* @param	[IN] pPageInfo				Page Info
* @param	[OUT] uiTotal				Total Record Number
* @param	[OUT] pPlanList 			Record Schedule List
* @return	Return 0 if succeeded, return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetStoragePlanList(DPSDK_INT32 iSessionID,
															 DPSDK_PAGE_INFO* pPageInfo,
															 DPSDK_UINT32* uiTotal,
															 DPSDK_STORAGE_PLAN_LIST* pPlanList);

/**
* @brief	Search Record Schedule
* @param	[IN] iSessionID				User Session ID
* @param	[IN] iPlanID				Record Schedule ID
* @param	[OUT] pPlanInfo 			Record Schedule
* @return	Return 0 if succeeded, return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetStoragePlan(DPSDK_INT32 iSessionID,
														 DPSDK_INT32 iPlanID,
														 DPSDK_STORAGE_PLAN_INFO* pPlanInfo);

/**
* @brief	Modify Record Schedule
* @param	[IN] iSessionID				User Session ID
* @param	[IN] pPlanInfo 				Record Schedule
* @return	Return 0 if succeeded, return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_SaveStoragePlan(DPSDK_INT32 iSessionID,
														  DPSDK_STORAGE_PLAN_INFO* pPlanInfo);

/**
* @brief	Add Record Schedule
* @param	[IN] iSessionID				User Session ID
* @param	[IN] pPlanInfo 				Record Schedule
* @param	[OUT] iPlanId 				Record Schedule ID
* @return	Return 0 if succeeded, return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_AddStoragePlan(DPSDK_INT32 iSessionID,
														 DPSDK_STORAGE_PLAN_INFO* pPlanInfo,
														 DPSDK_INT32* iPlanId);

/**
* @brief	Delete Record Schedule
* @param	[IN] iSessionID				User Session ID
* @param	[IN] iPlanNum 				Record Schedule Number
* @param	[IN] pPlanList 				Record Schedule id list
* @return	Return 0 if succeeded, return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_DeleteStoragePlan(DPSDK_INT32 iSessionID,
															DPSDK_INT32 iPlanNum,
															DPSDK_INT32* pPlanList);

/**
* @brief	Switch Record Schedule Status
* @param	[IN] iSessionID				User Session ID
* @param	[IN] iPlanId 				Record Schedule id
* @param	[IN] bStat 				Record Schedule Status, false:Disable, true:Enable
* @return	Return 0 if succeeded, return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_ChangeStoragePlanStatus(DPSDK_INT32 iSessionID,
																  DPSDK_INT32 iPlanId,
																  DPSDK_BOOL bStat);

/**
* @brief	Copy Record Schedule
* @param	[IN] iSessionID				User Session ID
* @param	[IN] pCopyParam				Copy Request Parameter
* @return	Return 0 if succeeded, return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_CopyStoragePlan(DPSDK_INT32 iSessionID,
														  DPSDK_STORAGE_PLAN_COPY_PARAM* pCopyParam);


//////////////////////////////////////////////////////////////////////////
/*
*	Storage configuration interface 
*/

/**
* @brief	Search HDD list 
* @param	[IN] iSessionID				User session ID
* @param	[IN] pPageInfo				Page info
* @param	[OUT] uiTotal				Total info
* @param	[OUT] pDiskList 			Storage configuration list
* @return	Return 0 if succeeded, return error code if failed. 
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetStorageDiskList(DPSDK_INT32 iSessionID,
															 DPSDK_PAGE_INFO* pPageInfo,
															 DPSDK_UINT32* uiTotal,
															 DPSDK_STORAGE_DISK_LIST* pDiskList);

/**
* @brief	Add network HDD
* @param	[IN] iSessionID				User session ID
* @param	[IN] pServerCode 			Server ID
* @param	[IN] pIP	 		        Network HDD IP
* @return	Return 0 if succeeded, return error code if failed. 
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_AddStorageDisk(DPSDK_INT32 iSessionID,
														 DPSDK_CHAR* pServerCode,
														 DPSDK_CHAR* pIP);

/**
* @brief	Format HDD
* @param	[IN] iSessionID				User session ID
* @param	[IN] pFormatParam 			Format parameters
* @return	Return 0 if succeeded, return error code if failed. 
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_FormatStorageDisk(DPSDK_INT32 iSessionID,
															DPSDK_STORAGE_DISK_FORMAT_PARAM* pFormatParam);

/**
* @brief	Get HDD Format Status
* @param	[IN] iSessionID				User session ID
* @param	[OUT] iStatus 				Storage format status
* @return	Return 0 if succeeded, return error code if failed. 
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetStorageFormatStatus(DPSDK_INT32 iSessionID,
																 DPSDK_INT32* iStatus);

/**
* @brief	Delete HDD
* @param	[IN] iSessionID				User session ID
* @param	[IN] pDeleteParam 			Delete HDD parameters
* @return	Return 0 if succeeded, return error code if failed. 
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_DeleteStorageDisk(DPSDK_INT32 iSessionID,
															DPSDK_STORAGE_DISK_DELETE_PARAM* pDeleteParam);

/**
* @brief	Take HDD from other server
* @param	[IN] iSessionID				User session ID
* @param	[IN] pRobParam 				Take HDD amount 
* @return	Return 0 if succeeded, return error code if failed
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_RobStorageDisk(DPSDK_INT32 iSessionID,
														 DPSDK_STORAGE_DISK_ROB_PARAM* pRobParam);

/**
* @brief	Switch HDD type. It is from video to image, or from image to video. 
* @param	[IN] iSessionID				User session ID
* @param	[IN] pChangeParam 			Switch HDD type parameters
* @return	Return 0 if succeeded, return error code if failed
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_ChangeStorageType(DPSDK_INT32 iSessionID,
															DPSDK_STORAGE_DISK_CHANGE_PARAM* pChangeParam);

/**
* @brief	Search local storage 
* @param	[IN] iSessionID				User session ID
* @param	[IN] fDataCallBack			Data synchronization call function. refer to DPSDK_DATA_STORAGE_GETLOGICALPARTITION for data type
* @param	[IN] pUserData				User data
* @return	Return 0 if succeeded, return error code if failed
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetLogicalPartition(DPSDK_INT32 iSessionID,
															DPSDK_DataCallback fDataCallBack,
															DPSDK_VOID* pUserData);

/**
* @brief	Create virtual HDD
* @param	[IN] iSessionID				User session ID
* @param	[IN] pCreateParam 			Virtual HDD parameters 
* @return	Return 0 if succeeded, return error code if failed
*/

DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_CreateVirtualDisk(DPSDK_INT32 iSessionID,
															DPSDK_STORAGE_VIRTUALDISK_PARAM* pCreateParam);

/**
* @brief	Delete virtual HDD
* @param	[IN] iSessionID				User session ID
* @param	[IN] pDeleteParam 			Virtual HDD parameters
* @return	Return 0 if succeeded, return error code if failed
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_DeleteVirtualDisk(DPSDK_INT32 iSessionID,
															DPSDK_STORAGE_VIRTUALDISK_DELETE_PARAM* pDeleteParam);



/**
* @brief	Search video service list
* @param	[IN] iSessionID				User session ID
* @param	[IN] pLocale				language zh_CN/en
* @param	[IN] uiBufLen				Buffer size
* @param	[OUT] pServerList 			Service list 
* @return	Return 0 if succeeded, return error code if failed
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetVideoServerList(DPSDK_INT32 iSessionID,
															 DPSDK_CHAR* pLocale,
	                                                         DPSDK_DISK_SERVER_LIST* pServerList,
                                                             DPSDK_UINT32 uiBufLen);

/**
* @brief	Search image service list 
* @param	[IN] iSessionID				User session ID
* @param	[IN] pLocale				language zh_CN/en
* @param	[IN] uiBufLen				Buffer size
* @param	[OUT] pServerList 			Service list 
* @return	Return 0 if succeeded, return error code if failed
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetPicServerList(DPSDK_INT32 iSessionID,
														   DPSDK_CHAR* pLocale,
	                                                       DPSDK_DISK_SERVER_LIST* pServerList,
	                                                       DPSDK_UINT32 uiBufLen);

//////////////////////////////////////////////////////////////////////////
/*
*	TV wall interface (Client)
*/

/**
* @brief	Get TV wall list 
* @param	[IN] iSessionID				User session ID
* @param	[OUT] pTVWallList 			TV wall list 
* @param	[IN] uiBufLen	 			Buffer size
* @return	Return 0 if succeeded, return error code if failed
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetTVWallList(DPSDK_INT32 iSessionID,
														DPSDK_TVWALL_LIST* pTVWallList,
														DPSDK_UINT32 uiBufLen);

/**
* @brief	Get TV wall information 
* @param	[IN] iSessionID				User session ID
* @param	[IN] iTVWallID 				TV wall ID
* @param	[OUT] pTVWallInfo 			TV wall info 
* @return	Return 0 if succeeded, return error code if failed
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetTVWallInfo(DPSDK_INT32 iSessionID,
														DPSDK_INT32 iTVWallID,
														DPSDK_TVWALL_INFO* pTVWallInfo);

/**
* @brief	Get decoded channel Xml of the bonding TV wall£®do not use now£©
* @param	[IN] iSessionID				User session ID
* @param	[IN] iTVWallID 				TV wall ID
* @param	[OUT] pScreenDecoderXml 	Decoded channel Xml of the bonding TV wall
* @return	Return 0 if succeeded, return error code if failed
* @note		pScreenDecoderXml shall be released by DPSDK_ReleaseDataBuffer
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetTVWallScreenDecoderXml(DPSDK_INT32 iSessionID,
																	DPSDK_INT32 iTVWallID,
																	DPSDK_CHAR** pScreenDecoderXml);

/**
* @brief	Get TV wall task list 
* @param	[IN] iSessionID				User session ID
* @param	[IN] iTVWallID 				TV wall ID
* @param	[OUT] pTaskList 			TV wall task list
* @param	[IN] uiBufLen	 			Buffer size 
* @return	Return 0 if succeeded, return error code if failed
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetTVWallTaskList(DPSDK_INT32 iSessionID,
															DPSDK_INT32 iTVWallID,
															DPSDK_TVWALL_TASK_LIST* pTaskList,
															DPSDK_UINT32 uiBufLen);

/**
* @brief	Get TV wall task info 
* @param	[IN] iSessionID				User session ID
* @param	[IN] iTVWallID 				TV wall ID
* @param	[IN] iTaskID 				Task ID
* @param	[IN] fDataCallBack			Data synchronization call function. Refer to DPSDK_DATA_TVWALL_TASK_INFO for data type. 
* @param	[IN] pUserData				User data 
* @return	Return 0 if succeeded, return error code if failed
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetTVWallTaskInfo(DPSDK_INT32 iSessionID, 
															DPSDK_INT32 iTVWallID, 
															DPSDK_INT32 iTaskID, 
															DPSDK_DataCallback fDataCallBack,
															DPSDK_VOID* pUserData);

/**
* @brief	Get TV wall task Xml £®do not use now£©
* @param	[IN] iSessionID				User session ID
* @param	[IN] iTVWallID 				TV wall ID
* @param	[IN] iTaskID 				Task ID
* @param	[OUT] pTaskXml			 	Task Xml info
* @return	Return 0 if succeeded, return error code if failed
* @note		pTaskXml shall be released by DPSDK_ReleaseDataBuffer
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetTVWallTaskXml(DPSDK_INT32 iSessionID,
														   DPSDK_INT32 iTVWallID,
														   DPSDK_INT32 iTaskID,
														   DPSDK_CHAR** pTaskXml);

/**
* @brief	Get current running TV wall task info
* @param	[IN] iSessionID				User session ID
* @param	[OUT] pTaskList 			TV wall task list
* @param	[IN] uiBufLen	 			Buffer size 
* @return	Return 0 if succeeded, return error code if failed
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetCurrentTVWallTaskInfo(DPSDK_INT32 iSessionID,
																   DPSDK_CURRENT_TVWALL_TASK_LIST* pTaskList,
																   DPSDK_UINT32 uiBufLen);

/**
* @brief	Delete TV wall task 
* @param	[IN] iSessionID				User session ID
* @param	[IN] iTVWallId 				TV wall ID
* @param	[IN] iTaskId 				Task ID
* @return	Return 0 if succeeded, return error code if failed
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_DeleteTVWallTask(DPSDK_INT32 iSessionID,
														   DPSDK_INT32 iTVWallId,
														   DPSDK_INT32 iTaskId);


/**
* @brief	Add TV wall task £®do not use now£©
* @param	[IN] iSessionID				User session ID
* @param	[IN] pTaskBaseInfo 			TV wall task info
* @param	[IN] pTaskXmlData 			TV wall task Xml
* @return	Return 0 if succeeded, return error code if failed
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_AddTVWallTask(DPSDK_INT32 iSessionID,
														DPSDK_TVWALL_TASK_BASE_INFO* pTaskBaseInfo,
														DPSDK_CHAR* pTaskXmlData); 

/**
* @brief	Add or save TV wall task 
* @param	[IN] iSessionID				User session ID
* @param	[IN] pTaskInfo 				TV wall task 
* @return	Return 0 if succeeded, return error code if failed
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_AddTVWallTaskEx(DPSDK_INT32 iSessionID,
														  DPSDK_TVWALL_TASK_INFO* pTaskInfo);

/**
* @brief	Print Decoder 
* @param	[IN] iSessionID				User session ID
* @param	[IN] pParam 				Print Decoder Param 
* @return	Return 0 if succeeded, return error code if failed
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_TVWallPrintDecoder(DPSDK_INT32 iSessionID,
														     DPSDK_TVWALL_PRINT_DECODER_PARAM* pParam);


/**
* @brief	Control output to TV wall (do not use now)
* @param	[IN] iSessionID				User session ID
* @param	[IN] pCtrlInfo 				Control output to TV wall info
* @param	[IN] pTaskXmlData 			TV wall task Xml
* @return	Return 0 if succeeded, return error code if failed
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_MapToTVWall(DPSDK_INT32 iSessionID,
													  DPSDK_TVWALL_CONTROL_INFO* pCtrlInfo,
													  DPSDK_CHAR* pTaskXmlData); 

/**
* @brief	Control output to TV wall 
* @param	[IN] iSessionID				User session ID
* @param	[IN] pCtrlInfo 				Control output to TV wall info
* @param	[IN] pTaskInfoList 			TV wall task list 
* @return	Return 0 if succeeded, return error code if failed
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_MapToTVWallEx(DPSDK_INT32 iSessionID,
														DPSDK_TVWALL_CONTROL_INFO* pCtrlInfo,
														DPSDK_TVWALL_TASK_INFO_LIST* pTaskInfoList);
/**
* @brief	Save TV wall task file 
* @param	[IN] iSessionID				User session ID
* @param	[IN] szFileName				File name
* @param	[IN] pTaskInfoList 			TV wall task list 
* @return	Return 0 if succeeded, return error code if failed
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_SaveTVWallProjectFile(DPSDK_INT32 iSessionID,
																DPSDK_CHAR* szFileName,
																DPSDK_TVWALL_TASK_INFO_LIST* pTaskInfoList);

/**
* @brief	Get TV wall task file 
* @param	[IN] iSessionID				User session ID
* @param	[IN] szFileName				File name 
* @param	[IN] fDataCallBack			Data synchronization call function. Refer to DPSDK_DATA_GET_TVWALL_PROJECT_FILE for data type
* @param	[IN] pUserData				User data 
* @return	Return 0 if succeeded, return error code if failed
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetTVWallProjectFile(DPSDK_INT32 iSessionID,
															   DPSDK_CHAR* szFileName,
															   DPSDK_DataCallback fDataCallBack,
															   DPSDK_VOID* pUserData);

/**
* @brief	Control screen on/off 
* @param	[IN] iSessionID				User session ID
* @param	[IN] pCtrlInfo 				On/off control info
* @return	Return 0 if succeeded, return error code if failed
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_TVWallPowerControl(DPSDK_INT32 iSessionID,
															 DPSDK_TVWALL_POWER_CONTROL_INFO* pCtrlInfo); 

/**
* @brief	Add frame on the screen 
* @param	[IN] iSessionID				User session ID
* @param	[IN] pScreenAddFrame 		Adding frame on the video parameters
* @return	Return 0 if succeeded, return error code if failed
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_TVWallScreenAddFrame(DPSDK_INT32 iSessionID,
															   DPSDK_TVWALL_SCREEN_ADD_FRAME* pScreenAddFrame);

/**
* @brief	Fisheye instant on TV wall 
* @param	[IN] iSessionID				User session ID
* @param	[IN] pFishEyeToWall 		Fisheye instant on TV wall  parameters
* @return	Return 0 if succeeded, return error code if failed
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_TVWallFishEyeToWall(DPSDK_INT32 iSessionID,
															  DPSDK_TVWALL_FISH_EYE_TO_WALL* pFishEyeToWall);

/**
* @brief	Open new window 
* @param	[IN] iSessionID				User session ID
* @param	[IN] pOpenWndList 			Opening new window list 
* @param	[IN] uiBufLen 				Buffer size 
* @return	Return 0 if succeeded, return error code if failed
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_TVWallOpenWindows(DPSDK_INT32 iSessionID,
															DPSDK_TVWALL_OPEN_WINDOW* pOpenWndList,
															DPSDK_INT32 uiBufLen);

/**
* @brief	Get TV wall capability set 
* @param	[IN] iSessionID				User session ID
* @param	[IN] iTvWallId		 		TV wall ID
* @param	[IN] pScreenCapsList 		        Capability set 
* @param	[IN] uiBufLen 				Buffer size 
* @return	Return 0 if succeeded, return error code if failed
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetTVWallScreenCaps(DPSDK_INT32 iSessionID,
															  DPSDK_INT32 iTvWallId,
															  DPSDK_TVWALL_SCREEN_CAPS_LIST* pScreenCapsList,
															  DPSDK_INT32 uiBufLen);

//////////////////////////////////////////////////////////////////////////
/*
*	TV wall interface (Manager)
*/

/**
* @brief	Save TV wall settings 
* @param	[IN] iSessionID				User session ID
* @param	[IN] pTVWallInfo 			TV wall info
* @return	Return 0 if succeeded, return error code if failed
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_SaveTVWallConfigInfo(DPSDK_INT32 iSessionID,
															   DPSDK_TVWALL_CONFIG_INFO* pTVWallInfo);

/**
* @brief	Add TV wall settings 
* @param	[IN] iSessionID				User session ID
* @param	[IN] pTVWallInfo 			TV wall info
* @param	[OUT] pTvWallID 			TV wall ID
* @return	Return 0 if succeeded, return error code if failed
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_AddTVWallConfigInfo(DPSDK_INT32 iSessionID,
															  DPSDK_TVWALL_CONFIG_INFO* pTVWallInfo,
															  DPSDK_INT32* pTvWallID);

/**
* @brief	Switch TV wall status
* @param	[IN] iSessionID				User session ID
* @param	[IN] iTvWallId 				TV wall ID
* @param	[IN] bStat 					Status false:disable,true:enable
* @return	Return 0 if succeeded, return error code if failed
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_SetTVWallStatus(DPSDK_INT32 iSessionID,
														  DPSDK_INT32 iTVWallId,
														  DPSDK_BOOL bStat); 

/**
* @brief	Delete TV wall 
* @param	[IN] iSessionID				User session ID
* @param	[IN] iTVWallId 				TV wall ID
* @return	Return 0 if succeeded, return error code if failed
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_DeleteTVWall(DPSDK_INT32 iSessionID,
													   DPSDK_INT32 iTVWallId);

/**
* @brief	Get TV wall configuration info
* @param	[IN] iSessionID				User session ID
* @param	[IN] iTVWallId 				TV wall ID
* @param	[OUT] pTvWallInfo 			TV wall info
* @return	Return 0 if succeeded, return error code if failed
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetTVWallConfigInfo(DPSDK_INT32 iSessionID,
															  DPSDK_INT32 iTVWallId,
															  DPSDK_TVWALL_CONFIG_INFO* pTVWallInfo);


/**
* @brief	Check the TV wall name exists or not
* @param	[IN] iSessionID				User session ID
* @param	[IN] pTVWallName			TV wall ID
* @param	[OUT] bResult 				falseL:does not exist, true: exist
* @return	Return 0 if succeeded, return error code if failed
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_CheckTVWallNameExist(DPSDK_INT32 iSessionID,
															   DPSDK_CHAR* pTVWallName,
															   DPSDK_BOOL* bResult); 

//////////////////////////////////////////////////////////////////////////
/*
 *	PTZ interface
 */

/**
* @brief	PTZ function operation 
* @param	[IN]	iSessionID					User session ID					
* @param	[IN]	pPtzOperateFunctionParam	PTZ function operation parameters
* @param	[OUT]	pPtzOperateFunctionResult	Operation result 
* @return	Return 0 if succeeded, return error code if failed
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_PtzOperateFunction(DPSDK_INT32 iSessionID, 
															 DPSDK_PTZOPERATE_FUNCTION_PARAM* pPtzOperateFunctionParam, 
															 DPSDK_PTZOPERATE_RESULT* pPtzOperateFunctionResult);
/**
* @brief	Operate PTZ camera 
* @param	[IN]	iSessionID					User session ID
* @param	[IN]	pPtzOperateCamereParam		PTZ camera operation parameters
* @param	[OUT]	pPtzOperateResult			Operation result 
* @return	Return 0 if succeeded, return error code if failed
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_PtzOperateCamera(DPSDK_INT32 iSessionID, 
														   DPSDK_PTZOPERATE_CAMERA_PARAM* pPtzOperateCamereParam, 
														   DPSDK_PTZOPERATE_RESULT* pPtzOperateResult);
/**
* @brief	PTZ direction control
* @param	[IN]	iSessionID					User session ID
* @param	[IN]	pPtzOperateDirectParam		PTZ direction control parameters
* @param	[OUT]	pPtzOperateResult			Operation result 
* @return	Return 0 if succeeded, return error code if failed
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_PtzOperateDirect(DPSDK_INT32 iSessionID, 
														   DPSDK_PTZOPERATE_DIRECT_PARAM* pPtzOperateDirectParam, 
													       DPSDK_PTZOPERATE_RESULT* pPtzOperateResult);

/**
* @brief	Motorzied focus control
* @param	[IN]	iSessionID					User session ID
* @param	[IN]	pPtzOperateFocusParam		Motorized  focus control parameters
* @param	[OUT]	pPtzOperateResult			Operation result 
* @return	Return 0 if succeeded, return error code if failed
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_PtzOperateFocus(DPSDK_INT32 iSessionID, 
														  DPSDK_PTZOPERATE_FOCUS_PARAM* pPtzOperateFocusParam, 
														  DPSDK_PTZOPERATE_RESULT* pPtzOperateResult);
/**
* @brief	Control preset  whg tag: do not use now
* @param	[IN]	iSessionID					User session ID
* @param	[IN]	pPtzOperatePrePointParam	Preset control parameters 
* @param	[OUT]	pPtzOperateResult			Operation result 
* @return	Return 0 if succeeded, return error code if failed
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_PtzOperatePresetPoint(DPSDK_INT32 iSessionID, 
																DPSDK_PTZOPERATE_PRESETPOINT_PARAM* pPtzOperatePrePointParam, 
																DPSDK_PTZOPERATE_RESULT* pPtzOperateResult);
/**
* @brief	3D positioning
* @param	[IN]	iSessionID					User session ID
* @param	[IN]	pPtzOperateSitPositionParam	3D positioning parameters
* @param	[OUT]	pPtzOperateResult			Operation result 
* @return	Return 0 if succeeded, return error code if failed
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_PtzSitPosition(DPSDK_INT32 iSessionID, 
														 DPSDK_PTZOPERATE_SITPOSITION_PARAM* pPtzOperateSitPositionParam, 
														 DPSDK_PTZOPERATE_RESULT* pPtzOperateResult);

/**
* @brief	3D positioning parameters(For VSL)
* @param	[IN]	iSessionID					User session ID
* @param	[IN]	pPtzOperateSitPositionParam	3D positioning parameters
* @param	[OUT]	pPtzOperateResult			Operation result 
* @return	Return 0 if succeeded, return error code if failed
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_PtzSitPositionEx(DPSDK_INT32 iSessionID,
														   DPSDK_PTZOPERATE_SITPOSITION_EX_PARAM* pPtzOperateSitPositionParam,
														   DPSDK_PTZOPERATE_RESULT* pPtzOperateResult);

/**
* @brief	Lock, unlock
* @param	[IN]	iSessionID					User session ID
* @param	[IN]	pPtzOperateArrangePtzParam	Lock, unlock parameters
* @param	[OUT]	pPtzOperateResult			Operation result 
* @return	Return 0 if succeeded, return error code if failed
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_PtzArrangePtz(DPSDK_INT32 iSessionID, 
														DPSDK_PTZOPERATE_ARRANGEPTZ_PARAM* pPtzOperateArrangePtzParam, 
														DPSDK_PTZOPERATE_RESULT* pPtzOperateResult);
/**
* @brief	Alarm output control 
* @param	[IN]	iSessionID					User session ID
* @param	[IN]	pAlarmOutParam				Alarm output control parameters
* @param	[OUT]	pPtzOperateResult			Operation result 
* @return	Return 0 if succeeded, return error code if failed
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_AlarmActionOut(DPSDK_INT32 iSessionID, 
														 DPSDK_PTZOPERATE_ALARMOUT_PARAM* pAlarmOutParam, 
														 DPSDK_PTZOPERATE_RESULT* pPtzOperateResult);
/**
* @brief	Search tour 
* @param	[IN]	iSessionID					User session ID
* @param	[IN]	pChannelId					Channel Id
* @param	[OUT]	pCruiseInfoXml				Tour info xml stream
* @return	Return 0 if succeeded, return error code if failed
* @note		pCruiseInfoXml shall be released by DPSDK_ReleaseDataBuffer
*
*			pCruiseInfoXml format as follows 
*			<CruiseInfos>
*				<CruiseInfo>
*					<ChannelId>channelid</ChannelId>
*					<CruiseId>cruiseid</CruiseId>
*					<Data>data</Data>
*				</CruiseInfo>
*			</CruiseInfos>
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_CruiseGetCruise(DPSDK_INT32 iSessionID, 
														  DPSDK_CHAR* pChannelId, 
														  DPSDK_CHAR** pCruiseInfoXml);
/**
* @brief	Delete tour 
* @param	[IN]	iSessionID					User session ID
* @param	[IN]	pChannelId					Channel Id
* @param	[IN]	iCruiseId					Tour Id
* @return	Return 0 if succeeded, return error code if failed
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_CruiseDelCruise(DPSDK_INT32 iSessionID, 
														  DPSDK_CHAR* pChannelId, 
														  DPSDK_INT32 iCruiseId);
/**
* @brief	Save tour 
* @param	[IN]	iSessionID					User session ID
* @param	[IN]	pChannelId					Channel Id
* @param	[IN]	iCruiseId					Tour Id
* @param	[IN]	pData						Tour info
* @return	Return 0 if succeeded, return error code if failed
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_CruiseSaveCruise(DPSDK_INT32 iSessionID,
														   DPSDK_CHAR* pChannelId,
														   DPSDK_INT32 iCruiseId,
														   DPSDK_CHAR* pData);
/**
* @brief	Start manual record
* @param	[IN]	iSessionID					User session ID
* @param	[IN]	pStartRemoteRecordParam		Start manual record parameters
* @param	[OUT]	pStartRemoteRecordResult	Operation result 
* @return	Return 0 if succeeded, return error code if failed
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_StartRemoteRecord(DPSDK_INT32 iSessionID, 
															DPSDK_PTZOPERATE_STARTREMOTERECORD_PARAM* pStartRemoteRecordParam, 
															DPSDK_PTZOPERATE_REMOTERECORD_RESULT* pStartRemoteRecordResult);
/**
* @brief	Stop manual record
* @param	[IN]	iSessionID					User session ID
* @param	[IN]	pStopRemoteRecordParam		Stop manual record parameters
* @param	[OUT]	pStopRemoteRecordResult		Operation result 
* @return	Return 0 if succeeded, return error code if failed
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_StopRemoteRecord(DPSDK_INT32 iSessionID, 
														   DPSDK_PTZOPERATE_STOPREMOTERECORD_PARAM* pStopRemoteRecordParam, 
														   DPSDK_PTZOPERATE_REMOTERECORD_RESULT* pStopRemoteRecordResult);
/**
* @brief	Get PTZ position
* @param	[IN]	iSessionID					User session ID
* @param	[IN]	pChannelId					Channel Id
* @param	[OUT]	pPtzPosition				PTZ position
* @return	Return 0 if succeeded, return error code if failed
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_QueryPtzPostion(DPSDK_INT32 iSessionID, 
														  DPSDK_CHAR* pChannelId, 
														  DPSDK_PTZOPERATE_POSITIONINFO* pPtzPosition);
/**
* @brief	Set PTZ vision domain 
* @param	[IN]	iSessionID					User session ID
* @param	[IN]	pChannelId					Channel Id
* @param	[IN]	iViewDir					View domain start angle
* @return	Return 0 if succeeded, return error code if failed
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_SetChannelViewInfo(DPSDK_INT32 iSessionID, 
															 DPSDK_CHAR* pChannelId, 
															 DPSDK_INT32 iViewDir);
/**
* @brief	Set PTZ vision domain 
* @param	[IN]	iSessionID					User session ID
* @param	[IN]	pChannelId					Channel Id
* @param	[OUT]	pPtzChannelViewInfo			  PTZ view domain info
* @return	Return 0 if succeeded, return error code if failed
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetChannelViewInfo(DPSDK_INT32 iSessionID, 
															 DPSDK_CHAR* pChannelId, 
															 DPSDK_PTZOPERATE_CHANNELVIEWINFO* pPtzChannelViewInfo);
/**
* @brief	Search heat map 
* @param	[IN]	iSessionID					User session ID
* @param	[IN]	pGetHeatMapParam			Search heat map parameters
* @param	[OUT]	pHeatMapInfoXml				Search heat map, xml stream
* @return	Return 0 if succeeded, return error code if failed
* @note		pHeatMapInfoXml shall be released by DPSDK_ReleaseDataBuffer
*
*			pHeatMapInfoXml format is shown as below 
*			<HeatMapInfos>
*				<HeatMapInfo>
*					<Width>width</Width>
*					<Height>height</Height>
*					<Average>average</Average>
*					<PicDataLen>picdatalen</PicDataLen>
*					<PicData>picdata</PicData>
*				</HeatMapInfo>
*			</HeatMapInfos>
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_QueryHeatmap(DPSDK_INT32 iSessionID, 
													   DPSDK_PTZOPERATE_GETHEATMAP_PARAM* pGetHeatMapParam, 
													   DPSDK_CHAR** pHeatMapInfoXml);

/**
* @brief	Search preset 
* @param	[IN]	iSessionID					User session ID
* @param	[IN]	pChannelId					Channel Id
* @param	[OUT]	pPresetPointList			   Preset info list 
* @param	[IN]	uiBufLen					Buffer size 
* @return	Return 0 if succeeded, return error code if failed
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_PtzGetPresetPoints(DPSDK_INT32 iSessionID,
															 DPSDK_CHAR* pChannelId,
															 DPSDK_PTZ_PRESETPOINT_LIST* pPresetPointList,
															 DPSDK_UINT32 uiBufLen);

/**
* @brief	Get motorized focus status 
* @param	[IN]	iSessionID					User session ID
* @param	[IN]	pChannelId					Channel Id
* @param	[OUT]	pFocusStatusInfo			Motorized focus status  status
* @return	Return 0 if succeeded, return error code if failed
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_PtzGetFocusStatus(DPSDK_INT32 iSessionID, 
															DPSDK_CHAR* pChannelId, 
															DPSDK_FOCUSSTATUS_INFO* pFocusStatusInfo);
/* @brief	Panoramic  plus PTZ camera operations
* @param	[IN]	iSessionID					User session ID
* @param	[IN]	pSmartTrackParam			Panoramic plus PTZ camera  parameters
* @return	Return 0 if succeeded, return error code if failed
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_SmartTrackOperate(DPSDK_INT32 iSessionID, 
															DPSDK_SMARTTRACK_PARAM* pSmartTrackParam);

/**
* @brief	Get linage video channel info
* @param	[IN]	iSessionID					User session ID
* @param	[out]	ppLinkedResourceParam		Link video channel info
* @return       Return 0 if succeeded, return error code if failed
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetChannelLinkResource(DPSDK_INT32 iSessionID, 
																 DPSDK_LINKED_RESOURCE_PARAM** ppLinkedResourceParam);

/**
* @brief	Release linkage video channel memory 
* @param	[IN]pLinkedResourceParam			Linkage video channel data
* @return     Return 0 if succeeded, return error code if failed
* @note     pLinkedResourceParam is getting from DPSDK_GetChannelLinkResource
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_ReleaseLinkResourceBuffer(DPSDK_LINKED_RESOURCE_PARAM* pLinkedResourceParam);

/**
* @brief	Get person type info 
* @param	[IN]	iSessionID					User session ID
* @param	[OUT]	pInfoXml					Get info xml stream
* @return	Return 0 if succeeded, return error code if failed
* @note		pInfoXml shall be released by DPSDK_ReleaseDataBuffer
*
*			pInfoXml format is shown as below
*			<PersonTypeInfos>
*				<PersonTypeInfo>
*					<PersonTypeId>id</PersonTypeId>
*					<PersonTypeName>name</PersonTypeName>
*				</PersonTypeInfo>
*			</PersonTypeInfos>
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetPersonTypeInfo(DPSDK_INT32 iSessionID,
															DPSDK_CHAR** pInfoXml);

/**
* @brief	Get human face database info
* @param	[IN]	iSessionID					User session ID
* @param	[OUT]	pInfoXml					Get info xml stream
* @return	Return 0 if succeeded, return error code if failed
* @note		pInfoXml shall be released by DPSDK_ReleaseDataBuffer
*
*			pInfoXml format is shown as below
*			<FaceRepositoryInfos>
*				<FaceRepositoryInfo>
*					<Id>id</Id>
*					<Name>name</Name>
*					<Color>color</Color>
*					<Memo>memo</Memo>
*				</FaceRepositoryInfo>
*			</FaceRepositoryInfos>
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetFaceRepositoryInfo(DPSDK_INT32 iSessionID,
																DPSDK_CHAR** pInfoXml);

/**
* @brief	Register human face info 
* @param	[IN]	iSessionID					User session ID
* @param	[IN]	pPersonInfo					Person info 
* @param	[IN]	pFaceImageData				Human face image data£®base64£©
* @return	Return 0 if succeeded, return error code if failed
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_IVSFaceRegisterFace(DPSDK_INT32 iSessionID,
															  DPSDK_PERSON_INFO* pPersonInfo,
															  DPSDK_CHAR* pFaceImageData);
/**
* @brief	Extract human face info 
* @param	[IN]	iSessionID					User session ID
* @param	[IN]	pDeviceCode					Device ID 
* @param	[IN]	pFaceImageData				Human face image data£®base64£©
* @param	[OUT]	pInfoXml				Get info xml stream
* @param	[OUT]	pFaceImageInfo				Human face original image
* @return	Return 0 if succeeded, return error code if failed
* @note		pInfoXml shall be released by DPSDK_ReleaseDataBuffer
*
*			pInfoXml format is shown as below
*			<FaceImageInfos>
*				<FaceImageInfo>
*					<ImageId>id</ImageId>
*					<ImageUrl>url</ImageUrl>
*					<ImageCodX>x</ImageCodX>
*					<ImageCodY>y</ImageCodY>
*					<ImageHeight>height</ImageHeight>
*					<ImageWidth>width</ImageWidth>
*				</FaceImageInfo>
*			</FaceImageInfos>
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_DistinguishImageFaces(DPSDK_INT32 iSessionID,
																DPSDK_CHAR* pDeviceCode,
																DPSDK_CHAR* pFaceImageData,
																DPSDK_CHAR** pInfoXml,
																DPSDK_FACEIMAGE_INFO* pFaceImageInfo);

/**
* @brief	Get channel real-time temperature 
* @param	[IN] iSessionID			User session ID
* @param	[IN] pChannelId			Channel ID
* @param	[IN] iPosX				x coordinates
* @param	[IN] iPosY				y coordinates
* @param	[OUT] pTemperatureInfo	Temperature data
* @return	Return 0 if succeeded, return error code if failed
* @note		The tested position coordinated, value 0~8192
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetChannelTemperature(DPSDK_INT32 iSessionID,
																DPSDK_CHAR* pChannelId,
																DPSDK_INT32 iPosX,
																DPSDK_INT32 iPosY,
																DPSDK_TEMPERATURE_INFO* pTemperatureInfo);

DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_SetChannelCruisePreset(DPSDK_INT32 iSessionID, 
																 DPSDK_PTZ_CHANNL_CURISEPRESET_PARAM* pCurisePreset);

// DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetChannelCruisePreset(DPSDK_INT32 iSessionID, 
// 																 const DPSDK_CHAR* pChannelId, 
// 																 DPSDK_PTZ_CHANNL_CURISEPRESET_PARAM* pCurisePreset);
 DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetChannelCruisePreset(DPSDK_INT32 iSessionID, 
 																 const DPSDK_CHAR* pChannelId, 
																 DPSDK_DataCallback fDataCallBack,
																 DPSDK_VOID* pUserData);

//////////////////////////////////////////////////////////////////////////
/*
*	AR interface
*/
/* @brief	turn to point
* @param	[IN]	iSessionID					 Session ID
* @param	[IN]	pTurnParam					 Turn param
* @return	success return 0, others return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_ARTurnToPoint(DPSDK_INT32 iSessionID,
														DPSDK_AR_TURN_PARAM* pTurnParam);

/* @brief	delete label template
* @param	[IN]	iSessionID					 Session ID
* @param	[IN]	pDelParam					 Delete param
* @return	success return 0, others return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_ARDeleteLabelTemplate(DPSDK_INT32 iSessionID,
																DPSDK_AR_DELETE_LABEL_TEM_PARAM* pDelParam);

/* @brief	add label template
* @param	[IN]	iSessionID					 Session ID
* @param	[IN]	pAddParam					 Add param
* @return	success return 0, others return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_ARAddLabelTemplate(DPSDK_INT32 iSessionID,
															 DPSDK_AR_ADD_LABEL_TEM_PARAM* pAddParam);

/* @brief	get label template list
* @param	[IN] iSessionID					 Session ID
* @param	[IN] pGetParam					 Get param
* @param	[IN] fDataCallBack				 Data callback function, data type: DPSDK_DATA_AR_GET_LABEL_TEMPLATE
* @param	[IN] pUserData					 User data
* @return	success return 0, others return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_ARGetLabelTemplate(DPSDK_INT32 iSessionID,
															 DPSDK_AR_GET_LABEL_TEM_PARAM* pGetParam,
															 DPSDK_DataCallback fDataCallBack,
															 DPSDK_VOID* pUserData);

/* @brief	update label template
* @param	[IN]	iSessionID					 Session ID
* @param	[IN]	pUpdateParam				 Update param
* @return	success return 0, others return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_ARUpdateLabelTemplate(DPSDK_INT32 iSessionID,
																DPSDK_AR_UPDATE_LABEL_TEM_PARAM* pUpdateParam);

/* @brief	delete label
* @param	[IN]	iSessionID					 Session ID
* @param	[IN]	pDelParam					 Delete param
* @return	success return 0, others return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_ARDeleteLabel(DPSDK_INT32 iSessionID,
														DPSDK_AR_DELETE_LABEL_PARAM* pDelParam);

/* @brief	get label
* @param	[IN] iSessionID					 Session ID
* @param	[IN] pGetParam					 Get param
* @param	[IN] fDataCallBack				 Data callback function, data type: DPSDK_DATA_AR_GET_LABEL
* @param	[IN] pUserData					 User data
* @return	success return 0, others return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_ARGetLabel(DPSDK_INT32 iSessionID,
													 DPSDK_AR_GET_LABEL_PARAM* pGetParam,
													 DPSDK_DataCallback fDataCallBack,
													 DPSDK_VOID* pUserData);

/* @brief	set label
* @param	[IN] iSessionID					 Session ID
* @param	[IN] pSetParam					 Set param
* @return	success return 0, others return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_ARSetLabel(DPSDK_INT32 iSessionID,
													 DPSDK_AR_SET_LABEL_PARAM* pSetParam);

//////////////////////////////////////////////////////////////////////////
/*
*	Cloud synopsis interface 
*/

/**
* @brief	Get cloud synopsis intelligent analysis time slice list 
* @param	[IN] iSessionID			User session ID
* @param	[IN] pQueryParam		Query parameter
* @param	[OUT] pTimeSecList		Time slice list
* @param	[IN] uiBufLen			Buffer size 
* @return	Return 0 if succeeded, return error code if failed
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetAbstractAnalysisTimeSec(DPSDK_INT32 iSessionID,
																	 DPSDK_ABSTRACT_ANALYSIS_TIME_QUERY_PARAM* pQueryParam,
																	 DPSDK_ABSTRACT_ANALYSIS_TIME_SEC_LIST* pTimeSecList,
																	 DPSDK_UINT32 uiBufLen);

/**
* @brief	Get cloud synopsis detailed info 
* @param	[IN] iSessionID				User session ID
* @param	[IN] pQueryParam			Query parameter
* @param	[IN] pPageInfo				Page info
* @param	[OUT] uiTotal				Total page 
* @param	[OUT] pObjDetailList		Detailed info list 
* @return	Return 0 if succeeded, return error code if failed
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetAbstractDetailInfo(DPSDK_INT32 iSessionID,
																DPSDK_ABSTRACT_OBJ_DETAIL_INFO_QUERY_PARAM* pQueryParam,
																DPSDK_PAGE_INFO* pPageInfo,
																DPSDK_UINT32* uiTotal,
																DPSDK_ABSTRACT_OBJ_DETAIL_LIST* pObjDetailList);

/**
* @brief	Get cloud synopsis detailed info by time 
* @param	[IN] iSessionID				User session ID
* @param	[IN] pChannelID				Channel ID
* @param	[IN] tBeginTime				Start time 
* @param	[IN] fDataCallBack			Data sync call function. Refer to DPSDK_DATA_ABS_INFO_TIME for data type. 
* @param	[IN] pUserData				User data 
* @return	Return 0 if succeeded, return error code if failed
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetAbstractDetailInfoByTime(DPSDK_INT32 iSessionID,
																	  DPSDK_CHAR* pChannelID,
																	  DPSDK_TIMET tBeginTime,
																	  DPSDK_DataCallback fDataCallBack,
																	  DPSDK_VOID* pUserData);

/**
* @brief	Set synopsis image lifecycle 
* @param	[IN] iSessionID				User session ID
* @param	[IN] pLifeCycleList			Synopsis image lifecycle
* @param	[IN] uiBufLe				Buffer size 
* @return	Return 0 if succeeded, return error code if failed
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_SetAbstractLifeCycle(DPSDK_INT32 iSessionID,
															   DPSDK_ABSTRACT_LIFE_CYCLE_LIST* pLifeCycleList,
															   DPSDK_UINT32 uiBufLen);

/**
* @brief	Get synopsis image lifecycle
* @param	[IN] iSessionID				User session ID
* @param	[IN] fDataCallBack			Data sync call function. Refer to DPSDK_DATA_ABS_LIFE_CYCLE for data type
* @param	[IN] pUserData				User data 
* @return	Return 0 if succeeded, return error code if failed
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetAbstractLifeCycle(DPSDK_INT32 iSessionID,
															   DPSDK_DataCallback fDataCallBack,
															   DPSDK_VOID* pUserData);


/**
* @brief	Get cloud synopsis abstract info 
* @param	[IN] iSessionID				User session ID
* @param	[IN] pQueryParam			Query parameter 
* @param	[IN] fDataCallBack			Data sync call function. Refer to DPSDK_DATA_ABS_SUMMARY_INFO for data type
* @param	[IN] pUserData				User data 
* @return	Return 0 if succeeded, return error code if failed
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetAbstractSummaryInfo(DPSDK_INT32 iSessionID,
																 DPSDK_ABSTRACT_OBJ_SUMMMARY_INFO_QUERY_PARAM* pQueryParam,
																 DPSDK_DataCallback fDataCallBack,
																 DPSDK_VOID* pUserData);

/**
* @brief	Modify cloud synopsis info
* @param	[IN] iSessionID				User session ID
* @param	[IN] pModifyParam			Modify parameter
* @return	Return 0 if succeeded, return error code if failed
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_ModifyAbstractInfo(DPSDK_INT32 iSessionID,
															 DPSDK_ABSTRACT_OBJ_MODIFY_PARAM* pModifyParam);

/**
* @brief	Cloud synopsis real-time task enable/disable 
* @param	[IN] iSessionID				User session ID
* @param	[IN] pChannelId				Channel ID
* @param	[IN] bEnable				Enable button 
* @return	Return 0 if succeeded, return error code if failed
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_EnableAbstractRealTask(DPSDK_INT32 iSessionID,
																 DPSDK_CHAR* pChannelId,
																 DPSDK_BOOL bEnable);

//////////////////////////////////////////////////////////////////////////
/*
*	Alarm controller interface
*/
/**
* @brief	alarm controller control
* @param	[IN] iSessionID				User session ID
* @param	[IN] pOperateParam			Alarm controller operation parameter
* @return	Successful return 0, failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_OperateAlarmHost(DPSDK_INT32 iSessionID,
															DPSDK_OPERATE_ALARMHOST_PARAM* pOperateParam);

/**
* @brief	zone control
* @param	[IN] iSessionID				User session ID
* @param	[IN] pOperateParam			Zone  operation parameter
* @param	[OUT] pIdList				Subsystem id list
* @param	[IN] iBufLen				Buffer size

* @return	Successful return 0, failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_OperateDefenceAreas(DPSDK_INT32 iSessionID,
																DPSDK_DEFENCEAREAS_PARAM* pOperateParam,
																DPSDK_SUBSYSTEM_ID_LIST* pIdList,
																DPSDK_INT32 iBufLen);

/**
* @brief	subsystem  operation control
* @param	[IN] iSessionID				User session ID
* @param	[IN] pOperateParam			Subsystem  operation  parameter
* @param	[OUT] pIdList				Subsystem id list
* @param	[IN] iBufLen				Buffer size
* @return	Successful return 0, failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_OperateSubSystem(DPSDK_INT32 iSessionID,
															DPSDK_DEFENCEAREAS_PARAM* pOperateParam,
															DPSDK_SUBSYSTEM_ID_LIST* pIdList,
															DPSDK_INT32 iBufLen);

/**
* @brief	zone move in/move out Subsystem 
* @param	[IN] iSessionID				User session ID
* @param	[IN] pMoveParam				Zone move parameter			
* @return	Successful return 0, failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_MoveDefenceAreas(DPSDK_INT32 iSessionID,
															DPSDK_MOVE_DEFENCEAREAS_PARAM* pMoveParam);


/**
* @brief	search alarm controller info details
* @param	[IN] iSessionID				User session ID
* @param	[IN] pDeviceCode			Device code
* @param	[IN] fDataCallBack			Data sync call function, data type see DPSDK_DATA_QUERY_ALARMHOST
* @param	[IN] pUserData				User data
* @return	Successful return 0, failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_QueryAlarmHost(DPSDK_INT32 iSessionID,
															DPSDK_CHAR* pDeviceCode,
															DPSDK_DataCallback fDataCallBack,
															DPSDK_VOID* pUserData);

/**
* @brief	 search subsystem info
* @param	[IN] iSessionID				User session ID
* @param	[IN] pSubSystemId			Subsystem SN
* @param	[OUT] pSubSystemInfo		Subsystem info
* @param	[IN] iBufLen				Buffer size
* @return	Successful return 0, failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_QuerySubSystem(DPSDK_INT32 iSessionID,
															DPSDK_CHAR* pSubSystemId,
															DPSDK_SUBSYSTEM_INFO* pSubSystemInfo,
															DPSDK_INT32 iBufLen);

/**
* @brief	get alarm controller zone and subsystem 
* @param	[IN] iSessionID				User session ID
* @param	[IN] pDeviceCode			Device code
* @param	[IN] fDataCallBack			Data sync call function, data type see DPSDK_DATA_GET_SUBSYSTEM_ALARMHOST_INFO
* @param	[IN] pUserData				User data
* @return	Successful return 0, failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetSubSystem_AlarmHost(DPSDK_INT32 iSessionID,
																DPSDK_CHAR* pDeviceCode,
																DPSDK_DataCallback fDataCallBack,
																DPSDK_VOID* pUserData);

/**
* @brief	modify subsystem info
* @param	[IN] iSessionID				User session ID
* @param	[IN] pUpdateSubSystmeParm	Subsystem info parameter
* @return	Successful return 0, failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_UpdateSubSystem(DPSDK_INT32 iSessionID,
														DPSDK_SUBSYSTEM_PARM* pUpdateSubSystmeParm);

/**
* @brief	refresh alarm controller zone and subsystem info (load from device)
* @param	[IN] iSessionID				User session ID
* @param	[IN] pDeviceCode			Device code
* @return	Successful return 0, failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_RefAlarmHostSubInfo(DPSDK_INT32 iSessionID,
															DPSDK_CHAR* pDeviceCode);


/**
* @brief	alarm output control
* @param	[IN] iSessionID				User session ID
* @param	[IN] pParam					alarm output control parameter
* @return	Successful return 0, failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_AlarmOutControl(DPSDK_INT32 iSessionID,
														DPSDK_ALARMOUT_CONTROL* pParam);

/**
* @brief	config siren status
* @param	[IN] iSessionID				User session ID
* @param	[IN] pParam					siren status parameter
* @return	Successful return 0, failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_AlarmStatus(DPSDK_INT32 iSessionID,
													DPSDK_ALARM_STATUS* pParam);

//////////////////////////////////////////////////////////////////////////
/*
*	A&C interface
*/


/**
* @brief	get A&C log
* @param	[IN] iSessionID				User session ID
* @param	[IN] pParam					A&C log info
* @param	[IN] fDataCallBack			Data sync call function, data type see DPSDK_DATA_GET_ACCESSCONTROL_LOG
* @param	[IN] pUserData				User data
* @return	Successful return 0£¨failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetAccessControlLog(DPSDK_INT32 iSessionID,
															DPSDK_ACCESSCONTROL_LOG* pParam,
															DPSDK_DataCallback fDataCallBack,
															DPSDK_VOID* pUserData);

/**
* @brief	export A&C log
* @param	[IN] iSessionID				User session ID
* @param	[IN] pExportParam			Export parameter
* @param	[OUT] pFileUrl				File URL
* @param	[IN] uiBufLen				Buffer size
* @return	Successful return 0, failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_ExportAccessLog(DPSDK_INT32 iSessionID,
														DPSDK_EXPORT_LOG_PARAM* pExportParam,
														DPSDK_CHAR* pFileUrl,
														DPSDK_UINT32 uiBufLen);


/**
* @brief	get A&C detail
* @param	[IN] iSessionID				User session ID
* @param	[IN] pDevCode				Device code
* @param	[OUT] pAccessDetail			A&C detail
* @return	Successful return 0, failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetAccessDoorDetail(DPSDK_INT32 iSessionID,
															DPSDK_DEVICE_CODE* pDevCode,
															DPSDK_ACCESS_DOOR_DETAIL* pAccessDetail);

/**
* @brief	door alarm input/output basic config
* @param	[IN] iSessionID				User session ID
* @param	[IN] pParam					Door alarm input/output basic config info
* @param	[IN] iBufLen				Buffer size
* @return	Successful return 0£¨failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_DoorAlarmInOut(DPSDK_INT32 iSessionID,
														DPSDK_DOOR_ALARM_INOUT* pParam,
														DPSDK_INT32 iBufLen);

/**
* @brief	get door alarm input/output basic config
* @param	[IN] iSessionID				User session ID
* @param	[IN] pDeviceId				Device id
* @param	[IN] fDataCallBack			Data sync call function, data type see DPSDK_DATA_GET_DOOR_ALARM_INOUT
* @param	[IN] pUserData				User data
* @return	Successful return 0£¨failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetDoorAlarmInOut(DPSDK_INT32 iSessionID,
															DPSDK_CHAR* pDeviceId,
															DPSDK_DataCallback fDataCallBack,
															DPSDK_VOID* pUserData);

/**
* @brief	door basic config 
* @param	[IN] iSessionID				User session ID
* @param	[IN] pParam					door basic info
* @return	Successful return 0£¨failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_DoorBaseConfigure(DPSDK_INT32 iSessionID,
															DPSDK_DOOR_INFO* pParam);

/**
* @brief	get door basic config 
* @param	[IN] iSessionID				User session ID
* @param	[IN] pChannelId				Channel id
* @param	[IN] fDataCallBack			Data sync call function, data type see DPSDK_DATA_GET_DOORBASE_CONFIGURE
* @param	[IN] pUserData				User data* @return	Successful return 0£¨failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetDoorBaseConfigure(DPSDK_INT32 iSessionID,
																DPSDK_CHAR* pChannelId,
																DPSDK_DataCallback fDataCallBack,
																DPSDK_VOID* pUserData);

/**
* @brief	 door basic config copy
* @param	[IN] iSessionID				User session ID
* @param	[IN] pParam					Copy door basic config  parameter
* @return	Successful return 0£¨failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_CoypDoorBaseConfigure(DPSDK_INT32 iSessionID,
																DPSDK_DOOR_BASE_CONFIGURE* pParam);

/**
* @brief	single door unlock/locl
* @param	[IN] iSessionID				User session ID
* @param	[IN] pChannelId				 door channel id
* @param	[IN] iStatus				 door status£ª0 lock£¨1 unlock
* @return	Successful return 0£¨failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_ControlDoor(DPSDK_INT32 iSessionID,
													DPSDK_CHAR* pChannelId,
													DPSDK_INT32 iStatus);

/**
* @brief	config global unlock channel
* @param	[IN] iSessionID				User session ID
* @param	[IN] pChannelList			Unlock channel type
* @param	[IN] iBufLen				Buffer size
* @return	Successful return 0£¨failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GlobalDoorChannel(DPSDK_INT32 iSessionID,
															DPSDK_CHANNEL_LIST* pChannelList,
															DPSDK_INT32 iBufLen);

/**
* @brief	get certain door reader property
* @param	[IN] iSessionID				User session ID
* @param	[IN] pDevCode				Device code
* @param	[IN] fDataCallBack			Data sync call function, data type see DPSDK_DATA_GET_DOOR_READER
* @param	[IN] pUserData				User data
* @return	Successful return 0£¨failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetDoorReader(DPSDK_INT32 iSessionID,
														DPSDK_DEVICE_CODE* pDevCode,
														DPSDK_DataCallback fDataCallBack,
														DPSDK_VOID* pUserData);
/**
* @brief	 get device controller
* @param	[IN] iSessionID				User session ID
* @param	[IN] pDevCodeList			Device code list
* @param	[IN] iBufLen				Buffer size
* @param	[IN] fDataCallBack			Data sync call function, data type see DPSDK_DATA_GET_DOOR_READER
* @param	[IN] pUserData				User data
* @return	Successful return 0£¨failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetDoorController(DPSDK_INT32 iSessionID,
															DPSDK_DEVICE_CODE_LIST* pDevCodeList,
															DPSDK_INT32 iBufLen,
															DPSDK_DataCallback fDataCallBack,
															DPSDK_VOID* pUserData);

/**
* @brief	global unlock/lock
* @param	[IN] iSessionID				User session ID
* @param	[IN] iStatus				 door status£ª0 global lock£¨1 global unlock
* @return	Successful return 0£¨failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GlobalControlDoor(DPSDK_INT32 iSessionID,
															DPSDK_INT32 iStatus);

/**
* @brief	 get global unlock/lock status 
* @param	[IN] iSessionID				User session ID
* @param	[OUT] pStatus				 door  status 
* @return	Successful return 0£¨failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetGlobalControlDoor(DPSDK_INT32 iSessionID,
																DPSDK_DOOR_GLOBAL_CONTROL_STATUE* pStatus);

/**
* @brief	 get A&C  status mode
* @param	[IN] iSessionID				User session ID
* @param	[IN] pChnIdList				Channel id list
* @param	[IN] iBufLen				Buffer size
* @param	[IN] fDataCallBack			Data sync call function, data type see DPSDK_DATA_GET_ACCESS_CONTROL_STATUS
* @param	[IN] pUserData				User data
* @return	Successful return 0£¨failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetAccessControlStatus(DPSDK_INT32 iSessionID,
																DPSDK_CHANNEL_LIST* pChnIdList,
																DPSDK_INT32 iBufLen,
																DPSDK_DataCallback fDataCallBack,
																DPSDK_VOID* pUserData);

/**
* @brief	add door group
* @param	[IN] iSessionID				User session ID
* @param	[IN] pParam					Request parameter
* @param	[OUT] pDoorGroupId			Door group id
* @param	[IN] iBufLen				Buffer size
* @return	Successful return 0£¨failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_AddDoorGroup(DPSDK_INT32 iSessionID,
													DPSDK_DOOR_GROUP_INFO* pParam,
													DPSDK_CHAR* pDoorGroupId,
													DPSDK_INT32 iBufLen);

/**
* @brief	add door group and user binding
* @param	[IN] iSessionID				User session ID
* @param	[IN] pPersonIdList			»Àid list
* @param	[IN] pDoorGroupId			Door group id
* @param	[IN] iBufLen				Buffer size
* @param	[OUT] pResult				Add door group and user binding result
* @return	Successful return 0£¨failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_AddDoorGroupPerson(DPSDK_INT32 iSessionID,
															DPSDK_PERSON_LIST* pPersonIdList,
															DPSDK_CHAR* pDoorGroupId,
															DPSDK_INT32 iBufLen,
															DPSDK_ADD_DOORGROUP_PERSON* pResult);


/**
* @brief	batch delete door group config
* @param	[IN] iSessionID				User session ID
* @param	[IN] pParam					Door group info parameter
* @param	[IN] iBufLen				Buffer size
* @return	Successful return 0£¨failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_DeleteDoorGroup(DPSDK_INT32 iSessionID,
														DPSDK_DOORGROUP_LIST* pParam,
														DPSDK_INT32 iBufLen);

/**
* @brief	 get door group config
* @param	[IN] iSessionID				User session ID
* @param	[IN] pDoorGroupId			Door group id
* @param	[IN] fDataCallBack			Data sync call function, data type see DPSDK_DATE_GET_DOORGROUP
* @param	[IN] pUserData				User data
* @return	Successful return 0£¨failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetDoorGroup(DPSDK_INT32 iSessionID,
													DPSDK_CHAR* pDoorGroupId,
													DPSDK_DataCallback fDataCallBack,
													DPSDK_VOID* pUserData);

/**
* @brief	 get all group list
* @param	[IN] iSessionID				User session ID
* @param	[IN] pPageInfo				Page info
* @param	[IN] fDataCallBack			Data sync call function, data type see DPSDK_DATA_GET_DOORGROUP_LIST
* @param	[IN] pUserData				User data
* @return	Successful return 0£¨failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetDoorGroupList(DPSDK_INT32 iSessionID,
													DPSDK_PAGE_INFO* pPageInfo,
													DPSDK_DataCallback fDataCallBack,
													DPSDK_VOID* pUserData);

/**
* @brief	 get door group and user binding
* @param	[IN] iSessionID				User session ID
* @param	[IN] pDoorGroupId			Door group id
* @param	[IN] fDataCallBack			Data sync call function, data type see DPSDK_DATA_GET_DOORGROUP_PERSON
* @param	[IN] pUserData				User data
* @return	Successful return 0£¨failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetDoorGroupPerson(DPSDK_INT32 iSessionID,
															DPSDK_CHAR* pDoorGroupId,
															DPSDK_DataCallback fDataCallBack,
															DPSDK_VOID* pUserData);

/**
* @brief	update door group config
* @param	[IN] iSessionID				User session ID
* @param	[IN] pParam					Request parameter
* @param	[IN] iBufLen				Buffer size
* @return	Successful return 0£¨failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_UpdateDoorGroup(DPSDK_INT32 iSessionID,
														DPSDK_DOOR_GROUP_INFO* pParam,
														DPSDK_INT32 iBufLen);

/**
* @brief	update door group and user binding
* @param	[IN] iSessionID				User session ID
* @param	[IN] pPersonIdList			Person ID list
* @param	[IN] pDoorGroupId			door group id
* @param	[IN] iBufLen				Buffer size
* @return	Successful return 0£¨failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_DoorGroupUpdatePerson(DPSDK_INT32 iSessionID,
															DPSDK_PERSON_LIST* pPersonIdList,
															DPSDK_CHAR* pDoorGroupId,
															DPSDK_INT32 iBufLen);

/**
* @brief	Save Channels
* @param	[IN] iSessionID				User session ID
* @param	[IN] pSaveParam				Save Param
* @return	Successful return 0£¨failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_AttendanceSaveChannels(DPSDK_INT32 iSessionID,
																DPSDK_ATTENDANCE_CHANNEL_PARAM* pSaveParam);

/**
* @brief	Del Channels
* @param	[IN] iSessionID				User session ID
* @param	[IN] pDelParam				del Param
* @return	Successful return 0£¨failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_AttendanceDelChannels(DPSDK_INT32 iSessionID,
															  DPSDK_ATTENDANCE_CHANNEL_PARAM* pDelParam);

/**
* @brief	Get Channels
* @param	[IN] iSessionID				User session ID
* @param	[IN] pPageInfo				Page info
* @param	[IN] fDataCallBack			Data sync call function, data type see DPSDK_DATA_GET_ATTENDANCE_CHANNELS_LIST
* @param	[IN] pUserData				User data
* @return	Successful return 0£¨failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_AttendanceGetChannels(DPSDK_INT32 iSessionID,
																DPSDK_PAGE_INFO* pPageInfo,
														        DPSDK_DataCallback fDataCallBack,
														        DPSDK_VOID* pUserData);

/**
* @brief	Get StatisRule
* @param	[IN] iSessionID				User session ID
* @param	[OUT] pStatisRule			Rule
* @return	Successful return 0£¨failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_AttendanceGetStatisRule(DPSDK_INT32 iSessionID,
																  DPSDK_INT32* pStatisRule);

/**
* @brief	Save StatisRule
* @param	[IN] iSessionID				User session ID
* @param	[IN] iStatisRule			Rule
* @return	Successful return 0£¨failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_AttendanceSaveStatisRule(DPSDK_INT32 iSessionID,
																   DPSDK_INT32 iStatisRule);

/**
* @brief	add TimeInterval
* @param	[IN] iSessionID				User session ID
* @param	[IN] pAddParam			    Add Param
* @return	Successful return 0£¨failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_AttendanceAddTimeInterval(DPSDK_INT32 iSessionID,
																    DPSDK_TIMEINTERVAL_DATA* pAddParam);

/**
* @brief	del TimeInterval
* @param	[IN] iSessionID				User session ID
* @param	[IN] pDelParam			    del Param
* @param	[IN] fDataCallBack			Data sync call function, data type see DPSDK_DATA_DEL_ATTENDANCE_TIMEINTERVAL_INFO
* @param	[IN] pUserData				User data
* @return	Successful return 0£¨failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_AttendanceDelTimeInterval(DPSDK_INT32 iSessionID,
																	DPSDK_DEL_TIMEINTERVAL_PARAM* pDelParam,
																	DPSDK_DataCallback fDataCallBack,
																	DPSDK_VOID* pUserData);

/**
* @brief	Get TimeInterval
* @param	[IN] iSessionID				User session ID
* @param	[IN] pIntervalId			Interval Id
* @param	[IN] fDataCallBack			Data sync call function, data type see DPSDK_DATA_GET_ATTENDANCE_TIMEINTERVAL_INFO
* @param	[IN] pUserData				User data
* @return	Successful return 0£¨failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_AttendanceGetTimeInterval(DPSDK_INT32 iSessionID,
																	DPSDK_CHAR* pIntervalId,
																    DPSDK_DataCallback fDataCallBack,
																    DPSDK_VOID* pUserData);

/**
* @brief	Batch Get TimeIntervals
* @param	[IN] iSessionID				User session ID
* @param	[IN] pIntervalIdList		IntervalId List
* @param	[IN] fDataCallBack			Data sync call function, data type see DPSDK_DATA_GET_ATTENDANCE_BATCH_TIMEINTERVAL_INFO
* @param	[IN] pUserData				User data
* @return	Successful return 0£¨failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_AttendanceBatchGetTimeIntervals(DPSDK_INT32 iSessionID,
																	 DPSDK_TIMEINTERVAL_ID_LIST* pIntervalIdList,
																	 DPSDK_DataCallback fDataCallBack,
																	 DPSDK_VOID* pUserData);


/**
* @brief	Get TimeIntervals
* @param	[IN] iSessionID				User session ID
* @param	[IN] pPageInfo				Page info
* @param	[IN] fDataCallBack			Data sync call function, data type see DPSDK_DATA_GET_ATTENDANCE_TIMEINTERVALS_INFO
* @param	[IN] pUserData				User data
* @return	Successful return 0£¨failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_AttendanceGetTimeIntervals(DPSDK_INT32 iSessionID,
																	DPSDK_PAGE_INFO* pPageInfo,
																	DPSDK_DataCallback fDataCallBack,
																	DPSDK_VOID* pUserData);

/**
* @brief	update TimeInterval
* @param	[IN] iSessionID				User session ID
* @param	[IN] pIntervalInfo			Interval Info
* @return	Successful return 0£¨failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_AttendanceUpdateTimeInterval(DPSDK_INT32 iSessionID,
																	   DPSDK_TIMEINTERVAL_DATA* pIntervalInfo);

/**
* @brief	add order
* @param	[IN] iSessionID				User session ID
* @param	[IN] pAddParam			    Add Param
* @return	Successful return 0£¨failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_AttendanceAddOrder(DPSDK_INT32 iSessionID,
															 DPSDK_ATTENDANCEORDER_INFO* pAddParam);

/**
* @brief	del order
* @param	[IN] iSessionID				User session ID
* @param	[IN] pDelIdList			    del Id List
* @param	[IN] fDataCallBack			Data sync call function, data type see DPSDK_DATA_DEL_ATTENDANCE_ORDERS_INFO 
* @param	[IN] pUserData				User data
* @return	Successful return 0£¨failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_AttendanceDelOrder(DPSDK_INT32 iSessionID,
															 DPSDK_ORDER_ID_LIST* pDelIdList,
															 DPSDK_DataCallback fDataCallBack,
															 DPSDK_VOID* pUserData);

/**
* @brief	Get order
* @param	[IN] iSessionID				User session ID
* @param	[IN] pOrderId			    Order Id
* @param	[IN] fDataCallBack			Data sync call function, data type see DPSDK_DATA_GET_ATTENDANCE_ORDER_INFO 
* @param	[IN] pUserData				User data
* @return	Successful return 0£¨failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_AttendanceGetOrder(DPSDK_INT32 iSessionID,
															DPSDK_CHAR* pOrderId,
															DPSDK_DataCallback fDataCallBack,
															DPSDK_VOID* pUserData);

/**
* @brief	Batch Get orders
* @param	[IN] iSessionID				User session ID
* @param	[IN] pOrderIdList			OrderId List
* @param	[IN] fDataCallBack			Data sync call function, data type see DPSDK_DATA_GET_ATTENDANCE_BATCH_ORDER_INFO 
* @param	[IN] pUserData				User data
* @return	Successful return 0£¨failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_AttendanceBatchGetOrders(DPSDK_INT32 iSessionID,
															  DPSDK_ORDER_ID_LIST* pOrderIdList,
															  DPSDK_DataCallback fDataCallBack,
															  DPSDK_VOID* pUserData);


/**
* @brief	Get orders
* @param	[IN] iSessionID				User session ID
* @param	[IN] pPageInfo				Page info
* @param	[IN] fDataCallBack			Data sync call function, data type see DPSDK_DATA_GET_ATTENDANCE_ORDERS_INFO 
* @param	[IN] pUserData				User data
* @return	Successful return 0£¨failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_AttendanceGetOrders(DPSDK_INT32 iSessionID,
															 DPSDK_PAGE_INFO* pPageInfo,
															 DPSDK_DataCallback fDataCallBack,
															 DPSDK_VOID* pUserData);

/**
* @brief	update order
* @param	[IN] iSessionID				User session ID
* @param	[IN] pOrderInfo			Order Info
* @return	Successful return 0£¨failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_AttendanceUpdateOrder(DPSDK_INT32 iSessionID,
																DPSDK_ATTENDANCEORDER_INFO* pOrderInfo);

/**
* @brief	update Schedule
* @param	[IN] iSessionID				User session ID
* @param	[IN] pUpdateParam			    Update Param
* @return	Successful return 0£¨failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_AttendanceUpdateSchedule(DPSDK_INT32 iSessionID,
																 DPSDK_UPDATE_PERSON_SCHEDULE* pUpdateParam);

/**
* @brief	update Dept Schedule
* @param	[IN] iSessionID				User session ID
* @param	[IN] pUpdateParam			    Update Param
* @return	Successful return 0£¨failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_AttendanceUpdateDeptSchedule(DPSDK_INT32 iSessionID,
																   DPSDK_UPDATE_DEPT_SCHEDULE* pUpdateParam);

/**
* @brief	Get Schedule
* @param	[IN] iSessionID				User session ID
* @param	[IN] pPersonId			    Person Id
* @param	[IN] fDataCallBack			Data sync call function, data type see DPSDK_DATA_GET_ATTENDANCE_SCHEDULE_INFO 
* @param	[IN] pUserData				User data
* @return	Successful return 0£¨failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_AttendanceGetSchedule(DPSDK_INT32 iSessionID,
															 DPSDK_CHAR* pPersonId,
															 DPSDK_DataCallback fDataCallBack,
															 DPSDK_VOID* pUserData);

/**
* @brief	Get Dept Schedule
* @param	[IN] iSessionID				User session ID
* @param	[IN] pDeptId			    dept Id
* @param	[IN] fDataCallBack			Data sync call function, data type see DPSDK_DATA_GET_ATTENDANCE_SCHEDULE_INFO 
* @param	[IN] pUserData				User data
* @return	Successful return 0£¨failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_AttendanceGetDeptSchedule(DPSDK_INT32 iSessionID,
																DPSDK_CHAR* pDeptId,
																DPSDK_DataCallback fDataCallBack,
																DPSDK_VOID* pUserData);


/**
* @brief	Import Persons
* @param	[IN] iSessionID				User Session ID
* @param	[IN] pPersonParam			Import Param
* @param	[IN] fDataCallBack			Data sync call function, data type see DPSDK_DATA_IMPORT_PERSONS
* @param	[IN] pUserData				User data
* @return	Return 0 if succeeded, return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_ImportPersons(DPSDK_INT32 iSessionID,
														DSPDK_IMPORTPERSON_PARAM* pPersonParam,
														DPSDK_DataCallback fDataCallBack,
														DPSDK_VOID* pUserData);

/**
* @brief	Export Persons
* @param	[IN] iSessionID				User Session ID
* @param	[IN] pPersonParam			Export Param
* @param	[OUT] pPersonResult			Export Result
* @return	Return 0 if succeeded, return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_ExportPersons(DPSDK_INT32 iSessionID,
														DSPDK_EXPORTPERSON_PARAM* pPersonParam,
														DSPDK_EXPORTPERSON_RESULT* pPersonResult);

/**
* @brief	Get Template Url
* @param	[IN] iSessionID				User Session ID
* @param	[IN] pParam				Get Template Url Param
* @param	[OUT] pResult			Get Template Url Result
* @return	Return 0 if succeeded, return error code if failed.
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetTemplateUrl(DPSDK_INT32 iSessionID,
														 DSPDK_TEMPLATEURL_PARAM* pParam,
														 DSPDK_TEMPLATEURL_RESULT* pResult);

/**
* @brief	Clear Person Face Date
* @param	[IN] iSessionID		SessionID
* @param	[IN] pDevId			Device id
* @return	success 0£¨failed error number
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_ClearPersonFaceDate(DPSDK_INT32 iSessionID,
															  DPSDK_CHAR* pDevId);


/**
* @brief	add organization
* @param	[IN] iSessionID				User session ID
* @param	[IN] pParam					Organization parameter
* @param	[OUT] pDepartmentCode		Organization code
* @param	[IN] iBufLen				Buffer size
* @return	Successful return 0, failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_AddDepartment(DPSDK_INT32 iSessionID,
														DPSDK_ADD_DEPARTMENT* pParam,
														DPSDK_CHAR* pDepartmentCode,
														DPSDK_INT32 iBufLen);

/**
* @brief	delete organization
* @param	[IN] iSessionID				User session ID
* @param	[IN] pDepartmentCode		Organization code
* @return	Successful return 0, failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_DelDepartment(DPSDK_INT32 iSessionID,
														DPSDK_CHAR* pDepartmentCode);

/**
* @brief	update organization
* @param	[IN] iSessionID				User session ID
* @param	[IN] pUpdateDepartment		Update parameter
* @return	Successful return 0£¨failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_UpdateDepartment(DPSDK_INT32 iSessionID,
														DPSDK_UPDATE_DEPARTMENT* pUpdateDepartment);


/**
* @brief	add remote verification config
* @param	[IN] iSessionID					User session ID
* @param	[IN] pParam						Remote verification parameter
* @param	[OUT] pRemoteVerificationId		Remote verification id
* @param	[IN] iBufLen					Buffer size
* @return	Successful return 0£¨failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_AddRemoteVerification(DPSDK_INT32 iSessionID,
																DPSDK_REMOTE_VERIFICATION_INFO* pParam,
																DPSDK_CHAR* pRemoteVerificationId,
																DPSDK_INT32 iBufLen);

/**
* @brief	batch delete remote verification
* @param	[IN] iSessionID					User session ID
* @param	[IN] pDelParam					Delete remote verification parameter
* @param	[IN] iBufLen					Buffer size
* @return	Successful return 0£¨failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_DelRemoteVerification(DPSDK_INT32 iSessionID,
																DPSDK_DEL_REMOTE_VERIFICATION* pDelParam,
																DPSDK_INT32 iBufLen,
																DPSDK_DataCallback fDataCallBack,
																DPSDK_VOID* pUserData);

/**
* @brief	ignore remote check
* @param	[IN] iSessionID					User session ID
* @param	[IN] pIgnoreParam				Ignore remote check parameter
* @return	Successful return 0£¨failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_IgnoreRemoteCheck(DPSDK_INT32 iSessionID, 
															DPSDK_IGNORE_REMOTE_CHECK* pIgnoreParam);

/**
* @brief	 get remote verification
* @param	[IN] iSessionID					User session ID
* @param	[IN] pRemoteVerificationId		Remote verification id
* @param	[OUT] pRemoteVerificationInfo	Remote verification info
* @return	Successful return 0£¨failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetRemoteVerification(DPSDK_INT32 iSessionID,
																DPSDK_CHAR* pRemoteVerificationId,
																DPSDK_REMOTE_VERIFICATION_INFO* pRemoteVerificationInfo);

/**
* @brief	 get remote verification door  list
* @param	[IN] iSessionID					User session ID
* @param	[IN] pPageInfo					Page info
* @param	[IN] fDataCallBack				Data sync call function, data type see DPSDK_DATA_GET_REMOTE_VERIFICATION_LIST
* @param	[IN] pUserData					User data
* @return	Successful return 0£¨failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetRemoteVerificationList(DPSDK_INT32 iSessionID,
																	DPSDK_PAGE_INFO* pPageInfo,
																	DPSDK_DataCallback fDataCallBack,
																	DPSDK_VOID* pUserData);

/**
* @brief	update remote verification
* @param	[IN] iSessionID					User session ID
* @param	[IN] pUpdateParam				Update remote verification parameter
* @return	Successful return 0£¨failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_UpdateRemoteVerification(DPSDK_INT32 iSessionID,
																DPSDK_REMOTE_VERIFICATION_INFO* pUpdateParam);

/**
* @brief	batch delete door card config
* @param	[IN] iSessionID					User session ID
* @param	[IN] pDelFirstCard				Batch delete door card parameter
* @param	[IN] iBufLen					Buffer size
* @return	Successful return 0£¨failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_DelFirstCard(DPSDK_INT32 iSessionID,
														DPSDK_DEL_FIRST_CARD* pDelFirstCard,
														DPSDK_INT32 iBufLen,
														DPSDK_DataCallback fDataCallBack,
														DPSDK_VOID* pUserData);

/**
* @brief	 get first card unlock config
* @param	[IN] iSessionID					User session ID
* @param	[IN] pFirstCardId				first card unlock id
* @param	[IN] fDataCallBack				Data sync call function, data type see DPSDK_DATA_GET_FIRST_CARD
* @param	[IN] pUserData					User data
* @return	Successful return 0£¨failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetFirstCard(DPSDK_INT32 iSessionID,
														DPSDK_CHAR* pFirstCardId,
														DPSDK_DataCallback fDataCallBack,
														DPSDK_VOID* pUserData);

/**
* @brief	 get all door list
* @param	[IN] iSessionID					User session ID
* @param	[IN] pKeyWord					Search keyword
* @param	[IN] pPageInfo					Page info
* @param	[IN] fDataCallBack				Data sync call function, data type see DPSDK_DATA_GET_FIRST_CARD_LIST
* @param	[IN] pUserData					User data
* @return	Successful return 0£¨failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetFirstCardList(DPSDK_INT32 iSessionID,
															DPSDK_CHAR* pKeyWord,
															DPSDK_PAGE_INFO* pPageInfo,
															DPSDK_DataCallback fDataCallBack,
															DPSDK_VOID* pUserData);

/**
* @brief	save first card unlock
* @param	[IN] iSessionID					User session ID
* @param	[IN] pParam						Save first card unlock parameter
* @param	[OUT] pFirstCardId				First card id
* @param	[IN] iBufLen					Buffer size
* @return	Successful return 0£¨failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_SaveFirstCard(DPSDK_INT32 iSessionID,
														DPSDK_FIRST_CARD_INFO* pParam,
														DPSDK_CHAR* pFirstCardId,
														DPSDK_INT32 iBufLen);

/**
* @brief	update door basic config include enable
* @param	[IN] iSessionID					User session ID
* @param	[IN] pParam						Update parameter
* @return	Successful return 0£¨failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_UpdateFirstCard(DPSDK_INT32 iSessionID,
														DPSDK_FIRST_CARD_INFO* pParam);

/**
* @brief	add multiple card unlock config
* @param	[IN] iSessionID					User session ID
* @param	[IN] pAddMultiCard				Add multiple card unlock parameter
* @param	[OUT] pMultiCardId				Multiple card unlock id
* @param	[IN] iBufLen					Buffer size
* @return	Successful return 0£¨failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_AddMultiCard(DPSDK_INT32 iSessionID,
														DPSDK_MULTI_CARD_INFO* pAddMultiCard,
														DPSDK_CHAR* pMultiCardId,
														DPSDK_INT32 iBufLen);

/**
* @brief	delete multiple card unlock config
* @param	[IN] iSessionID					User session ID
* @param	[IN] pDelMultiCard				Delete multiple card unlock parameter
* @param	[IN] iBufLen					Buffer size
* @return	Successful return 0£¨failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_DelMultiCard(DPSDK_INT32 iSessionID,
													DPSDK_DEL_MULTI_CARD* pDelMultiCard,
													DPSDK_INT32 iBufLen,
													DPSDK_DataCallback fDataCallBack,
													DPSDK_VOID* pUserData);

/**
* @brief	 get multiple card unlock config
* @param	[IN] iSessionID					User session ID
* @param	[IN] pMultCardId				Multiple card unlock id
* @param	[IN] fDataCallBack				Data sync call function, data type see DPSDK_DATA_GET_MULTI_CARD
* @param	[IN] pUserData					User data
* @return	Successful return 0£¨failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetMultiCard(DPSDK_INT32 iSessionID,
														DPSDK_CHAR* pMultCardId,
														DPSDK_DataCallback fDataCallBack,
														DPSDK_VOID* pUserData);

/**
* @brief	 get  door  list
* @param	[IN] iSessionID					User session ID
* @param	[IN] pPageInfo					Page info
* @param	[IN] fDataCallBack				Data sync call function, data type see DPSDK_DATA_GET_MULTI_CARD_LIST
* @param	[IN] pUserData					User data
* @return	Successful return 0£¨failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetMultiCardList(DPSDK_INT32 iSessionID,
															DPSDK_PAGE_INFO* pPageInfo,
															DPSDK_DataCallback fDataCallBack,
															DPSDK_VOID* pUserData);

/**
* @brief	update multiple card unlock config
* @param	[IN] iSessionID					User session ID
* @param	[IN] pSaveMultiCard				Update multiple card unlock config  parameter
* @return	Successful return 0£¨failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_UpdateMultiCard(DPSDK_INT32 iSessionID,
														DPSDK_MULTI_CARD_INFO* pUpdateMultiCard);

/**
* @brief	add inter-lock
* @param	[IN] iSessionID					User session ID
* @param	[IN] pAddInterDoorLock			Inter-lock parameter
* @param	[OUT] pInterDoorLockId			Inter-lock id
* @param	[IN] iBufLen					Buffer size
* @return	Successful return 0£¨failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_AddInterDoorLock(DPSDK_INT32 iSessionID,
														DPSDK_INTERDOOR_LOCK_INFO* pAddInterDoorLock,
														DPSDK_CHAR* pInterDoorLockId,
														DPSDK_INT32 iBufLen);

/**
* @brief	batch delete inter-lock config
* @param	[IN] iSessionID					User session ID
* @param	[IN] pInterDoorLock				Batch delete inter-lock parameter
* @param	[IN] iBufLen					Buffer size
* @return	Successful return 0£¨failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_DelInterDoorLock(DPSDK_INT32 iSessionID,
															DPSDK_DEL_INTER_DOOR_LOCK* pInterDoorLock,
															DPSDK_INT32 iBufLen,
															DPSDK_DataCallback fDataCallBack,
															DPSDK_VOID* pUserData); 


/**
* @brief	 get inter-lock list
* @param	[IN] iSessionID				User session ID
* @param	[IN] pPageInfo				Page info
* @param	[IN] fDataCallBack			Data sync call function, data type see DPSDK_DATA_GET_INTERDOOR_LOCK
* @param	[IN] pUserData				User data
* @return	Successful return 0, failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetInterDoorLock(DPSDK_INT32 iSessionID,
															DPSDK_PAGE_INFO* pPageInfo,
															DPSDK_DataCallback fDataCallBack,
															DPSDK_VOID* pUserData);

/**
* @brief	 search inter-lock config
* @param	[IN] iSessionID				User session ID
* @param	[IN] iInterDoorLockId		 inter-lock id
* @param	[IN] fDataCallBack			Data sync call function, data type see DPSDK_DATA_QUERY_INTERDOOR_LOCK
* @param	[IN] pUserData				User data
* @return	Successful return 0, failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_QueryInterDoorLock(DPSDK_INT32 iSessionID,
															DPSDK_CHAR* pInterDoorLockId,
															DPSDK_DataCallback fDataCallBack,
															DPSDK_VOID* pUserData);

/**
* @brief	update inter-lock config
* @param	[IN] iSessionID					User session ID
* @param	[IN] pUpdateInterDoorLock		Update inter-lock config list
* @return	Successful return 0£¨failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_UpdateInterDoorLock(DPSDK_INT32 iSessionID,
																DPSDK_INTERDOOR_LOCK_INFO* pUpdateInterDoorLock);

/**
* @brief	add anti-pass back config
* @param	[IN] iSessionID					User session ID
* @param	[IN] pParam						Anti-pass back config info list
* @param	[OUT] pAntiPassId				Anti-pass back config id
* @param	[IN] iBufLen					Buffer size
* @return	Successful return 0£¨failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_AddAntiPassBack(DPSDK_INT32 iSessionID,
														DPSDK_ANTI_PASSBACK_INFO* pParam,
														DPSDK_CHAR* pAntiPassId,
														DPSDK_INT32 iBufLen);


/**
* @brief	batch delete anti-pass back config
* @param	[IN] iSessionID					User session ID
* @param	[IN] pDelAntiPassBack			Batch delete anti-pass back config list
* @param	[IN] iBufLen					Buffer size
* @return	Successful return 0£¨failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_DelAntiPassBack(DPSDK_INT32 iSessionID,
														DPSDK_DEL_ANTI_PASS_BACK* pDelAntiPassBack,
														DPSDK_INT32 iBufLen,
														DPSDK_DataCallback fDataCallBack,
														DPSDK_VOID* pUserData);

/**
* @brief	 get  anti-pass back config list
* @param	[IN] iSessionID				User session ID
* @param	[IN] pPageInfo				Page info
* @param	[IN] fDataCallBack			Data sync call function, data type see DPSDK_DATA_GET_ANTI_PASSBACK
* @param	[IN] pUserData				User data
* @return	Successful return 0, failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetAntiPassBackList(DPSDK_INT32 iSessionID,
															DPSDK_PAGE_INFO* pPageInfo,
															DPSDK_DataCallback fDataCallBack,
															DPSDK_VOID* pUserData);

/**
* @brief	 Search single anti-pass back config
* @param	[IN] iSessionID				User session ID
* @param	[IN] pAntiPassBackId		 anti-pass back configid
* @param	[IN] fDataCallBack			Data sync call function, data type see DPSDK_DATA_QUERY_ANTI_PASSBACK
* @param	[IN] pUserData				User data
* @return	Successful return 0, failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_QueryAntiPassBack(DPSDK_INT32 iSessionID,
															DPSDK_CHAR* pAntiPassBackId,
															DPSDK_DataCallback fDataCallBack,
															DPSDK_VOID* pUserData);

/**
* @brief	update anti-pass back config
* @param	[IN] iSessionID					User session ID
* @param	[IN] pSaveAntiPassBack			Update anti-pass back config list
* @return	Successful return 0£¨failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_UpdateAntiPassBack(DPSDK_INT32 iSessionID,
															DPSDK_ANTI_PASSBACK_INFO* pUpdateAntiPassBack);

/**
* @brief	get person count
* @param	[IN] iSessionID					User session ID
* @param	[OUT] pPersonCount				Person count
* @return	Successful return 0£¨failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetPersonCount(DPSDK_INT32 iSessionID,
														 DPSDK_INT32* pPersonCount);

/**
* @brief	add all-in-one card user
* @param	[IN] iSessionID					User session ID
* @param	[IN] pParam						Add all-in-one card user info
* @param	[OUT] pPersonId					User id
* @param	[IN] iBufLen					Buffer size
* @return	Successful return 0£¨failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_AddCardPerson(DPSDK_INT32 iSessionID,
														DPSDK_CARD_INFO* pParam,
														DPSDK_CHAR* pPersonId,
														DPSDK_INT32 iBufLen);

/**
* @brief	batch add all-in-one card user
* @param	[IN] iSessionID					User session ID
* @param	[IN] pParamList					Add all-in-one card user info parameter list
* @param	[IN] fDataCallBack				Data sync call function, data type see DPSDK_REPEAT_CARD_PERSON_LIST
* @param	[IN] pUserData					User data
* @return	Successful return 0£¨failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_AddCardPersonList(DPSDK_INT32 iSessionID,
															DPSDK_CARD_PERSON_LIST* pParamList,
															DPSDK_DataCallback fDataCallBack,
															DPSDK_VOID* pUserData);


/**
* @brief	multiple user are authorizes door right and card no.
* @param	[IN] iSessionID					User session ID
* @param	[IN] pParam						 door right and card no. parameter
* @param	[IN] fDataCallBack				Data sync call function, data type see DPSDK_REPEAT_CARD_PERSON_LIST
* @param	[IN] pUserData					User data
* @return	Successful return 0£¨failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_AddCardRight(DPSDK_INT32 iSessionID,
													   DPSDK_CARD_PERSON_LIST* pParam,
													   DPSDK_DataCallback fDataCallBack,
													   DPSDK_VOID* pUserData);

/**
* @brief	get Fail Tasks.
* @param	[IN] iSessionID					User session ID
* @param	[IN] pParam						get Fail Tasks parameter
* @param	[IN] fDataCallBack				Data sync call function, data type see DPSDK_REPEAT_CARD_PERSON_LIST
* @param	[IN] pUserData					User data
* @return	Successful return 0£¨failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetFailTasks(DPSDK_INT32 iSessionID,
													   DPSDK_GET_FAIL_TASK_PARAM* pParam,
													   DPSDK_DataCallback fDataCallBack,
													   DPSDK_VOID* pUserData);

/**
* @brief	get Fail Rule Tasks.
* @param	[IN] iSessionID					User session ID
* @param	[IN] fDataCallBack				Data sync call function, data type see DPSDK_DATA_GET_FAIL_RULE_TASK_LIST
* @param	[IN] pUserData					User data
* @return	Successful return 0£¨failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetFailRuleTasks(DPSDK_INT32 iSessionID,
													       DPSDK_DataCallback fDataCallBack,
													       DPSDK_VOID* pUserData);


/**
* @brief	batch get image
* @param	[IN] iSessionID					User session ID
* @param	[IN] pParam						Batch get user imager parameter
* @param	[IN] iBufLen					Buffer size
* @param	[IN] fDataCallBack				Data sync call function, data type see DPSDK_DATA_GET_PERSONPIC_LIST
* @param	[IN] pUserData					User data
* @return	Successful return 0£¨failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetPersonPicList(DPSDK_INT32 iSessionID,
															DPSDK_GET_PERSONPIC* pParam,
															DPSDK_INT32 iBufLen,
															DPSDK_DataCallback fDataCallBack,
															DPSDK_VOID* pUserData);

/**
* @brief	add user group config
* @param	[IN] iSessionID					User session ID
* @param	[IN] pParam						Add user group config parameter
* @param	[OUT] pPersonGroupId			User group id
* @param	[IN] iBufLen					Buffer big
* @return	Successful return 0£¨failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_AddPersonGroup(DPSDK_INT32 iSessionID,
														DPSDK_PERSON_GROUP* pParam,
														DPSDK_CHAR* pPersonGroupId,
														DPSDK_INT32 iBufLen);

/**
* @brief	batch delete user
* @param	[IN] iSessionID					User session ID
* @param	[IN] pPersonList				User list
* @param	[IN] iBufLen					Buffer size
* @return	Successful return 0£¨failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_DelPerson(DPSDK_INT32 iSessionID,
													DPSDK_PERSON_LIST* pPersonList,
													DPSDK_INT32 iBufLen);

/**
* @brief	batch delete user group
* @param	[IN] iSessionID					User session ID
* @param	[IN] pPersonGroupList			User group list
* @param	[IN] iBufLen					Buffer size
* @return	Successful return 0£¨failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_DelPersonGroup(DPSDK_INT32 iSessionID,
														DPSDK_PERSON_LIST* pPersonGroupList,
														DPSDK_INT32 iBufLen);

/**
* @brief	 get all-in-one user
* @param	[IN] iSessionID					User session ID
* @param	[IN] pPersonId					User id
* @param	[IN] fDataCallBack				Data sync call function, data type see DPSDK_DATA_GET_CARD_PERSON
* @param	[IN] pUserData					User data
* @return	Successful return 0£¨failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetCardPerson(DPSDK_INT32 iSessionID,
														DPSDK_CHAR* pPersonId,
														DPSDK_DataCallback fDataCallBack,
														DPSDK_VOID* pUserData);

/**
* @brief	 get user group info list
* @param	[IN] iSessionID					User session ID
* @param	[IN] pPageInfo					Page info
* @param	[IN] fDataCallBack				Data sync call function, data type see DPSDK_DATA_GET_PERSON_GROUP_LIST
* @param	[IN] pUserData					User data
* @return	Successful return 0£¨failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetPersonGroupList(DPSDK_INT32 iSessionID,
														DPSDK_PAGE_INFO* pPageInfo,
														DPSDK_DataCallback fDataCallBack,
														DPSDK_VOID* pUserData);

/**
* @brief	 get department organization tree
* @param	[IN] iSessionID					User session ID
* @param	[IN] pKeyWord					 Search keyword
* @param	[IN] fDataCallBack				Data sync call function, data type see DPSDK_DATA_GET_DEPARTMENT_TREE
* @param	[IN] pUserData					User data
* @return	Successful return 0£¨failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetDepartmentTree(DPSDK_INT32 iSessionID,
															DPSDK_CHAR* pKeyWord,
															DPSDK_DataCallback fDataCallBack,
															DPSDK_VOID* pUserData);

/**
/**
* @brief	 get all-in-one user list
* @param	[IN] iSessionID					User session ID
* @param	[IN] pParam						 get  parameter
* @param	[IN] fDataCallBack				Data sync call function, data type see DPSDK_DATE_GET_PERSONLIST
* @param	[IN] pUserData					User data
* @return	Successful return 0£¨failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetPersonList(DPSDK_INT32 iSessionID,
														DPSDK_GETCARD_PERSON_INFO* pParam,
														DPSDK_DataCallback fDataCallBack,
														DPSDK_VOID* pUserData);

/**
* @brief	update user group config
* @param	[IN] iSessionID					User session ID
* @param	[IN] pParam						User group parameter
* @return	Successful return 0£¨failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_UpdatePersonGroup(DPSDK_INT32 iSessionID,
														DPSDK_PERSON_GROUP* pParam);


/**
* @brief	update all-in-one user
* @param	[IN] iSessionID					User session ID
* @param	[IN] pPersonId					User id
* @param	[IN] pParam						all-in-one user info
* @return	Successful return 0£¨failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_UpdateCardPerson(DPSDK_INT32 iSessionID,
															DPSDK_CHAR* pPersonId,
															DPSDK_CARD_INFO* pParam);

/**
* @brief	multiple user authorized door rule
* @param	[IN] iSessionID					User session ID
* @param	[IN] pParam						Door rule info parameter
* @param	[IN] fDataCallBack				Data sync call function, data type see DPSDK_DATA_ADD_RULE_RES
* @param	[IN] pUserData					User data
* @return	Successful return 0£¨failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_AddDoorRule(DPSDK_INT32 iSessionID,
													DPSDK_ACCESSCONTROL_RULE_INFO* pParam,
													DPSDK_DataCallback fDataCallBack,
													DPSDK_VOID* pUserData);

/**
* @brief	batch delete door rule
* @param	[IN] iSessionID					User session ID
* @param	[IN] pParam						Batch delete door rule parameter
* @param	[IN] iBufLen					Buffer size
* @return	Successful return 0£¨failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_DelDoorRule(DPSDK_INT32 iSessionID,
													DPSDK_DEL_DOOR_RULE* pParam,
													DPSDK_INT32 iBufLen);

/**
* @brief	 get A&C unlock rule
* @param	[IN] iSessionID					User session ID
* @param	[IN] pRuleId					Door rule id
* @param	[IN] fDataCallBack				Data sync call function, data type see DPSDK_DATA_GET_ACCESSCONTROL_RULE
* @param	[IN] pUserData					User data
* @return	Successful return 0£¨failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetAccessControlRule(DPSDK_INT32 iSessionID,
																DPSDK_CHAR* pRuleId,
																DPSDK_DataCallback fDataCallBack,
																DPSDK_VOID* pUserData);


/**
* @brief	 get user rule list
* @param	[IN] iSessionID					User session ID
* @param	[IN] pPageInfo					Page info
* @param	[IN] fDataCallBack				Data sync call function, data type see DPSDK_DATA_GET_ACCESSCONTROL_RULE_LIST
* @param	[IN] pUserData					User data
* @return	Successful return 0£¨failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetAccessControlRuleList(DPSDK_INT32 iSessionID,
																DPSDK_PAGE_INFO* pPageInfo,
																DPSDK_DataCallback fDataCallBack,
																DPSDK_VOID* pUserData);

/**
* @brief	update user door rule
* @param	[IN] iSessionID					User session ID
* @param	[IN] pParam						Door rule info parameter
* @param	[IN] fDataCallBack				Data sync call function, data type see DPSDK_DATA_UPDATE_RULE_RES
* @param	[IN] pUserData					User data
* @return	Successful return 0£¨failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_UpdateDoorRule(DPSDK_INT32 iSessionID,
														DPSDK_ACCESSCONTROL_RULE_INFO* pParam,
														DPSDK_DataCallback fDataCallBack,
														DPSDK_VOID* pUserData);

/**
* @brief	add holiday config
* @param	[IN] iSessionID					User session ID
* @param	[IN] pParam						Holiday info parameter
* @param	[OUT] pHolidayId				Holiday id
* @param	[IN] iBufLen					Buffer size
* @return	Successful return 0£¨failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_AddHoliday(DPSDK_INT32 iSessionID,
													DPSDK_HOLIDAY_INFO* pParam,
													DPSDK_CHAR* pHolidayId,
													DPSDK_INT32 iBufLen);

/**
* @brief	add holiday time group
* @param	[IN] iSessionID					User session ID
* @param	[IN] pParam						Holiday time group info parameter
* @param	[OUT] pHolidayGroupId			Holiday time id
* @param	[IN] iBufLen					Buffer size
* @return	Successful return 0£¨failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_AddHolidayGroup(DPSDK_INT32 iSessionID,
															DPSDK_HOLIDAYGROUP_INFO* pParam,
															DPSDK_CHAR* pHolidayGroupId,
															DPSDK_INT32 iBufLen);

/**
* @brief	batch delete holiday
* @param	[IN] iSessionID					User session ID
* @param	[IN] pParam						Holiday info parameter
* @param	[IN] iBufLen					Buffer size
* @return	Successful return 0£¨failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_DelHolidayList(DPSDK_INT32 iSessionID,
														DPSDK_DEL_HOLIDAY* pParam,
														DPSDK_INT32 iBufLen);

/**
* @brief	batch delete holiday group
* @param	[IN] iSessionID					User session ID
* @param	[IN] pParam						Holiday time group info parameter
* @param	[IN] iBufLen					Buffer size
* @return	Successful return 0£¨failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_DelHolidayGroupList(DPSDK_INT32 iSessionID,
															DPSDK_DEL_HOLIDAY* pParam,
															DPSDK_INT32 iBufLen);

/**
* @brief	delete holiday config
* @param	[IN] iSessionID					User session ID
* @param	[IN] pHolidayId					Holiday id
* @return	Successful return 0£¨failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_DelHoliday(DPSDK_INT32 iSessionID,
													DPSDK_CHAR* pHolidayId);

/**
* @brief	delete time group config
* @param	[IN] iSessionID					User session ID
* @param	[IN] pHolidayGroupId			Holiday time group id
* @return	Successful return 0£¨failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_DelHolidayGroup(DPSDK_INT32 iSessionID,
															DPSDK_CHAR* pHolidayGroupId);

/**
* @brief	 get holiday details config
* @param	[IN] iSessionID					User session ID
* @param	[IN] pHolidayId					Holiday id
* @param	[IN] fDataCallBack				Data sync call function, data type see DPSDK_DATA_GET_HOLIDAY_INFO
* @param	[IN] pUserData					User data
* @return	Successful return 0£¨failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetHoliday(DPSDK_INT32 iSessionID,
													DPSDK_CHAR* pHolidayId,
													DPSDK_DataCallback fDataCallBack,
													DPSDK_VOID* pUserData);

/**
* @brief	 get time group detail
* @param	[IN] iSessionID					User session ID
* @param	[IN] pHolidayGroupId			Holiday time group id
* @param	[IN] fDataCallBack				Data sync call function, data type see DPSDK_DATA_GET_HOLIDAYGROUP_INFO
* @param	[IN] pUserData					User data
* @return	Successful return 0£¨failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetHolidayGroup(DPSDK_INT32 iSessionID,
														DPSDK_CHAR* pHolidayGroupId,
														DPSDK_DataCallback fDataCallBack,
														DPSDK_VOID* pUserData);

/**
* @brief	 get holiday list
* @param	[IN] iSessionID					User session ID
* @param	[IN] pPageInfo					Page info
* @param	[IN] fDataCallBack				Data sync call function, data type see DPSDK_DATA_GET_HOLIDAY_LIST
* @param	[IN] pUserData					User data
* @return	Successful return 0£¨failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetHolidayList(DPSDK_INT32 iSessionID,
														DPSDK_PAGE_INFO* pPageInfo,
														DPSDK_DataCallback fDataCallBack,
														DPSDK_VOID* pUserData);

/**
* @brief	 get holiday group list
* @param	[IN] iSessionID					User session ID
* @param	[IN] pPageInfo					Page info
* @param	[IN] fDataCallBack				Data sync call function, data type see DPSDK_DATA_GET_HOLIDAYGROUP_LIST
* @param	[IN] pUserData					User data
* @return	Successful return 0£¨failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetHolidayGroupList(DPSDK_INT32 iSessionID,
															DPSDK_PAGE_INFO* pPageInfo,
															DPSDK_DataCallback fDataCallBack,
															DPSDK_VOID* pUserData);
/**
* @brief	update holiday config
* @param	[IN] iSessionID					User session ID
* @param	[IN] pParam						Holiday info parameter
* @param	[IN] iBufLen					Buffer size
* @param	[IN] pHolidayId					Holiday id
* @return	Successful return 0£¨failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_UpdateHoliday(DPSDK_INT32 iSessionID,
														DPSDK_HOLIDAY_INFO* pParam);

/**
* @brief	update holiday time group
* @param	[IN] iSessionID					User session ID
* @param	[IN] pParam						Holiday time group info parameter
* @return	Successful return 0£¨failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_UpdateHolidayGroup(DPSDK_INT32 iSessionID,
																DPSDK_HOLIDAYGROUP_INFO* pParam);

/**
* @brief	according to channel Id get current channel right user
* @param	[IN] iSessionID					User session ID
* @param	[IN] pChannelIdList				Channel id list
* @param	[IN] fDataCallBack				Data sync call function, data type see DPSDK_DATA_GET_PERSON_DEV_RIGHT
* @param	[IN] pUserData					User data
* @return	Successful return 0£¨failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetPersonDevRight(DPSDK_INT32 iSessionID,
															DPSDK_CHANNEL_ID_LIST* pChannelIdList,
															DPSDK_DataCallback fDataCallBack,
															DPSDK_VOID* pUserData);
/**
* @brief	 get A&C map log
* @param	[IN] iSessionID					User session ID
* @param	[IN] pParam						 Search A&C map parameter
* @param	[IN] fDataCallBack				Data sync call function, data type see DPSDK_DATA_GET_MAP_LOG_LIST
* @param	[IN] pUserData					User data
* @return	Successful return 0£¨failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetMapLogList(DPSDK_INT32 iSessionID,
														DPSDK_MAP_LOG_PARAM* pParam,
														DPSDK_DataCallback fDataCallBack,
														DPSDK_VOID* pUserData);
/**
* @brief	 get set global unlock channel
* @param	[IN] iSessionID					User session ID
* @param	[IN] fDataCallBack				Data sync call function, data type see DPSDK_DATA_GET_GLOBALCONTRAL_DOOR_CONFIG
* @param	[IN] pUserData					User data
* @return	Successful return 0£¨failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetGlobalDoorConfig(DPSDK_INT32 iSessionID,
															DPSDK_DataCallback fDataCallBack,
															DPSDK_VOID* pUserData);

/**
* @brief	issue fingerprint collection info
* @param	[IN] iSessionID					User session ID
* @param	[IN] pReaderId					ReaderID
* @return	Successful return 0£¨failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_CaptureFingerPrint(DPSDK_INT32 iSessionID,
															DPSDK_CHANNEL_ID* pReaderId);

/**
* @brief	failed to issue task and issue again
* @param	[IN] iSessionID					User session ID
* @param	[IN] pParam						Issue task parameter
* @return	Successful return 0£¨failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_IssuePersonCardTaskFail(DPSDK_INT32 iSessionID,
																DPSDK_ISSUE_FAIL_TASKS* pParam);

/**
* @brief	failed to issue task and issue again
* @param	[IN] iSessionID					User session ID
* @param	[IN] pParam						Issue task parameter
* @return	Successful return 0£¨failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_IssueRuleTaskFail(DPSDK_INT32 iSessionID,
															DPSDK_ISSUE_RULE_FAIL_TASKS* pParam);

/**
* @brief	release store structure data buffer
* @param	[IN] pStoreBuffer				buffer to be released
* @return	Successful return 0£¨failed return error code
* @note		pBuffer is sdk internal applied buffer space
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_ReleaseStoreDataBuffer(DPSDK_STORE_LIST* pStoreBuffer);


/**
* @brief	 get store list
* @param	[IN] iSessionID			User session ID
* @param	[IN] pcodes				Store codes list
* @param	[OUT] pStoreInfo		Store info
* @return	Successful return 0£¨failed return error code
* @note		pStoreInfo need DPSDK_ReleaseStoreDataBuffer interface to release
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetStores(DPSDK_INT32 iSessionID,
													DPSDK_STORE_CODE_LIST* pCodes,
													DPSDK_STORE_LIST** pStoreInfo);

/**
* @brief	 get store code list
* @param	[IN] iSessionID				User session ID
* @param	[IN] fDataCallBack			Data sync call function, data type see DPSDK_DATA_GET_STORE_CODE_LIST
* @param	[IN] pUserData				User data
* @return	Successful return 0£¨failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetStoreCodes(DPSDK_INT32 iSessionID,
														DPSDK_DataCallback fDataCallBack,
														DPSDK_VOID* pUserData);

/**
* @brief	 get store organization tree
* @param	[IN] iSessionID			User session ID
* @param	[IN] pStoreOrgTree		Store organization tree info
* @param	[OUT] pInfoXml			Store organization tree info Xml stream
* @return	Successful return 0£¨failed return error code
* @note		pInfoXml need DPSDK_ReleaseDataBuffer interface to release
pInfoXml format such as
<?xml version="1.0" encoding="UTF-8"?>
<Organization modifytime="1477374613">
<Department coding="001" name="root" orgType="1" sn="" memo="" modifytime="1471834069" depsort="0" gpsXStr="" gpsYStr="" sort="0">
<Store id="10000" sort="1"/>
<Department coding="001001" name="f" orgType="1" sn="" memo="" modifytime="1473336575" depsort="1" gpsXStr="" gpsYStr="" sort="1">
<Store id="10001" sort="2"/>
</Department>
<Department coding="001002" name="dddd" orgType="1" sn="" memo="" modifytime="1473336590" depsort="2" gpsXStr="" gpsYStr="" sort="2">
</Department>
</Department>
</Organization>
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetStoreOrgTree(DPSDK_INT32 iSessionID,
															DPSDK_STOREORG_TREE_INFO* pStoreOrgTree,
															DPSDK_CHAR** pInfoXml);

//////////////////////////////////////////////////////////////////////////
/*
*	Virtual talk interface
*/

/**
* @brief	call request
* @param	[IN] iSessionID			User session ID
* @param	[IN] pStartTalkParam	Call request parameter
* @param	[OUT] pMediaSessionID	Media session ID
* @return	Successful return 0, failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_StartCall(DPSDK_INT32 iSessionID,
													DPSDK_START_CALL_PARAM* pStartCallParam,
													DPSDK_INT32* pMediaSessionID);

/**
* @brief	stop call
* @param	[IN] iSessionID			User session ID
* @param	[IN] iMediaSessionID	Media session ID
* @return	Successful return 0, failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_StopCall(DPSDK_INT32 iSessionID,
												   DPSDK_INT32 iMediaSessionID);

/**
* @brief	release call
* @param	[IN] iSessionID			User session ID
* @param	[IN] iMediaSessionID	Media session ID
* @return	Successful return 0, failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_CeaseCall(DPSDK_INT32 iSessionID,
													DPSDK_INT32 iMediaSessionID);

/**
* @brief	request talk right
* @param	[IN] iSessionID			User session ID
* @param	[IN] iMediaSessionID	Media session ID
* @return	Successful return 0, failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_DemandCall(DPSDK_INT32 iSessionID,
													 DPSDK_INT32 iMediaSessionID);

/**
* @brief	call invitation
* @param	[IN] iSessionID			User session ID
* @param	[IN] pInviteCallParam	Request parameter
* @param	[OUT] pMediaSessionID	Media session ID
* @return	Successful return 0, failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_InviteCall(DPSDK_INT32 iSessionID,
													 DPSDK_INVITE_CALL_PARAM* pInviteCallParam,
													 DPSDK_INT32* pMediaSessionID);

/**
* @brief	call hang up
* @param	[IN] iSessionID			User session ID
* @param	[IN] iMediaSessionID	Media session ID
* @return	Successful return 0, failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_ByeCall(DPSDK_INT32 iSessionID,
												  DPSDK_INT32 iMediaSessionID);

/**
* @brief	change call status 
* @param	[IN] iSessionID			User session ID
* @param	[IN] iMediaSessionID	Media session ID
* @param	[IN] iCallStatus		Call status 
* @return	Successful return 0, failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_ModifyCallStatus(DPSDK_INT32 iSessionID,
														   DPSDK_INT32 iMediaSessionID,
														   DPSDK_INT32 iCallStatus);

/**
* @brief	intercom call request
* @param	[IN] iSessionID			User session ID
* @param	[IN] pStartTalkParam	Call request parameter
* @param	[OUT] pMediaSessionID	Media session ID
* @param	[OUT] pStartVtCallRes	Start VtCall Response
* @return	Successful return 0, failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_StartVtCall(DPSDK_INT32 iSessionID,
													  DPSDK_START_VTCALL_PARAM* pStartVtCallParam,
													  DPSDK_INT32* pMediaSessionID,
													  DPSDK_START_VTCALL_RES* pStartVtCallRes);

/**
* @brief	stop intercom call
* @param	[IN] iSessionID			User session ID
* @param	[IN] iMediaSessionID	Media session ID
* @return	Successful return 0, failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_StopVtCall(DPSDK_INT32 iSessionID,
													 DPSDK_INT32 iMediaSessionID);

/**
* @brief	intercom release call
* @param	[IN] iSessionID			User session ID
* @param	[IN] iMediaSessionID	Media session ID
* @return	Successful return 0, failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_CeaseVtCall(DPSDK_INT32 iSessionID,
													  DPSDK_INT32 iMediaSessionID);

/**
* @brief	require talk right
* @param	[IN] iSessionID			User session ID
* @param	[IN] iMediaSessionID	Media session ID
* @return	Successful return 0, failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_DemandVtCall(DPSDK_INT32 iSessionID,
													   DPSDK_INT32 iMediaSessionID);

/**
* @brief	intercom call invitation
* @param	[IN] iSessionID			User session ID
* @param	[IN] pInviteCallParam	Request parameter
* @param	[OUT] pMediaSessionID	Media session ID
* @return	Successful return 0, failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_InviteVtCall(DPSDK_INT32 iSessionID,
													   DPSDK_INVITE_VTCALL_PARAM* pInviteVtCallParam,
													   DPSDK_INT32* pMediaSessionID);

/**
* @brief	intercom call hang up
* @param	[IN] iSessionID			User session ID
* @param	[IN] iMediaSessionID	Media session ID
* @return	Successful return 0, failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_ByeVtCall(DPSDK_INT32 iSessionID,
													DPSDK_INT32 iMediaSessionID);

/**
* @brief	intercom change call status 
* @param	[IN] iSessionID			User session ID
* @param	[IN] iMediaSessionID	Media session ID
* @param	[IN] iCallStatus		Call status 
* @return	Successful return 0, failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_ModifyVtCallStatus(DPSDK_INT32 iSessionID,
															 DPSDK_INT32 iMediaSessionID,
															 DPSDK_INT32 iCallStatus);

/**
* @brief	intercom reject call request(after request is received, call before invite, so no need of iMediaSessionID)
* @param	[IN] iSessionID			User session ID
* @param	[IN] pParam				Request parameter
* @return	Successful return 0, failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_RejectVtCall(DPSDK_INT32 iSessionID,
													   DPSDK_VTCALL_REJECT_PARAM* pParam);

/**
* @brief	intercom cancel call request(after ring, you can call cancel, no need of  iMediaSessionID)
* @param	[IN] iSessionID			User session ID
* @param	[IN] pParam				Request parameter
* @return	Successful return 0, failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_CancelVtCall(DPSDK_INT32 iSessionID,
													   DPSDK_VTCALL_CANCEL_PARAM* pParam);

/**
* @brief	intercom info release
* @param	[IN] iSessionID			User session ID
* @param	[IN] pParam				Message parameter
* @return	Successful return 0, failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_VtCallPublishInfo(DPSDK_INT32 iSessionID,
															DPSDK_VTCALL_PUBLISH_PARAM* pParam);

/**
* @brief	intercom talk cancel info release
* @param	[IN] iSessionID			User session ID
* @param	[IN] pMsgID				Message ID
* @return	Successful return 0, failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_StopPublishInfo(DPSDK_INT32 iSessionID,
															DPSDK_CHAR* pMsgID);

/**
* @brief	according to call log id delete call log
* @param	[IN] iSessionID				User session ID
* @param	[IN] pCallLogList			Call log id list
* @param	[IN] iBufLen				Buffer size
* @return	Successful return 0£¨failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_DelCallLog(DPSDK_INT32 iSessionID,
													DPSDK_CALLLOG_ID_LIST* pCallLogList,
													DPSDK_INT32 iBufLen);

/**
* @brief	 get single call log
* @param	[IN] iSessionID				User session ID
* @param	[IN] pId					Main key id
* @param	[OUT] pCallLogInfo			Call log info
* @return	Successful return 0£¨failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetCallLog(DPSDK_INT32 iSessionID,
													DPSDK_CHAR* pId,
													DPSDK_CALLLOG_INFO* pCallLogInfo);

/**
* @brief	 get call log
* @param	[IN] iSessionID				User session ID
* @param	[IN] pParam					 get call log parameter
* @param	[IN] fDataCallBack			Data sync call function, data type see DPSDK_DATA_GET_CALLLOG_LIST_RECORD
* @param	[IN] pUserData				User data
* @return	Successful return 0£¨failed return error code
*/

DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetCallLogListRecord(DPSDK_INT32 iSessionID,
																DPSDK_CALLLOG_PARAM* pParam,
																DPSDK_DataCallback fDataCallBack,
																DPSDK_VOID* pUserData);

/**
* @brief	call log export
* @param	[IN] iSessionID				User session ID
* @param	[IN] pExportParam			Export parameter
* @param	[OUT] pFileUrl				File URL
* @param	[IN] uiBufLen				Buffer size
* @return	Successful return 0, failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_ExportCallLog(DPSDK_INT32 iSessionID,
														DPSDK_EXPORT_LOG_PARAM* pExportParam,
														DPSDK_CHAR* pFileUrl,
														DPSDK_UINT32 uiBufLen);


/**
* @brief	 get call number
* @param	[IN] iSessionID				User session ID
* @param	[IN] pParam					Get call number parameter
* @param	[IN] fDataCallBack			Data sync call function, data type see DPSDK_DATA_GET_CALLLOG_NUMBER
* @param	[IN] pUserData				User data
* @return	Successful return 0£¨failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetCallLogNumber(DPSDK_INT32 iSessionID,
														DPSDK_CALLNUMBER_PARAM* pParam,
														DPSDK_DataCallback fDataCallBack,
														DPSDK_VOID* pUserData);

/**
* @brief	add device group
* @param	[IN] iSessionID				User session ID
* @param	[IN] pParam					Device group parameter
* @param	[OUT] pId					Main key id
* @param	[IN] iBufLen				Buffer size
* @return	Successful return 0£¨failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_AddDevGroup(DPSDK_INT32 iSessionID,
													DPSDK_DEV_GROUP* pParam,
													DPSDK_CHAR* pId,
													DPSDK_INT32 iBufLen);

/**
* @brief	delete device group
* @param	[IN] iSessionID				User session ID
* @param	[IN] pParam					Device group parameter
* @param	[IN] iBufLen				Buffer size
* @return	Successful return 0£¨failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_DelDevGroup(DPSDK_INT32 iSessionID,
													DPSDK_DEV_GROUP_ID_LIST* pParam,
													DPSDK_INT32 iBufLen);

/**
* @brief	 get single device group info
* @param	[IN] iSessionID				User session ID
* @param	[IN] pId					Main key id
* @param	[IN] fDataCallBack			Data sync call function, data type see DPSDK_DATA_GET_DEVICE_GROUP
* @param	[IN] pUserData				User data
* @return	Successful return 0£¨failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetDevGroup(DPSDK_INT32 iSessionID,
													DPSDK_CHAR* pId,
													DPSDK_DataCallback fDataCallBack,
													DPSDK_VOID* pUserData);

/**
* @brief	 get all device groups
* @param	[IN] iSessionID				User session ID
* @param	[IN] pPageInfo				Page info
* @param	[IN] fDataCallBack			Data sync call function, data type see DPSDK_DATA_GET_DEVICE_GROUP_LIST
* @param	[IN] pUserData				User data
* @return	Successful return 0£¨failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetDevGroupList(DPSDK_INT32 iSessionID,
														DPSDK_PAGE_INFO* pPageInfo,
														DPSDK_DataCallback fDataCallBack,
														DPSDK_VOID* pUserData);

/**
* @brief	update device group
* @param	[IN] iSessionID				User session ID
* @param	[IN] pParam					Device group parameter
* @param	[IN] pDevGroupId			Device group id
* @return	Successful return 0£¨failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_UpdateDevGroup(DPSDK_INT32 iSessionID,
														DPSDK_DEV_GROUP* pParam,
														DPSDK_CHAR* pDevGroupId);

/**
* @brief	restore default device group
* @param	[IN] iSessionID				User session ID
* @return	Successful return 0£¨failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_RestoreDefaultDevGroup(DPSDK_INT32 iSessionID);

/**
* @brief	new group linked relation
* @param	[IN] iSessionID				User session ID
* @param	[IN] pParam					Group link relation parameter
* @param	[OUT] pId					Main key id
* @param	[IN] iBufLen				Buffer size
* @return	Successful return 0£¨failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_AddGroupRelation(DPSDK_INT32 iSessionID,
														DPSDK_GROUP_RELATION* pParam,
														DPSDK_CHAR* pId,
														DPSDK_INT32 iBufLen);

/**
* @brief	delete link relation among groups
* @param	[IN] iSessionID				User session ID
* @param	[IN] pParam					Inter-group linked relation parameter
* @param	[IN] iBufLen				Buffer size
* @return	Successful return 0£¨failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_DelGroupRelation(DPSDK_INT32 iSessionID,
														DPSDK_GROUP_RELATION_ID_LIST* pParam,
														DPSDK_INT32 iBufLen);

/**
* @brief	 get single group linked relation
* @param	[IN] iSessionID				User session ID
* @param	[IN] pId					Main key id
* @param	[IN] fDataCallBack			Data sync call function, data type see DPSDK_DATA_GET_RELATION_GROUP
* @param	[IN] pUserData				User data
* @return	Successful return 0£¨failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetGroupRelation(DPSDK_INT32 iSessionID,
														DPSDK_CHAR* pId,
														DPSDK_DataCallback fDataCallBack,
														DPSDK_VOID* pUserData);

/**
* @brief	group linked relation list
* @param	[IN] iSessionID				User session ID
* @param	[IN] pPageInfo				Page info
* @param	[IN] fDataCallBack			Data sync call function, data type see DPSDK_DATA_GET_RELATION_GROUP_LIST
* @param	[IN] pUserData				User data
* @return	Successful return 0£¨failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetGroupRelationList(DPSDK_INT32 iSessionID,
															DPSDK_PAGE_INFO* pPageInfo,
															DPSDK_DataCallback fDataCallBack,
															DPSDK_VOID* pUserData);


/**
* @brief	update group linked relation
* @param	[IN] iSessionID				User session ID
* @param	[IN] pParam					Group linked relation parameter
* @param	[IN] pId					Main key id
* @return	Successful return 0£¨failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_UpdateGroupRelation(DPSDK_INT32 iSessionID,
															DPSDK_GROUP_RELATION* pParam,
															DPSDK_CHAR* pId);

/**
* @brief	save info release detail info
* @param	[IN] iSessionID				User session ID
* @param	[IN] pParam					info release detail info
* @param	[OUT] pId					Main key id
* @param	[IN] iBufLen				Buffer size
* @return	Successful return 0£¨failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_SavePublisInfo(DPSDK_INT32 iSessionID,
														DPSDK_ADD_PUBLISH_INFO* pParam,
														DPSDK_CHAR* pId,
														DPSDK_INT32 iBufLen);

/**
* @brief	delete info release
* @param	[IN] iSessionID				User session ID
* @param	[IN] pParam					info release parameter
* @param	[IN] iBufLen				Buffer size
* @return	Successful return 0£¨failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_DelPublisInfo(DPSDK_INT32 iSessionID,
														DPSDK_PUBLISH_INFO_ID_LIST* pParam,
														DPSDK_INT32 iBufLen);

/**
* @brief	according to main key ID get single release info
* @param	[IN] iSessionID				User session ID
* @param	[IN] pId					Main key id
* @param	[IN] fDataCallBack			Data sync call function, data type see DPSDK_DATA_GET_INFO_PUBLISH
* @param	[IN] pUserData				User data
* @return	Successful return 0£¨failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetPublishInfo(DPSDK_INT32 iSessionID,
														DPSDK_CHAR* pId,
														DPSDK_DataCallback fDataCallBack,
														DPSDK_VOID* pUserData);

/**
* @brief	 get info release list
* @param	[IN] iSessionID				User session ID
* @param	[IN] pPageInfo				Page info
* @param	[IN] fDataCallBack			Data sync call function, data type see DPSDK_DATA_GET_INFO_PUBLISH_LIST
* @param	[IN] pUserData				User data
* @return	Successful return 0£¨failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetPublishInfoList(DPSDK_INT32 iSessionID,
															DPSDK_PAGE_INFO* pPageInfo,
															DPSDK_DataCallback fDataCallBack,
															DPSDK_VOID* pUserData);

/**
* @brief	update info release status 
* @param	[IN] iSessionID				User session ID
* @param	[IN] pParam					info release detail info
* @return	Successful return 0£¨failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_UpdatePublisInfo(DPSDK_INT32 iSessionID,
														DPSDK_PUBLISH_INFO* pParam);

/**
* @brief	add management group
* @param	[IN] iSessionID				User session ID
* @param	[IN] pParam					Management group info parameter
* @param	[OUT] pId					Main key id
* @param	[IN] iBufLen				Buffer size
* @return	Successful return 0£¨failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_AddManagerGroup(DPSDK_INT32 iSessionID,
														DPSDK_ADD_MANAGER_GROUP_INFO* pParam,
														DPSDK_CHAR* pId,
														DPSDK_INT32 iBufLen);

/**
* @brief	delete management group
* @param	[IN] iSessionID				User session ID
* @param	[IN] pParam					Management group parameter
* @param	[IN] iBufLen				Buffer size
* @return	Successful return 0£¨failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_DelManagerGroup(DPSDK_INT32 iSessionID,
														DPSDK_MANAGE_GROUP_ID_LIST* pParam,
														DPSDK_INT32 iBufLen);

/**
* @brief	 get single management group info
* @param	[IN] iSessionID				User session ID
* @param	[IN] pId					Main key id
* @param	[IN] fDataCallBack			Data sync call function, data type see DPSDK_DATA_GET_MANAGER_GROUP
* @param	[IN] pUserData				User data
* @return	Successful return 0£¨failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetManagerGroup(DPSDK_INT32 iSessionID,
														DPSDK_CHAR* pId,
														DPSDK_DataCallback fDataCallBack,
														DPSDK_VOID* pUserData);

/**
* @brief	 get management group list
* @param	[IN] iSessionID				User session ID
* @param	[IN] pPageInfo				Page info
* @param	[IN] fDataCallBack			Data sync call function, data type see DPSDK_DATA_GET_MANAGER_GROUP_LIST
* @param	[IN] pUserData				User data
* @return	Successful return 0£¨failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetManagerGroupList(DPSDK_INT32 iSessionID,
															DPSDK_PAGE_INFO* pPageInfo,
															DPSDK_DataCallback fDataCallBack,
															DPSDK_VOID* pUserData);

/**
* @brief	update management group
* @param	[IN] iSessionID				User session ID
* @param	[IN] pParam					Management group parameter
* @param	[IN] pManagerGroupId		Management group id
* @return	Successful return 0£¨failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_UpdateManagerGroup(DPSDK_INT32 iSessionID,
															DPSDK_ADD_MANAGER_GROUP_INFO* pParam, 
															DPSDK_CHAR* pManagerGroupId);

/**
* @brief	move member of management group to another group
* @param	[IN] iSessionID				User session ID
* @param	[IN] pParam					Management group parameter
* @return	Successful return 0£¨failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_MoveManagerGroup(DPSDK_INT32 iSessionID,
															DPSDK_MANAGER_GROUP_INFO* pParam);

/**
* @brief	restore default management group
* @param	[IN] iSessionID				User session ID
* @return	Successful return 0£¨failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_RestoreDefaultMgrGroup(DPSDK_INT32 iSessionID);


/**
* @brief	issue contact to VTO
* @param	[IN] iSessionID				User session ID
* @param	[IN] pParam					Issue contact to VTO parameter
* @return	Successful return 0£¨failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_VtoContact(DPSDK_INT32 iSessionID,
													DPSDK_VTO_DEV_PARAM* pParam);

/**
* @brief	according to VTH code get user name
* @param	[IN] iSessionID				User session ID
* @param	[IN] pVthCodeList			VTH code list
* @param	[IN] iBufLen				Buffer size
* @param	[IN] fDataCallBack			Data sync call function, data type see DPSDK_DATA_GET_DEVPERSON_BY_VTH
* @param	[IN] pUserData				User data
* @return	Successful return 0£¨failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetDevPersonByVth(DPSDK_INT32 iSessionID,
															DPSDK_DEVICE_CODE_LIST* pVthCodeList,
															DPSDK_INT32 iBufLen,
															DPSDK_DataCallback fDataCallBack,
															DPSDK_VOID* pUserData);

/**
* @brief	import intercom device
* @param	[IN] iSessionID				User session ID
* @param	[IN] pParam					Import intercom device parameter
* @param	[OUT] pResult				Import intercom device result
* @return	Successful return 0£¨failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_ImportVideoIntercomDev(DPSDK_INT32 iSessionID,
																DSPDK_VIDEOINTERCOM_PARAM* pParam,
																DSPDK_IMPORT_RESULT* pResult);


/**
* @brief	add resource
* @param	[IN] iSessionID				User session ID
* @param	[IN] pParam					Add resource parameter
* @param	[OUT] pNotify				Add resource return parameter
* @return	Successful return 0£¨failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_TZNewResource(DPSDK_INT32 iSessionID,
	const DPSDK_TZ_NEWRESOURCE_PARAM* pParam, DPSDK_TZ_NEWRESOURCE_NOTIFY* pNotify);

/**
* @brief	get cloud storage info
* @param	[IN] iSessionID				User session ID
* @param	[IN]	uiBufLen
* @param	[OUT] pEFSInfoList			Cloud storage info list
* @return	Successful return 0£¨failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_TZQueryEFSInfo(DPSDK_INT32 iSessionID,
	DPSDK_UINT32 uiBufLen, DPSDK_TZ_EFSINFO_LIST* pEFSInfoList);

/**
* @brief	get Function Config
* @param	[IN] iSessionID			User session ID
* @param	[OUT] pInfoXml			Store organization tree info Xml stream
* @return	Successful return 0£¨failed return error code
* @note		pInfoXml need DPSDK_ReleaseDataBuffer interface to release
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetFunctionConfig(DPSDK_INT32 iSessionID,
															DPSDK_CHAR** pInfoXml);

/**
* @brief	get Human List
* @param	[IN] iSessionID			User session ID
* @param	[IN] pLocale			 	International Voice 
* @param	[IN] pPageInfo				Page Info
* @param	[IN] pHumanSearchInfo		Search Info
* @param	[IN] fDataCallBack			Data sync callback function, refer to DPSDK_DATA_GET_HUMAN_LIST for data type
* @param	[OUT] pUserData				User Data
* @return	Successful return 0£¨failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetHumanList(DPSDK_INT32 iSessionID,
													   DPSDK_CHAR* pLocale,
													   DPSDK_PAGE_INFO* pPageInfo,
													   DPSDK_HUMANSEARCH_PARAM* pHumanSearchInfo,
													   DPSDK_DataCallback fDataCallBack,
													   DPSDK_VOID* pUserData);

/**
* @brief	get Human Count
* @param	[IN] iSessionID			User session ID
* @param	[IN] pHumanSearchInfo		Search Info
* @param	[OUT] pHumanCount			Human Count
* @return	Successful return 0£¨failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetHumanCount(DPSDK_INT32 iSessionID,
													   DPSDK_HUMANSEARCH_PARAM* pHumanSearchInfo,
													   DPSDK_INT32* pHumanCount);

/**
* @brief	Export Human List
* @param	[IN] iSessionID			User session ID
* @param	[IN] pLocale			 	International Voice 
* @param	[IN] pSessionID				session
* @param	[IN] pHumanSearchInfo		Search Info
* @return	Successful return 0£¨failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_ExportHumanList(DPSDK_INT32 iSessionID,
														DPSDK_CHAR* pLocale,
														DPSDK_CHAR* pSessionID,
													    DPSDK_EXPORT_HUMANSEARCH_PARAM* pHumanSearchInfo);


/**
* @brief	get Vehicle List
* @param	[IN] iSessionID			User session ID
* @param	[IN] pLocale			 	International Voice 
* @param	[IN] pPageInfo				Page Info
* @param	[IN] pVehicleSearchInfo		Search Info
* @param	[IN] fDataCallBack			Data sync callback function, refer to DPSDK_DATA_GET_VEHICLES_LIST for data type
* @param	[OUT] pUserData				User Data
* @return	Successful return 0£¨failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetVehicleList(DPSDK_INT32 iSessionID,
													   DPSDK_CHAR* pLocale,
													   DPSDK_PAGE_INFO* pPageInfo,
													   DPSDK_VEHICLE_PARAM* pVehicleSearchInfo,
													   DPSDK_DataCallback fDataCallBack,
													   DPSDK_VOID* pUserData);

/**
* @brief	get Vehicle Count
* @param	[IN] iSessionID			User session ID
* @param	[IN] pVehicleSearchInfo		Search Info
* @param	[OUT] pVehicleCount			Vehicle Count
* @return	Successful return 0£¨failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetVehicleCount(DPSDK_INT32 iSessionID,
														DPSDK_VEHICLE_PARAM* pVehicleSearchInfo,
														DPSDK_INT32* pVehicleCount);

/**
* @brief	Export VehicleList List
* @param	[IN] iSessionID			User session ID
* @param	[IN] pLocale			 	International Voice 
* @param	[IN] pSessionID				session
* @param	[IN] pVehicleSearchInfo		Search Info
* @return	Successful return 0£¨failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_ExportVehicleList(DPSDK_INT32 iSessionID,
														  DPSDK_CHAR* pLocale,
														  DPSDK_CHAR* pSessionID,
														  DPSDK_EXPORT_VEHICLESEARCH_PARAM* pVehicleSearchInfo);

/**
* @brief	get NonVehicle List
* @param	[IN] iSessionID			User session ID
* @param	[IN] pLocale			 	International Voice 
* @param	[IN] pPageInfo				Page Info
* @param	[IN] pNonVehicleSearchInfo		Search Info
* @param	[IN] fDataCallBack			Data sync callback function, refer to DPSDK_DATA_GET_NONVEHICLES_LIST for data type
* @param	[OUT] pUserData				User Data
* @return	Successful return 0£¨failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetNonVehicleList(DPSDK_INT32 iSessionID,
														 DPSDK_CHAR* pLocale,
														 DPSDK_PAGE_INFO* pPageInfo,
														 DPSDK_NONVEHICLE_PARAM* pVehicleSearchInfo,
														 DPSDK_DataCallback fDataCallBack,
														 DPSDK_VOID* pUserData);

/**
* @brief	get NonVehicle Count
* @param	[IN] iSessionID			User session ID
* @param	[IN] pVehicleSearchInfo		Search Info
* @param	[OUT] pNonVehicleCount			Vehicle Count
* @return	Successful return 0£¨failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_GetNonVehicleCount(DPSDK_INT32 iSessionID,
														  DPSDK_NONVEHICLE_PARAM* pVehicleSearchInfo,
														  DPSDK_INT32* pNonVehicleCount);

/**
* @brief	Export NonVehicleList List
* @param	[IN] iSessionID			User session ID
* @param	[IN] pLocale			 	International Voice 
* @param	[IN] pSessionID				session
* @param	[IN] pNonVehicleSearchInfo		Search Info
* @return	Successful return 0£¨failed return error code
*/
DPSDK_EXPORT DPSDK_INT32 DPSDK_CALL DPSDK_ExportNonVehicleList(DPSDK_INT32 iSessionID,
															DPSDK_CHAR* pLocale,
															DPSDK_CHAR* pSessionID,
															DPSDK_EXPORT_NONVEHICLESEARCH_PARAM* pNonVehicleSearchInfo);

#endif // _DPSDK_DPSDK_H_
