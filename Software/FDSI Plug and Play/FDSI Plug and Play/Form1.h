#pragma once

#include <Windows.h>	//Definitions for various common and not so common types like DWORD, PCHAR, HANDLE, etc.
#include <setupapi.h>	//From Platform SDK. Definitions needed for the SetupDixxx() functions, which we use to
						//find our plug and play device.
#include "FSUIPC.h"
#include "AdvForm.h"
#include "IniFile.h"

namespace FDSIPlugandPlay {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Xml;
	using namespace System::Text;

//-------------------------------------------------------BEGIN CUT AND PASTE BLOCK-----------------------------------------------------------------------------------
	using namespace System::Runtime::InteropServices;  //Need this to support "unmanaged" code.

	/*
	In order to use these unmanaged functions from within the managed .NET environment, we need
	to explicitly import the functions which we will be using from other .DLL file(s).  Simply
	including the appropriate header files is not enough. 

	Note: In order to avoid potential name conflicts in the header files (which we still use),
	I have renamed the functions by adding "UM" (unmanaged) onto the end of them.  To find 
	documentation for the functions in MSDN, search for the function name without the extra 
	"UM" attached.
	Note2: In the header files (such as setupapi.h), normally the function names are 
	remapped, depending upon if UNICODE is defined or not.  For example, two versions of the
	function SetupDiGetDeviceInterfaceDetail() exist.  One for UNICODE, and one for ANSI.  
	If the wrong version of the function is called, things won't work correctly.  Therefore,
	in order to make sure the correct one gets called (based on your compiler settings, which
	may or may not define "UNICODE"), it is useful to explicity specify the CharSet when doing
	the DLL import.
	*/

	#ifdef UNICODE
	#define	Seeifdef	Unicode
	#else
	#define Seeifdef	Ansi
	#endif

	//Returns a HDEVINFO type for a device information set (USB HID devices in
	//our case).  We will need the HDEVINFO as in input parameter for calling many of
	//the other SetupDixxx() functions.
	[DllImport("setupapi.dll" , CharSet = CharSet::Seeifdef, EntryPoint="SetupDiGetClassDevs")]		
	extern "C" HDEVINFO  SetupDiGetClassDevsUM(
		LPGUID  ClassGuid,					//Input: Supply the class GUID here. 
		PCTSTR  Enumerator,					//Input: Use NULL here, not important for our purposes
		HWND  hwndParent,					//Input: Use NULL here, not important for our purposes
		DWORD  Flags);						//Input: Flags describing what kind of filtering to use.

	//Gives us "PSP_DEVICE_INTERFACE_DATA" which contains the Interface specific GUID (different
	//from class GUID).  We need the interface GUID to get the device path.
	[DllImport("setupapi.dll" , CharSet = CharSet::Seeifdef, EntryPoint="SetupDiEnumDeviceInterfaces")]				
	extern "C" WINSETUPAPI BOOL WINAPI  SetupDiEnumDeviceInterfacesUM(
		HDEVINFO  DeviceInfoSet,			//Input: Give it the HDEVINFO we got from SetupDiGetClassDevs()
		PSP_DEVINFO_DATA  DeviceInfoData,	//Input (optional)
		LPGUID  InterfaceClassGuid,			//Input 
		DWORD  MemberIndex,					//Input: "Index" of the device you are interested in getting the path for.
		PSP_DEVICE_INTERFACE_DATA  DeviceInterfaceData);//Output: This function fills in an "SP_DEVICE_INTERFACE_DATA" structure.

	//SetupDiDestroyDeviceInfoList() frees up memory by destroying a DeviceInfoList
	[DllImport("setupapi.dll" , CharSet = CharSet::Seeifdef, EntryPoint="SetupDiDestroyDeviceInfoList")]
	extern "C" WINSETUPAPI BOOL WINAPI  SetupDiDestroyDeviceInfoListUM(			
		HDEVINFO  DeviceInfoSet);			//Input: Give it a handle to a device info list to deallocate from RAM.

	//SetupDiEnumDeviceInfo() fills in an "SP_DEVINFO_DATA" structure, which we need for SetupDiGetDeviceRegistryProperty()
	[DllImport("setupapi.dll" , CharSet = CharSet::Seeifdef, EntryPoint="SetupDiEnumDeviceInfo")]
	extern "C" WINSETUPAPI BOOL WINAPI  SetupDiEnumDeviceInfoUM(
		HDEVINFO  DeviceInfoSet,
		DWORD  MemberIndex,
		PSP_DEVINFO_DATA  DeviceInfoData);

	//SetupDiGetDeviceRegistryProperty() gives us the hardware ID, which we use to check to see if it has matching VID/PID
	[DllImport("setupapi.dll" , CharSet = CharSet::Seeifdef, EntryPoint="SetupDiGetDeviceRegistryProperty")]
	extern "C"	WINSETUPAPI BOOL WINAPI  SetupDiGetDeviceRegistryPropertyUM(
		HDEVINFO  DeviceInfoSet,
		PSP_DEVINFO_DATA  DeviceInfoData,
		DWORD  Property,
		PDWORD  PropertyRegDataType,
		PBYTE  PropertyBuffer,   
		DWORD  PropertyBufferSize,  
		PDWORD  RequiredSize);

	//SetupDiGetDeviceInterfaceDetail() gives us a device path, which is needed before CreateFile() can be used.
	[DllImport("setupapi.dll" , CharSet = CharSet::Seeifdef, EntryPoint="SetupDiGetDeviceInterfaceDetail")]
	extern "C" BOOL SetupDiGetDeviceInterfaceDetailUM(
		HDEVINFO DeviceInfoSet,										//Input: Wants HDEVINFO which can be obtained from SetupDiGetClassDevs()
		PSP_DEVICE_INTERFACE_DATA DeviceInterfaceData,				//Input: Pointer to an structure which defines the device interface.  
		PSP_DEVICE_INTERFACE_DETAIL_DATA DeviceInterfaceDetailData,	//Output: Pointer to a strucutre, which will contain the device path.
		DWORD DeviceInterfaceDetailDataSize,						//Input: Number of bytes to retrieve.
		PDWORD RequiredSize,										//Output (optional): Te number of bytes needed to hold the entire struct 
		PSP_DEVINFO_DATA DeviceInfoData);							//Output



//-------------------------------------------------------END CUT AND PASTE BLOCK-------------------------------------------------------------------------------------


	/// <summary>
	/// Summary for Form1
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class Form1 : public System::Windows::Forms::Form
	{
	public:
		Form1(void)
		{
			InitializeComponent();
			//Init combo boxes
			comboBox1->SelectedIndex = 0;
			comboBox2->SelectedIndex = 0;
			comboBox3->SelectedIndex = 0;
			comboBox4->SelectedIndex = 0;
			comboBox5->SelectedIndex = 0;
			comboBox6->SelectedIndex = 0;

			//load or create the INI file
			string Filename = "Config.ini";

			if ( File::Exists( "Config.ini" ) )
			{
				//load the data  - v1.0.0
				//V1.1.0 uses a single integer for ComboBox Value
				//Offsets for toggle switches
				string v1 = CIniFile::GetValue("ulCOMTEST1Offset","COM",Filename);
				ulCOMTEST1Offset = AdvancedOffsetArray[strtol(v1.c_str(), NULL, 0)];
				v1 = CIniFile::GetValue("ulCOMTEST2Offset","COM",Filename);
				ulCOMTEST2Offset = AdvancedOffsetArray[strtol(v1.c_str(), NULL, 0)];
				v1 = CIniFile::GetValue("ulNAVTEST1Offset","NAV",Filename);
				ulNAVTEST1Offset = AdvancedOffsetArray[strtol(v1.c_str(), NULL, 0)];
				v1 = CIniFile::GetValue("ulNAVTEST2Offset","NAV",Filename);
				ulNAVTEST2Offset = AdvancedOffsetArray[strtol(v1.c_str(), NULL, 0)];
				//Offset for COM TEST
				v1 = CIniFile::GetValue("ulADF1ANTOffset","ADF",Filename);
				ulADF1ANTOffset = AdvancedOffsetArray[strtol(v1.c_str(), NULL, 0)];
				v1 = CIniFile::GetValue("ulADF2ANTOffset","ADF",Filename);
				ulADF2ANTOffset = AdvancedOffsetArray[strtol(v1.c_str(), NULL, 0)];
				v1 = CIniFile::GetValue("ulADF1TONEOffset","ADF",Filename);
				ulADF1TONEOffset = AdvancedOffsetArray[strtol(v1.c_str(), NULL, 0)];
				v1 = CIniFile::GetValue("ulADF2TONEOffset","ADF",Filename);
				ulADF2TONEOffset = AdvancedOffsetArray[strtol(v1.c_str(), NULL, 0)];
				//Values for 'On' and 'Off' for ANT switch
				v1 = CIniFile::GetValue("ulADF1Value1","ADF",Filename);
				ulADF1Value1 = strtol(v1.c_str(), NULL, 0);
				v1 = CIniFile::GetValue("ulADF1Value2","ADF",Filename);
				ulADF1Value2 = strtol(v1.c_str(), NULL, 0);
				v1 = CIniFile::GetValue("ulADF2Value1","ADF",Filename);
				ulADF2Value1 = strtol(v1.c_str(), NULL, 0);
				v1 = CIniFile::GetValue("ulADF2Value2","ADF",Filename);
				ulADF2Value2 = strtol(v1.c_str(), NULL, 0);
				//Values for 'On' and 'Off' for TONE switch
				v1 = CIniFile::GetValue("ulTONE1Value1","ADF",Filename);
				ulTONE1Value1 = strtol(v1.c_str(), NULL, 0);
				v1 = CIniFile::GetValue("ulTONE1Value2","ADF",Filename);
				ulTONE1Value2 = strtol(v1.c_str(), NULL, 0);
				v1 = CIniFile::GetValue("ulTONE2Value1","ADF",Filename);
				ulTONE2Value1 = strtol(v1.c_str(), NULL, 0);
				v1 = CIniFile::GetValue("ulTONE2Value2","ADF",Filename);
				ulTONE2Value2 = strtol(v1.c_str(), NULL, 0);
				//values for COM TEST Switch
				v1 = CIniFile::GetValue("ulCOMTEST1Value1","COM",Filename);
				ulCOMTEST1Value1 = strtol(v1.c_str(), NULL, 0);
				v1 = CIniFile::GetValue("ulCOMTEST1Value2","COM",Filename);
				ulCOMTEST1Value2 = strtol(v1.c_str(), NULL, 0);
				v1 = CIniFile::GetValue("ulCOMTEST2Value1","COM",Filename);
				ulCOMTEST2Value1 = strtol(v1.c_str(), NULL, 0);
				v1 = CIniFile::GetValue("ulCOMTEST2Value2","COM",Filename);
				ulCOMTEST2Value2 = strtol(v1.c_str(), NULL, 0);
				//values for NAV TEST Switch
				v1 = CIniFile::GetValue("ulNAVTEST1Value1","NAV",Filename);
				ulNAVTEST1Value1 = strtol(v1.c_str(), NULL, 0);
				v1 = CIniFile::GetValue("ulNAVTEST1Value2","NAV",Filename);
				ulNAVTEST1Value2 = strtol(v1.c_str(), NULL, 0);
				v1 = CIniFile::GetValue("ulNAVTEST2Value1","NAV",Filename);
				ulNAVTEST2Value1 = strtol(v1.c_str(), NULL, 0);
				v1 = CIniFile::GetValue("ulNAVTEST2Value2","NAV",Filename);
				ulNAVTEST2Value2 = strtol(v1.c_str(), NULL, 0);
			}
			else
			{
				

				CIniFile::Create(Filename);
				//create the sections
				CIniFile::AddSection("COM",Filename);
				CIniFile::AddSection("NAV",Filename);
				CIniFile::AddSection("ADF",Filename);
				//create the keys
				CIniFile::SetValue("ulADF1ANTOffset","0","ADF",Filename);
				CIniFile::SetValue("ulADF2ANTOffset","0","ADF",Filename);
				CIniFile::SetValue("ulADF1TONEOffset","0","ADF",Filename);
				CIniFile::SetValue("ulADF2TONEOffset","0","ADF",Filename);
				CIniFile::SetValue("ulCOMTEST1Offset","0","COM",Filename);
				CIniFile::SetValue("ulCOMTEST2Offset","0","COM",Filename);
				CIniFile::SetValue("ulNAVTEST1Offset","0","NAV",Filename);
				CIniFile::SetValue("ulNAVTEST2Offset","0","NAV",Filename);
				CIniFile::SetValue("ulADF1Value1","0x0000","ADF",Filename);
				CIniFile::SetValue("ulADF1Value2","0x0000","ADF",Filename);
				CIniFile::SetValue("ulADF2Value1","0x0000","ADF",Filename);
				CIniFile::SetValue("ulADF2Value2","0x0000","ADF",Filename);
				CIniFile::SetValue("ulTONE1Value1","0x0000","ADF",Filename);
				CIniFile::SetValue("ulTONE1Value2","0x0000","ADF",Filename);
				CIniFile::SetValue("ulTONE2Value1","0x0000","ADF",Filename);
				CIniFile::SetValue("ulTONE2Value2","0x0000","ADF",Filename);
				CIniFile::SetValue("ulCOMTEST1Value1","0x0000","COM",Filename);
				CIniFile::SetValue("ulCOMTEST1Value2","0x0000","COM",Filename);
				CIniFile::SetValue("ulCOMTEST2Value1","0x0000","COM",Filename);
				CIniFile::SetValue("ulCOMTEST2Value2","0x0000","COM",Filename);
				CIniFile::SetValue("ulNAVTEST1Value1","0x0000","NAV",Filename);
				CIniFile::SetValue("ulNAVTEST1Value2","0x0000","NAV",Filename);
				CIniFile::SetValue("ulNAVTEST2Value1","0x0000","NAV",Filename);
				CIniFile::SetValue("ulNAVTEST2Value2","0x0000","NAV",Filename);
			}
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~Form1()
		{
			if (components)
			{
				FSUIPC_Close(); // Closing when it wasn't open is okay, so this is safe here
				delete components;
			}
		}
	private: System::Windows::Forms::PictureBox^  pictureBox1;
	private: System::Windows::Forms::Timer^  EventTimer;

	private: System::Windows::Forms::Button^  Refresh;
	public: System::Windows::Forms::TextBox^  DeviceTextBox1;







	private: System::Windows::Forms::ComboBox^  comboBox1;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::ComboBox^  comboBox2;
	private: System::Windows::Forms::ComboBox^  comboBox3;
	private: System::Windows::Forms::ComboBox^  comboBox4;
	private: System::Windows::Forms::ComboBox^  comboBox5;
	private: System::Windows::Forms::ComboBox^  comboBox6;
	private: System::Windows::Forms::TextBox^  DeviceTextBox2;
	private: System::Windows::Forms::TextBox^  DeviceTextBox3;
	private: System::Windows::Forms::TextBox^  DeviceTextBox4;
	private: System::Windows::Forms::TextBox^  DeviceTextBox5;
	private: System::Windows::Forms::TextBox^  DeviceTextBox6;


	private: System::Windows::Forms::Timer^  MainLoopTimer;
	private: System::Windows::Forms::Button^  AdvButton;



	protected: 

	private: System::ComponentModel::IContainer^  components;

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(Form1::typeid));
			this->pictureBox1 = (gcnew System::Windows::Forms::PictureBox());
			this->EventTimer = (gcnew System::Windows::Forms::Timer(this->components));
			this->Refresh = (gcnew System::Windows::Forms::Button());
			this->DeviceTextBox1 = (gcnew System::Windows::Forms::TextBox());
			this->comboBox1 = (gcnew System::Windows::Forms::ComboBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->comboBox2 = (gcnew System::Windows::Forms::ComboBox());
			this->comboBox3 = (gcnew System::Windows::Forms::ComboBox());
			this->comboBox4 = (gcnew System::Windows::Forms::ComboBox());
			this->comboBox5 = (gcnew System::Windows::Forms::ComboBox());
			this->comboBox6 = (gcnew System::Windows::Forms::ComboBox());
			this->DeviceTextBox2 = (gcnew System::Windows::Forms::TextBox());
			this->DeviceTextBox3 = (gcnew System::Windows::Forms::TextBox());
			this->DeviceTextBox4 = (gcnew System::Windows::Forms::TextBox());
			this->DeviceTextBox5 = (gcnew System::Windows::Forms::TextBox());
			this->DeviceTextBox6 = (gcnew System::Windows::Forms::TextBox());
			this->MainLoopTimer = (gcnew System::Windows::Forms::Timer(this->components));
			this->AdvButton = (gcnew System::Windows::Forms::Button());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox1))->BeginInit();
			this->SuspendLayout();
			// 
			// pictureBox1
			// 
			this->pictureBox1->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"pictureBox1.Image")));
			this->pictureBox1->Location = System::Drawing::Point(-1, 0);
			this->pictureBox1->Name = L"pictureBox1";
			this->pictureBox1->Size = System::Drawing::Size(488, 246);
			this->pictureBox1->TabIndex = 0;
			this->pictureBox1->TabStop = false;
			// 
			// EventTimer
			// 
			this->EventTimer->Enabled = true;
			this->EventTimer->Interval = 2500;
			this->EventTimer->Tick += gcnew System::EventHandler(this, &Form1::EventTimer_Tick);
			// 
			// Refresh
			// 
			this->Refresh->Location = System::Drawing::Point(15, 550);
			this->Refresh->Name = L"Refresh";
			this->Refresh->Size = System::Drawing::Size(82, 27);
			this->Refresh->TabIndex = 2;
			this->Refresh->Text = L"&Refresh";
			this->Refresh->UseVisualStyleBackColor = true;
			this->Refresh->Click += gcnew System::EventHandler(this, &Form1::Refresh_Click);
			// 
			// DeviceTextBox1
			// 
			this->DeviceTextBox1->Enabled = false;
			this->DeviceTextBox1->Location = System::Drawing::Point(36, 312);
			this->DeviceTextBox1->Name = L"DeviceTextBox1";
			this->DeviceTextBox1->Size = System::Drawing::Size(177, 20);
			this->DeviceTextBox1->TabIndex = 3;
			// 
			// comboBox1
			// 
			this->comboBox1->FormattingEnabled = true;
			this->comboBox1->Items->AddRange(gcnew cli::array< System::Object^  >(2) {L"FSDI COM1", L"FDSI COM2"});
			this->comboBox1->Location = System::Drawing::Point(254, 311);
			this->comboBox1->Name = L"comboBox1";
			this->comboBox1->Size = System::Drawing::Size(179, 21);
			this->comboBox1->TabIndex = 9;
			this->comboBox1->SelectedIndexChanged += gcnew System::EventHandler(this, &Form1::comboBox1_SelectedIndexChanged);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Font = (gcnew System::Drawing::Font(L"Arial", 12, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label1->Location = System::Drawing::Point(45, 280);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(154, 19);
			this->label1->TabIndex = 15;
			this->label1->Text = L"Detected Devices...";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Font = (gcnew System::Drawing::Font(L"Arial", 12, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label2->Location = System::Drawing::Point(295, 280);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(98, 19);
			this->label2->TabIndex = 16;
			this->label2->Text = L"Assign To...";
			// 
			// comboBox2
			// 
			this->comboBox2->FormattingEnabled = true;
			this->comboBox2->Items->AddRange(gcnew cli::array< System::Object^  >(2) {L"FSDI COM1", L"FDSI COM2"});
			this->comboBox2->Location = System::Drawing::Point(254, 336);
			this->comboBox2->Name = L"comboBox2";
			this->comboBox2->Size = System::Drawing::Size(179, 21);
			this->comboBox2->TabIndex = 17;
			this->comboBox2->SelectedIndexChanged += gcnew System::EventHandler(this, &Form1::comboBox2_SelectedIndexChanged);
			// 
			// comboBox3
			// 
			this->comboBox3->FormattingEnabled = true;
			this->comboBox3->Items->AddRange(gcnew cli::array< System::Object^  >(2) {L"FDSI NAV1", L"FDSI NAV2"});
			this->comboBox3->Location = System::Drawing::Point(254, 363);
			this->comboBox3->Name = L"comboBox3";
			this->comboBox3->Size = System::Drawing::Size(179, 21);
			this->comboBox3->TabIndex = 18;
			this->comboBox3->SelectedIndexChanged += gcnew System::EventHandler(this, &Form1::comboBox3_SelectedIndexChanged);
			// 
			// comboBox4
			// 
			this->comboBox4->FormattingEnabled = true;
			this->comboBox4->Items->AddRange(gcnew cli::array< System::Object^  >(2) {L"FDSI NAV1", L"FDSI NAV2"});
			this->comboBox4->Location = System::Drawing::Point(254, 388);
			this->comboBox4->Name = L"comboBox4";
			this->comboBox4->Size = System::Drawing::Size(179, 21);
			this->comboBox4->TabIndex = 19;
			this->comboBox4->SelectedIndexChanged += gcnew System::EventHandler(this, &Form1::comboBox4_SelectedIndexChanged);
			// 
			// comboBox5
			// 
			this->comboBox5->FormattingEnabled = true;
			this->comboBox5->Items->AddRange(gcnew cli::array< System::Object^  >(2) {L"FDSI ADF1", L"FDSI ADF2"});
			this->comboBox5->Location = System::Drawing::Point(254, 414);
			this->comboBox5->Name = L"comboBox5";
			this->comboBox5->Size = System::Drawing::Size(179, 21);
			this->comboBox5->TabIndex = 20;
			this->comboBox5->SelectedIndexChanged += gcnew System::EventHandler(this, &Form1::comboBox5_SelectedIndexChanged);
			// 
			// comboBox6
			// 
			this->comboBox6->FormattingEnabled = true;
			this->comboBox6->Items->AddRange(gcnew cli::array< System::Object^  >(2) {L"FDSI ADF1", L"FDSI ADF2"});
			this->comboBox6->Location = System::Drawing::Point(254, 441);
			this->comboBox6->Name = L"comboBox6";
			this->comboBox6->Size = System::Drawing::Size(179, 21);
			this->comboBox6->TabIndex = 21;
			this->comboBox6->SelectedIndexChanged += gcnew System::EventHandler(this, &Form1::comboBox6_SelectedIndexChanged);
			// 
			// DeviceTextBox2
			// 
			this->DeviceTextBox2->Enabled = false;
			this->DeviceTextBox2->Location = System::Drawing::Point(36, 337);
			this->DeviceTextBox2->Name = L"DeviceTextBox2";
			this->DeviceTextBox2->Size = System::Drawing::Size(177, 20);
			this->DeviceTextBox2->TabIndex = 22;
			// 
			// DeviceTextBox3
			// 
			this->DeviceTextBox3->Enabled = false;
			this->DeviceTextBox3->Location = System::Drawing::Point(36, 364);
			this->DeviceTextBox3->Name = L"DeviceTextBox3";
			this->DeviceTextBox3->Size = System::Drawing::Size(177, 20);
			this->DeviceTextBox3->TabIndex = 23;
			// 
			// DeviceTextBox4
			// 
			this->DeviceTextBox4->Enabled = false;
			this->DeviceTextBox4->Location = System::Drawing::Point(36, 389);
			this->DeviceTextBox4->Name = L"DeviceTextBox4";
			this->DeviceTextBox4->Size = System::Drawing::Size(177, 20);
			this->DeviceTextBox4->TabIndex = 24;
			// 
			// DeviceTextBox5
			// 
			this->DeviceTextBox5->Enabled = false;
			this->DeviceTextBox5->Location = System::Drawing::Point(36, 415);
			this->DeviceTextBox5->Name = L"DeviceTextBox5";
			this->DeviceTextBox5->Size = System::Drawing::Size(177, 20);
			this->DeviceTextBox5->TabIndex = 25;
			// 
			// DeviceTextBox6
			// 
			this->DeviceTextBox6->Enabled = false;
			this->DeviceTextBox6->Location = System::Drawing::Point(36, 442);
			this->DeviceTextBox6->Name = L"DeviceTextBox6";
			this->DeviceTextBox6->Size = System::Drawing::Size(177, 20);
			this->DeviceTextBox6->TabIndex = 26;
			// 
			// MainLoopTimer
			// 
			this->MainLoopTimer->Interval = 1000;
			this->MainLoopTimer->Tick += gcnew System::EventHandler(this, &Form1::MainLoopTimer_Tick);
			// 
			// AdvButton
			// 
			this->AdvButton->Location = System::Drawing::Point(391, 550);
			this->AdvButton->Name = L"AdvButton";
			this->AdvButton->Size = System::Drawing::Size(82, 27);
			this->AdvButton->TabIndex = 27;
			this->AdvButton->Text = L"&Advanced";
			this->AdvButton->UseVisualStyleBackColor = true;
			this->AdvButton->Click += gcnew System::EventHandler(this, &Form1::AdvButton_Click);
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::SystemColors::GradientInactiveCaption;
			this->ClientSize = System::Drawing::Size(485, 586);
			this->Controls->Add(this->AdvButton);
			this->Controls->Add(this->DeviceTextBox6);
			this->Controls->Add(this->DeviceTextBox5);
			this->Controls->Add(this->DeviceTextBox4);
			this->Controls->Add(this->DeviceTextBox3);
			this->Controls->Add(this->DeviceTextBox2);
			this->Controls->Add(this->comboBox6);
			this->Controls->Add(this->comboBox5);
			this->Controls->Add(this->comboBox4);
			this->Controls->Add(this->comboBox3);
			this->Controls->Add(this->comboBox2);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->comboBox1);
			this->Controls->Add(this->DeviceTextBox1);
			this->Controls->Add(this->Refresh);
			this->Controls->Add(this->pictureBox1);
			this->Icon = (cli::safe_cast<System::Drawing::Icon^  >(resources->GetObject(L"$this.Icon")));
			this->Name = L"Form1";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
			this->Text = L"FDSI - Detecting Devices... Please Wait";
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox1))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	
	private: System::Void EventTimer_Tick(System::Object^  sender, System::EventArgs^  e) {
					
					int i;
					//Check that the FSUIPC Dll is running
					Check_FSUIPC();
					System::String^ s1 = gcnew String( chMsg );

					if(FS_Detected ==1)
					{
						//Show Sim and FSUIPC Versions in titlebar
						this->Text = L"FDSI - " + s1;
						//disable the timer
						this->EventTimer->Enabled = false;

						//now update combo boxes with module information...
						i=DetectCOMModule();
						if(i>0)
						{
							uiCOM1Exsits=1;
							this->DeviceTextBox1->Text = "FDSI COM Module 1";
						}
						if(i>1)
						{
							uiCOM2Exsits=1;
							this->DeviceTextBox2->Text = "FDSI COM Module 2";
						}
						i=DetectNAVModule();
						if(i>0)
						{
							uiNAV1Exsits=1;
							this->DeviceTextBox3->Text = "FDSI NAV Module 1";
						}
						if(i==2)
						{
							uiNAV2Exsits=1;
							this->DeviceTextBox4->Text = "FDSI NAV Module 2";
						}
						i=DetectADFModule();
						if(i>0)
						{
							uiADF1Exsits=1;
							this->DeviceTextBox5->Text = "FDSI ADF Module 1";
						}
						if(i==2)
						{
							uiADF2Exsits=1;
							this->DeviceTextBox6->Text = "FDSI ADF Module 2";
						}

						this->MainLoopTimer->Enabled=true;

					}
					else
					{
						//No Devices
						this->EventTimer->Enabled = false;
						this->Text = L"FDSI - No Simulator Detected!";
						//leave timer enable to allow auto-detection
						//incase a Simulator Application is not yet started.
						if(MessageBox::Show("Test1.\n\rTest2.", "Test3", MessageBoxButtons::OK, MessageBoxIcon::Asterisk)==OK)

						//now update combo boxes with module information...
						i=DetectCOMModule();
						if(i>0)
						{
							uiCOM1Exsits=1;
							this->DeviceTextBox1->Text = "FDSI COM Module 1";
						}
						if(i>1)
						{
							uiCOM2Exsits=1;
							this->DeviceTextBox2->Text = "FDSI COM Module 2";
						}
						i=DetectNAVModule();
						if(i>0)
						{
							uiNAV1Exsits=1;
							this->DeviceTextBox3->Text = "FDSI NAV Module 1";
						}
						if(i==2)
						{
							uiNAV2Exsits=1;
							this->DeviceTextBox4->Text = "FDSI NAV Module 2";
						}
						i=DetectADFModule();
						if(i>0)
						{
							uiADF1Exsits=1;
							this->DeviceTextBox5->Text = "FDSI ADF Module 1";
						}
						if(i==2)
						{
							uiADF2Exsits=1;
							this->DeviceTextBox6->Text = "FDSI ADF Module 2";
						}

					}
			 }


private: System::Void Refresh_Click(System::Object^  sender, System::EventArgs^  e) {

			 FSUIPC_Close(); // Closing when it wasn't open is okay, so this is safe here
			 //When the Refresh button has been clicked, restart the FSUIPC and module detection
			 //by enabling the timer again.
			 FS_Detected=0;
			 //Clear textBoxes here...
			 this->DeviceTextBox1->Clear();
			 this->DeviceTextBox2->Clear();
			 this->DeviceTextBox3->Clear();
			 this->DeviceTextBox4->Clear();
			 this->DeviceTextBox5->Clear();
			 this->DeviceTextBox6->Clear();
			 this->EventTimer->Enabled = true;
		 }

private: System::Void Test_Click(System::Object^  sender, System::EventArgs^  e) {
			
		 }


private: System::Void MainLoopTimer_Tick(System::Object^  sender, System::EventArgs^  e) {
			 if(uiADF1Exsits)
			 {
				 ReadADFValue(1);
				 ReadANTSwitch(1);
				 ReadTONESwitch(1);
			 }
			 if(uiADF2Exsits)
			 {
				 ReadADFValue(2);
				 ReadANTSwitch(2);
				 ReadTONESwitch(2);
			 }
			 if(uiCOM1Exsits)
			 {
				ReadCOMValue(1);
				ReadCOMSwitch(1);
			 }
			 if(uiCOM2Exsits)
			 {
				ReadCOMValue(2);
				ReadCOMSwitch(2);
			 }
			 if(uiNAV1Exsits)
			 {
					ReadNAVValue(1);
					ReadNAVSwitch(1);
			 }
			 if(uiNAV2Exsits)
			 {

					ReadNAVValue(2);
					ReadNAVSwitch(2);
			 }
		 }
private: System::Void AdvButton_Click(System::Object^  sender, System::EventArgs^  e) {

			 AdvForm^ AdvancedForm = gcnew AdvForm();
			 AdvancedForm->Activate();
			 AdvancedForm->Visible = true;
		 }
private: System::Void comboBox1_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
			 if(comboBox1->SelectedIndex == 0)
			 {
				 COM1isCOM1=1;
				 COM1isCOM2=0;
			 }
			 if(comboBox1->SelectedIndex == 1)
			 {
				 COM1isCOM1=0;
				 COM1isCOM2=1;
			 }
		 }
private: System::Void comboBox2_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
			 if(comboBox2->SelectedIndex == 0)
			 {
				 COM2isCOM1=1;
				 COM2isCOM2=0;
			 }
			 if(comboBox2->SelectedIndex == 1)
			 {
				 COM2isCOM1=0;
				 COM2isCOM2=1;
			 }
		 }
private: System::Void comboBox3_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
			 if(comboBox3->SelectedIndex == 0)
			 {
				 NAV1isNAV1=1;
				 NAV1isNAV2=0;
			 }
			 if(comboBox3->SelectedIndex == 1)
			 {
				 NAV1isNAV1=0;
				 NAV1isNAV2=1;
			 }
		 }
private: System::Void comboBox4_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
			 if(comboBox4->SelectedIndex == 0)
			 {
				 NAV2isNAV1=1;
				 NAV2isNAV2=0;
			 }
			 if(comboBox4->SelectedIndex == 1)
			 {
				 NAV2isNAV1=0;
				 NAV2isNAV2=1;
			 }
		 }
private: System::Void comboBox5_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
			 if(comboBox5->SelectedIndex == 0)
			 {
				 ADF1isADF1=1;
				 ADF1isADF2=0;
			 }
			 if(comboBox5->SelectedIndex == 1)
			 {
				 ADF1isADF1=0;
				 ADF1isADF2=1;
			 }
		 }
private: System::Void comboBox6_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
			 if(comboBox6->SelectedIndex == 0)
			 {
				 ADF2isADF1=1;
				 ADF2isADF2=0;
			 }
			 if(comboBox6->SelectedIndex == 1)
			 {
				 ADF2isADF1=0;
				 ADF2isADF2=1;
			 }
		 }
};
}