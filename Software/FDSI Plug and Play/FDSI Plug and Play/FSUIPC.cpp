#include <windows.h>
#include <stdio.h>
#include "FSUIPC_User.h"
#include "Form1.h"

bool FS_Detected;
DWORD dwResult;
char chMsg[128], chTimeMsg[64];



char *pszErrors[] =
	{	"Okay",
		"Attempt to Open when already Open",
		"Cannot link to FSUIPC or WideClient",
		"Failed to Register common message with Windows",
		"Failed to create Atom for mapping filename",
		"Failed to create a file mapping object",
		"Failed to open a view to the file map",
		"Incorrect version of FSUIPC, or not FSUIPC",
		"Sim is not version requested",
		"Call cannot execute, link not Open",
		"Call cannot execute: no requests accumulated",
		"IPC timed out all retries",
		"IPC sendmessage failed all retries",
		"IPC request contains bad data",
		"Maybe running on WideClient, but FS not running on Server, or wrong FSUIPC",
		"Read or Write request cannot be added, memory for Process is full",
	};


void Check_FSUIPC(void)
{
	if (FSUIPC_Open(SIM_ANY, &dwResult))
	{	
		char chTime[3];
		BOOL fTimeOk = TRUE;
		static char *pFS[] = { "FS98", "FS2000", "CFS2", "CFS1", "Fly!", "FS2002", "FS2004", "FSX", "ESP" };  // Change made 060603

		// Okay, we're linked, and already the FSUIPC_Open has had an initial
		// exchange with FSUIPC to get its version number and to differentiate
		// between FS's.

		// Now to auto-Register with FSUIPC, to save the user of an Unregistered FSUIPC
		// having to Register UIPCHello for us:
		static char chOurKey[] = "M70FE9R2Q3JG"; // As obtained from FDSI Ireland

		if (FSUIPC_Write(0x8001, 12, chOurKey, &dwResult))
			FSUIPC_Process(&dwResult); // Process the request(s)

		// I've not checked the reslut of the above -- if it didn't register us,
		// and FSUIPC isn't fully user-Registered, the next request will not
		// return the FS lock time
		
		// As an example of retrieving data, well also get the FS clock time too:
		if (!FSUIPC_Read(0x238, 3, chTime, &dwResult) ||
				// If we wanted other reads/writes at the same time, we could put them here
				!FSUIPC_Process(&dwResult)) // Process the request(s)
			fTimeOk = FALSE;
		
		// Now display all the knowledge we've accrued:
		if (fTimeOk)
			sprintf_s(chTimeMsg, "Request for time ok: FS clock = %02d:%02d:%02d", chTime[0], chTime[1], chTime[2]);
		
		else
			sprintf_s(chTimeMsg, "Request for time failed: %s", pszErrors[dwResult]);
		
		
		sprintf_s(chMsg, "Sim is %s,   FSUIPC Version = %c.%c%c%c%c",
			(FSUIPC_FS_Version && (FSUIPC_FS_Version <= 9)) ? pFS[FSUIPC_FS_Version - 1] : "Unknown FS", // Change made 280808 - N.Evans - To include FSX & ESP
			'0' + (0x0f & (FSUIPC_Version >> 28)),
			'0' + (0x0f & (FSUIPC_Version >> 24)),
			'0' + (0x0f & (FSUIPC_Version >> 20)),
			'0' + (0x0f & (FSUIPC_Version >> 16)),
			(FSUIPC_Version & 0xffff) ? 'a' + (FSUIPC_Version & 0xff) - 1 : ' ');

		

		FS_Detected = 1;
	}

	else
	{
		//MessageBox (NULL, pszErrors[dwResult], "UIPChello: Failed to open link to FSUIPC", 0) ;
		FSUIPC_Close(); // Closing when it wasn't open is okay, so this is safe here
		FS_Detected = 0;
	}

	
}

