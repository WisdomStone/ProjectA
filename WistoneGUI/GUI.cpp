#include "stdafx.h"
#include "GUI.h"

#include <assert.h>	// YL 30.4

using namespace WistoneGUI;
	
GUI::GUI(void)
{
	InitializeComponent();

	secondsDuration			= 0;
	minutesDuration			= 0;
	hoursDuration			= 0;
	lastIndexOccurrence		= -2;
	newLastIndexOccurrence	= -1;
	commandCounter			= 0;
	buttonSendFlag			= false;
	// YL 4.5 ... added
	comPortConnection		= false;
	internetConnection		= false;
	// ... YL 4.5

	// YL 7.5 ... added
	thisGUI = new gcroot<GUI^>();
	*thisGUI = this;
	server = new Server(thisGUI);
	// ... YL 7.5

	updateCOMPortList();

	sendCommands_TextBox->ScrollBars = ScrollBars::Vertical;
	console_TextBox->ScrollBars = ScrollBars::Both;			
}

GUI::~GUI(void)			
{
	// YL 30.4 ... delete unmanaged object
	if (server != nullptr) 
	{
		delete server;
		server = nullptr;
	}
	// ... YL 30.4
	delete thisGUI; // YL 7.5
	if (components)
	{
		delete components;
		Application::Exit();
	}
}

/****************************************************************************
Function:
public: void systemMessage(int level, char* msg_discription, char *func_name, char *msg_value) 
- a global function, serving all other functions in all hierarchies and files in the SW
- receives: msg level, msg code, function name where it occurred and related value.
- open a window that shows:
	- the level (icon and word)
	- msg code and it short explanation
	- function name where it occurred
	- action needed (re-enter, input ignore, program terminated...)
	- help button will direct to relevant help page
- save a record into log file	
***************************************************************************/
void GUI::systemMessage(int level, char* msg_discription, char *func_name, char *msg_value) 
{
	char			msg[1000] = "";
	char			caption[100] = "";
	MessageBoxIcon	icon;

	// set window icon type according to level:	
	/*default:*/			icon = MessageBoxIcon::Stop;		strcpy_s(caption, 90, "Unknown Message Level");
	if (level == INFO_MSG) {icon = MessageBoxIcon::Asterisk;	strcpy_s(caption, 90, "Information");}
	if (level == NTIC_MSG) {icon = MessageBoxIcon::Information; strcpy_s(caption, 90, "Notice");}
	if (level == WRNG_MSG) {icon = MessageBoxIcon::Exclamation; strcpy_s(caption, 90, "Warning");}
	if (level == ERR_MSG)  {icon = MessageBoxIcon::Error;		strcpy_s(caption, 90, "Error");}

	// prepare the string for Message Box:
	if (msg_value[0] != 0) 
	{
		sprintf_s(msg, sizeof(msg), "function: %s()\ndescription: %s\nvalue: %s\n", func_name, msg_discription, msg_value);
	}
	else
	{
		sprintf_s(msg, sizeof(msg), "function: %s()\ndescription: %s\n", func_name, msg_discription);
	}
	if (level == INFO_MSG) // simple INFO message
	{
		// YL 15.6 ...
		// was: MessageBox::Show(%String(msg_value));
		MessageBox::Show(%String(msg));
		// ... YL 15.6
	}
	else
	{
		MessageBox::Show(%String(msg), %String(caption), MessageBoxButtons::OK, icon, MessageBoxDefaultButton::Button1, MessageBoxOptions::ServiceNotification);
	}
	return;
}

/****************************************************************************
Function:
public: void SetText()
***************************************************************************/
void GUI::SetText(String^ text)
{
	// InvokeRequired required compares the thread ID of the
	// calling thread to the thread ID of the creating thread.
	// If these threads are different, it returns true.  We can
	// use this attribute to determine if we can append text
	// directly to the textbox or if we must launch an a delegate
	// function instance to write to the textbox.
	if (this->console_TextBox->InvokeRequired)
	{
		// InvokeRequired returned TRUE meaning that this function
		// was called from a thread different than the current
		// thread.  We must launch a delegate function.

		// Create an instance of the SetTextCallback delegate and
		// assign the delegate function to be this function.  This
		// effectively causes this same SetText() function to be
		// called within the main thread instead of the second
		// thread.
		SetTextCallback^ d = gcnew SetTextCallback(this, &WistoneGUI::GUI::SetText);

		// Invoke the new delegate sending the same text to the
		// delegate that was passed into this function from the
		// other thread.
		this->Invoke(d, gcnew String(text));
	}
	else
	{
		// If this function was called from the same thread that 
		// holds the required objects then just add the text.
		console_TextBox->AppendText(text);
	}
}

/****************************************************************************
Function:
public: void setServerConnected(bool isConnected)
***************************************************************************/
void GUI::setServerConnected(bool isConnected) // YL 21.6
{
	serverConnected = isConnected;
	serverCounter = 0;
}

/****************************************************************************
Function:
public: void resetServerCounter(void)
***************************************************************************/
void GUI::resetServerCounter(void) // YL 21.6
{
	serverCounter = 0;
}


/***A C C M T R***/	
	
/****************************************************************************
Function:
private: System::Void read_readLL_AccmtrTab_Button_Click(System::Object^  sender, System::EventArgs^  e)
***************************************************************************/			 
System::Void GUI::read_readLL_AccmtrTab_Button_Click(System::Object^  sender, System::EventArgs^  e) 
{
	StreamWriter^ sw = System::IO::File::AppendText(commandsLogFilePath);
		
	// YL 4.12 added destination
	sw->WriteLine("{0} r accmtr {1} ", destination_readLL_AccmtrTab_TextBox->Text, address_readLL_AccmtrTab_NumericUpDown->Text); 
	try
	{
		// Write the data in the text box to the open serial port
		sendCommands_TextBox->Text = destination_readLL_AccmtrTab_TextBox->Text + " r accmtr "+ address_readLL_AccmtrTab_NumericUpDown->Text; 
		buttonSendFlag = true;
	}
	catch(...)
	{
	}
	sw->Close();
}
	
/****************************************************************************
Function:
private: System::Void updateValue_config_AccmtrTab_Button_Click(System::Object^  sender, System::EventArgs^  e)
***************************************************************************/			 
System::Void GUI::updateValue_config_AccmtrTab_Button_Click(System::Object^  sender, System::EventArgs^  e) 
{
	StreamWriter^ sw = System::IO::File::AppendText(commandsLogFilePath);
		
	// YL 4.12 added destination
	sw->WriteLine("{0} accmtr config {1} {2} ", destination_config_AccmtrTab_TextBox->Text, option_config_AccmtrTab_NumericUpDown->Text, parameter_config_AccmtrTab_ComboBox->Text);
	try
	{
		// Write the data in the text box to the open serial port
		sendCommands_TextBox->Text = destination_config_AccmtrTab_TextBox->Text + " accmtr config " + option_config_AccmtrTab_NumericUpDown->Text + " " + parameter_config_AccmtrTab_ComboBox->Text;
		buttonSendFlag = true;
	}
	catch(...)
	{
	}
	sw->Close();
}	
	
/****************************************************************************
Function:
private: System::Void option_config_AccmtrTab_NumericUpDown_ValueChanged(System::Object^  sender, System::EventArgs^  e)
***************************************************************************/			 
System::Void GUI::option_config_AccmtrTab_NumericUpDown_ValueChanged(System::Object^  sender, System::EventArgs^  e) 
{
	if (option_config_AccmtrTab_NumericUpDown->Value == 1)
	{
		parameter_config_AccmtrTab_ComboBox->Items->Clear();
		parameter_config_AccmtrTab_ComboBox->Items->Add("1");
		parameter_config_AccmtrTab_ComboBox->Items->Add("2");
		parameter_config_AccmtrTab_ComboBox->Items->Add("4");
	}
	else if (option_config_AccmtrTab_NumericUpDown->Value == 2)
	{
		parameter_config_AccmtrTab_ComboBox->Items->Clear();
		parameter_config_AccmtrTab_ComboBox->Items->Add("2");
		parameter_config_AccmtrTab_ComboBox->Items->Add("4");
		parameter_config_AccmtrTab_ComboBox->Items->Add("8");
	}
	else if (option_config_AccmtrTab_NumericUpDown->Value == 3)
	{
		parameter_config_AccmtrTab_ComboBox->Items->Clear();
		parameter_config_AccmtrTab_ComboBox->Items->Add("1");
		parameter_config_AccmtrTab_ComboBox->Items->Add("2");
		parameter_config_AccmtrTab_ComboBox->Items->Add("3");
		parameter_config_AccmtrTab_ComboBox->Items->Add("4");
	}
	else
	{
		parameter_config_AccmtrTab_ComboBox->Items->Clear();
		parameter_config_AccmtrTab_ComboBox->Items->Add("0");
	}
}

/****************************************************************************
Function:
private: System::Void write_writeLL_AccmtrTab_Button_Click(System::Object^  sender, System::EventArgs^  e)
***************************************************************************/		 
System::Void GUI::write_writeLL_AccmtrTab_Button_Click(System::Object^  sender, System::EventArgs^  e) 
{
    StreamWriter^ sw = System::IO::File::AppendText(commandsLogFilePath);
	int value;
		
	if (Int32::TryParse(address_writeLL_AccmtrTab_TextBox->Text,value))
	{
		if (value >= 0)
		{
			// YL 4.12 added destination
			sw->WriteLine("{0} w accmtr {1} {2} ", destination_writeLL_AccmtrTab_TextBox->Text, address_writeLL_AccmtrTab_TextBox->Text, data_writeLL_AccmtrTab_TextBox->Text);
			try
			{
				// Write the data in the text box to the open serial port
				sendCommands_TextBox->Text = destination_writeLL_AccmtrTab_TextBox->Text + " w accmtr " + address_writeLL_AccmtrTab_TextBox->Text + " " + data_writeLL_AccmtrTab_TextBox->Text;
				buttonSendFlag = true;
			}
			catch(...)
			{
			}
		}
		else
		{
			address_writeLL_AccmtrTab_TextBox->Text = "Error";
			address_writeLL_AccmtrTab_TextBox->Select();
		}
	}
	else
	{
		address_writeLL_AccmtrTab_TextBox->Text = "Error";
		address_writeLL_AccmtrTab_TextBox->Select();
	}
	sw->Close();
}	
	
/***A D C***/	
	
/****************************************************************************
Function:
private: System::Void read_readLL_AdcTab_Button_Click(System::Object^  sender, System::EventArgs^  e)
***************************************************************************/
System::Void GUI::read_readLL_AdcTab_Button_Click(System::Object^  sender, System::EventArgs^  e) 
{
	StreamWriter^ sw = System::IO::File::AppendText(commandsLogFilePath);
	int value;
		
	if (Int32::TryParse(address_readLL_AdcTab_TextBox->Text, value))
	{
		if(value >= 0)
		{
			// YL 4.12 added destination
			sw->WriteLine("{0} r adc {1} ", destination_readLL_AdcTab_TextBox->Text, address_readLL_AdcTab_TextBox->Text);
			try
			{
				// Write the data in the text box to the open serial port
				sendCommands_TextBox->Text = destination_readLL_AdcTab_TextBox->Text + " r adc " + address_readLL_AdcTab_TextBox->Text;
				buttonSendFlag = true;
			}
			catch(...)
			{
			}
		}
		else
		{
			address_readLL_AdcTab_TextBox->Text = "Error";
			address_readLL_AdcTab_TextBox->Select();
		}
	}
	else
	{
		address_readLL_AdcTab_TextBox->Text = "Error";
		address_readLL_AdcTab_TextBox->Select();
	}
	sw->Close();
}

/****************************************************************************
Function:
private: System::Void write_writeLL_AdcTab_Button_Click(System::Object^  sender, System::EventArgs^  e)
***************************************************************************/			 
System::Void GUI::write_writeLL_AdcTab_Button_Click(System::Object^  sender, System::EventArgs^  e) 
{
	StreamWriter^ sw = System::IO::File::AppendText(commandsLogFilePath);
	int value;
		
	if (Int32::TryParse(address_writeLL_AdcTab_TextBox->Text, value))
	{
		if (value >= 0)
		{
			// YL 4.12 added destination
			sw->WriteLine("{0} w adc {1} {2} ", destination_writeLL_AdcTab_TextBox->Text, address_writeLL_AdcTab_TextBox->Text, data_writeLL_AdcTab_ComboBox->Text);
			try
			{
				// Write the data in the text box to the open serial port
				sendCommands_TextBox->Text = destination_writeLL_AdcTab_TextBox->Text + " w adc " + address_writeLL_AdcTab_TextBox->Text + " " + data_writeLL_AdcTab_ComboBox->Text;
				buttonSendFlag = true;
			}
			catch(...)
			{
			}
		}
		else
		{
			address_writeLL_AdcTab_TextBox->Text = "Error";
			address_writeLL_AdcTab_TextBox->Select();
		}
	}
	else
	{
		address_writeLL_AdcTab_TextBox->Text = "Error";
		address_writeLL_AdcTab_TextBox->Select();
	}
	sw->Close();
}

/***A P P L I C A T I O N***/	
	
/****************************************************************************
Function:
private: System::Void detectAppStart()
***************************************************************************/		 
System::Void GUI::detectAppStart()
{ 
	int numOfBlocks = 0;
	array<String^> ^buffer = gcnew array<String^>(100);
	array<wchar_t> ^separator = gcnew array<wchar_t>(2);
	separator[0] = ' ';
	separator[1] = 0;
	String^ s = sendCommands_TextBox->Text;
		
	buffer = s->Split(separator);
	if (buffer->Length < MIN_PARAM_COUNT)
	{
		return;
	}
	if (Int32::TryParse(buffer[3], numOfBlocks))
	{
		numberOfBlocks_start_AppTab_TextBox->Text = buffer[3];
		address_start_AppTab_TextBox->Text = buffer[4];
		totalBytes = (numOfBlocks + 1) * TOTAL_BLOCK_SIZE + 100;
		// save transaction length in global variable:
		gTotalNumOfBytesToReceive = totalBytes;
	}
	else
	{
		sendCommands_TextBox->Text = "Error"; // YL check
		return;
	}
	// YL 30.4 ... what about ss mode?
	// was: if (s->StartsWith("app start ts")) 
	if (s->Contains("app start ts")) // to skip the network destination
	// ... YL 30.4
	{
		applicationStart = true;
		TS = true;
	}
	// YL 30.4 ...
	// was: else if(s->StartsWith("app start ost"))
	else if(s->Contains("app start ost"))
	// ... YL 30.4
	{
		applicationStart = true;
		OST = true;
	}
}	
	
/****************************************************************************
Function:
private: System::Void ss_RadioButton_Click(System::Object^  sender, System::EventArgs^  e)
***************************************************************************/			 
System::Void GUI::ss_RadioButton_Click(System::Object^  sender, System::EventArgs^  e) 
{
	SS = true;
	TS = false;
	OST = false;
	gTerminationFinder = "";
	// YL 17.6 ...
	// was:
	// outputFile_start_AppTab_TextBox->Enabled = false;
	// address_start_AppTab_TextBox->Enabled = true;
	// communication_start_AppTab_ComboBox->Enabled = false;
	// combination_start_AppTab_ComboBox->Enabled = true;
	// sessionTimeout_start_AppTab_TextBox->Enabled = false;
	EnableApplicationParametersInput = true;
	// ... YL 17.6
}

/****************************************************************************
Function:
private: System::Void ts_RadioButton_Click(System::Object^  sender, System::EventArgs^  e)
***************************************************************************/			 
System::Void GUI::ts_RadioButton_Click(System::Object^  sender, System::EventArgs^  e) 
{
	SS = false;
	TS = true;
	OST = false;
	gTerminationFinder = "";
	// YL 17.6 ...
	// was:
	// outputFile_start_AppTab_TextBox->Enabled = false; // YL 17.6 added
	// address_start_AppTab_TextBox->Enabled = true;
	// communication_start_AppTab_ComboBox->Enabled = true; 
	// combination_start_AppTab_ComboBox->Enabled = false;
	// sessionTimeout_start_AppTab_TextBox->Enabled = true; // YL 17.6 added
	EnableApplicationParametersInput = true;
	// ... YL 17.6
}
	
/****************************************************************************
Function:
private: System::Void ost_RadioButton_Click(System::Object^  sender, System::EventArgs^  e)
***************************************************************************/				 
System::Void GUI::ost_RadioButton_Click(System::Object^  sender, System::EventArgs^  e) 
{
	SS = false;
	TS = false;
	OST = true;
	gTerminationFinder = "";
	// YL 17.6 ...
	// was:
	// communication_start_AppTab_ComboBox->Enabled = true;
	// address_start_AppTab_TextBox->Enabled = false;
	// combination_start_AppTab_ComboBox->Enabled = true;
	EnableApplicationParametersInput = true;
	// ... YL 17.6
}

/****************************************************************************
Function:
private: System::Void startApplication_AppTab_Button_Click(System::Object^  sender, System::EventArgs^  e)
***************************************************************************/			 
System::Void GUI::startApplication_AppTab_Button_Click(System::Object^  sender, System::EventArgs^  e) 
{
	// Input parameters to read from GUI boxes:
	int	input_NumOfBlocks, input_StartAddress, input_Destination; // YL 3.12 added input_Destination
	int	valueAdd = 0, valueNum;

	// Retrieve input parameters from GUI boxes + check the input:
	if ((SS == false) && (TS == false) && (OST == false)) 
	{
		systemMessage(ERR_MSG, "Must choose an Application Mode: SS, TS or OST", "Start Application Clicked", "");
		return;
	}
	// YL 3.12 ... added destination + was: ComboBox instead of TextBox
	if (!Int32::TryParse(destination_start_AppTab_TextBox->Text, input_Destination)) 
	{
		systemMessage(ERR_MSG, "Enter Destination or 0 for Broadcast", "Start Application Clicked", "");
		destination_start_AppTab_TextBox->Select();
		return;
	}		
	// ... YL 3.12
	if ((OST == false) && (!Int32::TryParse(address_start_AppTab_TextBox->Text, input_StartAddress))) 
	{
		systemMessage(ERR_MSG, "Enter Start Address: 0 ~ 2097151", "Start Application Clicked", "");
		address_start_AppTab_TextBox->Select();
		return;
	}		
	if (!Int32::TryParse(numberOfBlocks_start_AppTab_TextBox->Text, input_NumOfBlocks)) 
	{
		systemMessage(ERR_MSG, "Enter Number of Blocks: 0 ~ 2097151", "Start Application Clicked", "");
		numberOfBlocks_start_AppTab_TextBox->Select();
		return;
	}
	if ((SS == false) && (String::IsNullOrEmpty(communication_start_AppTab_ComboBox->Text))) 
	{
		systemMessage(ERR_MSG, "Missing Communication", "Start Application Clicked", ""); // YL 16.9 was: Missing Destination
		return;
	}
	if ((TS == false) && (String::IsNullOrEmpty(combination_start_AppTab_ComboBox->Text))) 
	{
		systemMessage(ERR_MSG, "Missing Combination", "Start Application Clicked", "");
		return;
	}
	if ((SS == false) && (String::IsNullOrEmpty(outputFile_start_AppTab_TextBox->Text))) 
	{
		systemMessage(ERR_MSG, "Missing Output File name", "Start Application Clicked", "");
		return;
	}
	// YL 3.12 ... added destination
	if (input_Destination < 0 || input_Destination > MAX_SN) 
	{
		systemMessage(ERR_MSG, "Destination should be greater than zero and less than MAX_SN", "Start Application Clicked", "");
		destination_start_AppTab_TextBox->Select();
		return;
	}		
	// ... YL 3.12		
	if (input_NumOfBlocks <= 0) 
	{
		systemMessage(ERR_MSG, "Number of Blocks should be greater than zero", "Start Application Clicked", "");
		numberOfBlocks_start_AppTab_TextBox->Select();
		return;
	}
	if (input_NumOfBlocks > 1000) 
	{
		systemMessage(INFO_MSG, "Number of Blocks is large. Operation may take a while...", "Start Application Clicked", "");
		numberOfBlocks_start_AppTab_TextBox->Select();
	}
	if ((SS == false) && (communication_start_AppTab_ComboBox->Text->Equals(%String("usb")))) // YL 17.6 added (SS == false)
	{
		systemMessage(INFO_MSG, "Make sure the Stone has a valid USB connection", "Start Application Clicked", "");
	}
	if ((OST == false) && (input_StartAddress < 0)) 
	{
		systemMessage(ERR_MSG, "Start Address should be positive", "Start Application Clicked", "");
		address_start_AppTab_TextBox->Select();
		return;
	}
	// We allow to save blocks up to half of the FLASH memory. since the upper half is dedicated to ADS1282:
	if ((SS == true) && ((input_StartAddress + input_NumOfBlocks) > ((Math::Pow(2, 31) / 2) / TOTAL_BLOCK_SIZE))) 
	{
		systemMessage(ERR_MSG, "Number of Block and Start Address should not exceed half of FLASH memory size", "Start Application Clicked", "");
		address_start_AppTab_TextBox->Select();
		return;
	}
	// We allow to save blocks up to half of the FLASH memory. since the upper half is dedicated to ADS1282:
	if ((OST == false) && (input_StartAddress) > (Math::Pow(2, 31) / TOTAL_BLOCK_SIZE)) 
	{
		systemMessage(ERR_MSG, "Too large Start Address", "Start Application Clicked", "");
		address_start_AppTab_TextBox->Select();
		return;
	}
	if ((TS == false) && (combination_start_AppTab_ComboBox->Text->Equals(%String("dual")))) 
	{
		systemMessage(INFO_MSG, "Dual Combination Mode is not yet fully supported", "Start Application Clicked", "");
		return;
	}
	int sessionTimeout = 1;
	if ((SS == false) && (!Int32::TryParse(sessionTimeout_start_AppTab_TextBox->Text, sessionTimeout) || (sessionTimeout < 0 || sessionTimeout > MAX_SESSION_TIMEOUT))) 
	{
		systemMessage(ERR_MSG, "Enter Session Timeout Length in Seconds: 1 ~ 600", "Start Application Clicked", "");
		sessionTimeout_start_AppTab_TextBox->Select();
		return;
	}

	StreamWriter^ sw = System::IO::File::AppendText(commandsLogFilePath);
	// Disable input of application parameters (re-enabled when application finished running):
	EnableApplicationParametersInput = false;
	valueNum = input_NumOfBlocks;
	valueAdd = input_StartAddress;
	totalBytes = (valueNum + 1) * TOTAL_BLOCK_SIZE + 40;
	// save transaction length in global variable:
	gTotalNumOfBytesToReceive = totalBytes;
	applicationStart = true;
	if (SS == true) 
	{
		// YL 3.12 ... added destination 
		// was: sw->WriteLine("app start ss {0} {1} {2}", numberOfBlocks_start_AppTab_TextBox->Text, address_start_AppTab_TextBox->Text, combination_start_AppTab_ComboBox->Text);
		sw->WriteLine("{0} app start ss {1} {2} {3}", destination_start_AppTab_TextBox->Text, numberOfBlocks_start_AppTab_TextBox->Text, address_start_AppTab_TextBox->Text, combination_start_AppTab_ComboBox->Text);
		// ... YL 3.12
	}
	else if (TS == true) 
	{
		// YL 3.12 ... added destination
		// was: sw->WriteLine("app start ts {0} {1} {2}", numberOfBlocks_start_AppTab_TextBox->Text, address_start_AppTab_TextBox->Text, communication_start_AppTab_ComboBox->Text);
		sw->WriteLine("{0} app start ts {1} {2} {3}", destination_start_AppTab_TextBox->Text, numberOfBlocks_start_AppTab_TextBox->Text, address_start_AppTab_TextBox->Text, communication_start_AppTab_ComboBox->Text);
		// ... YL 3.12
	}
	else if (OST == true) 
	{
		// YL 3.12 ... added destination
		// was: sw->WriteLine("app start ost {0} {1} {2}", numberOfBlocks_start_AppTab_TextBox->Text, communication_start_AppTab_ComboBox->Text, combination_start_AppTab_ComboBox->Text);
		sw->WriteLine("{0} app start ost {1} {2} {3}", destination_start_AppTab_TextBox->Text, numberOfBlocks_start_AppTab_TextBox->Text, communication_start_AppTab_ComboBox->Text, combination_start_AppTab_ComboBox->Text);
		// ... YL 3.12
	}

	try 
	{
		// Write the data in the text box to the open serial port:
		if (SS == true) 
		{
			// YL 3.12 ... added destination
			// was: sendCommands_TextBox->Text = "app start ss " + numberOfBlocks_start_AppTab_TextBox->Text + " " +address_start_AppTab_TextBox->Text + " " + combination_start_AppTab_ComboBox->Text;
			sendCommands_TextBox->Text = destination_start_AppTab_TextBox->Text + " app start ss " + numberOfBlocks_start_AppTab_TextBox->Text + " " + address_start_AppTab_TextBox->Text + " " + combination_start_AppTab_ComboBox->Text;
			// ... YL 3.12
		}
		else if (TS == true) 
		{
			// YL 3.12 ... added destination			
			// was: sendCommands_TextBox->Text = "app start ts " + numberOfBlocks_start_AppTab_TextBox->Text + " " + address_start_AppTab_TextBox->Text + " " + communication_start_AppTab_ComboBox->Text;
			sendCommands_TextBox->Text = destination_start_AppTab_TextBox->Text + " app start ts " + numberOfBlocks_start_AppTab_TextBox->Text + " " + address_start_AppTab_TextBox->Text + " " + communication_start_AppTab_ComboBox->Text;
			// ... YL 3.12
		}
		else if (OST == true) 
		{
			// YL 3.12 ... added destination
			// was: sendCommands_TextBox->Text = "app start ost " + numberOfBlocks_start_AppTab_TextBox->Text + " " + communication_start_AppTab_ComboBox->Text + " " + combination_start_AppTab_ComboBox->Text;
			sendCommands_TextBox->Text = destination_start_AppTab_TextBox->Text + " app start ost " + numberOfBlocks_start_AppTab_TextBox->Text + " " + communication_start_AppTab_ComboBox->Text + " " + combination_start_AppTab_ComboBox->Text;
			// ... YL 3.12
		}
		buttonSendFlag = true;
		// YL 16.6 startApplication_start_AppTab_Button->Enabled = false;
		setDate_setDate_WakeUpTab_Button->Enabled = true;
		if (SS == false)
		{
			// Initialize and starting session timeout counter:
			timeoutCounter = 0;
			sessionTimeout_Timer->Enabled = true;
			// YL 6.11 in 5.1 there is additional line: inNetworkRecoveryMode = false;
		} 
	}
	catch(...) 
	{
	}
	sw->Close();				 
}

/****************************************************************************
Function:
private: System::Void stopApplication_AppTab_Button_Click(System::Object^  sender, System::EventArgs^  e)
***************************************************************************/		 
System::Void GUI::stopApplication_AppTab_Button_Click(System::Object^  sender, System::EventArgs^  e) 
{
	StreamWriter^ sw = System::IO::File::AppendText(commandsLogFilePath);
		
	// YL 4.12 added destination
	sw->WriteLine("{0} app stop", destination_start_AppTab_TextBox->Text);
	sw->Close();
	try 
	{
		// Write the data in the text box to the open serial port:
		sendCommands_TextBox->Text = destination_start_AppTab_TextBox->Text + " app stop";
		buttonSendFlag = true;
		// Enable application parameters input (disabled at startApplication_AppTab_Button_Clicks):
		applicationStart = false;
		EnableApplicationParametersInput = true;
		setDate_setDate_WakeUpTab_Button->Enabled = false;
		// YL 16.6 startApplication_start_AppTab_Button->Enabled = true;
		if(sessionTimeout_Timer->Enabled == true)
		{
			// Stopping session timeout counter:
			sessionTimeout_Timer->Enabled = false;
		}
		// YL 6.11 in 5.1 instead of if above there is:
		// if(SS == false)
		// {
		//	//stopping session timeout counter
		//	sessionTimeout_Timer->Enabled = false;
		//	inNetworkRecoveryMode = false;
		// }			
	}
	catch(...) 
	{
	}
}	
	
/****************************************************************************
Function:
private: System::Void updateValue_config_AppTab_Button_Click(System::Object^  sender, System::EventArgs^  e)
***************************************************************************/		 
System::Void GUI::updateValue_config_AppTab_Button_Click(System::Object^  sender, System::EventArgs^  e) 
{
	StreamWriter^ sw = System::IO::File::AppendText(commandsLogFilePath);
	// YL 4.12 ... added destination
	// was: sw->WriteLine("accmtr config {0} {1} ", value_config_AppTab_NumericUpDown->Text, parameter_config_AppTab_ComboBox->Text);
	sw->WriteLine("{0} accmtr config {1} {2} ", destination_config_AppTab_TextBox->Text, value_config_AppTab_NumericUpDown->Text, parameter_config_AppTab_ComboBox->Text);
	// ... YL 4.12
	try
	{
		// Write the data in the text box to the open serial port
		// YL 4.12 ... added destination
		// was: sendCommands_TextBox->Text = "accmtr config " + value_config_AppTab_NumericUpDown->Text + " " + parameter_config_AppTab_ComboBox->Text;
		sendCommands_TextBox->Text = destination_config_AppTab_TextBox->Text + " accmtr config " + value_config_AppTab_NumericUpDown->Text + " " + parameter_config_AppTab_ComboBox->Text;
		// ... YL 4.12
		buttonSendFlag = true;
	}
	catch(...)
	{
	}
	sw->Close();
}

/****************************************************************************
Function:
private: System::Void shutdown_AppTab_Button_Click(System::Object^  sender, System::EventArgs^  e)
***************************************************************************/			 
System::Void GUI::shutdown_AppTab_Button_Click(System::Object^  sender, System::EventArgs^  e) 
{
	StreamWriter^ sw = System::IO::File::AppendText(commandsLogFilePath);
		
	// YL 4.12 ... added destination
	// was: sw->WriteLine("app shutdown");
	sw->WriteLine("{0} app shutdown", destination_shutdown_AppTab_TextBox->Text);
	// ... YL 4.12 
	sw->Close();
	try
	{
		//Write the data in the text box to the open serial port
		// YL 4.12 ... added destination
		// was: sendCommands_TextBox->Text = "app shutdown";
		sendCommands_TextBox->Text = destination_shutdown_AppTab_TextBox->Text + " app shutdown";
		// ... YL 4.12
		buttonSendFlag = true;
	}
	catch(...)
	{
	}
}	
	
/***B U Z Z E R***/	
	
/****************************************************************************
Function:
private: System::Void apply_writeLL_BuzzerTab_Button_Click(System::Object^  sender, System::EventArgs^  e)
***************************************************************************/			 
System::Void GUI::apply_writeLL_BuzzerTab_Button_Click(System::Object^  sender, System::EventArgs^  e)
{
	StreamWriter^ sw = System::IO::File::AppendText(commandsLogFilePath);
		
	// YL 4.12 added destination
	sw->WriteLine("{0} w buzzer 0 {1}", destination_writeLL_BuzzerTab_TextBox->Text, beepDuration_writeLL_BuzzerTab_NumericUpDown->Text);
	sw->Close();
	try
	{
		// Write the data in the text box to the open serial port
		sendCommands_TextBox->Text = destination_writeLL_BuzzerTab_TextBox->Text + " w buzzer 0 "+ beepDuration_writeLL_BuzzerTab_NumericUpDown->Text;
		buttonSendFlag = true;
	}
	catch(...)
	{
	}
}
	
/***E E P R O M***/		
	
/****************************************************************************
Function:
private: System::Void read_readLL_EepromTab_Button_Click(System::Object^  sender, System::EventArgs^  e)
***************************************************************************/		 
System::Void GUI::read_readLL_EepromTab_Button_Click(System::Object^  sender, System::EventArgs^  e) 
{
	/* YL 3.12 the version of button14_Click is used instead of the commented
	StreamWriter^ sw = System::IO::File::AppendText(commandsLogFilePath);
		
	sw->WriteLine("r eeprom {0}", address_readLL_EepromTab_TextBox->Text);
	sw->Close();
	try
	{
		sendCommands_TextBox->Text = "r eeprom " + address_readLL_EepromTab_TextBox->Text;
		buttonSendFlag = true;
	}
	catch(...)
	{
	}
	*/
		
	StreamWriter^ sw = System::IO::File::AppendText(commandsLogFilePath);
	int value;
		
	if (Int32::TryParse(address_readLL_EepromTab_TextBox->Text,value))
	{
		if (value >= 0)
		{
			// YL 4.12 added destination
			sw->WriteLine("{0} r eeprom {1} ", destination_readLL_EepromTab_TextBox->Text, address_readLL_EepromTab_TextBox->Text);
			try
			{
				//Write the data in the text box to the open serial port
				sendCommands_TextBox->Text = destination_readLL_EepromTab_TextBox->Text + " r eeprom "+ address_readLL_EepromTab_TextBox->Text;
				buttonSendFlag = true;
			}
			catch(...)
			{
			}
		}
		else
		{
			address_readLL_EepromTab_TextBox->Text = "Error";
			address_readLL_EepromTab_TextBox->Select();
		}
	}
	else
	{
		address_readLL_EepromTab_TextBox->Text = "Error";
		address_readLL_EepromTab_TextBox->Select();
	}
	sw->Close();		
		
}
	
/****************************************************************************
Function:
private: System::Void write_writeLL_EepromTab_Button_Click(System::Object^  sender, System::EventArgs^  e)
***************************************************************************/		 
System::Void GUI::write_writeLL_EepromTab_Button_Click(System::Object^  sender, System::EventArgs^  e) 
{
	/* YL 3.12 the version of button15_Click is used instead of the commented
	StreamWriter^ sw = System::IO::File::AppendText(commandsLogFilePath);
		
	sw->WriteLine("w eeprom {0} {1}", address_writeLL_EepromTab_TextBox->Text, data_writeLL_EepromTab_TextBox->Text);
	sw->Close();
	try
	{
		sendCommands_TextBox->Text = "w eeprom " + address_writeLL_EepromTab_TextBox->Text + " " + data_writeLL_EepromTab_TextBox->Text;
		buttonSendFlag = true;
	}
	catch(...)
	{
	}
	*/
		
	StreamWriter^ sw = System::IO::File::AppendText(commandsLogFilePath);
	int value;
		
	if (Int32::TryParse(address_writeLL_EepromTab_TextBox->Text,value))
	{
		if(value >= 0)
		{
			// YL 4.12 added destination
			sw->WriteLine("{0} w eeprom {1} {2} ", destination_writeLL_EepromTab_TextBox->Text, address_writeLL_EepromTab_TextBox->Text,data_writeLL_EepromTab_TextBox->Text);
			try
			{
				// Write the data in the text box to the open serial port
				sendCommands_TextBox->Text = destination_writeLL_EepromTab_TextBox->Text + " w eeprom "+ address_writeLL_EepromTab_TextBox->Text +" " + data_writeLL_EepromTab_TextBox->Text;
				buttonSendFlag = true;
			}
			catch(...)
			{
			}
		}
		else
		{
			address_writeLL_EepromTab_TextBox->Text = "Error";
			address_writeLL_EepromTab_TextBox->Select();
		}
	}
	else
	{
		address_writeLL_EepromTab_TextBox->Text = "Error";
		address_writeLL_EepromTab_TextBox->Select();
	}
	sw->Close();		
		
}
			
/****************************************************************************
Function:
private: System::Void format_EepromTab_Button_Click(System::Object^  sender, System::EventArgs^  e)
***************************************************************************/			 
System::Void GUI::format_EepromTab_Button_Click(System::Object^  sender, System::EventArgs^  e) 
{
	StreamWriter^ sw = System::IO::File::AppendText(commandsLogFilePath);
		
	// YL 4.12 added destination
	sw->WriteLine("{0} eeprom format", destination_format_EepromTab_TextBox->Text);
	sw->Close();
	try
	{
		// Write the data in the text box to the open serial port
		sendCommands_TextBox->Text = destination_format_EepromTab_TextBox->Text + " eeprom format " ;
		buttonSendFlag = true;
	}
	catch(...)
	{
	}
}
	
/****************************************************************************
Function:
private: System::Void read_EepromTab_Button_Click(System::Object^  sender, System::EventArgs^  e)
***************************************************************************/	
System::Void GUI::read_EepromTab_Button_Click(System::Object^  sender, System::EventArgs^  e) 
{
	StreamWriter^ sw = System::IO::File::AppendText(commandsLogFilePath);
	int value;
		
	if (Int32::TryParse(address_read_EepromTab_TextBox->Text, value))
	{
		if (value >= 0 && value % EEPROM_PAGE_SIZE == 0)
		{
			// YL 4.12 added destination
			sw->WriteLine("{0} eeprom read {1} {2} ", destination_read_EepromTab_TextBox->Text, address_read_EepromTab_TextBox->Text, numberOfBytes_read_EepromTab_TextBox->Text);
			try
			{
				// Write the data in the text box to the open serial port
				sendCommands_TextBox->Text = destination_read_EepromTab_TextBox->Text + " eeprom read " + address_read_EepromTab_TextBox->Text + " " + numberOfBytes_read_EepromTab_TextBox->Text;
				buttonSendFlag = true;
			}
			catch(...)
			{
			}
		}
		else
		{
			address_read_EepromTab_TextBox->Text = "Error";
			address_read_EepromTab_TextBox->Select();
		}
	}
	else
	{
		address_read_EepromTab_TextBox->Text = "Error";
		address_read_EepromTab_TextBox->Select();
	}
	sw->Close();
}
	
/****************************************************************************
Function:
private: System::Void write_EepromTab_Button_Click(System::Object^  sender, System::EventArgs^  e)
***************************************************************************/		 
System::Void GUI::write_EepromTab_Button_Click(System::Object^  sender, System::EventArgs^  e) 
{
	StreamWriter^ sw = System::IO::File::AppendText(commandsLogFilePath);
	int value;
		
	if (Int32::TryParse(address_write_EepromTab_TextBox->Text, value))
	{
		if (value >= 0 && value % EEPROM_PAGE_SIZE == 0)
		{
			// YL 4.12 added destination
			sw->WriteLine("{0} eeprom write {1} {2}", destination_write_EepromTab_TextBox->Text, address_write_EepromTab_TextBox->Text, data_write_EepromTab_TextBox->Text);
			try
			{
				// Write the data in the text box to the open serial port
				sendCommands_TextBox->Text = destination_write_EepromTab_TextBox->Text + " eeprom write " + address_write_EepromTab_TextBox->Text + " " + data_write_EepromTab_TextBox->Text;
				buttonSendFlag = true;
			}
			catch(...)
			{
			}
		}
		else
		{
			address_write_EepromTab_TextBox->Text = "Error";
			address_write_EepromTab_TextBox->Select();
		}
	}
	else
	{
		address_write_EepromTab_TextBox->Text = "Error";
		address_write_EepromTab_TextBox->Select();
	}
	sw->Close();
}
	
/****************************************************************************
Function:
private: System::Void apply_getBoot_EepromTab_Button_Click(System::Object^  sender, System::EventArgs^  e)
***************************************************************************/		 
System::Void GUI::apply_getBoot_EepromTab_Button_Click(System::Object^  sender, System::EventArgs^  e) 
{
	StreamWriter^ sw = System::IO::File::AppendText(commandsLogFilePath);
		
	// YL 4.12 added destination
	sw->WriteLine("{0} eeprom gboot {1} ", destination_getBoot_EepromTab_TextBox->Text, entryNumber_getBoot_EepromTab_ComboBox->Text);
	// Write the data in the text box to the open serial port
	sendCommands_TextBox->Text = destination_getBoot_EepromTab_TextBox->Text + " eeprom gboot " + entryNumber_getBoot_EepromTab_ComboBox->Text;
	buttonSendFlag = true;
	sw->Close();
}
	
/****************************************************************************
Function:
private: System::Void apply_setBoot_EepromTab_Button_Click(System::Object^  sender, System::EventArgs^  e)
***************************************************************************/			 
System::Void GUI::apply_setBoot_EepromTab_Button_Click(System::Object^  sender, System::EventArgs^  e) 
{
	StreamWriter^ sw = System::IO::File::AppendText(commandsLogFilePath);
		
	// YL 4.12 added destination
	sw->WriteLine("{0} eeprom sboot {1} ", destination_setBoot_EepromTab_TextBox->Text, entryNumber_setBoot_EepromTab_ComboBox->Text, command_setBoot_EepromTab_TextBox->Text);
	// Write the data in the text box to the open serial port
	sendCommands_TextBox->Text = destination_setBoot_EepromTab_TextBox->Text + " eeprom sboot " + entryNumber_setBoot_EepromTab_ComboBox->Text + " " + command_setBoot_EepromTab_TextBox->Text;
	buttonSendFlag = true;
	sw->Close();
}	
	
/***F L A S H***/	
	
/****************************************************************************
Function:
private: System::Void format_FlashTab_Button_Click(System::Object^  sender, System::EventArgs^  e)
***************************************************************************/
System::Void GUI::format_FlashTab_Button_Click(System::Object^  sender, System::EventArgs^  e) 
{
	StreamWriter^ sw = System::IO::File::AppendText(commandsLogFilePath);
		
	// YL 4.12 added destination
	sw->WriteLine("{0} flash format ", destination_format_FlashTab_TextBox->Text);
	sw->Close();
	try
	{
		// Write the data in the text box to the open serial port
		sendCommands_TextBox->Text = destination_format_FlashTab_TextBox->Text + " flash format " ;
		buttonSendFlag = true;
	}
	catch(...)
	{
	}
}
	
/****************************************************************************
Function:
private: System::Void initialize_FlashTab_Button_Click(System::Object^  sender, System::EventArgs^  e)
***************************************************************************/			 
System::Void GUI::initialize_FlashTab_Button_Click(System::Object^  sender, System::EventArgs^  e) 
{
	StreamWriter^ sw = System::IO::File::AppendText(commandsLogFilePath);
		
	// YL 4.12 added destination
	sw->WriteLine("{0} flash minit ", destination_initialize_FlashTab_TextBox->Text);
	sw->Close();
	try
	{
		// Write the data in the text box to the open serial port
		sendCommands_TextBox->Text = destination_initialize_FlashTab_TextBox->Text + " flash minit " ;
		buttonSendFlag = true;
	}
	catch(...)
	{
	} 
}
	
/****************************************************************************
Function:
private: System::Void cardDetect_FlashTab_Button_Click(System::Object^  sender, System::EventArgs^  e)
***************************************************************************/				 
System::Void GUI::cardDetect_FlashTab_Button_Click(System::Object^  sender, System::EventArgs^  e) 
{
	StreamWriter^ sw = System::IO::File::AppendText(commandsLogFilePath);
		
	// YL 4.12 added destination
	sw->WriteLine("{0} flash gcd ", destination_cardDetect_FlashTab_TextBox->Text);
	sw->Close();
	try
	{
		//Write the data in the text box to the open serial port
		sendCommands_TextBox->Text = destination_cardDetect_FlashTab_TextBox->Text + " flash gcd " ;
		buttonSendFlag = true;
	}
	catch(...)
	{
	} 
}
	
/****************************************************************************
Function:
private: System::Void write_writeSector_FlashTab_Button_Click(System::Object^  sender, System::EventArgs^  e)
***************************************************************************/			 
System::Void GUI::write_writeSector_FlashTab_Button_Click(System::Object^  sender, System::EventArgs^  e) 
{
	StreamWriter^ sw = System::IO::File::AppendText(commandsLogFilePath);
	int value;
		
	if (Int32::TryParse(address_writeSector_FlashTab_TextBox->Text,value))
	{
		if (value >= 0)
		{
			// YL 4.12 added destination
			sw->WriteLine("{0} flash wsector {1} {2}", destination_writeSector_FlashTab_TextBox->Text, address_writeSector_FlashTab_TextBox->Text, data_writeSector_FlashTab_TextBox->Text);
			try
			{
				//Write the data in the text box to the open serial port
				sendCommands_TextBox->Text = destination_writeSector_FlashTab_TextBox->Text + " flash wsector " + address_writeSector_FlashTab_TextBox->Text + " " + data_writeSector_FlashTab_TextBox->Text ;
				buttonSendFlag = true;
			}
			catch(...)
			{
			}
		}
		else
		{
			address_writeSector_FlashTab_TextBox->Text = "Error";
			address_writeSector_FlashTab_TextBox->Select();
		}
	}
	else
	{
		address_writeSector_FlashTab_TextBox->Text = "Error";
		address_writeSector_FlashTab_TextBox->Select();
	}
	sw->Close();
}
	
/****************************************************************************
Function:
private: System::Void read_readSector_FlashTab_Button_Click(System::Object^  sender, System::EventArgs^  e)
***************************************************************************/			 
System::Void GUI::read_readSector_FlashTab_Button_Click(System::Object^  sender, System::EventArgs^  e) 
{
	StreamWriter^ sw = System::IO::File::AppendText(commandsLogFilePath);
	int value;
		
	if (Int32::TryParse(address_readSector_FlashTab_TextBox->Text,value))
	{
		if (value >= 0)
		{
			// YL 4.12 added destination
			sw->WriteLine("{0} flash rsector {1} ", destination_readSector_FlashTab_TextBox->Text, address_readSector_FlashTab_TextBox->Text);
			sw->Close();
			try
			{
				//Write the data in the text box to the open serial port
				sendCommands_TextBox->Text = destination_readSector_FlashTab_TextBox->Text + " flash rsector " + address_readSector_FlashTab_TextBox->Text;
				buttonSendFlag = true;
			}
			catch(...)
			{
			}
		}
		else
		{
			address_readSector_FlashTab_TextBox->Text = "Error";
			address_readSector_FlashTab_TextBox->Select();
		}
	}
	else
	{
		address_readSector_FlashTab_TextBox->Text = "Error";
		address_readSector_FlashTab_TextBox->Select();
	}
	sw->Close();
}
	
/****************************************************************************
Function:
private: System::Void getCapacity_FlashTab_Button_Click(System::Object^  sender, System::EventArgs^  e)
***************************************************************************/			 
System::Void GUI::getCapacity_FlashTab_Button_Click(System::Object^  sender, System::EventArgs^  e) 
{
	StreamWriter^ sw = System::IO::File::AppendText(commandsLogFilePath);
		
	// YL 4.12 added destination
	sw->WriteLine("{0} flash gcap ", destination_getCapacity_FlashTab_TextBox->Text);
	sw->Close();
	try
	{
		// Write the data in the text box to the open serial port
		sendCommands_TextBox->Text = destination_getCapacity_FlashTab_TextBox->Text + " flash gcap " ;
		buttonSendFlag = true;
	}
	catch(...)
	{
	} 
}

/****************************************************************************
Function:
private: System::Void read_readLL_FlashTab_Button_Click(System::Object^  sender, System::EventArgs^  e)
***************************************************************************/
System::Void GUI::read_readLL_FlashTab_Button_Click(System::Object^  sender, System::EventArgs^  e) 
{
	StreamWriter^ sw = System::IO::File::AppendText(commandsLogFilePath);
	int value;
		
	if (Int32::TryParse(address_readLL_FlashTab_TextBox->Text,value))
	{
		if (value >= 0)
		{
			// YL 4.12 added destination
			sw->WriteLine("{0} r flash {1} ", destination_readLL_FlashTab_TextBox->Text, address_readLL_FlashTab_TextBox->Text);
			try
			{
				//Write the data in the text box to the open serial port
				sendCommands_TextBox->Text = destination_readLL_FlashTab_TextBox->Text + " r flash " + address_readLL_FlashTab_TextBox->Text;
				buttonSendFlag = true;
			}
			catch(...)
			{
			}
		}
		else
		{
			address_readLL_FlashTab_TextBox->Text = "Error";
			address_readLL_FlashTab_TextBox->Select();
		}
	}
	else
	{
		address_readLL_FlashTab_TextBox->Text = "Error";
		address_readLL_FlashTab_TextBox->Select();
	}
	sw->Close();
}

/****************************************************************************
Function:
private: System::Void write_writeLL_FlashTab_Button_Click(System::Object^  sender, System::EventArgs^  e)
***************************************************************************/			 
System::Void GUI::write_writeLL_FlashTab_Button_Click(System::Object^  sender, System::EventArgs^  e) 
{				 
	StreamWriter^ sw = System::IO::File::AppendText(commandsLogFilePath);
	int value;
		
	if (Int32::TryParse(address_writeLL_FlashTab_TextBox->Text, value))
	{
		if (value >= 0)
		{
			// YL 4.12 added destination
			sw->WriteLine("{0} w flash {1} {2} ", destination_writeLL_FlashTab_TextBox->Text, address_writeLL_FlashTab_TextBox->Text, data_writeLL_FlashTab_TextBox->Text);
			try
			{
				// Write the data in the text box to the open serial port
				sendCommands_TextBox->Text = destination_writeLL_FlashTab_TextBox->Text + " w flash " + address_writeLL_FlashTab_TextBox->Text + " " + data_writeLL_FlashTab_TextBox->Text ;
				buttonSendFlag = true;
			}
			catch(...)
			{
			}
		}
		else
		{
			address_writeLL_FlashTab_TextBox->Text = "Error";
			address_writeLL_FlashTab_TextBox->Select();
		}
	}
	else
	{
		address_writeLL_FlashTab_TextBox->Text = "Error";
		address_writeLL_FlashTab_TextBox->Select();
	}
	sw->Close();
}	

/***L C D***/	
	
/****************************************************************************
Function:
private: System::Void clear_clearScreen_LcdTab_Button_Click(System::Object^  sender, System::EventArgs^  e)
***************************************************************************/			 
System::Void GUI::clear_clearScreen_LcdTab_Button_Click(System::Object^  sender, System::EventArgs^  e) 
{
	StreamWriter^ sw = System::IO::File::AppendText(commandsLogFilePath);
		
	// YL 4.12 added destination
	sw->WriteLine("{0} lcd clrscr", destination_clearScreen_LcdTab_TextBox->Text);
	sw->Close();
	lcd_clearScreen_LcdTab_TextBox->Clear();
	try
	{
		// Write the data in the text box to the open serial port
		sendCommands_TextBox->Text = destination_clearScreen_LcdTab_TextBox->Text + " lcd clrscr";
		buttonSendFlag = true;
	}
	catch(...)
	{
	}
}	
	
/****************************************************************************
Function:
private: System::Void write_writeLL_LcdTab_Button_Click(System::Object^  sender, System::EventArgs^  e)
***************************************************************************/			 
System::Void GUI::write_writeLL_LcdTab_Button_Click(System::Object^  sender, System::EventArgs^  e) 
{
	StreamWriter^ sw = System::IO::File::AppendText(commandsLogFilePath);
	int value;
		
	if (Int32::TryParse(address_writeLL_LcdTab_TextBox->Text,value))
	{
		if (value >= 0)
		{
			// YL 4.12 added destination
			sw->WriteLine("{0} w lcd {1} {2}", destination_writeLL_LcdTab_TextBox->Text, address_writeLL_LcdTab_TextBox->Text, data_writeLL_LcdTab_TextBox->Text);
			lcd_clearScreen_LcdTab_TextBox->Text = data_writeLL_LcdTab_TextBox->Text;
			try
			{
				// Write the data in the text box to the open serial port
				sendCommands_TextBox->Text = destination_writeLL_LcdTab_TextBox->Text + " w lcd "+ address_writeLL_LcdTab_TextBox->Text + " " + data_writeLL_LcdTab_TextBox->Text;
				buttonSendFlag = true;
			}
			catch(...)
			{
			}
		}
		else
		{
			address_writeLL_LcdTab_TextBox->Text = "Error";
			address_writeLL_LcdTab_TextBox->Select();
		}
	}
	else
	{
		address_writeLL_LcdTab_TextBox->Text = "Error";
		address_writeLL_LcdTab_TextBox->Select();
	}
	sw->Close();
}	
	
/***L E D***/
	
/****************************************************************************
Function:
private: System::Void ledOn_setLed_LedTab_RadioButton_Click(System::Object^  sender, System::EventArgs^  e)
***************************************************************************/			 
System::Void GUI::ledOn_setLed_LedTab_RadioButton_Click(System::Object^  sender, System::EventArgs^  e) 
{
	ledOn = true;
}
	
/****************************************************************************
Function:
private: System::Void ledOff_setLed_LedTab_RadioButton_Click(System::Object^  sender, System::EventArgs^  e)
***************************************************************************/			 
System::Void GUI::ledOff_setLed_LedTab_RadioButton_Click(System::Object^  sender, System::EventArgs^  e) 
{
	ledOn = false;
}
	
/****************************************************************************
Function:
private: System::Void apply_setLed_LedTab_Button_Click(System::Object^  sender, System::EventArgs^  e)
***************************************************************************/			 
System::Void GUI::apply_setLed_LedTab_Button_Click(System::Object^  sender, System::EventArgs^  e) 
{
	StreamWriter^ sw = System::IO::File::AppendText(commandsLogFilePath);
	
	int ledOnValue = ((ledOn == true)? 1: 0); // YL 23.6 added conversion to int for correct command format

	// YL 4.12 added destination
	sw->WriteLine("{0} w led {1} {2}", destination_setLed_LedTab_TextBox->Text, address_setLed_LedTab_ComboBox->Text, ledOnValue);
	sw->Close();
	try
	{
		sendCommands_TextBox->Text = destination_setLed_LedTab_TextBox->Text + " w led " + address_setLed_LedTab_ComboBox->Text + " " + ledOnValue;
		buttonSendFlag = true;
	}
	catch(...)
	{
	}
}		
	
/***S W I T C H***/		
	
/****************************************************************************
Function:
private: System::Void apply_readLL_SwitchTab_Button_Click(System::Object^  sender, System::EventArgs^  e)
***************************************************************************/
System::Void GUI::apply_readLL_SwitchTab_Button_Click(System::Object^  sender, System::EventArgs^  e) 
{
	StreamWriter^ sw = System::IO::File::AppendText(commandsLogFilePath);
		
	// YL 4.12 added destination
	sw->WriteLine("{0} r switch {1} {2}", destination_readLL_SwitchTab_TextBox->Text, address_readLL_SwitchTab_ComboBox->Text, switchId_readLL_SwitchTab_ComboBox->Text);
	sw->Close();
	try
	{
		//Write the data in the text box to the open serial port
		sendCommands_TextBox->Text = destination_readLL_SwitchTab_TextBox->Text + " r switch " + address_readLL_SwitchTab_ComboBox->Text + " " + switchId_readLL_SwitchTab_ComboBox->Text;
		buttonSendFlag = true;
	}
	catch(...)
	{
	}
}	
	
/***S Y S T E M***/	

/****************************************************************************
Function:
private: System::Void systemGetVersion_SystemTab_Button_Click(System::Object^  sender, System::EventArgs^  e)
***************************************************************************/			 
System::Void GUI::systemGetVersion_SystemTab_Button_Click(System::Object^  sender, System::EventArgs^  e) 
{
	StreamWriter^ sw = System::IO::File::AppendText(commandsLogFilePath);
		
	// YL 4.12 added destination
	sw->WriteLine("{0} sys gver ", destination_getSystemVersion_SystemTab_TextBox->Text);
	sw->Close();
	try
	{
		// Write the data in the text box to the open serial port
		sendCommands_TextBox->Text = destination_getSystemVersion_SystemTab_TextBox->Text + " sys gver ";
		buttonSendFlag = true;
	}
	catch(...)
	{
	}
}
	
/****************************************************************************
Function:
private: System::Void systemSetId_SystemTab_Button_Click(System::Object^  sender, System::EventArgs^  e)
***************************************************************************/			 
System::Void GUI::systemSetId_SystemTab_Button_Click(System::Object^  sender, System::EventArgs^  e) 
{
	StreamWriter^ sw = System::IO::File::AppendText(commandsLogFilePath);
	int value;
		
	if (Int32::TryParse(systemId_systemSetId_SystemTab_TextBox->Text,value))
	{
		if (value >= 0 && value <= MAX_SN)
		{	
			// YL 4.12 added destination
			sw->WriteLine("{0} sys sid {1}", destination_setSystemId_SystemTab_TextBox->Text ,systemId_systemSetId_SystemTab_TextBox->Text);
			try
			{
				// Write the data in the text box to the open serial port
				sendCommands_TextBox->Text = destination_setSystemId_SystemTab_TextBox->Text + " sys sid " + systemId_systemSetId_SystemTab_TextBox->Text;
				buttonSendFlag = true;
			}
			catch(...)
			{
			}
		}
		else
		{
			systemId_systemSetId_SystemTab_TextBox->Text = "Error";
			systemId_systemSetId_SystemTab_TextBox->Select();
		}
	}

	else
	{
		systemId_systemSetId_SystemTab_TextBox->Text = "Error";
		systemId_systemSetId_SystemTab_TextBox->Select();
	}        
	sw->Close();
}

/****************************************************************************
Function:
private: System::Void getPower_SystemTab_Button_Click(System::Object^  sender, System::EventArgs^  e)
***************************************************************************/			 
System::Void GUI::getPower_SystemTab_Button_Click(System::Object^  sender, System::EventArgs^  e) 
{		
	StreamWriter^ sw = System::IO::File::AppendText(commandsLogFilePath);
		
	// YL 4.12 added destination
	sw->WriteLine("{0} sys gpower ", destination_getPower_SystemTab_TextBox->Text);
	sw->Close();
	try
	{
		// Write the data in the text box to the open serial port
		sendCommands_TextBox->Text = destination_getPower_SystemTab_TextBox->Text + " sys gpower " ;
		buttonSendFlag = true;
	}
	catch(...)
	{
	}
}

/****************************************************************************
Function:
private: System::Void getLastError_SystemTab_Button_Click(System::Object^  sender, System::EventArgs^  e)
***************************************************************************/			 
System::Void GUI::getLastError_SystemTab_Button_Click(System::Object^  sender, System::EventArgs^  e) 
{
	StreamWriter^ sw = System::IO::File::AppendText(commandsLogFilePath);
		
	// YL 4.12 added destination
	sw->WriteLine("{0} sys gerr ", destination_getLastError_SystemTab_TextBox->Text);
	sw->Close();
	try
	{
		// Write the data in the text box to the open serial port
		sendCommands_TextBox->Text = destination_getLastError_SystemTab_TextBox->Text + " sys gerr " ;
		buttonSendFlag = true;
	}
	catch(...)
	{
	}
}
	
/****************************************************************************
Function:
private: System::Void setPower_SystemTab_Button_Click(System::Object^  sender, System::EventArgs^  e)
***************************************************************************/			 
System::Void GUI::setPower_SystemTab_Button_Click(System::Object^  sender, System::EventArgs^  e) 
{
	StreamWriter^ sw = System::IO::File::AppendText(commandsLogFilePath);
		
	// YL 4.12 added destination
	sw->WriteLine("{0} sys spower {1} {2} {3}", destination_setPower_SystemTab_TextBox->Text, enable5V_setPower_SystemTab_ComboBox->Text, enable12V_setPower_SystemTab_ComboBox->Text, enableM12V_setPower_SystemTab_ComboBox->Text);
	sw->Close();
	try
	{
		// Write the data in the text box to the open serial port
		sendCommands_TextBox->Text = destination_setPower_SystemTab_TextBox->Text + " sys spower "+ enable5V_setPower_SystemTab_ComboBox->Text + " " + enable12V_setPower_SystemTab_ComboBox->Text + " " + enableM12V_setPower_SystemTab_ComboBox->Text;
		buttonSendFlag = true;
	}
	catch(...)
	{
	}
}
	
/***T E M P***/

/****************************************************************************
Function:
private: System::Void getTemperature_TempTab_Button_Click(System::Object^  sender, System::EventArgs^  e) 
***************************************************************************/			 
System::Void GUI::getTemperature_TempTab_Button_Click(System::Object^  sender, System::EventArgs^  e) 
{
	StreamWriter^ sw = System::IO::File::AppendText(commandsLogFilePath);

	// YL 4.12 added destination
	sw->WriteLine("{0} temp gtemp", destination_getTemperature_TemperatureTab_TextBox->Text);
	sw->Close();
	try
	{
		// Write the data in the text box to the open serial port
		sendCommands_TextBox->Text = destination_getTemperature_TemperatureTab_TextBox->Text + " temp gtemp ";
		buttonSendFlag = true;
	}
	catch(...)
	{
	}
}
	
/****************************************************************************
Function:
private: System::Void read_readLL_TempTab_Button_Click(System::Object^  sender, System::EventArgs^  e) 
***************************************************************************/
System::Void GUI::read_readLL_TempTab_Button_Click(System::Object^  sender, System::EventArgs^  e) 
{
	StreamWriter^ sw = System::IO::File::AppendText(commandsLogFilePath);
	int value;
		
	if (Int32::TryParse(address_readLL_TempTab_TextBox->Text,value))
	{
		if (value >= 0 )
		{
			// YL 4.12 added destination
			sw->WriteLine("{0} r temp {1} ", destination_readLL_TemperatureTab_TextBox->Text,  address_readLL_TempTab_TextBox->Text);
			try
			{
				//Write the data in the text box to the open serial port
				sendCommands_TextBox->Text = destination_readLL_TemperatureTab_TextBox->Text + " r temp " + address_readLL_TempTab_TextBox->Text;
				buttonSendFlag = true;
			}
			catch(...)
			{
			}
		}
		else
		{
			address_readLL_TempTab_TextBox->Text = "Error";
			address_readLL_TempTab_TextBox->Select();
		}
	}
	else
	{
		address_readLL_TempTab_TextBox->Text = "Error";
		address_readLL_TempTab_TextBox->Select();
	}
	sw->Close();
}
	
/****************************************************************************
Function:
private: System::Void write_writeLL_TempTab_Button_Click(System::Object^  sender, System::EventArgs^  e) 
***************************************************************************/			 
System::Void GUI::write_writeLL_TempTab_Button_Click(System::Object^  sender, System::EventArgs^  e) 
{
	StreamWriter^ sw = System::IO::File::AppendText(commandsLogFilePath);
	int value;
		
	if (Int32::TryParse(address_writeLL_TempTab_TextBox->Text,value))
	{
		if (value >= 0)
		{
			// YL 4.12 added destination
			sw->WriteLine("{0} w temp {1} {2} ", destination_writeLL_TemperatureTab_TextBox->Text, address_writeLL_TempTab_TextBox->Text,data_writeLL_TempTab_TextBox->Text);
			try
			{
				// Write the data in the text box to the open serial port
				sendCommands_TextBox->Text = destination_writeLL_TemperatureTab_TextBox->Text + " w temp " + address_writeLL_TempTab_TextBox->Text + " " + data_writeLL_TempTab_TextBox->Text;
				buttonSendFlag = true;
			}
			catch(...)
			{
			}
		}
		else
		{
			address_writeLL_TempTab_TextBox->Text = "Error";
			address_writeLL_TempTab_TextBox->Select();
		}
	}
	else
	{
		address_writeLL_TempTab_TextBox->Text = "Error";
		address_writeLL_TempTab_TextBox->Select();
	}
	sw->Close();
}	
	
/***W A K E U P***/	

/****************************************************************************
Function:
private: System::Void getTime_WakeUpTab_Button_Click(System::Object^  sender, System::EventArgs^  e)
***************************************************************************/
System::Void GUI::getTime_WakeUpTab_Button_Click(System::Object^  sender, System::EventArgs^  e) 
{
	StreamWriter^ sw = System::IO::File::AppendText(commandsLogFilePath);
		
	// YL 4.12 added destination
	sw->WriteLine("{0} rtc gtime", destination_getTime_WakeUpTab_TextBox->Text);
	sw->Close();
	try
	{
		// Write the data in the text box to the open serial port
		sendCommands_TextBox->Text = destination_getTime_WakeUpTab_TextBox->Text + " rtc gtime";
		buttonSendFlag = true;
	}
	catch(...)
	{
	}
}
	
/****************************************************************************
Function:
private: System::Void setTime_WakeUpTab_Button_Click(System::Object^  sender, System::EventArgs^  e)
***************************************************************************/			 
System::Void GUI::setTime_WakeUpTab_Button_Click(System::Object^  sender, System::EventArgs^  e) 
{
	StreamWriter^ sw = System::IO::File::AppendText(commandsLogFilePath);
		
	// YL 4.12 added destination
	sw->WriteLine("{0} rtc stime {1} {2} {3}", destination_setTime_WakeUpTab_TextBox->Text, hour_setTime_WakeUpTab_NumericUpDown->Text, minute_setTime_WakeUpTab_NumericUpDown->Text, second_setTime_WakeUpTab_NumericUpDown->Text);
	sw->Close();
	try
	{
		// Write the data in the text box to the open serial port
		sendCommands_TextBox->Text = destination_setTime_WakeUpTab_TextBox->Text + " rtc stime " + hour_setTime_WakeUpTab_NumericUpDown->Text + " " + minute_setTime_WakeUpTab_NumericUpDown->Text + " " + second_setTime_WakeUpTab_NumericUpDown->Text;
		buttonSendFlag = true;
	}
	catch(...)
	{
	}
}
	
/****************************************************************************
Function:
private: System::Void setDate_WakeUpTab_Button_Click(System::Object^  sender, System::EventArgs^  e)
***************************************************************************/			 
System::Void GUI::setDate_WakeUpTab_Button_Click(System::Object^  sender, System::EventArgs^  e) 
{
	StreamWriter^ sw = System::IO::File::AppendText(commandsLogFilePath);
		
	// YL 4.12 added destination
	sw->WriteLine("{0} rtc sdate {1} {2} {3} {4}", destination_setDate_WakeUpTab_TextBox->Text, day_setDate_WakeUpTab_NumericUpDown->Text, month_setDate_WakeUpTab_NumericUpDown->Text, year_setDate_WakeUpTab_NumericUpDown->Text,weekDay_setDate_WakeUpTab_NumericUpDown->Text);
	sw->Close();
	try
	{
		// Write the data in the text box to the open serial port
		sendCommands_TextBox->Text = destination_setDate_WakeUpTab_TextBox->Text + " rtc sdate " + day_setDate_WakeUpTab_NumericUpDown->Text +" " + month_setDate_WakeUpTab_NumericUpDown->Text+ " " + year_setDate_WakeUpTab_NumericUpDown->Text + " " + weekDay_setDate_WakeUpTab_NumericUpDown->Text;
		buttonSendFlag = true;
	}
	catch(...)
	{
	}
}
	
/****************************************************************************
Function:
private: System::Void getDate_WakeUpTab_Button_Click(System::Object^  sender, System::EventArgs^  e)
***************************************************************************/			 
System::Void GUI::getDate_WakeUpTab_Button_Click(System::Object^  sender, System::EventArgs^  e) 
{
	StreamWriter^ sw = System::IO::File::AppendText(commandsLogFilePath);
		
	// YL 4.12 added destination
	sw->WriteLine("{0} rtc gdate", destination_getDate_WakeUpTab_TextBox->Text);
	sw->Close();
	try
	{
		// Write the data in the text box to the open serial port
		sendCommands_TextBox->Text = destination_getDate_WakeUpTab_TextBox->Text + " rtc gdate";
		buttonSendFlag = true;
	}
	catch(...)
	{
	}
}
	
/****************************************************************************
Function:
private: System::Void read_readLL_WakeUpTab_Button_Click(System::Object^  sender, System::EventArgs^  e) 
***************************************************************************/			 
System::Void GUI::read_readLL_WakeUpTab_Button_Click(System::Object^  sender, System::EventArgs^  e) 
{
	StreamWriter^ sw = System::IO::File::AppendText(commandsLogFilePath);
	int value;
		
	if (Int32::TryParse(address_readLL_WakeUpTab_TextBox->Text,value))
	{
		if (value >= 0)
		{
			// YL 4.12 added destination
			sw->WriteLine("{0} r rtc {1} ", destination_readLL_WakeUpTab_TextBox->Text, address_readLL_WakeUpTab_TextBox->Text);
			try
			{
				// Write the data in the text box to the open serial port
				sendCommands_TextBox->Text = destination_readLL_WakeUpTab_TextBox->Text + " r rtc "+ address_readLL_WakeUpTab_TextBox->Text;
				buttonSendFlag = true;
			}
			catch(...)
			{
			}
		}
		else
		{
			address_readLL_WakeUpTab_TextBox->Text = "Error";
			address_readLL_WakeUpTab_TextBox->Select();
		}
	}
	else
	{
		address_readLL_WakeUpTab_TextBox->Text = "Error";
		address_readLL_WakeUpTab_TextBox->Select();
	}
	sw->Close();
}
	
/****************************************************************************
Function:
private: System::Void write_writeLL_WakeUpTab_Button_Click(System::Object^  sender, System::EventArgs^  e) 
***************************************************************************/			 
System::Void GUI::write_writeLL_WakeUpTab_Button_Click(System::Object^  sender, System::EventArgs^  e) 
{
	StreamWriter^ sw = System::IO::File::AppendText(commandsLogFilePath);
	int value;
		
	if (Int32::TryParse(address_writeLL_WakeUpTab_TextBox->Text,value))
	{
		if (value >= 0)
		{
			// YL 4.12 added destination
			sw->WriteLine("{0} w rtc {1} {2}", destination_writeLL_WakeUpTab_TextBox->Text, address_writeLL_WakeUpTab_TextBox->Text, data_writeLL_WakeUpTab_TextBox->Text);
			try
			{
				// Write the data in the text box to the open serial port
				sendCommands_TextBox->Text = destination_writeLL_WakeUpTab_TextBox->Text + " w rtc " + address_writeLL_WakeUpTab_TextBox->Text +" " + data_writeLL_WakeUpTab_TextBox->Text;
				buttonSendFlag = true;
			}
			catch(...)
			{
			}
		}
		else
		{
			address_writeLL_WakeUpTab_TextBox->Text = "Error";
			address_writeLL_WakeUpTab_TextBox->Select();
		}
	}
	else
	{
		address_writeLL_WakeUpTab_TextBox->Text = "Error";
		address_writeLL_WakeUpTab_TextBox->Select();
	}
	sw->Close();
}	
	
/****************************************************************************
Function:
private: System::Void apply_setAlarm_WakeUpTab_Button_Click(System::Object^  sender, System::EventArgs^  e)
***************************************************************************/			 
System::Void GUI::apply_setAlarm_WakeUpTab_Button_Click(System::Object^  sender, System::EventArgs^  e) 
{
	StreamWriter^ sw = System::IO::File::AppendText(commandsLogFilePath);
	int value;
		
	if (hour_setAlarm_WakeUpTab_TextBox->Text != "x")
	{
		if (Int32::TryParse(hour_setAlarm_WakeUpTab_TextBox->Text,value))
		{
			if (value < MIN_HOUR || value > MAX_HOUR)
			{
				hour_setAlarm_WakeUpTab_TextBox->Text = "Error";
				hour_setAlarm_WakeUpTab_TextBox->Select();
				return;
			}
		}
		else
		{
			hour_setAlarm_WakeUpTab_TextBox->Text = "Error";
			hour_setAlarm_WakeUpTab_TextBox->Select();
			return;
		}
		}
		if (minute_setAlarm_WakeUpTab_TextBox->Text != "x")
		{
		if (Int32::TryParse(minute_setAlarm_WakeUpTab_TextBox->Text,value))
		{
			if (value < MIN_MINUTE || value > MAX_MINUTE)
			{
				minute_setAlarm_WakeUpTab_TextBox->Text = "Error";
				minute_setAlarm_WakeUpTab_TextBox->Select();
				return;
			}
		}
		else
		{
			minute_setAlarm_WakeUpTab_TextBox->Text = "Error";
			minute_setAlarm_WakeUpTab_TextBox->Select();
			return;
		}
	}
	if (day_setAlarm_WakeUpTab_TextBox->Text != "x")
	{
		if (Int32::TryParse(day_setAlarm_WakeUpTab_TextBox->Text,value))
		{
			if (value < MIN_DAY || value> MAX_DAY)
			{
				day_setAlarm_WakeUpTab_TextBox->Text = "Error";
				day_setAlarm_WakeUpTab_TextBox->Select();
				return;
			}
		}
		else
		{
			day_setAlarm_WakeUpTab_TextBox->Text = "Error";
			day_setAlarm_WakeUpTab_TextBox->Select();
			return;
		}
	}
	if (weekDay_setAlarm_WakeUpTab_TextBox->Text != "x")
	{
		if (Int32::TryParse(weekDay_setAlarm_WakeUpTab_TextBox->Text,value))
		{
			if (value < MIN_WEEKDAY || value > MAX_WEEKDAY)
			{
				weekDay_setAlarm_WakeUpTab_TextBox->Text = "Error Try Again!";
				weekDay_setAlarm_WakeUpTab_TextBox->SelectAll();
				return;
			}
		}
		else
		{
			weekDay_setAlarm_WakeUpTab_TextBox->Text = "Error Try Again!";
			weekDay_setAlarm_WakeUpTab_TextBox->SelectAll();
			return;
		}
	}
		// YL 4.12 added destination
		sw->WriteLine("{0} rtc swakeup {1} {2} {3} {4}", destination_setAlarm_WakeUpTab_TextBox->Text, hour_setAlarm_WakeUpTab_TextBox->Text, minute_setAlarm_WakeUpTab_TextBox->Text, day_setAlarm_WakeUpTab_TextBox->Text,weekDay_setAlarm_WakeUpTab_TextBox->Text);
		sw->Close();
	try
	{
		sendCommands_TextBox->Text = destination_setAlarm_WakeUpTab_TextBox->Text + " rtc swakeup " + hour_setAlarm_WakeUpTab_TextBox->Text + " " + minute_setAlarm_WakeUpTab_TextBox->Text + " " + day_setAlarm_WakeUpTab_TextBox->Text + " " + weekDay_setAlarm_WakeUpTab_TextBox->Text;
		buttonSendFlag = true;
	}
	catch(...)
	{
	}
}
		
/****************************************************************************
Function:
private: System::Void apply_setWakeUp_WakeUpTab_Button_Click(System::Object^  sender, System::EventArgs^  e)
***************************************************************************/		 
System::Void GUI::apply_setWakeUp_WakeUpTab_Button_Click(System::Object^  sender, System::EventArgs^  e) 
{
	StreamWriter^ sw = System::IO::File::AppendText(commandsLogFilePath);
		
	// YL 4.12 added destination
	sw->WriteLine("{0} rtc swakeup {1} ", destination_setWakeUp_WakeUpTab_TextBox->Text, delay_setWakeUp_WakeUpTab_TextBox->Text);
	// Write the data in the text box to the open serial port
	sendCommands_TextBox->Text = destination_setWakeUp_WakeUpTab_TextBox->Text + " rtc swakeup " + delay_setWakeUp_WakeUpTab_TextBox->Text;
	buttonSendFlag = true;
	sw->Close();
}
	
/****************************************************************************
Function:
private: System::Void apply_setBoot_WakeUpTab_Button_Click(System::Object^  sender, System::EventArgs^  e)
***************************************************************************/		 
System::Void GUI::apply_setBoot_WakeUpTab_Button_Click(System::Object^  sender, System::EventArgs^  e) 
{
	StreamWriter^ sw = System::IO::File::AppendText(commandsLogFilePath);
		
	// YL 4.12 added destination
	sw->WriteLine("{0} eeprom sboot {1} ", destination_setBoot_WakeUpTab_TextBox->Text, entryNumber_setBoot_WakeUpTab_ComboBox->Text, command_setBoot_WakeUpTab_TextBox->Text);
	// Write the data in the text box to the open serial port
	sendCommands_TextBox->Text = destination_setBoot_WakeUpTab_TextBox->Text + " eeprom sboot " + entryNumber_setBoot_WakeUpTab_ComboBox->Text + " " + command_setBoot_WakeUpTab_TextBox->Text;
	buttonSendFlag = true;
	sw->Close();
}
	
/***U T I L I T I E S***/	

// YL 30.4 added internet_RadioButton_CheckedChanged
/****************************************************************************
Function:
private: System::Void internet_RadioButton_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
***************************************************************************/
System::Void GUI::internet_RadioButton_CheckedChanged(System::Object^  sender, System::EventArgs^  e) 
{
	internetConnection = true;
	comPortConnection = false;
}

// YL 30.4 added comPort_RadioButton_CheckedChanged
/****************************************************************************
Function:
private: System::Void comPort_RadioButton_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
***************************************************************************/
System::Void GUI::comPort_RadioButton_CheckedChanged(System::Object^  sender, System::EventArgs^  e) 
{
	comPortConnection = true;
	internetConnection = false;
}

/****************************************************************************
Function:
private void updateCOMPortList()

Summary:
This function updates the COM ports listbox.

Description:
This function updates the COM ports listbox.  This function is launched 
periodically based on its Interval attribute (set in the form editor under
the properties window).

Precondition:
None

Parameters:
None

Return Values:
None

Remarks:
None
***************************************************************************/
void GUI::updateCOMPortList(void)
{
	int i;
	bool foundDifference;
	cli::array<System::String^, 1>^ portNames; 

	i = 0;
	foundDifference = false;

	// If the number of COM ports is different than the last time we
	// checked, then we know that the COM ports have changed and we
	// don't need to verify each entry.
	portNames = serialPort1->GetPortNames();
	if (comPort_ComboBox->Items->Count == portNames->Length)
	{
		// Search the entire SerialPort object. Look at COM port name
		// returned and see if it already exists in the list.
		for(i = 0; i < portNames->Length; i++)
		{
			if(comPort_ComboBox->Items[i]->Equals(portNames[i]) == false)
			{
				foundDifference = true;
			}
		}
	}
	else
	{
		foundDifference = true;
	}

	// If nothing has changed, exit the function.
	if (foundDifference == false)
	{
		return;
	}

	// If something has changed, then clear the list
	comPort_ComboBox->Items->Clear();

	// Add all of the current COM ports to the list
	try 
	{
		for (i = 0; i < portNames->Length; i++) 
		{
			comPort_ComboBox->Items->Add(portNames[i]);
		}
	}
	catch (...) 
	{
	}

	// Set the listbox to point to the first entry in the list
	comPort_ComboBox->SelectedIndex = 0;
}
	
/****************************************************************************
Function:
private void connect_Button_Click(object sender, EventArgs e)

Summary:
This function opens the COM port.

Description:
This function opens the COM port.  This function is launched when the 
connect_Button_Click button is clicked.  In addition to opening the COM port, 
this function will also change the Enable attribute of several of the form
objects to disable the user from opening a new COM port.

Precondition:
None

Parameters:
object sender     - Sender of the event (this form)
EventArgs e       - The event arguments

Return Values:
None

Remarks:
None
***************************************************************************/
System::Void GUI::connect_Button_Click(System::Object^  sender, System::EventArgs^  e) 
{
	// This section of code will try to open the COM port.
	// Please note that it is important to use a try/catch
	// statement when opening the COM port.  If a USB virtual
	// COM port is removed and the PC software tries to open
	// the COM port before it detects its removal then
	// an exception is thrown.  If the exception is not in a
	// try/catch statement this could result in the application
	// crashing.
	try
	{
		// YL 4.5 ... added; TODO - do we need to check comPort/internet connection when startApplication_AppTab_Button_Click
		if ((comPortConnection == false) && (internetConnection == false)) 
		{
			systemMessage(ERR_MSG, "Must choose a Connection Mode: COM Port or Internet", "connect_Button_Click", "");
			return;
		}
		// ... YL 4.5
		// YL 30.4 added if ...
		assert(!((comPortConnection == true) && (internetConnection == true)));
		if (comPortConnection == true) // YL 27.5 added else if
		{
			// Get the port name from the application list box.
			// the PortName attribute is a string name of the COM
			// port (e.g. - "COM1").
			serialPort1->PortName = comPort_ComboBox->Items[comPort_ComboBox->SelectedIndex]->ToString();
			// Open the COM port.
			serialPort1->Open();
			serialPort1->WriteTimeout = WRITE_TIMEOUT;
		}
		else if (internetConnection == true)
		{
			assert(server != nullptr);
			server->serverConnect();
		}
		// Change the state of the application objects
		// YL 30.4 ... added
		comPort_RadioButton->Enabled = false;
		internet_RadioButton->Enabled = false;
		// ... YL 30.4
		connect_Button->Enabled = false;
		disconnect_Button->Enabled = true;
		comPort_ComboBox->Enabled = false;
		allTabs->Enabled = true; // YL 16.6
		sendCommands_Button->Enabled = true;
		connection_Timer->Enabled = true;
		commands_Timer->Enabled = true;

		if (comPortConnection == true) // YL 27.5 added if
		{
			// Clear the textbox and print that we are connected:
			console_TextBox->Clear();
			console_TextBox->AppendText("Connected.\r\n");

			// Get SW version ID & PSU serial number
			// systemGetVersion_systemGetVersion_SystemTab_Button->PerformClick();
			sendCommands_TextBox->Text = "sys gver ";
			buttonSendFlag = true;
		}
		else if (internetConnection == true) // YL 21.6
		{
			server_Timer->Enabled = true;
		}
	}
		catch(...)
	{
		// If there was an exception, then close the handle to 
		// the device and assume that the device was removed
		// disconnect_Button_Click(this, gcnew EventArgs());
	}
}

/****************************************************************************
Function:
private void disconnect_Button_Click(object sender, EventArgs e)

Summary:
This function closes the COM port.

Description:
This function closes the COM port.  This function is launched when the 
disconnect button is clicked.  This function can also be called directly
from other functions.  In addition to closing the COM port, this 
function will also change the Enable attribute of several of the form
objects to enable the user to open a new COM port.

Precondition:
None

Parameters:
object sender     - Sender of the event (this form)
EventArgs e       - The event arguments

Return Values:
None

Remarks:
None
***************************************************************************/
System::Void GUI::disconnect_Button_Click(System::Object^  sender, System::EventArgs^  e) 
{
	// This section of code will try to close the COM port.
	// Please note that it is important to use a try/catch
	// statement when closing the COM port.  If a USB virtual
	// COM port is removed and the PC software tries to close
	// the COM port before it detects its removal then
	// an exception is thrown.  If the exception is not in a
	// try/catch statement this could result in the application
	// crashing.
	try
	{
		if (comPortConnection == true) // YL 27.5 added else if
		{
			// Dispose the In and Out buffers:
			serialPort1->DiscardInBuffer();
			serialPort1->DiscardOutBuffer();

			// Close the COM port:
			serialPort1->Close();
		}
		else if (internetConnection == true)
		{
			assert(server != nullptr);
			server->serverDisconnect();
		}
		//Reset the state of the application objects:
		// YL 30.4 ... added
		comPort_RadioButton->Enabled = true;
		internet_RadioButton->Enabled = true;
		// ... YL 30.4
		connect_Button->Enabled = true;
		disconnect_Button->Enabled = false;
		comPort_ComboBox->Enabled = true;
		allTabs->Enabled = false; // YL 16.6
		sendCommands_Button->Enabled = false;
		connection_Timer->Enabled = false;
		commands_Timer->Enabled = false;
		server_Timer->Enabled = false; // YL 21.6
		// YL 4.5 ... Reset the connection time:
		secondsDuration = 0;
		minutesDuration = 0;
		hoursDuration = 0;
		// ... YL 4.5
		connectionTime_TextBox->Text = "Disconnected";
	}
	// If there was an exception then there isn't much we can
	// do.  The port is no longer available.
	catch(...)
	{
	}
}

/****************************************************************************
Function:
private void serialPort1_DataReceived(object sender, SerialDataReceivedEventArgs e)
Summary:
This function prints any data received on the COM port.

Description:
This function is called when the data is received on the COM port.  This
function attempts to write that data to the txtDataReceived textbox.  If
an exception occurs the disconnect_Button_Click() function is called in order 
to close the COM port that caused the exception.

Algorithm:
	- read the bytes received from serial port into buffer
	- if (applicationStart==true )
	- append buffer to "CommandsLog.txt" and "SamplesLog.txt"
	- close both files (to flush the pipes)
	- parse incoming stream 
	- break into blocks
	- identify the in-bound debug info, and extract it
	- append data blocks (the 504 samples only) to "dataSamples.txt"
	- update progress:
	- num of received bytes
	- error indications sent within end of block
	- convert non-printed characters to '_', leave printed chars as is
	- display it in GUI
	- look for end of transaction:
	- using the byte counter, and
	- looking for ending string sent from Wistone
	- if end of transaction:
	- save-as the temp.txt
	- applicationStart = false
	- enable send commands
	- else // we are not within applicationStart
	- display the text received into GUI

Precondition:
None

Parameters:
object sender     - Sender of the event (this form)
SerialDataReceivedEventArgs e       - The event arguments

Return Values:
None

Remarks:
None
***************************************************************************/
System::Void GUI::serialPort1_DataReceived(System::Object^  sender, System::IO::Ports::SerialDataReceivedEventArgs^  e) 
{
	int				i;
	String^			header_string = "HEADER-BLOCK:";
	int				header_block_start_ptr;
	array<Byte>^	ready_block;
	array<Byte>^	ready_block_out = gcnew array<Byte>(DATA_BLOCK_SIZE);
	array<Byte>^	full_size_block;
	String^			block_line;
	Encoding^		u16LE = Encoding::Unicode;
	String^			outputFilePath;		// YL 16.6 was: String^ outFile;
	String^			termination_string = "COMPLETED";
	int				termination_string_ptr;
	Int16			x, y, z;

	// The ReadExisting() function will read all of the data that
	// is currently available in the COM port buffer.  In this 
	// example we are sending all of the available COM port data
	// to the SetText() function.
	//
	// NOTE: the <SerialPort>_DataReceived() function is launched
	// in a separate thread from the rest of the application.  A
	// delegate function is required in order to properly access
	// any managed objects inside of the other thread.  Since we
	// will be writing to a textBox (a managed object) the delegate
	// function is required.  Please see the SetText() function for 
	// more information about delegate functions and how to use them.
		 
	// Data was received - clear timeout counter:
	timeoutCounter = 0;
	// YL 15.6 ... in SS mode we don't specify outputFile
	// was: if (applicationStart == true) // YL 6.11 in 5.1 there is: if (applicationStart == true && TS == true) instead
	if ((applicationStart == true) && (SS == false))
	// ... YL 15.6
	{ 	// YS: Why was this done (makes no sense)? -> //AM was if (applicationStart == true)
		// extract file name + directory from the "save as" text box
		outputFilePath = outputDirectoryPath + "\\" + outputFile_start_AppTab_TextBox->Text; // YL 16.6 was: outputFilePath = outputFile_start_AppTab_TextBox->Text;
		// generate two file names: outfile.bin and outfile.txt
		outputFilePathBin = outputFilePath + ".bin";
		outputFilePathTxt = outputFilePath + ".txt";
		File::Delete(outputFilePath); // YL 30.12
		// in case of a new transaction beginning, reset buffer and status:
		if (gTotalNumOfBytesToReceive == totalBytes) 
		{
			gFoundHeaderBlock = false;
			gHeaderBlockCompleted = false;
			gPacketRetryErrorCounter = PACKET_RETRY_COUNT;
			gSensorSwOverflow = 0;
			gSensorHwOverflow = 0;
			gNumOfReceivedBlocks = 0;
			gBlockBuffer = "";
			gReqiredNumOfBlocks = Int32::Parse(numberOfBlocks_start_AppTab_TextBox->Text);

			// Clear output files:
			FileStream^ fb = gcnew FileStream(outputFilePathBin, FileMode::Create);
			FileStream^ ft = gcnew FileStream(outputFilePathTxt, FileMode::Create);
			System::IO::BinaryWriter^ bb = gcnew System::IO::BinaryWriter(fb);
			System::IO::StreamWriter^ bw1 = gcnew System::IO::StreamWriter(ft);
			bb->Close();
			bw1->Close();
			// YL 6.11 in 5.1 there is enclosing if-else:
//				if (!appStartedByRecovery)
//				{
//					// clear output files:
//					FileStream^ fb = gcnew FileStream(outputFilePathBin, FileMode::Create);
//					FileStream^ ft = gcnew FileStream(outputFilePathTxt, FileMode::Create);
//					System::IO::BinaryWriter^ bb = gcnew System::IO::BinaryWriter(fb);
//					System::IO::StreamWriter^ bw1 = gcnew System::IO::StreamWriter(ft);
//					bb->Close();
//					bw1->Close();
//				}
//				else
//				{
//					appStartedByRecovery = false;
//				}
				
		}
		// Read the bytes received from serial port into buffer:
		int num_of_bytes = serialPort1->BytesToRead;
		try 
		{
			FileStream^ fr1 = gcnew FileStream(outputFilePathTxt, FileMode::Append);
			FileStream^ fr2 = gcnew FileStream(samplesLogFilePath, FileMode::Append); // YL 16.6 was: FileStream^ fr2 = gcnew FileStream("SamplesLog.txt", FileMode::Append);
			FileStream^ fr3 = gcnew FileStream(outputFilePathBin, FileMode::Append);						 
			System::IO::StreamWriter^ bw1 = gcnew System::IO::StreamWriter(fr1);
			System::IO::BinaryWriter^ bw2 = gcnew System::IO::BinaryWriter(fr2);
			System::IO::BinaryWriter^ bw3 = gcnew System::IO::BinaryWriter(fr3);					 
			totalBytes -= num_of_bytes;
			array<Byte> ^buffer = gcnew array<Byte>(num_of_bytes);
			serialPort1->Read(buffer, 0, num_of_bytes);
			// append buffer to "SamplesLog.txt":
			bw2->Write(buffer);
			bw2->Close();

			// parse incoming stream 
			// when we send "app start" command, we get the following:							
			// "app start ost N wireless\n"									- echo
			// "WISTONE> HEADER-BLOCK:xx...xdd...denmxxxxxdd..dxnmxxxxxok\n"- header block + N data blocks
			// "WISTONE> TSCOMPLETED: ...\n"								- completion announcement
			// ddd...dd - 504 (= DATA_BLOCK_SIZE) bytes of samples data
			// e - Packer Error Rate - num of retransmitted packets within this block
			// n - Accel overflow counter (HW)
			// m - Accel block overflow counter (SW)
			// x - undefined
			//
			// break into blocks
			// append buffer to block buffer:
			for (i = 0; (i < num_of_bytes); i++) 
			{
				gBlockBuffer = String::Concat( gBlockBuffer, Convert::ToChar(buffer[i]) );
			}

			// go over the accumulated gBlockBuffer[] and look for ERROR indications from Wistone or Communication Plug:
			if (gBlockBuffer->Contains(%String("ERROR"))) 
			{
				// we have discovered an ERROR, so indicate that Application Mode is over:
				applicationStart = false;
				FileStream^ fr2 = gcnew FileStream(samplesLogFilePath, FileMode::Append); // YL 16.6 was: FileStream^ fr2 = gcnew FileStream("SamplesLog.txt", FileMode::Append);							 
				System::IO::BinaryWriter^ bw2 = gcnew System::IO::BinaryWriter(fr2);
				DateTime dt1 = DateTime::Now;
				bw2->Write("\n######### Terminated due to ERROR detection ##########\n");
				bw2->Write("\n******************   " + dt1.ToString() + "   ********************\n");
				Sleep(100);
				// Update Stats
				shouldUpdateStats = false;
				// Enable application parameters input(disabled at button_1_click_1)
				EnableApplicationParametersInput = true;
				bw1->Close();
				bw3->Close();
				delete[] buffer;
				return;
			}
				 
			// Check if we are at the start of transaction:
			if (gFoundHeaderBlock == false) 
			{
				// Look for start of transaction delimiter: "HEADER-BLOCK":
				header_block_start_ptr = gBlockBuffer->IndexOf(header_string);
				if (header_block_start_ptr != -1) 
				{
					gFoundHeaderBlock = true;
					gBlockBuffer = gBlockBuffer->Substring(header_block_start_ptr);
				}
			}
			if (gFoundHeaderBlock == true) // We are after synch stage, in the middle of transaction
			{
				// identify the in-bound debug info, and extract it
				// while we have at least one complete block in buffer:
				while (gBlockBuffer->Length >= TOTAL_BLOCK_SIZE) 
				{
					// we have a complete block in buffer
					// convert to Byte array:
					ready_block = u16LE->GetBytes( gBlockBuffer->Substring(0, DATA_BLOCK_SIZE) );
					// Convert array from Unicode (16bits) to Byte (8bit):
					for (i = 0; i < DATA_BLOCK_SIZE; i++)
					{
						ready_block_out[i] = ready_block[2 * i];
					}
					// Convert to Strings representing the numbers, write each three axes data in a separate line:
					for (i = 0; i< (DATA_BLOCK_SIZE / ACCMTR_SAMPLE_SIZE) ; i++) 
					{
						x = ((Int16)(ready_block_out[(i * ACCMTR_SAMPLE_SIZE) + 0]) << 8) + (Int16)(ready_block_out[(i * ACCMTR_SAMPLE_SIZE) + 1]);
						if (x & 0x8000) // this is a negative number
						{
							x = (~(x - 1)) * (-1);
						}
						x = x / 4;
						y = ((Int16)(ready_block_out[(i * ACCMTR_SAMPLE_SIZE) + 2]) << 8) + (Int16)(ready_block_out[(i * ACCMTR_SAMPLE_SIZE) + 3]);
						if (y & 0x8000) // this is a negative number
						{
							y = (~(y - 1)) * (-1);
						}
						y = y / 4;
						z = ((Int16)(ready_block_out[(i * ACCMTR_SAMPLE_SIZE) + 4]) << 8) + (Int16)(ready_block_out[(i * ACCMTR_SAMPLE_SIZE) + 5]);
						if (z & 0x8000) // this is a negative number
						{
							z = (~(z - 1)) * (-1);
						}
						z = z / 4;
						bw1->Write(Convert::ToString(x) + " " + Convert::ToString(y) + " " + Convert::ToString(z) + "\n");
						bw1->Flush();
					}
					full_size_block = u16LE->GetBytes(gBlockBuffer->Substring(0, TOTAL_BLOCK_SIZE));
					if (gHeaderBlockCompleted == true) 
					{
						// Its an inner block, get the error counters:
						if (gNumOfReceivedBlocks != 0) // Then all counters == 0, which will lead to division by zero! // YL 6.11 in 5.11 instead of this if there is: 	g_packet_retry_error_counter += Convert::ToByte(Convert::ToChar(g_block_buffer->Substring(504,1)));
						{
							gPacketRetryErrorCounter = Convert::ToByte(Convert::ToChar(gBlockBuffer->Substring(RETRY_COUNTER_LOCATION, 1)));
						}
						gSensorHwOverflow += Convert::ToByte(Convert::ToChar(gBlockBuffer->Substring(HW_OVERFLOW_LOCATION, 1)));
						gSensorSwOverflow += Convert::ToByte(Convert::ToChar(gBlockBuffer->Substring(SW_OVERFLOW_LOCATION, 1)));
						// Divide data by 4, to get 14bit numbers:
						for (i = 0; i < DATA_BLOCK_SIZE; i = i + 2) 
						{
							Byte MSB = (Byte)ready_block_out[i];
							Byte LSB = (Byte)ready_block_out[i + 1];
							Byte temp = (Byte)((MSB & 0x03) << 6);
							MSB = MSB >> 2;
							LSB = (LSB >> 2 ) | temp;
							ready_block_out[i] = MSB;
							ready_block_out[i + 1] = LSB;
						}
						bw3->Write(ready_block_out);
						gNumOfReceivedBlocks++;
					}
					gHeaderBlockCompleted = true;
					// Convert non-printed characters to '_', leave printed chars as is:
					block_line = "";
					for (i = 0; i < TOTAL_BLOCK_SIZE; i++) 
					{
						if (full_size_block[2 * i] < (Byte)32) 
						{
							block_line = String::Concat(block_line, %String("_"));
						}
						else
						{
							block_line = String::Concat(block_line, Convert::ToChar(full_size_block[2 * i]) );
						}
					}
					block_line = String::Concat(block_line, %String("\n"));
					// Display it in GUI:
					SetText(block_line);
					// Cut the complete block from buffer:
					gBlockBuffer = gBlockBuffer->Substring(TOTAL_BLOCK_SIZE);
				}
			}
			delete[] buffer;
			// Close file (to flush the pipes):
			bw1->Close();
			bw3->Close();

			// - look for end of transaction:
			//	 - looking for ending string sent from Wistone
			//	 - using the byte counter, and
			if (gNumOfReceivedBlocks >= gReqiredNumOfBlocks) 
			{
				// Indicate that application is done:
				applicationStart = false;
				// Disable session timeout timer:
				if (sessionTimeout_Timer->Enabled == true) // YL 6.11 in 5.1 there is: if(SS == false) instead
				{
					sessionTimeout_Timer->Enabled = false;
					// YL 6.11 in 5.1 there is additional line: inNetworkRecoveryMode = false;
				}
				FileStream^ fr2 = gcnew FileStream(samplesLogFilePath, FileMode::Append); // YL 16.6 was: FileStream^ fr2 = gcnew FileStream("SamplesLog.txt", FileMode::Append);						 
				System::IO::BinaryWriter^ bw2 = gcnew System::IO::BinaryWriter(fr2);
				DateTime dt1 = DateTime::Now;
				bw2->Write("\n******************   " + dt1.ToString() + "   ********************\n");
				Sleep(100);
				// Update Stats:
				shouldUpdateStats = true;
				// Enable application parameters input(disabled at button_1_click_1):
				EnableApplicationParametersInput = true;
				bw2->Close();
				bw1->Close();
			}
		}
		catch(ArgumentNullException^ a) 
		{
			ArgumentNullException^ tmp = a;
		}
	}
		
	// YL 6.11 in 5.1 here comes additional code:
//		else if (inNetworkRecoveryMode)
//		{
//			try 
//			{
//				String ^toDisplay = serialPort1->ReadExisting();
//				//need to get the full reply first!
//				recoveryStepReply = String::Concat(recoveryStepReply,toDisplay);
//				if (recoveryStepReply->IndexOf("ok") == -1)
//				{
//					//need to wait longer
//					return;
//				}
//				else
//				{
//					toDisplay = String::Copy(recoveryStepReply);
//					recoveryStepReply = "";
//				}
//				//either reconnect or app stop were sent:
//				bool static recoveryReconnectSent = false;
//				int success_string_ptr;
//				if(recoveryReconnectSent)
//				{
//					success_string_ptr = toDisplay->IndexOf("RECONNECTED");
//				}
//				else
//				{
//					success_string_ptr = toDisplay->IndexOf("APPSTOPPED");
//				}
//				if (success_string_ptr != -1)
//				{
//					if(recoveryReconnectSent)
//					{
//						recoveryReconnectSent = false;
//						button29_Click(nullptr,EventArgs::Empty);//means sendAppStop();
//					}
//					else
//					{
//						if (TS == true)
//						{
//							int newNumOfBlocks = Int32::Parse(textBoxApplication_NumOfBlocks->Text);
//							newNumOfBlocks -= g_num_of_received_blocks;
//							textBoxApplication_NumOfBlocks->Text = newNumOfBlocks.ToString();
//							int newAddress = Int32::Parse(textBoxApplication_Address->Text);
//							newAddress += g_num_of_received_blocks;
//							textBoxApplication_Address->Text = newAddress.ToString();
//						}
//						else if(OST == true)
//						{
//							int newNumOfBlocks = Int32::Parse(textBoxApplication_NumOfBlocks->Text);
//							newNumOfBlocks -= g_num_of_received_blocks;
//							textBoxApplication_NumOfBlocks->Text = newNumOfBlocks.ToString();
//						}
//						inNetworkRecoveryMode = false;
//						appStartedByRecovery = true;
//							
//						button1_Click_1(nullptr,EventArgs::Empty);
//					}
//				}
//				else
//				{
//					if(recoveryReconnectSent)
//					{
//						if (MessageBox::Show(
//							"Do you want to retry connecting or cancel this session? (data already retrieved will be saved)", 
//							"Reconnect Failed!", MessageBoxButtons::RetryCancel, 
//							MessageBoxIcon::Question)
//							== System::Windows::Forms::DialogResult::Cancel)
//						{
//							//abort session
//							FileStream^ fr2 = gcnew FileStream("allSamples.txt", FileMode::Append);						 
//							System::IO::BinaryWriter^ bw2 = gcnew System::IO::BinaryWriter(fr2);
//							DateTime dt1 = DateTime::Now;
//							bw2->Write("\n******************   " + dt1.ToString() + "   ********************\n");
//							Sleep(100);
//							//Update Stats
//							shouldUpdateStats = true;
//							bw2->Close();
//							EnableApplicationParametersInput = true;
//							inNetworkRecoveryMode = false;
//							recoveryReconnectSent = false;
//							toDisplay = "Session Aborted!";
//						}
//						else
//						{
//							sendReconnectSession();
//						}
//					}
//					else
//					{
//						recoveryReconnectSent = true;
//						sendReconnectSession();
//					}
//				}
//				//toDisplay->Replace("\0", "");
//				//  - display the text received into GUI
//				SetText(toDisplay);
//			}	 
//			catch(...) 
//			{
//				//If there was an exception, then close the handle to 
//				//  the device and assume that the device was removed
// //			buttonClose_Click(this, gcnew EventArgs());
//				buttonClose_Click(sender, e);
//			}
//		}		
	else 
	{ // We are not within applicationStart or inNetworkRecoveryMode:
		try 
		{
			String ^toDisplay = serialPort1->ReadExisting();
			// Look for termination string: "COMPLETED":
			gTerminationFinder = String::Concat(gTerminationFinder, toDisplay);
			termination_string_ptr = gTerminationFinder->IndexOf(termination_string);

			if (termination_string_ptr != -1)
			{
				EnableApplicationParametersInput = true;
			}
			else
			{
				gTerminationFinder = gTerminationFinder->Substring((gTerminationFinder->Length) - (termination_string->Length));
			}
			// toDisplay->Replace("\0", "");
			//  - display the text received into GUI
			SetText(toDisplay);
		}
		catch(...) 
		{
			// If there was an exception, then close the handle to 
			// the device and assume that the device was removed
			disconnect_Button_Click(sender, e);
		}
	}
}

/****************************************************************************
Function:
private: void sendReconnectSession()
***************************************************************************/
void GUI::sendReconnectSession()
{
	StreamWriter^ sw = System::IO::File::AppendText(commandsLogFilePath);
	sw->WriteLine("app reconnect"); // YL 6.11 in 5.1 the text is: plug reconnect
	sw->Close();
	try
	{
		//Write the data in the text box to the open serial port
		sendCommands_TextBox->Text = "app sleep"; // YL 6.11 in 5.1 the text is: plug reconnect
		buttonSendFlag = true;
	}
	catch(...)
	{
	}
}

/****************************************************************************
Function:
public void SetText(string text)

Summary:
This function prints the input text to the txtDataReceived textbox.

Description:
This function prints the input text to the txtDataReceived textbox.  If
the calling thread is the same as the thread that owns the textbox, then
the AppendText() method is called directly.  If a thread other than the
main thread calls this function, then an instance of the delegate function
is created so that the function runs again in the main thread.

Precondition:
None

Parameters:
string text     - Text that needs to be printed to the textbox

Return Values:
None

Remarks:
None
***************************************************************************/
// stringToCharArray:
// convert C++ style String to C style string (i.e. char *)
// if input String is longer than output string - truncate it
void GUI::stringToCharArray(String^ from_string, int max_to_char_len, char* to_chars)
{
	int			i, j, is_hebrew, tmp1, tmp2;
	errno_t     returnValue;
	size_t      pReturnValue;
	mbstate_t   mbstate;
	char		alef = '�';
	String^		lut = "���������������������������";

	// go over all String characters
	for (i = 0; ((i < (from_string->Length)) && (i < (max_to_char_len - 1))); i++) 
	{
		is_hebrew = 0;
		// compare to LUT, if equals then it's an Hebrew letter
		for (j = 0; j < (lut->Length); j++)
		{
			if (((int)from_string[i]) == ((int)lut[j])) 
			{
				is_hebrew = 1;
			}
		}	
		if (is_hebrew) 
		{
			// it's Hebrew, convert differently the wide character
			tmp1 = (int)from_string[i];
			tmp2 = (int)lut[0];
			to_chars[i] = alef;
			to_chars[i] += tmp1 - tmp2;
		}
		else 
		{
			// Reset to initial conversion state
			memset(&mbstate, 0, sizeof(mbstate));
			returnValue = wcrtomb_s(&pReturnValue, &(to_chars[i]), sizeof(char), from_string[i], &mbstate);
		}
	}
	to_chars[i] = 0; // append end of string to result

	return;
}

/****************************************************************************
Function:
private: System::Void browse_Button_Click(System::Object^  sender, System::EventArgs^  e)
***************************************************************************/	
System::Void GUI::browse_Button_Click(System::Object^  sender, System::EventArgs^  e) 
{
	try
	{
		openFileDialog1->ShowDialog();
		scriptFile_TextBox->Text = openFileDialog1->FileName;
		System::IO::StreamReader^ sr = gcnew System::IO::StreamReader(openFileDialog1->FileName);
		sendCommands_TextBox->Text = sr->ReadToEnd();
	}
	catch(...)
	{
		scriptFile_TextBox->Text = "";
	}
}
	
/****************************************************************************
Function:
private: System::Void sendCommands_Button_Click(System::Object^  sender, System::EventArgs^  e)
***************************************************************************/			 
System::Void GUI::sendCommands_Button_Click(System::Object^  sender, System::EventArgs^  e) 
{
	detectAppStart(); 
	buttonSendFlag = true;
	commandCounter = 0;
}
	
/****************************************************************************
Function:
private: System::Void clearConsole_Button_Click(System::Object^  sender, System::EventArgs^  e)
***************************************************************************/
System::Void GUI::clearConsole_Button_Click(System::Object^  sender, System::EventArgs^  e) 
{
	// Clear the textbox and print that we are connected.
	console_TextBox->Clear();
	// YL 23.6 console_TextBox->AppendText("WISTONE>\n\r"); - looks unnecessary
}
	
/****************************************************************************
Function:
private: System::Void countNumberOfCharacters_Button_Click(System::Object^  sender, System::EventArgs^  e)
***************************************************************************/			 
System::Void GUI::countNumberOfCharacters_Button_Click(System::Object^  sender, System::EventArgs^  e) 
{
	int length = console_TextBox->Text->Length;
	countNumberOfCharacters_TextBox->Text = length.ToString();
}
		
/****************************************************************************
Function:
private: System::Void GUI_Load(System::Object^  sender, System::EventArgs^  e)
***************************************************************************/				 		 
System::Void GUI::GUI_Load(System::Object^  sender, System::EventArgs^  e) 
{			 
	// YL 16.6 ...
	outputDirectoryPath = ".\\Output";
	IntPtr outputDirectoryPathPtr = System::Runtime::InteropServices::Marshal::StringToHGlobalUni(outputDirectoryPath);
	if (!CreateDirectory((LPCWSTR)outputDirectoryPathPtr.ToPointer(), NULL) && (GetLastError() != ERROR_ALREADY_EXISTS)) 
	{
		systemMessage(ERR_MSG, "Failed to create Output Directory", "GUI_Load", "");
	}
	System::Runtime::InteropServices::Marshal::FreeHGlobal(outputDirectoryPathPtr);
	commandsLogFilePath = outputDirectoryPath + "\\CommandsLog.txt"; // was: fileName = "CommandsLog.txt";
	samplesLogFilePath = outputDirectoryPath + "\\SamplesLog.txt";
	// ... YL 16.6
	applicationStart = false;
	// YL 6.11 in 5.1 there is additional line: appStartedByRecovery = false;
	SS = false;
	TS = false;
	OST = false;
	ledOn = false;
	totalBytes = 0;
	// YL 3.5 ...
	// was: systemTab->Visible = false;
	systemTab->Visible = true;
	// ... YL 3.5
	//set retry counter to PACKET_RETRY_COUNT - num of packets needed with no error:
	gPacketRetryErrorCounter = PACKET_RETRY_COUNT;	// YL 6.11 in 5.1 this line is absent
}	
	
/****************************************************************************
Function:
private: System::Void login_Button_Click(System::Object^  sender, System::EventArgs^  e)
***************************************************************************/	    
System::Void GUI::login_Button_Click(System::Object^  sender, System::EventArgs^  e) 
{
	/*if (userName_TextBox->Text == "wistone" && password_TextBox->Text == "yulia")*/ // YL 3.12 commented for debug
	/*{*/
		sendCommands_TextBox->Visible = true;
		sendCommands_Button->Visible = true;
		systemTab->Visible = true;
		flashTab->Visible = true;
		eepromTab->Visible = true;
		lcdTab->Visible = true;
		wakeUpTab->Visible = true;
		temperatureTab->Visible = true;
		ledTab->Visible = true;
		buzzerTab->Visible = true;
		switchTab->Visible = true;
		ADCTab->Visible = true;
		accelerometerTab->Visible = true;
		//Client Config Tab Enable:
		updateValue_config_AppTab_Button->Visible = true;
		value_config_AppTab_NumericUpDown->Visible = true;
		parameter_config_AppTab_ComboBox->Visible = true;
		value_config_AppTab_Label->Visible = true;
		parameter_config_AppTab_Label->Visible = true;
		config_AppTab_TextBox->Visible = true;
	/*}*/
}	
	
/****************************************************************************
Function:
private: System::Void updateAppParametersWithOffsets()
***************************************************************************/		 
System::Void GUI::updateAppParametersWithOffsets() 
{
	//add offset changes + output name change.
	if (TS == true)
	{
		int newNumOfBlocks = Int32::Parse(numberOfBlocks_start_AppTab_TextBox->Text);
		newNumOfBlocks -= gNumOfReceivedBlocks;
		numberOfBlocks_start_AppTab_TextBox->Text = newNumOfBlocks.ToString();
		int newAddress = Int32::Parse(address_start_AppTab_TextBox->Text);
		newAddress += gNumOfReceivedBlocks;
		address_start_AppTab_TextBox->Text = newAddress.ToString();
	}
	/*else if(OST == true)
	{
		int newNumOfBlocks = Int32::Parse(numberOfBlocks_start_AppTab_TextBox->Text);
		newNumOfBlocks -= gNumOfReceivedBlocks;
		numberOfBlocks_start_AppTab_TextBox->Text = newNumOfBlocks.ToString();
	} no point in doing this in OST mode*/
	stopApplication_AppTab_Button_Click(nullptr,EventArgs::Empty);
	//changing name to avoid deleting older information
	outputFile_start_AppTab_TextBox->Text += "-new";
}
	
// YL 6.11 in 5.1 instead of updateAppParametersWithOffsets there is:
// private: System::Void startSessionTimeoutRecovery() 
// {
//	inNetworkRecoveryMode = true;
//	recoveryStepReply = "";
//	button29_Click(nullptr,EventArgs::Empty);//means sendAppStop();
// }

/***T I M E R 1 COM ports***/	
	
/****************************************************************************
Function:
private: System::Void comPorts_Timer_Tick(System::Object^  sender, System::EventArgs^  e) 
***************************************************************************/
System::Void GUI::comPorts_Timer_Tick(System::Object^  sender, System::EventArgs^  e) 
{
	// Update the COM ports list so that we can detect
	// new COM ports that have been added.
	updateCOMPortList();
}
	
/***T I M E R 2 connection***/		
	
/****************************************************************************
Function:
private: System::Void connection_Timer_Tick(System::Object^  sender, System::EventArgs^  e)
***************************************************************************/
System::Void GUI::connection_Timer_Tick(System::Object^  sender, System::EventArgs^  e) 
{
	connectionTime_TextBox->Text = "Connected:";
	secondsDuration += 1;
	if (secondsDuration == 60)
	{
		secondsDuration = 0;
		minutesDuration += 1;
		if (minutesDuration == 60)
		{
			minutesDuration = 0;
			hoursDuration += 1;
			if (hoursDuration  == 100)
			{
				hoursDuration = 0;
			}
		}
	}
	if (hoursDuration < 10)
	{
		connectionTime_TextBox->Text += "0";
	}
	connectionTime_TextBox->Text += hoursDuration.ToString();
	connectionTime_TextBox->Text += ":";
	if(minutesDuration < 10)
	{
		connectionTime_TextBox->Text += "0";
	}
	connectionTime_TextBox->Text += minutesDuration.ToString();
	connectionTime_TextBox->Text += ":";
	if(secondsDuration < 10)
	{
		connectionTime_TextBox->Text += "0";
	}

	connectionTime_TextBox->Text += secondsDuration.ToString();
}
	
/***T I M E R 3 commands***/		
	
/****************************************************************************
Function:
private: System::Void commands_Timer_Tick(System::Object^  sender, System::EventArgs^  e)
***************************************************************************/
System::Void GUI::commands_Timer_Tick(System::Object^  sender, System::EventArgs^  e) 
{
	try
	{
		if (buttonSendFlag == false)
		{
			return;
		}
		if (sendCommands_TextBox->Lines->Length == 0)
		{
			return;
		}
		if (commandCounter >= sendCommands_TextBox->Lines->Length)
		{
			commandCounter = 0;
			buttonSendFlag = false;
			return;
		}
		try
		{
			// YL 27.4 ...
			// was: serialPort1->Write(sendCommands_TextBox->Lines[commandCounter]); // MSDN: to get a writable collection of lines (by default the collection of lines is a read-only copy of the lines in the TextBox)
			array<String^>^ commands = gcnew array<String^>(sendCommands_TextBox->Lines->Length); 
			commands = sendCommands_TextBox->Lines;
			// YL 27.5 ... added if else
			if (comPortConnection)
			{
				serialPort1->Write(commands[commandCounter]);
				// ... YL 27.4
				serialPort1->Write("\r");
			}
			else if (internetConnection)
			{
				IntPtr ptrToNativeString = System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(commands[commandCounter]);
				server->serverSend(static_cast<char*>(ptrToNativeString.ToPointer()));
			}
			// ... YL 27.5
		}
		catch (TimeoutException ^) 
		{
			console_TextBox->AppendText("ERROR: serialPort->Write() - timeout occurred!\n\r");
			EnableApplicationParametersInput = true;	// re-enable the Application Tab
		}
		lastIndexOccurrence = newLastIndexOccurrence;
		commandCounter++;
	}
	catch(...)
	{
		// If there was an exception, then close the handle to 
		// the device and assume that the device was removed
		disconnect_Button_Click(sender, e);
	}
}	
	
/***T I M E R 4 statistics***/	
		
/****************************************************************************
Function:
private: System::Void meanAndStatsUpdate(int block_num,String^ sampleFileName)
***************************************************************************/
System::Void GUI::meanAndStatsUpdate(int block_num,String^ sampleFileName) 
{
	FileStream^ fr = gcnew FileStream(sampleFileName, FileMode::Open, FileAccess::Read);
	BinaryReader^ br = gcnew BinaryReader(fr);
	long length = (long)br->BaseStream->Length;
	long axisLength = 84 * block_num;
	array<Byte> ^readBuffer = gcnew array<Byte>(length);
	array<int> ^tmp_axis = gcnew array<int>(axisLength);

	br->Read(readBuffer, 0, length);
	br->Close();

	// go over samples
	for (int j = 0; j < 3; j++) // per axis (x,y,z)
	{
		for (int i = 0; i < block_num; i++) // per block
		{
			for (int k = i * DATA_BLOCK_SIZE + j * 2; k < i * DATA_BLOCK_SIZE + j * 2 + DATA_BLOCK_SIZE; k += ACCMTR_SAMPLE_SIZE) // per sample
			{
				Byte MSB = readBuffer[k];
				Byte LSB = readBuffer[k + 1];
				// multiply by 4, to get the sign bit at the MSbit:
				MSB = (Byte)(MSB << 2) | ((LSB >> 6) && 0x03);
				LSB = (Byte)(LSB << 2);
				int sign = getSign(MSB);
				removeLsb(&MSB, &LSB);
				if (sign == -1)
				{
					twosCompement(&MSB, &LSB);
				}
				tmp_axis[i * 84 + (k - (i * DATA_BLOCK_SIZE + j * 2)) / ACCMTR_SAMPLE_SIZE] = ((((int)MSB & 0xFF) << 8) + ((int)LSB & 0xFF)) * sign;
			}
		}
		double result = mean(tmp_axis,axisLength);
		if (j == 0) 
		{
			x_TextBox->Text = result.ToString(); 
		}
		else if (j == 1)
		{
			y_TextBox->Text=result.ToString();
		}
		else z_TextBox->Text=result.ToString(); 
	}
}
	
/****************************************************************************
Function:
private: double mean(array<int> ^tmp_axis, int length)
***************************************************************************/
double GUI::mean(array<int> ^tmp_axis, int length)
{
	long sum = 0;
	for (int i = 0; i < length; i++)
	{
		sum += tmp_axis[i];
	}
	return sum / double(length); 
}
	
/****************************************************************************
Function:
private: void twosCompement(Byte* MSB, Byte* LSB)
***************************************************************************/			 
void GUI::twosCompement(Byte* MSB, Byte* LSB)
{
	*MSB = (~(*MSB)) & 0x3F;
	*LSB = ~(*LSB);
	if ((*LSB) == 0xff)
	{
		(*MSB)++;
		(*LSB) = 0;
	}
	else 
	{
		(*LSB)++;
	}
}
	
/****************************************************************************
Function:
private: int getSign(Byte number)
***************************************************************************/			 
int GUI::getSign(Byte number)
{
	if (number & 0x80)
	{
		return -1;
	}
	return 1 ;
}
	
/****************************************************************************
Function:
private: void removeLsb(Byte* MSB, Byte* LSB)
***************************************************************************/
void GUI::removeLsb(Byte* MSB, Byte* LSB)
{
	Byte temp = ((*MSB) & 0x03) << 6;
		(*MSB) = (*MSB) >> 2;
		(*LSB) = ((*LSB) >> 2) | temp;
}
	
/****************************************************************************
Function:
private: System::Void statistics_Timer_Tick(System::Object^  sender, System::EventArgs^  e)
***************************************************************************/			 
System::Void GUI::statistics_Timer_Tick(System::Object^  sender, System::EventArgs^  e) 
{
	char	string_to_gui[100];

	if (shouldUpdateStats)
	{
		// Update Stats based on samples
		// YL 16.6 ...
		// was: meanAndStatsUpdate(Int32::Parse(numberOfBlocks_start_AppTab_TextBox->Text), (outputFile_start_AppTab_TextBox->Text) + ".bin");
		meanAndStatsUpdate(Int32::Parse(numberOfBlocks_start_AppTab_TextBox->Text), outputFilePathBin);
		// ... YL 16.6
		shouldUpdateStats = false;
	}
	// update progress in GUI:
	sprintf_s(string_to_gui, sizeof(string_to_gui), "%d", (1 - (PACKET_RETRY_COUNT / gPacketRetryErrorCounter))); // YL 6.11 in 5.1 was: sprintf_s(string_to_gui, sizeof(string_to_gui), "%d", (g_packet_retry_error_counter));
	// YL 27.4 ...
	// was: packetErrorRate_TextBox->Text::set(%String(string_to_gui));
	packetErrorRate_TextBox->Text = %String(string_to_gui);
	// ... YL 27.4
	sprintf_s(string_to_gui, sizeof(string_to_gui), "%d", (gNumOfReceivedBlocks * TOTAL_BLOCK_SIZE));
	bytes_TextBox->Text = %String(string_to_gui);	// YL 27.4 was: bytes_TextBox->Text::set(%String(string_to_gui));
	sprintf_s(string_to_gui, sizeof(string_to_gui), "%d", gNumOfReceivedBlocks);
	blocks_TextBox->Text = %String(string_to_gui);	// YL 27.4 was: blocks_TextBox->Text::set(%String(string_to_gui));
	sprintf_s(string_to_gui, sizeof(string_to_gui), "%d", gSensorHwOverflow);
	hw_TextBox->Text = %String(string_to_gui);		// YL 27.4 was: hw_TextBox->Text::set(%String(string_to_gui));
	sprintf_s(string_to_gui, sizeof(string_to_gui), "%d", gSensorSwOverflow);
	sw_TextBox->Text = %String(string_to_gui);		// YL 27.4 was: sw_TextBox->Text::set(%String(string_to_gui));
}		
	
/***T I M E R 5 application***/		
	
/****************************************************************************
Function:
private: System::Void application_Timer_Tick(System::Object^  sender, System::EventArgs^  e)
***************************************************************************/			 
System::Void GUI::application_Timer_Tick(System::Object^  sender, System::EventArgs^  e) 
{
	if (EnableApplicationParametersInput)
	{
		// Enabling input of application parameters:
		ss_RadioButton->Enabled = true;
		ts_RadioButton->Enabled = true;
		ost_RadioButton->Enabled = true;
		startApplication_start_AppTab_Button->Enabled = true;
		stopApplication_start_AppTab_Button->Enabled = false; // YL 17.6 added
		destination_start_AppTab_TextBox->Enabled = true; // YL 15.6
		numberOfBlocks_start_AppTab_TextBox->Enabled = true;
		// YL 17.6 ...
		// was:
		// if (SS == false)
		// {
		// 	outputFile_start_AppTab_TextBox->Enabled = true;
		// 	sessionTimeout_start_AppTab_TextBox->Enabled = true;
		// }
		// communication_start_AppTab_ComboBox->Enabled = true;
		// if (OST == false)
		// {
		// 	address_start_AppTab_TextBox->Enabled = true;
		// }
		if (SS == true)
		{
			address_start_AppTab_TextBox->Enabled = true;
			communication_start_AppTab_ComboBox->Enabled = false;
			combination_start_AppTab_ComboBox->Enabled = true;
			sessionTimeout_start_AppTab_TextBox->Enabled = false;
			outputFile_start_AppTab_TextBox->Enabled = false;
		}
		else if (TS == true)
		{
			address_start_AppTab_TextBox->Enabled = true;
			communication_start_AppTab_ComboBox->Enabled = true;
			combination_start_AppTab_ComboBox->Enabled = false;
			sessionTimeout_start_AppTab_TextBox->Enabled = true;
			outputFile_start_AppTab_TextBox->Enabled = true;
		}
		else if (OST == true)
		{
			address_start_AppTab_TextBox->Enabled = false;
			communication_start_AppTab_ComboBox->Enabled = true;
			combination_start_AppTab_ComboBox->Enabled = true;
			sessionTimeout_start_AppTab_TextBox->Enabled = true;
			outputFile_start_AppTab_TextBox->Enabled = false;
		}
		// ... YL 17.6
	}
	else
	{
		// Disabling input of application parameters:
		ss_RadioButton->Enabled = false;
		ts_RadioButton->Enabled = false;
		ost_RadioButton->Enabled = false;
		startApplication_start_AppTab_Button->Enabled = false;
		stopApplication_start_AppTab_Button->Enabled = true; // YL 17.6 added
		outputFile_start_AppTab_TextBox->Enabled = false;
		destination_start_AppTab_TextBox->Enabled = false; // YL 15.6
		address_start_AppTab_TextBox->Enabled = false;
		numberOfBlocks_start_AppTab_TextBox->Enabled = false;
		communication_start_AppTab_ComboBox->Enabled = false;
		combination_start_AppTab_ComboBox->Enabled = false; // YL 17.6 added
		sessionTimeout_start_AppTab_TextBox->Enabled = false;
	}
}

/***T I M E R 6 session timeout***/

/****************************************************************************
Function:
private: System::Void sessionTimeout_Timer_Tick(System::Object^  sender, System::EventArgs^  e)
***************************************************************************/
System::Void GUI::sessionTimeout_Timer_Tick(System::Object^  sender, System::EventArgs^  e) 
{
	timeoutCounter++;
	if (timeoutCounter == Int32::Parse(sessionTimeout_start_AppTab_TextBox->Text))
	{
		if ((MessageBox::Show( "Networking timeout occurred. Try moving closer to the Wistone!"
			"Would you like to stop this run and update the application parameters accordingly?"
			"If the problem persists, press yes. Otherwise, press no.", "Session Timeout Occurred", 
			MessageBoxButtons::YesNo, MessageBoxIcon::Question) == System::Windows::Forms::DialogResult::Yes))
		{
			updateAppParametersWithOffsets();	// YL 6.11 in 5.1 was: startSessionTimeoutRecovery();
		}
		else
		{
			timeoutCounter = 0;
		}
	}
}

/***T I M E R 7 server***/

/****************************************************************************
Function:
private: System::Void server_Timer_Tick(System::Object^  sender, System::EventArgs^  e)
***************************************************************************/
System::Void GUI::server_Timer_Tick(System::Object^  sender, System::EventArgs^  e) // YL 21.6
{
	if (!serverConnected)
	{
		return;
	}
	server_Timer->Stop();
	serverCounter++;
	if (serverCounter == MAX_SERVER_COUNTER)
	{
		server->serverReconnect();
		serverCounter = 0;
	}
	server_Timer->Start();
}

/****************************************************************************
Function:
private: System::Void get_hub_stats_Button_Click(System::Object^  sender, System::EventArgs^  e)
***************************************************************************/
System::Void GUI::get_hub_stats_Button_Click(System::Object^  sender, System::EventArgs^  e)
{
	StreamWriter^ sw = System::IO::File::AppendText(commandsLogFilePath);

	sw->WriteLine("hub gstat ");
	sw->Close();
	try
	{
		// Write the data in the text box to the open serial port
		sendCommands_TextBox->Text =  "hub gstat ";
		buttonSendFlag = true;
	}
	catch (...)
	{
	}
}
/****************************************************************************
Function:
private: System::Void phone_reset_Button_Click(System::Object^  sender, System::EventArgs^  e)
***************************************************************************/
System::Void GUI::phone_reset_Button_Click(System::Object^  sender, System::EventArgs^  e)
{
	StreamWriter^ sw = System::IO::File::AppendText(commandsLogFilePath);

	sw->WriteLine("plug phone_reset ");
	sw->Close();
	try
	{
		// Write the data in the text box to the open serial port
		sendCommands_TextBox->Text = "plug phone_reset ";
		buttonSendFlag = true;
	}
	catch (...)
	{
	}
}


