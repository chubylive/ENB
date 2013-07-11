//GAP opcode
#define GAP_DeviceInit 						0xFE00
#define GAP_DeviceDiscoveryRequest 			0xFE04
#define GAP_GetParam 						0xFE31
#define GAP_SetParam 						0xFE30
#define GAP_ConfigDeviceAddr 				0xFE03
#define GAP_EndDiscoverable 				0xFE08
#define GAP_UpdateAdvertisingData 			0xFE07
#define GAP_MakeDiscoverable 				0xFE06

//GAP Event Code
#define HCI_LE_ExtEvent 					0xFF


//GAP Event
#define GAP_HCI_ExtentionCommandStatus 		0x067F
#define GAP_DeviceInitDone				 	0x0600
#define GAP_DeviceDiscoveryDone 			0x0601
#define GAP_DeviceInformation 				0x060D
#define GAP_DeviceDiscoveryDone 			0x0601
#define GAP_AdvertDataUpdate				0x0602
#define GAP_EndDiscoverable_Event 			0x0604 	

//GAP Profile Roles
#define GAP_PROFILE_BROADCASTER   0x01 //!< A device that sends advertising events only.
#define GAP_PROFILE_OBSERVER      0x02 //!< A device that receives advertising events only.
#define GAP_PROFILE_PERIPHERAL    0x04 //!< A device that accepts the establishment of an LE physical link using the connection establishment procedure
#define GAP_PROFILE_CENTRAL       0x08 //!< A device that supports the Central role initiates the establishment of a physical connection
