#ifndef DPSDK_DPSDKERROR_H
#define DPSDK_DPSDKERROR_H

// General error[-99£¬0]
#define	DPSDK_SUCCESS						0				// Successful
#define DPSDK_FAILED						-1				// Failed
#define	DPSDK_UNKNOWN_ERROR			        -2		        // Unknown error
#define	DPSDK_FUNCTION_CALL_ERROR			-3		        // Internal function calling error
#define	DPSDK_INVAILD_PARAM					-4			    // Illegal parameter
#define DPSDK_NO_INIT						-5				// System uninitialized
#define DPSDK_USER_LOGINED					-6				// User is logged in
#define DPSDK_USER_INVALID					-7				// Invalid user. User session ID is invalid
#define DPSDK_USER_NOLOGINED				-8				// User not logged in
#define DPSDK_BUFFER_NOT_ENOUGH				-9				// Buffer insufficient
#define DPSDK_MQ_INITIALIZD					-10				// MQ initialized
#define DPSDK_INVALID_DATA					-11				// Invalid data
#define DPSDK_MALLOC_FAILED					-12				// Memory distribution failed
#define DPSDK_XML_INVALID					-13				// Invalid XML data
#define DPSDK_SERVER_TYPE_UNMATCH			-14				// The Service type of login is mismatching

// Rest error code[-119, -100]
#define DPSDK_REST_REQUEST_TIMEOUT          -101			// Rest request timeout. This error code keeps the same as that inside Rest

// Preview error [-299£¬-200]
#define DPSDK_INVAILD_MEDIA_SESSIONID		-200			// Invalid media session ID (including preview, playback, intercom)
#define DPSDK_OPEN_SOUND_FAILED				-201			// Open audio failed
#define	DPSDK_TALKING_WITH_OTHER			-202			// Open audio failed. In intercom with other device
#define DPSDK_MEDIA_TYPE_UNMATCH			-203			// Media type mismatching

// Talk error[-349£¬-300]
#define DPSDK_TALK_PARAM_UNMATCH			-300			// Intercom parameter mismatching

// RRSPSDK error code[-499, -400]
#define DPSDK_RTSP_UDP_LISTEN_FAILED		-400			// UDP monitor failed
#define DPSDK_RTSP_CONNECT_SERVER_FAILED	-401			// Link service failed
#define DPSDK_RTSP_SENDDATA_FAILED			-402			// Data transmission failed
#define DPSDK_RTSP_COMMAND_CANCELED			-403			// Order is cancelled
#define DPSDK_RTSP_PORT_PARSE_FAILED		-404			// Port analysis failed
#define DPSDK_RTSP_SETUP_TIMEOUT			-405			// RTSP setup timeout
#define DPSDK_RTSP_PLAY_TIMEOUT				-406			// RTSP play timeout

//Sip error code 
#define DPSDK_SIP_SCS_DATA_PARSE_ERROR		-500			// SCS data analysis failed
#define DPSDK_SIP_COMMAND_CANCELED			-501			// Order is cancelled

#endif // _DPSDK_DPSDKERROR_H_
