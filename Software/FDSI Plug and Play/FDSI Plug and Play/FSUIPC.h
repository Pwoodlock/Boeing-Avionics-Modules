#ifndef FSUIPC_H
#define FSUIPC_H

#include "FSUIPC_User.h"

extern bool FS_Detected;
extern char chMsg[128], chTimeMsg[64];
extern char test[10];

extern bool COMTEST1Toggle;
extern bool COMTEST2Toggle;
extern bool NAVTEST1Toggle;
extern bool NAVTEST2Toggle;
extern bool ADFTEST1Toggle;
extern bool ADFTEST2Toggle;

//devices present
extern unsigned int uiCOM1Exsits;
extern unsigned int uiCOM2Exsits;
extern unsigned int uiNAV1Exsits;
extern unsigned int uiNAV2Exsits;
extern unsigned int uiADF1Exsits;
extern unsigned int uiADF2Exsits;

extern unsigned int COM1isCOM1;
extern unsigned int COM2isCOM2;
extern unsigned int NAV1isNAV1;
extern unsigned int NAV2isNAV2;
extern unsigned int ADF1isADF1;
extern unsigned int ADF2isADF2;

extern unsigned int COM1isCOM2;
extern unsigned int COM2isCOM1;
extern unsigned int NAV1isNAV2;
extern unsigned int NAV2isNAV1;
extern unsigned int ADF1isADF2;
extern unsigned int ADF2isADF1;

//display values
extern unsigned int uiCOMData;
extern unsigned int uiCOMStandbyData;
extern unsigned int uiNAVData;
extern unsigned int uiNAVStandbyData;
extern unsigned int uiCOMTFRData;
extern unsigned int uiADFData;
extern unsigned int uiADFDataEx;
//switch offsets - read from ini file
extern DWORD ulADF1ANTOffset;
extern DWORD ulADF2ANTOffset;
extern DWORD ulADF1TONEOffset;
extern DWORD ulADF2TONEOffset;
extern DWORD ulCOMTEST1Offset;
extern DWORD ulCOMTEST2Offset;
extern DWORD ulNAVTEST1Offset;
extern DWORD ulNAVTEST2Offset;
//switch values
extern unsigned int ulADF1Value1;
extern unsigned int ulADF1Value2;
extern unsigned int ulADF2Value1;
extern unsigned int ulADF2Value2;
extern unsigned int ulTONE1Value1;
extern unsigned int ulTONE1Value2;
extern unsigned int ulTONE2Value1;
extern unsigned int ulTONE2Value2;
extern unsigned int ulCOMTEST1Value1;
extern unsigned int ulCOMTEST1Value2;
extern unsigned int ulCOMTEST2Value1;
extern unsigned int ulCOMTEST2Value2;
extern unsigned int ulNAVTEST1Value1;
extern unsigned int ulNAVTEST1Value2;
extern unsigned int ulNAVTEST2Value1;
extern unsigned int ulNAVTEST2Value2;

// Added for version 1.1.0
// The following Arrays contain offsets and values for the Advanced Tab
// dropdown selection boxes
extern DWORD AdvancedOffsetArray[11];


extern void Check_FSUIPC(void);
extern int DetectCOMModule(void);
extern int DetectNAVModule(void);
extern int DetectADFModule(void);
extern void ReadADFValue(int module);
extern void ReadANTSwitch(int module);
extern void ReadTONESwitch(int module);
extern void ReadCOMValue(int module);
extern void ReadCOMSwitch(int module);
extern void ReadNAVValue(int module);
extern void ReadNAVSwitch(int module);



#endif //FSUIPC_H