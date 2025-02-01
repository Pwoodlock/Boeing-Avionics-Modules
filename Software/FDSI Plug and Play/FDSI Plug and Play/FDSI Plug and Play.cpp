// FDSI Plug and Play.cpp : main project file.

#include "stdafx.h"
#include "Form1.h"
#include <stdio.h>


//pushbutton toggle
bool COMTEST1Toggle = false;
bool COMTEST2Toggle = false;
bool NAVTEST1Toggle = false;
bool NAVTEST2Toggle = false;
bool ADFTEST1Toggle = false;
bool ADFTEST2Toggle = false;

//devices present
unsigned int uiCOM1Exsits=0;
unsigned int uiCOM2Exsits=0;
unsigned int uiNAV1Exsits=0;
unsigned int uiNAV2Exsits=0;
unsigned int uiADF1Exsits=0;
unsigned int uiADF2Exsits=0;

unsigned int COM1isCOM1=0;
unsigned int COM2isCOM2=0;
unsigned int NAV1isNAV1=0;
unsigned int NAV2isNAV2=0;
unsigned int ADF1isADF1=0;
unsigned int ADF2isADF2=0;

unsigned int COM1isCOM2=0;
unsigned int COM2isCOM1=0;
unsigned int NAV1isNAV2=0;
unsigned int NAV2isNAV1=0;
unsigned int ADF1isADF2=0;
unsigned int ADF2isADF1=0;

//display values
unsigned int uiCOMData=0;
unsigned int uiCOMStandbyData=0;
unsigned int uiNAVData=0;
unsigned int uiNAVStandbyData=0;
//unsigned int uiCOMTFRData=0;
unsigned int uiADFData=0;
unsigned int uiADFDataEx=0;
//switch offsets - read from ini file
DWORD ulADF1ANTOffset=0;
DWORD ulADF2ANTOffset=0;
DWORD ulADF1TONEOffset=0;
DWORD ulADF2TONEOffset=0;
DWORD ulCOMTEST1Offset=0;
DWORD ulCOMTEST2Offset=0;
DWORD ulNAVTEST1Offset=0;
DWORD ulNAVTEST2Offset=0;
//switch values
unsigned int ulADF1Value1=0;
unsigned int ulADF1Value2=0;
unsigned int ulADF2Value1=0;
unsigned int ulADF2Value2=0;
unsigned int ulTONE1Value1=0;
unsigned int ulTONE1Value2=0;
unsigned int ulTONE2Value1=0;
unsigned int ulTONE2Value2=0;
unsigned int ulCOMTEST1Value1=0;
unsigned int ulCOMTEST1Value2=0;
unsigned int ulCOMTEST2Value1=0;
unsigned int ulCOMTEST2Value2=0;
unsigned int ulNAVTEST1Value1=0;
unsigned int ulNAVTEST1Value2=0;
unsigned int ulNAVTEST2Value1=0;
unsigned int ulNAVTEST2Value2=0;

// Added for version 1.1.0
// The following Arrays contain offsets and values for the Advanced Tab
// dropdown selection boxes

//list of offsets used for buttons
#define DME_SELECT  0x0378
#define GPS_NAV		0x132C
#define RADIO_SOUND	0x3122
#define NO_ERR		0xEEEE	 // used to send the "No Err" to the COM/NAV Panels

DWORD AdvancedOffsetArray[11]={DME_SELECT,GPS_NAV,RADIO_SOUND,RADIO_SOUND,RADIO_SOUND,RADIO_SOUND,RADIO_SOUND,RADIO_SOUND,RADIO_SOUND,RADIO_SOUND,NO_ERR};


char test[10];


using namespace FDSIPlugandPlay;
using namespace System;
using namespace System::Text;
using namespace System::IO;
using namespace std;


//Modify this value to match the VID and PID in your USB device descriptor.
//Use the formatting: "Vid_xxxx&Pid_xxxx" where xxxx is a 16-bit hexadecimal number.
//Here we will list a DEVICE ID for each module type that is produced
#define COM_DEVICE_ID  "Vid_16d0&Pid_0420"	
#define NAV_DEVICE_ID  "Vid_16d0&Pid_0421"	
#define ADF_DEVICE_ID  "Vid_16d0&Pid_0422"	

//  Variables that need to have wide scope.
	HANDLE COM1WriteHandle = INVALID_HANDLE_VALUE;	//Need to get a write "handle" to our device before we can write to it.
	HANDLE COM1ReadHandle = INVALID_HANDLE_VALUE;	//Need to get a read "handle" to our device before we can read from it.
	HANDLE COM2WriteHandle = INVALID_HANDLE_VALUE;	//Need to get a write "handle" to our device before we can write to it.
	HANDLE COM2ReadHandle = INVALID_HANDLE_VALUE;	//Need to get a read "handle" to our device before we can read from it.
	HANDLE NAV1WriteHandle = INVALID_HANDLE_VALUE;	//Need to get a write "handle" to our device before we can write to it.
	HANDLE NAV1ReadHandle = INVALID_HANDLE_VALUE;	//Need to get a read "handle" to our device before we can read from it.
	HANDLE NAV2WriteHandle = INVALID_HANDLE_VALUE;	//Need to get a write "handle" to our device before we can write to it.
	HANDLE NAV2ReadHandle = INVALID_HANDLE_VALUE;	//Need to get a read "handle" to our device before we can read from it.
	HANDLE ADF1WriteHandle = INVALID_HANDLE_VALUE;	//Need to get a write "handle" to our device before we can write to it.
	HANDLE ADF1ReadHandle = INVALID_HANDLE_VALUE;	//Need to get a read "handle" to our device before we can read from it.
	HANDLE ADF2WriteHandle = INVALID_HANDLE_VALUE;	//Need to get a write "handle" to our device before we can write to it.
	HANDLE ADF2ReadHandle = INVALID_HANDLE_VALUE;	//Need to get a read "handle" to our device before we can read from it.



[STAThreadAttribute]
int main(array<System::String ^> ^args)
{
	// Enabling Windows XP visual effects before any controls are created
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false); 

	// Create the main window and run it
	Application::Run(gcnew Form1());
	return 0;
}



int DetectCOMModule(void)
{
		//Globally Unique Identifier (GUID) for HID class devices.  Windows uses GUIDs to identify things.
						GUID InterfaceClassGuid = {0x4d1e55b2, 0xf16f, 0x11cf, 0x88, 0xcb, 0x00, 0x11, 0x11, 0x00, 0x00, 0x30}; 

						HDEVINFO DeviceInfoTable = INVALID_HANDLE_VALUE;
						PSP_DEVICE_INTERFACE_DATA InterfaceDataStructure = new SP_DEVICE_INTERFACE_DATA;
						PSP_DEVICE_INTERFACE_DETAIL_DATA DetailedInterfaceDataStructure = new SP_DEVICE_INTERFACE_DETAIL_DATA;
						SP_DEVINFO_DATA DevInfoData;

						DWORD InterfaceIndex = 0;
						DWORD StatusLastError = 0;
						DWORD dwRegType;
						DWORD dwRegSize;
						DWORD StructureSize = 0;
						PBYTE PropertyValueBuffer;
						bool MatchFound = false;
						DWORD ErrorStatus;
						unsigned char numDevicesDetected=0;

						String^ DeviceIDToFind = COM_DEVICE_ID;

						//First populate a list of plugged in devices (by specifying "DIGCF_PRESENT"), which are of the specified class GUID. 
						DeviceInfoTable = SetupDiGetClassDevsUM(&InterfaceClassGuid, NULL, NULL, DIGCF_PRESENT | DIGCF_DEVICEINTERFACE);

						//Now look through the list we just populated.  We are trying to see if any of them match our device. 
						while(true)
						{
							InterfaceDataStructure->cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);
							SetupDiEnumDeviceInterfacesUM(DeviceInfoTable, NULL, &InterfaceClassGuid, InterfaceIndex, InterfaceDataStructure);
							ErrorStatus = GetLastError();
							if(ERROR_NO_MORE_ITEMS == GetLastError())	//Did we reach the end of the list of matching devices in the DeviceInfoTable?
							{	//Cound not find the device.  Must not have been attached.
								SetupDiDestroyDeviceInfoListUM(DeviceInfoTable);	//Clean up the old structure we no longer need.
								return numDevicesDetected;		//reached end of list so return numdevices
							}
							

							//Now retrieve the hardware ID from the registry.  The hardware ID contains the VID and PID, which we will then 
							//check to see if it is the correct device or not.

							//Initialize an appropriate SP_DEVINFO_DATA structure.  We need this structure for SetupDiGetDeviceRegistryProperty().
							DevInfoData.cbSize = sizeof(SP_DEVINFO_DATA);
							SetupDiEnumDeviceInfoUM(DeviceInfoTable, InterfaceIndex, &DevInfoData);

							//First query for the size of the hardware ID, so we can know how big a buffer to allocate for the data.
							SetupDiGetDeviceRegistryPropertyUM(DeviceInfoTable, &DevInfoData, SPDRP_HARDWAREID, &dwRegType, NULL, 0, &dwRegSize);

							//Allocate a buffer for the hardware ID.
							PropertyValueBuffer = (BYTE *) malloc (dwRegSize);
							if(PropertyValueBuffer == NULL)	//if null, error, couldn't allocate enough memory
							{	//Can't really recover from this situation, just exit instead.
								SetupDiDestroyDeviceInfoListUM(DeviceInfoTable);	//Clean up the old structure we no longer need.
								return 0;		
							}

							//Retrieve the hardware IDs for the current device we are looking at.  PropertyValueBuffer gets filled with a 
							//REG_MULTI_SZ (array of null terminated strings).  To find a device, we only care about the very first string in the
							//buffer, which will be the "device ID".  The device ID is a string which contains the VID and PID, in the example 
							//format "Vid_04d8&Pid_003f".
							SetupDiGetDeviceRegistryPropertyUM(DeviceInfoTable, &DevInfoData, SPDRP_HARDWAREID, &dwRegType, PropertyValueBuffer, dwRegSize, NULL);

							//Now check if the first string in the hardware ID matches the device ID of my USB device.
							#ifdef UNICODE
							String^ DeviceIDFromRegistry = gcnew String((wchar_t *)PropertyValueBuffer);
							#else
							String^ DeviceIDFromRegistry = gcnew String((char *)PropertyValueBuffer);
							#endif
							//Convert both strings to lower case.  This makes the code more robust/portable accross OS Versions
							DeviceIDFromRegistry = DeviceIDFromRegistry->ToLowerInvariant();	
							DeviceIDToFind = DeviceIDToFind->ToLowerInvariant();				
							//Now check if the hardware ID we are looking at contains the correct VID/PID
							MatchFound = DeviceIDFromRegistry->Contains(DeviceIDToFind);		
							if(MatchFound == true)
							{


								numDevicesDetected++;
								//Device must have been found.  Open read and write handles.  In order to do this, we will need the actual device path first.
								//We can get the path by calling SetupDiGetDeviceInterfaceDetail(), however, we have to call this function twice:  The first
								//time to get the size of the required structure/buffer to hold the detailed interface data, then a second time to actually 
								//get the structure (after we have allocated enough memory for the structure.)
								DetailedInterfaceDataStructure->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);
								//First call populates "StructureSize" with the correct value
								SetupDiGetDeviceInterfaceDetailUM(DeviceInfoTable, InterfaceDataStructure, NULL, NULL, &StructureSize, NULL);	
								DetailedInterfaceDataStructure = (PSP_DEVICE_INTERFACE_DETAIL_DATA)(malloc(StructureSize));		//Allocate enough memory
								if(DetailedInterfaceDataStructure == NULL)	//if null, error, couldn't allocate enough memory
								{	//Can't really recover from this situation, just exit instead.
									SetupDiDestroyDeviceInfoListUM(DeviceInfoTable);	//Clean up the old structure we no longer need.
									return 0;		
								}
								DetailedInterfaceDataStructure->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);
								 //Now call SetupDiGetDeviceInterfaceDetail() a second time to receive the goods.  
								SetupDiGetDeviceInterfaceDetailUM(DeviceInfoTable, InterfaceDataStructure, DetailedInterfaceDataStructure, StructureSize, NULL, NULL); 

								//We now have the proper device path, and we can finally open read and write handles to the device.
								//We store the handles in the global variables "WriteHandle" and "ReadHandle", which we will use later to actually communicate.
								if(numDevicesDetected==1)
									COM1WriteHandle = CreateFile((DetailedInterfaceDataStructure->DevicePath), GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, 0);

								if(numDevicesDetected==2)
									COM2WriteHandle = CreateFile((DetailedInterfaceDataStructure->DevicePath), GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, 0);

								ErrorStatus = GetLastError();
								if(ErrorStatus == ERROR_SUCCESS)
								{
									//ToggleLED_btn->Enabled = true;				//Make button no longer greyed out
								}

								if(numDevicesDetected==1)
									COM1ReadHandle = CreateFile((DetailedInterfaceDataStructure->DevicePath), GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, 0);
								
								if(numDevicesDetected==2)
									COM2ReadHandle = CreateFile((DetailedInterfaceDataStructure->DevicePath), GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, 0);

								ErrorStatus = GetLastError();
								if(ErrorStatus == ERROR_SUCCESS)
								{
									//GetPushbuttonState_btn->Enabled = true;		//Make button no longer greyed out
									//StateLabel->Enabled = true;					//Make label no longer greyed out
								}
								
								
								MatchFound = false;
							}

							if(numDevicesDetected==1)
							{
								if(InterfaceIndex==2)
								{
										SetupDiDestroyDeviceInfoListUM(DeviceInfoTable);	//Clean up the old structure we no longer need.
										return numDevicesDetected;	
								}
							}

							if(numDevicesDetected==2)
							{
								SetupDiDestroyDeviceInfoListUM(DeviceInfoTable);	//Clean up the old structure we no longer need.
								return numDevicesDetected;	
							}

							InterfaceIndex++;	

							//Keep looping until we either find a device with matching VID and PID, or until we run out of items.
						}//end of while(true)
}	

int DetectNAVModule(void)
{
		//Globally Unique Identifier (GUID) for HID class devices.  Windows uses GUIDs to identify things.
						GUID InterfaceClassGuid = {0x4d1e55b2, 0xf16f, 0x11cf, 0x88, 0xcb, 0x00, 0x11, 0x11, 0x00, 0x00, 0x30}; 

						HDEVINFO DeviceInfoTable = INVALID_HANDLE_VALUE;
						PSP_DEVICE_INTERFACE_DATA InterfaceDataStructure = new SP_DEVICE_INTERFACE_DATA;
						PSP_DEVICE_INTERFACE_DETAIL_DATA DetailedInterfaceDataStructure = new SP_DEVICE_INTERFACE_DETAIL_DATA;
						SP_DEVINFO_DATA DevInfoData;

						DWORD InterfaceIndex = 0;
						DWORD StatusLastError = 0;
						DWORD dwRegType;
						DWORD dwRegSize;
						DWORD StructureSize = 0;
						PBYTE PropertyValueBuffer;
						bool MatchFound = false;
						DWORD ErrorStatus;
						unsigned char numDevicesDetected=0;

						String^ DeviceIDToFind = NAV_DEVICE_ID;

						//First populate a list of plugged in devices (by specifying "DIGCF_PRESENT"), which are of the specified class GUID. 
						DeviceInfoTable = SetupDiGetClassDevsUM(&InterfaceClassGuid, NULL, NULL, DIGCF_PRESENT | DIGCF_DEVICEINTERFACE);

						//Now look through the list we just populated.  We are trying to see if any of them match our device. 
						while(true)
						{
							InterfaceDataStructure->cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);
							SetupDiEnumDeviceInterfacesUM(DeviceInfoTable, NULL, &InterfaceClassGuid, InterfaceIndex, InterfaceDataStructure);
							ErrorStatus = GetLastError();
							if(ERROR_NO_MORE_ITEMS == GetLastError())	//Did we reach the end of the list of matching devices in the DeviceInfoTable?
							{	//Cound not find the device.  Must not have been attached.
								SetupDiDestroyDeviceInfoListUM(DeviceInfoTable);	//Clean up the old structure we no longer need.
								return numDevicesDetected;		//reached end of list so return numdevices
							}
							

							//Now retrieve the hardware ID from the registry.  The hardware ID contains the VID and PID, which we will then 
							//check to see if it is the correct device or not.

							//Initialize an appropriate SP_DEVINFO_DATA structure.  We need this structure for SetupDiGetDeviceRegistryProperty().
							DevInfoData.cbSize = sizeof(SP_DEVINFO_DATA);
							SetupDiEnumDeviceInfoUM(DeviceInfoTable, InterfaceIndex, &DevInfoData);

							//First query for the size of the hardware ID, so we can know how big a buffer to allocate for the data.
							SetupDiGetDeviceRegistryPropertyUM(DeviceInfoTable, &DevInfoData, SPDRP_HARDWAREID, &dwRegType, NULL, 0, &dwRegSize);

							//Allocate a buffer for the hardware ID.
							PropertyValueBuffer = (BYTE *) malloc (dwRegSize);
							if(PropertyValueBuffer == NULL)	//if null, error, couldn't allocate enough memory
							{	//Can't really recover from this situation, just exit instead.
								SetupDiDestroyDeviceInfoListUM(DeviceInfoTable);	//Clean up the old structure we no longer need.
								return 0;		
							}

							//Retrieve the hardware IDs for the current device we are looking at.  PropertyValueBuffer gets filled with a 
							//REG_MULTI_SZ (array of null terminated strings).  To find a device, we only care about the very first string in the
							//buffer, which will be the "device ID".  The device ID is a string which contains the VID and PID, in the example 
							//format "Vid_04d8&Pid_003f".
							SetupDiGetDeviceRegistryPropertyUM(DeviceInfoTable, &DevInfoData, SPDRP_HARDWAREID, &dwRegType, PropertyValueBuffer, dwRegSize, NULL);

							//Now check if the first string in the hardware ID matches the device ID of my USB device.
							#ifdef UNICODE
							String^ DeviceIDFromRegistry = gcnew String((wchar_t *)PropertyValueBuffer);
							#else
							String^ DeviceIDFromRegistry = gcnew String((char *)PropertyValueBuffer);
							#endif
							//Convert both strings to lower case.  This makes the code more robust/portable accross OS Versions
							DeviceIDFromRegistry = DeviceIDFromRegistry->ToLowerInvariant();	
							DeviceIDToFind = DeviceIDToFind->ToLowerInvariant();				
							//Now check if the hardware ID we are looking at contains the correct VID/PID
							MatchFound = DeviceIDFromRegistry->Contains(DeviceIDToFind);		
							if(MatchFound == true)
							{


								numDevicesDetected++;
								//Device must have been found.  Open read and write handles.  In order to do this, we will need the actual device path first.
								//We can get the path by calling SetupDiGetDeviceInterfaceDetail(), however, we have to call this function twice:  The first
								//time to get the size of the required structure/buffer to hold the detailed interface data, then a second time to actually 
								//get the structure (after we have allocated enough memory for the structure.)
								DetailedInterfaceDataStructure->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);
								//First call populates "StructureSize" with the correct value
								SetupDiGetDeviceInterfaceDetailUM(DeviceInfoTable, InterfaceDataStructure, NULL, NULL, &StructureSize, NULL);	
								DetailedInterfaceDataStructure = (PSP_DEVICE_INTERFACE_DETAIL_DATA)(malloc(StructureSize));		//Allocate enough memory
								if(DetailedInterfaceDataStructure == NULL)	//if null, error, couldn't allocate enough memory
								{	//Can't really recover from this situation, just exit instead.
									SetupDiDestroyDeviceInfoListUM(DeviceInfoTable);	//Clean up the old structure we no longer need.
									return 0;		
								}
								DetailedInterfaceDataStructure->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);
								 //Now call SetupDiGetDeviceInterfaceDetail() a second time to receive the goods.  
								SetupDiGetDeviceInterfaceDetailUM(DeviceInfoTable, InterfaceDataStructure, DetailedInterfaceDataStructure, StructureSize, NULL, NULL); 

								//We now have the proper device path, and we can finally open read and write handles to the device.
								//We store the handles in the global variables "WriteHandle" and "ReadHandle", which we will use later to actually communicate.
								if(numDevicesDetected==1)
									NAV1WriteHandle = CreateFile((DetailedInterfaceDataStructure->DevicePath), GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, 0);

								if(numDevicesDetected==2)
									NAV2WriteHandle = CreateFile((DetailedInterfaceDataStructure->DevicePath), GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, 0);

								ErrorStatus = GetLastError();
								if(ErrorStatus == ERROR_SUCCESS)
								{
									//ToggleLED_btn->Enabled = true;				//Make button no longer greyed out
								}

								if(numDevicesDetected==1)
									NAV1ReadHandle = CreateFile((DetailedInterfaceDataStructure->DevicePath), GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, 0);
								
								if(numDevicesDetected==2)
									NAV2ReadHandle = CreateFile((DetailedInterfaceDataStructure->DevicePath), GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, 0);

								ErrorStatus = GetLastError();
								if(ErrorStatus == ERROR_SUCCESS)
								{
									//GetPushbuttonState_btn->Enabled = true;		//Make button no longer greyed out
									//StateLabel->Enabled = true;					//Make label no longer greyed out
								}
								
								
								MatchFound = false;
							}

							if(numDevicesDetected==1)
							{
								if(InterfaceIndex==2)
								{
										SetupDiDestroyDeviceInfoListUM(DeviceInfoTable);	//Clean up the old structure we no longer need.
										return numDevicesDetected;	
								}
							}

							if(numDevicesDetected==2)
							{
								SetupDiDestroyDeviceInfoListUM(DeviceInfoTable);	//Clean up the old structure we no longer need.
								return numDevicesDetected;	
							}

							InterfaceIndex++;	

							//Keep looping until we either find a device with matching VID and PID, or until we run out of items.
						}//end of while(true)	
						
}	

int DetectADFModule(void)
{
		//Globally Unique Identifier (GUID) for HID class devices.  Windows uses GUIDs to identify things.
						GUID InterfaceClassGuid = {0x4d1e55b2, 0xf16f, 0x11cf, 0x88, 0xcb, 0x00, 0x11, 0x11, 0x00, 0x00, 0x30}; 

						HDEVINFO DeviceInfoTable = INVALID_HANDLE_VALUE;
						PSP_DEVICE_INTERFACE_DATA InterfaceDataStructure = new SP_DEVICE_INTERFACE_DATA;
						PSP_DEVICE_INTERFACE_DETAIL_DATA DetailedInterfaceDataStructure = new SP_DEVICE_INTERFACE_DETAIL_DATA;
						SP_DEVINFO_DATA DevInfoData;

						DWORD InterfaceIndex = 0;
						DWORD StatusLastError = 0;
						DWORD dwRegType;
						DWORD dwRegSize;
						DWORD StructureSize = 0;
						PBYTE PropertyValueBuffer;
						bool MatchFound = false;
						DWORD ErrorStatus;
						unsigned char numDevicesDetected=0;

						String^ DeviceIDToFind = ADF_DEVICE_ID;

						//First populate a list of plugged in devices (by specifying "DIGCF_PRESENT"), which are of the specified class GUID. 
						DeviceInfoTable = SetupDiGetClassDevsUM(&InterfaceClassGuid, NULL, NULL, DIGCF_PRESENT | DIGCF_DEVICEINTERFACE);

						//Now look through the list we just populated.  We are trying to see if any of them match our device. 
						while(true)
						{
							InterfaceDataStructure->cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);
							SetupDiEnumDeviceInterfacesUM(DeviceInfoTable, NULL, &InterfaceClassGuid, InterfaceIndex, InterfaceDataStructure);
							ErrorStatus = GetLastError();
							if(ERROR_NO_MORE_ITEMS == GetLastError())	//Did we reach the end of the list of matching devices in the DeviceInfoTable?
							{	//Cound not find the device.  Must not have been attached.
								SetupDiDestroyDeviceInfoListUM(DeviceInfoTable);	//Clean up the old structure we no longer need.
								return numDevicesDetected;		//reached end of list so return numdevices
							}
							

							//Now retrieve the hardware ID from the registry.  The hardware ID contains the VID and PID, which we will then 
							//check to see if it is the correct device or not.

							//Initialize an appropriate SP_DEVINFO_DATA structure.  We need this structure for SetupDiGetDeviceRegistryProperty().
							DevInfoData.cbSize = sizeof(SP_DEVINFO_DATA);
							SetupDiEnumDeviceInfoUM(DeviceInfoTable, InterfaceIndex, &DevInfoData);

							//First query for the size of the hardware ID, so we can know how big a buffer to allocate for the data.
							SetupDiGetDeviceRegistryPropertyUM(DeviceInfoTable, &DevInfoData, SPDRP_HARDWAREID, &dwRegType, NULL, 0, &dwRegSize);

							//Allocate a buffer for the hardware ID.
							PropertyValueBuffer = (BYTE *) malloc (dwRegSize);
							if(PropertyValueBuffer == NULL)	//if null, error, couldn't allocate enough memory
							{	//Can't really recover from this situation, just exit instead.
								SetupDiDestroyDeviceInfoListUM(DeviceInfoTable);	//Clean up the old structure we no longer need.
								return 0;		
							}

							//Retrieve the hardware IDs for the current device we are looking at.  PropertyValueBuffer gets filled with a 
							//REG_MULTI_SZ (array of null terminated strings).  To find a device, we only care about the very first string in the
							//buffer, which will be the "device ID".  The device ID is a string which contains the VID and PID, in the example 
							//format "Vid_04d8&Pid_003f".
							SetupDiGetDeviceRegistryPropertyUM(DeviceInfoTable, &DevInfoData, SPDRP_HARDWAREID, &dwRegType, PropertyValueBuffer, dwRegSize, NULL);

							//Now check if the first string in the hardware ID matches the device ID of my USB device.
							#ifdef UNICODE
							String^ DeviceIDFromRegistry = gcnew String((wchar_t *)PropertyValueBuffer);
							#else
							String^ DeviceIDFromRegistry = gcnew String((char *)PropertyValueBuffer);
							#endif
							//Convert both strings to lower case.  This makes the code more robust/portable accross OS Versions
							DeviceIDFromRegistry = DeviceIDFromRegistry->ToLowerInvariant();	
							DeviceIDToFind = DeviceIDToFind->ToLowerInvariant();				
							//Now check if the hardware ID we are looking at contains the correct VID/PID
							MatchFound = DeviceIDFromRegistry->Contains(DeviceIDToFind);		
							if(MatchFound == true)
							{


								numDevicesDetected++;
								//Device must have been found.  Open read and write handles.  In order to do this, we will need the actual device path first.
								//We can get the path by calling SetupDiGetDeviceInterfaceDetail(), however, we have to call this function twice:  The first
								//time to get the size of the required structure/buffer to hold the detailed interface data, then a second time to actually 
								//get the structure (after we have allocated enough memory for the structure.)
								DetailedInterfaceDataStructure->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);
								//First call populates "StructureSize" with the correct value
								SetupDiGetDeviceInterfaceDetailUM(DeviceInfoTable, InterfaceDataStructure, NULL, NULL, &StructureSize, NULL);	
								DetailedInterfaceDataStructure = (PSP_DEVICE_INTERFACE_DETAIL_DATA)(malloc(StructureSize));		//Allocate enough memory
								if(DetailedInterfaceDataStructure == NULL)	//if null, error, couldn't allocate enough memory
								{	//Can't really recover from this situation, just exit instead.
									SetupDiDestroyDeviceInfoListUM(DeviceInfoTable);	//Clean up the old structure we no longer need.
									return 0;		
								}
								DetailedInterfaceDataStructure->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);
								 //Now call SetupDiGetDeviceInterfaceDetail() a second time to receive the goods.  
								SetupDiGetDeviceInterfaceDetailUM(DeviceInfoTable, InterfaceDataStructure, DetailedInterfaceDataStructure, StructureSize, NULL, NULL); 

								//We now have the proper device path, and we can finally open read and write handles to the device.
								//We store the handles in the global variables "WriteHandle" and "ReadHandle", which we will use later to actually communicate.
								if(numDevicesDetected==1)
									ADF1WriteHandle = CreateFile((DetailedInterfaceDataStructure->DevicePath), GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, 0);

								if(numDevicesDetected==2)
									ADF2WriteHandle = CreateFile((DetailedInterfaceDataStructure->DevicePath), GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, 0);

								ErrorStatus = GetLastError();
								if(ErrorStatus == ERROR_SUCCESS)
								{
									//ToggleLED_btn->Enabled = true;				//Make button no longer greyed out
								}

								if(numDevicesDetected==1)
									ADF1ReadHandle = CreateFile((DetailedInterfaceDataStructure->DevicePath), GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, 0);
								
								if(numDevicesDetected==2)
									ADF2ReadHandle = CreateFile((DetailedInterfaceDataStructure->DevicePath), GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, 0);

								ErrorStatus = GetLastError();
								if(ErrorStatus == ERROR_SUCCESS)
								{
									//GetPushbuttonState_btn->Enabled = true;		//Make button no longer greyed out
									//StateLabel->Enabled = true;					//Make label no longer greyed out
								}
								
								
								MatchFound = false;
							}

							if(numDevicesDetected==1)
							{
								if(InterfaceIndex==2)
								{
										SetupDiDestroyDeviceInfoListUM(DeviceInfoTable);	//Clean up the old structure we no longer need.
										return numDevicesDetected;	
								}
							}

							if(numDevicesDetected==2)
							{
								SetupDiDestroyDeviceInfoListUM(DeviceInfoTable);	//Clean up the old structure we no longer need.
								return numDevicesDetected;	
							}

							InterfaceIndex++;	

							//Keep looping until we either find a device with matching VID and PID, or until we run out of items.
						}//end of while(true)	
}	
void ReadADFValue(int module)
{
	int i;
	DWORD dwResult;
	DWORD BytesWritten = 0;
	DWORD BytesRead = 0;
	unsigned char OutputPacketBuffer[65];	//Allocate a memory buffer equal to our endpoint size + 1
	unsigned char InputPacketBuffer[65];	//Allocate a memory buffer equal to our endpoint size + 1

	for(i=0; i<65; i++)
	{
		InputPacketBuffer[i] = 0;				//The first byte is the "Report ID" and does not get transmitted over the USB bus.  Always set = 0.
		OutputPacketBuffer[i] = 0;				//The first byte is the "Report ID" and does not get transmitted over the USB bus.  Always set = 0.
	}
	OutputPacketBuffer[1] = 0x81;			//0x81 is the "Get Pushbutton State" command in the firmware
											//For simplicity, we will leave the rest of the buffer uninitialized, but you could put real
											//data in it if you like.

	//The basic Windows I/O functions WriteFile() and ReadFile() can be used to read and write to HID class USB devices 
	//(once we have the read and write handles to the device, which are obtained with CreateFile()).

	//To get the pushbutton state, first, we send a packet with our "Get Pushbutton State" command in it.
	//The following call to WriteFile() sends 64 bytes of data to the USB device.

	if(module==1)
	{
		if(ADF1isADF1)
		{
			WriteFile(ADF1WriteHandle, &OutputPacketBuffer, 65, &BytesWritten, 0);	//Blocking function, unless an "overlapped" structure is used
			ReadFile(ADF1ReadHandle, &InputPacketBuffer, 65, &BytesRead, 0);		//Blocking function, unless an "overlapped" structure is used
		}
		if(ADF1isADF2)
		{
			WriteFile(ADF2WriteHandle, &OutputPacketBuffer, 65, &BytesWritten, 0);	//Blocking function, unless an "overlapped" structure is used
			ReadFile(ADF2ReadHandle, &InputPacketBuffer, 65, &BytesRead, 0);		//Blocking function, unless an "overlapped" structure is used
		}

		if(InputPacketBuffer[2]==0x01)	//ADF Frequency has changed
		{
				uiADFData = 0;
				uiADFData = InputPacketBuffer[6];
				uiADFData |= (InputPacketBuffer[5]<<4);
				uiADFData |= (InputPacketBuffer[4]<<8);

				uiADFDataEx = 0;
				uiADFDataEx = InputPacketBuffer[7];
				uiADFDataEx |= (InputPacketBuffer[3]<<8);

				if (FSUIPC_Write(0x034C, 2, &uiADFData, &dwResult))
					FSUIPC_Process(&dwResult); // Process the request(s)

				if (FSUIPC_Write(0x0356, 2, &uiADFDataEx, &dwResult))
					FSUIPC_Process(&dwResult); // Process the request(s)
		}
	}
	if(module==2)
	{
		if(ADF2isADF1)
		{
			WriteFile(ADF1WriteHandle, &OutputPacketBuffer, 65, &BytesWritten, 0);	//Blocking function, unless an "overlapped" structure is used
			ReadFile(ADF1ReadHandle, &InputPacketBuffer, 65, &BytesRead, 0);		//Blocking function, unless an "overlapped" structure is used
		}
		if(ADF2isADF2)
		{
			WriteFile(ADF2WriteHandle, &OutputPacketBuffer, 65, &BytesWritten, 0);	//Blocking function, unless an "overlapped" structure is used
			ReadFile(ADF2ReadHandle, &InputPacketBuffer, 65, &BytesRead, 0);		//Blocking function, unless an "overlapped" structure is used
		}

		if(InputPacketBuffer[2]==0x01)	//ADF Frequency has changed
		{
				uiADFData = 0;
				uiADFData = InputPacketBuffer[6];
				uiADFData |= (InputPacketBuffer[5]<<4);
				uiADFData |= (InputPacketBuffer[4]<<8);

				uiADFDataEx = 0;
				uiADFDataEx = InputPacketBuffer[7];
				uiADFDataEx |= (InputPacketBuffer[3]<<8);

				if (FSUIPC_Write(0x02D4, 2, &uiADFData, &dwResult))
					FSUIPC_Process(&dwResult); // Process the request(s)

				if (FSUIPC_Write(0x02D6, 2, &uiADFDataEx, &dwResult))
					FSUIPC_Process(&dwResult); // Process the request(s)
		}
	}

}


void ReadANTSwitch(int module)
{
	int i;
	DWORD dwDataLength=0;
	DWORD dwResult;
	char tempData[3];
	DWORD temp;
	DWORD BytesWritten = 0;
	DWORD BytesRead = 0;
	unsigned char OutputPacketBuffer[65];	//Allocate a memory buffer equal to our endpoint size + 1
	unsigned char InputPacketBuffer[65];	//Allocate a memory buffer equal to our endpoint size + 1

	for(i=0; i<65; i++)
	{
		InputPacketBuffer[i] = 0;				//The first byte is the "Report ID" and does not get transmitted over the USB bus.  Always set = 0.
		OutputPacketBuffer[i] = 0;				//The first byte is the "Report ID" and does not get transmitted over the USB bus.  Always set = 0.
	}
	OutputPacketBuffer[1] = 0x71;			//0x81 is the "Get Pushbutton State" command in the firmware
											//For simplicity, we will leave the rest of the buffer uninitialized, but you could put real
											//data in it if you like.

	//The basic Windows I/O functions WriteFile() and ReadFile() can be used to read and write to HID class USB devices 
	//(once we have the read and write handles to the device, which are obtained with CreateFile()).

	//To get the pushbutton state, first, we send a packet with our "Get Pushbutton State" command in it.
	//The following call to WriteFile() sends 64 bytes of data to the USB device.
	if(module==1)
	{
		if(ADF1isADF1)
		{
			WriteFile(ADF1WriteHandle, &OutputPacketBuffer, 65, &BytesWritten, 0);	//Blocking function, unless an "overlapped" structure is used
			ReadFile(ADF1ReadHandle, &InputPacketBuffer, 65, &BytesRead, 0);		//Blocking function, unless an "overlapped" structure is used
		}
		if(ADF1isADF2)
		{
			WriteFile(ADF2WriteHandle, &OutputPacketBuffer, 65, &BytesWritten, 0);	//Blocking function, unless an "overlapped" structure is used
			ReadFile(ADF2ReadHandle, &InputPacketBuffer, 65, &BytesRead, 0);		//Blocking function, unless an "overlapped" structure is used
		}
		if(InputPacketBuffer[2]==0x01)	//ANT switch has changed
		{
			if(InputPacketBuffer[3]==0x01)		//switch is high
			{
				if(ulADF1ANTOffset==GPS_NAV)
				{
					dwDataLength = 4;
				}
				if(ulADF1ANTOffset==DME_SELECT)
				{
					dwDataLength = 2;
				}
				//need to do a bit mask operation?
				if(ulADF1ANTOffset==RADIO_SOUND)
				{
					dwDataLength = 1;
					FSUIPC_Read(RADIO_SOUND, 1, tempData, &dwResult);
					FSUIPC_Process(&dwResult);
					if(ulADF1Value1==0xAA)
					{
						temp = tempData[0]|0x80;	//set COM1 Transmit
						temp = temp & 0xBF;			//clear COM2 Transmit
					}
					else if (ulADF1Value1==0xBB)
					{
						temp = tempData[0]|0x40;	//set COM2 Transmit
						temp = temp & 0x7F;			//clear COM1 Transmit
					}
					else
					{
						temp = tempData[0]|ulADF1Value1;
					}
					ulADF1Value1 = temp;
				}
				
				//send offset and value to FSUIPC
				if (FSUIPC_Write(ulADF1ANTOffset, dwDataLength, &ulADF1Value1, &dwResult))
					FSUIPC_Process(&dwResult); // Process the request(s)
			}
			else								//switch is low
			{
				if(ulADF1ANTOffset==GPS_NAV)
				{
					dwDataLength = 4;
				}
				if(ulADF1ANTOffset==DME_SELECT)
				{
					dwDataLength = 2;
				}
				//need to do a bit mask operation?
				if(ulADF1ANTOffset==RADIO_SOUND)
				{
					dwDataLength = 1;
					FSUIPC_Read(RADIO_SOUND, 1, tempData, &dwResult);
					FSUIPC_Process(&dwResult);
					if(ulADF1Value2==0xAA)
					{
						temp = tempData[0]|0x80;	//set COM1 Transmit
						temp = temp & 0xBF;			//clear COM2 Transmit
					}
					else if (ulADF1Value2==0xBB)
					{
						temp = tempData[0]|0x40;	//set COM2 Transmit
						temp = temp & 0x7F;			//clear COM1 Transmit
					}
					else
					{
						temp = tempData[0]|ulADF1Value2;
					}
					ulADF1Value2 = temp;
				}
				
				//send offset and value to FSUIPC
				if (FSUIPC_Write(ulADF1ANTOffset, dwDataLength, &ulADF1Value2, &dwResult))
					FSUIPC_Process(&dwResult); // Process the request(s)
			}
		}
	}
	if(module==2)
	{
		if(ADF2isADF1)
		{
			WriteFile(ADF1WriteHandle, &OutputPacketBuffer, 65, &BytesWritten, 0);	//Blocking function, unless an "overlapped" structure is used
			ReadFile(ADF1ReadHandle, &InputPacketBuffer, 65, &BytesRead, 0);		//Blocking function, unless an "overlapped" structure is used
		}
		if(ADF2isADF2)
		{
			WriteFile(ADF2WriteHandle, &OutputPacketBuffer, 65, &BytesWritten, 0);	//Blocking function, unless an "overlapped" structure is used
			ReadFile(ADF2ReadHandle, &InputPacketBuffer, 65, &BytesRead, 0);		//Blocking function, unless an "overlapped" structure is used
		}
		if(InputPacketBuffer[2]==0x01)	//ANT switch has changed
		{
			if(InputPacketBuffer[3]==0x01)		//switch is high
			{
				if(ulADF2ANTOffset==GPS_NAV)
				{
					dwDataLength = 4;
				}
				if(ulADF2ANTOffset==DME_SELECT)
				{
					dwDataLength = 2;
				}
				//need to do a bit mask operation?
				if(ulADF2ANTOffset==RADIO_SOUND)
				{
					dwDataLength = 1;
					FSUIPC_Read(RADIO_SOUND, 1, tempData, &dwResult);
					FSUIPC_Process(&dwResult);
					if(ulADF2Value1==0xAA)
					{
						temp = tempData[0]|0x80;	//set COM1 Transmit
						temp = temp & 0xBF;			//clear COM2 Transmit
					}
					else if (ulADF2Value1==0xBB)
					{
						temp = tempData[0]|0x40;	//set COM2 Transmit
						temp = temp & 0x7F;			//clear COM1 Transmit
					}
					else
					{
						temp = tempData[0]|ulADF2Value1;
					}
					ulADF2Value1 = temp;
				}
				
				//send offset and value to FSUIPC
				if (FSUIPC_Write(ulADF2ANTOffset, dwDataLength, &ulADF2Value1, &dwResult))
					FSUIPC_Process(&dwResult); // Process the request(s)
			}
			else								//switch is low
			{
				if(ulADF2ANTOffset==GPS_NAV)
				{
					dwDataLength = 4;
				}
				if(ulADF2ANTOffset==DME_SELECT)
				{
					dwDataLength = 2;
				}
				//need to do a bit mask operation?
				if(ulADF2ANTOffset==RADIO_SOUND)
				{
					dwDataLength = 1;
					FSUIPC_Read(RADIO_SOUND, 1, tempData, &dwResult);
					FSUIPC_Process(&dwResult);
					if(ulADF2Value2==0xAA)
					{
						temp = tempData[0]|0x80;	//set COM1 Transmit
						temp = temp & 0xBF;			//clear COM2 Transmit
					}
					else if (ulADF2Value2==0xBB)
					{
						temp = tempData[0]|0x40;	//set COM2 Transmit
						temp = temp & 0x7F;			//clear COM1 Transmit
					}
					else
					{
						temp = tempData[0]|ulADF2Value2;
					}
					ulADF2Value2 = temp;
				}
				
				//send offset and value to FSUIPC
				if (FSUIPC_Write(ulADF2ANTOffset, dwDataLength, &ulADF2Value2, &dwResult))
					FSUIPC_Process(&dwResult); // Process the request(s)
			}
		}
	}
}
void ReadTONESwitch(int module)
{
	int i;
	DWORD dwDataLength=0;
	DWORD dwResult;
	char tempData[3];
	DWORD temp;
	DWORD BytesWritten = 0;
	DWORD BytesRead = 0;
	unsigned char OutputPacketBuffer[65];	//Allocate a memory buffer equal to our endpoint size + 1
	unsigned char InputPacketBuffer[65];	//Allocate a memory buffer equal to our endpoint size + 1

	for(i=0; i<65; i++)
	{
		InputPacketBuffer[i] = 0;				//The first byte is the "Report ID" and does not get transmitted over the USB bus.  Always set = 0.
		OutputPacketBuffer[i] = 0;				//The first byte is the "Report ID" and does not get transmitted over the USB bus.  Always set = 0.
	}

	OutputPacketBuffer[1] = 0x61;			//0x81 is the "Get Pushbutton State" command in the firmware
											//For simplicity, we will leave the rest of the buffer uninitialized, but you could put real
											//data in it if you like.

	//The basic Windows I/O functions WriteFile() and ReadFile() can be used to read and write to HID class USB devices 
	//(once we have the read and write handles to the device, which are obtained with CreateFile()).

	//To get the pushbutton state, first, we send a packet with our "Get Pushbutton State" command in it.
	//The following call to WriteFile() sends 64 bytes of data to the USB device.
	if(module==1)
	{
		if(ADF1isADF1)
		{
			WriteFile(ADF1WriteHandle, &OutputPacketBuffer, 65, &BytesWritten, 0);	//Blocking function, unless an "overlapped" structure is used
			ReadFile(ADF1ReadHandle, &InputPacketBuffer, 65, &BytesRead, 0);		//Blocking function, unless an "overlapped" structure is used
		}
		if(ADF1isADF2)
		{
			WriteFile(ADF2WriteHandle, &OutputPacketBuffer, 65, &BytesWritten, 0);	//Blocking function, unless an "overlapped" structure is used
			ReadFile(ADF2ReadHandle, &InputPacketBuffer, 65, &BytesRead, 0);		//Blocking function, unless an "overlapped" structure is used
		}
		if(InputPacketBuffer[2]==0x01)	//ANT switch has changed
		{
			if(InputPacketBuffer[3]==0x01)		//switch is high
			{
				if(ulADF1TONEOffset==GPS_NAV)
				{
					dwDataLength = 4;
				}
				if(ulADF1TONEOffset==DME_SELECT)
				{
					dwDataLength = 2;
				}
				//need to do a bit mask operation?
				if(ulADF1TONEOffset==RADIO_SOUND)
				{
					dwDataLength = 1;
					FSUIPC_Read(RADIO_SOUND, 1, tempData, &dwResult);
					FSUIPC_Process(&dwResult);
					if(ulTONE1Value1==0xAA)
					{
						temp = tempData[0]|0x80;	//set COM1 Transmit
						temp = temp & 0xBF;			//clear COM2 Transmit
					}
					else if (ulTONE1Value1==0xBB)
					{
						temp = tempData[0]|0x40;	//set COM2 Transmit
						temp = temp & 0x7F;			//clear COM1 Transmit
					}
					else
					{
						temp = tempData[0]|ulTONE1Value1;
					}
					ulTONE1Value1 = temp;
				}
				
				//send offset and value to FSUIPC
				if (FSUIPC_Write(ulADF1TONEOffset, dwDataLength, &ulTONE1Value1, &dwResult))
					FSUIPC_Process(&dwResult); // Process the request(s)
			}
			else								//switch is low
			{
				if(ulADF1TONEOffset==GPS_NAV)
				{
					dwDataLength = 4;
				}
				if(ulADF1TONEOffset==DME_SELECT)
				{
					dwDataLength = 2;
				}
				//need to do a bit mask operation?
				if(ulADF1TONEOffset==RADIO_SOUND)
				{
					dwDataLength = 1;
					FSUIPC_Read(RADIO_SOUND, 1, tempData, &dwResult);
					FSUIPC_Process(&dwResult);
					if(ulTONE1Value2==0xAA)
					{
						temp = tempData[0]|0x80;	//set COM1 Transmit
						temp = temp & 0xBF;			//clear COM2 Transmit
					}
					else if (ulTONE1Value2==0xBB)
					{
						temp = tempData[0]|0x40;	//set COM2 Transmit
						temp = temp & 0x7F;			//clear COM1 Transmit
					}
					else
					{
						temp = tempData[0]|ulTONE1Value2;
					}
					ulTONE1Value2 = temp;
				}
				
				//send offset and value to FSUIPC
				if (FSUIPC_Write(ulADF1TONEOffset, dwDataLength, &ulTONE1Value2, &dwResult))
					FSUIPC_Process(&dwResult); // Process the request(s)
			}
		}
	}
	if(module==2)
	{
		if(ADF2isADF1)
		{
			WriteFile(ADF1WriteHandle, &OutputPacketBuffer, 65, &BytesWritten, 0);	//Blocking function, unless an "overlapped" structure is used
			ReadFile(ADF1ReadHandle, &InputPacketBuffer, 65, &BytesRead, 0);		//Blocking function, unless an "overlapped" structure is used
		}
		if(ADF2isADF2)
		{
			WriteFile(ADF2WriteHandle, &OutputPacketBuffer, 65, &BytesWritten, 0);	//Blocking function, unless an "overlapped" structure is used
			ReadFile(ADF2ReadHandle, &InputPacketBuffer, 65, &BytesRead, 0);		//Blocking function, unless an "overlapped" structure is used
		}
		if(InputPacketBuffer[2]==0x01)	//ANT switch has changed
		{
			if(InputPacketBuffer[3]==0x01)		//switch is high
			{
				if(ulADF2TONEOffset==GPS_NAV)
				{
					dwDataLength = 4;
				}
				if(ulADF2TONEOffset==DME_SELECT)
				{
					dwDataLength = 2;
				}
				//need to do a bit mask operation?
				if(ulADF2TONEOffset==RADIO_SOUND)
				{
					dwDataLength = 1;
					FSUIPC_Read(RADIO_SOUND, 1, tempData, &dwResult);
					FSUIPC_Process(&dwResult);
					if(ulTONE2Value1==0xAA)
					{
						temp = tempData[0]|0x80;	//set COM1 Transmit
						temp = temp & 0xBF;			//clear COM2 Transmit
					}
					else if (ulTONE2Value1==0xBB)
					{
						temp = tempData[0]|0x40;	//set COM2 Transmit
						temp = temp & 0x7F;			//clear COM1 Transmit
					}
					else
					{
						temp = tempData[0]|ulTONE2Value1;
					}
					ulTONE2Value1 = temp;
				}
				
				//send offset and value to FSUIPC
				if (FSUIPC_Write(ulADF2TONEOffset, dwDataLength, &ulTONE2Value1, &dwResult))
					FSUIPC_Process(&dwResult); // Process the request(s)
			}
			else								//switch is low
			{
				if(ulADF2TONEOffset==GPS_NAV)
				{
					dwDataLength = 4;
				}
				if(ulADF2TONEOffset==DME_SELECT)
				{
					dwDataLength = 2;
				}
				//need to do a bit mask operation?
				if(ulADF2TONEOffset==RADIO_SOUND)
				{
					dwDataLength = 1;
					FSUIPC_Read(RADIO_SOUND, 1, tempData, &dwResult);
					FSUIPC_Process(&dwResult);
					if(ulTONE2Value2==0xAA)
					{
						temp = tempData[0]|0x80;	//set COM1 Transmit
						temp = temp & 0xBF;			//clear COM2 Transmit
					}
					else if (ulTONE2Value2==0xBB)
					{
						temp = tempData[0]|0x40;	//set COM2 Transmit
						temp = temp & 0x7F;			//clear COM1 Transmit
					}
					else
					{
						temp = tempData[0]|ulTONE2Value2;
					}
					ulTONE2Value2 = temp;
				}
				
				//send offset and value to FSUIPC
				if (FSUIPC_Write(ulADF2TONEOffset, dwDataLength, &ulTONE2Value2, &dwResult))
					FSUIPC_Process(&dwResult); // Process the request(s)
			}
		}
	}
}

void ReadCOMValue(int module)
{

	int i;
	DWORD dwResult;
	DWORD BytesWritten = 0;
	DWORD BytesRead = 0;
	unsigned char OutputPacketBuffer[65];	//Allocate a memory buffer equal to our endpoint size + 1
	unsigned char InputPacketBuffer[65];	//Allocate a memory buffer equal to our endpoint size + 1

	for(i=0; i<65; i++)
	{
		InputPacketBuffer[i] = 0;				//The first byte is the "Report ID" and does not get transmitted over the USB bus.  Always set = 0.
		OutputPacketBuffer[i] = 0;				//The first byte is the "Report ID" and does not get transmitted over the USB bus.  Always set = 0.
	}
	OutputPacketBuffer[1] = 0x82;			//0x81 is the "Get Pushbutton State" command in the firmware
											//For simplicity, we will leave the rest of the buffer uninitialized, but you could put real
											//data in it if you like.

	//The basic Windows I/O functions WriteFile() and ReadFile() can be used to read and write to HID class USB devices 
	//(once we have the read and write handles to the device, which are obtained with CreateFile()).

	//To get the pushbutton state, first, we send a packet with our "Get Pushbutton State" command in it.
	//The following call to WriteFile() sends 64 bytes of data to the USB device.
	if(module==1)
	{
		if(COM1isCOM1)
		{
			WriteFile(COM1WriteHandle, &OutputPacketBuffer, 65, &BytesWritten, 0);	//Blocking function, unless an "overlapped" structure is used
			ReadFile(COM1ReadHandle, &InputPacketBuffer, 65, &BytesRead, 0);		//Blocking function, unless an "overlapped" structure is used
		}
		if(COM1isCOM2)
		{
			WriteFile(COM2WriteHandle, &OutputPacketBuffer, 65, &BytesWritten, 0);	//Blocking function, unless an "overlapped" structure is used
			ReadFile(COM2ReadHandle, &InputPacketBuffer, 65, &BytesRead, 0);		//Blocking function, unless an "overlapped" structure is used
		}

		if(InputPacketBuffer[2]==0x01)	//COM1 Frequency has changed
		{

				uiCOMStandbyData = 0x0000;
				uiCOMStandbyData |= (InputPacketBuffer[7]<<12);
				uiCOMStandbyData |= (InputPacketBuffer[8]<<8);
				uiCOMStandbyData |= (InputPacketBuffer[9]<<4);
				uiCOMStandbyData |= InputPacketBuffer[10];

				//send COM1 stby Freq
				FSUIPC_Write(0x311A, 2, &uiCOMStandbyData, &dwResult);
				//FSUIPC_Process(&dwResult); // Process the request(s)

				uiCOMData = 0x0000;
				uiCOMData |= (InputPacketBuffer[3]<<12);
				uiCOMData |= (InputPacketBuffer[4]<<8);
				uiCOMData |= (InputPacketBuffer[5]<<4);
				uiCOMData |= InputPacketBuffer[6];
				
				//send COM1 Freq
				FSUIPC_Write(0x034E, 2, &uiCOMData, &dwResult);
					FSUIPC_Process(&dwResult); // Process the request(s)
				
		}
	}
	if(module==2)
	{
		if(COM2isCOM1)
		{
			WriteFile(COM1WriteHandle, &OutputPacketBuffer, 65, &BytesWritten, 0);	//Blocking function, unless an "overlapped" structure is used
			ReadFile(COM1ReadHandle, &InputPacketBuffer, 65, &BytesRead, 0);		//Blocking function, unless an "overlapped" structure is used
		}
		if(COM2isCOM2)
		{
			WriteFile(COM2WriteHandle, &OutputPacketBuffer, 65, &BytesWritten, 0);	//Blocking function, unless an "overlapped" structure is used
			ReadFile(COM2ReadHandle, &InputPacketBuffer, 65, &BytesRead, 0);		//Blocking function, unless an "overlapped" structure is used
		}

		if(InputPacketBuffer[2]==0x01)	//COM2 Frequency has changed
		{

				uiCOMStandbyData = 0x0000;
				uiCOMStandbyData |= (InputPacketBuffer[7]<<12);
				uiCOMStandbyData |= (InputPacketBuffer[8]<<8);
				uiCOMStandbyData |= (InputPacketBuffer[9]<<4);
				uiCOMStandbyData |= InputPacketBuffer[10];

				//send COM2 stby Freq
				FSUIPC_Write(0x311Cd, 2, &uiCOMStandbyData, &dwResult);
				//FSUIPC_Process(&dwResult); // Process the request(s)

				uiCOMData = 0x0000;
				uiCOMData |= (InputPacketBuffer[3]<<12);
				uiCOMData |= (InputPacketBuffer[4]<<8);
				uiCOMData |= (InputPacketBuffer[5]<<4);
				uiCOMData |= InputPacketBuffer[6];
				
				//send COM2 Freq
				FSUIPC_Write(0x3118, 2, &uiCOMData, &dwResult);
					FSUIPC_Process(&dwResult); // Process the request(s)
				
		}
	}


}
void ReadCOMSwitch(int module)
{
	int i;
	DWORD dwDataLength=0;
	DWORD dwResult;
	char tempData[3];
	DWORD temp;
	DWORD BytesWritten = 0;
	DWORD BytesRead = 0;
	unsigned char OutputPacketBuffer[65];	//Allocate a memory buffer equal to our endpoint size + 1
	unsigned char InputPacketBuffer[65];	//Allocate a memory buffer equal to our endpoint size + 1

	for(i=0; i<65; i++)
	{
		InputPacketBuffer[i] = 0;				//The first byte is the "Report ID" and does not get transmitted over the USB bus.  Always set = 0.
		OutputPacketBuffer[i] = 0;				//The first byte is the "Report ID" and does not get transmitted over the USB bus.  Always set = 0.
	}
	OutputPacketBuffer[1] = 0x72;			//0x72 is the "Get Pushbutton State" command in the firmware
											//For simplicity, we will leave the rest of the buffer uninitialized, but you could put real
											//data in it if you like.

	//The basic Windows I/O functions WriteFile() and ReadFile() can be used to read and write to HID class USB devices 
	//(once we have the read and write handles to the device, which are obtained with CreateFile()).

	//To get the pushbutton state, first, we send a packet with our "Get Pushbutton State" command in it.
	//The following call to WriteFile() sends 64 bytes of data to the USB device.
	if(module==1)
	{
		if(COM1isCOM1)
		{
			WriteFile(COM1WriteHandle, &OutputPacketBuffer, 65, &BytesWritten, 0);	//Blocking function, unless an "overlapped" structure is used
			ReadFile(COM1ReadHandle, &InputPacketBuffer, 65, &BytesRead, 0);		//Blocking function, unless an "overlapped" structure is used
		}
		if(COM1isCOM2)
		{
			WriteFile(COM2WriteHandle, &OutputPacketBuffer, 65, &BytesWritten, 0);	//Blocking function, unless an "overlapped" structure is used
			ReadFile(COM2ReadHandle, &InputPacketBuffer, 65, &BytesRead, 0);		//Blocking function, unless an "overlapped" structure is used
		}

		if(InputPacketBuffer[2]==0x01)	//ANT switch has been pressed
		{
			//display "No Err" ?
			if(ulCOMTEST1Offset==NO_ERR)
			{
				OutputPacketBuffer[1] = 0xEE;
				if(COM1isCOM1)
				{
					WriteFile(COM1WriteHandle, &OutputPacketBuffer, 65, &BytesWritten, 0);	//Blocking function, unless an "overlapped" structure is used
					ReadFile(COM1ReadHandle, &InputPacketBuffer, 65, &BytesRead, 0);		//Blocking function, unless an "overlapped" structure is used
				}
				if(COM1isCOM2)
				{
					WriteFile(COM2WriteHandle, &OutputPacketBuffer, 65, &BytesWritten, 0);	//Blocking function, unless an "overlapped" structure is used
					ReadFile(COM2ReadHandle, &InputPacketBuffer, 65, &BytesRead, 0);		//Blocking function, unless an "overlapped" structure is used
				}
				return;
			}

			if(COMTEST1Toggle==true)		//
			{
				if(ulCOMTEST1Offset==GPS_NAV)
				{
					dwDataLength = 4;
				}
				if(ulCOMTEST1Offset==DME_SELECT)
				{
					dwDataLength = 2;
				}
				//need to do a bit mask operation?
				if(ulCOMTEST1Offset==RADIO_SOUND)
				{
					dwDataLength = 1;
					FSUIPC_Read(RADIO_SOUND, 1, tempData, &dwResult);
					FSUIPC_Process(&dwResult);
					if(ulCOMTEST1Value1==0xAA)
					{
						temp = tempData[0]|0x80;	//set COM1 Transmit
						temp = temp & 0xBF;			//clear COM2 Transmit
					}
					else if (ulCOMTEST1Value1==0xBB)
					{
						temp = tempData[0]|0x40;	//set COM2 Transmit
						temp = temp & 0x7F;			//clear COM1 Transmit
					}
					else
					{
						temp = tempData[0]|ulCOMTEST1Value1;
					}
					ulCOMTEST1Value1 = temp;
				}
				
				//send offset and value to FSUIPC
				if (FSUIPC_Write(ulCOMTEST1Offset, dwDataLength, &ulCOMTEST1Value1, &dwResult))
					FSUIPC_Process(&dwResult); // Process the request(s)
				COMTEST1Toggle=false;
			}
			else								//switch is low
			{
				if(ulCOMTEST1Offset==GPS_NAV)
				{
					dwDataLength = 4;
				}
				if(ulCOMTEST1Offset==DME_SELECT)
				{
					dwDataLength = 2;
				}
				//need to do a bit mask operation?
				if(ulCOMTEST1Offset==RADIO_SOUND)
				{
					dwDataLength = 1;
					FSUIPC_Read(RADIO_SOUND, 1, tempData, &dwResult);
					FSUIPC_Process(&dwResult);
					if(ulCOMTEST1Value2==0xAA)
					{
						temp = tempData[0]|0x80;	//set COM1 Transmit
						temp = temp & 0xBF;			//clear COM2 Transmit
					}
					else if (ulCOMTEST1Value2==0xBB)
					{
						temp = tempData[0]|0x40;	//set COM2 Transmit
						temp = temp & 0x7F;			//clear COM1 Transmit
					}
					else
					{
						temp = tempData[0]|ulCOMTEST1Value2;
					}
					temp = tempData[0]&ulCOMTEST1Value2;
					ulCOMTEST1Value2 = temp;
				}
				
				//send offset and value of(0) to FSUIPC
				if (FSUIPC_Write(ulCOMTEST1Offset, dwDataLength, &ulCOMTEST1Value2, &dwResult))
					FSUIPC_Process(&dwResult); // Process the request(s)
				COMTEST1Toggle=true;
			}
		}
	}
	if(module==2)
	{
		if(COM2isCOM1)
		{
			WriteFile(COM1WriteHandle, &OutputPacketBuffer, 65, &BytesWritten, 0);	//Blocking function, unless an "overlapped" structure is used
			ReadFile(COM1ReadHandle, &InputPacketBuffer, 65, &BytesRead, 0);		//Blocking function, unless an "overlapped" structure is used
		}
		if(COM2isCOM2)
		{
			WriteFile(COM2WriteHandle, &OutputPacketBuffer, 65, &BytesWritten, 0);	//Blocking function, unless an "overlapped" structure is used
			ReadFile(COM2ReadHandle, &InputPacketBuffer, 65, &BytesRead, 0);		//Blocking function, unless an "overlapped" structure is used
		}

		if(InputPacketBuffer[2]==0x01)	//ANT switch has been pressed
		{
			//display "No Err" ?
			if(ulCOMTEST2Offset==NO_ERR)
			{
				OutputPacketBuffer[1] = 0xEE;
				if(COM2isCOM1)
				{
					WriteFile(COM1WriteHandle, &OutputPacketBuffer, 65, &BytesWritten, 0);	//Blocking function, unless an "overlapped" structure is used
					ReadFile(COM1ReadHandle, &InputPacketBuffer, 65, &BytesRead, 0);		//Blocking function, unless an "overlapped" structure is used
				}
				if(COM2isCOM2)
				{
					WriteFile(COM2WriteHandle, &OutputPacketBuffer, 65, &BytesWritten, 0);	//Blocking function, unless an "overlapped" structure is used
					ReadFile(COM2ReadHandle, &InputPacketBuffer, 65, &BytesRead, 0);		//Blocking function, unless an "overlapped" structure is used
				}
				return;
			}

			if(COMTEST2Toggle==true)		//
			{
				
				//need to do a bit mask operation?
				if(ulCOMTEST2Offset==RADIO_SOUND)
				{
					dwDataLength = 1;
					FSUIPC_Read(RADIO_SOUND, 1, tempData, &dwResult);
					FSUIPC_Process(&dwResult);
					if(ulCOMTEST2Value1==0xAA)
					{
						temp = tempData[0]|0x80;	//set COM1 Transmit
						temp = temp & 0xBF;			//clear COM2 Transmit
					}
					else if (ulCOMTEST2Value1==0xBB)
					{
						temp = tempData[0]|0x40;	//set COM2 Transmit
						temp = temp & 0x7F;			//clear COM1 Transmit
					}
					else
					{
						temp = tempData[0]|ulCOMTEST2Value1;
					}
					ulCOMTEST2Value1 = temp;
				}
				
				//send offset and value to FSUIPC
				if (FSUIPC_Write(ulCOMTEST2Offset, dwDataLength, &ulCOMTEST2Value1, &dwResult))
					FSUIPC_Process(&dwResult); // Process the request(s)
				COMTEST2Toggle=false;
			}
			else								//switch is low
			{
				
				//need to do a bit mask operation?
				if(ulCOMTEST2Offset==RADIO_SOUND)
				{
					dwDataLength = 1;
					FSUIPC_Read(RADIO_SOUND, 1, tempData, &dwResult);
					FSUIPC_Process(&dwResult);
					if(ulCOMTEST2Value2==0xAA)
					{
						temp = tempData[0]|0x80;	//set COM1 Transmit
						temp = temp & 0xBF;			//clear COM2 Transmit
					}
					else if (ulCOMTEST2Value2==0xBB)
					{
						temp = tempData[0]|0x40;	//set COM2 Transmit
						temp = temp & 0x7F;			//clear COM1 Transmit
					}
					else
					{
						temp = tempData[0]|ulCOMTEST2Value2;
					}
					temp = tempData[0]&ulCOMTEST2Value2;
					ulCOMTEST2Value2 = temp;
				}
				
				//send offset and value of(0) to FSUIPC
				if (FSUIPC_Write(ulCOMTEST2Offset, dwDataLength, &ulCOMTEST2Value2, &dwResult))
					FSUIPC_Process(&dwResult); // Process the request(s)
				COMTEST2Toggle=true;
			}
		}
	}
}
void ReadNAVValue(int module)
{

	int i;
	DWORD dwResult;
	DWORD BytesWritten = 0;
	DWORD BytesRead = 0;
	unsigned char OutputPacketBuffer[65];	//Allocate a memory buffer equal to our endpoint size + 1
	unsigned char InputPacketBuffer[65];	//Allocate a memory buffer equal to our endpoint size + 1

	for(i=0; i<65; i++)
	{
		InputPacketBuffer[i] = 0;				//The first byte is the "Report ID" and does not get transmitted over the USB bus.  Always set = 0.
		OutputPacketBuffer[i] = 0;				//The first byte is the "Report ID" and does not get transmitted over the USB bus.  Always set = 0.
	}
	OutputPacketBuffer[1] = 0x83;			//0x81 is the "Get Pushbutton State" command in the firmware
											//For simplicity, we will leave the rest of the buffer uninitialized, but you could put real
											//data in it if you like.

	//The basic Windows I/O functions WriteFile() and ReadFile() can be used to read and write to HID class USB devices 
	//(once we have the read and write handles to the device, which are obtained with CreateFile()).
	

	if(module==1)
	{
		if(NAV1isNAV1)
		{
			WriteFile(NAV1WriteHandle, &OutputPacketBuffer, 65, &BytesWritten, 0);	//Blocking function, unless an "overlapped" structure is used
			ReadFile(NAV1ReadHandle, &InputPacketBuffer, 65, &BytesRead, 0);		//Blocking function, unless an "overlapped" structure is used
		}
		if(NAV1isNAV2)
		{
			WriteFile(NAV2WriteHandle, &OutputPacketBuffer, 65, &BytesWritten, 0);	//Blocking function, unless an "overlapped" structure is used
			ReadFile(NAV2ReadHandle, &InputPacketBuffer, 65, &BytesRead, 0);		//Blocking function, unless an "overlapped" structure is used
		}
		if(InputPacketBuffer[2]==0x01)	//NAV1 Frequency has changed
		{

				uiNAVStandbyData = 0x0000;
				uiNAVStandbyData |= (InputPacketBuffer[7]<<12);
				uiNAVStandbyData |= (InputPacketBuffer[8]<<8);
				uiNAVStandbyData |= (InputPacketBuffer[9]<<4);
				uiNAVStandbyData |= InputPacketBuffer[10];

				//send NAV1 stby Freq
				FSUIPC_Write(0x311E, 2, &uiNAVStandbyData, &dwResult);
				//FSUIPC_Process(&dwResult); // Process the request(s)

				uiNAVData = 0x0000;
				uiNAVData |= (InputPacketBuffer[3]<<12);
				uiNAVData |= (InputPacketBuffer[4]<<8);
				uiNAVData |= (InputPacketBuffer[5]<<4);
				uiNAVData |= InputPacketBuffer[6];
				
				//send NAV1 Freq
				FSUIPC_Write(0x0350, 2, &uiNAVData, &dwResult);
					FSUIPC_Process(&dwResult); // Process the request(s)
				
		}
	}

	if(module==2)
	{
		if(NAV2isNAV1)
		{
			WriteFile(NAV1WriteHandle, &OutputPacketBuffer, 65, &BytesWritten, 0);	//Blocking function, unless an "overlapped" structure is used
			ReadFile(NAV1ReadHandle, &InputPacketBuffer, 65, &BytesRead, 0);		//Blocking function, unless an "overlapped" structure is used
		}
		if(NAV2isNAV2)
		{
			WriteFile(NAV2WriteHandle, &OutputPacketBuffer, 65, &BytesWritten, 0);	//Blocking function, unless an "overlapped" structure is used
			ReadFile(NAV2ReadHandle, &InputPacketBuffer, 65, &BytesRead, 0);		//Blocking function, unless an "overlapped" structure is used
		}
		if(InputPacketBuffer[2]==0x01)	//NAV1 Frequency has changed
		{

				uiNAVStandbyData = 0x0000;
				uiNAVStandbyData |= (InputPacketBuffer[7]<<12);
				uiNAVStandbyData |= (InputPacketBuffer[8]<<8);
				uiNAVStandbyData |= (InputPacketBuffer[9]<<4);
				uiNAVStandbyData |= InputPacketBuffer[10];

				//send NAV1 stby Freq
				FSUIPC_Write(0x3120, 2, &uiNAVStandbyData, &dwResult);
				//FSUIPC_Process(&dwResult); // Process the request(s)

				uiNAVData = 0x0000;
				uiNAVData |= (InputPacketBuffer[3]<<12);
				uiNAVData |= (InputPacketBuffer[4]<<8);
				uiNAVData |= (InputPacketBuffer[5]<<4);
				uiNAVData |= InputPacketBuffer[6];
				
				//send NAV1 Freq
				FSUIPC_Write(0x0352, 2, &uiNAVData, &dwResult);
					FSUIPC_Process(&dwResult); // Process the request(s)
				
		}
	}

}
void ReadNAVSwitch(int module)
{
	int i;
	DWORD dwDataLength=0;
	DWORD dwResult;
	char tempData[3];
	DWORD temp;
	DWORD BytesWritten = 0;
	DWORD BytesRead = 0;
	unsigned char OutputPacketBuffer[65];	//Allocate a memory buffer equal to our endpoint size + 1
	unsigned char InputPacketBuffer[65];	//Allocate a memory buffer equal to our endpoint size + 1

	for(i=0; i<65; i++)
	{
		InputPacketBuffer[i] = 0;				//The first byte is the "Report ID" and does not get transmitted over the USB bus.  Always set = 0.
		OutputPacketBuffer[i] = 0;				//The first byte is the "Report ID" and does not get transmitted over the USB bus.  Always set = 0.
	}
	OutputPacketBuffer[1] = 0x73;			//0x72 is the "Get Pushbutton State" command in the firmware
											//For simplicity, we will leave the rest of the buffer uninitialized, but you could put real
											//data in it if you like.

	//The basic Windows I/O functions WriteFile() and ReadFile() can be used to read and write to HID class USB devices 
	//(once we have the read and write handles to the device, which are obtained with CreateFile()).
	

	if(module==1)
	{
		if(NAV1isNAV1)
		{
			WriteFile(NAV1WriteHandle, &OutputPacketBuffer, 65, &BytesWritten, 0);	//Blocking function, unless an "overlapped" structure is used
			ReadFile(NAV1ReadHandle, &InputPacketBuffer, 65, &BytesRead, 0);		//Blocking function, unless an "overlapped" structure is used
		}
		if(NAV1isNAV2)
		{
			WriteFile(NAV2WriteHandle, &OutputPacketBuffer, 65, &BytesWritten, 0);	//Blocking function, unless an "overlapped" structure is used
			ReadFile(NAV2ReadHandle, &InputPacketBuffer, 65, &BytesRead, 0);		//Blocking function, unless an "overlapped" structure is used
		}
		if(InputPacketBuffer[2]==0x01)	//ANT switch has been pressed
		{
			//display "No Err" ?
			if(ulNAVTEST1Offset==NO_ERR)
			{
				OutputPacketBuffer[1] = 0xEE;
				if(NAV1isNAV1)
				{
					WriteFile(NAV1WriteHandle, &OutputPacketBuffer, 65, &BytesWritten, 0);	//Blocking function, unless an "overlapped" structure is used
					ReadFile(NAV1ReadHandle, &InputPacketBuffer, 65, &BytesRead, 0);		//Blocking function, unless an "overlapped" structure is used
				}
				if(NAV1isNAV2)
				{
					WriteFile(NAV2WriteHandle, &OutputPacketBuffer, 65, &BytesWritten, 0);	//Blocking function, unless an "overlapped" structure is used
					ReadFile(NAV2ReadHandle, &InputPacketBuffer, 65, &BytesRead, 0);		//Blocking function, unless an "overlapped" structure is used
				}
				return;
			}

			if(NAVTEST1Toggle==true)		//
			{
				if(ulNAVTEST1Offset==GPS_NAV)
				{
					dwDataLength = 4;
				}
				if(ulNAVTEST1Offset==DME_SELECT)
				{
					dwDataLength = 2;
				}
				//need to do a bit mask operation?
				if(ulNAVTEST1Offset==RADIO_SOUND)
				{
					dwDataLength = 1;
					FSUIPC_Read(RADIO_SOUND, 1, tempData, &dwResult);
					FSUIPC_Process(&dwResult);
					if(ulNAVTEST1Value1==0xAA)
					{
						temp = tempData[0]|0x80;	//set COM1 Transmit
						temp = temp & 0xBF;			//clear COM2 Transmit
					}
					else if (ulNAVTEST1Value1==0xBB)
					{
						temp = tempData[0]|0x40;	//set COM2 Transmit
						temp = temp & 0x7F;			//clear COM1 Transmit
					}
					else
					{
						temp = tempData[0]|ulNAVTEST1Value1;
					}
					ulNAVTEST1Value1 = temp;
				}
				
				//send offset and value to FSUIPC
				if (FSUIPC_Write(ulNAVTEST1Offset, dwDataLength, &ulNAVTEST1Value1, &dwResult))
					FSUIPC_Process(&dwResult); // Process the request(s)
				NAVTEST1Toggle=false;
			}
			else								//switch is low
			{
				if(ulNAVTEST1Offset==GPS_NAV)
				{
					dwDataLength = 4;
				}
				if(ulNAVTEST1Offset==DME_SELECT)
				{
					dwDataLength = 2;
				}
				//need to do a bit mask operation?
				if(ulNAVTEST1Offset==RADIO_SOUND)
				{
					dwDataLength = 1;
					FSUIPC_Read(RADIO_SOUND, 1, tempData, &dwResult);
					FSUIPC_Process(&dwResult);
					if(ulNAVTEST1Value2==0xAA)
					{
						temp = tempData[0]|0x80;	//set COM1 Transmit
						temp = temp & 0xBF;			//clear COM2 Transmit
					}
					else if (ulNAVTEST1Value2==0xBB)
					{
						temp = tempData[0]|0x40;	//set COM2 Transmit
						temp = temp & 0x7F;			//clear COM1 Transmit
					}
					else
					{
						temp = tempData[0]|ulNAVTEST1Value2;
					}
					temp = tempData[0]&ulNAVTEST1Value2;
					ulNAVTEST1Value2 = temp;
				}
				
				//send offset and value of(0) to FSUIPC
				if (FSUIPC_Write(ulNAVTEST1Offset, dwDataLength, &ulNAVTEST1Value2, &dwResult))
					FSUIPC_Process(&dwResult); // Process the request(s)
				NAVTEST1Toggle=true;
			}
		}
	}

	if(module==2)
	{
		if(NAV2isNAV1)
		{
			WriteFile(NAV1WriteHandle, &OutputPacketBuffer, 65, &BytesWritten, 0);	//Blocking function, unless an "overlapped" structure is used
			ReadFile(NAV1ReadHandle, &InputPacketBuffer, 65, &BytesRead, 0);		//Blocking function, unless an "overlapped" structure is used
		}
		if(NAV2isNAV2)
		{
			WriteFile(NAV2WriteHandle, &OutputPacketBuffer, 65, &BytesWritten, 0);	//Blocking function, unless an "overlapped" structure is used
			ReadFile(NAV2ReadHandle, &InputPacketBuffer, 65, &BytesRead, 0);		//Blocking function, unless an "overlapped" structure is used
		}
		if(InputPacketBuffer[2]==0x01)	//ANT switch has been pressed
		{
			//display "No Err" ?
			if(ulNAVTEST2Offset==NO_ERR)
			{
				OutputPacketBuffer[1] = 0xEE;
				if(NAV2isNAV1)
				{
					WriteFile(NAV1WriteHandle, &OutputPacketBuffer, 65, &BytesWritten, 0);	//Blocking function, unless an "overlapped" structure is used
					ReadFile(NAV1ReadHandle, &InputPacketBuffer, 65, &BytesRead, 0);		//Blocking function, unless an "overlapped" structure is used
				}
				if(NAV2isNAV2)
				{
					WriteFile(NAV2WriteHandle, &OutputPacketBuffer, 65, &BytesWritten, 0);	//Blocking function, unless an "overlapped" structure is used
					ReadFile(NAV2ReadHandle, &InputPacketBuffer, 65, &BytesRead, 0);		//Blocking function, unless an "overlapped" structure is used
				}
				return;
			}

			if(NAVTEST2Toggle==true)		//
			{
				
				//need to do a bit mask operation?
				if(ulNAVTEST2Offset==RADIO_SOUND)
				{
					dwDataLength = 1;
					FSUIPC_Read(RADIO_SOUND, 1, tempData, &dwResult);
					FSUIPC_Process(&dwResult);
					if(ulNAVTEST2Value1==0xAA)
					{
						temp = tempData[0]|0x80;	//set COM1 Transmit
						temp = temp & 0xBF;			//clear COM2 Transmit
					}
					else if (ulNAVTEST2Value1==0xBB)
					{
						temp = tempData[0]|0x40;	//set COM2 Transmit
						temp = temp & 0x7F;			//clear COM1 Transmit
					}
					else
					{
						temp = tempData[0]|ulNAVTEST2Value1;
					}
					ulNAVTEST2Value1 = temp;
				}
				
				//send offset and value to FSUIPC
				if (FSUIPC_Write(ulNAVTEST2Offset, dwDataLength, &ulNAVTEST2Value1, &dwResult))
					FSUIPC_Process(&dwResult); // Process the request(s)
				NAVTEST2Toggle=false;
			}
			else								//switch is low
			{
				
				//need to do a bit mask operation?
				if(ulNAVTEST2Offset==RADIO_SOUND)
				{
					dwDataLength = 1;
					FSUIPC_Read(RADIO_SOUND, 1, tempData, &dwResult);
					FSUIPC_Process(&dwResult);
					if(ulNAVTEST2Value2==0xAA)
					{
						temp = tempData[0]|0x80;	//set COM1 Transmit
						temp = temp & 0xBF;			//clear COM2 Transmit
					}
					else if (ulNAVTEST2Value2==0xBB)
					{
						temp = tempData[0]|0x40;	//set COM2 Transmit
						temp = temp & 0x7F;			//clear COM1 Transmit
					}
					else
					{
						temp = tempData[0]|ulNAVTEST2Value2;
					}
					temp = tempData[0]&ulNAVTEST2Value2;
					ulNAVTEST2Value2 = temp;
				}
				
				//send offset and value of(0) to FSUIPC
				if (FSUIPC_Write(ulNAVTEST2Offset, dwDataLength, &ulNAVTEST2Value2, &dwResult))
					FSUIPC_Process(&dwResult); // Process the request(s)
				NAVTEST2Toggle=true;
			}
		}
	}
}