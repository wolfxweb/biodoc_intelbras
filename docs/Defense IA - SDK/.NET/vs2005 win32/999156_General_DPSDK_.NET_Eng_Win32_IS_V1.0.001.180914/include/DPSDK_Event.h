#ifndef DPSDK_DPSDKEVENT_H
#define DPSDK_DPSDKEVENT_H

#define DPSDK_EVENT_SERVER_OFFLINE						1		// Service offline
#define DPSDK_EVENT_RELOGIN_SUCCESS						2		// Service reconnection successfully
#define DPSDK_EVENT_ALARM_ALARMEVENT					3		// Alarm event
#define DPSDK_EVENT_ALARM_CONFIRMALARM					4		// Alarm confirmation
#define DPSDK_EVENT_ALARM_ALARMPICTURE					5		// Alarm linked picture
#define DPSDK_EVENT_ALARM_EXPORTALARM					6		// Alarm export
#define DPSDK_EVENT_DEVICE_STATUS						7		// Device status change
#define DPSDK_EVENT_CHANNEL_STATUS						8		// Channel status change
#define DPSDK_EVENT_ADD_ORG								9		// Add organization
#define DPSDK_EVENT_MODIFY_ORG							10		// Modify organization
#define DPSDK_EVENT_DELETE_ORG							11		// Delete organization		The style of data is xml <NodeIDs><NodeID>OrganizationID</NodeID></NodeIDs>
#define DPSDK_EVENT_MOVE_ORG							12		// Move organization
#define DPSDK_EVENT_ADD_DEVICE							13		// Add device
#define DPSDK_EVENT_MODIFY_DEVICE						14		// Modify device
#define DPSDK_EVENT_DELETE_DEVICE						15		// Delete device
#define DPSDK_EVENT_MOVE_DEVICE							16		// Move device
#define DPSDK_EVENT_ALERT_USER							17		// User alert
#define DPSDK_EVENT_USER_LOCKED							18		// User locked
#define DPSDK_EVENT_USER_PWD_CHANGED					19		// User password changed
#define DPSDK_EVENT_USER_OVER_DATA						20		// User expired			
#define DPSDK_EVENT_SYNC_TIME							21		// Synchronous time
#define DPSDK_EVENT_USER_UNLOCKED						22		// User unlocked
#define DPSDK_EVENT_USERDATA_STATE_CHANGE				23		// User data online status change
#define DPSDK_EVENT_USERDATA_ADD						24		// Add user data
#define DPSDK_EVENT_USERDATA_DELETE						25		// Delete user data	
#define DPSDK_EVENT_USER_DELETE							26		// User deleted
#define DPSDK_EVENT_VIEW_INFO_CHANGED					27		// Visible range changed
#define DPSDK_EVENT_DEV_LOCATION_CHANGED				28		// Add and change device on the bitmap
#define DPSDK_EVENT_LOCK_STATUS_CHANGED					29		// Notification of the change of PTZ locking status
#define DPSDK_EVENT_RADER_FRAME_INFO					30		// Rader information report
#define DPSDK_EVENT_FACE_INFO							31		// Face recognition information
#define DPSDK_EVENT_UPDATE_PERSONTYPE					32		// Person type update
#define DPSDK_EVENT_USERDEFINEDATA_ALERT				33		// Change of user defined data
#define DPSDK_EVENT_POS_DATA_PUSH						34		// POS data report
#define DPSDK_EVENT_ADD_RELATION						35		// Add video related channel
#define DPSDK_EVENT_MODIFY_RELATION						36		// Modify video related channel
#define DPSDK_EVENT_DELETE_RELATION						37		// Delete video related channel
#define DPSDK_EVENT_BITMAP_INFO_CHANGED					38		// Bitmap information change
#define DPSDK_EVENT_UPDATE_LICENSE						39		// License authorization update
#define DPSDK_EVENT_RECORD_LOCK							40		// Record lock notice
#define DPSDK_EVENT_RECORD_UNLOCK						41		// Record unlock notice
#define DPSDK_EVENT_ADD_RECORD_TAG						42		// Record tag adding notice
#define DPSDK_EVENT_DELETE_RECORD_TAG					43		// Record tag deletion notice
#define DPSDK_EVENT_CHANNEL_INTELLISTATE				44		// Smart status change
#define DPSDK_EVENT_ADD_TIMETEMPLATE					45		// Add time template
#define DPSDK_EVENT_MODIFY_TIMETEMPLATE					46		// Modify time template
#define DPSDK_EVENT_DELETE_TIMETEMPLATE					47		// Delete time template
#define	DPSDK_EVENT_ADD_STORAGEPLAN						48		// Add record plan
#define	DPSDK_EVENT_MODIFY_STORAGEPLAN					49		// Modify record plan
#define	DPSDK_EVENT_DELETE_STORAGEPLAN					50		// Delete record plan
#define DPSDK_EVENT_ADD_TVWALL							51		// Add TV wall
#define DPSDK_EVENT_MODIFY_TVWALL						52		// Modify TV wall
#define DPSDK_EVENT_DELETE_TVWALL						53		// Delete TV wall
#define DPSDK_EVENT_ADD_ALARMPLAN						54		// Add event config
#define DPSDK_EVENT_MODIFY_ALARMPLAN					55		// Modify event config
#define DPSDK_EVENT_DELETE_ALARMPLAN					56		// Delete event config
#define DPSDK_EVENT_JSON_TRANSPORT_DATA					57		// Json passthrough data notice
#define DPSDK_EVENT_MODIFY_FTP_CONFIG					58		// ftp config modification notice
#define DPSDK_EVENT_UPDATE_FACE_PERSONTYPE				59		// Face library update

#define  DPSDK_EVENT_MASTER_SLAVE						60		//AttachMasterSlaveGroup calls back subscribed message

#define DPSDK_EVENT_UPDATE_CUSTOMALARMTYPE				61		// Update custom alarm type

#define DPSDK_EVENT_ADD_EMAIL_ADDRESS                   62      // Add Email Address

#define DPSDK_EVENT_SYSTEMINFO_WARNING                  63      // SystemInfo Warning

#define DPSDK_EVENT_UNAUTHDEVICES_INFO                  64

#define DPSDK_EVENT_CLOSE_REMOTECHECK_WINDOW            65

#define DPSDK_EVENT_NOTIFY_ALARMPROCESS_RECORD          66
#define DPSDK_EVENT_ASSGIN_ALARMPROCESS_RECORD          67
#define DPSDK_EVENT_DELETE_ALARMPROCESS_RECORD          68
#define DPSDK_EVENT_ADD_ALARMPROCESS_TEMPLE             69
#define DPSDK_EVENT_UPDATE_ALARMPROCESS_TEMPLE          70
#define DPSDK_EVENT_DELETE_ALARMPROCESS_TEMPLE          71
#define DPSDK_EVENT_NOTIFY_RESOURE_BINDING              72
#define DPSDK_EVENT_NOTIFY_DELETE_UNAUTHDEVICE          73
#define DPSDK_EVENT_NOTIFY_ADD_UNAUTHDEVICE				74
#define DPSDK_EVENT_NOTIFY_UPDATE_UNAUTHDEVICE          75
#define DPSDK_EVENT_MANUAL_BACKUP_FILE			        76
#define DPSDK_EVENT_ADD_SECTION					        77
#define DPSDK_EVENT_MODIFY_SECTION						78
#define DPSDK_EVENT_DEL_SECTION					        79
#define DPSDK_EVENT_MEDIA_KEY_CHANGE			        80

//Video status
#define DPSDK_EVENT_MEDIA_FIRST_DATA_PARSE				99		// Video receives the first frame parse
#define DPSDK_EVENT_MEDIA_FIRST_RECEIVE					100		// Video receives the first frame data
#define DPSDK_EVENT_MEDIA_MTSOFFLINE					101		// Video MTS offline
#define DPSDK_EVENT_MEDIA_ENCHANGE						102		// Call back resolution change. The corresponding structure is MEDIA_ENCHANGE
#define DPSDK_EVENT_MEDIA_SCREENSHOT					103		// Call back screenshot. The corresponding structure is MEDIA_DISPLAY
#define DPSDK_EVENT_MEDIA_RECORD_FINISH_FILE			104		// Finish the split of a record file. The corresponding structure is FileStoreInfo
#define DPSDK_EVENT_MEDIA_RECORD_ABNORMAL				105		// Record abnormal. The corresponding structure is NULL
#define DPSDK_EVENT_MEDIA_VISIBLE_DEC					106		// Decoding display call-back

//Local playback
#define DPSDK_EVENT_LOCAL_PLAYBACK_FILE_INDEX			107		// Completion of index file creatior.Upside down effective after receiving the notice
#define DPSDK_EVENT_LOCAL_PLAYBACK_FILE_END				108		// The end of local playback
#define DPSDK_EVENT_LOCAL_PLAYBACK_FILE_TIME_DONE		109		// Locale file start and end time

//Playback state
#define DPSDK_EVENT_PLAYBACK_FRAME_STEP_NOTIFY			111		// Notice after single-frame switch
#define DPSDK_EVENT_PLAYBACK_DATAOVER_RECEIVE			112		// Playback receives 0 packet
#define DPSDK_EVENT_PLAYBACK_RENDER_OVER				113		// Playback decoding is finished
#define DPSDK_EVENT_PLAYBACK_BUFFER_UPPER				114		// Playback buffer size exceeds upper limit
#define DPSDK_EVENT_PLAYBACK_BUFFER_LOWER				115		// Playback buffer size is below the lower limit
#define DPSDK_EVENT_PLAYBACK_RENDER_CREATE				116		// Playback decoding is started
#define DPSDK_EVENT_PLAYBACK_START_PLAYBACK				117		// Start playback
#define DPSDK_EVENT_PLAYBACK_STOP_PLAYBACK				118		// Stop playback
#define DPSDK_EVENT_PLAYBACK_SEEK						119		// Jump play

//Bayonet state
#define DPSDK_EVENT_BAYONET_PICINFO_RECIVE				120		// Receive picture information
#define DPSDK_EVENT_BAYONET_PICDATA_RECIVE				121		// Receive picture data
#define DPSDK_EVENT_BAYONET_PICDATA_OVER				122		// A set of picture data is over
#define DPSDK_EVENT_BAYONET_RTP_CLOSE					123		// Rtp connection break
#define DPSDK_EVENT_BAYONET_SURVEY_ALARM				124		// Bayonet survey alarm

//Download state
#define DPSDK_EVENT_DOWNLOAD_PROGRESS					130		//Download progress
#define DPSDK_EVENT_DOWNLOAD_CUT_FILE					131		//Split is finished
#define DPSDK_EVENT_DOWNLOAD_RENDER_OVER				132		//Decoding is finished
#define DPSDK_EVENT_DOWNLOAD_FILE_STARTPLAYBACK			133		//Start download by file
#define DPSDK_EVENT_DOWNLOAD_TIME_STARTPLAYBACK			134		//Start download by time
#define DPSDK_EVENT_DOWNLOAD_VIDEO_EXPECTION			135		//Abnormal video stream
#define DPSDK_EVENT_DOWNLOAD_PAUSE_PLAYBACK				136		//Pause download
#define DPSDK_EVENT_DOWNLOAD_RESUME_PALYBACK			137		//Continue download
#define DPSDK_EVENT_DOWNLOAD_SEEK_PLAYBACK				138		//Locate download
#define DPSDK_EVENT_DOWNLOAD_STOP_PLAYBACK				139		//Stop download
#define DPSDK_EVENT_DOWNLOAD_RECEIVE_FIRST_DATA			140		//Obtain data
#define DPSDK_EVENT_DOWNLOAD_SS_EXPECTION				141		//SS abnormality		
#define DPSDK_EVENT_DOWNLOAD_RTSP_DISCONNECT			142		//RTSP disconnected
#define DPSDK_EVENT_DOWNLOAD_GET_NEXT_FILE				143		//Obtain next file, to be used by central record playing by file
#define DPSDK_EVENT_DOWNLOAD_GET_NEXT_TIME				144		//Obtain next time, to be used playing by time

#define DPSDK_EVENT_DELETE_NETDISK						150		//Delete netdisk
#define DPSDK_EVENT_ADD_NETDISK							151		//Add netdisk
#define DPSDK_EVENT_MODIFY_NETDISK						152		//Modify netdisk
#define DPSDK_EVENT_ADD_VIRTUALDISK						153		//Add virtual disk
#define DPSDK_EVENT_DELETE_VIRTUALDISK					154		//Add virtual disk

#define DPSDK_EVENT_MODIFY_USER							160		//Modify user
#define DPSDK_EVENT_TVWALL_STATUS						161		//TV wall status

#define DPSDK_EVENT_MODIFY_SERVICEPORT					170		//Modify service port
#define DPSDK_EVENT_MODIFY_SERVICEREGISTER				171		//Modify internal and external network mapping

#define DPSDK_EVENT_DEVICE_SEARCH_STATUS				175		//Obtain channel info status of searching device
#define DPSDK_EVENT_COPY_ALARM_PLAN						176		//Copy alarm plan
#define DPSDK_EVENT_ADD_GROUP_RIGHTS					177		//Add group right
#define DPSDK_EVENT_DEL_GROUP_RIGHTS					178		//Delete group right

#define DPSDK_EVENT_ADD_STORE							180		// Add store
#define DPSDK_EVENT_MODIFY_STORE						181		// Modify store
#define DPSDK_EVENT_DELETE_STORE						182		// Delete store
#define DPSDK_EVENT_MOVE_STORE							183		// Move store

// Door
#define DPSDK_EVENT_ADD_MULTICARD						190		// Add multi-card open config
#define DPSDK_EVENT_MODIFY_MULTICARD					191		// Update multi-card open config
#define DPSDK_EVENT_DELETE_MULTICARD					192		// Delete multi-card open config
#define DPSDK_EVENT_ADD_ANTI_PASSBACK					193		// Add anti-passback config
#define DPSDK_EVENT_MODIFY_ANTI_PASSBACK				194		// Update anti-passback config
#define DPSDK_EVENT_DELETE_ANTI_PASSBACK				195		// Delete anti-passback config
#define DPSDK_EVENT_ADD_FIRSTCARD						196		// Add first-card open config
#define DPSDK_EVENT_MODIFY_FIRSTCARD					197		// Update first-card open config
#define DPSDK_EVENT_DELETE_FIRSTCARD					198		// Delete first-card open config
#define DPSDK_EVENT_ADD_INTERDOOR_LOCK					199		// Add multi-door interlocking config
#define DPSDK_EVENT_MODIFY_INTERDOOR_LOCK				200		// Update multi-door interlocking config
#define DPSDK_EVENT_DELETE_INTERDOOR_LOCK				201		// Delete multi-door interlocking config
#define DPSDK_EVENT_ADD_PERSON							202		// Add person
#define DPSDK_EVENT_UPDATE_PERSON						203		// Update person
#define DPSDK_EVENT_DELETE_PERSON						204		// Delete person
#define DPSDK_EVENT_ADD_PERSONGROUP						205		// Add person group
#define DPSDK_EVENT_UPDATE_PERSONGROUP					206		// Update person group
#define DPSDK_EVENT_DELETE_PERSONGROUP					207		// Delete person group
#define DPSDK_EVENT_ADD_CARD_RIGHT						208		// Add door right
#define DPSDK_EVENT_ADD_DEPARTMENT						209		// Add department
#define DPSDK_EVENT_UPDATE_DEPARTMENT					210		// Update department
#define DPSDK_EVENT_DELETE_DEPARTMENT					211		// Delete department
#define DPSDK_EVENT_ADD_DOORGROUP						212		// Add door group
#define DPSDK_EVENT_UPDATE_DOORGROUP					213		// Update door group
#define DPSDK_EVENT_DELETE_DOORGROUP					214		// Delete door group
#define DPSDK_EVENT_ADD_DOORGROUP_PERSON				215		// Add person and door group bonding
#define DPSDK_EVENT_UPDATE_DOORGROUP_PERSON				216		// Update person and door group bonding
#define DPSDK_EVENT_UPDATE_GlOBAL_CONTROL_CONFIG		217		// Update config of global open channel
#define DPSDK_EVENT_ADD_REMOTEVERIFICATION				218		// Add remote verification config
#define DPSDK_EVENT_UPDATE_REMOTEVERIFICATION			219		// Update remote verification config
#define DPSDK_EVENT_DELETE_REMOTEVERIFICATION			220		// Delete remote verification config
#define DPSDK_EVENT_ADD_DOOR_RULE						221		// Add door rule
#define DPSDK_EVENT_UPDATE_DOOR_RULE					222		// Update door rule
#define DPSDK_EVENT_DELETE_DOOR_RULE					223		// Delete door rule
#define DPSDK_EVENT_CHANGE_SERVER_DEVICE				224		// Device switch service
#define DPSDK_EVENT_ADD_HOLIDAY							225		// Add holiday config
#define DPSDK_EVENT_UPDATE_HOLIDAY						226		// Update holiday config
#define DPSDK_EVENT_DELETE_HOLIDAY						227		// Delete holiday config
#define DPSDK_EVENT_ADD_HOLIDAYGROUP					228		// Add holiday group config
#define DPSDK_EVENT_UPDATE_HOLIDAYGROUP					229		// Update holiday group config
#define DPSDK_EVENT_DELETE_HOLIDAYGROUP					230		// Delete holiday group config
#define DPSDK_EVENT_DOORACESS_RECORD					231		// Send alarm notice to users with rights of this device
#define DPSDK_EVENT_DOORACESS_STATUS					232		// Access status change
#define DPSDK_EVENT_ADD_DOOR_CONFIG						233		// Basic config of adding door
#define DPSDK_EVENT_MODIFY_DOOR_CONFIG					234		// Basic config of updating door
#define DPSDK_EVENT_ADD_ALARMLINK_CONFIG				235		// Add access alarm linkage config
#define DPSDK_EVENT_MODIFY_ALARMLINK_CONFIG				236		// Update access alarm linkage config
#define DPSDK_EVENT_CAPTURE_FINGERPRINT					237		// Collect fingerprint
#define DPSDK_EVENT_DOOR_ALAWYSOPENCLOSE				238		// NO/NC message notice
#define DPSDK_EVENT_DOOR_GLOBAL_CONTROL_STATUS			239		// Door global NO/NC operation notice
#define DPSDK_EVENT_ISSUE_PERSONCARD_TASK_FAIL			240		// Failed to issue task. Issue notice again
#define DPSDK_EVENT_DOOR_GLOBALCTRL_FAILED_CHANNELS_NOTIFY	241
#define DPSDK_EVENT_BATCH_ADD_PERSON					242
#define DPSDK_EVENT_IMPORT_PERSON						243
#define DPSDK_EVENT_EXPORT_PERSON						244
#define DPSDK_EVENT_LICENSE_EXPIRED						245
#define DPSDK_EVENT_HUMAN_INFO							246
#define DPSDK_EVENT_VEHICLE_INFO						247
#define DPSDK_EVENT_NONVEHICLE_INFO						248
#define DPSDK_EVENT_ATTENDANCE_ADD_ORDER				249
#define DPSDK_EVENT_ATTENDANCE_MODIFY_ORDER				250
#define DPSDK_EVENT_ATTENDANCE_DEL_ORDER				251
#define DPSDK_EVENT_ATTENDANCE_ADD_TIMEINTERVAL		    252
#define DPSDK_EVENT_ATTENDANCE_MODIFY_TIMEINTERVAL		253
#define DPSDK_EVENT_ATTENDANCE_DEL_TIMEINTERVAL			254
#define DPSDK_EVENT_ATTENDANCE_MODIFY_CHANNELS			255
#define DPSDK_EVENT_ATTENDANCE_MODIFY_PERSON_SCHEDULE	256
#define DPSDK_EVENT_ATTENDANCE_MODIFY_STATISRULE		257
#define DPSDK_EVENT_ATTENDANCE_MODIFY_DEPT_SCHEDULE		258
#define DPSDK_EVENT_ISSUE_PERSONCARD_RULE_TASK_FAIL		259		// Failed to issue task. Issue notice again
#define DPSDK_EVENT_FILEDOWNLOAD_URL					260



/*
* VtCall
*/
#define DPSDK_EVENT_ADD_DEV_GROUP						400		// Add device group
#define DPSDK_EVENT_MODIFY_DEV_GROUP					401		// Modify device group
#define DPSDK_EVENT_DEL_DEV_GROUP						402		// Delete device group
#define DPSDK_EVENT_ADD_MANAGER_GROUP					403		// Add management group
#define DPSDK_EVENT_MODIFY_MANAGER_GROUP				404		// Modify management group
#define DPSDK_EVENT_DEL_MANAGER_GROUP					405		// Delete management group
#define DPSDK_EVENT_ADD_GROUP_RELATION					406		// Add group relation
#define DPSDK_EVENT_MODIFY_GROUP_RELATION				407		// Modify group relation
#define DPSDK_EVENT_DEL_GROUP_RELATION					408		// Delete group relation
#define DPSDK_EVENT_ADD_INFO_PUBLISH					409		// Add info release
#define DPSDK_EVENT_MODIFY_INFO_PUBLISH					410		// Modify info release
#define DPSDK_EVENT_DEL_INFO_PUBLISH					411		// Delete info release
#define DPSDK_EVENT_VIDEO_INTERCOMUNIT_ENABLE			412		// Enable visual intercom device unit
#define DPSDK_EVENT_VIDEO_INTERCOMBUILDING_ENABLE		413		// Enable visual intercom building
#define DPSDK_EVENT_ADD_CALLLOG							414		// Notify present client immediately after a call record is generated
#define DPSDK_EVENT_ADD_DEV_SIPID						415		// Add a device, notify the client about obtained SIPID  
#define DPSDK_EVENT_UPDATE_DEV_SIPID					416		// Update a device, notify the client about obtained SIPID
#define DPSDK_EVENT_DEL_DEV_SIPID					    417		// Delete a device, notify the client about obtained SIPID
#define DPSDK_EVENT_SYNC_CONTACT					    418		// The result of synchronize contact

#define DPSDK_EVENT_SCS_STATUS							430		// SCS status change
#define DPSDK_EVENT_VTCALL_INVITE						431		// Visual intercom call invitation notice
#define DPSDK_EVENT_VTCALL_BYE							432		// Visual intercom call hangup notice
#define DPSDK_EVENT_VTCALL_RING							433		// Visual intercom call ringing notice
#define DPSDK_EVENT_VTCALL_CANCEL						434		// Notice that calling party cancels call
#define DPSDK_EVENT_VTCALL_BUSY							435		// Visual intercom call busy notice
#define DPSDK_EVENT_CALL_INVITE							441		// Intercom call invitation notice
#define DPSDK_EVENT_SCS_MESSAGE							442		// SCS json message

// Alarm host
#define DPSDK_EVENT_ALARMHOST_DEFENCEAREASTATUS			500		// Zone status notice
#define DPSDK_EVENT_ALARMHOST_SUBSYSTEMSTATUS			501		// Subsystem status notice
#define DPSDK_EVENT_ALARMHOST_STATUS					502		// Alarm host status notice
#define DPSDK_EVENT_REFRESH_ALARMHOST_SUBINFO			503		// Alarm host refresh notice



#endif