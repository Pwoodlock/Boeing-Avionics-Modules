/********************************************************************
 FileName:     main.c
 Dependencies: See INCLUDES section
 Processor:		PIC18 or PIC24 USB Microcontrollers
 Hardware:		The code is natively intended to be used on the following
 				hardware platforms: PICDEM™ FS USB Demo Board, 
 				PIC18F87J50 FS USB Plug-In Module, or
 				Explorer 16 + PIC24 USB PIM.  The firmware may be
 				modified for use on other USB platforms by editing the
 				HardwareProfile.h file.
 Complier:  	Microchip C18 (for PIC18) or C30 (for PIC24)
 Company:		Microchip Technology, Inc.

 Software License Agreement:

 The software supplied herewith by Microchip Technology Incorporated
 (the “Company”) for its PIC® Microcontroller is intended and
 supplied to you, the Company’s customer, for use solely and
 exclusively on Microchip PIC Microcontroller products. The
 software is owned by the Company and/or its supplier, and is
 protected under applicable copyright laws. All rights are reserved.
 Any use in violation of the foregoing restrictions may subject the
 user to criminal sanctions under applicable laws, as well as to
 civil liability for the breach of the terms and conditions of this
 license.

 THIS SOFTWARE IS PROVIDED IN AN “AS IS” CONDITION. NO WARRANTIES,
 WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
 TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT,
 IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR
 CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.

********************************************************************
 File Description:

 Change History:
  Rev   Date         Description
  1.0   11/19/2004   Initial release
  2.1   02/26/2007   Updated for simplicity and to use common
                     coding style
********************************************************************/

#ifndef USBMOUSE_C
#define USBMOUSE_C

/** INCLUDES *******************************************************/
#include "GenericTypeDefs.h"
#include "Compiler.h"
#include "usb_config.h"
#include "./USB/usb_device.h"
#include "./USB/usb.h"

#include "HardwareProfile.h"

#include "./USB/usb_function_hid.h"

#include "Globals.h"
#include <delays.h>

/** CONFIGURATION **************************************************/

        #pragma config PLLDIV   = 10         // (40 MHz crystal on PICDEM FS USB board)
        #pragma config CPUDIV   = OSC1_PLL2   
        #pragma config USBDIV   = 2         // Clock source from 96MHz PLL/2
        #pragma config FOSC     = HSPLL_HS
        #pragma config FCMEN    = OFF
        #pragma config IESO     = OFF
        #pragma config PWRT     = OFF
        #pragma config BOR      = ON
        #pragma config BORV     = 3
        #pragma config VREGEN   = ON      //USB Voltage Regulator
        #pragma config WDT      = OFF
        #pragma config WDTPS    = 32768
        #pragma config MCLRE    = ON
        #pragma config LPT1OSC  = OFF
        #pragma config PBADEN   = OFF
//      #pragma config CCP2MX   = ON
        #pragma config STVREN   = ON
        #pragma config LVP      = OFF
//      #pragma config ICPRT    = OFF       // Dedicated In-Circuit Debug/Programming
        #pragma config XINST    = OFF       // Extended Instruction Set
        #pragma config CP0      = OFF
        #pragma config CP1      = OFF
//      #pragma config CP2      = OFF
//      #pragma config CP3      = OFF
        #pragma config CPB      = OFF
//      #pragma config CPD      = OFF
        #pragma config WRT0     = OFF
        #pragma config WRT1     = OFF
//      #pragma config WRT2     = OFF
//      #pragma config WRT3     = OFF
        #pragma config WRTB     = OFF       // Boot Block Write Protection
        #pragma config WRTC     = OFF
//      #pragma config WRTD     = OFF
        #pragma config EBTR0    = OFF
        #pragma config EBTR1    = OFF
//      #pragma config EBTR2    = OFF
//      #pragma config EBTR3    = OFF
        #pragma config EBTRB    = OFF




/** VARIABLES ******************************************************/
#pragma udata
int displayDelay=50;
BOOL emulate_mode;
BYTE movement_length;
BYTE vector = 0;
BYTE pb1Pressed=0, pb2Pressed=0;
unsigned long Timeout=0x1FF;
unsigned long display1Value, display2Value;
unsigned long display1Value_bak, display2Value_bak;
static unsigned long standbyMSB, standbyLSB, directionMSB, directionLSB;
BYTE old_ENC_A,old_ENC_C;
BYTE DisplayNoErr=0,DisplayNoErr_Timer=0;
unsigned long DisplayNoErr_Count;

#pragma udata USB_VARS
unsigned char ReceivedDataBuffer[64];
unsigned char ToSendDataBuffer[64];
#pragma udata

USB_HANDLE USBOutHandle = 0;
USB_HANDLE USBInHandle = 0;
BOOL blinkStatusValid = TRUE;

/** PRIVATE PROTOTYPES *********************************************/
//void BlinkUSBStatus(void);
BOOL Switch2IsPressed(void);
BOOL Switch3IsPressed(void);
void Emulate_Mouse(void);
static void InitializeSystem(void);
void ProcessIO(void);
void UserInit(void);
void YourHighPriorityISRCode();
void YourLowPriorityISRCode();
void display1(unsigned long value);
void display2(unsigned long value);
void swapDisplays(void);
void updateEncoders(void);
void updateSwitches(void);

/** VECTOR REMAPPING ***********************************************/
#if defined(__18CXX)
	//On PIC18 devices, addresses 0x00, 0x08, and 0x18 are used for
	//the reset, high priority interrupt, and low priority interrupt
	//vectors.  However, the current Microchip USB bootloader 
	//examples are intended to occupy addresses 0x00-0x7FF or
	//0x00-0xFFF depending on which bootloader is used.  Therefore,
	//the bootloader code remaps these vectors to new locations
	//as indicated below.  This remapping is only necessary if you
	//wish to program the hex file generated from this project with
	//the USB bootloader.  If no bootloader is used, edit the
	//usb_config.h file and comment out the following defines:
	//#define PROGRAMMABLE_WITH_USB_HID_BOOTLOADER
	//#define PROGRAMMABLE_WITH_USB_LEGACY_CUSTOM_CLASS_BOOTLOADER
	
	#if defined(PROGRAMMABLE_WITH_USB_HID_BOOTLOADER)
		#define REMAPPED_RESET_VECTOR_ADDRESS			0x1000
		#define REMAPPED_HIGH_INTERRUPT_VECTOR_ADDRESS	0x1008
		#define REMAPPED_LOW_INTERRUPT_VECTOR_ADDRESS	0x1018
	#elif defined(PROGRAMMABLE_WITH_USB_MCHPUSB_BOOTLOADER)	
		#define REMAPPED_RESET_VECTOR_ADDRESS			0x800
		#define REMAPPED_HIGH_INTERRUPT_VECTOR_ADDRESS	0x808
		#define REMAPPED_LOW_INTERRUPT_VECTOR_ADDRESS	0x818
	#else	
		#define REMAPPED_RESET_VECTOR_ADDRESS			0x00
		#define REMAPPED_HIGH_INTERRUPT_VECTOR_ADDRESS	0x08
		#define REMAPPED_LOW_INTERRUPT_VECTOR_ADDRESS	0x18
	#endif
	
	#if defined(PROGRAMMABLE_WITH_USB_HID_BOOTLOADER)||defined(PROGRAMMABLE_WITH_USB_MCHPUSB_BOOTLOADER)
	extern void _startup (void);        // See c018i.c in your C18 compiler dir
	#pragma code REMAPPED_RESET_VECTOR = REMAPPED_RESET_VECTOR_ADDRESS
	void _reset (void)
	{
	    _asm goto _startup _endasm
	}
	#endif
	#pragma code REMAPPED_HIGH_INTERRUPT_VECTOR = REMAPPED_HIGH_INTERRUPT_VECTOR_ADDRESS
	void Remapped_High_ISR (void)
	{
	     _asm goto YourHighPriorityISRCode _endasm
	}
	#pragma code REMAPPED_LOW_INTERRUPT_VECTOR = REMAPPED_LOW_INTERRUPT_VECTOR_ADDRESS
	void Remapped_Low_ISR (void)
	{
	     _asm goto YourLowPriorityISRCode _endasm
	}
	
	#if defined(PROGRAMMABLE_WITH_USB_HID_BOOTLOADER)||defined(PROGRAMMABLE_WITH_USB_MCHPUSB_BOOTLOADER)
	//Note: If this project is built while one of the bootloaders has
	//been defined, but then the output hex file is not programmed with
	//the bootloader, addresses 0x08 and 0x18 would end up programmed with 0xFFFF.
	//As a result, if an actual interrupt was enabled and occured, the PC would jump
	//to 0x08 (or 0x18) and would begin executing "0xFFFF" (unprogrammed space).  This
	//executes as nop instructions, but the PC would eventually reach the REMAPPED_RESET_VECTOR_ADDRESS
	//(0x1000 or 0x800, depending upon bootloader), and would execute the "goto _startup".  This
	//would effective reset the application.
	
	//To fix this situation, we should always deliberately place a 
	//"goto REMAPPED_HIGH_INTERRUPT_VECTOR_ADDRESS" at address 0x08, and a
	//"goto REMAPPED_LOW_INTERRUPT_VECTOR_ADDRESS" at address 0x18.  When the output
	//hex file of this project is programmed with the bootloader, these sections do not
	//get bootloaded (as they overlap the bootloader space).  If the output hex file is not
	//programmed using the bootloader, then the below goto instructions do get programmed,
	//and the hex file still works like normal.  The below section is only required to fix this
	//scenario.
	#pragma code HIGH_INTERRUPT_VECTOR = 0x08
	void High_ISR (void)
	{
	     _asm goto REMAPPED_HIGH_INTERRUPT_VECTOR_ADDRESS _endasm
	}
	#pragma code LOW_INTERRUPT_VECTOR = 0x18
	void Low_ISR (void)
	{
	     _asm goto REMAPPED_LOW_INTERRUPT_VECTOR_ADDRESS _endasm
	}
	#endif	//end of "#if defined(PROGRAMMABLE_WITH_USB_HID_BOOTLOADER)||defined(PROGRAMMABLE_WITH_USB_LEGACY_CUSTOM_CLASS_BOOTLOADER)"

	#pragma code
	
	
	//These are your actual interrupt handling routines.
	#pragma interrupt YourHighPriorityISRCode
	void YourHighPriorityISRCode()
	{
			if (INTCONbits.TMR0IF)
		    {                                   //check for TMR0 overflow
		      

			if(ENC_A != old_ENC_A)
			{
					updateEncoders();
					if(ENC_D ^ ENC_A ==0)
					{
						display2Value-=5;
						if(display2Value<10850)display2Value=11750;
					}
					else
					{	
						display2Value+=5;
						if(display2Value>11750)display2Value=10850;
					}
				
				standbyMSB = display2Value/100;
				standbyLSB = display2Value-(standbyMSB*100);
			}

			//check for Encoder changes
			if(ENC_C != old_ENC_C)
			{
					updateEncoders();
					if(ENC_B ^ ENC_C ==1)
					{
						standbyMSB--;
						if(standbyMSB==107)standbyMSB=117;
					}
					else
					{
						standbyMSB++;
						if(standbyMSB==118)standbyMSB=108;
					}

					display2Value = (standbyMSB*100)+standbyLSB;
			}

			if(DisplayNoErr==1)
				{
					DisplayNoErr_Count++;
					if(DisplayNoErr_Count>999)
					{
						DisplayNoErr_Count=0;
						DisplayNoErr=0;
						//restore display values...
						display2Value = display2Value_bak;
					}
				}
				else
				{
					display2(display2Value);
				}

			  //Display Character 1 = ON
			  PORTD = 0x01;
			  PORTB = 0;
			  //Display Character 1 data
			  PORTA = (numbers[ucDisplay1[0]] & 0x3F);
			  PORTE = ((numbers[ucDisplay1[0]]>>6) & 0x03);
			  Delay100TCYx(displayDelay);
			  //Display Character 2 = ON
			  PORTD = 0x02;
			  PORTB = 0;
			  //Display Character 2 data
			  PORTA = (numbers[ucDisplay1[1]] & 0x3F);
			  PORTE = ((numbers[ucDisplay1[1]]>>6) & 0x03);
			  Delay100TCYx(displayDelay);
			  //Display Character 3 = ON
			  PORTD = 0x04;
			  PORTB = 0;
			  //Display Character 3 data
			  PORTA = (numbers[ucDisplay1[2]] & 0x3F);
			  PORTE = ((numbers[ucDisplay1[2]]>>6) | 0x01);	//decimal point on
			  Delay100TCYx(displayDelay);
			  //Display Character 4 = ON
			  PORTD = 0x08;
			  PORTB = 0;
			  //Display Character 4 data
			  PORTA = (numbers[ucDisplay1[3]] & 0x3F);
			  PORTE = ((numbers[ucDisplay1[3]]>>6) & 0x03);	
			  Delay100TCYx(displayDelay);
			  //Display Character 5 = ON
			  PORTD = 0x10;
			  PORTB = 0;
			  //Display Character 5 data
			  PORTA = (numbers[ucDisplay1[4]] & 0x3F);
			  PORTE = ((numbers[ucDisplay1[4]]>>6) & 0x03);
			  Delay100TCYx(displayDelay);
			  //Display Character 7 = ON
			  PORTD = 0x40;
			  PORTB = 0;
			  //Display Character 7 data
			  PORTA = (numbers[ucDisplay2[0]] & 0x3F);
			  PORTE = ((numbers[ucDisplay2[0]]>>6) & 0x03);
			  Delay100TCYx(displayDelay);
			  //Display Character 8 = ON
			  PORTD = 0x80;
			  PORTB = 0;
			  //Display Character 8 data
			  PORTA = (numbers[ucDisplay2[1]] & 0x3F);
			  PORTE = ((numbers[ucDisplay2[1]]>>6) & 0x03);
			  Delay100TCYx(displayDelay);
			  //Display Character 9 = ON
			  PORTD = 0;
			  PORTB = 0x01;
			  //Display Character 9 data
			  PORTA = (numbers[ucDisplay2[2]] & 0x3F);
			  PORTE = ((numbers[ucDisplay2[2]]>>6) | 0x01);//decimal point on
			  Delay100TCYx(displayDelay);
			  //Display Character 10 = ON
			  PORTD = 0;
			  PORTB = 0x02;
			  //Display Character 10 data
			  PORTA = (numbers[ucDisplay2[3]] & 0x3F);
			  PORTE = ((numbers[ucDisplay2[3]]>>6) & 0x03);	
			  Delay100TCYx(displayDelay);
			  //Display Character 11 = ON
			  PORTD = 0;
			  PORTB = 0x04;
			  //Display Character 11 data
			  PORTA = (numbers[ucDisplay2[4]] & 0x3F);
			  PORTE = ((numbers[ucDisplay2[4]]>>6) & 0x03);
			  //Delay100TCYx(displayDelay);

			  
			  //the timeout value stops the module from crashing, should a USB request not be received. i.e. FDSI Plug and play is not running
			  
			  Timeout--;
			  if(Timeout==0)
			  {
			  	pb1Pressed=0;	//clear button flag
				Timeout=0x1FF;	//reset timeout value
			  }
			  
			  //only do this if pb1 has NOT already been pressed, otherwise, the displays will keep swapping while the button is held
			  if(!pb1Pressed)
			  {	
				  if(pb1==0)		// if pb1 is pressed
			      {
		  		  	Timeout=0x1FF;	//reset timeout value
				  	swapDisplays(); //swap the standby and active values on the displays
				  	pb1Pressed=1;	//set the pb1Pressed flag for the USB routine.
			  	  }
			  }

			  //only do this if pb2 has NOT already been pressed.
			  if(!pb2Pressed)
			  {	
				  if(pb2==0)		// if pb1 is pressed
			      {
		  		  	Timeout=0x1FF;	//reset timeout value
				  	pb2Pressed=1;	//set the pb2Pressed flag for the USB routine.
			  	  }
			  }

			

            INTCONbits.TMR0IF = 0;            //clear interrupt flag
	   }
			
	}	//This return will be a "retfie fast", since this is in a #pragma interrupt section 
	#pragma interruptlow YourLowPriorityISRCode
	void YourLowPriorityISRCode()
	{
	
	
	}	//This return will be a "retfie", since this is in a #pragma interruptlow section 

#elif defined(__C30__)
    #if defined(PROGRAMMABLE_WITH_USB_HID_BOOTLOADER)
        /*
         *	ISR JUMP TABLE
         *
         *	It is necessary to define jump table as a function because C30 will
         *	not store 24-bit wide values in program memory as variables.
         *
         *	This function should be stored at an address where the goto instructions 
         *	line up with the remapped vectors from the bootloader's linker script.
         *  
         *  For more information about how to remap the interrupt vectors,
         *  please refer to AN1157.  An example is provided below for the T2
         *  interrupt with a bootloader ending at address 0x1400
         */
        void __attribute__ ((address(0x1404))) ISRTable(){
        
        	asm("reset"); //reset instruction to prevent runaway code
        	asm("goto %0"::"i"(&_T2Interrupt));  //T2Interrupt's address
        }
    #endif
#endif //of "#if defined(__18CXX)"




/** DECLARATIONS ***************************************************/
#pragma code

/********************************************************************
 * Function:        void main(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        Main program entry point.
 *
 * Note:            None
 *******************************************************************/
#if defined(__18CXX)
void main(void)
#else
int main(void)
#endif
{
//    //This can be used for user entry into the bootloader  
//    #if defined(__C30__) 
//        mInitSwitch2();
//        if(sw2 == 0)
//        {
//            EnterBootloader();
//        }
//    #endif

    InitializeSystem();

	TRISA = 0;		//outputs
	TRISB = 0;		
	TRISD = 0;		//outputs
	TRISE = 0;

	PORTA = 0xFF;
	PORTB = 0;
	PORTD = 0;
	PORTE = 0xFF;

	mInitAllSwitches();
	mInitEncoder();
	updateEncoders();

	display1Value = 10850;
	standbyMSB = 117;
	standbyLSB = 50;
	display2Value = (standbyMSB*100)+standbyLSB;

	display1(display1Value);
	display2(display2Value);


	INTCON = 0x20;                //disable global and enable TMR0 interrupt
	INTCON2 = 0x84;               //TMR0 high priority
	RCONbits.IPEN = 1;            //enable priority levels
	TMR0H = 0;                    //clear timer
	TMR0L = 0;                    //clear timer
	T0CON = 0xC7;                 //set up timer0
	INTCONbits.GIEH = 1;          //enable interrupts

    while(1)
    {
		// Check bus status and service USB interrupts.
        USBDeviceTasks(); // Interrupt or polling method.  If using polling, must call
        				  // this function periodically.  This function will take care
        				  // of processing and responding to SETUP transactions 
        				  // (such as during the enumeration process when you first
        				  // plug in).  USB hosts require that USB devices should accept
        				  // and process SETUP packets in a timely fashion.  Therefore,
        				  // when using polling, this function should be called 
        				  // frequently (such as once about every 100 microseconds) at any
        				  // time that a SETUP packet might reasonably be expected to
        				  // be sent by the host to your device.  In most cases, the
        				  // USBDeviceTasks() function does not take very long to
        				  // execute (~50 instruction cycles) before it returns.
    				  
		

		// Application-specific tasks.
		
		// Application related code may be added here, or in the ProcessIO() function.
        ProcessIO();        
    }//end while
}//end main


/********************************************************************
 * Function:        static void InitializeSystem(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        InitializeSystem is a centralize initialization
 *                  routine. All required USB initialization routines
 *                  are called from here.
 *
 *                  User application initialization routine should
 *                  also be called from here.                  
 *
 * Note:            None
 *******************************************************************/
static void InitializeSystem(void)
{
    #if (defined(__18CXX) & !defined(PIC18F87J50_PIM))
        ADCON1 |= 0x0F;                 // Default all pins to digital
    #elif defined(__C30__)
        AD1PCFGL = 0xFFFF;
    #endif

    #if defined(PIC18F87J50_PIM)
	//On the PIC18F87J50 Family of USB microcontrollers, the PLL will not power up and be enabled
	//by default, even if a PLL enabled oscillator configuration is selected (such as HS+PLL).
	//This allows the device to power up at a lower initial operating frequency, which can be
	//advantageous when powered from a source which is not gauranteed to be adequate for 48MHz
	//operation.  On these devices, user firmware needs to manually set the OSCTUNE<PLLEN> bit to
	//power up the PLL.
    {
        unsigned int pll_startup_counter = 600;
        OSCTUNEbits.PLLEN = 1;  //Enable the PLL and wait 2+ms until the PLL locks before enabling USB module
        while(pll_startup_counter--);
    }
    //Device switches over automatically to PLL output after PLL is locked and ready.

	//Configure all I/O pins to use digital input buffers.  The PIC18F87J50 Family devices
	//use the ANCONx registers to control this, which is different from other devices which
	//use the ADCON1 register for this purpose.
    WDTCONbits.ADSHR = 1;			// Select alternate SFR location to access ANCONx registers
    ANCON0 = 0xFF;                  // Default all pins to digital
    ANCON1 = 0xFF;                  // Default all pins to digital
    WDTCONbits.ADSHR = 0;			// Select normal SFR locations
    #endif
    
//	The USB specifications require that USB peripheral devices must never source
//	current onto the Vbus pin.  Additionally, USB peripherals should not source
//	current on D+ or D- when the host/hub is not actively powering the Vbus line.
//	When designing a self powered (as opposed to bus powered) USB peripheral
//	device, the firmware should make sure not to turn on the USB module and D+
//	or D- pull up resistor unless Vbus is actively powered.  Therefore, the
//	firmware needs some means to detect when Vbus is being powered by the host.
//	A 5V tolerant I/O pin can be connected to Vbus (through a resistor), and
// 	can be used to detect when Vbus is high (host actively powering), or low
//	(host is shut down or otherwise not supplying power).  The USB firmware
// 	can then periodically poll this I/O pin to know when it is okay to turn on
//	the USB module/D+/D- pull up resistor.  When designing a purely bus powered
//	peripheral device, it is not possible to source current on D+ or D- when the
//	host is not actively providing power on Vbus. Therefore, implementing this
//	bus sense feature is optional.  This firmware can be made to use this bus
//	sense feature by making sure "USE_USB_BUS_SENSE_IO" has been defined in the
//	HardwareProfile.h file.    
    #if defined(USE_USB_BUS_SENSE_IO)
    tris_usb_bus_sense = INPUT_PIN; // See HardwareProfile.h
    #endif
    
//	If the host PC sends a GetStatus (device) request, the firmware must respond
//	and let the host know if the USB peripheral device is currently bus powered
//	or self powered.  See chapter 9 in the official USB specifications for details
//	regarding this request.  If the peripheral device is capable of being both
//	self and bus powered, it should not return a hard coded value for this request.
//	Instead, firmware should check if it is currently self or bus powered, and
//	respond accordingly.  If the hardware has been configured like demonstrated
//	on the PICDEM FS USB Demo Board, an I/O pin can be polled to determine the
//	currently selected power source.  On the PICDEM FS USB Demo Board, "RA2" 
//	is used for	this purpose.  If using this feature, make sure "USE_SELF_POWER_SENSE_IO"
//	has been defined in HardwareProfile.h, and that an appropriate I/O pin has been mapped
//	to it in HardwareProfile.h.
    #if defined(USE_SELF_POWER_SENSE_IO)
    tris_self_power = INPUT_PIN;	// See HardwareProfile.h
    #endif
    
    USBDeviceInit();	//usb_device.c.  Initializes USB module SFRs and firmware
    					//variables to known states.
    UserInit();

}//end InitializeSystem



/******************************************************************************
 * Function:        void UserInit(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This routine should take care of all of the demo code
 *                  initialization that is required.
 *
 * Note:            
 *
 *****************************************************************************/
void UserInit(void)
{
    //Initialize all of the LED pins
    //mInitAllLEDs();
    
    //Initialize all of the push buttons
    //mInitAllSwitches();
    //old_sw2 = sw2;
    //old_sw3 = sw3;
    
    //initialize the variable holding the handle for the last
    // transmission
    USBOutHandle = 0;
    USBInHandle = 0;

    //blinkStatusValid = TRUE;
}//end UserInit

/********************************************************************
 * Function:        void ProcessIO(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This function is a place holder for other user
 *                  routines. It is a mixture of both USB and
 *                  non-USB tasks.
 *
 * Note:            None
 *******************************************************************/
void ProcessIO(void)
{   
	int i,j;

    // User Application USB tasks
    if((USBDeviceState < CONFIGURED_STATE)||(USBSuspendControl==1)) return;
    
    if(!HIDRxHandleBusy(USBOutHandle))
    {   
        blinkStatusValid = FALSE;
        switch(ReceivedDataBuffer[0])
        {
			
			case 0xEE:
				ToSendDataBuffer[0] = 0xEE;	
				
				//backup display values...
				display2Value_bak = display2Value;
				//Create "No Err"
				for(i=0; i<6; i++)
				{
				ucDisplay2[i]=Char_E;
				}
				
				if(!HIDTxHandleBusy(USBInHandle))
			    {
			            USBInHandle = HIDTxPacket(HID_EP,(BYTE*)&ToSendDataBuffer,64);
			    }
				DisplayNoErr=1;

			break;

            case 0x83:  //Request for TFR button press status
				if(pb1Pressed == 1)
				{	
					ToSendDataBuffer[0] = 0x83;				
					//load the display data into the send buffer
					ToSendDataBuffer[1] = 0x01;				//first byte says TFR button has been pressed
					//send COM frequency
					ToSendDataBuffer[2] = ucDisplay1[1];	
					ToSendDataBuffer[3] = ucDisplay1[2];		
					ToSendDataBuffer[4] = ucDisplay1[3];		
					ToSendDataBuffer[5] = ucDisplay1[4];		
					//send COM Standby frequency
					ToSendDataBuffer[6] = ucDisplay2[1];	
					ToSendDataBuffer[7] = ucDisplay2[2];		
					ToSendDataBuffer[8] = ucDisplay2[3];		
					ToSendDataBuffer[9] = ucDisplay2[4];
		
					pb1Pressed=0;
				}
				else
				{
					ToSendDataBuffer[0] = 0x83;	
					for(j=1; j<64; j++)
					{
						ToSendDataBuffer[j] = 0x00;	
					}
				}
				if(!HIDTxHandleBusy(USBInHandle))
			    {
			            USBInHandle = HIDTxPacket(HID_EP,(BYTE*)&ToSendDataBuffer,64);
			    }
			break;

			case 0x73:  //Request for TEST button press status
				if(pb2Pressed == 1)
				{	
					ToSendDataBuffer[0] = 0x73;				
					//load the display data into the send buffer
					ToSendDataBuffer[1] = 0x01;				//first byte says TFR button has been pressed		
					pb2Pressed=0;
				}
				else
				{
					ToSendDataBuffer[0] = 0x73;	
					for(i=1; i<64; i++)
					{
						ToSendDataBuffer[i] = 0x00;	
					}
				}
				if(!HIDTxHandleBusy(USBInHandle))
			    {
			            USBInHandle = HIDTxPacket(HID_EP,(BYTE*)&ToSendDataBuffer,64);
			    }
			break;

		}
		
        //Re-arm the OUT endpoint for the next packet
        USBOutHandle = HIDRxPacket(HID_EP,(BYTE*)&ReceivedDataBuffer,64);
    }

    
}//end ProcessIO

// ******************************************************************************************************
// ************** USB Callback Functions ****************************************************************
// ******************************************************************************************************
// The USB firmware stack will call the callback functions USBCBxxx() in response to certain USB related
// events.  For example, if the host PC is powering down, it will stop sending out Start of Frame (SOF)
// packets to your device.  In response to this, all USB devices are supposed to decrease their power
// consumption from the USB Vbus to <2.5mA each.  The USB module detects this condition (which according
// to the USB specifications is 3+ms of no bus activity/SOF packets) and then calls the USBCBSuspend()
// function.  You should modify these callback functions to take appropriate actions for each of these
// conditions.  For example, in the USBCBSuspend(), you may wish to add code that will decrease power
// consumption from Vbus to <2.5mA (such as by clock switching, turning off LEDs, putting the
// microcontroller to sleep, etc.).  Then, in the USBCBWakeFromSuspend() function, you may then wish to
// add code that undoes the power saving things done in the USBCBSuspend() function.

// The USBCBSendResume() function is special, in that the USB stack will not automatically call this
// function.  This function is meant to be called from the application firmware instead.  See the
// additional comments near the function.

/******************************************************************************
 * Function:        void USBCBSuspend(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        Call back that is invoked when a USB suspend is detected
 *
 * Note:            None
 *****************************************************************************/
void USBCBSuspend(void)
{
	//Example power saving code.  Insert appropriate code here for the desired
	//application behavior.  If the microcontroller will be put to sleep, a
	//process similar to that shown below may be used:
	
	//ConfigureIOPinsForLowPower();
	//SaveStateOfAllInterruptEnableBits();
	//DisableAllInterruptEnableBits();
	//EnableOnlyTheInterruptsWhichWillBeUsedToWakeTheMicro();	//should enable at least USBActivityIF as a wake source
	//Sleep();
	//RestoreStateOfAllPreviouslySavedInterruptEnableBits();	//Preferrably, this should be done in the USBCBWakeFromSuspend() function instead.
	//RestoreIOPinsToNormal();									//Preferrably, this should be done in the USBCBWakeFromSuspend() function instead.

	//IMPORTANT NOTE: Do not clear the USBActivityIF (ACTVIF) bit here.  This bit is 
	//cleared inside the usb_device.c file.  Clearing USBActivityIF here will cause 
	//things to not work as intended.	
	

    #if defined(__C30__)
    #if 0
        U1EIR = 0xFFFF;
        U1IR = 0xFFFF;
        U1OTGIR = 0xFFFF;
        IFS5bits.USB1IF = 0;
        IEC5bits.USB1IE = 1;
        U1OTGIEbits.ACTVIE = 1;
        U1OTGIRbits.ACTVIF = 1;
        TRISA &= 0xFF3F;
        LATAbits.LATA6 = 1;
        Sleep();
        LATAbits.LATA6 = 0;
    #endif
    #endif
}


/******************************************************************************
 * Function:        void _USB1Interrupt(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This function is called when the USB interrupt bit is set
 *					In this example the interrupt is only used when the device
 *					goes to sleep when it receives a USB suspend command
 *
 * Note:            None
 *****************************************************************************/
#if 0
void __attribute__ ((interrupt)) _USB1Interrupt(void)
{
    #if !defined(self_powered)
        if(U1OTGIRbits.ACTVIF)
        {
            LATAbits.LATA7 = 1;
        
            IEC5bits.USB1IE = 0;
            U1OTGIEbits.ACTVIE = 0;
            IFS5bits.USB1IF = 0;
        
            //USBClearInterruptFlag(USBActivityIFReg,USBActivityIFBitNum);
            USBClearInterruptFlag(USBIdleIFReg,USBIdleIFBitNum);
            //USBSuspendControl = 0;
            LATAbits.LATA7 = 0;
        }
    #endif
}
#endif

/******************************************************************************
 * Function:        void USBCBWakeFromSuspend(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        The host may put USB peripheral devices in low power
 *					suspend mode (by "sending" 3+ms of idle).  Once in suspend
 *					mode, the host may wake the device back up by sending non-
 *					idle state signalling.
 *					
 *					This call back is invoked when a wakeup from USB suspend 
 *					is detected.
 *
 * Note:            None
 *****************************************************************************/
void USBCBWakeFromSuspend(void)
{
	// If clock switching or other power savings measures were taken when
	// executing the USBCBSuspend() function, now would be a good time to
	// switch back to normal full power run mode conditions.  The host allows
	// a few milliseconds of wakeup time, after which the device must be 
	// fully back to normal, and capable of receiving and processing USB
	// packets.  In order to do this, the USB module must receive proper
	// clocking (IE: 48MHz clock must be available to SIE for full speed USB
	// operation).
}

/********************************************************************
 * Function:        void USBCB_SOF_Handler(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        The USB host sends out a SOF packet to full-speed
 *                  devices every 1 ms. This interrupt may be useful
 *                  for isochronous pipes. End designers should
 *                  implement callback routine as necessary.
 *
 * Note:            None
 *******************************************************************/
void USBCB_SOF_Handler(void)
{
    // No need to clear UIRbits.SOFIF to 0 here.
    // Callback caller is already doing that.
}

/*******************************************************************
 * Function:        void USBCBErrorHandler(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        The purpose of this callback is mainly for
 *                  debugging during development. Check UEIR to see
 *                  which error causes the interrupt.
 *
 * Note:            None
 *******************************************************************/
void USBCBErrorHandler(void)
{
    // No need to clear UEIR to 0 here.
    // Callback caller is already doing that.

	// Typically, user firmware does not need to do anything special
	// if a USB error occurs.  For example, if the host sends an OUT
	// packet to your device, but the packet gets corrupted (ex:
	// because of a bad connection, or the user unplugs the
	// USB cable during the transmission) this will typically set
	// one or more USB error interrupt flags.  Nothing specific
	// needs to be done however, since the SIE will automatically
	// send a "NAK" packet to the host.  In response to this, the
	// host will normally retry to send the packet again, and no
	// data loss occurs.  The system will typically recover
	// automatically, without the need for application firmware
	// intervention.
	
	// Nevertheless, this callback function is provided, such as
	// for debugging purposes.
}


/*******************************************************************
 * Function:        void USBCBCheckOtherReq(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        When SETUP packets arrive from the host, some
 * 					firmware must process the request and respond
 *					appropriately to fulfill the request.  Some of
 *					the SETUP packets will be for standard
 *					USB "chapter 9" (as in, fulfilling chapter 9 of
 *					the official USB specifications) requests, while
 *					others may be specific to the USB device class
 *					that is being implemented.  For example, a HID
 *					class device needs to be able to respond to
 *					"GET REPORT" type of requests.  This
 *					is not a standard USB chapter 9 request, and 
 *					therefore not handled by usb_device.c.  Instead
 *					this request should be handled by class specific 
 *					firmware, such as that contained in usb_function_hid.c.
 *
 * Note:            None
 *******************************************************************/
void USBCBCheckOtherReq(void)
{
    USBCheckHIDRequest();
}//end


/*******************************************************************
 * Function:        void USBCBStdSetDscHandler(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        The USBCBStdSetDscHandler() callback function is
 *					called when a SETUP, bRequest: SET_DESCRIPTOR request
 *					arrives.  Typically SET_DESCRIPTOR requests are
 *					not used in most applications, and it is
 *					optional to support this type of request.
 *
 * Note:            None
 *******************************************************************/
void USBCBStdSetDscHandler(void)
{
    // Must claim session ownership if supporting this request
}//end


/*******************************************************************
 * Function:        void USBCBInitEP(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This function is called when the device becomes
 *                  initialized, which occurs after the host sends a
 * 					SET_CONFIGURATION (wValue not = 0) request.  This 
 *					callback function should initialize the endpoints 
 *					for the device's usage according to the current 
 *					configuration.
 *
 * Note:            None
 *******************************************************************/
void USBCBInitEP(void)
{
    //enable the HID endpoint
    USBEnableEndpoint(HID_EP,USB_IN_ENABLED|USB_OUT_ENABLED|USB_HANDSHAKE_ENABLED|USB_DISALLOW_SETUP);
    //Re-arm the OUT endpoint for the next packet
    USBOutHandle = HIDRxPacket(HID_EP,(BYTE*)&ReceivedDataBuffer,64);
}

/********************************************************************
 * Function:        void USBCBSendResume(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        The USB specifications allow some types of USB
 * 					peripheral devices to wake up a host PC (such
 *					as if it is in a low power suspend to RAM state).
 *					This can be a very useful feature in some
 *					USB applications, such as an Infrared remote
 *					control	receiver.  If a user presses the "power"
 *					button on a remote control, it is nice that the
 *					IR receiver can detect this signalling, and then
 *					send a USB "command" to the PC to wake up.
 *					
 *					The USBCBSendResume() "callback" function is used
 *					to send this special USB signalling which wakes 
 *					up the PC.  This function may be called by
 *					application firmware to wake up the PC.  This
 *					function should only be called when:
 *					
 *					1.  The USB driver used on the host PC supports
 *						the remote wakeup capability.
 *					2.  The USB configuration descriptor indicates
 *						the device is remote wakeup capable in the
 *						bmAttributes field.
 *					3.  The USB host PC is currently sleeping,
 *						and has previously sent your device a SET 
 *						FEATURE setup packet which "armed" the
 *						remote wakeup capability.   
 *
 *					This callback should send a RESUME signal that
 *                  has the period of 1-15ms.
 *
 * Note:            Interrupt vs. Polling
 *                  -Primary clock
 *                  -Secondary clock ***** MAKE NOTES ABOUT THIS *******
 *                   > Can switch to primary first by calling USBCBWakeFromSuspend()
 
 *                  The modifiable section in this routine should be changed
 *                  to meet the application needs. Current implementation
 *                  temporary blocks other functions from executing for a
 *                  period of 1-13 ms depending on the core frequency.
 *
 *                  According to USB 2.0 specification section 7.1.7.7,
 *                  "The remote wakeup device must hold the resume signaling
 *                  for at lest 1 ms but for no more than 15 ms."
 *                  The idea here is to use a delay counter loop, using a
 *                  common value that would work over a wide range of core
 *                  frequencies.
 *                  That value selected is 1800. See table below:
 *                  ==========================================================
 *                  Core Freq(MHz)      MIP         RESUME Signal Period (ms)
 *                  ==========================================================
 *                      48              12          1.05
 *                       4              1           12.6
 *                  ==========================================================
 *                  * These timing could be incorrect when using code
 *                    optimization or extended instruction mode,
 *                    or when having other interrupts enabled.
 *                    Make sure to verify using the MPLAB SIM's Stopwatch
 *                    and verify the actual signal on an oscilloscope.
 *******************************************************************/
void USBCBSendResume(void)
{
    static WORD delay_count;
    
    USBResumeControl = 1;                // Start RESUME signaling
    
    delay_count = 1800U;                // Set RESUME line for 1-13 ms
    do
    {
        delay_count--;
    }while(delay_count);
    USBResumeControl = 0;
}

void display1(unsigned long value)
{
	unsigned long a,b,c,d,e,lv;

	if(value>99999)value=99999;	//should never do this
	
	lv=value;

	//first 3 digits here
	a=lv/10000;
	lv-=(a*10000);
	b=lv/1000;
	lv-=(b*1000);
	c=lv/100;	
	lv-=(c*100);
	d=lv/10;
	lv-=(d*10);
	e=lv;

	

	ucDisplay1[0]=a;
	ucDisplay1[1]=b;
	ucDisplay1[2]=c;
	ucDisplay1[3]=d;
	ucDisplay1[4]=e;
}

void display2(unsigned long value)
{
		unsigned long a,b,c,d,e,lv;

	if(value>99999)value=99999;	//should never do this
	
	lv=value;

	//first 3 digits here
	a=lv/10000;
	lv-=(a*10000);
	b=lv/1000;
	lv-=(b*1000);
	c=lv/100;	
	lv-=(c*100);
	d=lv/10;
	lv-=(d*10);
	e=lv;

	

	ucDisplay2[0]=a;
	ucDisplay2[1]=b;
	ucDisplay2[2]=c;
	ucDisplay2[3]=d;
	ucDisplay2[4]=e;
}

void swapDisplays(void)
{
	unsigned long displaySwapValue;

	//swap the values
	displaySwapValue = display2Value;
	display2Value = display1Value;
	display1Value = displaySwapValue;
	//send them to the displays
	display1(display1Value);
	standbyMSB = display2Value/100;
	display2Value -= (standbyMSB*100);
	standbyLSB = display2Value;
	display2Value = (standbyMSB*100)+standbyLSB;
	display2(display2Value);
}

void updateEncoders(void)
{
	old_ENC_A = ENC_A;
	old_ENC_C = ENC_C;
}

/** EOF main.c *************************************************/
#endif
