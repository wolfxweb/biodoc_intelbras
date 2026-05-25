/**
 *
 * File Name£ºDPSDKDefine_Preview.h
 * File Mark£º
 * Abstract£ºpreview enumeration structure dedfinition
 *
 * Current Version£º1.0
 * Original Author£º36259
 * Date£º2017/4/12
 * Edition Record£ºCreation
*/


#ifndef DPSDK_DPSDKDEFINEPREVIEW_H
#define DPSDK_DPSDKDEFINEPREVIEW_H

#include "DPSDKDefine.h"

// Server Info - Map
typedef struct  
{
	//Video Basic Parameter
	DPSDK_CHAR		szCodeId[DPSDK_CHANNEL_CODE_LEN];	// Channel id or device id
	DPSDK_INT32		iStreamType;						// Stream type 1=main stream, 2=sub stream 
	HWND			pHWnd;								// Window handle
	DPSDK_CHAR		szTrackId[10];						// Track ID 
	DPSDK_CHAR		szExtend[10];						// Expansion data 
	DPSDK_CHAR		szPlanId[10];						// Record plan ID 
	DPSDK_INT32		iDataType;							// Video type£º1=video, 2=audio, 3=A/V
	DPSDK_BOOL      bIsMulitcast;						// Broadcast or not

	//Play Mode Parameter
	DPSDK_UINT32	enumPlayMode;						// Play mode 0:live priority 1:fluency priority 2: balance priority 3: customize£¬see Play_Mode_e definition
	DPSDK_INT32		iDelayTime;							// Delay time
	DPSDK_BOOL		bDecode;							// Hardware speed enable

	//Split preview
	DPSDK_BOOL		bPreview;							// Split preview or not
	DPSDK_INT32		iScreenNum;							// Split
	DPSDK_INT32		iStartChnlIndex;					// Start channel

	//Transcoding parameter
	DPSDK_INT32		iUsedVcs;							// Mark require vcs transcoding or not.0£ºnot required£»1£ºrequired.
	DPSDK_CHAR		szVideoCode[DPSDK_REAL_PARAM_LEN];	// Video encode mode, refer to video encode format custom string
	DPSDK_CHAR		szResolution[DPSDK_REAL_PARAM_LEN];	// Stream resolution, refer to resolution custom string
	DPSDK_INT32		iFps;								// Stream frame rate
	DPSDK_INT32		iBps;								// Stream bit rate
}DPSDK_REALPLAY_PARAM;


	/** Video stream callback
		@param		pHeadData£ºstream head
		@param		iHeadLen£ºstream length
		@param		pData£ºstream data
		@param		iLen£ºdata length
		@param		uiTime£ºtime stamp
		@param		pUserData£ºuser data
	@return
	*/
typedef void (DPSDK_CALL* RealDataFunc)(char* pHeadData, int iHeadLen, char* pData, int iLen, unsigned int uiTime, void* pUserParam);
#endif // DPSDK_DPSDKDEFINEPREVIEW_H
