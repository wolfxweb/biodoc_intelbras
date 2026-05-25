#ifndef DPSDK_DPSDKDEFINE_H
#define DPSDK_DPSDKDEFINE_H

typedef int					DPSDK_INT32;
typedef unsigned int		DPSDK_UINT32;
typedef long long			DPSDK_INT64;
typedef unsigned long long	DPSDK_UINT64;
typedef void*				DPSDK_LPVOID;
typedef long				DPSDK_LONG;
typedef unsigned long		DPSDK_ULONG;
typedef bool				DPSDK_BOOL;

typedef char				DPSDK_CHAR;
typedef unsigned char		DPSDK_UCHAR;
typedef float				DPSDK_FLOAT;
typedef double				DPSDK_DOUBLE;
typedef void				DPSDK_VOID;
typedef short				DPSDK_SHORT;
typedef unsigned short		DPSDK_USHORT;
typedef long long			DPSDK_TIMET;

#ifdef _WIN32

#ifdef DPSDK_EXPORTS
#define DPSDK_EXPORT extern "C" __declspec(dllexport)
#else
#define DPSDK_EXPORT extern "C" __declspec(dllimport)
#endif

#define DPSDK_CALL				__stdcall

#ifndef _WIN32_WINNT		// Allow to use functions of Windows XP or higher version
#define _WIN32_WINNT 0x0501	// Change this value to corresponding value, to apply to other Windows versions
#endif	

#else

#include <string.h>
#include <stddef.h>
#include <stdint.h>

#define DPSDK_EXPORT extern "C"
#define DPSDK_CALLTYPE
#define DPSDK_CALL	
#endif

#ifdef _WIN32
#include "Winsock2.h"
#include <windows.h>
typedef HWND HCWND;
#else
typedef void* HCWND;
#endif

#ifdef _WIN32
typedef RECT DPSDK_RECT;
typedef DWORD DPSDK_DWORD;
typedef HDC DPSDK_HDC;
#else
typedef struct 
{
	DPSDK_LONG    left;
	DPSDK_LONG    top;
	DPSDK_LONG    right;
	DPSDK_LONG    bottom;
}DPSDK_RECT;

#define __stdcall
typedef unsigned int DPSDK_DWORD;
typedef void* DPSDK_HDC;

#endif

#define DPSDK_NAME_LEN              128					// Username length
#define DPSDK_NAME_LEN_EX			256					// Organization,device,channel name length
#define DPSDK_PWD_LEN               128					// Password length
#define DPSDK_IP_LEN                64					// IP length
#define DPSDK_MACADDRESS_LEN        128					// MAC address length
#define DPSDK_IMEI_LEN              128					// Length of the mobile phone IMEI identification code
#define DPSDK_VIDEO_PARAM_LEN		20					// Length of video parameter
#define DPSDK_URL_LEN				256					// URL Length
#define DPSDK_FTP_MODIFY_URL_LEN	256*2				// URL Length (the notify of FTP configuration modify)
#define DPSDK_FILE_PATH_LEN			1024				// Length of file path
#define MASTERSALVE_CLASS_LEN       16					// Length of Algorithm scheme type
#define DPSDK_ALARM_ALARMCODE_LEN				50		// Length of alarm code
#define DPSDK_ALARM_HANDLERUSER_LEN				64		// Username length of the alarm handler
#define DPSDK_ALARM_HANDLEMESSAGE_LEN			766		// Length of alarm handle message(255*3 + 1)
#define DPSDK_ALARM_EMAILRECEIVER_LEN			320		// Length of the notification email for alarm handling
#define DPSDK_ALARM_TIME_LEN					15		// Alarm relative time length
#define DPSDK_ALARM_DEVICEID_LEN				56		// Device ID length
#define DPSDK_ALARM_CHANNELID_LEN				64		// Channel ID length
#define DPSDK_ALARM_ORGID_LEN					20		// Length of organization node ID
#define DPSDK_ALARM_ALARMID_LEN					20		// Alarm ID length
#define DPSDK_ALARM_DEVICENAME_LEN				256		// Length of device name
#define DPSDK_ALARM_CHANNELNAME_LEN				256		// Length of channel name
#define DPSDK_ALARM_ALARMPICTURE_LEN			256*16	// Address length of the alarm picture
#define DPSDK_ALARM_IVS_ALARMPICTURE_LEN		256		// Address length of alarm smart snapshot picture
#define DPSDK_ALARM_GROUPNAME_LEN				50		// Length of group name
#define DPSDK_ALARM_ALARMSOURCE_LEN				100		// Length of alarm source
#define DPSDK_ALARM_ALARMTYPENAME_LEN			256		// Length of alarm type name. (Temporary, no corresponding data available in the database)
#define DPSDK_ALARM_LANGUAGE_LEN				20		// Language length (Temporary, no corresponding data available in the database)
#define DPSDK_ALARM_ALARMEXPORTDOWNLOADPATH_LEN	256		// URL address length for alarm export (Temporary, no corresponding data available in the database)
#define DPSDK_ALARM_NODECODE_LEN				90		// Code length for surveillance node
#define DPSDK_ALARM_ALARMMESSAGE_LEN			256		// Length of alarm extended information (Temporary, no corresponding data available in the database)
#define DPSDK_ALARM_EMAILRECEIVERLIST_SIZE		10		// Max email receiver list number for alarm handling notification
#define DPSDK_ALARM_LINKVEDIOINFOLIST_SIZE		16		// Max channel number for alarm linked videos
#define DPSDK_ALARM_MAIL_SUBJECT_LEN			256		// Length of alarm email's title 
#define DPSDK_ALARM_MAIL_FORMAT_LEN				512		// Alarm mail format string length
#define DPSDK_ALARM_MEMO_NUM_MAX                200
#define DPSDK_RECORD_NUM_MAX                    6000
#define DPSDK_ALARM_ALARMCOMMENT_LEN		    1024
#define DPSDK_PRESET_POINT_NUM_MAX				256

#define DPSDK_ORG_CODE_LEN						96		// Organization code length
#define DPSDK_ORG_SN_LEN						56		// Organization SN code length
#define DPSDK_DEVICE_ID_LEN						56		// Device ID length
#define DPSDK_SN_LEN							56		// SN length
#define DPSDK_CHANNEL_ID_LEN					64		// Channel ID length
#define DPSDK_GPS_LEN							50		// GPS length
#define DPSDK_TYPE_LEN							20		// Type length
#define DPSDK_DEVICE_NAME_LEN					256		// Device name length
#define DPSDK_CHANNEL_NAME_LEN					256		// Channel name length
#define DPSDK_ORG_NAME_LEN						256		// Length of organization name
#define DPSDK_SERVER_CODE_LEN					64		// Service code
#define DPSDK_COLOR_LEN							16		// Length of color

#define DPSDK_PTZ_EXTEND_LEN					256		// (Temporary)
#define DPSDK_PTZ_TIME_LEN						15		// Length of time
#define DPSDK_PRESETPOINT_CODE_LEN				50		// Preset point code number
#define DPSDK_PRESETPOINT_NAME_LEN				50		// Preset point name
#define DPSDK_DEVICE_CODE_LEN					56		// Device code length
#define DPSDK_USER_LEVEL_LEN					20		// User level

#define DPSDK_BITMAP_FILE_HEADER_LEN			14		// File header length of bmp picture
#define DPSDK_BITMAP_INFO_HEADER_LEN			40		// Information header length of bmp picture
#define DPSDK_TVWALL_NAME_LEN					50		// TV Wall name
#define DPSDK_TVWALL_OWNERCODE_LEN				90		// TV Wall owner code
#define DPSDK_TVWALL_SPLIT_CAP_LEN				128		// Length of Open Windows or ScreenCaps

#define DPSDK_LINKED_CHANNEL_SIZE 				16		//Max number of video linked channels

#define DPSDK_BIRTHDAY_LEN						20		// Birthday length
#define DPSDK_TIME_LEN							15		// Time length of timestamp
#define DPSDK_PERSON_ID_LEN						50		// Person ID
#define DPSDK_PERSONTYPE_NAME_LEN				100		// Length of person type
#define DPSDK_USERDEFINEDATA_FILENAME_LEN		256		// Length of filename for user defined data

#define DPSDK_PLATE_NUMBER_LEN					20		// Length of car plate number
#define DPSDK_MEMURIGHT_LEN						64		// Length of right menu
#define DPSDK_ID_LEN							64
#define DPSDK_ID_LEN_EX							128
#define DPSDK_USER_ID_LEN						64		// Length of User ID,role ID,group ID
#define DPSDK_ERROR_LEN						    100     // Length of error message 
#define DPSDK_PWD_EXPIRY_LEN					32		// Password expiry length
#define DPSDK_USER_REMARK_LEN					512		// User remark length
#define DPSDK_MEMO_LEN							256		// Length of description information
#define DPSDK_CONTENT_LEN						512		// Length of send message 
#define DPSDK_CONFIG_PARAM_LEN					64		// Max length of configuration parameter
#define DPSDK_KEYWORD_LEN						50		// Length of keyword
#define DPSDK_PHONE_LEN							30		// Length of phone number
#define DPSDK_TIME_STRING_LEN					20		// Length of time string 
#define DPSDK_DB_ID_LEN							16		// Length of database ID
#define DPSDK_SUBJECT_LEN						32		// Length of video tag's title
#define DPSDK_DAY_IN_MONTH						31		// Max days of one month
#define DPSDK_ALARM_CHANNEL_ID_LEN				64		// videos of linked alarm channel
#define DPSDK_RECORD_FILE_NAME_LEN				256		// Length of record file's name
#define DPSDK_DISDK_ID_LEN						32		// Length of disk ID
#define DPSDK_TRACK_ID_LEN						16		// Length of track ID 
#define DPSDK_RECORD_TAG_SUBJECT_LEN			64		// Length of video tag's title 
#define DPSDK_RECORD_TAG_CONTENT_LEN			256		// Length of video tag's content
#define	DPSDK_KEY_WORD_LEN						64		// Length of video tag's keywords
#define DPSDK_LOCK_RECORD_REASON_LEN			128		// The reason of lock video
#define DPSDK_FINGER_PRINT_LEN					1200	// Length of fingerprint code
#define DPSDK_CARD_NO_LEN						64		// Length of card number
#define DPSDK_PICTURE_LEN						256		// Length of picture compression data
#define DPSDK_ALARM_CODE_LEN					128		// Length of alarm code
#define DPSDK_STORENAME_LEN						64		// Length of Store name
#define DPSDK_MAP_NAME_LEN						64		// Length of map name
#define DPSDK_MAP_ID_LEN						64		// Length of map ID
#define DPSDK_STORE_CODE_LEN					256		// Length of store code
#define DPSDK_STORE_NAME_LEN					256		// Length of Store name
#define DPSDK_STORE_ORGID_LEN					20		// Length of Store organization ID
#define DPSDK_PERSON_NO_LEN						64		// Length of person code
#define DPSDK_NATIONALITRY_LEN					64		// Length of nationality
#define DPSDK_TZ_APPEARPOINT_LEN				10		// Length of AppearPoint
#define DPSDK_TZ_RECORDID_LEN					64		// Length of case Record Id
#define DPSDK_TZ_CHNCODE_LEN					32		// Length of case channel Code
#define DPSDK_TZ_ADDRESS_LEN					256		// Length of case address
#define DPSDK_TZ_TIME_LEN						64		// Length of case time
#define DPSDK_TZ_CREATOR_LEN					32		// Length of case creator
#define DPSDK_TZ_DEVCODE_LEN					32		// Length of case device Code
#define DPSDK_TZ_GPS_LEN						24		// Length of case GPS
#define DPSDK_TZ_MEMO_LEN						512		// Length of case memo
#define DPSDK_TZ_DICTYPE_LEN					10		// Length of dictionary type
#define DPSDK_TZ_RESOURCECODE_LEN				64		// Length of resource code
#define DPSDK_TZ_RESOURCENAME_LEN				128		// Length of resource name
#define DPSDK_TZ_PATH_LEN						256		// Length of path
#define DPSDK_TZ_TIMEDIFF_LEN					8		// Length of time difference
#define DPSDK_TZ_FILESIZE_LEN					18		// Length of file size
#define DPSDK_TZ_STRING_LEN						64		// General Length of TZ
#define DPSDK_MEDIA_KEY_LEN						49
#define DPSDK_MEDIA_KEY_MAX_LEN					1177
#define DPSDK_FINGER_PRINT_NAME					64

#define DPSDK_UNLOCKMODES_LEN					256

#define DPSDK_LANGUAGE_LEN                      10      // Length of language 

#define DPSDK_MAX_DOOR_LOCK_ID_NUM              100     // Max. available multi-door interlocking ID

#define DPSDK_MAX_VTO_SIPID_NUM					20		// Max number of vthRelated Confirm VTO SipId

#define DPSDK_HAS_INT_FIELD_FALSE				-1		// Integer field is not available

// Log rank The higher the level is, the less the content of the output is
typedef enum
{
	LOG_LEVEL_DEBUG	= 2,		// debugging Do not print normally for debugging and use
	LOG_LEVEL_INFO	= 4,		// information
	LOG_LEVEL_WARN	= 5,		// notice
	LOG_LEVEL_ERR	= 6,		// error
}DPSDK_LOG_LEVEL_TYPE;

typedef struct
{
    DPSDK_UINT32   uiIPType;                  // IP type, refer to DPSDK_IP_TYPE 
    DPSDK_CHAR     szIP[DPSDK_IP_LEN];        // IP address
}DPSDK_IP;

typedef enum
{
    CLIENT_PC       = 1,    // PC Client
    CLIENT_MAC      = 2,    // MAC Client
    CLIENT_ANDROID  = 3,    // Android client
    CLIENT_IPHONE   = 4,    // IPhone client
	CLIENT_PAD		= 5,	// PAD client
	CLIENT_WEB		= 6,	// WEB client
	CLIENT_OTHER	= 7,	// Other
	CLIENT_CONFIG	= 8,	// configuration tool
}DPSDK_CLIENT_TYPE;

// Data type definition of data synchronization callback function
typedef enum
{
	// Organization, device
	DPSDK_DATA_ORG_INFO = 1,							// Organizational data A detailed view of the structure DPSDK_ORG_INFO
	DPSDK_DATA_DEVICE_INFO = 2,							// Device data A detailed view of the structure DPSDK_DEV_ALL_INFO_LIST
	DPSDK_DATA_COLLECT_ORG_INFO = 3,					// Collection tree A detailed view of the structure DPSDK_COLLECTION_ORG_INFO
	DPSDK_DATA_DEVICE_LAYERED = 4,						// Hierarchical acquisition of device tree A detailed view of the structure DPSDK_LAYERED_RESULT_LIST
	DPSDK_DATA_DEVICE_LIST_BY_ORG = 5,					// Device data A detailed view of the structure DPSDK_DEV_ALL_INFO_LIST
	DPSDK_DATA_ALL_ORG_INFO = 6,						// All organizational data A detailed view of the structure DPSDK_ALL_ORG_INFO
	//Save device
	DPSDK_DATA_DELETE_DEVICE_INFO = 100,				// Delete the information of device A detailed view of the structure DPSDK_DELETE_DEVICE_FAIL_LIST	
	DPSDK_DATA_SEARCH_CHANNELS_INFO = 101,				// Search the information of channel A detailed view of the structure DPSDK_SEARCH_CHANNEL_LIST
	DPSDK_DATA_QUERY_DEVICE_INFO = 102,					// Query the information of device A detailed view of the structure DPSDK_UPDATE_DEVICE_INFO
	DPSDK_DATA_GET_UNSEARCH_DEVICE = 103,				// Get UnSearched Device A detailed view of the structure  DPSDK_DEVICE_LIST
	DPSDK_DATA_GET_CURISE_PRESET_INFO = 104,
	DPSDK_DATA_ADD_DEVICE_LIST = 105,
	DPSDK_DATA_GET_UNAUTH_DEVICE = 106,
	DPSDK_DATA_SAVE_UNAUTH_DEVICE = 107,
	DPSDK_DATA_UPDATE_DEVICE_INFO = 108,
	DPSDK_DATA_GET_DEVICE_TYPE = 109,					// Get The Device Type A detailed view of the structure DPSDK_DEVICE_TYPE_INFO
	DPSDK_DATA_GET_HIS_MEDIA_KEY = 110,					// Get The Device Type A detailed view of the structure DPSDK_MEDIA_KEY_LIST
	//User
	DPSDK_DATA_USER_DETAIL_INFO = 500,					// Get detail information of user A detailed view of the structure DPSDK_USER_DETAIL_INFO_LIST
	DPSDK_DATA_USER_GET_EMAIL_INFO = 501,				// Get user email info detailed view of the structure DPSDK_USER_EMAIL_INFO
	DPSDK_DATA_USER_GROUP_LIST = 502,					// Get group list of user A detailed view of the structure DPSDK_USER_GROUP_LIST
	DPSDK_DATA_USER_USERNAME_LIST = 503,
	DPSDK_DATA_USER_BASE_INFO_LIST = 504,
	DPSDK_DATA_USER_GET_APP_USERS = 505,				// Get app users detailed view of the structure DPSDK_APP_USER_LIST
	//Abstract
	DPSDK_DATA_ABS_INFO_TIME = 1000,					// Get detail information of abstract by time A detailed view of the structure DPSDK_ABSTRACT_OBJ_DETAIL_LIST
	DPSDK_DATA_ABS_LIFE_CYCLE = 1001,					// Get life cycle of abstract picture A detailed view of the structure DPSDK_ABSTRACT_LIFE_CYCLE_LIST
	DPSDK_DATA_ABS_SUMMARY_INFO = 1002,					// Get profile information of abstract A detailed view of the structure DPSDK_ABSTRACT_OBJ_SUMMMARY_LIST
	//TVWALL
	DPSDK_DATA_TVWALL_TASK_INFO = 2000,					// Get TVWALL task information A detailed view of the structure DPSDK_TVWALL_TASK_INFO
	DPSDK_DATA_GET_TVWALL_PROJECT_FILE = 2001,			// Get TVWALL project file A detailed view of the structure DPSDK_TVWALL_TASK_INFO_LIST
	// Alarm,alarm plan
	DPSDK_DATA_ALARM_PLAN_LIST = 3000,					// Get alarm plan list A detailed view of the structure DPSDK_ALARM_PLAN_DETAIL_INFO_LIST
	DPSDK_DATA_PLAN_DETAIL_INFO = 3001,					// Get the information of alarm plan A detailed view of the structure DPSDK_ALARM_PLAN_DETAIL_INFO
	DPSDK_DATA_TIME_TEMPLATE_INFO = 3002,				// Delete time template A detailed view of the structure DPSDK_DELETE_TIME_TEMPLATE_INFO

	// Alarm process
	DPSDK_DATA_PEND_ALARM_LIST = 3300,					// Get pend alarm list A detailed view of the structure DPSDK_PEND_ALARM_LIST
	DPSDK_DATA_PROCESSED_ALARM_LIST = 3301,				// Get processed alarm list A detailed view of the structure DPSDK_PROCESSED_ALARM_LIST
	DPSDK_DATA_ALARM_TEMPLATE_LIST = 3302,				// Get alarm template list A detailed view of the structure DPSDK_ALARM_TEMPLATE_LIST
	DPSDK_DATA_ALARM_TEMPLATE_INFO = 3303,				// Get alarm template list A detailed view of the structure DPSDK_ALARM_TEMPLATE_INFO
	DPSDK_DATA_ALARM_FACEALARM_TYPE_LIST = 3304,		// Get alarm faceAlarm Type list A detailed view of the structure DPSDK_CHANNEL_FACEALARM_TYPE_LIST

	// Resource binding
	DPSDK_DATA_RESOURCE_BINDING_INFO = 3401,			// Get resource binding info A detailed view of the structure DPSDK_RESOURCE_BINDING_INFO

	//bay
	DPSDK_DATA_ITC_SECTION_LIST = 3801,					// Get itc section info A detailed view of the structure DPSDK_ITC_SECTION_LIST

	// Record,storage
	DPSDK_DATA_STORAGE_GETLOGICALPARTITION = 4000,		// Query local storage A detailed view of the structure DPSDK_STORAGE_LOGICALPARTITION_LIST
	// Alarmhost
	DPSDK_DATA_GET_SUBSYSTEM_ALARMHOST_INFO = 5000,		// Query alarm host defenceArea and subsystem  A detailed view of the structure DPSDK_GET_INFO
	DPSDK_DATA_QUERY_ALARMHOST = 5001,					// Query detail information of alarm host A detailed view of the structure DPSDK_ALARMHOST_INFO
	// Acess
	DPSDK_DATA_GET_INTERDOOR_LOCK = 6000,				// Get list of inter door A detailed view of the structure DPSDK_GET_INTERDOOR_LOCK
	DPSDK_DATA_QUERY_INTERDOOR_LOCK = 6001,				// Query configuration of inter door A detailed view of the structure DPSDK_INTERDOOR_LOCK_INFO
	DPSDK_DATA_GET_ANTI_PASSBACK = 6002,				// Get anti pass back A detailed view of the structure DPSDK_GET_ANTI_PASSBACK
	DPSDK_DATA_QUERY_ANTI_PASSBACK = 6003,				// Query configuration of single anti pass back A detailed view of the structure DPSDK_ANTI_PASSBACK_INFO
	DPSDK_DATA_GET_DEPARTMENT_TREE = 6004,				// Get user tree of card A detailed view of the structure DPSDK_DEPARTMENT_TREE_INFO
	DPSDK_DATE_GET_DOORGROUP = 6005,					// Get configuration of door group A detailed view of the structure DPSDK_DOOR_GROUP_INFO
	DPSDK_DATE_GET_PERSONLIST = 6006,					// Get list of card user A detailed view of the structure DPSDK_CARD_LIST
	DPSDK_DATA_GET_CARD_PERSON = 6007,					// Get card user A detailed view of the structure DPSDK_CARD_INFO
	DPSDK_DATA_GET_ACCESSCONTROL_LOG = 6008,			// Get log of acess A detailed view of the structure DPSDK_ACCESSCONTROL_LIST
	DPSDK_DATA_GET_DOOR_ALARM_INOUT = 6009,				// Get basic configuration of door's alarm in and out A detailed view of the structure DPSDK_GET_DOOR_ALARM_INOUT
	DPSDK_DATA_GET_ACCESSCONTROL_RULE = 6010,			// Get rules of open door about acess A detailed view of the structure DPSDK_ACCESS_CONTROL_RULE_INFO
	DPSDK_DATA_GET_ACCESSCONTROL_RULE_LIST = 6011,		// Get list of user's rule A detailed view of the structure DPSDK_ACCESSCONTRO_RULE_LIST
	DPSDK_DATA_UPDATE_DOOR_RULE = 6012,					// Update door's rule A detailed view of the structure DPSDK_ACCESSCONTROL_RULE
	DPSDK_DATA_GET_HOLIDAY_INFO = 6013,					// Get detail information of holiday A detailed view of the structure DPSDK_HOLIDAY_INFO
	DPSDK_DATA_GET_HOLIDAYGROUP_INFO = 6014,			// Get detail information of time group A detailed view of the structure DPSDK_HOLIDAYGROUP_INFO
	DPSDK_DATA_GET_HOLIDAY_LIST = 6015,					// Get list of holiday A detailed view of the structure DPSDK_GET_INFO_LIST
	DPSDK_DATA_GET_HOLIDAYGROUP_LIST = 6016,			// Get list of holiday group A detailed view of the structure DPSDK_GET_INFO_LIST
	DPSDK_DATA_GET_DOOR_READER = 6017,					// Get reader of door A detailed view of the structure DPSDK_DOOR_READER
	DPSDK_DATA_GET_FIRST_CARD_LIST = 6018,				// Get list of all door A detailed view of the structure DPSDK_GET_FIRST_CARD_LIST
	DPSDK_DATA_GET_FIRST_CARD = 6019,					// Get configuration of first card to open door A detailed view of the structure DPSDK_FIRST_CARD_INFO
	DPSDK_DATA_GET_MULTI_CARD = 6020,					// Get configuration of multi card to open door A detailed view of the structure DPSDK_MULTI_CARD_INFO
	DPSDK_DATA_GET_MULTI_CARD_LIST = 6021,				// Get list of door A detailed view of the structure DPSDK_MULTI_CARD_LIST
	DPSDK_DATA_GET_DOORGROUP_LIST = 6022,				// Get list of all door group A detailed view of the structure DPSDK_DOOR_GROUP_LIST
	DPSDK_DATA_GET_REMOTE_VERIFICATION = 6023,			// Get remote verification A detailed view of the structure DPSDK_REMOTE_VERIFICATION_INFO
	DPSDK_DATA_GET_REMOTE_VERIFICATION_LIST = 6024,		// Get list of remote verification A detailed view of the structure DPSDK_REMOTE_VERIFICATION_LIST
	DPSDK_DATA_GET_PERSON_GROUP_LIST = 6025,			// Get information list of person group A detailed view of the structure DPSDK_PERSON_GROUP_LIST
	DPSDK_DATA_GET_DOORGROUP_PERSON = 6026,				// Obtain door group and person bonding. A detailed view of the structure DPSDK_PERSON_DATA_LIST
	DPSDK_DATA_GET_PERSONPIC_LIST = 6027,				// Get the picture of person by batch A detailed view of the structure DPSDK_PERSONPIC_LIST
	DPSDK_DATA_GET_PERSON_DEV_RIGHT = 6028,				// Get authorized user by channel id to the channel A detailed view of the structure DPSDK_PERSON_LIST
	DPSDK_DATA_GET_MAP_LOG_LIST		= 6029,				// Get log list of access map A detailed view of the structure DPSDK_MAP_LOG_LIST
	DPSDK_DATA_GET_GLOBALCONTRAL_DOOR_CONFIG = 6030,	// Get channel of global control door A detailed view of the structure DPSDK_CHANNEL_ID_LIST
	DPSDK_DATA_GET_DOOR_CONTROLLER = 6031,				// Get controller of device A detailed view of the structure DPSDK_DEV_CONTROLLER_LIST
	DPSDK_DATA_GET_ACCESS_CONTROL_STATUS = 6032,		// Get status model of access A detailed view of the structure DPSDK_ACCESS_CONTROL_STATUS_LIST
	DPSDK_DATA_GET_DOORBASE_CONFIGURE = 6033,			// Get basic configuration of door A detailed view of the structure DPSDK_DOOR_INFO
	DPSDK_DATA_ADD_CARDPERSON_LIST = 6034,
	DPSDK_DATA_ADD_CARD_RIGHT = 6035,
	DPSDK_DATA_ADD_RULE_RES = 6036,						// Add rule result A detailed view of the structure DPSDK_ADD_RULE_RES
	DPSDK_DATA_UPDATE_RULE_RES = 6037,					// Update rule result A detailed view of the structure DPSDK_UPDATE_RULE_RES
	DPSDK_DATA_DEL_ANTI_PASSBACK = 6038,
	DPSDK_DATA_DEL_FIRST_CARD = 6039,
	DPSDK_DATA_DEL_INTERDOOR_LOCK = 6040,
	DPSDK_DATA_DEL_MULTI_CARD = 6041,
	DPSDK_DATA_DEL_REMOTE_VERIFICATION_LIST = 6042,
	DPSDK_DATA_GET_FAIL_TASK_LIST = 6043,
	DPSDK_DATA_IMPORT_PERSONS = 6044,
	DPSDK_DATA_GET_ATTENDANCE_CHANNELS_LIST = 6045,		    // Get Att channels A detailed view of the structure DPSDK_GET_CHANNEL_RESULT	
	DPSDK_DATA_GET_ATTENDANCE_TIMEINTERVAL_INFO = 6046,		    // Get time interval A detailed view of the structure DPSDK_TIMEINTERVAL_DATA	
	DPSDK_DATA_GET_ATTENDANCE_TIMEINTERVALS_INFO = 6047,		    // Get time intervals A detailed view of the structure DPSDK_GET_TIMEINTERVAL_LIST
	DPSDK_DATA_GET_ATTENDANCE_ORDER_INFO = 6048,					// Get order info A detailed view of the structure DPSDK_ATTENDANCEORDER_INFO	
	DPSDK_DATA_GET_ATTENDANCE_ORDERS_INFO = 6049,					// Get orders info A detailed view of the structure DPSDK_ATTENDANCEORDER_LIST	
	DPSDK_DATA_GET_ATTENDANCE_SCHEDULE_INFO = 6050,			    // Get schedule info A detailed view of the structure DPSDK_PERSON_SCHEDULE_INFO
	DPSDK_DATA_GET_ATTENDANCE_DEPTSCHEDULE_INFO = 6051,			    // Get DeptSchedule info A detailed view of the structure DPSDK_DEPT_SCHEDULE_INFO
	DPSDK_DATA_DEL_ATTENDANCE_ORDERS_INFO = 6052,			    // Del orders info A detailed view of the structure DPSDK_ATTENDANCE_ORDER_NAME_LIST
	DPSDK_DATA_DEL_ATTENDANCE_TIMEINTERVAL_INFO = 6053,		    // Del time interval A detailed view of the structure DPSDK_ATTENDANCE_TIMEINTERVAL_NAME_LIST
	DPSDK_DATA_GET_ATTENDANCE_BATCH_ORDER_INFO = 6054,			// Batch Get orders info A detailed view of the structure DPSDK_BATCH_ATTENDANCEORDER_LIST
	DPSDK_DATA_GET_ATTENDANCE_BATCH_TIMEINTERVAL_INFO = 6055,		    // Batch Get time interval A detailed view of the structure DPSDK_BATCH_TIMEINTERVAL_LIST	
	DPSDK_DATA_GET_FAIL_RULE_TASK_LIST = 6056,					// Get Fail rule tasks A detailed view of the structure DPSDK_ISSUE_RULE_FAIL_TASKS

	//Store
	DPSDK_DATA_GET_STORE_CODE_LIST = 6500,				// Get store code list  A detailed view of the structure DPSDK_STORE_CODE_LIST
	
	//Vtcall
	//DPSDK_DATA_GET_CALLLOG_LIST = 7000,				// Get list of call log A detailed view of the structure DPSDK_CALLLOG_LIST
	DPSDK_DATA_GET_CALLLOG_LIST_RECORD = 7001,			// Get call log A detailed view of the structure DPSDK_CALLLOG_LIST
	DPSDK_DATA_GET_CALLLOG_NUMBER = 7002,				// Get call number A detailed view of the structure DPSDK_GET_CALLNUMBER_NUMBER
	DPSDK_DATA_GET_DEVICE_GROUP = 7003,					// Get information of single device group A detailed view of the structure DPSDK_DEV_GROUP_INFO
	DPSDK_DATA_GET_DEVICE_GROUP_LIST = 7004,			// Get information of all device groups A detailed view of the structure  DPSDK_DEV_GROUP_LIST
	DPSDK_DATA_GENERATE_DEVICE_GROUP_DEFAULT = 7005,	// Generate default device group A detailed view of the structure DPSDK_DEV_GROUP_INFO
	DPSDK_DATA_GET_RELATION_GROUP = 7006,				// Get relation of single group relate A detailed view of the structure DPSDK_RELATION_GROUP_INFO
	DPSDK_DATA_GET_RELATION_GROUP_LIST = 7007,			// Get relation list of group relate A detailed view of the structure DPSDK_RELATION_GROUP_LIST
	DPSDK_DATA_GET_INFO_PUBLISH = 7008,					// Get published information of single message A detailed view of the structure DPSDK_PUBLISH_INFO
	DPSDK_DATA_GET_INFO_PUBLISH_LIST = 7009,			// Get published information list of message A detailed view of the structure DPSDK_PUBLISH_INFO_LIST
	DPSDK_DATA_GET_MANAGER_GROUP = 7010,				// Get information of single manager group A detailed view of the structure DPSDK_GET_MANAGER_GROUP_INFO
	DPSDK_DATA_GET_MANAGER_GROUP_LIST = 7011,			// Get manager group list A detailed view of the structure DPSDK_MANAGER_GROUP_INFO_LIST
	DPSDK_DATA_GENERATE_MANAGER_GROUP_DEFAULT = 7012,	// Generate default manager group A detailed view of the structure DPSDK_GET_MANAGER_GROUP_INFO
	DPSDK_DATA_GET_DEVPERSON_BY_VTH = 7013,				// Get device Person by VTH code A detailed view of the structure DPSDK_VTH_DEV_LIST

	//AR
	DPSDK_DATA_AR_GET_LABEL_TEMPLATE = 8001,			// Get label template list A detailed view of the structure DPSDK_AR_LABEL_TEM_LIST
	DPSDK_DATA_AR_GET_LABEL = 8002,						// Get label list A detailed view of the structure DPSDK_AR_LABEL_LIST

	DPSDK_DATA_GET_CONFIG_TYPE = 8500,

	DPSDK_DATA_GET_HUMAN_LIST = 8600,					// Get list of human detailed view of the structure DPSDK_HUMAN_LIST		
	DPSDK_DATA_GET_VEHICLES_LIST = 8601,				// Get list of vehicle detailed view of the structure DPSDK_VEHICLE_LIST
	DPSDK_DATA_GET_NONVEHICLES_LIST = 8602,				// Get list of non vehicle detailed view of the structure DPSDK_NONVEHICLE_LIST

}DPSDK_DATA_TYPE;


typedef struct
{
	DPSDK_BOOL	   bDomainUser;								 // Whether or not domain login
    DPSDK_CHAR     szUserName[DPSDK_NAME_LEN];               // User name
    DPSDK_CHAR     szPWD[DPSDK_PWD_LEN];                     // Cleartext password, the login type is0(basic account), can not be empty
    DPSDK_IP       struIP;                                   // Login server IP
    DPSDK_UINT32   uiPort;                                   // Login server port
    DPSDK_CHAR     szMACAddress[DPSDK_MACADDRESS_LEN];       // MAC address
    DPSDK_CHAR     szIMEI[DPSDK_IMEI_LEN];                   // Check code for mobile client landing platform
    DPSDK_UINT32   uiClientType;                             // Client type: Reference DPSDK_CLIENT_TYPE
    DPSDK_CHAR     szReserve[32];                            // Reserved field
	DPSDK_IP       struClientIP;                             // Client IP
}DPSDK_LOGIN_PARAM;



typedef struct
{
	DPSDK_CHAR		szIP[DPSDK_IP_LEN];						// server IP						 
	DPSDK_UINT32	uiPort;									// server port
}DPSDK_SERVER_INFO;

typedef struct
{
	DPSDK_CHAR		szChannelCode[DPSDK_CHANNEL_ID_LEN];	// Channel code						 
	DPSDK_INT32		iX;										// X coordinates
	DPSDK_INT32		iY;										// Y coordinates
}DPSDK_RADIO_POINT_PARAM;

typedef struct
{
	DPSDK_CHAR		szTemperUnit[DPSDK_TYPE_LEN];			// the unit of temperature				 
	DPSDK_INT32		iTemperAver;							// the value of temperature
}DPSDK_RADIO_POINT_INFO;

typedef enum
{
	FTP_UNKNOW			= 0,	// Unknown
	FTP_ALARM			= 1,	// Alarm
	FTP_TAG				= 2,	// Tag
	FTP_DOOR			= 3,	// Access
}DPSDK_FTP_DATA_TYPE;

// FTP server information
typedef struct  
{
	DPSDK_CHAR		szUrl[DPSDK_URL_LEN];					// FTP absolute path ftps://192.168.1.1:21
	DPSDK_CHAR		szUserName[DPSDK_NAME_LEN];				// User name
	DPSDK_CHAR		szPWD[DPSDK_PWD_LEN];					// Password
	DPSDK_UINT32	uiDataType;								// Data type Refer: DPSDK_FTP_DATA_TYPE
}DPSDK_FTP_SERVER_INFO;

// FTP server list information
typedef struct  
{
	DPSDK_UINT32			uiTotal;						// The total number of result
	DPSDK_FTP_SERVER_INFO	struFtpServerInfo[1];			// FTP server information
}DPSDK_FTP_SERVER_INFO_LIST;

// The notify of FTP server configuration modify
typedef struct
{
	DPSDK_CHAR		szUrl[DPSDK_FTP_MODIFY_URL_LEN];		// FTP absolute path ftps://192.168.1.1:21|ftps://192.168.1.2:21
	DPSDK_CHAR		szUserName[DPSDK_NAME_LEN];				// User name
	DPSDK_CHAR		szPWD[DPSDK_PWD_LEN];					// Password
}DPSDK_FTP_CONFIG_MODIFY_NOTIFY;

// Https model
typedef enum 
{
	DPSDK_HTTPS_UNENABLE	= 0,							// Not enable https
	DPSDK_HTTPS_ONEWAY_AUTH = 1,							// Enable https one-way authentication
	DPSDK_HTTPS_TWOWAY_AUTH = 2								// Enable https two-way authentication
}DPSDK_HTTPS_MODE;
//////////////////////////////////////////////////////////////////////////
/*
 *	The related define of video module
 */

typedef enum
{
	DPSDK_VIDEO_CMD_LOCK		= 0,							// Lock the current camera
	DPSDK_VIDEO_CMD_UNLOCK_ONE	= 1,							// Unlock the current camera
}DPSDK_VIDEO_LOCK_TYPE;

// Playback mode 
typedef enum
{
	DPSDK_STREAM_REAL_MODE		= 0,			// Real time priority mode
	DPSDK_STREAM_SMOOTH_MODE	= 1,			// Fluency priority model
	DPSDK_STREAM_POISE_MODE		= 2,			// Equilibrium priority model
	DPSDK_STREAM_CUSTOM_MODE	= 3,			// Custom priority mode
}DPSDK_STREAM_MODE;

// Enhanced support for functional enumeration
typedef enum 
{
	DPSDK_IVSE_DEHAZE		= 0,			// Go to the fog
	DPSDK_IVSE_DENOISE		= 1,			// Denoising
	DPSDK_IVSE_WB			= 2,			// Color correction
	DPSDK_IVSE_LOWLIGHT		= 3,			// Low illumination enhancement 
	DPSDK_IVSE_HDR			= 4,			// Wide dynamic
	DPSDK_IVSE_NUM			= 5				// Support enhanced number of functions
}DPSDK_IVSE_FUNC_TYPE;

// Intelligence
typedef enum
{
	DPSDK_IVS_RULE_VISIBLE	= 1,			// Rule
	DPSDK_IVS_OBJ_VISIBLE	= 2,			// Target box
	DPSDK_IVS_LOCUS_VISIBLE	= 3,			// trajectory == The interface is no longer displayed£¿
	DPSDK_IVS_DRAW_TRACKEX2	= 4,			// Face capture
}DPSDK_IVS_VISIBLE;

// 4K Split screen mode
typedef enum
{
	DPSDK_SPLIT_ORG			= 0,			// Basic pattern
	//Four 4K
	DPSDK_SPLIT_1P_3		= 1,			// 1P+3 pattern
	DPSDK_SPLIT_1P_5		= 2,			// 1P+5 pattern
	//Three
	DPSDK_SPLIT_3_3_1P		= 4,			// 1P+3 pattern
	DPSDK_SPLIT_3_6_1P		= 5,			// 1P+6 pattern
}DPSDK_SPLIT_TRECE_TYPE;

// Capture image format
typedef enum
{
	DPSDK_PIC_FORMAT_BMP		= 0,			// BMP type
	DPSDK_PIC_FORMAT_JPEG		= 1,			// JPEG type
	DPSDK_PIC_FORMAT_BMP24		= 6,			// BMP24 type
}DPSDK_PIC_FORMAT;

// Playback direction
typedef enum
{
	DPSDK_FORWARD_DIRECTION		= 0,			// Following discharge
	DPSDK_BACK_DIRECTION		= 1,			// Upside down
}DPSDK_PLAY_DIRECTION;

// Buffer Type
typedef enum
{
	DPSDK_VAX_BUF_VIDEO_SRC		= 1,			// Video Source Buffer
	DPSDK_VAX_BUF_AUDIO_SRC		= 2,			// Audio Source Buffer
	DPSDK_VAX_BUF_VIDEO_RENDER  = 3,			// Decoded Video Data Buffer
	DPSDK_VAX_BUF_AUDIO_RENDER  = 4,			// Decoded Audio Data Buffer
}DPSDK_VAX_BUF_TYPE;

// Fish eye opening type
typedef enum
{
	DPSDK_FISH_CORRECT			= 0,			// Fish eye correction
	DPSDK_FISH_CORRECT_AND_LINK = 1,			// Fish ball linkage and correction
	DPSDK_FISH_LINK				= 2,			// Fish ball linkage
}DPSDK_FISH_TYPE;

// Fish eye installation mode
typedef enum
{
	DPSDK_EMOUNT_MODE_INVALID	= 0,
	DPSDK_EMOUNT_MODE_CEIL		= 1,		// Top loading
	DPSDK_EMOUNT_MODE_WALL		= 2,		// Wall mounted
	DPSDK_EMOUNT_MODE_FLOOR		= 3,        // floor mounted
	DPSDK_EMOUNT_MODE_NUM
}DPSDK_FISH_MOUNTMODE;

// Fish eye image display model
typedef enum
{
	DPSDK_SHOW_MODE_INVALID									= 0,
	DPSDK_SHOW_MODE_OFF										= 1,	// Shut off the fish eye algorithm library and shut off the outside
	DPSDK_SHOW_MODE_ORIGINAL								= 2,	// Primitive pattern(Square),Zoom ratio
	DPSDK_SHOW_MODE_PANORAMA								= 3,	// 1p
	DPSDK_SHOW_MODE_PANORAMA_PLUS_ONE_EPTZ					= 4,	// 1p+1
	DPSDK_SHOW_MODE_DOUBLE_PANORAMA							= 5,	// 2p
	DPSDK_SHOW_MODE_ORIGINAL_PLUS_DOUBLE_PANORAMA			= 6,	// 1+2p
	DPSDK_SHOW_MODE_ORIGINAL_PLUS_THREE_EPTZ_REGION			= 7,	// 1+3
	DPSDK_SHOW_MODE_PANORAMA_PLUS_THREE_EPTZ_REGION			= 8,	// 1p+3
	DPSDK_SHOW_MODE_ORIGINAL_PLUS_TWO_EPTZ_REGION			= 9,	// 1+2	
	DPSDK_SHOW_MODE_ORIGINAL_PLUS_FOUR_EPTZ_REGION			= 10,	// 1+4
	DPSDK_SHOW_MODE_PANORAMA_PLUS_FOUR_EPTZ_REGION			= 11,	// 1p+4
	DPSDK_SHOW_MODE_PANORAMA_PLUS_SIX_EPTZ_REGION			= 12,	// 1p+6
	DPSDK_SHOW_MODE_ORIGINAL_PLUS_EIGHT_EPTZ_REGION			= 13,	// 1+8
	DPSDK_SHOW_MODE_PANORAMA_PLUS_EIGHT_EPTZ_REGION			= 14,	// 1p+8
	DPSDK_SHOW_MODE_TWO_EPTZ_REGION_WITH_ORIGINAL			= 15,	// 1F+2	
	DPSDK_SHOW_MODE_FOUR_EPTZ_REGION_WITH_ORIGINAL			= 16,	// 1F+4	
	DPSDK_SHOW_MODE_DOUBLE_PANORAMA_WITH_ORIGINAL			= 17,	// 1F+2p
	DPSDK_SHOW_MODE_FOUR_EPTZ_REGION_WITH_PANORAMA			= 18,	// 1p(F)+4

	DPSDK_SHOW_MODE_TWO_EPTZ_REGION							= 19,	// 2 Frame
	DPSDK_SHOW_MODE_SINGLE									= 20,	// Single picture
	DPSDK_SHOW_MODE_FOUR_EPTZ_REGION						= 21,	// 4 Frame
	DPSDK_SHOW_MODE_USER_DEFINED							= 22,	// User custom
	DPSDK_FISHEYE_CALIBRATE_MODE_ORIGINAL_PLUS_ONE_EPTZ_REGION = 23,// 1+1
	DPSDK_FISHEYE_CALIBRATE_MODE_ONE_EPTZ_REGION			= 24,	// 1 Frame
	DPSDK_SHOW_MODE_NUM
}DPSDK_FISH_SHOWMODES;

// The related operate of masterSlave track 
typedef enum
{
	DPSDK_LinkOpen,						//masterSlaveGroup.open masterSlave track group
	DPSDK_LinkLocate,					//masterSlaveGroup.locate choose a locate on main camera and linkage slave camera
	DPSDK_LinkRectLocate,				//masterSlaveGroup.rectLocate choose a rectLocate on main camera and linkage slave camera
	DPSDK_LinkTrack,					//masterSlaveGroup.track choose Object to track manually
	DPSDK_LinkMasterSlaveGlobal,		//masterSlaveGlobal  MasterSlave link configuration(enable auto tracking)
	DPSDK_LinkClose,					//masterSlaveGroup.close Close master-slave mode tracker example
	DPSDK_LinkAttach,					//masterSlaveGroup.attach  Track subscribed object, report auto/manual tracking start and end object
	DPSDK_LinkDetach,					//masterSlaveGroup.detach  Unsubscribe tracking object
}DPSDK_SmartTrack_Oper_e;

// An option for an electronic cloud platform to bloom and move
typedef enum
{
	DPSDK_EPTZ_CMD_INVALID						= 0,	
	DPSDK_EPTZ_CMD_ZOOM_IN						= 1,		// Enlarge
	DPSDK_EPTZ_CMD_ZOOM_OUT						= 2,		// Narrow
	DPSDK_EPTZ_CMD_UP							= 3,		// Move upwards
	DPSDK_EPTZ_CMD_DOWN							= 4,		// Move down
	DPSDK_EPTZ_CMD_LEFT							= 5,		// Move to the left
	DPSDK_EPTZ_CMD_RIGHT						= 6,		// Move to the right
	DPSDK_EPTZ_CMD_ROTATE_CLOCKWISE_AUTO		= 7,		// Clockwise rotation
	DPSDK_EPTZ_CMD_ROTATE_ANTICLOCKWISE_AUTO	= 8,		// Automatic counter clockwise rotation
	DPSDK_EPTZ_CMD_STOP							= 9,		// Stop it
	DPSDK_EPTZ_CMD_SHOW_REGION					= 10,		// Frame selection and enlargement
	DPSDK_EPTZ_CMD_EXIT_SHOW_REGION				= 11,		// Exit frame selection and enlargement
	DPSDK_EPTZ_CMD_DEFAULT						= 12,		// Restore the default
	DPSDK_EPTZ_CMD_ORIGIN_ROTATE				= 13,		// Circular rotation

	DPSDK_EPTZ_CMD_SET_CUR_REGION				= 0x20,		// Configure the location information of the specified window
	DPSDK_EPTZ_CMD_GET_CUR_REGION				= 0x21,		// Get the location information of the specified window
	DPSDK_EPTZ_CMD_IS_IN_PANORAMA_REGION		= 0x22,		// Whether the input point is in the current panoramic point chain area
	DPSDK_EPTZ_CMD_TAP_VIEW						= 0x23,		// Display the specified location,The point is to see
	DPSDK_EPTZ_CMD_SET_FOCUS					= 0x24,		// Setting window location information
	DPSDK_EPTZ_CMD_GET_FOCUS					= 0x25,		// Get the window location information

	DPSDK_EPTZ_CMD_PTZ_CALI						= 0x26,		// Calibration of fish ball linkage
	DPSDK_EPTZ_CMD_GET_PTZ_RLT					= 0x27,		// Acquisition of fish ball linkage location information 
	DPSDK_EPTZ_CMD_NUM
} DPSDK_FISH_EPTZCMD;

// Bolt type
typedef enum
{
	// Bolt type
	DPSDK_IPCTYPE_200WN				= 0,	//
	DPSDK_IPCTYPE_130WN				= 1,
	DPSDK_IPCTYPE_D1WN				= 2,
	DPSDK_IPCTYPE_100WN				= 3,
	DPSDK_IPCTYPE_FE				= 4,	// Fisheye

	// Type of ball machine
	DPSDK_SPCTYPE_D6501				= 100,	// Sony Movement Sixty-fiveSpeed Dome Cameras
	DPSDK_HSPCTYPE_D6A2030E			= 101,	// core 2030E£¬6ASpeed Dome Cameras
}DPSDK_CAM_TYPE;

// Lens type
typedef enum 
{
	DPSDK_LENTYPE_NORM				= 0,	// Distortion free lens
	DPSDK_LENTYPE_Lens0361 			= 1,	// Three point sixMm bolt shot
	DPSDK_LENTYPE_Lens2880			= 2,	// One hundred and thirtyThe degree of wide-angle lens
	DPSDK_LENTYPE_Lens0362 			= 3,	// Three point sixMm bolt shot
	DPSDK_LENTYPE_Lens0401 			= 4,	// FourMm bolt shot

	DPSDK_LENTYPE_TEST1				= 100,	// Debug parameters
}DPSDK_LEN_TYPE;

// Stream type
typedef enum
{
	STREAM_UNKNOW_STREAM = 0,								// Unknown
	STREAM_MAIN_STREAM = 1,									// Main stream
	STREAM_SUB_STREAM = 2,								    // Auxiliary code stream
	STREAM_THIRD_STREAM = 3,								// Three bit stream
	STREAM_LOCAL_SIGNAL_STREAM = 5,							// Local signal
}DPSDK_STREAM_TYPE;

// Record type
typedef enum
{
	DPSDK_RECORD_TYPE_ALL = 0,
	DPSDK_RECORD_TYPE_MANUAL = 1,					// Manual record
	DPSDK_RECORD_TYPE_ALARM = 2,					// Alarm record
	DPSDK_RECORD_TYPE_MOTION_DETECT = 3,			// Dynamic detection
	DPSDK_RECORD_TYPE_VIDEO_LOST = 4,				// Video loss
	DPSDK_RECORD_TYPE_VIDEO_SHELTER = 5,			// Video occlusion
	DPSDK_RECORD_TYPE_TIMER = 6,					// Timing video
	DPSDK_RECORD_TYPE_ALLDAY = 7,					// All-weather video
	DPSDK_RECORD_TYPE_FILE_RECORD = 8,				// File video conversion
	DPSDK_RECORD_TYPE_NORMAL = 9,					// Ordinary video

	DPSDK_RECORD_TYPE_CARD = 25,					// Card number video There is no this in the protocol library for the time being
	DPSDK_RECORD_TYPE_ALARM_BEGIN = 10,				// Alarm start Definition in the match protocol stack 10~300 -m -f -cSpecial alarm
	DPSDK_RECORD_TYPE_ALARM_END = 1000,				// End of intelligent alarm Definition in the match protocol stack 300~1000Intelligent alarm
}DPSDK_RECORD_TYPE;

//Video source
typedef enum
{
	DPSDK_SOURCE_TYPE_ALL = 1,						// All video, including platform video and device video
	DPSDK_SOURCE_TYPE_DEVICE = 2,					// Device video
	DPSDK_SOURCE_TYPE_CENTER = 3,					// Platform video
}DPSDK_SOURCE_TYPE;

typedef enum 
{
	DPSDK_PB_NORMAL			= 1024,
	DPSDK_PB_NORMAL_FAST2	= DPSDK_PB_NORMAL * 2,
	DPSDK_PB_NORMAL_FAST4	= DPSDK_PB_NORMAL * 4,
	DPSDK_PB_NORMAL_FAST8	= DPSDK_PB_NORMAL * 8,
	DPSDK_PB_NORMAL_FAST16	= DPSDK_PB_NORMAL * 16,
	DPSDK_PB_NORMAL_SLOW2	= DPSDK_PB_NORMAL / 2,
	DPSDK_PB_NORMAL_SLOW4	= DPSDK_PB_NORMAL / 4,
	DPSDK_PB_NORMAL_SLOW8	= DPSDK_PB_NORMAL / 8,
	DPSDK_PB_NORMAL_SLOW16	= DPSDK_PB_NORMAL / 16,
}DPSDK_PLAYBACK_SPEED;

typedef enum
{
	DPSDK_FILE_FORMAT_NORMAL = 0,			// Original stream
	DPSDK_FILE_FORMAT_AVI = 1,				// avi format
	DPSDK_FILE_FORMAT_MP4 = 2,				// mp4 format
	DPSDK_FILE_FORMAT_FLV = 3,				// flv format
	DPSDK_FILE_FORMAT_ASF = 4,				// asf format
}DPSDK_DOWNLOAD_RECORD_FILE_FORMAT;

typedef enum 
{
	DPSDK_NAME_RULE_TIME_CHANNELID = 0,
	DPSDK_NAME_RULE_TIME_CHANNELNAME = 1,
	DPSDK_NAME_RULE_CHANNELID_TIME = 2,
	DPSDK_NAME_RULE_CHANNELNAME_TIME = 3
}DPSDK_RECORD_FILE_NAME_RULE;

// Parameters of fish ball linkage initialization channel
typedef struct
{
	DPSDK_INT32   iHimgWidth;                       // From the camera image width
	DPSDK_INT32   iHimgHeight;						// From the camera image
	DPSDK_INT32   *arriZoomList;					// Ball multiplier table
	DPSDK_INT32   iZoomListSize;					// The number of multiple tables of the ball machine
}DPSDK_MHFPTZ_INIT_PARAM;

typedef struct
{
	DPSDK_INT32 iWidth;
	DPSDK_INT32 iHeight;
}DPSDK_FISH_SIZE;

typedef struct
{
	DPSDK_SHORT shX;
	DPSDK_SHORT shY;
}DPSDK_FISH_POINT2D;

typedef struct
{
	DPSDK_INT32 iX;
	DPSDK_INT32 iY;
	DPSDK_INT32 iHAngle;
	DPSDK_INT32 iVAngle;
	DPSDK_INT32 iAvailable;
	DPSDK_INT32 arriReserved[3];
}DPSDK_FISH_REGIONPARAM;

typedef struct
{
	DPSDK_FISH_REGIONPARAM		arrstruRegionParam[9];		// The sequence number of an array corresponds to a window IDNumber, incoming configuration needs matching ID
	DPSDK_INT32					iCircularOffset;
	DPSDK_INT32					iPanoramaOffset;
	DPSDK_INT32					iUseRegionParam;			// When it is valid, use this value to initialize it; if you don't save the information, please set it 0
	DPSDK_INT32					arriReserved[1];
}DPSDK_FISH_MODEINITPARAM;

typedef struct
{
	DPSDK_UINT32			uiSubMountMode;			// Sub image installation mode, Only when the image master correction mode is user defined mode, This value is valid, See DPSDK_FISH_MOUNTMODE
	DPSDK_UINT32			uiSubCalibrateMode;		// Subimage correction model, Only when the image master correction mode is user defined mode, This value is valid, See DPSDK_FISH_SHOWMODES
	DPSDK_FISH_SIZE			struImgOutput;			// Subimage output resolution
	DPSDK_FISH_POINT2D		struUpperLeft;			// Subimage offset
	DPSDK_INT32				arriReserved[3];		// Reserved bytes
}DPSDK_FISH_SUBMODE;

typedef struct
{
	DPSDK_FISH_SIZE			struMainShowSize;		// Not enabled for the time being, Main display ratio, 4:3, 16:9µÈ, The optimal results are output based on the algorithm (no deformation case)Try to reach the ratio as far as possible)
	DPSDK_FISH_SIZE			struImgOutput;			// Output image resolution(Pre scale), The image master correction mode is external input when the user custom mode is used, Other modes Internal return
	DPSDK_FISH_SIZE			struFloatMainShowSize;	// Output double BufferWhen used, the main display window of the floating window is resolved for the time being with the old method of operation.The width to height ratio of the floating circle needs to be 1:1The width to height ratio of the floating Wall panorama needs to be 16:9

	DPSDK_FISH_SUBMODE*		pSubMode;				// Subpattern information, The image master correction mode is external input when the user custom mode is used, Other patterns are internally returned
	DPSDK_INT32				iSubModeNum;		    // Subpattern number, The image master correction mode is external input when the user custom mode is used, Other patterns are internally returned
	DPSDK_INT32				iOutputSizeRatio;		// Not enabled for the time being, The scaling ratio of the corrected output imageQ8.format,Range 0-256, Two hundred and fifty-sixTo maintain the maximum output resolution
	DPSDK_INT32				arriReserved[1];		// Reserved bytes
}DPSDK_FISH_OUTPUTFORMAT;

typedef struct
{	
	/*Necessary Parameters*/
	DPSDK_INT32					iZoomType;			 // Multiple Control Mode---- Expected Adaptive And Double The Size Of The Box
	DPSDK_INT32					iHCamWax;			 // The Expectation Multiplier Corresponds To The Ball Anglex (horizontal)
	DPSDK_INT32					iHCamWay;			 // The Expectation Multiplier Corresponds To The Ball Angley (vertical)
	DPSDK_INT32					iHCamWMul;			 // Expected Multiplier (benchmark Multiplier)
	DPSDK_INT32					iCfgType;			 // Configuration Method, Default Is1: Using Configuration Parameters 1:.using The Method Of Parameter Configuration, 0Use Device Type Collocation Method

	// Main Camera Parameters							         
	// Lens Parameters
	DPSDK_INT32					iPrmRE;				 // Projection Radius
	DPSDK_INT32					iPrmMul;			 // Projection Ratio
	DPSDK_INT32					iPrmDX;				 // X irection Shift
	DPSDK_INT32					iPrmDY;				 // Y irection Shift
	DPSDK_INT32					iPrmCW;				 // CMOS Wide (practical Use)
	DPSDK_INT32					iPrmCH;				 // CMOS High (high Practical Use)

	// Main Camera And Slave Type Configuration (Cfg_type is  0, it is valid to set this parameter at the time A) DefaultOne Hundred And ThirtyDegree, One Hundred And Thirty000 And Bolt200W65Speed Dome Cameras
	DPSDK_UINT32				iMLenType;			 // Main Camera Lens Type, See DPSDK_LEN_TYPE
	DPSDK_UINT32				iMCamType;			 // Main Camera Type, See DPSDK_CAM_TYPE
	DPSDK_UINT32				iHCamType;			 // From Camera Type, See DPSDK_CAM_TYPE

	// Ball Machine Parameters
	DPSDK_INT32					iHImgWidth;          // From The Camera Image Width
	DPSDK_INT32					iHImgHeight;         // From The Camera Image
	DPSDK_INT32					iPrmFax;     

	/*Default Parameters*/
	//Main Camera Parameters
	DPSDK_INT32					iMCamFC;			 // Equivalent Focal Length Of Camera
	DPSDK_INT32					iMCamCW;			 // Lens Target Height
	DPSDK_INT32					iMCamCH;			 // Wide Shot Target
	DPSDK_INT32					iCamHeight;          // Camera Height (meter), (temporarily Unused)
	DPSDK_INT32					iPrmMA;				 // Focal Length

	// From Camera Parameters
	// Ball Machine Parameters
	DPSDK_INT32					iPrmHW;				 // CMOS Width
	DPSDK_INT32					iPrmHH;				 // CMOS Height
	DPSDK_INT32					iPrmFO;				 // Equivalent Focal Length
	DPSDK_INT32					iPrmCA;				 // Field Of View Parameter
	DPSDK_INT32					iPrmMMul;			 // Maximum Ratio
}DPSDK_MHFPTZ_CONFIGPARAM;

// Fish eye parameters
typedef struct
{
	DPSDK_FISH_SIZE				struMainStreamSize;		// The original width of the main stream is high, and when the incoming resolution is different from it, it is shown that the auxiliary code stream is the scaling of this resolution
	DPSDK_INT32					iOriginX;				// The center abscissa of the fish eye circle in the input image, Normalization to 0-8192coordinate system
	DPSDK_INT32					iOriginY;				// The center longitudinal coordinates of the fish eye circle in the input image, Normalization to 0-8192coordinate system
	DPSDK_INT32					iRadius;				// The radius of the fish eye circle in the input image, Normalization to 0-8192coordinate system
	DPSDK_INT32					iLensDirection;		    // Angle of rotation, Q7format, Range 0-360*128, Generally matched 0
	DPSDK_UINT32				uiMainMountMode;		// Master installation mode,See DPSDK_FISH_MOUNTMODE
	DPSDK_UINT32				uiMainCalibrateMode;	// Image master correction model,See DPSDK_FISH_SHOWMODES
	DPSDK_FISH_MODEINITPARAM	struModeInitParam;		// The external afferent mode initializes each picture information, which is suitable for the mode switch to restore to the last state
	DPSDK_FISH_OUTPUTFORMAT*	pOutputFormat;			// Output image information
	DPSDK_MHFPTZ_CONFIGPARAM*	pConfigParam;			// Fish ball linkage configuration parameters
	DPSDK_INT32					iEnableAutoContrast;    //IN OUT/*Open automatic contrast, 0Close, 1open, This function increases the time consuming of the algorithm, It needs good performance The machine is recommended to open*/
	DPSDK_INT32              	iAlphaHistogram;        //IN OUT /*histogram IIRStrength 0-255, default 128, The bigger the current frame of reference*/
	DPSDK_INT32              	iAlphaGray;             //IN OUT/*Grayscale tensile strength 0-255, default 245, The bigger the weak contrast*/
	DPSDK_FISH_SIZE          	struCaptureSize;		//OUT/*Corresponding to the current mode of screen resolution*/
	DPSDK_INT32					iMhfptzIndex;           //IN /*Serial number of fish ball linkage ball machine 0,1,2.*/... / /At present, the client supports only one fish and one ball, so this parameter Default filling 0There will be an abnormality
	DPSDK_INT32					arriReserved[1];		// Reserved bytes
}DPSDK_FISH_OPTPARAM;

// Watchman
typedef struct 
{
public:
	DPSDK_BOOL  bAutoTrack;			//open¡¢close device autoTrack
	DPSDK_INT32 iObjectID;			//ivs object id
	DPSDK_INT32 iToken;				//token  useless
	DPSDK_INT32 iDirection;          //rectlocate direction
	DPSDK_INT32 x;					//locate x or rect locate x0
	DPSDK_INT32 y;					//locate y or rect locate y0
	DPSDK_INT32 x1;					//rect locate x1
	DPSDK_INT32 y1;					//rect locate y1
}DPSDK_SMARTTRACK_OPTPARAM;

// EPTZ parameter
typedef struct
{
	DPSDK_UINT32				uiPtzCmd;			// The operation of the cloud platform shows that DPSDK_FISH_EPTZCMD definition
	DPSDK_INT32					iWinId;				// To carry onEptzWindow number, upper left cornerWinIdÎª0Increase from left to right							
	DPSDK_INT32					iArg1;
	DPSDK_INT32					iArg2;
	DPSDK_INT32					iArg3;
	DPSDK_INT32					iArg4;
	DPSDK_INT32					iArg5;
	DPSDK_INT32					iArg6;
	DPSDK_INT32					arriReserved0[6];	// Reserved bytes
	DPSDK_LPVOID				pParam;				// Fish ball linkage
	DPSDK_LPVOID				pResult;
	DPSDK_LPVOID				pArg;   
	DPSDK_INT32					arriReserved1[7];	// Reserved bytes
}DPSDK_FISH_EPTZPARAM;

// External configuration from camera parameters(Fish ball linkage)
typedef struct
{
	DPSDK_INT32				iHCamWax;				// The expectation multiplier corresponds to the dome angle x(horizontal)
	DPSDK_INT32				iHCamWay;				// The expectation multiplier corresponds to the dome angle y (vertical)
	DPSDK_INT32				iHCamWMul;				// Expectation multiplier (benchmark multiplier)
	DPSDK_UINT32			uiHCamType;				// From camera type,See DPSDK_CAM_TYPE
}DPSDK_SUBORDINATE_CAMCONFIGPARAM;

// Fish parameter
typedef struct
{
	DPSDK_INT32						iFitmode; 
	DPSDK_INT32						iDisplaymode; 
	DPSDK_INT32						iWidth; 
	DPSDK_INT32						iHeight; 
	DPSDK_INT32						iOriginX; 
	DPSDK_INT32						iOriginY; 
	DPSDK_INT32						iRadius;
	DPSDK_INT32						iWidthRatio; 
	DPSDK_INT32						iHeightRatio; 
	DPSDK_SUBORDINATE_CAMCONFIGPARAM struSubCamConfigParam;
}DPSDK_FISH_PARAMS;

typedef struct
{
	DPSDK_INT32						iCircleX;			// The center abscissa of the fish eye circle in the input image		
	DPSDK_INT32						iCircleY;			// The center longitudinal coordinates of the fish eye circle in the input image
	DPSDK_INT32						iRadius;			// The radius of the fish eye circle in the input image
	DPSDK_LONG						lWidthRatio;		// The original width of the main stream
	DPSDK_LONG						lHeightRatio;		// The original height of the main stream
}DPSDK_FISH_UPDATE_PARAM;

/**	
* @brief	Fisheye Data Callback Function
* @param [OUT] uszCorrectMode		Correction Mode
* @param [OUT] uRadius				Radius [0,8192)
* @param [OUT] uCircleX				Abscissa of Circle Center
* @param [OUT] uCircleY				Ordinate of Circle Center
* @param [OUT] uWidthRatio			Width Ratio
* @param [OUT] uHeigthRatio			Height Ratio
* @param [OUT] uszGain				Gain
* @param [OUT] uszDenoiseLevel		Noise Reduction Level
* @param [OUT] uszInstallStyle		Fisheye Installation Type
* @param [OUT] pUserData			User Data
* @return
*/
typedef DPSDK_VOID (DPSDK_CALL *DPSDK_FISHEYE_CALLBACK)(DPSDK_UCHAR uszCorrectMode,
														DPSDK_USHORT uRadius,
													    DPSDK_USHORT uCircleX,
														DPSDK_USHORT uCircleY,
														DPSDK_UINT32 uWidthRatio,
														DPSDK_UINT32 uHeigthRatio,
														DPSDK_UCHAR uszGain,
														DPSDK_UCHAR uszDenoiseLevel,
														DPSDK_UCHAR uszInstallStyle,
														DPSDK_LPVOID pUserData);

/**	
* @brief	Video Drawing Callback Function
* @param [OUT] hDc			Drawing Handle
* @param [OUT] pWnd			Window Handle
* @param [OUT] pUserData	User Data
* @return
*/
typedef DPSDK_VOID (DPSDK_CALL* DPSDK_DRAW_CALLBACK)(DPSDK_HDC hDc, 
													 HCWND pWnd, 
													 DPSDK_LPVOID pUserData);

/**	
* @brief	Data Callback Function after Source Data Analysis
* @param [OUT] pUserData	User Data
* @param [OUT] iEncode		MPEG4, H264, STDH264
* @return
*/
typedef DPSDK_VOID (DPSDK_CALL* DPSDK_DEMUXDEC_CALLBACK)(DPSDK_LPVOID pUserData, 
														 DPSDK_INT32 iEncode);

/**
* @brief	Media Stream Callback Function
* @param	[IN] iMediaType Corresponding Business of Media Stream
* @param	[IN] pData		Media Stream Data
* @param	[IN] iDataLen	Data Length
* @param	[IN] pUserParam	User Parameter
* @return	Returned value is 0 in case of success
*/
typedef DPSDK_INT32 (DPSDK_CALL* DPSDK_REALDATA_CALLBACK)(DPSDK_INT32 iMediaType, 
														  DPSDK_CHAR* pData, 
														  DPSDK_INT32 iDataLen,
														  DPSDK_VOID* pUserParam);

/**
* @brief	Callback Function of Playback on TV Wall
* @param	[IN] pData		Media Stream Data
* @param	[IN] iDataLen	Data Length
* @param	[IN] pUserParam	User Parameter
* @return	Returned value is 0 in case of success
*/
typedef DPSDK_INT32(DPSDK_CALL* DPSDK_TVWALL_PLAYBACK_CALLBACK)(DPSDK_CHAR* pData,
																DPSDK_INT32 iDataLen,
																DPSDK_VOID* pUserParam);

/**
* @brief	Media Event Callback Function
* @param	[IN] iEventType		 Event Type
* @param	[IN] iMediaSessionID Media Session ID
* @param	[IN] pUserParam		 User Parameter
* @return	Returned value is 0 in case of success
*/
typedef DPSDK_VOID(DPSDK_CALL* DPSDK_EVENT_CALLBACK)(DPSDK_INT32 iEventType,
													 DPSDK_INT32 iMediaSessionID,
													 DPSDK_VOID* pUserParam);

/**
* @brief	Record Event Callback Function
* @param	[IN] iEventType		 Event Type
* @param	[IN] iMediaSessionID Media Session ID
* @param	[IN] pData			 The information of record callback, see DPSDK_DOWNLOAD_NOTIFY
* @param	[IN] pUserParam		  User Parameter
* @return	Returned value is 0 in case of success
*/
typedef DPSDK_VOID(DPSDK_CALL* DPSDK_EVENT_DOWNLOAD_CALLBACK)(DPSDK_INT32 iEventType,
															  DPSDK_INT32 iMediaSessionID,
															  DPSDK_VOID* pData, 
															  DPSDK_VOID* pUserParam);

/**
* @brief	Local Record Event Callback Funciton
* @param	[IN] iEventType		 Event Type
* @param	[IN] iMediaSessionID Media Session ID
* @param	[IN] pUserParam		 User Parameter
* @return	Returned value is 0 in case of success
*/
typedef DPSDK_VOID(DPSDK_CALL* DPSDK_EVENT_LOCALPLAY_CALLBACK)(DPSDK_INT32 iEventType,
															   DPSDK_INT32 iMediaSessionID,		
															   DPSDK_VOID* pUserParam);

/**
* @brief	IVS Data Callback Funciton
* @param	[IN] pData		Media Stream Data
* @param	[IN] iDataLen	Data Length
* @param	[IN] lRealLen	Real Length
* @param	[IN] pReserved	Reserved Param
* @param	[IN] pUserData		 User Data
* @return	Returned value is 0 in case of success
*/
typedef DPSDK_VOID(DPSDK_CALL* DPSDK_IVSDATA_CALLBACK)(DPSDK_CHAR* pData,
													   DPSDK_LONG lType,
													   DPSDK_LONG lDateLen,
													   DPSDK_LONG lRealLen,
													   DPSDK_VOID* pReserved,
													   DPSDK_VOID* pUserData);
															   
// Basic video parameters
typedef struct
{
	HCWND			pHWnd;								// Window handle
	//Basic video parameters
	DPSDK_CHAR		szCodeId[DPSDK_DEVICE_ID_LEN];		// Channel ID Or equipment ID
	DPSDK_CHAR		szDeviceCode[DPSDK_DEVICE_ID_LEN];	// Device code for request media key
	DPSDK_INT32		iStreamType;						// Code stream type 1=Main stream, 2=Auxiliary code stream
	DPSDK_INT32		iDataType;							// Video type:1=video, 2=audio frequency, 3=Audio and video
	DPSDK_INT32		iDecodeType;						// Decode type See DPSDK_DECODE_TYPE Definition
	DPSDK_INT32		iStreamMode;						// Playback mode See DPSDK_STREAM_MODE Definition
	DPSDK_UINT32	uiDelayTime;						// Play delay time, when IstreamMode is  DPSDK_STREAM_CUSTOM_MODETime, it is  effective Company MS
}DPSDK_MEDIA_BASE_PARAM;

// DPSDK_MEDIA_CALLBACK
typedef struct
{
	DPSDK_REALDATA_CALLBACK	fRealDataCallBack;			// Bitstream callback
	DPSDK_LPVOID	pRealUserData;						// Code stream callback user data

	DPSDK_FISHEYE_CALLBACK fFishEyeCallBack;			// Fish eye data callback
	DPSDK_LPVOID	pFishEyeUserData;					// Fish eye data callback user data

	DPSDK_DRAW_CALLBACK		fDrawCallBack;				// Video plotting callback
	DPSDK_LPVOID	pDrawUserData;						// Video plotting callback user data

	DPSDK_DEMUXDEC_CALLBACK	fDemuxDecCallBack;			// Data callback for the analysis of source data
	DPSDK_LPVOID	pDemuxDecUserData;					// Data callback to user data analyzed by source data

	DPSDK_EVENT_CALLBACK fEventCallBack;				// Event callbacks
	DPSDK_LPVOID	pEventUserData;						// Event callback user data

	DPSDK_TVWALL_PLAYBACK_CALLBACK fTVWallPlaybackCallBack; // Replay the back wall callback
	DPSDK_LPVOID	pTVWallPlaybackUserData;				// Playback the upper wall callback user data

	DPSDK_EVENT_LOCALPLAY_CALLBACK	fEventLocalCallBack;// Local Record Event Callback
	DPSDK_LPVOID					pEventLocalUserData;// Local Record Event Callback user data

	DPSDK_IVSDATA_CALLBACK			fIVSDataCallBack;
	DPSDK_LPVOID					pIVSUserData;		// Local Record Event Callback user data
}DPSDK_MEDIA_CALLBACK;

// Unicast video parameters
typedef struct  
{
	DPSDK_MEDIA_BASE_PARAM	struMediaBaseParam;			// Basic video parameters
	DPSDK_MEDIA_CALLBACK	struMediaCallBack;			// Video callback structure
	
	//Transcoding parameter
	DPSDK_INT32		iUsedVcs;							// Whether the tag needs to pass throughVCSTranscoding.0It means that there is no need for transcoding;1It means that transcoding is required
	DPSDK_CHAR		szVideoCode[DPSDK_VIDEO_PARAM_LEN];	// Video coding format, reference video coding format to define strings
	DPSDK_CHAR		szResolution[DPSDK_VIDEO_PARAM_LEN];// Code stream resolution, reference stream resolution definition string
	DPSDK_INT32		iFps;								// Frame rate
	DPSDK_INT32		iBps;								// Bit stream code stream
}DPSDK_REALPLAY_PARAM;

//TCP link way
typedef enum
{
	DPSDK_TCPMODE_DEFAULT = 0,							// Default link way
	DPSDK_TCPMODE_ACTIVE = 1,							//tcp active link way
	DPSDK_TCPMODE_PASSIVE = 2							//tcp passive link way
}DPSDK_TCP_MODE;

typedef struct
{
	DPSDK_BOOL      bMuticast;							// tag of multicast: true multicast, false Unicast	
	DPSDK_CHAR		szSSrc[DPSDK_IP_LEN];					
	DPSDK_CHAR		szSrcIp[DPSDK_IP_LEN];				
	DPSDK_INT32		iSrcPort;							
}DPSDK_RTSP_INFO;

// Video parameter //for new Protocol:28181
typedef struct
{
	DPSDK_MEDIA_CALLBACK	struMediaCallBack;			// Video callback function
	DPSDK_CHAR		szCodeId[DPSDK_DEVICE_ID_LEN];		// Channel id or device id
	DPSDK_INT32		iStreamType;						// Code stream type 1=Main stream, 2=Auxiliary code stream
	DPSDK_INT32		iDataType;							// Video type:1=video, 2=audio frequency, 3=Audio and video
	DPSDK_INT32		iTransMode;							// 0:UDP, 1:TCP
	DPSDK_CHAR		szDstIp[DPSDK_IP_LEN];				// Destination IP
	DPSDK_INT32		iDstPort;							// Destination port
	DPSDK_CHAR		szTrcakID[DPSDK_TRACK_ID_LEN];		// TrackID
	DPSDK_INT32		iRtpHeadType;						// 1TRP, 2 NONE
	DPSDK_CHAR		szDevCode[DPSDK_DEVICE_ID_LEN];		// Device code
	DPSDK_TCP_MODE	iTcpMode;							// 0,Default link way£¬1active link way£¬2passive link way
}DPSDK_REALPLAY_TO_IP_PARAM;

// Multicast parameter
typedef struct  
{
	DPSDK_MEDIA_BASE_PARAM struMediaBaseParam;			// Basic video parameters
	DPSDK_MEDIA_CALLBACK struMediaCallBack;				// Video callback structure

	DPSDK_CHAR		szTrackId[DPSDK_VIDEO_PARAM_LEN];	// Track ID
}DPSDK_MULITCAST_REALPLAY_PARAM;

// Multi screen preview video parameters
typedef struct  
{
	DPSDK_MEDIA_BASE_PARAM	struMediaBaseParam;			// Basic video parameters
	DPSDK_MEDIA_CALLBACK	struMediaCallBack;			// Video callback structure

	DPSDK_CHAR		szTrackId[DPSDK_VIDEO_PARAM_LEN];	// Track ID

	// Multi picture preview
	DPSDK_INT32		iScreenNum;							// Multi picture segmentation number
	DPSDK_INT32		iStartChnlIndex;					// Starting channel
}DPSDK_MULITVIEW_REALPLAY_PARAM;

// Video status
typedef enum
{
	RECORD_STATUS_IDLE		= 0,						// Video does not make it possible
	RECORD_STATUS_NORMAL	= 1,						// In the ordinary video
	RECORD_STATUS_EXCEPTION = 2,						// Abnormal
	RECORD_STATUS_MANUAL	= 3,						// Manual video is being triggered
}DPSDK_RECORD_STATUS;

// Channel video information
typedef struct
{
	DPSDK_CHAR szChannelCode[DPSDK_CHANNEL_ID_LEN];		// Channel coding
	DPSDK_INT32 iChannelSeq;							// Channel number
	DPSDK_INT32 iRecordStatus;							// Video status SeeDPSDK_RECORD_STATUSDefinition
	DPSDK_INT32 iFlow;									// Average flow rate (Kbps£©
	DPSDK_INT32 iStreamType;							// Code stream type See DPSDK_STREAM_TYPEDefinition
	DPSDK_INT32 iUsedCapacity;							// Used storage capacity
}DPSDK_RECORD_STATUS_INFO;

typedef struct
{
	DPSDK_UINT32				uTagId;						// The value of tag unique id is 0 when add
	DPSDK_TIMET					tTagTime;					// The video time of marking
	DPSDK_UINT32				uOwnerId;					// User ID
	DPSDK_SOURCE_TYPE			iSourceType;				// Video source 
	DPSDK_CHAR					szCameraId[DPSDK_CHANNEL_ID_LEN];			// Camera ID   
	DPSDK_CHAR					szSubject[DPSDK_RECORD_TAG_SUBJECT_LEN];	// The title of tag(name)
	DPSDK_CHAR					szContent[DPSDK_RECORD_TAG_CONTENT_LEN];	// The content of tag
	DPSDK_CHAR					szUrl[DPSDK_URL_LEN];						// The URL address of save picture
}DPSDK_ADD_RECORD_TAG_PARAM;

typedef struct  
{
	DPSDK_CHAR						szCameraId[DPSDK_CHANNEL_ID_LEN];		// Camera ID
	DPSDK_SOURCE_TYPE				iSourceType;							// Video source, platform record or device record
	DPSDK_TIMET						tStartTime;								// Star time
	DPSDK_TIMET						tEndTime;								// End time
	DPSDK_CHAR						szKeyword[DPSDK_KEY_WORD_LEN];			// Keyword
}DPSDK_QUERY_RECORD_TAG_PARAM;

typedef struct  
{
	DPSDK_UINT32					uTagId;										// Video tag ID
	DPSDK_SOURCE_TYPE				iSourceType;								// Video source, platform video or device video
	DPSDK_TIMET						tTagTime;									// Marking time
	DPSDK_CHAR						szCameraId[DPSDK_CHANNEL_ID_LEN];			// CameraID
	DPSDK_CHAR						szSubject[DPSDK_RECORD_TAG_SUBJECT_LEN];	// The title of tag(name)
	DPSDK_CHAR						szContent[DPSDK_RECORD_TAG_CONTENT_LEN];	// The content of tag
	DPSDK_CHAR						szUrl[DPSDK_URL_LEN];						// The URL address of save picture
}DPSDK_SINGLE_RECORD_TAG_INFO;

typedef struct
{
	DPSDK_CHAR						szCameraId[DPSDK_CHANNEL_ID_LEN];			// Camera ID
	DPSDK_INT32						iRetCount;									// The number of videos returned
	DPSDK_SINGLE_RECORD_TAG_INFO	struSingleRecordTagInfo[1];					// The information of video tag
}DPSDK_RECORD_TAG_INFO_LIST;

typedef struct  
{
	DPSDK_CHAR						szCameraId[DPSDK_CHANNEL_ID_LEN];			// Camera ID
	DPSDK_SOURCE_TYPE				iSourceType;								// Video source, platform video or device video
	DPSDK_TIMET						tStartTime;									// The start time of lock
	DPSDK_TIMET						tEndTime;									// The end time of lock
	DPSDK_CHAR						szReason[DPSDK_LOCK_RECORD_REASON_LEN];		// The reason of lock
}DPSDK_LOCK_RECORD_PARAM;

// Lock record file
typedef struct
{
	DPSDK_CHAR						szCameraId[DPSDK_CHANNEL_ID_LEN];			// Camera ID
	DPSDK_CHAR						szFilename[DPSDK_RECORD_FILE_NAME_LEN];		// The name of the video (different manufacturers are different in the identification of the documents)
}DPSDK_LOCK_RECORD_FILE_PARAM;

// Unlock record file
typedef struct
{
	DPSDK_CHAR						szCameraId[DPSDK_CHANNEL_ID_LEN];			// Camera ID
	DPSDK_CHAR						szFilename[DPSDK_RECORD_FILE_NAME_LEN];		// The name of the video (different manufacturers are different in the identification of the documents)
	DPSDK_BOOL						bForce;										// Whether or not compulsory
}DPSDK_UNLOCK_RECORD_FILE_PARAM;

// Locking or unlocking the results of video files
typedef struct
{
	DPSDK_INT32 iLockNum;														// Lock number
}DPSDK_LOCK_RECORD_FILE_RESULT;

typedef struct  
{
	DPSDK_CHAR						szUserId[DPSDK_USER_ID_LEN];				// User ID
	DPSDK_CHAR						szCameraId[DPSDK_CHANNEL_ID_LEN];			// Camera ID
	DPSDK_SOURCE_TYPE				iSourceType;								// Video source, platform video or device video
	DPSDK_TIMET						tStartTime;									// The start time of lock
	DPSDK_TIMET						tEndTime;									// The unlock time of lock
	DPSDK_CHAR						szReason[DPSDK_LOCK_RECORD_REASON_LEN];		// The reason of lock
}DPSDK_QUERY_LOCK_RECORD_PARAM;

typedef struct  
{
	DPSDK_INT32						iLockId;									// Video lock ID
	DPSDK_CHAR						szUserId[DPSDK_USER_ID_LEN];				// Lock user
	DPSDK_SOURCE_TYPE				iSourceType;								// Video source, platform video or device video
	DPSDK_TIMET						tOperateTime;								// Operate time
	DPSDK_CHAR						szCameraId[DPSDK_CHANNEL_ID_LEN];			// Camera ID
	DPSDK_CHAR						szUserIp[DPSDK_IP_LEN];						// User IP
	DPSDK_CHAR						szUserName[DPSDK_NAME_LEN];					// User name
	DPSDK_TIMET						tStartTime;									// The start time of lock
	DPSDK_TIMET						tEndTime;									// The end time of lock
	DPSDK_CHAR						szReason[DPSDK_LOCK_RECORD_REASON_LEN];		// The reason of lock
	DPSDK_CHAR						szFileName[DPSDK_RECORD_FILE_NAME_LEN];		// The name of record file
}DPSDK_SINGLE_RECORD_LOCK_INFO;

typedef struct  
{
	DPSDK_CHAR						szCameraID[DPSDK_CHANNEL_ID_LEN];			// Camera ID
	DPSDK_INT32						iRetCount;									// The number of returned
	DPSDK_SINGLE_RECORD_LOCK_INFO	struSingleRecordLockInfo[1];				// Used to save the informations of video lock which are query, the size of it depend on iReqCount
}DPSDK_RECORD_LOCK_INFO_LIST;

typedef struct  
{
	DPSDK_CHAR							szCameraId[DPSDK_CHANNEL_ID_LEN];			// Channel ID
	DPSDK_SOURCE_TYPE					iSourceType;								// Video source type
	DPSDK_INT32							iYear;										// Year
	DPSDK_INT32							iMonth;										// Month
}DPSDK_QUERY_RECORD_DATE_PARAM;

typedef struct
{
	DPSDK_INT32		RecordDays[DPSDK_DAY_IN_MONTH];	// The record is video taped. 0 start for the first day
}DPSDK_RECORD_DATE_INFO;

typedef struct
{
	DPSDK_CHAR							szAlarmCode[DPSDK_CHANNEL_ID_LEN];			// 	Alarm ID
}DPSDK_QUERY_ALARM_RECORD_PARAM;

// Query record information
typedef struct
{
	DPSDK_CHAR							szCameraId[DPSDK_CHANNEL_ID_LEN];			// Channel ID
	DPSDK_STREAM_TYPE					iStreamType;								// Code stream type
	DPSDK_SOURCE_TYPE					iSourceType;								// Video source type
	DPSDK_RECORD_TYPE					iRecordType;								// Video type
	DPSDK_TIMET							tBeginTime;									// Start time
	DPSDK_TIMET							tEndTime; 									// End time
}DPSDK_QUERY_RECORD_PARAM;

typedef struct
{
	DPSDK_SOURCE_TYPE	iSourceType;							// Video source
	DPSDK_RECORD_TYPE	iRecordType;							// Video type. See RecordType_e
	DPSDK_TIMET		iStartTime;									// Start time
	DPSDK_TIMET		iEndTime;									// End time						
	DPSDK_CHAR		szName[DPSDK_RECORD_FILE_NAME_LEN];			// The name of the video (different manufacturers are different in the identification of the documents)
	DPSDK_INT64		iLength;									// File length, unit KB
	DPSDK_STREAM_TYPE	iStreamType;							// Code stream type

																// Here's the information needed for the center video
	DPSDK_INT64		iPlanId;									// Video plan ID
	DPSDK_INT32		iSSId;										// Storage service ID			
	DPSDK_CHAR		szDiskId[DPSDK_DISDK_ID_LEN];				// Disk ID
	DPSDK_INT32		iFileHandle;								// File handle				
	DPSDK_CHAR		szChannelCode[DPSDK_CHANNEL_ID_LEN];		// Channel coding
	DPSDK_BOOL		bRecordHidden;								// Video hiding state True: concealment £»False Visible
	DPSDK_BOOL		bForgotten;									// Do you forget to forget the video

																// The informations of alarm video what are add
	DPSDK_CHAR		szAlarmChannelId[DPSDK_ALARM_CHANNEL_ID_LEN];	// Video camera ID

	DPSDK_BOOL bLocked;											// Whether or not to be locked,Device video will not be locked
}DPSDK_SINGLE_RECORD_INFO;

// Record information
typedef struct
{
	DPSDK_CHAR		szCameraId[DPSDK_CHANNEL_ID_LEN];			// Channel ID
	DPSDK_UINT32	iRetCount;									// Return the number of records, that is record number of recorded video records
	DPSDK_UINT32	iPlaybackType;								// the type of playback eg.time\file\both
	DPSDK_SINGLE_RECORD_INFO	struSingleRecord[1];			// Video recording information
}DPSDK_RECORD_INFO_LIST;

// Playback parameter
typedef struct
{
	DPSDK_MEDIA_CALLBACK	struMediaCallBack;			// Video callback function

	HCWND			pHWnd;								// Window handle
	DPSDK_INT32		iDirection;							// Playback direction See DPSDK_PLAY_DIRECTION Definition
	DPSDK_INT32		iDecodeType;						// Decode type See DPSDK_DECODE_TYPE Definition

	DPSDK_CHAR		szCodeId[DPSDK_CHANNEL_ID_LEN];		// Channel id or device id
	DPSDK_CHAR		szDeviceCode[DPSDK_DEVICE_ID_LEN];
	DPSDK_TIMET		tBeginTime;							// Start time(time stamp)
	DPSDK_TIMET		tPlayTime;							// Start playing time
	DPSDK_TIMET		tEndTime;							// End time(time stamp)
	DPSDK_INT32		iRecordSource;						// Video source, see see DPSDK_SOURCE_TYPE
	DPSDK_INT32		iStreamType;						// Code stream type, see see DPSDK_STREAM_TYPE
	DPSDK_INT32		iRecordType;						// Video type, see see DPSDK_RECORD_TYPE
}DPSDK_PLAYBACK_BY_TIME_PARAM;

typedef enum
{
	PB_MODE_NORMAL	 = 0,								// Ordinary video
	PB_MODE_DOWNLOAD = 1,								// Down load
}DPSDK_PLACK_MODE;

// Playback by time parameter //for new Protocol:28181
typedef struct
{
	DPSDK_MEDIA_CALLBACK	struMediaCallBack;			// Video callback function
	DPSDK_CHAR		szCodeId[DPSDK_CHANNEL_ID_LEN];		// Channel id or device id
	DPSDK_INT32		iRecordSource;						// Video source, see DPSDK_SOURCE_TYPE
	DPSDK_PLACK_MODE iPlayBackMode;						// 0£¬Ordinary video, 1 Down load
	DPSDK_CHAR		szDstIp[DPSDK_IP_LEN];				// Destination IP
	DPSDK_INT32		iDstPort;							// Destination port
	DPSDK_CHAR		szTrcakID[DPSDK_TRACK_ID_LEN];		// Track ID
	DPSDK_TIMET		tBeginTime;							// Start time(time stamp) 
	DPSDK_TIMET		tPlayTime;							// Start time of play
	DPSDK_TIMET		tEndTime;							// End time(time stamp) 
	DPSDK_INT32		iStreamType;						// Code stream type, see see DPSDK_STREAM_TYPE
	DPSDK_INT32		iRtpHeadType;						// 1TRP, 2 NONE
	DPSDK_PLAYBACK_SPEED iSpeed;						// Play speed
	DPSDK_CHAR		szDevCode[DPSDK_DEVICE_ID_LEN];		// Device code
	DPSDK_TCP_MODE	iTcpMode;							// 0,default link way£¬1 active link way£¬2 passive link way
	DPSDK_INT32		iDirection;							// Playback direction See DPSDK_PLAY_DIRECTION Definition
}DPSDK_PLAYBACK_TO_IP_BY_TIME_PARAM;

// Playback by file parameter//for new Protocol:28181
typedef struct
{
	DPSDK_MEDIA_CALLBACK	struMediaCallBack;			// Video callback function
	DPSDK_CHAR		szCodeId[DPSDK_CHANNEL_ID_LEN];		// Channel id
	DPSDK_INT32		iRecordSource;						// Video source, see DPSDK_SOURCE_TYPE
	DPSDK_PLACK_MODE	iPlayBackMode;					// 0£¬Ordinary video, 1 Down load
	DPSDK_CHAR		szFilename[DPSDK_RECORD_FILE_NAME_LEN];	// The name of the video (different manufacturers are different in the identification of the documents)
	DPSDK_TIMET		tBeginTime;							// Start time
	DPSDK_TIMET		tEndTime;							// End time
	DPSDK_UINT64	uSSId;								// Storage service ID(Return to the query)
	DPSDK_UINT64	uFileHandle;						// File handle(Return to the query)
	DPSDK_CHAR		szDiskId[DPSDK_DISDK_ID_LEN];		// Disk ID(Return to the query)
	DPSDK_CHAR		szDstIp[DPSDK_IP_LEN];				// Destination IP
	DPSDK_UINT32	iDstPort;							// Destination port
	DPSDK_CHAR		szTrcakID[DPSDK_TRACK_ID_LEN];		//TrackID
	DPSDK_INT32		iRtpHeadType;						// 1TRP, 2 NONE
	DPSDK_UINT64	uiFileOffset;						// The offset of playback by file, unit byte
	DPSDK_INT32		iDirection;							// Playback direction See DPSDK_PLAY_DIRECTION Definition
}DPSDK_PLAYBACK_TO_IP_BY_FILE_PARAM;

// Playback parameter
typedef struct
{
	DPSDK_MEDIA_CALLBACK	struMediaCallBack;			// Video callback function
	
	HCWND			pHWnd;								// Window handle
	DPSDK_INT32		iDirection;							// Playback direction See DPSDK_PLAY_DIRECTION Definition
	DPSDK_INT32		iDecodeType;						// Decode type See DPSDK_DECODE_TYPE Definition

	DPSDK_CHAR		szCodeId[DPSDK_CHANNEL_ID_LEN];		// Channel ID
	DPSDK_CHAR		szDeviceCode[DPSDK_DEVICE_ID_LEN];
	DPSDK_TIMET		tBeginTime;							// Start time
	DPSDK_TIMET		tEndTime;							// End time
	DPSDK_INT32		iRecordSource;						// Video source, see DPSDK_SOURCE_TYPE
	DPSDK_UINT64	uSSId;								// Storage service (IDReturn to the query)
	DPSDK_UINT64	uFileHandle;						// File handle(Return to the query)
	DPSDK_CHAR		szDiskId[DPSDK_DISDK_ID_LEN];		// Disk ID(Return to the query)
	DPSDK_CHAR		szFilename[DPSDK_RECORD_FILE_NAME_LEN];	//The name of the video (different manufacturers are different in the identification of the documents)
}DPSDK_PLAYBACK_BY_FILE_PARAM;

// Local playback parameter
typedef struct
{
	DPSDK_MEDIA_CALLBACK	struMediaCallBack;			// Video callback function

	HCWND			pHWnd;								// Window handle
	DPSDK_CHAR		szFilePath[DPSDK_FILE_PATH_LEN];	// Local playback file path
}DPSDK_PLAYBACK_BY_LOCALFILE_PARAM;

typedef struct  
{
	DPSDK_TIMET		tBeginTime;							// Start time
	DPSDK_TIMET		tEndTime;							// End time
	DPSDK_PLAYBACK_SPEED	iSpeed;						// Playback speed
	DPSDK_INT32		iDirection;							// Playback direction See DPSDK_PLAY_DIRECTION Definition
	DPSDK_INT32     iNotCheckIFrameTimeOffset;			// Check IFrameTimeOffset, 0 check, 1 not check
 }DPSDK_PLAYBACK_SEEK_PARAM;

typedef enum 
{
	TALK_TYPE_DEVICE = 1,								// Device
	TALK_TYPE_CHANNEL,									// Channel
	TALK_TYPE_DEVTODEV,									// Device to device talk
}DPSDK_TALK_TYPE;

// Audio type
typedef enum 
{
	TALK_CODING_DEFAULT = 0,							//default
	TALK_CODING_PCM = 1,								//PCM
	TALK_CODING_G711a,									//G711a
	TALK_CODING_AMR,									//AMR
	TALK_CODING_G711u,									//G711u
	TALK_CODING_G726,									//G726
	TALK_CODING_G723_53,								//G723_53
	TALK_CODING_G723_63,								//G723_63
	TALK_CODING_AAC = 8,								//add by fengjian 2012.8.8
	TALK_CODING_G722 = 101,								//G722 Used by Hk
	TALK_CODING_G711_MU,								//G711 Used by Hk
}DPSDK_AUDIO_TYPE;					

// Digits
typedef enum
{
	TALK_AUDIO_BIT_8 = 8,
	TALK_AUDIO_BIT_16 = 16,
}DPSDK_TALK_BIT;					

// Precision
typedef enum
{
	TALK_AUDIO_SAM_8K = 8000,
	TALK_AUDIO_SAM_16K = 16000,
	TALK_AUDIO_SAM_32K = 32000,
	TALK_AUDIO_SAM_48K = 48000,
	TALK_AUDIO_SAM_8192 = 8192,
}DPSDK_TALK_SAMPLE_RATE;	

// Start talk parameter
typedef struct
{
	DPSDK_CHAR szDevId[DPSDK_DEVICE_ID_LEN];			// Device code 
	DPSDK_INT32 iChnlNo;								// Channel code
	DPSDK_INT32 iTalkType;								// Talk type see DPSDK_TALK_TYPE
	DPSDK_INT32 iAudioType;								// Audio type see DPSDK_AUDIO_TYPE
	DPSDK_INT32 iTalkBit;								// Digits see DPSDK_TALK_BIT
	DPSDK_INT32 iSampleRate;							// Sample rate see DPSDK_TALK_SAMPLE_RATE

	DPSDK_BOOL bNeedDHHead;								// whether or not need dh reader
	DPSDK_BOOL bBroadCast;								// talk or broadcast
	DPSDK_EVENT_CALLBACK fEventCallBack;				// Event callback
	DPSDK_LPVOID	pEventUserData;						// The user data of event callback
}DPSDK_START_TALK_PARAM;

typedef struct
{
	DPSDK_INT32 iAudioType;								// Audio type see DPSDK_AUDIO_TYPE
	DPSDK_INT32 iTalkBit;								// Digits see DPSDK_TALK_BIT
	DPSDK_INT32 iSampleRate;							//Sample rate see DPSDK_TALK_SAMPLE_RATE
}DPSDK_START_TALK_RESULT;

// ROIData type definition
typedef struct
{
	DPSDK_INT32					iX;				// Top left corner x coordinate
	DPSDK_INT32					iY;				// Top left corner y coordinate
	DPSDK_INT32					iWidth;			// Regional width
	DPSDK_INT32					iHeight;		// Regional height
}DPSDK_IVSE_ROI;

// Input parameters of video enhancement algorithm
typedef struct
{
	DPSDK_UINT32		uiFuncType;		// Functional options See DPSDK_IVSE_FUNC_TYPE
	DPSDK_IVSE_ROI		struRoi;        // ROI To configure
	DPSDK_INT32			iMode;			// 0 Representing the picture pattern, 1 For video mode, please use the video when you use it 1Video mode
	DPSDK_INT32			iParam[2];		// Processing parameters, range[1,5]
}DPSDK_IVSE_INFO;

// Matrix coordinate of stitching algorithm 
typedef struct
{
	DPSDK_INT32					iLeft;
	DPSDK_INT32					iTop;
	DPSDK_INT32					iRight;
	DPSDK_INT32					iBottom;
	DPSDK_INT32					iX;				// Central point coordinates, used for scaling, recording positions at normal scale and narrowing to the edge
	DPSDK_INT32					iY;				// Central point coordinates, used for scaling, recording positions at normal scale and narrowing to the edge
	DPSDK_INT32					iPicWidth;		// Picture resolution-width
	DPSDK_INT32					iPicHeight;		// Picture resolution-height
}DPSDK_DISPLAY_RECT;

// Video file path
typedef struct
{
	DPSDK_UINT32		uiStoreLen;		// Video length
	DPSDK_TIMET 		lBeginTime;		// Video start time
	DPSDK_TIMET 		lEndTime;		// Video end time
	DPSDK_CHAR			szFile[DPSDK_FILE_PATH_LEN];	// Full path of video files
}DPSDK_FILE_STORE_INFO;

// Video file list
typedef struct
{
	DPSDK_UINT64				ulTotal;			// Total number of video files
	DPSDK_FILE_STORE_INFO		struFileList[1];	// Video file list
}DPSDK_FILE_STORE_LIST;

//Download parameters by time
typedef struct
{
	DPSDK_EVENT_DOWNLOAD_CALLBACK	fEventCallBack;				// Event callbacks
	DPSDK_LPVOID					pEventUserData;				// Event callback user data

	DPSDK_CHAR			szChannelID[DPSDK_CHANNEL_ID_LEN];		// Channel ID
	DPSDK_CHAR			szDeviceCode[DPSDK_DEVICE_ID_LEN];		// Device code
	DPSDK_SOURCE_TYPE	iSourceType;							// Video source
	DPSDK_STREAM_TYPE	iStreamType;							// Code stream type
	DPSDK_RECORD_TYPE	iRecordType;							// Video type
	DPSDK_TIMET			tBeginTime;								// start time
	DPSDK_TIMET			tEndTime;								// End time

	DPSDK_CHAR			szChannelName[DPSDK_CHANNEL_NAME_LEN];	// Channel name
	DPSDK_CHAR			szDownloadPath[DPSDK_FILE_PATH_LEN];	// Downloading path
	DPSDK_RECORD_FILE_NAME_RULE			iNameRule;				// Download file naming rules
	DPSDK_CHAR			szDownloadFileName[DPSDK_FILE_PATH_LEN];// Download the name of the file, if it is empty, use INameRuleThe defined rules are generated, not empty,Neglecting INameRule, szDownloadPath, szChannelNamefield
	DPSDK_INT32			iSplitFileSize;							// Division of file size, unit MB£¬0Non segmentation
	DPSDK_DOWNLOAD_RECORD_FILE_FORMAT	iFileFormat;			// Download file format
}DPSDK_DOWNLOAD_BY_TIME_PARAM;

//Download parameters by file
typedef struct  
{
	DPSDK_EVENT_DOWNLOAD_CALLBACK	fEventCallBack;				// Event callbacks
	DPSDK_LPVOID					pEventUserData;				// Event callback user data

	DPSDK_CHAR			szChannelID[DPSDK_CHANNEL_ID_LEN];		// Channel ID
	DPSDK_CHAR			szDeviceCode[DPSDK_DEVICE_ID_LEN];		// Device code
	DPSDK_SOURCE_TYPE	iSourceType;							// Video source
	DPSDK_TIMET			tBeginTime;								// Start time
	DPSDK_TIMET			tEndTime;								// End time
	DPSDK_UINT64		uSSId;									// Storage service (IDReturn to the query)
	DPSDK_UINT64		uFileHandle;							// File handle(Return to the query)
	DPSDK_CHAR			szDiskId[DPSDK_DISDK_ID_LEN];			// Disk ID(Return to the query)
	DPSDK_CHAR			szFilename[DPSDK_RECORD_FILE_NAME_LEN];	// The name of the video (different manufacturers are different in the identification of the documents)

	DPSDK_CHAR			szChannelName[DPSDK_CHANNEL_NAME_LEN];	// Channel name
	DPSDK_CHAR			szDownloadPath[DPSDK_FILE_PATH_LEN];	// Downloading path
	DPSDK_RECORD_FILE_NAME_RULE			iNameRule;				// Download file naming rules
	DPSDK_CHAR			szDownloadFileName[DPSDK_FILE_PATH_LEN];// Download the name of the file, if it is empty, use INameRuleThe defined rules are generated, not empty,Neglecting INameRule, szDownloadPath, szChannelNamefield
	DPSDK_INT32			iSplitFileSize;							// Division of file size, unit MB£¬0Non segmentation
	DPSDK_DOWNLOAD_RECORD_FILE_FORMAT	iFileFormat;			// Download file format
}DPSDK_DOWNLOAD_BY_FILE_PARAM;

typedef struct  
{
	DPSDK_INT32			iDownloadID;
	DPSDK_INT32			iFileID;
	DPSDK_INT32			iDownloadMode;
	DPSDK_INT32			iRecordSource;
	DPSDK_INT32			iRecordType;
	DPSDK_INT32			iStreamType;
	DPSDK_UINT64		uiCurFileSize;
	DPSDK_UINT64		uiPrevFileSize;
	DPSDK_TIMET			tBeginTime;
	DPSDK_TIMET			tEndTime;
	DPSDK_INT32			iDownloadState;
	DPSDK_CHAR			szChannelId[DPSDK_CHANNEL_ID_LEN];
	DPSDK_UINT64		uiFileHandle;
	DPSDK_CHAR			szDiskId[DPSDK_DISDK_ID_LEN];
	DPSDK_INT32			iDownloadStatus;
	DPSDK_INT32			iFileCount;
	DPSDK_CHAR			szDownloadFileName[1][DPSDK_FILE_PATH_LEN];
}DPSDK_DOWNLOAD_RECORD_INFO;

// Picture turn BMP format
typedef struct
{
	DPSDK_CHAR*			pBuf;			// Image data pointer
	DPSDK_LONG			lSize;			// Image data size
	DPSDK_LONG			lWidth;			// Image width
	DPSDK_LONG			lHeight;		// Image height
	DPSDK_LONG			lType;			// Image type
	DPSDK_CHAR			szFileName[DPSDK_FILE_PATH_LEN];		// File name to be saved.It is best to BMP as a file extension
}DPSDK_CONVERT_BMP;

// Picture turn jpeg format
typedef struct
{
	DPSDK_CHAR*			pBuf;			// Image data pointer
	DPSDK_LONG			lSize;			// Image data size
	DPSDK_LONG			lWidth;			// Image width
	DPSDK_LONG			lHeight;		// Image height
	DPSDK_LONG			lType;			// Image type
	DPSDK_INT32			iQuality;		// Image compression quality,region[0, 100]
	DPSDK_CHAR			szFileName[DPSDK_FILE_PATH_LEN];		// File name to be saved.It is best to jpg as a file extension
}DPSDK_CONVERT_JPEG;

// Fish data callback structure
typedef struct 
{
	DPSDK_UCHAR			uszCorrectMode;		// Correction model
	DPSDK_USHORT		uRadius;			// Radius[0,8192)
	DPSDK_USHORT		uCircleX;			// The center abscissa of circle 
	DPSDK_USHORT		uCircleY;			// The center longitudinal coordinates of circle 
	DPSDK_UINT32		uWidthRatio;		// Width ratio
	DPSDK_UINT32		uHeigthRatio;		// Height ratio
	DPSDK_UCHAR			uszGain;			// Gain
	DPSDK_UCHAR			uszDenoiseLevel;	// Denoise level
	DPSDK_UCHAR			uszInstallStyle;	// Fisheye installing mode
}DPSDK_FISH_EYE_INFO;

// Video draw callback structure
typedef struct
{
	DPSDK_HDC			hDc;		// Draw handle
	HCWND				pWnd;		// Window handle
}DPSDK_VIDEO_DRAW;

//Resolution change callback structure
typedef struct 
{
	DPSDK_INT32			iWidth;			// Image width
	DPSDK_INT32			iHeight;		// Image height
}DPSDK_ENCHANGE;

// Screen shots callback structure
typedef struct 
{
	DPSDK_LONG			lPort;		// Channel code
	DPSDK_CHAR*			pBuf;		// The data of returned image
	DPSDK_LONG			lSize;		// User data
	DPSDK_LONG			lWidth;		// Image width, unit:pixel
	DPSDK_LONG			lHeight;	// Image height
	DPSDK_LONG			lStamp;		// The information of time scale, unit:ms
	DPSDK_LONG			lType;		// Data type,T_RGB32,T_UYVY
}DPSDK_SCREENSHOT;

// Decode display callback
typedef struct  
{
	DPSDK_LONG			lPort;		// Channel code
	DPSDK_CHAR*			pBuf;		// The data of returned image
	DPSDK_LONG			lSize;		// User data
	DPSDK_LONG			lWidth;		// Image width, unit:pixel
	DPSDK_LONG			lHeight;	// Image height
	DPSDK_LONG			lStamp;		// The information of time scale, unit:ms
	DPSDK_LONG			lType;		// Data type,T_RGB32,T_UYVY
	DPSDK_LONG			lFrameRate;	// Frame rate
	DPSDK_LONG			lFrameNo;	// Frame number
}DPSDK_VISIBLEDEC;

//Event callback parameter structure
typedef struct 
{
	DPSDK_INT32			iSessionID;				// Conversation ID
	DPSDK_LPVOID		pBuf;					// Message structure
	DPSDK_UINT32		uiBufLen;				// Message structure length
}DPSDK_EVENT_PARAM;

// Card type monitoring dictionary type
typedef enum 
{
	DPSDK_LANE_NUMBER				= 19,		// Lane number
	DPSDK_TRAFFIC_DICTIONARY		= 20,		// Driving direction
	DPSDK_VEHICLE_PLATE_COLOR		= 22,		// License plate color
	DPSDK_VEHICLE_COLOR				= 2001,		// Vehicle color
	DPSDK_VEHICLE_TYPE				= 2002,		// Vehicle type
	DPSDK_PROVINCE					= 2003,		// Vehicle type
	DPSDK_VIOLATION_TYPE			= 2006,		// Violation type
	DPSDK_CONTROL_TYPE				= 2007,		// Control type
	DPSDK_CONTROL_STATUS			= 2008,		// Control state
	DPSDK_CONTROL_LEVEL				= 2009,		// Control level
	DPSDK_MONITOR_SPEED				= 2010,		// Interval velocity measurement of violation type
	DPSDK_VEHICLE_TRADEMARK			= 2016,		// Vehicle trademark
	DPSDK_VEHICLE_PLATE_TYPE		= 2017,		// License plate type
	DPSDK_VEHICLE_TYPE_STATUS		= 2018,		// Vehicle type state
	DPSDK_VEHICLE_USE_STATUS		= 2019,		// Vehicle status
	DPSDK_CREDENTIALS_TYPE			= 2027,		// Document type
	DPSDK_PROVINCE_TYPE				= 2028,		// Province type
	DPSDK_CITY_TYPE					= 2029,		// Urban type
}DPSDK_BAYONET_DICTIONARY_TYPE;

/**
* @brief	Picture Data Callback Function
* @param	[IN] iSession		 Returned Session of Corresponding Request
* @param	[IN] pData			 Picture Stream Data
* @param	[IN] iDataLen		 Data Length
* @param	[IN] pUserParam		 User Parameter
* @param	[IN] iPicEventType	 Picture Event Type
* @return	Returned value is 0 in case of success
*/
typedef DPSDK_INT32(DPSDK_CALL *DPSDK_PICDATA_CALLBACK)(DPSDK_INT32 iSession, DPSDK_CHAR* pData, DPSDK_INT32 iDataLen, DPSDK_LPVOID pUserParam, DPSDK_INT32 iPicEventType);

// Port picture monitoring parameter structure
typedef struct
{
	DPSDK_CHAR				szCodeId[DPSDK_DEVICE_ID_LEN];		// Channel id or device id
	DPSDK_UINT32			uiDataType;							// Subscription type:1=Vehicle information, 2=Vehicle information+Picture information
	DPSDK_UINT32			uiStreamType;						// Stream type:1=Main stream, 2=Auxiliary code stream

	DPSDK_PICDATA_CALLBACK  fBayPicCallBack;					// Card port picture monitoring callback
	DPSDK_LPVOID			pUserData;							// User data
}DPSDK_PICTURE_MONITOR;

// Bayonet dispatched alarm
typedef struct
{
	DPSDK_CHAR			szChannelID[DPSDK_CHANNEL_ID_LEN];			// Alarm channel ID
	DPSDK_TIMET			lAlarmTime;									// Alarm time (unit: seconds))
	DPSDK_CHAR			szPlateNumber[DPSDK_PLATE_NUMBER_LEN];		// Number plate number
	DPSDK_INT32			iPlateColor;								// License plate color coding 99-Unidentified,0-Blue,1-Yellow,2-White,3-ºÚColor,100-Other colors
	DPSDK_INT32			iSurveyType;								// Control type encoding 1-Overspeed vehicles,2-Theft of vehicles,3-Traffic accident vehicle,4-suspicion Vehicles,5-Intercepting vehicles,6-Check and check,7-Observation and tracking,8-High risk vehicles,9-White list,11-Special abnormal vehicle,12-Yellow label vehicle
	DPSDK_INT32			iCarColor;									// Vehicle color coding 0-White,1-Black,2-Red,3-Yellow,4-Silver gray£¬5-Blue,6-Green,7-Orange,8-Violet£¬9-Green,10-Pink,11-Brown,99-Unidentified,100-Other
	DPSDK_CHAR			szImgPath[DPSDK_URL_LEN];					// Snapshot the image download path
}DPSDK_CAR_SURVEY_ALARM;

// E-mail address
typedef struct
{
	DPSDK_CHAR				szEmailAddr[DPSDK_ALARM_EMAILRECEIVER_LEN];		// E-mail address
}DPSDK_EMAILADDRESS;

typedef struct
{
	DPSDK_CHAR				szID[DPSDK_ID_LEN];								// Section ID
	DPSDK_CHAR				szSectionName[DPSDK_NAME_LEN];					// Section name
}DPSDK_ITC_SECTION_INFO;

typedef struct
{
	DPSDK_UINT32			uiTotal;										// Section number
	DPSDK_ITC_SECTION_INFO* pSectionList;									// Section list
}DPSDK_ITC_SECTION_LIST;

typedef struct
{
	DPSDK_CHAR				szCarNum[DPSDK_ID_LEN];							// Car number
	DPSDK_INT32				iCarColor;	
}DPSDK_CAR_CTRL_PARAM;


//Alarm confirmation parameter
typedef struct
{
	DPSDK_CHAR				szAlarmCode[DPSDK_ALARM_ALARMCODE_LEN];			// Alarm code
	DPSDK_CHAR				szHandleUser[DPSDK_ALARM_HANDLERUSER_LEN];		// Handling human username
	DPSDK_CHAR				szHandleMessage[DPSDK_ALARM_HANDLEMESSAGE_LEN];	// Handling opinions
	DPSDK_UINT32			uiEmailRevceiverNumber;							// Alarm processing mailbox number
	DPSDK_INT32				iHandleStatus;									// Processing state (Reference resources AlarmDealWith_e)
	DPSDK_CHAR              szAlarmComment[DPSDK_ALARM_ALARMCOMMENT_LEN];
	DPSDK_EMAILADDRESS		struEmailReceiverList[1];						// Alarm processing notification mailbox list
}DPSDK_CONFIRMALARM_PARAM;

// Channel code
typedef struct
{
	DPSDK_CHAR szId[DPSDK_CHANNEL_ID_LEN];							// Channel id
}DPSDK_CHANNEL_ID;

// Device code
typedef struct
{
	DPSDK_CHAR szId[DPSDK_DEVICE_CODE_LEN];							// Device id
}DPSDK_DEVICE_ID;

//Alarm query parameters
typedef struct 
{
	DPSDK_CHAR				szBeginTime[DPSDK_ALARM_TIME_LEN];				// Start time of alarm yyyymmddhhmmss
	DPSDK_CHAR				szEndTime[DPSDK_ALARM_TIME_LEN];				// The end time of the alarm yyyymmddhhmmss
	DPSDK_CHAR				szHandleBeginTime[DPSDK_ALARM_TIME_LEN];		// Alarm processing start time yyyymmddhhmmss
	DPSDK_CHAR				szHandleEndTime[DPSDK_ALARM_TIME_LEN];			// Alarm processing end time yyyymmddhhmmss
	//DPSDK_CHAR				szDeviceId[DPSDK_ALARM_DEVICEID_LEN];			// Device ID
	//DPSDK_CHAR				szChannelId[DPSDK_ALARM_CHANNELID_LEN];			// Channel ID
	DPSDK_DEVICE_ID*		pDeviceIdList;									// Device list
	DPSDK_INT32				iDeviceIdNum;									// Device quantity
    DPSDK_CHANNEL_ID*       pChannelIdList;
	DPSDK_INT32             iChannelIdNum;
	DPSDK_CHAR				szOrgId[DPSDK_ALARM_ORGID_LEN];					// Organization node ID
	DPSDK_CHAR				szAlarmId[DPSDK_ALARM_ALARMID_LEN];				// Alarm ID
	DPSDK_CHAR				szAlarmCode[DPSDK_ALARM_ALARMCODE_LEN];			// Alarm code (specifying this condition to ignore other conditions£©
	DPSDK_CHAR				szHandleUser[DPSDK_ALARM_HANDLERUSER_LEN];		// Alarm processing person
	DPSDK_INT32				iPageSize;										// Number of alerts per page
	DPSDK_INT32				iPageNo;										// Query page number (from 1Start£©
	DPSDK_INT32				iSortType;										// Sort fields (1=Alarm time,2=Alarm type,3=Alarm level,4=report Police officer,5=Processing state£©
	DPSDK_INT32				iSortOrder;										// Sort direction (0=Ascending order,1=Descending order£©
	DPSDK_INT32*			pAlarmType;										// Alarm type (Reference resources Alarm_type_e)
	DPSDK_UINT32			uiAlarmTypeNumber;								// Number of alarm types
	DPSDK_INT32*			pAlarmGrade;									// Alarm level (Reference resources AlarmLevel_e)
	DPSDK_UINT32			uiAlarmGradeNumber;								// The number of alarm levels
	DPSDK_INT32*			pAlarmStatus;									// Alarm state (Reference resources AlarmState_e)
	DPSDK_UINT32			uiAlarmStatusNumber;							// The number of alarm states
	DPSDK_INT32*			pHandleStatus;									// Alarm processing state (Reference resources AlarmDealWith_e)
	DPSDK_UINT32			uiHandleStatusNumber;							// The number of state of the alarm processing
}DPSDK_QUERYALARM_PARAM;

//Alarm total query parameters
typedef struct
{
	DPSDK_CHAR				szBeginTime[DPSDK_ALARM_TIME_LEN];				// Start time of alarm yyyymmddhhmmss
	DPSDK_CHAR				szEndTime[DPSDK_ALARM_TIME_LEN];				// The end time of the alarm yyyymmddhhmmss
	DPSDK_CHAR				szHandleBeginTime[DPSDK_ALARM_TIME_LEN];		// Alarm processing start time yyyymmddhhmmss
	DPSDK_CHAR				szHandleEndTime[DPSDK_ALARM_TIME_LEN];			// Alarm processing end time yyyymmddhhmmss
	//DPSDK_CHAR				szDeviceId[DPSDK_ALARM_DEVICEID_LEN];			// Device ID
	//DPSDK_CHAR				szChannelId[DPSDK_ALARM_CHANNELID_LEN];			// Channel ID
	DPSDK_DEVICE_ID*		pDeviceIdList;									// Device list
	DPSDK_INT32				iDeviceIdNum;									// Device quantity
	DPSDK_CHANNEL_ID*       pChannelIdList;
	DPSDK_INT32             iChannelIdNum;
	DPSDK_CHAR				szOrgId[DPSDK_ALARM_ORGID_LEN];					// Organization node ID
	DPSDK_CHAR				szAlarmId[DPSDK_ALARM_ALARMID_LEN];				// Alarm ID
	DPSDK_CHAR				szAlarmCode[DPSDK_ALARM_ALARMCODE_LEN];			// Alarm code (specifying this condition to ignore other conditions)
	DPSDK_CHAR				szHandleUser[DPSDK_ALARM_HANDLERUSER_LEN];		// Alarm processing person
	DPSDK_INT32*			pAlarmType;										// Alarm type (Reference resources Alarm_type_e)
	DPSDK_UINT32			uiAlarmTypeNumber;								// Number of alarm types
	DPSDK_INT32*			pAlarmGrade;									// Alarm level (Reference resources AlarmLevel_e)
	DPSDK_UINT32			uiAlarmGradeNumber;								// The number of alarm levels
	DPSDK_INT32*			pAlarmStatus;									// Alarm state (Reference resources AlarmState_e)
	DPSDK_UINT32			uiAlarmStatusNumber;							// The number of alarm states
	DPSDK_INT32*			pHandleStatus;									// Alarm processing state (Reference resources AlarmDealWith_e)
	DPSDK_UINT32			uiHandleStatusNumber;							// The number of state of the alarm processing
}DPSDK_QUERYALARMCOUNT_PARAM;

// Shielded alarm parameter
typedef struct
{
	DPSDK_CHAR				szAlarmCodeSource[DPSDK_ALARM_ALARMSOURCE_LEN];	// Shielded alarm source (device alarm for device code, channel alarm as channel Code, system alarm for service code)
	DPSDK_INT32				iAlarmType;										// Shielding alarm type (Reference resources Alarm_type_e)
	DPSDK_INT32				iDuration;										// The length of the shielding time (unit: Second)
}DPSDK_BLOCKALARM_PARAM;

typedef struct
{
	DPSDK_CHAR				szLocale[DPSDK_ID_LEN];
	DPSDK_CHAR				szAlarmCode[DPSDK_ALARM_ALARMCODE_LEN];			// Alarm code
}DPSDK_CAR_ALARM_PARAM;

typedef struct
{
	DPSDK_CHAR				szAlarmCode[DPSDK_ALARM_ALARMCODE_LEN];			// Alarm code
	DPSDK_CHAR				szAlarmTime[DPSDK_ALARM_TIME_LEN];				// Alarm time
	DPSDK_CHAR				szCarColor[DPSDK_COLOR_LEN];						
	DPSDK_CHAR				szCarNum[DPSDK_ID_LEN];
	DPSDK_CHAR				szCarType[DPSDK_TYPE_LEN];
	DPSDK_CHAR				szCarImgUrl[DPSDK_URL_LEN];
	DPSDK_CHAR				szChannelId[DPSDK_ALARM_CHANNELID_LEN];
	DPSDK_CHAR				szChannelName[DPSDK_ALARM_CHANNELNAME_LEN];
	DPSDK_CHAR				szBlacklistType[DPSDK_TYPE_LEN];
	DPSDK_CHAR				szCarNumcolor[DPSDK_COLOR_LEN];
}DPSDK_CAR_BLOCKALARM_RESULT;

typedef struct
{
	DPSDK_CHAR				szAlarmCode[DPSDK_ALARM_ALARMCODE_LEN];			// Alarm code
	DPSDK_CHAR				szAlarmTime[DPSDK_ALARM_TIME_LEN];				// Alarm time
	DPSDK_CHAR				szCarColor[DPSDK_COLOR_LEN];						
	DPSDK_CHAR				szCarNum[DPSDK_ID_LEN];
	DPSDK_CHAR				szCarType[DPSDK_TYPE_LEN];
	DPSDK_CHAR				szCarImgUrl[DPSDK_URL_LEN];
	DPSDK_CHAR				szChannelId[DPSDK_ALARM_CHANNELID_LEN];
	DPSDK_CHAR				szChannelName[DPSDK_ALARM_CHANNELNAME_LEN];
	DPSDK_CHAR				szPeccancyType[DPSDK_TYPE_LEN];
	DPSDK_CHAR				szCarWayCode[DPSDK_ID_LEN];
	DPSDK_CHAR              szCarSpeed[DPSDK_ID_LEN];
}DPSDK_CAR_PECCANCYALARM_RESULT;

typedef struct
{
	DPSDK_CHAR				szAlarmCode[DPSDK_ALARM_ALARMCODE_LEN];			// Alarm code
}DPSDK_FACE_ALARM_PARAM;

typedef struct
{
	DPSDK_CHAR				szAlarmCode[DPSDK_ALARM_ALARMCODE_LEN];			// Alarm code
	DPSDK_CHAR				szAlarmTime[DPSDK_ALARM_TIME_LEN];				// Alarm time
	DPSDK_CHAR				szName[DPSDK_NAME_LEN];						
	DPSDK_CHAR				szRepositoryName[DPSDK_NAME_LEN];
	DPSDK_CHAR				szPersonId[DPSDK_USER_ID_LEN];
	DPSDK_CHAR				szPersonTypeName[DPSDK_NAME_LEN];
	DPSDK_INT32             iGender;                                        // 1:man, 2:girl
	DPSDK_CHAR				szBirthday[DPSDK_ID_LEN];
	DPSDK_CHAR				szNationality[DPSDK_NATIONALITRY_LEN];
	DPSDK_CHAR				szDetectionImageUrl[DPSDK_URL_LEN];
	DPSDK_CHAR				szRepositoryImageUrl[DPSDK_URL_LEN];
	DPSDK_CHAR              szSimilarity[DPSDK_ID_LEN];
}DPSDK_FACE_ALARM_RESULT;

// Alarm export parameters
typedef struct
{
	DPSDK_INT32				iSortType;										// Sort fields (1=Alarm time,2=Alarm type,3=Alarm level,4=AlarmTake care of people,5=Processing state)
	DPSDK_INT32				iSortOrder;										// Sort direction (0=Ascending order,1=Descending order)
	DPSDK_CHAR				szAlarmId[DPSDK_ALARM_ALARMID_LEN];				// Alarm ID
	DPSDK_CHAR				szAlarmCode[DPSDK_ALARM_ALARMCODE_LEN];			// Alarm code
	DPSDK_CHAR				szOrgId[DPSDK_ALARM_ORGID_LEN];					// Organization node ID
	//DPSDK_CHAR				szDeviceId[DPSDK_ALARM_DEVICEID_LEN];			// Device ID
	//DPSDK_CHAR				szChannelId[DPSDK_ALARM_CHANNELID_LEN];			// Channel ID
	DPSDK_DEVICE_ID*		pDeviceIdList;									// Device list
	DPSDK_INT32				iDeviceIdNum;									// Device quantity
	DPSDK_CHANNEL_ID*       pChannelIdList;
	DPSDK_INT32             iChannelIdNum;
	DPSDK_CHAR				szBeginTime[DPSDK_ALARM_TIME_LEN];				// Start time of alarm yyyymmddhhmmss
	DPSDK_CHAR				szEndTime[DPSDK_ALARM_TIME_LEN];				// The end time of the alarm yyyymmddhhmmss
	DPSDK_CHAR				szHandleBeginTime[DPSDK_ALARM_TIME_LEN];		// Alarm processing start time yyyymmddhhmmss
	DPSDK_CHAR				szHandleEndTime[DPSDK_ALARM_TIME_LEN];			// Alarm processing end time yyyymmddhhmmss
	DPSDK_CHAR				szHandleUser[DPSDK_ALARM_HANDLERUSER_LEN];		// Alarm processing person
	DPSDK_CHAR				szLanguage[DPSDK_ALARM_LANGUAGE_LEN];			// Language
	DPSDK_INT32*			pAlarmType;										// Alarm type(Reference resources Alarm_type_e)
	DPSDK_UINT32			uiAlarmTypeNumber;								// Number of alarm types
	DPSDK_INT32*			pAlarmGrade;									// Alarm level(Reference resources AlarmLevel_e)
	DPSDK_UINT32			uiAlarmGradeNumber;								// The number of alarm levels
	DPSDK_INT32*			pAlarmStatus;									// Alarm state(Reference resources AlarmState_e)
	DPSDK_UINT32			uiAlarmStatusNumber;							// The number of alarm states
	DPSDK_INT32*			pHandleStatus;									// Alarm processing state(Reference resources AlarmDealWith_e)
	DPSDK_UINT32			uiHandleStatusNumber;							// The number of state of the alarm processing
}DPSDK_ALARMEXPORT_PARAM;

// Ask for handle alarm parameters
typedef struct
{
	DPSDK_CHAR szRecordID[DPSDK_ID_LEN];									// Record ID
}DPSDK_ASK_FOR_HANDLE_PARAM;

typedef struct
{
	DPSDK_INT32 iRecordType;												// 0=pend,1=processing,2=processed
	DPSDK_CHAR	szRecordID[DPSDK_ID_LEN];									// Record ID
	DPSDK_CHAR  szOrgCode[DPSDK_ORG_CODE_LEN];								// Organization code
	DPSDK_CHAR	szOrgName[DPSDK_ORG_NAME_LEN];								// Organization name
	DPSDK_CHAR	szDeviceID[DPSDK_DEVICE_ID_LEN];							// Device ID of PAAS 
	DPSDK_CHAR	szDeviceCode[DPSDK_DEVICE_CODE_LEN];						// Device code
	DPSDK_CHAR  szDeviceName[DPSDK_ALARM_DEVICENAME_LEN];					// Device name
	DPSDK_CHAR	szChannelId[DPSDK_ALARM_CHANNELID_LEN];						// Channel ID of PAAS
	DPSDK_CHAR	szChannelCode[DPSDK_ALARM_CHANNELID_LEN];					// Channel code
	DPSDK_CHAR	szChannelName[DPSDK_CHANNEL_NAME_LEN];						// Channel name
	DPSDK_TIMET tAlarmTime;													// Alarm time
	DPSDK_INT32 iAlarmType;													// Alarm type
	DPSDK_INT32 iAlarmGrade;												// Alarm grade
	DPSDK_CHAR	szAlarmName[DPSDK_NAME_LEN];								// Alarm name
	DPSDK_INT32 iAlarmBusType;												// Business type
	DPSDK_CHAR  szAlarmBusName[DPSDK_NAME_LEN];								// Business name
	DPSDK_INT32 iAlarmTemplateId;											// Alarm template ID
	DPSDK_INT32 iAlarmTimes;												// Alarm times
}DPSDK_ALARM_INFO;

typedef struct
{
	DPSDK_UINT32					uiTotal;								// Total number of pend alarm information
	DPSDK_ALARM_INFO*				pPendAlarmList;							// Pend alarm information
}DPSDK_PEND_ALARM_LIST;

typedef struct
{
	DPSDK_UINT32					uiTotal;								// Total number of processed alarm information
	DPSDK_ALARM_INFO*				pProcessedAlarmList;					// Processed alarm information
}DPSDK_PROCESSED_ALARM_LIST;

typedef struct
{
	DPSDK_BOOL  bOther;														// false=disable edit
	DPSDK_CHAR	szContent[DPSDK_ALARM_ALARMCOMMENT_LEN];					// Alarm content
	DPSDK_CHAR	szTip[DPSDK_ALARM_ALARMCOMMENT_LEN];						// Alarm tip
}DPSDK_ALARM_TEMPLATE_DATA_DETAIL;

typedef struct
{
	DPSDK_CHAR	szCategoryName[DPSDK_NAME_LEN];								// Template type name
	DPSDK_BOOL  bRadio;														// true=single select
	DPSDK_INT32 iDetailsNum;												// Detail information number
	DPSDK_ALARM_TEMPLATE_DATA_DETAIL* pStruDetails;							// Detail information list
}DPSDK_ALARM_TEMPLATE_DATA;

typedef struct
{
	DPSDK_CHAR	szAlarmTypeId[DPSDK_ID_LEN];
	DPSDK_CHAR	szAlarmTypeName[DPSDK_NAME_LEN];
	DPSDK_INT32 iOther;														// 0:no, 1:yes
}DPSDK_CHANNEL_FACEALARM_TYPE_INFO;

typedef struct
{
	DPSDK_INT32 iNum;
	DPSDK_CHANNEL_FACEALARM_TYPE_INFO* pAlarmTypeInfo;
}DPSDK_CHANNEL_FACEALARM_TYPE_LIST;

typedef struct
{
	DPSDK_INT32 iID;														// Template ID
	DPSDK_CHAR	szTemplateName[DPSDK_NAME_LEN];								// Template name
	DPSDK_BOOL  bAlarmInRelate;												// true=relate to alarm in channel						
	DPSDK_INT32 iDataNum;
	DPSDK_ALARM_TEMPLATE_DATA* pStruData;
}DPSDK_ALARM_TEMPLATE_INFO;

typedef struct
{
	DPSDK_UINT32					uiTotal;								// Total number of processed alarm information
	DPSDK_ALARM_TEMPLATE_INFO*		pAlarmTemplateList;						// Alarm template information
}DPSDK_ALARM_TEMPLATE_LIST;

typedef struct
{
	DPSDK_INT32 iTemplateID;												// Template ID
}DPSDK_GET_TEMPLATE_PARAM;


typedef struct
{
	DPSDK_CHAR	szRecordID[DPSDK_ID_LEN];									// Record ID
	DPSDK_CHAR	szHandleContent[DPSDK_ALARM_ALARMCOMMENT_LEN];				// Handle content
}DPSDK_ALARM_ONFILE_PARAM;

typedef struct
{
	DPSDK_INT32	iSearchType;												// Search type,1=channel,2=organization,3=device
	DPSDK_CHAR	szCode[DPSDK_ALARM_CHANNELID_LEN];							// Channel,organization or device code
	DPSDK_TIMET tStartTime;													// Start time
	DPSDK_TIMET tEndTime;													// End time
	DPSDK_INT32 iAlarmType;													// Alarm type
	DPSDK_CHAR szKey[DPSDK_KEY_WORD_LEN];									// Keyword
}DPSDK_GET_HANDLE_ALARM_PARAM;

typedef struct
{
	DPSDK_ALARM_INFO struAlarmInfo;
	DPSDK_CHAR szHandleUser[DPSDK_ALARM_HANDLERUSER_LEN];					// Alarm processing person
	DPSDK_TIMET tHandleTime;												// Alarm time
	DPSDK_CHAR szHandleMessage[DPSDK_ALARM_HANDLEMESSAGE_LEN];				// Handling opinions
}DPSDK_HANDLE_ALARM_INFO;

typedef struct
{
	DPSDK_INT32 iHandleAlarmNum;											// Handle alarm number
	DPSDK_HANDLE_ALARM_INFO struHandleAlarmList[1];							// Handle alarm list
}DPSDK_HANDLE_ALARM_LIST; 

typedef struct
{
	DPSDK_INT32	iSearchType;												// Search type,1=channel,2=organization,3=device
	DPSDK_CHAR	szCode[DPSDK_ALARM_CHANNELID_LEN];							// Channel,organization or device code
	DPSDK_TIMET tStartTime;													// Start time
	DPSDK_TIMET tEndTime;													// End time
	DPSDK_INT32 iAlarmType;													// Alarm type
	DPSDK_CHAR szKey[DPSDK_KEY_WORD_LEN];									// Keyword
}DPSDK_EXPORT_HANDLE_ALARM_PARAM;

// Get binding resource parameters
typedef struct
{
	DPSDK_CHAR	szKeyCode[DPSDK_ID_LEN];									// Key code of binding resource
}DPSDK_GET_RESOURCE_BINDING_PARAM;

typedef struct
{
	DPSDK_INT32 iCustomType;												// Custom type
	DPSDK_CHAR szID[DPSDK_ID_LEN];											// Resource ID
	DPSDK_CHAR szName[DPSDK_NAME_LEN];										// Resource name
}DPSDK_RESOURCE_CODE;

typedef struct
{
	DPSDK_INT32 iBindingValueType;											// Binding value type
	DPSDK_INT32 iResourceType;												// Resource type
	DPSDK_INT32 iResourceCodeNum;											// Resource code number
	DPSDK_RESOURCE_CODE* pResourceCodeList;									// Resource code list
}DPSDK_RESOURCE_INFO;

typedef struct
{
	DPSDK_CHAR szKeyCode[DPSDK_ID_LEN];										// Key code of binding resource
	DPSDK_CHAR szKeyName[DPSDK_NAME_LEN];									// Key name
	DPSDK_INT32 iKeyType;													// Key type
	DPSDK_INT32 iResourceNum;												// Resource number
	DPSDK_RESOURCE_INFO* pResourceList;										// Resource list
}DPSDK_RESOURCE_BINDING_INFO;

typedef struct
{
	DPSDK_CHAR              szMemo[DPSDK_MEMO_LEN];	// Alarm memo data
}DPSDK_ALARM_MEMO_INFO;

// Alarm information
typedef struct
{
	DPSDK_CHAR				szAlarmId[DPSDK_ALARM_ALARMID_LEN];							// Alarm ID
	DPSDK_CHAR				szDeviceId[DPSDK_ALARM_DEVICEID_LEN];						// Device ID
	DPSDK_CHAR				szDeviceName[DPSDK_ALARM_DEVICENAME_LEN];					// Device name
	DPSDK_CHAR				szChannelId[DPSDK_ALARM_CHANNELID_LEN];						// Channel ID
	DPSDK_CHAR				szChannelName[DPSDK_ALARM_CHANNELNAME_LEN];					// Channel name
	DPSDK_INT32				iAlarmGrade;												// Alarm level(Reference resources AlarmLevel_e)
	DPSDK_INT32				iAlarmType;													// Alarm type(Reference resources Alarm_type_e)
	DPSDK_INT32				iAlarmStatus;												// Alarm state(Reference resources AlarmState_e)
	DPSDK_CHAR				szHandleUser[DPSDK_ALARM_HANDLERUSER_LEN];					// Alarm processing person
	DPSDK_CHAR				szHandleTime[DPSDK_ALARM_TIME_LEN];							// Alarm processing time yyyymmddhhmmss
	DPSDK_INT32				iHandleStatus;												// Alarm processing state(Reference resources AlarmDealWith_e)
	DPSDK_CHAR				szHandleMessage[DPSDK_ALARM_HANDLEMESSAGE_LEN];				// Handling opinions
	DPSDK_CHAR				szAlarmCode[DPSDK_ALARM_ALARMCODE_LEN];						// Alarm code
	DPSDK_CHAR				szAlarmTime[DPSDK_ALARM_TIME_LEN];							// Alarm time yyyymmddhhmmss
	DPSDK_CHAR				szAlarmPicture[DPSDK_ALARM_ALARMPICTURE_LEN];				// Alarm snapshot path
	DPSDK_UINT32			uiAlarmPictureSize;											// Alarm snapshot size
	DPSDK_UINT32			uiEmailReceiverListSize;									// The actual number of notification mailbox lists is not greater than that of the alarm DPSDK_EMAILRECEIVERLIST_SIZE
	DPSDK_EMAILADDRESS		struEmailReceiverList[DPSDK_ALARM_EMAILRECEIVERLIST_SIZE];	// Alarm processing notification mailbox list(Most return DPSDK_EMAILRECEIVERLIST_SIZEA mail address)
	DPSDK_ALARM_MEMO_INFO   struMemoList[DPSDK_ALARM_MEMO_NUM_MAX];                     // Alarm memo list
	DPSDK_INT32             iMemoNum;                                                   // Alarm memo number
	DPSDK_TIMET             tQueryTime;
}DPSDK_ALARM_DETAILINFO;

// Alarm record list
typedef struct
{
	DPSDK_TIMET					tQueryTime;				// The service time of this query,unit seconds
	DPSDK_UINT32				uiTotal;				// Total number of alarm records
	DPSDK_ALARM_DETAILINFO		struAlarmInfoList[1];	// Alarm record
}DPSDK_ALARM_DETAILINFO_LIST;

// Alarm processing record
typedef struct
{
	DPSDK_CHAR				szHandleUser[DPSDK_ALARM_HANDLERUSER_LEN];		// Alarm processing person
	DPSDK_CHAR				szHandleTime[DPSDK_ALARM_TIME_LEN];				// Alarm processing time yyyymmddhhmmss
	DPSDK_CHAR				szHandleMessage[DPSDK_ALARM_HANDLEMESSAGE_LEN];	// Warning handling opinion
	DPSDK_INT32				iHandleStatus;									// Alarm processing state(Reference resources AlarmDealWith_e)
}DPSDK_ALARMPROCESS_DETAILINFO;

// Alarm processing record list
typedef struct
{
	DPSDK_UINT32					uiTotal;						// Total number of alarm processing information
	DPSDK_ALARMPROCESS_DETAILINFO	struAlarmProcessInfoList[1];	// Alarm processing information
}DPSDK_ALARMPROCESS_DETAILINFO_LIST;

// Alarm linkage video information
typedef struct
{
	DPSDK_CHAR			szLinkVedioId[DPSDK_ALARM_CHANNELID_LEN];		// Linkage video channel ID
	DPSDK_INT32			iStreamType;									// Code stream type
	DPSDK_INT32			iScreenId;										// Screen ID
}DPSDK_ALARMLINKVEDIO_INFO;

// Alarm event (notice)
typedef struct
{
	DPSDK_CHAR					szAlarmCode[DPSDK_ALARM_ALARMCODE_LEN];				// Alarm code
	DPSDK_CHAR					szAlarmNodeCode[DPSDK_ALARM_NODECODE_LEN];			// Alarm source code
	DPSDK_CHAR					szAlarmTime[DPSDK_ALARM_TIME_LEN];					// Alarm time yyyymmddhhmmss
	DPSDK_INT32					iAlarmGrade;										// Alarm level (Reference resources AlarmLevel_e)
	DPSDK_INT32					iAlarmStatus;										// Alarm state (Reference resources AlarmState_e)
	DPSDK_INT32					iAlarmObjType;										// Alarm object type (Reference resources AlarmObject_e)
	DPSDK_INT32					iAlarmType;											// Alarm type (Reference resources Alarm_type_e)
	DPSDK_INT32					iAlarmCategory;										// Type of alarm (Reference resources AlarmCategory_e)
	DPSDK_CHAR					szAlarmMessage[DPSDK_ALARM_ALARMMESSAGE_LEN];		// Alarm extension information (for example,GPSThe extended information of the alarm includes the latitude and longitude, the heightEtc
	DPSDK_UINT32				uiAlarmLinkVedioListSize;							// Alarm video linkage information list number(Not greater than DPSDK_ALARM_LINKVEDIOINFOLIST_SIZE)
	DPSDK_ALARMLINKVEDIO_INFO	struAlarmLinkVedioList[DPSDK_ALARM_LINKVEDIOINFOLIST_SIZE];	// Alarm video linkage information list(Most returnDPSDK_ALARM_LINKVEDIOINFOLIST_SIZEVideo linkage information)
	DPSDK_CHAR					szAlarmPicture[DPSDK_ALARM_IVS_ALARMPICTURE_LEN];	// Alarm smart snapshot path
	DPSDK_UINT32				uiAlarmPictureSize;									// Alarm smart snapshot size
	DPSDK_INT32					iScreenNum;											// The number of Screen
	DPSDK_ALARM_MEMO_INFO       struMemoList[DPSDK_ALARM_MEMO_NUM_MAX];             // Alarm memo list
	DPSDK_INT32                 iMemoNum;                                           // Alarm memo number
}DPSDK_ALARMEVENT_NOTIFY;

// Alarm information (notice
typedef struct
{
	DPSDK_CHAR				szAlarmCode[DPSDK_ALARM_ALARMCODE_LEN];				// Alarm code
	DPSDK_CHAR				szAlarmTime[DPSDK_ALARM_TIME_LEN];					// Alarm time yyyymmddhhmmss
	DPSDK_CHAR				szAlarmPicture[DPSDK_ALARM_ALARMPICTURE_LEN];		// Alarm snapshot path
	DPSDK_UINT32			uiAlarmPictureSize;									// Alarm snapshot size
}DPSDK_ALARM_DETAILINFO_NOTIFY;

// Alarm confirmation (notice)
typedef struct
{
	DPSDK_CHAR				szAlarmCode[DPSDK_ALARM_ALARMCODE_LEN];						// Alarm code
	DPSDK_CHAR				szAlarmTime[DPSDK_ALARM_TIME_LEN];							// Alarm time yyyymmddhhmmss
	DPSDK_INT32				iHandleStatus;												// Processing state
	DPSDK_CHAR				szHandleMessage[DPSDK_ALARM_HANDLEMESSAGE_LEN];				// Handling opinions
	DPSDK_CHAR				szHandleUser[DPSDK_ALARM_HANDLERUSER_LEN];					// Handling human user name
	DPSDK_UINT32			uiEmailReceiverListSize;									// The actual number of notification mailbox lists is not greater than that of the alarm. DPSDK_EMAILRECEIVERLIST_SIZE£©
	DPSDK_EMAILADDRESS		struEmailReceiverList[DPSDK_ALARM_EMAILRECEIVERLIST_SIZE];	// Alarm processing notification mailbox list(Most return PSDK_EMAILRECEIVERLIST_SIZEA mail address)
	DPSDK_INT32				iAlarmGrade;												// Alarm level (Reference resources AlarmLevel_e)
	DPSDK_CHAR				szAlarmPicture[DPSDK_ALARM_IVS_ALARMPICTURE_LEN];			// Alarm smart snapshot path
	DPSDK_UINT32			uiAlarmPictureSize;											// Alarm smart snapshot size
	DPSDK_INT32				iAlarmStatus;												// Alarm state (Reference resources AlarmState_e)
	DPSDK_INT32				iAlarmType;													// Alarm type (Reference resources Alarm_type_e)
	DPSDK_CHAR				szDeviceId[DPSDK_ALARM_DEVICEID_LEN];						// Device ID
	DPSDK_INT32				iChannelSeq;												// Channel number
	DPSDK_INT32				iUnitType;													// Unit type
	DPSDK_INT32				iAlarmObjType;												// Alarm object type (Reference resources AlarmObject_e)
	DPSDK_CHAR				szAlarmNodeCode[DPSDK_ALARM_NODECODE_LEN];					// Alarm source code	
	DPSDK_ALARM_MEMO_INFO   struMemoList[DPSDK_ALARM_MEMO_NUM_MAX];						// Alarm memo list
	DPSDK_INT32             iMemoNum;													// Alarm memo number
	DPSDK_CHAR              szAlarmComment[DPSDK_ALARM_ALARMCOMMENT_LEN];
}DPSDK_ALARMCONFIRM_NOTIFY;

// Alarm export results (notice)
typedef struct
{
	DPSDK_UINT32				uiSessionId;											// Session marking
	DPSDK_CHAR					szDownloadPath[DPSDK_ALARM_ALARMEXPORTDOWNLOADPATH_LEN];// Downloading path
}DPSDK_ALARMEXPORT_RESULT_NOTIFY;

// Organization Query Node Type
typedef enum
{
	SUB_SIGNAL_CODE,												// First-level Child Node
	SUB_ALL_CODE,													// All Child Nodes
}DPSDK_SUB_CODE_TYPE;

// Unit Type
typedef enum
{
	DEV_UNIT_UNKOWN,												// Unknown
	DEV_UNIT_ENC,													// Encoding
	DEV_UNIT_DEC,													// Decoding
	DEV_UNIT_ALARMIN,												// Alarm input
	DEV_UNIT_ALARMOUT,												// Alarm output
	DEV_UNIT_TVWALLIN,												// TvWall input
	DEV_UNIT_TVWALLOUT,												// TvWall output
	DEV_UNIT_DOORCTRL,												// Access control
	DEV_UNIT_VOICE,													// Talk
	DEV_UNIT_PE								= 10,					// Power Environment (PE)
	DEV_UNIT_POS							= 11,					// POS
	DEV_UNIT_VIRTUAL						= 12,					// Virtual unit, which belongs to general intelligent server
	DEV_UNIT_ROADGATE						= 14,					// Barrier
	DEV_UNIT_LED							= 15,					// LED
	DEV_UNIT_DISPATCHER						= 33,					// Dispatcher
}DPSDK_DEV_UNIT_TYPE;

//Organization query conditions
typedef struct  
{
	DPSDK_CHAR szOrgCode[DPSDK_ORG_CODE_LEN];						// Oorganization code is the length which is the default query root organization
	//DPSDK_INT32 iSubNodeType;										// Query subnode type See DPSDK_SUB_CODE_TYPE Definition
	//DPSDK_INT32 iContainDevice;									// Include device 0=Do not contain 1=Contain
	DPSDK_INT32 iChannelTypeList[1];								// Channel type set that needs to be querying see DPSDK_DEV_UNIT_TYPEDefinition
}DPSDK_QUERY_ORG_INFO;

typedef struct
{
	DPSDK_CHAR szOrgCode[DPSDK_ORG_CODE_LEN];						// Organization code, if 0 query the root node.
	DPSDK_INT32 iChannelType;										// Channel type. Please refer to DPSDK_DEV_UNIT_TYPE. DPSDK_HAS_INT_FIELD_FALSE, it is not available.
	DPSDK_INT32 iAnaylseEnabled;									// Enable or not.1 represents enable; 0 represents disabled. DPSDK_HAS_INT_FIELD_FALSE,it is not available.
	DPSDK_INT32 iState;												// 0. Offline; 1. online. DPSDK_HAS_INT_FIELD_FALSE,it is not available.
	DPSDK_FLOAT fMinMapX;											// Min. longitude; not available beyond [-180,180]
	DPSDK_FLOAT fMaxMapX;											// Max. longitude; not available beyond [-180,180]
	DPSDK_FLOAT fMinMapY;											// Min. dimensionality; not available beyond [-90,90]
	DPSDK_FLOAT fMaxMapY;											// Max. dimensionality; not available beyond [-90,90]
	DPSDK_CHAR szKeyword[DPSDK_CHANNEL_NAME_LEN];					// Keyword; fuzzy search of channel name
}DPSDK_QUERY_FACE_CHANNEL_PARAM;

typedef struct
{
	DPSDK_INT32 iType;												// 1=face alarm,2=face snap,3=GPS alarm
	DPSDK_INT32 iRange;												// 1=all,2=part
	DPSDK_INT32 iChannelCodeNum;									// Channel number
	DPSDK_CHANNEL_ID* pChannelCodeList;								// iType=1/2 and iRange=2 is effective
	DPSDK_INT32 iDeviceCodeNum;										// Device number
	DPSDK_DEVICE_ID* pDeviceCodeList;								// iType=3 and iRange=2 is effective
}DPSDK_CONFIG_FACE_EVENT_PARAM;

typedef struct
{
	DPSDK_CHAR szDeviceCode[DPSDK_DEVICE_ID_LEN];					// Device code
	DPSDK_INT32 iDeviceType;										// Device type
	DPSDK_CHAR szChannelName[DPSDK_CHANNEL_NAME_LEN];				// Channel name
	DPSDK_CHAR szChannelCode[DPSDK_CHANNEL_ID_LEN];					// Channel code
	DPSDK_INT32 iChannelType;										// Channel type
	DPSDK_INT32 iDomainID;											// Domain ID
	DPSDK_DOUBLE fGpsX;												// Longitude
	DPSDK_DOUBLE fGpsY;												// Latitude
	DPSDK_INT32 iState;												// 0.Offling£»1.Online
	DPSDK_INT32 iAnaylseEnabled;									// Face analysis status£¬0 Stop£»1 Enable
	DPSDK_INT32 iAnaylseType;										// Face analysis type£¬0 Video stream£»1 Picture stream
	DPSDK_CHAR szOrgCode[DPSDK_ORG_CODE_LEN];						// Organization code
}DPSDK_FACE_CHANNEL_INFO;

typedef struct
{
	DPSDK_UINT32 uiTotal;											// The number of face channel
	DPSDK_FACE_CHANNEL_INFO struChannelList[1];						// The list of face channel
}DPSDK_FACE_CHANNEL_LIST;

// Organization of basic data
typedef struct
{
	DPSDK_CHAR	szOrgCode[DPSDK_ORG_CODE_LEN];						// organization Code
	DPSDK_CHAR	szOrgName[DPSDK_ORG_NAME_LEN];						// Organization name
	DPSDK_CHAR	szOrgSN[DPSDK_SN_LEN];								// organization SN code
	DPSDK_INT32	iOrgType;											// Organization node type
	DPSDK_INT32	iOrgSort;											// Organization sort
	DPSDK_TIMET tModifyTime;										// Modify time

	//
	DPSDK_CHAR	szParentCode[DPSDK_ORG_CODE_LEN];					// Organization parent node ID

	//Cloud SDK parameter
	DPSDK_CHAR	szParentName[DPSDK_ORG_NAME_LEN];					// Organization parent node name
	DPSDK_CHAR	szPlatformCode[DPSDK_ORG_CODE_LEN];					// GB ID
	DPSDK_INT32	iGroupNumber;										// Number of department under this node
	DPSDK_INT32	iDevivceNumber;										// Number of device under this node
	DPSDK_INT32 iChannelNum;										// Number of channel under this node

}DPSDK_ORG_BASE_INFO;

// Device data for organizing trees
typedef struct
{
	DPSDK_CHAR	szDeviceId[DPSDK_DEVICE_ID_LEN];					// Device ID
	DPSDK_INT32	iSort;												// Sort
}DPSDK_ORG_SUB_DEV_INFO;

// The channel data of the organization tree
typedef struct
{
	DPSDK_CHAR	szChannelId[DPSDK_CHANNEL_ID_LEN];					// Channel ID
	DPSDK_INT32	iSort;												// Sort
}DPSDK_ORG_SUB_CHANNEL_INFO;

// Basic Organizational Data
typedef struct DPSDK_ORG_INFO_T
{
 	DPSDK_ORG_BASE_INFO struOrgBaseInfo;							// Organization Info
	DPSDK_INT32 iDevNum;											// Number of Child Device
	DPSDK_ORG_SUB_DEV_INFO* pDevList;								// List of Child Device
	DPSDK_INT32 iChannelNum;										// Number of Sub-channel
	DPSDK_ORG_SUB_CHANNEL_INFO* pChannelList;						// List of Sub-channel
	DPSDK_INT32 iOrgNum;											// Number of Sub-organization
	DPSDK_ORG_INFO_T* pOrgList;										// List of Sub-organization
}DPSDK_ORG_INFO;

// Single organization data
typedef struct DPSDK_SINGLE_ORG_INFO_T
{
	DPSDK_CHAR	szOrgCode[DPSDK_ORG_CODE_LEN];						// organization code
	DPSDK_CHAR	szOrgName[DPSDK_ORG_NAME_LEN];						// Organization name
	DPSDK_CHAR	szOrgSN[DPSDK_SN_LEN];								// Organization name
	DPSDK_BOOL	bHasData;											// Whether there is direct data
	DPSDK_INT32	iOrgSort;											// Organization sort 											//
	DPSDK_CHAR	szParentCode[DPSDK_ORG_CODE_LEN];					// Organization parent node ID
}DPSDK_SINGLE_ORG_INFO;

// All Organizational Info (Recursion Tree)
typedef struct DPSDK_ALL_ORG_INFO_T
{
	DPSDK_SINGLE_ORG_INFO struOrgBaseInfo;							// Info about Organization at this Level
	DPSDK_INT32 iOrgNum;											// Number of Sub-organizations
	DPSDK_ALL_ORG_INFO_T* pOrgList;									// List of Sub-organizations
}DPSDK_ALL_ORG_INFO;

// Creat organization parameter
typedef struct
{
	DPSDK_CHAR	szOrgName[DPSDK_ORG_NAME_LEN];						// Organization name
	DPSDK_CHAR	szParentCode[DPSDK_ORG_CODE_LEN];					// Organization parent node ID
	DPSDK_CHAR	szPlatformCode[DPSDK_ORG_CODE_LEN];					//GB ID
}DPSDK_ORG_CREATE_PARAM;

// Basic Organizational Data
typedef struct
{
	DPSDK_UINT32 uiTotal;											// The total number
	DPSDK_ORG_BASE_INFO struOrgInfo[1];								// The list of organization information
}DPSDK_ORG_LIST;

// Hierarchical acquisition of device tree request parameters
typedef struct
{
	DPSDK_CHAR	szID[DPSDK_ORG_CODE_LEN];							// Node ID£¬Represent one code of orgCode,deviceCode,channelCode
	DPSDK_INT32 iNodeType;											// DPSDK_NODE_TYPE definition 1:Organization,2:Equipment,3:passageway
	DPSDK_INT32 iOrgType;											// 1: Basic organization
	DPSDK_INT32 iShowDev;											// 0: No device nodes are needed,1: Need device node
	DPSDK_INT32 iDeep;												// 2: organization+Equipment,3Organization+equipment+passageway

	DPSDK_INT32 iCategoryNum;										// Device large class list length
	DPSDK_INT32* pCategoryList;										// Device large list

	DPSDK_INT32 iChannelTypeNum;									// Channel type list length
	DPSDK_INT32* pChannelTypeList;									// Channel type set that needs to be querying See DPSDK_DEV_UNIT_TYPE Definition

	DPSDK_CHAR szKeyWord[DPSDK_MEMO_LEN];							// Search keywords
}DPSDK_GET_DEVICE_LAYERED_PARAM;

// Hierarchical acquisition of device tree node information
typedef struct
{
	DPSDK_CHAR	szID[DPSDK_ORG_CODE_LEN];							// Organization code
	DPSDK_CHAR	szName[DPSDK_ORG_NAME_LEN];							// Organization name
	DPSDK_BOOL isParent;											// Whether it is a parent node
	DPSDK_CHAR	szParentID[DPSDK_ORG_CODE_LEN];						// Parent node code
}DPSDK_LAYERED_ORG_INFO;

// Hierarchical acquisition of device tree device node information
typedef struct
{
	DPSDK_CHAR	szID[DPSDK_DEVICE_ID_LEN];							// Device code
	DPSDK_CHAR	szName[DPSDK_DEVICE_NAME_LEN];						// Device name
	DPSDK_BOOL isParent;											// Whether it is a parent node
	DPSDK_CHAR	szParentID[DPSDK_ORG_CODE_LEN];						// Parent node code

	DPSDK_INT32 iCatagory;											// Device category
	DPSDK_INT32 iSubType;											// Device subtype
	DPSDK_CHAR szIP[DPSDK_IP_LEN];									// Device IP
}DPSDK_LAYERED_DEVICE_INFO;

//Hierarchical acquisition of device tree channel node information
typedef struct
{
	DPSDK_CHAR	szID[DPSDK_DEVICE_ID_LEN];							// Device code
	DPSDK_CHAR	szName[DPSDK_DEVICE_NAME_LEN];						// Device name
	DPSDK_BOOL isParent;											// Whether it is a parent node
	DPSDK_CHAR	szParentID[DPSDK_ORG_CODE_LEN];						// Parent node code

	DPSDK_INT32 iChannelSeq;										// Channel code
	DPSDK_INT32 iUnitType;											// Unit type
	DPSDK_INT32 iChannelType;										// Channel type
}DPSDK_LAYERED_CHANNEL_INFO;

typedef enum
{
	NODE_TYPE_ORG			= 1,									// Organization
	NODE_TYPE_DEV			= 2,									// Device
	NODE_TYPE_CHANNEL		= 3,									// Channel
}DPSDK_NODE_TYPE;

// Gradation gets the result of the device tree
typedef struct
{
	DPSDK_INT32 iNodeType;											// See DPSDK_NODE_TYPE Definition 1:Organization,2:Equipment,3:passageway
	/*DPSDK_LAYERED_ORG_INFO* pNodeOrg;
	DPSDK_LAYERED_DEVICE_INFO* pNodeDevice;
	DPSDK_LAYERED_CHANNEL_INFO* pNodeChannel;*/

	DPSDK_CHAR	szID[DPSDK_ORG_CODE_LEN];							// Node ID
	DPSDK_CHAR	szName[DPSDK_ORG_NAME_LEN];							// Node name
	DPSDK_BOOL isParent;											// Whether it is a parent node
	DPSDK_CHAR	szParentID[DPSDK_ORG_CODE_LEN];						// Parent node ID
	DPSDK_INT32 iSort;												// Sort value
	DPSDK_INT32 iStatus;											// Channel state see DPSDK_DEV_STATUS Definition
	//
	DPSDK_INT32 iType1;												// iNodeType For equipment, it represents a large class of equipment.INodeTypeUnit type for channel time
	DPSDK_INT32 iType2;												// iNodeType A small class of devices is represented when the device is used.INodeTypeExpress channel type for channel
	DPSDK_CHAR szSN[DPSDK_SN_LEN];									// SN code
	// Equipment information
	DPSDK_CHAR szIP[DPSDK_IP_LEN];									// Device IP
	// Channel information
	DPSDK_INT32 iChannelSeq;										// Channel code
	DPSDK_INT32 iDomainID;											// Domain  ID
}DPSDK_LAYERED_RESULT;

// Obtain Layered List of Device Tree Returned Result
typedef struct
{
	DPSDK_INT32 iResultNum;											// List Length
	DPSDK_LAYERED_RESULT* pResultList;								// Result List
}DPSDK_LAYERED_RESULT_LIST;

typedef enum {
	DPSDK_INTELLI_NONE = 0,											// No abstract
	DPSDK_INTELLI_ABSTRACT = 0x1,									// Video abstract
}DPSDK_CHANNEL_INTELLI_STATE;

// Collection channel information
typedef struct
{
	DPSDK_CHAR szChannelName[DPSDK_CHANNEL_NAME_LEN];				// Channel name
	DPSDK_CHAR szChannelID[DPSDK_CHANNEL_ID_LEN];					// Channel ID
	DPSDK_INT32 iCameraType;										// Camera type see DPSDK_CAMERA_TYPE
	DPSDK_INT32 szPlatformID[DPSDK_ID_LEN];							// Platform ID
	DPSDK_CHAR szDeviceID[DPSDK_DEVICE_ID_LEN];						// Device ID
	DPSDK_CHAR szDeviceName[DPSDK_DEVICE_NAME_LEN];					// Device name
	DPSDK_INT32 iStatus;											// Channel status see DPSDK_DEV_STATUS
	DPSDK_INT32 iBitrate;											// The rule code rate of front configuration unit:kbps
	DPSDK_INT32 iIntelliState;										// Intelligent processing status
	DPSDK_CHAR szRegionID[DPSDK_ID_LEN];							// The id of VAL domain which channel belong to
	DPSDK_DOUBLE fLongitude;										// Longitude
	DPSDK_DOUBLE fLatitude;											// Latitude
}DPSDK_COLLECTION_CHANNEL_INFO;

// Collection channel information
typedef struct
{
	DPSDK_UINT32 uiTotal;											// The number of collection channel list
	DPSDK_COLLECTION_CHANNEL_INFO struChannelList;					// Collection channel list
}DPSDK_COLLECTION_CHANNEL_LIST;

// Add/update collection channel information
typedef struct
{
	DPSDK_CHAR szOrgCode[DPSDK_ORG_CODE_LEN];						// Organization ID
	DPSDK_CHAR szOldOrgCode[DPSDK_ORG_CODE_LEN];					// Old Organization ID, if "" add collection channel else update collection
	DPSDK_CHAR szChannelID[DPSDK_CHANNEL_ID_LEN];					// Channel ID
	DPSDK_CHAR szDeviceID[DPSDK_DEVICE_ID_LEN];						// Device ID
	DPSDK_INT32 iUnitType;											// Unit type
}DPSDK_COLLECTION_INFO;

// Add collection channel information
typedef struct
{
	DPSDK_INT32 iNum;												// The number of collection channel list
	DPSDK_COLLECTION_INFO struCollectList[1];						// Collection channel list
}DPSDK_COLLECTION_LIST;

// Add collection channel information
typedef struct
{
	DPSDK_CHAR szOrgCode[DPSDK_ORG_CODE_LEN];						// Organization ID
	DPSDK_CHAR szChannelID[DPSDK_CHANNEL_ID_LEN];					// Channel ID
	DPSDK_CHAR szChannelName[DPSDK_CHANNEL_NAME_LEN];				// Channel name
	DPSDK_INT32 iChannelSeq;										// Channel code
	DPSDK_CHAR szOrgName[DPSDK_ORG_NAME_LEN];						// Organization name
	DPSDK_CHAR szDeviceID[DPSDK_DEVICE_ID_LEN];						// Device D
	DPSDK_INT32 iUnitType;											// Unit type
	DPSDK_INT32 iChannelType;										// Channel type
	DPSDK_INT32 iCameraType;										// Camera type
	DPSDK_DOUBLE fGpsX;												// Longitude
	DPSDK_DOUBLE fGpsY;												// Latitude
	DPSDK_INT32 iMapID;												// Map
	DPSDK_INT32 iState;												// 0.Offline£»1.Online
	DPSDK_INT32 iBitrate;											// The rule code rate of front-end configuration unit:kbps
	DPSDK_INT32 iIntelliState;										// Intelligent status
}DPSDK_COLLECTION_DETAIL_INFO;

// Add collection channel information
typedef struct
{
	DPSDK_INT32 iNum;												// The number of collection channel list
	DPSDK_COLLECTION_DETAIL_INFO struCollectList[1];				// Collection channel list
}DPSDK_COLLECTION_DETAIL_LIST;

// Channel code
typedef struct
{
	DPSDK_CHAR szId[DPSDK_CHANNEL_ID_LEN];							// Channel id
}DPSDK_ID;

// Collection channel information request parameters
typedef struct
{
	DPSDK_CHAR szKeyword[DPSDK_CHANNEL_NAME_LEN];					// Keywords channel name
	DPSDK_INT32 iNodeCodeNum;										// The number of code
	DPSDK_INT32 iNodeType;											// 1.Channel£»2.Collection organization
	DPSDK_ID struNodeCodeList[1];									// The list of code
}DPSDK_COLLECTION_DETAIL_PARAM;

// Collectors' information
typedef struct DPSDK_COLLECTION_ORG_INFO_T
{
	DPSDK_CHAR szOrgCode[DPSDK_ORG_CODE_LEN];						// Organization ID
	DPSDK_CHAR szOrgName[DPSDK_ORG_NAME_LEN];						// Organization name
	DPSDK_INT32 iSort;												// Sort value
	DPSDK_BOOL bHasData;											// Whether there is direct data (non - organized nodes, can be channels)
	DPSDK_CHAR szParentCode[DPSDK_ORG_CODE_LEN];					// Parent organization ID

	DPSDK_INT32 iSubOrgNum;											// Sub organization number
	DPSDK_COLLECTION_ORG_INFO_T* pSuvOrgList;						// Sub organization list
}DPSDK_COLLECTION_ORG_INFO;

// Front-end state
typedef enum
{
	DEV_STATUS_UNDEFINE						= 0,					// Unknown
	DEV_STATUS_ONLINE						= 1,					// On-line
	DEV_STATUS_OFFLINE,												// Off-line
	DEV_STATUS_FORBID,												// Disable
}DPSDK_DEV_STATUS;

// Device type£¬match web
typedef enum
{
	DEV_TYPE_ENC_BEGIN			= 0,								// Device code
	DEV_TYPE_DVR				= DEV_TYPE_ENC_BEGIN + 1,			// DVR
	DEV_TYPE_IPC				= DEV_TYPE_ENC_BEGIN + 2,			// IPC
	DEV_TYPE_NVS				= DEV_TYPE_ENC_BEGIN + 3,			// NVS
	DEV_TYPE_MCD				= DEV_TYPE_ENC_BEGIN + 4,			// MCD
	DEV_TYPE_MDVR				= DEV_TYPE_ENC_BEGIN + 5,			// MDVR
	DEV_TYPE_NVR				= DEV_TYPE_ENC_BEGIN + 6,			// NVR
	DEV_TYPE_SVR				= DEV_TYPE_ENC_BEGIN + 7,			// SVR
	DEV_TYPE_PCNVR				= DEV_TYPE_ENC_BEGIN + 8,			// PCNVR£¬PSS self-carried small server
	DEV_TYPE_PVR				= DEV_TYPE_ENC_BEGIN + 9,			// PVR
	DEV_TYPE_EVS				= DEV_TYPE_ENC_BEGIN + 10,			// EVS
	DEV_TYPE_MPGS				= DEV_TYPE_ENC_BEGIN + 11,			// MPGS
	DEV_TYPE_SMART_IPC			= DEV_TYPE_ENC_BEGIN + 12,			// SMART_IPC
	DEV_TYPE_SMART_TINGSHEN		= DEV_TYPE_ENC_BEGIN + 13,			// Hearing host
	DEV_TYPE_SMART_NVR			= DEV_TYPE_ENC_BEGIN + 14,			// SMART_NVR
	DEV_TYPE_PRC				= DEV_TYPE_ENC_BEGIN + 15,			// Capsule
	DEV_TYPE_JT808				= DEV_TYPE_ENC_BEGIN + 18,			// JT808
	DEV_TYPE_THDVR				= DEV_TYPE_ENC_BEGIN + 19,			// Enter via third party server
	DEV_TYPE_VTT				= DEV_TYPE_ENC_BEGIN + 21,			// VTT
	DEV_TYPE_TC                 = DEV_TYPE_ENC_BEGIN + 26,			// TC( ThermalCamera )
	DEV_TYPE_DSJ				= DEV_TYPE_ENC_BEGIN + 27,			// DSJ
	DEV_TYPE_MASTERSLAVE		= DEV_TYPE_ENC_BEGIN + 34,			// Master slave tracking all-in-one IPC£¨multiple channel)
	DEV_TYPE_MCS				= DEV_TYPE_ENC_BEGIN + 35,			// Micro wave
	DEV_TYPE_WATCHER			= DEV_TYPE_ENC_BEGIN + 36,			// WATCHER(A panoramic view of PTZ)
	DEV_TYPE_SMARTWATCHER		= DEV_TYPE_ENC_BEGIN + 39,			// Smart watcher(Used in VSL)
	DEV_TYPE_IVSS				= DEV_TYPE_ENC_BEGIN + 43,			// IVSS(Used in Pro, the IVS devices which are not face server)
	DEV_TYPE_MNVR				= DEV_TYPE_ENC_BEGIN + 44,			// ³µÔØNVR
	DEV_TYPE_ENC_END,

	DEV_TYPE_TVWALL_BEGIN		= 100,
	DEV_TYPE_BIGSCREEN			= DEV_TYPE_TVWALL_BEGIN + 1,		// video wall
	DEV_TYPE_TVWALL_END,

	DEV_TYPE_DEC_BEGIN			= 200,								// Decode device
	DEV_TYPE_NVD				= DEV_TYPE_DEC_BEGIN + 1,			// NVD
	DEV_TYPE_SNVD				= DEV_TYPE_DEC_BEGIN + 2,			// SNVD
	DEV_TYPE_UDS				= DEV_TYPE_DEC_BEGIN + 5,			// UDS
	DEV_TYPE_DEC_END,

	DEV_TYPE_MATRIX_BEGIN		= 300,								// Matrix device
	DEV_MATRIX_M60				= DEV_TYPE_MATRIX_BEGIN	+ 1,		// M60
	DEV_MATRIX_NVR6000			= DEV_TYPE_MATRIX_BEGIN + 2,		// NVR6000
	DEV_TYPE_MATRIX_END,

	DEV_TYPE_IVS_BEGIN			= 400,								// IVS device
	DEV_TYPE_ISD				= DEV_TYPE_IVS_BEGIN + 1,			// ISD smart dome
	DEV_TYPE_IVS_B				= DEV_TYPE_IVS_BEGIN + 2,			// IVS-B behavior analysis service
	DEV_TYPE_IVS_V				= DEV_TYPE_IVS_BEGIN + 3,			// IVS-V video quality diagnosis service
	DEV_TYPE_IVS_FR				= DEV_TYPE_IVS_BEGIN + 4,			// IVS-FR face recognition service
	DEV_TYPE_IVS_PC				= DEV_TYPE_IVS_BEGIN + 5,			// IVS-PC Flow rate statistics service
	DEV_TYPE_IVS_M				= DEV_TYPE_IVS_BEGIN + 6,			// IVS_M master-slave tracking smart box
	DEV_TYPE_IVS_PC_BOX			= DEV_TYPE_IVS_BEGIN + 7,			// IVS-PC smart box 
	DEV_TYPE_IVS_B_BOX			= DEV_TYPE_IVS_BEGIN + 8,			// IVS-B smart box
	DEV_TYPE_IVS_M_BOX			= DEV_TYPE_IVS_BEGIN + 9,			// IVS-M box
	DEV_TYPE_IVS_PRC			= DEV_TYPE_IVS_BEGIN + 10,			// Capsule
	DEV_TYPE_IVS_F7300			= DEV_TYPE_IVS_BEGIN + 12,			// IVS-F7300
	DEV_TYPE_IVS_7500			= DEV_TYPE_IVS_BEGIN + 13,			// IVS-7500
	DEV_TYPE_IVS_IVSS			= DEV_TYPE_IVS_BEGIN + 14,			// IVSS
	DEV_TYPE_IVS_END			= 499,

	DEV_TYPE_BAYONET_BEGIN		= 500,								// -C relative device
	DEV_TYPE_CAPTURE			= DEV_TYPE_BAYONET_BEGIN + 1,		// ANPR device
	DEV_TYPE_SPEED				= DEV_TYPE_BAYONET_BEGIN + 2,		// Measure speed device
	DEV_TYPE_TRAFFIC_LIGHT		= DEV_TYPE_BAYONET_BEGIN + 3,		// Run red light device
	DEV_TYPE_INCORPORATE		= DEV_TYPE_BAYONET_BEGIN + 4,		// All in one device
	DEV_TYPE_PLATEDISTINGUISH	= DEV_TYPE_BAYONET_BEGIN + 5,		// Plate recognition device
	DEV_TYPE_VIOLATESNAPPIC		= DEV_TYPE_BAYONET_BEGIN + 6,		// Illegal parking detection device
	DEV_TYPE_PARKINGSTATUSDEV	= DEV_TYPE_BAYONET_BEGIN + 7,		// Parking detection device
	DEV_TYPE_ENTRANCE			= DEV_TYPE_BAYONET_BEGIN + 8,		// Entrance/exit device
	DEV_TYPE_VIOLATESNAPBALL	= DEV_TYPE_BAYONET_BEGIN + 9,		// Illegal parking snapshot speed dome
	DEV_TYPE_THIRDBAYONET		= DEV_TYPE_BAYONET_BEGIN + 10,		// Third parking ANPR device
	DEV_TYPE_ULTRASONIC			= DEV_TYPE_BAYONET_BEGIN + 11,		// Ultrasonic parking detector
	DEV_TYPE_FACE_CAPTURE		= DEV_TYPE_BAYONET_BEGIN + 12,		// Face snapshot device
	DEV_TYPE_ITC_SMART_NVR		= DEV_TYPE_BAYONET_BEGIN + 13,		// ANPR intelligent NVR device
	DEV_TYPE_PARKINGAREASNAP	= DEV_TYPE_BAYONET_BEGIN + 14,		// Parking zone snapshot device
	DEV_TYPE_ITC_EVS			= DEV_TYPE_BAYONET_BEGIN + 15,		// EVS storage device
	DEV_TYPE_BAYONET_END,

	DEV_TYPE_ALARM_BEGIN		= 600,								// Alarm device
	DEV_TYPE_ALARMHOST			= DEV_TYPE_ALARM_BEGIN + 1,			// Alarm controller
	DEV_TYPE_VIDEO_ALARMHOST	= DEV_TYPE_ALARM_BEGIN + 2,			// Video Alarm controller
	DEV_TYPE_ALARM_END,

	DEV_TYPE_DOORCTRL_BEGIN		= 700,
	DEV_TYPE_DOORCTRL_DOOR		= DEV_TYPE_DOORCTRL_BEGIN + 1,		// Acess
	DEV_TYPE_ACCESS_CONTROL_CENTRAL = DEV_TYPE_DOORCTRL_BEGIN + 2,	// Control central
	DEV_TYPE_ACCESS_FACE_GATE	= DEV_TYPE_DOORCTRL_BEGIN + 3,		// Face gate
	DEV_TYPE_ELEVATOR_CONTROL	= DEV_TYPE_DOORCTRL_BEGIN + 4,		// Elevatop control
	DEV_TYPE_ACCESS_CONTROL_2ND	= DEV_TYPE_DOORCTRL_BEGIN + 5,
	DEV_TYPE_DOORCTRL_END,

	DEV_TYPE_PE_BEGIN			= 800,
	DEV_TYPE_PE_PE				= DEV_TYPE_PE_BEGIN + 1,			// PE
	DEV_TYPE_PE_AE6016			= DEV_TYPE_PE_BEGIN + 2,			// AE6016 device
	DEV_TYPE_PE_NVS				= DEV_TYPE_PE_BEGIN + 3,			// NVS device with PE function
	DEV_TYPE_PE_END,

	DEV_TYPE_VOICE_BEGIN		= 900,								// Ip talk
	DEV_TYPE_VOICE_MIKE			= DEV_TYPE_VOICE_BEGIN + 1,
	DEV_TYPE_VOICE_NET			= DEV_TYPE_VOICE_BEGIN + 2,
	DEV_TYPE_VOICE_END,

	DEV_TYPE_IP_BEGIN			= 1000,								// IP device£¨connect device via network£©
	DEV_TYPE_IP_SCNNER			= DEV_TYPE_IP_BEGIN + 1,			// Scanner
	DEV_TYPE_IP_SWEEP			= DEV_TYPE_IP_BEGIN + 2,			// Sweep
	DEV_TYPE_IP_POWERCONTROL	= DEV_TYPE_IP_BEGIN + 3,			// Power controller
	DEV_TYPE_IP_END,

	DEV_TYPE_MULTIFUNALARM_BEGIN= 1100,								// Multi-functional alarm controller
	DEV_TYPE_VEDIO_ALARMHOST	= DEV_TYPE_MULTIFUNALARM_BEGIN + 1,	// Video alarm controller
	DEV_TYPE_MULTIFUNALARM_END,

	DEV_TYPE_SLUICE_BEGIN		= 1200,
	DEV_TYPE_SLUICE_DEV			= DEV_TYPE_SLUICE_BEGIN + 1,		// ANPR barrier device
	DEV_TYPE_SLUICE_PARKING		= DEV_TYPE_SLUICE_BEGIN + 2,		// Parking barrier device
	DEV_TYPE_SLUICE_STOPBUFFER	= DEV_TYPE_SLUICE_BEGIN + 3,		// Video stop buffer
	DEV_TYPE_SLUICE_END,

	DEV_TYPE_ELECTRIC_BEGIN		= 1300,
	DEV_TYPE_ELECTRIC_DEV		= DEV_TYPE_ELECTRIC_BEGIN + 1,		// Grid device
	DEV_TYPE_ELECTRIC_END,

	DEV_TYPE_LED_BEGIN			= 1400,
	DEV_TYPE_LED_DEV			= DEV_TYPE_LED_BEGIN + 1,			// LED device
	DEV_TYPE_LED_END,

	DEV_TYPE_VIBRATIONFIBER_BEGIN	= 1500,		
	DEV_TYPE_VIBRATIONFIBER_DEV	= DEV_TYPE_VIBRATIONFIBER_BEGIN + 1,// Vibration fiber device 
	DEV_TYPE_VIBRATIONFIBER_END,

	DEV_TYPE_PATROL_BEGIN		= 1600,		
	DEV_TYPE_PATROL_DEV			= DEV_TYPE_PATROL_BEGIN + 1,		// Patrol wand device
	DEV_TYPE_PATROL_SPOT		= DEV_TYPE_PATROL_BEGIN + 2,		// Patrol point device
	DEV_TYPE_PATROL_END,

	DEV_TYPE_SENTRY_BOX_BEGIN	= 1700,		
	DEV_TYPE_SENTRY_BOX_DEV		= DEV_TYPE_SENTRY_BOX_BEGIN + 1,	// Sentry box device
	DEV_TYPE_SENTRY_BOX_END,

	DEV_TYPE_COURT_BEGIN		= 1800,		
	DEV_TYPE_COURT_DEV			= DEV_TYPE_COURT_BEGIN + 1,			// Court device
	DEV_TYPE_COURT_END,

	DEV_TYPE_VIDEO_TALK_BEGIN	= 1900,
	DEV_TYPE_VIDEO_TALK_VTNC	= DEV_TYPE_VIDEO_TALK_BEGIN + 1, 
	DEV_TYPE_VIDEO_TALK_VTO		= DEV_TYPE_VIDEO_TALK_BEGIN + 2,  
	DEV_TYPE_VIDEO_TALK_VTH		= DEV_TYPE_VIDEO_TALK_BEGIN + 3,  
	DEV_TYPE_VIDEO_TALK_MVTH = DEV_TYPE_VIDEO_TALK_BEGIN + 4,
	DEV_TYPE_VIDEO_TALK_WALLMACHINE = DEV_TYPE_VIDEO_TALK_BEGIN + 5,
	DEV_TYPE_VIDEO_TALK_DOORLOCK_VTH= DEV_TYPE_VIDEO_TALK_BEGIN + 6, 
	DEV_TYPE_VIDEO_TALK_2ND_VTO = DEV_TYPE_VIDEO_TALK_BEGIN + 7,
	DEV_TYPE_VIDEO_TALK_VTS = DEV_TYPE_VIDEO_TALK_BEGIN + 8,
	DEV_TYPE_VIDEO_TALK_SIPCALL = DEV_TYPE_VIDEO_TALK_BEGIN + 10,
	DEV_TYPE_VIDEO_TALK_END,

	DEV_TYPE_BROADCAST_BEGIN	= 2000,
	DEV_TYPE_BROADCAST_ITC_T6700R = DEV_TYPE_BROADCAST_BEGIN + 1,	// ITC_T6700R broadcast device
	DEV_TYPE_BROADCAST_END,

	DEV_TYPE_VIDEO_RECORD_SERVER_BEGIN = 2100,
	DEV_TYPE_VIDEO_RECORD_SERVER_BNVR	= DEV_TYPE_VIDEO_RECORD_SERVER_BEGIN + 1, // BNVR device
	DEV_TYPE_VIDEO_RECORD_SERVER_OE	= DEV_TYPE_VIDEO_RECORD_SERVER_BEGIN + 2, 		// surgery device(operation equipment)
	DEV_TYPE_VIDEO_RECORD_SERVER_END,

	DEV_TYPE_DISPATCHER_BEGIN	= 2200,
	DEV_TYPE_DISPATCHER			= DEV_TYPE_DISPATCHER_BEGIN + 1,	// Dispatch device
	DEV_TYPE_DISPATCHER_END,

	DEV_TYPE_ALARM_STUB_BEGIN			= 3400,								// Alarm tower device typw
	DEV_TYPE_ALARM_STUB_VTA				= DEV_TYPE_ALARM_STUB_BEGIN + 1,
	DEV_TYPE_ALARM_STUB_END,

	DEV_TYPE_POS_BEGIN		= 4000,
	DEV_TYPE_POS_BOX		= DEV_TYPE_POS_BEGIN + 1,		// POS box
	DEV_TYPE_POS_END,

	DEV_TYPE_TRANSCODER_BEGIN = 4100,                               // Transcoder device
	DEV_TYPE_TRANSCODER_VCS = DEV_TYPE_TRANSCODER_BEGIN + 1,
	DEV_TYPE_TRANSCODER_END,

	DEV_TYPE_OTHER_DEVICE_BEGIN = 4200,                             // Device without channel
	DEV_TYPE_OTHER_DEVICE_B100 = DEV_TYPE_OTHER_DEVICE_BEGIN + 1,
	DEV_TYPE_OTHER_DEVICE_B300 = DEV_TYPE_OTHER_DEVICE_BEGIN + 2,
	DEV_TYPE_OTHER_DEVICE_END,

	DEV_TYPE_RADAR_BEGIN = 4300,						// Radar device
	DEV_TYPE_RADAR_B100 = DEV_TYPE_RADAR_BEGIN + 1,		
	DEV_TYPE_RADAR_B300 = DEV_TYPE_RADAR_BEGIN + 2,
	DEV_TYPE_RADAR_END,

}DPSDK_DEV_TYPE;


// The define of TVWALL type
typedef enum
{
	TVWALL_TYPE_UNKNOW		 = 0,
	TVWALL_TYPE_NVD			 = 1,			// Decoder
	TVWALL_TYPE_SNVD		 = 2,			// Soft decoder
	TVWALL_TYPE_MATRIX		 = 3,			// Matrix
	TVWALL_TYPE_CTRL		 = 4,			// Decoding splicer
	TVWALL_TYPE_UDS			 = 5,			// Universal decoder, which is device edition of soft decoder
	TVWALL_TYPE_COURT		 = 6,			// Court trial host info
	TVWALL_TYPE_NVR6000		 = 7,			// Decoding NVR
	TVWALL_TYPE_AB			 = 8,			// AB controller
	TVWALL_TYPE_B10_PLATFORM = 9,			// Hikvision B10 integrated video platform
	TVWALL_TYPE_INFINOVAMARTRIX	 = 10,		// Infinova Matrix
	TVWALL_TYPE_B10_INTEGRAL	 = 11,		// Hikvision B10 integral machine
	TVWALL_TYPE_B20_PLATFORM	 = 12,		// Hikvision B20 integrated video platform
	TVWALL_TYPE_B20_INTEGRAL	 = 13,		// Hikvision B20 integral machine
}DPSDK_TVWALL_DEVICE_TYPE;

// Channel type
typedef enum
{
	CHNL_TYPE_ENC_BEGIN,											// Encode channel
	CHNL_TYPE_STREAM						= 1,					// Video
	CHNL_TYPE_PIC,													// Picture
	CHNL_TYPE_MIX,													// Double bit stream
	CHNL_TYPE_POS,													// POS channel
	CHNL_TYPE_ENC_END						= 10,
}DPSDK_CHANNEL_TYPE;//"channelType"

// Basic equipment information
typedef struct
{
	DPSDK_CHAR szDeviceID[DPSDK_DEVICE_ID_LEN];						// Device ID
	DPSDK_CHAR szDeviceName[DPSDK_NAME_LEN_EX];						// Device name
	DPSDK_CHAR szUserName[DPSDK_NAME_LEN];							// Device login user
	DPSDK_CHAR szUserPwd[DPSDK_PWD_LEN];							// Device login password
	DPSDK_CHAR szIP[DPSDK_IP_LEN];									// Device additionIP
	DPSDK_CHAR szDeviceSn[DPSDK_SN_LEN];							// Device serial number
	DPSDK_USHORT ushPort;											// Device add port
	DPSDK_CHAR szDevIP[DPSDK_IP_LEN];								// Device real ip
	DPSDK_USHORT ushDevPort;										// Device real port
	DPSDK_INT32 iManFac;											// Manufacturer
	DPSDK_INT32 iStatus;											// Device status	See DPSDK_DEV_STATUS Definition
	DPSDK_INT32 iDevType;											// Device type	see DPSDK_DEV_TYPE Definition
	DPSDK_CHAR szOrgCode[DPSDK_ORG_CODE_LEN];						// The organization code
	DPSDK_INT32 iDomainID;											// Domain ID
	DPSDK_INT32 iDevModel;											// Device model
	DPSDK_CHAR szSipId[DPSDK_ID_LEN];
	DPSDK_CHAR szSipPwd[DPSDK_PWD_LEN];
	DPSDK_INT32 iSipIdNum;
	DPSDK_ID vthRelatedConfirmVTOSipId[DPSDK_MAX_VTO_SIPID_NUM];
	DPSDK_INT32 iUnitEnable;
	DPSDK_INT32 iBuildingEnable;
	DPSDK_CHAR szSoftwareVersion[DPSDK_ID_LEN];
	DPSDK_CHAR szHardwareVersion[DPSDK_ID_LEN];
}DPSDK_DEV_INFO;

// Channel basic information
typedef struct
{
	DPSDK_CHAR szChannelID[DPSDK_CHANNEL_ID_LEN];					// Channel ID
	DPSDK_INT32 iChannelSeq;										// Channel serial number, begin at 0
	DPSDK_CHAR szChannelName[DPSDK_NAME_LEN_EX];					// Channel name
	DPSDK_CHAR szChnlSN[DPSDK_SN_LEN];								// Channel SN code
	DPSDK_INT32 iChannelType;										// Channel type		see DPSDK_CHANNEL_TYPE  Only coded channels are currently available classification
	DPSDK_INT32 iStatus;											// Channel status	see DPSDK_DEV_STATUS
	DPSDK_INT32 iDomainID;											// Domain ID
}DPSDK_BASE_CHANNEL_INFO;

// Camera type
typedef enum
{
	CAMERA_TYPE_NORMAL,												// Bolt
	CAMERA_TYPE_SD,													// Speed Dome Cameras
	CAMERA_TYPE_HALFSD,												// Hemisphere
	CAMERA_TYPE_EVIDENCE,											// Evidence channel
}DPSDK_CAMERA_TYPE;

// Lock holder information
typedef struct
{
	DPSDK_CHAR		szLockUserName[DPSDK_NAME_LEN];	// User name for lockin cloud
	DPSDK_INT32		iLockUserLevel;					// Lock user level of cloud
}DPSDK_PTZ_LOCKUSER;

// Operating parameters of cloud platform
typedef struct
{
	typedef enum
	{
		PtzOF_Show_PtzMenu				= 0,		// Display "Cloud platform menu"
		PtzOF_Move_PtzMenu				= 1,		// Control "Menu direction of the cloud platform"
		PtzOF_Confirm_PtzMenuItem		= 2,		// Determine "Cloud platform menu item"
		PtzOF_Set_LineScannBorder		= 3,		// Set up "Line scavenging boundary"
		PtzOF_Switch_LineScanBorder		= 4,		// Switch "Line scan"
		PtzOF_Switch_AutoRotate			= 5,		// Switch "Horizontal rotation"
		PtzOF_Switch_Light				= 6,		// Switch "lighting"
		PtzOF_Switch_RainBrush			= 7,		// Switch "Wiper"
		PtzOF_Switch_InfraredLight		= 8,		// Switch "infrared light"
		PtzOF_Switch_AssisentPoint		= 9,		// Switch "Auxiliary point"
		PtzOF_Switch_Cruise				= 10,		// Switch "Cruise function"
		PtzOF_Switch_Track				= 11,		// Switch "Cruising"
		PtzOF_Switch_SetTrack			= 12,		// Switch "Track setting"
	}PtzOperateFunction_e;

	DPSDK_CHAR				szChannelId[DPSDK_CHANNEL_ID_LEN];	// Channel ID
	PtzOperateFunction_e	iPtzOFType;							// Operating function type of cloud platform
	DPSDK_INT32				iCruiseId;							// (This variable is only in Switch_CruiseOperation when effective) Cruise ID
	DPSDK_INT32				iTrackId;							// (This variable is only in Switch_Track¡¢Switch_SetTrackEffective operation during operation ID
	DPSDK_INT32				iSwitchMode;						// (This variable is only in SwitchEffective operation)0-Close£¬1-open
	DPSDK_INT32				iBorderType;						// (This variable is only in Set_LineScannBorderEffective operation)16-Left boundary17-.Right boundary
	DPSDK_INT32				iAssisentType;						// (This variable is only in Switch_AssisentPointEffective operation)23-Backlight compensation,24-Number doubled,27- Color turn black,35-Shutter time,41-Brightness,42-Image flip,43-The name of the preset point is hidden,80-Restore factory settings
	DPSDK_INT32				iMoveType;							// (This variable is only in Move_PtzMenuEffective operation)25-Upward movement,26-Move down,27-Left shift,28-Right
	DPSDK_INT32				iSwitchPtzMenu;						// (This variable is only in Show_PtzMenuEffective operation)22=Open the platform menu,23=Close the cloud table menu
}DPSDK_PTZOPERATE_FUNCTION_PARAM;

// Function operation result of cloud platform 
typedef struct
{
	DPSDK_PTZ_LOCKUSER		struLockUser;
	DPSDK_INT32				iResult;				// Operation results:0-failure,1-success
}DPSDK_PTZOPERATE_RESULT;

// Operating cloud platform camera parameters
typedef struct
{
	DPSDK_CHAR				szChannelId[DPSDK_CHANNEL_ID_LEN];	// Channel ID
	DPSDK_INT32				iDirect;							// Direction:1-increase,2-decrease
	DPSDK_INT32				iCommand;							// Order:0-stop it£¬1-open
	DPSDK_INT32				iStep;								// Step
	DPSDK_INT32				iOperateType;						// Operation type:1-variable,2-zoom,3-aperture
	DPSDK_CHAR				szExtend[DPSDK_PTZ_EXTEND_LEN];		// Extended data
	DPSDK_DOUBLE			fSpeed;								// The speed of PTZ,normalized 0~1£¬0 represent default speed, used when vsl zoomed
	DPSDK_INT32				iDuration;							// Duration,unit:ms
}DPSDK_PTZOPERATE_CAMERA_PARAM;

// Cloud platform direction control parameters
typedef struct
{
	DPSDK_CHAR				szChannelId[DPSDK_CHANNEL_ID_LEN];	// Channel ID
	//DPSDK_DOUBLE			fPan;								// The control speed of vertical direction,(-1,1), turn right when bigger than 0, other turn left
	//DPSDK_DOUBLE			fTilt;								// The control speed of Horizontal direction,(-1,1),upper when bigger than 0, other turn down
	//DPSDK_DOUBLE			fZoom;								//  The control speed of variable,(-1,1),amplification when bigger than 0£¬other narrow
	//															// The PTZ stop turning when the value of fPan, Tilt and fZoom are 0

	DPSDK_INT32				iStepY;								// Vertical direction step
	DPSDK_INT32				iStepX;								// Horizontal direction step
	DPSDK_INT32				iDirect;							// Direction:1-On,2-Under the,3-Left,4-Right,5-Upper left,6-Lower left,7-On the right,8-lower right
	DPSDK_INT32				iCommand;							// Order:0-Stop it, 1-open
	DPSDK_CHAR				szExtend[DPSDK_PTZ_EXTEND_LEN];		// Extended data
	DPSDK_INT32				iDuration;							// The duration of PTZ turned, unit:ms
}DPSDK_PTZOPERATE_DIRECT_PARAM;

// Operating cloud platform zoom parameters
typedef struct
{
	DPSDK_CHAR				szChannelId[DPSDK_CHANNEL_ID_LEN];	// Channel ID
	DPSDK_DOUBLE			fChange;							// The relative change value of focus,(-1,1)
	DPSDK_DOUBLE			fSpeed;								// The speed of PTZ turning, normalized 0~1£¬0 represent default speed
}DPSDK_PTZ_FOCUS_PARAM;

// Operating cloud platform aperture parameters
typedef struct
{
	DPSDK_CHAR				szChannelId[DPSDK_CHANNEL_ID_LEN];	// Channel ID
	DPSDK_DOUBLE			fChange;							// The change value of aperture,(-1,1)
}DPSDK_PTZ_IRIS_PARAM;

// Operating cloud platform variable parameters
typedef struct
{
	DPSDK_CHAR				szChannelId[DPSDK_CHANNEL_ID_LEN];	// Channel ID
	DPSDK_DOUBLE			fChange;							// The change value of variable,(-1,1)
	DPSDK_INT32				iDuration;							// Duration,unit:ms
}DPSDK_PTZ_ZOOM_PARAM;

// Electric focusing control parameters
typedef struct
{
	DPSDK_CHAR				szChannelId[DPSDK_CHANNEL_ID_LEN];	// Channel ID
	DPSDK_FLOAT				fFocus;								// Focal length
	DPSDK_FLOAT				fZoom;								// Multiple
	DPSDK_INT32				iOperateType;						// Operation type:0-Reset,1-Continuous focusing,2-Autofocus
}DPSDK_PTZOPERATE_FOCUS_PARAM;

// Control preset point parameters
typedef struct
{
	DPSDK_CHAR				szChannelId[DPSDK_CHANNEL_ID_LEN];			// Channel ID
	DPSDK_CHAR				szPointCode[DPSDK_PRESETPOINT_CODE_LEN];	// Preset point coding
	DPSDK_CHAR				szPointName[DPSDK_PRESETPOINT_NAME_LEN];	// Preset point name
	DPSDK_INT32				iOperateType;								// Operation type:1-Location, 2-Set up, 3-delete, 4-Update working time
	DPSDK_CHAR				szStartTime[DPSDK_PTZ_TIME_LEN];			// Start time(time stamp)
	DPSDK_CHAR				szEndTime[DPSDK_PTZ_TIME_LEN];				// End time(time stamp)
	DPSDK_DOUBLE			fSpeed;										// The speed of PTZ turning, normalized 0~1£¬0 represent default speed
}DPSDK_PTZOPERATE_PRESETPOINT_PARAM;

// Get the preset point information
typedef struct
{
	DPSDK_CHAR				szPointName[DPSDK_PRESETPOINT_NAME_LEN];	// Preset point name
	DPSDK_CHAR				szPointCode[DPSDK_PRESETPOINT_CODE_LEN];	// Preset point encoding, from 1 start
	DPSDK_INT32				iPointType;									// Preset point type,0=Ordinary preset point,1=Preset points that have been set for intelligent rules
}DPSDK_PTZ_PRESETPOINT_INFO;

// Get a list of preset points
typedef struct
{
	DPSDK_UINT32			uiTotal;									// Total
	DPSDK_PTZ_PRESETPOINT_INFO struPresetPointInfo[1];					// Preset point list
}DPSDK_PTZ_PRESETPOINT_LIST;

// Set current point of PTZ as preset point
typedef struct
{
	DPSDK_CHAR				szChannelId[DPSDK_CHANNEL_ID_LEN];			// Channel ID
	DPSDK_CHAR				szPointCode[DPSDK_ID_LEN];					// Preset point encoding, from 1 start
	DPSDK_CHAR				szPointName[DPSDK_PRESETPOINT_NAME_LEN];	// Preset point name
}DPSDK_PTZ_PRESETPOINT_SET_PARAM;

// Delete preset point
typedef struct
{
	DPSDK_CHAR				szChannelId[DPSDK_CHANNEL_ID_LEN];			// Channel ID
	DPSDK_CHAR				szPointCode[DPSDK_ID_LEN];					// Preset point encoding, from 1 start
}DPSDK_PTZ_PRESETPOINT_DEL_PARAM;

// Locating ball machine to current preset point
typedef struct
{
	DPSDK_CHAR				szChannelId[DPSDK_CHANNEL_ID_LEN];			// Channel ID
	DPSDK_CHAR				szPointCode[DPSDK_ID_LEN];					// Preset point encoding, from 1 start
	DPSDK_DOUBLE			fSpeed;										// The speed of PTZ turning, normalized 0~1£¬0 represent default speed
}DPSDK_PTZ_PRESETPOINT_LOCATE_PARAM;

// Three dimensional positioning parameters
typedef struct
{
	DPSDK_CHAR				szChannelId[DPSDK_CHANNEL_ID_LEN];		// Channel ID
	DPSDK_DOUBLE			fPointX;								// Horizontal coordinates:-8192 ~ ~ Eight thousand one hundred and ninety-two
	DPSDK_DOUBLE			fPointY;								// Vertical coordinates:-8192 ~ ~ Eight thousand one hundred and ninety-two
	DPSDK_DOUBLE			fPointZ;								// Variable number:-4 ~ ~ 4
	DPSDK_CHAR				szExtend[DPSDK_PTZ_EXTEND_LEN];			// Extended data
}DPSDK_PTZOPERATE_SITPOSITION_PARAM;

// Three dimensional positioning parameters(Used by VSL)
typedef struct
{
	DPSDK_CHAR				szChannelId[DPSDK_CHANNEL_ID_LEN];		// Channel ID
	DPSDK_DOUBLE			fCandidateBoxMidX;						// x value of center point of candidate box
	DPSDK_DOUBLE			fCandidateBoxMidY;						// y value of center point of candidate box
	DPSDK_INT32				iCandidateBoxWidth;						// Width of candidate box, the value on horizontal coordinate, corresponding to x
	DPSDK_INT32				iCandidateBoxHeight;					// Height of candidate box, the value on vertical coordinate, corresponding to y
	DPSDK_INT32				iWindowWidth;							// Width of window, the value on horizontal coordinate, corresponding to x
	DPSDK_INT32				iWindowHeight;							// Height of window,the value on vertical coordinate, corresponding to y
	DPSDK_BOOL				bZoomOut;								// Zoom in and out zone bit, true is to zoom in, and false is to zoom out
}DPSDK_PTZOPERATE_SITPOSITION_EX_PARAM;

// Lock the unlocking parameters
typedef struct
{
	DPSDK_CHAR				szChannelId[DPSDK_CHANNEL_ID_LEN];		// Channel ID
	DPSDK_UINT32			uiLockTime;								// Lock time, unit second,texpression has been locked until the release or Bei Qiang Wins
	DPSDK_INT32				iOperateType;							// Operation type:0-Unknown, 1-Lock the current camera, 2-Unlock the current camera, 3-Unlock all the cameras locked by the user, 4-Lock all the cameras, 5-Query lock state
	DPSDK_CHAR				szUserCode[DPSDK_USER_ID_LEN];
	DPSDK_CHAR				szUserName[DPSDK_NAME_LEN];
	DPSDK_CHAR				szExtend[DPSDK_PTZ_EXTEND_LEN];			// Extended data
}DPSDK_PTZOPERATE_ARRANGEPTZ_PARAM;

// Open the manual video parameters
typedef struct
{
	DPSDK_CHAR					szChannelId[DPSDK_CHANNEL_ID_LEN];	// Channel ID
	DPSDK_INT32					iStreamType;						// Code stream type (code stream type:1-Main stream, 2-Auxiliary code stream)
	DPSDK_INT32					iRecordDuration;					// Video length(default 12*3600s)
}DPSDK_PTZOPERATE_STARTREMOTERECORD_PARAM;

// Open/Stop the result of manual video
typedef struct
{
	DPSDK_INT32					iPlanId;							// Plan ID
	DPSDK_CHAR					szNow[DPSDK_PTZ_TIME_LEN];			// Current time(time stamp)
}DPSDK_PTZOPERATE_REMOTERECORD_RESULT;

// Turn off the manual video parameters
typedef struct
{
	DPSDK_CHAR					szChannelId[DPSDK_CHANNEL_ID_LEN];  // Channel ID
	DPSDK_INT32					iStreamType;						// Code stream type (code stream type:1-Main stream, 2-Auxiliary code stream)
	DPSDK_BOOL					bForce;							    // Is it forced to close
}DPSDK_PTZOPERATE_STOPREMOTERECORD_PARAM;

// The point information of PTZ
typedef struct
{
	DPSDK_INT32					iPointX;							// Horizontal coordinates:-8192 ~~ 8192 
	DPSDK_INT32					iPointY;							// Vertical coordinates:-8192 ~~ 8192 
	DPSDK_INT32					iPointZ;							// The value of variable:-4 ~~ 4 					
}DPSDK_PTZOPERATE_POSITIONINFO;

// The information of PTZ's visible range
typedef struct
{
	DPSDK_INT32					iAzimuth;							// The start of azimuth of visible range
	DPSDK_INT32					iDistance;							// The distance of visible range(radius)
	DPSDK_INT32					iAngle;								// The angle of visible range
}DPSDK_PTZOPERATE_CHANNELVIEWINFO;

// The parameter of ling gun ball
typedef struct 
{
	DPSDK_CHAR					szDevId[DPSDK_DEVICE_ID_LEN];		// Device id
	DPSDK_INT32					iGroup;								// Operate group id(0,1)
	DPSDK_INT32					eOprCmd;							// Operate type,see DPSDK_SmartTrack_Oper_e
	DPSDK_SMARTTRACK_OPTPARAM	OptParam;							// Operate parameter
}DPSDK_SMARTTRACK_PARAM;

typedef struct
{
	DPSDK_CHAR					szChannelCode[DPSDK_CHANNEL_ID_LEN];// Channel code
	DPSDK_INT32					iIndex;								// Label seq
}DPSDK_AR_TURN_PARAM;

typedef struct
{
	DPSDK_INT32					iModelType;							// Template type
}DPSDK_AR_DELETE_LABEL_TEM_PARAM;

typedef struct
{
	DPSDK_INT32					iX;
	DPSDK_INT32					iY;
	DPSDK_INT32					iWidth;
	DPSDK_INT32					iHeight;
}DPSDK_AR_TEMPLATE_LAYOUT;

typedef struct
{
	DPSDK_INT32					iKeyType;
	DPSDK_CHAR					szKeyName[DPSDK_NAME_LEN];
	DPSDK_AR_TEMPLATE_LAYOUT	struKeyNameLayout;
	DPSDK_CHAR					szKeyValue[DPSDK_NAME_LEN];
	DPSDK_AR_TEMPLATE_LAYOUT	struKeyValueLayout;
}DPSDK_AR_TEMPLATE_KEY;

typedef struct
{
	DPSDK_INT32					iWidth;								
	DPSDK_INT32					iHeight;
	DPSDK_INT32					iKeyNum;
	DPSDK_AR_TEMPLATE_KEY*      pKeyList;
}DPSDK_AR_TEMPLATE_DATA;

typedef struct
{
	DPSDK_INT32					iModelType;							// Template type
	DPSDK_CHAR					szModelName[DPSDK_NAME_LEN];		// Template name
	DPSDK_CHAR					szIconUrl[DPSDK_URL_LEN];			// Template label url
	DPSDK_AR_TEMPLATE_DATA		struModelData;						// Template data
}DPSDK_AR_LABEL_TEMPLATE_INFO;

typedef struct
{
	DPSDK_AR_LABEL_TEMPLATE_INFO struTemplateInfo;					// Template info
	DPSDK_INT32					 iUserDefine;						// 0:system£¬1:custom
}DPSDK_AR_ADD_LABEL_TEM_PARAM;

typedef struct
{
	DPSDK_AR_LABEL_TEMPLATE_INFO struTemplateInfo;					// Template info
	DPSDK_INT32					 iUserDefine;						// 0:system£¬1:custom
}DPSDK_AR_UPDATE_LABEL_TEM_PARAM;

typedef struct
{
	DPSDK_INT32					iModelType;							// Template type,iModelType = -1:all
}DPSDK_AR_GET_LABEL_TEM_PARAM;

typedef struct
{
	DPSDK_INT32						iTotalCount;
	DPSDK_AR_LABEL_TEMPLATE_INFO*	pTemplateInfoList;				
}DPSDK_AR_LABEL_TEM_LIST;

typedef struct
{
	DPSDK_CHAR					szChannelCode[DPSDK_CHANNEL_ID_LEN];
	DPSDK_INT32					iIndexNum;
	DPSDK_INT32*				pIndexList;
}DPSDK_AR_DELETE_LABEL_PARAM;

typedef struct
{
	DPSDK_CHAR					szChannelCode[DPSDK_CHANNEL_ID_LEN];
}DPSDK_AR_GET_LABEL_PARAM;

// Point information
typedef struct
{
	DPSDK_INT32 iX;									// x coordinates
	DPSDK_INT32 iY;									// y coordinates
}DPSDK_POINT;

typedef struct
{
	DPSDK_INT32					iIndex;							
	DPSDK_BOOL					bEnable;
	DPSDK_BOOL					bTitleAttribute;					// TRUE : has child label
	DPSDK_CHAR					szTitleName[DPSDK_NAME_LEN];
	DPSDK_INT32					iTitleType;
	DPSDK_INT32					iTitleLevel;

	DPSDK_INT32					iPointX;
	DPSDK_INT32					iPointY;
	DPSDK_INT32					iShapType;							// 0=fold line,1=sector,point 1 is the center of circle,point 2 and point 3 is the end of two sides
	DPSDK_INT32					iPolygonNum;
	DPSDK_POINT*				pPolygonList;						
}DPSDK_AR_LABEL_BASE_INFO;

typedef struct
{
	DPSDK_AR_LABEL_BASE_INFO	struBaseInfo;
	DPSDK_INT32					iModelType;							// Template type
	DPSDK_AR_TEMPLATE_DATA		struModelData;						// Template data
}DPSDK_AR_EXTRA_SPOT_DATA;

typedef struct
{
	DPSDK_AR_LABEL_BASE_INFO	struBaseInfo;
	DPSDK_INT32					iModelType;							// Template type
	DPSDK_AR_TEMPLATE_DATA		struModelData;						// Template data
	DPSDK_INT32					iExtraSpotNum;
	DPSDK_AR_EXTRA_SPOT_DATA*	pExtraSpotList;
}DPSDK_AR_LABEL_INFO;

typedef struct
{
	DPSDK_INT32						iTotalCount;
	DPSDK_AR_LABEL_INFO*			pLabelList;				
}DPSDK_AR_LABEL_LIST;

typedef struct
{
	DPSDK_CHAR					szChannelCode[DPSDK_CHANNEL_ID_LEN];
	DPSDK_AR_LABEL_INFO			struLabelInfo;
}DPSDK_AR_SET_LABEL_PARAM;

typedef struct 
{
	DPSDK_CHAR					szChannelId[DPSDK_CHANNEL_ID_LEN];	// Channel ID
} DPSDK_LINKED_CHANNEL;

typedef struct 
{
	DPSDK_CHAR					szChannelId[DPSDK_CHANNEL_ID_LEN];	// Channel ID
	DPSDK_INT32					iNum;								// The number of link video channel
	DPSDK_LINKED_CHANNEL		LinkChannel[DPSDK_LINKED_CHANNEL_SIZE];	// Video channel
} DPSDK_LINKED_CHANNELS_PARAM;

typedef struct 
{
	DPSDK_INT32 iNum;
	DPSDK_LINKED_CHANNELS_PARAM* pLinkedChannelList;
} DPSDK_LINKED_RESOURCE_PARAM;


// Query heat map parameter
typedef struct
{
	DPSDK_CHAR					szChannelId[DPSDK_CHANNEL_ID_LEN];	// Channel ID
	DPSDK_INT32					iSessionId;							// The session Id of query
	DPSDK_CHAR					szBeginTime[DPSDK_PTZ_TIME_LEN];	// Start time(time stamp)
	DPSDK_CHAR					szEndTime[DPSDK_PTZ_TIME_LEN];		// End time(time stamp)
}DPSDK_PTZOPERATE_GETHEATMAP_PARAM;

typedef struct
{
	DPSDK_LONG					lId;
	DPSDK_INT32					iPresetIndex;
	DPSDK_INT32					iIdleTime;
	DPSDK_CHAR					szBeginTime[DPSDK_PTZ_TIME_LEN];	// Start time(time stamp)
	DPSDK_CHAR					szEndTime[DPSDK_PTZ_TIME_LEN];		// End time(time stamp)
}DPSDK_PTZ_CURISEPRESET_PARAM;

typedef struct
{
	DPSDK_CHAR						szChannelId[DPSDK_CHANNEL_ID_LEN];	// Channel ID
	DPSDK_CHAR						szChannelSn[DPSDK_CHANNEL_ID_LEN];	// Channel SN
	DPSDK_INT32						iStatu;								//
	DPSDK_INT32						iParamNum;
	DPSDK_PTZ_CURISEPRESET_PARAM*	pCurisePresetList;
}DPSDK_PTZ_CHANNL_CURISEPRESET_PARAM;

// Alarm output control parameters
typedef struct
{
	DPSDK_CHAR					szChannelId[DPSDK_CHANNEL_ID_LEN];	// Channel ID
	DPSDK_INT32					iOperateType;						// Control type:1-Status control,2-Pattern control
	DPSDK_INT32					iCommand;							// Control commands: state control,1-Open,0-Shut down; mode control:0-Close£¬1-Automatically,2-Manual
}DPSDK_PTZOPERATE_ALARMOUT_PARAM;

// Get status information of focus
typedef struct
{
	DPSDK_CHAR					szChannelId[DPSDK_CHANNEL_ID_LEN];	// Channel ID
	DPSDK_INT32					iStatus;							// Focus status£º0-normal£¬1-Focusing
	DPSDK_FLOAT					fZoom;								// Multiple
	DPSDK_FLOAT					fFocus;								// Focal length
}DPSDK_FOCUSSTATUS_INFO;

//M60 M30 M70Remote channel type
typedef enum
{
	REMOTE_TYPE_UNKNOW			= 0,	
	REMOTE_TYPE_LOCAL_CHAN		= 1,	// Local coded channel
	REMOTE_TYPE_REMOTE_CHAN		= 2,	// Remote channel
	REMOTE_TYPE_LOWER_CHAN		= 3,	// Cascaded channel
	REMOTE_TYPE_MATRIX_CHAN		= 4,	// Analog matrix channel
}DPSDK_CHANNEL_REMOTE_TYPE;

// Channel extent information
typedef struct
{
	DPSDK_INT32 iRoadway;											// Roadway 
	DPSDK_FLOAT fCartMaxSpeed;										// Max speed of cart
	DPSDK_FLOAT fCartMinSpeed;										// Min speed of cart
	DPSDK_FLOAT fDollyMaxSpeed;										// Max speed of dolly
	DPSDK_FLOAT fDollyMinSpeed;										// Min speed of dolly
	DPSDK_FLOAT fDirection;											// Direction
}DPSDK_CHANNEL_EXT_INFO;

// Coded channel information
typedef struct  
{
	DPSDK_BASE_CHANNEL_INFO struChannelInfo;
	DPSDK_CHANNEL_EXT_INFO struChnExtInfo;							// Channel extent information

	DPSDK_INT32 iCameraType;										// Camera type See DPSDK_CAMERA_TYPE Definition
	DPSDK_CHAR szLatitude[DPSDK_GPS_LEN];							// Latitude
	DPSDK_CHAR szLongitude[DPSDK_GPS_LEN];							// Longitude
	DPSDK_INT32 iCameraFunction;									// 0 No support function 1 Support fish eye 2 Support electric focusing
	DPSDK_CHAR szMulticastIP[DPSDK_IP_LEN];							// Multicast IP
	DPSDK_USHORT ushMulticastPort;									// Multicast port
	DPSDK_CHAR szNVR_IPCIP[DPSDK_IP_LEN];							// NVR Front end IPC IP
	DPSDK_INT32 iChannelRemoteType;									// Remote channel type See DPSDK_CHANNEL_REMOTE_TYPE Definition
	DPSDK_INT32 iFaceFunction;										// Face function, 0= not support, 1= snap, 2= identify
	DPSDK_INT32 iIntelliState;										// Intelligent status 0 off-ling,1 on-ling
	DPSDK_INT32 iTargetDetection;									// Intelligent status 0= not support

	// The type of unit in which the channel belongs
	DPSDK_INT32 iTrackID;											// Flow type
	DPSDK_INT32 iStreamType;										// Code stream type See DPSDK_STREAM_TYPE Definition
	DPSDK_BOOL bZeroEncode;											// Does it support 0 Channel multi picture coding
	DPSDK_CHAR szKeyCode[DPSDK_ID_LEN];								// Key code
	DPSDK_CHAR szDB33Code[DPSDK_ID_LEN];							// DB code
	DPSDK_INT32 iPCFlag;											// 1=Area,2=In-Out,3=Out Door
}DPSDK_ENC_CHANNEL_INFO;

// Decoder video source
typedef enum
{
	DECODE_MODE_UNDEFINE,											// Undefined
	DECODE_MODE_ACTIVE,												// Active
	DECODE_MODE_PASSIVE,											// Passive
	DECODE_MODE_PUSH,												// Push flow
}DPSDK_DECODE_MODE;

// Decoding channel information
typedef struct  
{
	DPSDK_BASE_CHANNEL_INFO struChannelInfo;
	DPSDK_INT32 iMaxSpliteNum;										// Maximum division number equipment related
	
	// The type of unit in which the channel belongs
	DPSDK_INT32 iDecodeMode;										// Decoding mode See DPSDK_DECODE_MODE Definition
	DPSDK_BOOL  bConbineStatus;										// Does it support conbine
}DPSDK_DEC_CHANNEL_INFO;

// Alarm input channel information
typedef struct  
{
	DPSDK_BASE_CHANNEL_INFO struChannelInfo;
	DPSDK_INT32 iAlarmType;											// Alarm type
	DPSDK_INT32 iAlarmLevel;										// Alarm level
}DPSDK_ALARMIN_CHANNEL_INFO;

// Alarm output channel information 
typedef struct
{
	DPSDK_BASE_CHANNEL_INFO struChannelInfo;
	DPSDK_INT32 iAlarmType;											// Alarm type
}DPSDK_ALARMOUT_CHANNEL_INFO;

// Large screen input channel data
typedef struct
{
	DPSDK_BASE_CHANNEL_INFO struChannelInfo;
	DPSDK_INT32 iCameraType;										// Camera type See DPSDK_CAMERA_TYPEDefinition
	DPSDK_INT32 iChannelRemoteType;									// Remote channel type See DPSDK_CHANNEL_REMOTE_TYPEDefinition
}DPSDK_TVWALLIN_CHANNEL_INFO;

// Large screen output channel data
typedef struct  
{
	DPSDK_BASE_CHANNEL_INFO struChannelInfo;
	DPSDK_INT32 iDecodeMode;										// Decoding mode See DPSDK_DECODE_MODEDefinition
}DPSDK_TVWALLOUT_CHANNEL_INFO;

// Access data
typedef struct  
{
	DPSDK_BASE_CHANNEL_INFO struChannelInfo;

	// Unit attributes of a channel
	DPSDK_INT32 iThirdControl;										// Whether third party control is allowed 0 no 1 yes
	DPSDK_INT32 iFingerPrintAuth;
	DPSDK_INT32 iCardAuth;
	DPSDK_INT32 iFaceAuth;
	DPSDK_INT32 iUserIsolate;
	DPSDK_CHAR szUnlockModes[DPSDK_UNLOCKMODES_LEN];
}DPSDK_DOOR_CHANNEL_INFO;

// Voice Channel Data
typedef struct  
{
	DPSDK_BASE_CHANNEL_INFO struChannelInfo;
	
	// Channel Cell Attribute
	DPSDK_CHAR szVoiceIP[DPSDK_IP_LEN];								// Voice Service Address
	DPSDK_CHAR szClientIP[DPSDK_IP_LEN];							// Voice Client Address
	DPSDK_USHORT ushVoicePort;										// Voice Service Port
	DPSDK_USHORT ushStatusPort;										// Voice Status Port
}DPSDK_VOICE_CHANNEL_INFO;

// Channel gate data
typedef struct  
{
	DPSDK_BASE_CHANNEL_INFO struChannelInfo;
	DPSDK_CHAR szSluiceType[DPSDK_TYPE_LEN];						// Channel gate type
}DPSDK_ROADGATE_CHANNEL_INFO;

// LED Channel data
typedef struct
{
	DPSDK_BASE_CHANNEL_INFO struChannelInfo;
	DPSDK_INT32 iFreeParkingSpace;									// Residual parking space
	DPSDK_CHAR szLEDChnlDesc[DPSDK_URL_LEN];						// Description information
}DPSDK_LED_CHANNEL_INFO;

// Dispatcher channel data
typedef struct  
{
	DPSDK_BASE_CHANNEL_INFO struChannelInfo;
	DPSDK_CHAR szCallNum[DPSDK_TYPE_LEN];							// Phone number
}DPSDK_DISPATCHER_CHANNEL_INFO;

// POS Channel data
typedef struct 
{
	DPSDK_BASE_CHANNEL_INFO struChannelInfo;
	DPSDK_CHAR szLinkChnl[DPSDK_CHANNEL_ID_LEN];					// POS Channel binding video source
}DPSDK_POS_CHANNEL_INFO;

// Virtual Channel Data
typedef struct
{
	DPSDK_BASE_CHANNEL_INFO struChannelInfo;
	DPSDK_CHAR szVideoSource[DPSDK_CHANNEL_ID_LEN];
}DPSDK_VIRTUAL_CHANNEL_INFO;

// Device data 
typedef struct
{
	DPSDK_DEV_INFO struDevInfo;										// Device data 
	
	// Coding channel
	DPSDK_INT32 iEncChnlNum;										// Number of coded channels 
	DPSDK_ENC_CHANNEL_INFO* pEncChnlInfoList;						// Code channel list 
	// Decoding channel
	DPSDK_INT32 iDecChnlNum;										// Decode channel number 
	DPSDK_DEC_CHANNEL_INFO* pDecChnlInfoList;						// Decode channel list 
	// Alarm input channel
	DPSDK_INT32 iAlarmInChnlNum;									// Number of alarm input 
	DPSDK_ALARMIN_CHANNEL_INFO* pAlarmInChnlInfoList;				// Alarm input list 
	// Alarm output channel
	DPSDK_INT32 iAlarmOutChnlNum;									// Alarm output number 
	DPSDK_ALARMOUT_CHANNEL_INFO* pAlarmOutChnlInfoList;				// Alarm output list 
	// Large screen input channel
	DPSDK_INT32 iTvWallInChnlNum;									// Large screen input channel number 
	DPSDK_TVWALLIN_CHANNEL_INFO* pTvWallInChnlInfoList;				// Large screen input channel list 

	DPSDK_INT32 iTvWallOutChnlNum;									// Large screen output channel number 
	DPSDK_TVWALLOUT_CHANNEL_INFO* pTvWallOutChnlInfoList;			// Large screen output channel list 
	// Entrance guard channel
	DPSDK_INT32 iDoorChnlNum;										// Number of access channels 
	DPSDK_DOOR_CHANNEL_INFO* pDoorChnlInfoList;						// List of access channels 
	// Voice channel
	DPSDK_INT32 iVoiceChnlNum;										// Voice channel number 
	DPSDK_VOICE_CHANNEL_INFO* pVoiceChnlInfoList;					// Voice channel list 
	// Channel gate 
	DPSDK_INT32 iRoadGateChnlNum;									// Number of channel gates 
	DPSDK_ROADGATE_CHANNEL_INFO* pRoadGateChnlInfoList;				// List of channel gates 
	// LED channel
	DPSDK_INT32 iLEDChnlNum;										// LED Number of channels 
	DPSDK_LED_CHANNEL_INFO* pLEDChnlInfoList;						// LED Channel list 
	// Dispatcher channel
	DPSDK_INT32 iDispatcherChnlNum;									// Number of channels for the dispatcher 
	DPSDK_DISPATCHER_CHANNEL_INFO* pDispatcherChnlInfoList;			// Scheduler list 
	// POS channel
	DPSDK_INT32 iPosChnlNum;										// POS Number of channels 
	DPSDK_POS_CHANNEL_INFO* pPosChnlInfoList;						// POS Channel list 
	// Virtual channel
	DPSDK_INT32 iVirtualChnlNum;									// Number of virtual channels  
	DPSDK_VIRTUAL_CHANNEL_INFO* pVirtualChnlInfoList;				// Virtual channel list
}DPSDK_DEV_ALL_INFO;

// Compression method
typedef enum
{
	COMPRESS_DISABLE			= 0,								// No use of compression
	COMPRESS_DEFAULT			= 1,								// Using the default compression method
}DPSDK_COMPRESS_TYPE;

typedef struct DPSDK_QUERY_DEV_PARAM
{
	DPSDK_DEVICE_ID struDevId;								// Device id
	DPSDK_INT32 iFetchFlag;
	DPSDK_IP struDeviceIp;									// Device ip
	DPSDK_INT32 iDevicePort;								// Device port
	DPSDK_CHAR szLoginName[DPSDK_NAME_LEN];					// Login name
	DPSDK_CHAR szLoginPWD[DPSDK_PWD_LEN];					// Device password (encrypt)
	DPSDK_INT32 iProtocolType;								// Protocol see DPSDK_DEVICE_PROTOCOL_TYPE
}DPSDK_QUERY_DEV_PARAM;

// Organization device condition
typedef struct DPSDK_QUERY_DEV_INFO
{
	DPSDK_CHAR szOrgCode[DPSDK_ORG_CODE_LEN];						// Organization code
	DPSDK_UINT32 uiCategoriesCount;									// The number of device categories which are need to query
	DPSDK_INT32 iCategoriesList[128];								// The device categories function which are need to query see DPSDK_DEV_UNIT_TYPE
	DPSDK_DEVICE_ID struDevIdList[1];								// The device id list which are need to query
}DPSDK_QUERY_DEV_INFO;

typedef struct DPSDK_DEVICE_TYPE_DATA
{
	DPSDK_INT32 iTypeId;
	DPSDK_CHAR szTypeCode[DPSDK_ID_LEN];
	DPSDK_CHAR szTypeName[DPSDK_NAME_LEN];
	DPSDK_INT32 iDeviceType;
}DPSDK_DEVICE_TYPE_DATA;

typedef struct DPSDK_DEVICE_TYPE_LIST
{
	DPSDK_INT32 iCategoryId;
	DPSDK_CHAR	szCategoryName[DPSDK_NAME_LEN];
	DPSDK_INT32 iDeviceTypeNum;
	DPSDK_DEVICE_TYPE_DATA* pDeviceTypes;
}DPSDK_DEVICE_TYPE_LIST;

typedef struct DPSDK_DEVICE_TYPE_INFO
{
	DPSDK_INT32 iDevTypeNum;
	DPSDK_DEVICE_TYPE_LIST* pDeviceTypeList;
}DPSDK_DEVICE_TYPE_INFO;

// Device list
typedef struct  DPSDK_DEV_ALL_INFO_LIST
{
	DPSDK_INT32 iDevNum;											// Number of devices
	DPSDK_DEV_ALL_INFO* pDevAllInfoList;							// Device list data
}DPSDK_DEV_ALL_INFO_LIST;

// Menu information
typedef struct
{
	DPSDK_CHAR szMenuRight[DPSDK_MEMURIGHT_LEN];					// Menu right information
}DPSDK_MENU_RIGHT;

#define DPSDK_MAX_RIGHT_NUM		256									// Max number of right

// The list of user right
typedef struct
{
	DPSDK_INT32 iMenuRightNum;										// The number of menu right
	DPSDK_MENU_RIGHT struMenuRightList[DPSDK_MAX_RIGHT_NUM];		// Menu right
	DPSDK_INT32 iFunctionRightNum;									// The number of function right
	DPSDK_UINT32 uiFunctionRightList[DPSDK_MAX_RIGHT_NUM];			// Function right: 1 preview video, 5 lock video, 6 marking video, 2 control PTZ, 3 audio talk
}DPSDK_USER_RIGHT_LIST;

//// Channel code
//typedef struct
//{
//	DPSDK_CHAR szId[DPSDK_CHANNEL_ID_LEN];							// Channel id
//}DPSDK_CHANNEL_ID;

// User ID
typedef struct
{
	DPSDK_CHAR szUserId[DPSDK_USER_ID_LEN];							// User id
}DPSDK_USER_ID;

// Client name
typedef struct
{
	DPSDK_CHAR szX32[DPSDK_URL_LEN];								//  Client name X32
	DPSDK_CHAR szX64[DPSDK_URL_LEN];								//  Client name X64
}DPSDK_CLIENT_NAME;

// Node code
typedef struct
{
	DPSDK_CHAR szOrgCode[DPSDK_ORG_CODE_LEN];						// Node code
}DPSDK_ORG_CODE;

// The list of channel id
typedef struct
{
	DPSDK_INT32 iChannelNum;										// The number of channel
	DPSDK_INT32 iMaxChannelNum;										// Max number of channel
	DPSDK_CHANNEL_ID* pChannelId;									// Channel id
}DPSDK_CHANNEL_ID_LIST;

// The list of channel id
typedef struct
{
	DPSDK_INT32 iChannelNum;										// The number of channel
	DPSDK_CHANNEL_ID struChannelList[1];							// The list of channel
}DPSDK_CHANNEL_LIST;

// The list of organization node
typedef struct
{
	DPSDK_INT32 iOrgNum;											// The number of node
	DPSDK_ORG_CODE* pOrgCode;										// node code
}DPSDK_ORE_CODE_LIST;

// User group information
typedef struct
{
	DPSDK_CHAR szGroupId[DPSDK_USER_ID_LEN];						// Group id
	DPSDK_CHAR szGroupName[DPSDK_NAME_LEN];							// Group name
	DPSDK_USER_RIGHT_LIST struUserRightList;						// User right
	DPSDK_INT32 iChannelNum;										// The number of channel
	DPSDK_CHANNEL_ID* pChannelId;									// Channel id
}DPSDK_USER_GROUP_INFO;

// The list of user group
typedef struct
{
	DPSDK_INT32 iUserGroupNum;										// The number of user group
	DPSDK_USER_GROUP_INFO* pUserGroupInfo;							// User group information
}DPSDK_USER_GROUP_LIST;

// Paging information 
typedef struct
{
	DPSDK_UINT32 uiPage;											// The current paging, from 1 start
	DPSDK_UINT32 uiPageSize;										// Page size
}DPSDK_PAGE_INFO;

typedef struct
{
	DPSDK_CHAR szKeyWord[DPSDK_NAME_LEN];						// VTH name|app user name|SIP
	DPSDK_INT32 iUserStatus;									// -1=all, 0=Frozen, 1=normal
}DPSDK_SEARCH_USER_INFO;

typedef struct
{
	DPSDK_SEARCH_USER_INFO struSearchInfo;						// Search Info
	DPSDK_PAGE_INFO struPageInfo;								// page Info
}DPSDK_GET_APP_USERS_PARAM;

typedef struct
{
	DPSDK_CHAR szUserId[DPSDK_USER_ID_LEN];							// User id
	DPSDK_CHAR szUserName[DPSDK_NAME_LEN];							// User name
	DPSDK_INT32 iUserType;											// 1=PC£¬2=Android£¬3=IOS
	DPSDK_CHAR szDeviceName[DPSDK_DEVICE_NAME_LEN];					// Device name
	DPSDK_CHAR szSipId[DPSDK_ID_LEN];								// Vttalk SIP-ID
	DPSDK_TIMET tLastLoginDate;
	DPSDK_INT32 iUserStatus;										// 0=Frozen, 1=normal
	DPSDK_TIMET tLastUpdatePasswordDate;
}DPSDK_APP_USER_INFO;

// The list of app user
typedef struct
{
	DPSDK_INT32 iTotalCount;									// Total Count
	DPSDK_INT32 iUserNum;										// The number of user
	DPSDK_APP_USER_INFO* pAppUserInfo;						    // App User information
}DPSDK_APP_USER_LIST;

typedef struct
{
	DPSDK_INT32 iUserNum;										// The number of user
	DPSDK_USER_ID* pUserList;									// App User list
}DPSDK_DELETE_APP_USER_PARAM;

typedef struct
{
	DPSDK_CHAR szUserId[DPSDK_USER_ID_LEN];							// User id
	DPSDK_CHAR szUserName[DPSDK_NAME_LEN];							// User name
	DPSDK_INT32 iUserStatus;										// 0=Frozen, 1=normal
	DPSDK_CHAR szPassword[DPSDK_PWD_LEN];							// Password (encrypt)
}DPSDK_UPDATE_APP_USER_PARAM;

// The basic information of user
typedef struct
{
	DPSDK_CHAR szGroupId[DPSDK_USER_ID_LEN];						// Group id
	DPSDK_CHAR szUserId[DPSDK_USER_ID_LEN];							// User id
	DPSDK_CHAR szUserName[DPSDK_NAME_LEN];							// User name
	DPSDK_BOOL bIsOnline;											// Is it on-line
}DPSDK_USER_BASE_INFO;

#define DPSDK_MAX_GROUP_ID_NUM		512								// Max number of group id

// Query user basic information parameter
typedef struct
{
	DPSDK_INT32 iGroupIdNum;										// The number of group id
	DPSDK_USER_ID szGroupIdList[DPSDK_MAX_GROUP_ID_NUM];			// The list of group id
}DPSDK_QUERY_USER_BASE_INFO;

#define DPSDK_MAX_USER_NUM		100									// Max number of user

// The list of user basic information
typedef struct
{
	DPSDK_INT32 iUserBaseInfoNum;									// The number of user basic information
	//DPSDK_USER_BASE_INFO szUserBaseInfoList[DPSDK_MAX_USER_NUM];	// The list of user basic information, The initialize length of list id iPageSize, the real length of is is iUserBaseInfoNum
	DPSDK_USER_BASE_INFO* pUserBaseInfo;
}DPSDK_USER_BASE_INFO_LIST;

// User detail information
typedef struct
{
	DPSDK_USER_BASE_INFO struUserBaseInfo;							// Basic information

	DPSDK_CHAR szPassword[DPSDK_PWD_LEN];							// Password
	DPSDK_CHAR szPasswordExpiry[DPSDK_PWD_EXPIRY_LEN];				// The expiry of password
	DPSDK_CHAR szMac[DPSDK_MACADDRESS_LEN];							// Mac address
	DPSDK_CHAR szRemark[DPSDK_USER_REMARK_LEN];						// Memo
	DPSDK_CHAR szMailAddress[DPSDK_ALARM_EMAILRECEIVER_LEN];		// Mail address
	DPSDK_USER_RIGHT_LIST struUserRightList;						// User right
	DPSDK_CHANNEL_ID_LIST struChannelIdList;						// The list of channel id
	DPSDK_ORE_CODE_LIST   struOrgCodeList;							// The list of organization node code
}DPSDK_USER_DETAIL_INFO;

// The parameter of query user detail information
typedef struct
{
	DPSDK_INT32 iUserIdNum;											// The number of user id
	DPSDK_USER_ID szUserIdList[DPSDK_MAX_USER_NUM];					// The list of user id
}DPSDK_QUERY_USER_DETAIL_INFO;

// The list of user detail information
typedef struct
{
	DPSDK_INT32 iUserDetailInfoNum;									// The number of user detail information
	DPSDK_USER_DETAIL_INFO* pUserDetailInfoList;					// The list of user detail information
}DPSDK_USER_DETAIL_INFO_LIST;

// Role information
typedef struct
{
	DPSDK_CHAR szRoleId[DPSDK_USER_ID_LEN];							// The ID of role
	DPSDK_CHAR szRoleName[DPSDK_NAME_LEN];							// The name of role
	DPSDK_INT32 iUserIDNum;											// The number of user ID
	DPSDK_USER_ID* pUserIDList;										// The list of user ID
}DPSDK_USER_EMAIL_ROLE_INFO;

// User information
typedef struct
{
	DPSDK_CHAR szUserID[DPSDK_USER_ID_LEN];							// The ID of user
	DPSDK_CHAR szUserName[DPSDK_NAME_LEN];							// The name of user
	DPSDK_CHAR szAddress[DPSDK_ALARM_EMAILRECEIVER_LEN];			// Email address
}
DPSDK_USER_EMAIL_USER_INFO;

// Email information
typedef struct
{
	DPSDK_INT32 iRoleNum;											// The number of role information
	DPSDK_USER_EMAIL_ROLE_INFO* pRoleList;							// The list of role information
	DPSDK_INT32 iUserNum;											// The number of user information
	DPSDK_USER_EMAIL_USER_INFO* pUserList;							// The list of user information
}DPSDK_USER_EMAIL_INFO;


// Email attachment
typedef struct
{
	DPSDK_CHAR szAttachmentPath[DPSDK_URL_LEN];						// The path of mail attachment
}DPSDK_USER_EMAIL_ATTACHMENT;

// Email recipient
typedef struct
{
	DPSDK_CHAR szName[DPSDK_NAME_LEN];								// The name of recipient
	DPSDK_CHAR szAddress[DPSDK_ALARM_EMAILRECEIVER_LEN];			// The mail address of recipient
}DPSDK_USER_EMAIL_RECIPIENT;

// Send email message parameter
typedef struct
{
	DPSDK_CHAR* pSubject;											// Email subject
	DPSDK_CHAR* pMessage;											// Email message
	DPSDK_INT32 iAttachmentPathNum;									// The number of mail attachment
	DPSDK_USER_EMAIL_ATTACHMENT* pAttachmentList;					// The list of mail attachment
	DPSDK_INT32 iRecipientNum;										// The number of recipient
	DPSDK_USER_EMAIL_RECIPIENT* pRecipientList;						// recipient
	DPSDK_INT32 iCCRecipientNum;									// The number of copy recipient
	DPSDK_USER_EMAIL_RECIPIENT* pCCRecipientList;					// copy recipient
	DPSDK_INT32 iBCCRecipientNum;									// The number of privite copy recipient
	DPSDK_USER_EMAIL_RECIPIENT* pBCCRecipientList;					// privite copy recipient
}DPSDK_SEND_EMAIL_PARAM;

// Save user
typedef struct
{
	DPSDK_CHAR szUserId[DPSDK_USER_ID_LEN];							// User id, when add user need not to assignment
	DPSDK_CHAR szGroupId[DPSDK_USER_ID_LEN];						// Group id
	DPSDK_CHAR szUserName[DPSDK_NAME_LEN];							// User  name
	DPSDK_CHAR szPassword[DPSDK_PWD_LEN];							// Password
	DPSDK_BOOL bIsPassword;											// Is it encryption
	DPSDK_CHAR szPasswordExpiry[DPSDK_PWD_EXPIRY_LEN];				// The expiry of password
	DPSDK_CHAR szMac[DPSDK_MACADDRESS_LEN];							// Mac address
	DPSDK_CHAR szRemark[DPSDK_USER_REMARK_LEN];						// Memo
	DPSDK_CHAR szMailAddress[DPSDK_ALARM_EMAILRECEIVER_LEN];		// Mail address
	DPSDK_USER_RIGHT_LIST struUserRightList;						// User right
	DPSDK_CHANNEL_ID_LIST struChannelIdList;						// The list of channel id
	DPSDK_ORE_CODE_LIST   struOrgCodeList;							// The list of organization node code
}DPSDK_EDIT_USER_INFO;

// User encrypted question
typedef struct
{
	DPSDK_INT32 iQuestionId;										// Question ID
	DPSDK_CHAR szDesc[DPSDK_MEMO_LEN];								// The detail describe of question
}DPSDK_USER_QUESTION_INFO;

// The list of user encrypted question
typedef struct
{
	DPSDK_INT32 iQuestionNum;										// The number of user encrypted question
	DPSDK_USER_QUESTION_INFO struQuestionList[1];					// The list of user encrypted question
}DPSDK_USER_QUESTION_LIST;

// The answer of user encrypted question
typedef struct
{
	DPSDK_INT32 iQuestionId;										// QuestionID
	DPSDK_CHAR szAnswer[DPSDK_MEMO_LEN];							// The answer of question
}DPSDK_USER_QUESTION_ANSWER_INFO;

// The list user encrypted question
typedef struct
{
	DPSDK_INT32 iQuestionNum;										// The number of user encrypted question
	DPSDK_USER_QUESTION_ANSWER_INFO struAnswerList[1];				// The list of answer
}DPSDK_USER_QUESTION_ANSWER_LIST;

// Time template
typedef struct
{
	DPSDK_INT32 iType;												// Template type
	DPSDK_INT32 iId;												// Template id
	DPSDK_CHAR szName[DPSDK_NAME_LEN];								// Template name
}DPSDK_TIME_TEMPLATE_BASE_INFO;

#define DPSDK_MAX_TIME_TEMPLATE_NUM		100							// Max nmuber of time template 
typedef struct
{
	DPSDK_INT32 iTemplateInfoNum;									// The number of time template 
	DPSDK_TIME_TEMPLATE_BASE_INFO szTemplateInfoList[DPSDK_MAX_TIME_TEMPLATE_NUM];				// The list of time template 
}DPSDK_TIME_TEMPLATE_BASE_INFO_LIST;

// The time slot of time template 
typedef struct
{
	DPSDK_CHAR szTimeStart[DPSDK_TIME_LEN];							// The point of start time
	DPSDK_CHAR szTimeEnd[DPSDK_TIME_LEN];							// The point of end time
}DPSDK_TIME_TEMPLATE_TIME_SLOT;

#define DPSDK_MAX_TIME_SLOT_NUM		100								// Max number of time slot
// The list of time slot
typedef struct
{
	DPSDK_INT32 iTimeSlotNum;										// The number of time slot
	DPSDK_TIME_TEMPLATE_TIME_SLOT szTimeSlotList[DPSDK_MAX_TIME_SLOT_NUM];						// The list of time slot
}DPSDK_TIME_TEMPLATE_TIME_SLOT_LIST;

// Time template
typedef struct
{
	DPSDK_TIME_TEMPLATE_BASE_INFO struBaseInfo;						// Basic information
	DPSDK_INT32 iType;												// Template type
	DPSDK_CHAR szMemo[DPSDK_MEMO_LEN];								// Video memo
	DPSDK_INT32 iLoopType;											// Loop type

	DPSDK_TIME_TEMPLATE_TIME_SLOT_LIST struTimeSlotList1;			// The list of time slot on monday
	DPSDK_TIME_TEMPLATE_TIME_SLOT_LIST struTimeSlotList2;			// The list of time slot on tuesday
	DPSDK_TIME_TEMPLATE_TIME_SLOT_LIST struTimeSlotList3;			// The list of time slot on wednesday
	DPSDK_TIME_TEMPLATE_TIME_SLOT_LIST struTimeSlotList4;			// The list of time slot on thursday
	DPSDK_TIME_TEMPLATE_TIME_SLOT_LIST struTimeSlotList5;			// The list of time slot on friday
	DPSDK_TIME_TEMPLATE_TIME_SLOT_LIST struTimeSlotList6;			// The list of time slot on saturday
	DPSDK_TIME_TEMPLATE_TIME_SLOT_LIST struTimeSlotList7;			// The list of time slot on sunday
	
}DPSDK_TIME_TEMPLATE_DETAIL_INFO;

#define DPSDK_LINK_VIDEO_MAX_NUM		100							// Max number of link video plan
#define DPSDK_STORAGE_PLAN_MAX_NUM		100							// Max number of storage plan
#define DPSDK_ALARM_PLAN_MAX_NUM		100							// Max number of alarm plan

typedef struct
{
	DPSDK_INT32 iStoPlansNum;										// The number of storage plan
	DPSDK_CHANNEL_ID* pStoPlans;									// The list of storage plan
}DPSDK_STO_PLANS_LIST;

typedef struct
{
	DPSDK_INT32 iAlarmPlansNum;										// The number of alarm plan
	DPSDK_CHANNEL_ID* pAlarmPlans;									// The list of alarm plan
}DPSDK_ALARM_PLANS_LIST;

typedef struct  
{
	DPSDK_CHAR szName[DPSDK_NAME_LEN];
}DPSDK_NAME;

typedef struct
{
	DPSDK_INT32 iNum;												// 
	DPSDK_NAME* pNameList;											// 
}DPSDK_NAME_LIST;

// Delete time template information
typedef struct
{
	DPSDK_CHAR szName[DPSDK_NAME_LEN];								// Time template name
	DPSDK_CHANNEL_ID_LIST struBackStoragePlanChnIds;				// The list of channel id about back storage plan
	DPSDK_CHANNEL_ID_LIST struRemoteVerificationChnIds;				// The list of remote verification channel id
	DPSDK_CHANNEL_ID_LIST struAlarmPlanDevChnIds;					// The channel of alarm plan source or list of device id
	DPSDK_CHANNEL_ID_LIST struMultiCardChnIds;						// The list of channel id about multi card
	DPSDK_CHANNEL_ID_LIST struDoorConfigChnIds;						// The list of channel id about door's basic configuration
	DPSDK_CHANNEL_ID_LIST struStoragePlanChnIds;					// The list of channel id about video plan
	DPSDK_CHANNEL_ID_LIST struFirstCardChnIds;						// The list of channel id about first card
	DPSDK_NAME_LIST	struAntiPassBackNames;							// The list of passback's name
	DPSDK_NAME_LIST	struInterDoorLockNames;							// The list of interdoor's name
	DPSDK_NAME_LIST	struDoorGroupNames;								// The list of door group's name
}DPSDK_DELETE_TIME_TEMPLATE_INFO;

// Alarm plan
typedef struct
{
	DPSDK_INT32 iId;												// Alarm plan id
	DPSDK_CHAR szName[DPSDK_NAME_LEN];								// Alarm plan name
	DPSDK_BOOL bEnable;												// Is it enable alarm plan
	DPSDK_INT32 iAlarmType;											// Alarm type
	DPSDK_INT32 iTimeTemplateId;									// Time template id
	DPSDK_INT32	iAlarmGrade;										// Alarm level(see AlarmLevel_e)
}DPSDK_ALARM_PLAN_BASE_INFO;

// Alarm plan list
typedef struct
{
	DPSDK_INT32 iAlarmPlanInfoNum;									// The number of alarm plan
	DPSDK_ALARM_PLAN_BASE_INFO szAlarmPlanInfoList[1];				// The list of alarm plan
}DPSDK_ALARM_PLAN_BASE_INFO_LIST;

// Linage PTZ
typedef struct
{
	DPSDK_CHAR szChannelId[DPSDK_ALARM_CHANNELID_LEN];				// Channel Id
	DPSDK_CHAR szPreset[DPSDK_PRESETPOINT_CODE_LEN];				// Preset point
}DPSDK_ALARM_LINK_PTZ_INFO;

// Linkage video information
typedef struct
{
	DPSDK_CHAR szChannelId[DPSDK_ALARM_CHANNELID_LEN];				// Channel Id
	DPSDK_CHAR szDeviceName[DPSDK_ALARM_DEVICENAME_LEN];			// Device name
	DPSDK_INT32 iChannelType;										// Channel type
	DPSDK_INT32 iBelongedDeviceCategory;							// Device category
	DPSDK_INT32 iDeviceType;										// Device type
	DPSDK_INT32 iScreenID;											// The id of screen which binding channel
}DPSDK_ALARM_RECORD_NODE_INFO;

// Linkage record information
typedef struct
{
	DPSDK_INT32 iStoragePosition;									// Storage point  see DPSDK_STORAGE_POSITION
	DPSDK_UINT32 uiRecordTime;										// Record time
	DPSDK_UINT32 uiPreRecordTime;									// Pre-record time
	DPSDK_INT32 iAssistantStream;									// Code stream type

	DPSDK_BOOL bSaveSnapshort;										// Is store picture
	DPSDK_BOOL bViewLinkCamera;										// Is linkage camera
	DPSDK_BOOL bLinkSelf;											// Is linkage self
	DPSDK_BOOL bOpenClient;											// Is display linkage on client
	DPSDK_INT32 iScreenNum;											// The number of window

	DPSDK_INT32 iAlarmRecordNum;									// The number of linkage video
	DPSDK_ALARM_RECORD_NODE_INFO* pAlarmRecordNodeList;				// The list of linkage video
}DPSDK_ALARM_RECORD_LIST;

// Link output channel information
typedef struct
{
	DPSDK_CHAR szChannelId[DPSDK_ALARM_CHANNELID_LEN];				// Output channel id
	DPSDK_UINT32 uiResidenceTime;									// Residence time
}DPSDK_ALARM_LINK_OUTPUT_INFO;

// Link mail imforrmation
typedef struct
{
	DPSDK_INT32 iSelectedUserId;									// Send user id
	DPSDK_CHAR szEmailAddress[DPSDK_ALARM_EMAILRECEIVER_LEN];		// Send mail's address, it is valid when the value of iSelectedUserId is 0
}DPSDK_ALARM_LINK_MAIL_INFO;

// Link TVWALL information
typedef struct
{
	DPSDK_CHAR szChannelId[DPSDK_ALARM_CHANNELID_LEN];				// Channel id
	DPSDK_INT32 iWindowId;											// Window id
	DPSDK_INT32 iWallId;											// TVWALL id
}DPSDK_ALARM_LINK_WALL_INFO;

// Link acess information
typedef struct
{
	DPSDK_CHAR szChannelId[DPSDK_ALARM_CHANNELID_LEN];				// Channel id
	DPSDK_INT32 iReactionType;										// Acess operate type 0£ºOpen door£»1: Close door; 2: Normally open; 3:  Normally close
}DPSDK_ALARM_LINK_DOOR_INFO;

// Alarm source information
typedef struct
{
	DPSDK_ALARM_PLAN_BASE_INFO struBaseInfo;						// Alarm plan's basic information

	DPSDK_INT32 iAlarmSourceNum;									// The number of alarm source
	DPSDK_CHANNEL_ID* pAlarmSourceCode;								// Alarm source code

	DPSDK_ALARM_RECORD_LIST struAlarmRecordList;					// The list of linkage video

	DPSDK_INT32 iAlarmPTZNum;										// The number of linkage PTZ
	DPSDK_ALARM_LINK_PTZ_INFO* pLinkPTZ;							// Link PTZ

	DPSDK_INT32 iAlarmLinkOutputNum;								// The number of linkage output channel
	DPSDK_ALARM_LINK_OUTPUT_INFO* pAlarmLinkOutputList;				// The list of linkage output channel

	DPSDK_CHAR szMailSubject[DPSDK_ALARM_MAIL_SUBJECT_LEN];			// Email title
	DPSDK_CHAR szMailFormat[DPSDK_ALARM_MAIL_FORMAT_LEN];			// Email formatting string

	DPSDK_INT32 iAlarmLinkMailNum;									// The number of linkage mail
	DPSDK_ALARM_LINK_MAIL_INFO* pAlarmLinkMailList;					// The list of linkage mail

	DPSDK_INT32 iAlarmLinkWallNum;									// The number of linkage TVWALL
	DPSDK_ALARM_LINK_WALL_INFO* pAlarmLinkWallList;					// The list of linkage TVWALL

	DPSDK_INT32 iAlarmLinkDoorNum;									// Then number of linkage acess
	DPSDK_ALARM_LINK_DOOR_INFO* pAlarmLinkDoorList;					// The list of linkage acess
}DPSDK_ALARM_SOURCE_INFO;

// User mail's information
typedef struct
{
	DPSDK_CHAR szUserName[DPSDK_NAME_LEN];							// User name
	DPSDK_CHAR szEmailAddress[DPSDK_ALARM_EMAILRECEIVER_LEN];		// User email's address
}DPSDK_USER_MAIL_INFO;

// The list of user mail
typedef struct
{
	DPSDK_INT32 iUserMailNum;										// The number of user's mail
	DPSDK_USER_MAIL_INFO struUserMailInfo[1];						// The list of user's mail
}DPSDK_USER_MAIL_LIST;

// Mails' information which are auto added
typedef struct
{
	DPSDK_CHAR szEmailAddress[DPSDK_ALARM_EMAILRECEIVER_LEN];		// Email address
}DPSDK_MAIL_INFO;

// The list of user mail
typedef struct
{
	DPSDK_INT32 iMailNum;											// The number of mail 
	DPSDK_MAIL_INFO struMailInfo[1];								// The list of mail 
}DPSDK_MAIL_LIST;

#define DPSDK_MAX_ALARM_SOURCE_NUM		128							// Max number of alarm source
// The detail information of alarm plan
typedef struct
{
	DPSDK_CHANNEL_ID struChannelId;									// Channel ID
	DPSDK_INT32 iAlarmSourceNum;									// The number of alarm source
	DPSDK_ALARM_SOURCE_INFO* pAlarmSourceList;						// The list of alarm source
}DPSDK_ALARM_PLAN_DETAIL_INFO;

// The list of alarm plan
typedef struct
{
	DPSDK_INT32 iAlarmPlanInfoNum;									// The number of alarm plan
	DPSDK_ALARM_PLAN_DETAIL_INFO* pAlarmPlanInfoList;				// The list of alarm plan
}DPSDK_ALARM_PLAN_DETAIL_INFO_LIST;

// Alarm plan copy parameter
typedef struct
{
	DPSDK_CHAR szSrcChannelId[DPSDK_CHANNEL_ID_LEN];				// Source channel ID
	DPSDK_INT32 iDestChannelNum;									// The number of destination channel id
	DPSDK_CHANNEL_ID* pDestChannelList;								// The list of destination channel id
}DPSDK_ALARM_PLAN_COPY_PARAM;

//
typedef struct
{
	DPSDK_CHAR szSeq[DPSDK_ID_LEN];									// Seq
}DPSDK_MANUAL_BACKUP_RESULT;

// Auto backup parameter
typedef struct
{
	DPSDK_CHAR szEncryptPassword[DPSDK_PWD_LEN];					// Encrypt Password
	DPSDK_CHAR szBackUpPath[DPSDK_FILE_PATH_LEN];					// Auto backup path
	DPSDK_INT32 iCycleTime;											// Backup cycle
	DPSDK_INT32 iDate;												// The date of backup cycle
	DPSDK_CHAR szTime[DPSDK_TIME_LEN];								// The time of cycle backup
	DPSDK_BOOL bEnable;												//true:Open£¬false£ºClose
}DPSDK_AUTO_BACKUP_PARAM;

// Auto backup file information
typedef struct
{
	DPSDK_CHAR szSN[DPSDK_SN_LEN];									// The id of backup restoration file
	DPSDK_TIMET tTime;												// The time of backup restoration
	DPSDK_UINT32 uiSize;											// The of backup file£¨kb£©
	DPSDK_CHAR szName[DPSDK_NAME_LEN_EX];
}DPSDK_AUTO_BACKUP_FILE_INFO;

#define DPSDK_MAX_BACKUP_FILE_NUM		100							// Max number of backup file
// The list of auto backup file
typedef struct
{
	DPSDK_INT32 iFileNum;																		// The number of file which return from server
	DPSDK_AUTO_BACKUP_FILE_INFO szAutoBackUpFileList[DPSDK_MAX_BACKUP_FILE_NUM];				// The list of auto backup file
}DPSDK_AUTO_BACKUP_FILE_LIST;

// Linkage video information
typedef struct
{
	DPSDK_CHAR szChannelCode[DPSDK_CHANNEL_ID_LEN];					// Channel code
	DPSDK_CHAR szChannelName[DPSDK_CHANNEL_NAME_LEN];				// Source channel name
	DPSDK_CHAR szDeviceCode[DPSDK_DEVICE_ID_LEN];					// Source channel device code
	DPSDK_CHAR szDeviceName[DPSDK_DEVICE_NAME_LEN];					// Source channel device name
}DPSDK_VIDEO_RELATION_INFO;

#define DPSDK_MAX_VIDEO_RELATION_NUM		128						// Max number of linkage video channel
// The list of linkage video
typedef struct
{
	DPSDK_INT32 iVideoRelationNum;																// The number of linkage video
	DPSDK_VIDEO_RELATION_INFO szVideoRelationList[DPSDK_MAX_VIDEO_RELATION_NUM];				// The list of linkage video
}DPSDK_VIDEO_RELATION_LIST;

// Save linkage video's information
typedef struct
{
	DPSDK_CHAR szId[DPSDK_CHANNEL_ID_LEN];							// Binding resource's id
	DPSDK_CHAR szName[DPSDK_CHANNEL_NAME_LEN];						// Name
	DPSDK_INT32 iType;												// Node type
	DPSDK_CHAR szPId[DPSDK_ID_LEN];									// Parent node
	DPSDK_CHAR szPPId[DPSDK_ID_LEN];								// Grandfather node
}DPSDK_VIDEO_RELATION_NODE;

// Save linkage video's parameter
typedef struct
{
	DPSDK_INT32 iOperation;											// Operate type insert 0, update 1
	DPSDK_CHAR szSourceId[DPSDK_CHANNEL_ID_LEN];					// Channel id
	DPSDK_CHAR szName[DPSDK_CHANNEL_NAME_LEN];						// Channel name
	DPSDK_INT32 iRelationNodeNum;									// The number of linkage
	DPSDK_VIDEO_RELATION_NODE szRelationNodeList[DPSDK_MAX_VIDEO_RELATION_NUM];					// The list of linkage
}DPSDK_SAVE_VIDEO_RELATION_PARAM;

// The list of linkage video information
typedef struct
{
	DPSDK_CHAR szSourceId[DPSDK_CHANNEL_ID_LEN];					// Channel id
	DPSDK_INT32 iRelationNodeNum;									// The number of linkage
	DPSDK_CHAR szIdList[DPSDK_MAX_VIDEO_RELATION_NUM][DPSDK_CHANNEL_ID_LEN];					// The list of linkage
}DPSDK_VIDEO_RELATION_NODE_LIST;

// Parameter Configure information
typedef struct
{
	DPSDK_INT32 iId;												// Parameter Configure id
	DPSDK_CHAR szConfigParam1[DPSDK_CONFIG_PARAM_LEN];				// Parameter 1
	DPSDK_CHAR szConfigParam2[DPSDK_CONFIG_PARAM_LEN];				// Parameter 2
	DPSDK_CHAR szConfigParam3[DPSDK_CONFIG_PARAM_LEN];				// Parameter 3
}DPSDK_SERVER_CONFIG_INFO; 

typedef struct
{
	DPSDK_CHAR szConfigType[DPSDK_TYPE_LEN];
}DPSDK_CONFIG_TYPE; 

typedef struct
{
	DPSDK_INT32 iConfigNum;
	DPSDK_CONFIG_TYPE* pConfigTypeList;
}DPSDK_CONFIG_TYPE_INFO; 

// https certificate information
typedef struct
{
	DPSDK_SERVER_CONFIG_INFO struConfigInfo;						// Parameter Configure information
	DPSDK_UINT32 uiHttpsFileLength;									// The length of https certificate 
	DPSDK_CHAR* pHttpsFileData;										// The data of https certificate
}DPSDK_HTTPS_CONFIG_INFO;

typedef struct
{
	DPSDK_UINT32 uiStorageTime;										// The time of save image,unit: day
	DPSDK_INT32 iRemainderSize;
}DPSDK_IMAGE_STORAGE_INFO;

// Storage Configure information 
typedef struct
{
	DPSDK_UINT32 uiLogSaveTime;										// The time of save log,unit: day
	DPSDK_UINT32 uiAlarmInfoSaveTime;								// The time of save alarm information,unit: day
	DPSDK_UINT32 uiHeatMapSaveTime;									// The time of save heat map,unit: day
	DPSDK_UINT32 uiPosSaveTime;										// The time of save pos,unit: day
	DPSDK_UINT32 uiPicRecordSaveTime;								// The time of save picRecord,unit: day
	DPSDK_UINT32 uiFaceRecordSaveTime;
	DPSDK_UINT32 uiAccessSaveTime;
	DPSDK_UINT32 uiVtalkCallSaveTime;
	DPSDK_IMAGE_STORAGE_INFO struImageStorageInfo;
}DPSDK_STORAGE_CONFIG_INFO;

// Pos end information 
typedef struct
{
	DPSDK_CHAR szPosEnd[DPSDK_MEMO_LEN];							// Pos end
}DPSDK_POS_END_INFO;

// Weather information 
typedef struct
{
	DPSDK_BOOL bEnable;												// false=disenable,true=enable
	DPSDK_CHAR szLocation[DPSDK_NAME_LEN];							// Location
	DPSDK_CHAR szCountry[DPSDK_NAME_LEN];							// Country
}DPSDK_WEATHER_INFO;

// FTP Configure information
typedef struct
{
	DPSDK_CHAR szLANPath[DPSDK_IP_LEN];								// LAN IP
	DPSDK_CHAR szWANPath[DPSDK_IP_LEN];								// Network IP
	DPSDK_CHAR szUserName[DPSDK_NAME_LEN];							// User name
	DPSDK_CHAR szPWD[DPSDK_PWD_LEN];								// Password
}DPSDK_FTP_CONFIG_INFO;

// Sync time configure information
typedef struct
{
	DPSDK_BOOL bEnable;												// Enable
	DPSDK_CHAR szStartTime[DPSDK_TIME_LEN];							// Start time hh:mm:ss
	DPSDK_UINT32 uiInterval;										// Time slot, unit: s
	DPSDK_BOOL bImmediately;										// Whether sync in time
}DPSDK_TIME_SYNC_CONFIG_INFO;

// Encrypt type
typedef enum
{
	MAIL_ENCRYPT_DISABLE = 0,										// No encrypt
	MAIL_ENCRYPT_TLS = 1,											// TLS encrypt
	MAIL_ENCRYPT_SSL = 2,											// SSL encrypt
}DPSDK_MAIL_ENCRYPT_TYPE;

// Mail configure information
typedef struct
{
	DPSDK_BOOL bEnable;												// Enable
	DPSDK_CHAR szSMTPServer[DPSDK_IP_LEN];							// SMTP mail address
	DPSDK_INT32 iSMTPPort;											// SMTP port
	DPSDK_INT32 iEncryptType;										// Encrypt type see DPSDK_MAIL_ENCRYPT_TYPE
	DPSDK_CHAR szPWD[DPSDK_PWD_LEN];								// Password
	DPSDK_CHAR szSenderMailAddress[DPSDK_ALARM_EMAILRECEIVER_LEN];	// The mail of sender
	DPSDK_CHAR szRecipientMailAddress[DPSDK_ALARM_EMAILRECEIVER_LEN];// Sending test mail
}DPSDK_MAIL_CONFIG_INFO;


// User statue
typedef struct
{
	DPSDK_CHAR szName[DPSDK_NAME_LEN];								// User name
	DPSDK_UINT32 uiTotalTimes;										// The total number of login
	DPSDK_UINT32 uiTotalLoginTime;									// The total time of login
	DPSDK_TIMET tLastLoginTime;										// The time of last login
}DPSDK_USER_STATUS_INFO;

// The list of user status
typedef struct
{
	DPSDK_INT32 iUserStatusNum;										// The number of user status
	DPSDK_USER_STATUS_INFO szUserStatusList[DPSDK_MAX_USER_NUM];	// The list of user status
}DPSDK_USER_STATUS_LIST;

// The parameter of query device status
typedef struct
{
	DPSDK_CHAR szKeyWord[DPSDK_KEYWORD_LEN];						// The keywords of search
	DPSDK_TIMET tBeginTime;											// Start time
	DPSDK_TIMET tEndTime;											// End time
	DPSDK_CHAR szOrgCode[DPSDK_ORG_CODE_LEN];						// Query organization code
}DPSDK_QUERY_DEVICE_STATUS_PARAM;

// Device status information 
typedef struct
{
	DPSDK_TIMET tTime;												// The time of device on-line and off-line
	DPSDK_INT32 iStatus;											// Device status
	DPSDK_CHAR szDeviceName[DPSDK_DEVICE_NAME_LEN];					// Device name
	DPSDK_CHAR szOrgName[DPSDK_ORG_NAME_LEN];						// Organization name
	DPSDK_IP struIP;												// Device ip
}DPSDK_DEVICE_STATUS_INFO;

#define DPSDK_MAX_DEVICE_NUM		100								// Max number of single query
// The list of device status
typedef struct
{
	DPSDK_INT32 iDeviceStatusNum;									// The number of device status
	DPSDK_DEVICE_STATUS_INFO szDeviceStatusList[DPSDK_MAX_DEVICE_NUM];// The list of device status
}DPSDK_DEVICE_STATUS_LIST;

// The parameter of query device health report
typedef struct
{
	DPSDK_CHAR szKeyWord[DPSDK_KEYWORD_LEN];						// The keywords of search
	DPSDK_CHAR szOrgCode[DPSDK_ORG_CODE_LEN];						// Query organization code
}DPSDK_QUERY_DEVICE_HEALTH_REPORT_PARAM;

// The information of device health report
typedef struct
{
	DPSDK_CHAR szDeviceCode[DPSDK_DEVICE_CODE_LEN];					// Device code
	DPSDK_CHAR szDeviceName[DPSDK_DEVICE_NAME_LEN];					// Device name
	DPSDK_INT32 iType;												// Device type
	DPSDK_IP struIP;												// Device ip
	DPSDK_INT32 iStatus;											// Device on-line status 
	DPSDK_INT32 iHardDiskStatus;									// Hard disk status
	DPSDK_BOOL bVideoLoss;											// Video loss
	DPSDK_BOOL bTampering;											// Video tampering 
	DPSDK_BOOL bExternalAlarm;										// External alarm
	DPSDK_BOOL bManualRecord;										// Manual record
	DPSDK_BOOL bAutoRecord;											// Auto record
}DPSDK_DEVICE_HEALTH_REPORT_INFO;

// The list of device health report
typedef struct
{
	DPSDK_INT32 iDeviceHealthReportNum;													// The number of device health report
	DPSDK_DEVICE_HEALTH_REPORT_INFO szDeviceHealthReportList[DPSDK_MAX_DEVICE_NUM];		// The list of device health report
}DPSDK_DEVICE_HEALTH_REPORT_LIST;

// Query log parameter
typedef struct
{
	DPSDK_CHAR szKeyWord[DPSDK_KEYWORD_LEN];						// The keywords of search
	DPSDK_INT32 iEventType;											// Query module event
	DPSDK_TIMET tBeginTime;											// Start time
	DPSDK_TIMET tEndTime;											// End time
}DPSDK_QUERY_LOG_PARAM;

// Export log type
typedef enum
{
	EXPORT_LOG_CVS = 1,													// cvs
	EXPORT_LOG_XLS = 2,													// xls
	EXPORT_LOG_TXT = 3,													// txt
}DPSDK_EXPORT_LOG_TYPE;

//Device code
typedef struct
{
	DPSDK_CHAR szDeviceCode[DPSDK_DEVICE_CODE_LEN];			// Device code
}DPSDK_DEVICE_CODE;

// Export system log parameter
typedef struct
{
	DPSDK_CHAR szCardNum[DPSDK_MEMO_LEN];							// Card number
	DPSDK_CHAR szPersonNum[DPSDK_MEMO_LEN];							// Person code
	DPSDK_CHAR szPersonName[DPSDK_NAME_LEN];						// Person name
	DPSDK_CHAR szDepartmentId[DPSDK_ID_LEN];						// Organization code id
	DPSDK_CHAR szKeyWord[DPSDK_KEYWORD_LEN];						// The keywords of search
	//DPSDK_CHAR szDeviceCode[DPSDK_DEVICE_CODE_LEN];					// Device code
	DPSDK_INT32 iDeviceNum;											// device quantity
	DPSDK_DEVICE_CODE* pDeviceCodeList;								// device code list
	DPSDK_INT32 iEventType;											// Query module event
	DPSDK_TIMET tBeginTime;											// Start time
	DPSDK_TIMET tEndTime;											// End time
	DPSDK_INT32 iExportType;										// Export type see DPSDK_EXPORT_LOG_TYPE
	DPSDK_CHAR* pLocal;												// International
	DPSDK_INT32 iChannelNum;										// The number of channel
	DPSDK_CHANNEL_ID* pChannelIdList;									// The list of channel id
	DPSDK_INT32 iSubEventType;
}DPSDK_EXPORT_LOG_PARAM;

// Log information
typedef struct
{
	DPSDK_TIMET tTime;												// Operate time
	DPSDK_CHAR szUserName[DPSDK_NAME_LEN];							// Operator name
	DPSDK_INT32 iEventType;											// Query module event
	DPSDK_CHAR szEventContent[DPSDK_ALARM_ALARMMESSAGE_LEN];		// Event content
	DPSDK_IP struIP;												// Operator ip
}DPSDK_LOG_INFO;

// The detail information of acess
typedef struct  
{
	DPSDK_CHAR szDevCode[DPSDK_DEVICE_CODE_LEN];					// Device code
	DPSDK_CHAR szDevMode[DPSDK_MEMO_LEN];							// Device module
	DPSDK_CHAR szSoftVersion[DPSDK_MEMO_LEN];						// soft version
	DPSDK_INT32 iRecordCount;										// The number of record
	DPSDK_INT32 iAlarmCount;										// The number of alarm
}DPSDK_ACCESS_DOOR_DETAIL;

#define DPSDK_MAX_LOG_NUM		100									// Max number of log single query
// The list of log
typedef struct
{
	DPSDK_INT32 iLogNum;											// The number of log
	DPSDK_LOG_INFO szLogList[DPSDK_MAX_LOG_NUM];					// The list of log
}DPSDK_LOG_LIST;

// Sub-service information
typedef struct
{
	DPSDK_CHAR szName[DPSDK_NAME_LEN];								// Server name
	DPSDK_INT32 iType;												// Server type
	DPSDK_INT32 iRunningStatus;										// Run status
}DPSDK_CHILD_SERVER_INFO;

// Server status information
#define DPSDK_MAX_CHILD_SERVER_NUM		100							// Max number of sub-service
typedef struct
{
	DPSDK_CHAR szName[DPSDK_NAME_LEN];								// Service name
	DPSDK_IP struIP;												// Service ip
	DPSDK_INT32 iType;												// Service type
	DPSDK_INT32 iRunningStatus;										// Run status
	DPSDK_INT32 iEnableStatus;										// Enable status
	DPSDK_INT32 iServerId;											// Service code

	DPSDK_INT32 iServerNum;											// The number of service
	DPSDK_CHILD_SERVER_INFO szServer[DPSDK_MAX_CHILD_SERVER_NUM];	// The list of service
}DPSDK_SERVER_STATUS_INFO;

#define DPSDK_MAX_SERVER_NUM		100								// Max number of service single query
// The list of service status
typedef struct
{
	DPSDK_INT32 iStatusInfoNum;										// The number of service status
	DPSDK_SERVER_STATUS_INFO szStatusInfoList[DPSDK_MAX_SERVER_NUM];// The list of service status
}DPSDK_SERVER_STATUS_LIST;

// Service information
typedef struct
{
	DPSDK_CHAR szName[DPSDK_NAME_LEN];								// Service name
	DPSDK_INT32 iPort;												// Service port
	DPSDK_INT32 iStatus;											// Service status
}DPSDK_REMOTE_SERVER_INFO;

// The list of service
typedef struct
{
	DPSDK_INT32 iServerNum;											// The number of service
	DPSDK_REMOTE_SERVER_INFO szServerList[1];						// The list of service
}DPSDK_SERVER_LIST;

// Change service status
typedef struct
{
	DPSDK_CHAR szName[DPSDK_NAME_LEN];								// Service name
	DPSDK_INT32 iOperation;											// Operate
	DPSDK_INT32 iPort;												// Service port
}DPSDK_CHANGE_SERVER_INFO;

// Change service status
typedef struct
{
	DPSDK_BOOL bStartAll;											// true Open all services, false Close all services
}DPSDK_CHANGE_SERVER_STATUS_PARAM;

// Change service ip, port, the mapping of internal and external network
typedef struct
{
	DPSDK_BOOL bMaster;												// Is it master model, true/false,will options
	DPSDK_IP struRegisterIP;										// Register ip (CMS address), will options
	DPSDK_INT32 iRegisterPort;										// Register port(CMS port),will options
	DPSDK_IP struMasterIP;											// Host ip, the default value is registerIp£¬optional
	DPSDK_INT32 iMasterPort;										// Host port, the default value is port tomcat visite, optional
	DPSDK_IP struMappingIP;											// Mapping ip£¬optional
}DPSDK_CONFIG_SERVER_INFO;

// The information of server function
typedef struct
{
	DPSDK_CHAR szServerName[DPSDK_NAME_LEN];						// Service name
	DPSDK_INT32 iStatus;											// Status: 1 open, 0 close
}DPSDK_SERVER_FUNCTION_INFO;

// The list of server function
typedef struct
{
	DPSDK_UINT32 uiTotal;											// The number of server function
	DPSDK_SERVER_FUNCTION_INFO struFunctionList[1];					// The list of server function
}DPSDK_SERVER_FUNCTION_LIST;

// Order information
typedef struct
{
	DPSDK_UINT32 uiOrderType;										// Order type
	DPSDK_UINT32 uiDirection;										// Order direction, 0£ºAscending order, 1£ºDescending order
}DPSDK_ORDER_INFO;

typedef enum
{
	DEVICE_PROTOCOL_TYPE_DAHUA = 1,									// Dahua protocol
	DEVICE_PROTOCOL_TYPE_ONVIF = 2,									// Onvif protocol
	DEVICE_PROTOCOL_TYPE_HIKVISION = 3,								// HIKVISION protocol
}DPSDK_DEVICE_PROTOCOL_TYPE;

typedef enum
{
	DEVICE_STATUS_TYPE_ALL = 1,										// 
	DEVICE_STATUS_TYPE_MANAGED = 2,									// 
	DEVICE_STATUS_TYPE_UNMANAGED = 3,								// 
}DPSDK_DEVICE_STATUS_TYPE;

// Search device's information
typedef struct
{
	DPSDK_CHAR szDummyId[DPSDK_ID_LEN];								// Dummy id,the value of it will to background when join management
	DPSDK_INT32 iInitStatus;										// Initialize status
	DPSDK_CHAR szDeviceName[DPSDK_DEVICE_NAME_LEN];					// Device name
	DPSDK_INT32 iDeviceType;										// Device type see DPSDK_DEV_TYPE
	DPSDK_IP struDeviceIP;											// Device ip
	DPSDK_INT32 iDevicePort;										// Device port
	DPSDK_CHAR szMac[DPSDK_MACADDRESS_LEN];							// Mac address
	DPSDK_CHAR szLoginName[DPSDK_NAME_LEN];							// User name
	DPSDK_INT32 iDeviceModel;										// Device model see DPSDK_DEVICE_MODEL
	DPSDK_CHAR szDeviceSN[DPSDK_SN_LEN];							// serial number
	DPSDK_INT32 iProtocolType;										// Protocol type see DPSDK_DEVICE_PROTOCOL_TYPE
	DPSDK_INT32 iEncChannelNum;										// The number of encode channel
	DPSDK_INT32 iMgrStatus;											// System management status see DPSDK_DEVICE_STATUS_TYPE
	DPSDK_INT32 iManufacturer;
}DPSDK_SEARCH_DEVICE_INFO;

#define DPSDK_MAX_SEARCH_DEVICE_NUM		500							// Max number of device single query



// Search device list
typedef struct
{
	DPSDK_INT32 iSearchDeviceNum;									// The number of search device
	DPSDK_SEARCH_DEVICE_INFO szSearchDevList[1];					// The list of search device
}DPSDK_SEARCH_DEVICE_LIST;

// The request parameter of search device 
typedef struct
{
	DPSDK_IP struStartIP;											// Start IP
	DPSDK_IP struEndIP;												// End IP
	DPSDK_INT32 iProtocolType;										// Protocol type see DPSDK_DEVICE_PROTOCOL_TYPE
	DPSDK_INT32 iStatus;											// see DPSDK_DEVICE_STATUS_TYPE
}DPSDK_SEARCH_DEVICE_PARAM;

// Device join management
typedef struct
{
	DPSDK_INT32 iDummyIdNum;										// The number of dummy id 
	DPSDK_CHAR szDummyIdList[DPSDK_MAX_DEVICE_NUM][DPSDK_ID_LEN];	// The list of dummy id 
	DPSDK_CHAR szOrgCode[DPSDK_ORG_CODE_LEN];						// Organization code
	DPSDK_CHAR szVideoServerCode[DPSDK_SERVER_CODE_LEN];			// Video server code
	DPSDK_CHAR szLoginName[DPSDK_NAME_LEN];							// Login name
	DPSDK_CHAR szLoginPWD[DPSDK_PWD_LEN];							// Device password (encrypt)
}DPSDK_SAVE_DEVICE_INFO;

// Add device information
typedef struct
{
	DPSDK_CHAR szDeviceName[DPSDK_DEVICE_NAME_LEN];					// Device name
	DPSDK_INT32 iDeviceCategory;									// Device category
	DPSDK_IP struDeviceIp;											// Device ip
	DPSDK_INT32 iDevicePort;										// Device port
	DPSDK_CHAR szOrgCode[DPSDK_ORG_CODE_LEN];						// Organization code
	DPSDK_CHAR szLoginName[DPSDK_NAME_LEN];							// Login name
	DPSDK_CHAR szLoginPWD[DPSDK_PWD_LEN];							// Device password (encrypt)
	DPSDK_CHAR szDeviceSn[DPSDK_SN_LEN];							// Device serial number
	DPSDK_INT32 iProtocolType;										// Protocol see DPSDK_DEVICE_PROTOCOL_TYPE
	DPSDK_INT32 iDeviceType;										// Device type see DPSDK_DEV_TYPE
	DPSDK_INT32 iDeviceModel;										// Device model see DPSDK_DEVICE_MODEL
	DPSDK_CHAR szMemo[DPSDK_MEMO_LEN];								// Memo information
	DPSDK_INT32 iManFac;											// Manufacture see DPSDK_DEVICE_MANFAC
	DPSDK_CHAR szVideoServerCode[DPSDK_SERVER_CODE_LEN];			// Video server code
	DPSDK_CHAR szItcServerCode[DPSDK_SERVER_CODE_LEN];				// Picture server code
	DPSDK_INT32 iEncChannelNum;										// The number of encode channel, optional
	DPSDK_INT32 iAlarmInChannelNum;									// he number of alarm input channel, optional
	DPSDK_INT32 iAlarmOutChannelNum;								// The number of alarm output channel, optional
	DPSDK_INT32 iDecChannelNum;										// The number of decode channel, optional
	DPSDK_INT32 iAccessChannelNum;									// The number of acess channel, optional
	DPSDK_INT32 iDevDecodeMode;										// Decode model
	DPSDK_INT32  iConbineStatus;									// Is it support conbine, 1 support conbine,0 not support conbine
	DPSDK_CHAR szPlatformId[DPSDK_ID_LEN];							// Platform ID
	DPSDK_CHAR szRegionId[DPSDK_ID_LEN];							// VAL domain ID
	DPSDK_CHAR szAddType[DPSDK_MEMO_LEN];							// Add type:ip-ip address,sn-serial number
	DPSDK_CHAR szSipId[DPSDK_ID_LEN];								// Vttalk SIP-ID
	DPSDK_CHAR szSipPwd[DPSDK_PWD_LEN];								// Vttalk SIP password(cipher)
	DPSDK_CHAR szDummyId[DPSDK_ID_LEN];                             // Dummy Id
}DPSDK_ADD_DEVICE_INFO;

#define DPSDK_MAX_ADD_DEVICE_NUM		100							// Max number of device single add

// The list of add device
typedef struct
{
	DPSDK_INT32 iAddDeviceNum;										// The number of add device
	DPSDK_ADD_DEVICE_INFO szAddDevList[DPSDK_MAX_ADD_DEVICE_NUM];	// The list of add device
}DPSDK_ADD_DEVICE_LIST;

// The result of add device
typedef struct
{
	DPSDK_DEVICE_ID struDeviceID;									// Device ID
}DPSDK_ADD_DEVICE_RESULT;

// The result of save device
typedef struct
{
	//DPSDK_CHAR szErrorMsg[DPSDK_ERROR_LEN];
	DPSDK_INT32 iErrorCode;
	DPSDK_BOOL bResult;
	DPSDK_CHAR szDummyId[DPSDK_ID_LEN];
}DPSDK_SAVE_DEVICE_RESULT;

// The result of save device
typedef struct
{
	DPSDK_INT32 iSaveDeviceNum;									// The number of save device
	DPSDK_SAVE_DEVICE_RESULT struSaveDeviceList[1];				// The list of save device
}DPSDK_SAVE_DEVICE_LIST;

// The dummy Id
typedef struct
{
	DPSDK_CHAR szDummyId[DPSDK_ID_LEN];                             // Dummy Id	
	DPSDK_INT32 iErrorCode;
}DPSDK_FAILED_DEVICE;

// The result of add device
typedef struct
{
	DPSDK_INT32 iSuccessNum;											// The success number of add device
	DPSDK_INT32 iFailNum;												// The fail number of add device
	DPSDK_DEVICE_ID* pDeviceIdList;										// The success list of add device
	DPSDK_FAILED_DEVICE* pFailedDeviceIdList;										// The list of dummy id 
}DPSDK_ADD_DEVICE_RESULT_LIST;

// Device dictionary
typedef struct
{
	DPSDK_INT32 iID;												// ID
	DPSDK_INT32 iCode;												// 
	DPSDK_CHAR szName[DPSDK_NAME_LEN];								// Name
	DPSDK_CHAR szInName[DPSDK_NAME_LEN];							// Internationalize name 
	DPSDK_CHAR szParam1[DPSDK_MEMO_LEN];							// Parameter 1
	DPSDK_CHAR szParam2[DPSDK_MEMO_LEN];							// Parameter 2
	DPSDK_INT32 iSort;												// Sort
	DPSDK_INT32 iStatus;											// Status
	DPSDK_CHAR szMemo[DPSDK_MEMO_LEN];								// Describe
	DPSDK_INT32 iTypeCode;											// 
	DPSDK_BOOL bAllowEdit;											// 
	DPSDK_BOOL bAllowDelete;										//
}DPSDK_DEV_DICTIONARY;

// The list of device dictionary
typedef struct
{
	DPSDK_INT32 iDictionaryNum;										// The number of dictionary
	DPSDK_DEV_DICTIONARY szDictionaryDevList[1];					// The list of dictionary
}DPSDK_DEV_DICTIONARY_LIST;

typedef struct
{
	DPSDK_CHAR szId[DPSDK_ID_LEN_EX];
}DPSDK_READ_ID;

// Update channel information
typedef struct
{
	DPSDK_CHAR szDeviceCode[DPSDK_DEVICE_ID_LEN];					// Device code
	DPSDK_INT32 iUnitSeq;											// Unit sequence
	DPSDK_INT32 iChannelSeq;										// Channel sequence
	DPSDK_CHAR szChannelName[DPSDK_CHANNEL_NAME_LEN];				// Channel name
	DPSDK_CHAR szChannelId[DPSDK_CHANNEL_ID_LEN];					// Channel ID
}DPSDK_UPDATE_CHANNEL_INFO;

// Update channel information
typedef struct
{
	DPSDK_CHAR szDeviceCode[DPSDK_DEVICE_ID_LEN];					// Device code
	DPSDK_INT32 iUnitSeq;											// Unit sequence
	DPSDK_INT32 iChannelSeq;										// Channel sequence
	DPSDK_CHAR szChannelName[DPSDK_CHANNEL_NAME_LEN];				// Channel name
	DPSDK_CHAR szChannelId[DPSDK_CHANNEL_ID_LEN];					// Channel ID
	DPSDK_INT32 iSubControllerSeq;
	DPSDK_INT32 iReadIdNum;
	DPSDK_READ_ID* pReadIdList;
}DPSDK_UPDATE_ACCESS_CONTROL_CHANNEL_INFO;

// Update channel external information
typedef struct
{
	DPSDK_INT32 iRoadway;											// Roadway
	DPSDK_FLOAT fCartMaxSpeed;										// Max speed of cart
	DPSDK_FLOAT fCartMinSpeed;										// Min speed of cart
	DPSDK_FLOAT fDollyMaxSpeed;										// Max speed of doll
	DPSDK_FLOAT fDollyMinSpeed;										// Min speed of doll
	DPSDK_FLOAT fDirection;											// The direction of car
}DPSDK_UPDATE_CHANNEL_EXT_INFO;

// Update encode channel information
typedef struct
{
	DPSDK_UPDATE_CHANNEL_INFO struBaseChnInfo;						// Basic information 
	DPSDK_CHAR szChannelSn[DPSDK_SN_LEN];							// The single serial number of channel
	DPSDK_CHAR szKeyCode[DPSDK_MEMO_LEN];							// Key code
	DPSDK_INT32 iChannelType;										// Channel type see DPSDK_CHANNEL_TYPE
	DPSDK_INT32 iChannelRemoteType;									// Remote channel type see DPSDK_CHANNEL_REMOTE_TYPE
	DPSDK_INT32 iCameraFunction;									// 0£ºhave no support	1£ºsupport fish eye		2£ºsupport motorized focus 
	DPSDK_INT32 iCameraType;										// Camera type see DPSDK_CAMERA_TYPE
	DPSDK_INT32 iFaceFunction;										// Face function,0 not support£¬1 snap£¬2 identify
	DPSDK_INT32 iPCFlag;											// 1=Area,2=In-Out,3=Out Door
	DPSDK_UPDATE_CHANNEL_EXT_INFO struChnExtInfo;					// External information
}DPSDK_UPDATE_ENC_CHANNEL_INFO;

// Update decode channel information
typedef struct
{
	DPSDK_UPDATE_CHANNEL_INFO struBaseChnInfo;						// Basic information
	DPSDK_CHAR szChannelSn[DPSDK_SN_LEN];							// The single serial number of channel 
	DPSDK_INT32 iMaxSplitNum;										// The max number of split
}DPSDK_UPDATE_DEC_CHANNEL_INFO;

// Update big screen input information
typedef struct
{
	DPSDK_UPDATE_CHANNEL_INFO struBaseChnInfo;						// Basic information
	DPSDK_CHAR szChannelSn[DPSDK_SN_LEN];							// The single serial number of channel 
	DPSDK_INT32 iCameraType;										// Camera type see DPSDK_CAMERA_TYPE
	DPSDK_INT32 iChannelRemoteType;									// Remote channel type see DPSDK_CHANNEL_REMOTE_TYPE
}DPSDK_UPDATE_SCREEN_INPUT_CHANNEL_INFO;

typedef struct
{
	DPSDK_INT32 iSubControllerSeq;
	DPSDK_CHAR szName[DPSDK_NAME_LEN_EX];
	DPSDK_INT32 iProperty;
}DPSDK_DEVICE_CONTROLLER_INFO;

// Update device information
typedef struct
{
	DPSDK_CHAR szDeviceCode[DPSDK_DEVICE_ID_LEN];					// Device code
	DPSDK_CHAR szDeviceName[DPSDK_DEVICE_NAME_LEN];					// Device name
	DPSDK_CHAR szSerialNo[DPSDK_SN_LEN];							// Serial number
	DPSDK_INT32 iDeviceType;										// Device type
	DPSDK_INT32 iDeviceCategory;									// Device category
	DPSDK_INT32 iDeviceModel;										// Device model
	DPSDK_INT32 iManFac;											// Manufacture see DPSDK_DEVICE_MANFAC
	DPSDK_INT32 iProtocolType;										// Protocol type see DPSDK_DEVICE_PROTOCOL_TYPE
	DPSDK_IP struDeviceIp;											// Device ip
	DPSDK_INT32 iDevicePort;										// Device port
	DPSDK_CHAR szLoginName[DPSDK_NAME_LEN];							// Login Name
	DPSDK_CHAR szLoginPWD[DPSDK_PWD_LEN];							// Device password (encrypt)
	DPSDK_CHAR szOrgCode[DPSDK_ORG_CODE_LEN];						// Organization code
	DPSDK_CHAR szDeviceSn[DPSDK_SN_LEN];							// Device serial number
	DPSDK_CHAR szMemo[DPSDK_MEMO_LEN];								// Memo information

	DPSDK_CHAR szVideoServerCode[DPSDK_SERVER_CODE_LEN];			// Video server code
	DPSDK_CHAR szItcServerCode[DPSDK_SERVER_CODE_LEN];				// Picture server code
	DPSDK_CHAR szPlatformId[DPSDK_ID_LEN];							// Platform ID
	DPSDK_INT32 iStatus;											// online¡¢offline¡¢unknown
	DPSDK_CHAR szSipId[DPSDK_ID_LEN];								// Vtcall SIP-ID
	DPSDK_CHAR szSipPwd[DPSDK_PWD_LEN];								// Vtcall SIP password
	DPSDK_CHAR szCallNum[DPSDK_PHONE_LEN];							// Vtcall call number
	DPSDK_INT32 iUnitEnable;
	DPSDK_INT32 iBuildingEnable;

	DPSDK_CHAR	szProxyIp[DPSDK_IP_LEN];							// Proxy Ip
	DPSDK_INT32 iProxyPort;											// Proxy Port

	DPSDK_CHAR szSoftwareVersion[DPSDK_ID_LEN];
	DPSDK_CHAR szHardwareVersion[DPSDK_ID_LEN];

	// Encode unit
	DPSDK_INT32 iStreamType;										// Code stream type see DPSDK_STREAM_TYPE
	DPSDK_BOOL bZeroEncode;											// Is it support zero channel and multi picture
	DPSDK_INT32 iEncChnNum;											// The number of encode channel 
	DPSDK_UPDATE_ENC_CHANNEL_INFO* pEncChnList;						// The list of encode channel

	// Decode unit
	DPSDK_INT32 iDecDecodeMode;										// Decode model
	DPSDK_INT32 iConbineStatus;										// TVWALL Conbine status, 1 support conbine, 0 not support conbine
	DPSDK_INT32 iDecChnNum;											// The number of decode channel
	DPSDK_UPDATE_DEC_CHANNEL_INFO* pDecChnList;						// The list of decode channel

	// Alarm input unit
	DPSDK_INT32 iAlarmInputChnNum;									// The number of alarm input channel
	DPSDK_UPDATE_CHANNEL_INFO* pAlarmInputChnList;					// The list of alarm input channel

	// Alarm output unit
	DPSDK_INT32 iAlarmOutputChnNum;									// The number of alarm output channel
	DPSDK_UPDATE_CHANNEL_INFO* pAlarmOutputChnList;					// The list of alarm output channel

	// Big screen input unit
	DPSDK_INT32 iScreenInputChnNum;									// The number of big screen input channel
	DPSDK_UPDATE_SCREEN_INPUT_CHANNEL_INFO* pScreenInputChnList;	// The list of big screen input channel

	// Big screen output unit
	DPSDK_INT32 iScreenOutputDecodeMode;							// Decode model
	DPSDK_INT32 iScreenOutputChnNum;								// The number of big screen output channel
	DPSDK_UPDATE_CHANNEL_INFO* pScreenOutputChnList;				// The list of big screen output channel

	// Acess unit
	DPSDK_INT32 iFingerPrintAuth;
	DPSDK_INT32 iCardAuth;
	DPSDK_INT32 iFaceAuth;
	DPSDK_INT32 iUserIsolate;
	DPSDK_CHAR szUnlockModes[DPSDK_UNLOCKMODES_LEN];
	DPSDK_INT32 iAccessControlChnNum;								// The number of  acess channel
	DPSDK_UPDATE_ACCESS_CONTROL_CHANNEL_INFO* pAccessControlChnList;				// The list of  acess channel

	// Audio unit
	DPSDK_INT32 iVoiceChnNum;										// The number of audio channel
	DPSDK_UPDATE_CHANNEL_INFO* pVoiceChnList;						// The list of audio channel

	// POS unit
	DPSDK_INT32 iPosChnNum;											// The number of POS channel
	DPSDK_UPDATE_CHANNEL_INFO* pPosChnList;							// The list of POS channel

	// Virtual unit
	DPSDK_INT32 iVirChnNum;											// The number of virtual channel
	DPSDK_UPDATE_CHANNEL_INFO* pVirChnList;							// The list of virtual channel

	DPSDK_INT32 iControllerNum;
	DPSDK_DEVICE_CONTROLLER_INFO* pControllerList;
}DPSDK_UPDATE_DEVICE_INFO;

// The parameter of search channel
typedef struct
{
	DPSDK_INT32 iDevicePort;										// Device port
	DPSDK_INT32 iDeviceCategory;									// Device category
	DPSDK_INT32 iManFac;											// Manufacture see DPSDK_DEVICE_MANFAC
	DPSDK_CHAR szDevIP[DPSDK_IP_LEN];								// Device IP
	DPSDK_CHAR szLoginName[DPSDK_NAME_LEN];							// Login Name
	DPSDK_CHAR szLoginPwd[DPSDK_PWD_LEN];							// Device Login password
}DPSDK_SEARCH_CHANNEL_INFO;

// Encode unit
typedef struct
{
	DPSDK_INT32 iChannelSeq;										// Channel serial number, from 0 start
	DPSDK_CHAR szChannelName[DPSDK_NAME_LEN_EX];					// Channel name
	DPSDK_INT32 iCameraType;										// Camera type see DPSDK_CAMERA_TYPE
	DPSDK_INT32 iCameraFunction;									// 0£ºhave no support function	1£ºsuppotr fish eye		2£ºsupport motorized focus 
	DPSDK_INT32 iInterfaceType;										// Interface type
}DPSDK_ENCODE_CHANNEL_INFO;

// Decode unit
typedef struct
{
	DPSDK_INT32 iChannelSeq;										// Channel serial number, from 0 start
	DPSDK_CHAR szChannelName[DPSDK_NAME_LEN_EX];					// Channel name
	DPSDK_INT32 iInterfaceType;										// Interface type
	DPSDK_INT32 iSplitMode;											// Split model£¬1,4,9,16
}DPSDK_DECODE_CHANNEL_INFO;

// Big screen input
typedef struct
{
	DPSDK_INT32 iChannelSeq;										// Channel serial number, from 0 start
	DPSDK_CHAR szChannelName[DPSDK_NAME_LEN_EX];					// Channel name
	DPSDK_INT32 iCameraType;										// Camera type see DPSDK_CAMERA_TYPE
	DPSDK_INT32 iInterfaceType;										// Interface type
	DPSDK_INT32 iChannelRemoteType;									// Remote channel type
}DPSDK_SCREEN_INPUT_INFO;

// Channel basic information 
typedef struct
{
	DPSDK_INT32 iChannelSeq;										// Channel serial number, from 0 start
	DPSDK_CHAR szChannelName[DPSDK_NAME_LEN_EX];					// Channel name
}DPSDK_CHANNEL_BASE_INFO;

// Search channel information
typedef struct
{
	DPSDK_CHAR szDeviceName[DPSDK_DEVICE_NAME_LEN];					// Device name
	DPSDK_CHAR szSerialNo[DPSDK_SN_LEN];							// Serial number
	DPSDK_INT32 iDeviceType;										// Device type
	DPSDK_INT32 iDeviceCategory;									// Device category
	DPSDK_INT32 iManFac;											// Manufacture see DPSDK_DEVICE_MANFAC

	// Encode unit
	DPSDK_INT32 iStreamType;										// Code stream type see DPSDK_STREAM_TYPE
	DPSDK_BOOL bZeroEncode;											// Is it support zero channel and multi picture
	DPSDK_INT32 iEncChnNum;											// The number of encode channel 
	DPSDK_ENCODE_CHANNEL_INFO* pEncChnList;							// The list of encode channel 

	// Decode unit
	DPSDK_INT32 iConbineStatus;										// Is it support conbine 0: not support 1:support
	DPSDK_INT32 iDecChnNum;											// The number of decode channel 
	DPSDK_DECODE_CHANNEL_INFO* pDecChnList;							// The list of decode channel 

	// Alarm input unit
	DPSDK_INT32 iAlarmInputChnNum;									// The number of alarm input channel 
	DPSDK_CHANNEL_BASE_INFO* pAlarmInputChnList;					// The list of alarm input channel 

	// Alarm output unit
	DPSDK_INT32 iAlarmOutputChnNum;									// The number of alarm output channel 
	DPSDK_CHANNEL_BASE_INFO* pAlarmOutputChnList;					// The list of alarm output channel 

	// Big screen input unit
	DPSDK_INT32 iScreenInputChnNum;									// The number of big screen input channel 
	DPSDK_SCREEN_INPUT_INFO* pScreenInputChnList;					// The list of big screen input channel 

	// Big screen output unit
	DPSDK_INT32 iScreenOutputChnNum;								// The number of big screen output channel 
	DPSDK_CHANNEL_BASE_INFO* pScreenOutputChnList;					// The list of big screen output channel 

	// Acess unit
	DPSDK_INT32 iAccessControlChnNum;								// The number of acess channel
	DPSDK_CHANNEL_BASE_INFO* pAccessControlChnList;					// The list of acess channel
}DPSDK_SEARCH_CHANNEL_LIST;

// Device initialize
typedef struct
{
	DPSDK_CHAR szDummyId[DPSDK_ID_LEN];								// The tag id of auto search
	DPSDK_IP struDeviceIP;											// Device ip
	DPSDK_CHAR szSubMask[DPSDK_IP_LEN];								// Sub-mask
	DPSDK_CHAR szGateway[DPSDK_IP_LEN];								// Default get way
	DPSDK_CHAR szPreSetEmail[DPSDK_ALARM_EMAILRECEIVER_LEN];		// Pre-set mail
	DPSDK_CHAR szPreSetPhone[DPSDK_PHONE_LEN];						// Pre-set phone number
	DPSDK_CHAR szLoginName[DPSDK_NAME_LEN];							// Login name
	DPSDK_CHAR szLoginPWD[DPSDK_PWD_LEN];							// Device password(encrypt)
}DPSDK_INIT_DEVICE_INFO;

// Modify device IP
typedef struct
{
	DPSDK_CHAR szDummyId[DPSDK_ID_LEN];								// The tag id of auto search
	DPSDK_CHAR szDeviceIP[DPSDK_IP_LEN];							// New device ip
	DPSDK_CHAR szSubMask[DPSDK_IP_LEN];								// Sub-mask
	DPSDK_CHAR szGateway[DPSDK_IP_LEN];								// Default get way
	DPSDK_CHAR szLoginName[DPSDK_NAME_LEN];							// Login name
	DPSDK_CHAR szLoginPWD[DPSDK_PWD_LEN];							// Device password(encrypt)
}DPSDK_MODIFY_DEVICE_IP_INFO;

// Export device
typedef struct
{
	DPSDK_CHAR szDeviceName[DPSDK_DEVICE_NAME_LEN];					// Device name
	DPSDK_IP struDeviceIp;											// Device ip
	DPSDK_INT32 iDevicePort;										// Device port
	DPSDK_INT32 iManFac;											// Manufacture
	DPSDK_CHAR szOrgCode[DPSDK_ORG_CODE_LEN];						// Organization code
	DPSDK_CHAR szLoginName[DPSDK_NAME_LEN];							// Login name
	DPSDK_CHAR szLoginPWD[DPSDK_PWD_LEN];							// Device password(encrypt)
	DPSDK_CHAR szDeviceSn[DPSDK_SN_LEN];							// Device serial number
	DPSDK_INT32 iAddType;											// Add type
}DPSDK_EXPORT_DEVICE_INFO;

// The list of export device
#define DPSDK_MAX_EXPORT_DEVICE_NUM		500							// Max number of export device 

typedef struct
{
	DPSDK_INT32	iExportDevNum;												// The number of export device
	DPSDK_EXPORT_DEVICE_INFO szExportDevList[DPSDK_MAX_EXPORT_DEVICE_NUM];	// The list of export device
}DPSDK_EXPORT_DEVICE_LIST;


// Get the reason of device off-line request parameters 
typedef struct
{
	DPSDK_INT32 iDeviceNum;											// The number of device
	DPSDK_DEVICE_ID* pDeviceList;									// The list of device
}DPSDK_GET_DEVICE_OFFLINE_REASON_PARAM;

// The result of query device off-line
typedef struct
{
	DPSDK_CHAR szDeviceId[DPSDK_DEVICE_ID_LEN];						// Device ID
	DPSDK_CHAR szReason[DPSDK_MEMO_LEN];							// Off-line reason
}DPSDK_GET_DEVICE_OFFLINE_REASON_RESULT;

// The result of query device off-line list
typedef struct
{
	DPSDK_UINT32 uiTotal;											// The number of result
	DPSDK_GET_DEVICE_OFFLINE_REASON_RESULT struResultList[1];		// The list of result
}DPSDK_GET_DEVICE_OFFLINE_REASON_RESULT_LIST;

// The information of delete device fail
typedef struct
{
	DPSDK_INT32 iAlarmPlansNum;										// The number of alarm plan
	DPSDK_INT32 iStoPlansNum;										// The number of storage plan
	DPSDK_INT32 iVideoWallsNum;										// The number of TVWALL plan
	DPSDK_INT32 iVideoRelationsNum;                                 // The number of VideoRelation
	DPSDK_INT32 iDoorInUseNum;                                      // The number of Door In Use
	DPSDK_INT32 iAttendanceLocationNum;
	DPSDK_CHANNEL_ID* pAttendanceLocations;
	DPSDK_CHANNEL_ID* pAlarmPlans;									// Alarm plan
	DPSDK_CHANNEL_ID* pStoPlans;									// Storage plan
	DPSDK_CHANNEL_ID* pVideoWalls;									// TVWALL
	DPSDK_CHANNEL_ID* pVideoRelations;							    // Video Relation
	DPSDK_CHANNEL_ID* pDoorInUse;                                   // Door In Use
	DPSDK_INT32 iDoorGroupNamesNum;                                 // The number of Door Group Name
	DPSDK_INT32 iPersonIdsNum;										// The number of Person Id
	DPSDK_INT32 iFirstCardChnIdsNum;								// The number of FirstCard ChnId
	DPSDK_INT32 iMultiCardChnIdNum;									// The number of MultiCard ChnId
	DPSDK_INT32 iRemoteVerificationChnIdsNum;						// The number of Remote Verification ChnId
	DPSDK_INT32 iAntiPassBackNamesNum;								// The number of Anti PassBack Name
	DPSDK_INT32 iInterDoorLockNamesNum;								// The number of Inter DoorLock Name
	DPSDK_CHANNEL_ID* pDoorGroupNames;                              // Door Group Name
	DPSDK_CHANNEL_ID* pPersonIds;									// Person Id
	DPSDK_CHANNEL_ID* pFirstCardChnIds;								// FirstCard ChnId
	DPSDK_CHANNEL_ID* pMultiCardChnIds;								// MultiCard ChnId
	DPSDK_CHANNEL_ID* pRemoteVerificationChnIds;					// Remote Verification ChnId
	DPSDK_CHANNEL_ID* pAntiPassBackNames;							// AntiPassBack Name
	DPSDK_CHANNEL_ID* pInterDoorLockNames;							// Inter DoorLock Name
	DPSDK_CHAR szDeviceCode[DPSDK_DEVICE_CODE_LEN];					// Device code
	//DPSDK_BOOL bIsLinkVideo;										// Is there linkage video, true yes, false no
	DPSDK_INT32 iKeyCodeNum;										// Key code number
	DPSDK_ID* pKeyCodes;											// Key code list
}DPSDK_DELETE_DEVICE_FAIL_INFO;


// The list of device 
typedef struct
{
	DPSDK_INT32	iDevNum;											// The number of device 
	DPSDK_DEVICE_ID struDevIdList[1];								// The list of device id
}DPSDK_DEVICE_LIST;

// The list of delete device fail
typedef struct
{
	DPSDK_INT32	iDelDevFailNum;										// The number of delete device fail
	DPSDK_DELETE_DEVICE_FAIL_INFO* pDelDevFailList;					// The list of delete device fail
}DPSDK_DELETE_DEVICE_FAIL_LIST;

typedef struct
{
	DPSDK_CHAR szId[DPSDK_CHANNEL_ID_LEN];
	DPSDK_CHAR szDeviceIp[DPSDK_IP_LEN];
	DPSDK_CHAR szDeviceName[DPSDK_DEVICE_NAME_LEN];
	DPSDK_INT32 iDeviceType;
}DPSDK_UNAUTH_DEVICE;

typedef struct
{
	DPSDK_INT32	iNum;
	DPSDK_UNAUTH_DEVICE* pUnAuthDevice;					
}DPSDK_UNAUTH_DEVICE_LIST;

typedef struct
{
	DPSDK_CHAR szId[DPSDK_CHANNEL_ID_LEN];
	DPSDK_INT32 iErrorCode;
}DPSDK_FAILED_UNAUTH_DEVICE;

typedef struct
{
	DPSDK_INT32 iDeviceCodeNum;
	DPSDK_DEVICE_ID* pDeviceCodeList;
	DPSDK_INT32	iFailedDevicesNum;
	DPSDK_FAILED_UNAUTH_DEVICE* pFailedDevices;					
}DPSDK_SAVE_UNAUTH_DEVICE_LIST;

// Searcher device channel information 
typedef struct
{
	DPSDK_CHAR szDeviceCode[DPSDK_DEVICE_CODE_LEN];					// Device code
	DPSDK_INT32 iStatus;											// 1-Finish search,0- unfinished search
}DPSDK_SOSO_CHANNEL_INFO;

// Get list of searcher device channel information
typedef struct
{
	DPSDK_INT32 iDeviceNum;											// The number of device
	DPSDK_SOSO_CHANNEL_INFO struSosoChannelList[1];					// The list of searcher device channel information
}DPSDK_SOSO_CHANNEL_LIST;

// Organization code
typedef struct
{
	DPSDK_CHAR szName[DPSDK_ORG_NAME_LEN];							// Organization name
	DPSDK_INT32 iType;												// Organization type	
	DPSDK_CHAR szParentCode[DPSDK_ORG_CODE_LEN];					// Parent organization ID
	DPSDK_CHAR szPlatformCode[DPSDK_ID_LEN];						// Platform ID
}DPSDK_ORG_EDIT_PARAM;

// Organization code
typedef struct
{
	DPSDK_CHAR szSrcNodeName[DPSDK_ORG_NAME_LEN];					// Source organization name
	DPSDK_CHAR szSrcNodeCode[DPSDK_ORG_CODE_LEN];					// Source organization ID
	DPSDK_CHAR szDestNodeName[DPSDK_ORG_NAME_LEN];					// Destination organization name 
	DPSDK_CHAR szDestNodeCode[DPSDK_ORG_CODE_LEN];					// Destination organization ID
	DPSDK_INT32 iPosition;											// 0 move into organization; 1 in front of organization; -1 behind organization
}DPSDK_ORG_MOVE_PARAM;

// ITC control
typedef struct
{
	DPSDK_CHAR szChannelCode[DPSDK_CHANNEL_ID_LEN];					// Channel code
	DPSDK_INT32 iOperateType;										// "0:up 1:down"
}DPSDK_ITC_PARAM;

// Module code
typedef enum
{
	//MODULE_BASE = 1,												// Basic module
	MODULE_POS = 32,												// POS
	MODULE_BAY = 37,												// Bay
	MODULE_RADAR = 42,												// Radar
	MODULE_ASC = 43,												// ASC
	MODULE_VTO = 44,												// VTO
	MODULE_BUSSINESS_INTELLIGENCE = 45,								// bussiness Intelligence
	MODULE_CCTV = 50,
	MODULE_EG_IP = 51,
	MODULE_EG_CHANNEL = 52,
	MODULE_INTERCOM_VTO = 53,
	MODULE_SIP_ID = 54,
	MODULE_ALARMHOST_IP = 55,
	MODULE_ALARMHOST_DEFENSE = 56,
	MODULE_PASSAGEWAY = 57,
	MODULE_FACERECOGNITION_CHANNEL = 58,
	MODULE_CLIENT_NUM = 59,
	MODULE_APP_NUM = 60,
}DPSDK_MODULE_CODE;

// License information
typedef struct
{
	DPSDK_INT32 iCode;												// Module code see DPSDK_MODULE_CODE
	DPSDK_BOOL bStatus;												// Is it enable
	DPSDK_INT32 iCount;												// Number, -1:unrestricted
	DPSDK_INT32 iUsedCount;											// The Number has been used
	DPSDK_TIMET tTime;                                              // Time
}DPSDK_LICENSE_INFO;

// The list of license
typedef struct
{
	DPSDK_INT32 iVersion;
	DPSDK_TIMET tActivationDate;                                    // Time
	DPSDK_INT32 iLicenseNum;										// The number of license
	DPSDK_LICENSE_INFO struLicenseInfo[1];							// The list of license
}DPSDK_LICENSE_LIST;

// The position of store video
typedef enum
{
	STORAGE_DISABLED = 0,											// Store disabled
	STORAGE_SEARVER	= 1,											// Store in server
	STORAGE_SEARVER_ADDITIONAL = 2,									// Store in server by additional record
	STORAGE_DEVICE = 3,												// Store in device
}DPSDK_STORAGE_POSITION;

#define DPSDK_MAX_STORAGE_PLAN_NODE_NUM		32						// Max channel number of video plan
#define DPSDK_MAX_STORAGE_PLAN_NUM			100						// Max video plan number of single query
// Video plan
typedef struct
{
	DPSDK_INT32 iId;												// Record id
	DPSDK_CHAR szChannelId[DPSDK_CHANNEL_ID_LEN];					// Channel ID
	DPSDK_INT32 iPlanType;											// Storage position see DPSDK_STORAGE_POSITION
	DPSDK_INT32 iRecordType;										// Code stream type DPSDK_STREAM_TYPE
	DPSDK_BOOL bStat;												// Enable status
	DPSDK_CHAR szMemo[DPSDK_USER_REMARK_LEN];						// Memo
	DPSDK_INT32 iTimeTempId;										// The id of time template
	DPSDK_CHAR szTemplateName[DPSDK_NAME_LEN];						// The name of time template(Not need when save)
}DPSDK_STORAGE_PLAN_INFO;

// The list of video plan
typedef struct
{
	DPSDK_INT32 iPlanNum;											// The number of video plan
	DPSDK_STORAGE_PLAN_INFO szPlanList[DPSDK_MAX_STORAGE_PLAN_NUM];	// The list of video plan
}DPSDK_STORAGE_PLAN_LIST;

// The parameter of copy video plan
typedef struct
{
	DPSDK_INT32 iId;												// Record id
	DPSDK_INT32 iDestChannelNum;									// The number of destination channel id
	DPSDK_CHANNEL_ID* pDestChannelList;								// The list of destination channel id
}DPSDK_STORAGE_PLAN_COPY_PARAM;

// Dist type
typedef enum
{
	DISK_TYPE_UNKNOWN = 0,											// Unknown
	DISK_TYPE_VIDEO = 1,											// Video
	DISK_TYPE_PIC = 2,												// Picture
	DISK_TYPE_UNSET = 99,											// Not setup
}DPSDK_DISK_TYPE;

// The status of file system
typedef enum
{
	FILESYSTEM_STATUS_OK = 1,										// Normal
	FILESYSTEM_STATUS_OTHER = 2,									// Abnormal
}DPSDK_FILESYSTEM_STATUS;

// Disk information
typedef struct
{
	DPSDK_CHAR szServerName[DPSDK_NAME_LEN];						// Server name
	DPSDK_CHAR szServerIP[DPSDK_IP_LEN];							// Server IP address
	DPSDK_INT32 iServerPort;										// Server port
	DPSDK_CHAR szIP[DPSDK_IP_LEN];									// IP address
	DPSDK_INT32 iDiskType;											// Disk type see DPSDK_DISK_TYPE
	DPSDK_CHAR szPath[DPSDK_FILE_PATH_LEN];							// Disk path
	DPSDK_INT32 iFileSystemStatus;									// The status of file system see DPSDK_FILESYSTEM_STATUS, OK: Normal, Other: Abnormal
	DPSDK_CHAR szDiskName[DPSDK_NAME_LEN];							// Disk name
	DPSDK_UINT64 uiCapacity;										// Capacity
	DPSDK_UINT64 uiFreeSpace;										// Free space
	DPSDK_BOOL bFormat;												// Is it format
	DPSDK_BOOL bBelong;												// Is it belong to
	DPSDK_INT32 iIsSupportFormat;									// 0:not support 1:support
}DPSDK_STORAGE_DISK_INFO;

#define DPSDK_MAX_STORAGE_DISK_NUM			100						// Max number of disk single query
// Disk information
typedef struct
{
	DPSDK_INT32 iDiskNum;											// The number of disk
	DPSDK_STORAGE_DISK_INFO szDiskList[DPSDK_MAX_STORAGE_DISK_NUM];	// The list of disk
	DPSDK_INT32 iNextPage;											// The number of next page, -1 represent no next page
}DPSDK_STORAGE_DISK_LIST;

// The parameter of disk format
typedef struct
{
	DPSDK_INT32 iDiskType;											// Disk type see DPSDK_DISK_TYPE
	DPSDK_CHAR szPaths[DPSDK_FILE_PATH_LEN * 100];					// List of disk path, using "," to slit when has multi path
	DPSDK_CHAR szServerIP[DPSDK_IP_LEN];							// Server ip
	DPSDK_INT32 iServerPort;										// Server port
}DPSDK_STORAGE_DISK_FORMAT_PARAM;

// The parameter of delete disk
typedef struct
{
	DPSDK_CHAR szServerIP[DPSDK_IP_LEN];							// Server ip
	DPSDK_INT32 iServerPort;										// Server port
	DPSDK_CHAR szIP[DPSDK_IP_LEN];									// Network disk ip
}DPSDK_STORAGE_DISK_DELETE_PARAM;

// The parameter of disk rob
typedef struct
{
	DPSDK_CHAR szPath[DPSDK_FILE_PATH_LEN];							// Disk path
	DPSDK_CHAR szServerIP[DPSDK_IP_LEN];							// Server ip
	DPSDK_INT32 iServerPort;										// Server port
	DPSDK_INT32 iDiskType;											// Disk type see DPSDK_DISK_TYPE
}DPSDK_STORAGE_DISK_ROB_PARAM;

// The parameter of change disk type
typedef struct
{
	DPSDK_INT32 iDiskType;											// Disk type see DPSDK_DISK_TYPE
	DPSDK_CHAR szPath[DPSDK_FILE_PATH_LEN];							// Disk path
	DPSDK_CHAR szServerIP[DPSDK_IP_LEN];							// Server IP
	DPSDK_INT32 iServerPort;										// Server port
}DPSDK_STORAGE_DISK_CHANGE_PARAM;

// Virtual disk information
typedef struct
{
	DPSDK_INT32 iVirtualDiskType;									// Virtual disk type see DPSDK_DISK_TYPE
	DPSDK_CHAR szVirtualPath[DPSDK_FILE_PATH_LEN];					// Virtual disk path
	DPSDK_UINT64 uiCapacity;										// Capacity
}DPSDK_STORAGE_VIRTUALDISK_PARAM;

// The parameter of delete virtual disk
typedef struct
{
	DPSDK_INT32 iVirtualDiskType;									// Virtual disk type DPSDK_DISK_TYPE
	DPSDK_CHAR szVirtualPath[DPSDK_FILE_PATH_LEN];					// Virtual disk path
}DPSDK_STORAGE_VIRTUALDISK_DELETE_PARAM;

// Local storage disk type
typedef enum
{
	DISK_TYPE_FAT32 = 1,											// FAT32
	DISK_TYPE_NTFS = 2,												// NTFS
}DPSDK_LOGICALDISK_TYPE;

// Local storage information
typedef struct
{
	DPSDK_UINT64 uiFreeSpace;										// Free space
	DPSDK_UINT64 uiCapacity;										// Capacity
	DPSDK_INT32 iDiskType;											// Disk type see DPSDK_LOGICALDISK_TYPE
	DPSDK_CHAR szPath[DPSDK_FILE_PATH_LEN];							// The logo of local disk
	DPSDK_CHAR szDiskName[DPSDK_NAME_LEN];							// The name of local disk
	DPSDK_CHAR szServerIP[DPSDK_IP_LEN];							// Server IP address
	DPSDK_INT32 iServerPort;										// Server port
	DPSDK_BOOL bHealthStatus;										// Is it get
	DPSDK_INT32 iDataType;                                          // data Type, 1:video, 2: pic
	DPSDK_INT32 iIsSupportFormat;                                   // 0: not Support, 1: Support
	DPSDK_INT32 iIsSupportSetType;                                  // 0: not Support, 1: Support
	DPSDK_INT32 iIsSupportDelimit;                                  // 0: not Support, 1: Support
	DPSDK_INT32 iVirtualDiskNum;									// The number of virtual disk
	DPSDK_STORAGE_VIRTUALDISK_PARAM* pVirturalDisk;					// The information of virtual disk
}DPSDK_STORAGE_LOGICALPARTITION_INFO;

// The list of local storage information
typedef struct
{
	DPSDK_INT32 iLogicalPartitionNum;								// The disk number of local storage
	DPSDK_STORAGE_LOGICALPARTITION_INFO* pLogicalPartitionList;		// The disk list of local storage
}DPSDK_STORAGE_LOGICALPARTITION_LIST;


// Service information 
typedef struct
{
	DPSDK_CHAR szServerCode[DPSDK_SERVER_CODE_LEN];					// Server code
	DPSDK_CHAR szServerName[DPSDK_NAME_LEN];					    // Server name
}DPSDK_DISK_SERVER_INFO;

// Service information
typedef struct
{
	DPSDK_INT32 iServerNum;											// The number of server
	DPSDK_DISK_SERVER_INFO struDiskServer[1];						// The list of server
}DPSDK_DISK_SERVER_LIST;

typedef struct  
{
	DPSDK_INT32 iKeyLen;
	DPSDK_CHAR szKeyValue[DPSDK_ID_LEN];						// Key
	DPSDK_CHAR szAlgorithm[DPSDK_ID_LEN];						// Algorithm
	DPSDK_TIMET tCreateData;									// Create data
}DPSDK_MEDIA_KEY_INFO; 

typedef struct  
{
	DPSDK_CHAR szDeviceCode[DPSDK_DEVICE_ID_LEN];
	DPSDK_TIMET tStartTime;
	DPSDK_TIMET tEndTime;
}DPSDK_QUERY_MEDIA_KEY_PARAM;

typedef struct  
{
	DPSDK_INT32 iKeyNum;
	DPSDK_MEDIA_KEY_INFO* pKeyList;
}DPSDK_MEDIA_KEY_LIST;

typedef struct  
{
	DPSDK_CHAR szDeviceCode[DPSDK_DEVICE_ID_LEN];
	DPSDK_MEDIA_KEY_INFO struKey;
}DPSDK_MEDIA_KEY_NOTIFY; 

// The basic information of TVWALL 
typedef struct
{
	DPSDK_INT32 iTVWallId;											// TVWALL ID
	DPSDK_CHAR szTVWallName[DPSDK_NAME_LEN];						// TVWALL name
	DPSDK_CHAR szOrgCode[DPSDK_ORG_CODE_LEN];						// Organization code
	DPSDK_INT32 iState;												// Enable status£º0 unable,1 enable
	DPSDK_UINT32 uiVersion;											// TVWALL version

	DPSDK_UINT32 uiSustainTime;										// Duration(s)
	DPSDK_CHAR szTVWallDesc[DPSDK_MEMO_LEN];						// Describe
}DPSDK_TVWALL_BASE_INFO;

// The list of TVWALL
typedef struct
{
	DPSDK_UINT32 uiTotal;											// The number of TVWALL information
	DPSDK_TVWALL_BASE_INFO struTVWallBaseInfo[1];					// The list of TVWALL information
}DPSDK_TVWALL_LIST;

// Screen position of TV wall
typedef struct
{
	DPSDK_FLOAT fLeft;											// The distance to the left of the screen, the percentage
	DPSDK_FLOAT fTop;											// The margin on the screen, the percentage
	DPSDK_FLOAT fWidth;											// Screen width, percentage
	DPSDK_FLOAT fHeight;										// Screen height, percentage
}DPSDK_TVWALL_SCREEN_POS;

//Common screen info under the splicing screen
typedef struct
{
	DPSDK_INT32	iId;												// Screen ID
	DPSDK_INT32	iChannelNo;											// Channel sequence number
	DPSDK_TVWALL_SCREEN_POS struScreenPos;							// Screen position
	DPSDK_INT32	iScreenSeq;											// Screen serial number, logical screen valid
}DPSDK_COMBINED_SCREEN_INFO;

#define DPSDK_MAX_COMBINED_SCREEN_NUM		128
#define DPSDK_MAX_SPILIT_CAPACITY_NUM		64
#define DPSDK_MAX_SCREEN_DECODER_NUM	    128
//A list of decoded channels for screen binding
typedef struct
{
	DPSDK_CHAR szDeviceId[DPSDK_DEVICE_ID_LEN];						// Device for decoding channel ID
	DPSDK_CHAR szChannelId[DPSDK_CHANNEL_ID_LEN];					// Decoding channel ID
	DPSDK_INT32	iChannelNo;											// Channel number, corresponding XMLIn (SEQChannel number-1To show whether the screen is fused)
	DPSDK_INT32	iUnit;												// Subordinate unit
	//DPSDK_CHAR szDecoderName[DPSDK_DEVICE_NAME_LEN];				// Decoder name
	DPSDK_INT32	iDecoderType;										// Decoder type, reference DPSDK_TVWALL_DEVICE_TYPE
	//DPSDK_BOOL	bStatus;										// Is it on-line, true: on-line; false: off-line
	//DPSDK_BOOL	bDecoderRight;									// Is the channel has decode right
	DPSDK_BOOL	bIsChildrenDecoder;									// Splice+The decoder wall scheme is used.True:The video output channel bound by this screen is a splice+Decoder The

	DPSDK_INT32	iScreenId;											// Screen ID£¨WebEnd configuration ID)
	DPSDK_CHAR szScreenName[DPSDK_NAME_LEN];						// Screen name
	DPSDK_TVWALL_SCREEN_POS struScreenPos;							// Screen position

	DPSDK_BOOL bIsCombinedScreen;									// Is it a splicing screen?XMLIn Type is 1 or 3 as it is a combination screen)
	DPSDK_BOOL bIsScreenAlarmWall;									// Is it an alarm linkage screen
	//DPSDK_INT32	iSplitCapacity;									// The max capacity of split window

	//DPSDK_INT32 iSplitCapacityNum;								// The number of split capacity function
	//DPSDK_INT32 szSplitCapacity[DPSDK_MAX_SPILIT_CAPACITY_NUM];	// Split capacity
	//DPSDK_INT32	iOpenWndCapacity;								// Max split capacity 
	//DPSDK_INT32	iBigScreenOut;									// The output port code of big screen control

	DPSDK_INT32 iCombinedScreenNum;									// The number of ordinary screens contained in the splicing screen
	DPSDK_COMBINED_SCREEN_INFO szCombinedScreen[DPSDK_MAX_COMBINED_SCREEN_NUM];					// Common screen info contained in the splicing screen

}DPSDK_SCREEN_DECODER_INFO;

// The list of decode channel binding to screen
typedef struct
{
	DPSDK_INT32 iScreenDecoderNum;									// Number of decoding channel
	DPSDK_SCREEN_DECODER_INFO struScreenDecoder[DPSDK_MAX_SCREEN_DECODER_NUM];		// List of decoding channel
}DPSDK_SCREEN_DECODER_LIST;


// TV Wall Info
typedef struct
{
	DPSDK_TVWALL_BASE_INFO struBaseInfo;							// TV wall info
	DPSDK_SCREEN_DECODER_LIST struScreenDecoderList;				// List of decoding channel bonded with screen
}DPSDK_TVWALL_INFO;

// TV Wall Task Info
typedef struct
{
	DPSDK_INT32 iTVWallId;											// TV Wall ID
	DPSDK_INT32 iTaskId;											// Task ID
	DPSDK_CHAR szTaskName[DPSDK_NAME_LEN];							// Task name
	DPSDK_CHAR szTaskDesc[DPSDK_MEMO_LEN];							// Task description
}DPSDK_TVWALL_TASK_BASE_INFO;

// TV Wall Task List
typedef struct
{
	DPSDK_UINT32 uiTotal;											// Number of TV wall task
	DPSDK_TVWALL_TASK_BASE_INFO struTVWallTask[1];					// List of TV wall task
}DPSDK_TVWALL_TASK_LIST;

// Video Source Info
typedef struct
{
	DPSDK_CHAR szChannelCode[DPSDK_CHANNEL_ID_LEN];					// Channel no.
	DPSDK_CHAR szChannelName[DPSDK_CHANNEL_NAME_LEN];				// Channel name
	DPSDK_CHAR szDeviceCode[DPSDK_DEVICE_ID_LEN];					// Device ID
	DPSDK_INT32	iChnlNo;											// Channel S./N.
	DPSDK_INT32	iSubStream;											// For stream type, please refer to DPSDK_STREAM_TYPE definition.
	DPSDK_INT32	iTimeSpan;											// Dwell time
	DPSDK_INT32	iPrePoint;											// Preset point S./N.
	DPSDK_INT32	iProvider;											// Manuafacturer
	DPSDK_CHAR szOsdText[DPSDK_MEMO_LEN];							// OSD info
	DPSDK_BOOL bEnableOsd;											// OSD enable
	DPSDK_INT32 iPatrolMode;										// Tour mode: tour=0; non-tour onwall =1; tour preview=2;
	DPSDK_INT32 iFishFitMode;										// Fisheye mounting mode: unknown=-1;wall=0; ceiling=1;floor =2£»
	DPSDK_INT32 iFishShowMode;										// Fisheye display mode: unknown=-1; panoramic=0; dual panoramic=1; single screen PTZ=2; 4-split PTZ=3; output +3 modes=4; original mode=5
	DPSDK_CHAR szDepartmentCode[DPSDK_ORG_CODE_LEN];				// Channel department ID

	// It is used to save task.
	DPSDK_INT32 iTrackId;											// 501PS packet,601 original frame,701 Dahua frame, 801 Dahua standard frame, 901 TS packet, 101 (below 300) RTP standard strema
	DPSDK_INT32 iConnType;											// 0: TCP; 1: UDP; 2: multicast; 3: domain name; 4: auto register; 5: ONVIF; 6: GB28181; 7: HTTP webpage

}DPSDK_TVWALL_VIDEO_SOURCE_INFO;

//// Video source list
//typedef struct
//{
//	DPSDK_UINT32 uiTotal;											// List length
//	DPSDK_TVWALL_VIDEO_SOURCE_INFO* pVideoSourceList;				// Video source list
//}DPSDK_TVWALL_VIDEO_SOURCE_LIST;

//// Video source of TV wall task sub-window
//typedef struct
//{
//	DPSDK_INT32 iSubWndID;											// Sub-window ID
//	DPSDK_UINT32 uiTotal;											// Video source list length
//	DPSDK_TVWALL_VIDEO_SOURCE_LIST* pVideoSourceList;				// Video source list
//}DPSDK_TVWALL_TASK_SUBWND_VIDEO_SOURCE;
//
//// Video source of TV wall task window
//typedef struct
//{
//	DPSDK_INT32 iWndID;												// Window ID
//
//	DPSDK_UINT32 uiTotal;											// Video source list length
//	DPSDK_TVWALL_VIDEO_SOURCE_LIST* pVideoSourceList;				// Video source list
//
//	DPSDK_UINT32 uiSubWndTotal;										// Sub-window video source list length
//	DPSDK_TVWALL_TASK_SUBWND_VIDEO_SOURCE* pSubWndVideoSourceList;	// Sub-window video source list
//}DPSDK_TVWALL_TASK_WND_VIDEO_SOURCE;
//
//// Video source of TV wall task single-screen
//typedef struct
//{
//	DPSDK_INT32 iScreenID;											// Screen ID
//	DPSDK_UINT32 uiTotal;											// List length
//	DPSDK_TVWALL_TASK_WND_VIDEO_SOURCE* pWndVideoSourceList;		// Video source list of TV task window
//}DPSDK_TVWALL_TASK_SCREEN_VIDEO_SOURCE;
//
//// Video source list of TV task
//typedef struct
//{
//	DPSDK_UINT32 uiTotal;											// List length
//	DPSDK_TVWALL_TASK_SCREEN_VIDEO_SOURCE* pScreenVideoSourceList;	// Video source list of TV wall task single-screen
//}DPSDK_TVWALL_TASK_VIDEO_SOURCE_LIST;

// TV wall sub-window info
typedef struct
{
	DPSDK_INT32 iWndID;												// Window ID
	DPSDK_FLOAT fLeft;												// Window position left
	DPSDK_FLOAT fTop;												// Top
	DPSDK_FLOAT fWidth;												// Width
	DPSDK_FLOAT fHeight;											// Height
	DPSDK_INT32 iZorder;											// z order (omit it in case of split)
	DPSDK_CHAR szName[DPSDK_NAME_LEN];								// Window title (split into "") 
	DPSDK_INT32 isAlarm;											// Alarm on wall=1; client on wall=0
	DPSDK_INT32 isHighLight;										// Highlight=1; non-highlight=0; no processing =-1
	DPSDK_INT32 isOpenAudio;										// Audio enabled=1; audio disabled; no processing=-1

	DPSDK_UINT32 uiVideoSourceNum;									// Video source list length
	DPSDK_TVWALL_VIDEO_SOURCE_INFO* pVideoSourceList;				// Video source list 
}DPSDK_TVWALL_SUBWND_INFO;

// TV wall window info
typedef struct
{
	DPSDK_INT32 iWndID;												// Window ID
	DPSDK_FLOAT fLeft;												// Window position left
	DPSDK_FLOAT fTop;												// Top
	DPSDK_FLOAT fWidth;												// Width
	DPSDK_FLOAT fHeight;											// Height
	DPSDK_INT32 iZorder;											// z order (omit it in case of split)
	DPSDK_CHAR szName[DPSDK_NAME_LEN];								// Window title (split into "")
	DPSDK_INT32 isAlarm;											// Alarm on wall=1; client on wall=0
	DPSDK_INT32 isHighLight;										// Highlight=1; non-highlight=0; no processing =-1
	DPSDK_INT32 isOpenAudio;										// Audio enabled=1; audio disabled; no processing=-1

	DPSDK_UINT32 uiVideoSourceNum;									// Video source list length
	DPSDK_TVWALL_VIDEO_SOURCE_INFO* pVideoSourceList;				// Video source list

	DPSDK_INT32 iScreenMode;										// Split=1; windowing=2
	DPSDK_INT32 iSubWinNum;											// Number of sub-window
	DPSDK_TVWALL_SUBWND_INFO* pSubWndList;							// Window operation info
}DPSDK_TVWALL_WND_INFO;

// TV wall task screen operation info
typedef struct
{
	DPSDK_INT32 iScreenID;											// Screen ID
	DPSDK_INT32 iSplitNum;											// Screen split/windowing number
	DPSDK_INT32 iScreenMode;										// Split=1; windowing=2
	DPSDK_UINT32 uiTotal;											// Window list number
	DPSDK_TVWALL_WND_INFO* pWndList;								// Window list

	// It will be used to save task
	DPSDK_CHAR szDecodeId[DPSDK_DEVICE_ID_LEN];						// Decoding device ID
	DPSDK_INT32 iTvIdx;												// Channel no.. -1 represents splicing window
	DPSDK_INT32 iVisitorMode;										// Direct connection (1), pull stream (2), push stream to decoder (3), please refer to DPSDK_DECODER_MODE definition
	DPSDK_FLOAT fLeft;												// Screen position, left
	DPSDK_FLOAT fTop;												// Screen position, top
	DPSDK_FLOAT fWidth;												// Screen position, width
	DPSDK_FLOAT fHeight;											// Screen position, height
	DPSDK_CHAR szScreenName[DPSDK_NAME_LEN];						// Screen name
}DPSDK_TVWALL_TASK_SCREEN_OPER_INFO;

// TV wall task screen operation info list
typedef struct
{
	DPSDK_UINT32 uiTotal;											// TV wall task screen operation info number
	DPSDK_TVWALL_TASK_SCREEN_OPER_INFO* pScreenOperList;			// TV wall task screen operation info list
}DPSDK_TVWALL_TASK_SCREEN_OPER_LIST;

// Device info required by video source when decoder is in the decoding mode of direct connection
typedef struct
{
	DPSDK_CHAR szID[DPSDK_CHANNEL_ID_LEN];							// Channel ID
	DPSDK_CHAR szIP[DPSDK_IP_LEN];									// Device IP (unicast) or multicast IP (multicast)
	DPSDK_INT32 iPort;												// Port(unicast) or multicast port(multicast)
	DPSDK_CHAR szUserName[DPSDK_NAME_LEN];							// Username
	DPSDK_CHAR szPassword[DPSDK_PWD_LEN];							// Password
	DPSDK_INT32 iNo;												// Channel no.
	DPSDK_INT32 iChannelNum;										// Total number of channel
	DPSDK_INT32 iType;												// Device type
	DPSDK_CHAR szChannelName[DPSDK_CHANNEL_NAME_LEN];				// Channel name
}DPSDK_TVWALL_TASK_CHANNEL_EXT_INFO;

//Device info list required by video source when decoder is in the decoding mode of direct connection
typedef struct
{
	DPSDK_UINT32 uiTotal;											// Device info number required by video source when decoder is in the decoding mode of direct connection
	DPSDK_TVWALL_TASK_CHANNEL_EXT_INFO* pChannelExtList;			// Device info list required by video source when decoder is in the decoding mode of direct connection
}DPSDK_TVWALL_TASK_CHANNEL_EXT_LIST;


typedef enum
{
	TVWALL_PROJ_TIMING			= 0,								//Timing plan
	TVWALL_PROJ_RELATIVE_TIME	= 1,								//Tour plan
}DPSDK_TVWALL_PROJECT_TYPE;

typedef struct
{
	DPSDK_INT32				iTaskId;								// Task ID
	DPSDK_TIMET				tBeginTime;								// Start time
	DPSDK_TIMET				tEndTime;								// End time
	DPSDK_INT32				iSpan;									// Dwell time
}DPSDK_TVWALL_PROJECT_TASK_INFO;

typedef struct
{
	DPSDK_CHAR szProjName[DPSDK_NAME_LEN];					//Plan name
	DPSDK_INT32 iType;										//Plan type
	DPSDK_INT32 iTaskNum;									//Task number
	DPSDK_TVWALL_PROJECT_TASK_INFO* pTaskInfoList;			//Task info
}DPSDK_TVWALL_PROJECT_INFO;

typedef struct
{
	DPSDK_UINT32 uiCount;									// Plan number
	DPSDK_TVWALL_PROJECT_INFO* pProjList;					// Plan list
}DPSDK_TVWALL_PROJECT_LIST;

// TV wall task info
typedef struct
{
	DPSDK_TVWALL_TASK_BASE_INFO struBaseInfo;						// TV wall task info
	//DPSDK_TVWALL_TASK_VIDEO_SOURCE_LIST struVideoSourceList;		// TV wall task video source list
	DPSDK_TVWALL_TASK_SCREEN_OPER_LIST struScreenOperList;			// TV wall task screen operation info list
	DPSDK_TVWALL_TASK_CHANNEL_EXT_LIST struChannelExtList;			// Device info list required by video source when decoder is in the decoding mode of direct connection
}DPSDK_TVWALL_TASK_INFO;

typedef struct
{
	DPSDK_INT32 iTime;
}DPSDK_TVWALL_PRINT_DECODER_PARAM;

// TV wall task info
typedef struct
{
	DPSDK_INT32 iTVWallTaskNum;										// TV wall task number
	DPSDK_TVWALL_TASK_INFO* pTVWallTaskList;						// TV wall task list
	DPSDK_TVWALL_PROJECT_LIST struProjectList;						// TV wall plan list
}DPSDK_TVWALL_TASK_INFO_LIST;

// On-going TV wall task info
typedef struct
{
	DPSDK_INT32 iTVWallId;											// TV wall ID
	DPSDK_INT32 iTaskId;											// Task ID
	DPSDK_CHAR szTaskName[DPSDK_NAME_LEN];							// Task name
	DPSDK_INT32 iPlanId;											// Plan ID
	DPSDK_CHAR szPlanName[DPSDK_NAME_LEN];							// Plan name
	DPSDK_INT32 iDataType;											// Data type: 0=task, 1=plan 
}DPSDK_CURRENT_TVWALL_TASK_INFO;

// On-going TV wall task list
typedef struct
{
	DPSDK_UINT32 uiTotal;											// TV wall task number
	DPSDK_CURRENT_TVWALL_TASK_INFO struTVWallTask[1];				// TV wall task list
}DPSDK_CURRENT_TVWALL_TASK_LIST;

// TV wall control command
typedef enum
{
	TVWALL_PLAN_TASK = 0,						// Task on wall/task switch 
	TVWALL_ONESCREEN_SHOW = 1,					// Bond video source
	TVWALL_ONESCREEN_CLOSE = 2,					// Cancel video source
	TVWALL_SCREEN_SPLIT = 3,					// Split
	TVWALL_WINDOW_OPEN = 4,						// Windowing
	TVWALL_WINDOW_CLOSE = 5,					// Close window
	TVWALL_WINDOW_MOVING = 6,					// Window movement
	TVWALL_WINDOW_ZCONTROL = 7,					// Top
	TVWALL_POWER_CTRL = 8,						// Screen switch

	TVWALL_ONESCREEN_CLOSE_ALL = 9,				// smartpss use: close single screen (all windows of single screen)
	TVWALL_CLOSE_PROJECT,						//Disable plan
	//9,  // Enable plan task
	//10,  // Alarm task
	//11,  // Disable plan task
	TVWALL_ONESCREEN_CLEAR = 12,				// Clear single screen; only need corresponding _tvIndex, _screenId, _tvType and _tvWallDBId
	TVWALL_SCREEN_ADDFRAME = 13,				// Highlight window; only need corresponding _tvIndex, _screenId, _subTvIndex, _tvType and _tvWallDBId, Use splitNum here to represent highlight (1) or non-highlight (0); reuse position if it needs RGBA color info
	TVWALL_SPLITWIN_MAX = 14,					// Zoom in split single screen
	TVWALL_TOUR_PAUSE = 15,						// Pause on-wall channel tour. If _screenId is -1, it is valid to the entire wall;if _subTvIndex is -1, it is valid to the entire screen (but _screenId shall be a valid value£©
	TVWALL_TOUR_RESUME = 16,					// Resume on-wall channel tour, same as 15
	TVWALL_SINGLEWINDOW_CHANGE_SOURCE = 17,		// Single-window video source is switched to previous on-wall, _screenId and _subTvIndex shall be valid
	TVWALL_SINGLEWINDOW_SOUND_SWITCH = 18,		// Single-window audio switch control; only need corresponding _tvIndex, _screenId, _subTvIndex, _tvType and _tvWallDBId; use splitNum to represent enable (1) or disable (0)	
	TVWALL_OPENWINDOW_SPLIT = 22,				// Windowing split
}DPSDK_TVWALL_CONTROL_TYPE;

// On-wall control operation
typedef struct
{
	DPSDK_INT32	iControlType;										// Control command. Please refer to DPSDK_TVWALL_CONTROL_TYPE definition
	DPSDK_CHAR szMatrixId[DPSDK_DEVICE_ID_LEN];						// Decoder and matrix ID
	DPSDK_INT32	iTvIndex;											// Screen no.. Ordinary screen is decoding channel no., and splicing screen is screen ID
	DPSDK_INT32 iSubTvIndex;										// Sub-screen no. (valid to splicing screen)
	DPSDK_INT32	iTvType;											// Screen type: 0-non-splicing screen,1-splicing screen
	DPSDK_INT32	iSplitNum;											// Screen split or window number
	//DPSDK_CHAR* pTaskData;											// xml data (refer to xml format)
	//DPSDK_INT32	iTaskLen;											// xml data length
	DPSDK_TVWALL_SCREEN_POS struScreenPos;							// Screen position
	DPSDK_INT32	iTvWallDBId;										// TV wall config proposal DBId
	DPSDK_INT32	iTvWallVersion;										// TV wall version no.
	DPSDK_INT32	iScreenId;											// Screen ID applies to single screen operation in real-time mode; task mode is invalid	
	DPSDK_INT32	iZoder;												// z order, -1 bottom,0 top
	DPSDK_INT32	iSubWindNo;											// Selected split screen no. in window
}DPSDK_TVWALL_CONTROL_INFO;

// Screen switch control
typedef struct
{
	DPSDK_CHAR szMatrixId[DPSDK_DEVICE_ID_LEN];						// Decoder and matrix ID
	DPSDK_INT32	iTvWallId;											// TV wall ID
	DPSDK_INT32	iScreenId;											// Screen ID
	DPSDK_BOOL bPowerOn;											// Enable or not
}DPSDK_TVWALL_POWER_CONTROL_INFO;

// Screen frame
typedef struct
{
	DPSDK_CHAR szMatrixId[DPSDK_DEVICE_ID_LEN];						// Decoder and matrix ID
	DPSDK_INT32	iTvWallId;											// TV wall ID
	DPSDK_INT32	iScreenId;											// Screen ID
	DPSDK_INT32 iSubTvId;											// Window ID
	DPSDK_INT32	iTvIndex;											// In case of splicing screen, it is consistent with screenId; in case of non-splicing screen, it is channel no..
	DPSDK_INT32	iTvType;											// Screen type: 0-non-splicing screen, 1-splicing screen
	DPSDK_BOOL bAddFrame;											// Add frame or not, false: not add, true: add
}DPSDK_TVWALL_SCREEN_ADD_FRAME;

//Fisheye real-time on-wall
typedef struct
{
	DPSDK_CHAR szMatrixId[DPSDK_DEVICE_ID_LEN];						// Decoder and matrix ID
	DPSDK_INT32	iTvWallId;											// TV wall ID
	DPSDK_INT32	iScreenId;											// Screen ID
	DPSDK_INT32 iSubTvId;											// Window ID
	DPSDK_INT32	iTvIndex;											// In case of splicing screen, it is consistent with screenId; in case of non-splicing screen, it is channel no..
	DPSDK_INT32	iTvType;											// Screen type: 0-non-splicing screen, 1-splicing screen
	DPSDK_INT32 iOriginX;
	DPSDK_INT32 iOriginY;
	DPSDK_INT32 iRadius;
	DPSDK_INT32 iLensDirection;

	DPSDK_INT32	iMount;												//Mounting mode
	DPSDK_INT32	iView;												//Viewing mode

	DPSDK_INT32 iEnableAutoContrast;
	DPSDK_INT32 iAlphaHistogram;
	DPSDK_INT32 iAlphaGray;
	DPSDK_INT32 iCaptureSize_width;
	DPSDK_INT32 iCaptureSize_height;
	DPSDK_INT32 iMainStreamSize_width;
	DPSDK_INT32 iMainStreamSize_height;
	DPSDK_INT32 iModeInitParameters_circularOffset;
	DPSDK_INT32 iModeInitParameters_panoramaOffset;
	DPSDK_INT32 iModeInitParameters_useRegionParameters;
	DPSDK_INT32 iModeInitParameters_regionParameters_x;
	DPSDK_INT32 iModeInitParameters_regionParameters_y;
	DPSDK_INT32 iModeInitParameters_regionParameters_hAngle;
	DPSDK_INT32 iModeInitParameters_regionParameters_vAngle;
	DPSDK_INT32 iModeInitParameters_regionParameters_available;
	DPSDK_INT32 iEPtz_optWayType;
	DPSDK_INT32 iEPtz_optWinNum;
	DPSDK_INT32 iEPtz_optWayData;
}DPSDK_TVWALL_FISH_EYE_TO_WALL;

//Window info
typedef struct
{
	DPSDK_TVWALL_SCREEN_POS struWndRect;							//When tvwall_control_e is DTVWALL_WINDOW_OPEN, TVWALL_WINDOW_MOVING is valid
	DPSDK_INT32 iWindowId;											//Window ID
	DPSDK_INT32 iSpliteNum;											//Split number. When tvwall_control_e is TVWALL_SCREEN_SPLIT, it is valid.
	DPSDK_INT32 iOrder;												//Window Z order
}DPSDK_TVWALL_WINDOW_INFO;

// Windowing
typedef struct
{
	DPSDK_INT32	iTvWallId;											// TV wall ID
	DPSDK_INT32	iScreenId;											// Screen ID
	DPSDK_INT32	iTvIndex;											// Physical channel no., which is the same as screen ID in case of splicing screen. 
	DPSDK_CHAR szMatrixId[DPSDK_DEVICE_ID_LEN];						// Decoder and matrix ID
	DPSDK_INT32	iTvType;											// 0-non-splicing screen, 1-splicing screen 
	DPSDK_TVWALL_WINDOW_INFO struWndInfo[1];						// Window list
}DPSDK_TVWALL_OPEN_WINDOW;

//Screen windowing or split capability set
typedef struct
{
	DPSDK_INT32 iScreenId;											// Screen ID
	DPSDK_INT32 iMaxFreeWindows;									// Max. supported number of window, eg:36 
	DPSDK_CHAR szSplitCaps[DPSDK_TVWALL_SPLIT_CAP_LEN];				// Supported window layout, eg:1,4,6,8,9,16,25,36 
}DPSDK_TVWALL_SCREEN_CAPS;

//Screen windowing or split capability set list
typedef struct
{
	DPSDK_INT32	iTvWallId;											// TV wall ID
	DPSDK_UINT32 uiTotal;											// Number of returned capability set 
	DPSDK_TVWALL_SCREEN_CAPS struScreenCapsList[1];					// Capability set list
}DPSDK_TVWALL_SCREEN_CAPS_LIST;

// Screen type
typedef enum
{
	TVWALL_BIG_SCREEN = 1,											// TV wall
	TVWALL_MONITOR_SCREEN = 2,										// Monitor
	TVWALL_WINDOW_SCREEN = 3,										// Window
}DPSDK_TVWALL_SCREEN_TYPE;

// TV wall screen info
typedef struct
{
	DPSDK_INT32 iScreenId;											// Screen ID
	DPSDK_INT32 iParentId;											// Parent screen ID
	DPSDK_CHAR szScreenName[DPSDK_NAME_LEN];						// Screen name
	DPSDK_INT32 iScreenType;										// For screen type, refer to DPSDK_TVWALL_SCREEN_TYPE definition
	DPSDK_INT32 iDeviceCategory;									// Device category
	DPSDK_CHAR szDeviceId[DPSDK_DEVICE_ID_LEN];						// Device code
	DPSDK_CHAR szDeviceName[DPSDK_DEVICE_NAME_LEN];					// Device name
	DPSDK_CHAR szChannelId[DPSDK_CHANNEL_ID_LEN];					// Channel code
	DPSDK_CHAR szChannelName[DPSDK_CHANNEL_NAME_LEN];				// Channel name
	DPSDK_FLOAT fTop;												// Screen top margin (pixel)
	DPSDK_FLOAT fLeft;												// Screen left margin (pixel)
	DPSDK_FLOAT fWidth;												// Screen width (pixel)
	DPSDK_FLOAT fHeight;											// Screen height (pixel)
	DPSDK_BOOL bMergeedBigScreen;									// Is it splicing screen
	DPSDK_BOOL bNewScreen;											// Modify or not
	DPSDK_FLOAT fPerviewTop;										// Screen top margin
	DPSDK_FLOAT fPerviewLeft;										// Screen left margin
	DPSDK_FLOAT fPerviewWidth;										// Screen width
	DPSDK_FLOAT fPerviewHeight;										// Screen height
}DPSDK_TVWALL_SCREEN_INFO;

// TV wall splicing screen info
typedef struct
{
	DPSDK_TVWALL_SCREEN_INFO struScreenInfo;						// Screen info
	DPSDK_INT32 iScreenNum;											// Sub-screen number
	DPSDK_INT32 iMaxScreenNum;										// Max. sub-screen number
	DPSDK_TVWALL_SCREEN_INFO* pScreenList;							// Sub-screen list
}DPSDK_TVWALL_BIG_SCREEN_INFO;

// TV wall config info
typedef struct
{
	DPSDK_INT32 iTvWallId;											// TV wall ID
	DPSDK_CHAR szTvWallName[DPSDK_NAME_LEN];						// TV wall name
	DPSDK_CHAR szTvWallDesc[DPSDK_USER_REMARK_LEN];					// TV wall description
	DPSDK_FLOAT fWidth;												// TV wall width(step 1)
	DPSDK_FLOAT fHeight;											// TV wall height (step 1)
	DPSDK_FLOAT fPerviewWidth;										// TV wall width (step 2)
	DPSDK_FLOAT fPerviewHeight;										// TV wall height (step 2)

	DPSDK_INT32 iWallScreenNum;										// Number of ordinary screen
	DPSDK_INT32 iMaxScreenNum;										// Max. number of ordinary screen
	DPSDK_TVWALL_SCREEN_INFO* pWallScreenList;						// List of ordinary screen (except splicing screen)

	DPSDK_INT32 iBigScreenNum;										// Number of splicing screen
	DPSDK_INT32 iMaxBigScreenNum;									// Max. number of splicing screen
	DPSDK_TVWALL_BIG_SCREEN_INFO* pBigScreenList;					// List of splicing screen
}DPSDK_TVWALL_CONFIG_INFO;

// Device status change notice
typedef struct
{
	DPSDK_CHAR szDeviceID[DPSDK_DEVICE_ID_LEN];						// Device ID
	DPSDK_INT32 iStatus;											// For device status, refer to DPSDK_DEV_STATUS definition
	DPSDK_CHAR szOfflineReason[DPSDK_MEMO_LEN];						// Offline reason
}DPSDK_DEV_STATUS_NOTIFY;

// Channel status change notice
typedef struct  
{
	DPSDK_CHAR szChannelID[DPSDK_CHANNEL_ID_LEN];					// Channel ID
	DPSDK_INT32 iStatus;											// For channel status, refer to DPSDK_DEV_STATUS definition
}DPSDK_CHANNEL_STATUS_NOTIFY;

//  Channel smart status change notice
typedef struct
{
	DPSDK_CHAR szChannelID[DPSDK_CHANNEL_ID_LEN];					// Channel ID
	DPSDK_INT32 iIntelliState;										// Smart status
}DPSDK_CHANNEL_INTELLISTATE_NOTIFY; 

// Organization movement notice
typedef struct 
{
	DPSDK_CHAR szOldOrgCode[DPSDK_ORG_CODE_LEN];						// Old organization node code 
	DPSDK_CHAR szOldParentOrgCode[DPSDK_ORG_CODE_LEN];					// Old organization parent node code
	DPSDK_CHAR szNewOrgCode[DPSDK_ORG_CODE_LEN];						// New organization node code
	DPSDK_CHAR szNewParentOrgCode[DPSDK_ORG_CODE_LEN];					// New organization parent node code
}DPSDK_MOVE_ORG_NOTIFY;

// Add device notice
typedef struct
{
	DPSDK_CHAR szOrgCode[DPSDK_ORG_CODE_LEN];							// Organization code
	DPSDK_DEV_ALL_INFO struDevAllInfo;									// Device data
}DPSDK_ADD_DEVICE_NOTIFY;

// Modify device notice
typedef struct
{
	DPSDK_CHAR szOldOrgCode[DPSDK_ORG_CODE_LEN];						// Old organization
	DPSDK_CHAR szNewOrgCode[DPSDK_ORG_CODE_LEN];						// New organization
	DPSDK_DEV_ALL_INFO struDevAllInfo;									// Device data	
}DPSDK_MODIFY_DEVICE_NOTIFY;

// Delete device notice in batches
typedef struct  
{
	DPSDK_CHAR szOrgCode[DPSDK_ORG_CODE_LEN];							// Organization code
	DPSDK_CHAR szDeviceID[DPSDK_DEVICE_ID_LEN];							// Device ID
}DPSDK_DELETE_DEVICE_NOTIFY;

// Move device notice in batches
typedef struct  
{
	DPSDK_CHAR szDeviceID[DPSDK_DEVICE_ID_LEN];							// Device ID
	DPSDK_CHAR szOldOrgCode[DPSDK_ORG_CODE_LEN];						// Previous organization code of device
	DPSDK_CHAR szNewOrgCode[DPSDK_ORG_CODE_LEN];						//  New organization code of device
}DPSDK_MOVE_DEVICE_NOTIFY; 

//User organization search condition
typedef struct  
{
	DPSDK_CHAR szOrgCode[DPSDK_ORG_CODE_LEN];						// If organization code length is 0, search root organization by default.
	DPSDK_INT32 iSubNodeType;										// Search child node type. Refer to DPSDK_SUB_CODE_TYPE definition.
}DPSDK_QUERY_USER_ORG_INFO;

// User status
typedef enum
{
	USER_OFFLINE,			// Offline
	USER_ONLINE,			// Online
}DPSDK_USER_STATUS;

// User online status notice
typedef struct
{
	DPSDK_CHAR szUserId[DPSDK_USER_ID_LEN];								// User ID
	DPSDK_INT32 iOnlineStatus;											// Online status, refer to DPSDK_USER_STATUS definition.
}DPSDK_USERONLINESTATUS_NOTIFY;

// Delete user notice
typedef struct
{
	DPSDK_CHAR szUserId[DPSDK_USER_ID_LEN];								// User ID
}DPSDK_USERDELETE_NOTIFY;

// Add user notice
typedef struct
{
	DPSDK_CHAR szUserId[DPSDK_USER_ID_LEN];								// User ID
	DPSDK_CHAR	szUserName[DPSDK_NAME_LEN];								// Username
	DPSDK_CHAR	szRoleID[DPSDK_USER_ID_LEN];							// Role ID
}DPSDK_USERADD_NOTIFY;

//  Visible range change notice
typedef struct
{
	DPSDK_CHAR	szChannelId[DPSDK_CHANNEL_ID_LEN];					// Channel ID
	DPSDK_INT32	iAzimuth;											// Start angle of visible range
	DPSDK_INT32	iDistance;											// Distance (radius)of visible range
	DPSDK_INT32	iAngle;												// Included angle of visible range
}DPSDK_VIEWINFO_CHANGED_NOTIFY;

// Subscription notice of master-slave tracking of fixed-PTZ camera 
typedef struct
{
	DPSDK_CHAR		szDevId[DPSDK_DEVICE_ID_LEN];		// Device ID
	DPSDK_INT32		iGroup;								//Subscribed group
	DPSDK_INT32     iSubscribeID;                      // Server subscription ID
	DPSDK_INT32     iSlaveID;                          // Slave S./N.in tracking group
	DPSDK_CHAR      szClass[MASTERSALVE_CLASS_LEN];    // Algorithm proposal type
	DPSDK_INT32     iObjectID;                         // Algorithm object ID
}DPSDK_SMARTTRACKOBJECT_NOTIFY;

// Device code info
typedef struct
{
	DPSDK_CHAR	szDeviceCode[DPSDK_DEVICE_CODE_LEN];				// Device code
}DPSDK_DEVICECODE_INFO;

// Notice of adding and modifying device on map
typedef struct
{
	DPSDK_UINT32			uiDeviceCodeListSize;					// Number of device code list
	DPSDK_DEVICECODE_INFO	struDeviceCodeList[1];					// Device code list		
}DPSDK_DEVICELOCATION_NOTIFY;

// PTZ locking status change notice
typedef struct
{
	DPSDK_INT32			iLockStatus;								// Locking status, 0=unlock, 1=lock
	DPSDK_CHAR			szChannelId[DPSDK_CHANNEL_ID_LEN];			// Channel ID
	DPSDK_CHAR			szLockUserName[DPSDK_NAME_LEN];				// Name of the user locking PTZ 
	DPSDK_CHAR			szLockUserLevel[DPSDK_USER_LEVEL_LEN];		// Level of the user locking PTZ
}DPSDK_LOCKSTATUS_CHANGED_NOTIFY;

// TV wall notice  <add/modify>
typedef struct  
{
	DPSDK_UINT32 uiTvWallId;										// TV wall ID
	DPSDK_CHAR szTvWallName[DPSDK_TVWALL_NAME_LEN];					// TV wall name
	DPSDK_CHAR szOwnerCode[DPSDK_TVWALL_OWNERCODE_LEN];				// TV wall code
	DPSDK_INT32 iStates;											// TV wall status, 0=disable, 1=enable
	DPSDK_CHAR szTvWallXml[1];										// TV wall XML
}DPSDK_TVWALL_NOTIFY;

// Radar target info
typedef struct
{
	DPSDK_INT32		iId;				// Info ID
	DPSDK_INT32		iTargetId;			// Target ID   		
	DPSDK_FLOAT		fTargetLen;			// Target length    	
	DPSDK_FLOAT		fSpeed_X;			// X direction speed        		
	DPSDK_FLOAT		fSpeed_Y;			// Y direction speed       		
	DPSDK_FLOAT		fCod_X;				// X coordinate   			
	DPSDK_FLOAT		fCod_Y;				// Y coordinate   			
	DPSDK_FLOAT		fDistance;			// Distance      	
	DPSDK_FLOAT		fAzimuth;			// Azimuth angle       	
	DPSDK_FLOAT		fSNR;				// Target SNR	         	
	DPSDK_FLOAT		fEN;				// Target peak energy
}DPSDK_RADER_TARGET_INFO;

// Radar info report notice
typedef struct
{
	DPSDK_CHAR				szRaderId[DPSDK_DEVICE_ID_LEN];		// Radar ID
	DPSDK_UINT32			uiRaderTargetInfoSize;				// Number of radar target info
	DPSDK_RADER_TARGET_INFO	struRaderTargetInfo[1];				// Radar target info list
}DPSDK_RADERFRAME_NOTIFY;

// Similar face info
typedef struct
{
	DPSDK_CHAR			szUID[DPSDK_PERSON_ID_LEN];			// Platform personnel ID, virtual value
	DPSDK_CHAR			szFaceImageUrl[DPSDK_URL_LEN];		// Face image
	DPSDK_CHAR			szName[DPSDK_NAME_LEN];				// Name
	DPSDK_INT32			iGender;							// Gender: 0-unknown, 1-male, 2-female
	DPSDK_CHAR			szBirthday[DPSDK_BIRTHDAY_LEN];		// Birthday
	DPSDK_INT32			iPersonType;						// Personnel type (for compatibility)
	DPSDK_INT32			iPersonTypeId;						// Personnel type ID
	DPSDK_CHAR			szPersonTypeName[DPSDK_NAME_LEN];	// Personnel type name
	DPSDK_CHAR			szPersonId[DPSDK_PERSON_ID_LEN];	// Personnel ID
	DPSDK_FLOAT			fSimilarity;						// Similarity
	DPSDK_BOOL			bSurveillance;						// Surveillance or not
	DPSDK_CHAR			szRepositoryId[DPSDK_DB_ID_LEN];			// Library ID
	DPSDK_CHAR			szRepositoryName[DPSDK_PERSONTYPE_NAME_LEN];// Library name
	DPSDK_INT32			iRepositoryColor;					// Library color: 1=gray, 2=red, 3=orange, 4=yellow£¬5=green£¬6=cyan£¬7=blue£¬8=purple
	DPSDK_CHAR			szNationality[DPSDK_NATIONALITRY_LEN];// Nationality
	DPSDK_CHAR			szMemo[DPSDK_MEMO_LEN];				// Note
}DPSDK_FACE_INFO;

// User status
typedef enum
{
	FACE_ALARM_EVENT			= 1,						// Face Alarm
	FACE_TRAFFIC_JUNCTION_EVENT = 2,						// Traffic Junction
	FACE_ANALYSIS_EVENT			= 3,						// Face Analysis 
}DPSDK_FACE_EVENT_TYPE;

// Face capture info notice
typedef struct
{
	DPSDK_CHAR			szChannelId[DPSDK_CHANNEL_ID_LEN];	// Channel ID
	DPSDK_CHAR			szAlarmID[DPSDK_ID_LEN];			// Alarm ID
	DPSDK_CHAR			szRecordID[DPSDK_ID_LEN];			// Record ID
	DPSDK_INT32			iEventType;							// Event Type
	DPSDK_INT32			iFaceImageId;						// Captured face ID
	DPSDK_CHAR			szFaceImageUrl[DPSDK_URL_LEN];		// Captured face image
	DPSDK_BOOL			bHited;								// Recognition or not (0-capture, 1-recognition)
	DPSDK_CHAR			szPictureUrl[DPSDK_URL_LEN];		// Scenario
	DPSDK_INT32			iRecAge;							// Recognize age
	DPSDK_INT32			iRecExpress;						// Recognize express
	DPSDK_INT32			iRecFringe;							// Recognize bang, 0-none, 1-yes
	DPSDK_INT32			iRecSex;							// Recognize gender, 0-unknown, 1-male, 2-female
	DPSDK_INT32			iRecEye;							// Recognize eye, 1-Not recognized, 2-closed eye 3-open eye
	DPSDK_INT32			iRecMouth;						// Recognize mouth, 1-Not recognized, 2-closed mouth, 3-open mouth
	DPSDK_INT32			iRecMask;						    // Recognize mask, 1-Not recognized, 2- without mask, 3-with mask
	DPSDK_INT32			iRecBeard;					        // Recognize mustache, 1-Not recognized, 2-without mustache, 3-with mustache
	DPSDK_INT32			iRecGlasses;						// Recognize glasses, 0-none, 1-glasses, 2-sunglasses
	DPSDK_INT32			iRecEmotion;						// Recognize emotion, 0-smiling, 1-angry, 2-sad, 3-disgusted, 4-afraid, 5-surprised, 6-normal, 7- laughter, 8-glad, 9-puzzled, 10-screamy
	DPSDK_INT32			iAppearTimes;						// Historical frequency of occurrence
	DPSDK_CHAR			szBeginTime[DPSDK_TIME_LEN];		// Time of entering view
	DPSDK_CHAR			szEndTime[DPSDK_TIME_LEN];			// Time of leaving view
	DPSDK_UINT32		uiSimilarFaceListSize;				// Number of similar face info
	DPSDK_FACE_INFO		struSimilarFaceList[1];				// List of similar face info (there will be result when hited=1)
}DPSDK_FACE_INFO_NOTIFY;

//Face library update info notice
typedef struct  
{
	DPSDK_INT32			iOperateType;								// Operation type: 1-add, 2-update, 3-delete
	DPSDK_CHAR			szRepositoryId[DPSDK_DB_ID_LEN];			// Library ID
	DPSDK_CHAR			szRepositoryName[DPSDK_PERSONTYPE_NAME_LEN];// Library name
	DPSDK_INT32			iRepositoryColor;							// Library color: 1=gray, 2=red, 3=orange, 4=yellow£¬5=green£¬6=cyan£¬7=blue£¬8=purple
	DPSDK_CHAR			szMemo[DPSDK_MEMO_LEN];						// Note
}DPSDK_UPDATE_FACE_REPOSITORY_NOTIFY;

// Personnel type change notice
typedef struct
{
	DPSDK_INT32			iOperateType;								// Operation type: 1-add, 2-update, 3-delete
	DPSDK_INT32			iPersonTypeId;								// Personnel type ID
	DPSDK_CHAR			szPersonTypeName[DPSDK_PERSONTYPE_NAME_LEN];// Personnel type name
}DPSDK_PERSONTYPE_NOTIFY;

// Personnel info
typedef struct
{
	DPSDK_CHAR			szName[DPSDK_NAME_LEN];						// Name
	DPSDK_INT32			iGender;									// Gender
	DPSDK_CHAR			szBirthday[DPSDK_BIRTHDAY_LEN];				// Birthday
	DPSDK_INT32			iPersonType;								// Personnel type ID
	DPSDK_CHAR			szPersonId[DPSDK_PERSON_ID_LEN];			// Personnel ID
	DPSDK_CHAR			szNationality[DPSDK_NATIONALITRY_LEN];		// Nationality
	DPSDK_CHAR			szMemo[DPSDK_MEMO_LEN];						// Note
	DPSDK_CHAR			szRepositoryId[DPSDK_DB_ID_LEN];			// Library ID
}DPSDK_PERSON_INFO;

// Personnel info
typedef struct
{
	DPSDK_INT32			iPersonNum;									// Personnel number
	DPSDK_PERSON_INFO* pPersonList;									// Personnel info list
}DPSDK_PERSON_INFO_LIST;

// Face image info
typedef struct
{
	DPSDK_CHAR			szFaceImageUrl[DPSDK_URL_LEN];				// Face image
}DPSDK_FACEIMAGE_INFO;

//User self-defining data change type
typedef enum 
{
	AddUserDefineData,												//Add user self-defining data
	ModifyUserDefineData,											//Modify user self-defining data
	DeleteUserDefineData,											//Delete user self-defining data
}DPSDK_ALERT_USERDEFINEDATA_TYPE;

// Change user self-defining data
typedef struct
{
	DPSDK_INT32 iAlertUserDefineDataType;							//user self-defining data change type. Refer to DPSDK_ALERT_USERDEFINEDATA_TYPE definition
	DPSDK_CHAR szFileName[DPSDK_USERDEFINEDATA_FILENAME_LEN];		//Self-defining user file name
	DPSDK_CHAR szFileData[1];										//Self-defining user file info
}DPSDK_USERDEFINE_INFO;

typedef struct
{
	DPSDK_CHAR				szDeviceCode[DPSDK_DEVICE_ID_LEN];
	DPSDK_CHAR				szPOSChnlId[DPSDK_CHANNEL_ID_LEN];
	DPSDK_INT32				iPOSDataLen;
	DPSDK_CHAR				*pPOSData;
	DPSDK_TIMET				lPostTime;
}DPSDK_POS_DATA_NOTIFY;

typedef struct
{
	DPSDK_CHAR szChannelId[DPSDK_CHANNEL_ID_LEN];	//Channel ID
	DPSDK_INT32 iNum;	//Number of linked video channel
	DPSDK_LINKED_CHANNEL	LinkChannel[DPSDK_LINKED_CHANNEL_SIZE];
	DPSDK_INT32 iLocation;	//Central record or device record
} DPSDK_ADD_RELATION_NOTIFY;

typedef struct
{
	DPSDK_CHAR szChannelId[DPSDK_CHANNEL_ID_LEN];	//Channel ID
	DPSDK_INT32 iNum;	//Number of linked video channel
	DPSDK_LINKED_CHANNEL	LinkChannel[DPSDK_LINKED_CHANNEL_SIZE];
	DPSDK_INT32 iLocation;	//Central record or device record
} DPSDK_MODIFY_RELATION_NOTIFY;

typedef struct
{
	DPSDK_CHAR	szChannelId[DPSDK_CHANNEL_ID_LEN];	//Channel ID
} DPSDK_DELETE_RELATION_NOTIFY;

// Map change notice info structure
typedef struct
{
	DPSDK_INT32 iOperateType;						//Operation type: 1-add, 2-modify, 3-delete
	DPSDK_CHAR	szMapID[DPSDK_DEVICE_ID_LEN];		//Map ID
	DPSDK_CHAR	szMapName[DPSDK_NAME_LEN];			//Map name
	DPSDK_CHAR	szMapPath[DPSDK_URL_LEN];			//Map path
	DPSDK_CHAR	szParentID[DPSDK_DEVICE_ID_LEN];	//Superior ID
	DPSDK_INT32	iStatus;							//Enable status, 0=disabled,1=enabled
} DPSDK_BITMAP_INFO_NOTIFY;

//Decoding type
typedef enum
{
	DPSDK_DECODE_SW = 0,							//CPU decoding
	DPSDK_DECODE_HW = 1,							//GPU decoding
	DPSDK_DECODE_HW_FAST = 2,						//GPU fast decoding
}DPSDK_DECODE_TYPE;

// Menu right list
typedef struct
{
	DPSDK_UINT32 uiTotal;							// Total obtained result
	DPSDK_MENU_RIGHT struMenuRight[1];				// Menu right list
}DPSDK_MENU_RIGHT_LIST;

typedef struct
{
	DPSDK_CHAR szUserName[DPSDK_NAME_LEN];							// User name;
}DPSDK_USER_INFO;

typedef struct
{
	DPSDK_INT32 iUserNameNum;						
	DPSDK_USER_INFO* pUserNameList;
}DPSDK_USER_LIST;


// Face menu info
typedef struct
{
	DPSDK_INT32 iID;											// Menu ID
	DPSDK_CHAR szMenuCode[DPSDK_MEMURIGHT_LEN];					// Menu code
	DPSDK_CHAR szMenuName[DPSDK_NAME_LEN];						// Menu name
	DPSDK_CHAR szMenuIcon1[DPSDK_URL_LEN];						// Menu icon 1
	DPSDK_CHAR szMenuIcon2[DPSDK_URL_LEN];						// Menu icon 2
	DPSDK_CHAR szMenuPath1[DPSDK_URL_LEN];						// Menu URL path 1
	DPSDK_CHAR szMenuPath2[DPSDK_URL_LEN];						// Menu URL path 2
	DPSDK_BOOL bMenu;											// false: button,true: menu
	DPSDK_BOOL bDataScope;										// false: data right is not required, true: data right is required
	DPSDK_INT32 iSort;											// Sort key
	DPSDK_CHAR szSystemName[DPSDK_NAME_LEN];					// System name
}DPSDK_FACE_MENU;

// Face menu list
typedef struct
{
	DPSDK_UINT32 uiTotal;							// Total obtained result
	DPSDK_FACE_MENU struMenu[1];					// Menu list
}DPSDK_FACE_MENU_LIST;

// Vehicle big data menu info
typedef struct
{
	DPSDK_INT32 iID;											// Menu ID
	DPSDK_CHAR szMenuCode[DPSDK_MEMURIGHT_LEN];					// Menu code
	DPSDK_CHAR szMenuName[DPSDK_NAME_LEN];						// Menu name
	DPSDK_CHAR szMenuIcon1[DPSDK_URL_LEN];						// Menu icon 1
	DPSDK_CHAR szMenuIcon2[DPSDK_URL_LEN];						// Menu icon 2
	DPSDK_CHAR szMenuPath1[DPSDK_URL_LEN];						// Menu URL path 1
	DPSDK_CHAR szMenuPath2[DPSDK_URL_LEN];						// Menu URL path 2
	DPSDK_BOOL bMenu;											// false: button,true: menu
	DPSDK_BOOL bDataScope;										// false: data right is not required, true: data right is required
	DPSDK_INT32 iSort;											// Sort key
	DPSDK_CHAR szSystemName[DPSDK_NAME_LEN];					// System name
}DPSDK_VEHICLE_MENU;

// Vehicle big data menu info
typedef struct
{
	DPSDK_UINT32 uiTotal;							// Total obtained result
	DPSDK_VEHICLE_MENU struMenu[1];					// Menu list
}DPSDK_VEHICLE_MENU_LIST;

// All sub service menu info
typedef struct
{
	DPSDK_INT32 iID;											// Menu ID
	DPSDK_CHAR szMenuCode[DPSDK_MEMURIGHT_LEN];					// Menu code
	DPSDK_CHAR szMenuName[DPSDK_NAME_LEN];						// Menu name
	DPSDK_CHAR szMenuIcon1[DPSDK_URL_LEN];						// Menu icon 1
	DPSDK_CHAR szMenuIcon2[DPSDK_URL_LEN];						// Menu icon 2
	DPSDK_CHAR szMenuPath1[DPSDK_URL_LEN];						// Menu URL path 1
	DPSDK_CHAR szMenuPath2[DPSDK_URL_LEN];						// Menu URL path 2
	DPSDK_BOOL bMenu;											// false: button,true: menu
	DPSDK_BOOL bDataScope;										// false: data right is not required, true: data right is required
	DPSDK_INT32 iSort;											// Sort key
	DPSDK_CHAR szSystemName[DPSDK_NAME_LEN];					// System name
}DPSDK_SERVICE_MENU;

// All sub service menu list
typedef struct
{
	DPSDK_UINT32 uiTotal;										// Total obtained result
	DPSDK_SERVICE_MENU struMenu[1];								// Menu list
}DPSDK_SERVICE_MENU_LIST;

// E-map menu info
typedef struct
{
	DPSDK_INT32 iID;											// Menu ID
	DPSDK_CHAR szMenuCode[DPSDK_MEMURIGHT_LEN];					// Menu code
	DPSDK_CHAR szMenuName[DPSDK_NAME_LEN];						// Menu name
	DPSDK_CHAR szMenuIcon1[DPSDK_URL_LEN];						// Menu icon 1
	DPSDK_CHAR szMenuIcon2[DPSDK_URL_LEN];						// Menu icon 2
	DPSDK_CHAR szMenuPath1[DPSDK_URL_LEN];						// Menu URL path 1
	DPSDK_CHAR szMenuPath2[DPSDK_URL_LEN];						// Menu URL path 2
	DPSDK_BOOL bMenu;											// false: button,true: menu
	DPSDK_BOOL bDataScope;										// false:false: data right is not required, true: data right is required
	DPSDK_INT32 iSort;											// Sort key
	DPSDK_CHAR szSystemName[DPSDK_NAME_LEN];					//  System name
}DPSDK_EMAP_MENU;

// E-map menu info
typedef struct
{
	DPSDK_UINT32 uiTotal;							// Total obtained result
	DPSDK_EMAP_MENU struMenu[1];					// Menu list
}DPSDK_EMAP_MENU_LIST;

// User online status
typedef enum
{
	USER_STSTUS_UNDEFINE = 0,						//Undefined
	USER_STATUS_ONLINE = 1,							//Online
	USER_STATUS_OFFLINE = 2,						//Offline
}DPSDK_USER_ONLINESTATUS_TYPE;

#define DPSDK_MAX_SERVER_ABILITY_NUM	64			// Max. number of service capability set

// Service capability
typedef enum
{
	SERVER_ABILITY_NULL = 0,						// 
	SERVER_ABILITY_VIDEO	= 1,					// Video
	SERVER_ABILITY_SYNOPSIS = 2,					// Cloud Synopsis
	SERVER_ABILITY_VEHICLE	= 3,					// Vehicle
	SERVER_ABILITY_FACE		= 4,					// Face
}DPSDK_SERVER_ABILITY;

typedef enum
{
	DEVICE_MODEL_UNKNOW = 0,						// Unknown
	DEVICE_MODEL_IPC_123 = 1,						//
	DEVICE_MODEL_IPC_245 = 2,						//
	DEVICE_MODEL_IPC_256 = 3,						// 
}DPSDK_DEVICE_MODEL;

typedef enum
{
	DEVICE_MANFAC_UNKNOW = 0,						// Unknown
	DEVICE_MANFAC_DAHUA = 1,						// Dahua
	DEVICE_MANFAC_HIKVISION = 2,					// HIKVISION
	DEVICE_MANFAC_UNIVIEW = 3,						// UNIVIEW
}DPSDK_DEVICE_MANFAC;
//User login result
typedef struct
{
	DPSDK_UINT32 iSvrAbilityNum;					// Number of service capability set
	DPSDK_INT32 iServerAbility[DPSDK_MAX_SERVER_ABILITY_NUM];// Service capability set
	DPSDK_CHAR szSipNum[DPSDK_ID_LEN];				// User telephone number
}DPSDK_LOGIN_RESULT;

// User login data
typedef struct
{
	DPSDK_LOGIN_PARAM struLoginParam;				// User login info
	DPSDK_CHAR szUserId[DPSDK_USER_ID_LEN];			// User ID
	DPSDK_USHORT ushHttpPort;						// HTTP port
	DPSDK_LOGIN_RESULT struLoginRes;				// Login return result
}DPSDK_USER_LOGIN_DATA;

// User login data
typedef struct
{
	DPSDK_BOOL bSSLEnable;								// SSL enable
	DPSDK_CHAR szCertificatePath[DPSDK_FILE_PATH_LEN];	// Certificate path
}DPSDK_MQ_SSL_INFO;

// Temperature unit
typedef enum
{
	TEMPERATURE_UNIT_UNKNOWN = 0,					// Unknown
	TEMPERATURE_UNIT_CENTIGRADE = 1,				// Degree centigrade
	TEMPERATURE_UNIT_FAHRENHEIT = 2,				// Fahrenheit degree
}DPSDK_TEMPERATURE_UNIT_TYPE;

// Temperature data
typedef struct
{
	DPSDK_FLOAT fTemperatureValue;					//Temperature value
	DPSDK_INT32 iTemperatureType;					//Temperature type
}DPSDK_TEMPERATURE_INFO;

// Smart analysis time slice of cloud synopsis
typedef struct
{
	DPSDK_TIMET tBeginTime;							//Start time of smart analysis
	DPSDK_TIMET tEndTime;							//End time of smart analysis
}DPSDK_ABSTRACT_ANALYSIS_TIME_SEC;

// Smart analysis time slice of cloud synopsis
typedef struct
{
	DPSDK_UINT32 uiTotal;							// Time slice number
	DPSDK_ABSTRACT_ANALYSIS_TIME_SEC struTimeSec[1];// Time slice list
}DPSDK_ABSTRACT_ANALYSIS_TIME_SEC_LIST;

// Smart analysis time slice request parameter of cloud synopsis
typedef struct
{
	DPSDK_CHAR szChannelId[DPSDK_CHANNEL_ID_LEN];	// Channel ID
	DPSDK_TIMET tBeginTime;							// Start time
	DPSDK_TIMET tEndTime;							// End time
}DPSDK_ABSTRACT_ANALYSIS_TIME_QUERY_PARAM;

// Color
typedef struct
{
	DPSDK_CHAR szColor[DPSDK_COLOR_LEN];			// Color
}DPSDK_COLOR;


// Location info
typedef struct
{
	DPSDK_INT32 iX;										// x coordinate
	DPSDK_INT32 iY;										// y coordinate
	DPSDK_INT32 iWidth;									// Width
	DPSDK_INT32 iHeight;									// Height
}DPSDK_POSITION;

// Line info
typedef struct
{
	DPSDK_INT32 iPointNum;							// Point number
	DPSDK_POINT* pPointList;						// Point list
	DPSDK_INT32 iDerect;							// Tripwire direction, 1: from left to right, 2: from right to left, 3: bilateral
}DPSDK_LINE;

typedef enum
{
	ABS_OBJ_OTHER			= 0,					// Other
	ABS_OBJ_HUMAN			= 1,					// Person
	ABS_OBJ_VEHICLE			= 2,					// Vehicle
}DPSDK_ABS_OBJ_TYPE;

typedef enum
{
	ABS_OBJ_COLOR_BLACK		= 0,					// Black
	ABS_OBJ_COLOR_WHITE		= 1,					// White
	ABS_OBJ_COLOR_GRAY		= 2,					// Gray
	ABS_OBJ_COLOR_RED		= 3,					// Red
	ABS_OBJ_COLOR_ORANGE	= 4,					// Orange
	ABS_OBJ_COLOR_YELLOW	= 5,					// Yellow
	ABS_OBJ_COLOR_GREEN		= 6,					// Green
	ABS_OBJ_COLOR_BLUE		= 7,					// Blue
	ABS_OBJ_COLOR_CYAN		= 8,					// Cyan
	ABS_OBJ_COLOR_PURPLE	= 9,					// Purple
	ABS_OBJ_COLOR_ALL		= 10,					//All
}DPSDK_ABS_OBJ_COLOR;

typedef enum
{
	ABS_OBJ_SIZE_SMALL		= 0,					// Small
	ABS_OBJ_SIZE_MIDDLE		= 1,					// Medium
	ABS_OBJ_SIZE_BIG		= 2,					// Large
	ABS_OBJ_SIZE_ALL		= 3,					// Unlimited
}DPSDK_ABS_OBJ_SIZE;

typedef enum
{
	ABS_OBJ_SPEED_SLOW		= 0,					// Slow
	ABS_OBJ_SPEED_MIDDLE	= 1,					// Medium
	ABS_OBJ_SPEED_FAST		= 2,					// Quick
	ABS_OBJ_SPEED_ALL		= 3,					// Unlimited
}DPSDK_ABS_OBJ_SPEED;

// Cloud synopsis detailed info request property
typedef struct
{
	DPSDK_INT32 iObjTypeNum;						// Target type number
	DPSDK_INT32* pObjTypeList;						//  Target type list. Refer to DPSDK_ABS_OBJ_TYPE definition
	DPSDK_INT32 iColor;								// Target color list. Refer to DPSDK_ABS_OBJ_COLOR definition
	DPSDK_INT32 iDirect;							// Target direction list. 0-360, 12 o'clock direction is 0. Clockwise rotation
	DPSDK_INT32 iSize;								//  Target size list. Refer to DPSDK_ABS_OBJ_SIZE definition
	DPSDK_INT32 iSpeed;								//  Target speed list. Refer to DPSDK_ABS_OBJ_SPEED definition
}DPSDK_ABSTRACT_OBJ_ATTR;

// Cloud synopsis detailed info request condition
typedef struct
{
	DPSDK_INT32 iDetictArrayWidth;					// Array width
	DPSDK_INT32 iDetictArrayHeight;					// Array height
	DPSDK_POINT** pDetectZone;						// Detection zone

	DPSDK_INT32 iMaskArrayWidth;					// Array width
	DPSDK_INT32 iMaskArrayHeight;					// Array height
	DPSDK_POINT** pMaskZone;						// Mask zone

	DPSDK_INT32 iLineNum;							// Line number
	DPSDK_LINE* pLineList;							// Line list
}DPSDK_ABSTRACT_ADV_CONDITION;

// Cloud synopsis detailed info request parameter
typedef struct
{
	//DPSDK_CHAR szChannelId[DPSDK_CHANNEL_ID_LEN];	// Channel ID
	//DPSDK_TIMET tTime;								// Time
	
	DPSDK_CHAR szRegionId[DPSDK_ID_LEN];			// Search domain
	DPSDK_INT32 iOrgCodeNum;						// Organization code number
	DPSDK_ID* pOrgCodeList;							// Organization code
	DPSDK_INT32 iDevIdNum;							// Device ID number
	DPSDK_DEVICE_ID* pDevIdList;					// Device ID list
	DPSDK_INT32 iChannelIdNum;						// Channel ID number
	DPSDK_CHANNEL_ID* pChannelIdList;				// Channel ID list
	DPSDK_TIMET tBeginTime;							// Start time
	DPSDK_TIMET tEndTime;							// End time
	// objectAttribute
	DPSDK_ABSTRACT_OBJ_ATTR struObjAttr;			// Target property
	//advancedConditions
	DPSDK_ABSTRACT_ADV_CONDITION struAdvCond;		// Advanced query condition
}DPSDK_ABSTRACT_OBJ_DETAIL_INFO_QUERY_PARAM;

// Cloud synopsis image info
typedef struct
{
	DPSDK_INT32 iWidth;									// Width
	DPSDK_INT32 iHeight;									// Height
	DPSDK_POSITION struPos;									// Location info
	DPSDK_TIMET tTime;										// Time
	DPSDK_CHAR szUrl[DPSDK_FILE_PATH_LEN];					// Original image address
	DPSDK_CHAR szCutoutUrl[DPSDK_FILE_PATH_LEN];			// Sectional image address
	DPSDK_CHAR szSmallUrl[DPSDK_FILE_PATH_LEN];				// Small image address
}DPSDK_ABSTRACT_OBJ_PIC_INFO;

typedef enum
{
	ABS_TYPE_HUAMN			= 1,							// Person
	ABS_TYPE_VECHILE		= 2,							// Vehicle
	ABS_TYPE_OTHER			= 3,							// Other
}DPSDK_ABSTRACT_OBJ_TYPE;

// Cloud synopsis detailed info
typedef struct
{
	DPSDK_CHAR szId[DPSDK_ID_LEN];							// Single ID of target
	DPSDK_CHAR szChannelId[DPSDK_CHANNEL_ID_LEN];			// Channel ID
	DPSDK_CHAR szChannelName[DPSDK_CHANNEL_NAME_LEN];		// Channel name
	DPSDK_TIMET tBeginTime;									// Target appearing time
	DPSDK_TIMET tEndTime;									// Target disappearing time
	DPSDK_INT32 iType;										// Target type. Refer to DPSDK_ABSTRACT_OBJ_TYPE definition.
	DPSDK_INT32 iDirect;									// Target direction list. 0-360, 12 o'clock direction is 0. Clockwise rotation
	DPSDK_INT32 iSize;										// Target size. Refer to DPSDK_ABS_OBJ_SIZE definition
	DPSDK_INT32 iSpeed;										// Target speed. Refer to DPSDK_ABS_OBJ_SPEED definition
	DPSDK_ABSTRACT_OBJ_PIC_INFO struPic;					// Image info
}DPSDK_ABSTRACT_OBJ_DETAIL_INFO;

//Cloud synopsis detailed info list
typedef struct
{
	DPSDK_UINT32 uiTotal;									// Detailed info number
	DPSDK_ABSTRACT_OBJ_DETAIL_INFO struObjDetailInfo[1];	// Detailed info list
}DPSDK_ABSTRACT_OBJ_DETAIL_LIST;

typedef enum
{
	ABS_PIC_VIDEO			= 1,							// Real-time synopsis
	ABS_PIC_BAY_DEV			= 2,							// ANPR device
}DPSDK_ABS_PIC_TYPE;

// Cloud synopsis image lifecycle
typedef struct
{
	DPSDK_INT32 iPicType;									// Refer to DPSDK_ABS_PIC_TYPE definition
	DPSDK_INT32 iLifeCycle;									// Lifecycle
	DPSDK_INT32 iDataNum;									// 
	DPSDK_INT32 iParityNum;									//
}DPSDK_ABSTRACT_LIFE_CYCLE_INFO;

// Cloud synopsis image lifecycle list
typedef struct
{
	DPSDK_UINT32 uiTotal;									// Number
	DPSDK_ABSTRACT_LIFE_CYCLE_INFO struList[1];				// Cloud synopsis image lifecycle list
}DPSDK_ABSTRACT_LIFE_CYCLE_LIST;

// Cloud synopsis summary info query parameter
typedef struct
{
	DPSDK_CHAR szChannelId[DPSDK_CHANNEL_ID_LEN];			// Channel ID
	DPSDK_TIMET tBeginTime;									// Target appearing time
	DPSDK_TIMET tEndTime;									// Target disappearing time
}DPSDK_ABSTRACT_OBJ_SUMMMARY_INFO_QUERY_PARAM;

// Cloud synopsis summary info
typedef struct
{
	DPSDK_TIMET tBeginTime;									// Start time
	DPSDK_TIMET tEndTime;									// End time
	DPSDK_INT32 iObjectTimeNum;								// Target time number
	DPSDK_TIMET* pObjectTimeList;							// Target time list
}DPSDK_ABSTRACT_OBJ_SUMMMARY_INFO;

// Cloud synopsis summary info list
typedef struct
{
	
	DPSDK_UINT32 uiTotal;									// Summary info number
	DPSDK_ABSTRACT_OBJ_SUMMMARY_INFO* pObjSummaryInfoList;	// Summary info list
}DPSDK_ABSTRACT_OBJ_SUMMMARY_LIST;

// Cloud synopsis detailed info change parameter
typedef struct
{
	DPSDK_CHAR szChannelId[DPSDK_CHANNEL_ID_LEN];			// Channel ID
	DPSDK_CHAR szId[DPSDK_ID_LEN];							// Single ID of target
	DPSDK_INT32 iType;										// Target type
	DPSDK_INT32 iDirect;									// Target direction
}DPSDK_ABSTRACT_OBJ_MODIFY_PARAM;

//Operation type
typedef enum
{
	DPSDK_OPERATE_IMMEDIATE_PROTECTION = 1,					// Arm now
	DPSDK_OPERATE_FORCE_PROTECTION = 2,						// Forced arm
	DPSDK_OPERATE_STOP_PROTECTION = 3,						// Disarm
	DPSDK_OPERATE_STOP_ALARM = 4,							// Stop alarm
	DPSDK_OPERATE_STOP_NORMAL = 5,							// Normal
	DPSDK_OPERATE_STOP_PL = 6,								// Bypass
	DPSDK_OPERATE_STOP_GL = 7,								// Isolation
}DPSDK_OPERATE_TYPE;

//Status type
typedef enum
{
	DPSDK_STATUS_PROTECTION = 1,							// Arm
	DPSDK_STATUS_UNPROTECTION = 2,							// Disarm
	DPSDK_STATUS_OFFLINE = 3,								// Offline
	DPSDK_STATUS_PARTIAL_PROTECTION = 4,					// Partial Arm
	DPSDK_STATUS_ALARM = 5,									// Alarm
}DPSDK_STATUS_TYPE;

// Alarm host operation parameter
typedef struct
{
	DPSDK_CHAR szDeviceCode[DPSDK_DEVICE_CODE_LEN];			// Device code
	DPSDK_INT32 iOperate;									// Operation type of alarm host. Refer to DPSDK_OPERATE_TYPE
}DPSDK_OPERATE_ALARMHOST_PARAM;

// Zone operation parameter
typedef struct
{
	DPSDK_INT32 iNum;										// Zone number
	DPSDK_CHANNEL_ID* pDefenceAreasId;						// Zone ID list
	DPSDK_INT32 iOperate;									// Zone operation type. Refer to DPSDK_OPERATE_TYPE
}DPSDK_DEFENCEAREAS_PARAM;

// Subsystem ID
typedef struct
{
	DPSDK_CHAR szSubsystemId[DPSDK_ID_LEN];					// Subsystem ID
}DPSDK_SUBSYSTEM_ID;

// Subsystem ID list
typedef struct
{
	DPSDK_INT32 iNum;										// Subsystem number
	DPSDK_SUBSYSTEM_ID struSubSystemIdList[1];				// Subsystem ID list
}DPSDK_SUBSYSTEM_ID_LIST;

//Zone movement parameter
typedef struct
{
	DPSDK_INT32 iNum;										// Zone number
	DPSDK_CHANNEL_ID* pDefenceAreasId;						// Zone ID list
	DPSDK_CHAR szSubSystemId[DPSDK_CHANNEL_ID_LEN];			// Subsystem ID. If it is null, the zone is moved out of subsystem.
}DPSDK_MOVE_DEFENCEAREAS_PARAM;

// Alarm host info details
typedef struct
{
	
	DPSDK_CHAR szDeviceName[DPSDK_DEVICE_NAME_LEN];			// Alarm host name
	//DPSDK_CHAR* pAlarmOutputControl;						// Alarm output control, with multiple channel no. such as 0, 1, 2
	DPSDK_CHAR szDeviceCode[DPSDK_DEVICE_CODE_LEN];			// Device code
	DPSDK_CHAR szSoftVersion[DPSDK_MEMO_LEN];				// Software version

	DPSDK_TIMET	tDeviceTime;								// Device time (timestamp)
	DPSDK_INT32 iAlarmInChannelCount;						// Number of alarm input channel, namely zone number
	DPSDK_INT32 iAlarmOutChannelCount;						// Number of alarm output channel
	DPSDK_INT32 iEncoderChannelCount;						// Number of video channel

	DPSDK_INT32 iAlarmStatus;								// Siren status, 0-disabled, 1-enabled
	DPSDK_INT32 iBatteryStatus;								// Storage battery status
	DPSDK_INT32 iPowerStatus;								// Power supply status
	DPSDK_INT32 iDeviceStatus;								// Alarm host status. Refer to DPSDK_STATUS_TYPE
	DPSDK_FLOAT fChargePercent;								// Electric quantity, 75%
	DPSDK_INT32 iNum;										// Number of alarm output control
	DPSDK_INT32* pAlarmOutputControl;						// Alarm output control, with multiple channel no. such as 0, 1, 2
}DPSDK_ALARMHOST_INFO;

// Zone info
typedef struct
{
	DPSDK_INT32 iDefenceAreasStatus;						// Zone status. Refer to DPSDK_STATUS_TYPE
	DPSDK_CHAR szDefenceAreasId[DPSDK_CHANNEL_ID_LEN]; ;	// Single ID of zone
	DPSDK_CHAR szChannelId[DPSDK_CHANNEL_ID_LEN];			// Channel ID
	DPSDK_CHAR szDefenceAreasName[DPSDK_DEVICE_NAME_LEN];	// Zone name
}DPSDK_DEFENCEAREAS_INFO;

// Subsystem info details
typedef struct
{
	DPSDK_CHAR szSubSystemId[DPSDK_CHANNEL_ID_LEN]; ;		// Single ID of subsystem
	DPSDK_INT32 iSubSystemStatus;							// Subsystem status. Refer to DPSDK_STATUS_TYPE
	DPSDK_CHAR szSubSystemName[DPSDK_DEVICE_NAME_LEN];		// Subsystem name
	DPSDK_INT32 iDefenceAreasNum;							// Zone number
	DPSDK_DEFENCEAREAS_INFO* pDefenceAreasList;				// Zone info list
}DPSDK_SUBSYSTEM_INFO;

// Zone and subsystem parameter
typedef struct
{
	DPSDK_INT32 iDefenceAreasNum;							// Zone number
	DPSDK_DEFENCEAREAS_INFO* pDefenceAreasList;				// Zone info list
	DPSDK_INT32 iSubSystemNum;								// Subsystem number
	DPSDK_SUBSYSTEM_INFO* pSubSystemList;					// Subsystem info list
}DPSDK_GET_INFO;

// Modify zone and sub-system info parameter
typedef struct
{
	DPSDK_INT32 iAlarmStatus;								// Siren status, 0-disabled, 1-enabled
	DPSDK_TIMET	tDeviceTime;								// Device time, format yyyy-MM-dd HH:mm:ss
	DPSDK_CHAR szDeviceCode[DPSDK_DEVICE_CODE_LEN];			// Device code
	DPSDK_INT32 iAlarmOutputNum;							// Number of alarm output channel
	DPSDK_INT32 iAlarmOutputControl[1];						// Alarm output control, with multiple channel no. 0,1,2
}DPSDK_UPDATE_INFO;

// Sub-system info parameter
typedef struct
{
	DPSDK_CHAR szSubSystemId[DPSDK_CHANNEL_ID_LEN]; ;		// Single ID of subsystem
	DPSDK_CHAR szSubSystemName[DPSDK_DEVICE_NAME_LEN];		// Subsystem name
}DPSDK_SUBSYSTEM_PARM;

//
typedef struct
{
	DPSDK_INT32 iStatus;									// Channel ID
	DPSDK_CHAR szChannelId[DPSDK_CHANNEL_ID_LEN];			// Status, 1-enabled, 0-disabled
}DPSDK_ALARMOUT_CONTROL;

//
typedef struct
{
	DPSDK_INT32 iAlarmStatus;								// Siren status, 0-disabled, 1-enabled
	DPSDK_CHAR szDevCode[DPSDK_DEVICE_CODE_LEN];			// Device code
}DPSDK_ALARM_STATUS;

// Access control
typedef struct
{
	DPSDK_CHAR szpKeyWord[DPSDK_MEMO_LEN];					// Search keywords, options
	DPSDK_CHAR szPersonName[DPSDK_NAME_LEN];				// Person name
	DPSDK_CHAR szDepartmentId[DPSDK_ID_LEN];				// Organization code ID
	DPSDK_CHAR szCardNo[DPSDK_CARD_NO_LEN];					// Card no.
	DPSDK_CHAR szPersonNo[DPSDK_PERSON_NO_LEN];				// Person no.

	DPSDK_TIMET tBeginTime;									// Start time, timestamp (unit: second), required
	DPSDK_TIMET tEndTime;									// End time, timestamp (unit: second), required
	DPSDK_INT32 iPage;										//Start query page, min. 1. It is 1 by default if not filled in.
	DPSDK_INT32 iPageSize;									// Number of records on every page. Min. 1, max. 256. It is 32 by default if not filled in.
	DPSDK_INT32 iEventType;									// Event type. Select all if not filled in. Optional.
	DPSDK_INT32 iSubEventType;								// Event sub-type
	DPSDK_INT32 iQueryType;									// 0:fuzzy query, 1:accurate query
	DPSDK_INT32 iChannelNum;								// Channel number
	DPSDK_CHANNEL_ID* pChannelIdList;						// Channel ID list
}DPSDK_ACCESSCONTROL_LOG;

typedef struct
{
	DPSDK_TIMET tTime;										// Occurrence time
	DPSDK_CHAR szCardNo[DPSDK_CARD_NO_LEN];					// Card no.
	DPSDK_CHAR szDeviceName[DPSDK_DEVICE_NAME_LEN];			// Device name
	DPSDK_CHAR szAlarmCode[DPSDK_ALARM_CODE_LEN];			// Alarm no.
	DPSDK_CHAR szChannelName[DPSDK_NAME_LEN];				// Channel name
	DPSDK_CHAR szChannelId[DPSDK_CHANNEL_ID_LEN];			// Channel ID
	DPSDK_CHAR szFirstName[DPSDK_NAME_LEN];					// Card first name	
	DPSDK_CHAR szLastName[DPSDK_NAME_LEN];					// Card last name
	DPSDK_CHAR szPersonId[DPSDK_PERSON_ID_LEN];				// Card user ID		
	DPSDK_INT32 iEventType;									// Event type
	DPSDK_INT32 iSubEventType;								// Event type
	DPSDK_INT32 iInOutStatus;								// In/out status
}DPSDK_ACCESSCONTROL_INFO;

typedef struct
{
	DPSDK_INT32 iNextPage;									// Next page; -1 means there is no next page.
	DPSDK_INT32 iTotalCount;								// Total records
	DPSDK_INT32 iNum;										// Record info number
	DPSDK_ACCESSCONTROL_INFO* pAccessControlList;			// Access control info list
}DPSDK_ACCESSCONTROL_LIST;

typedef struct
{
	DPSDK_CHAR szPersonId[DPSDK_PERSON_ID_LEN];				// Person ID
}DPSDK_PERSON_ID;

typedef struct
{
	DPSDK_INT32 iPersonNum;									// Person quantity
	DPSDK_PERSON_ID struPersonIdList[1];					// Person ID list
}DPSDK_PERSON_LIST;

typedef struct
{
	DPSDK_INT32 iChannelNum;
	DPSDK_CHANNEL_ID* pChannelList;
}DPSDK_ATTENDANCE_CHANNEL_PARAM;

typedef struct
{
	DPSDK_CHAR szChannelId[DPSDK_CHANNEL_ID_LEN];							// Channel id
	DPSDK_CHAR szChannelName[DPSDK_CHANNEL_NAME_LEN];					    // Channel name
}DPSDK_ATT_CHANNEL_INFO;

typedef struct
{
	DPSDK_INT32 iTotalCount;
	DPSDK_INT32 iChannelInfoNum;
	DPSDK_ATT_CHANNEL_INFO* pRelChannelInfo;
}DPSDK_GET_CHANNEL_RESULT;

typedef struct
{
	DPSDK_CHAR szStartTime[DPSDK_TIME_LEN];
	DPSDK_CHAR szEndTime[DPSDK_TIME_LEN];
	DPSDK_FLOAT fWorkHours;
	DPSDK_CHAR szSignInStartTime[DPSDK_TIME_LEN];
	DPSDK_CHAR szSignInEndTime[DPSDK_TIME_LEN];
	DPSDK_INT32 iMustSignIn;
	DPSDK_CHAR szSignOutStartTime[DPSDK_TIME_LEN];
	DPSDK_CHAR szSignOutEndTime[DPSDK_TIME_LEN];
	DPSDK_INT32 iMustSignOut;
}DPSDK_WORKATT_INFO;

typedef struct
{
	DPSDK_CHAR szIntervalId[DPSDK_ID_LEN];
	DPSDK_CHAR szName[DPSDK_NAME_LEN_EX];
	DPSDK_INT32 iAttendanceModel;
	DPSDK_INT32 iColor;
	DPSDK_INT32 iLater;
	DPSDK_INT32 iLaterAbsence;
	DPSDK_INT32 iEarly;
	DPSDK_INT32 iEarlyAbsence;
	DPSDK_INT32 iWorkOvertime;
	DPSDK_FLOAT fFreeWorkHours;
	DPSDK_INT32 iFreeWorkLimit;
	DPSDK_CHAR szFreeLimitTime[DPSDK_TIME_LEN];
	DPSDK_FLOAT fRecordWorkHours;
	DPSDK_INT32 iEnableOddInEvenOut;
	DPSDK_INT32 iSwingCardInterval;
	DPSDK_INT32 iWorkAttNum;
	DPSDK_WORKATT_INFO* pWorkAttInfo;
}DPSDK_TIMEINTERVAL_DATA;

typedef struct  
{
	DPSDK_INT32	iTimeIntervalNum;
	DPSDK_TIMEINTERVAL_DATA* pTimeIntervalList;	
}DPSDK_BATCH_TIMEINTERVAL_LIST;

typedef struct  
{
	DPSDK_CHAR szId[DPSDK_ID_LEN];	
}DPSDK_TIMEINTERVAL_ID;

typedef struct  
{
	DPSDK_INT32	iIdNum;
	DPSDK_TIMEINTERVAL_ID* pIntervalIdList;	
}DPSDK_TIMEINTERVAL_ID_LIST;

typedef struct  
{
	DPSDK_INT32	iIdNum;
	DPSDK_TIMEINTERVAL_ID* pIntervalIdList;	
}DPSDK_DEL_TIMEINTERVAL_PARAM;

typedef struct  
{
	DPSDK_CHAR szStartTime[DPSDK_TIME_LEN];
	DPSDK_CHAR szEndTime[DPSDK_TIME_LEN];
}DPSDK_FIXEDWORK_INFO;

typedef struct  
{
	DPSDK_CHAR szIntervalId[DPSDK_ID_LEN];
	DPSDK_CHAR szName[DPSDK_NAME_LEN_EX];
	DPSDK_INT32 iAttModel;
	DPSDK_INT32 iColor;
	DPSDK_INT32 iFixedWorkNum;
	DPSDK_FIXEDWORK_INFO* pFixedWorkList;
}DPSDK_TIMEINTERVAL_INFO;

typedef struct  
{
	DPSDK_INT32 iTotalCount;
	DPSDK_INT32 iNum;
	DPSDK_TIMEINTERVAL_INFO* pIntervalInfo;	
}DPSDK_GET_TIMEINTERVAL_LIST;

typedef struct  
{
	DPSDK_CHAR szId[DPSDK_ID_LEN];
	DPSDK_CHAR szName[DPSDK_NAME_LEN_EX];
	DPSDK_INT32 iLoopType;
	DPSDK_INT32 iLoopValue;
	DPSDK_INT32 iIntervalNum;
	DPSDK_TIMEINTERVAL_ID* pIntervalIdList;
}DPSDK_ATTENDANCEORDER_INFO;

typedef struct  
{
	DPSDK_INT32 iOrderNum;
	DPSDK_ATTENDANCEORDER_INFO* pOrderList;
}DPSDK_BATCH_ATTENDANCEORDER_LIST;

typedef struct
{
	DPSDK_CHAR szId[DPSDK_ID_LEN];
	DPSDK_CHAR szName[DPSDK_NAME_LEN_EX];
	DPSDK_INT32 iColor;
}DPSDK_INTERVAL_INFO;

typedef struct  
{
	DPSDK_CHAR szId[DPSDK_ID_LEN];
	DPSDK_CHAR szName[DPSDK_NAME_LEN_EX];
	DPSDK_INT32 iLoopType;
}DPSDK_ATTENDANCEORDER_DATA;

typedef struct  
{
	DPSDK_INT32 iTotalCount;
	DPSDK_INT32 iNum;
	DPSDK_ATTENDANCEORDER_DATA* pOrderInfo;	
}DPSDK_ATTENDANCEORDER_LIST;

typedef struct  
{
	DPSDK_CHAR szName[DPSDK_NAME_LEN_EX];
}DPSDK_ORDER_NAME;

typedef struct  
{
	DPSDK_INT32 iNum;
	DPSDK_ORDER_NAME* pFailedOrderList;	
}DPSDK_ATTENDANCE_ORDER_NAME_LIST;

typedef struct  
{
	DPSDK_CHAR szName[DPSDK_NAME_LEN_EX];
}DPSDK_TIMEINTERVAL_NAME;

typedef struct  
{
	DPSDK_INT32 iNum;
	DPSDK_TIMEINTERVAL_NAME* pFailedIntervalList;	
}DPSDK_ATTENDANCE_TIMEINTERVAL_NAME_LIST;

typedef struct
{
	DPSDK_CHAR szId[DPSDK_ID_LEN];
}DPSDK_ORDER_ID;

typedef struct  
{
	DPSDK_INT32 iIdNum;
	DPSDK_ORDER_ID* pOrgIdList;
}DPSDK_ORDER_ID_LIST;

typedef struct  
{
	DPSDK_CHAR szTempDate[DPSDK_TIME_LEN];
	DPSDK_CHAR szIntervalId[DPSDK_ID_LEN];
}DPSDK_SCHEDULE_TEMP_ORDER_INFO;

typedef struct  
{
	DPSDK_CHAR szStartDate[DPSDK_TIME_LEN];
	DPSDK_CHAR szEndDate[DPSDK_TIME_LEN];
	DPSDK_CHAR szOrderId[DPSDK_ID_LEN];
}DPSDK_STANDARD_SCHEDULE_ORDER_INFO;

typedef struct  
{
	DPSDK_INT32 iPersonIdNum;
	DPSDK_ID* pPersonIdList;
	DPSDK_INT32 iStandardOrderNum;
	DPSDK_STANDARD_SCHEDULE_ORDER_INFO* pStandardOrderList;
	DPSDK_INT32 iTempOrderNum;
	DPSDK_SCHEDULE_TEMP_ORDER_INFO* pTempOrderList;
}DPSDK_MODIFY_SCHEDULE_NOTIFY;

typedef struct  
{
	DPSDK_CHAR szPersonId[DPSDK_ID_LEN];
	DPSDK_INT32 iStandardOrderNum;
	DPSDK_STANDARD_SCHEDULE_ORDER_INFO* pStandardOrderList;
	DPSDK_INT32 iTempOrderNum;
	DPSDK_SCHEDULE_TEMP_ORDER_INFO* pTempOrderList;
}DPSDK_PERSON_SCHEDULE_INFO;

typedef struct  
{
	DPSDK_CHAR szDeptId[DPSDK_ID_LEN];
	DPSDK_INT32 iOrderNum;
	DPSDK_STANDARD_SCHEDULE_ORDER_INFO* pOrderList;
}DPSDK_DEPT_SCHEDULE_INFO;

typedef struct  
{
	DPSDK_INT32 iPersonIdNum;
	DPSDK_ID* pPersonIdList;
	DPSDK_INT32 iStandardOrderNum;
	DPSDK_STANDARD_SCHEDULE_ORDER_INFO* pStandardOrderList;
	DPSDK_INT32 iTempOrderNum;
	DPSDK_SCHEDULE_TEMP_ORDER_INFO* pTempOrderList;
}DPSDK_UPDATE_PERSON_SCHEDULE;

typedef struct  
{
	DPSDK_INT32 iDeptIdNum;
	DPSDK_ID* pDeptIdList;
	DPSDK_INT32 iStandardOrderNum;
	DPSDK_STANDARD_SCHEDULE_ORDER_INFO* pStandardOrderList;
}DPSDK_UPDATE_DEPT_SCHEDULE;

typedef struct  
{
	DPSDK_INT32 iStatisticsRule;
}DPSDK_STATISTICE_RULE_INFO;

typedef struct  
{
	DPSDK_INT32 iFileLen;									// File length
	DPSDK_CHAR* pPersonFile;								// Person file
}DSPDK_IMPORTPERSON_PARAM;

typedef struct  
{
	DPSDK_CHAR	szLocale[DPSDK_ID_LEN];
}DSPDK_TEMPLATEURL_PARAM;

typedef struct  
{
	DPSDK_CHAR	szTemplateUrl[DPSDK_URL_LEN];
}DSPDK_TEMPLATEURL_RESULT;

typedef struct  
{
	DPSDK_CHAR szSession[DPSDK_ID_LEN];
	DPSDK_INT32 iRepeatPersonNum;
	DPSDK_PERSON_ID* pRepeatPersonId;
	DPSDK_INT32 iIlleglePersonNum;
	DPSDK_PERSON_ID* pIlleglePersonId;
}DSPDK_IMPORTPERSON_RESULT;

typedef struct  
{
	DPSDK_CHAR szLocale[DPSDK_ID_LEN];
	DPSDK_INT32 iExportType;
	DPSDK_CHAR szDeptCode[DPSDK_ID_LEN];
}DSPDK_EXPORTPERSON_PARAM;

typedef struct  
{
	DPSDK_CHAR szFileUrl[DPSDK_URL_LEN];
	DPSDK_CHAR szSession[DPSDK_ID_LEN];
}DSPDK_EXPORTPERSON_RESULT;

typedef struct
{
	DPSDK_CHAR szDoorGroupId[DPSDK_PERSON_ID_LEN];			// Door group ID
}DPSDK_DOORGROUP_ID;

typedef struct
{
	DPSDK_INT32 iNum;										// Door group quantity
	DPSDK_DOORGROUP_ID struDoorGroupList[1];				// Door group ID list
}DPSDK_DOORGROUP_LIST;

typedef struct
{
	DPSDK_CHAR szDoorGroupId[DPSDK_CHANNEL_ID_LEN];			// Door group ID
	DPSDK_CHAR szFailedReason[DPSDK_MEMO_LEN];				// Failure reason
}DPSDK_ADD_DOORGROUP_PERSON;

// Door group info
typedef struct
{
	DPSDK_CHAR szDoorGroupName[DPSDK_DEVICE_NAME_LEN];		// Door group name
	DPSDK_CHAR szDoorGroupId[DPSDK_CHANNEL_ID_LEN];			// Door group ID
	DPSDK_CHAR szHolidayGroupId[DPSDK_CHANNEL_ID_LEN];		// Holiday group ID
	DPSDK_INT32 iTimeTemplateId;							// Time template ID
	DPSDK_INT32 iNum;										// Channel quantity
	DPSDK_CHANNEL_ID struChannelList[1];					// Channel list
}DPSDK_DOOR_GROUP_INFO;

typedef struct
{
	DPSDK_CHAR szDoorGroupName[DPSDK_DEVICE_NAME_LEN];		// Door group name
	DPSDK_INT32 iTimeTemplateId;							// Time template ID
	DPSDK_CHAR szDoorGroupId[DPSDK_CHANNEL_ID_LEN];			// Door group ID
	DPSDK_CHAR szHolidayGroupId[DPSDK_CHANNEL_ID_LEN];		// Holiday group ID
}DPSDK_DOOR_GROUP;

typedef struct
{
	DPSDK_CHAR szName[DPSDK_NAME_LEN];						// Name
}DPSDK_READER_NAME;

//Multi-door interlocking list
typedef struct  
{
	DPSDK_CHAR szInterDoorLockId[DPSDK_ID_LEN];             // Multi-door interlocking id
}DPSDK_DOOR_LOCK_ID;

// Door info
typedef struct
{
	DPSDK_TIMET	tDoorLockInterval;							// Lock hold time
	DPSDK_TIMET	tUnLockInterval;							// Unlock overtime, second
	DPSDK_INT32	iAlarmTypeNumber;							// Alarm type quantity
	DPSDK_INT32* pAlarmTypeList;							// Alarm type list (refer to Alarm_type_e)
	DPSDK_INT32 iUnlockMode;								// Open type: 0 card,1 password,2 fingerprint, 3 template Id, 4 Card + Password, 5 Card / Password / Fingerprint, 6 Card + Fingerprint
	DPSDK_INT32 iCloseTimeTemplateId;						// NC period
	DPSDK_INT32 iOpenTemplateId;							// NO period
	DPSDK_INT32 iPeriodOpenTemplateId;						// Open ID by period
	DPSDK_INT32 iHolidayTemplateId;							// holidayTemplateId, time template ID
	DPSDK_INT32 iDoorStatus;								// Door status, 0 normal, 1 NC, 2 NO
	DPSDK_INT32 iState;										// Issue status,0 not issued, 1 being issued, 2 issued successfully, 3 failed to issue,4 Failed to delete issue
	
	DPSDK_INT32 iReaderStatus;								// Reading head in/out, 0 means in and 1 means out.
	DPSDK_CHAR szChannelId[DPSDK_CHANNEL_ID_LEN];			//  Channel ID
	DPSDK_CHAR szChannelName[DPSDK_CHANNEL_NAME_LEN];		//  Channel name
	DPSDK_BOOL bSensorEnable;								// Door contact enable 1/0
	DPSDK_BOOL bEnable;										// Enable this door 1/0
	DPSDK_BOOL bAlarmEnable;								// Enable alarm 1/0
	DPSDK_BOOL bPeriodOpen;									// true£¬false, enable opening by period or not
	DPSDK_BOOL bLockTongueEnable;
	DPSDK_CHAR szInterDoorLockId[DPSDK_ID_LEN];             // Selected multi-door interlocking ID 
	DPSDK_BOOL bSuperPassWordEnable;
	DPSDK_CHAR szSuperPassWord[DPSDK_PWD_LEN];
	DPSDK_INT32 iNum;										// Multi-door interlocking ID quantity
	DPSDK_DOOR_LOCK_ID struInterDoorLockIdList[DPSDK_MAX_DOOR_LOCK_ID_NUM];               // Available multi-door interlocking ID list
}DPSDK_DOOR_INFO;

// Copy basic config of door
typedef struct
{
	DPSDK_CHAR szChannelId[DPSDK_CHANNEL_ID_LEN];			// Channel ID
	DPSDK_INT32 iNum;										// Target channel quantity
	DPSDK_CHANNEL_ID* pTargetChnIdList;						//  Target channel ID list
}DPSDK_DOOR_BASE_CONFIGURE;

typedef struct
{
	DPSDK_CHAR szChannelId[DPSDK_CHANNEL_ID_LEN];			// Channel ID
	DPSDK_INT32 iNum;										// Reading head quantity
	DPSDK_CHANNEL_ID* pReaderIdList;						// Reading head ID list
}DPSDK_DOOR_READER_INFO;

typedef struct
{
	DPSDK_INT32 iDoorReaderNum;								// Reading head quantity of door
	DPSDK_DOOR_READER_INFO* pDoorReaderList;				// Reading head info list of door
}DPSDK_DOOR_READER;

typedef struct
{
	DPSDK_INT32 iChannelInSeq;								// Alarm input channel S./N.
	DPSDK_INT32 iTime;
	DPSDK_INT32 iInStatus;									// Alarm input status,0 NC, 1 NO
	DPSDK_INT32 iDoorStatus;								// Alarm linkage door status,0 no operation, 1 auto, 2 open door,3 close door, 4 NO, 5 NC
	DPSDK_INT32 iOutNum;									// Alarm output channel quantity
	DPSDK_INT32* pChannelOutSeqList;						// Alarm output channel S./N. list
	DPSDK_INT32 iDoorNum;									// Door quantity
	DPSDK_INT32* pDoorSeqList;								// Channel no. list
}DPSDK_DEVICE_INOUTCONFIG;

//
typedef struct
{
	DPSDK_CHAR szDeviceCode[DPSDK_DEVICE_CODE_LEN];			// Device code
	DPSDK_INT32 iState;										// Issue status,0 not issued, 1 being issued, 2 issued successfully, 3 failed to issue,4 Failed to delete issue
	DPSDK_INT32	iDeviceNum;									// Device quantity
	DPSDK_DEVICE_INOUTCONFIG struDeviceInOutList[1];		// Device input/output config list
}DPSDK_DOOR_ALARM_INOUT;

// Door group list
typedef struct
{
	DPSDK_INT32 iTotalCount;								// Total records
	DPSDK_INT32 iNextPage;									// Next page; -1 means there is no next page.
	DPSDK_INT32 iDoorGroupNum;								// Door group quantity
	DPSDK_DOOR_GROUP struDoorGroupList[1];					// Door group info list
}DPSDK_DOOR_GROUP_LIST;

//
typedef struct
{
	DPSDK_INT32 iState;										// Issue status,0 not issued, 1 being issued, 2 issued successfully, 3 failed to issue,4 Failed to delete issue
	DPSDK_CHAR szDevCode[DPSDK_DEVICE_CODE_LEN];			// Device code
	DPSDK_INT32	iDeviceNum;									// Device quantity
	DPSDK_DEVICE_INOUTCONFIG* pDeviceInOutList;				// Device input/output config list
}DPSDK_GET_DOOR_ALARM_INOUT;

// Cancel door right info
typedef struct
{
	DPSDK_CHAR szPersonId[DPSDK_PERSON_ID_LEN];				// User ID
	DPSDK_INT32 iChannelNum;								// Channel quantity
	DPSDK_CHANNEL_ID* pChannelIdList;						//  Channel ID list
	DPSDK_INT32 iDoorGroupNum;								// Door group quantity
	DPSDK_CHANNEL_ID* pDoorGroupList;						//  Door group ID list
}DPSDK_DOOR_UNAUTHORIZE;

typedef struct
{
	DPSDK_CHAR szName[DPSDK_NAME_LEN];						// Organization name
	DPSDK_CHAR szParentCode[DPSDK_ORG_CODE_LEN];			// Parent node code
}DPSDK_ADD_DEPARTMENT;

typedef struct
{
	DPSDK_CHAR szDepartmentName[DPSDK_NAME_LEN];			// Organization name
	DPSDK_CHAR szDepartmentCode[DPSDK_ORG_CODE_LEN];		// Organization code
	DPSDK_CHAR szParentName[DPSDK_NAME_LEN];				// Parent organization name
	DPSDK_CHAR szParentCode[DPSDK_ORG_CODE_LEN];			// Parent node code
}DPSDK_UPDATE_DEPARTMENT;

// Remote verification id
typedef struct
{
	DPSDK_CHAR szId[DPSDK_ID_LEN];							// Remote verification ID
}DPSDK_REMOTE_VERIFICATION_ID;

// Delete remote verification
typedef struct
{
	DPSDK_INT32 iNum;										// Remote verification quantity
	DPSDK_REMOTE_VERIFICATION_ID struRemoteVerificationId[1];// Remote verification ID list
}DPSDK_DEL_REMOTE_VERIFICATION;

typedef struct
{
	DPSDK_CHAR szName[DPSDK_NAME_LEN];
}DPSDK_DELETE_INFO;

typedef struct
{
	DPSDK_INT32 iNum;
	DPSDK_DELETE_INFO* pFailedDeleteInfo;
}DPSDK_DEL_REMOTE_VERIFICATION_RESULT;

typedef struct
{
	DPSDK_CHAR szChannelCode[DPSDK_CHANNEL_ID_LEN];			// Channel no.
	DPSDK_CHAR szMemo[DPSDK_MEMO_LEN];						// Note
	DPSDK_INT32 iStatus;									// 0=ignore, 1=pass
}DPSDK_IGNORE_REMOTE_CHECK;

// Card id
typedef struct
{
	DPSDK_CHAR szId[DPSDK_ID_LEN];							// Card ID
}DPSDK_FIRST_CARD_ID;

// Delete card
typedef struct
{
	DPSDK_INT32 iNum;										// Card quantity
	DPSDK_FIRST_CARD_ID struFirstCardId[1];					// Card ID list
}DPSDK_DEL_FIRST_CARD;

typedef struct
{
	DPSDK_INT32 iNum;
	DPSDK_DELETE_INFO* pFailedDeleteInfo;
}DPSDK_DEL_FIRST_CARD_RESULT;

//
typedef struct
{
	DPSDK_CHAR szId[DPSDK_ID_LEN];							// Multi-card open ID
}DPSDK_MULTI_CARD_ID;

// Delete multi-card open
typedef struct
{
	DPSDK_INT32 iNum;										// Multi-card open quantity
	DPSDK_MULTI_CARD_ID struMultiCardId[1];					// Multi-card open ID list
}DPSDK_DEL_MULTI_CARD;

typedef struct
{
	DPSDK_INT32 iNum;
	DPSDK_DELETE_INFO* pFailedDeleteInfo;
}DPSDK_DEL_MULTI_CARD_RESULT;

//
typedef struct
{
	DPSDK_CHAR szId[DPSDK_ID_LEN];							// Multi-door interlocking ID
}DPSDK_INTER_DOOR_LOCK_ID;

// Delete multi-door interlocking config
typedef struct
{
	DPSDK_INT32 iNum;										// Multi-door interlocking quantity
	DPSDK_INTER_DOOR_LOCK_ID struInterDoorLockId[1];		// Multi-door interlocking ID list
}DPSDK_DEL_INTER_DOOR_LOCK;

typedef struct
{
	DPSDK_INT32 iNum;
	DPSDK_DELETE_INFO* pFailedDeleteInfo;
}DPSDK_DEL_INTER_DOOR_LOCK_RESULT;

//
typedef struct
{
	DPSDK_CHAR szId[DPSDK_ID_LEN];							// Anti-passback config ID
}DPSDK_ANTI_PASS_BACK_ID;

// Delete anti-passback config
typedef struct
{
	DPSDK_INT32 iNum;										// Anti-passback config quantity
	DPSDK_ANTI_PASS_BACK_ID struAntiPassBackId[1];			//Anti-passback config ID list
}DPSDK_DEL_ANTI_PASS_BACK;

typedef struct
{
	DPSDK_INT32 iNum;
	DPSDK_DELETE_INFO* pFailedDeleteInfo;
}DPSDK_DEL_ANTI_PASS_BACK_RESULT;

// Remote verification info
typedef struct
{
	DPSDK_BOOL bEnable;										// 
	DPSDK_INT32 iTemplateId;								//Template ID
	DPSDK_CHAR szRemoteVerificationId[DPSDK_ID_LEN];		// Remote verification ID
	DPSDK_CHAR szChannelId[DPSDK_CHANNEL_ID_LEN];			// Channel ID
	DPSDK_INT32 iState;										//  Issue status,0 not issued, 1 being issued, 2 issued successfully, 3 failed to issue,4 Failed to delete issue
	//DPSDK_CHAR szDeviceCode[DPSDK_DEVICE_CODE_LEN];			// Device code
}DPSDK_REMOTE_VERIFICATION_INFO;


// Remote verification list
typedef struct
{
	DPSDK_INT32 iNextPage;									// Next page quantity; -1 means there is no next page.
	DPSDK_INT32 iTotalCount;								// Total records
	DPSDK_INT32 iNum;										// Remote verification quantity
	DPSDK_REMOTE_VERIFICATION_INFO* pRemoteVerificationList;// Remote verification list
}DPSDK_REMOTE_VERIFICATION_LIST;

// Group info
typedef struct
{
	DPSDK_INT32 iCount;										// Card quantity
	DPSDK_INT32 iValidCount;								// Quantity of card to be verified
	DPSDK_INT32 iSort;										// Sequence
	DPSDK_CHAR szId[DPSDK_ID_LEN];							// Person ID
	DPSDK_INT32 iUnlockMode;								// Unlock mode
	DPSDK_CHAR szGroupName[DPSDK_NAME_LEN];
}DPSDK_GROUP_INFO;

// Multi-card info
typedef struct
{
	DPSDK_INT32 iTimeTemplateId;							// Time template ID
	DPSDK_INT32 iState;										// Issue status,0 not issued, 1 being issued, 2 issued successfully, 3 failed to issue,4 Failed to delete issue
	DPSDK_CHAR szMultiCardId[DPSDK_ID_LEN];					//  Multi-card ID
	DPSDK_CHAR szChannelId[DPSDK_ID_LEN];					// Door channel Id 
	DPSDK_BOOL bEnable;										// Enable multi-card open or not
	DPSDK_INT32 iNum;										// Group quantity
	DPSDK_GROUP_INFO* pGroupInfoList;						// Group info list
}DPSDK_MULTI_CARD_INFO;

// Multi-card info list
typedef struct
{
	DPSDK_INT32 iNextPage;									// Next page quantity; -1 means there is no next page.
	DPSDK_INT32 iTotalCount;								// Total records
	DPSDK_INT32 iNum;										// Card quantity
	DPSDK_MULTI_CARD_INFO* pMultiCardList;					// Multi-card info list
}DPSDK_MULTI_CARD_LIST;

// Group info
typedef struct
{
	DPSDK_INT32 iSort;										// Sequence
	DPSDK_INT32 iChannelNum;								// Channel quantity
	DPSDK_CHANNEL_ID* pChannelList;							// Door channel list
}DPSDK_GROUP_ID;

typedef enum
{
	DPSDK_DEVICE_TYPE_CONTROL = 0,							// 0 access controller
	DPSDK_DEVICE_TYPE_CENTER_CONTROL = 1,					// 1 access master controller
}DPSDK_DEVICE_TYPE;



// Multi-door interlocking list
typedef struct
{
	DPSDK_BOOL bEnable;										// Enable or not
	DPSDK_CHAR szInterDoorLockName[DPSDK_NAME_LEN];			// Multi-door interlocking name
	DPSDK_CHAR szDeviceCode[DPSDK_DEVICE_CODE_LEN];			// Device code
	DPSDK_CHAR szRemark[DPSDK_USER_REMARK_LEN];
	DPSDK_INT32 iState;										// Issue status,0 not issued, 1 being issued, 2 issued successfully, 3 failed to issue,4 Failed to delete issue
	DPSDK_INT32 iTimeTemplateId;							// Time template ID
	DPSDK_CHAR szInterDoorLockId[DPSDK_ID_LEN];				// Multi-door interlocking ID
	DPSDK_INT32 iGroupNum;									// Group quantity
	DPSDK_GROUP_ID* pGroupList;								// Group ID list
}DPSDK_INTERDOOR_LOCK_INFO;

// Obtain multi-door interlocking info
typedef struct
{
	DPSDK_INT32 iNextPage;									// Next page quantity; -1 means there is no next page.
	DPSDK_INT32 iTotalCount;								// Total records
	DPSDK_INT32 iNum;										// Door quantity
	DPSDK_INTERDOOR_LOCK_INFO* pInterDoorLockList;			// Multi-door interlocking info list
}DPSDK_GET_INTERDOOR_LOCK;


typedef struct
{
	DPSDK_CHAR szPersonId[DPSDK_PERSON_ID_LEN];
	DPSDK_CHAR szDepartmentId[DPSDK_ORG_CODE_LEN];
	DPSDK_CHAR szDepartmentName[DPSDK_ORG_NAME_LEN];
	DPSDK_CHAR szLastName[DPSDK_NAME_LEN];
	DPSDK_CHAR szFirstName[DPSDK_NAME_LEN];
}DPSDK_PERSON_DATA;

typedef struct
{
	DPSDK_INT32 iPersonNum;									// Person quantity
	DPSDK_PERSON_DATA struPersonList[1];					// Person list
}DPSDK_PERSON_DATA_LIST;

typedef struct
{
	DPSDK_INT32 iTimeTemplateId;							//  Time template ID
	DPSDK_CHAR szChannelId[DPSDK_ID_LEN];					// Door channel ID 
	DPSDK_CHAR szFirstCardId[DPSDK_ID_LEN];					// First-card open ID
	DPSDK_INT32 iDoorStatus;								// Door status, 2 normal,1 NO
	DPSDK_INT32 iState;										// Issue status,0 not issued, 1 being issued, 2 issued successfully, 3 failed to issue,4 Failed to delete issue
	DPSDK_BOOL bEnable;										// Enable first-card open or not
	DPSDK_INT32 iNum;										// Person quantity
	DPSDK_PERSON_DATA* pPersonList;							// Person list
}DPSDK_FIRST_CARD_INFO;

typedef struct
{
	DPSDK_INT32 iNextPage;									// Next page quantity; -1 means there is no next page.
	DPSDK_INT32 iTotalCount;								// Total records
	DPSDK_INT32 iNum;										// Record format
	DPSDK_FIRST_CARD_INFO* pGetFirstCardList;				// Obtain first-card info list
}DPSDK_GET_FIRST_CARD_LIST;

// Anti-passback config info list
typedef struct
{
	DPSDK_BOOL bEnable;										// Enable or not						
	DPSDK_CHAR szAPBName[DPSDK_NAME_LEN];					// Anti-passback config name
	DPSDK_CHAR szDeviceCode[DPSDK_DEVICE_CODE_LEN];			// Device code
	DPSDK_CHAR szAntiPassId[DPSDK_ID_LEN];					// Anti-passback config ID
	DPSDK_CHAR szRemark[DPSDK_USER_REMARK_LEN];
	DPSDK_INT32 iTimeTemplateId;							// Time template ID
	DPSDK_INT32 iState;										// Issue status,0 not issued, 1 being issued, 2 issued successfully, 3 failed to issue,4 Failed to delete issue
	DPSDK_INT32 iResetTime;									// Reset interval
	DPSDK_INT32 iGroupNum;									// Group quantity
	DPSDK_GROUP_ID* pGroupList;								// Group category
	DPSDK_INT32 iPersonNum;									// Person quantity
	DPSDK_PERSON_DATA* pRersonInfoList;						// Person list
}DPSDK_ANTI_PASSBACK_INFO;

// Anti-passback config info
typedef struct
{
	DPSDK_INT32 iNextPage;									// Next page quantity; -1 means there is no next page.
	DPSDK_INT32 iTotalCout;									// Total records
	DPSDK_INT32 iNum;										// Anti-passback config records
	DPSDK_ANTI_PASSBACK_INFO* pInterDoorLockList;			// Anti-passback config info list
}DPSDK_GET_ANTI_PASSBACK;

// Gender
typedef enum
{
	DPSDK_GENDER_FEMALE = 0,								// 0 female
	DPSDK_GENDER_MALE = 1,									// 1 male
}DPSDK_PERSON_GENDER;

// ID category
typedef enum
{
	DPSDK_ID_TYPE_ID = 0,									// 0 ID
	DPSDK_ID_TYPE_OFFICEID = 1,								// 1 officeId
	DPSDK_ID_TYPE_STUDENTID = 2,							// 2 studentId
	DPSDK_ID_TYPE_DRIVERLICENSE = 3,						// 3 dverLicense
	DPSDK_ID_TYPE_PADDPOR = 4,								// 4 passport
}DPSDK_ID_TYPE;

//All-in-one card category
typedef enum
{
	DPSDK_CARD_TYPE_GENNERRAL = 0,							// 0 GeneralCard
	DPSDK_CARD_TYPE_VIP = 1,								// 1 VIP
	DPSDK_CARD_TYPE_GUEST = 2,								// 2 Guest
	DPSDK_CARD_TYPE_PATROL = 3,								// 3 Patrol
	DPSDK_CARD_TYPE_BLACKLIST = 4,							// 4 blackList
	DPSDK_CARD_TYPE_DURESE = 5,								// 5 Duress
}DPSDK_CARD_TYPE;

typedef enum
{
	DPSDK_TYPE_ACCESSCONTROL = 1,							// 1 means access person
	DPSDK_TYPE_VIDEOTALK = 2,								// 2 means video intercom
}DPSDK_CARD_PERSON_TYPE;

typedef enum
{
	DPSDK_DEGREE_TYPE_UNSCHOOL = 0,							// 0 no education background
	DPSDK_DEGREE_TYPE_PRIMARY = 1,							// 1 primary school
	DPSDK_DEGREE_TYPE_JUNIOR = 2,							// 2 junior middle school
	DPSDK_DEGREE_TYPE_HIGH = 3,								// 3 senior middle school
	DPSDK_DEGREE_TYPE_BACHELOR = 4,							// 4 bachelor 
	DPSDK_DEGREE_TYPE_MASTER = 5,							// 5 master
	DPSDK_DEGREE_TYPE_DOCTER = 6,							// 6 doctor
	DPSDK_DEGREE_TYPE_PROFESSOR = 7,						// 7 professor
}DPSDK_DEGREE_TYPE;

typedef struct
{
	DPSDK_INT32 iTimeTemplateId;							// Time template ID
	DPSDK_INT32 iHoilidayGroupId;							// Holiday time template ID
	DPSDK_CHAR szIdNo[DPSDK_ID_LEN];						// ID no.
	DPSDK_CHAR szTel[DPSDK_PHONE_LEN];						// Telephone
	DPSDK_CHAR szEmail[DPSDK_ALARM_EMAILRECEIVER_LEN];		// Email
	DPSDK_CHAR szAddress[DPSDK_USER_REMARK_LEN];					// Address
	DPSDK_CHAR szBirthday[DPSDK_BIRTHDAY_LEN];				// Birthday
	DPSDK_CHAR szNationalityId[DPSDK_ID_LEN_EX];				// Refer to country code
	DPSDK_CHAR szRemark[DPSDK_USER_REMARK_LEN];					//
	DPSDK_CHAR szNickName[DPSDK_NAME_LEN];					// Nick Name
	DPSDK_TIMET tInitialTime;								// Timestamp start time
	DPSDK_TIMET tExpireTime;								// Timestamp overtime

	DPSDK_INT32 iUseTimes;									// Use time is 200 by default, which cannot be modified
	DPSDK_INT32 iDegree;									// Refer to DPSDK_DEGREE_TYPE
	DPSDK_INT32 iMaritalStatus;								// 0 confidential, 1 married 2 unmarried
	DPSDK_INT32 iIdType;									// For ID type, refer to DPSDK_ID_TYPE
}DPSDK_CARD_DETAIL_INFO;

typedef struct
{
	DPSDK_CHAR szDepartmentId[DPSDK_ORG_CODE_LEN];			// Organization code ID
	DPSDK_CHAR szRoomId[DPSDK_ORG_CODE_LEN];				// Room ID
	DPSDK_CHAR szId[DPSDK_ID_LEN];							// Database record ID uniqueness constraint
	DPSDK_CHAR szPersonId[DPSDK_ID_LEN];					// It cannot repeat verification return error. Decide person ID and room ID according to type
	DPSDK_CHAR szFirstName[DPSDK_NAME_LEN];					// First name
	DPSDK_CHAR szLastName[DPSDK_NAME_LEN];					// Last name. Pay attention to client translation,and deefine according to European and American habit.
	DPSDK_CHAR* pPicData;									// Picture data
	DPSDK_INT32 iPicLenght;									// Picture data size
	DPSDK_INT32 iType;										// 1 means access person,2 means video intercom. Refer to DPSDK_CARD_PERSON_TYPE
	DPSDK_INT32 iCardType;									// For all-in-one card type, refer to DPSDK_CARD_TYPE
	DPSDK_INT32 iGender;									// For gender, refer to DPSDK_PERSON_GENDER
	DPSDK_INT32 iStatus;									// 0: On the job, 1:quit
}DPSDK_CARD_BASE_INFO;

typedef struct
{
	DPSDK_CHAR szFingerPrintName[DPSDK_FINGER_PRINT_NAME];
	DPSDK_CHAR szFingerPrintId[DPSDK_FINGER_PRINT_LEN];		// Fingerprint code/600 digits
	//DPSDK_INT32 iPosition;									// 1 to 10 from left to right
	DPSDK_INT32 iMasterFlag;
	DPSDK_BOOL bThreaFlag;									// Is it duress fingerprint or not
}DPSDK_FINGER_PRINTS_INFO;

typedef struct
{
	DPSDK_CHAR szCardNo[DPSDK_CARD_NO_LEN];					// Card no.
	DPSDK_TIMET tIssueDate;									// Card-issuing time, utc time
	DPSDK_TIMET tChangeDate;								// Operation time
	DPSDK_INT32 iCardState;									// 0 frozen, 1 non-frozen
	DPSDK_INT32 iDuressFlag;
	DPSDK_INT32 iMasterFlag;
}DPSDK_CARDS_INFO;

typedef struct
{
	DPSDK_CHAR szPassWord[DPSDK_PWD_LEN];
	DPSDK_INT32 iCardNum;
	DPSDK_CARDS_INFO* pCardsInfo;
	DPSDK_INT32 iFingerPrintsNum;							// Fingerprint quantity
	DPSDK_FINGER_PRINTS_INFO* pFingerPrintsInfo;			// Fingerprint info list
}DPSDK_AUTHENTICATION;

typedef struct
{
	DPSDK_CHAR szCardNum[DPSDK_MEMO_LEN];					// Card no.
	DPSDK_CHAR szPersonId[DPSDK_MEMO_LEN];					// Person ID
}DPSDK_PERSON;

typedef struct
{
	DPSDK_INT32 iPersonNum;									// Person quantity
	DPSDK_PERSON* pPersonList;								// Person info list
	DPSDK_INT32 iChannelIdNum;								// Channel quantity
	DPSDK_CHANNEL_ID* pChannelIdList;						// Channel ID list
	DPSDK_INT32 iDoorGroupNum;								// Group quantity
	DPSDK_CHANNEL_ID* pDoorGroupIdList;						// Group ID list
}DPSDK_ACCESS_RIGHT;

typedef struct
{
	DPSDK_INT32 iPage;										// Start query page, min. 1. It is 1 by default if not filled in.
	DPSDK_INT32 iPageSize;									// Number of records on every page. Min. 1, max. 256. It is 32 by default if not filled in.
	DPSDK_INT32 iPersonNum;									// Person quantity
	DPSDK_PERSON_ID struPersonIdList[1];					// Person ID list
}DPSDK_GET_PERSONPIC;

typedef struct
{
	DPSDK_CHAR szPersonId[DPSDK_ID_LEN];					// Person ID
	DPSDK_INT32 iPicLength;									// Picture data size
	DPSDK_CHAR* pPic;										// Person picture data
}DPSDK_PERSONPIC_INFO;

typedef struct
{
	DPSDK_INT32 iNextPage;									// Next page quantity; -1 means there is no next page.
	DPSDK_INT32 iTotalCount;								// Total records
	DPSDK_INT32 iNum;										// Record info quantity
	DPSDK_PERSONPIC_INFO* pAccessControlList;				//  Person picture data list
}DPSDK_PERSONPIC_LIST;

// Card info
typedef struct
{
	DPSDK_CARD_DETAIL_INFO struDetailInfo;					// All-in-one card details
	DPSDK_CARD_BASE_INFO struBaseInfo;						// all-in-one card basic info
	DPSDK_AUTHENTICATION struAuthentication;				// Identity authentication
	DPSDK_ACCESS_RIGHT struAccessRight;						// Access right
}DPSDK_CARD_INFO;

//// Add all-in-one card user in batches
//typedef struct
//{
//	DPSDK_INT32 iCardPersonNum;								// All-in-one card user quantity
//	DPSDK_CARD_INFO* pAddCardPersonList;					// Add all-in-one card user list
//}DPSDK_CARD_PERSON_LIST;

typedef struct
{
	DPSDK_CHAR szCardNum[DPSDK_MEMO_LEN];					// Card no.
}DPSDK_CARD_NUM;

typedef struct
{
	DPSDK_INT32 iCardNum;									// Card quantity
	DPSDK_CARD_NUM* pCardNumList;							// Card no. list
	DPSDK_INT32 iPersonIdNum;                               // Person Id quantity
	DPSDK_PERSON_ID* pPersonIdList;							// Person Id list
}DPSDK_REPEAT_CARD_PERSON_LIST;

typedef struct
{
	DPSDK_CHAR szPersonId[DPSDK_PERSON_ID_LEN];				// Person ID
	DPSDK_INT32 iNum;										// Card quantity
	DPSDK_CARD_NUM* pCardNumList;							// Card no. list
}DPSDK_CARD_PERSON_PARAM;

typedef struct
{
	DPSDK_CHAR szDepartmentId[DPSDK_ORG_CODE_LEN];			// Organization ID
	DPSDK_TIMET tIntialTime;								// Start time
	DPSDK_TIMET tExpireTime;								// Overtime
	DPSDK_INT32 iCardType;									// Card type
	DPSDK_INT32 iPersonNum;									// Person quantity
	DPSDK_CARD_PERSON_PARAM* pPersonList;					// Person list
}DPSDK_CARD_PERSON_INFO;

// Add all-in-one card user in batches
typedef struct
{
	DPSDK_CARD_PERSON_INFO struCardPersonInfo;				// All-in-one card user info
	DPSDK_INT32 iChannelNum;								// Channel quantity
	DPSDK_CHANNEL_ID* pChannelIdList;						// Channel ID list
	DPSDK_INT32 iDoorGroupNum;								// Door group quantity
	DPSDK_CHANNEL_ID* pDoorGroupIdList;						// Door group ID list
}DPSDK_CARD_PERSON_LIST;

typedef struct
{
	DPSDK_CHAR szPersonId[DPSDK_PERSON_ID_LEN];				// User ID
	//DPSDK_CHAR szCardNo[DPSDK_CARD_NO_LEN];
}DPSDK_TASK_QUERY_INFO;

typedef struct
{
	DPSDK_INT32 iTaskNum;
	DPSDK_TASK_QUERY_INFO* pTaskQueryInfoList;
}DPSDK_GET_FAIL_TASK_PARAM;

// Person group info
typedef struct
{
	DPSDK_CHAR szPersonGroupName[DPSDK_NAME_LEN];			// Person group name
	DPSDK_CHAR szPersonGroupId[DPSDK_PERSON_ID_LEN];		// Person group ID
	DPSDK_CHAR szID[DPSDK_ID_LEN];
	DPSDK_INT32 iSyncDeviceFlag;
	DPSDK_INT32 iPersonNum;									// Person quantity
	DPSDK_PERSON_DATA* pPerson;								// Person list
}DPSDK_PERSON_GROUP;

typedef struct
{
	DPSDK_INT32 iNextPage;									//Next page quantity; -1 means there is no next page.
	DPSDK_INT32 iTotalCount;								// Total records
	DPSDK_INT32 iNum;										// Person quantity
	DPSDK_PERSON_GROUP* pPersonGroupList;
}DPSDK_PERSON_GROUP_LIST;

// Door right info
typedef struct
{
	DPSDK_CHAR szPersonId[DPSDK_PERSON_ID_LEN];				// User ID
	DPSDK_INT32 iChannelNum;								// Channel quantity
	DPSDK_CHANNEL_ID* pChannelIdList;						// Channel ID list
	DPSDK_INT32 iDoorGroupNum;								// Door group quantity
	DPSDK_CHANNEL_ID* pDoorGroupList;						// Door group ID list
}DPSDK_DOOR_AUTHORIZE;

// Organization node info
typedef struct DPSDK_DEPARTMENT_TREE_INFO_t
{
	DPSDK_CHAR szName[DPSDK_ORG_NAME_LEN];					// Organization node name
	DPSDK_CHAR szId[DPSDK_ORG_CODE_LEN];					// Organization node ID
	DPSDK_CHAR szParentName[DPSDK_NAME_LEN];				// Parent organization name
	DPSDK_CHAR szParentId[DPSDK_PERSON_ID_LEN];				// Parent organization ID
	DPSDK_INT32 iNum;										// Child organization node quantity
	DPSDK_DEPARTMENT_TREE_INFO_t* subDepartmenTreeList;		// Child organization node info list
}DPSDK_DEPARTMENT_TREE_INFO;

// Obtain all-in-one card info
typedef struct
{
	DPSDK_CHAR szDepartmentId[DPSDK_ORG_CODE_LEN];			// Organization code ID
	DPSDK_CHAR szId[DPSDK_ORG_CODE_LEN];					// Person ID
	DPSDK_INT32 iPage;										// Start query page, min. 1. It is 1 by default if not filled in.
	DPSDK_INT32 iPageSize;									// Number of records on every page. Min. 1, max. 256. It is 32 by default if not filled in.
	DPSDK_CHAR szKey[DPSDK_KEYWORD_LEN];					// Keyword
	DPSDK_CHAR szChannelId[DPSDK_CHANNEL_ID_LEN];			// Channel ID
	DPSDK_INT32 iNeedCardNum;								// 0:not need, 1:need
	DPSDK_ORDER_INFO struOrderInfo;
}DPSDK_GETCARD_PERSON_INFO;

// Card no.
typedef struct
{
	DPSDK_CHAR szCardNo[DPSDK_CARD_NO_LEN];					// Card no.
}DPSDK_CARD_NO;

// All-in-one card info
typedef struct
{
	DPSDK_CHAR szId[DPSDK_CHANNEL_ID_LEN];					// Database record ID uniqueness constraint
	DPSDK_CHAR szPersonId[DPSDK_PERSON_ID_LEN];				// It cannot repeat verification return error. Decide person ID and room ID according to type
	DPSDK_CHAR szFirstName[DPSDK_NAME_LEN];					// First name
	DPSDK_CHAR szLastName[DPSDK_NAME_LEN];					//  Last name. Pay attention to client translation,and deefine according to European and American habit.
	DPSDK_CHAR szDepartmentId[DPSDK_ORG_CODE_LEN];			// Organization code ID
	DPSDK_CHAR szRoomId[DPSDK_ID_LEN];						// Room ID
	DPSDK_INT32 iHasFinger;                                 // 1:has 0:has not

	//DPSDK_INT32 iType;										// 1 means access person,2 means video intercom. Refer to DPSDK_CARD_PERSON_TYPE
	DPSDK_INT32 iStatus;
	DPSDK_INT32 iCardType;									// All-in-one card type. Refer to DPSDK_CARD_TYPE.
	DPSDK_INT32 iGender;									// Gender. Refer to DPSDK_PERSON_GENDER.
	DPSDK_INT32 iCardNum;									// Card quantity
	DPSDK_CARD_NO* pCardNoList;								// Card no. type
}DPSDK_PERSONCARD_INFO;

// All-in-one card info list
typedef struct
{
	DPSDK_INT32 iTotalCount;								// Total records
	DPSDK_INT32 iNextPage;									// Next page quantity; -1 means there is no next page.
	DPSDK_INT32 iNum;										//  All-in-one card quantity
	DPSDK_PERSONCARD_INFO* pCardList;						//  All-in-one card info list
}DPSDK_CARD_LIST;

typedef struct
{
	DPSDK_CHAR szCardNo[DPSDK_CARD_NO_LEN];					// Card no.
	DPSDK_CHAR szDepartmentId[DPSDK_ORG_CODE_LEN];			// Organization code ID
	DPSDK_CHAR szPersonId[DPSDK_PERSON_ID_LEN];				// Person ID
	DPSDK_CHAR szPersonName[DPSDK_NAME_LEN];				// Person name
	DPSDK_CHAR szUnLockPassWord[DPSDK_PWD_LEN];				// Unlock password
	DPSDK_CHAR szCardPassWord[DPSDK_PWD_LEN];				// Card password
	DPSDK_CHAR szPicUrl[DPSDK_URL_LEN];						// Picture url

	DPSDK_TIMET tEndTime;									// End time
	DPSDK_TIMET tBeginTime;									// Start time

	DPSDK_INT32 iCardType;									// All-in-one card type. Refer to DPSDK_CARD_TYPE
	DPSDK_INT32 iUseTimes;									// Use time is 200 by default, which cannot be modified

}DPSDK_QUERY_CARD_BASE_INFO;

// Query all-in-one card info
typedef struct
{
	DPSDK_CARD_DETAIL_INFO struDetailInfo;					//  All-in-one card details
	DPSDK_QUERY_CARD_BASE_INFO struBaseInfo;				// All-in-one card basic info
	DPSDK_CHAR szFingerPrint[DPSDK_FINGER_PRINT_LEN];		// Fingerprint code
}DPSDK_QUERY_CARD_INFO;

typedef struct
{
	DPSDK_CHAR szDoorRuleId[DPSDK_ID_LEN];					// Door rule ID
}DPSDK_DOOR_RULE;

//
typedef struct
{
	DPSDK_INT32 iRuleNum;									// Door rule quantity
	DPSDK_DOOR_RULE struDoorRule[1];						// Door rule ID list
}DPSDK_DEL_DOOR_RULE;

typedef struct
{
	DPSDK_CHAR szRuleName[DPSDK_NAME_LEN];
	DPSDK_CHAR szRuleId[DPSDK_ID_LEN];
	DPSDK_CHAR szRemark[DPSDK_USER_REMARK_LEN];
	DPSDK_TIMET tTime;										// utc timestamp
	DPSDK_INT32	iPersonNum;									// Person quantity
	DPSDK_PERSON_DATA* pPersonList;							// Person ID list
	DPSDK_INT32 iChannelNum;								// Channel quantity
	DPSDK_CHANNEL_ID* pChannelIdList;						// Channel ID list
	DPSDK_INT32 iDoorGroupNum;								// Door group quantity
	DPSDK_CHANNEL_ID* pDoorGroupIdList;						// Door group ID list
}DPSDK_ACCESSCONTROL_RULE_INFO;

typedef struct
{
	DPSDK_CHAR szId[DPSDK_CHANNEL_ID_LEN];					// doorGroup Id
	DPSDK_CHAR szName[DPSDK_CHANNEL_NAME_LEN];				// doorGroup Name
}DPSDK_DOORGROUP_INFO;

typedef struct
{
	DPSDK_CHAR szRuleName[DPSDK_NAME_LEN];
	DPSDK_CHAR szRuleId[DPSDK_ID_LEN];
	DPSDK_CHAR szRemark[DPSDK_USER_REMARK_LEN];
	DPSDK_TIMET tTime;										// utc timestamp
	DPSDK_INT32	iPersonNum;									// Person quantity
	DPSDK_PERSON_DATA* pPersonList;							// Person ID list
	DPSDK_INT32 iChannelNum;								// Channel quantity
	DPSDK_CHANNEL_ID* pChannelIdList;						// Channel ID list
	DPSDK_INT32 iDoorGroupNum;								// Door group quantity
	DPSDK_DOORGROUP_INFO* pDoorGroupInfoList;				// Door group info list
}DPSDK_ACCESS_CONTROL_RULE_INFO;

//
typedef struct
{
	DPSDK_CHAR szRuleID[DPSDK_ID_LEN];
	DPSDK_INT32 iHasRulePersonIDNum;
	DPSDK_PERSON_ID* pHasRulePersonIDList;
}
DPSDK_ADD_RULE_RES;

//
typedef struct
{
	DPSDK_INT32 iHasRulePersonIDNum;
	DPSDK_PERSON_ID* pHasRulePersonIDList;
}
DPSDK_UPDATE_RULE_RES;

//
typedef struct
{
	DPSDK_INT32 iNextPage;									// Next page quantity; -1 means there is no next page.
	DPSDK_INT32 iTotalCount;								// Total records
	DPSDK_INT32	iNum;										// Record quantity
	DPSDK_ACCESS_CONTROL_RULE_INFO *pAccessControlRuleList;	// Access rule record info list
}DPSDK_ACCESSCONTRO_RULE_LIST;

//
typedef struct
{
	DPSDK_CHAR szHolidayId[DPSDK_ID_LEN];					// Holiday ID
}DPSDK_HOLIDAY_ID;

//
typedef struct
{
	DPSDK_CHAR szBeginTime[DPSDK_TIME_LEN];					// Start time, 02:00:00
	DPSDK_CHAR szEndTime[DPSDK_TIME_LEN];					// End time, 04:00:00
}DPSDK_PERIOD_INFO;

typedef struct
{
	DPSDK_CHAR szHolidayId[DPSDK_ID_LEN];					// Holiday ID
	DPSDK_CHAR szHolidayName[DPSDK_NAME_LEN];				// Holiday name
	//DPSDK_CHAR szDescription[DPSDK_MEMO_LEN];				// Holiday description
	DPSDK_TIMET tBeginTime;									// utc start date
	DPSDK_TIMET tEndTime;									// utc end time
	//DPSDK_INT32 iPeriodNum;									// Period quantity
	//DPSDK_PERIOD_INFO* pPeriod;								// Holiday period info list
}DPSDK_HOLIDAY_INFO;

typedef struct
{
	DPSDK_CHAR szHolidayGroupName[DPSDK_NAME_LEN];			// Holiday group name
	DPSDK_CHAR szHolidayGroupId[DPSDK_ID_LEN];				// Holiday group ID
	DPSDK_CHAR szDescription[DPSDK_MEMO_LEN];				// Holiday group description
	DPSDK_INT32 iHolidayNum;								// Holiday quantity
	DPSDK_HOLIDAY_ID* pHolidayId;							// Holiday ID list
	DPSDK_INT32 iPeriodNum;									// Period quantity
	DPSDK_PERIOD_INFO* pPeriod;								// Holiday period info list
}DPSDK_HOLIDAYGROUP_INFO;

typedef struct
{
	DPSDK_INT32 iHolidayNum;								//  Holiday quantity
	DPSDK_HOLIDAY_ID struHolidayId[1];						// Holiday ID list
}DPSDK_DEL_HOLIDAY;

typedef struct
{
	DPSDK_CHAR szId[DPSDK_ID_LEN];							// ID
	DPSDK_CHAR szName[DPSDK_NAME_LEN];						// Name
}DPSDK_PAGEDATA_INFO;

typedef struct
{
	DPSDK_INT32 iNextPage;									// Next page quantity; -1 means there is no next page.
	DPSDK_INT32 iTotalCout;									// Total records
	DPSDK_INT32 iNum;										// Record quantity
	DPSDK_PAGEDATA_INFO* pGetInfoList;						// Info list
}DPSDK_GET_INFO_LIST;

//Store codes
typedef struct
{
	DPSDK_CHAR szStoreCode[DPSDK_STORE_CODE_LEN];				//Store code
}DPSDK_STORE_CODE;

//Map info
typedef struct
{
	DPSDK_CHAR				szMapName[DPSDK_MAP_NAME_LEN];		//Map name
	DPSDK_CHAR				szMapId[DPSDK_MAP_ID_LEN];			//Map ID
}DPSDK_MAP_INFO;

//Store info
typedef struct
{
	DPSDK_INT32				iMapNum;							//Map quantity
	DPSDK_CHAR				szStoreName[DPSDK_STORENAME_LEN];	//Store name
	DPSDK_CHAR				szStoreCode[DPSDK_STORENAME_LEN];	//Store code
	DPSDK_MAP_INFO*			pMapList;							//Map list
}DPSDK_STORE_INFO;


// Store list
typedef struct
{
	DPSDK_INT32				iStoreNum;						//Store quantity
	DPSDK_STORE_INFO*		pStoreList;						//Store list
}DPSDK_STORE_LIST;

//Store codes list
typedef struct
{
	DPSDK_INT32 iStoreNum;									// Store quantity
	DPSDK_STORE_CODE *pStoreCode;							// Store codes list
}DPSDK_STORE_CODE_LIST;

// Store organization tree info
typedef struct
{
	DPSDK_CHAR				szOrgCode[DPSDK_ORG_CODE_LEN];	//Organization code
	DPSDK_BOOL				bContainStore;					//Inlcude store node or not: 0=excluding, 1= including
	DPSDK_BOOL				bChildStatus;					//Include child node: 0= level 1 child node, 1=all child nodes
}DPSDK_STOREORG_TREE_INFO;

typedef enum
{
	CALL_TYPE_SINGLE_CALL,								// Single call
	CALL_TYPE_GROUP_CALL,								// Group call
	CALL_TYPE_VT_CALL,									// Video intercom
}DPSDK_CALL_TYPE;

// Call service status
typedef enum
{
	CALL_STATUS_PREPARED,								//Prepare
	CALL_STATUS_REQTOSCS,								//Request SCS
	CALL_STATUS_CALLING,								//Calling
	CALL_STATUS_RECVING,								//Receiving
	CALL_STATUS_CEASED,									//Call release
}DPSDK_CALL_STATUS;

// Call request parameter
typedef struct
{
	DPSDK_CHAR szGroupId[DPSDK_ID_LEN];					//Group ID
	DPSDK_INT32 iCallType;								//Call type. Refer to DPSDK_CALL_TYPE definition.

	DPSDK_REALPLAY_PARAM struRealParam;					// Video parameter
}DPSDK_START_CALL_PARAM;

// Video intercom call request parameter
typedef struct
{
	DPSDK_CHAR szUserId[DPSDK_ID_LEN];					//User ID
	DPSDK_INT32 iCallType;								//Call type. Refer to DPSDK_CALL_TYPE definition.
	DPSDK_INT32 iAudioType;								//Audio type. Refer to DPSDK_AUDIO_TYPE definition.
	DPSDK_MEDIA_CALLBACK	struMediaCallBack;			// Video callback structure
}DPSDK_START_VTCALL_PARAM;

typedef struct
{
	DPSDK_CHAR szTransferNum[DPSDK_ID_LEN];				// Transfer number
}DPSDK_START_VTCALL_RES;

// Call request parameter
typedef struct
{
	DPSDK_CHAR szGroupId[DPSDK_ID_LEN];					//Group ID
	DPSDK_INT32 iCallType;								//Call type. Refer to DPSDK_CALL_TYPE definition.
	DPSDK_CHAR szCallerId[DPSDK_ID_LEN];
	DPSDK_CHAR szRtpServIP[DPSDK_IP_LEN];
	DPSDK_CHAR szSendChnlID[DPSDK_CHANNEL_ID_LEN];
	DPSDK_CHAR szRecvChnlID[DPSDK_CHANNEL_ID_LEN];
	DPSDK_CHAR szHostID[DPSDK_ID_LEN];
	DPSDK_INT32	iRtpPort;
	DPSDK_INT32	iAudioType;
	DPSDK_INT32	iAudioBit;
	DPSDK_INT32	iSampleRate;
	DPSDK_INT32	iCallId;
	DPSDK_INT32	iDlgId;
	DPSDK_INT32	iTid;

	DPSDK_REALPLAY_PARAM struRealParam;					// Video parameter
}DPSDK_INVITE_CALL_PARAM;

// Video intercom call request parameter
typedef struct
{
	DPSDK_CHAR szUserId[DPSDK_ID_LEN];					//User ID
	DPSDK_INT32 iCallType;								//Call type. Refer to DPSDK_CALL_TYPE definition.
	DPSDK_CHAR szRtpServIP[DPSDK_IP_LEN];
	DPSDK_INT32 iRtpAPort;
	DPSDK_INT32 iRtpVPort;
	DPSDK_INT32 iAudioType;
	DPSDK_INT32 iAudioBit;
	DPSDK_INT32 iSampleRate;
	DPSDK_INT32 iCallId;
	DPSDK_INT32 iDlgId;
	DPSDK_INT32 iTid;
	DPSDK_MEDIA_CALLBACK	struMediaCallBack;			// Video callback structure
}DPSDK_INVITE_VTCALL_PARAM;

// Refuse video intercom request parameter
typedef struct
{
	DPSDK_CHAR szUserId[DPSDK_ID_LEN];				//User ID
	DPSDK_INT32 iCallId;								
	DPSDK_INT32 iDlgId;	
	DPSDK_INT32 iTid;
}DPSDK_VTCALL_REJECT_PARAM;

// Cancel video intercom request parameter
typedef struct
{
	DPSDK_CHAR szUserId[DPSDK_ID_LEN];				//User ID
	DPSDK_INT32 iCallId;
	DPSDK_INT32 iDlgId;
}DPSDK_VTCALL_CANCEL_PARAM;

// Video intercom info release request parameter
typedef struct
{
	DPSDK_CHAR szMsgId[DPSDK_ID_LEN];					//User ID
	DPSDK_INT32 iReceiverNum;							//Info receiver quantity
	DPSDK_ID* pReceiverList;							//Info receiver list
}DPSDK_VTCALL_PUBLISH_PARAM;

// Call log ID
typedef struct
{
	DPSDK_CHAR szCallLogId[DPSDK_ID_LEN];					// Call log ID
}DPSDK_CALLLOG_ID;

typedef struct
{
	DPSDK_INT32 iCallLogNum;								// Call log quantity
	DPSDK_CALLLOG_ID struCallLogList[1];					// Call log ID list
}DPSDK_CALLLOG_ID_LIST;

typedef struct
{
	DPSDK_CHAR szId[DPSDK_ID_LEN];							// Major key ID
	DPSDK_CHAR szCallNum[DPSDK_PHONE_LEN];					// Call number
	DPSDK_CHAR szAddress[DPSDK_MEMO_LEN];					// Address
	DPSDK_CHAR szFormatCallTime[DPSDK_MEMO_LEN];			// Call date (Yesterday, Today ...)
	DPSDK_CHAR szFormatTalkTime[DPSDK_MEMO_LEN];			// Call duration (00:15)
	DPSDK_INT32 iTalkTime;									// Call duration
	//DPSDK_INT32 iEndStatus;									// Final status
	DPSDK_INT32 iCallType;									// Call type: (0: outbound, 1: inbound)
	DPSDK_INT32 iEndState;									// Final status (0-not received, 1-received)
	DPSDK_TIMET tCallTime;									// Call time (timestamp)
	DPSDK_CHAR szDeviceName[DPSDK_DEVICE_NAME_LEN];			// The name of device
}DPSDK_CALLLOG_INFO;

//Device code list
typedef struct
{
	DPSDK_INT32 iDevNum;									// Device quantity
	DPSDK_DEVICE_CODE struDevCodeList[1];					// Device code list
}DPSDK_DEVICE_CODE_LIST;

// Controller info
typedef struct
{
	DPSDK_CHAR szName[DPSDK_NAME_LEN];						// Name
	DPSDK_CHAR szId[DPSDK_CHANNEL_ID_LEN];					// S./N.
	DPSDK_INT32 iChannelNum;								// Channel quantity
	DPSDK_CHANNEL_ID* pChannelIdList;						// Channel ID list
}DPSDK_CONTROLLER_INFO;

// Device controller info
typedef struct
{
	DPSDK_CHAR szDeviceCode[DPSDK_DEVICE_CODE_LEN];			// Device code
	DPSDK_INT32 iControllerNum;								// Controller quantity
	DPSDK_CONTROLLER_INFO* pControllerList;					// Controller list
}DPSDK_DEV_CONTROLLER_INFO;

// Device controller list
typedef struct
{
	DPSDK_INT32 iNum;										//Device controller quantity
	DPSDK_DEV_CONTROLLER_INFO* pDevControllerList;			// Device controller info list
}DPSDK_DEV_CONTROLLER_LIST;

//Access status mode info
typedef struct  
{
	DPSDK_CHAR szChannelId[DPSDK_CHANNEL_ID_LEN];			// Channel ID
	DPSDK_INT32 iStatus;									// Status 0- normal, 1-NC, 2-NO
}DPSDK_ACCESS_CONTROL_STATUS_INFO;

//Access status mode list
typedef struct  
{
	DPSDK_INT32 iStatusNum;									// Access status mode quantity
	DPSDK_ACCESS_CONTROL_STATUS_INFO* pStatusList;			// Access status mode list
}DPSDK_ACCESS_CONTROL_STATUS_LIST;

typedef struct
{
	DPSDK_INT32 iPage;										// Start query page, min. 1. It is 1 by default if not filled in.
	DPSDK_INT32 iPageSize;									// Start query page, min. 1. It is 1 by default if not filled in.
	DPSDK_INT32 iDeviceNum;									// Call device quantity
	DPSDK_DEVICE_CODE* pDeviceCodeList;						// Call device code list
	DPSDK_TIMET tCallStartTime;								// Call period start time
	DPSDK_TIMET tCallEndTime;								// Call period end time
}DPSDK_CALLLOG_PARAM;

// Call parameter
typedef struct
{
	DPSDK_INT32 iUseNum;									// User quantity
	DPSDK_USER_ID* pUseIdList;								// User ID list
	DPSDK_INT32 iDeviceNum;									//Call device quantity
	DPSDK_DEVICE_CODE* pDeviceCodeList;						// Call device code list
}DPSDK_CALLNUMBER_PARAM;

// User call info
typedef struct
{
	DPSDK_CHAR szUserId[DPSDK_USER_ID_LEN];					// User ID
	DPSDK_CHAR szCallNumber[DPSDK_PHONE_LEN];				// Call number
}DPSDK_USER_CALLNUMBER_INFO;

// Device call info
typedef struct
{
	DPSDK_CHAR szDeviceCode[DPSDK_DEVICE_CODE_LEN];			// Device code
	DPSDK_CHAR szCallNumber[DPSDK_PHONE_LEN];				// Call number
}DPSDK_DEV_CALLNUMBER_INFO;

// Obtain call number
typedef struct
{
	DPSDK_INT32 iUserNum;									// User quantity
	DPSDK_USER_CALLNUMBER_INFO* pUserCallNumList;			// User call info list
	DPSDK_INT32 iDevNum;									// Device quantity
	DPSDK_DEV_CALLNUMBER_INFO* pDevCallNumList;				// Device call info list
}DPSDK_GET_CALLNUMBER_NUMBER;

typedef struct
{
	DPSDK_INT32 iTotalCount;								// Total records
	DPSDK_INT32 iNextPage;									// Next page quantity; -1 means there is no next page.
	DPSDK_INT32 iNum;										// Record quantity
	DPSDK_CALLLOG_INFO* pCallLogList;						// Call log list
}DPSDK_CALLLOG_LIST;

typedef struct
{
	DPSDK_CHAR szDevGroupId[DPSDK_ID_LEN];					//  Device group ID
}DPSDK_DEV_GROUP_ID;

typedef struct
{
	DPSDK_INT32 iDevGroupNum;								// Device group quantity
	DPSDK_DEV_GROUP_ID struDevGroupIdList[1];				//  Device group ID list
}DPSDK_DEV_GROUP_ID_LIST;

typedef struct
{
	DPSDK_CHAR szDevGroupName[DPSDK_NAME_LEN];				//  Device group name
	DPSDK_INT32 iDevNum;									// Device quantity
	DPSDK_DEVICE_CODE* pDevCodeList;						// Device code list
}DPSDK_DEV_GROUP;

typedef struct
{
	DPSDK_CHAR szDevCode[DPSDK_DEVICE_CODE_LEN];			// Device code
	DPSDK_CHAR szDevName[DPSDK_DEVICE_NAME_LEN];			// Device name
	DPSDK_CHAR szCallNum[DPSDK_PHONE_LEN];					// Call number
	DPSDK_INT32 iDevType;									// Device type
}DPSDK_DEV_MEM_INFO;

typedef struct
{
	DPSDK_CHAR szId[DPSDK_ID_LEN];							// Major key ID
	DPSDK_CHAR szDevGroupName[DPSDK_NAME_LEN];				// Device group name
	DPSDK_INT32 iNum;										// Device group member quantity
	DPSDK_DEV_MEM_INFO* pDevMemList;						// Device group member info list
}DPSDK_DEV_GROUP_INFO;

typedef struct
{
	DPSDK_INT32 iTotalCount;								// Total records
	DPSDK_INT32 iNextPage;									// Next page quantity; -1 means there is no next page.
	DPSDK_INT32 iNum;										// Record quantity
	DPSDK_DEV_GROUP_INFO* pDevGroupList;					// Device group info list
}DPSDK_DEV_GROUP_LIST;

typedef struct
{
	DPSDK_CHAR szGroupRelationId[DPSDK_ID_LEN];				// Group relation ID
}DPSDK_GROUP_RELATION_ID;

typedef struct
{
	DPSDK_CHAR szDevGroupId[DPSDK_ID_LEN];					// Device group major key ID
}DPSDK_DEVGROUP_ID;

typedef struct
{
	DPSDK_CHAR szGroupId[DPSDK_ID_LEN];						// Group major key ID
}DPSDK_MANAGER_GROUP_ID;

typedef struct
{
	DPSDK_CHAR szManagerGroupName[DPSDK_NAME_LEN];			// Management group name
	DPSDK_CHAR szManagerGroupId[DPSDK_ID_LEN];				// Management group major key ID
	DPSDK_INT32 iManagerGroupSort;							// Management group sequence
	DPSDK_INT32 iGroupType;									// Group type
}DPSDK_GROUP_RELATION_MEM;

typedef struct
{
	DPSDK_CHAR szRelationName[DPSDK_NAME_LEN];				// Group relation name
	DPSDK_INT32 iDevGroupNum;								// Device group quantity
	DPSDK_DEVGROUP_ID* pDevGroupIdList;						// Device major key ID list
	DPSDK_INT32	iManagerGroupNum;							// Management group quantity
	DPSDK_GROUP_RELATION_MEM* pManagerGroupList;			// Management group list
}DPSDK_GROUP_RELATION;

typedef struct
{
	DPSDK_INT32 iNum;										//  Group relation quantity
	DPSDK_GROUP_RELATION_ID struGroupRelationList[1];		//  Group relation ID list
}DPSDK_GROUP_RELATION_ID_LIST;

typedef struct
{
	DPSDK_CHAR szGroupId[DPSDK_ID_LEN];						// Group major key ID
	DPSDK_CHAR szGroupName[DPSDK_NAME_LEN];					// Group name
	DPSDK_INT32 iGroupType;									// Group type
	DPSDK_INT32 iGroupSort;									// Group sequence
}DPSDK_DEVICE_GROUP_MEM;

typedef struct
{
	DPSDK_CHAR szRelationName[DPSDK_NAME_LEN];				// Group relation name
	DPSDK_CHAR szId[DPSDK_ID_LEN];							// Major key ID
	DPSDK_INT32	iManagerGroupNum;							// Management group quantity
	DPSDK_GROUP_RELATION_MEM* pManagerGroupList;			//Management group list
	DPSDK_INT32 iNum;										// Device group quantity
	DPSDK_DEVICE_GROUP_MEM* pDeviceGroupList;				// Device group info list
}DPSDK_RELATION_GROUP_INFO;

typedef struct
{
	DPSDK_INT32 iTotalCount;								// Total records
	DPSDK_INT32 iNextPage;									// Next page quantity; -1 means there is no next page.
	DPSDK_INT32 iRelationGroupNum;							// Record quantity
	DPSDK_RELATION_GROUP_INFO* pRelationGroupList;			// Related group list
}DPSDK_RELATION_GROUP_LIST;

typedef struct
{
	DPSDK_CHAR szSubject[DPSDK_MEMO_LEN];					// Sending subject
	DPSDK_CHAR szContent[DPSDK_CONTENT_LEN];				// Sending content
	DPSDK_INT32 iInfoType;									// Sending info type
	DPSDK_INT32 iVthDevNum;									// VTH device quantity
	DPSDK_DEVICE_CODE* pVthDevCodeList;						// VTH device code list
}DPSDK_ADD_PUBLISH_INFO;

typedef struct
{
	DPSDK_CHAR szId[DPSDK_ID_LEN];							// Record ID
	DPSDK_CHAR szInfoId[DPSDK_ID_LEN];						// Info ID
	DPSDK_CHAR szRoomNum[DPSDK_ID_LEN];						// Room no.
	DPSDK_CHAR szReason[DPSDK_MEMO_LEN];					// Reason for sending failure
	DPSDK_INT32 iStatus;									// Status
	DPSDK_TIMET tUpdateTime;								// Update time
}DPSDK_FAIL_PUBLISH_INFO;

typedef struct
{
	DPSDK_CHAR szReason[DPSDK_MEMO_LEN];					// Reason for sending failure
	DPSDK_CHAR szVthDevCode[DPSDK_DEVICE_CODE_LEN];			//  VTH device code
	DPSDK_INT32 iStatus;									// Status
}DPSDK_VTHDEV_INFO;

typedef struct
{
	DPSDK_CHAR szId[DPSDK_ID_LEN];							// Major key ID
	DPSDK_CHAR szIssuerId[DPSDK_ID_LEN];					// Issuer ID
	DPSDK_CHAR szContent[DPSDK_CONTENT_LEN];				// Sending content
	DPSDK_CHAR szSubject[DPSDK_MEMO_LEN];					// Sending subject
	DPSDK_CHAR szInfoTypeName[DPSDK_NAME_LEN];				// Info category name
	DPSDK_INT32 iStatus;									// Status
	DPSDK_INT32 iInfoType;									// Info type: notice/announcement
	DPSDK_INT32 iSendStatus;								// Sending status details
	DPSDK_TIMET tCreatTime;									// Creation time
	DPSDK_INT32 ivthDevNum;									// VTH device quantity
	DPSDK_VTHDEV_INFO* pVthDevInfoList;						// VTH device info list
}DPSDK_PUBLISH_INFO;

typedef struct
{
	DPSDK_INT32 iTotalCount;								// Total records
	DPSDK_INT32 iNextPage;									// Next page quantity; -1 means there is no next page.
	DPSDK_INT32 iNum;										// Record quantity
	DPSDK_PUBLISH_INFO* pPublishInfoList;					// Issued info list
}DPSDK_PUBLISH_INFO_LIST;

typedef struct
{
	DPSDK_CHAR szPublishInfoId[DPSDK_ID_LEN];				//  Issued info ID
}DPSDK_PUBLISH_INFO_ID;

typedef struct
{
	DPSDK_INT32 iNum;										// Issued info quantity
	DPSDK_PUBLISH_INFO_ID struPublishInfoList[1];			// Issued info ID list
}DPSDK_PUBLISH_INFO_ID_LIST;

typedef struct
{
	DPSDK_CHAR szDevCode[DPSDK_DEVICE_CODE_LEN];			// Device code
	DPSDK_CHAR szId[DPSDK_ID_LEN];							// Major key ID
	DPSDK_CHAR szName[DPSDK_NAME_LEN];						// Device/user name
	DPSDK_INT32 iSort;										// Sequence
	DPSDK_CHAR szCallNum[DPSDK_PHONE_LEN];					// Call number
	DPSDK_INT32 iManagerType;								// Manager type (PC/mobile phone/management device)
}DPSDK_MANAGER_GROUP_MEM;

typedef struct  
{
	DPSDK_CHAR szOldMgrGroupId[DPSDK_ID_LEN];				// Original management group ID
	DPSDK_CHAR szNewMgrGroupId[DPSDK_ID_LEN];				// New management group ID
	DPSDK_INT32 iUserNum;									// User quantity
	DPSDK_PERSON_ID* pUserIdList;							// User ID list
	DPSDK_INT32 iDevNum;									// Device quantity
	DPSDK_DEVICE_CODE* pDevCodeList;						// Device code list
}DPSDK_MANAGER_GROUP_INFO;

typedef struct
{
	DPSDK_CHAR szManagerGroupId[DPSDK_ID_LEN];				// Management group ID
}DPSDK_MANAGE_GROUP_ID;

typedef struct
{
	DPSDK_INT32 iNum;										// Management group quantity
	DPSDK_MANAGE_GROUP_ID struMagerGroupList[1];			// Management group ID list
}DPSDK_MANAGE_GROUP_ID_LIST;

typedef struct
{
	DPSDK_CHAR szId[DPSDK_ID_LEN];							// Major key ID
	DPSDK_CHAR szManagerGroupName[DPSDK_NAME_LEN];			// Management group name
	DPSDK_INT32 iDevNum;									// Device quantity
	DPSDK_MANAGER_GROUP_MEM* pDevInfoList;					// Device info list
	DPSDK_INT32 iUserNum;									// User quantity
	DPSDK_MANAGER_GROUP_MEM* pUserInfoList;					// User info list
}DPSDK_GET_MANAGER_GROUP_INFO;

typedef struct
{
	DPSDK_CHAR szManagerGroupName[DPSDK_NAME_LEN];			// Management group name
	DPSDK_INT32 iDevNum;									// Device quantity
	DPSDK_MANAGER_GROUP_MEM* pDevInfoList;					// Device info list
	DPSDK_INT32 iUserNum;									// User quantity
	DPSDK_MANAGER_GROUP_MEM* pUserInfoList;					// User info list
}DPSDK_ADD_MANAGER_GROUP_INFO;

typedef struct
{
	DPSDK_INT32 iTotalCount;								// Total records
	DPSDK_INT32 iNextPage;									// Next page quantity; -1 means there is no next page.
	DPSDK_INT32 iNum;										// Record quantity
	DPSDK_GET_MANAGER_GROUP_INFO* pManagerGroupList;		// Management group info list
}DPSDK_MANAGER_GROUP_INFO_LIST;

// VTH device code
typedef struct
{
	DPSDK_CHAR szVthDevCode[DPSDK_DEVICE_CODE_LEN];			// VTH device code
	DPSDK_CHAR szNickName[DPSDK_NAME_LEN];					// 
	DPSDK_CHAR szPersonID[DPSDK_PERSON_ID_LEN];             // Person Id
	DPSDK_CHAR szFirstName[DPSDK_NAME_LEN];					// First name
	DPSDK_CHAR szLastName[DPSDK_NAME_LEN];					// Last name
}DPSDK_VTH_DEV_INFO;

// VTH device code
typedef struct
{
	DPSDK_INT32 iVthDevNum;									// VTH device quantity
	DPSDK_VTH_DEV_INFO* pVthDevInfo;						// VTH device info
}DPSDK_VTH_DEV_LIST;

// 	Issue contacts to VTO
typedef struct
{
	DPSDK_CHAR szVtoDevCode[DPSDK_DEVICE_CODE_LEN];			// VTO device code
	DPSDK_INT32 iVthDevNum;									// VTH device quantity
	DPSDK_VTH_DEV_INFO* pVthDevInfo;						// VTH device info
}DPSDK_VTO_DEV_PARAM;

// Map log info
typedef struct
{
	DPSDK_TIMET tTime;										// Occurrence time
	DPSDK_CHAR szCardNum[DPSDK_MEMO_LEN];					// Card no.
	DPSDK_CHAR szDeviceName[DPSDK_NAME_LEN];				// Device name
	DPSDK_CHAR szChannelName[DPSDK_NAME_LEN];				// Channel name
	DPSDK_CHAR szChannelId[DPSDK_CHANNEL_ID_LEN];			// Channel ID
	DPSDK_CHAR szPersonId[DPSDK_PERSON_ID_LEN];				// Person ID
	DPSDK_CHAR szAlarmCode[DPSDK_ALARM_ALARMCODE_LEN];		// Alarm no.
	DPSDK_CHAR szMapId[DPSDK_MAP_ID_LEN];					// Map ID
	DPSDK_INT32 iInOutStatus;								// In/out status
	DPSDK_DOUBLE fGpsX;										// Longitude
	DPSDK_DOUBLE fGpsY;										// Latitude
}DPSDK_MAP_LOG_INFO;

// Map log info list
typedef struct
{
	DPSDK_INT32 iNextPage;									 //Next page quantity; -1 means there is no next page.
	DPSDK_INT32 iTotalCount;								 //Total records
	DPSDK_INT32 iNum;										 //Map log info record quantity
	DPSDK_MAP_LOG_INFO* pMapInfoList;						 //Map log info list
}DPSDK_MAP_LOG_LIST;

// Access map log query parameter
typedef struct
{
	DPSDK_INT32 iPage;										// Start query page, min. 1. It is 1 by default if not filled in.
	DPSDK_INT32 iPageSize;									// Number of records on every page. Min. 1, max. 256. It is 32 by default if not filled in.
	DPSDK_CHAR szCardNum[DPSDK_MEMO_LEN];					// Card no.
	DPSDK_CHAR szPersonId[DPSDK_PERSON_ID_LEN];				// Person ID
	DPSDK_TIMET tBeginTime;									// Start time, timestamp (unit: second), required
	DPSDK_TIMET tEndTime;									// End time, timestamp (unit: second)
}DPSDK_MAP_LOG_PARAM;

typedef struct
{
	DPSDK_CHAR szFingerPrintName[DPSDK_FINGER_PRINT_NAME];
}DPSDK_FINGER_PRINT_POSITION_INFO;

//Failed task info
typedef struct  
{
	//DPSDK_CHAR szRecordId[DPSDK_MEMO_LEN];					// Failed task ID
	//DPSDK_CHAR szGroupId[DPSDK_USER_ID_LEN];				// Door group ID. Null string means this device is ticked separately.
	DPSDK_CHAR szPersonIdInfo[DPSDK_PERSON_ID_LEN];			// PersonId Info
	DPSDK_CHAR szFacePicInfo[DPSDK_TYPE_LEN];				// FacePic Info
	DPSDK_INT32 iFingerPrintInfoNum;
	DPSDK_FINGER_PRINT_POSITION_INFO* pFingerPrintInfoList;
	DPSDK_INT32 iCardNum;									// Card quantity
	DPSDK_CARD_NO* pCardNoList;								// Card no. category
	DPSDK_INT32 iChnNum;									// Channel quantity
	DPSDK_CHANNEL_ID* pChannelIdList;						// Channel ID list
}DPSDK_TASK_FAIL_INFO;

// 
typedef struct  
{
	//DPSDK_CHAR szRuleId[DPSDK_ID_LEN];						// Rule ID
	DPSDK_CHAR szPersonId[DPSDK_PERSON_ID_LEN];				// Person ID
	//DPSDK_CHAR szCardNum[DPSDK_MEMO_LEN];					// Card no.
	DPSDK_INT32 iNum;										// Failed task quantity
	DPSDK_TASK_FAIL_INFO* pFailTaskList;					// Failed task info list
}DPSDK_ISSUE_PERSONCARD_TASK_FAIL;

// 
typedef struct
{
	DPSDK_INT32 iFailNum;									// Failed task quantity
	DPSDK_ISSUE_PERSONCARD_TASK_FAIL* pFailTaskFailList;	// Failed task info list
}DPSDK_ISSUE_PERSONCARD_TASK_FAIL_LIST;

typedef struct
{
	DPSDK_CHAR szPersonId[DPSDK_PERSON_ID_LEN];
}DPSDK_ISSUE_FAIL_TASK;

typedef struct
{
	DPSDK_INT32 iFailNum;									// Failed task quantity
	DPSDK_ISSUE_FAIL_TASK* pFailTaskList;	// Failed task info list
}DPSDK_ISSUE_FAIL_TASKS;

typedef struct
{
	DPSDK_CHAR szRuleId[DPSDK_ID_LEN];						// Rule ID
}DPSDK_ISSUE_RULE_FAIL_TASK;

typedef struct
{
	DPSDK_INT32 iFailNum;									// Failed task quantity
	DPSDK_ISSUE_RULE_FAIL_TASK* pFailTaskList;	// Failed task info list
}DPSDK_ISSUE_RULE_FAIL_TASKS;

//Import video intercom device parameter
typedef struct  
{
	DPSDK_INT32 iMode;										// Import mode: 0- ordinary import,with prompt of the repeatedly imported device; 1-forced import"
	DPSDK_INT32 iFileLen;									// File length
	DPSDK_CHAR* pDeviceFile;								// AES encryption file
}DSPDK_VIDEOINTERCOM_PARAM;

//Import video intercom device result
typedef struct
{
	DPSDK_INT32 iSuccessCount;								// Successful quantity
	DPSDK_INT32 iFailCount;									// Failed quantity
	DPSDK_INT32 iAddCount;									// Add quantity
	DPSDK_INT32 iUpdateCount;								// Update quantity
}DSPDK_IMPORT_RESULT;

typedef struct
{
	DPSDK_INT32	iSourceType;								//Record source
	DPSDK_INT32 iOwnerId;									//Operator ID
	DPSDK_CHAR	szCameraId[DPSDK_CHANNEL_ID_LEN];			//Camera ID
	DPSDK_CHAR	szStartTime[DPSDK_TIME_STRING_LEN];			//Start time of locked record
	DPSDK_CHAR	szStopTime[DPSDK_TIME_STRING_LEN];			//End time of locked record
	DPSDK_CHAR	szOperTime[DPSDK_TIME_STRING_LEN];			//Operating time
	DPSDK_CHAR	szOwnerIp[DPSDK_IP_LEN];					//Operator Ip
	DPSDK_CHAR	szOwnerName[DPSDK_NAME_LEN];				//Operator name
	DPSDK_CHAR	szReason[DPSDK_MEMO_LEN];					//Operating cause
	DPSDK_CHAR	szDBId[DPSDK_DB_ID_LEN];					//DB ID
}DPSDK_RECORD_LOCK_NOTIFY;

typedef struct
{
	DPSDK_CHAR	szDBId[DPSDK_DB_ID_LEN];
} DPSDK_DB_ID;

typedef struct  
{
	DPSDK_INT32		iCount;
	DPSDK_DB_ID*	pDBIds;
}DPSDK_RECORD_UNLOCK_NOTIFY;

typedef struct
{
	DPSDK_UINT32	uTagId;
	DPSDK_TIMET		tTagTime;
	DPSDK_UINT32	uOwnerId;
	DPSDK_TIMET		tOwnerTime;
	DPSDK_INT32		iRecordSource;
	DPSDK_CHAR		szCameraId[DPSDK_CHANNEL_ID_LEN];
	DPSDK_CHAR		szSubject[DPSDK_SUBJECT_LEN];
	DPSDK_CHAR		szContent[DPSDK_MEMO_LEN];
	DPSDK_CHAR		szURL[DPSDK_URL_LEN];
}DPSDK_ADD_RECORD_TAG_NOTIFY;

typedef struct
{
	DPSDK_INT32		iCount;
	DPSDK_UINT32*	pDBIds;
}DPSDK_DELETE_RECORD_TAG_NOTIFY;

typedef struct  
{
	DPSDK_UINT64	uiDownloadSize;
	DPSDK_UINT32	uiDownloadSpeed;
}DPSDK_DOWNLOAD_PROGRESS_IFNO;

typedef struct  
{
	DPSDK_CHAR		szFilePath[DPSDK_FILE_PATH_LEN];
	DPSDK_UINT64	uiFileSize;
}DPSDK_DOWNLOAD_CUT_FILE_INFO;

typedef struct  
{
	DPSDK_INT32		iEventType;
	DPSDK_INT32		iResult;
	DPSDK_INT32		iDownloadID;
	DPSDK_CHAR		szChannelID[DPSDK_CHANNEL_ID_LEN];
	DPSDK_DOWNLOAD_PROGRESS_IFNO	struProgress;
	DPSDK_DOWNLOAD_CUT_FILE_INFO	struCutFile;
}DPSDK_DOWNLOAD_NOTIFY;

typedef struct
{
	DPSDK_INT32		iCount;
	DPSDK_UINT32*	pTimeTemplateId;
}DPSDK_DELETE_TIMETEMPLATE_NOTIFY;

typedef struct
{
	DPSDK_INT32		iCount;
	DPSDK_UINT32*	pPlanId;
}DPSDK_DELETE_STORAGEPLAN_NOTIFY;

typedef struct 
{
	DPSDK_CHAR		szChannelID[DPSDK_CHANNEL_ID_LEN];
}DPSDK_ALARM_PLAN_NOTIFY;

typedef struct
{
	DPSDK_INT32 iBufLen;
	DPSDK_CHAR* pData;
}DPSDK_JSON_DATA_NOTIFY;

typedef struct
{
	DPSDK_CHAR szServerIP[DPSDK_IP_LEN];							// Server IP address
	DPSDK_CHAR szIP[DPSDK_IP_LEN];									// Network disc IP
}DPSDK_DELETE_NETDISK_NOTIFY;


typedef struct
{
	DPSDK_CHAR szServerName[DPSDK_NAME_LEN];						// Server name
	DPSDK_CHAR szVolumeName[DPSDK_NAME_LEN];						// Volume name
	DPSDK_CHAR szIP[DPSDK_IP_LEN];									// Network disc IP
	DPSDK_CHAR szServerIP[DPSDK_IP_LEN];							// Server IP address
	DPSDK_INT32 iServerPort;										// Server port
	DPSDK_INT32 iDiskType;											// Disc type. Refer to DPSDK_DISK_TYPE definition.
	DPSDK_CHAR szPath[DPSDK_FILE_PATH_LEN];							// Disc path
	DPSDK_INT32 iFileSystemStatus;									// File system status. Refer to DPSDK_FILESYSTEM_STATUS definition.OK: normal,Other: abnormal
	DPSDK_DOUBLE fCapacity;											// Capacity
	DPSDK_DOUBLE fFreeSpace;										// Surplus capacity
	DPSDK_BOOL bFormat;												// Format or not
	DPSDK_BOOL bBelong;												// Does it belong to
}DPSDK_SINGLE_NETDISK_NOTIFY_INFO;

typedef struct
{
	DPSDK_INT32 iNetDiskNum;										// Network disc format
	DPSDK_SINGLE_NETDISK_NOTIFY_INFO* pNetDiskList;					// Network disc size
}DPSDK_ADD_NETDISK_NOTIFY;

typedef struct
{
	DPSDK_INT32 iNetDiskNum;										// Network disc format
	DPSDK_SINGLE_NETDISK_NOTIFY_INFO* pNetDiskList;					// Network disc size
}DPSDK_MODIFY_NETDISK_NOTIFY;

typedef struct
{
	DPSDK_INT32 iDiskType;											// Disc type. Refer to DPSDK_DISK_TYPE definition.
	DPSDK_CHAR szPath[DPSDK_FILE_PATH_LEN];							// Disc path
	DPSDK_UINT64 uiCapacity;										// Capacity
}DPSDK_ADD_VIRTUALDISK_NOTIFY;

typedef struct
{
	DPSDK_CHAR szPath[DPSDK_FILE_PATH_LEN];							// Disc path
}DPSDK_DELETE_VIRTUALDISK_NOTIFY;

typedef struct
{
	DPSDK_CHAR szUserId[DPSDK_USER_ID_LEN];							// User ID
}DPSDK_MODIFY_USER_NOTIFY;

typedef struct
{
	DPSDK_INT32 iTVWallId;											// TV wall ID
	DPSDK_INT32 iStatus;											// TV wall status: 0.disabled; 1. enabled
}DPSDK_TVWALL_STATUS_NOTIFY;

typedef struct
{
	DPSDK_CHAR szServiceName[DPSDK_NAME_LEN];						// Server name
	DPSDK_INT32 iServerPort;										// Server port
	DPSDK_INT32 iStatus;											// Server status: 0.disabled; 1. enabled
}DPSDK_MODIFY_SERVICEPORT_NOTIFY;

typedef struct
{
	DPSDK_CHAR szRegisterIp[DPSDK_IP_LEN];							// Registration Ip
	DPSDK_INT32 iRegisterPort;										// Registration port
	DPSDK_CHAR szMasterIp[DPSDK_IP_LEN];							// Host Ip
	DPSDK_INT32 iMasterPort;										// Host port
	DPSDK_CHAR szMappingIp[DPSDK_IP_LEN];							// Mapping Ip
	DPSDK_BOOL bMaster;												//Is it main mode, true/false
}DPSDK_MODIFY_SERVICEREGISTER_NOTIFY;

typedef struct
{
	DPSDK_CHAR szDeviceCode[DPSDK_DEVICE_CODE_LEN];					// Device code
	DPSDK_INT32 iStatus;											// Device status
}DPSDK_DEVICE_SEARCH_STATUS_NOTIFY;

typedef struct
{
	DPSDK_CHAR szChannelId[DPSDK_CHANNEL_ID_LEN];					// Channel ID
}DPSDK_COPY_ALARM_PLAN_NOTIFY;

typedef struct
{
	DPSDK_CHAR szGroupId[DPSDK_USER_ID_LEN];						// Group ID
	DPSDK_INT32	iOrgNum;											// Node quantity	
	DPSDK_ORG_CODE* pOrgCodeList;									// Node code list
	DPSDK_INT32	iChannelNum;										// Channel quantity	
	DPSDK_CHANNEL_ID* pChannelList;									// Channel ID list
}DPSDK_GROUP_RIGHTS_INFO;

typedef struct
{
	DPSDK_CHAR szStoreCode[DPSDK_STORE_CODE_LEN];				//Store code
	DPSDK_CHAR szStoreName[DPSDK_STORE_NAME_LEN];				//Store name
	DPSDK_CHAR szStoreOrgId[DPSDK_STORE_ORGID_LEN];				//Store organization ID
	DPSDK_CHAR szStoreOrgCode[DPSDK_ORG_CODE_LEN];				//Store organization code
	DPSDK_CHAR szStoreOrgOldCode[DPSDK_ORG_CODE_LEN];			//Old store organization code
	DPSDK_INT32	iMapNum;										//Map quantity
	DPSDK_MAP_INFO*	pMapList;									//Map info list
}DPSDK_ADD_STORE_NOTIFY;

typedef struct
{
	DPSDK_CHAR szStoreCode[DPSDK_STORE_CODE_LEN];				//Store code
	DPSDK_CHAR szStoreName[DPSDK_STORE_NAME_LEN];				//Store name
	DPSDK_CHAR szStoreOrgId[DPSDK_STORE_ORGID_LEN];				//Store organization ID
	DPSDK_CHAR szStoreOrgCode[DPSDK_ORG_CODE_LEN];				//Store organization code
	DPSDK_CHAR szStoreOrgOldCode[DPSDK_ORG_CODE_LEN];			//Old store organization code
	DPSDK_INT32	iMapNum;										//Map quantity
	DPSDK_MAP_INFO*	pMapList;									//Map info list
}DPSDK_MODIFY_STORE_NOTIFY;

typedef struct
{
	DPSDK_CHAR szStoreCode[DPSDK_STORE_CODE_LEN];				//Store code
	DPSDK_CHAR szStoreOrgCode[DPSDK_ORG_CODE_LEN];				//Store Organization code
}DPSDK_DELETE_STORE_INFO;

typedef struct
{
	DPSDK_INT32	iDelStoreNum;									//Deleted store quantity
	DPSDK_DELETE_STORE_INFO* pDelStoreList;						//Deleted store list
}DPSDK_DELETE_STORE_NOTIFY;

typedef struct
{
	DPSDK_CHAR szStoreCode[DPSDK_STORE_CODE_LEN];				//Store code
	DPSDK_CHAR szStoreOrgCode[DPSDK_ORG_CODE_LEN];				//Store organization code
	DPSDK_CHAR szStoreOrgOldCode[DPSDK_ORG_CODE_LEN];			//Old store organization code
}DPSDK_MOVE_STORE_INFO;

typedef struct
{
	DPSDK_INT32	iMoveStoreNum;									// Moved store quantity
	DPSDK_MOVE_STORE_INFO* pMoveStoreList;						//Moved store list
}DPSDK_MOVE_STORE_NOTIFY;


// Add device group of video intercom
typedef struct
{
	DPSDK_DEV_GROUP_INFO struDevGroupInfo;						// Device group info
}DPSDK_ADD_DEV_GROUP_NOTIFY;

// Modify device group of video intercom
typedef struct
{
	DPSDK_DEV_GROUP_INFO struDevGroupInfo;						// Device group info
}DPSDK_MODIFY_DEV_GROUP_NOTIFY;

// Delete device group of video intercom
typedef struct
{
	DPSDK_INT32 iDevGroupNum;									// Device group quantity
	DPSDK_DEV_GROUP_ID* pDevGroupIdList;						// Device group ID list
}DPSDK_DEL_DEV_GROUP_NOTIFY;

// Add management group of video intercom
typedef struct
{
	DPSDK_GET_MANAGER_GROUP_INFO struMgrGroupInfo;				// Management group info
}DPSDK_ADD_MANAGER_GROUP_NOTIFY;

// Modify management group of video intercom
typedef struct
{
	DPSDK_GET_MANAGER_GROUP_INFO struMgrGroupInfo;				// Management group info
}DPSDK_MODIFY_MANAGER_GROUP_NOTIFY;

// Delete management group of video intercom
typedef struct
{
	DPSDK_INT32 iNum;											// Management group quantity
	DPSDK_MANAGE_GROUP_ID* pMagerGroupList;						// Management group ID list
}DPSDK_DEL_MANAGER_GROUP_NOTIFY;

// Add group relation of video intercom
typedef struct
{
	DPSDK_RELATION_GROUP_INFO struGroupRelationInfo;			// Group relation info
}DPSDK_ADD_GROUP_RELATION_NOTIFY;

// Modify group relation of video intercom
typedef struct
{
	DPSDK_RELATION_GROUP_INFO struGroupRelationInfo;			// Group relation info
}DPSDK_MODIFY_GROUP_RELATION_NOTIFY;

// Delete group relation of video intercom
typedef struct
{
	DPSDK_INT32 iNum;											// Group relation quantity
	DPSDK_GROUP_RELATION_ID* pGroupRelationList;				// Group relation ID list
}DPSDK_DEL_GROUP_RELATION_NOTIFY;

// Add issued info of video intercom
typedef struct
{
	DPSDK_PUBLISH_INFO struPublishInfo;				// Add issued info details
}DPSDK_ADD_INFO_PUBLISH_NOTIFY;

// Modify issued info of video intercom
typedef struct
{
	DPSDK_PUBLISH_INFO struPublishInfo;				// Modify issued info details
}DPSDK_MODIFY_INFO_PUBLISH_NOTIFY;

// Delete issued info of video intercom
typedef struct
{
	DPSDK_INT32 iNum;										// Issued info quantity
	DPSDK_PUBLISH_INFO_ID* pPublishInfoList;				// Issued info ID list
}DPSDK_DEL_INFO_PUBLISH_NOTIFY;

// SCS status notice
typedef struct
{
	DPSDK_INT32 iStatus;									// Service status
}DPSDK_SCS_STATUS_NOTIFY; 

// SCS info notice
typedef struct
{
	DPSDK_CHAR*	pJsonData;									// Received json data
	DPSDK_INT32 iDataLen;									// Data length
}DPSDK_SCS_MESSAGE_NOTIFY;

//// SCS text data notice
//typedef struct
//{
//	DPSDK_CHAR*	pJsonData;									// Received json text data
//	DPSDK_INT32 iDataLen;									// Data length
//}DPSDK_SCS_TEXT_NOTIFY;

// Intercom call invitation notice
typedef struct
{
	DPSDK_INT32 iAudioType;									//Audio type. Refer to DPSDK_AUDIO_TYPE definition.
	DPSDK_INT32 iTalkBit;									//Digit. Refer to DPSDK_TALK_BIT definition.
	DPSDK_INT32 iSampleRate;								//Sampling rate. Refer to DPSDK_TALK_SAMPLE_RATE definition.

	DPSDK_CHAR	szRtpServIP[DPSDK_IP_LEN];
	DPSDK_INT32	iRtpPort;									// Port
	DPSDK_INT32	iCallType;									// Call type. Refer to DPSDK_CALL_TYPE definition.
	DPSDK_CHAR	szGroupId[DPSDK_USER_ID_LEN];				// Group ID
	DPSDK_CHAR	szCallerID[DPSDK_USER_ID_LEN];				// Caller ID
	DPSDK_CHAR	szRecvChnlID[DPSDK_CHANNEL_ID_LEN];			// Receiving Channel ID
	DPSDK_CHAR	szSendChnlID[DPSDK_CHANNEL_ID_LEN];			// Sending Channel ID
	DPSDK_INT32	iCallId;
	DPSDK_INT32	iDlgId;
	DPSDK_INT32 iTId;
}DPSDK_CALL_INVITE_NOTIFY;

// Video intercom call invitation notice
typedef struct
{
	DPSDK_INT32 iAudioType;									//Audio type. Refer to DPSDK_AUDIO_TYPE definition.
	DPSDK_INT32 iTalkBit;									//Digit. Refer to DPSDK_TALK_BIT definition.
	DPSDK_INT32 iSampleRate;								//Sampling rate. Refer to DPSDK_TALK_SAMPLE_RATE definition.

	DPSDK_CHAR	szRtpServIP[DPSDK_IP_LEN];
	DPSDK_INT32	iRtpAPort;									// Audio port
	DPSDK_INT32	iRtpVPort;									// Video port
	DPSDK_INT32	iCallType;									// Call type. Refer to DPSDK_CALL_TYPE definition.
	DPSDK_CHAR	szUserId[DPSDK_USER_ID_LEN];				// Caller ID
	DPSDK_INT32	iCallId;
	DPSDK_INT32	iDlgId;
	DPSDK_INT32 iTId;
}DPSDK_VTCALL_INVITE_NOTIFY;

// Video intercom call invitation notice
typedef struct
{
	DPSDK_CHAR szCallerId[DPSDK_ID_LEN];				//Group call initiator
	DPSDK_CHAR szGroupId[DPSDK_ID_LEN];					//Group ID
	DPSDK_INT32 iTransId;		
}DPSDK_VTCALL_BYE_NOTIFY;

// Video intercom ringing notice
typedef struct
{
	DPSDK_CHAR szUserId[DPSDK_USER_ID_LEN];				// Device ID or channel ID
	DPSDK_INT32	iCallId;
	DPSDK_INT32	iDlgId;
	DPSDK_INT32	iTId;
}DPSDK_VTCALL_RING_NOTIFY;

// Calling party's call cancellation notice
typedef struct
{
	DPSDK_CHAR szUserId[DPSDK_USER_ID_LEN];				// Device ID or channel ID
	DPSDK_INT32	iCallId;
	DPSDK_INT32	iDlgId;
}DPSDK_VTCALL_CANCEL_NOTIFY;

// Video intercom busy notice
typedef struct
{
	DPSDK_CHAR szUserId[DPSDK_USER_ID_LEN];				// Device ID or channel ID
	DPSDK_INT32	iCallId;
	DPSDK_INT32	iDlgId;
	DPSDK_INT32	iTId;
}DPSDK_VTCALL_BUSY_NOTIFY;

//Delete multi-card open config
typedef struct
{
	DPSDK_INT32 iNum;										// Multi-card open quantity
	DPSDK_MULTI_CARD_ID* pMultiCardId;						// Multi-card open ID list
}DPSDK_DEL_MULTICARD_NOTIFY;

// Delete anti-passback config
typedef struct
{
	DPSDK_INT32 iNum;										// Anti-passback config quantity
	DPSDK_ANTI_PASS_BACK_ID* pAntiPassBackId;				// Anti-passback config ID list
}DPSDK_DEL_ANTI_PASSBACK_NOTIFY;

// Delete first-card open config
typedef struct
{
	DPSDK_INT32 iNum;										// Card quantity
	DPSDK_FIRST_CARD_ID* pFirstCardId;						//  Card ID list
}DPSDK_DEL_FIRSTCARD_NOTIFY;

// Delete multi-door interlock config
typedef struct
{
	DPSDK_INT32 iNum;										//  Multi-door interlock quantity
	DPSDK_INTER_DOOR_LOCK_ID* pInterDoorLockId;				//  Multi-door interlock ID list
}DPSDK_DEL_INTERDOORLOCK_NOTIFY;

// Delete person
typedef struct
{
	DPSDK_INT32 iPersonNum;									// Person quantity
	DPSDK_PERSON_ID* pPersonIdList;							// Person ID list
}DPSDK_DEL_PERSON_NOTIFY;

typedef struct
{
	DPSDK_INT32 iPersonNum;									// Person quantity
	DPSDK_PERSON_ID* pPersonIdList;							// Person ID list
}DPSDK_BATCH_ADD_PERSON_NOTIFY;

typedef struct
{
	DPSDK_INT32 iProcessRatio;
	DPSDK_CHAR szSession[DPSDK_ID_LEN];
	DPSDK_INT32 iReturnCode;
	DPSDK_INT32 iFailedPersonIdNum;
	DPSDK_PERSON_ID* pFailedPersonId;
	DPSDK_INT32 iRepeatPersonIdNum;
	DPSDK_PERSON_ID* pRepeatPersonId;
	DPSDK_INT32 iIlleglePersonIdNum;
	DPSDK_PERSON_ID* pIlleglePersonId;
}DPSDK_IMPORT_PERSON_NOTIFY;

typedef struct
{
	DPSDK_INT32 iProcessRatio;
	DPSDK_CHAR szFileUrl[DPSDK_URL_LEN];
	DPSDK_CHAR szSession[DPSDK_ID_LEN];
}DPSDK_EXPORT_PERSON_NOTIFY;

// Person info
typedef struct
{
	DPSDK_INT32			iCardType;							// Card type
	DPSDK_INT32			iStatus;
	DPSDK_INT32         iHasFinger;							// 0:no, 1:have
	DPSDK_INT32			iGender;							// Gender
	DPSDK_CHAR			szLastName[DPSDK_NAME_LEN];			// 
	DPSDK_CHAR			szFirstName[DPSDK_NAME_LEN];		// 
	DPSDK_CHAR			szDepartmentId[DPSDK_ID_LEN];		// 
	DPSDK_CHAR			szRoomId[DPSDK_ID_LEN];				// 
	DPSDK_CHAR			szPersonId[DPSDK_PERSON_ID_LEN];	// 
	DPSDK_BOOL			bUpdatePic;							// 0: unchanged,1 changed
	DPSDK_INT32 iCardNum;									// Card quantity
	DPSDK_CARD_NO* pCardNoList;								// Card no. category
}DPSDK_PERSON_NOTIFY;

// Card right info
typedef struct
{
	DPSDK_CHAR szCardNum[DPSDK_MEMO_LEN];					// Card no.
	DPSDK_CHAR szPersonId[DPSDK_PERSON_ID_LEN];				// Person ID
	DPSDK_INT32 iChannelNum;								// Channel quantity
	DPSDK_CHANNEL_ID* pChannelIdList;						// Channel ID list
	DPSDK_INT32 iDoorGroupNum;								// Door group quantity
	DPSDK_DOORGROUP_ID* pDoorGroupIdList;					// Door group ID list
}DPSDK_CARDRIGHT_INFO;

// Add card right
typedef struct
{
	DPSDK_INT32 iCardNum;									// Card quantity
	DPSDK_CARDRIGHT_INFO* pCardRightList;					// Card right info list
}DPSDK_ADD_CARDRIGHT_NOTIFY;

typedef struct
{
	DPSDK_CHAR szChannelId[DPSDK_CHANNEL_ID_LEN];
	DPSDK_CHAR szHumanImageUrl[DPSDK_URL_LEN];
	DPSDK_CHAR szPictureUrl[DPSDK_URL_LEN];
	DPSDK_CHAR szFaceImageUrl[DPSDK_URL_LEN];
	DPSDK_CHAR szGender[DPSDK_TYPE_LEN];
	DPSDK_CHAR szHat[DPSDK_TYPE_LEN];
	DPSDK_CHAR szBag[DPSDK_TYPE_LEN];
	DPSDK_CHAR szCoat[DPSDK_TYPE_LEN];
	DPSDK_CHAR szCoatColor[DPSDK_TYPE_LEN];
	DPSDK_CHAR szTrousers[DPSDK_TYPE_LEN];
	DPSDK_CHAR szTrousersColor[DPSDK_TYPE_LEN];
	DPSDK_CHAR szEmotion[DPSDK_TYPE_LEN];
	DPSDK_CHAR szBeard[DPSDK_TYPE_LEN];
	DPSDK_CHAR szGlasses[DPSDK_TYPE_LEN];
	DPSDK_CHAR szMask[DPSDK_TYPE_LEN];
	DPSDK_RECT struFaceImage;
	DPSDK_RECT struHumanImage;
	DPSDK_INT32 iAge;
	DPSDK_TIMET tCaptureTime;
}DPSDK_HUMAN_INFO_NOTIFY;

typedef struct
{
	DPSDK_CHAR szChannelId[DPSDK_CHANNEL_ID_LEN];
	DPSDK_CHAR szPictureUrl[DPSDK_URL_LEN];
	DPSDK_CHAR szPlateImageUrl[DPSDK_URL_LEN];
	DPSDK_CHAR szPlate[DPSDK_ID_LEN];
	DPSDK_CHAR szCarType[DPSDK_TYPE_LEN];
	DPSDK_CHAR szCarColor[DPSDK_COLOR_LEN];
	DPSDK_CHAR szPlateColor[DPSDK_COLOR_LEN];
	DPSDK_CHAR szCarBrand[DPSDK_ID_LEN];
	DPSDK_RECT struCarImage;
	DPSDK_TIMET tCaptureTime;
}DPSDK_VEHICLE_INFO_NOTIFY;

typedef struct
{
	DPSDK_CHAR szChannelId[DPSDK_CHANNEL_ID_LEN];
	DPSDK_CHAR szPictureUrl[DPSDK_URL_LEN];
	DPSDK_CHAR szCarImageUrl[DPSDK_URL_LEN];
	DPSDK_CHAR szCarType[DPSDK_TYPE_LEN];
	DPSDK_CHAR szCarColor[DPSDK_COLOR_LEN];
	DPSDK_INT32 iRiderNum;
	DPSDK_RECT struCarImage;
	DPSDK_TIMET tCaptureTime;
}DPSDK_NONVEHICLE_INFO_NOTIFY;

typedef struct
{
	DPSDK_CHAR szUrl[DPSDK_URL_LEN];
	DPSDK_CHAR szSession[DPSDK_ID_LEN];
}DPSDK_FILEDOWNLOAD_INFO_NOTIFY;

typedef struct
{
	DPSDK_CHAR szType[DPSDK_TYPE_LEN];
}DPSDK_TYPE;

typedef struct
{
	DPSDK_TIMET tStartTime;
	DPSDK_TIMET tEndTime;
	DPSDK_CHAR szGender[DPSDK_TYPE_LEN];
	DPSDK_CHAR szCoat[DPSDK_TYPE_LEN];
	DPSDK_INT32 iCoatColorNum;
	DPSDK_TYPE* pCoatColorList;
	DPSDK_CHAR szTrousers[DPSDK_TYPE_LEN];
	DPSDK_INT32 iTrousersColorNum;
	DPSDK_TYPE* pTrousersColorList;
	DPSDK_CHAR szHat[DPSDK_TYPE_LEN];
	DPSDK_CHAR szBag[DPSDK_TYPE_LEN];
	DPSDK_INT32 iChannelNum;
	DPSDK_CHANNEL_ID* pChannelIdList;
}DPSDK_HUMANSEARCH_PARAM;

typedef struct
{
	DPSDK_INT32 iIdNum;
	DPSDK_ID* pIdList;
	DPSDK_HUMANSEARCH_PARAM struHumanSearchBase;
}DPSDK_EXPORT_HUMANSEARCH_PARAM;

typedef struct
{
	DPSDK_CHAR szId[DPSDK_ID_LEN];
	DPSDK_CHAR szChannelId[DPSDK_CHANNEL_ID_LEN];
	DPSDK_CHAR szHumanImageUrl[DPSDK_URL_LEN];
	DPSDK_CHAR szPictureUrl[DPSDK_URL_LEN];
	DPSDK_CHAR szFaceImageUrl[DPSDK_URL_LEN];
	DPSDK_CHAR szGender[DPSDK_TYPE_LEN];
	DPSDK_CHAR szHat[DPSDK_TYPE_LEN];
	DPSDK_CHAR szCoat[DPSDK_TYPE_LEN];
	DPSDK_CHAR szCoatColor[DPSDK_TYPE_LEN];
	DPSDK_CHAR szCoatColorName[DPSDK_NAME_LEN];
	DPSDK_CHAR szBag[DPSDK_TYPE_LEN];
	DPSDK_CHAR szTrousers[DPSDK_TYPE_LEN];
	DPSDK_CHAR szTrousersColor[DPSDK_TYPE_LEN];
	DPSDK_CHAR szTrousersColorName[DPSDK_NAME_LEN];
	DPSDK_CHAR szEmotion[DPSDK_TYPE_LEN];
	DPSDK_CHAR szBeard[DPSDK_TYPE_LEN];
	DPSDK_CHAR szGlasses[DPSDK_TYPE_LEN];
	DPSDK_CHAR szMask[DPSDK_TYPE_LEN];
	DPSDK_RECT struFaceImage;
	DPSDK_RECT struHumanImage;
	DPSDK_INT32 iAge;
	DPSDK_TIMET tCaptureTime;
}DPSDK_HUMAN_INFO;

typedef struct
{
	DPSDK_INT32 iHumanNum;
	DPSDK_HUMAN_INFO* pHumanList;
}DPSDK_HUMAN_LIST;

typedef struct
{
	DPSDK_TIMET tStartTime;
	DPSDK_TIMET tEndTime;
	DPSDK_INT32 iChannelNum;
	DPSDK_CHANNEL_ID* pChannelIdList;
	DPSDK_INT32 iCarTypeNum;
	DPSDK_ID* pCarTypeList;
	DPSDK_INT32 iCarColorNum;
	DPSDK_ID* pCarColorList;
}DPSDK_VEHICLE_BASE_PARAM;

typedef struct
{
	DPSDK_CHAR szId[DPSDK_ID_LEN];
	DPSDK_CHAR szChannelId[DPSDK_CHANNEL_ID_LEN];
	DPSDK_CHAR szPictureUrl[DPSDK_URL_LEN];
	DPSDK_CHAR szCarType[DPSDK_TYPE_LEN];
	DPSDK_CHAR szCarTypeName[DPSDK_NAME_LEN];
	DPSDK_CHAR szCarColor[DPSDK_COLOR_LEN];
	DPSDK_CHAR szCarColorName[DPSDK_NAME_LEN];
	DPSDK_RECT struCarImage;
	DPSDK_TIMET tCaptureTime;
}DPSDK_VEHICLE_BASE_INFO;

typedef struct
{
	DPSDK_VEHICLE_BASE_PARAM struVehicleBase;
	DPSDK_INT32 iPlateNum;
	DPSDK_ID* pPlateList;
	DPSDK_INT32 iCarBrandNum;
	DPSDK_ID* pCarBrandList;
	DPSDK_INT32 iPlateColorNum;
	DPSDK_ID* pPlateColorList;
}DPSDK_VEHICLE_PARAM;

typedef struct
{
	DPSDK_INT32 iIdNum;
	DPSDK_ID* pIdList;
	DPSDK_VEHICLE_PARAM struVehicleSearchBase;
}DPSDK_EXPORT_VEHICLESEARCH_PARAM;

typedef struct
{
	DPSDK_VEHICLE_BASE_INFO struVehicleBaseInfo;
	DPSDK_CHAR szPlateImageUrl[DPSDK_URL_LEN];
	DPSDK_CHAR szPlate[DPSDK_ID_LEN];
	DPSDK_CHAR szPlateColor[DPSDK_COLOR_LEN];
	DPSDK_CHAR szPlateColorName[DPSDK_NAME_LEN];
	DPSDK_CHAR szCarBrand[DPSDK_ID_LEN];
	DPSDK_CHAR szCarBrandName[DPSDK_NAME_LEN];
}DPSDK_VEHICLE_INFO;

typedef struct
{
	DPSDK_INT32 iVehicleNum;
	DPSDK_VEHICLE_INFO* pVehicleList;
}DPSDK_VEHICLE_LIST;

typedef struct
{
	DPSDK_VEHICLE_BASE_PARAM struVehicleBase;
	DPSDK_CHAR szRiderNum[DPSDK_TYPE_LEN];
	//DPSDK_INT32 iRiderNum;
}DPSDK_NONVEHICLE_PARAM;

typedef struct
{
	DPSDK_INT32 iIdNum;
	DPSDK_ID* pIdList;
	DPSDK_NONVEHICLE_PARAM struNonVehicleSearchBase;
}DPSDK_EXPORT_NONVEHICLESEARCH_PARAM;

typedef struct
{
	DPSDK_VEHICLE_BASE_INFO struVehicleBaseInfo;
	DPSDK_CHAR szCarImageUrl[DPSDK_URL_LEN];
	DPSDK_INT32 iRiderNum;
}DPSDK_NONVEHICLE_INFO;

typedef struct
{
	DPSDK_INT32 iVehicleNum;
	DPSDK_NONVEHICLE_INFO* pVehicleList;
}DPSDK_NONVEHICLE_LIST;

// Organization info
typedef struct
{
	DPSDK_CHAR szDepartmentName[DPSDK_NAME_LEN];			// Organization name
	DPSDK_CHAR szDepartmentCode[DPSDK_ORG_CODE_LEN];		// Organization code
	DPSDK_CHAR szParentCode[DPSDK_ORG_CODE_LEN];			// Parent node code
}DPSDK_DEPARTMENT_NOTIFY;

// Organization code
typedef struct
{
	DPSDK_CHAR szDepartmentCode[DPSDK_ORG_CODE_LEN];		// Organization code
}DPSDK_DEPARTMENT_CODE;

// Delete organization 
typedef struct
{
	DPSDK_INT32 iDepartmentNum;								// Organization quantity
	DPSDK_DEPARTMENT_CODE* pDepartmentCodeList;				// Organization code list
}DPSDK_DEL_DEPARTMENT_NOTIFY;

// Door group info
typedef struct
{
	DPSDK_CHAR szDoorGroupName[DPSDK_DEVICE_NAME_LEN];		// Door group name
	DPSDK_CHAR szDoorGroupId[DPSDK_CHANNEL_ID_LEN];			// Door group ID
	DPSDK_INT32 iTimeTemplateId;							// Time template ID
	DPSDK_INT32 iNum;										// Channel quantity
	DPSDK_CHANNEL_ID* pChannelList;							// Channel list
}DPSDK_DOORGROUP_NOTIFY;

// Delete door group 
typedef struct
{
	DPSDK_INT32 iNum;										// Door group quantity
	DPSDK_DOORGROUP_ID* pDoorGroupList;						// Door group ID list
}DPSDK_DEL_DOORGROUP_NOTIFY;

// 
typedef struct
{
	DPSDK_CHAR szdoorGroupId[DPSDK_DEVICE_NAME_LEN];		// Door group ID
	DPSDK_INT32 iPersonNum;									// Person quantity
	DPSDK_PERSON_DATA* pPersonList;							// Person ID list
}DPSDK_DOORGROUPPERSON_NOTIFY;

// Global door channel
typedef struct
{
	DPSDK_INT32 iChannelNum;								// Channel quantity
	DPSDK_CHANNEL_ID* pChannelList;							// Channel list
}DPSDK_GLOBAL_CONTROL_CONFIG_NOTIFY;

// Delete remote verification
typedef struct
{
	DPSDK_INT32 iNum;										// Remote verification quantity
	DPSDK_REMOTE_VERIFICATION_ID* pRemoteVerificationId;	// Remote verification ID list
}DPSDK_DEL_REMOTE_VERIFICATION_NOTIFY;

// Delete door rule
typedef struct
{
	DPSDK_INT32 iRuleNum;									// Door rule quantity
	DPSDK_DOOR_RULE* pDoorRuleList;							// Door rule ID list
}DPSDK_DEL_DOORRULE_NOTIFY;

// Device switch service
typedef struct
{
	DPSDK_INT32 iDevNum;									// Device quantity
	DPSDK_DEVICE_ID* pDevList;								// Device list
}DPSDK_CHANGE_SERVER_DEVICES_NOTIFY;

// Delete holiday
typedef struct
{
	DPSDK_INT32 iHolidayNum;								// Holiday quantity
	DPSDK_HOLIDAY_ID* pHolidayId;							// Holiday ID list
}DPSDK_DEL_HOLIDAY_NOTIFY;

// Access card using event (alarm notice info)
typedef struct
{
	DPSDK_CHAR szChannelCode[DPSDK_CHANNEL_ID_LEN];			//Device channel
	DPSDK_CHAR szChannelName[DPSDK_NAME_LEN_EX];			//Channel name
	DPSDK_CHAR szDepartmentCode[DPSDK_ORG_CODE_LEN];		//Organization code
	DPSDK_CHAR szDepartmentName[DPSDK_NAME_LEN_EX];			//Organization name
	DPSDK_CHAR szCardNum[DPSDK_MEMO_LEN];					//Card no.
	DPSDK_CHAR szFirstName[DPSDK_NAME_LEN];					//Card user first name
	DPSDK_CHAR szLastName[DPSDK_NAME_LEN];					//Card user last name
	DPSDK_CHAR szPersonId[DPSDK_PERSON_ID_LEN];				//Card user ID
	DPSDK_CHAR szPicture1[DPSDK_ALARM_ALARMPICTURE_LEN];	// Snapshot path
	DPSDK_CHAR szPicture2[DPSDK_ALARM_ALARMPICTURE_LEN];	// Snapshot path
	DPSDK_CHAR szPicture3[DPSDK_ALARM_ALARMPICTURE_LEN];	// Snapshot path
	DPSDK_CHAR szPicture4[DPSDK_ALARM_ALARMPICTURE_LEN];	// Snapshot path
	DPSDK_CHAR szMemo[DPSDK_MEMO_LEN];						// Description
	DPSDK_CHAR szAlarmCode[DPSDK_ALARM_ALARMCODE_LEN];		// Alarm code
	DPSDK_INT32 iCardType;									// Card type
	DPSDK_INT32 iInOutStatus;								// In/out status
	DPSDK_INT32 iEventType;									// Event type
	DPSDK_INT32 iDeviceType;								// Device type
	DPSDK_TIMET tSwipDate;									// Card using time, timestamp
	DPSDK_CHAR szTel[DPSDK_PHONE_LEN];
	DPSDK_CHAR szIDNum[DPSDK_ID_LEN];
	DPSDK_INT32 iGender;
}DPSDK_ACESSDOOR_RECORD_NOTIFY;

// Access status change notice
typedef struct
{
	DPSDK_CHAR szChannelCode[DPSDK_CHANNEL_ID_LEN];			// Device channel
	DPSDK_TIMET tSwipDate;									// Card using time, timestamp
	DPSDK_INT32 iDoorStatus;								// Door status,1- open door,2-close door
}DPSDK_ACESSDOOR_STATUS_NOTIFY;

// Add door basic config notice
typedef struct
{
	DPSDK_DOOR_INFO struDoorInfo;							//  Door basic config notice
}DPSDK_ADD_DOORCONFIG_NOTIFY;

// Modify door basic config notice
typedef struct
{
	DPSDK_DOOR_INFO struDoorInfo;							// Door basic config notice
}DPSDK_MODIFY_DOORCONFIG_NOTIFY;

// Add access alarm linkage config
typedef struct
{
	DPSDK_DOOR_ALARM_INOUT* pAlarmLinkInfo;				// Alarm linkage info
}DSPDK_ADD_ALARMLINK_CONFIG_NOTIFY;

// Update access alarm linkage config
typedef struct
{
	DPSDK_DOOR_ALARM_INOUT* pAlarmLinkInfo;				// Alarm linkage info
}DSPDK_MODIFY_ALARMLINK_CONFIG_NOTIFY;

typedef struct
{
	DPSDK_INT32 iStatus;
	DPSDK_INT32 iChannelNum;
	DPSDK_CHANNEL_ID* pChannelList;
}DSPDK_DOOR_GLOBALCTRLFAILED_CHANNEL_NOTIFY;

//Fingerprint collection
typedef struct
{
	DPSDK_INT32 iBufLen;
	DPSDK_CHAR* pFingerPrintData;
}DPSDK_CAPTURE_FINGERPRINT; 

//NO/NC
typedef struct
{
	DPSDK_INT32 iConfigType;							// NO: 1, NC: 0
	DPSDK_INT32 iStatus;								// NO (NC) failure or success, 0: failure; 1: success
	DPSDK_CHAR szChannelId[DPSDK_CHANNEL_ID_LEN];		// Channel ID
}DPSDK_DOOR_ALAWYS_OPEN_CLOSE;

//Zone status notice
typedef struct
{
	DPSDK_INT32 iStatus;								// 
	DPSDK_CHAR szChannelId[DPSDK_CHANNEL_ID_LEN];		// Channel ID
	DPSDK_CHAR szDevCode[DPSDK_DEVICE_ID_LEN];			// Device code
	DPSDK_CHAR szDefenceAreaId[DPSDK_CHANNEL_ID_LEN];	// Zone ID
}DPSDK_ALARMHOST_DEFENCEAREASTATUS_NOTIFY;

//Subsystem status notice
typedef struct
{
	DPSDK_INT32 iStatus;								// 
	DPSDK_CHAR szDevCode[DPSDK_DEVICE_ID_LEN];			// Device code
	DPSDK_CHAR szSubSystemId[DPSDK_CHANNEL_ID_LEN];		// Subsystem ID
}DPSDK_ALARMHOST_SUBSYSTEMSTATUS_NOTIFY;

//Alarm host status notice
typedef struct
{
	DPSDK_INT32 iStatus;								// 
	DPSDK_CHAR szDevCode[DPSDK_DEVICE_ID_LEN];			// Device code
}DPSDK_ALARMHOST_STATUS_NOTIFY;

// Alarm host refresh notice
typedef struct  
{
	DPSDK_INT32 iStatus;								// 
	DPSDK_CHAR szDevCode[DPSDK_DEVICE_ID_LEN];			// Device code
	DPSDK_INT32 iDefenceAreaNum;						// Zone quantity
	DPSDK_DEFENCEAREAS_INFO* pDefenceAreaList;			// Zone info list
	DPSDK_INT32 iSubSystemNum;							// Subsystem quantity
	DPSDK_SUBSYSTEM_INFO* pSubSystemList;				// Subsystem info list
}DPSDK_REFRESH_ALARMHOST_SUBINFO_NOTIFY;

// Door global NO/NC operation notice
typedef struct  
{
	DPSDK_INT32 iStatus;								//  0: restore to norma; 1: global clode; 2: global open
}DPSDK_DOOR_GLOBAL_CONTROL_STATUE;

typedef struct
{
	DPSDK_INT32 iStatus;								//  
}DPSDK_STATUS;

// Real-time notify present client after a call record is generated
typedef struct
{
	DPSDK_CALLLOG_INFO struCallLogInfo;					// Call info
}DPSDK_ADD_CALL_LOG_NOTIFY;

typedef struct  
{
	DPSDK_INT32 iOperateType;									// Operating type: 1=Add, 2=update, 3=delete
	DPSDK_INT32 iGroupId;										// Alarm type category
	DPSDK_INT32	iAlarmTypeId;									// Alarm type ID
	DPSDK_CHAR	szAlarmTypeName[DPSDK_ALARM_ALARMTYPENAME_LEN];	// Alarm type name
	DPSDK_BOOL	bOther;											// Does it belong to others:0=no,1=yes
}DPSDK_UPDATE_CUSTOMALARMTYPE_NOTIFY;

//Notify the client about obtained SIPID
typedef struct
{
	DPSDK_CHAR szDevCode[DPSDK_DEVICE_ID_LEN];					// Device code
	DPSDK_CHAR szSipId[DPSDK_MEMO_LEN];							// SIPID
}DPSDK_DEVICE_SIPID_NOTIFY;

//Notify the client about add email Address
typedef struct
{
	DPSDK_CHAR				szEmailAddr[DPSDK_ALARM_EMAILRECEIVER_LEN];		// E-mail address
}DPSDK_ADD_EMAILADDRESS_NOTIFY;

typedef struct
{
	DPSDK_FLOAT				fFreeCapcity;
	DPSDK_FLOAT				fCpuPercent;
	DPSDK_FLOAT				fMemoryPercent;
}DPSDK_SYSTEMINFO_WARNING_NOTIFY;

typedef struct
{
	DPSDK_INT32				iCount;
}DPSDK_UNAUTHDEVICES_INFO_NOTIFY;

typedef struct
{
	DPSDK_CHAR szChannelCode[DPSDK_CHANNEL_ID_LEN];			// Device channel
}DPSDK_CLOSE_REMOTECHECK_WIND_NOTIFY;

typedef struct
{
	DPSDK_ALARM_INFO struAlarmInfo;
}DPSDK_ALARM_PROCESS_RECORD_NOTIFY;

typedef struct
{
	DPSDK_INT32 iRecordType;
	DPSDK_CHAR	szRecordID[DPSDK_ID_LEN];
}DPSDK_DELETE_ALARM_PROCESS_RECORD_NOTIFY;

typedef struct
{
	DPSDK_ALARM_TEMPLATE_INFO struAlarmTempInfo;
}DPSDK_ALARM_PROCESS_TEMPLATE_NOTIFY;

typedef struct
{
	DPSDK_INT32 iID;
}DPSDK_DELETE_ALARM_PROCESS_TEMPLATE_NOTIFY;

typedef struct
{
	DPSDK_CHAR szKeyCode[DPSDK_ID_LEN];
	DPSDK_INT32 iKeyType;
	DPSDK_INT32 iBindingKeyType;
}RESOURE_BINDING_INFO;

typedef struct
{
	DPSDK_INT32 iResoureNum;
	RESOURE_BINDING_INFO* pResoureInfo;
}DPSDK_NOTIFY_RESOURE_BINDING_NOTIFY;

typedef struct
{
	DPSDK_INT32 iIDNum;
	DPSDK_DEVICE_ID* pIDList;
}DPSDK_DELETE_UNAUTH_DEVICE_NOTIFY;

typedef struct
{
	DPSDK_DEVICE_ID struID;
	DPSDK_CHAR szDeviceIP[DPSDK_IP_LEN];
	DPSDK_CHAR szDeviceName[DPSDK_NAME_LEN];
	DPSDK_INT32 iDeviceType;
}DPSDK_ADD_UNAUTH_DEVICE_NOTIFY;

typedef struct
{
	DPSDK_DEVICE_ID struID;
	DPSDK_CHAR szDeviceIP[DPSDK_IP_LEN];
	DPSDK_CHAR szDeviceName[DPSDK_NAME_LEN];
	DPSDK_INT32 iDeviceType;
}DPSDK_UPDATE_UNAUTH_DEVICE_NOTIFY;

typedef struct
{
	DPSDK_CHAR szSeq[DPSDK_ID_LEN];
	DPSDK_INT32 iCode;
	DPSDK_CHAR szBackUpPath[DPSDK_FILE_PATH_LEN];
}DPSDK_MANUAL_BACKUP_FILE_NOTIFY;

typedef struct
{
	DPSDK_INT32 iNum;
	DPSDK_ITC_SECTION_INFO* pSectionList;
}DPSDK_TIC_DEL_SECTION_NOTIFY;

//Notify the result of contact
typedef struct
{
	DPSDK_CHAR szDevCode[DPSDK_DEVICE_ID_LEN];					// Device code
	DPSDK_INT32 iStatus;										// Result, 0=failure, 1=success
}DPSDK_SYNC_CONTACT_NOTIFY;

//Add resource parameter
typedef struct
{
	DPSDK_CHAR				szAppearPoint[DPSDK_TZ_APPEARPOINT_LEN];				// Point of appearance(dictionary)
	DPSDK_CHAR				szChannelCode[DPSDK_TZ_CHNCODE_LEN];					// Channel code
	DPSDK_CHAR				szCaseRecordIdx[DPSDK_TZ_RECORDID_LEN];					// Unique identification code for related cases (required)
	DPSDK_CHAR				szCollectAddr[DPSDK_TZ_ADDRESS_LEN];					// Resource collection address
	DPSDK_CHAR				szCollectTime[DPSDK_TZ_TIME_LEN];						// Resource collection (required)
	DPSDK_CHAR				szCreatePersonCode[DPSDK_TZ_CREATOR_LEN];				// Founder code
	DPSDK_CHAR				szDataSource[DPSDK_TZ_DICTYPE_LEN];						// Resource source (dictionary) (required)
	DPSDK_CHAR				szDeviceCode[DPSDK_TZ_DEVCODE_LEN];						// Device code
	DPSDK_CHAR				szGpsX[DPSDK_TZ_GPS_LEN];								// Longitude
	DPSDK_CHAR				szGpsY[DPSDK_TZ_GPS_LEN];								// Latitude
	DPSDK_CHAR				szMemo[DPSDK_TZ_MEMO_LEN];								// Resource remark
	DPSDK_CHAR				szRecordType[DPSDK_TZ_DICTYPE_LEN];						// Record type (dictionary)
	DPSDK_CHAR				szResourceCode[DPSDK_TZ_RESOURCECODE_LEN];				// Unique resource code, web does not need this parameter when adding, provided for the view library
	DPSDK_CHAR				szResourceName[DPSDK_TZ_RESOURCENAME_LEN];				// Resource name(required)
	DPSDK_CHAR				szResourceType[DPSDK_TZ_DICTYPE_LEN];					// Resource type (dictionary) (required)
	DPSDK_CHAR				szStoragePath[DPSDK_TZ_PATH_LEN];						// Resource storage address
	DPSDK_CHAR				szVideoPicPath[DPSDK_TZ_PATH_LEN];						// Picture path of video
	DPSDK_CHAR				szFileSize[DPSDK_TZ_FILESIZE_LEN];						// File size (required)
	DPSDK_CHAR				szTimeDiff[DPSDK_TZ_TIMEDIFF_LEN];						// Time difference
	DPSDK_CHAR				szBeijingTime[DPSDK_TZ_TIME_LEN];						// Beijing time (required)
}DPSDK_TZ_NEWRESOURCE_PARAM;

typedef struct
{
	DPSDK_CHAR				szResourceCode[DPSDK_TZ_RESOURCECODE_LEN];				// Unique resource code, web does not need this parameter when adding, provided for the view library
}DPSDK_TZ_NEWRESOURCE_NOTIFY;

//Cloud storage info
typedef struct
{
	DPSDK_CHAR				szBucket[DPSDK_TZ_STRING_LEN];							// Cloud storage bucket
	DPSDK_CHAR				szEfsIp[DPSDK_IP_LEN];									// Cloud storage address
	DPSDK_CHAR				szModeM[DPSDK_TZ_STRING_LEN];							// Cloud storage M
	DPSDK_CHAR				szModeN[DPSDK_TZ_STRING_LEN];							// Cloud storage N
	DPSDK_CHAR				szPassWord[DPSDK_PWD_LEN];								// Cloud storage password
	DPSDK_CHAR				szPort[DPSDK_TZ_STRING_LEN];							// Cloud storage port
	DPSDK_CHAR				szUserName[DPSDK_NAME_LEN];								// Cloud storage user name
	DPSDK_CHAR				szIsLocal[DPSDK_TZ_STRING_LEN];							// Is local, 1=yes,0=no
}DPSDK_TZ_EFS_INFO;

//Cloud storage info list
typedef struct
{
	DPSDK_UINT32						uiTotal;									// Cloud storage info number	
	DPSDK_TZ_EFS_INFO					struEFSList[1];								// Cloud storage info list
}DPSDK_TZ_EFSINFO_LIST;

#endif // _DPSDK_DPSDKDEFINE_H_
