#pragma once

#include "FSUIPC.h"
#include "IniFile.h"





using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;
using namespace System::IO;
using namespace System::Text;
using namespace std;


namespace FDSIPlugandPlay {

	/// <summary>
	/// Summary for AdvForm
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class AdvForm : public System::Windows::Forms::Form
	{
	public:
		AdvForm(void)
		{
			InitializeComponent();
			//read the Config.ini file
			string Filename = "Config.ini";

			//load the data V1.1.0 - changes include combo box and fixed values
			
			string v1 = CIniFile::GetValue("ulADF1ANTOffset","ADF",Filename);
			this->cmbADFANTOffset1->SelectedIndex = strtol(v1.c_str(), NULL, 0);
			v1 = CIniFile::GetValue("ulADF2ANTOffset","ADF",Filename);
			this->cmbADFANTOffset2->SelectedIndex = strtol(v1.c_str(), NULL, 0);
			v1 = CIniFile::GetValue("ulADF1TONEOffset","ADF",Filename);
			this->cmbADFTONEOffset1->SelectedIndex = strtol(v1.c_str(), NULL, 0);
			v1 = CIniFile::GetValue("ulADF2TONEOffset","ADF",Filename);
			this->cmbADFTONEOffset2->SelectedIndex = strtol(v1.c_str(), NULL, 0);


			v1 = CIniFile::GetValue("ulCOMTEST1Offset","COM",Filename);
			this->cmbCOMOffset1->SelectedIndex = strtol(v1.c_str(), NULL, 0);
			v1 = CIniFile::GetValue("ulCOMTEST2Offset","COM",Filename);
			this->cmbCOMOffset2->SelectedIndex = strtol(v1.c_str(), NULL, 0);

			v1 = CIniFile::GetValue("ulNAVTEST1Offset","NAV",Filename);
			this->cmbNAVOffset1->SelectedIndex = strtol(v1.c_str(), NULL, 0);
			v1 = CIniFile::GetValue("ulNAVTEST2Offset","NAV",Filename);
			this->cmbNAVOffset2->SelectedIndex = strtol(v1.c_str(), NULL, 0);

			v1 = CIniFile::GetValue("ulCOMTEST1Value1","COM",Filename);
			ulCOMTEST1Value1 = strtol(v1.c_str(), NULL, 0);
			v1 = CIniFile::GetValue("ulCOMTEST1Value2","COM",Filename);
			ulCOMTEST1Value2 = strtol(v1.c_str(), NULL, 0);
			v1 = CIniFile::GetValue("ulCOMTEST2Value1","COM",Filename);
			ulCOMTEST2Value1 = strtol(v1.c_str(), NULL, 0);
			v1 = CIniFile::GetValue("ulCOMTEST2Value2","COM",Filename);
			ulCOMTEST2Value2 = strtol(v1.c_str(), NULL, 0);

			v1 = CIniFile::GetValue("ulADF1Value1","ADF",Filename);
			ulADF1Value1 = strtol(v1.c_str(), NULL, 0);
			v1 = CIniFile::GetValue("ulADF1Value2","ADF",Filename);
			ulADF1Value2 = strtol(v1.c_str(), NULL, 0);
			v1 = CIniFile::GetValue("ulADF2Value1","ADF",Filename);
			ulADF2Value1 = strtol(v1.c_str(), NULL, 0);
			v1 = CIniFile::GetValue("ulADF2Value2","ADF",Filename);
			ulADF2Value2 = strtol(v1.c_str(), NULL, 0);

			v1 = CIniFile::GetValue("ulTONE1Value1","ADF",Filename);
			ulTONE1Value1 = strtol(v1.c_str(), NULL, 0);
			v1 = CIniFile::GetValue("ulTONE1Value2","ADF",Filename);
			ulTONE1Value2 = strtol(v1.c_str(), NULL, 0);
			v1 = CIniFile::GetValue("ulTONE2Value1","ADF",Filename);
			ulTONE2Value1 = strtol(v1.c_str(), NULL, 0);
			v1 = CIniFile::GetValue("ulTONE2Value2","ADF",Filename);
			ulTONE2Value2 = strtol(v1.c_str(), NULL, 0);

			v1 = CIniFile::GetValue("ulNAVTEST1Value1","NAV",Filename);
			ulNAVTEST1Value1 = strtol(v1.c_str(), NULL, 0);
			v1 = CIniFile::GetValue("ulNAVTEST1Value2","NAV",Filename);
			ulNAVTEST1Value2 = strtol(v1.c_str(), NULL, 0);
			v1 = CIniFile::GetValue("ulNAVTEST2Value1","NAV",Filename);
			ulNAVTEST2Value1 = strtol(v1.c_str(), NULL, 0);
			v1 = CIniFile::GetValue("ulNAVTEST2Value2","NAV",Filename);
			ulNAVTEST2Value2 = strtol(v1.c_str(), NULL, 0);
		}

	public:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~AdvForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::TabControl^  tabControl1;
	protected: 
	private: System::Windows::Forms::TabPage^  tabPage1;
	private: System::Windows::Forms::TabPage^  tabPage2;
	private: System::Windows::Forms::TabPage^  tabPage3;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Button^  OkButton;
	private: System::Windows::Forms::Button^  CancelButton;
	private: System::Windows::Forms::GroupBox^  groupBox2;
	private: System::Windows::Forms::GroupBox^  groupBox1;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::Label^  label5;
	private: System::Windows::Forms::Label^  label7;



	private: System::Windows::Forms::Label^  label4;




















	private: System::Windows::Forms::GroupBox^  groupBox3;









	private: System::Windows::Forms::Label^  label21;
	private: System::Windows::Forms::TextBox^  textBox1;
	private: System::Windows::Forms::Label^  label12;
	private: System::Windows::Forms::TextBox^  textBox2;
	private: System::Windows::Forms::TextBox^  textBox3;
	private: System::Windows::Forms::TextBox^  textBox4;
	private: System::Windows::Forms::Label^  label13;
	private: System::Windows::Forms::Label^  label14;
	private: System::Windows::Forms::TextBox^  textBox5;
	private: System::Windows::Forms::TextBox^  textBox6;
	private: System::Windows::Forms::Label^  label15;
	private: System::Windows::Forms::Label^  label16;
private: System::Windows::Forms::GroupBox^  groupBox4;






private: System::Windows::Forms::Label^  label24;
private: System::Windows::Forms::GroupBox^  groupBox6;






private: System::Windows::Forms::Label^  label36;








private: System::Windows::Forms::TextBox^  textBox7;
private: System::Windows::Forms::Label^  label25;
private: System::Windows::Forms::TextBox^  textBox8;
private: System::Windows::Forms::Label^  label26;
private: System::Windows::Forms::TextBox^  textBox9;
private: System::Windows::Forms::Label^  label27;
private: System::Windows::Forms::Label^  label28;
public: System::Windows::Forms::ComboBox^  cmbCOMOffset1;
public: System::Windows::Forms::ComboBox^  cmbCOMOffset2;


public: System::Windows::Forms::ComboBox^  cmbNAVOffset2;



public: System::Windows::Forms::ComboBox^  cmbADFANTOffset2;
public: System::Windows::Forms::ComboBox^  cmbADFTONEOffset1;

public: System::Windows::Forms::ComboBox^  cmbADFANTOffset1;
private: System::Windows::Forms::GroupBox^  groupBox5;
public: System::Windows::Forms::ComboBox^  cmbNAVOffset1;





private: System::Windows::Forms::Label^  label32;
public: System::Windows::Forms::ComboBox^  cmbADFTONEOffset2;
private: 

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->tabControl1 = (gcnew System::Windows::Forms::TabControl());
			this->tabPage1 = (gcnew System::Windows::Forms::TabPage());
			this->groupBox4 = (gcnew System::Windows::Forms::GroupBox());
			this->cmbCOMOffset2 = (gcnew System::Windows::Forms::ComboBox());
			this->label24 = (gcnew System::Windows::Forms::Label());
			this->groupBox3 = (gcnew System::Windows::Forms::GroupBox());
			this->cmbCOMOffset1 = (gcnew System::Windows::Forms::ComboBox());
			this->label21 = (gcnew System::Windows::Forms::Label());
			this->tabPage2 = (gcnew System::Windows::Forms::TabPage());
			this->groupBox6 = (gcnew System::Windows::Forms::GroupBox());
			this->cmbNAVOffset2 = (gcnew System::Windows::Forms::ComboBox());
			this->label36 = (gcnew System::Windows::Forms::Label());
			this->groupBox5 = (gcnew System::Windows::Forms::GroupBox());
			this->cmbNAVOffset1 = (gcnew System::Windows::Forms::ComboBox());
			this->label32 = (gcnew System::Windows::Forms::Label());
			this->tabPage3 = (gcnew System::Windows::Forms::TabPage());
			this->groupBox2 = (gcnew System::Windows::Forms::GroupBox());
			this->cmbADFTONEOffset2 = (gcnew System::Windows::Forms::ComboBox());
			this->cmbADFANTOffset2 = (gcnew System::Windows::Forms::ComboBox());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->label7 = (gcnew System::Windows::Forms::Label());
			this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
			this->cmbADFTONEOffset1 = (gcnew System::Windows::Forms::ComboBox());
			this->cmbADFANTOffset1 = (gcnew System::Windows::Forms::ComboBox());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->OkButton = (gcnew System::Windows::Forms::Button());
			this->CancelButton = (gcnew System::Windows::Forms::Button());
			this->textBox1 = (gcnew System::Windows::Forms::TextBox());
			this->label12 = (gcnew System::Windows::Forms::Label());
			this->textBox2 = (gcnew System::Windows::Forms::TextBox());
			this->textBox3 = (gcnew System::Windows::Forms::TextBox());
			this->textBox4 = (gcnew System::Windows::Forms::TextBox());
			this->label13 = (gcnew System::Windows::Forms::Label());
			this->label14 = (gcnew System::Windows::Forms::Label());
			this->textBox5 = (gcnew System::Windows::Forms::TextBox());
			this->textBox6 = (gcnew System::Windows::Forms::TextBox());
			this->label15 = (gcnew System::Windows::Forms::Label());
			this->label16 = (gcnew System::Windows::Forms::Label());
			this->textBox7 = (gcnew System::Windows::Forms::TextBox());
			this->label25 = (gcnew System::Windows::Forms::Label());
			this->textBox8 = (gcnew System::Windows::Forms::TextBox());
			this->label26 = (gcnew System::Windows::Forms::Label());
			this->textBox9 = (gcnew System::Windows::Forms::TextBox());
			this->label27 = (gcnew System::Windows::Forms::Label());
			this->label28 = (gcnew System::Windows::Forms::Label());
			this->tabControl1->SuspendLayout();
			this->tabPage1->SuspendLayout();
			this->groupBox4->SuspendLayout();
			this->groupBox3->SuspendLayout();
			this->tabPage2->SuspendLayout();
			this->groupBox6->SuspendLayout();
			this->groupBox5->SuspendLayout();
			this->tabPage3->SuspendLayout();
			this->groupBox2->SuspendLayout();
			this->groupBox1->SuspendLayout();
			this->SuspendLayout();
			// 
			// tabControl1
			// 
			this->tabControl1->Controls->Add(this->tabPage1);
			this->tabControl1->Controls->Add(this->tabPage2);
			this->tabControl1->Controls->Add(this->tabPage3);
			this->tabControl1->Location = System::Drawing::Point(6, 27);
			this->tabControl1->Name = L"tabControl1";
			this->tabControl1->SelectedIndex = 0;
			this->tabControl1->Size = System::Drawing::Size(521, 367);
			this->tabControl1->TabIndex = 0;
			// 
			// tabPage1
			// 
			this->tabPage1->Controls->Add(this->groupBox4);
			this->tabPage1->Controls->Add(this->groupBox3);
			this->tabPage1->Location = System::Drawing::Point(4, 22);
			this->tabPage1->Name = L"tabPage1";
			this->tabPage1->Padding = System::Windows::Forms::Padding(3);
			this->tabPage1->Size = System::Drawing::Size(513, 341);
			this->tabPage1->TabIndex = 0;
			this->tabPage1->Text = L"COM";
			this->tabPage1->UseVisualStyleBackColor = true;
			// 
			// groupBox4
			// 
			this->groupBox4->Controls->Add(this->cmbCOMOffset2);
			this->groupBox4->Controls->Add(this->label24);
			this->groupBox4->Location = System::Drawing::Point(6, 173);
			this->groupBox4->Name = L"groupBox4";
			this->groupBox4->Size = System::Drawing::Size(482, 148);
			this->groupBox4->TabIndex = 15;
			this->groupBox4->TabStop = false;
			this->groupBox4->Text = L"COM2 Module";
			// 
			// cmbCOMOffset2
			// 
			this->cmbCOMOffset2->FormattingEnabled = true;
			this->cmbCOMOffset2->Items->AddRange(gcnew cli::array< System::Object^  >(11) {L"DME 1/2 Select", L"GPS/NAV switch [FS2000+]", 
				L"COM1 Transmit [FS2000+]", L"COM2 Transmit [FS2000+]", L"COM receive both [FS2000+]", L"NAV1 sound [FS2000+]", L"NAV2 sound [FS2000+]", 
				L"Marker sound [FS2000+]", L"DME sound [FS2000+]", L"ADF sound [FS2000+]", L"Display \"No Err\""});
			this->cmbCOMOffset2->Location = System::Drawing::Point(131, 49);
			this->cmbCOMOffset2->Name = L"cmbCOMOffset2";
			this->cmbCOMOffset2->Size = System::Drawing::Size(281, 21);
			this->cmbCOMOffset2->TabIndex = 16;
			this->cmbCOMOffset2->SelectedIndexChanged += gcnew System::EventHandler(this, &AdvForm::cmbCOMOffset2_SelectedIndexChanged);
			// 
			// label24
			// 
			this->label24->AutoSize = true;
			this->label24->Location = System::Drawing::Point(20, 52);
			this->label24->Name = L"label24";
			this->label24->Size = System::Drawing::Size(74, 13);
			this->label24->TabIndex = 0;
			this->label24->Text = L"COMM TEST:";
			// 
			// groupBox3
			// 
			this->groupBox3->Controls->Add(this->cmbCOMOffset1);
			this->groupBox3->Controls->Add(this->label21);
			this->groupBox3->Location = System::Drawing::Point(6, 19);
			this->groupBox3->Name = L"groupBox3";
			this->groupBox3->Size = System::Drawing::Size(482, 148);
			this->groupBox3->TabIndex = 1;
			this->groupBox3->TabStop = false;
			this->groupBox3->Text = L"COM1 Module";
			// 
			// cmbCOMOffset1
			// 
			this->cmbCOMOffset1->FormattingEnabled = true;
			this->cmbCOMOffset1->Items->AddRange(gcnew cli::array< System::Object^  >(11) {L"DME 1/2 Select", L"GPS/NAV switch [FS2000+]", 
				L"COM1 Transmit [FS2000+]", L"COM2 Transmit [FS2000+]", L"COM receive both [FS2000+]", L"NAV1 sound [FS2000+]", L"NAV2 sound [FS2000+]", 
				L"Marker sound [FS2000+]", L"DME sound [FS2000+]", L"ADF sound [FS2000+]", L"Display \"No Err\""});
			this->cmbCOMOffset1->Location = System::Drawing::Point(131, 49);
			this->cmbCOMOffset1->Name = L"cmbCOMOffset1";
			this->cmbCOMOffset1->Size = System::Drawing::Size(281, 21);
			this->cmbCOMOffset1->TabIndex = 15;
			this->cmbCOMOffset1->SelectedIndexChanged += gcnew System::EventHandler(this, &AdvForm::cmbCOMOffset1_SelectedIndexChanged);
			// 
			// label21
			// 
			this->label21->AutoSize = true;
			this->label21->Location = System::Drawing::Point(20, 52);
			this->label21->Name = L"label21";
			this->label21->Size = System::Drawing::Size(74, 13);
			this->label21->TabIndex = 0;
			this->label21->Text = L"COMM TEST:";
			// 
			// tabPage2
			// 
			this->tabPage2->Controls->Add(this->groupBox6);
			this->tabPage2->Controls->Add(this->groupBox5);
			this->tabPage2->Location = System::Drawing::Point(4, 22);
			this->tabPage2->Name = L"tabPage2";
			this->tabPage2->Padding = System::Windows::Forms::Padding(3);
			this->tabPage2->Size = System::Drawing::Size(513, 341);
			this->tabPage2->TabIndex = 1;
			this->tabPage2->Text = L"NAV";
			this->tabPage2->UseVisualStyleBackColor = true;
			// 
			// groupBox6
			// 
			this->groupBox6->Controls->Add(this->cmbNAVOffset2);
			this->groupBox6->Controls->Add(this->label36);
			this->groupBox6->Location = System::Drawing::Point(6, 174);
			this->groupBox6->Name = L"groupBox6";
			this->groupBox6->Size = System::Drawing::Size(482, 148);
			this->groupBox6->TabIndex = 5;
			this->groupBox6->TabStop = false;
			this->groupBox6->Text = L"NAV2 Module";
			// 
			// cmbNAVOffset2
			// 
			this->cmbNAVOffset2->FormattingEnabled = true;
			this->cmbNAVOffset2->Items->AddRange(gcnew cli::array< System::Object^  >(11) {L"DME 1/2 Select", L"GPS/NAV switch [FS2000+]", 
				L"COM1 Transmit [FS2000+]", L"COM2 Transmit [FS2000+]", L"COM receive both [FS2000+]", L"NAV1 sound [FS2000+]", L"NAV2 sound [FS2000+]", 
				L"Marker sound [FS2000+]", L"DME sound [FS2000+]", L"ADF sound [FS2000+]", L"Display \"No Err\""});
			this->cmbNAVOffset2->Location = System::Drawing::Point(130, 49);
			this->cmbNAVOffset2->Name = L"cmbNAVOffset2";
			this->cmbNAVOffset2->Size = System::Drawing::Size(281, 21);
			this->cmbNAVOffset2->TabIndex = 17;
			this->cmbNAVOffset2->SelectedIndexChanged += gcnew System::EventHandler(this, &AdvForm::cmbNAVOffset2_SelectedIndexChanged);
			// 
			// label36
			// 
			this->label36->AutoSize = true;
			this->label36->Location = System::Drawing::Point(20, 52);
			this->label36->Name = L"label36";
			this->label36->Size = System::Drawing::Size(63, 13);
			this->label36->TabIndex = 0;
			this->label36->Text = L"NAV TEST:";
			// 
			// groupBox5
			// 
			this->groupBox5->Controls->Add(this->cmbNAVOffset1);
			this->groupBox5->Controls->Add(this->label32);
			this->groupBox5->Location = System::Drawing::Point(6, 20);
			this->groupBox5->Name = L"groupBox5";
			this->groupBox5->Size = System::Drawing::Size(482, 148);
			this->groupBox5->TabIndex = 4;
			this->groupBox5->TabStop = false;
			this->groupBox5->Text = L"NAV1 Module";
			// 
			// cmbNAVOffset1
			// 
			this->cmbNAVOffset1->FormattingEnabled = true;
			this->cmbNAVOffset1->Items->AddRange(gcnew cli::array< System::Object^  >(11) {L"DME 1/2 Select", L"GPS/NAV switch [FS2000+]", 
				L"COM1 Transmit [FS2000+]", L"COM2 Transmit [FS2000+]", L"COM receive both [FS2000+]", L"NAV1 sound [FS2000+]", L"NAV2 sound [FS2000+]", 
				L"Marker sound [FS2000+]", L"DME sound [FS2000+]", L"ADF sound [FS2000+]", L"Display \"No Err\""});
			this->cmbNAVOffset1->Location = System::Drawing::Point(130, 49);
			this->cmbNAVOffset1->Name = L"cmbNAVOffset1";
			this->cmbNAVOffset1->Size = System::Drawing::Size(281, 21);
			this->cmbNAVOffset1->TabIndex = 16;
			this->cmbNAVOffset1->SelectedIndexChanged += gcnew System::EventHandler(this, &AdvForm::cmbNAVOffset1_SelectedIndexChanged);
			// 
			// label32
			// 
			this->label32->AutoSize = true;
			this->label32->Location = System::Drawing::Point(20, 52);
			this->label32->Name = L"label32";
			this->label32->Size = System::Drawing::Size(63, 13);
			this->label32->TabIndex = 0;
			this->label32->Text = L"NAV TEST:";
			// 
			// tabPage3
			// 
			this->tabPage3->Controls->Add(this->groupBox2);
			this->tabPage3->Controls->Add(this->groupBox1);
			this->tabPage3->Location = System::Drawing::Point(4, 22);
			this->tabPage3->Name = L"tabPage3";
			this->tabPage3->Size = System::Drawing::Size(513, 341);
			this->tabPage3->TabIndex = 2;
			this->tabPage3->Text = L"ADF";
			this->tabPage3->UseVisualStyleBackColor = true;
			// 
			// groupBox2
			// 
			this->groupBox2->Controls->Add(this->cmbADFTONEOffset2);
			this->groupBox2->Controls->Add(this->cmbADFANTOffset2);
			this->groupBox2->Controls->Add(this->label5);
			this->groupBox2->Controls->Add(this->label7);
			this->groupBox2->Location = System::Drawing::Point(14, 166);
			this->groupBox2->Name = L"groupBox2";
			this->groupBox2->Size = System::Drawing::Size(482, 155);
			this->groupBox2->TabIndex = 1;
			this->groupBox2->TabStop = false;
			this->groupBox2->Text = L"ADF2 Module";
			// 
			// cmbADFTONEOffset2
			// 
			this->cmbADFTONEOffset2->FormattingEnabled = true;
			this->cmbADFTONEOffset2->Items->AddRange(gcnew cli::array< System::Object^  >(10) {L"DME 1/2 Select", L"GPS/NAV switch [FS2000+]", 
				L"COM1 Transmit [FS2000+]", L"COM2 Transmit [FS2000+]", L"COM receive both [FS2000+]", L"NAV1 sound [FS2000+]", L"NAV2 sound [FS2000+]", 
				L"Marker sound [FS2000+]", L"DME sound [FS2000+]", L"ADF sound [FS2000+]"});
			this->cmbADFTONEOffset2->Location = System::Drawing::Point(130, 73);
			this->cmbADFTONEOffset2->Name = L"cmbADFTONEOffset2";
			this->cmbADFTONEOffset2->Size = System::Drawing::Size(281, 21);
			this->cmbADFTONEOffset2->TabIndex = 20;
			this->cmbADFTONEOffset2->SelectedIndexChanged += gcnew System::EventHandler(this, &AdvForm::cmbADFTONEOffset2_SelectedIndexChanged);
			// 
			// cmbADFANTOffset2
			// 
			this->cmbADFANTOffset2->FormattingEnabled = true;
			this->cmbADFANTOffset2->Items->AddRange(gcnew cli::array< System::Object^  >(10) {L"DME 1/2 Select", L"GPS/NAV switch [FS2000+]", 
				L"COM1 Transmit [FS2000+]", L"COM2 Transmit [FS2000+]", L"COM receive both [FS2000+]", L"NAV1 sound [FS2000+]", L"NAV2 sound [FS2000+]", 
				L"Marker sound [FS2000+]", L"DME sound [FS2000+]", L"ADF sound [FS2000+]"});
			this->cmbADFANTOffset2->Location = System::Drawing::Point(130, 49);
			this->cmbADFANTOffset2->Name = L"cmbADFANTOffset2";
			this->cmbADFANTOffset2->Size = System::Drawing::Size(281, 21);
			this->cmbADFANTOffset2->TabIndex = 19;
			this->cmbADFANTOffset2->SelectedIndexChanged += gcnew System::EventHandler(this, &AdvForm::cmbADFANTOffset2_SelectedIndexChanged);
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Location = System::Drawing::Point(45, 76);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(70, 13);
			this->label5->TabIndex = 9;
			this->label5->Text = L"Tone Switch:";
			this->label5->TextAlign = System::Drawing::ContentAlignment::TopRight;
			// 
			// label7
			// 
			this->label7->AutoSize = true;
			this->label7->Location = System::Drawing::Point(22, 52);
			this->label7->Name = L"label7";
			this->label7->Size = System::Drawing::Size(93, 13);
			this->label7->TabIndex = 5;
			this->label7->Text = L"ADF/ANT Switch:";
			// 
			// groupBox1
			// 
			this->groupBox1->Controls->Add(this->cmbADFTONEOffset1);
			this->groupBox1->Controls->Add(this->cmbADFANTOffset1);
			this->groupBox1->Controls->Add(this->label4);
			this->groupBox1->Controls->Add(this->label2);
			this->groupBox1->Location = System::Drawing::Point(14, 12);
			this->groupBox1->Name = L"groupBox1";
			this->groupBox1->Size = System::Drawing::Size(482, 148);
			this->groupBox1->TabIndex = 0;
			this->groupBox1->TabStop = false;
			this->groupBox1->Text = L"ADF1 Module";
			// 
			// cmbADFTONEOffset1
			// 
			this->cmbADFTONEOffset1->FormattingEnabled = true;
			this->cmbADFTONEOffset1->Items->AddRange(gcnew cli::array< System::Object^  >(10) {L"DME 1/2 Select", L"GPS/NAV switch [FS2000+]", 
				L"COM1 Transmit [FS2000+]", L"COM2 Transmit [FS2000+]", L"COM receive both [FS2000+]", L"NAV1 sound [FS2000+]", L"NAV2 sound [FS2000+]", 
				L"Marker sound [FS2000+]", L"DME sound [FS2000+]", L"ADF sound [FS2000+]"});
			this->cmbADFTONEOffset1->Location = System::Drawing::Point(130, 73);
			this->cmbADFTONEOffset1->Name = L"cmbADFTONEOffset1";
			this->cmbADFTONEOffset1->Size = System::Drawing::Size(281, 21);
			this->cmbADFTONEOffset1->TabIndex = 18;
			this->cmbADFTONEOffset1->SelectedIndexChanged += gcnew System::EventHandler(this, &AdvForm::cmbADFTONEOffset1_SelectedIndexChanged);
			// 
			// cmbADFANTOffset1
			// 
			this->cmbADFANTOffset1->FormattingEnabled = true;
			this->cmbADFANTOffset1->Items->AddRange(gcnew cli::array< System::Object^  >(10) {L"DME 1/2 Select", L"GPS/NAV switch [FS2000+]", 
				L"COM1 Transmit [FS2000+]", L"COM2 Transmit [FS2000+]", L"COM receive both [FS2000+]", L"NAV1 sound [FS2000+]", L"NAV2 sound [FS2000+]", 
				L"Marker sound [FS2000+]", L"DME sound [FS2000+]", L"ADF sound [FS2000+]"});
			this->cmbADFANTOffset1->Location = System::Drawing::Point(130, 49);
			this->cmbADFANTOffset1->Name = L"cmbADFANTOffset1";
			this->cmbADFANTOffset1->Size = System::Drawing::Size(281, 21);
			this->cmbADFANTOffset1->TabIndex = 17;
			this->cmbADFANTOffset1->SelectedIndexChanged += gcnew System::EventHandler(this, &AdvForm::cmbADFANTOffset1_SelectedIndexChanged);
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(43, 76);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(70, 13);
			this->label4->TabIndex = 4;
			this->label4->Text = L"Tone Switch:";
			this->label4->TextAlign = System::Drawing::ContentAlignment::TopRight;
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(20, 52);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(93, 13);
			this->label2->TabIndex = 0;
			this->label2->Text = L"ADF/ANT Switch:";
			// 
			// label1
			// 
			this->label1->Anchor = System::Windows::Forms::AnchorStyles::Top;
			this->label1->AutoSize = true;
			this->label1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label1->Location = System::Drawing::Point(230, 9);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(135, 16);
			this->label1->TabIndex = 1;
			this->label1->Text = L"Advanced Options";
			// 
			// OkButton
			// 
			this->OkButton->Location = System::Drawing::Point(335, 409);
			this->OkButton->Name = L"OkButton";
			this->OkButton->Size = System::Drawing::Size(82, 27);
			this->OkButton->TabIndex = 2;
			this->OkButton->Text = L"&OK";
			this->OkButton->UseVisualStyleBackColor = true;
			this->OkButton->Click += gcnew System::EventHandler(this, &AdvForm::OkButton_Click);
			// 
			// CancelButton
			// 
			this->CancelButton->Location = System::Drawing::Point(440, 409);
			this->CancelButton->Name = L"CancelButton";
			this->CancelButton->Size = System::Drawing::Size(82, 27);
			this->CancelButton->TabIndex = 3;
			this->CancelButton->Text = L"&Cancel";
			this->CancelButton->UseVisualStyleBackColor = true;
			this->CancelButton->Click += gcnew System::EventHandler(this, &AdvForm::CancelButton_Click);
			// 
			// textBox1
			// 
			this->textBox1->Location = System::Drawing::Point(329, 47);
			this->textBox1->Name = L"textBox1";
			this->textBox1->Size = System::Drawing::Size(82, 20);
			this->textBox1->TabIndex = 14;
			// 
			// label12
			// 
			this->label12->AutoSize = true;
			this->label12->Location = System::Drawing::Point(351, 22);
			this->label12->Name = L"label12";
			this->label12->Size = System::Drawing::Size(43, 13);
			this->label12->TabIndex = 13;
			this->label12->Text = L"Value 2";
			// 
			// textBox2
			// 
			this->textBox2->Location = System::Drawing::Point(230, 47);
			this->textBox2->Name = L"textBox2";
			this->textBox2->Size = System::Drawing::Size(82, 20);
			this->textBox2->TabIndex = 11;
			// 
			// textBox3
			// 
			this->textBox3->Location = System::Drawing::Point(329, 73);
			this->textBox3->Name = L"textBox3";
			this->textBox3->Size = System::Drawing::Size(82, 20);
			this->textBox3->TabIndex = 15;
			// 
			// textBox4
			// 
			this->textBox4->Location = System::Drawing::Point(230, 73);
			this->textBox4->Name = L"textBox4";
			this->textBox4->Size = System::Drawing::Size(82, 20);
			this->textBox4->TabIndex = 12;
			// 
			// label13
			// 
			this->label13->AutoSize = true;
			this->label13->Location = System::Drawing::Point(252, 22);
			this->label13->Name = L"label13";
			this->label13->Size = System::Drawing::Size(43, 13);
			this->label13->TabIndex = 5;
			this->label13->Text = L"Value 1";
			// 
			// label14
			// 
			this->label14->AutoSize = true;
			this->label14->Location = System::Drawing::Point(43, 76);
			this->label14->Name = L"label14";
			this->label14->Size = System::Drawing::Size(70, 13);
			this->label14->TabIndex = 4;
			this->label14->Text = L"Tone Switch:";
			this->label14->TextAlign = System::Drawing::ContentAlignment::TopRight;
			// 
			// textBox5
			// 
			this->textBox5->Location = System::Drawing::Point(131, 73);
			this->textBox5->Name = L"textBox5";
			this->textBox5->Size = System::Drawing::Size(82, 20);
			this->textBox5->TabIndex = 3;
			// 
			// textBox6
			// 
			this->textBox6->Location = System::Drawing::Point(131, 47);
			this->textBox6->Name = L"textBox6";
			this->textBox6->Size = System::Drawing::Size(82, 20);
			this->textBox6->TabIndex = 2;
			// 
			// label15
			// 
			this->label15->AutoSize = true;
			this->label15->Location = System::Drawing::Point(161, 22);
			this->label15->Name = L"label15";
			this->label15->Size = System::Drawing::Size(35, 13);
			this->label15->TabIndex = 1;
			this->label15->Text = L"Offset";
			// 
			// label16
			// 
			this->label16->AutoSize = true;
			this->label16->Location = System::Drawing::Point(20, 52);
			this->label16->Name = L"label16";
			this->label16->Size = System::Drawing::Size(93, 13);
			this->label16->TabIndex = 0;
			this->label16->Text = L"ADF/ANT Switch:";
			// 
			// textBox7
			// 
			this->textBox7->Location = System::Drawing::Point(329, 47);
			this->textBox7->Name = L"textBox7";
			this->textBox7->Size = System::Drawing::Size(82, 20);
			this->textBox7->TabIndex = 14;
			// 
			// label25
			// 
			this->label25->AutoSize = true;
			this->label25->Location = System::Drawing::Point(351, 22);
			this->label25->Name = L"label25";
			this->label25->Size = System::Drawing::Size(43, 13);
			this->label25->TabIndex = 13;
			this->label25->Text = L"Value 2";
			// 
			// textBox8
			// 
			this->textBox8->Location = System::Drawing::Point(230, 47);
			this->textBox8->Name = L"textBox8";
			this->textBox8->Size = System::Drawing::Size(82, 20);
			this->textBox8->TabIndex = 11;
			// 
			// label26
			// 
			this->label26->AutoSize = true;
			this->label26->Location = System::Drawing::Point(252, 22);
			this->label26->Name = L"label26";
			this->label26->Size = System::Drawing::Size(43, 13);
			this->label26->TabIndex = 5;
			this->label26->Text = L"Value 1";
			// 
			// textBox9
			// 
			this->textBox9->Location = System::Drawing::Point(131, 47);
			this->textBox9->Name = L"textBox9";
			this->textBox9->Size = System::Drawing::Size(82, 20);
			this->textBox9->TabIndex = 2;
			// 
			// label27
			// 
			this->label27->AutoSize = true;
			this->label27->Location = System::Drawing::Point(161, 22);
			this->label27->Name = L"label27";
			this->label27->Size = System::Drawing::Size(35, 13);
			this->label27->TabIndex = 1;
			this->label27->Text = L"Offset";
			// 
			// label28
			// 
			this->label28->AutoSize = true;
			this->label28->Location = System::Drawing::Point(20, 52);
			this->label28->Name = L"label28";
			this->label28->Size = System::Drawing::Size(74, 13);
			this->label28->TabIndex = 0;
			this->label28->Text = L"COMM TEST:";
			// 
			// AdvForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(521, 434);
			this->ControlBox = false;
			this->Controls->Add(this->CancelButton);
			this->Controls->Add(this->OkButton);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->tabControl1);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::Fixed3D;
			this->MaximizeBox = false;
			this->MinimizeBox = false;
			this->Name = L"AdvForm";
			this->ShowIcon = false;
			this->ShowInTaskbar = false;
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
			this->TopMost = true;
			this->tabControl1->ResumeLayout(false);
			this->tabPage1->ResumeLayout(false);
			this->groupBox4->ResumeLayout(false);
			this->groupBox4->PerformLayout();
			this->groupBox3->ResumeLayout(false);
			this->groupBox3->PerformLayout();
			this->tabPage2->ResumeLayout(false);
			this->groupBox6->ResumeLayout(false);
			this->groupBox6->PerformLayout();
			this->groupBox5->ResumeLayout(false);
			this->groupBox5->PerformLayout();
			this->tabPage3->ResumeLayout(false);
			this->groupBox2->ResumeLayout(false);
			this->groupBox2->PerformLayout();
			this->groupBox1->ResumeLayout(false);
			this->groupBox1->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

	private: System::Void OkButton_Click(System::Object^  sender, System::EventArgs^  e) {

				//write INI file with new values
			    string Filename = "Config.ini";
				char writeData[7];
				sprintf_s(writeData, "%d", this->cmbADFANTOffset1->SelectedIndex);
				CIniFile::SetValue("ulADF1ANTOffset",writeData,"ADF",Filename);
				sprintf_s(writeData, "%d", this->cmbADFANTOffset2->SelectedIndex);
				CIniFile::SetValue("ulADF2ANTOffset",writeData,"ADF",Filename);
				sprintf_s(writeData, "%d", this->cmbADFTONEOffset1->SelectedIndex);
				CIniFile::SetValue("ulADF1TONEOffset",writeData,"ADF",Filename);
				sprintf_s(writeData, "%d", this->cmbADFTONEOffset2->SelectedIndex);
				CIniFile::SetValue("ulADF2TONEOffset",writeData,"ADF",Filename);

				sprintf_s(writeData, "%d", this->cmbCOMOffset1->SelectedIndex);
				CIniFile::SetValue("ulCOMTEST1Offset",writeData,"COM",Filename);
				sprintf_s(writeData, "%d", this->cmbCOMOffset2->SelectedIndex);
				CIniFile::SetValue("ulCOMTEST2Offset",writeData,"COM",Filename);

				sprintf_s(writeData, "%d", this->cmbNAVOffset1->SelectedIndex);
				CIniFile::SetValue("ulNAVTEST1Offset",writeData,"NAV",Filename);
				sprintf_s(writeData, "%d", this->cmbNAVOffset2->SelectedIndex);
				CIniFile::SetValue("ulNAVTEST2Offset",writeData,"NAV",Filename);

				sprintf_s(writeData, "%d", ulADF1Value1);
				CIniFile::SetValue("ulADF1Value1",writeData,"ADF",Filename);
				sprintf_s(writeData, "%d", ulADF1Value2);
				CIniFile::SetValue("ulADF1Value2",writeData,"ADF",Filename);
				sprintf_s(writeData, "%d", ulADF2Value1);
				CIniFile::SetValue("ulADF2Value1",writeData,"ADF",Filename);
				sprintf_s(writeData, "%d", ulADF2Value2);
				CIniFile::SetValue("ulADF2Value2",writeData,"ADF",Filename);

				sprintf_s(writeData, "%d", ulTONE1Value1);
				CIniFile::SetValue("ulTONE1Value1",writeData,"ADF",Filename);
				sprintf_s(writeData, "%d", ulTONE1Value2);
				CIniFile::SetValue("ulTONE1Value2",writeData,"ADF",Filename);
				sprintf_s(writeData, "%d", ulTONE2Value1);
				CIniFile::SetValue("ulTONE2Value1",writeData,"ADF",Filename);
				sprintf_s(writeData, "%d", ulTONE2Value2);
				CIniFile::SetValue("ulTONE2Value2",writeData,"ADF",Filename);

				

				sprintf_s(writeData, "%d", ulCOMTEST1Value1);
				CIniFile::SetValue("ulCOMTEST1Value1",writeData,"COM",Filename);
				sprintf_s(writeData, "%d", ulCOMTEST1Value2);
				CIniFile::SetValue("ulCOMTEST1Value2",writeData,"COM",Filename);
				sprintf_s(writeData, "%d", ulCOMTEST2Value1);
				CIniFile::SetValue("ulCOMTEST2Value1",writeData,"COM",Filename);
				sprintf_s(writeData, "%d", ulCOMTEST2Value2);
				CIniFile::SetValue("ulCOMTEST2Value2",writeData,"COM",Filename);

				

				sprintf_s(writeData, "%d", ulNAVTEST1Value1);
				CIniFile::SetValue("ulNAVTEST1Value1",writeData,"NAV",Filename);
				sprintf_s(writeData, "%d", ulNAVTEST1Value2);
				CIniFile::SetValue("ulNAVTEST1Value2",writeData,"NAV",Filename);
				sprintf_s(writeData, "%d", ulNAVTEST2Value1);
				CIniFile::SetValue("ulNAVTEST2Value1",writeData,"NAV",Filename);
				sprintf_s(writeData, "%d", ulNAVTEST2Value2);
				CIniFile::SetValue("ulNAVTEST2Value2",writeData,"NAV",Filename);

				this->Close();
			 }

private: System::Void CancelButton_Click(System::Object^  sender, System::EventArgs^  e) {
				this->Close();
		 }
private: System::Void cmbCOMOffset1_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {

			ulCOMTEST1Offset = AdvancedOffsetArray[cmbCOMOffset1->SelectedIndex];
			switch(cmbCOMOffset1->SelectedIndex)
			{
				case 0:
					ulCOMTEST1Value1=1;
					ulCOMTEST1Value2=2;
				break;

				case 1:
					ulCOMTEST1Value1=0;
					ulCOMTEST1Value2=1;
				break;

				case 2:
					ulCOMTEST1Value1=0xAA;//0x80;
					ulCOMTEST1Value2=0xAA;//0x7F;
				break;

				case 3:
					ulCOMTEST1Value1=0xBB;//0x40;
					ulCOMTEST1Value2=0xBB;//0xBF;
				break;

				case 4:
					ulCOMTEST1Value1=0x20;
					ulCOMTEST1Value2=0xDF;
				break;

				case 5:
					ulCOMTEST1Value1=0x10;
					ulCOMTEST1Value2=0xEF;
				break;

				case 6:
					ulCOMTEST1Value1=0x08;
					ulCOMTEST1Value2=0xF7;
				break;

				case 7:
					ulCOMTEST1Value1=0x04;
					ulCOMTEST1Value2=0xFB;
				break;

				case 8:
					ulCOMTEST1Value1=0x02;
					ulCOMTEST1Value2=0xFD;
				break;

				case 9:
					ulCOMTEST1Value1=0x01;
					ulCOMTEST1Value2=0xFE;
				break;
			}
		 }
private: System::Void cmbCOMOffset2_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
			 
			 ulCOMTEST2Offset = AdvancedOffsetArray[cmbCOMOffset2->SelectedIndex];

			 switch(cmbCOMOffset2->SelectedIndex)
			{
				case 0:
					ulCOMTEST2Value1=1;
					ulCOMTEST2Value2=2;
				break;

				case 1:
					ulCOMTEST2Value1=0;
					ulCOMTEST2Value2=1;
				break;

				case 2:
					ulCOMTEST2Value1=0xAA;//0x80;
					ulCOMTEST2Value2=0xAA;//0x7F;
				break;

				case 3:
					ulCOMTEST2Value1=0xBB;//0x40;
					ulCOMTEST2Value2=0xBB;//0xBF;
				break;

				case 4:
					ulCOMTEST2Value1=0x20;
					ulCOMTEST2Value2=0xDF;
				break;

				case 5:
					ulCOMTEST2Value1=0x10;
					ulCOMTEST2Value2=0xEF;
				break;

				case 6:
					ulCOMTEST2Value1=0x08;
					ulCOMTEST2Value2=0xF7;
				break;

				case 7:
					ulCOMTEST2Value1=0x04;
					ulCOMTEST2Value2=0xFB;
				break;

				case 8:
					ulCOMTEST2Value1=0x02;
					ulCOMTEST2Value2=0xFD;
				break;

				case 9:
					ulCOMTEST2Value1=0x01;
					ulCOMTEST2Value2=0xFE;
				break;
			}
		 }
private: System::Void cmbNAVOffset1_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {

			 ulNAVTEST1Offset = AdvancedOffsetArray[cmbNAVOffset1->SelectedIndex];
			 switch(cmbNAVOffset1->SelectedIndex)
			{
				case 0:
					ulNAVTEST1Value1=1;
					ulNAVTEST1Value2=2;
				break;

				case 1:
					ulNAVTEST1Value1=0;
					ulNAVTEST1Value2=1;
				break;

				case 2:
					ulNAVTEST1Value1=0xAA;//0x80;
					ulNAVTEST1Value2=0xAA;//0x7F;
				break;

				case 3:
					ulNAVTEST1Value1=0xBB;//0x40;
					ulNAVTEST1Value2=0xBB;//0xBF;
				break;

				case 4:
					ulNAVTEST1Value1=0x20;
					ulNAVTEST1Value2=0xDF;
				break;

				case 5:
					ulNAVTEST1Value1=0x10;
					ulNAVTEST1Value2=0xEF;
				break;

				case 6:
					ulNAVTEST1Value1=0x08;
					ulNAVTEST1Value2=0xF7;
				break;

				case 7:
					ulNAVTEST1Value1=0x04;
					ulNAVTEST1Value2=0xFB;
				break;

				case 8:
					ulNAVTEST1Value1=0x02;
					ulNAVTEST1Value2=0xFD;
				break;

				case 9:
					ulNAVTEST1Value1=0x01;
					ulNAVTEST1Value2=0xFE;
				break;
			}
		 }
private: System::Void cmbNAVOffset2_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {

			 ulNAVTEST2Offset = AdvancedOffsetArray[cmbNAVOffset2->SelectedIndex];
			 switch(cmbNAVOffset2->SelectedIndex)
			{
				case 0:
					ulNAVTEST2Value1=1;
					ulNAVTEST2Value2=2;
				break;

				case 1:
					ulNAVTEST2Value1=0;
					ulNAVTEST2Value2=1;
				break;

				case 2:
					ulNAVTEST2Value1=0xAA;//0x80;
					ulNAVTEST2Value2=0xAA;//0x7F;
				break;

				case 3:
					ulNAVTEST2Value1=0xBB;//0x40;
					ulNAVTEST2Value2=0xBB;//0xBF;
				break;

				case 4:
					ulNAVTEST2Value1=0x20;
					ulNAVTEST2Value2=0xDF;
				break;

				case 5:
					ulNAVTEST2Value1=0x10;
					ulNAVTEST2Value2=0xEF;
				break;

				case 6:
					ulNAVTEST2Value1=0x08;
					ulNAVTEST2Value2=0xF7;
				break;

				case 7:
					ulNAVTEST2Value1=0x04;
					ulNAVTEST2Value2=0xFB;
				break;

				case 8:
					ulNAVTEST2Value1=0x02;
					ulNAVTEST2Value2=0xFD;
				break;

				case 9:
					ulNAVTEST2Value1=0x01;
					ulNAVTEST2Value2=0xFE;
				break;
			}
		 }
private: System::Void cmbADFANTOffset1_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {

			ulADF1ANTOffset = AdvancedOffsetArray[cmbADFANTOffset1->SelectedIndex];
			switch(cmbCOMOffset2->SelectedIndex)
			{
				case 0:
					ulADF1Value1=1;
					ulADF1Value2=2;
				break;

				case 1:
					ulADF1Value1=0;
					ulADF1Value2=1;
				break;

				case 2:
					ulADF1Value1=0xAA;//0x80;
					ulADF1Value2=0xAA;//0x7F;
				break;

				case 3:
					ulADF1Value1=0xBB;//0x40;
					ulADF1Value2=0xBB;//0xBF;
				break;

				case 4:
					ulADF1Value1=0x20;
					ulADF1Value2=0xDF;
				break;

				case 5:
					ulADF1Value1=0x10;
					ulADF1Value2=0xEF;
				break;

				case 6:
					ulADF1Value1=0x08;
					ulADF1Value2=0xF7;
				break;

				case 7:
					ulADF1Value1=0x04;
					ulADF1Value2=0xFB;
				break;

				case 8:
					ulADF1Value1=0x02;
					ulADF1Value2=0xFD;
				break;

				case 9:
					ulADF1Value1=0x01;
					ulADF1Value2=0xFE;
				break;
			}
		 }
private: System::Void cmbADFTONEOffset1_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {

			ulADF1TONEOffset = AdvancedOffsetArray[cmbADFTONEOffset1->SelectedIndex];
			switch(cmbCOMOffset2->SelectedIndex)
			{
				case 0:
					ulTONE1Value1=1;
					ulTONE1Value2=2;
				break;

				case 1:
					ulTONE1Value1=0;
					ulTONE1Value2=1;
				break;

				case 2:
					ulTONE1Value1=0xAA;//0x80;
					ulTONE1Value2=0xAA;//0x7F;
				break;

				case 3:
					ulTONE1Value1=0xBB;//0x40;
					ulTONE1Value2=0xBB;//0xBF;
				break;

				case 4:
					ulTONE1Value1=0x20;
					ulTONE1Value2=0xDF;
				break;

				case 5:
					ulTONE1Value1=0x10;
					ulTONE1Value2=0xEF;
				break;

				case 6:
					ulTONE1Value1=0x08;
					ulTONE1Value2=0xF7;
				break;

				case 7:
					ulTONE1Value1=0x04;
					ulTONE1Value2=0xFB;
				break;

				case 8:
					ulTONE1Value1=0x02;
					ulTONE1Value2=0xFD;
				break;

				case 9:
					ulTONE1Value1=0x01;
					ulTONE1Value2=0xFE;
				break;
			}
		 }
private: System::Void cmbADFANTOffset2_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {

			ulADF2ANTOffset = AdvancedOffsetArray[cmbADFANTOffset2->SelectedIndex];
			switch(cmbCOMOffset2->SelectedIndex)
			{
				case 0:
					ulADF2Value1=1;
					ulADF2Value2=2;
				break;

				case 1:
					ulADF2Value1=0;
					ulADF2Value2=1;
				break;

				case 2:
					ulADF2Value1=0xAA;//0x80;
					ulADF2Value2=0xAA;//0x7F;
				break;

				case 3:
					ulADF2Value1=0xBB;//0x40;
					ulADF2Value2=0xBB;//0xBF;
				break;

				case 4:
					ulADF2Value1=0x20;
					ulADF2Value2=0xDF;
				break;

				case 5:
					ulADF2Value1=0x10;
					ulADF2Value2=0xEF;
				break;

				case 6:
					ulADF2Value1=0x08;
					ulADF2Value2=0xF7;
				break;

				case 7:
					ulADF2Value1=0x04;
					ulADF2Value2=0xFB;
				break;

				case 8:
					ulADF2Value1=0x02;
					ulADF2Value2=0xFD;
				break;

				case 9:
					ulADF2Value1=0x01;
					ulADF2Value2=0xFE;
				break;
			}
		 }
private: System::Void cmbADFTONEOffset2_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {

			 ulADF2TONEOffset = AdvancedOffsetArray[cmbADFTONEOffset2->SelectedIndex];
			 switch(cmbCOMOffset2->SelectedIndex)
			{
				case 0:
					ulTONE2Value1=1;
					ulTONE2Value2=2;
				break;

				case 1:
					ulTONE2Value1=0;
					ulTONE2Value2=1;
				break;

				case 2:
					ulTONE2Value1=0xAA;//0x80;
					ulTONE2Value2=0xAA;//0x7F;
				break;

				case 3:
					ulTONE2Value1=0xBB;//0x40;
					ulTONE2Value2=0xBB;//0xBF;
				break;

				case 4:
					ulTONE2Value1=0x20;
					ulTONE2Value2=0xDF;
				break;

				case 5:
					ulTONE2Value1=0x10;
					ulTONE2Value2=0xEF;
				break;

				case 6:
					ulTONE2Value1=0x08;
					ulTONE2Value2=0xF7;
				break;

				case 7:
					ulTONE2Value1=0x04;
					ulTONE2Value2=0xFB;
				break;

				case 8:
					ulTONE2Value1=0x02;
					ulTONE2Value2=0xFD;
				break;

				case 9:
					ulTONE2Value1=0x01;
					ulTONE2Value2=0xFE;
				break;
			}
		 }
};
}
