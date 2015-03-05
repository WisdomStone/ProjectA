#pragma once

#include "server.h"	// YL 25.4

#include <stdio.h>
#include <string>
#include <vcclr.h>  // for gcroot
#include <windows.h>

namespace WistoneGUI 
{
	using namespace std;
	using namespace System::Text;
	using namespace System::IO;
	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	public ref class GUI : public System::Windows::Forms::Form
	{
	public:
		GUI(void);
		void	systemMessage(int level, char* msg_discription, char *func_name, char *msg_value);
		void	SetText(String^ text); // YL 7.5 SetText - changed private to public
		void	setServerConnected(bool isConnected); // YL 21.6
		void	resetServerCounter(void); // YL 21.6

	protected:
		~GUI();

	private:
		System::ComponentModel::IContainer^  components;

		gcroot<GUI^>*	thisGUI; // YL 7.5
		Server*			server;  // YL 30.4
		bool			internetConnection, comPortConnection;	// YL 30.4
		int 			secondsDuration, minutesDuration, hoursDuration;
		int 			lastIndexOccurrence, newLastIndexOccurrence;
		int 			totalBytes;
		int 			commandCounter;	
		int 			timeoutCounter;
		int				serverCounter; // YL 20.6
		bool			serverConnected; // YL 21.6
		bool 			buttonSendFlag;				
		bool 			ledOn;
		bool 			SS, TS, OST;
		bool 			applicationStart;
		bool 			shouldUpdateStats;
		static bool		EnableApplicationParametersInput = true;			
		String^ 		outputDirectoryPath;	// YL 16.6 added
		String^ 		commandsLogFilePath;	// YL 16.6 was: fileName;
		String^			samplesLogFilePath;		// YL 16.6 added
		String^			outputFilePathBin;		// YL 16.6 added
		String^			outputFilePathTxt;		// YL 16.6 added
		Int32			gTotalNumOfBytesToReceive, gReqiredNumOfBlocks, gNumOfReceivedBlocks;		
		int				gPacketRetryErrorCounter;
		int				gSensorHwOverflow, gSensorSwOverflow;
		bool			gFoundHeaderBlock, gHeaderBlockCompleted;
		String^			gBlockBuffer;
		String^			gTerminationFinder;
		static int		INFO_MSG = 0;
		static int		NTIC_MSG = 1;
		static int		WRNG_MSG = 2;
		static int		ERR_MSG  = 3;
		// YL 5.5 ... changed MACROs to static int
		static int		MAX_SN					= 100;		// serial number of the wistone
		static int		BLOCK_BUFFER_LEN		= 2000;		// max size in BYTEs of buffer that accepts the blocks for parsing
		// YL 3.12 ... magic numbers replacement
		static int		ACCMTR_SAMPLE_SIZE		= 6;		// bytes
		static int		TOTAL_BLOCK_SIZE		= 512; 		// bytes
		static int		DATA_BLOCK_SIZE			= 504; 		// bytes
		static int		LAST_DATA_BYTE			= ((TOTAL_BLOCK_SIZE - DATA_BLOCK_SIZE) - 1); // in block
		static int		RETRY_COUNTER_LOCATION	= (LAST_DATA_BYTE + 1); // in block
		static int		HW_OVERFLOW_LOCATION	= (LAST_DATA_BYTE + 2); // in block
		static int		SW_OVERFLOW_LOCATION	= (LAST_DATA_BYTE + 3); // in block
		static int		PACKET_RETRY_COUNT		= 9;
		static int		EEPROM_PAGE_SIZE		= 64;		// bytes
		static int		WRITE_TIMEOUT			= 1000;		// seconds
		static int		MAX_SESSION_TIMEOUT		= 600;		// seconds
		static int		MAX_SERVER_COUNTER		= 60;		// 60 x 5 seconds interval // YL 21.6
		static int		MIN_PARAM_COUNT			= 4;
		static int		MIN_HOUR				= 0;
		static int		MAX_HOUR				= 23;
		static int		MIN_MINUTE				= 0;
		static int		MAX_MINUTE				= 59;
		static int		MIN_DAY					= 1;
		static int		MAX_DAY					= 31;
		static int		MIN_WEEKDAY				= 1;
		static int		MAX_WEEKDAY				= 7;
		// ... YL 3.12
		// ... YL 5.5
		// YL 6.11 in 5.1 there are 3 additional fields:
		// bool 		inNetworkRecoveryMode;
		// bool 		appStartedByRecovery;
		// String^ 		recoveryStepReply;			
			
		// Create a delegate function for this thread that will take
		// in a string and will write it to console_TextBox
		delegate void SetTextCallback(String^ text);

	// YL 2.10 apply_writeLL_BuzzerTab_Button
	// - apply - the text on the widget
	// - writeLL - category sub-tab
	// - BuzzerTab - category tab 
	// - Button - the type of the widget
	
	// Application Mode Group Box:	
	private: System::Windows::Forms::GroupBox^  	applicationMode_GroupBox;
	private: System::Windows::Forms::RadioButton^  	ss_RadioButton;		
	private: System::Windows::Forms::RadioButton^  	ts_RadioButton;
	private: System::Windows::Forms::RadioButton^  	ost_RadioButton;	
	
	// Connection Group Box:
	private: System::Windows::Forms::GroupBox^		connection_GroupBox;
	private: System::Windows::Forms::RadioButton^	comPort_RadioButton;
	private: System::Windows::Forms::RadioButton^	internet_RadioButton;
	private: System::Windows::Forms::ComboBox^		comPort_ComboBox;
	private: System::Windows::Forms::Button^		connect_Button;
	private: System::Windows::Forms::Button^  		disconnect_Button;
	private: System::Windows::Forms::TextBox^  		connectionTime_TextBox;	
	
	// Login Group Box:
	private: System::Windows::Forms::GroupBox^  login_GroupBox;
	private: System::Windows::Forms::Label^  	userName_Label;
	private: System::Windows::Forms::TextBox^  	userName_TextBox;
	private: System::Windows::Forms::Label^  	password_Label;
	private: System::Windows::Forms::TextBox^  	password_TextBox;
	private: System::Windows::Forms::Button^  	login_Button;
	
	// Console Group Box:
	private: System::Windows::Forms::GroupBox^	console_GroupBox;	
	private: System::Windows::Forms::Button^	clearConsole_Button;
	private: System::Windows::Forms::TextBox^	console_TextBox;
	
	// Debug Information Group Box:
	private: System::Windows::Forms::GroupBox^  debugInformation_GroupBox;
	private: System::Windows::Forms::Label^  	meanOfAxes_Label;
	private: System::Windows::Forms::Label^  	x_Label;
	private: System::Windows::Forms::Label^  	y_Label;
	private: System::Windows::Forms::Label^  	z_Label;
	private: System::Windows::Forms::TextBox^  	x_TextBox;
	private: System::Windows::Forms::TextBox^  	y_TextBox;
	private: System::Windows::Forms::TextBox^ 	z_TextBox;
	private: System::Windows::Forms::Label^  	errorRates_Label;
	private: System::Windows::Forms::Label^  	packetErrorRate_Label;
	private: System::Windows::Forms::TextBox^  	packetErrorRate_TextBox;
	private: System::Windows::Forms::Label^  	sensorOverflow_Label;
	private: System::Windows::Forms::Label^  	hw_Label;
	private: System::Windows::Forms::Label^  	sw_Label;
	private: System::Windows::Forms::TextBox^  	hw_TextBox;	
	private: System::Windows::Forms::TextBox^  	sw_TextBox;
	private: System::Windows::Forms::Label^  	receivedDataCounters_Label;
	private: System::Windows::Forms::Label^  	blocks_Label;
	private: System::Windows::Forms::Label^  	bytes_Label;
	private: System::Windows::Forms::TextBox^  	blocks_TextBox;
	private: System::Windows::Forms::TextBox^  	bytes_TextBox;
	private: System::Windows::Forms::Button^  	countNumberOfCharacters_Button;
	private: System::Windows::Forms::TextBox^  	countNumberOfCharacters_TextBox;
	
	// Command Line Interface Group Box:
	private: System::Windows::Forms::GroupBox^  commandLineInterface_GroupBox;
	private: System::Windows::Forms::Label^  	commandScript_Label;
	private: System::Windows::Forms::TextBox^  	sendCommands_TextBox;	
	private: System::Windows::Forms::Button^  	sendCommands_Button;
	private: System::Windows::Forms::Label^  	scriptFile_Label;
	private: System::Windows::Forms::TextBox^  	scriptFile_TextBox;
	private: System::Windows::Forms::Button^  	browse_Button;
	
	// Application:
	private: System::Windows::Forms::TabPage^  		tabApplication;
	private: System::Windows::Forms::TabControl^  	applicationTab;
	
	// Application Tab - Start:
	private: System::Windows::Forms::TabPage^  	start_AppTab_TabPage;
	private: System::Windows::Forms::TextBox^  	start_AppTab_TextBox;
	private: System::Windows::Forms::Label^  	outputFile_start_AppTab_Label;
	private: System::Windows::Forms::TextBox^  	outputFile_start_AppTab_TextBox;
	private: System::Windows::Forms::ToolTip^	outputFile_start_AppTab_ToolTip; // YL 16.6 added
	private: System::Windows::Forms::Button^  	startApplication_start_AppTab_Button;
	private: System::Windows::Forms::Button^  	stopApplication_start_AppTab_Button;
	private: System::Windows::Forms::Label^  	destination_start_AppTab_Label;	
	private: System::Windows::Forms::TextBox^  	destination_start_AppTab_TextBox;	// YL 3.12 was: ComboBox instead of TextBox
	private: System::Windows::Forms::Label^  	address_start_AppTab_Label;
	private: System::Windows::Forms::TextBox^  	address_start_AppTab_TextBox;
	private: System::Windows::Forms::Label^  	numberOfBlocks_start_AppTab_Label;
	private: System::Windows::Forms::TextBox^  	numberOfBlocks_start_AppTab_TextBox;
	private: System::Windows::Forms::Label^  	communication_start_AppTab_Label;
	private: System::Windows::Forms::ComboBox^  communication_start_AppTab_ComboBox;
	private: System::Windows::Forms::Label^  	combination_start_AppTab_Label;
	private: System::Windows::Forms::ComboBox^  combination_start_AppTab_ComboBox;
	private: System::Windows::Forms::Label^  	sessionTimeout_start_AppTab_Label;	
	private: System::Windows::Forms::TextBox^  	sessionTimeout_start_AppTab_TextBox;

	// Application Tab - Shutdown:
	private: System::Windows::Forms::TabPage^  	shutdown_AppTab_TabPage;
	private: System::Windows::Forms::TextBox^  	shutdown_AppTab_TextBox;
	private: System::Windows::Forms::Button^  	shutdown_shutdown_AppTab_Button;
	private: System::Windows::Forms::Label^  	destination_shutdown_AppTab_Label;
	private: System::Windows::Forms::TextBox^  	destination_shutdown_AppTab_TextBox;

	// Application Tab - Config:
	private: System::Windows::Forms::TabPage^  		config_AppTab_TabPage;
	private: System::Windows::Forms::TextBox^  		config_AppTab_TextBox;
	private: System::Windows::Forms::Label^  		parameter_config_AppTab_Label;
	private: System::Windows::Forms::ComboBox^  	parameter_config_AppTab_ComboBox;
	private: System::Windows::Forms::Label^  		value_config_AppTab_Label;
	private: System::Windows::Forms::NumericUpDown^	value_config_AppTab_NumericUpDown;
	private: System::Windows::Forms::Button^  		updateValue_config_AppTab_Button;
	private: System::Windows::Forms::Label^  		destination_config_AppTab_Label;
	private: System::Windows::Forms::TextBox^  		destination_config_AppTab_TextBox;		

	// WakeUp Tab:
	private: System::Windows::Forms::TabPage^  		tabWakeUp;
	private: System::Windows::Forms::TabControl^  	wakeUpTab;
	
	// WakeUp Tab - Set Time:
	private: System::Windows::Forms::TabPage^  		setTime_WakeUpTab_TabPage;
	private: System::Windows::Forms::TextBox^  		setTime_WakeUpTab_TextBox;
	private: System::Windows::Forms::Label^  		hour_setTime_WakeUpTab_Label;
	private: System::Windows::Forms::NumericUpDown^ hour_setTime_WakeUpTab_NumericUpDown;
	private: System::Windows::Forms::Label^  		minute_setTime_WakeUpTab_Label;	
	private: System::Windows::Forms::NumericUpDown^ minute_setTime_WakeUpTab_NumericUpDown;
	private: System::Windows::Forms::Label^  		second_setTime_WakeUpTab_Label;
	private: System::Windows::Forms::NumericUpDown^ second_setTime_WakeUpTab_NumericUpDown;
	private: System::Windows::Forms::Button^  		setTime_setTime_WakeUpTab_Button;
	private: System::Windows::Forms::Label^  		destination_setTime_WakeUpTab_Label;
	private: System::Windows::Forms::TextBox^  		destination_setTime_WakeUpTab_TextBox;	
	
	// WakeUp Tab - Set Date:
	private: System::Windows::Forms::TabPage^  		setDate_WakeUpTab_TabPage;	
	private: System::Windows::Forms::TextBox^  		setDate_WakeUpTab_TextBox;
	private: System::Windows::Forms::Label^  		day_setDate_WakeUpTab_Label;
	private: System::Windows::Forms::NumericUpDown^ day_setDate_WakeUpTab_NumericUpDown;
	private: System::Windows::Forms::Label^  		month_setDate_WakeUpTab_Label;
	private: System::Windows::Forms::NumericUpDown^ month_setDate_WakeUpTab_NumericUpDown;
	private: System::Windows::Forms::Label^  		year_setDate_WakeUpTab_Label;
	private: System::Windows::Forms::NumericUpDown^ year_setDate_WakeUpTab_NumericUpDown;
	private: System::Windows::Forms::Label^  		weekDay_setDate_WakeUpTab_Label;
	private: System::Windows::Forms::NumericUpDown^ weekDay_setDate_WakeUpTab_NumericUpDown;
	private: System::Windows::Forms::Button^  		setDate_setDate_WakeUpTab_Button;
	private: System::Windows::Forms::Label^  		destination_setDate_WakeUpTab_Label;
	private: System::Windows::Forms::TextBox^  		destination_setDate_WakeUpTab_TextBox;	
	
	// WakeUp Tab - Get Time:
	private: System::Windows::Forms::TabPage^  	getTime_WakeUpTab_TabPage;
	private: System::Windows::Forms::TextBox^  	getTime_WakeUpTab_TextBox;
	private: System::Windows::Forms::Button^   	getTime_getTime_WakeUpTab_Button;
	private: System::Windows::Forms::Label^  	destination_getTime_WakeUpTab_Label;
	private: System::Windows::Forms::TextBox^  	destination_getTime_WakeUpTab_TextBox;

	// WakeUp Tab - Get Date:
	private: System::Windows::Forms::TabPage^ 	getDate_WakeUpTab_TabPage;
	private: System::Windows::Forms::TextBox^  	getDate_WakeUpTab_TextBox;
	private: System::Windows::Forms::Button^   	getDate_getDate_WakeUpTab_Button;
	private: System::Windows::Forms::Label^  	destination_getDate_WakeUpTab_Label;
	private: System::Windows::Forms::TextBox^  	destination_getDate_WakeUpTab_TextBox;	
	
	// WakeUp Tab - Set Wake Up:
	private: System::Windows::Forms::TabPage^  	setWakeUp_WakeUpTab_TabPage;	
	private: System::Windows::Forms::TextBox^  	setWakeUp_WakeUpTab_TextBox;	
	private: System::Windows::Forms::Label^  	delay_setWakeUp_WakeUpTab_Label;
	private: System::Windows::Forms::TextBox^  	delay_setWakeUp_WakeUpTab_TextBox;
	private: System::Windows::Forms::Button^  	apply_setWakeUp_WakeUpTab_Button;
	private: System::Windows::Forms::Label^  	destination_setWakeUp_WakeUpTab_Label;
	private: System::Windows::Forms::TextBox^  	destination_setWakeUp_WakeUpTab_TextBox;	

	// WakeUp Tab - Set Alarm:
	private: System::Windows::Forms::TabPage^  	setAlarm_WakeUpTab_TabPage;
	private: System::Windows::Forms::TextBox^  	setAlarm_WakeUpTab_TextBox;	
	private: System::Windows::Forms::Label^  	hour_setAlarm_WakeUpTab_Label;
	private: System::Windows::Forms::TextBox^  	hour_setAlarm_WakeUpTab_TextBox;
	private: System::Windows::Forms::Label^  	minute_setAlarm_WakeUpTab_Label;
	private: System::Windows::Forms::TextBox^  	minute_setAlarm_WakeUpTab_TextBox;
	private: System::Windows::Forms::Label^  	day_setAlarm_WakeUpTab_Label;
	private: System::Windows::Forms::TextBox^  	day_setAlarm_WakeUpTab_TextBox;
	private: System::Windows::Forms::Label^  	weekDay_setAlarm_WakeUpTab_Label;
	private: System::Windows::Forms::TextBox^  	weekDay_setAlarm_WakeUpTab_TextBox;	
	private: System::Windows::Forms::Button^  	apply_setAlarm_WakeUpTab_Button;
	private: System::Windows::Forms::Label^  	destination_setAlarm_WakeUpTab_Label;
	private: System::Windows::Forms::TextBox^  	destination_setAlarm_WakeUpTab_TextBox;	

	// WakeUp Tab - Set Boot:
	private: System::Windows::Forms::TabPage^ 	setBoot_WakeUpTab_TabPage;
	private: System::Windows::Forms::TextBox^  	setBoot_WakeUpTab_TextBox;
	private: System::Windows::Forms::Label^  	entryNumber_setBoot_WakeUpTab_Label;
	private: System::Windows::Forms::ComboBox^  entryNumber_setBoot_WakeUpTab_ComboBox;	
	private: System::Windows::Forms::Label^  	command_setBoot_WakeUpTab_Label;
	private: System::Windows::Forms::TextBox^  	command_setBoot_WakeUpTab_TextBox;
	private: System::Windows::Forms::Button^  	apply_setBoot_WakeUpTab_Button;
	private: System::Windows::Forms::Label^ 	destination_setBoot_WakeUpTab_Label;
	private: System::Windows::Forms::TextBox^  	destination_setBoot_WakeUpTab_TextBox;	
	
	// WakeUp Tab - Read Low Level:	
	private: System::Windows::Forms::TabPage^  	readLL_WakeUpTab_TabPage;
	private: System::Windows::Forms::TextBox^  	readLL_WakeUpTab_TextBox;
	private: System::Windows::Forms::Label^  	address_readLL_WakeUpTab_Label;
	private: System::Windows::Forms::TextBox^  	address_readLL_WakeUpTab_TextBox;
	private: System::Windows::Forms::Button^  	read_readLL_WakeUpTab_Button;
	private: System::Windows::Forms::Label^  	destination_readLL_WakeUpTab_Label;
	private: System::Windows::Forms::TextBox^  	destination_readLL_WakeUpTab_TextBox;	

	// WakeUp Tab - Write Low Level:
	private: System::Windows::Forms::TabPage^  	writeLL_WakeUpTab_TabPage;
	private: System::Windows::Forms::TextBox^  	writeLL_WakeUpTab_TextBox;
	private: System::Windows::Forms::Label^  	address_writeLL_WakeUpTab_Label;
	private: System::Windows::Forms::TextBox^  	address_writeLL_WakeUpTab_TextBox;
	private: System::Windows::Forms::Label^  	data_writeLL_WakeUpTab_Label;	
	private: System::Windows::Forms::TextBox^  	data_writeLL_WakeUpTab_TextBox;
	private: System::Windows::Forms::Button^  	write_writeLL_WakeUpTab_Button;
	private: System::Windows::Forms::Label^  	destination_writeLL_WakeUpTab_Label;
	private: System::Windows::Forms::TextBox^  	destination_writeLL_WakeUpTab_TextBox;	

	// System Tab: 
	private: System::Windows::Forms::TabPage^  		tabSystem;
	private: System::Windows::Forms::TabControl^  	systemTab;	
	
	// System Tab - Get System Version:
	private: System::Windows::Forms::TabPage^  	systemGetVersion_SystemTab_TabPage;
	private: System::Windows::Forms::TextBox^  	systemGetVersion_SystemTab_TextBox;
	private: System::Windows::Forms::Button^  	systemGetVersion_systemGetVersion_SystemTab_Button;	
	private: System::Windows::Forms::Label^  	destination_getSystemVersion_SystemTab_Label;
	private: System::Windows::Forms::TextBox^  	destination_getSystemVersion_SystemTab_TextBox;	
	
	// System Tab - Set System ID:
	private: System::Windows::Forms::TabPage^  	systemSetId_SystemTab_TabPage;
	private: System::Windows::Forms::TextBox^  	systemSetId_SystemTab_TextBox;
	private: System::Windows::Forms::Label^  	systemId_systemSetId_SystemTab_Label;
	private: System::Windows::Forms::TextBox^  	systemId_systemSetId_SystemTab_TextBox;
	private: System::Windows::Forms::Button^  	systemSetId_systemSetId_SystemTab_Button;
	private: System::Windows::Forms::Label^  	destination_setSystemId_SystemTab_Label;
	private: System::Windows::Forms::TextBox^  	destination_setSystemId_SystemTab_TextBox;	
	
	// System Tab - Get Power:
	private: System::Windows::Forms::TabPage^  	getPower_SystemTab_TabPage;
	private: System::Windows::Forms::TextBox^  	getPower_SystemTab_TextBox;
	private: System::Windows::Forms::Button^  	getPower_getPower_SystemTab_Button;
	private: System::Windows::Forms::Label^  	destination_getPower_SystemTab_Label;
	private: System::Windows::Forms::TextBox^  	destination_getPower_SystemTab_TextBox;	
		
	// System Tab - Set Power:
	private: System::Windows::Forms::TabPage^  	setPower_SystemTab_TabPage;
	private: System::Windows::Forms::TextBox^  	setPower_SystemTab_TextBox;
	private: System::Windows::Forms::Label^  	enable5V_setPower_SystemTab_Label;
	private: System::Windows::Forms::ComboBox^  enable5V_setPower_SystemTab_ComboBox;
	private: System::Windows::Forms::Label^  	enable12V_setPower_SystemTab_Label;
	private: System::Windows::Forms::ComboBox^  enable12V_setPower_SystemTab_ComboBox;
	private: System::Windows::Forms::Label^  	enableM12V_setPower_SystemTab_Label;
	private: System::Windows::Forms::ComboBox^  enableM12V_setPower_SystemTab_ComboBox;
	private: System::Windows::Forms::Button^  	setPower_setPower_SystemTab_Button;
	private: System::Windows::Forms::Label^  	destination_setPower_SystemTab_Label;
	private: System::Windows::Forms::TextBox^  	destination_setPower_SystemTab_TextBox;	

	// System Tab - Get Last Error:
	private: System::Windows::Forms::TabPage^  	getLastError_SystemTab_TabPage;
	private: System::Windows::Forms::TextBox^  	getLastError_SystemTab_TextBox;
	private: System::Windows::Forms::Button^  	getLastError_getLastError_SystemTab_Button;
	private: System::Windows::Forms::Label^  	destination_getLastError_SystemTab_Label;
	private: System::Windows::Forms::TextBox^  	destination_getLastError_SystemTab_TextBox;	
	
	// Flash Tab:
	private: System::Windows::Forms::TabPage^  		tabFlash;
	private: System::Windows::Forms::TabControl^  	flashTab;	
	
	// Flash Tab - Format:
	private: System::Windows::Forms::TabPage^  	format_FlashTab_TabPage;
	private: System::Windows::Forms::TextBox^  	format_FlashTab_TextBox;
	private: System::Windows::Forms::Button^  	format_format_FlashTab_Button;
	private: System::Windows::Forms::Label^  	destination_format_FlashTab_Label;
	private: System::Windows::Forms::TextBox^  	destination_format_FlashTab_TextBox;	
	
	// Flash Tab - Initialize:
	private: System::Windows::Forms::TabPage^  	initialize_FlashTab_TabPage;
	private: System::Windows::Forms::TextBox^  	initialize_FlashTab_TextBox;
	private: System::Windows::Forms::Button^  	initialize_initialize_FlashTab_Button;
	private: System::Windows::Forms::Label^  	destination_initialize_FlashTab_Label;
	private: System::Windows::Forms::TextBox^  	destination_initialize_FlashTab_TextBox;	
	
	// Flash Tab - Card Detect:
	private: System::Windows::Forms::TabPage^  	cardDetect_FlashTab_TabPage;
	private: System::Windows::Forms::TextBox^  	cardDetect_FlashTab_TextBox;
	private: System::Windows::Forms::Button^  	cardDetect_cardDetect_FlashTab_Button;	
	private: System::Windows::Forms::Label^  	destination_cardDetect_FlashTab_Label;
	private: System::Windows::Forms::TextBox^  	destination_cardDetect_FlashTab_TextBox;	
	
	// Flash Tab - Write Sector:
	private: System::Windows::Forms::TabPage^  	writeSector_FlashTab_TabPage;
	private: System::Windows::Forms::TextBox^  	writeSector_FlashTab_TextBox;
	private: System::Windows::Forms::Label^  	address_writeSector_FlashTab_Label;
	private: System::Windows::Forms::TextBox^  	address_writeSector_FlashTab_TextBox;
	private: System::Windows::Forms::Label^  	data_writeSector_FlashTab_Label;	
	private: System::Windows::Forms::TextBox^  	data_writeSector_FlashTab_TextBox;
	private: System::Windows::Forms::Button^  	write_writeSector_FlashTab_Button;
	private: System::Windows::Forms::Label^  	destination_writeSector_FlashTab_Label;
	private: System::Windows::Forms::TextBox^  	destination_writeSector_FlashTab_TextBox;	

	// Flash Tab - Read Sector:
	private: System::Windows::Forms::TabPage^  	readSector_FlashTab_TabPage;
	private: System::Windows::Forms::TextBox^  	readSector_FlashTab_TextBox;
	private: System::Windows::Forms::Label^  	address_readSector_FlashTab_Label;
	private: System::Windows::Forms::TextBox^  	address_readSector_FlashTab_TextBox;
	private: System::Windows::Forms::Button^  	read_readSector_FlashTab_Button;
	private: System::Windows::Forms::Label^  	destination_readSector_FlashTab_Label;
	private: System::Windows::Forms::TextBox^  	destination_readSector_FlashTab_TextBox;	
	
	// Flash Tab - Get Capacity:
	private: System::Windows::Forms::TabPage^  	getCapacity_FlashTab_TabPage;
	private: System::Windows::Forms::TextBox^  	getCapacity_FlashTab_TextBox;
	private: System::Windows::Forms::Button^  	getCapacity_getCapacity_FlashTab_Button;
	private: System::Windows::Forms::Label^  	destination_getCapacity_FlashTab_Label;
	private: System::Windows::Forms::TextBox^  	destination_getCapacity_FlashTab_TextBox;	
	
	// Flash Tab - Read Low Level:
	private: System::Windows::Forms::TabPage^  	readLL_FlashTab_TabPage;
	private: System::Windows::Forms::TextBox^  	readLL_FlashTab_TextBox;
	private: System::Windows::Forms::Label^  	address_readLL_FlashTab_Label;
	private: System::Windows::Forms::TextBox^  	address_readLL_FlashTab_TextBox;
	private: System::Windows::Forms::Button^  	read_readLL_FlashTab_Button;
	private: System::Windows::Forms::Label^  	destination_readLL_FlashTab_Label;
	private: System::Windows::Forms::TextBox^  	destination_readLL_FlashTab_TextBox;	
	
	// Flash Tab - Write Low Level:
	private: System::Windows::Forms::TabPage^  	writeLL_FlashTab_TabPage;
	private: System::Windows::Forms::TextBox^  	writeLL_FlashTab_TextBox;
	private: System::Windows::Forms::Label^  	address_writeLL_FlashTab_Label;
	private: System::Windows::Forms::TextBox^  	address_writeLL_FlashTab_TextBox;
	private: System::Windows::Forms::Label^  	data_writeLL_FlashTab_Label;
	private: System::Windows::Forms::TextBox^  	data_writeLL_FlashTab_TextBox;
	private: System::Windows::Forms::Button^  	write_writeLL_FlashTab_Button;
	private: System::Windows::Forms::Label^  	destination_writeLL_FlashTab_Label;
	private: System::Windows::Forms::TextBox^  	destination_writeLL_FlashTab_TextBox;	
	
	// EEPROM Tab:
	private: System::Windows::Forms::TabPage^  		tabEEPROM;
	private: System::Windows::Forms::TabControl^  	eepromTab;	
	
	// EEPROM Tab - Format:
	private: System::Windows::Forms::TabPage^  	format_EepromTab_TabPage;
	private: System::Windows::Forms::TextBox^  	format_EepromTab_TextBox;
	private: System::Windows::Forms::Button^  	format_format_EepromTab_Button;
	private: System::Windows::Forms::Label^  	destination_format_EepromTab_Label;
	private: System::Windows::Forms::TextBox^  	destination_format_EepromTab_TextBox;	
	
	// EEPROM Tab - Read:
	private: System::Windows::Forms::TabPage^  	read_EepromTab_TabPage;
	private: System::Windows::Forms::TextBox^  	read_EepromTab_TextBox;	
	private: System::Windows::Forms::Label^  	address_read_EepromTab_Label;
	private: System::Windows::Forms::TextBox^  	address_read_EepromTab_TextBox;
	private: System::Windows::Forms::Label^  	numberOfBytes_read_EepromTab_Label;
	private: System::Windows::Forms::TextBox^  	numberOfBytes_read_EepromTab_TextBox;
	private: System::Windows::Forms::Button^  	read_read_EepromTab_Button;
	private: System::Windows::Forms::Label^  	destination_read_EepromTab_Label;
	private: System::Windows::Forms::TextBox^  	destination_read_EepromTab_TextBox;	
	
	// EEPROM Tab - Write:
	private: System::Windows::Forms::TabPage^  	write_EepromTab_TabPage;
	private: System::Windows::Forms::TextBox^  	write_EepromTab_TextBox;
	private: System::Windows::Forms::Label^  	address_write_EepromTab_Label;
	private: System::Windows::Forms::TextBox^  	address_write_EepromTab_TextBox;
	private: System::Windows::Forms::Label^  	data_write_EepromTab_Label;
	private: System::Windows::Forms::TextBox^  	data_write_EepromTab_TextBox;
	private: System::Windows::Forms::Button^  	write_write_EepromTab_Button;
	private: System::Windows::Forms::Label^  	destination_write_EepromTab_Label;
	private: System::Windows::Forms::TextBox^  	destination_write_EepromTab_TextBox;	
	
	// EEPROM Tab - Set Boot:
	private: System::Windows::Forms::TabPage^  	setBoot_EepromTab_TabPage;
	private: System::Windows::Forms::TextBox^  	setBoot_EepromTab_TextBox;
	private: System::Windows::Forms::Label^  	entryNumber_setBoot_EepromTab_Label;
	private: System::Windows::Forms::ComboBox^  entryNumber_setBoot_EepromTab_ComboBox;
	private: System::Windows::Forms::Label^  	command_setBoot_EepromTab_Label;
	private: System::Windows::Forms::TextBox^  	command_setBoot_EepromTab_TextBox;
	private: System::Windows::Forms::Button^  	apply_setBoot_EepromTab_Button;
	private: System::Windows::Forms::Label^  	destination_setBoot_EepromTab_Label;
	private: System::Windows::Forms::TextBox^  	destination_setBoot_EepromTab_TextBox;	
	
	// EEPROM Tab - Get Boot:
	private: System::Windows::Forms::TabPage^  	getBoot_EepromTab_TabPage;
	private: System::Windows::Forms::TextBox^  	getBoot_EepromTab_TextBox;
	private: System::Windows::Forms::Label^  	entryNumber_getBoot_EepromTab_Label;
	private: System::Windows::Forms::ComboBox^  entryNumber_getBoot_EepromTab_ComboBox;
	private: System::Windows::Forms::Button^  	apply_getBoot_EepromTab_Button;
	private: System::Windows::Forms::Label^  	destination_getBoot_EepromBoot_Label;
	private: System::Windows::Forms::TextBox^  	destination_getBoot_EepromTab_TextBox;	
	
	// EEPROM Tab - Read Low Level:
	private: System::Windows::Forms::TabPage^  	readLL_EepromTab_TabPage;
	private: System::Windows::Forms::TextBox^  	readLL_EepromTab_TextBox;
	private: System::Windows::Forms::Label^  	address_readLL_EepromTab_Label;
	private: System::Windows::Forms::TextBox^  	address_readLL_EepromTab_TextBox;
	private: System::Windows::Forms::Button^  	read_readLL_EepromTab_Button;
	private: System::Windows::Forms::Label^  	destination_readLL_EepromTab_Label;
	private: System::Windows::Forms::TextBox^  	destination_readLL_EepromTab_TextBox;	
	
	// EEPROM Tab - Write Low Level:
	private: System::Windows::Forms::TabPage^  	writeLL_EepromTab_TabPage;
	private: System::Windows::Forms::TextBox^  	writeLL_EepromTab_TextBox;
	private: System::Windows::Forms::Label^  	address_writeLL_EepromTab_Label;
	private: System::Windows::Forms::TextBox^  	address_writeLL_EepromTab_TextBox;
	private: System::Windows::Forms::Label^  	data_writeLL_EepromTab_Label;
	private: System::Windows::Forms::TextBox^  	data_writeLL_EepromTab_TextBox;
	private: System::Windows::Forms::Button^  	write_writeLL_EepromTab_Button;
	private: System::Windows::Forms::Label^  	destination_writeLL_EepromTab_Label;
	private: System::Windows::Forms::TextBox^  	destination_writeLL_EepromTab_TextBox;	
	
	// LCD Tab:
	private: System::Windows::Forms::TabPage^  		tabLCD;
	private: System::Windows::Forms::TabControl^  	lcdTab;	
	
	// LCD Tab - Clear Screen:
	private: System::Windows::Forms::TabPage^  	clearScreen_LcdTab_TabPage;
	private: System::Windows::Forms::TextBox^  	clearScreen_LcdTab_TextBox;	
	private: System::Windows::Forms::Label^  	lcd_clearScreen_LcdTab_Label;
	private: System::Windows::Forms::TextBox^  	lcd_clearScreen_LcdTab_TextBox;
	private: System::Windows::Forms::Button^  	clear_clearScreen_LcdTab_Button;
	private: System::Windows::Forms::Label^  	destination_clearScreen_LcdTab_Label;
	private: System::Windows::Forms::TextBox^  	destination_clearScreen_LcdTab_TextBox;
	
	// LCD Tab - Write Low Level:
	private: System::Windows::Forms::TabPage^  	writeLL_LcdTab_TabPage;
	private: System::Windows::Forms::TextBox^  	writeLL_LcdTab_TextBox;
	private: System::Windows::Forms::Label^  	address_writeLL_LcdTab_Label;
	private: System::Windows::Forms::TextBox^  	address_writeLL_LcdTab_TextBox;
	private: System::Windows::Forms::Label^  	data_writeLL_LcdTab_Label;
	private: System::Windows::Forms::TextBox^  	data_writeLL_LcdTab_TextBox;
	private: System::Windows::Forms::Button^  	write_writeLL_LcdTab_Button;
	private: System::Windows::Forms::Label^  	destination_writeLL_LcdTab_Label;
	private: System::Windows::Forms::TextBox^  	destination_writeLL_LcdTab_TextBox;		
	
	// Temperature Tab:
	private: System::Windows::Forms::TabPage^  		tabTemperature;
	private: System::Windows::Forms::TabControl^  	temperatureTab;
	
	// Temperature Tab - Get Temperature:
	private: System::Windows::Forms::TabPage^  	getTemperature_TempTab_TabPage;
	private: System::Windows::Forms::TextBox^  	getTemperature_TempTab_TextBox;
	private: System::Windows::Forms::Button^  	getTemperature_getTemperature_TempTab_Button;
	private: System::Windows::Forms::Label^  	destination_getTemperature_TemperatureTab_Label;
	private: System::Windows::Forms::TextBox^  	destination_getTemperature_TemperatureTab_TextBox;
	
	// Temperature Tab - Read Low Level:
	private: System::Windows::Forms::TabPage^  	readLL_TempTab_TabPage;
	private: System::Windows::Forms::TextBox^  	readLL_TempTab_TextBox;
	private: System::Windows::Forms::Label^  	address_readLL_TempTab_Label;
	private: System::Windows::Forms::TextBox^  	address_readLL_TempTab_TextBox;
	private: System::Windows::Forms::Button^  	read_readLL_TempTab_Button;
	private: System::Windows::Forms::Label^  	destination_readLL_TemperatureTab_Label;
	private: System::Windows::Forms::TextBox^  	destination_readLL_TemperatureTab_TextBox;
	
	// Temperature Tab - Write Low Level:
	private: System::Windows::Forms::TabPage^  	writeLL_TempTab_TabPage;
	private: System::Windows::Forms::TextBox^  	writeLL_TempTab_TextBox;
	private: System::Windows::Forms::Label^  	address_writeLL_TempTab_Label;
	private: System::Windows::Forms::TextBox^  	address_writeLL_TempTab_TextBox;
	private: System::Windows::Forms::Label^  	data_writeLL_TempTab_Label;
	private: System::Windows::Forms::TextBox^  	data_writeLL_TempTab_TextBox;
	private: System::Windows::Forms::Button^  	write_writeLL_TempTab_Button;
	private: System::Windows::Forms::Label^  	destination_writeLL_TemperatureTab_Label;
	private: System::Windows::Forms::TextBox^  	destination_writeLL_TemperatureTab_TextBox;		
	
	// Accelerometer Tab:
	private: System::Windows::Forms::TabPage^  		tabAccelerometer;
	private: System::Windows::Forms::TabControl^  	accelerometerTab;
	
	// Accelerometer Tab - Config:
	private: System::Windows::Forms::TabPage^  		config_AccmtrTab_TabPage;
	private: System::Windows::Forms::TextBox^  		config_AccmtrTab_TextBox;
	private: System::Windows::Forms::Label^  		parameter_config_AccmtrTab_Label;
	private: System::Windows::Forms::ComboBox^  	parameter_config_AccmtrTab_ComboBox;
	private: System::Windows::Forms::Label^  		option_config_AccmtrTab_Label;
	private: System::Windows::Forms::NumericUpDown^ option_config_AccmtrTab_NumericUpDown;
	private: System::Windows::Forms::Button^  		updateValue_config_AccmtrTab_Button;
	private: System::Windows::Forms::Label^  		destination_config_AccmtrTab_Label;
	private: System::Windows::Forms::TextBox^  		destination_config_AccmtrTab_TextBox;
	
	// Accelerometer Tab - Read Low Level:
	private: System::Windows::Forms::TabPage^  		readLL_AccmtrTab_TabPage;
	private: System::Windows::Forms::TextBox^  		readLL_AccmtrTab_TextBox;
	private: System::Windows::Forms::Label^  		address_readLL_AccmtrTab_Label;
	private: System::Windows::Forms::NumericUpDown^ address_readLL_AccmtrTab_NumericUpDown;
	private: System::Windows::Forms::Button^  		read_readLL_AccmtrTab_Button;
	private: System::Windows::Forms::Label^  		destination_readLL_AccmtrTab_Label;
	private: System::Windows::Forms::TextBox^  		destination_readLL_AccmtrTab_TextBox;
		
	// Accelerometer Tab - Write Low Level:
	private: System::Windows::Forms::TabPage^	writeLL_AccmtrTab_TabPage;
	private: System::Windows::Forms::TextBox^  	writeLL_AccmtrTab_TextPage;
	private: System::Windows::Forms::Label^  	address_writeLL_AccmtrTab_Label;
	private: System::Windows::Forms::TextBox^  	address_writeLL_AccmtrTab_TextBox;
	private: System::Windows::Forms::Label^  	data_writeLL_AccmtrTab_Label;
	private: System::Windows::Forms::TextBox^  	data_writeLL_AccmtrTab_TextBox;
	private: System::Windows::Forms::Button^  	write_writeLL_AccmtrTab_Button;
	private: System::Windows::Forms::Label^  	destination_writeLL_AccmtrTab_Label;
	private: System::Windows::Forms::TextBox^  	destination_writeLL_AccmtrTab_TextBox;	
	
	// LED Tab:
	private: System::Windows::Forms::TabPage^  		tabLed;
	private: System::Windows::Forms::TabControl^  	ledTab;	
	
	// LED Tab - Set Led:
	private: System::Windows::Forms::TabPage^  		setLed_LedTab_TabPage;
	private: System::Windows::Forms::TextBox^  		setLed_LedTab_TextBox;
	private: System::Windows::Forms::RadioButton^  	ledOn_setLed_LedTab_RadioButton;
	private: System::Windows::Forms::RadioButton^  	ledOff_setLed_LedTab_RadioButton;
	private: System::Windows::Forms::Button^  		apply_setLed_LedTab_Button;
	private: System::Windows::Forms::Label^  		address_setLed_LedTab_Label;
	private: System::Windows::Forms::ComboBox^  	address_setLed_LedTab_ComboBox;
	private: System::Windows::Forms::Label^  		destination_setLed_LedTab_Label;
	private: System::Windows::Forms::TextBox^  		destination_setLed_LedTab_TextBox;	
	
	// Buzzer Tab:
	private: System::Windows::Forms::TabPage^  		tabBuzzer;
	private: System::Windows::Forms::TabControl^  	buzzerTab;
	
	// Buzzer Tab - Write Low Level:
	private: System::Windows::Forms::TabPage^  		writeLL_BuzzerTab_TabPage;
	private: System::Windows::Forms::TextBox^  		writeLL_BuzzerTab_TextBox;
	private: System::Windows::Forms::Label^  		beepDuration_writeLL_BuzzerTab_Label;
	private: System::Windows::Forms::NumericUpDown^ beepDuration_writeLL_BuzzerTab_NumericUpDown;
	private: System::Windows::Forms::Button^  		apply_writeLL_BuzzerTab_Button;
	private: System::Windows::Forms::Label^  		destination_writeLL_BuzzerTab_Label;
	private: System::Windows::Forms::TextBox^  		destination_writeLL_BuzzerTab_TextBox;	
	
	// Switch Tab:
	private: System::Windows::Forms::TabPage^  		tabSwitch;
	private: System::Windows::Forms::TabControl^  	switchTab;	
	
	// Switch Tab - Read Low Level:
	private: System::Windows::Forms::TabPage^  	readLL_SwitchTab_TabPage;
	private: System::Windows::Forms::TextBox^  	readLL_SwitchTab_TextBox;
	private: System::Windows::Forms::Label^  	switchId_readLL_SwitchTab_Label;
	private: System::Windows::Forms::ComboBox^  switchId_readLL_SwitchTab_ComboBox;
	private: System::Windows::Forms::Label^  	address_readLL_SwitchTab_Label;
	private: System::Windows::Forms::ComboBox^  address_readLL_SwitchTab_ComboBox;
	private: System::Windows::Forms::Button^  	apply_readLL_SwitchTab_Button;
	private: System::Windows::Forms::Label^  	destination_readLL_SwitchTab_Label;
	private: System::Windows::Forms::TextBox^  	destination_readLL_SwitchTab_TextBox;	
	
	// ADC Tab:
	private: System::Windows::Forms::TabPage^  		tabADC;	
	private: System::Windows::Forms::TabControl^  	ADCTab;	
	
	// ADC Tab - Read Low Level:
	private: System::Windows::Forms::TabPage^  	readLL_AdcTab_TabPage;
	private: System::Windows::Forms::TextBox^  	readLL_AdcTab_TextBox;
	private: System::Windows::Forms::Label^  	address_readLL_AdcTab_Label;
	private: System::Windows::Forms::TextBox^  	address_readLL_AdcTab_TextBox;
	private: System::Windows::Forms::Button^  	read_readLL_AdcTab_Button;
	private: System::Windows::Forms::Label^  	data_readLL_AdcTab_Label;
	private: System::Windows::Forms::Label^  	destination_readLL_AdcTab_Label;
	private: System::Windows::Forms::TextBox^  	destination_readLL_AdcTab_TextBox;	
	
	// ADC Tab - Write Low Level:
	private: System::Windows::Forms::TabPage^  	writeLL_AdcTab_TabPage;
	private: System::Windows::Forms::TextBox^  	writeLL_AdcTab_TextBox;
	private: System::Windows::Forms::Label^  	address_writeLL_AdcTab_Label;
	private: System::Windows::Forms::TextBox^  	address_writeLL_AdcTab_TextBox;
	private: System::Windows::Forms::ComboBox^  data_writeLL_AdcTab_ComboBox;
	private: System::Windows::Forms::Button^  	write_writeLL_AdcTab_Button;
	private: System::Windows::Forms::Label^  	destination_writeLL_AdcTab_Label;
	private: System::Windows::Forms::TextBox^  	destination_writeLL_AdcTab_TextBox;	
	
	// Help Tab:
	private: System::Windows::Forms::TabPage^  		tabHelp;
	private: System::Windows::Forms::TabControl^  	helpTab;	
	
	// Help Tab - Introduction:
	private: System::Windows::Forms::TabPage^  introduction_HelpTab_TabPage;
	private: System::Windows::Forms::TextBox^  introduction_HelpTab_TextBox;
	
	// Help Tab - Instruction:
	private: System::Windows::Forms::TabPage^  instruction_HelpTab_TabPage;
	private: System::Windows::Forms::TextBox^  instruction_HelpTab_TextBox;

	// Hub Tab:
	private: System::Windows::Forms::TabPage^		tabHub; // YL 16.6 added
	private: System::Windows::Forms::TabControl^	hubTab; // YL 16.6 added

	// Hub Tab - Statistics:
	private: System::Windows::Forms::TabPage^  statistics_HubTab_TabPage; // YL 16.6 added
	
	// allTabs:
	private: System::Windows::Forms::TabControl^  allTabs;
	
	// Files:
	private: System::Windows::Forms::OpenFileDialog^  openFileDialog1;
	
	// Timers:
	private: System::Windows::Forms::Timer^  comPorts_Timer;
	private: System::Windows::Forms::Timer^  connection_Timer;
	private: System::Windows::Forms::Timer^  commands_Timer;
	private: System::Windows::Forms::Timer^  statistics_Timer;
	private: System::Windows::Forms::Timer^  application_Timer;
	private: System::Windows::Forms::Timer^  sessionTimeout_Timer;
	private: System::Windows::Forms::Timer^  server_Timer; // YL 20.6

	// Ports:
	private: System::IO::Ports::SerialPort^  serialPort1;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(GUI::typeid));
			this->console_TextBox = (gcnew System::Windows::Forms::TextBox());
			this->sendCommands_Button = (gcnew System::Windows::Forms::Button());
			this->sendCommands_TextBox = (gcnew System::Windows::Forms::TextBox());
			this->browse_Button = (gcnew System::Windows::Forms::Button());
			this->scriptFile_TextBox = (gcnew System::Windows::Forms::TextBox());
			this->connect_Button = (gcnew System::Windows::Forms::Button());
			this->comPort_ComboBox = (gcnew System::Windows::Forms::ComboBox());
			this->disconnect_Button = (gcnew System::Windows::Forms::Button());
			this->serialPort1 = (gcnew System::IO::Ports::SerialPort(this->components));
			this->comPorts_Timer = (gcnew System::Windows::Forms::Timer(this->components));
			this->openFileDialog1 = (gcnew System::Windows::Forms::OpenFileDialog());
			this->clearConsole_Button = (gcnew System::Windows::Forms::Button());
			this->connectionTime_TextBox = (gcnew System::Windows::Forms::TextBox());
			this->connection_Timer = (gcnew System::Windows::Forms::Timer(this->components));
			this->commands_Timer = (gcnew System::Windows::Forms::Timer(this->components));
			this->countNumberOfCharacters_Button = (gcnew System::Windows::Forms::Button());
			this->countNumberOfCharacters_TextBox = (gcnew System::Windows::Forms::TextBox());
			this->tabSwitch = (gcnew System::Windows::Forms::TabPage());
			this->switchTab = (gcnew System::Windows::Forms::TabControl());
			this->readLL_SwitchTab_TabPage = (gcnew System::Windows::Forms::TabPage());
			this->destination_readLL_SwitchTab_Label = (gcnew System::Windows::Forms::Label());
			this->destination_readLL_SwitchTab_TextBox = (gcnew System::Windows::Forms::TextBox());
			this->switchId_readLL_SwitchTab_ComboBox = (gcnew System::Windows::Forms::ComboBox());
			this->address_readLL_SwitchTab_Label = (gcnew System::Windows::Forms::Label());
			this->address_readLL_SwitchTab_ComboBox = (gcnew System::Windows::Forms::ComboBox());
			this->apply_readLL_SwitchTab_Button = (gcnew System::Windows::Forms::Button());
			this->switchId_readLL_SwitchTab_Label = (gcnew System::Windows::Forms::Label());
			this->readLL_SwitchTab_TextBox = (gcnew System::Windows::Forms::TextBox());
			this->tabBuzzer = (gcnew System::Windows::Forms::TabPage());
			this->buzzerTab = (gcnew System::Windows::Forms::TabControl());
			this->writeLL_BuzzerTab_TabPage = (gcnew System::Windows::Forms::TabPage());
			this->destination_writeLL_BuzzerTab_Label = (gcnew System::Windows::Forms::Label());
			this->destination_writeLL_BuzzerTab_TextBox = (gcnew System::Windows::Forms::TextBox());
			this->apply_writeLL_BuzzerTab_Button = (gcnew System::Windows::Forms::Button());
			this->beepDuration_writeLL_BuzzerTab_Label = (gcnew System::Windows::Forms::Label());
			this->beepDuration_writeLL_BuzzerTab_NumericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->writeLL_BuzzerTab_TextBox = (gcnew System::Windows::Forms::TextBox());
			this->tabLed = (gcnew System::Windows::Forms::TabPage());
			this->ledTab = (gcnew System::Windows::Forms::TabControl());
			this->setLed_LedTab_TabPage = (gcnew System::Windows::Forms::TabPage());
			this->destination_setLed_LedTab_Label = (gcnew System::Windows::Forms::Label());
			this->destination_setLed_LedTab_TextBox = (gcnew System::Windows::Forms::TextBox());
			this->address_setLed_LedTab_Label = (gcnew System::Windows::Forms::Label());
			this->address_setLed_LedTab_ComboBox = (gcnew System::Windows::Forms::ComboBox());
			this->apply_setLed_LedTab_Button = (gcnew System::Windows::Forms::Button());
			this->ledOff_setLed_LedTab_RadioButton = (gcnew System::Windows::Forms::RadioButton());
			this->ledOn_setLed_LedTab_RadioButton = (gcnew System::Windows::Forms::RadioButton());
			this->setLed_LedTab_TextBox = (gcnew System::Windows::Forms::TextBox());
			this->tabAccelerometer = (gcnew System::Windows::Forms::TabPage());
			this->accelerometerTab = (gcnew System::Windows::Forms::TabControl());
			this->config_AccmtrTab_TabPage = (gcnew System::Windows::Forms::TabPage());
			this->destination_config_AccmtrTab_Label = (gcnew System::Windows::Forms::Label());
			this->destination_config_AccmtrTab_TextBox = (gcnew System::Windows::Forms::TextBox());
			this->parameter_config_AccmtrTab_ComboBox = (gcnew System::Windows::Forms::ComboBox());
			this->option_config_AccmtrTab_NumericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->updateValue_config_AccmtrTab_Button = (gcnew System::Windows::Forms::Button());
			this->parameter_config_AccmtrTab_Label = (gcnew System::Windows::Forms::Label());
			this->config_AccmtrTab_TextBox = (gcnew System::Windows::Forms::TextBox());
			this->option_config_AccmtrTab_Label = (gcnew System::Windows::Forms::Label());
			this->readLL_AccmtrTab_TabPage = (gcnew System::Windows::Forms::TabPage());
			this->destination_readLL_AccmtrTab_Label = (gcnew System::Windows::Forms::Label());
			this->destination_readLL_AccmtrTab_TextBox = (gcnew System::Windows::Forms::TextBox());
			this->address_readLL_AccmtrTab_NumericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->read_readLL_AccmtrTab_Button = (gcnew System::Windows::Forms::Button());
			this->address_readLL_AccmtrTab_Label = (gcnew System::Windows::Forms::Label());
			this->readLL_AccmtrTab_TextBox = (gcnew System::Windows::Forms::TextBox());
			this->writeLL_AccmtrTab_TabPage = (gcnew System::Windows::Forms::TabPage());
			this->destination_writeLL_AccmtrTab_Label = (gcnew System::Windows::Forms::Label());
			this->destination_writeLL_AccmtrTab_TextBox = (gcnew System::Windows::Forms::TextBox());
			this->data_writeLL_AccmtrTab_Label = (gcnew System::Windows::Forms::Label());
			this->address_writeLL_AccmtrTab_Label = (gcnew System::Windows::Forms::Label());
			this->write_writeLL_AccmtrTab_Button = (gcnew System::Windows::Forms::Button());
			this->data_writeLL_AccmtrTab_TextBox = (gcnew System::Windows::Forms::TextBox());
			this->address_writeLL_AccmtrTab_TextBox = (gcnew System::Windows::Forms::TextBox());
			this->writeLL_AccmtrTab_TextPage = (gcnew System::Windows::Forms::TextBox());
			this->tabTemperature = (gcnew System::Windows::Forms::TabPage());
			this->temperatureTab = (gcnew System::Windows::Forms::TabControl());
			this->getTemperature_TempTab_TabPage = (gcnew System::Windows::Forms::TabPage());
			this->destination_getTemperature_TemperatureTab_Label = (gcnew System::Windows::Forms::Label());
			this->destination_getTemperature_TemperatureTab_TextBox = (gcnew System::Windows::Forms::TextBox());
			this->getTemperature_getTemperature_TempTab_Button = (gcnew System::Windows::Forms::Button());
			this->getTemperature_TempTab_TextBox = (gcnew System::Windows::Forms::TextBox());
			this->readLL_TempTab_TabPage = (gcnew System::Windows::Forms::TabPage());
			this->destination_readLL_TemperatureTab_Label = (gcnew System::Windows::Forms::Label());
			this->destination_readLL_TemperatureTab_TextBox = (gcnew System::Windows::Forms::TextBox());
			this->read_readLL_TempTab_Button = (gcnew System::Windows::Forms::Button());
			this->address_readLL_TempTab_Label = (gcnew System::Windows::Forms::Label());
			this->address_readLL_TempTab_TextBox = (gcnew System::Windows::Forms::TextBox());
			this->readLL_TempTab_TextBox = (gcnew System::Windows::Forms::TextBox());
			this->writeLL_TempTab_TabPage = (gcnew System::Windows::Forms::TabPage());
			this->destination_writeLL_TemperatureTab_Label = (gcnew System::Windows::Forms::Label());
			this->destination_writeLL_TemperatureTab_TextBox = (gcnew System::Windows::Forms::TextBox());
			this->write_writeLL_TempTab_Button = (gcnew System::Windows::Forms::Button());
			this->data_writeLL_TempTab_Label = (gcnew System::Windows::Forms::Label());
			this->data_writeLL_TempTab_TextBox = (gcnew System::Windows::Forms::TextBox());
			this->address_writeLL_TempTab_TextBox = (gcnew System::Windows::Forms::TextBox());
			this->writeLL_TempTab_TextBox = (gcnew System::Windows::Forms::TextBox());
			this->address_writeLL_TempTab_Label = (gcnew System::Windows::Forms::Label());
			this->tabWakeUp = (gcnew System::Windows::Forms::TabPage());
			this->wakeUpTab = (gcnew System::Windows::Forms::TabControl());
			this->setTime_WakeUpTab_TabPage = (gcnew System::Windows::Forms::TabPage());
			this->destination_setTime_WakeUpTab_Label = (gcnew System::Windows::Forms::Label());
			this->destination_setTime_WakeUpTab_TextBox = (gcnew System::Windows::Forms::TextBox());
			this->setTime_setTime_WakeUpTab_Button = (gcnew System::Windows::Forms::Button());
			this->second_setTime_WakeUpTab_Label = (gcnew System::Windows::Forms::Label());
			this->minute_setTime_WakeUpTab_Label = (gcnew System::Windows::Forms::Label());
			this->hour_setTime_WakeUpTab_Label = (gcnew System::Windows::Forms::Label());
			this->second_setTime_WakeUpTab_NumericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->minute_setTime_WakeUpTab_NumericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->hour_setTime_WakeUpTab_NumericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->setTime_WakeUpTab_TextBox = (gcnew System::Windows::Forms::TextBox());
			this->setDate_WakeUpTab_TabPage = (gcnew System::Windows::Forms::TabPage());
			this->destination_setDate_WakeUpTab_Label = (gcnew System::Windows::Forms::Label());
			this->destination_setDate_WakeUpTab_TextBox = (gcnew System::Windows::Forms::TextBox());
			this->weekDay_setDate_WakeUpTab_Label = (gcnew System::Windows::Forms::Label());
			this->weekDay_setDate_WakeUpTab_NumericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->setDate_setDate_WakeUpTab_Button = (gcnew System::Windows::Forms::Button());
			this->year_setDate_WakeUpTab_Label = (gcnew System::Windows::Forms::Label());
			this->month_setDate_WakeUpTab_Label = (gcnew System::Windows::Forms::Label());
			this->day_setDate_WakeUpTab_Label = (gcnew System::Windows::Forms::Label());
			this->year_setDate_WakeUpTab_NumericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->month_setDate_WakeUpTab_NumericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->day_setDate_WakeUpTab_NumericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->setDate_WakeUpTab_TextBox = (gcnew System::Windows::Forms::TextBox());
			this->getTime_WakeUpTab_TabPage = (gcnew System::Windows::Forms::TabPage());
			this->destination_getTime_WakeUpTab_Label = (gcnew System::Windows::Forms::Label());
			this->destination_getTime_WakeUpTab_TextBox = (gcnew System::Windows::Forms::TextBox());
			this->getTime_getTime_WakeUpTab_Button = (gcnew System::Windows::Forms::Button());
			this->getTime_WakeUpTab_TextBox = (gcnew System::Windows::Forms::TextBox());
			this->getDate_WakeUpTab_TabPage = (gcnew System::Windows::Forms::TabPage());
			this->destination_getDate_WakeUpTab_Label = (gcnew System::Windows::Forms::Label());
			this->destination_getDate_WakeUpTab_TextBox = (gcnew System::Windows::Forms::TextBox());
			this->getDate_getDate_WakeUpTab_Button = (gcnew System::Windows::Forms::Button());
			this->getDate_WakeUpTab_TextBox = (gcnew System::Windows::Forms::TextBox());
			this->setWakeUp_WakeUpTab_TabPage = (gcnew System::Windows::Forms::TabPage());
			this->destination_setWakeUp_WakeUpTab_Label = (gcnew System::Windows::Forms::Label());
			this->destination_setWakeUp_WakeUpTab_TextBox = (gcnew System::Windows::Forms::TextBox());
			this->delay_setWakeUp_WakeUpTab_TextBox = (gcnew System::Windows::Forms::TextBox());
			this->setWakeUp_WakeUpTab_TextBox = (gcnew System::Windows::Forms::TextBox());
			this->delay_setWakeUp_WakeUpTab_Label = (gcnew System::Windows::Forms::Label());
			this->apply_setWakeUp_WakeUpTab_Button = (gcnew System::Windows::Forms::Button());
			this->setAlarm_WakeUpTab_TabPage = (gcnew System::Windows::Forms::TabPage());
			this->destination_setAlarm_WakeUpTab_Label = (gcnew System::Windows::Forms::Label());
			this->destination_setAlarm_WakeUpTab_TextBox = (gcnew System::Windows::Forms::TextBox());
			this->minute_setAlarm_WakeUpTab_TextBox = (gcnew System::Windows::Forms::TextBox());
			this->day_setAlarm_WakeUpTab_TextBox = (gcnew System::Windows::Forms::TextBox());
			this->weekDay_setAlarm_WakeUpTab_TextBox = (gcnew System::Windows::Forms::TextBox());
			this->hour_setAlarm_WakeUpTab_TextBox = (gcnew System::Windows::Forms::TextBox());
			this->setAlarm_WakeUpTab_TextBox = (gcnew System::Windows::Forms::TextBox());
			this->weekDay_setAlarm_WakeUpTab_Label = (gcnew System::Windows::Forms::Label());
			this->apply_setAlarm_WakeUpTab_Button = (gcnew System::Windows::Forms::Button());
			this->day_setAlarm_WakeUpTab_Label = (gcnew System::Windows::Forms::Label());
			this->minute_setAlarm_WakeUpTab_Label = (gcnew System::Windows::Forms::Label());
			this->hour_setAlarm_WakeUpTab_Label = (gcnew System::Windows::Forms::Label());
			this->setBoot_WakeUpTab_TabPage = (gcnew System::Windows::Forms::TabPage());
			this->destination_setBoot_WakeUpTab_Label = (gcnew System::Windows::Forms::Label());
			this->destination_setBoot_WakeUpTab_TextBox = (gcnew System::Windows::Forms::TextBox());
			this->command_setBoot_WakeUpTab_TextBox = (gcnew System::Windows::Forms::TextBox());
			this->command_setBoot_WakeUpTab_Label = (gcnew System::Windows::Forms::Label());
			this->apply_setBoot_WakeUpTab_Button = (gcnew System::Windows::Forms::Button());
			this->entryNumber_setBoot_WakeUpTab_Label = (gcnew System::Windows::Forms::Label());
			this->setBoot_WakeUpTab_TextBox = (gcnew System::Windows::Forms::TextBox());
			this->entryNumber_setBoot_WakeUpTab_ComboBox = (gcnew System::Windows::Forms::ComboBox());
			this->readLL_WakeUpTab_TabPage = (gcnew System::Windows::Forms::TabPage());
			this->destination_readLL_WakeUpTab_Label = (gcnew System::Windows::Forms::Label());
			this->destination_readLL_WakeUpTab_TextBox = (gcnew System::Windows::Forms::TextBox());
			this->read_readLL_WakeUpTab_Button = (gcnew System::Windows::Forms::Button());
			this->address_readLL_WakeUpTab_Label = (gcnew System::Windows::Forms::Label());
			this->address_readLL_WakeUpTab_TextBox = (gcnew System::Windows::Forms::TextBox());
			this->readLL_WakeUpTab_TextBox = (gcnew System::Windows::Forms::TextBox());
			this->writeLL_WakeUpTab_TabPage = (gcnew System::Windows::Forms::TabPage());
			this->destination_writeLL_WakeUpTab_Label = (gcnew System::Windows::Forms::Label());
			this->destination_writeLL_WakeUpTab_TextBox = (gcnew System::Windows::Forms::TextBox());
			this->write_writeLL_WakeUpTab_Button = (gcnew System::Windows::Forms::Button());
			this->data_writeLL_WakeUpTab_Label = (gcnew System::Windows::Forms::Label());
			this->data_writeLL_WakeUpTab_TextBox = (gcnew System::Windows::Forms::TextBox());
			this->address_writeLL_WakeUpTab_TextBox = (gcnew System::Windows::Forms::TextBox());
			this->writeLL_WakeUpTab_TextBox = (gcnew System::Windows::Forms::TextBox());
			this->address_writeLL_WakeUpTab_Label = (gcnew System::Windows::Forms::Label());
			this->tabLCD = (gcnew System::Windows::Forms::TabPage());
			this->lcdTab = (gcnew System::Windows::Forms::TabControl());
			this->clearScreen_LcdTab_TabPage = (gcnew System::Windows::Forms::TabPage());
			this->destination_clearScreen_LcdTab_Label = (gcnew System::Windows::Forms::Label());
			this->destination_clearScreen_LcdTab_TextBox = (gcnew System::Windows::Forms::TextBox());
			this->lcd_clearScreen_LcdTab_TextBox = (gcnew System::Windows::Forms::TextBox());
			this->lcd_clearScreen_LcdTab_Label = (gcnew System::Windows::Forms::Label());
			this->clear_clearScreen_LcdTab_Button = (gcnew System::Windows::Forms::Button());
			this->clearScreen_LcdTab_TextBox = (gcnew System::Windows::Forms::TextBox());
			this->writeLL_LcdTab_TabPage = (gcnew System::Windows::Forms::TabPage());
			this->destination_writeLL_LcdTab_Label = (gcnew System::Windows::Forms::Label());
			this->destination_writeLL_LcdTab_TextBox = (gcnew System::Windows::Forms::TextBox());
			this->write_writeLL_LcdTab_Button = (gcnew System::Windows::Forms::Button());
			this->data_writeLL_LcdTab_Label = (gcnew System::Windows::Forms::Label());
			this->data_writeLL_LcdTab_TextBox = (gcnew System::Windows::Forms::TextBox());
			this->address_writeLL_LcdTab_TextBox = (gcnew System::Windows::Forms::TextBox());
			this->writeLL_LcdTab_TextBox = (gcnew System::Windows::Forms::TextBox());
			this->address_writeLL_LcdTab_Label = (gcnew System::Windows::Forms::Label());
			this->tabFlash = (gcnew System::Windows::Forms::TabPage());
			this->flashTab = (gcnew System::Windows::Forms::TabControl());
			this->format_FlashTab_TabPage = (gcnew System::Windows::Forms::TabPage());
			this->destination_format_FlashTab_Label = (gcnew System::Windows::Forms::Label());
			this->destination_format_FlashTab_TextBox = (gcnew System::Windows::Forms::TextBox());
			this->format_format_FlashTab_Button = (gcnew System::Windows::Forms::Button());
			this->format_FlashTab_TextBox = (gcnew System::Windows::Forms::TextBox());
			this->initialize_FlashTab_TabPage = (gcnew System::Windows::Forms::TabPage());
			this->destination_initialize_FlashTab_Label = (gcnew System::Windows::Forms::Label());
			this->destination_initialize_FlashTab_TextBox = (gcnew System::Windows::Forms::TextBox());
			this->initialize_initialize_FlashTab_Button = (gcnew System::Windows::Forms::Button());
			this->initialize_FlashTab_TextBox = (gcnew System::Windows::Forms::TextBox());
			this->cardDetect_FlashTab_TabPage = (gcnew System::Windows::Forms::TabPage());
			this->destination_cardDetect_FlashTab_Label = (gcnew System::Windows::Forms::Label());
			this->destination_cardDetect_FlashTab_TextBox = (gcnew System::Windows::Forms::TextBox());
			this->cardDetect_cardDetect_FlashTab_Button = (gcnew System::Windows::Forms::Button());
			this->cardDetect_FlashTab_TextBox = (gcnew System::Windows::Forms::TextBox());
			this->writeSector_FlashTab_TabPage = (gcnew System::Windows::Forms::TabPage());
			this->destination_writeSector_FlashTab_Label = (gcnew System::Windows::Forms::Label());
			this->destination_writeSector_FlashTab_TextBox = (gcnew System::Windows::Forms::TextBox());
			this->write_writeSector_FlashTab_Button = (gcnew System::Windows::Forms::Button());
			this->data_writeSector_FlashTab_Label = (gcnew System::Windows::Forms::Label());
			this->data_writeSector_FlashTab_TextBox = (gcnew System::Windows::Forms::TextBox());
			this->address_writeSector_FlashTab_TextBox = (gcnew System::Windows::Forms::TextBox());
			this->writeSector_FlashTab_TextBox = (gcnew System::Windows::Forms::TextBox());
			this->address_writeSector_FlashTab_Label = (gcnew System::Windows::Forms::Label());
			this->readSector_FlashTab_TabPage = (gcnew System::Windows::Forms::TabPage());
			this->destination_readSector_FlashTab_Label = (gcnew System::Windows::Forms::Label());
			this->destination_readSector_FlashTab_TextBox = (gcnew System::Windows::Forms::TextBox());
			this->read_readSector_FlashTab_Button = (gcnew System::Windows::Forms::Button());
			this->address_readSector_FlashTab_Label = (gcnew System::Windows::Forms::Label());
			this->address_readSector_FlashTab_TextBox = (gcnew System::Windows::Forms::TextBox());
			this->readSector_FlashTab_TextBox = (gcnew System::Windows::Forms::TextBox());
			this->getCapacity_FlashTab_TabPage = (gcnew System::Windows::Forms::TabPage());
			this->destination_getCapacity_FlashTab_Label = (gcnew System::Windows::Forms::Label());
			this->destination_getCapacity_FlashTab_TextBox = (gcnew System::Windows::Forms::TextBox());
			this->getCapacity_getCapacity_FlashTab_Button = (gcnew System::Windows::Forms::Button());
			this->getCapacity_FlashTab_TextBox = (gcnew System::Windows::Forms::TextBox());
			this->readLL_FlashTab_TabPage = (gcnew System::Windows::Forms::TabPage());
			this->destination_readLL_FlashTab_Label = (gcnew System::Windows::Forms::Label());
			this->destination_readLL_FlashTab_TextBox = (gcnew System::Windows::Forms::TextBox());
			this->read_readLL_FlashTab_Button = (gcnew System::Windows::Forms::Button());
			this->address_readLL_FlashTab_Label = (gcnew System::Windows::Forms::Label());
			this->address_readLL_FlashTab_TextBox = (gcnew System::Windows::Forms::TextBox());
			this->readLL_FlashTab_TextBox = (gcnew System::Windows::Forms::TextBox());
			this->writeLL_FlashTab_TabPage = (gcnew System::Windows::Forms::TabPage());
			this->destination_writeLL_FlashTab_Label = (gcnew System::Windows::Forms::Label());
			this->destination_writeLL_FlashTab_TextBox = (gcnew System::Windows::Forms::TextBox());
			this->write_writeLL_FlashTab_Button = (gcnew System::Windows::Forms::Button());
			this->data_writeLL_FlashTab_Label = (gcnew System::Windows::Forms::Label());
			this->data_writeLL_FlashTab_TextBox = (gcnew System::Windows::Forms::TextBox());
			this->address_writeLL_FlashTab_TextBox = (gcnew System::Windows::Forms::TextBox());
			this->writeLL_FlashTab_TextBox = (gcnew System::Windows::Forms::TextBox());
			this->address_writeLL_FlashTab_Label = (gcnew System::Windows::Forms::Label());
			this->tabApplication = (gcnew System::Windows::Forms::TabPage());
			this->applicationTab = (gcnew System::Windows::Forms::TabControl());
			this->start_AppTab_TabPage = (gcnew System::Windows::Forms::TabPage());
			this->destination_start_AppTab_TextBox = (gcnew System::Windows::Forms::TextBox());
			this->destination_start_AppTab_Label = (gcnew System::Windows::Forms::Label());
			this->sessionTimeout_start_AppTab_Label = (gcnew System::Windows::Forms::Label());
			this->sessionTimeout_start_AppTab_TextBox = (gcnew System::Windows::Forms::TextBox());
			this->stopApplication_start_AppTab_Button = (gcnew System::Windows::Forms::Button());
			this->outputFile_start_AppTab_Label = (gcnew System::Windows::Forms::Label());
			this->applicationMode_GroupBox = (gcnew System::Windows::Forms::GroupBox());
			this->ss_RadioButton = (gcnew System::Windows::Forms::RadioButton());
			this->ts_RadioButton = (gcnew System::Windows::Forms::RadioButton());
			this->ost_RadioButton = (gcnew System::Windows::Forms::RadioButton());
			this->combination_start_AppTab_Label = (gcnew System::Windows::Forms::Label());
			this->combination_start_AppTab_ComboBox = (gcnew System::Windows::Forms::ComboBox());
			this->startApplication_start_AppTab_Button = (gcnew System::Windows::Forms::Button());
			this->address_start_AppTab_Label = (gcnew System::Windows::Forms::Label());
			this->address_start_AppTab_TextBox = (gcnew System::Windows::Forms::TextBox());
			this->numberOfBlocks_start_AppTab_Label = (gcnew System::Windows::Forms::Label());
			this->numberOfBlocks_start_AppTab_TextBox = (gcnew System::Windows::Forms::TextBox());
			this->communication_start_AppTab_Label = (gcnew System::Windows::Forms::Label());
			this->communication_start_AppTab_ComboBox = (gcnew System::Windows::Forms::ComboBox());
			this->outputFile_start_AppTab_TextBox = (gcnew System::Windows::Forms::TextBox());
			this->start_AppTab_TextBox = (gcnew System::Windows::Forms::TextBox());
			this->shutdown_AppTab_TabPage = (gcnew System::Windows::Forms::TabPage());
			this->destination_shutdown_AppTab_Label = (gcnew System::Windows::Forms::Label());
			this->destination_shutdown_AppTab_TextBox = (gcnew System::Windows::Forms::TextBox());
			this->shutdown_shutdown_AppTab_Button = (gcnew System::Windows::Forms::Button());
			this->shutdown_AppTab_TextBox = (gcnew System::Windows::Forms::TextBox());
			this->config_AppTab_TabPage = (gcnew System::Windows::Forms::TabPage());
			this->destination_config_AppTab_Label = (gcnew System::Windows::Forms::Label());
			this->destination_config_AppTab_TextBox = (gcnew System::Windows::Forms::TextBox());
			this->parameter_config_AppTab_ComboBox = (gcnew System::Windows::Forms::ComboBox());
			this->value_config_AppTab_NumericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->updateValue_config_AppTab_Button = (gcnew System::Windows::Forms::Button());
			this->parameter_config_AppTab_Label = (gcnew System::Windows::Forms::Label());
			this->config_AppTab_TextBox = (gcnew System::Windows::Forms::TextBox());
			this->value_config_AppTab_Label = (gcnew System::Windows::Forms::Label());
			this->tabSystem = (gcnew System::Windows::Forms::TabPage());
			this->systemTab = (gcnew System::Windows::Forms::TabControl());
			this->systemGetVersion_SystemTab_TabPage = (gcnew System::Windows::Forms::TabPage());
			this->destination_getSystemVersion_SystemTab_Label = (gcnew System::Windows::Forms::Label());
			this->destination_getSystemVersion_SystemTab_TextBox = (gcnew System::Windows::Forms::TextBox());
			this->systemGetVersion_systemGetVersion_SystemTab_Button = (gcnew System::Windows::Forms::Button());
			this->systemGetVersion_SystemTab_TextBox = (gcnew System::Windows::Forms::TextBox());
			this->systemSetId_SystemTab_TabPage = (gcnew System::Windows::Forms::TabPage());
			this->destination_setSystemId_SystemTab_Label = (gcnew System::Windows::Forms::Label());
			this->destination_setSystemId_SystemTab_TextBox = (gcnew System::Windows::Forms::TextBox());
			this->systemSetId_SystemTab_TextBox = (gcnew System::Windows::Forms::TextBox());
			this->systemSetId_systemSetId_SystemTab_Button = (gcnew System::Windows::Forms::Button());
			this->systemId_systemSetId_SystemTab_Label = (gcnew System::Windows::Forms::Label());
			this->systemId_systemSetId_SystemTab_TextBox = (gcnew System::Windows::Forms::TextBox());
			this->getPower_SystemTab_TabPage = (gcnew System::Windows::Forms::TabPage());
			this->destination_getPower_SystemTab_Label = (gcnew System::Windows::Forms::Label());
			this->destination_getPower_SystemTab_TextBox = (gcnew System::Windows::Forms::TextBox());
			this->getPower_getPower_SystemTab_Button = (gcnew System::Windows::Forms::Button());
			this->getPower_SystemTab_TextBox = (gcnew System::Windows::Forms::TextBox());
			this->setPower_SystemTab_TabPage = (gcnew System::Windows::Forms::TabPage());
			this->destination_setPower_SystemTab_Label = (gcnew System::Windows::Forms::Label());
			this->destination_setPower_SystemTab_TextBox = (gcnew System::Windows::Forms::TextBox());
			this->enableM12V_setPower_SystemTab_Label = (gcnew System::Windows::Forms::Label());
			this->enable12V_setPower_SystemTab_Label = (gcnew System::Windows::Forms::Label());
			this->enable5V_setPower_SystemTab_Label = (gcnew System::Windows::Forms::Label());
			this->enableM12V_setPower_SystemTab_ComboBox = (gcnew System::Windows::Forms::ComboBox());
			this->enable12V_setPower_SystemTab_ComboBox = (gcnew System::Windows::Forms::ComboBox());
			this->enable5V_setPower_SystemTab_ComboBox = (gcnew System::Windows::Forms::ComboBox());
			this->setPower_setPower_SystemTab_Button = (gcnew System::Windows::Forms::Button());
			this->setPower_SystemTab_TextBox = (gcnew System::Windows::Forms::TextBox());
			this->getLastError_SystemTab_TabPage = (gcnew System::Windows::Forms::TabPage());
			this->destination_getLastError_SystemTab_Label = (gcnew System::Windows::Forms::Label());
			this->destination_getLastError_SystemTab_TextBox = (gcnew System::Windows::Forms::TextBox());
			this->getLastError_SystemTab_TextBox = (gcnew System::Windows::Forms::TextBox());
			this->getLastError_getLastError_SystemTab_Button = (gcnew System::Windows::Forms::Button());
			this->tabHelp = (gcnew System::Windows::Forms::TabPage());
			this->helpTab = (gcnew System::Windows::Forms::TabControl());
			this->introduction_HelpTab_TabPage = (gcnew System::Windows::Forms::TabPage());
			this->introduction_HelpTab_TextBox = (gcnew System::Windows::Forms::TextBox());
			this->instruction_HelpTab_TabPage = (gcnew System::Windows::Forms::TabPage());
			this->instruction_HelpTab_TextBox = (gcnew System::Windows::Forms::TextBox());
			this->allTabs = (gcnew System::Windows::Forms::TabControl());
			this->tabEEPROM = (gcnew System::Windows::Forms::TabPage());
			this->eepromTab = (gcnew System::Windows::Forms::TabControl());
			this->format_EepromTab_TabPage = (gcnew System::Windows::Forms::TabPage());
			this->destination_format_EepromTab_Label = (gcnew System::Windows::Forms::Label());
			this->destination_format_EepromTab_TextBox = (gcnew System::Windows::Forms::TextBox());
			this->format_format_EepromTab_Button = (gcnew System::Windows::Forms::Button());
			this->format_EepromTab_TextBox = (gcnew System::Windows::Forms::TextBox());
			this->read_EepromTab_TabPage = (gcnew System::Windows::Forms::TabPage());
			this->destination_read_EepromTab_Label = (gcnew System::Windows::Forms::Label());
			this->destination_read_EepromTab_TextBox = (gcnew System::Windows::Forms::TextBox());
			this->numberOfBytes_read_EepromTab_Label = (gcnew System::Windows::Forms::Label());
			this->numberOfBytes_read_EepromTab_TextBox = (gcnew System::Windows::Forms::TextBox());
			this->read_read_EepromTab_Button = (gcnew System::Windows::Forms::Button());
			this->address_read_EepromTab_Label = (gcnew System::Windows::Forms::Label());
			this->address_read_EepromTab_TextBox = (gcnew System::Windows::Forms::TextBox());
			this->read_EepromTab_TextBox = (gcnew System::Windows::Forms::TextBox());
			this->write_EepromTab_TabPage = (gcnew System::Windows::Forms::TabPage());
			this->destination_write_EepromTab_Label = (gcnew System::Windows::Forms::Label());
			this->destination_write_EepromTab_TextBox = (gcnew System::Windows::Forms::TextBox());
			this->write_write_EepromTab_Button = (gcnew System::Windows::Forms::Button());
			this->data_write_EepromTab_Label = (gcnew System::Windows::Forms::Label());
			this->data_write_EepromTab_TextBox = (gcnew System::Windows::Forms::TextBox());
			this->address_write_EepromTab_TextBox = (gcnew System::Windows::Forms::TextBox());
			this->write_EepromTab_TextBox = (gcnew System::Windows::Forms::TextBox());
			this->address_write_EepromTab_Label = (gcnew System::Windows::Forms::Label());
			this->setBoot_EepromTab_TabPage = (gcnew System::Windows::Forms::TabPage());
			this->destination_setBoot_EepromTab_Label = (gcnew System::Windows::Forms::Label());
			this->destination_setBoot_EepromTab_TextBox = (gcnew System::Windows::Forms::TextBox());
			this->command_setBoot_EepromTab_TextBox = (gcnew System::Windows::Forms::TextBox());
			this->command_setBoot_EepromTab_Label = (gcnew System::Windows::Forms::Label());
			this->apply_setBoot_EepromTab_Button = (gcnew System::Windows::Forms::Button());
			this->entryNumber_setBoot_EepromTab_Label = (gcnew System::Windows::Forms::Label());
			this->setBoot_EepromTab_TextBox = (gcnew System::Windows::Forms::TextBox());
			this->entryNumber_setBoot_EepromTab_ComboBox = (gcnew System::Windows::Forms::ComboBox());
			this->getBoot_EepromTab_TabPage = (gcnew System::Windows::Forms::TabPage());
			this->destination_getBoot_EepromBoot_Label = (gcnew System::Windows::Forms::Label());
			this->destination_getBoot_EepromTab_TextBox = (gcnew System::Windows::Forms::TextBox());
			this->apply_getBoot_EepromTab_Button = (gcnew System::Windows::Forms::Button());
			this->entryNumber_getBoot_EepromTab_Label = (gcnew System::Windows::Forms::Label());
			this->getBoot_EepromTab_TextBox = (gcnew System::Windows::Forms::TextBox());
			this->entryNumber_getBoot_EepromTab_ComboBox = (gcnew System::Windows::Forms::ComboBox());
			this->readLL_EepromTab_TabPage = (gcnew System::Windows::Forms::TabPage());
			this->destination_readLL_EepromTab_Label = (gcnew System::Windows::Forms::Label());
			this->destination_readLL_EepromTab_TextBox = (gcnew System::Windows::Forms::TextBox());
			this->read_readLL_EepromTab_Button = (gcnew System::Windows::Forms::Button());
			this->address_readLL_EepromTab_Label = (gcnew System::Windows::Forms::Label());
			this->address_readLL_EepromTab_TextBox = (gcnew System::Windows::Forms::TextBox());
			this->readLL_EepromTab_TextBox = (gcnew System::Windows::Forms::TextBox());
			this->writeLL_EepromTab_TabPage = (gcnew System::Windows::Forms::TabPage());
			this->destination_writeLL_EepromTab_Label = (gcnew System::Windows::Forms::Label());
			this->destination_writeLL_EepromTab_TextBox = (gcnew System::Windows::Forms::TextBox());
			this->write_writeLL_EepromTab_Button = (gcnew System::Windows::Forms::Button());
			this->data_writeLL_EepromTab_Label = (gcnew System::Windows::Forms::Label());
			this->data_writeLL_EepromTab_TextBox = (gcnew System::Windows::Forms::TextBox());
			this->address_writeLL_EepromTab_TextBox = (gcnew System::Windows::Forms::TextBox());
			this->writeLL_EepromTab_TextBox = (gcnew System::Windows::Forms::TextBox());
			this->address_writeLL_EepromTab_Label = (gcnew System::Windows::Forms::Label());
			this->tabADC = (gcnew System::Windows::Forms::TabPage());
			this->ADCTab = (gcnew System::Windows::Forms::TabControl());
			this->readLL_AdcTab_TabPage = (gcnew System::Windows::Forms::TabPage());
			this->destination_readLL_AdcTab_Label = (gcnew System::Windows::Forms::Label());
			this->destination_readLL_AdcTab_TextBox = (gcnew System::Windows::Forms::TextBox());
			this->read_readLL_AdcTab_Button = (gcnew System::Windows::Forms::Button());
			this->address_readLL_AdcTab_Label = (gcnew System::Windows::Forms::Label());
			this->address_readLL_AdcTab_TextBox = (gcnew System::Windows::Forms::TextBox());
			this->readLL_AdcTab_TextBox = (gcnew System::Windows::Forms::TextBox());
			this->writeLL_AdcTab_TabPage = (gcnew System::Windows::Forms::TabPage());
			this->destination_writeLL_AdcTab_Label = (gcnew System::Windows::Forms::Label());
			this->destination_writeLL_AdcTab_TextBox = (gcnew System::Windows::Forms::TextBox());
			this->data_writeLL_AdcTab_ComboBox = (gcnew System::Windows::Forms::ComboBox());
			this->write_writeLL_AdcTab_Button = (gcnew System::Windows::Forms::Button());
			this->data_readLL_AdcTab_Label = (gcnew System::Windows::Forms::Label());
			this->address_writeLL_AdcTab_TextBox = (gcnew System::Windows::Forms::TextBox());
			this->writeLL_AdcTab_TextBox = (gcnew System::Windows::Forms::TextBox());
			this->address_writeLL_AdcTab_Label = (gcnew System::Windows::Forms::Label());
			this->tabHub = (gcnew System::Windows::Forms::TabPage());
			this->hubTab = (gcnew System::Windows::Forms::TabControl());
			this->statistics_HubTab_TabPage = (gcnew System::Windows::Forms::TabPage());
			this->x_TextBox = (gcnew System::Windows::Forms::TextBox());
			this->z_TextBox = (gcnew System::Windows::Forms::TextBox());
			this->y_TextBox = (gcnew System::Windows::Forms::TextBox());
			this->z_Label = (gcnew System::Windows::Forms::Label());
			this->x_Label = (gcnew System::Windows::Forms::Label());
			this->y_Label = (gcnew System::Windows::Forms::Label());
			this->meanOfAxes_Label = (gcnew System::Windows::Forms::Label());
			this->packetErrorRate_TextBox = (gcnew System::Windows::Forms::TextBox());
			this->statistics_Timer = (gcnew System::Windows::Forms::Timer(this->components));
			this->application_Timer = (gcnew System::Windows::Forms::Timer(this->components));
			this->bytes_Label = (gcnew System::Windows::Forms::Label());
			this->bytes_TextBox = (gcnew System::Windows::Forms::TextBox());
			this->hw_TextBox = (gcnew System::Windows::Forms::TextBox());
			this->hw_Label = (gcnew System::Windows::Forms::Label());
			this->sw_Label = (gcnew System::Windows::Forms::Label());
			this->sw_TextBox = (gcnew System::Windows::Forms::TextBox());
			this->blocks_Label = (gcnew System::Windows::Forms::Label());
			this->blocks_TextBox = (gcnew System::Windows::Forms::TextBox());
			this->scriptFile_Label = (gcnew System::Windows::Forms::Label());
			this->commandScript_Label = (gcnew System::Windows::Forms::Label());
			this->connection_GroupBox = (gcnew System::Windows::Forms::GroupBox());
			this->internet_RadioButton = (gcnew System::Windows::Forms::RadioButton());
			this->comPort_RadioButton = (gcnew System::Windows::Forms::RadioButton());
			this->password_TextBox = (gcnew System::Windows::Forms::TextBox());
			this->userName_TextBox = (gcnew System::Windows::Forms::TextBox());
			this->userName_Label = (gcnew System::Windows::Forms::Label());
			this->password_Label = (gcnew System::Windows::Forms::Label());
			this->login_Button = (gcnew System::Windows::Forms::Button());
			this->login_GroupBox = (gcnew System::Windows::Forms::GroupBox());
			this->console_GroupBox = (gcnew System::Windows::Forms::GroupBox());
			this->debugInformation_GroupBox = (gcnew System::Windows::Forms::GroupBox());
			this->receivedDataCounters_Label = (gcnew System::Windows::Forms::Label());
			this->sensorOverflow_Label = (gcnew System::Windows::Forms::Label());
			this->packetErrorRate_Label = (gcnew System::Windows::Forms::Label());
			this->errorRates_Label = (gcnew System::Windows::Forms::Label());
			this->commandLineInterface_GroupBox = (gcnew System::Windows::Forms::GroupBox());
			this->sessionTimeout_Timer = (gcnew System::Windows::Forms::Timer(this->components));
			this->outputFile_start_AppTab_ToolTip = (gcnew System::Windows::Forms::ToolTip(this->components));
			this->server_Timer = (gcnew System::Windows::Forms::Timer(this->components));
			this->tabSwitch->SuspendLayout();
			this->switchTab->SuspendLayout();
			this->readLL_SwitchTab_TabPage->SuspendLayout();
			this->tabBuzzer->SuspendLayout();
			this->buzzerTab->SuspendLayout();
			this->writeLL_BuzzerTab_TabPage->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->beepDuration_writeLL_BuzzerTab_NumericUpDown))->BeginInit();
			this->tabLed->SuspendLayout();
			this->ledTab->SuspendLayout();
			this->setLed_LedTab_TabPage->SuspendLayout();
			this->tabAccelerometer->SuspendLayout();
			this->accelerometerTab->SuspendLayout();
			this->config_AccmtrTab_TabPage->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->option_config_AccmtrTab_NumericUpDown))->BeginInit();
			this->readLL_AccmtrTab_TabPage->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->address_readLL_AccmtrTab_NumericUpDown))->BeginInit();
			this->writeLL_AccmtrTab_TabPage->SuspendLayout();
			this->tabTemperature->SuspendLayout();
			this->temperatureTab->SuspendLayout();
			this->getTemperature_TempTab_TabPage->SuspendLayout();
			this->readLL_TempTab_TabPage->SuspendLayout();
			this->writeLL_TempTab_TabPage->SuspendLayout();
			this->tabWakeUp->SuspendLayout();
			this->wakeUpTab->SuspendLayout();
			this->setTime_WakeUpTab_TabPage->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->second_setTime_WakeUpTab_NumericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->minute_setTime_WakeUpTab_NumericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->hour_setTime_WakeUpTab_NumericUpDown))->BeginInit();
			this->setDate_WakeUpTab_TabPage->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->weekDay_setDate_WakeUpTab_NumericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->year_setDate_WakeUpTab_NumericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->month_setDate_WakeUpTab_NumericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->day_setDate_WakeUpTab_NumericUpDown))->BeginInit();
			this->getTime_WakeUpTab_TabPage->SuspendLayout();
			this->getDate_WakeUpTab_TabPage->SuspendLayout();
			this->setWakeUp_WakeUpTab_TabPage->SuspendLayout();
			this->setAlarm_WakeUpTab_TabPage->SuspendLayout();
			this->setBoot_WakeUpTab_TabPage->SuspendLayout();
			this->readLL_WakeUpTab_TabPage->SuspendLayout();
			this->writeLL_WakeUpTab_TabPage->SuspendLayout();
			this->tabLCD->SuspendLayout();
			this->lcdTab->SuspendLayout();
			this->clearScreen_LcdTab_TabPage->SuspendLayout();
			this->writeLL_LcdTab_TabPage->SuspendLayout();
			this->tabFlash->SuspendLayout();
			this->flashTab->SuspendLayout();
			this->format_FlashTab_TabPage->SuspendLayout();
			this->initialize_FlashTab_TabPage->SuspendLayout();
			this->cardDetect_FlashTab_TabPage->SuspendLayout();
			this->writeSector_FlashTab_TabPage->SuspendLayout();
			this->readSector_FlashTab_TabPage->SuspendLayout();
			this->getCapacity_FlashTab_TabPage->SuspendLayout();
			this->readLL_FlashTab_TabPage->SuspendLayout();
			this->writeLL_FlashTab_TabPage->SuspendLayout();
			this->tabApplication->SuspendLayout();
			this->applicationTab->SuspendLayout();
			this->start_AppTab_TabPage->SuspendLayout();
			this->applicationMode_GroupBox->SuspendLayout();
			this->shutdown_AppTab_TabPage->SuspendLayout();
			this->config_AppTab_TabPage->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->value_config_AppTab_NumericUpDown))->BeginInit();
			this->tabSystem->SuspendLayout();
			this->systemTab->SuspendLayout();
			this->systemGetVersion_SystemTab_TabPage->SuspendLayout();
			this->systemSetId_SystemTab_TabPage->SuspendLayout();
			this->getPower_SystemTab_TabPage->SuspendLayout();
			this->setPower_SystemTab_TabPage->SuspendLayout();
			this->getLastError_SystemTab_TabPage->SuspendLayout();
			this->tabHelp->SuspendLayout();
			this->helpTab->SuspendLayout();
			this->introduction_HelpTab_TabPage->SuspendLayout();
			this->instruction_HelpTab_TabPage->SuspendLayout();
			this->allTabs->SuspendLayout();
			this->tabEEPROM->SuspendLayout();
			this->eepromTab->SuspendLayout();
			this->format_EepromTab_TabPage->SuspendLayout();
			this->read_EepromTab_TabPage->SuspendLayout();
			this->write_EepromTab_TabPage->SuspendLayout();
			this->setBoot_EepromTab_TabPage->SuspendLayout();
			this->getBoot_EepromTab_TabPage->SuspendLayout();
			this->readLL_EepromTab_TabPage->SuspendLayout();
			this->writeLL_EepromTab_TabPage->SuspendLayout();
			this->tabADC->SuspendLayout();
			this->ADCTab->SuspendLayout();
			this->readLL_AdcTab_TabPage->SuspendLayout();
			this->writeLL_AdcTab_TabPage->SuspendLayout();
			this->tabHub->SuspendLayout();
			this->hubTab->SuspendLayout();
			this->connection_GroupBox->SuspendLayout();
			this->login_GroupBox->SuspendLayout();
			this->console_GroupBox->SuspendLayout();
			this->debugInformation_GroupBox->SuspendLayout();
			this->commandLineInterface_GroupBox->SuspendLayout();
			this->SuspendLayout();
			// 
			// console_TextBox
			// 
			this->console_TextBox->Font = (gcnew System::Drawing::Font(L"Miriam Fixed", 8));
			this->console_TextBox->Location = System::Drawing::Point(20, 25);
			this->console_TextBox->Multiline = true;
			this->console_TextBox->Name = L"console_TextBox";
			this->console_TextBox->Size = System::Drawing::Size(437, 386);
			this->console_TextBox->TabIndex = 9;
			// 
			// sendCommands_Button
			// 
			this->sendCommands_Button->AutoSize = true;
			this->sendCommands_Button->BackColor = System::Drawing::Color::MistyRose;
			this->sendCommands_Button->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
			this->sendCommands_Button->DialogResult = System::Windows::Forms::DialogResult::Cancel;
			this->sendCommands_Button->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->sendCommands_Button->Location = System::Drawing::Point(553, 107);
			this->sendCommands_Button->Name = L"sendCommands_Button";
			this->sendCommands_Button->Size = System::Drawing::Size(77, 38);
			this->sendCommands_Button->TabIndex = 8;
			this->sendCommands_Button->Text = L"Send\r\nCommands";
			this->sendCommands_Button->UseVisualStyleBackColor = false;
			this->sendCommands_Button->Click += gcnew System::EventHandler(this, &GUI::sendCommands_Button_Click);
			// 
			// sendCommands_TextBox
			// 
			this->sendCommands_TextBox->Location = System::Drawing::Point(95, 31);
			this->sendCommands_TextBox->Multiline = true;
			this->sendCommands_TextBox->Name = L"sendCommands_TextBox";
			this->sendCommands_TextBox->Size = System::Drawing::Size(451, 114);
			this->sendCommands_TextBox->TabIndex = 7;
			// 
			// browse_Button
			// 
			this->browse_Button->AutoSize = true;
			this->browse_Button->Location = System::Drawing::Point(554, 164);
			this->browse_Button->Name = L"browse_Button";
			this->browse_Button->Size = System::Drawing::Size(75, 23);
			this->browse_Button->TabIndex = 12;
			this->browse_Button->Text = L"Browse";
			this->browse_Button->UseVisualStyleBackColor = true;
			this->browse_Button->Click += gcnew System::EventHandler(this, &GUI::browse_Button_Click);
			// 
			// scriptFile_TextBox
			// 
			this->scriptFile_TextBox->Location = System::Drawing::Point(95, 166);
			this->scriptFile_TextBox->Name = L"scriptFile_TextBox";
			this->scriptFile_TextBox->Size = System::Drawing::Size(451, 20);
			this->scriptFile_TextBox->TabIndex = 13;
			// 
			// connect_Button
			// 
			this->connect_Button->BackColor = System::Drawing::Color::MistyRose;
			this->connect_Button->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->connect_Button->Location = System::Drawing::Point(168, 46);
			this->connect_Button->Name = L"connect_Button";
			this->connect_Button->Size = System::Drawing::Size(73, 23);
			this->connect_Button->TabIndex = 14;
			this->connect_Button->Text = L"Connect";
			this->connect_Button->UseVisualStyleBackColor = false;
			this->connect_Button->Click += gcnew System::EventHandler(this, &GUI::connect_Button_Click);
			// 
			// comPort_ComboBox
			// 
			this->comPort_ComboBox->FormattingEnabled = true;
			this->comPort_ComboBox->Location = System::Drawing::Point(99, 24);
			this->comPort_ComboBox->Name = L"comPort_ComboBox";
			this->comPort_ComboBox->Size = System::Drawing::Size(62, 21);
			this->comPort_ComboBox->TabIndex = 15;
			// 
			// disconnect_Button
			// 
			this->disconnect_Button->BackColor = System::Drawing::Color::DarkGray;
			this->disconnect_Button->Enabled = false;
			this->disconnect_Button->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->disconnect_Button->Location = System::Drawing::Point(247, 46);
			this->disconnect_Button->Name = L"disconnect_Button";
			this->disconnect_Button->Size = System::Drawing::Size(80, 23);
			this->disconnect_Button->TabIndex = 16;
			this->disconnect_Button->Text = L"Disconnect";
			this->disconnect_Button->UseVisualStyleBackColor = false;
			this->disconnect_Button->Click += gcnew System::EventHandler(this, &GUI::disconnect_Button_Click);
			// 
			// serialPort1
			// 
			this->serialPort1->BaudRate = 19200;
			this->serialPort1->ParityReplace = static_cast<System::Byte>(61);
			this->serialPort1->ReadBufferSize = 16384;
			this->serialPort1->DataReceived += gcnew System::IO::Ports::SerialDataReceivedEventHandler(this, &GUI::serialPort1_DataReceived);
			// 
			// comPorts_Timer
			// 
			this->comPorts_Timer->Enabled = true;
			this->comPorts_Timer->Interval = 1;
			this->comPorts_Timer->Tick += gcnew System::EventHandler(this, &GUI::comPorts_Timer_Tick);
			// 
			// openFileDialog1
			// 
			this->openFileDialog1->FileName = L"openFileDialog1";
			// 
			// clearConsole_Button
			// 
			this->clearConsole_Button->AutoSize = true;
			this->clearConsole_Button->BackColor = System::Drawing::Color::MistyRose;
			this->clearConsole_Button->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->clearConsole_Button->Location = System::Drawing::Point(176, 423);
			this->clearConsole_Button->Name = L"clearConsole_Button";
			this->clearConsole_Button->Size = System::Drawing::Size(134, 26);
			this->clearConsole_Button->TabIndex = 18;
			this->clearConsole_Button->Text = L"Clear Console";
			this->clearConsole_Button->UseVisualStyleBackColor = false;
			this->clearConsole_Button->Click += gcnew System::EventHandler(this, &GUI::clearConsole_Button_Click);
			// 
			// connectionTime_TextBox
			// 
			this->connectionTime_TextBox->BackColor = System::Drawing::SystemColors::ButtonFace;
			this->connectionTime_TextBox->Dock = System::Windows::Forms::DockStyle::Bottom;
			this->connectionTime_TextBox->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->connectionTime_TextBox->Location = System::Drawing::Point(0, 715);
			this->connectionTime_TextBox->Multiline = true;
			this->connectionTime_TextBox->Name = L"connectionTime_TextBox";
			this->connectionTime_TextBox->Size = System::Drawing::Size(1172, 27);
			this->connectionTime_TextBox->TabIndex = 21;
			this->connectionTime_TextBox->Text = L"Disconnected";
			// 
			// connection_Timer
			// 
			this->connection_Timer->Interval = 1000;
			this->connection_Timer->Tick += gcnew System::EventHandler(this, &GUI::connection_Timer_Tick);
			// 
			// commands_Timer
			// 
			this->commands_Timer->Interval = 500;
			this->commands_Timer->Tick += gcnew System::EventHandler(this, &GUI::commands_Timer_Tick);
			// 
			// countNumberOfCharacters_Button
			// 
			this->countNumberOfCharacters_Button->Location = System::Drawing::Point(6, 165);
			this->countNumberOfCharacters_Button->Name = L"countNumberOfCharacters_Button";
			this->countNumberOfCharacters_Button->Size = System::Drawing::Size(155, 25);
			this->countNumberOfCharacters_Button->TabIndex = 22;
			this->countNumberOfCharacters_Button->Text = L"Count Number of Characters:";
			this->countNumberOfCharacters_Button->UseVisualStyleBackColor = true;
			this->countNumberOfCharacters_Button->Click += gcnew System::EventHandler(this, &GUI::countNumberOfCharacters_Button_Click);
			// 
			// countNumberOfCharacters_TextBox
			// 
			this->countNumberOfCharacters_TextBox->Location = System::Drawing::Point(272, 165);
			this->countNumberOfCharacters_TextBox->Multiline = true;
			this->countNumberOfCharacters_TextBox->Name = L"countNumberOfCharacters_TextBox";
			this->countNumberOfCharacters_TextBox->Size = System::Drawing::Size(69, 20);
			this->countNumberOfCharacters_TextBox->TabIndex = 23;
			// 
			// tabSwitch
			// 
			this->tabSwitch->BackColor = System::Drawing::Color::WhiteSmoke;
			this->tabSwitch->Controls->Add(this->switchTab);
			this->tabSwitch->Location = System::Drawing::Point(4, 22);
			this->tabSwitch->Name = L"tabSwitch";
			this->tabSwitch->Padding = System::Windows::Forms::Padding(3);
			this->tabSwitch->Size = System::Drawing::Size(633, 339);
			this->tabSwitch->TabIndex = 9;
			this->tabSwitch->Text = L"Switch";
			// 
			// switchTab
			// 
			this->switchTab->Controls->Add(this->readLL_SwitchTab_TabPage);
			this->switchTab->Location = System::Drawing::Point(0, 0);
			this->switchTab->Name = L"switchTab";
			this->switchTab->SelectedIndex = 0;
			this->switchTab->Size = System::Drawing::Size(640, 345);
			this->switchTab->TabIndex = 15;
			// 
			// readLL_SwitchTab_TabPage
			// 
			this->readLL_SwitchTab_TabPage->BackColor = System::Drawing::Color::WhiteSmoke;
			this->readLL_SwitchTab_TabPage->Controls->Add(this->destination_readLL_SwitchTab_Label);
			this->readLL_SwitchTab_TabPage->Controls->Add(this->destination_readLL_SwitchTab_TextBox);
			this->readLL_SwitchTab_TabPage->Controls->Add(this->switchId_readLL_SwitchTab_ComboBox);
			this->readLL_SwitchTab_TabPage->Controls->Add(this->address_readLL_SwitchTab_Label);
			this->readLL_SwitchTab_TabPage->Controls->Add(this->address_readLL_SwitchTab_ComboBox);
			this->readLL_SwitchTab_TabPage->Controls->Add(this->apply_readLL_SwitchTab_Button);
			this->readLL_SwitchTab_TabPage->Controls->Add(this->switchId_readLL_SwitchTab_Label);
			this->readLL_SwitchTab_TabPage->Controls->Add(this->readLL_SwitchTab_TextBox);
			this->readLL_SwitchTab_TabPage->Location = System::Drawing::Point(4, 22);
			this->readLL_SwitchTab_TabPage->Name = L"readLL_SwitchTab_TabPage";
			this->readLL_SwitchTab_TabPage->Padding = System::Windows::Forms::Padding(3);
			this->readLL_SwitchTab_TabPage->Size = System::Drawing::Size(632, 319);
			this->readLL_SwitchTab_TabPage->TabIndex = 1;
			this->readLL_SwitchTab_TabPage->Text = L"Read (Low Level)";
			// 
			// destination_readLL_SwitchTab_Label
			// 
			this->destination_readLL_SwitchTab_Label->AutoSize = true;
			this->destination_readLL_SwitchTab_Label->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(177)));
			this->destination_readLL_SwitchTab_Label->Location = System::Drawing::Point(46, 146);
			this->destination_readLL_SwitchTab_Label->Name = L"destination_readLL_SwitchTab_Label";
			this->destination_readLL_SwitchTab_Label->Size = System::Drawing::Size(83, 17);
			this->destination_readLL_SwitchTab_Label->TabIndex = 19;
			this->destination_readLL_SwitchTab_Label->Text = L"Destination:";
			// 
			// destination_readLL_SwitchTab_TextBox
			// 
			this->destination_readLL_SwitchTab_TextBox->Location = System::Drawing::Point(140, 142);
			this->destination_readLL_SwitchTab_TextBox->Name = L"destination_readLL_SwitchTab_TextBox";
			this->destination_readLL_SwitchTab_TextBox->Size = System::Drawing::Size(71, 20);
			this->destination_readLL_SwitchTab_TextBox->TabIndex = 18;
			// 
			// switchId_readLL_SwitchTab_ComboBox
			// 
			this->switchId_readLL_SwitchTab_ComboBox->FormattingEnabled = true;
			this->switchId_readLL_SwitchTab_ComboBox->Items->AddRange(gcnew cli::array< System::Object^  >(2) {L"0", L"1"});
			this->switchId_readLL_SwitchTab_ComboBox->Location = System::Drawing::Point(140, 181);
			this->switchId_readLL_SwitchTab_ComboBox->Name = L"switchId_readLL_SwitchTab_ComboBox";
			this->switchId_readLL_SwitchTab_ComboBox->Size = System::Drawing::Size(68, 21);
			this->switchId_readLL_SwitchTab_ComboBox->TabIndex = 17;
			// 
			// address_readLL_SwitchTab_Label
			// 
			this->address_readLL_SwitchTab_Label->AutoSize = true;
			this->address_readLL_SwitchTab_Label->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->address_readLL_SwitchTab_Label->Location = System::Drawing::Point(46, 217);
			this->address_readLL_SwitchTab_Label->Name = L"address_readLL_SwitchTab_Label";
			this->address_readLL_SwitchTab_Label->Size = System::Drawing::Size(64, 17);
			this->address_readLL_SwitchTab_Label->TabIndex = 16;
			this->address_readLL_SwitchTab_Label->Text = L"Address:";
			// 
			// address_readLL_SwitchTab_ComboBox
			// 
			this->address_readLL_SwitchTab_ComboBox->FormattingEnabled = true;
			this->address_readLL_SwitchTab_ComboBox->Items->AddRange(gcnew cli::array< System::Object^  >(2) {L"1", L"2"});
			this->address_readLL_SwitchTab_ComboBox->Location = System::Drawing::Point(140, 217);
			this->address_readLL_SwitchTab_ComboBox->Name = L"address_readLL_SwitchTab_ComboBox";
			this->address_readLL_SwitchTab_ComboBox->Size = System::Drawing::Size(68, 21);
			this->address_readLL_SwitchTab_ComboBox->TabIndex = 15;
			// 
			// apply_readLL_SwitchTab_Button
			// 
			this->apply_readLL_SwitchTab_Button->AutoSize = true;
			this->apply_readLL_SwitchTab_Button->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
			this->apply_readLL_SwitchTab_Button->Location = System::Drawing::Point(276, 250);
			this->apply_readLL_SwitchTab_Button->Name = L"apply_readLL_SwitchTab_Button";
			this->apply_readLL_SwitchTab_Button->Size = System::Drawing::Size(80, 28);
			this->apply_readLL_SwitchTab_Button->TabIndex = 13;
			this->apply_readLL_SwitchTab_Button->Text = L"Apply";
			this->apply_readLL_SwitchTab_Button->UseVisualStyleBackColor = true;
			this->apply_readLL_SwitchTab_Button->Click += gcnew System::EventHandler(this, &GUI::apply_readLL_SwitchTab_Button_Click);
			// 
			// switchId_readLL_SwitchTab_Label
			// 
			this->switchId_readLL_SwitchTab_Label->AutoSize = true;
			this->switchId_readLL_SwitchTab_Label->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->switchId_readLL_SwitchTab_Label->Location = System::Drawing::Point(46, 185);
			this->switchId_readLL_SwitchTab_Label->Name = L"switchId_readLL_SwitchTab_Label";
			this->switchId_readLL_SwitchTab_Label->Size = System::Drawing::Size(69, 17);
			this->switchId_readLL_SwitchTab_Label->TabIndex = 9;
			this->switchId_readLL_SwitchTab_Label->Text = L"Switch ID:";
			// 
			// readLL_SwitchTab_TextBox
			// 
			this->readLL_SwitchTab_TextBox->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->readLL_SwitchTab_TextBox->Location = System::Drawing::Point(8, 3);
			this->readLL_SwitchTab_TextBox->Multiline = true;
			this->readLL_SwitchTab_TextBox->Name = L"readLL_SwitchTab_TextBox";
			this->readLL_SwitchTab_TextBox->ReadOnly = true;
			this->readLL_SwitchTab_TextBox->Size = System::Drawing::Size(620, 114);
			this->readLL_SwitchTab_TextBox->TabIndex = 7;
			this->readLL_SwitchTab_TextBox->Text = L"Get Wistone\'s Switch:\r\n\r\n^ Choose \"Destination\" ( Network ID of Wistone or 0 for " 
				L"Broadcast )\r\n^ Choose: \"Address\" ( of the switch )\r\n^ Press \"Apply\"";
			// 
			// tabBuzzer
			// 
			this->tabBuzzer->BackColor = System::Drawing::Color::WhiteSmoke;
			this->tabBuzzer->Controls->Add(this->buzzerTab);
			this->tabBuzzer->Location = System::Drawing::Point(4, 22);
			this->tabBuzzer->Name = L"tabBuzzer";
			this->tabBuzzer->Padding = System::Windows::Forms::Padding(3);
			this->tabBuzzer->Size = System::Drawing::Size(633, 339);
			this->tabBuzzer->TabIndex = 8;
			this->tabBuzzer->Text = L"Buzzer";
			// 
			// buzzerTab
			// 
			this->buzzerTab->Controls->Add(this->writeLL_BuzzerTab_TabPage);
			this->buzzerTab->Location = System::Drawing::Point(0, 0);
			this->buzzerTab->Name = L"buzzerTab";
			this->buzzerTab->SelectedIndex = 0;
			this->buzzerTab->Size = System::Drawing::Size(640, 345);
			this->buzzerTab->TabIndex = 15;
			// 
			// writeLL_BuzzerTab_TabPage
			// 
			this->writeLL_BuzzerTab_TabPage->BackColor = System::Drawing::Color::WhiteSmoke;
			this->writeLL_BuzzerTab_TabPage->Controls->Add(this->destination_writeLL_BuzzerTab_Label);
			this->writeLL_BuzzerTab_TabPage->Controls->Add(this->destination_writeLL_BuzzerTab_TextBox);
			this->writeLL_BuzzerTab_TabPage->Controls->Add(this->apply_writeLL_BuzzerTab_Button);
			this->writeLL_BuzzerTab_TabPage->Controls->Add(this->beepDuration_writeLL_BuzzerTab_Label);
			this->writeLL_BuzzerTab_TabPage->Controls->Add(this->beepDuration_writeLL_BuzzerTab_NumericUpDown);
			this->writeLL_BuzzerTab_TabPage->Controls->Add(this->writeLL_BuzzerTab_TextBox);
			this->writeLL_BuzzerTab_TabPage->Location = System::Drawing::Point(4, 22);
			this->writeLL_BuzzerTab_TabPage->Name = L"writeLL_BuzzerTab_TabPage";
			this->writeLL_BuzzerTab_TabPage->Padding = System::Windows::Forms::Padding(3);
			this->writeLL_BuzzerTab_TabPage->Size = System::Drawing::Size(632, 319);
			this->writeLL_BuzzerTab_TabPage->TabIndex = 1;
			this->writeLL_BuzzerTab_TabPage->Text = L"Write (Low Level)";
			// 
			// destination_writeLL_BuzzerTab_Label
			// 
			this->destination_writeLL_BuzzerTab_Label->AutoSize = true;
			this->destination_writeLL_BuzzerTab_Label->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(177)));
			this->destination_writeLL_BuzzerTab_Label->Location = System::Drawing::Point(46, 178);
			this->destination_writeLL_BuzzerTab_Label->Name = L"destination_writeLL_BuzzerTab_Label";
			this->destination_writeLL_BuzzerTab_Label->Size = System::Drawing::Size(83, 17);
			this->destination_writeLL_BuzzerTab_Label->TabIndex = 14;
			this->destination_writeLL_BuzzerTab_Label->Text = L"Destination:";
			// 
			// destination_writeLL_BuzzerTab_TextBox
			// 
			this->destination_writeLL_BuzzerTab_TextBox->Location = System::Drawing::Point(171, 176);
			this->destination_writeLL_BuzzerTab_TextBox->Name = L"destination_writeLL_BuzzerTab_TextBox";
			this->destination_writeLL_BuzzerTab_TextBox->Size = System::Drawing::Size(58, 20);
			this->destination_writeLL_BuzzerTab_TextBox->TabIndex = 13;
			// 
			// apply_writeLL_BuzzerTab_Button
			// 
			this->apply_writeLL_BuzzerTab_Button->AutoSize = true;
			this->apply_writeLL_BuzzerTab_Button->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
			this->apply_writeLL_BuzzerTab_Button->Location = System::Drawing::Point(276, 250);
			this->apply_writeLL_BuzzerTab_Button->Name = L"apply_writeLL_BuzzerTab_Button";
			this->apply_writeLL_BuzzerTab_Button->Size = System::Drawing::Size(80, 28);
			this->apply_writeLL_BuzzerTab_Button->TabIndex = 12;
			this->apply_writeLL_BuzzerTab_Button->Text = L"Apply";
			this->apply_writeLL_BuzzerTab_Button->UseVisualStyleBackColor = true;
			this->apply_writeLL_BuzzerTab_Button->Click += gcnew System::EventHandler(this, &GUI::apply_writeLL_BuzzerTab_Button_Click);
			// 
			// beepDuration_writeLL_BuzzerTab_Label
			// 
			this->beepDuration_writeLL_BuzzerTab_Label->AutoSize = true;
			this->beepDuration_writeLL_BuzzerTab_Label->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->beepDuration_writeLL_BuzzerTab_Label->Location = System::Drawing::Point(46, 217);
			this->beepDuration_writeLL_BuzzerTab_Label->Name = L"beepDuration_writeLL_BuzzerTab_Label";
			this->beepDuration_writeLL_BuzzerTab_Label->Size = System::Drawing::Size(103, 17);
			this->beepDuration_writeLL_BuzzerTab_Label->TabIndex = 9;
			this->beepDuration_writeLL_BuzzerTab_Label->Text = L"Beep Duration:";
			// 
			// beepDuration_writeLL_BuzzerTab_NumericUpDown
			// 
			this->beepDuration_writeLL_BuzzerTab_NumericUpDown->Location = System::Drawing::Point(171, 216);
			this->beepDuration_writeLL_BuzzerTab_NumericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {10, 0, 
				0, 0});
			this->beepDuration_writeLL_BuzzerTab_NumericUpDown->Name = L"beepDuration_writeLL_BuzzerTab_NumericUpDown";
			this->beepDuration_writeLL_BuzzerTab_NumericUpDown->Size = System::Drawing::Size(58, 20);
			this->beepDuration_writeLL_BuzzerTab_NumericUpDown->TabIndex = 8;
			// 
			// writeLL_BuzzerTab_TextBox
			// 
			this->writeLL_BuzzerTab_TextBox->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->writeLL_BuzzerTab_TextBox->Location = System::Drawing::Point(6, 0);
			this->writeLL_BuzzerTab_TextBox->Multiline = true;
			this->writeLL_BuzzerTab_TextBox->Name = L"writeLL_BuzzerTab_TextBox";
			this->writeLL_BuzzerTab_TextBox->ReadOnly = true;
			this->writeLL_BuzzerTab_TextBox->Size = System::Drawing::Size(620, 109);
			this->writeLL_BuzzerTab_TextBox->TabIndex = 7;
			this->writeLL_BuzzerTab_TextBox->Text = L"Set Wistone\'s Buzzer:\r\n\r\n^ Choose \"Destination\" ( Network ID of Wistone or 0 for " 
				L"Broadcast )\r\n^ Choose: \"Beep Duration\" ( in seconds )\r\n^ Press \"Apply\"\r\n";
			// 
			// tabLed
			// 
			this->tabLed->BackColor = System::Drawing::Color::WhiteSmoke;
			this->tabLed->Controls->Add(this->ledTab);
			this->tabLed->Location = System::Drawing::Point(4, 22);
			this->tabLed->Name = L"tabLed";
			this->tabLed->Padding = System::Windows::Forms::Padding(3);
			this->tabLed->Size = System::Drawing::Size(633, 339);
			this->tabLed->TabIndex = 7;
			this->tabLed->Text = L"Led";
			// 
			// ledTab
			// 
			this->ledTab->Controls->Add(this->setLed_LedTab_TabPage);
			this->ledTab->Location = System::Drawing::Point(0, 0);
			this->ledTab->Name = L"ledTab";
			this->ledTab->SelectedIndex = 0;
			this->ledTab->Size = System::Drawing::Size(640, 345);
			this->ledTab->TabIndex = 14;
			// 
			// setLed_LedTab_TabPage
			// 
			this->setLed_LedTab_TabPage->BackColor = System::Drawing::Color::WhiteSmoke;
			this->setLed_LedTab_TabPage->Controls->Add(this->destination_setLed_LedTab_Label);
			this->setLed_LedTab_TabPage->Controls->Add(this->destination_setLed_LedTab_TextBox);
			this->setLed_LedTab_TabPage->Controls->Add(this->address_setLed_LedTab_Label);
			this->setLed_LedTab_TabPage->Controls->Add(this->address_setLed_LedTab_ComboBox);
			this->setLed_LedTab_TabPage->Controls->Add(this->apply_setLed_LedTab_Button);
			this->setLed_LedTab_TabPage->Controls->Add(this->ledOff_setLed_LedTab_RadioButton);
			this->setLed_LedTab_TabPage->Controls->Add(this->ledOn_setLed_LedTab_RadioButton);
			this->setLed_LedTab_TabPage->Controls->Add(this->setLed_LedTab_TextBox);
			this->setLed_LedTab_TabPage->Location = System::Drawing::Point(4, 22);
			this->setLed_LedTab_TabPage->Name = L"setLed_LedTab_TabPage";
			this->setLed_LedTab_TabPage->Padding = System::Windows::Forms::Padding(3);
			this->setLed_LedTab_TabPage->Size = System::Drawing::Size(632, 319);
			this->setLed_LedTab_TabPage->TabIndex = 1;
			this->setLed_LedTab_TabPage->Text = L"Set Led";
			// 
			// destination_setLed_LedTab_Label
			// 
			this->destination_setLed_LedTab_Label->AutoSize = true;
			this->destination_setLed_LedTab_Label->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(177)));
			this->destination_setLed_LedTab_Label->Location = System::Drawing::Point(58, 147);
			this->destination_setLed_LedTab_Label->Name = L"destination_setLed_LedTab_Label";
			this->destination_setLed_LedTab_Label->Size = System::Drawing::Size(83, 17);
			this->destination_setLed_LedTab_Label->TabIndex = 16;
			this->destination_setLed_LedTab_Label->Text = L"Destination:";
			// 
			// destination_setLed_LedTab_TextBox
			// 
			this->destination_setLed_LedTab_TextBox->Location = System::Drawing::Point(146, 147);
			this->destination_setLed_LedTab_TextBox->Name = L"destination_setLed_LedTab_TextBox";
			this->destination_setLed_LedTab_TextBox->Size = System::Drawing::Size(71, 20);
			this->destination_setLed_LedTab_TextBox->TabIndex = 15;
			// 
			// address_setLed_LedTab_Label
			// 
			this->address_setLed_LedTab_Label->AutoSize = true;
			this->address_setLed_LedTab_Label->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->address_setLed_LedTab_Label->Location = System::Drawing::Point(58, 182);
			this->address_setLed_LedTab_Label->Name = L"address_setLed_LedTab_Label";
			this->address_setLed_LedTab_Label->Size = System::Drawing::Size(64, 17);
			this->address_setLed_LedTab_Label->TabIndex = 14;
			this->address_setLed_LedTab_Label->Text = L"Address:";
			// 
			// address_setLed_LedTab_ComboBox
			// 
			this->address_setLed_LedTab_ComboBox->FormattingEnabled = true;
			this->address_setLed_LedTab_ComboBox->Items->AddRange(gcnew cli::array< System::Object^  >(2) {L"1", L"2"});
			this->address_setLed_LedTab_ComboBox->Location = System::Drawing::Point(147, 182);
			this->address_setLed_LedTab_ComboBox->Name = L"address_setLed_LedTab_ComboBox";
			this->address_setLed_LedTab_ComboBox->Size = System::Drawing::Size(70, 21);
			this->address_setLed_LedTab_ComboBox->TabIndex = 13;
			// 
			// apply_setLed_LedTab_Button
			// 
			this->apply_setLed_LedTab_Button->AutoSize = true;
			this->apply_setLed_LedTab_Button->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
			this->apply_setLed_LedTab_Button->Location = System::Drawing::Point(276, 250);
			this->apply_setLed_LedTab_Button->Name = L"apply_setLed_LedTab_Button";
			this->apply_setLed_LedTab_Button->Size = System::Drawing::Size(80, 28);
			this->apply_setLed_LedTab_Button->TabIndex = 12;
			this->apply_setLed_LedTab_Button->Text = L"Apply";
			this->apply_setLed_LedTab_Button->UseVisualStyleBackColor = true;
			this->apply_setLed_LedTab_Button->Click += gcnew System::EventHandler(this, &GUI::apply_setLed_LedTab_Button_Click);
			// 
			// ledOff_setLed_LedTab_RadioButton
			// 
			this->ledOff_setLed_LedTab_RadioButton->AutoSize = true;
			this->ledOff_setLed_LedTab_RadioButton->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->ledOff_setLed_LedTab_RadioButton->Location = System::Drawing::Point(253, 209);
			this->ledOff_setLed_LedTab_RadioButton->Name = L"ledOff_setLed_LedTab_RadioButton";
			this->ledOff_setLed_LedTab_RadioButton->Size = System::Drawing::Size(116, 21);
			this->ledOff_setLed_LedTab_RadioButton->TabIndex = 9;
			this->ledOff_setLed_LedTab_RadioButton->TabStop = true;
			this->ledOff_setLed_LedTab_RadioButton->Text = L"Power Led Off";
			this->ledOff_setLed_LedTab_RadioButton->UseVisualStyleBackColor = true;
			this->ledOff_setLed_LedTab_RadioButton->CheckedChanged += gcnew System::EventHandler(this, &GUI::ledOff_setLed_LedTab_RadioButton_Click);
			// 
			// ledOn_setLed_LedTab_RadioButton
			// 
			this->ledOn_setLed_LedTab_RadioButton->AutoSize = true;
			this->ledOn_setLed_LedTab_RadioButton->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->ledOn_setLed_LedTab_RadioButton->Location = System::Drawing::Point(253, 182);
			this->ledOn_setLed_LedTab_RadioButton->Name = L"ledOn_setLed_LedTab_RadioButton";
			this->ledOn_setLed_LedTab_RadioButton->Size = System::Drawing::Size(116, 21);
			this->ledOn_setLed_LedTab_RadioButton->TabIndex = 8;
			this->ledOn_setLed_LedTab_RadioButton->TabStop = true;
			this->ledOn_setLed_LedTab_RadioButton->Text = L"Power Led On";
			this->ledOn_setLed_LedTab_RadioButton->UseVisualStyleBackColor = true;
			this->ledOn_setLed_LedTab_RadioButton->CheckedChanged += gcnew System::EventHandler(this, &GUI::ledOn_setLed_LedTab_RadioButton_Click);
			// 
			// setLed_LedTab_TextBox
			// 
			this->setLed_LedTab_TextBox->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->setLed_LedTab_TextBox->Location = System::Drawing::Point(6, 3);
			this->setLed_LedTab_TextBox->Multiline = true;
			this->setLed_LedTab_TextBox->Name = L"setLed_LedTab_TextBox";
			this->setLed_LedTab_TextBox->ReadOnly = true;
			this->setLed_LedTab_TextBox->Size = System::Drawing::Size(620, 125);
			this->setLed_LedTab_TextBox->TabIndex = 7;
			this->setLed_LedTab_TextBox->Text = L"Set Wistone\'s LED:\r\n\r\n^ Choose \"Destination\" ( Network ID of Wistone or 0 for Bro" 
				L"adcast )\r\n^ Choose: \"Address\" ( of the LED )\r\n^ Choose: \"Power Led On\" or \"Power" 
				L" Led Off\"\r\n^ Press \"Set Led\"";
			// 
			// tabAccelerometer
			// 
			this->tabAccelerometer->BackColor = System::Drawing::Color::WhiteSmoke;
			this->tabAccelerometer->Controls->Add(this->accelerometerTab);
			this->tabAccelerometer->Location = System::Drawing::Point(4, 22);
			this->tabAccelerometer->Name = L"tabAccelerometer";
			this->tabAccelerometer->Padding = System::Windows::Forms::Padding(3);
			this->tabAccelerometer->Size = System::Drawing::Size(633, 339);
			this->tabAccelerometer->TabIndex = 6;
			this->tabAccelerometer->Text = L"Accelerometer";
			// 
			// accelerometerTab
			// 
			this->accelerometerTab->Controls->Add(this->config_AccmtrTab_TabPage);
			this->accelerometerTab->Controls->Add(this->readLL_AccmtrTab_TabPage);
			this->accelerometerTab->Controls->Add(this->writeLL_AccmtrTab_TabPage);
			this->accelerometerTab->Location = System::Drawing::Point(0, 0);
			this->accelerometerTab->Name = L"accelerometerTab";
			this->accelerometerTab->SelectedIndex = 0;
			this->accelerometerTab->Size = System::Drawing::Size(640, 345);
			this->accelerometerTab->TabIndex = 0;
			// 
			// config_AccmtrTab_TabPage
			// 
			this->config_AccmtrTab_TabPage->BackColor = System::Drawing::Color::WhiteSmoke;
			this->config_AccmtrTab_TabPage->Controls->Add(this->destination_config_AccmtrTab_Label);
			this->config_AccmtrTab_TabPage->Controls->Add(this->destination_config_AccmtrTab_TextBox);
			this->config_AccmtrTab_TabPage->Controls->Add(this->parameter_config_AccmtrTab_ComboBox);
			this->config_AccmtrTab_TabPage->Controls->Add(this->option_config_AccmtrTab_NumericUpDown);
			this->config_AccmtrTab_TabPage->Controls->Add(this->updateValue_config_AccmtrTab_Button);
			this->config_AccmtrTab_TabPage->Controls->Add(this->parameter_config_AccmtrTab_Label);
			this->config_AccmtrTab_TabPage->Controls->Add(this->config_AccmtrTab_TextBox);
			this->config_AccmtrTab_TabPage->Controls->Add(this->option_config_AccmtrTab_Label);
			this->config_AccmtrTab_TabPage->Location = System::Drawing::Point(4, 22);
			this->config_AccmtrTab_TabPage->Name = L"config_AccmtrTab_TabPage";
			this->config_AccmtrTab_TabPage->Padding = System::Windows::Forms::Padding(3);
			this->config_AccmtrTab_TabPage->Size = System::Drawing::Size(632, 319);
			this->config_AccmtrTab_TabPage->TabIndex = 1;
			this->config_AccmtrTab_TabPage->Text = L"Config";
			// 
			// destination_config_AccmtrTab_Label
			// 
			this->destination_config_AccmtrTab_Label->AutoSize = true;
			this->destination_config_AccmtrTab_Label->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(177)));
			this->destination_config_AccmtrTab_Label->Location = System::Drawing::Point(22, 271);
			this->destination_config_AccmtrTab_Label->Name = L"destination_config_AccmtrTab_Label";
			this->destination_config_AccmtrTab_Label->Size = System::Drawing::Size(83, 17);
			this->destination_config_AccmtrTab_Label->TabIndex = 22;
			this->destination_config_AccmtrTab_Label->Text = L"Destination:";
			// 
			// destination_config_AccmtrTab_TextBox
			// 
			this->destination_config_AccmtrTab_TextBox->Location = System::Drawing::Point(106, 271);
			this->destination_config_AccmtrTab_TextBox->Name = L"destination_config_AccmtrTab_TextBox";
			this->destination_config_AccmtrTab_TextBox->Size = System::Drawing::Size(47, 20);
			this->destination_config_AccmtrTab_TextBox->TabIndex = 21;
			// 
			// parameter_config_AccmtrTab_ComboBox
			// 
			this->parameter_config_AccmtrTab_ComboBox->FormattingEnabled = true;
			this->parameter_config_AccmtrTab_ComboBox->Items->AddRange(gcnew cli::array< System::Object^  >(1) {L"0"});
			this->parameter_config_AccmtrTab_ComboBox->Location = System::Drawing::Point(254, 271);
			this->parameter_config_AccmtrTab_ComboBox->Name = L"parameter_config_AccmtrTab_ComboBox";
			this->parameter_config_AccmtrTab_ComboBox->Size = System::Drawing::Size(58, 21);
			this->parameter_config_AccmtrTab_ComboBox->TabIndex = 20;
			// 
			// option_config_AccmtrTab_NumericUpDown
			// 
			this->option_config_AccmtrTab_NumericUpDown->Location = System::Drawing::Point(386, 271);
			this->option_config_AccmtrTab_NumericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {3, 0, 0, 0});
			this->option_config_AccmtrTab_NumericUpDown->Name = L"option_config_AccmtrTab_NumericUpDown";
			this->option_config_AccmtrTab_NumericUpDown->Size = System::Drawing::Size(58, 20);
			this->option_config_AccmtrTab_NumericUpDown->TabIndex = 19;
			this->option_config_AccmtrTab_NumericUpDown->ValueChanged += gcnew System::EventHandler(this, &GUI::option_config_AccmtrTab_NumericUpDown_ValueChanged);
			// 
			// updateValue_config_AccmtrTab_Button
			// 
			this->updateValue_config_AccmtrTab_Button->AutoSize = true;
			this->updateValue_config_AccmtrTab_Button->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
			this->updateValue_config_AccmtrTab_Button->Location = System::Drawing::Point(486, 271);
			this->updateValue_config_AccmtrTab_Button->Name = L"updateValue_config_AccmtrTab_Button";
			this->updateValue_config_AccmtrTab_Button->Size = System::Drawing::Size(92, 23);
			this->updateValue_config_AccmtrTab_Button->TabIndex = 18;
			this->updateValue_config_AccmtrTab_Button->Text = L"Update Value";
			this->updateValue_config_AccmtrTab_Button->UseVisualStyleBackColor = true;
			this->updateValue_config_AccmtrTab_Button->Click += gcnew System::EventHandler(this, &GUI::updateValue_config_AccmtrTab_Button_Click);
			// 
			// parameter_config_AccmtrTab_Label
			// 
			this->parameter_config_AccmtrTab_Label->AutoSize = true;
			this->parameter_config_AccmtrTab_Label->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->parameter_config_AccmtrTab_Label->Location = System::Drawing::Point(172, 271);
			this->parameter_config_AccmtrTab_Label->Name = L"parameter_config_AccmtrTab_Label";
			this->parameter_config_AccmtrTab_Label->Size = System::Drawing::Size(78, 17);
			this->parameter_config_AccmtrTab_Label->TabIndex = 17;
			this->parameter_config_AccmtrTab_Label->Text = L"Parameter:";
			// 
			// config_AccmtrTab_TextBox
			// 
			this->config_AccmtrTab_TextBox->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->config_AccmtrTab_TextBox->Location = System::Drawing::Point(6, 1);
			this->config_AccmtrTab_TextBox->Multiline = true;
			this->config_AccmtrTab_TextBox->Name = L"config_AccmtrTab_TextBox";
			this->config_AccmtrTab_TextBox->ReadOnly = true;
			this->config_AccmtrTab_TextBox->Size = System::Drawing::Size(618, 266);
			this->config_AccmtrTab_TextBox->TabIndex = 13;
			this->config_AccmtrTab_TextBox->Text = resources->GetString(L"config_AccmtrTab_TextBox.Text");
			// 
			// option_config_AccmtrTab_Label
			// 
			this->option_config_AccmtrTab_Label->AutoSize = true;
			this->option_config_AccmtrTab_Label->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->option_config_AccmtrTab_Label->Location = System::Drawing::Point(332, 271);
			this->option_config_AccmtrTab_Label->Name = L"option_config_AccmtrTab_Label";
			this->option_config_AccmtrTab_Label->Size = System::Drawing::Size(48, 17);
			this->option_config_AccmtrTab_Label->TabIndex = 15;
			this->option_config_AccmtrTab_Label->Text = L"Value:";
			// 
			// readLL_AccmtrTab_TabPage
			// 
			this->readLL_AccmtrTab_TabPage->BackColor = System::Drawing::Color::WhiteSmoke;
			this->readLL_AccmtrTab_TabPage->Controls->Add(this->destination_readLL_AccmtrTab_Label);
			this->readLL_AccmtrTab_TabPage->Controls->Add(this->destination_readLL_AccmtrTab_TextBox);
			this->readLL_AccmtrTab_TabPage->Controls->Add(this->address_readLL_AccmtrTab_NumericUpDown);
			this->readLL_AccmtrTab_TabPage->Controls->Add(this->read_readLL_AccmtrTab_Button);
			this->readLL_AccmtrTab_TabPage->Controls->Add(this->address_readLL_AccmtrTab_Label);
			this->readLL_AccmtrTab_TabPage->Controls->Add(this->readLL_AccmtrTab_TextBox);
			this->readLL_AccmtrTab_TabPage->Location = System::Drawing::Point(4, 22);
			this->readLL_AccmtrTab_TabPage->Name = L"readLL_AccmtrTab_TabPage";
			this->readLL_AccmtrTab_TabPage->Padding = System::Windows::Forms::Padding(3);
			this->readLL_AccmtrTab_TabPage->Size = System::Drawing::Size(632, 319);
			this->readLL_AccmtrTab_TabPage->TabIndex = 0;
			this->readLL_AccmtrTab_TabPage->Text = L"Read(Low Level)";
			// 
			// destination_readLL_AccmtrTab_Label
			// 
			this->destination_readLL_AccmtrTab_Label->AutoSize = true;
			this->destination_readLL_AccmtrTab_Label->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(177)));
			this->destination_readLL_AccmtrTab_Label->Location = System::Drawing::Point(46, 181);
			this->destination_readLL_AccmtrTab_Label->Name = L"destination_readLL_AccmtrTab_Label";
			this->destination_readLL_AccmtrTab_Label->Size = System::Drawing::Size(83, 17);
			this->destination_readLL_AccmtrTab_Label->TabIndex = 19;
			this->destination_readLL_AccmtrTab_Label->Text = L"Destination:";
			// 
			// destination_readLL_AccmtrTab_TextBox
			// 
			this->destination_readLL_AccmtrTab_TextBox->Location = System::Drawing::Point(136, 179);
			this->destination_readLL_AccmtrTab_TextBox->Name = L"destination_readLL_AccmtrTab_TextBox";
			this->destination_readLL_AccmtrTab_TextBox->Size = System::Drawing::Size(71, 20);
			this->destination_readLL_AccmtrTab_TextBox->TabIndex = 18;
			// 
			// address_readLL_AccmtrTab_NumericUpDown
			// 
			this->address_readLL_AccmtrTab_NumericUpDown->Location = System::Drawing::Point(136, 214);
			this->address_readLL_AccmtrTab_NumericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {3, 0, 0, 0});
			this->address_readLL_AccmtrTab_NumericUpDown->Name = L"address_readLL_AccmtrTab_NumericUpDown";
			this->address_readLL_AccmtrTab_NumericUpDown->Size = System::Drawing::Size(71, 20);
			this->address_readLL_AccmtrTab_NumericUpDown->TabIndex = 17;
			// 
			// read_readLL_AccmtrTab_Button
			// 
			this->read_readLL_AccmtrTab_Button->AutoSize = true;
			this->read_readLL_AccmtrTab_Button->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
			this->read_readLL_AccmtrTab_Button->Location = System::Drawing::Point(276, 250);
			this->read_readLL_AccmtrTab_Button->Name = L"read_readLL_AccmtrTab_Button";
			this->read_readLL_AccmtrTab_Button->Size = System::Drawing::Size(80, 28);
			this->read_readLL_AccmtrTab_Button->TabIndex = 15;
			this->read_readLL_AccmtrTab_Button->Text = L"Read";
			this->read_readLL_AccmtrTab_Button->UseVisualStyleBackColor = true;
			this->read_readLL_AccmtrTab_Button->Click += gcnew System::EventHandler(this, &GUI::read_readLL_AccmtrTab_Button_Click);
			// 
			// address_readLL_AccmtrTab_Label
			// 
			this->address_readLL_AccmtrTab_Label->AutoSize = true;
			this->address_readLL_AccmtrTab_Label->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->address_readLL_AccmtrTab_Label->Location = System::Drawing::Point(46, 217);
			this->address_readLL_AccmtrTab_Label->Name = L"address_readLL_AccmtrTab_Label";
			this->address_readLL_AccmtrTab_Label->Size = System::Drawing::Size(64, 17);
			this->address_readLL_AccmtrTab_Label->TabIndex = 14;
			this->address_readLL_AccmtrTab_Label->Text = L"Address:";
			// 
			// readLL_AccmtrTab_TextBox
			// 
			this->readLL_AccmtrTab_TextBox->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->readLL_AccmtrTab_TextBox->Location = System::Drawing::Point(6, 0);
			this->readLL_AccmtrTab_TextBox->Multiline = true;
			this->readLL_AccmtrTab_TextBox->Name = L"readLL_AccmtrTab_TextBox";
			this->readLL_AccmtrTab_TextBox->ReadOnly = true;
			this->readLL_AccmtrTab_TextBox->Size = System::Drawing::Size(617, 101);
			this->readLL_AccmtrTab_TextBox->TabIndex = 12;
			this->readLL_AccmtrTab_TextBox->Text = L"Read Accelerometer Register:\r\n\r\n^ Choose \"Destination\" ( Network ID of Wistone or" 
				L" 0 for Broadcast )\r\n^ Choose \"Address\" ( of the register )\r\n^ Press \"Read\"";
			// 
			// writeLL_AccmtrTab_TabPage
			// 
			this->writeLL_AccmtrTab_TabPage->BackColor = System::Drawing::Color::WhiteSmoke;
			this->writeLL_AccmtrTab_TabPage->Controls->Add(this->destination_writeLL_AccmtrTab_Label);
			this->writeLL_AccmtrTab_TabPage->Controls->Add(this->destination_writeLL_AccmtrTab_TextBox);
			this->writeLL_AccmtrTab_TabPage->Controls->Add(this->data_writeLL_AccmtrTab_Label);
			this->writeLL_AccmtrTab_TabPage->Controls->Add(this->address_writeLL_AccmtrTab_Label);
			this->writeLL_AccmtrTab_TabPage->Controls->Add(this->write_writeLL_AccmtrTab_Button);
			this->writeLL_AccmtrTab_TabPage->Controls->Add(this->data_writeLL_AccmtrTab_TextBox);
			this->writeLL_AccmtrTab_TabPage->Controls->Add(this->address_writeLL_AccmtrTab_TextBox);
			this->writeLL_AccmtrTab_TabPage->Controls->Add(this->writeLL_AccmtrTab_TextPage);
			this->writeLL_AccmtrTab_TabPage->Location = System::Drawing::Point(4, 22);
			this->writeLL_AccmtrTab_TabPage->Name = L"writeLL_AccmtrTab_TabPage";
			this->writeLL_AccmtrTab_TabPage->Padding = System::Windows::Forms::Padding(3);
			this->writeLL_AccmtrTab_TabPage->Size = System::Drawing::Size(632, 319);
			this->writeLL_AccmtrTab_TabPage->TabIndex = 2;
			this->writeLL_AccmtrTab_TabPage->Text = L"Write (Low Level)";
			// 
			// destination_writeLL_AccmtrTab_Label
			// 
			this->destination_writeLL_AccmtrTab_Label->AutoSize = true;
			this->destination_writeLL_AccmtrTab_Label->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(177)));
			this->destination_writeLL_AccmtrTab_Label->Location = System::Drawing::Point(46, 142);
			this->destination_writeLL_AccmtrTab_Label->Name = L"destination_writeLL_AccmtrTab_Label";
			this->destination_writeLL_AccmtrTab_Label->Size = System::Drawing::Size(83, 17);
			this->destination_writeLL_AccmtrTab_Label->TabIndex = 20;
			this->destination_writeLL_AccmtrTab_Label->Text = L"Destination:";
			// 
			// destination_writeLL_AccmtrTab_TextBox
			// 
			this->destination_writeLL_AccmtrTab_TextBox->Location = System::Drawing::Point(136, 142);
			this->destination_writeLL_AccmtrTab_TextBox->Name = L"destination_writeLL_AccmtrTab_TextBox";
			this->destination_writeLL_AccmtrTab_TextBox->Size = System::Drawing::Size(71, 20);
			this->destination_writeLL_AccmtrTab_TextBox->TabIndex = 19;
			// 
			// data_writeLL_AccmtrTab_Label
			// 
			this->data_writeLL_AccmtrTab_Label->AutoSize = true;
			this->data_writeLL_AccmtrTab_Label->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->data_writeLL_AccmtrTab_Label->Location = System::Drawing::Point(46, 217);
			this->data_writeLL_AccmtrTab_Label->Name = L"data_writeLL_AccmtrTab_Label";
			this->data_writeLL_AccmtrTab_Label->Size = System::Drawing::Size(42, 17);
			this->data_writeLL_AccmtrTab_Label->TabIndex = 18;
			this->data_writeLL_AccmtrTab_Label->Text = L"Data:";
			// 
			// address_writeLL_AccmtrTab_Label
			// 
			this->address_writeLL_AccmtrTab_Label->AutoSize = true;
			this->address_writeLL_AccmtrTab_Label->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->address_writeLL_AccmtrTab_Label->Location = System::Drawing::Point(46, 179);
			this->address_writeLL_AccmtrTab_Label->Name = L"address_writeLL_AccmtrTab_Label";
			this->address_writeLL_AccmtrTab_Label->Size = System::Drawing::Size(64, 17);
			this->address_writeLL_AccmtrTab_Label->TabIndex = 17;
			this->address_writeLL_AccmtrTab_Label->Text = L"Address:";
			// 
			// write_writeLL_AccmtrTab_Button
			// 
			this->write_writeLL_AccmtrTab_Button->AutoSize = true;
			this->write_writeLL_AccmtrTab_Button->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
			this->write_writeLL_AccmtrTab_Button->Location = System::Drawing::Point(276, 250);
			this->write_writeLL_AccmtrTab_Button->Name = L"write_writeLL_AccmtrTab_Button";
			this->write_writeLL_AccmtrTab_Button->Size = System::Drawing::Size(80, 28);
			this->write_writeLL_AccmtrTab_Button->TabIndex = 16;
			this->write_writeLL_AccmtrTab_Button->Text = L"Write";
			this->write_writeLL_AccmtrTab_Button->UseVisualStyleBackColor = true;
			this->write_writeLL_AccmtrTab_Button->Click += gcnew System::EventHandler(this, &GUI::write_writeLL_AccmtrTab_Button_Click);
			// 
			// data_writeLL_AccmtrTab_TextBox
			// 
			this->data_writeLL_AccmtrTab_TextBox->Location = System::Drawing::Point(136, 214);
			this->data_writeLL_AccmtrTab_TextBox->Multiline = true;
			this->data_writeLL_AccmtrTab_TextBox->Name = L"data_writeLL_AccmtrTab_TextBox";
			this->data_writeLL_AccmtrTab_TextBox->Size = System::Drawing::Size(71, 20);
			this->data_writeLL_AccmtrTab_TextBox->TabIndex = 15;
			// 
			// address_writeLL_AccmtrTab_TextBox
			// 
			this->address_writeLL_AccmtrTab_TextBox->Location = System::Drawing::Point(136, 179);
			this->address_writeLL_AccmtrTab_TextBox->Name = L"address_writeLL_AccmtrTab_TextBox";
			this->address_writeLL_AccmtrTab_TextBox->Size = System::Drawing::Size(71, 20);
			this->address_writeLL_AccmtrTab_TextBox->TabIndex = 14;
			this->address_writeLL_AccmtrTab_TextBox->Text = L"0";
			// 
			// writeLL_AccmtrTab_TextPage
			// 
			this->writeLL_AccmtrTab_TextPage->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->writeLL_AccmtrTab_TextPage->Location = System::Drawing::Point(6, 6);
			this->writeLL_AccmtrTab_TextPage->Multiline = true;
			this->writeLL_AccmtrTab_TextPage->Name = L"writeLL_AccmtrTab_TextPage";
			this->writeLL_AccmtrTab_TextPage->ReadOnly = true;
			this->writeLL_AccmtrTab_TextPage->Size = System::Drawing::Size(617, 105);
			this->writeLL_AccmtrTab_TextPage->TabIndex = 13;
			this->writeLL_AccmtrTab_TextPage->Text = L"Write Accelerometer Register:\r\n\r\n^ Choose \"Destination\" ( Network ID of Wistone o" 
				L"r 0 for Broadcast )\r\n^ Enter: \"Address\" ( of the register ) and \"Data\" ( valid r" 
				L"ange: 0 - 255 )\r\n^ Press \"Write\"";
			// 
			// tabTemperature
			// 
			this->tabTemperature->BackColor = System::Drawing::Color::WhiteSmoke;
			this->tabTemperature->Controls->Add(this->temperatureTab);
			this->tabTemperature->Location = System::Drawing::Point(4, 22);
			this->tabTemperature->Name = L"tabTemperature";
			this->tabTemperature->Size = System::Drawing::Size(633, 339);
			this->tabTemperature->TabIndex = 11;
			this->tabTemperature->Text = L"Temperature";
			// 
			// temperatureTab
			// 
			this->temperatureTab->Controls->Add(this->getTemperature_TempTab_TabPage);
			this->temperatureTab->Controls->Add(this->readLL_TempTab_TabPage);
			this->temperatureTab->Controls->Add(this->writeLL_TempTab_TabPage);
			this->temperatureTab->Location = System::Drawing::Point(0, 0);
			this->temperatureTab->Name = L"temperatureTab";
			this->temperatureTab->SelectedIndex = 0;
			this->temperatureTab->Size = System::Drawing::Size(640, 345);
			this->temperatureTab->TabIndex = 13;
			// 
			// getTemperature_TempTab_TabPage
			// 
			this->getTemperature_TempTab_TabPage->BackColor = System::Drawing::Color::WhiteSmoke;
			this->getTemperature_TempTab_TabPage->Controls->Add(this->destination_getTemperature_TemperatureTab_Label);
			this->getTemperature_TempTab_TabPage->Controls->Add(this->destination_getTemperature_TemperatureTab_TextBox);
			this->getTemperature_TempTab_TabPage->Controls->Add(this->getTemperature_getTemperature_TempTab_Button);
			this->getTemperature_TempTab_TabPage->Controls->Add(this->getTemperature_TempTab_TextBox);
			this->getTemperature_TempTab_TabPage->Location = System::Drawing::Point(4, 22);
			this->getTemperature_TempTab_TabPage->Name = L"getTemperature_TempTab_TabPage";
			this->getTemperature_TempTab_TabPage->Padding = System::Windows::Forms::Padding(3);
			this->getTemperature_TempTab_TabPage->Size = System::Drawing::Size(632, 319);
			this->getTemperature_TempTab_TabPage->TabIndex = 2;
			this->getTemperature_TempTab_TabPage->Text = L"Get Temperature";
			// 
			// destination_getTemperature_TemperatureTab_Label
			// 
			this->destination_getTemperature_TemperatureTab_Label->AutoSize = true;
			this->destination_getTemperature_TemperatureTab_Label->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(177)));
			this->destination_getTemperature_TemperatureTab_Label->Location = System::Drawing::Point(43, 217);
			this->destination_getTemperature_TemperatureTab_Label->Name = L"destination_getTemperature_TemperatureTab_Label";
			this->destination_getTemperature_TemperatureTab_Label->Size = System::Drawing::Size(83, 17);
			this->destination_getTemperature_TemperatureTab_Label->TabIndex = 14;
			this->destination_getTemperature_TemperatureTab_Label->Text = L"Destination:";
			// 
			// destination_getTemperature_TemperatureTab_TextBox
			// 
			this->destination_getTemperature_TemperatureTab_TextBox->Location = System::Drawing::Point(127, 217);
			this->destination_getTemperature_TemperatureTab_TextBox->Name = L"destination_getTemperature_TemperatureTab_TextBox";
			this->destination_getTemperature_TemperatureTab_TextBox->Size = System::Drawing::Size(70, 20);
			this->destination_getTemperature_TemperatureTab_TextBox->TabIndex = 13;
			// 
			// getTemperature_getTemperature_TempTab_Button
			// 
			this->getTemperature_getTemperature_TempTab_Button->AutoSize = true;
			this->getTemperature_getTemperature_TempTab_Button->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
			this->getTemperature_getTemperature_TempTab_Button->Location = System::Drawing::Point(266, 250);
			this->getTemperature_getTemperature_TempTab_Button->Name = L"getTemperature_getTemperature_TempTab_Button";
			this->getTemperature_getTemperature_TempTab_Button->Size = System::Drawing::Size(97, 28);
			this->getTemperature_getTemperature_TempTab_Button->TabIndex = 12;
			this->getTemperature_getTemperature_TempTab_Button->Text = L"Get Temperature";
			this->getTemperature_getTemperature_TempTab_Button->UseVisualStyleBackColor = true;
			this->getTemperature_getTemperature_TempTab_Button->Click += gcnew System::EventHandler(this, &GUI::getTemperature_TempTab_Button_Click);
			// 
			// getTemperature_TempTab_TextBox
			// 
			this->getTemperature_TempTab_TextBox->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->getTemperature_TempTab_TextBox->Location = System::Drawing::Point(6, 6);
			this->getTemperature_TempTab_TextBox->Multiline = true;
			this->getTemperature_TempTab_TextBox->Name = L"getTemperature_TempTab_TextBox";
			this->getTemperature_TempTab_TextBox->ReadOnly = true;
			this->getTemperature_TempTab_TextBox->Size = System::Drawing::Size(617, 90);
			this->getTemperature_TempTab_TextBox->TabIndex = 0;
			this->getTemperature_TempTab_TextBox->Text = L"Get Wistone Temperature:\r\n\r\n^ Choose \"Destination\" ( Network ID of Wistone or 0 f" 
				L"or Broadcast )\r\n^ Press \"Get Temperature\" to get the temperature in Celsius\r\n";
			// 
			// readLL_TempTab_TabPage
			// 
			this->readLL_TempTab_TabPage->BackColor = System::Drawing::Color::WhiteSmoke;
			this->readLL_TempTab_TabPage->Controls->Add(this->destination_readLL_TemperatureTab_Label);
			this->readLL_TempTab_TabPage->Controls->Add(this->destination_readLL_TemperatureTab_TextBox);
			this->readLL_TempTab_TabPage->Controls->Add(this->read_readLL_TempTab_Button);
			this->readLL_TempTab_TabPage->Controls->Add(this->address_readLL_TempTab_Label);
			this->readLL_TempTab_TabPage->Controls->Add(this->address_readLL_TempTab_TextBox);
			this->readLL_TempTab_TabPage->Controls->Add(this->readLL_TempTab_TextBox);
			this->readLL_TempTab_TabPage->Location = System::Drawing::Point(4, 22);
			this->readLL_TempTab_TabPage->Name = L"readLL_TempTab_TabPage";
			this->readLL_TempTab_TabPage->Padding = System::Windows::Forms::Padding(3);
			this->readLL_TempTab_TabPage->Size = System::Drawing::Size(632, 319);
			this->readLL_TempTab_TabPage->TabIndex = 0;
			this->readLL_TempTab_TabPage->Text = L"Read (Low Level)";
			// 
			// destination_readLL_TemperatureTab_Label
			// 
			this->destination_readLL_TemperatureTab_Label->AutoSize = true;
			this->destination_readLL_TemperatureTab_Label->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(177)));
			this->destination_readLL_TemperatureTab_Label->Location = System::Drawing::Point(43, 182);
			this->destination_readLL_TemperatureTab_Label->Name = L"destination_readLL_TemperatureTab_Label";
			this->destination_readLL_TemperatureTab_Label->Size = System::Drawing::Size(83, 17);
			this->destination_readLL_TemperatureTab_Label->TabIndex = 14;
			this->destination_readLL_TemperatureTab_Label->Text = L"Destination:";
			// 
			// destination_readLL_TemperatureTab_TextBox
			// 
			this->destination_readLL_TemperatureTab_TextBox->Location = System::Drawing::Point(127, 179);
			this->destination_readLL_TemperatureTab_TextBox->Name = L"destination_readLL_TemperatureTab_TextBox";
			this->destination_readLL_TemperatureTab_TextBox->Size = System::Drawing::Size(70, 20);
			this->destination_readLL_TemperatureTab_TextBox->TabIndex = 13;
			// 
			// read_readLL_TempTab_Button
			// 
			this->read_readLL_TempTab_Button->AutoSize = true;
			this->read_readLL_TempTab_Button->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
			this->read_readLL_TempTab_Button->Location = System::Drawing::Point(266, 250);
			this->read_readLL_TempTab_Button->Name = L"read_readLL_TempTab_Button";
			this->read_readLL_TempTab_Button->Size = System::Drawing::Size(97, 28);
			this->read_readLL_TempTab_Button->TabIndex = 12;
			this->read_readLL_TempTab_Button->Text = L"Read";
			this->read_readLL_TempTab_Button->UseVisualStyleBackColor = true;
			this->read_readLL_TempTab_Button->Click += gcnew System::EventHandler(this, &GUI::read_readLL_TempTab_Button_Click);
			// 
			// address_readLL_TempTab_Label
			// 
			this->address_readLL_TempTab_Label->AutoSize = true;
			this->address_readLL_TempTab_Label->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->address_readLL_TempTab_Label->Location = System::Drawing::Point(42, 217);
			this->address_readLL_TempTab_Label->Name = L"address_readLL_TempTab_Label";
			this->address_readLL_TempTab_Label->Size = System::Drawing::Size(64, 17);
			this->address_readLL_TempTab_Label->TabIndex = 10;
			this->address_readLL_TempTab_Label->Text = L"Address:";
			// 
			// address_readLL_TempTab_TextBox
			// 
			this->address_readLL_TempTab_TextBox->Location = System::Drawing::Point(127, 217);
			this->address_readLL_TempTab_TextBox->Name = L"address_readLL_TempTab_TextBox";
			this->address_readLL_TempTab_TextBox->Size = System::Drawing::Size(70, 20);
			this->address_readLL_TempTab_TextBox->TabIndex = 9;
			this->address_readLL_TempTab_TextBox->Text = L"0";
			// 
			// readLL_TempTab_TextBox
			// 
			this->readLL_TempTab_TextBox->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->readLL_TempTab_TextBox->Location = System::Drawing::Point(6, 6);
			this->readLL_TempTab_TextBox->Multiline = true;
			this->readLL_TempTab_TextBox->Name = L"readLL_TempTab_TextBox";
			this->readLL_TempTab_TextBox->ReadOnly = true;
			this->readLL_TempTab_TextBox->Size = System::Drawing::Size(617, 106);
			this->readLL_TempTab_TextBox->TabIndex = 8;
			this->readLL_TempTab_TextBox->Text = L"Read the registers of Wistone\'s Temperature Sensor:\r\n\r\n^ Choose \"Destination\" ( N" 
				L"etwork ID of Wistone or 0 for Broadcast )\r\n^ Enter: \"Address\" ( of the register " 
				L")\r\n^ Press: \"Read\"";
			// 
			// writeLL_TempTab_TabPage
			// 
			this->writeLL_TempTab_TabPage->BackColor = System::Drawing::Color::WhiteSmoke;
			this->writeLL_TempTab_TabPage->Controls->Add(this->destination_writeLL_TemperatureTab_Label);
			this->writeLL_TempTab_TabPage->Controls->Add(this->destination_writeLL_TemperatureTab_TextBox);
			this->writeLL_TempTab_TabPage->Controls->Add(this->write_writeLL_TempTab_Button);
			this->writeLL_TempTab_TabPage->Controls->Add(this->data_writeLL_TempTab_Label);
			this->writeLL_TempTab_TabPage->Controls->Add(this->data_writeLL_TempTab_TextBox);
			this->writeLL_TempTab_TabPage->Controls->Add(this->address_writeLL_TempTab_TextBox);
			this->writeLL_TempTab_TabPage->Controls->Add(this->writeLL_TempTab_TextBox);
			this->writeLL_TempTab_TabPage->Controls->Add(this->address_writeLL_TempTab_Label);
			this->writeLL_TempTab_TabPage->Location = System::Drawing::Point(4, 22);
			this->writeLL_TempTab_TabPage->Name = L"writeLL_TempTab_TabPage";
			this->writeLL_TempTab_TabPage->Padding = System::Windows::Forms::Padding(3);
			this->writeLL_TempTab_TabPage->Size = System::Drawing::Size(632, 319);
			this->writeLL_TempTab_TabPage->TabIndex = 1;
			this->writeLL_TempTab_TabPage->Text = L"Write (Low Level)";
			// 
			// destination_writeLL_TemperatureTab_Label
			// 
			this->destination_writeLL_TemperatureTab_Label->AutoSize = true;
			this->destination_writeLL_TemperatureTab_Label->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(177)));
			this->destination_writeLL_TemperatureTab_Label->Location = System::Drawing::Point(43, 141);
			this->destination_writeLL_TemperatureTab_Label->Name = L"destination_writeLL_TemperatureTab_Label";
			this->destination_writeLL_TemperatureTab_Label->Size = System::Drawing::Size(83, 17);
			this->destination_writeLL_TemperatureTab_Label->TabIndex = 14;
			this->destination_writeLL_TemperatureTab_Label->Text = L"Destination:";
			// 
			// destination_writeLL_TemperatureTab_TextBox
			// 
			this->destination_writeLL_TemperatureTab_TextBox->Location = System::Drawing::Point(127, 139);
			this->destination_writeLL_TemperatureTab_TextBox->Name = L"destination_writeLL_TemperatureTab_TextBox";
			this->destination_writeLL_TemperatureTab_TextBox->Size = System::Drawing::Size(71, 20);
			this->destination_writeLL_TemperatureTab_TextBox->TabIndex = 13;
			// 
			// write_writeLL_TempTab_Button
			// 
			this->write_writeLL_TempTab_Button->AutoSize = true;
			this->write_writeLL_TempTab_Button->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
			this->write_writeLL_TempTab_Button->Location = System::Drawing::Point(266, 250);
			this->write_writeLL_TempTab_Button->Name = L"write_writeLL_TempTab_Button";
			this->write_writeLL_TempTab_Button->Size = System::Drawing::Size(97, 28);
			this->write_writeLL_TempTab_Button->TabIndex = 12;
			this->write_writeLL_TempTab_Button->Text = L"Write";
			this->write_writeLL_TempTab_Button->UseVisualStyleBackColor = true;
			this->write_writeLL_TempTab_Button->Click += gcnew System::EventHandler(this, &GUI::write_writeLL_TempTab_Button_Click);
			// 
			// data_writeLL_TempTab_Label
			// 
			this->data_writeLL_TempTab_Label->AutoSize = true;
			this->data_writeLL_TempTab_Label->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->data_writeLL_TempTab_Label->Location = System::Drawing::Point(43, 217);
			this->data_writeLL_TempTab_Label->Name = L"data_writeLL_TempTab_Label";
			this->data_writeLL_TempTab_Label->Size = System::Drawing::Size(42, 17);
			this->data_writeLL_TempTab_Label->TabIndex = 11;
			this->data_writeLL_TempTab_Label->Text = L"Data:";
			// 
			// data_writeLL_TempTab_TextBox
			// 
			this->data_writeLL_TempTab_TextBox->Location = System::Drawing::Point(127, 217);
			this->data_writeLL_TempTab_TextBox->Multiline = true;
			this->data_writeLL_TempTab_TextBox->Name = L"data_writeLL_TempTab_TextBox";
			this->data_writeLL_TempTab_TextBox->Size = System::Drawing::Size(71, 20);
			this->data_writeLL_TempTab_TextBox->TabIndex = 10;
			// 
			// address_writeLL_TempTab_TextBox
			// 
			this->address_writeLL_TempTab_TextBox->Location = System::Drawing::Point(127, 179);
			this->address_writeLL_TempTab_TextBox->Name = L"address_writeLL_TempTab_TextBox";
			this->address_writeLL_TempTab_TextBox->Size = System::Drawing::Size(71, 20);
			this->address_writeLL_TempTab_TextBox->TabIndex = 8;
			this->address_writeLL_TempTab_TextBox->Text = L"0";
			// 
			// writeLL_TempTab_TextBox
			// 
			this->writeLL_TempTab_TextBox->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->writeLL_TempTab_TextBox->Location = System::Drawing::Point(6, 3);
			this->writeLL_TempTab_TextBox->Multiline = true;
			this->writeLL_TempTab_TextBox->Name = L"writeLL_TempTab_TextBox";
			this->writeLL_TempTab_TextBox->ReadOnly = true;
			this->writeLL_TempTab_TextBox->Size = System::Drawing::Size(617, 101);
			this->writeLL_TempTab_TextBox->TabIndex = 7;
			this->writeLL_TempTab_TextBox->Text = L"Write the registers of Wistone\'s Temperature Sensor:\r\n\r\n^ Choose \"Destination\" ( " 
				L"Network ID of Wistone or 0 for Broadcast )\r\n^ Enter: \"Address\" ( of the register" 
				L" ) and \"Data\"\r\n^ Press: \"Write\"";
			// 
			// address_writeLL_TempTab_Label
			// 
			this->address_writeLL_TempTab_Label->AutoSize = true;
			this->address_writeLL_TempTab_Label->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->address_writeLL_TempTab_Label->Location = System::Drawing::Point(43, 179);
			this->address_writeLL_TempTab_Label->Name = L"address_writeLL_TempTab_Label";
			this->address_writeLL_TempTab_Label->Size = System::Drawing::Size(64, 17);
			this->address_writeLL_TempTab_Label->TabIndex = 9;
			this->address_writeLL_TempTab_Label->Text = L"Address:";
			// 
			// tabWakeUp
			// 
			this->tabWakeUp->BackColor = System::Drawing::Color::WhiteSmoke;
			this->tabWakeUp->Controls->Add(this->wakeUpTab);
			this->tabWakeUp->Location = System::Drawing::Point(4, 22);
			this->tabWakeUp->Name = L"tabWakeUp";
			this->tabWakeUp->Padding = System::Windows::Forms::Padding(3);
			this->tabWakeUp->Size = System::Drawing::Size(633, 339);
			this->tabWakeUp->TabIndex = 5;
			this->tabWakeUp->Text = L"WakeUp";
			// 
			// wakeUpTab
			// 
			this->wakeUpTab->Controls->Add(this->setTime_WakeUpTab_TabPage);
			this->wakeUpTab->Controls->Add(this->setDate_WakeUpTab_TabPage);
			this->wakeUpTab->Controls->Add(this->getTime_WakeUpTab_TabPage);
			this->wakeUpTab->Controls->Add(this->getDate_WakeUpTab_TabPage);
			this->wakeUpTab->Controls->Add(this->setWakeUp_WakeUpTab_TabPage);
			this->wakeUpTab->Controls->Add(this->setAlarm_WakeUpTab_TabPage);
			this->wakeUpTab->Controls->Add(this->setBoot_WakeUpTab_TabPage);
			this->wakeUpTab->Controls->Add(this->readLL_WakeUpTab_TabPage);
			this->wakeUpTab->Controls->Add(this->writeLL_WakeUpTab_TabPage);
			this->wakeUpTab->Location = System::Drawing::Point(0, 0);
			this->wakeUpTab->Name = L"wakeUpTab";
			this->wakeUpTab->SelectedIndex = 0;
			this->wakeUpTab->Size = System::Drawing::Size(640, 345);
			this->wakeUpTab->TabIndex = 1;
			// 
			// setTime_WakeUpTab_TabPage
			// 
			this->setTime_WakeUpTab_TabPage->BackColor = System::Drawing::Color::WhiteSmoke;
			this->setTime_WakeUpTab_TabPage->Controls->Add(this->destination_setTime_WakeUpTab_Label);
			this->setTime_WakeUpTab_TabPage->Controls->Add(this->destination_setTime_WakeUpTab_TextBox);
			this->setTime_WakeUpTab_TabPage->Controls->Add(this->setTime_setTime_WakeUpTab_Button);
			this->setTime_WakeUpTab_TabPage->Controls->Add(this->second_setTime_WakeUpTab_Label);
			this->setTime_WakeUpTab_TabPage->Controls->Add(this->minute_setTime_WakeUpTab_Label);
			this->setTime_WakeUpTab_TabPage->Controls->Add(this->hour_setTime_WakeUpTab_Label);
			this->setTime_WakeUpTab_TabPage->Controls->Add(this->second_setTime_WakeUpTab_NumericUpDown);
			this->setTime_WakeUpTab_TabPage->Controls->Add(this->minute_setTime_WakeUpTab_NumericUpDown);
			this->setTime_WakeUpTab_TabPage->Controls->Add(this->hour_setTime_WakeUpTab_NumericUpDown);
			this->setTime_WakeUpTab_TabPage->Controls->Add(this->setTime_WakeUpTab_TextBox);
			this->setTime_WakeUpTab_TabPage->Location = System::Drawing::Point(4, 22);
			this->setTime_WakeUpTab_TabPage->Name = L"setTime_WakeUpTab_TabPage";
			this->setTime_WakeUpTab_TabPage->Padding = System::Windows::Forms::Padding(3);
			this->setTime_WakeUpTab_TabPage->Size = System::Drawing::Size(632, 319);
			this->setTime_WakeUpTab_TabPage->TabIndex = 2;
			this->setTime_WakeUpTab_TabPage->Text = L"Set Time";
			// 
			// destination_setTime_WakeUpTab_Label
			// 
			this->destination_setTime_WakeUpTab_Label->AutoSize = true;
			this->destination_setTime_WakeUpTab_Label->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(177)));
			this->destination_setTime_WakeUpTab_Label->Location = System::Drawing::Point(66, 123);
			this->destination_setTime_WakeUpTab_Label->Name = L"destination_setTime_WakeUpTab_Label";
			this->destination_setTime_WakeUpTab_Label->Size = System::Drawing::Size(83, 17);
			this->destination_setTime_WakeUpTab_Label->TabIndex = 13;
			this->destination_setTime_WakeUpTab_Label->Text = L"Destination:";
			// 
			// destination_setTime_WakeUpTab_TextBox
			// 
			this->destination_setTime_WakeUpTab_TextBox->Location = System::Drawing::Point(149, 121);
			this->destination_setTime_WakeUpTab_TextBox->Name = L"destination_setTime_WakeUpTab_TextBox";
			this->destination_setTime_WakeUpTab_TextBox->Size = System::Drawing::Size(48, 20);
			this->destination_setTime_WakeUpTab_TextBox->TabIndex = 12;
			// 
			// setTime_setTime_WakeUpTab_Button
			// 
			this->setTime_setTime_WakeUpTab_Button->AutoSize = true;
			this->setTime_setTime_WakeUpTab_Button->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
			this->setTime_setTime_WakeUpTab_Button->Location = System::Drawing::Point(276, 250);
			this->setTime_setTime_WakeUpTab_Button->Name = L"setTime_setTime_WakeUpTab_Button";
			this->setTime_setTime_WakeUpTab_Button->Size = System::Drawing::Size(80, 28);
			this->setTime_setTime_WakeUpTab_Button->TabIndex = 11;
			this->setTime_setTime_WakeUpTab_Button->Text = L"Set Time";
			this->setTime_setTime_WakeUpTab_Button->UseVisualStyleBackColor = true;
			this->setTime_setTime_WakeUpTab_Button->Click += gcnew System::EventHandler(this, &GUI::setTime_WakeUpTab_Button_Click);
			// 
			// second_setTime_WakeUpTab_Label
			// 
			this->second_setTime_WakeUpTab_Label->AutoSize = true;
			this->second_setTime_WakeUpTab_Label->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->second_setTime_WakeUpTab_Label->ForeColor = System::Drawing::SystemColors::Desktop;
			this->second_setTime_WakeUpTab_Label->Location = System::Drawing::Point(65, 225);
			this->second_setTime_WakeUpTab_Label->Name = L"second_setTime_WakeUpTab_Label";
			this->second_setTime_WakeUpTab_Label->Size = System::Drawing::Size(60, 17);
			this->second_setTime_WakeUpTab_Label->TabIndex = 6;
			this->second_setTime_WakeUpTab_Label->Text = L"Second:";
			// 
			// minute_setTime_WakeUpTab_Label
			// 
			this->minute_setTime_WakeUpTab_Label->AutoSize = true;
			this->minute_setTime_WakeUpTab_Label->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->minute_setTime_WakeUpTab_Label->ForeColor = System::Drawing::SystemColors::Desktop;
			this->minute_setTime_WakeUpTab_Label->Location = System::Drawing::Point(65, 191);
			this->minute_setTime_WakeUpTab_Label->Name = L"minute_setTime_WakeUpTab_Label";
			this->minute_setTime_WakeUpTab_Label->Size = System::Drawing::Size(54, 17);
			this->minute_setTime_WakeUpTab_Label->TabIndex = 5;
			this->minute_setTime_WakeUpTab_Label->Text = L"Minute:";
			// 
			// hour_setTime_WakeUpTab_Label
			// 
			this->hour_setTime_WakeUpTab_Label->AutoSize = true;
			this->hour_setTime_WakeUpTab_Label->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->hour_setTime_WakeUpTab_Label->ForeColor = System::Drawing::SystemColors::Desktop;
			this->hour_setTime_WakeUpTab_Label->Location = System::Drawing::Point(66, 157);
			this->hour_setTime_WakeUpTab_Label->Name = L"hour_setTime_WakeUpTab_Label";
			this->hour_setTime_WakeUpTab_Label->Size = System::Drawing::Size(43, 17);
			this->hour_setTime_WakeUpTab_Label->TabIndex = 4;
			this->hour_setTime_WakeUpTab_Label->Text = L"Hour:";
			// 
			// second_setTime_WakeUpTab_NumericUpDown
			// 
			this->second_setTime_WakeUpTab_NumericUpDown->Location = System::Drawing::Point(149, 223);
			this->second_setTime_WakeUpTab_NumericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {59, 0, 0, 0});
			this->second_setTime_WakeUpTab_NumericUpDown->Name = L"second_setTime_WakeUpTab_NumericUpDown";
			this->second_setTime_WakeUpTab_NumericUpDown->Size = System::Drawing::Size(48, 20);
			this->second_setTime_WakeUpTab_NumericUpDown->TabIndex = 3;
			// 
			// minute_setTime_WakeUpTab_NumericUpDown
			// 
			this->minute_setTime_WakeUpTab_NumericUpDown->Location = System::Drawing::Point(149, 189);
			this->minute_setTime_WakeUpTab_NumericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {59, 0, 0, 0});
			this->minute_setTime_WakeUpTab_NumericUpDown->Name = L"minute_setTime_WakeUpTab_NumericUpDown";
			this->minute_setTime_WakeUpTab_NumericUpDown->Size = System::Drawing::Size(48, 20);
			this->minute_setTime_WakeUpTab_NumericUpDown->TabIndex = 2;
			// 
			// hour_setTime_WakeUpTab_NumericUpDown
			// 
			this->hour_setTime_WakeUpTab_NumericUpDown->Location = System::Drawing::Point(149, 155);
			this->hour_setTime_WakeUpTab_NumericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {23, 0, 0, 0});
			this->hour_setTime_WakeUpTab_NumericUpDown->Name = L"hour_setTime_WakeUpTab_NumericUpDown";
			this->hour_setTime_WakeUpTab_NumericUpDown->Size = System::Drawing::Size(48, 20);
			this->hour_setTime_WakeUpTab_NumericUpDown->TabIndex = 1;
			// 
			// setTime_WakeUpTab_TextBox
			// 
			this->setTime_WakeUpTab_TextBox->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->setTime_WakeUpTab_TextBox->Location = System::Drawing::Point(6, 4);
			this->setTime_WakeUpTab_TextBox->Multiline = true;
			this->setTime_WakeUpTab_TextBox->Name = L"setTime_WakeUpTab_TextBox";
			this->setTime_WakeUpTab_TextBox->ReadOnly = true;
			this->setTime_WakeUpTab_TextBox->Size = System::Drawing::Size(620, 100);
			this->setTime_WakeUpTab_TextBox->TabIndex = 0;
			this->setTime_WakeUpTab_TextBox->Text = L"Set Wistone Time:\r\n\r\n^ Choose \"Destination\" ( Network ID of Wistone or 0 for Broa" 
				L"dcast )\r\n^ Choose: hour, minute and second\r\n^ Press \"Set Time\"\r\n";
			// 
			// setDate_WakeUpTab_TabPage
			// 
			this->setDate_WakeUpTab_TabPage->BackColor = System::Drawing::Color::WhiteSmoke;
			this->setDate_WakeUpTab_TabPage->Controls->Add(this->destination_setDate_WakeUpTab_Label);
			this->setDate_WakeUpTab_TabPage->Controls->Add(this->destination_setDate_WakeUpTab_TextBox);
			this->setDate_WakeUpTab_TabPage->Controls->Add(this->weekDay_setDate_WakeUpTab_Label);
			this->setDate_WakeUpTab_TabPage->Controls->Add(this->weekDay_setDate_WakeUpTab_NumericUpDown);
			this->setDate_WakeUpTab_TabPage->Controls->Add(this->setDate_setDate_WakeUpTab_Button);
			this->setDate_WakeUpTab_TabPage->Controls->Add(this->year_setDate_WakeUpTab_Label);
			this->setDate_WakeUpTab_TabPage->Controls->Add(this->month_setDate_WakeUpTab_Label);
			this->setDate_WakeUpTab_TabPage->Controls->Add(this->day_setDate_WakeUpTab_Label);
			this->setDate_WakeUpTab_TabPage->Controls->Add(this->year_setDate_WakeUpTab_NumericUpDown);
			this->setDate_WakeUpTab_TabPage->Controls->Add(this->month_setDate_WakeUpTab_NumericUpDown);
			this->setDate_WakeUpTab_TabPage->Controls->Add(this->day_setDate_WakeUpTab_NumericUpDown);
			this->setDate_WakeUpTab_TabPage->Controls->Add(this->setDate_WakeUpTab_TextBox);
			this->setDate_WakeUpTab_TabPage->Location = System::Drawing::Point(4, 22);
			this->setDate_WakeUpTab_TabPage->Name = L"setDate_WakeUpTab_TabPage";
			this->setDate_WakeUpTab_TabPage->Padding = System::Windows::Forms::Padding(3);
			this->setDate_WakeUpTab_TabPage->Size = System::Drawing::Size(632, 319);
			this->setDate_WakeUpTab_TabPage->TabIndex = 4;
			this->setDate_WakeUpTab_TabPage->Text = L"Set Date";
			// 
			// destination_setDate_WakeUpTab_Label
			// 
			this->destination_setDate_WakeUpTab_Label->AutoSize = true;
			this->destination_setDate_WakeUpTab_Label->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(177)));
			this->destination_setDate_WakeUpTab_Label->Location = System::Drawing::Point(59, 164);
			this->destination_setDate_WakeUpTab_Label->Name = L"destination_setDate_WakeUpTab_Label";
			this->destination_setDate_WakeUpTab_Label->Size = System::Drawing::Size(83, 17);
			this->destination_setDate_WakeUpTab_Label->TabIndex = 18;
			this->destination_setDate_WakeUpTab_Label->Text = L"Destination:";
			// 
			// destination_setDate_WakeUpTab_TextBox
			// 
			this->destination_setDate_WakeUpTab_TextBox->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(177)));
			this->destination_setDate_WakeUpTab_TextBox->Location = System::Drawing::Point(150, 162);
			this->destination_setDate_WakeUpTab_TextBox->Name = L"destination_setDate_WakeUpTab_TextBox";
			this->destination_setDate_WakeUpTab_TextBox->Size = System::Drawing::Size(48, 23);
			this->destination_setDate_WakeUpTab_TextBox->TabIndex = 17;
			// 
			// weekDay_setDate_WakeUpTab_Label
			// 
			this->weekDay_setDate_WakeUpTab_Label->AutoSize = true;
			this->weekDay_setDate_WakeUpTab_Label->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->weekDay_setDate_WakeUpTab_Label->ForeColor = System::Drawing::SystemColors::Desktop;
			this->weekDay_setDate_WakeUpTab_Label->Location = System::Drawing::Point(382, 223);
			this->weekDay_setDate_WakeUpTab_Label->Name = L"weekDay_setDate_WakeUpTab_Label";
			this->weekDay_setDate_WakeUpTab_Label->Size = System::Drawing::Size(77, 17);
			this->weekDay_setDate_WakeUpTab_Label->TabIndex = 16;
			this->weekDay_setDate_WakeUpTab_Label->Text = L"Week Day:";
			// 
			// weekDay_setDate_WakeUpTab_NumericUpDown
			// 
			this->weekDay_setDate_WakeUpTab_NumericUpDown->Location = System::Drawing::Point(465, 220);
			this->weekDay_setDate_WakeUpTab_NumericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {7, 0, 0, 0});
			this->weekDay_setDate_WakeUpTab_NumericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 0});
			this->weekDay_setDate_WakeUpTab_NumericUpDown->Name = L"weekDay_setDate_WakeUpTab_NumericUpDown";
			this->weekDay_setDate_WakeUpTab_NumericUpDown->Size = System::Drawing::Size(48, 20);
			this->weekDay_setDate_WakeUpTab_NumericUpDown->TabIndex = 15;
			this->weekDay_setDate_WakeUpTab_NumericUpDown->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 0});
			// 
			// setDate_setDate_WakeUpTab_Button
			// 
			this->setDate_setDate_WakeUpTab_Button->AutoSize = true;
			this->setDate_setDate_WakeUpTab_Button->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
			this->setDate_setDate_WakeUpTab_Button->Location = System::Drawing::Point(276, 250);
			this->setDate_setDate_WakeUpTab_Button->Name = L"setDate_setDate_WakeUpTab_Button";
			this->setDate_setDate_WakeUpTab_Button->Size = System::Drawing::Size(80, 28);
			this->setDate_setDate_WakeUpTab_Button->TabIndex = 14;
			this->setDate_setDate_WakeUpTab_Button->Text = L"Set Date";
			this->setDate_setDate_WakeUpTab_Button->UseVisualStyleBackColor = true;
			this->setDate_setDate_WakeUpTab_Button->Click += gcnew System::EventHandler(this, &GUI::setDate_WakeUpTab_Button_Click);
			// 
			// year_setDate_WakeUpTab_Label
			// 
			this->year_setDate_WakeUpTab_Label->AutoSize = true;
			this->year_setDate_WakeUpTab_Label->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->year_setDate_WakeUpTab_Label->ForeColor = System::Drawing::SystemColors::Desktop;
			this->year_setDate_WakeUpTab_Label->Location = System::Drawing::Point(417, 191);
			this->year_setDate_WakeUpTab_Label->Name = L"year_setDate_WakeUpTab_Label";
			this->year_setDate_WakeUpTab_Label->Size = System::Drawing::Size(42, 17);
			this->year_setDate_WakeUpTab_Label->TabIndex = 13;
			this->year_setDate_WakeUpTab_Label->Text = L"Year:";
			// 
			// month_setDate_WakeUpTab_Label
			// 
			this->month_setDate_WakeUpTab_Label->AutoSize = true;
			this->month_setDate_WakeUpTab_Label->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->month_setDate_WakeUpTab_Label->ForeColor = System::Drawing::SystemColors::Desktop;
			this->month_setDate_WakeUpTab_Label->Location = System::Drawing::Point(239, 191);
			this->month_setDate_WakeUpTab_Label->Name = L"month_setDate_WakeUpTab_Label";
			this->month_setDate_WakeUpTab_Label->Size = System::Drawing::Size(51, 17);
			this->month_setDate_WakeUpTab_Label->TabIndex = 12;
			this->month_setDate_WakeUpTab_Label->Text = L"Month:";
			// 
			// day_setDate_WakeUpTab_Label
			// 
			this->day_setDate_WakeUpTab_Label->AutoSize = true;
			this->day_setDate_WakeUpTab_Label->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->day_setDate_WakeUpTab_Label->ForeColor = System::Drawing::SystemColors::Desktop;
			this->day_setDate_WakeUpTab_Label->Location = System::Drawing::Point(105, 191);
			this->day_setDate_WakeUpTab_Label->Name = L"day_setDate_WakeUpTab_Label";
			this->day_setDate_WakeUpTab_Label->Size = System::Drawing::Size(37, 17);
			this->day_setDate_WakeUpTab_Label->TabIndex = 11;
			this->day_setDate_WakeUpTab_Label->Text = L"Day:";
			// 
			// year_setDate_WakeUpTab_NumericUpDown
			// 
			this->year_setDate_WakeUpTab_NumericUpDown->Location = System::Drawing::Point(464, 191);
			this->year_setDate_WakeUpTab_NumericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {99, 0, 0, 0});
			this->year_setDate_WakeUpTab_NumericUpDown->Name = L"year_setDate_WakeUpTab_NumericUpDown";
			this->year_setDate_WakeUpTab_NumericUpDown->Size = System::Drawing::Size(48, 20);
			this->year_setDate_WakeUpTab_NumericUpDown->TabIndex = 10;
			this->year_setDate_WakeUpTab_NumericUpDown->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {12, 0, 0, 0});
			// 
			// month_setDate_WakeUpTab_NumericUpDown
			// 
			this->month_setDate_WakeUpTab_NumericUpDown->Location = System::Drawing::Point(294, 191);
			this->month_setDate_WakeUpTab_NumericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {12, 0, 0, 0});
			this->month_setDate_WakeUpTab_NumericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 0});
			this->month_setDate_WakeUpTab_NumericUpDown->Name = L"month_setDate_WakeUpTab_NumericUpDown";
			this->month_setDate_WakeUpTab_NumericUpDown->Size = System::Drawing::Size(48, 20);
			this->month_setDate_WakeUpTab_NumericUpDown->TabIndex = 9;
			this->month_setDate_WakeUpTab_NumericUpDown->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 0});
			// 
			// day_setDate_WakeUpTab_NumericUpDown
			// 
			this->day_setDate_WakeUpTab_NumericUpDown->Location = System::Drawing::Point(150, 191);
			this->day_setDate_WakeUpTab_NumericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {31, 0, 0, 0});
			this->day_setDate_WakeUpTab_NumericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 0});
			this->day_setDate_WakeUpTab_NumericUpDown->Name = L"day_setDate_WakeUpTab_NumericUpDown";
			this->day_setDate_WakeUpTab_NumericUpDown->Size = System::Drawing::Size(48, 20);
			this->day_setDate_WakeUpTab_NumericUpDown->TabIndex = 8;
			this->day_setDate_WakeUpTab_NumericUpDown->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 0});
			// 
			// setDate_WakeUpTab_TextBox
			// 
			this->setDate_WakeUpTab_TextBox->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->setDate_WakeUpTab_TextBox->Location = System::Drawing::Point(3, 4);
			this->setDate_WakeUpTab_TextBox->Multiline = true;
			this->setDate_WakeUpTab_TextBox->Name = L"setDate_WakeUpTab_TextBox";
			this->setDate_WakeUpTab_TextBox->ReadOnly = true;
			this->setDate_WakeUpTab_TextBox->Size = System::Drawing::Size(624, 101);
			this->setDate_WakeUpTab_TextBox->TabIndex = 7;
			this->setDate_WakeUpTab_TextBox->Text = L"Set Wistone Date:\r\n\r\n^ Choose \"Destination\" ( Network ID of Wistone or 0 for Broa" 
				L"dcast )\r\n^ Choose the date: day, month, year and weekday \r\n^ Press \"Set Date\"";
			// 
			// getTime_WakeUpTab_TabPage
			// 
			this->getTime_WakeUpTab_TabPage->BackColor = System::Drawing::Color::WhiteSmoke;
			this->getTime_WakeUpTab_TabPage->Controls->Add(this->destination_getTime_WakeUpTab_Label);
			this->getTime_WakeUpTab_TabPage->Controls->Add(this->destination_getTime_WakeUpTab_TextBox);
			this->getTime_WakeUpTab_TabPage->Controls->Add(this->getTime_getTime_WakeUpTab_Button);
			this->getTime_WakeUpTab_TabPage->Controls->Add(this->getTime_WakeUpTab_TextBox);
			this->getTime_WakeUpTab_TabPage->Location = System::Drawing::Point(4, 22);
			this->getTime_WakeUpTab_TabPage->Name = L"getTime_WakeUpTab_TabPage";
			this->getTime_WakeUpTab_TabPage->Padding = System::Windows::Forms::Padding(3);
			this->getTime_WakeUpTab_TabPage->Size = System::Drawing::Size(632, 319);
			this->getTime_WakeUpTab_TabPage->TabIndex = 3;
			this->getTime_WakeUpTab_TabPage->Text = L"Get Time";
			// 
			// destination_getTime_WakeUpTab_Label
			// 
			this->destination_getTime_WakeUpTab_Label->AutoSize = true;
			this->destination_getTime_WakeUpTab_Label->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(177)));
			this->destination_getTime_WakeUpTab_Label->Location = System::Drawing::Point(64, 192);
			this->destination_getTime_WakeUpTab_Label->Name = L"destination_getTime_WakeUpTab_Label";
			this->destination_getTime_WakeUpTab_Label->Size = System::Drawing::Size(83, 17);
			this->destination_getTime_WakeUpTab_Label->TabIndex = 14;
			this->destination_getTime_WakeUpTab_Label->Text = L"Destination:";
			// 
			// destination_getTime_WakeUpTab_TextBox
			// 
			this->destination_getTime_WakeUpTab_TextBox->Location = System::Drawing::Point(150, 192);
			this->destination_getTime_WakeUpTab_TextBox->Name = L"destination_getTime_WakeUpTab_TextBox";
			this->destination_getTime_WakeUpTab_TextBox->Size = System::Drawing::Size(57, 20);
			this->destination_getTime_WakeUpTab_TextBox->TabIndex = 13;
			// 
			// getTime_getTime_WakeUpTab_Button
			// 
			this->getTime_getTime_WakeUpTab_Button->AutoSize = true;
			this->getTime_getTime_WakeUpTab_Button->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
			this->getTime_getTime_WakeUpTab_Button->Location = System::Drawing::Point(276, 250);
			this->getTime_getTime_WakeUpTab_Button->Name = L"getTime_getTime_WakeUpTab_Button";
			this->getTime_getTime_WakeUpTab_Button->Size = System::Drawing::Size(80, 28);
			this->getTime_getTime_WakeUpTab_Button->TabIndex = 12;
			this->getTime_getTime_WakeUpTab_Button->Text = L"Get Time";
			this->getTime_getTime_WakeUpTab_Button->UseVisualStyleBackColor = true;
			this->getTime_getTime_WakeUpTab_Button->Click += gcnew System::EventHandler(this, &GUI::getTime_WakeUpTab_Button_Click);
			// 
			// getTime_WakeUpTab_TextBox
			// 
			this->getTime_WakeUpTab_TextBox->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->getTime_WakeUpTab_TextBox->Location = System::Drawing::Point(6, 4);
			this->getTime_WakeUpTab_TextBox->Multiline = true;
			this->getTime_WakeUpTab_TextBox->Name = L"getTime_WakeUpTab_TextBox";
			this->getTime_WakeUpTab_TextBox->ReadOnly = true;
			this->getTime_WakeUpTab_TextBox->Size = System::Drawing::Size(620, 88);
			this->getTime_WakeUpTab_TextBox->TabIndex = 2;
			this->getTime_WakeUpTab_TextBox->Text = L"Get Wistone Time:\r\n\r\n^ Choose \"Destination\" ( Network ID of Wistone or 0 for Broa" 
				L"dcast )\r\n^ Press \"Get Time\" to get the time in [ hour : minute : second ] format" 
				L"";
			// 
			// getDate_WakeUpTab_TabPage
			// 
			this->getDate_WakeUpTab_TabPage->BackColor = System::Drawing::Color::WhiteSmoke;
			this->getDate_WakeUpTab_TabPage->Controls->Add(this->destination_getDate_WakeUpTab_Label);
			this->getDate_WakeUpTab_TabPage->Controls->Add(this->destination_getDate_WakeUpTab_TextBox);
			this->getDate_WakeUpTab_TabPage->Controls->Add(this->getDate_getDate_WakeUpTab_Button);
			this->getDate_WakeUpTab_TabPage->Controls->Add(this->getDate_WakeUpTab_TextBox);
			this->getDate_WakeUpTab_TabPage->Location = System::Drawing::Point(4, 22);
			this->getDate_WakeUpTab_TabPage->Name = L"getDate_WakeUpTab_TabPage";
			this->getDate_WakeUpTab_TabPage->Padding = System::Windows::Forms::Padding(3);
			this->getDate_WakeUpTab_TabPage->Size = System::Drawing::Size(632, 319);
			this->getDate_WakeUpTab_TabPage->TabIndex = 5;
			this->getDate_WakeUpTab_TabPage->Text = L"Get Date";
			// 
			// destination_getDate_WakeUpTab_Label
			// 
			this->destination_getDate_WakeUpTab_Label->AutoSize = true;
			this->destination_getDate_WakeUpTab_Label->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(177)));
			this->destination_getDate_WakeUpTab_Label->Location = System::Drawing::Point(64, 192);
			this->destination_getDate_WakeUpTab_Label->Name = L"destination_getDate_WakeUpTab_Label";
			this->destination_getDate_WakeUpTab_Label->Size = System::Drawing::Size(83, 17);
			this->destination_getDate_WakeUpTab_Label->TabIndex = 14;
			this->destination_getDate_WakeUpTab_Label->Text = L"Destination:";
			// 
			// destination_getDate_WakeUpTab_TextBox
			// 
			this->destination_getDate_WakeUpTab_TextBox->Location = System::Drawing::Point(150, 192);
			this->destination_getDate_WakeUpTab_TextBox->Name = L"destination_getDate_WakeUpTab_TextBox";
			this->destination_getDate_WakeUpTab_TextBox->Size = System::Drawing::Size(57, 20);
			this->destination_getDate_WakeUpTab_TextBox->TabIndex = 13;
			// 
			// getDate_getDate_WakeUpTab_Button
			// 
			this->getDate_getDate_WakeUpTab_Button->AutoSize = true;
			this->getDate_getDate_WakeUpTab_Button->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
			this->getDate_getDate_WakeUpTab_Button->Location = System::Drawing::Point(276, 250);
			this->getDate_getDate_WakeUpTab_Button->Name = L"getDate_getDate_WakeUpTab_Button";
			this->getDate_getDate_WakeUpTab_Button->Size = System::Drawing::Size(80, 28);
			this->getDate_getDate_WakeUpTab_Button->TabIndex = 12;
			this->getDate_getDate_WakeUpTab_Button->Text = L"Get Date";
			this->getDate_getDate_WakeUpTab_Button->UseVisualStyleBackColor = true;
			this->getDate_getDate_WakeUpTab_Button->Click += gcnew System::EventHandler(this, &GUI::getDate_WakeUpTab_Button_Click);
			// 
			// getDate_WakeUpTab_TextBox
			// 
			this->getDate_WakeUpTab_TextBox->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->getDate_WakeUpTab_TextBox->Location = System::Drawing::Point(5, 4);
			this->getDate_WakeUpTab_TextBox->Multiline = true;
			this->getDate_WakeUpTab_TextBox->Name = L"getDate_WakeUpTab_TextBox";
			this->getDate_WakeUpTab_TextBox->ReadOnly = true;
			this->getDate_WakeUpTab_TextBox->Size = System::Drawing::Size(617, 88);
			this->getDate_WakeUpTab_TextBox->TabIndex = 3;
			this->getDate_WakeUpTab_TextBox->Text = L"Get Wistone Date:\r\n\r\n^ Choose \"Destination\" ( Network ID of Wistone or 0 for Broa" 
				L"dcast )\r\n^ Press \"Get Date\" to get the date in [ day/month/year - weekday ] form" 
				L"at";
			// 
			// setWakeUp_WakeUpTab_TabPage
			// 
			this->setWakeUp_WakeUpTab_TabPage->BackColor = System::Drawing::Color::WhiteSmoke;
			this->setWakeUp_WakeUpTab_TabPage->Controls->Add(this->destination_setWakeUp_WakeUpTab_Label);
			this->setWakeUp_WakeUpTab_TabPage->Controls->Add(this->destination_setWakeUp_WakeUpTab_TextBox);
			this->setWakeUp_WakeUpTab_TabPage->Controls->Add(this->delay_setWakeUp_WakeUpTab_TextBox);
			this->setWakeUp_WakeUpTab_TabPage->Controls->Add(this->setWakeUp_WakeUpTab_TextBox);
			this->setWakeUp_WakeUpTab_TabPage->Controls->Add(this->delay_setWakeUp_WakeUpTab_Label);
			this->setWakeUp_WakeUpTab_TabPage->Controls->Add(this->apply_setWakeUp_WakeUpTab_Button);
			this->setWakeUp_WakeUpTab_TabPage->Location = System::Drawing::Point(4, 22);
			this->setWakeUp_WakeUpTab_TabPage->Name = L"setWakeUp_WakeUpTab_TabPage";
			this->setWakeUp_WakeUpTab_TabPage->Padding = System::Windows::Forms::Padding(3);
			this->setWakeUp_WakeUpTab_TabPage->Size = System::Drawing::Size(632, 319);
			this->setWakeUp_WakeUpTab_TabPage->TabIndex = 7;
			this->setWakeUp_WakeUpTab_TabPage->Text = L"Set Wake Up";
			// 
			// destination_setWakeUp_WakeUpTab_Label
			// 
			this->destination_setWakeUp_WakeUpTab_Label->AutoSize = true;
			this->destination_setWakeUp_WakeUpTab_Label->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(177)));
			this->destination_setWakeUp_WakeUpTab_Label->Location = System::Drawing::Point(64, 192);
			this->destination_setWakeUp_WakeUpTab_Label->Name = L"destination_setWakeUp_WakeUpTab_Label";
			this->destination_setWakeUp_WakeUpTab_Label->Size = System::Drawing::Size(83, 17);
			this->destination_setWakeUp_WakeUpTab_Label->TabIndex = 35;
			this->destination_setWakeUp_WakeUpTab_Label->Text = L"Destination:";
			// 
			// destination_setWakeUp_WakeUpTab_TextBox
			// 
			this->destination_setWakeUp_WakeUpTab_TextBox->Location = System::Drawing::Point(150, 192);
			this->destination_setWakeUp_WakeUpTab_TextBox->Name = L"destination_setWakeUp_WakeUpTab_TextBox";
			this->destination_setWakeUp_WakeUpTab_TextBox->Size = System::Drawing::Size(57, 20);
			this->destination_setWakeUp_WakeUpTab_TextBox->TabIndex = 34;
			// 
			// delay_setWakeUp_WakeUpTab_TextBox
			// 
			this->delay_setWakeUp_WakeUpTab_TextBox->Location = System::Drawing::Point(150, 229);
			this->delay_setWakeUp_WakeUpTab_TextBox->Name = L"delay_setWakeUp_WakeUpTab_TextBox";
			this->delay_setWakeUp_WakeUpTab_TextBox->Size = System::Drawing::Size(57, 20);
			this->delay_setWakeUp_WakeUpTab_TextBox->TabIndex = 33;
			this->delay_setWakeUp_WakeUpTab_TextBox->Text = L"1";
			// 
			// setWakeUp_WakeUpTab_TextBox
			// 
			this->setWakeUp_WakeUpTab_TextBox->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->setWakeUp_WakeUpTab_TextBox->Location = System::Drawing::Point(4, 3);
			this->setWakeUp_WakeUpTab_TextBox->Multiline = true;
			this->setWakeUp_WakeUpTab_TextBox->Name = L"setWakeUp_WakeUpTab_TextBox";
			this->setWakeUp_WakeUpTab_TextBox->ReadOnly = true;
			this->setWakeUp_WakeUpTab_TextBox->Size = System::Drawing::Size(618, 141);
			this->setWakeUp_WakeUpTab_TextBox->TabIndex = 31;
			this->setWakeUp_WakeUpTab_TextBox->Text = resources->GetString(L"setWakeUp_WakeUpTab_TextBox.Text");
			// 
			// delay_setWakeUp_WakeUpTab_Label
			// 
			this->delay_setWakeUp_WakeUpTab_Label->AutoSize = true;
			this->delay_setWakeUp_WakeUpTab_Label->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->delay_setWakeUp_WakeUpTab_Label->ForeColor = System::Drawing::SystemColors::Desktop;
			this->delay_setWakeUp_WakeUpTab_Label->Location = System::Drawing::Point(64, 230);
			this->delay_setWakeUp_WakeUpTab_Label->Name = L"delay_setWakeUp_WakeUpTab_Label";
			this->delay_setWakeUp_WakeUpTab_Label->Size = System::Drawing::Size(48, 17);
			this->delay_setWakeUp_WakeUpTab_Label->TabIndex = 32;
			this->delay_setWakeUp_WakeUpTab_Label->Text = L"Delay:";
			// 
			// apply_setWakeUp_WakeUpTab_Button
			// 
			this->apply_setWakeUp_WakeUpTab_Button->AutoSize = true;
			this->apply_setWakeUp_WakeUpTab_Button->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
			this->apply_setWakeUp_WakeUpTab_Button->FlatAppearance->BorderColor = System::Drawing::Color::White;
			this->apply_setWakeUp_WakeUpTab_Button->Location = System::Drawing::Point(276, 250);
			this->apply_setWakeUp_WakeUpTab_Button->Name = L"apply_setWakeUp_WakeUpTab_Button";
			this->apply_setWakeUp_WakeUpTab_Button->Size = System::Drawing::Size(80, 28);
			this->apply_setWakeUp_WakeUpTab_Button->TabIndex = 30;
			this->apply_setWakeUp_WakeUpTab_Button->Text = L"Apply";
			this->apply_setWakeUp_WakeUpTab_Button->UseVisualStyleBackColor = true;
			this->apply_setWakeUp_WakeUpTab_Button->Click += gcnew System::EventHandler(this, &GUI::apply_setWakeUp_WakeUpTab_Button_Click);
			// 
			// setAlarm_WakeUpTab_TabPage
			// 
			this->setAlarm_WakeUpTab_TabPage->BackColor = System::Drawing::Color::WhiteSmoke;
			this->setAlarm_WakeUpTab_TabPage->Controls->Add(this->destination_setAlarm_WakeUpTab_Label);
			this->setAlarm_WakeUpTab_TabPage->Controls->Add(this->destination_setAlarm_WakeUpTab_TextBox);
			this->setAlarm_WakeUpTab_TabPage->Controls->Add(this->minute_setAlarm_WakeUpTab_TextBox);
			this->setAlarm_WakeUpTab_TabPage->Controls->Add(this->day_setAlarm_WakeUpTab_TextBox);
			this->setAlarm_WakeUpTab_TabPage->Controls->Add(this->weekDay_setAlarm_WakeUpTab_TextBox);
			this->setAlarm_WakeUpTab_TabPage->Controls->Add(this->hour_setAlarm_WakeUpTab_TextBox);
			this->setAlarm_WakeUpTab_TabPage->Controls->Add(this->setAlarm_WakeUpTab_TextBox);
			this->setAlarm_WakeUpTab_TabPage->Controls->Add(this->weekDay_setAlarm_WakeUpTab_Label);
			this->setAlarm_WakeUpTab_TabPage->Controls->Add(this->apply_setAlarm_WakeUpTab_Button);
			this->setAlarm_WakeUpTab_TabPage->Controls->Add(this->day_setAlarm_WakeUpTab_Label);
			this->setAlarm_WakeUpTab_TabPage->Controls->Add(this->minute_setAlarm_WakeUpTab_Label);
			this->setAlarm_WakeUpTab_TabPage->Controls->Add(this->hour_setAlarm_WakeUpTab_Label);
			this->setAlarm_WakeUpTab_TabPage->Location = System::Drawing::Point(4, 22);
			this->setAlarm_WakeUpTab_TabPage->Name = L"setAlarm_WakeUpTab_TabPage";
			this->setAlarm_WakeUpTab_TabPage->Padding = System::Windows::Forms::Padding(3);
			this->setAlarm_WakeUpTab_TabPage->Size = System::Drawing::Size(632, 319);
			this->setAlarm_WakeUpTab_TabPage->TabIndex = 6;
			this->setAlarm_WakeUpTab_TabPage->Text = L"Set Alarm";
			// 
			// destination_setAlarm_WakeUpTab_Label
			// 
			this->destination_setAlarm_WakeUpTab_Label->AutoSize = true;
			this->destination_setAlarm_WakeUpTab_Label->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(177)));
			this->destination_setAlarm_WakeUpTab_Label->Location = System::Drawing::Point(60, 163);
			this->destination_setAlarm_WakeUpTab_Label->Name = L"destination_setAlarm_WakeUpTab_Label";
			this->destination_setAlarm_WakeUpTab_Label->Size = System::Drawing::Size(83, 17);
			this->destination_setAlarm_WakeUpTab_Label->TabIndex = 33;
			this->destination_setAlarm_WakeUpTab_Label->Text = L"Destination:";
			// 
			// destination_setAlarm_WakeUpTab_TextBox
			// 
			this->destination_setAlarm_WakeUpTab_TextBox->Location = System::Drawing::Point(150, 161);
			this->destination_setAlarm_WakeUpTab_TextBox->Name = L"destination_setAlarm_WakeUpTab_TextBox";
			this->destination_setAlarm_WakeUpTab_TextBox->Size = System::Drawing::Size(45, 20);
			this->destination_setAlarm_WakeUpTab_TextBox->TabIndex = 32;
			// 
			// minute_setAlarm_WakeUpTab_TextBox
			// 
			this->minute_setAlarm_WakeUpTab_TextBox->Location = System::Drawing::Point(296, 192);
			this->minute_setAlarm_WakeUpTab_TextBox->Name = L"minute_setAlarm_WakeUpTab_TextBox";
			this->minute_setAlarm_WakeUpTab_TextBox->Size = System::Drawing::Size(45, 20);
			this->minute_setAlarm_WakeUpTab_TextBox->TabIndex = 31;
			this->minute_setAlarm_WakeUpTab_TextBox->Text = L"x";
			// 
			// day_setAlarm_WakeUpTab_TextBox
			// 
			this->day_setAlarm_WakeUpTab_TextBox->Location = System::Drawing::Point(465, 190);
			this->day_setAlarm_WakeUpTab_TextBox->Name = L"day_setAlarm_WakeUpTab_TextBox";
			this->day_setAlarm_WakeUpTab_TextBox->Size = System::Drawing::Size(45, 20);
			this->day_setAlarm_WakeUpTab_TextBox->TabIndex = 30;
			this->day_setAlarm_WakeUpTab_TextBox->Text = L"x";
			// 
			// weekDay_setAlarm_WakeUpTab_TextBox
			// 
			this->weekDay_setAlarm_WakeUpTab_TextBox->Location = System::Drawing::Point(465, 220);
			this->weekDay_setAlarm_WakeUpTab_TextBox->Name = L"weekDay_setAlarm_WakeUpTab_TextBox";
			this->weekDay_setAlarm_WakeUpTab_TextBox->Size = System::Drawing::Size(45, 20);
			this->weekDay_setAlarm_WakeUpTab_TextBox->TabIndex = 29;
			this->weekDay_setAlarm_WakeUpTab_TextBox->Text = L"x";
			// 
			// hour_setAlarm_WakeUpTab_TextBox
			// 
			this->hour_setAlarm_WakeUpTab_TextBox->Location = System::Drawing::Point(150, 191);
			this->hour_setAlarm_WakeUpTab_TextBox->Name = L"hour_setAlarm_WakeUpTab_TextBox";
			this->hour_setAlarm_WakeUpTab_TextBox->Size = System::Drawing::Size(45, 20);
			this->hour_setAlarm_WakeUpTab_TextBox->TabIndex = 25;
			this->hour_setAlarm_WakeUpTab_TextBox->Text = L"x";
			// 
			// setAlarm_WakeUpTab_TextBox
			// 
			this->setAlarm_WakeUpTab_TextBox->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->setAlarm_WakeUpTab_TextBox->Location = System::Drawing::Point(4, 4);
			this->setAlarm_WakeUpTab_TextBox->Multiline = true;
			this->setAlarm_WakeUpTab_TextBox->Name = L"setAlarm_WakeUpTab_TextBox";
			this->setAlarm_WakeUpTab_TextBox->ReadOnly = true;
			this->setAlarm_WakeUpTab_TextBox->Size = System::Drawing::Size(618, 140);
			this->setAlarm_WakeUpTab_TextBox->TabIndex = 22;
			this->setAlarm_WakeUpTab_TextBox->Text = resources->GetString(L"setAlarm_WakeUpTab_TextBox.Text");
			// 
			// weekDay_setAlarm_WakeUpTab_Label
			// 
			this->weekDay_setAlarm_WakeUpTab_Label->AutoSize = true;
			this->weekDay_setAlarm_WakeUpTab_Label->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->weekDay_setAlarm_WakeUpTab_Label->ForeColor = System::Drawing::SystemColors::Desktop;
			this->weekDay_setAlarm_WakeUpTab_Label->Location = System::Drawing::Point(382, 220);
			this->weekDay_setAlarm_WakeUpTab_Label->Name = L"weekDay_setAlarm_WakeUpTab_Label";
			this->weekDay_setAlarm_WakeUpTab_Label->Size = System::Drawing::Size(77, 17);
			this->weekDay_setAlarm_WakeUpTab_Label->TabIndex = 24;
			this->weekDay_setAlarm_WakeUpTab_Label->Text = L"Week Day:";
			// 
			// apply_setAlarm_WakeUpTab_Button
			// 
			this->apply_setAlarm_WakeUpTab_Button->AutoSize = true;
			this->apply_setAlarm_WakeUpTab_Button->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
			this->apply_setAlarm_WakeUpTab_Button->Location = System::Drawing::Point(276, 250);
			this->apply_setAlarm_WakeUpTab_Button->Name = L"apply_setAlarm_WakeUpTab_Button";
			this->apply_setAlarm_WakeUpTab_Button->Size = System::Drawing::Size(80, 28);
			this->apply_setAlarm_WakeUpTab_Button->TabIndex = 21;
			this->apply_setAlarm_WakeUpTab_Button->Text = L"Apply";
			this->apply_setAlarm_WakeUpTab_Button->UseVisualStyleBackColor = true;
			this->apply_setAlarm_WakeUpTab_Button->Click += gcnew System::EventHandler(this, &GUI::apply_setAlarm_WakeUpTab_Button_Click);
			// 
			// day_setAlarm_WakeUpTab_Label
			// 
			this->day_setAlarm_WakeUpTab_Label->AutoSize = true;
			this->day_setAlarm_WakeUpTab_Label->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->day_setAlarm_WakeUpTab_Label->ForeColor = System::Drawing::SystemColors::Desktop;
			this->day_setAlarm_WakeUpTab_Label->Location = System::Drawing::Point(422, 191);
			this->day_setAlarm_WakeUpTab_Label->Name = L"day_setAlarm_WakeUpTab_Label";
			this->day_setAlarm_WakeUpTab_Label->Size = System::Drawing::Size(37, 17);
			this->day_setAlarm_WakeUpTab_Label->TabIndex = 20;
			this->day_setAlarm_WakeUpTab_Label->Text = L"Day:";
			// 
			// minute_setAlarm_WakeUpTab_Label
			// 
			this->minute_setAlarm_WakeUpTab_Label->AutoSize = true;
			this->minute_setAlarm_WakeUpTab_Label->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->minute_setAlarm_WakeUpTab_Label->ForeColor = System::Drawing::SystemColors::Desktop;
			this->minute_setAlarm_WakeUpTab_Label->Location = System::Drawing::Point(239, 191);
			this->minute_setAlarm_WakeUpTab_Label->Name = L"minute_setAlarm_WakeUpTab_Label";
			this->minute_setAlarm_WakeUpTab_Label->Size = System::Drawing::Size(54, 17);
			this->minute_setAlarm_WakeUpTab_Label->TabIndex = 19;
			this->minute_setAlarm_WakeUpTab_Label->Text = L"Minute:";
			// 
			// hour_setAlarm_WakeUpTab_Label
			// 
			this->hour_setAlarm_WakeUpTab_Label->AutoSize = true;
			this->hour_setAlarm_WakeUpTab_Label->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->hour_setAlarm_WakeUpTab_Label->ForeColor = System::Drawing::SystemColors::Desktop;
			this->hour_setAlarm_WakeUpTab_Label->Location = System::Drawing::Point(101, 192);
			this->hour_setAlarm_WakeUpTab_Label->Name = L"hour_setAlarm_WakeUpTab_Label";
			this->hour_setAlarm_WakeUpTab_Label->Size = System::Drawing::Size(43, 17);
			this->hour_setAlarm_WakeUpTab_Label->TabIndex = 18;
			this->hour_setAlarm_WakeUpTab_Label->Text = L"Hour:";
			// 
			// setBoot_WakeUpTab_TabPage
			// 
			this->setBoot_WakeUpTab_TabPage->BackColor = System::Drawing::Color::WhiteSmoke;
			this->setBoot_WakeUpTab_TabPage->Controls->Add(this->destination_setBoot_WakeUpTab_Label);
			this->setBoot_WakeUpTab_TabPage->Controls->Add(this->destination_setBoot_WakeUpTab_TextBox);
			this->setBoot_WakeUpTab_TabPage->Controls->Add(this->command_setBoot_WakeUpTab_TextBox);
			this->setBoot_WakeUpTab_TabPage->Controls->Add(this->command_setBoot_WakeUpTab_Label);
			this->setBoot_WakeUpTab_TabPage->Controls->Add(this->apply_setBoot_WakeUpTab_Button);
			this->setBoot_WakeUpTab_TabPage->Controls->Add(this->entryNumber_setBoot_WakeUpTab_Label);
			this->setBoot_WakeUpTab_TabPage->Controls->Add(this->setBoot_WakeUpTab_TextBox);
			this->setBoot_WakeUpTab_TabPage->Controls->Add(this->entryNumber_setBoot_WakeUpTab_ComboBox);
			this->setBoot_WakeUpTab_TabPage->Location = System::Drawing::Point(4, 22);
			this->setBoot_WakeUpTab_TabPage->Name = L"setBoot_WakeUpTab_TabPage";
			this->setBoot_WakeUpTab_TabPage->Padding = System::Windows::Forms::Padding(3);
			this->setBoot_WakeUpTab_TabPage->Size = System::Drawing::Size(632, 319);
			this->setBoot_WakeUpTab_TabPage->TabIndex = 8;
			this->setBoot_WakeUpTab_TabPage->Text = L"Set Boot";
			// 
			// destination_setBoot_WakeUpTab_Label
			// 
			this->destination_setBoot_WakeUpTab_Label->AutoSize = true;
			this->destination_setBoot_WakeUpTab_Label->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(177)));
			this->destination_setBoot_WakeUpTab_Label->Location = System::Drawing::Point(46, 160);
			this->destination_setBoot_WakeUpTab_Label->Name = L"destination_setBoot_WakeUpTab_Label";
			this->destination_setBoot_WakeUpTab_Label->Size = System::Drawing::Size(83, 17);
			this->destination_setBoot_WakeUpTab_Label->TabIndex = 31;
			this->destination_setBoot_WakeUpTab_Label->Text = L"Destination:";
			// 
			// destination_setBoot_WakeUpTab_TextBox
			// 
			this->destination_setBoot_WakeUpTab_TextBox->Location = System::Drawing::Point(151, 156);
			this->destination_setBoot_WakeUpTab_TextBox->Name = L"destination_setBoot_WakeUpTab_TextBox";
			this->destination_setBoot_WakeUpTab_TextBox->Size = System::Drawing::Size(62, 20);
			this->destination_setBoot_WakeUpTab_TextBox->TabIndex = 30;
			// 
			// command_setBoot_WakeUpTab_TextBox
			// 
			this->command_setBoot_WakeUpTab_TextBox->Location = System::Drawing::Point(151, 217);
			this->command_setBoot_WakeUpTab_TextBox->MaxLength = 60;
			this->command_setBoot_WakeUpTab_TextBox->Name = L"command_setBoot_WakeUpTab_TextBox";
			this->command_setBoot_WakeUpTab_TextBox->Size = System::Drawing::Size(322, 20);
			this->command_setBoot_WakeUpTab_TextBox->TabIndex = 29;
			// 
			// command_setBoot_WakeUpTab_Label
			// 
			this->command_setBoot_WakeUpTab_Label->AutoSize = true;
			this->command_setBoot_WakeUpTab_Label->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->command_setBoot_WakeUpTab_Label->Location = System::Drawing::Point(46, 217);
			this->command_setBoot_WakeUpTab_Label->Name = L"command_setBoot_WakeUpTab_Label";
			this->command_setBoot_WakeUpTab_Label->Size = System::Drawing::Size(75, 17);
			this->command_setBoot_WakeUpTab_Label->TabIndex = 28;
			this->command_setBoot_WakeUpTab_Label->Text = L"Command:";
			// 
			// apply_setBoot_WakeUpTab_Button
			// 
			this->apply_setBoot_WakeUpTab_Button->AutoSize = true;
			this->apply_setBoot_WakeUpTab_Button->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
			this->apply_setBoot_WakeUpTab_Button->Location = System::Drawing::Point(276, 250);
			this->apply_setBoot_WakeUpTab_Button->Name = L"apply_setBoot_WakeUpTab_Button";
			this->apply_setBoot_WakeUpTab_Button->Size = System::Drawing::Size(80, 28);
			this->apply_setBoot_WakeUpTab_Button->TabIndex = 27;
			this->apply_setBoot_WakeUpTab_Button->Text = L"Apply";
			this->apply_setBoot_WakeUpTab_Button->UseVisualStyleBackColor = true;
			this->apply_setBoot_WakeUpTab_Button->Click += gcnew System::EventHandler(this, &GUI::apply_setBoot_WakeUpTab_Button_Click);
			// 
			// entryNumber_setBoot_WakeUpTab_Label
			// 
			this->entryNumber_setBoot_WakeUpTab_Label->AutoSize = true;
			this->entryNumber_setBoot_WakeUpTab_Label->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->entryNumber_setBoot_WakeUpTab_Label->Location = System::Drawing::Point(46, 188);
			this->entryNumber_setBoot_WakeUpTab_Label->Name = L"entryNumber_setBoot_WakeUpTab_Label";
			this->entryNumber_setBoot_WakeUpTab_Label->Size = System::Drawing::Size(99, 17);
			this->entryNumber_setBoot_WakeUpTab_Label->TabIndex = 26;
			this->entryNumber_setBoot_WakeUpTab_Label->Text = L"Entry Number:";
			// 
			// setBoot_WakeUpTab_TextBox
			// 
			this->setBoot_WakeUpTab_TextBox->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->setBoot_WakeUpTab_TextBox->Location = System::Drawing::Point(6, 4);
			this->setBoot_WakeUpTab_TextBox->Multiline = true;
			this->setBoot_WakeUpTab_TextBox->Name = L"setBoot_WakeUpTab_TextBox";
			this->setBoot_WakeUpTab_TextBox->ReadOnly = true;
			this->setBoot_WakeUpTab_TextBox->Size = System::Drawing::Size(616, 129);
			this->setBoot_WakeUpTab_TextBox->TabIndex = 25;
			this->setBoot_WakeUpTab_TextBox->Text = resources->GetString(L"setBoot_WakeUpTab_TextBox.Text");
			// 
			// entryNumber_setBoot_WakeUpTab_ComboBox
			// 
			this->entryNumber_setBoot_WakeUpTab_ComboBox->FormattingEnabled = true;
			this->entryNumber_setBoot_WakeUpTab_ComboBox->Items->AddRange(gcnew cli::array< System::Object^  >(10) {L"0", L"1", L"2", 
				L"3", L"4", L"5", L"6", L"7", L"8", L"9"});
			this->entryNumber_setBoot_WakeUpTab_ComboBox->Location = System::Drawing::Point(151, 186);
			this->entryNumber_setBoot_WakeUpTab_ComboBox->Name = L"entryNumber_setBoot_WakeUpTab_ComboBox";
			this->entryNumber_setBoot_WakeUpTab_ComboBox->Size = System::Drawing::Size(62, 21);
			this->entryNumber_setBoot_WakeUpTab_ComboBox->TabIndex = 24;
			// 
			// readLL_WakeUpTab_TabPage
			// 
			this->readLL_WakeUpTab_TabPage->BackColor = System::Drawing::Color::WhiteSmoke;
			this->readLL_WakeUpTab_TabPage->Controls->Add(this->destination_readLL_WakeUpTab_Label);
			this->readLL_WakeUpTab_TabPage->Controls->Add(this->destination_readLL_WakeUpTab_TextBox);
			this->readLL_WakeUpTab_TabPage->Controls->Add(this->read_readLL_WakeUpTab_Button);
			this->readLL_WakeUpTab_TabPage->Controls->Add(this->address_readLL_WakeUpTab_Label);
			this->readLL_WakeUpTab_TabPage->Controls->Add(this->address_readLL_WakeUpTab_TextBox);
			this->readLL_WakeUpTab_TabPage->Controls->Add(this->readLL_WakeUpTab_TextBox);
			this->readLL_WakeUpTab_TabPage->Location = System::Drawing::Point(4, 22);
			this->readLL_WakeUpTab_TabPage->Name = L"readLL_WakeUpTab_TabPage";
			this->readLL_WakeUpTab_TabPage->Padding = System::Windows::Forms::Padding(3);
			this->readLL_WakeUpTab_TabPage->Size = System::Drawing::Size(632, 319);
			this->readLL_WakeUpTab_TabPage->TabIndex = 0;
			this->readLL_WakeUpTab_TabPage->Text = L"Read (Low Level)";
			// 
			// destination_readLL_WakeUpTab_Label
			// 
			this->destination_readLL_WakeUpTab_Label->AutoSize = true;
			this->destination_readLL_WakeUpTab_Label->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(177)));
			this->destination_readLL_WakeUpTab_Label->Location = System::Drawing::Point(66, 158);
			this->destination_readLL_WakeUpTab_Label->Name = L"destination_readLL_WakeUpTab_Label";
			this->destination_readLL_WakeUpTab_Label->Size = System::Drawing::Size(83, 17);
			this->destination_readLL_WakeUpTab_Label->TabIndex = 14;
			this->destination_readLL_WakeUpTab_Label->Text = L"Destination:";
			// 
			// destination_readLL_WakeUpTab_TextBox
			// 
			this->destination_readLL_WakeUpTab_TextBox->Location = System::Drawing::Point(150, 154);
			this->destination_readLL_WakeUpTab_TextBox->Name = L"destination_readLL_WakeUpTab_TextBox";
			this->destination_readLL_WakeUpTab_TextBox->Size = System::Drawing::Size(57, 20);
			this->destination_readLL_WakeUpTab_TextBox->TabIndex = 13;
			// 
			// read_readLL_WakeUpTab_Button
			// 
			this->read_readLL_WakeUpTab_Button->AutoSize = true;
			this->read_readLL_WakeUpTab_Button->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
			this->read_readLL_WakeUpTab_Button->Location = System::Drawing::Point(276, 250);
			this->read_readLL_WakeUpTab_Button->Name = L"read_readLL_WakeUpTab_Button";
			this->read_readLL_WakeUpTab_Button->Size = System::Drawing::Size(80, 28);
			this->read_readLL_WakeUpTab_Button->TabIndex = 12;
			this->read_readLL_WakeUpTab_Button->Text = L"Read";
			this->read_readLL_WakeUpTab_Button->UseVisualStyleBackColor = true;
			this->read_readLL_WakeUpTab_Button->Click += gcnew System::EventHandler(this, &GUI::read_readLL_WakeUpTab_Button_Click);
			// 
			// address_readLL_WakeUpTab_Label
			// 
			this->address_readLL_WakeUpTab_Label->AutoSize = true;
			this->address_readLL_WakeUpTab_Label->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->address_readLL_WakeUpTab_Label->Location = System::Drawing::Point(66, 194);
			this->address_readLL_WakeUpTab_Label->Name = L"address_readLL_WakeUpTab_Label";
			this->address_readLL_WakeUpTab_Label->Size = System::Drawing::Size(64, 17);
			this->address_readLL_WakeUpTab_Label->TabIndex = 10;
			this->address_readLL_WakeUpTab_Label->Text = L"Address:";
			// 
			// address_readLL_WakeUpTab_TextBox
			// 
			this->address_readLL_WakeUpTab_TextBox->Location = System::Drawing::Point(150, 194);
			this->address_readLL_WakeUpTab_TextBox->Name = L"address_readLL_WakeUpTab_TextBox";
			this->address_readLL_WakeUpTab_TextBox->Size = System::Drawing::Size(57, 20);
			this->address_readLL_WakeUpTab_TextBox->TabIndex = 9;
			this->address_readLL_WakeUpTab_TextBox->Text = L"0";
			// 
			// readLL_WakeUpTab_TextBox
			// 
			this->readLL_WakeUpTab_TextBox->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->readLL_WakeUpTab_TextBox->Location = System::Drawing::Point(6, 4);
			this->readLL_WakeUpTab_TextBox->Multiline = true;
			this->readLL_WakeUpTab_TextBox->Name = L"readLL_WakeUpTab_TextBox";
			this->readLL_WakeUpTab_TextBox->ReadOnly = true;
			this->readLL_WakeUpTab_TextBox->Size = System::Drawing::Size(616, 105);
			this->readLL_WakeUpTab_TextBox->TabIndex = 8;
			this->readLL_WakeUpTab_TextBox->Text = L"Read the registers of Wistone\'s Real Time Clock:\r\n\r\n^ Choose \"Destination\" ( Netw" 
				L"ork ID of Wistone or 0 for Broadcast )\r\n^ Enter: \"Address\" ( of the register )\r\n" 
				L"^ Press \"Read\"";
			// 
			// writeLL_WakeUpTab_TabPage
			// 
			this->writeLL_WakeUpTab_TabPage->BackColor = System::Drawing::Color::WhiteSmoke;
			this->writeLL_WakeUpTab_TabPage->Controls->Add(this->destination_writeLL_WakeUpTab_Label);
			this->writeLL_WakeUpTab_TabPage->Controls->Add(this->destination_writeLL_WakeUpTab_TextBox);
			this->writeLL_WakeUpTab_TabPage->Controls->Add(this->write_writeLL_WakeUpTab_Button);
			this->writeLL_WakeUpTab_TabPage->Controls->Add(this->data_writeLL_WakeUpTab_Label);
			this->writeLL_WakeUpTab_TabPage->Controls->Add(this->data_writeLL_WakeUpTab_TextBox);
			this->writeLL_WakeUpTab_TabPage->Controls->Add(this->address_writeLL_WakeUpTab_TextBox);
			this->writeLL_WakeUpTab_TabPage->Controls->Add(this->writeLL_WakeUpTab_TextBox);
			this->writeLL_WakeUpTab_TabPage->Controls->Add(this->address_writeLL_WakeUpTab_Label);
			this->writeLL_WakeUpTab_TabPage->Location = System::Drawing::Point(4, 22);
			this->writeLL_WakeUpTab_TabPage->Name = L"writeLL_WakeUpTab_TabPage";
			this->writeLL_WakeUpTab_TabPage->Padding = System::Windows::Forms::Padding(3);
			this->writeLL_WakeUpTab_TabPage->Size = System::Drawing::Size(632, 319);
			this->writeLL_WakeUpTab_TabPage->TabIndex = 1;
			this->writeLL_WakeUpTab_TabPage->Text = L"Write (Low Level)";
			// 
			// destination_writeLL_WakeUpTab_Label
			// 
			this->destination_writeLL_WakeUpTab_Label->AutoSize = true;
			this->destination_writeLL_WakeUpTab_Label->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(177)));
			this->destination_writeLL_WakeUpTab_Label->Location = System::Drawing::Point(66, 158);
			this->destination_writeLL_WakeUpTab_Label->Name = L"destination_writeLL_WakeUpTab_Label";
			this->destination_writeLL_WakeUpTab_Label->Size = System::Drawing::Size(83, 17);
			this->destination_writeLL_WakeUpTab_Label->TabIndex = 14;
			this->destination_writeLL_WakeUpTab_Label->Text = L"Destination:";
			// 
			// destination_writeLL_WakeUpTab_TextBox
			// 
			this->destination_writeLL_WakeUpTab_TextBox->Location = System::Drawing::Point(150, 154);
			this->destination_writeLL_WakeUpTab_TextBox->Name = L"destination_writeLL_WakeUpTab_TextBox";
			this->destination_writeLL_WakeUpTab_TextBox->Size = System::Drawing::Size(57, 20);
			this->destination_writeLL_WakeUpTab_TextBox->TabIndex = 13;
			// 
			// write_writeLL_WakeUpTab_Button
			// 
			this->write_writeLL_WakeUpTab_Button->AutoSize = true;
			this->write_writeLL_WakeUpTab_Button->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
			this->write_writeLL_WakeUpTab_Button->Location = System::Drawing::Point(276, 250);
			this->write_writeLL_WakeUpTab_Button->Name = L"write_writeLL_WakeUpTab_Button";
			this->write_writeLL_WakeUpTab_Button->Size = System::Drawing::Size(80, 28);
			this->write_writeLL_WakeUpTab_Button->TabIndex = 12;
			this->write_writeLL_WakeUpTab_Button->Text = L"Write";
			this->write_writeLL_WakeUpTab_Button->UseVisualStyleBackColor = true;
			this->write_writeLL_WakeUpTab_Button->Click += gcnew System::EventHandler(this, &GUI::write_writeLL_WakeUpTab_Button_Click);
			// 
			// data_writeLL_WakeUpTab_Label
			// 
			this->data_writeLL_WakeUpTab_Label->AutoSize = true;
			this->data_writeLL_WakeUpTab_Label->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->data_writeLL_WakeUpTab_Label->Location = System::Drawing::Point(66, 228);
			this->data_writeLL_WakeUpTab_Label->Name = L"data_writeLL_WakeUpTab_Label";
			this->data_writeLL_WakeUpTab_Label->Size = System::Drawing::Size(42, 17);
			this->data_writeLL_WakeUpTab_Label->TabIndex = 11;
			this->data_writeLL_WakeUpTab_Label->Text = L"Data:";
			// 
			// data_writeLL_WakeUpTab_TextBox
			// 
			this->data_writeLL_WakeUpTab_TextBox->Location = System::Drawing::Point(150, 228);
			this->data_writeLL_WakeUpTab_TextBox->Multiline = true;
			this->data_writeLL_WakeUpTab_TextBox->Name = L"data_writeLL_WakeUpTab_TextBox";
			this->data_writeLL_WakeUpTab_TextBox->Size = System::Drawing::Size(57, 20);
			this->data_writeLL_WakeUpTab_TextBox->TabIndex = 10;
			// 
			// address_writeLL_WakeUpTab_TextBox
			// 
			this->address_writeLL_WakeUpTab_TextBox->Location = System::Drawing::Point(150, 193);
			this->address_writeLL_WakeUpTab_TextBox->Name = L"address_writeLL_WakeUpTab_TextBox";
			this->address_writeLL_WakeUpTab_TextBox->Size = System::Drawing::Size(57, 20);
			this->address_writeLL_WakeUpTab_TextBox->TabIndex = 8;
			this->address_writeLL_WakeUpTab_TextBox->Text = L"0";
			// 
			// writeLL_WakeUpTab_TextBox
			// 
			this->writeLL_WakeUpTab_TextBox->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->writeLL_WakeUpTab_TextBox->Location = System::Drawing::Point(6, 4);
			this->writeLL_WakeUpTab_TextBox->Multiline = true;
			this->writeLL_WakeUpTab_TextBox->Name = L"writeLL_WakeUpTab_TextBox";
			this->writeLL_WakeUpTab_TextBox->ReadOnly = true;
			this->writeLL_WakeUpTab_TextBox->Size = System::Drawing::Size(616, 101);
			this->writeLL_WakeUpTab_TextBox->TabIndex = 7;
			this->writeLL_WakeUpTab_TextBox->Text = L"Write the registers of Wistone\'s Real Time Clock:\r\n\r\n^ Choose \"Destination\" ( Net" 
				L"work ID of Wistone or 0 for Broadcast )\r\n^ Enter: \"Address\" ( of the register ) " 
				L"and \"Data\"\r\n^ Press \"Write\"";
			// 
			// address_writeLL_WakeUpTab_Label
			// 
			this->address_writeLL_WakeUpTab_Label->AutoSize = true;
			this->address_writeLL_WakeUpTab_Label->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->address_writeLL_WakeUpTab_Label->Location = System::Drawing::Point(66, 194);
			this->address_writeLL_WakeUpTab_Label->Name = L"address_writeLL_WakeUpTab_Label";
			this->address_writeLL_WakeUpTab_Label->Size = System::Drawing::Size(64, 17);
			this->address_writeLL_WakeUpTab_Label->TabIndex = 9;
			this->address_writeLL_WakeUpTab_Label->Text = L"Address:";
			// 
			// tabLCD
			// 
			this->tabLCD->BackColor = System::Drawing::Color::WhiteSmoke;
			this->tabLCD->Controls->Add(this->lcdTab);
			this->tabLCD->Location = System::Drawing::Point(4, 22);
			this->tabLCD->Name = L"tabLCD";
			this->tabLCD->Padding = System::Windows::Forms::Padding(3);
			this->tabLCD->Size = System::Drawing::Size(633, 339);
			this->tabLCD->TabIndex = 4;
			this->tabLCD->Text = L"LCD";
			// 
			// lcdTab
			// 
			this->lcdTab->Controls->Add(this->clearScreen_LcdTab_TabPage);
			this->lcdTab->Controls->Add(this->writeLL_LcdTab_TabPage);
			this->lcdTab->Location = System::Drawing::Point(0, 0);
			this->lcdTab->Name = L"lcdTab";
			this->lcdTab->SelectedIndex = 0;
			this->lcdTab->Size = System::Drawing::Size(640, 345);
			this->lcdTab->TabIndex = 1;
			// 
			// clearScreen_LcdTab_TabPage
			// 
			this->clearScreen_LcdTab_TabPage->BackColor = System::Drawing::Color::WhiteSmoke;
			this->clearScreen_LcdTab_TabPage->Controls->Add(this->destination_clearScreen_LcdTab_Label);
			this->clearScreen_LcdTab_TabPage->Controls->Add(this->destination_clearScreen_LcdTab_TextBox);
			this->clearScreen_LcdTab_TabPage->Controls->Add(this->lcd_clearScreen_LcdTab_TextBox);
			this->clearScreen_LcdTab_TabPage->Controls->Add(this->lcd_clearScreen_LcdTab_Label);
			this->clearScreen_LcdTab_TabPage->Controls->Add(this->clear_clearScreen_LcdTab_Button);
			this->clearScreen_LcdTab_TabPage->Controls->Add(this->clearScreen_LcdTab_TextBox);
			this->clearScreen_LcdTab_TabPage->Location = System::Drawing::Point(4, 22);
			this->clearScreen_LcdTab_TabPage->Name = L"clearScreen_LcdTab_TabPage";
			this->clearScreen_LcdTab_TabPage->Padding = System::Windows::Forms::Padding(3);
			this->clearScreen_LcdTab_TabPage->Size = System::Drawing::Size(632, 319);
			this->clearScreen_LcdTab_TabPage->TabIndex = 2;
			this->clearScreen_LcdTab_TabPage->Text = L"Clear Screen";
			// 
			// destination_clearScreen_LcdTab_Label
			// 
			this->destination_clearScreen_LcdTab_Label->AutoSize = true;
			this->destination_clearScreen_LcdTab_Label->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(177)));
			this->destination_clearScreen_LcdTab_Label->Location = System::Drawing::Point(54, 126);
			this->destination_clearScreen_LcdTab_Label->Name = L"destination_clearScreen_LcdTab_Label";
			this->destination_clearScreen_LcdTab_Label->Size = System::Drawing::Size(83, 17);
			this->destination_clearScreen_LcdTab_Label->TabIndex = 31;
			this->destination_clearScreen_LcdTab_Label->Text = L"Destination:";
			// 
			// destination_clearScreen_LcdTab_TextBox
			// 
			this->destination_clearScreen_LcdTab_TextBox->Location = System::Drawing::Point(145, 122);
			this->destination_clearScreen_LcdTab_TextBox->Name = L"destination_clearScreen_LcdTab_TextBox";
			this->destination_clearScreen_LcdTab_TextBox->Size = System::Drawing::Size(71, 20);
			this->destination_clearScreen_LcdTab_TextBox->TabIndex = 30;
			// 
			// lcd_clearScreen_LcdTab_TextBox
			// 
			this->lcd_clearScreen_LcdTab_TextBox->Location = System::Drawing::Point(145, 162);
			this->lcd_clearScreen_LcdTab_TextBox->Multiline = true;
			this->lcd_clearScreen_LcdTab_TextBox->Name = L"lcd_clearScreen_LcdTab_TextBox";
			this->lcd_clearScreen_LcdTab_TextBox->Size = System::Drawing::Size(415, 36);
			this->lcd_clearScreen_LcdTab_TextBox->TabIndex = 29;
			// 
			// lcd_clearScreen_LcdTab_Label
			// 
			this->lcd_clearScreen_LcdTab_Label->AutoSize = true;
			this->lcd_clearScreen_LcdTab_Label->BackColor = System::Drawing::SystemColors::ButtonFace;
			this->lcd_clearScreen_LcdTab_Label->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 11, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->lcd_clearScreen_LcdTab_Label->Location = System::Drawing::Point(54, 162);
			this->lcd_clearScreen_LcdTab_Label->Name = L"lcd_clearScreen_LcdTab_Label";
			this->lcd_clearScreen_LcdTab_Label->Size = System::Drawing::Size(42, 18);
			this->lcd_clearScreen_LcdTab_Label->TabIndex = 28;
			this->lcd_clearScreen_LcdTab_Label->Text = L"LCD:";
			// 
			// clear_clearScreen_LcdTab_Button
			// 
			this->clear_clearScreen_LcdTab_Button->AutoSize = true;
			this->clear_clearScreen_LcdTab_Button->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
			this->clear_clearScreen_LcdTab_Button->Location = System::Drawing::Point(276, 250);
			this->clear_clearScreen_LcdTab_Button->Name = L"clear_clearScreen_LcdTab_Button";
			this->clear_clearScreen_LcdTab_Button->Size = System::Drawing::Size(80, 28);
			this->clear_clearScreen_LcdTab_Button->TabIndex = 11;
			this->clear_clearScreen_LcdTab_Button->Text = L"Clear";
			this->clear_clearScreen_LcdTab_Button->UseVisualStyleBackColor = true;
			this->clear_clearScreen_LcdTab_Button->Click += gcnew System::EventHandler(this, &GUI::clear_clearScreen_LcdTab_Button_Click);
			// 
			// clearScreen_LcdTab_TextBox
			// 
			this->clearScreen_LcdTab_TextBox->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->clearScreen_LcdTab_TextBox->Location = System::Drawing::Point(6, 6);
			this->clearScreen_LcdTab_TextBox->Multiline = true;
			this->clearScreen_LcdTab_TextBox->Name = L"clearScreen_LcdTab_TextBox";
			this->clearScreen_LcdTab_TextBox->ReadOnly = true;
			this->clearScreen_LcdTab_TextBox->Size = System::Drawing::Size(620, 89);
			this->clearScreen_LcdTab_TextBox->TabIndex = 0;
			this->clearScreen_LcdTab_TextBox->Text = L"Clear LCD Screen:\r\n\r\n^ Choose \"Destination\" ( Network ID of Wistone or 0 for Broa" 
				L"dcast )\r\n^ Press \"Clear\"";
			// 
			// writeLL_LcdTab_TabPage
			// 
			this->writeLL_LcdTab_TabPage->BackColor = System::Drawing::Color::WhiteSmoke;
			this->writeLL_LcdTab_TabPage->Controls->Add(this->destination_writeLL_LcdTab_Label);
			this->writeLL_LcdTab_TabPage->Controls->Add(this->destination_writeLL_LcdTab_TextBox);
			this->writeLL_LcdTab_TabPage->Controls->Add(this->write_writeLL_LcdTab_Button);
			this->writeLL_LcdTab_TabPage->Controls->Add(this->data_writeLL_LcdTab_Label);
			this->writeLL_LcdTab_TabPage->Controls->Add(this->data_writeLL_LcdTab_TextBox);
			this->writeLL_LcdTab_TabPage->Controls->Add(this->address_writeLL_LcdTab_TextBox);
			this->writeLL_LcdTab_TabPage->Controls->Add(this->writeLL_LcdTab_TextBox);
			this->writeLL_LcdTab_TabPage->Controls->Add(this->address_writeLL_LcdTab_Label);
			this->writeLL_LcdTab_TabPage->Location = System::Drawing::Point(4, 22);
			this->writeLL_LcdTab_TabPage->Name = L"writeLL_LcdTab_TabPage";
			this->writeLL_LcdTab_TabPage->Padding = System::Windows::Forms::Padding(3);
			this->writeLL_LcdTab_TabPage->Size = System::Drawing::Size(632, 319);
			this->writeLL_LcdTab_TabPage->TabIndex = 1;
			this->writeLL_LcdTab_TabPage->Text = L"Write (Low Level)";
			// 
			// destination_writeLL_LcdTab_Label
			// 
			this->destination_writeLL_LcdTab_Label->AutoSize = true;
			this->destination_writeLL_LcdTab_Label->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(177)));
			this->destination_writeLL_LcdTab_Label->Location = System::Drawing::Point(54, 127);
			this->destination_writeLL_LcdTab_Label->Name = L"destination_writeLL_LcdTab_Label";
			this->destination_writeLL_LcdTab_Label->Size = System::Drawing::Size(83, 17);
			this->destination_writeLL_LcdTab_Label->TabIndex = 14;
			this->destination_writeLL_LcdTab_Label->Text = L"Destination:";
			// 
			// destination_writeLL_LcdTab_TextBox
			// 
			this->destination_writeLL_LcdTab_TextBox->Location = System::Drawing::Point(143, 124);
			this->destination_writeLL_LcdTab_TextBox->Name = L"destination_writeLL_LcdTab_TextBox";
			this->destination_writeLL_LcdTab_TextBox->Size = System::Drawing::Size(71, 20);
			this->destination_writeLL_LcdTab_TextBox->TabIndex = 13;
			// 
			// write_writeLL_LcdTab_Button
			// 
			this->write_writeLL_LcdTab_Button->AutoSize = true;
			this->write_writeLL_LcdTab_Button->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
			this->write_writeLL_LcdTab_Button->Location = System::Drawing::Point(276, 250);
			this->write_writeLL_LcdTab_Button->Name = L"write_writeLL_LcdTab_Button";
			this->write_writeLL_LcdTab_Button->Size = System::Drawing::Size(80, 28);
			this->write_writeLL_LcdTab_Button->TabIndex = 12;
			this->write_writeLL_LcdTab_Button->Text = L"Write";
			this->write_writeLL_LcdTab_Button->UseVisualStyleBackColor = true;
			this->write_writeLL_LcdTab_Button->Click += gcnew System::EventHandler(this, &GUI::write_writeLL_LcdTab_Button_Click);
			// 
			// data_writeLL_LcdTab_Label
			// 
			this->data_writeLL_LcdTab_Label->AutoSize = true;
			this->data_writeLL_LcdTab_Label->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->data_writeLL_LcdTab_Label->Location = System::Drawing::Point(54, 199);
			this->data_writeLL_LcdTab_Label->Name = L"data_writeLL_LcdTab_Label";
			this->data_writeLL_LcdTab_Label->Size = System::Drawing::Size(42, 17);
			this->data_writeLL_LcdTab_Label->TabIndex = 11;
			this->data_writeLL_LcdTab_Label->Text = L"Data:";
			// 
			// data_writeLL_LcdTab_TextBox
			// 
			this->data_writeLL_LcdTab_TextBox->Location = System::Drawing::Point(143, 199);
			this->data_writeLL_LcdTab_TextBox->Multiline = true;
			this->data_writeLL_LcdTab_TextBox->Name = L"data_writeLL_LcdTab_TextBox";
			this->data_writeLL_LcdTab_TextBox->Size = System::Drawing::Size(350, 21);
			this->data_writeLL_LcdTab_TextBox->TabIndex = 10;
			// 
			// address_writeLL_LcdTab_TextBox
			// 
			this->address_writeLL_LcdTab_TextBox->Location = System::Drawing::Point(143, 162);
			this->address_writeLL_LcdTab_TextBox->Name = L"address_writeLL_LcdTab_TextBox";
			this->address_writeLL_LcdTab_TextBox->Size = System::Drawing::Size(70, 20);
			this->address_writeLL_LcdTab_TextBox->TabIndex = 8;
			this->address_writeLL_LcdTab_TextBox->Text = L"0";
			// 
			// writeLL_LcdTab_TextBox
			// 
			this->writeLL_LcdTab_TextBox->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->writeLL_LcdTab_TextBox->Location = System::Drawing::Point(6, 6);
			this->writeLL_LcdTab_TextBox->Multiline = true;
			this->writeLL_LcdTab_TextBox->Name = L"writeLL_LcdTab_TextBox";
			this->writeLL_LcdTab_TextBox->ReadOnly = true;
			this->writeLL_LcdTab_TextBox->Size = System::Drawing::Size(620, 100);
			this->writeLL_LcdTab_TextBox->TabIndex = 7;
			this->writeLL_LcdTab_TextBox->Text = L"Write LCD Screen:\r\n\r\n^ Choose \"Destination\" ( Network ID of Wistone or 0 for Broa" 
				L"dcast )\r\n^ Enter: \"Address\" ( on LCD screen ) and \"Data\"\r\n^ Press \"Write\"";
			// 
			// address_writeLL_LcdTab_Label
			// 
			this->address_writeLL_LcdTab_Label->AutoSize = true;
			this->address_writeLL_LcdTab_Label->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->address_writeLL_LcdTab_Label->Location = System::Drawing::Point(54, 162);
			this->address_writeLL_LcdTab_Label->Name = L"address_writeLL_LcdTab_Label";
			this->address_writeLL_LcdTab_Label->Size = System::Drawing::Size(64, 17);
			this->address_writeLL_LcdTab_Label->TabIndex = 9;
			this->address_writeLL_LcdTab_Label->Text = L"Address:";
			// 
			// tabFlash
			// 
			this->tabFlash->BackColor = System::Drawing::Color::WhiteSmoke;
			this->tabFlash->Controls->Add(this->flashTab);
			this->tabFlash->Location = System::Drawing::Point(4, 22);
			this->tabFlash->Name = L"tabFlash";
			this->tabFlash->Padding = System::Windows::Forms::Padding(3);
			this->tabFlash->Size = System::Drawing::Size(633, 339);
			this->tabFlash->TabIndex = 2;
			this->tabFlash->Text = L"Flash";
			// 
			// flashTab
			// 
			this->flashTab->Controls->Add(this->format_FlashTab_TabPage);
			this->flashTab->Controls->Add(this->initialize_FlashTab_TabPage);
			this->flashTab->Controls->Add(this->cardDetect_FlashTab_TabPage);
			this->flashTab->Controls->Add(this->writeSector_FlashTab_TabPage);
			this->flashTab->Controls->Add(this->readSector_FlashTab_TabPage);
			this->flashTab->Controls->Add(this->getCapacity_FlashTab_TabPage);
			this->flashTab->Controls->Add(this->readLL_FlashTab_TabPage);
			this->flashTab->Controls->Add(this->writeLL_FlashTab_TabPage);
			this->flashTab->Location = System::Drawing::Point(0, 0);
			this->flashTab->Name = L"flashTab";
			this->flashTab->SelectedIndex = 0;
			this->flashTab->Size = System::Drawing::Size(640, 345);
			this->flashTab->TabIndex = 0;
			// 
			// format_FlashTab_TabPage
			// 
			this->format_FlashTab_TabPage->BackColor = System::Drawing::Color::WhiteSmoke;
			this->format_FlashTab_TabPage->Controls->Add(this->destination_format_FlashTab_Label);
			this->format_FlashTab_TabPage->Controls->Add(this->destination_format_FlashTab_TextBox);
			this->format_FlashTab_TabPage->Controls->Add(this->format_format_FlashTab_Button);
			this->format_FlashTab_TabPage->Controls->Add(this->format_FlashTab_TextBox);
			this->format_FlashTab_TabPage->Location = System::Drawing::Point(4, 22);
			this->format_FlashTab_TabPage->Name = L"format_FlashTab_TabPage";
			this->format_FlashTab_TabPage->Padding = System::Windows::Forms::Padding(3);
			this->format_FlashTab_TabPage->Size = System::Drawing::Size(632, 319);
			this->format_FlashTab_TabPage->TabIndex = 2;
			this->format_FlashTab_TabPage->Text = L"Format";
			// 
			// destination_format_FlashTab_Label
			// 
			this->destination_format_FlashTab_Label->AutoSize = true;
			this->destination_format_FlashTab_Label->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(177)));
			this->destination_format_FlashTab_Label->Location = System::Drawing::Point(64, 192);
			this->destination_format_FlashTab_Label->Name = L"destination_format_FlashTab_Label";
			this->destination_format_FlashTab_Label->Size = System::Drawing::Size(83, 17);
			this->destination_format_FlashTab_Label->TabIndex = 12;
			this->destination_format_FlashTab_Label->Text = L"Destination:";
			// 
			// destination_format_FlashTab_TextBox
			// 
			this->destination_format_FlashTab_TextBox->Location = System::Drawing::Point(150, 192);
			this->destination_format_FlashTab_TextBox->Name = L"destination_format_FlashTab_TextBox";
			this->destination_format_FlashTab_TextBox->Size = System::Drawing::Size(57, 20);
			this->destination_format_FlashTab_TextBox->TabIndex = 11;
			// 
			// format_format_FlashTab_Button
			// 
			this->format_format_FlashTab_Button->AutoSize = true;
			this->format_format_FlashTab_Button->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
			this->format_format_FlashTab_Button->Location = System::Drawing::Point(276, 250);
			this->format_format_FlashTab_Button->Name = L"format_format_FlashTab_Button";
			this->format_format_FlashTab_Button->Size = System::Drawing::Size(80, 28);
			this->format_format_FlashTab_Button->TabIndex = 10;
			this->format_format_FlashTab_Button->Text = L"Format";
			this->format_format_FlashTab_Button->UseVisualStyleBackColor = true;
			this->format_format_FlashTab_Button->Click += gcnew System::EventHandler(this, &GUI::format_FlashTab_Button_Click);
			// 
			// format_FlashTab_TextBox
			// 
			this->format_FlashTab_TextBox->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->format_FlashTab_TextBox->Location = System::Drawing::Point(6, 4);
			this->format_FlashTab_TextBox->Multiline = true;
			this->format_FlashTab_TextBox->Name = L"format_FlashTab_TextBox";
			this->format_FlashTab_TextBox->ReadOnly = true;
			this->format_FlashTab_TextBox->Size = System::Drawing::Size(620, 94);
			this->format_FlashTab_TextBox->TabIndex = 0;
			this->format_FlashTab_TextBox->Text = L"Format Wistone\'s Flash:\r\n\r\n^ Choose \"Destination\" ( Network ID of Wistone or 0 fo" 
				L"r Broadcast )\r\n^ Press \"Format\" to entirely clear Flash memory";
			// 
			// initialize_FlashTab_TabPage
			// 
			this->initialize_FlashTab_TabPage->BackColor = System::Drawing::Color::WhiteSmoke;
			this->initialize_FlashTab_TabPage->Controls->Add(this->destination_initialize_FlashTab_Label);
			this->initialize_FlashTab_TabPage->Controls->Add(this->destination_initialize_FlashTab_TextBox);
			this->initialize_FlashTab_TabPage->Controls->Add(this->initialize_initialize_FlashTab_Button);
			this->initialize_FlashTab_TabPage->Controls->Add(this->initialize_FlashTab_TextBox);
			this->initialize_FlashTab_TabPage->Location = System::Drawing::Point(4, 22);
			this->initialize_FlashTab_TabPage->Name = L"initialize_FlashTab_TabPage";
			this->initialize_FlashTab_TabPage->Padding = System::Windows::Forms::Padding(3);
			this->initialize_FlashTab_TabPage->Size = System::Drawing::Size(632, 319);
			this->initialize_FlashTab_TabPage->TabIndex = 4;
			this->initialize_FlashTab_TabPage->Text = L"Initialize";
			// 
			// destination_initialize_FlashTab_Label
			// 
			this->destination_initialize_FlashTab_Label->AutoSize = true;
			this->destination_initialize_FlashTab_Label->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(177)));
			this->destination_initialize_FlashTab_Label->Location = System::Drawing::Point(64, 192);
			this->destination_initialize_FlashTab_Label->Name = L"destination_initialize_FlashTab_Label";
			this->destination_initialize_FlashTab_Label->Size = System::Drawing::Size(83, 17);
			this->destination_initialize_FlashTab_Label->TabIndex = 13;
			this->destination_initialize_FlashTab_Label->Text = L"Destination:";
			// 
			// destination_initialize_FlashTab_TextBox
			// 
			this->destination_initialize_FlashTab_TextBox->Location = System::Drawing::Point(150, 192);
			this->destination_initialize_FlashTab_TextBox->Name = L"destination_initialize_FlashTab_TextBox";
			this->destination_initialize_FlashTab_TextBox->Size = System::Drawing::Size(57, 20);
			this->destination_initialize_FlashTab_TextBox->TabIndex = 12;
			// 
			// initialize_initialize_FlashTab_Button
			// 
			this->initialize_initialize_FlashTab_Button->AutoSize = true;
			this->initialize_initialize_FlashTab_Button->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
			this->initialize_initialize_FlashTab_Button->Location = System::Drawing::Point(276, 250);
			this->initialize_initialize_FlashTab_Button->Name = L"initialize_initialize_FlashTab_Button";
			this->initialize_initialize_FlashTab_Button->Size = System::Drawing::Size(80, 28);
			this->initialize_initialize_FlashTab_Button->TabIndex = 11;
			this->initialize_initialize_FlashTab_Button->Text = L"Initialize";
			this->initialize_initialize_FlashTab_Button->UseVisualStyleBackColor = true;
			this->initialize_initialize_FlashTab_Button->Click += gcnew System::EventHandler(this, &GUI::initialize_FlashTab_Button_Click);
			// 
			// initialize_FlashTab_TextBox
			// 
			this->initialize_FlashTab_TextBox->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->initialize_FlashTab_TextBox->Location = System::Drawing::Point(6, 4);
			this->initialize_FlashTab_TextBox->Multiline = true;
			this->initialize_FlashTab_TextBox->Name = L"initialize_FlashTab_TextBox";
			this->initialize_FlashTab_TextBox->ReadOnly = true;
			this->initialize_FlashTab_TextBox->Size = System::Drawing::Size(620, 94);
			this->initialize_FlashTab_TextBox->TabIndex = 1;
			this->initialize_FlashTab_TextBox->Text = L"Initialize Wistone\'s Flash:\r\n\r\n^ Choose \"Destination\" ( Network ID of Wistone or " 
				L"0 for Broadcast )\r\n^ Press \"Initialize\" to initialize Flash media after every Fl" 
				L"ash removal\r\n\r\n";
			// 
			// cardDetect_FlashTab_TabPage
			// 
			this->cardDetect_FlashTab_TabPage->BackColor = System::Drawing::Color::WhiteSmoke;
			this->cardDetect_FlashTab_TabPage->Controls->Add(this->destination_cardDetect_FlashTab_Label);
			this->cardDetect_FlashTab_TabPage->Controls->Add(this->destination_cardDetect_FlashTab_TextBox);
			this->cardDetect_FlashTab_TabPage->Controls->Add(this->cardDetect_cardDetect_FlashTab_Button);
			this->cardDetect_FlashTab_TabPage->Controls->Add(this->cardDetect_FlashTab_TextBox);
			this->cardDetect_FlashTab_TabPage->Location = System::Drawing::Point(4, 22);
			this->cardDetect_FlashTab_TabPage->Name = L"cardDetect_FlashTab_TabPage";
			this->cardDetect_FlashTab_TabPage->Padding = System::Windows::Forms::Padding(3);
			this->cardDetect_FlashTab_TabPage->Size = System::Drawing::Size(632, 319);
			this->cardDetect_FlashTab_TabPage->TabIndex = 3;
			this->cardDetect_FlashTab_TabPage->Text = L"Card Detect";
			// 
			// destination_cardDetect_FlashTab_Label
			// 
			this->destination_cardDetect_FlashTab_Label->AutoSize = true;
			this->destination_cardDetect_FlashTab_Label->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(177)));
			this->destination_cardDetect_FlashTab_Label->Location = System::Drawing::Point(64, 192);
			this->destination_cardDetect_FlashTab_Label->Name = L"destination_cardDetect_FlashTab_Label";
			this->destination_cardDetect_FlashTab_Label->Size = System::Drawing::Size(83, 17);
			this->destination_cardDetect_FlashTab_Label->TabIndex = 26;
			this->destination_cardDetect_FlashTab_Label->Text = L"Destination:";
			// 
			// destination_cardDetect_FlashTab_TextBox
			// 
			this->destination_cardDetect_FlashTab_TextBox->Location = System::Drawing::Point(150, 192);
			this->destination_cardDetect_FlashTab_TextBox->Name = L"destination_cardDetect_FlashTab_TextBox";
			this->destination_cardDetect_FlashTab_TextBox->Size = System::Drawing::Size(57, 20);
			this->destination_cardDetect_FlashTab_TextBox->TabIndex = 25;
			// 
			// cardDetect_cardDetect_FlashTab_Button
			// 
			this->cardDetect_cardDetect_FlashTab_Button->AutoSize = true;
			this->cardDetect_cardDetect_FlashTab_Button->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
			this->cardDetect_cardDetect_FlashTab_Button->Location = System::Drawing::Point(276, 250);
			this->cardDetect_cardDetect_FlashTab_Button->Name = L"cardDetect_cardDetect_FlashTab_Button";
			this->cardDetect_cardDetect_FlashTab_Button->Size = System::Drawing::Size(80, 28);
			this->cardDetect_cardDetect_FlashTab_Button->TabIndex = 24;
			this->cardDetect_cardDetect_FlashTab_Button->Text = L"Card Detect";
			this->cardDetect_cardDetect_FlashTab_Button->UseVisualStyleBackColor = true;
			this->cardDetect_cardDetect_FlashTab_Button->Click += gcnew System::EventHandler(this, &GUI::cardDetect_FlashTab_Button_Click);
			// 
			// cardDetect_FlashTab_TextBox
			// 
			this->cardDetect_FlashTab_TextBox->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->cardDetect_FlashTab_TextBox->Location = System::Drawing::Point(6, 4);
			this->cardDetect_FlashTab_TextBox->Multiline = true;
			this->cardDetect_FlashTab_TextBox->Name = L"cardDetect_FlashTab_TextBox";
			this->cardDetect_FlashTab_TextBox->ReadOnly = true;
			this->cardDetect_FlashTab_TextBox->Size = System::Drawing::Size(620, 85);
			this->cardDetect_FlashTab_TextBox->TabIndex = 1;
			this->cardDetect_FlashTab_TextBox->Text = L"Wistone\'s Flash Card Detect:\r\n\r\n^ Choose \"Destination\" ( Network ID of Wistone or" 
				L" 0 for Broadcast )\r\n^ Press \"Card Detect\" to check for Flash Card";
			// 
			// writeSector_FlashTab_TabPage
			// 
			this->writeSector_FlashTab_TabPage->BackColor = System::Drawing::Color::WhiteSmoke;
			this->writeSector_FlashTab_TabPage->Controls->Add(this->destination_writeSector_FlashTab_Label);
			this->writeSector_FlashTab_TabPage->Controls->Add(this->destination_writeSector_FlashTab_TextBox);
			this->writeSector_FlashTab_TabPage->Controls->Add(this->write_writeSector_FlashTab_Button);
			this->writeSector_FlashTab_TabPage->Controls->Add(this->data_writeSector_FlashTab_Label);
			this->writeSector_FlashTab_TabPage->Controls->Add(this->data_writeSector_FlashTab_TextBox);
			this->writeSector_FlashTab_TabPage->Controls->Add(this->address_writeSector_FlashTab_TextBox);
			this->writeSector_FlashTab_TabPage->Controls->Add(this->writeSector_FlashTab_TextBox);
			this->writeSector_FlashTab_TabPage->Controls->Add(this->address_writeSector_FlashTab_Label);
			this->writeSector_FlashTab_TabPage->Location = System::Drawing::Point(4, 22);
			this->writeSector_FlashTab_TabPage->Name = L"writeSector_FlashTab_TabPage";
			this->writeSector_FlashTab_TabPage->Padding = System::Windows::Forms::Padding(3);
			this->writeSector_FlashTab_TabPage->Size = System::Drawing::Size(632, 319);
			this->writeSector_FlashTab_TabPage->TabIndex = 5;
			this->writeSector_FlashTab_TabPage->Text = L"Write Sector";
			// 
			// destination_writeSector_FlashTab_Label
			// 
			this->destination_writeSector_FlashTab_Label->AutoSize = true;
			this->destination_writeSector_FlashTab_Label->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(177)));
			this->destination_writeSector_FlashTab_Label->Location = System::Drawing::Point(41, 152);
			this->destination_writeSector_FlashTab_Label->Name = L"destination_writeSector_FlashTab_Label";
			this->destination_writeSector_FlashTab_Label->Size = System::Drawing::Size(83, 17);
			this->destination_writeSector_FlashTab_Label->TabIndex = 13;
			this->destination_writeSector_FlashTab_Label->Text = L"Destination:";
			// 
			// destination_writeSector_FlashTab_TextBox
			// 
			this->destination_writeSector_FlashTab_TextBox->Location = System::Drawing::Point(124, 150);
			this->destination_writeSector_FlashTab_TextBox->Name = L"destination_writeSector_FlashTab_TextBox";
			this->destination_writeSector_FlashTab_TextBox->Size = System::Drawing::Size(71, 20);
			this->destination_writeSector_FlashTab_TextBox->TabIndex = 12;
			// 
			// write_writeSector_FlashTab_Button
			// 
			this->write_writeSector_FlashTab_Button->AutoSize = true;
			this->write_writeSector_FlashTab_Button->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
			this->write_writeSector_FlashTab_Button->Location = System::Drawing::Point(276, 250);
			this->write_writeSector_FlashTab_Button->Name = L"write_writeSector_FlashTab_Button";
			this->write_writeSector_FlashTab_Button->Size = System::Drawing::Size(80, 28);
			this->write_writeSector_FlashTab_Button->TabIndex = 11;
			this->write_writeSector_FlashTab_Button->Text = L"Write";
			this->write_writeSector_FlashTab_Button->UseVisualStyleBackColor = true;
			this->write_writeSector_FlashTab_Button->Click += gcnew System::EventHandler(this, &GUI::write_writeSector_FlashTab_Button_Click);
			// 
			// data_writeSector_FlashTab_Label
			// 
			this->data_writeSector_FlashTab_Label->AutoSize = true;
			this->data_writeSector_FlashTab_Label->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->data_writeSector_FlashTab_Label->Location = System::Drawing::Point(43, 217);
			this->data_writeSector_FlashTab_Label->Name = L"data_writeSector_FlashTab_Label";
			this->data_writeSector_FlashTab_Label->Size = System::Drawing::Size(42, 17);
			this->data_writeSector_FlashTab_Label->TabIndex = 6;
			this->data_writeSector_FlashTab_Label->Text = L"Data:";
			// 
			// data_writeSector_FlashTab_TextBox
			// 
			this->data_writeSector_FlashTab_TextBox->Location = System::Drawing::Point(124, 217);
			this->data_writeSector_FlashTab_TextBox->Multiline = true;
			this->data_writeSector_FlashTab_TextBox->Name = L"data_writeSector_FlashTab_TextBox";
			this->data_writeSector_FlashTab_TextBox->Size = System::Drawing::Size(403, 20);
			this->data_writeSector_FlashTab_TextBox->TabIndex = 5;
			// 
			// address_writeSector_FlashTab_TextBox
			// 
			this->address_writeSector_FlashTab_TextBox->Location = System::Drawing::Point(124, 183);
			this->address_writeSector_FlashTab_TextBox->Name = L"address_writeSector_FlashTab_TextBox";
			this->address_writeSector_FlashTab_TextBox->Size = System::Drawing::Size(71, 20);
			this->address_writeSector_FlashTab_TextBox->TabIndex = 3;
			// 
			// writeSector_FlashTab_TextBox
			// 
			this->writeSector_FlashTab_TextBox->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->writeSector_FlashTab_TextBox->Location = System::Drawing::Point(6, 4);
			this->writeSector_FlashTab_TextBox->Multiline = true;
			this->writeSector_FlashTab_TextBox->Name = L"writeSector_FlashTab_TextBox";
			this->writeSector_FlashTab_TextBox->ReadOnly = true;
			this->writeSector_FlashTab_TextBox->Size = System::Drawing::Size(620, 114);
			this->writeSector_FlashTab_TextBox->TabIndex = 2;
			this->writeSector_FlashTab_TextBox->Text = L"Write Sector to Wistone\'s Flash:\r\n\r\n^ Choose \"Destination\" ( Network ID of Wiston" 
				L"e or 0 for Broadcast )\r\n^ Enter \"Address\" ( sector address )\r\n^ Enter \"Data\" - m" 
				L"aximum 0.5 KB\r\n^ Press \"Write\"";
			// 
			// address_writeSector_FlashTab_Label
			// 
			this->address_writeSector_FlashTab_Label->AutoSize = true;
			this->address_writeSector_FlashTab_Label->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->address_writeSector_FlashTab_Label->Location = System::Drawing::Point(43, 186);
			this->address_writeSector_FlashTab_Label->Name = L"address_writeSector_FlashTab_Label";
			this->address_writeSector_FlashTab_Label->Size = System::Drawing::Size(64, 17);
			this->address_writeSector_FlashTab_Label->TabIndex = 4;
			this->address_writeSector_FlashTab_Label->Text = L"Address:";
			// 
			// readSector_FlashTab_TabPage
			// 
			this->readSector_FlashTab_TabPage->BackColor = System::Drawing::Color::WhiteSmoke;
			this->readSector_FlashTab_TabPage->Controls->Add(this->destination_readSector_FlashTab_Label);
			this->readSector_FlashTab_TabPage->Controls->Add(this->destination_readSector_FlashTab_TextBox);
			this->readSector_FlashTab_TabPage->Controls->Add(this->read_readSector_FlashTab_Button);
			this->readSector_FlashTab_TabPage->Controls->Add(this->address_readSector_FlashTab_Label);
			this->readSector_FlashTab_TabPage->Controls->Add(this->address_readSector_FlashTab_TextBox);
			this->readSector_FlashTab_TabPage->Controls->Add(this->readSector_FlashTab_TextBox);
			this->readSector_FlashTab_TabPage->Location = System::Drawing::Point(4, 22);
			this->readSector_FlashTab_TabPage->Name = L"readSector_FlashTab_TabPage";
			this->readSector_FlashTab_TabPage->Padding = System::Windows::Forms::Padding(3);
			this->readSector_FlashTab_TabPage->Size = System::Drawing::Size(632, 319);
			this->readSector_FlashTab_TabPage->TabIndex = 6;
			this->readSector_FlashTab_TabPage->Text = L"Read Sector";
			// 
			// destination_readSector_FlashTab_Label
			// 
			this->destination_readSector_FlashTab_Label->AutoSize = true;
			this->destination_readSector_FlashTab_Label->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(177)));
			this->destination_readSector_FlashTab_Label->Location = System::Drawing::Point(40, 152);
			this->destination_readSector_FlashTab_Label->Name = L"destination_readSector_FlashTab_Label";
			this->destination_readSector_FlashTab_Label->Size = System::Drawing::Size(83, 17);
			this->destination_readSector_FlashTab_Label->TabIndex = 13;
			this->destination_readSector_FlashTab_Label->Text = L"Destination:";
			// 
			// destination_readSector_FlashTab_TextBox
			// 
			this->destination_readSector_FlashTab_TextBox->Location = System::Drawing::Point(124, 150);
			this->destination_readSector_FlashTab_TextBox->Name = L"destination_readSector_FlashTab_TextBox";
			this->destination_readSector_FlashTab_TextBox->Size = System::Drawing::Size(71, 20);
			this->destination_readSector_FlashTab_TextBox->TabIndex = 12;
			// 
			// read_readSector_FlashTab_Button
			// 
			this->read_readSector_FlashTab_Button->AutoSize = true;
			this->read_readSector_FlashTab_Button->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
			this->read_readSector_FlashTab_Button->Location = System::Drawing::Point(276, 250);
			this->read_readSector_FlashTab_Button->Name = L"read_readSector_FlashTab_Button";
			this->read_readSector_FlashTab_Button->Size = System::Drawing::Size(80, 28);
			this->read_readSector_FlashTab_Button->TabIndex = 11;
			this->read_readSector_FlashTab_Button->Text = L"Read";
			this->read_readSector_FlashTab_Button->UseVisualStyleBackColor = true;
			this->read_readSector_FlashTab_Button->Click += gcnew System::EventHandler(this, &GUI::read_readSector_FlashTab_Button_Click);
			// 
			// address_readSector_FlashTab_Label
			// 
			this->address_readSector_FlashTab_Label->AutoSize = true;
			this->address_readSector_FlashTab_Label->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->address_readSector_FlashTab_Label->Location = System::Drawing::Point(42, 186);
			this->address_readSector_FlashTab_Label->Name = L"address_readSector_FlashTab_Label";
			this->address_readSector_FlashTab_Label->Size = System::Drawing::Size(64, 17);
			this->address_readSector_FlashTab_Label->TabIndex = 7;
			this->address_readSector_FlashTab_Label->Text = L"Address:";
			// 
			// address_readSector_FlashTab_TextBox
			// 
			this->address_readSector_FlashTab_TextBox->Location = System::Drawing::Point(124, 183);
			this->address_readSector_FlashTab_TextBox->Name = L"address_readSector_FlashTab_TextBox";
			this->address_readSector_FlashTab_TextBox->Size = System::Drawing::Size(71, 20);
			this->address_readSector_FlashTab_TextBox->TabIndex = 6;
			// 
			// readSector_FlashTab_TextBox
			// 
			this->readSector_FlashTab_TextBox->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->readSector_FlashTab_TextBox->Location = System::Drawing::Point(6, 4);
			this->readSector_FlashTab_TextBox->Multiline = true;
			this->readSector_FlashTab_TextBox->Name = L"readSector_FlashTab_TextBox";
			this->readSector_FlashTab_TextBox->ReadOnly = true;
			this->readSector_FlashTab_TextBox->Size = System::Drawing::Size(620, 98);
			this->readSector_FlashTab_TextBox->TabIndex = 5;
			this->readSector_FlashTab_TextBox->Text = L"Read Sector from Wistone\'s Flash:\r\n\r\n^ Choose \"Destination\" ( Network ID of Wisto" 
				L"ne or 0 for Broadcast )\r\n^ Enter \"Address\" ( sector address )\r\n^ Press \"Read\"";
			// 
			// getCapacity_FlashTab_TabPage
			// 
			this->getCapacity_FlashTab_TabPage->BackColor = System::Drawing::Color::WhiteSmoke;
			this->getCapacity_FlashTab_TabPage->Controls->Add(this->destination_getCapacity_FlashTab_Label);
			this->getCapacity_FlashTab_TabPage->Controls->Add(this->destination_getCapacity_FlashTab_TextBox);
			this->getCapacity_FlashTab_TabPage->Controls->Add(this->getCapacity_getCapacity_FlashTab_Button);
			this->getCapacity_FlashTab_TabPage->Controls->Add(this->getCapacity_FlashTab_TextBox);
			this->getCapacity_FlashTab_TabPage->Location = System::Drawing::Point(4, 22);
			this->getCapacity_FlashTab_TabPage->Name = L"getCapacity_FlashTab_TabPage";
			this->getCapacity_FlashTab_TabPage->Padding = System::Windows::Forms::Padding(3);
			this->getCapacity_FlashTab_TabPage->Size = System::Drawing::Size(632, 319);
			this->getCapacity_FlashTab_TabPage->TabIndex = 7;
			this->getCapacity_FlashTab_TabPage->Text = L"Get Capacity";
			// 
			// destination_getCapacity_FlashTab_Label
			// 
			this->destination_getCapacity_FlashTab_Label->AutoSize = true;
			this->destination_getCapacity_FlashTab_Label->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(177)));
			this->destination_getCapacity_FlashTab_Label->Location = System::Drawing::Point(64, 192);
			this->destination_getCapacity_FlashTab_Label->Name = L"destination_getCapacity_FlashTab_Label";
			this->destination_getCapacity_FlashTab_Label->Size = System::Drawing::Size(83, 17);
			this->destination_getCapacity_FlashTab_Label->TabIndex = 13;
			this->destination_getCapacity_FlashTab_Label->Text = L"Destination:";
			// 
			// destination_getCapacity_FlashTab_TextBox
			// 
			this->destination_getCapacity_FlashTab_TextBox->Location = System::Drawing::Point(150, 192);
			this->destination_getCapacity_FlashTab_TextBox->Name = L"destination_getCapacity_FlashTab_TextBox";
			this->destination_getCapacity_FlashTab_TextBox->Size = System::Drawing::Size(57, 20);
			this->destination_getCapacity_FlashTab_TextBox->TabIndex = 12;
			// 
			// getCapacity_getCapacity_FlashTab_Button
			// 
			this->getCapacity_getCapacity_FlashTab_Button->AutoSize = true;
			this->getCapacity_getCapacity_FlashTab_Button->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
			this->getCapacity_getCapacity_FlashTab_Button->Location = System::Drawing::Point(276, 250);
			this->getCapacity_getCapacity_FlashTab_Button->Name = L"getCapacity_getCapacity_FlashTab_Button";
			this->getCapacity_getCapacity_FlashTab_Button->Size = System::Drawing::Size(80, 28);
			this->getCapacity_getCapacity_FlashTab_Button->TabIndex = 11;
			this->getCapacity_getCapacity_FlashTab_Button->Text = L"Get Capacity";
			this->getCapacity_getCapacity_FlashTab_Button->UseVisualStyleBackColor = true;
			this->getCapacity_getCapacity_FlashTab_Button->Click += gcnew System::EventHandler(this, &GUI::getCapacity_FlashTab_Button_Click);
			// 
			// getCapacity_FlashTab_TextBox
			// 
			this->getCapacity_FlashTab_TextBox->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->getCapacity_FlashTab_TextBox->Location = System::Drawing::Point(6, 5);
			this->getCapacity_FlashTab_TextBox->Multiline = true;
			this->getCapacity_FlashTab_TextBox->Name = L"getCapacity_FlashTab_TextBox";
			this->getCapacity_FlashTab_TextBox->ReadOnly = true;
			this->getCapacity_FlashTab_TextBox->Size = System::Drawing::Size(620, 86);
			this->getCapacity_FlashTab_TextBox->TabIndex = 2;
			this->getCapacity_FlashTab_TextBox->Text = L"Get the Capacity of Wistone\'s Flash:\r\n\r\n^ Choose \"Destination\" ( Network ID of Wi" 
				L"stone or 0 for Broadcast )\r\n^ Press \"Get Capacity\" to get Flash capacity in sect" 
				L"ors";
			// 
			// readLL_FlashTab_TabPage
			// 
			this->readLL_FlashTab_TabPage->BackColor = System::Drawing::Color::WhiteSmoke;
			this->readLL_FlashTab_TabPage->Controls->Add(this->destination_readLL_FlashTab_Label);
			this->readLL_FlashTab_TabPage->Controls->Add(this->destination_readLL_FlashTab_TextBox);
			this->readLL_FlashTab_TabPage->Controls->Add(this->read_readLL_FlashTab_Button);
			this->readLL_FlashTab_TabPage->Controls->Add(this->address_readLL_FlashTab_Label);
			this->readLL_FlashTab_TabPage->Controls->Add(this->address_readLL_FlashTab_TextBox);
			this->readLL_FlashTab_TabPage->Controls->Add(this->readLL_FlashTab_TextBox);
			this->readLL_FlashTab_TabPage->Location = System::Drawing::Point(4, 22);
			this->readLL_FlashTab_TabPage->Name = L"readLL_FlashTab_TabPage";
			this->readLL_FlashTab_TabPage->Padding = System::Windows::Forms::Padding(3);
			this->readLL_FlashTab_TabPage->Size = System::Drawing::Size(632, 319);
			this->readLL_FlashTab_TabPage->TabIndex = 0;
			this->readLL_FlashTab_TabPage->Text = L"Read (Low Level)";
			// 
			// destination_readLL_FlashTab_Label
			// 
			this->destination_readLL_FlashTab_Label->AutoSize = true;
			this->destination_readLL_FlashTab_Label->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(177)));
			this->destination_readLL_FlashTab_Label->Location = System::Drawing::Point(38, 152);
			this->destination_readLL_FlashTab_Label->Name = L"destination_readLL_FlashTab_Label";
			this->destination_readLL_FlashTab_Label->Size = System::Drawing::Size(83, 17);
			this->destination_readLL_FlashTab_Label->TabIndex = 13;
			this->destination_readLL_FlashTab_Label->Text = L"Destination:";
			// 
			// destination_readLL_FlashTab_TextBox
			// 
			this->destination_readLL_FlashTab_TextBox->Location = System::Drawing::Point(124, 150);
			this->destination_readLL_FlashTab_TextBox->Name = L"destination_readLL_FlashTab_TextBox";
			this->destination_readLL_FlashTab_TextBox->Size = System::Drawing::Size(71, 20);
			this->destination_readLL_FlashTab_TextBox->TabIndex = 12;
			// 
			// read_readLL_FlashTab_Button
			// 
			this->read_readLL_FlashTab_Button->AutoSize = true;
			this->read_readLL_FlashTab_Button->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
			this->read_readLL_FlashTab_Button->Location = System::Drawing::Point(276, 250);
			this->read_readLL_FlashTab_Button->Name = L"read_readLL_FlashTab_Button";
			this->read_readLL_FlashTab_Button->Size = System::Drawing::Size(80, 28);
			this->read_readLL_FlashTab_Button->TabIndex = 11;
			this->read_readLL_FlashTab_Button->Text = L"Read";
			this->read_readLL_FlashTab_Button->UseVisualStyleBackColor = true;
			this->read_readLL_FlashTab_Button->Click += gcnew System::EventHandler(this, &GUI::read_readLL_FlashTab_Button_Click);
			// 
			// address_readLL_FlashTab_Label
			// 
			this->address_readLL_FlashTab_Label->AutoSize = true;
			this->address_readLL_FlashTab_Label->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->address_readLL_FlashTab_Label->Location = System::Drawing::Point(38, 186);
			this->address_readLL_FlashTab_Label->Name = L"address_readLL_FlashTab_Label";
			this->address_readLL_FlashTab_Label->Size = System::Drawing::Size(64, 17);
			this->address_readLL_FlashTab_Label->TabIndex = 10;
			this->address_readLL_FlashTab_Label->Text = L"Address:";
			// 
			// address_readLL_FlashTab_TextBox
			// 
			this->address_readLL_FlashTab_TextBox->Location = System::Drawing::Point(124, 183);
			this->address_readLL_FlashTab_TextBox->Name = L"address_readLL_FlashTab_TextBox";
			this->address_readLL_FlashTab_TextBox->Size = System::Drawing::Size(71, 20);
			this->address_readLL_FlashTab_TextBox->TabIndex = 9;
			this->address_readLL_FlashTab_TextBox->Text = L"0";
			// 
			// readLL_FlashTab_TextBox
			// 
			this->readLL_FlashTab_TextBox->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->readLL_FlashTab_TextBox->Location = System::Drawing::Point(6, 4);
			this->readLL_FlashTab_TextBox->Multiline = true;
			this->readLL_FlashTab_TextBox->Name = L"readLL_FlashTab_TextBox";
			this->readLL_FlashTab_TextBox->ReadOnly = true;
			this->readLL_FlashTab_TextBox->Size = System::Drawing::Size(620, 108);
			this->readLL_FlashTab_TextBox->TabIndex = 8;
			this->readLL_FlashTab_TextBox->Text = L"Read a byte from Wistone\'s Flash:\r\n\r\n^ Choose \"Destination\" ( Network ID of Wisto" 
				L"ne or 0 for Broadcast )\r\n^ Enter \"Address\" ( of the byte )\r\n^ Press \"Read\"";
			// 
			// writeLL_FlashTab_TabPage
			// 
			this->writeLL_FlashTab_TabPage->BackColor = System::Drawing::Color::WhiteSmoke;
			this->writeLL_FlashTab_TabPage->Controls->Add(this->destination_writeLL_FlashTab_Label);
			this->writeLL_FlashTab_TabPage->Controls->Add(this->destination_writeLL_FlashTab_TextBox);
			this->writeLL_FlashTab_TabPage->Controls->Add(this->write_writeLL_FlashTab_Button);
			this->writeLL_FlashTab_TabPage->Controls->Add(this->data_writeLL_FlashTab_Label);
			this->writeLL_FlashTab_TabPage->Controls->Add(this->data_writeLL_FlashTab_TextBox);
			this->writeLL_FlashTab_TabPage->Controls->Add(this->address_writeLL_FlashTab_TextBox);
			this->writeLL_FlashTab_TabPage->Controls->Add(this->writeLL_FlashTab_TextBox);
			this->writeLL_FlashTab_TabPage->Controls->Add(this->address_writeLL_FlashTab_Label);
			this->writeLL_FlashTab_TabPage->Location = System::Drawing::Point(4, 22);
			this->writeLL_FlashTab_TabPage->Name = L"writeLL_FlashTab_TabPage";
			this->writeLL_FlashTab_TabPage->Padding = System::Windows::Forms::Padding(3);
			this->writeLL_FlashTab_TabPage->Size = System::Drawing::Size(632, 319);
			this->writeLL_FlashTab_TabPage->TabIndex = 1;
			this->writeLL_FlashTab_TabPage->Text = L"Write (Low Level)";
			// 
			// destination_writeLL_FlashTab_Label
			// 
			this->destination_writeLL_FlashTab_Label->AutoSize = true;
			this->destination_writeLL_FlashTab_Label->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(177)));
			this->destination_writeLL_FlashTab_Label->Location = System::Drawing::Point(38, 152);
			this->destination_writeLL_FlashTab_Label->Name = L"destination_writeLL_FlashTab_Label";
			this->destination_writeLL_FlashTab_Label->Size = System::Drawing::Size(83, 17);
			this->destination_writeLL_FlashTab_Label->TabIndex = 14;
			this->destination_writeLL_FlashTab_Label->Text = L"Destination:";
			// 
			// destination_writeLL_FlashTab_TextBox
			// 
			this->destination_writeLL_FlashTab_TextBox->Location = System::Drawing::Point(124, 150);
			this->destination_writeLL_FlashTab_TextBox->Name = L"destination_writeLL_FlashTab_TextBox";
			this->destination_writeLL_FlashTab_TextBox->Size = System::Drawing::Size(71, 20);
			this->destination_writeLL_FlashTab_TextBox->TabIndex = 13;
			// 
			// write_writeLL_FlashTab_Button
			// 
			this->write_writeLL_FlashTab_Button->AutoSize = true;
			this->write_writeLL_FlashTab_Button->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
			this->write_writeLL_FlashTab_Button->Location = System::Drawing::Point(276, 250);
			this->write_writeLL_FlashTab_Button->Name = L"write_writeLL_FlashTab_Button";
			this->write_writeLL_FlashTab_Button->Size = System::Drawing::Size(80, 28);
			this->write_writeLL_FlashTab_Button->TabIndex = 12;
			this->write_writeLL_FlashTab_Button->Text = L"Write";
			this->write_writeLL_FlashTab_Button->UseVisualStyleBackColor = true;
			this->write_writeLL_FlashTab_Button->Click += gcnew System::EventHandler(this, &GUI::write_writeLL_FlashTab_Button_Click);
			// 
			// data_writeLL_FlashTab_Label
			// 
			this->data_writeLL_FlashTab_Label->AutoSize = true;
			this->data_writeLL_FlashTab_Label->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->data_writeLL_FlashTab_Label->Location = System::Drawing::Point(38, 217);
			this->data_writeLL_FlashTab_Label->Name = L"data_writeLL_FlashTab_Label";
			this->data_writeLL_FlashTab_Label->Size = System::Drawing::Size(42, 17);
			this->data_writeLL_FlashTab_Label->TabIndex = 11;
			this->data_writeLL_FlashTab_Label->Text = L"Data:";
			// 
			// data_writeLL_FlashTab_TextBox
			// 
			this->data_writeLL_FlashTab_TextBox->Location = System::Drawing::Point(124, 213);
			this->data_writeLL_FlashTab_TextBox->Multiline = true;
			this->data_writeLL_FlashTab_TextBox->Name = L"data_writeLL_FlashTab_TextBox";
			this->data_writeLL_FlashTab_TextBox->Size = System::Drawing::Size(70, 21);
			this->data_writeLL_FlashTab_TextBox->TabIndex = 10;
			// 
			// address_writeLL_FlashTab_TextBox
			// 
			this->address_writeLL_FlashTab_TextBox->Location = System::Drawing::Point(124, 183);
			this->address_writeLL_FlashTab_TextBox->Name = L"address_writeLL_FlashTab_TextBox";
			this->address_writeLL_FlashTab_TextBox->Size = System::Drawing::Size(71, 20);
			this->address_writeLL_FlashTab_TextBox->TabIndex = 8;
			this->address_writeLL_FlashTab_TextBox->Text = L"0";
			// 
			// writeLL_FlashTab_TextBox
			// 
			this->writeLL_FlashTab_TextBox->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->writeLL_FlashTab_TextBox->Location = System::Drawing::Point(6, 4);
			this->writeLL_FlashTab_TextBox->Multiline = true;
			this->writeLL_FlashTab_TextBox->Name = L"writeLL_FlashTab_TextBox";
			this->writeLL_FlashTab_TextBox->ReadOnly = true;
			this->writeLL_FlashTab_TextBox->Size = System::Drawing::Size(620, 106);
			this->writeLL_FlashTab_TextBox->TabIndex = 7;
			this->writeLL_FlashTab_TextBox->Text = L"Write a byte to Wistone\'s Flash:\r\n\r\n^ Choose \"Destination\" ( Network ID of Wiston" 
				L"e or 0 for Broadcast )\r\n^ Enter \"Address\" ( of the byte ) and \"Data\"\r\n^ Press \"W" 
				L"rite\"";
			// 
			// address_writeLL_FlashTab_Label
			// 
			this->address_writeLL_FlashTab_Label->AutoSize = true;
			this->address_writeLL_FlashTab_Label->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->address_writeLL_FlashTab_Label->Location = System::Drawing::Point(38, 186);
			this->address_writeLL_FlashTab_Label->Name = L"address_writeLL_FlashTab_Label";
			this->address_writeLL_FlashTab_Label->Size = System::Drawing::Size(64, 17);
			this->address_writeLL_FlashTab_Label->TabIndex = 9;
			this->address_writeLL_FlashTab_Label->Text = L"Address:";
			// 
			// tabApplication
			// 
			this->tabApplication->BackColor = System::Drawing::Color::WhiteSmoke;
			this->tabApplication->Controls->Add(this->applicationTab);
			this->tabApplication->Location = System::Drawing::Point(4, 22);
			this->tabApplication->Name = L"tabApplication";
			this->tabApplication->Padding = System::Windows::Forms::Padding(3);
			this->tabApplication->Size = System::Drawing::Size(633, 339);
			this->tabApplication->TabIndex = 1;
			this->tabApplication->Text = L"Application";
			// 
			// applicationTab
			// 
			this->applicationTab->Controls->Add(this->start_AppTab_TabPage);
			this->applicationTab->Controls->Add(this->shutdown_AppTab_TabPage);
			this->applicationTab->Controls->Add(this->config_AppTab_TabPage);
			this->applicationTab->Location = System::Drawing::Point(0, 0);
			this->applicationTab->Name = L"applicationTab";
			this->applicationTab->SelectedIndex = 0;
			this->applicationTab->Size = System::Drawing::Size(640, 345);
			this->applicationTab->TabIndex = 2;
			// 
			// start_AppTab_TabPage
			// 
			this->start_AppTab_TabPage->BackColor = System::Drawing::Color::WhiteSmoke;
			this->start_AppTab_TabPage->Controls->Add(this->destination_start_AppTab_TextBox);
			this->start_AppTab_TabPage->Controls->Add(this->destination_start_AppTab_Label);
			this->start_AppTab_TabPage->Controls->Add(this->sessionTimeout_start_AppTab_Label);
			this->start_AppTab_TabPage->Controls->Add(this->sessionTimeout_start_AppTab_TextBox);
			this->start_AppTab_TabPage->Controls->Add(this->stopApplication_start_AppTab_Button);
			this->start_AppTab_TabPage->Controls->Add(this->outputFile_start_AppTab_Label);
			this->start_AppTab_TabPage->Controls->Add(this->applicationMode_GroupBox);
			this->start_AppTab_TabPage->Controls->Add(this->combination_start_AppTab_Label);
			this->start_AppTab_TabPage->Controls->Add(this->combination_start_AppTab_ComboBox);
			this->start_AppTab_TabPage->Controls->Add(this->startApplication_start_AppTab_Button);
			this->start_AppTab_TabPage->Controls->Add(this->address_start_AppTab_Label);
			this->start_AppTab_TabPage->Controls->Add(this->address_start_AppTab_TextBox);
			this->start_AppTab_TabPage->Controls->Add(this->numberOfBlocks_start_AppTab_Label);
			this->start_AppTab_TabPage->Controls->Add(this->numberOfBlocks_start_AppTab_TextBox);
			this->start_AppTab_TabPage->Controls->Add(this->communication_start_AppTab_Label);
			this->start_AppTab_TabPage->Controls->Add(this->communication_start_AppTab_ComboBox);
			this->start_AppTab_TabPage->Controls->Add(this->outputFile_start_AppTab_TextBox);
			this->start_AppTab_TabPage->Controls->Add(this->start_AppTab_TextBox);
			this->start_AppTab_TabPage->Location = System::Drawing::Point(4, 22);
			this->start_AppTab_TabPage->Name = L"start_AppTab_TabPage";
			this->start_AppTab_TabPage->Padding = System::Windows::Forms::Padding(3);
			this->start_AppTab_TabPage->Size = System::Drawing::Size(632, 319);
			this->start_AppTab_TabPage->TabIndex = 2;
			this->start_AppTab_TabPage->Text = L"Start";
			// 
			// destination_start_AppTab_TextBox
			// 
			this->destination_start_AppTab_TextBox->Location = System::Drawing::Point(537, 140);
			this->destination_start_AppTab_TextBox->Name = L"destination_start_AppTab_TextBox";
			this->destination_start_AppTab_TextBox->Size = System::Drawing::Size(57, 20);
			this->destination_start_AppTab_TextBox->TabIndex = 27;
			// 
			// destination_start_AppTab_Label
			// 
			this->destination_start_AppTab_Label->AutoSize = true;
			this->destination_start_AppTab_Label->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(177)));
			this->destination_start_AppTab_Label->Location = System::Drawing::Point(414, 145);
			this->destination_start_AppTab_Label->Name = L"destination_start_AppTab_Label";
			this->destination_start_AppTab_Label->Size = System::Drawing::Size(83, 17);
			this->destination_start_AppTab_Label->TabIndex = 26;
			this->destination_start_AppTab_Label->Text = L"Destination:";
			// 
			// sessionTimeout_start_AppTab_Label
			// 
			this->sessionTimeout_start_AppTab_Label->AutoSize = true;
			this->sessionTimeout_start_AppTab_Label->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->sessionTimeout_start_AppTab_Label->Location = System::Drawing::Point(414, 275);
			this->sessionTimeout_start_AppTab_Label->Name = L"sessionTimeout_start_AppTab_Label";
			this->sessionTimeout_start_AppTab_Label->Size = System::Drawing::Size(117, 17);
			this->sessionTimeout_start_AppTab_Label->TabIndex = 24;
			this->sessionTimeout_start_AppTab_Label->Text = L"Session Timeout:";
			// 
			// sessionTimeout_start_AppTab_TextBox
			// 
			this->sessionTimeout_start_AppTab_TextBox->Location = System::Drawing::Point(537, 274);
			this->sessionTimeout_start_AppTab_TextBox->Name = L"sessionTimeout_start_AppTab_TextBox";
			this->sessionTimeout_start_AppTab_TextBox->Size = System::Drawing::Size(57, 20);
			this->sessionTimeout_start_AppTab_TextBox->TabIndex = 23;
			this->sessionTimeout_start_AppTab_TextBox->Text = L"Seconds";
			// 
			// stopApplication_start_AppTab_Button
			// 
			this->stopApplication_start_AppTab_Button->AutoSize = true;
			this->stopApplication_start_AppTab_Button->BackColor = System::Drawing::Color::DarkGray;
			this->stopApplication_start_AppTab_Button->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
			this->stopApplication_start_AppTab_Button->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->stopApplication_start_AppTab_Button->Location = System::Drawing::Point(214, 275);
			this->stopApplication_start_AppTab_Button->Name = L"stopApplication_start_AppTab_Button";
			this->stopApplication_start_AppTab_Button->Size = System::Drawing::Size(110, 23);
			this->stopApplication_start_AppTab_Button->TabIndex = 22;
			this->stopApplication_start_AppTab_Button->Text = L"Stop Application";
			this->stopApplication_start_AppTab_Button->UseVisualStyleBackColor = false;
			this->stopApplication_start_AppTab_Button->Click += gcnew System::EventHandler(this, &GUI::stopApplication_AppTab_Button_Click);
			// 
			// outputFile_start_AppTab_Label
			// 
			this->outputFile_start_AppTab_Label->AutoSize = true;
			this->outputFile_start_AppTab_Label->Location = System::Drawing::Point(29, 246);
			this->outputFile_start_AppTab_Label->Name = L"outputFile_start_AppTab_Label";
			this->outputFile_start_AppTab_Label->Size = System::Drawing::Size(61, 13);
			this->outputFile_start_AppTab_Label->TabIndex = 21;
			this->outputFile_start_AppTab_Label->Text = L"Output File:";
			// 
			// applicationMode_GroupBox
			// 
			this->applicationMode_GroupBox->Controls->Add(this->ss_RadioButton);
			this->applicationMode_GroupBox->Controls->Add(this->ts_RadioButton);
			this->applicationMode_GroupBox->Controls->Add(this->ost_RadioButton);
			this->applicationMode_GroupBox->Location = System::Drawing::Point(32, 126);
			this->applicationMode_GroupBox->Name = L"applicationMode_GroupBox";
			this->applicationMode_GroupBox->Size = System::Drawing::Size(207, 97);
			this->applicationMode_GroupBox->TabIndex = 20;
			this->applicationMode_GroupBox->TabStop = false;
			this->applicationMode_GroupBox->Text = L"Application Mode:";
			// 
			// ss_RadioButton
			// 
			this->ss_RadioButton->AutoSize = true;
			this->ss_RadioButton->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->ss_RadioButton->Location = System::Drawing::Point(6, 19);
			this->ss_RadioButton->Name = L"ss_RadioButton";
			this->ss_RadioButton->Size = System::Drawing::Size(170, 19);
			this->ss_RadioButton->TabIndex = 1;
			this->ss_RadioButton->TabStop = true;
			this->ss_RadioButton->Text = L"Sample and Store in Flash";
			this->ss_RadioButton->UseVisualStyleBackColor = true;
			this->ss_RadioButton->CheckedChanged += gcnew System::EventHandler(this, &GUI::ss_RadioButton_Click);
			// 
			// ts_RadioButton
			// 
			this->ts_RadioButton->AutoSize = true;
			this->ts_RadioButton->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->ts_RadioButton->Location = System::Drawing::Point(6, 43);
			this->ts_RadioButton->Name = L"ts_RadioButton";
			this->ts_RadioButton->Size = System::Drawing::Size(186, 19);
			this->ts_RadioButton->TabIndex = 2;
			this->ts_RadioButton->TabStop = true;
			this->ts_RadioButton->Text = L"Transmit Samples from Flash";
			this->ts_RadioButton->UseVisualStyleBackColor = true;
			this->ts_RadioButton->CheckedChanged += gcnew System::EventHandler(this, &GUI::ts_RadioButton_Click);
			// 
			// ost_RadioButton
			// 
			this->ost_RadioButton->AutoSize = true;
			this->ost_RadioButton->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->ost_RadioButton->Location = System::Drawing::Point(6, 68);
			this->ost_RadioButton->Name = L"ost_RadioButton";
			this->ost_RadioButton->Size = System::Drawing::Size(196, 19);
			this->ost_RadioButton->TabIndex = 3;
			this->ost_RadioButton->TabStop = true;
			this->ost_RadioButton->Text = L"Realtime Sample and Transmit";
			this->ost_RadioButton->UseVisualStyleBackColor = true;
			this->ost_RadioButton->CheckedChanged += gcnew System::EventHandler(this, &GUI::ost_RadioButton_Click);
			// 
			// combination_start_AppTab_Label
			// 
			this->combination_start_AppTab_Label->AutoSize = true;
			this->combination_start_AppTab_Label->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->combination_start_AppTab_Label->Location = System::Drawing::Point(414, 249);
			this->combination_start_AppTab_Label->Name = L"combination_start_AppTab_Label";
			this->combination_start_AppTab_Label->Size = System::Drawing::Size(90, 17);
			this->combination_start_AppTab_Label->TabIndex = 19;
			this->combination_start_AppTab_Label->Text = L"Combination:";
			// 
			// combination_start_AppTab_ComboBox
			// 
			this->combination_start_AppTab_ComboBox->FormattingEnabled = true;
			this->combination_start_AppTab_ComboBox->Items->AddRange(gcnew cli::array< System::Object^  >(2) {L"dual", L"single"});
			this->combination_start_AppTab_ComboBox->Location = System::Drawing::Point(537, 248);
			this->combination_start_AppTab_ComboBox->Name = L"combination_start_AppTab_ComboBox";
			this->combination_start_AppTab_ComboBox->Size = System::Drawing::Size(57, 21);
			this->combination_start_AppTab_ComboBox->TabIndex = 18;
			// 
			// startApplication_start_AppTab_Button
			// 
			this->startApplication_start_AppTab_Button->AutoSize = true;
			this->startApplication_start_AppTab_Button->BackColor = System::Drawing::Color::MistyRose;
			this->startApplication_start_AppTab_Button->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
			this->startApplication_start_AppTab_Button->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->startApplication_start_AppTab_Button->Location = System::Drawing::Point(94, 275);
			this->startApplication_start_AppTab_Button->Name = L"startApplication_start_AppTab_Button";
			this->startApplication_start_AppTab_Button->Size = System::Drawing::Size(111, 23);
			this->startApplication_start_AppTab_Button->TabIndex = 17;
			this->startApplication_start_AppTab_Button->Text = L"Start Application";
			this->startApplication_start_AppTab_Button->UseVisualStyleBackColor = false;
			this->startApplication_start_AppTab_Button->Click += gcnew System::EventHandler(this, &GUI::startApplication_AppTab_Button_Click);
			// 
			// address_start_AppTab_Label
			// 
			this->address_start_AppTab_Label->AutoSize = true;
			this->address_start_AppTab_Label->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->address_start_AppTab_Label->Location = System::Drawing::Point(414, 171);
			this->address_start_AppTab_Label->Name = L"address_start_AppTab_Label";
			this->address_start_AppTab_Label->Size = System::Drawing::Size(64, 17);
			this->address_start_AppTab_Label->TabIndex = 11;
			this->address_start_AppTab_Label->Text = L"Address:";
			// 
			// address_start_AppTab_TextBox
			// 
			this->address_start_AppTab_TextBox->Location = System::Drawing::Point(537, 170);
			this->address_start_AppTab_TextBox->Name = L"address_start_AppTab_TextBox";
			this->address_start_AppTab_TextBox->Size = System::Drawing::Size(57, 20);
			this->address_start_AppTab_TextBox->TabIndex = 10;
			// 
			// numberOfBlocks_start_AppTab_Label
			// 
			this->numberOfBlocks_start_AppTab_Label->AutoSize = true;
			this->numberOfBlocks_start_AppTab_Label->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->numberOfBlocks_start_AppTab_Label->Location = System::Drawing::Point(414, 197);
			this->numberOfBlocks_start_AppTab_Label->Name = L"numberOfBlocks_start_AppTab_Label";
			this->numberOfBlocks_start_AppTab_Label->Size = System::Drawing::Size(123, 17);
			this->numberOfBlocks_start_AppTab_Label->TabIndex = 9;
			this->numberOfBlocks_start_AppTab_Label->Text = L"Number of Blocks:";
			// 
			// numberOfBlocks_start_AppTab_TextBox
			// 
			this->numberOfBlocks_start_AppTab_TextBox->Location = System::Drawing::Point(537, 197);
			this->numberOfBlocks_start_AppTab_TextBox->Name = L"numberOfBlocks_start_AppTab_TextBox";
			this->numberOfBlocks_start_AppTab_TextBox->Size = System::Drawing::Size(57, 20);
			this->numberOfBlocks_start_AppTab_TextBox->TabIndex = 8;
			// 
			// communication_start_AppTab_Label
			// 
			this->communication_start_AppTab_Label->AutoSize = true;
			this->communication_start_AppTab_Label->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->communication_start_AppTab_Label->Location = System::Drawing::Point(414, 223);
			this->communication_start_AppTab_Label->Name = L"communication_start_AppTab_Label";
			this->communication_start_AppTab_Label->Size = System::Drawing::Size(108, 17);
			this->communication_start_AppTab_Label->TabIndex = 7;
			this->communication_start_AppTab_Label->Text = L"Communication:";
			// 
			// communication_start_AppTab_ComboBox
			// 
			this->communication_start_AppTab_ComboBox->FormattingEnabled = true;
			this->communication_start_AppTab_ComboBox->Items->AddRange(gcnew cli::array< System::Object^  >(2) {L"usb", L"wireless"});
			this->communication_start_AppTab_ComboBox->Location = System::Drawing::Point(537, 222);
			this->communication_start_AppTab_ComboBox->Name = L"communication_start_AppTab_ComboBox";
			this->communication_start_AppTab_ComboBox->Size = System::Drawing::Size(57, 21);
			this->communication_start_AppTab_ComboBox->TabIndex = 6;
			// 
			// outputFile_start_AppTab_TextBox
			// 
			this->outputFile_start_AppTab_TextBox->Location = System::Drawing::Point(94, 244);
			this->outputFile_start_AppTab_TextBox->Name = L"outputFile_start_AppTab_TextBox";
			this->outputFile_start_AppTab_TextBox->Size = System::Drawing::Size(230, 20);
			this->outputFile_start_AppTab_TextBox->TabIndex = 5;
			this->outputFile_start_AppTab_ToolTip->SetToolTip(this->outputFile_start_AppTab_TextBox, resources->GetString(L"outputFile_start_AppTab_TextBox.ToolTip"));
			// 
			// start_AppTab_TextBox
			// 
			this->start_AppTab_TextBox->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->start_AppTab_TextBox->Location = System::Drawing::Point(6, 4);
			this->start_AppTab_TextBox->Multiline = true;
			this->start_AppTab_TextBox->Name = L"start_AppTab_TextBox";
			this->start_AppTab_TextBox->ReadOnly = true;
			this->start_AppTab_TextBox->Size = System::Drawing::Size(617, 117);
			this->start_AppTab_TextBox->TabIndex = 0;
			this->start_AppTab_TextBox->Text = L"Start / Stop new Wistone operation:\r\n\r\n^ Select Application Mode\r\n^ Select Applic" 
				L"ation Parameters\r\n^ Press \"Start Application\"\r\n^ Press \"Stop Application\" (if re" 
				L"quired)";
			// 
			// shutdown_AppTab_TabPage
			// 
			this->shutdown_AppTab_TabPage->BackColor = System::Drawing::Color::WhiteSmoke;
			this->shutdown_AppTab_TabPage->Controls->Add(this->destination_shutdown_AppTab_Label);
			this->shutdown_AppTab_TabPage->Controls->Add(this->destination_shutdown_AppTab_TextBox);
			this->shutdown_AppTab_TabPage->Controls->Add(this->shutdown_shutdown_AppTab_Button);
			this->shutdown_AppTab_TabPage->Controls->Add(this->shutdown_AppTab_TextBox);
			this->shutdown_AppTab_TabPage->Location = System::Drawing::Point(4, 22);
			this->shutdown_AppTab_TabPage->Name = L"shutdown_AppTab_TabPage";
			this->shutdown_AppTab_TabPage->Padding = System::Windows::Forms::Padding(3);
			this->shutdown_AppTab_TabPage->Size = System::Drawing::Size(632, 319);
			this->shutdown_AppTab_TabPage->TabIndex = 5;
			this->shutdown_AppTab_TabPage->Text = L"Shutdown";
			// 
			// destination_shutdown_AppTab_Label
			// 
			this->destination_shutdown_AppTab_Label->AutoSize = true;
			this->destination_shutdown_AppTab_Label->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(177)));
			this->destination_shutdown_AppTab_Label->Location = System::Drawing::Point(64, 196);
			this->destination_shutdown_AppTab_Label->Name = L"destination_shutdown_AppTab_Label";
			this->destination_shutdown_AppTab_Label->Size = System::Drawing::Size(83, 17);
			this->destination_shutdown_AppTab_Label->TabIndex = 23;
			this->destination_shutdown_AppTab_Label->Text = L"Destination:";
			// 
			// destination_shutdown_AppTab_TextBox
			// 
			this->destination_shutdown_AppTab_TextBox->Location = System::Drawing::Point(150, 192);
			this->destination_shutdown_AppTab_TextBox->Name = L"destination_shutdown_AppTab_TextBox";
			this->destination_shutdown_AppTab_TextBox->Size = System::Drawing::Size(57, 20);
			this->destination_shutdown_AppTab_TextBox->TabIndex = 22;
			// 
			// shutdown_shutdown_AppTab_Button
			// 
			this->shutdown_shutdown_AppTab_Button->AutoSize = true;
			this->shutdown_shutdown_AppTab_Button->BackColor = System::Drawing::Color::Tomato;
			this->shutdown_shutdown_AppTab_Button->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
			this->shutdown_shutdown_AppTab_Button->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->shutdown_shutdown_AppTab_Button->Location = System::Drawing::Point(270, 250);
			this->shutdown_shutdown_AppTab_Button->Name = L"shutdown_shutdown_AppTab_Button";
			this->shutdown_shutdown_AppTab_Button->Size = System::Drawing::Size(88, 32);
			this->shutdown_shutdown_AppTab_Button->TabIndex = 21;
			this->shutdown_shutdown_AppTab_Button->Text = L"Shutdown";
			this->shutdown_shutdown_AppTab_Button->UseVisualStyleBackColor = false;
			this->shutdown_shutdown_AppTab_Button->Click += gcnew System::EventHandler(this, &GUI::shutdown_AppTab_Button_Click);
			// 
			// shutdown_AppTab_TextBox
			// 
			this->shutdown_AppTab_TextBox->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->shutdown_AppTab_TextBox->Location = System::Drawing::Point(6, 4);
			this->shutdown_AppTab_TextBox->Multiline = true;
			this->shutdown_AppTab_TextBox->Name = L"shutdown_AppTab_TextBox";
			this->shutdown_AppTab_TextBox->ReadOnly = true;
			this->shutdown_AppTab_TextBox->Size = System::Drawing::Size(617, 81);
			this->shutdown_AppTab_TextBox->TabIndex = 20;
			this->shutdown_AppTab_TextBox->Text = L"Shutdown Main Power:\r\n\r\n^  Select Destination ( Network ID of Wistone or 0 for Br" 
				L"oadcast )\r\n ^ Press Shutdown button to turn Wistone off";
			// 
			// config_AppTab_TabPage
			// 
			this->config_AppTab_TabPage->BackColor = System::Drawing::Color::WhiteSmoke;
			this->config_AppTab_TabPage->Controls->Add(this->destination_config_AppTab_Label);
			this->config_AppTab_TabPage->Controls->Add(this->destination_config_AppTab_TextBox);
			this->config_AppTab_TabPage->Controls->Add(this->parameter_config_AppTab_ComboBox);
			this->config_AppTab_TabPage->Controls->Add(this->value_config_AppTab_NumericUpDown);
			this->config_AppTab_TabPage->Controls->Add(this->updateValue_config_AppTab_Button);
			this->config_AppTab_TabPage->Controls->Add(this->parameter_config_AppTab_Label);
			this->config_AppTab_TabPage->Controls->Add(this->config_AppTab_TextBox);
			this->config_AppTab_TabPage->Controls->Add(this->value_config_AppTab_Label);
			this->config_AppTab_TabPage->Location = System::Drawing::Point(4, 22);
			this->config_AppTab_TabPage->Name = L"config_AppTab_TabPage";
			this->config_AppTab_TabPage->Size = System::Drawing::Size(632, 319);
			this->config_AppTab_TabPage->TabIndex = 6;
			this->config_AppTab_TabPage->Text = L"Config";
			// 
			// destination_config_AppTab_Label
			// 
			this->destination_config_AppTab_Label->AutoSize = true;
			this->destination_config_AppTab_Label->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(177)));
			this->destination_config_AppTab_Label->Location = System::Drawing::Point(14, 282);
			this->destination_config_AppTab_Label->Name = L"destination_config_AppTab_Label";
			this->destination_config_AppTab_Label->Size = System::Drawing::Size(87, 17);
			this->destination_config_AppTab_Label->TabIndex = 28;
			this->destination_config_AppTab_Label->Text = L"Destination: ";
			// 
			// destination_config_AppTab_TextBox
			// 
			this->destination_config_AppTab_TextBox->Location = System::Drawing::Point(104, 282);
			this->destination_config_AppTab_TextBox->Name = L"destination_config_AppTab_TextBox";
			this->destination_config_AppTab_TextBox->Size = System::Drawing::Size(57, 20);
			this->destination_config_AppTab_TextBox->TabIndex = 27;
			// 
			// parameter_config_AppTab_ComboBox
			// 
			this->parameter_config_AppTab_ComboBox->FormattingEnabled = true;
			this->parameter_config_AppTab_ComboBox->Items->AddRange(gcnew cli::array< System::Object^  >(1) {L"0"});
			this->parameter_config_AppTab_ComboBox->Location = System::Drawing::Point(257, 282);
			this->parameter_config_AppTab_ComboBox->Name = L"parameter_config_AppTab_ComboBox";
			this->parameter_config_AppTab_ComboBox->Size = System::Drawing::Size(72, 21);
			this->parameter_config_AppTab_ComboBox->TabIndex = 26;
			// 
			// value_config_AppTab_NumericUpDown
			// 
			this->value_config_AppTab_NumericUpDown->Location = System::Drawing::Point(401, 282);
			this->value_config_AppTab_NumericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {3, 0, 0, 0});
			this->value_config_AppTab_NumericUpDown->Name = L"value_config_AppTab_NumericUpDown";
			this->value_config_AppTab_NumericUpDown->Size = System::Drawing::Size(72, 20);
			this->value_config_AppTab_NumericUpDown->TabIndex = 25;
			// 
			// updateValue_config_AppTab_Button
			// 
			this->updateValue_config_AppTab_Button->AutoSize = true;
			this->updateValue_config_AppTab_Button->BackColor = System::Drawing::Color::MistyRose;
			this->updateValue_config_AppTab_Button->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
			this->updateValue_config_AppTab_Button->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->updateValue_config_AppTab_Button->Location = System::Drawing::Point(506, 281);
			this->updateValue_config_AppTab_Button->Name = L"updateValue_config_AppTab_Button";
			this->updateValue_config_AppTab_Button->Size = System::Drawing::Size(94, 23);
			this->updateValue_config_AppTab_Button->TabIndex = 24;
			this->updateValue_config_AppTab_Button->Text = L"Update Value";
			this->updateValue_config_AppTab_Button->UseVisualStyleBackColor = false;
			this->updateValue_config_AppTab_Button->Click += gcnew System::EventHandler(this, &GUI::updateValue_config_AppTab_Button_Click);
			// 
			// parameter_config_AppTab_Label
			// 
			this->parameter_config_AppTab_Label->AutoSize = true;
			this->parameter_config_AppTab_Label->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->parameter_config_AppTab_Label->Location = System::Drawing::Point(176, 282);
			this->parameter_config_AppTab_Label->Name = L"parameter_config_AppTab_Label";
			this->parameter_config_AppTab_Label->Size = System::Drawing::Size(78, 17);
			this->parameter_config_AppTab_Label->TabIndex = 23;
			this->parameter_config_AppTab_Label->Text = L"Parameter:";
			// 
			// config_AppTab_TextBox
			// 
			this->config_AppTab_TextBox->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->config_AppTab_TextBox->Location = System::Drawing::Point(6, 3);
			this->config_AppTab_TextBox->Multiline = true;
			this->config_AppTab_TextBox->Name = L"config_AppTab_TextBox";
			this->config_AppTab_TextBox->ReadOnly = true;
			this->config_AppTab_TextBox->Size = System::Drawing::Size(620, 266);
			this->config_AppTab_TextBox->TabIndex = 21;
			this->config_AppTab_TextBox->Text = resources->GetString(L"config_AppTab_TextBox.Text");
			// 
			// value_config_AppTab_Label
			// 
			this->value_config_AppTab_Label->AutoSize = true;
			this->value_config_AppTab_Label->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->value_config_AppTab_Label->Location = System::Drawing::Point(350, 282);
			this->value_config_AppTab_Label->Name = L"value_config_AppTab_Label";
			this->value_config_AppTab_Label->Size = System::Drawing::Size(48, 17);
			this->value_config_AppTab_Label->TabIndex = 22;
			this->value_config_AppTab_Label->Text = L"Value:";
			// 
			// tabSystem
			// 
			this->tabSystem->BackColor = System::Drawing::Color::WhiteSmoke;
			this->tabSystem->Controls->Add(this->systemTab);
			this->tabSystem->Location = System::Drawing::Point(4, 22);
			this->tabSystem->Name = L"tabSystem";
			this->tabSystem->Padding = System::Windows::Forms::Padding(3);
			this->tabSystem->Size = System::Drawing::Size(633, 339);
			this->tabSystem->TabIndex = 0;
			this->tabSystem->Text = L"System";
			// 
			// systemTab
			// 
			this->systemTab->Controls->Add(this->systemGetVersion_SystemTab_TabPage);
			this->systemTab->Controls->Add(this->systemSetId_SystemTab_TabPage);
			this->systemTab->Controls->Add(this->getPower_SystemTab_TabPage);
			this->systemTab->Controls->Add(this->setPower_SystemTab_TabPage);
			this->systemTab->Controls->Add(this->getLastError_SystemTab_TabPage);
			this->systemTab->Location = System::Drawing::Point(0, 0);
			this->systemTab->Name = L"systemTab";
			this->systemTab->SelectedIndex = 0;
			this->systemTab->Size = System::Drawing::Size(640, 345);
			this->systemTab->TabIndex = 1;
			// 
			// systemGetVersion_SystemTab_TabPage
			// 
			this->systemGetVersion_SystemTab_TabPage->BackColor = System::Drawing::Color::WhiteSmoke;
			this->systemGetVersion_SystemTab_TabPage->Controls->Add(this->destination_getSystemVersion_SystemTab_Label);
			this->systemGetVersion_SystemTab_TabPage->Controls->Add(this->destination_getSystemVersion_SystemTab_TextBox);
			this->systemGetVersion_SystemTab_TabPage->Controls->Add(this->systemGetVersion_systemGetVersion_SystemTab_Button);
			this->systemGetVersion_SystemTab_TabPage->Controls->Add(this->systemGetVersion_SystemTab_TextBox);
			this->systemGetVersion_SystemTab_TabPage->Location = System::Drawing::Point(4, 22);
			this->systemGetVersion_SystemTab_TabPage->Name = L"systemGetVersion_SystemTab_TabPage";
			this->systemGetVersion_SystemTab_TabPage->Padding = System::Windows::Forms::Padding(3);
			this->systemGetVersion_SystemTab_TabPage->Size = System::Drawing::Size(632, 319);
			this->systemGetVersion_SystemTab_TabPage->TabIndex = 2;
			this->systemGetVersion_SystemTab_TabPage->Text = L"Get System Version";
			// 
			// destination_getSystemVersion_SystemTab_Label
			// 
			this->destination_getSystemVersion_SystemTab_Label->AutoSize = true;
			this->destination_getSystemVersion_SystemTab_Label->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(177)));
			this->destination_getSystemVersion_SystemTab_Label->Location = System::Drawing::Point(64, 192);
			this->destination_getSystemVersion_SystemTab_Label->Name = L"destination_getSystemVersion_SystemTab_Label";
			this->destination_getSystemVersion_SystemTab_Label->Size = System::Drawing::Size(83, 17);
			this->destination_getSystemVersion_SystemTab_Label->TabIndex = 11;
			this->destination_getSystemVersion_SystemTab_Label->Text = L"Destination:";
			// 
			// destination_getSystemVersion_SystemTab_TextBox
			// 
			this->destination_getSystemVersion_SystemTab_TextBox->Location = System::Drawing::Point(150, 192);
			this->destination_getSystemVersion_SystemTab_TextBox->Name = L"destination_getSystemVersion_SystemTab_TextBox";
			this->destination_getSystemVersion_SystemTab_TextBox->Size = System::Drawing::Size(57, 20);
			this->destination_getSystemVersion_SystemTab_TextBox->TabIndex = 10;
			// 
			// systemGetVersion_systemGetVersion_SystemTab_Button
			// 
			this->systemGetVersion_systemGetVersion_SystemTab_Button->AutoSize = true;
			this->systemGetVersion_systemGetVersion_SystemTab_Button->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
			this->systemGetVersion_systemGetVersion_SystemTab_Button->Location = System::Drawing::Point(260, 250);
			this->systemGetVersion_systemGetVersion_SystemTab_Button->Name = L"systemGetVersion_systemGetVersion_SystemTab_Button";
			this->systemGetVersion_systemGetVersion_SystemTab_Button->Size = System::Drawing::Size(109, 28);
			this->systemGetVersion_systemGetVersion_SystemTab_Button->TabIndex = 9;
			this->systemGetVersion_systemGetVersion_SystemTab_Button->Text = L"Get System Version";
			this->systemGetVersion_systemGetVersion_SystemTab_Button->UseVisualStyleBackColor = true;
			this->systemGetVersion_systemGetVersion_SystemTab_Button->Click += gcnew System::EventHandler(this, &GUI::systemGetVersion_SystemTab_Button_Click);
			// 
			// systemGetVersion_SystemTab_TextBox
			// 
			this->systemGetVersion_SystemTab_TextBox->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->systemGetVersion_SystemTab_TextBox->Location = System::Drawing::Point(6, 6);
			this->systemGetVersion_SystemTab_TextBox->Multiline = true;
			this->systemGetVersion_SystemTab_TextBox->Name = L"systemGetVersion_SystemTab_TextBox";
			this->systemGetVersion_SystemTab_TextBox->ReadOnly = true;
			this->systemGetVersion_SystemTab_TextBox->Size = System::Drawing::Size(617, 91);
			this->systemGetVersion_SystemTab_TextBox->TabIndex = 0;
			this->systemGetVersion_SystemTab_TextBox->Text = L"Get Wistone\'s System Version:\r\n\r\n^ Choose \"Destination\" ( Network ID of Wistone o" 
				L"r 0 for Broadcast )\r\n^ Press \"Get System Version\" to get SW version ID & PSU ser" 
				L"ial number";
			// 
			// systemSetId_SystemTab_TabPage
			// 
			this->systemSetId_SystemTab_TabPage->BackColor = System::Drawing::Color::WhiteSmoke;
			this->systemSetId_SystemTab_TabPage->Controls->Add(this->destination_setSystemId_SystemTab_Label);
			this->systemSetId_SystemTab_TabPage->Controls->Add(this->destination_setSystemId_SystemTab_TextBox);
			this->systemSetId_SystemTab_TabPage->Controls->Add(this->systemSetId_SystemTab_TextBox);
			this->systemSetId_SystemTab_TabPage->Controls->Add(this->systemSetId_systemSetId_SystemTab_Button);
			this->systemSetId_SystemTab_TabPage->Controls->Add(this->systemId_systemSetId_SystemTab_Label);
			this->systemSetId_SystemTab_TabPage->Controls->Add(this->systemId_systemSetId_SystemTab_TextBox);
			this->systemSetId_SystemTab_TabPage->Location = System::Drawing::Point(4, 22);
			this->systemSetId_SystemTab_TabPage->Name = L"systemSetId_SystemTab_TabPage";
			this->systemSetId_SystemTab_TabPage->Padding = System::Windows::Forms::Padding(3);
			this->systemSetId_SystemTab_TabPage->Size = System::Drawing::Size(632, 319);
			this->systemSetId_SystemTab_TabPage->TabIndex = 4;
			this->systemSetId_SystemTab_TabPage->Text = L"Set System ID";
			// 
			// destination_setSystemId_SystemTab_Label
			// 
			this->destination_setSystemId_SystemTab_Label->AutoSize = true;
			this->destination_setSystemId_SystemTab_Label->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(177)));
			this->destination_setSystemId_SystemTab_Label->Location = System::Drawing::Point(64, 192);
			this->destination_setSystemId_SystemTab_Label->Name = L"destination_setSystemId_SystemTab_Label";
			this->destination_setSystemId_SystemTab_Label->Size = System::Drawing::Size(83, 17);
			this->destination_setSystemId_SystemTab_Label->TabIndex = 17;
			this->destination_setSystemId_SystemTab_Label->Text = L"Destination:";
			// 
			// destination_setSystemId_SystemTab_TextBox
			// 
			this->destination_setSystemId_SystemTab_TextBox->Location = System::Drawing::Point(150, 192);
			this->destination_setSystemId_SystemTab_TextBox->Name = L"destination_setSystemId_SystemTab_TextBox";
			this->destination_setSystemId_SystemTab_TextBox->Size = System::Drawing::Size(57, 20);
			this->destination_setSystemId_SystemTab_TextBox->TabIndex = 16;
			// 
			// systemSetId_SystemTab_TextBox
			// 
			this->systemSetId_SystemTab_TextBox->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->systemSetId_SystemTab_TextBox->Location = System::Drawing::Point(6, 6);
			this->systemSetId_SystemTab_TextBox->Multiline = true;
			this->systemSetId_SystemTab_TextBox->Name = L"systemSetId_SystemTab_TextBox";
			this->systemSetId_SystemTab_TextBox->ReadOnly = true;
			this->systemSetId_SystemTab_TextBox->Size = System::Drawing::Size(617, 97);
			this->systemSetId_SystemTab_TextBox->TabIndex = 15;
			this->systemSetId_SystemTab_TextBox->Text = L"Set Wistone\'s System ID:\r\n\r\n^ Choose \"Destination\" ( Network ID of Wistone or 0 f" 
				L"or Broadcast )\r\n^ Enter: \"System ID\" ( HW SN, Valid Range: 0 - 100 )\r\n^ Press \"S" 
				L"et SystemID\"";
			// 
			// systemSetId_systemSetId_SystemTab_Button
			// 
			this->systemSetId_systemSetId_SystemTab_Button->AutoSize = true;
			this->systemSetId_systemSetId_SystemTab_Button->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
			this->systemSetId_systemSetId_SystemTab_Button->Location = System::Drawing::Point(260, 250);
			this->systemSetId_systemSetId_SystemTab_Button->Name = L"systemSetId_systemSetId_SystemTab_Button";
			this->systemSetId_systemSetId_SystemTab_Button->Size = System::Drawing::Size(109, 28);
			this->systemSetId_systemSetId_SystemTab_Button->TabIndex = 14;
			this->systemSetId_systemSetId_SystemTab_Button->Text = L"Set System ID";
			this->systemSetId_systemSetId_SystemTab_Button->UseVisualStyleBackColor = true;
			this->systemSetId_systemSetId_SystemTab_Button->Click += gcnew System::EventHandler(this, &GUI::systemSetId_SystemTab_Button_Click);
			// 
			// systemId_systemSetId_SystemTab_Label
			// 
			this->systemId_systemSetId_SystemTab_Label->AutoSize = true;
			this->systemId_systemSetId_SystemTab_Label->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->systemId_systemSetId_SystemTab_Label->Location = System::Drawing::Point(64, 221);
			this->systemId_systemSetId_SystemTab_Label->Name = L"systemId_systemSetId_SystemTab_Label";
			this->systemId_systemSetId_SystemTab_Label->Size = System::Drawing::Size(75, 17);
			this->systemId_systemSetId_SystemTab_Label->TabIndex = 13;
			this->systemId_systemSetId_SystemTab_Label->Text = L"System ID:";
			// 
			// systemId_systemSetId_SystemTab_TextBox
			// 
			this->systemId_systemSetId_SystemTab_TextBox->Location = System::Drawing::Point(150, 221);
			this->systemId_systemSetId_SystemTab_TextBox->Multiline = true;
			this->systemId_systemSetId_SystemTab_TextBox->Name = L"systemId_systemSetId_SystemTab_TextBox";
			this->systemId_systemSetId_SystemTab_TextBox->Size = System::Drawing::Size(57, 20);
			this->systemId_systemSetId_SystemTab_TextBox->TabIndex = 12;
			// 
			// getPower_SystemTab_TabPage
			// 
			this->getPower_SystemTab_TabPage->BackColor = System::Drawing::Color::WhiteSmoke;
			this->getPower_SystemTab_TabPage->Controls->Add(this->destination_getPower_SystemTab_Label);
			this->getPower_SystemTab_TabPage->Controls->Add(this->destination_getPower_SystemTab_TextBox);
			this->getPower_SystemTab_TabPage->Controls->Add(this->getPower_getPower_SystemTab_Button);
			this->getPower_SystemTab_TabPage->Controls->Add(this->getPower_SystemTab_TextBox);
			this->getPower_SystemTab_TabPage->Location = System::Drawing::Point(4, 22);
			this->getPower_SystemTab_TabPage->Name = L"getPower_SystemTab_TabPage";
			this->getPower_SystemTab_TabPage->Padding = System::Windows::Forms::Padding(3);
			this->getPower_SystemTab_TabPage->Size = System::Drawing::Size(632, 319);
			this->getPower_SystemTab_TabPage->TabIndex = 3;
			this->getPower_SystemTab_TabPage->Text = L"Get Power";
			// 
			// destination_getPower_SystemTab_Label
			// 
			this->destination_getPower_SystemTab_Label->AutoSize = true;
			this->destination_getPower_SystemTab_Label->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(177)));
			this->destination_getPower_SystemTab_Label->Location = System::Drawing::Point(64, 192);
			this->destination_getPower_SystemTab_Label->Name = L"destination_getPower_SystemTab_Label";
			this->destination_getPower_SystemTab_Label->Size = System::Drawing::Size(83, 17);
			this->destination_getPower_SystemTab_Label->TabIndex = 17;
			this->destination_getPower_SystemTab_Label->Text = L"Destination:";
			// 
			// destination_getPower_SystemTab_TextBox
			// 
			this->destination_getPower_SystemTab_TextBox->Location = System::Drawing::Point(150, 192);
			this->destination_getPower_SystemTab_TextBox->Name = L"destination_getPower_SystemTab_TextBox";
			this->destination_getPower_SystemTab_TextBox->Size = System::Drawing::Size(57, 20);
			this->destination_getPower_SystemTab_TextBox->TabIndex = 16;
			// 
			// getPower_getPower_SystemTab_Button
			// 
			this->getPower_getPower_SystemTab_Button->AutoSize = true;
			this->getPower_getPower_SystemTab_Button->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
			this->getPower_getPower_SystemTab_Button->Location = System::Drawing::Point(260, 250);
			this->getPower_getPower_SystemTab_Button->Name = L"getPower_getPower_SystemTab_Button";
			this->getPower_getPower_SystemTab_Button->Size = System::Drawing::Size(109, 28);
			this->getPower_getPower_SystemTab_Button->TabIndex = 15;
			this->getPower_getPower_SystemTab_Button->Text = L"Get Power";
			this->getPower_getPower_SystemTab_Button->UseVisualStyleBackColor = true;
			this->getPower_getPower_SystemTab_Button->Click += gcnew System::EventHandler(this, &GUI::getPower_SystemTab_Button_Click);
			// 
			// getPower_SystemTab_TextBox
			// 
			this->getPower_SystemTab_TextBox->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->getPower_SystemTab_TextBox->Location = System::Drawing::Point(6, 6);
			this->getPower_SystemTab_TextBox->Multiline = true;
			this->getPower_SystemTab_TextBox->Name = L"getPower_SystemTab_TextBox";
			this->getPower_SystemTab_TextBox->ReadOnly = true;
			this->getPower_SystemTab_TextBox->Size = System::Drawing::Size(617, 92);
			this->getPower_SystemTab_TextBox->TabIndex = 1;
			this->getPower_SystemTab_TextBox->Text = L"Get Wistone\'s Power Status:\r\n\r\n^ Choose \"Destination\" ( Network ID of Wistone or " 
				L"0 for Broadcast )\r\n^ Press \"Get Power\" to get the status of Wistone\'s power sour" 
				L"ces\r\n";
			// 
			// setPower_SystemTab_TabPage
			// 
			this->setPower_SystemTab_TabPage->BackColor = System::Drawing::Color::WhiteSmoke;
			this->setPower_SystemTab_TabPage->Controls->Add(this->destination_setPower_SystemTab_Label);
			this->setPower_SystemTab_TabPage->Controls->Add(this->destination_setPower_SystemTab_TextBox);
			this->setPower_SystemTab_TabPage->Controls->Add(this->enableM12V_setPower_SystemTab_Label);
			this->setPower_SystemTab_TabPage->Controls->Add(this->enable12V_setPower_SystemTab_Label);
			this->setPower_SystemTab_TabPage->Controls->Add(this->enable5V_setPower_SystemTab_Label);
			this->setPower_SystemTab_TabPage->Controls->Add(this->enableM12V_setPower_SystemTab_ComboBox);
			this->setPower_SystemTab_TabPage->Controls->Add(this->enable12V_setPower_SystemTab_ComboBox);
			this->setPower_SystemTab_TabPage->Controls->Add(this->enable5V_setPower_SystemTab_ComboBox);
			this->setPower_SystemTab_TabPage->Controls->Add(this->setPower_setPower_SystemTab_Button);
			this->setPower_SystemTab_TabPage->Controls->Add(this->setPower_SystemTab_TextBox);
			this->setPower_SystemTab_TabPage->Location = System::Drawing::Point(4, 22);
			this->setPower_SystemTab_TabPage->Name = L"setPower_SystemTab_TabPage";
			this->setPower_SystemTab_TabPage->Padding = System::Windows::Forms::Padding(3);
			this->setPower_SystemTab_TabPage->Size = System::Drawing::Size(632, 319);
			this->setPower_SystemTab_TabPage->TabIndex = 6;
			this->setPower_SystemTab_TabPage->Text = L"Set Power";
			// 
			// destination_setPower_SystemTab_Label
			// 
			this->destination_setPower_SystemTab_Label->AutoSize = true;
			this->destination_setPower_SystemTab_Label->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(177)));
			this->destination_setPower_SystemTab_Label->Location = System::Drawing::Point(65, 129);
			this->destination_setPower_SystemTab_Label->Name = L"destination_setPower_SystemTab_Label";
			this->destination_setPower_SystemTab_Label->Size = System::Drawing::Size(83, 17);
			this->destination_setPower_SystemTab_Label->TabIndex = 33;
			this->destination_setPower_SystemTab_Label->Text = L"Destination:";
			// 
			// destination_setPower_SystemTab_TextBox
			// 
			this->destination_setPower_SystemTab_TextBox->Location = System::Drawing::Point(164, 127);
			this->destination_setPower_SystemTab_TextBox->Name = L"destination_setPower_SystemTab_TextBox";
			this->destination_setPower_SystemTab_TextBox->Size = System::Drawing::Size(45, 20);
			this->destination_setPower_SystemTab_TextBox->TabIndex = 32;
			// 
			// enableM12V_setPower_SystemTab_Label
			// 
			this->enableM12V_setPower_SystemTab_Label->AutoSize = true;
			this->enableM12V_setPower_SystemTab_Label->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->enableM12V_setPower_SystemTab_Label->Location = System::Drawing::Point(60, 228);
			this->enableM12V_setPower_SystemTab_Label->Name = L"enableM12V_setPower_SystemTab_Label";
			this->enableM12V_setPower_SystemTab_Label->Size = System::Drawing::Size(90, 17);
			this->enableM12V_setPower_SystemTab_Label->TabIndex = 31;
			this->enableM12V_setPower_SystemTab_Label->Text = L"-12V Enable:";
			// 
			// enable12V_setPower_SystemTab_Label
			// 
			this->enable12V_setPower_SystemTab_Label->AutoSize = true;
			this->enable12V_setPower_SystemTab_Label->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->enable12V_setPower_SystemTab_Label->Location = System::Drawing::Point(64, 193);
			this->enable12V_setPower_SystemTab_Label->Name = L"enable12V_setPower_SystemTab_Label";
			this->enable12V_setPower_SystemTab_Label->Size = System::Drawing::Size(85, 17);
			this->enable12V_setPower_SystemTab_Label->TabIndex = 30;
			this->enable12V_setPower_SystemTab_Label->Text = L"12V Enable:";
			// 
			// enable5V_setPower_SystemTab_Label
			// 
			this->enable5V_setPower_SystemTab_Label->AutoSize = true;
			this->enable5V_setPower_SystemTab_Label->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->enable5V_setPower_SystemTab_Label->Location = System::Drawing::Point(64, 158);
			this->enable5V_setPower_SystemTab_Label->Name = L"enable5V_setPower_SystemTab_Label";
			this->enable5V_setPower_SystemTab_Label->Size = System::Drawing::Size(85, 17);
			this->enable5V_setPower_SystemTab_Label->TabIndex = 29;
			this->enable5V_setPower_SystemTab_Label->Text = L"5V   Enable:";
			// 
			// enableM12V_setPower_SystemTab_ComboBox
			// 
			this->enableM12V_setPower_SystemTab_ComboBox->FormattingEnabled = true;
			this->enableM12V_setPower_SystemTab_ComboBox->Items->AddRange(gcnew cli::array< System::Object^  >(2) {L"0", L"1"});
			this->enableM12V_setPower_SystemTab_ComboBox->Location = System::Drawing::Point(164, 228);
			this->enableM12V_setPower_SystemTab_ComboBox->Name = L"enableM12V_setPower_SystemTab_ComboBox";
			this->enableM12V_setPower_SystemTab_ComboBox->Size = System::Drawing::Size(45, 21);
			this->enableM12V_setPower_SystemTab_ComboBox->TabIndex = 25;
			// 
			// enable12V_setPower_SystemTab_ComboBox
			// 
			this->enable12V_setPower_SystemTab_ComboBox->FormattingEnabled = true;
			this->enable12V_setPower_SystemTab_ComboBox->Items->AddRange(gcnew cli::array< System::Object^  >(2) {L"0", L"1"});
			this->enable12V_setPower_SystemTab_ComboBox->Location = System::Drawing::Point(164, 194);
			this->enable12V_setPower_SystemTab_ComboBox->Name = L"enable12V_setPower_SystemTab_ComboBox";
			this->enable12V_setPower_SystemTab_ComboBox->Size = System::Drawing::Size(45, 21);
			this->enable12V_setPower_SystemTab_ComboBox->TabIndex = 24;
			// 
			// enable5V_setPower_SystemTab_ComboBox
			// 
			this->enable5V_setPower_SystemTab_ComboBox->FormattingEnabled = true;
			this->enable5V_setPower_SystemTab_ComboBox->Items->AddRange(gcnew cli::array< System::Object^  >(2) {L"0", L"1"});
			this->enable5V_setPower_SystemTab_ComboBox->Location = System::Drawing::Point(164, 160);
			this->enable5V_setPower_SystemTab_ComboBox->Name = L"enable5V_setPower_SystemTab_ComboBox";
			this->enable5V_setPower_SystemTab_ComboBox->Size = System::Drawing::Size(45, 21);
			this->enable5V_setPower_SystemTab_ComboBox->TabIndex = 23;
			// 
			// setPower_setPower_SystemTab_Button
			// 
			this->setPower_setPower_SystemTab_Button->AutoSize = true;
			this->setPower_setPower_SystemTab_Button->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
			this->setPower_setPower_SystemTab_Button->Location = System::Drawing::Point(260, 250);
			this->setPower_setPower_SystemTab_Button->Name = L"setPower_setPower_SystemTab_Button";
			this->setPower_setPower_SystemTab_Button->Size = System::Drawing::Size(109, 28);
			this->setPower_setPower_SystemTab_Button->TabIndex = 17;
			this->setPower_setPower_SystemTab_Button->Text = L"Set Power";
			this->setPower_setPower_SystemTab_Button->UseVisualStyleBackColor = true;
			this->setPower_setPower_SystemTab_Button->Click += gcnew System::EventHandler(this, &GUI::setPower_SystemTab_Button_Click);
			// 
			// setPower_SystemTab_TextBox
			// 
			this->setPower_SystemTab_TextBox->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->setPower_SystemTab_TextBox->Location = System::Drawing::Point(6, 6);
			this->setPower_SystemTab_TextBox->Multiline = true;
			this->setPower_SystemTab_TextBox->Name = L"setPower_SystemTab_TextBox";
			this->setPower_SystemTab_TextBox->ReadOnly = true;
			this->setPower_SystemTab_TextBox->Size = System::Drawing::Size(617, 104);
			this->setPower_SystemTab_TextBox->TabIndex = 16;
			this->setPower_SystemTab_TextBox->Text = L"Set Wistone\'s Power:\r\n\r\n^ Choose \"Destination\" ( Network ID of Wistone or 0 for B" 
				L"roadcast )\r\n^ Enable requred power sources\r\n^ Press \"Set Power\"";
			// 
			// getLastError_SystemTab_TabPage
			// 
			this->getLastError_SystemTab_TabPage->BackColor = System::Drawing::Color::WhiteSmoke;
			this->getLastError_SystemTab_TabPage->Controls->Add(this->destination_getLastError_SystemTab_Label);
			this->getLastError_SystemTab_TabPage->Controls->Add(this->destination_getLastError_SystemTab_TextBox);
			this->getLastError_SystemTab_TabPage->Controls->Add(this->getLastError_SystemTab_TextBox);
			this->getLastError_SystemTab_TabPage->Controls->Add(this->getLastError_getLastError_SystemTab_Button);
			this->getLastError_SystemTab_TabPage->Location = System::Drawing::Point(4, 22);
			this->getLastError_SystemTab_TabPage->Name = L"getLastError_SystemTab_TabPage";
			this->getLastError_SystemTab_TabPage->Padding = System::Windows::Forms::Padding(3);
			this->getLastError_SystemTab_TabPage->Size = System::Drawing::Size(632, 319);
			this->getLastError_SystemTab_TabPage->TabIndex = 5;
			this->getLastError_SystemTab_TabPage->Text = L"Get Last Error";
			// 
			// destination_getLastError_SystemTab_Label
			// 
			this->destination_getLastError_SystemTab_Label->AutoSize = true;
			this->destination_getLastError_SystemTab_Label->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(177)));
			this->destination_getLastError_SystemTab_Label->Location = System::Drawing::Point(64, 192);
			this->destination_getLastError_SystemTab_Label->Name = L"destination_getLastError_SystemTab_Label";
			this->destination_getLastError_SystemTab_Label->Size = System::Drawing::Size(83, 17);
			this->destination_getLastError_SystemTab_Label->TabIndex = 19;
			this->destination_getLastError_SystemTab_Label->Text = L"Destination:";
			// 
			// destination_getLastError_SystemTab_TextBox
			// 
			this->destination_getLastError_SystemTab_TextBox->Location = System::Drawing::Point(150, 192);
			this->destination_getLastError_SystemTab_TextBox->Name = L"destination_getLastError_SystemTab_TextBox";
			this->destination_getLastError_SystemTab_TextBox->Size = System::Drawing::Size(57, 20);
			this->destination_getLastError_SystemTab_TextBox->TabIndex = 18;
			// 
			// getLastError_SystemTab_TextBox
			// 
			this->getLastError_SystemTab_TextBox->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->getLastError_SystemTab_TextBox->Location = System::Drawing::Point(1, 3);
			this->getLastError_SystemTab_TextBox->Multiline = true;
			this->getLastError_SystemTab_TextBox->Name = L"getLastError_SystemTab_TextBox";
			this->getLastError_SystemTab_TextBox->ReadOnly = true;
			this->getLastError_SystemTab_TextBox->Size = System::Drawing::Size(622, 78);
			this->getLastError_SystemTab_TextBox->TabIndex = 17;
			this->getLastError_SystemTab_TextBox->Text = L"Get Wistone\'s Last Error:\r\n\r\n^ Choose \"Destination\" ( Network ID of Wistone or 0 " 
				L"for Broadcast )\r\n^ Press \"Get Last Error\" to get last program error";
			// 
			// getLastError_getLastError_SystemTab_Button
			// 
			this->getLastError_getLastError_SystemTab_Button->AutoSize = true;
			this->getLastError_getLastError_SystemTab_Button->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
			this->getLastError_getLastError_SystemTab_Button->Location = System::Drawing::Point(260, 250);
			this->getLastError_getLastError_SystemTab_Button->Name = L"getLastError_getLastError_SystemTab_Button";
			this->getLastError_getLastError_SystemTab_Button->Size = System::Drawing::Size(109, 28);
			this->getLastError_getLastError_SystemTab_Button->TabIndex = 16;
			this->getLastError_getLastError_SystemTab_Button->Text = L"Get Last Error";
			this->getLastError_getLastError_SystemTab_Button->UseVisualStyleBackColor = true;
			this->getLastError_getLastError_SystemTab_Button->Click += gcnew System::EventHandler(this, &GUI::getLastError_SystemTab_Button_Click);
			// 
			// tabHelp
			// 
			this->tabHelp->BackColor = System::Drawing::Color::WhiteSmoke;
			this->tabHelp->Controls->Add(this->helpTab);
			this->tabHelp->Location = System::Drawing::Point(4, 22);
			this->tabHelp->Name = L"tabHelp";
			this->tabHelp->Padding = System::Windows::Forms::Padding(3);
			this->tabHelp->Size = System::Drawing::Size(633, 339);
			this->tabHelp->TabIndex = 10;
			this->tabHelp->Text = L"Help";
			// 
			// helpTab
			// 
			this->helpTab->Controls->Add(this->introduction_HelpTab_TabPage);
			this->helpTab->Controls->Add(this->instruction_HelpTab_TabPage);
			this->helpTab->Location = System::Drawing::Point(0, 0);
			this->helpTab->Name = L"helpTab";
			this->helpTab->SelectedIndex = 0;
			this->helpTab->Size = System::Drawing::Size(641, 343);
			this->helpTab->TabIndex = 0;
			// 
			// introduction_HelpTab_TabPage
			// 
			this->introduction_HelpTab_TabPage->BackColor = System::Drawing::Color::WhiteSmoke;
			this->introduction_HelpTab_TabPage->Controls->Add(this->introduction_HelpTab_TextBox);
			this->introduction_HelpTab_TabPage->Location = System::Drawing::Point(4, 22);
			this->introduction_HelpTab_TabPage->Name = L"introduction_HelpTab_TabPage";
			this->introduction_HelpTab_TabPage->Padding = System::Windows::Forms::Padding(3);
			this->introduction_HelpTab_TabPage->Size = System::Drawing::Size(633, 317);
			this->introduction_HelpTab_TabPage->TabIndex = 0;
			this->introduction_HelpTab_TabPage->Text = L"Introduction";
			// 
			// introduction_HelpTab_TextBox
			// 
			this->introduction_HelpTab_TextBox->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->introduction_HelpTab_TextBox->Location = System::Drawing::Point(6, 6);
			this->introduction_HelpTab_TextBox->Multiline = true;
			this->introduction_HelpTab_TextBox->Name = L"introduction_HelpTab_TextBox";
			this->introduction_HelpTab_TextBox->ReadOnly = true;
			this->introduction_HelpTab_TextBox->Size = System::Drawing::Size(622, 308);
			this->introduction_HelpTab_TextBox->TabIndex = 1;
			// 
			// instruction_HelpTab_TabPage
			// 
			this->instruction_HelpTab_TabPage->BackColor = System::Drawing::Color::WhiteSmoke;
			this->instruction_HelpTab_TabPage->Controls->Add(this->instruction_HelpTab_TextBox);
			this->instruction_HelpTab_TabPage->Location = System::Drawing::Point(4, 22);
			this->instruction_HelpTab_TabPage->Name = L"instruction_HelpTab_TabPage";
			this->instruction_HelpTab_TabPage->Padding = System::Windows::Forms::Padding(3);
			this->instruction_HelpTab_TabPage->Size = System::Drawing::Size(633, 317);
			this->instruction_HelpTab_TabPage->TabIndex = 1;
			this->instruction_HelpTab_TabPage->Text = L"Instruction";
			// 
			// instruction_HelpTab_TextBox
			// 
			this->instruction_HelpTab_TextBox->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->instruction_HelpTab_TextBox->Location = System::Drawing::Point(6, 6);
			this->instruction_HelpTab_TextBox->Multiline = true;
			this->instruction_HelpTab_TextBox->Name = L"instruction_HelpTab_TextBox";
			this->instruction_HelpTab_TextBox->ReadOnly = true;
			this->instruction_HelpTab_TextBox->Size = System::Drawing::Size(624, 290);
			this->instruction_HelpTab_TextBox->TabIndex = 1;
			// 
			// allTabs
			// 
			this->allTabs->Controls->Add(this->tabApplication);
			this->allTabs->Controls->Add(this->tabWakeUp);
			this->allTabs->Controls->Add(this->tabSystem);
			this->allTabs->Controls->Add(this->tabFlash);
			this->allTabs->Controls->Add(this->tabEEPROM);
			this->allTabs->Controls->Add(this->tabLCD);
			this->allTabs->Controls->Add(this->tabTemperature);
			this->allTabs->Controls->Add(this->tabAccelerometer);
			this->allTabs->Controls->Add(this->tabLed);
			this->allTabs->Controls->Add(this->tabBuzzer);
			this->allTabs->Controls->Add(this->tabSwitch);
			this->allTabs->Controls->Add(this->tabADC);
			this->allTabs->Controls->Add(this->tabHelp);
			this->allTabs->Controls->Add(this->tabHub);
			this->allTabs->Enabled = false;
			this->allTabs->ImeMode = System::Windows::Forms::ImeMode::On;
			this->allTabs->Location = System::Drawing::Point(24, 106);
			this->allTabs->Name = L"allTabs";
			this->allTabs->SelectedIndex = 0;
			this->allTabs->Size = System::Drawing::Size(641, 365);
			this->allTabs->TabIndex = 0;
			// 
			// tabEEPROM
			// 
			this->tabEEPROM->BackColor = System::Drawing::Color::WhiteSmoke;
			this->tabEEPROM->Controls->Add(this->eepromTab);
			this->tabEEPROM->Location = System::Drawing::Point(4, 22);
			this->tabEEPROM->Name = L"tabEEPROM";
			this->tabEEPROM->Padding = System::Windows::Forms::Padding(3);
			this->tabEEPROM->Size = System::Drawing::Size(633, 339);
			this->tabEEPROM->TabIndex = 3;
			this->tabEEPROM->Text = L"EEPROM";
			// 
			// eepromTab
			// 
			this->eepromTab->Controls->Add(this->format_EepromTab_TabPage);
			this->eepromTab->Controls->Add(this->read_EepromTab_TabPage);
			this->eepromTab->Controls->Add(this->write_EepromTab_TabPage);
			this->eepromTab->Controls->Add(this->setBoot_EepromTab_TabPage);
			this->eepromTab->Controls->Add(this->getBoot_EepromTab_TabPage);
			this->eepromTab->Controls->Add(this->readLL_EepromTab_TabPage);
			this->eepromTab->Controls->Add(this->writeLL_EepromTab_TabPage);
			this->eepromTab->Location = System::Drawing::Point(0, 0);
			this->eepromTab->Name = L"eepromTab";
			this->eepromTab->SelectedIndex = 0;
			this->eepromTab->Size = System::Drawing::Size(640, 345);
			this->eepromTab->TabIndex = 13;
			// 
			// format_EepromTab_TabPage
			// 
			this->format_EepromTab_TabPage->BackColor = System::Drawing::Color::WhiteSmoke;
			this->format_EepromTab_TabPage->Controls->Add(this->destination_format_EepromTab_Label);
			this->format_EepromTab_TabPage->Controls->Add(this->destination_format_EepromTab_TextBox);
			this->format_EepromTab_TabPage->Controls->Add(this->format_format_EepromTab_Button);
			this->format_EepromTab_TabPage->Controls->Add(this->format_EepromTab_TextBox);
			this->format_EepromTab_TabPage->Location = System::Drawing::Point(4, 22);
			this->format_EepromTab_TabPage->Name = L"format_EepromTab_TabPage";
			this->format_EepromTab_TabPage->Padding = System::Windows::Forms::Padding(3);
			this->format_EepromTab_TabPage->Size = System::Drawing::Size(632, 319);
			this->format_EepromTab_TabPage->TabIndex = 2;
			this->format_EepromTab_TabPage->Text = L"Format";
			// 
			// destination_format_EepromTab_Label
			// 
			this->destination_format_EepromTab_Label->AutoSize = true;
			this->destination_format_EepromTab_Label->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(177)));
			this->destination_format_EepromTab_Label->Location = System::Drawing::Point(64, 192);
			this->destination_format_EepromTab_Label->Name = L"destination_format_EepromTab_Label";
			this->destination_format_EepromTab_Label->Size = System::Drawing::Size(83, 17);
			this->destination_format_EepromTab_Label->TabIndex = 13;
			this->destination_format_EepromTab_Label->Text = L"Destination:";
			// 
			// destination_format_EepromTab_TextBox
			// 
			this->destination_format_EepromTab_TextBox->Location = System::Drawing::Point(150, 192);
			this->destination_format_EepromTab_TextBox->Name = L"destination_format_EepromTab_TextBox";
			this->destination_format_EepromTab_TextBox->Size = System::Drawing::Size(57, 20);
			this->destination_format_EepromTab_TextBox->TabIndex = 12;
			// 
			// format_format_EepromTab_Button
			// 
			this->format_format_EepromTab_Button->AutoSize = true;
			this->format_format_EepromTab_Button->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
			this->format_format_EepromTab_Button->Location = System::Drawing::Point(276, 250);
			this->format_format_EepromTab_Button->Name = L"format_format_EepromTab_Button";
			this->format_format_EepromTab_Button->Size = System::Drawing::Size(80, 28);
			this->format_format_EepromTab_Button->TabIndex = 11;
			this->format_format_EepromTab_Button->Text = L"Format";
			this->format_format_EepromTab_Button->UseVisualStyleBackColor = true;
			this->format_format_EepromTab_Button->Click += gcnew System::EventHandler(this, &GUI::format_EepromTab_Button_Click);
			// 
			// format_EepromTab_TextBox
			// 
			this->format_EepromTab_TextBox->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->format_EepromTab_TextBox->Location = System::Drawing::Point(6, 4);
			this->format_EepromTab_TextBox->Multiline = true;
			this->format_EepromTab_TextBox->Name = L"format_EepromTab_TextBox";
			this->format_EepromTab_TextBox->ReadOnly = true;
			this->format_EepromTab_TextBox->Size = System::Drawing::Size(621, 98);
			this->format_EepromTab_TextBox->TabIndex = 0;
			this->format_EepromTab_TextBox->Text = L"Format Wistone\'s EEPROM:\r\n\r\n^ Choose \"Destination\" ( Network ID of Wistone or 0 f" 
				L"or Broadcast )\r\n^ Press \"Format\" to entirely clear EEPROM\'s memory";
			// 
			// read_EepromTab_TabPage
			// 
			this->read_EepromTab_TabPage->BackColor = System::Drawing::Color::WhiteSmoke;
			this->read_EepromTab_TabPage->Controls->Add(this->destination_read_EepromTab_Label);
			this->read_EepromTab_TabPage->Controls->Add(this->destination_read_EepromTab_TextBox);
			this->read_EepromTab_TabPage->Controls->Add(this->numberOfBytes_read_EepromTab_Label);
			this->read_EepromTab_TabPage->Controls->Add(this->numberOfBytes_read_EepromTab_TextBox);
			this->read_EepromTab_TabPage->Controls->Add(this->read_read_EepromTab_Button);
			this->read_EepromTab_TabPage->Controls->Add(this->address_read_EepromTab_Label);
			this->read_EepromTab_TabPage->Controls->Add(this->address_read_EepromTab_TextBox);
			this->read_EepromTab_TabPage->Controls->Add(this->read_EepromTab_TextBox);
			this->read_EepromTab_TabPage->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(177)));
			this->read_EepromTab_TabPage->Location = System::Drawing::Point(4, 22);
			this->read_EepromTab_TabPage->Name = L"read_EepromTab_TabPage";
			this->read_EepromTab_TabPage->Padding = System::Windows::Forms::Padding(3);
			this->read_EepromTab_TabPage->Size = System::Drawing::Size(632, 319);
			this->read_EepromTab_TabPage->TabIndex = 3;
			this->read_EepromTab_TabPage->Text = L"Read";
			// 
			// destination_read_EepromTab_Label
			// 
			this->destination_read_EepromTab_Label->AutoSize = true;
			this->destination_read_EepromTab_Label->Location = System::Drawing::Point(46, 161);
			this->destination_read_EepromTab_Label->Name = L"destination_read_EepromTab_Label";
			this->destination_read_EepromTab_Label->Size = System::Drawing::Size(83, 17);
			this->destination_read_EepromTab_Label->TabIndex = 19;
			this->destination_read_EepromTab_Label->Text = L"Destination:";
			// 
			// destination_read_EepromTab_TextBox
			// 
			this->destination_read_EepromTab_TextBox->Location = System::Drawing::Point(172, 159);
			this->destination_read_EepromTab_TextBox->Name = L"destination_read_EepromTab_TextBox";
			this->destination_read_EepromTab_TextBox->Size = System::Drawing::Size(71, 23);
			this->destination_read_EepromTab_TextBox->TabIndex = 18;
			// 
			// numberOfBytes_read_EepromTab_Label
			// 
			this->numberOfBytes_read_EepromTab_Label->AutoSize = true;
			this->numberOfBytes_read_EepromTab_Label->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->numberOfBytes_read_EepromTab_Label->Location = System::Drawing::Point(46, 220);
			this->numberOfBytes_read_EepromTab_Label->Name = L"numberOfBytes_read_EepromTab_Label";
			this->numberOfBytes_read_EepromTab_Label->Size = System::Drawing::Size(120, 17);
			this->numberOfBytes_read_EepromTab_Label->TabIndex = 17;
			this->numberOfBytes_read_EepromTab_Label->Text = L"Number Of Bytes:";
			// 
			// numberOfBytes_read_EepromTab_TextBox
			// 
			this->numberOfBytes_read_EepromTab_TextBox->Location = System::Drawing::Point(172, 220);
			this->numberOfBytes_read_EepromTab_TextBox->Name = L"numberOfBytes_read_EepromTab_TextBox";
			this->numberOfBytes_read_EepromTab_TextBox->Size = System::Drawing::Size(71, 23);
			this->numberOfBytes_read_EepromTab_TextBox->TabIndex = 16;
			// 
			// read_read_EepromTab_Button
			// 
			this->read_read_EepromTab_Button->AutoSize = true;
			this->read_read_EepromTab_Button->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
			this->read_read_EepromTab_Button->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F));
			this->read_read_EepromTab_Button->Location = System::Drawing::Point(276, 250);
			this->read_read_EepromTab_Button->Name = L"read_read_EepromTab_Button";
			this->read_read_EepromTab_Button->Size = System::Drawing::Size(80, 28);
			this->read_read_EepromTab_Button->TabIndex = 15;
			this->read_read_EepromTab_Button->Text = L"Read";
			this->read_read_EepromTab_Button->UseVisualStyleBackColor = true;
			this->read_read_EepromTab_Button->Click += gcnew System::EventHandler(this, &GUI::read_EepromTab_Button_Click);
			// 
			// address_read_EepromTab_Label
			// 
			this->address_read_EepromTab_Label->AutoSize = true;
			this->address_read_EepromTab_Label->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->address_read_EepromTab_Label->Location = System::Drawing::Point(46, 190);
			this->address_read_EepromTab_Label->Name = L"address_read_EepromTab_Label";
			this->address_read_EepromTab_Label->Size = System::Drawing::Size(64, 17);
			this->address_read_EepromTab_Label->TabIndex = 14;
			this->address_read_EepromTab_Label->Text = L"Address:";
			// 
			// address_read_EepromTab_TextBox
			// 
			this->address_read_EepromTab_TextBox->Location = System::Drawing::Point(172, 189);
			this->address_read_EepromTab_TextBox->Name = L"address_read_EepromTab_TextBox";
			this->address_read_EepromTab_TextBox->Size = System::Drawing::Size(71, 23);
			this->address_read_EepromTab_TextBox->TabIndex = 13;
			// 
			// read_EepromTab_TextBox
			// 
			this->read_EepromTab_TextBox->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->read_EepromTab_TextBox->Location = System::Drawing::Point(6, 4);
			this->read_EepromTab_TextBox->Multiline = true;
			this->read_EepromTab_TextBox->Name = L"read_EepromTab_TextBox";
			this->read_EepromTab_TextBox->ReadOnly = true;
			this->read_EepromTab_TextBox->Size = System::Drawing::Size(621, 149);
			this->read_EepromTab_TextBox->TabIndex = 12;
			this->read_EepromTab_TextBox->Text = L"Format Wistone\'s EEPROM:\r\n\r\n^ Choose \"Destination\" ( Network ID of Wistone or 0 f" 
				L"or Broadcast )\r\n^ Press \"Format\" to entirely clear EEPROM\'s memory";
			// 
			// write_EepromTab_TabPage
			// 
			this->write_EepromTab_TabPage->BackColor = System::Drawing::Color::WhiteSmoke;
			this->write_EepromTab_TabPage->Controls->Add(this->destination_write_EepromTab_Label);
			this->write_EepromTab_TabPage->Controls->Add(this->destination_write_EepromTab_TextBox);
			this->write_EepromTab_TabPage->Controls->Add(this->write_write_EepromTab_Button);
			this->write_EepromTab_TabPage->Controls->Add(this->data_write_EepromTab_Label);
			this->write_EepromTab_TabPage->Controls->Add(this->data_write_EepromTab_TextBox);
			this->write_EepromTab_TabPage->Controls->Add(this->address_write_EepromTab_TextBox);
			this->write_EepromTab_TabPage->Controls->Add(this->write_EepromTab_TextBox);
			this->write_EepromTab_TabPage->Controls->Add(this->address_write_EepromTab_Label);
			this->write_EepromTab_TabPage->Location = System::Drawing::Point(4, 22);
			this->write_EepromTab_TabPage->Name = L"write_EepromTab_TabPage";
			this->write_EepromTab_TabPage->Padding = System::Windows::Forms::Padding(3);
			this->write_EepromTab_TabPage->Size = System::Drawing::Size(632, 319);
			this->write_EepromTab_TabPage->TabIndex = 4;
			this->write_EepromTab_TabPage->Text = L"Write";
			// 
			// destination_write_EepromTab_Label
			// 
			this->destination_write_EepromTab_Label->AutoSize = true;
			this->destination_write_EepromTab_Label->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(177)));
			this->destination_write_EepromTab_Label->Location = System::Drawing::Point(46, 161);
			this->destination_write_EepromTab_Label->Name = L"destination_write_EepromTab_Label";
			this->destination_write_EepromTab_Label->Size = System::Drawing::Size(83, 17);
			this->destination_write_EepromTab_Label->TabIndex = 20;
			this->destination_write_EepromTab_Label->Text = L"Destination:";
			// 
			// destination_write_EepromTab_TextBox
			// 
			this->destination_write_EepromTab_TextBox->Location = System::Drawing::Point(172, 161);
			this->destination_write_EepromTab_TextBox->Name = L"destination_write_EepromTab_TextBox";
			this->destination_write_EepromTab_TextBox->Size = System::Drawing::Size(71, 20);
			this->destination_write_EepromTab_TextBox->TabIndex = 19;
			// 
			// write_write_EepromTab_Button
			// 
			this->write_write_EepromTab_Button->AutoSize = true;
			this->write_write_EepromTab_Button->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
			this->write_write_EepromTab_Button->Location = System::Drawing::Point(276, 250);
			this->write_write_EepromTab_Button->Name = L"write_write_EepromTab_Button";
			this->write_write_EepromTab_Button->Size = System::Drawing::Size(80, 28);
			this->write_write_EepromTab_Button->TabIndex = 18;
			this->write_write_EepromTab_Button->Text = L"Write";
			this->write_write_EepromTab_Button->UseVisualStyleBackColor = true;
			this->write_write_EepromTab_Button->Click += gcnew System::EventHandler(this, &GUI::write_EepromTab_Button_Click);
			// 
			// data_write_EepromTab_Label
			// 
			this->data_write_EepromTab_Label->AutoSize = true;
			this->data_write_EepromTab_Label->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->data_write_EepromTab_Label->Location = System::Drawing::Point(46, 220);
			this->data_write_EepromTab_Label->Name = L"data_write_EepromTab_Label";
			this->data_write_EepromTab_Label->Size = System::Drawing::Size(42, 17);
			this->data_write_EepromTab_Label->TabIndex = 17;
			this->data_write_EepromTab_Label->Text = L"Data:";
			// 
			// data_write_EepromTab_TextBox
			// 
			this->data_write_EepromTab_TextBox->Location = System::Drawing::Point(172, 217);
			this->data_write_EepromTab_TextBox->MaxLength = 60;
			this->data_write_EepromTab_TextBox->Multiline = true;
			this->data_write_EepromTab_TextBox->Name = L"data_write_EepromTab_TextBox";
			this->data_write_EepromTab_TextBox->Size = System::Drawing::Size(353, 20);
			this->data_write_EepromTab_TextBox->TabIndex = 16;
			// 
			// address_write_EepromTab_TextBox
			// 
			this->address_write_EepromTab_TextBox->Location = System::Drawing::Point(172, 190);
			this->address_write_EepromTab_TextBox->Name = L"address_write_EepromTab_TextBox";
			this->address_write_EepromTab_TextBox->Size = System::Drawing::Size(71, 20);
			this->address_write_EepromTab_TextBox->TabIndex = 14;
			// 
			// write_EepromTab_TextBox
			// 
			this->write_EepromTab_TextBox->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->write_EepromTab_TextBox->Location = System::Drawing::Point(6, 4);
			this->write_EepromTab_TextBox->Multiline = true;
			this->write_EepromTab_TextBox->Name = L"write_EepromTab_TextBox";
			this->write_EepromTab_TextBox->ReadOnly = true;
			this->write_EepromTab_TextBox->Size = System::Drawing::Size(621, 133);
			this->write_EepromTab_TextBox->TabIndex = 13;
			this->write_EepromTab_TextBox->Text = resources->GetString(L"write_EepromTab_TextBox.Text");
			// 
			// address_write_EepromTab_Label
			// 
			this->address_write_EepromTab_Label->AutoSize = true;
			this->address_write_EepromTab_Label->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->address_write_EepromTab_Label->Location = System::Drawing::Point(46, 190);
			this->address_write_EepromTab_Label->Name = L"address_write_EepromTab_Label";
			this->address_write_EepromTab_Label->Size = System::Drawing::Size(64, 17);
			this->address_write_EepromTab_Label->TabIndex = 15;
			this->address_write_EepromTab_Label->Text = L"Address:";
			// 
			// setBoot_EepromTab_TabPage
			// 
			this->setBoot_EepromTab_TabPage->BackColor = System::Drawing::Color::WhiteSmoke;
			this->setBoot_EepromTab_TabPage->Controls->Add(this->destination_setBoot_EepromTab_Label);
			this->setBoot_EepromTab_TabPage->Controls->Add(this->destination_setBoot_EepromTab_TextBox);
			this->setBoot_EepromTab_TabPage->Controls->Add(this->command_setBoot_EepromTab_TextBox);
			this->setBoot_EepromTab_TabPage->Controls->Add(this->command_setBoot_EepromTab_Label);
			this->setBoot_EepromTab_TabPage->Controls->Add(this->apply_setBoot_EepromTab_Button);
			this->setBoot_EepromTab_TabPage->Controls->Add(this->entryNumber_setBoot_EepromTab_Label);
			this->setBoot_EepromTab_TabPage->Controls->Add(this->setBoot_EepromTab_TextBox);
			this->setBoot_EepromTab_TabPage->Controls->Add(this->entryNumber_setBoot_EepromTab_ComboBox);
			this->setBoot_EepromTab_TabPage->Location = System::Drawing::Point(4, 22);
			this->setBoot_EepromTab_TabPage->Name = L"setBoot_EepromTab_TabPage";
			this->setBoot_EepromTab_TabPage->Padding = System::Windows::Forms::Padding(3);
			this->setBoot_EepromTab_TabPage->Size = System::Drawing::Size(632, 319);
			this->setBoot_EepromTab_TabPage->TabIndex = 5;
			this->setBoot_EepromTab_TabPage->Text = L"Set Boot";
			// 
			// destination_setBoot_EepromTab_Label
			// 
			this->destination_setBoot_EepromTab_Label->AutoSize = true;
			this->destination_setBoot_EepromTab_Label->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(177)));
			this->destination_setBoot_EepromTab_Label->Location = System::Drawing::Point(46, 161);
			this->destination_setBoot_EepromTab_Label->Name = L"destination_setBoot_EepromTab_Label";
			this->destination_setBoot_EepromTab_Label->Size = System::Drawing::Size(83, 17);
			this->destination_setBoot_EepromTab_Label->TabIndex = 25;
			this->destination_setBoot_EepromTab_Label->Text = L"Destination:";
			// 
			// destination_setBoot_EepromTab_TextBox
			// 
			this->destination_setBoot_EepromTab_TextBox->Location = System::Drawing::Point(172, 161);
			this->destination_setBoot_EepromTab_TextBox->Name = L"destination_setBoot_EepromTab_TextBox";
			this->destination_setBoot_EepromTab_TextBox->Size = System::Drawing::Size(71, 20);
			this->destination_setBoot_EepromTab_TextBox->TabIndex = 24;
			// 
			// command_setBoot_EepromTab_TextBox
			// 
			this->command_setBoot_EepromTab_TextBox->Location = System::Drawing::Point(172, 217);
			this->command_setBoot_EepromTab_TextBox->MaxLength = 60;
			this->command_setBoot_EepromTab_TextBox->Name = L"command_setBoot_EepromTab_TextBox";
			this->command_setBoot_EepromTab_TextBox->Size = System::Drawing::Size(353, 20);
			this->command_setBoot_EepromTab_TextBox->TabIndex = 23;
			// 
			// command_setBoot_EepromTab_Label
			// 
			this->command_setBoot_EepromTab_Label->AutoSize = true;
			this->command_setBoot_EepromTab_Label->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->command_setBoot_EepromTab_Label->Location = System::Drawing::Point(46, 220);
			this->command_setBoot_EepromTab_Label->Name = L"command_setBoot_EepromTab_Label";
			this->command_setBoot_EepromTab_Label->Size = System::Drawing::Size(75, 17);
			this->command_setBoot_EepromTab_Label->TabIndex = 22;
			this->command_setBoot_EepromTab_Label->Text = L"Command:";
			// 
			// apply_setBoot_EepromTab_Button
			// 
			this->apply_setBoot_EepromTab_Button->AutoSize = true;
			this->apply_setBoot_EepromTab_Button->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
			this->apply_setBoot_EepromTab_Button->Location = System::Drawing::Point(276, 250);
			this->apply_setBoot_EepromTab_Button->Name = L"apply_setBoot_EepromTab_Button";
			this->apply_setBoot_EepromTab_Button->Size = System::Drawing::Size(80, 28);
			this->apply_setBoot_EepromTab_Button->TabIndex = 21;
			this->apply_setBoot_EepromTab_Button->Text = L"Apply";
			this->apply_setBoot_EepromTab_Button->UseVisualStyleBackColor = true;
			this->apply_setBoot_EepromTab_Button->Click += gcnew System::EventHandler(this, &GUI::apply_setBoot_EepromTab_Button_Click);
			// 
			// entryNumber_setBoot_EepromTab_Label
			// 
			this->entryNumber_setBoot_EepromTab_Label->AutoSize = true;
			this->entryNumber_setBoot_EepromTab_Label->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->entryNumber_setBoot_EepromTab_Label->Location = System::Drawing::Point(46, 190);
			this->entryNumber_setBoot_EepromTab_Label->Name = L"entryNumber_setBoot_EepromTab_Label";
			this->entryNumber_setBoot_EepromTab_Label->Size = System::Drawing::Size(99, 17);
			this->entryNumber_setBoot_EepromTab_Label->TabIndex = 20;
			this->entryNumber_setBoot_EepromTab_Label->Text = L"Entry Number:";
			// 
			// setBoot_EepromTab_TextBox
			// 
			this->setBoot_EepromTab_TextBox->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->setBoot_EepromTab_TextBox->Location = System::Drawing::Point(6, 4);
			this->setBoot_EepromTab_TextBox->Multiline = true;
			this->setBoot_EepromTab_TextBox->Name = L"setBoot_EepromTab_TextBox";
			this->setBoot_EepromTab_TextBox->ReadOnly = true;
			this->setBoot_EepromTab_TextBox->Size = System::Drawing::Size(621, 143);
			this->setBoot_EepromTab_TextBox->TabIndex = 19;
			this->setBoot_EepromTab_TextBox->Text = resources->GetString(L"setBoot_EepromTab_TextBox.Text");
			// 
			// entryNumber_setBoot_EepromTab_ComboBox
			// 
			this->entryNumber_setBoot_EepromTab_ComboBox->FormattingEnabled = true;
			this->entryNumber_setBoot_EepromTab_ComboBox->Items->AddRange(gcnew cli::array< System::Object^  >(10) {L"0", L"1", L"2", 
				L"3", L"4", L"5", L"6", L"7", L"8", L"9"});
			this->entryNumber_setBoot_EepromTab_ComboBox->Location = System::Drawing::Point(172, 190);
			this->entryNumber_setBoot_EepromTab_ComboBox->Name = L"entryNumber_setBoot_EepromTab_ComboBox";
			this->entryNumber_setBoot_EepromTab_ComboBox->Size = System::Drawing::Size(71, 21);
			this->entryNumber_setBoot_EepromTab_ComboBox->TabIndex = 18;
			// 
			// getBoot_EepromTab_TabPage
			// 
			this->getBoot_EepromTab_TabPage->BackColor = System::Drawing::Color::WhiteSmoke;
			this->getBoot_EepromTab_TabPage->Controls->Add(this->destination_getBoot_EepromBoot_Label);
			this->getBoot_EepromTab_TabPage->Controls->Add(this->destination_getBoot_EepromTab_TextBox);
			this->getBoot_EepromTab_TabPage->Controls->Add(this->apply_getBoot_EepromTab_Button);
			this->getBoot_EepromTab_TabPage->Controls->Add(this->entryNumber_getBoot_EepromTab_Label);
			this->getBoot_EepromTab_TabPage->Controls->Add(this->getBoot_EepromTab_TextBox);
			this->getBoot_EepromTab_TabPage->Controls->Add(this->entryNumber_getBoot_EepromTab_ComboBox);
			this->getBoot_EepromTab_TabPage->Location = System::Drawing::Point(4, 22);
			this->getBoot_EepromTab_TabPage->Name = L"getBoot_EepromTab_TabPage";
			this->getBoot_EepromTab_TabPage->Size = System::Drawing::Size(632, 319);
			this->getBoot_EepromTab_TabPage->TabIndex = 6;
			this->getBoot_EepromTab_TabPage->Text = L"Get Boot";
			// 
			// destination_getBoot_EepromBoot_Label
			// 
			this->destination_getBoot_EepromBoot_Label->AutoSize = true;
			this->destination_getBoot_EepromBoot_Label->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(177)));
			this->destination_getBoot_EepromBoot_Label->Location = System::Drawing::Point(46, 190);
			this->destination_getBoot_EepromBoot_Label->Name = L"destination_getBoot_EepromBoot_Label";
			this->destination_getBoot_EepromBoot_Label->Size = System::Drawing::Size(83, 17);
			this->destination_getBoot_EepromBoot_Label->TabIndex = 27;
			this->destination_getBoot_EepromBoot_Label->Text = L"Destination:";
			// 
			// destination_getBoot_EepromTab_TextBox
			// 
			this->destination_getBoot_EepromTab_TextBox->Location = System::Drawing::Point(172, 190);
			this->destination_getBoot_EepromTab_TextBox->Name = L"destination_getBoot_EepromTab_TextBox";
			this->destination_getBoot_EepromTab_TextBox->Size = System::Drawing::Size(71, 20);
			this->destination_getBoot_EepromTab_TextBox->TabIndex = 26;
			// 
			// apply_getBoot_EepromTab_Button
			// 
			this->apply_getBoot_EepromTab_Button->AutoSize = true;
			this->apply_getBoot_EepromTab_Button->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
			this->apply_getBoot_EepromTab_Button->Location = System::Drawing::Point(276, 250);
			this->apply_getBoot_EepromTab_Button->Name = L"apply_getBoot_EepromTab_Button";
			this->apply_getBoot_EepromTab_Button->Size = System::Drawing::Size(80, 28);
			this->apply_getBoot_EepromTab_Button->TabIndex = 25;
			this->apply_getBoot_EepromTab_Button->Text = L"Apply";
			this->apply_getBoot_EepromTab_Button->UseVisualStyleBackColor = true;
			this->apply_getBoot_EepromTab_Button->Click += gcnew System::EventHandler(this, &GUI::apply_getBoot_EepromTab_Button_Click);
			// 
			// entryNumber_getBoot_EepromTab_Label
			// 
			this->entryNumber_getBoot_EepromTab_Label->AutoSize = true;
			this->entryNumber_getBoot_EepromTab_Label->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->entryNumber_getBoot_EepromTab_Label->Location = System::Drawing::Point(46, 220);
			this->entryNumber_getBoot_EepromTab_Label->Name = L"entryNumber_getBoot_EepromTab_Label";
			this->entryNumber_getBoot_EepromTab_Label->Size = System::Drawing::Size(99, 17);
			this->entryNumber_getBoot_EepromTab_Label->TabIndex = 24;
			this->entryNumber_getBoot_EepromTab_Label->Text = L"Entry Number:";
			// 
			// getBoot_EepromTab_TextBox
			// 
			this->getBoot_EepromTab_TextBox->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->getBoot_EepromTab_TextBox->Location = System::Drawing::Point(5, 3);
			this->getBoot_EepromTab_TextBox->Multiline = true;
			this->getBoot_EepromTab_TextBox->Name = L"getBoot_EepromTab_TextBox";
			this->getBoot_EepromTab_TextBox->ReadOnly = true;
			this->getBoot_EepromTab_TextBox->Size = System::Drawing::Size(622, 108);
			this->getBoot_EepromTab_TextBox->TabIndex = 23;
			this->getBoot_EepromTab_TextBox->Text = resources->GetString(L"getBoot_EepromTab_TextBox.Text");
			// 
			// entryNumber_getBoot_EepromTab_ComboBox
			// 
			this->entryNumber_getBoot_EepromTab_ComboBox->FormattingEnabled = true;
			this->entryNumber_getBoot_EepromTab_ComboBox->Items->AddRange(gcnew cli::array< System::Object^  >(10) {L"0", L"1", L"2", 
				L"3", L"4", L"5", L"6", L"7", L"8", L"9"});
			this->entryNumber_getBoot_EepromTab_ComboBox->Location = System::Drawing::Point(172, 217);
			this->entryNumber_getBoot_EepromTab_ComboBox->Name = L"entryNumber_getBoot_EepromTab_ComboBox";
			this->entryNumber_getBoot_EepromTab_ComboBox->Size = System::Drawing::Size(70, 21);
			this->entryNumber_getBoot_EepromTab_ComboBox->TabIndex = 22;
			// 
			// readLL_EepromTab_TabPage
			// 
			this->readLL_EepromTab_TabPage->BackColor = System::Drawing::Color::WhiteSmoke;
			this->readLL_EepromTab_TabPage->Controls->Add(this->destination_readLL_EepromTab_Label);
			this->readLL_EepromTab_TabPage->Controls->Add(this->destination_readLL_EepromTab_TextBox);
			this->readLL_EepromTab_TabPage->Controls->Add(this->read_readLL_EepromTab_Button);
			this->readLL_EepromTab_TabPage->Controls->Add(this->address_readLL_EepromTab_Label);
			this->readLL_EepromTab_TabPage->Controls->Add(this->address_readLL_EepromTab_TextBox);
			this->readLL_EepromTab_TabPage->Controls->Add(this->readLL_EepromTab_TextBox);
			this->readLL_EepromTab_TabPage->Location = System::Drawing::Point(4, 22);
			this->readLL_EepromTab_TabPage->Name = L"readLL_EepromTab_TabPage";
			this->readLL_EepromTab_TabPage->Padding = System::Windows::Forms::Padding(3);
			this->readLL_EepromTab_TabPage->Size = System::Drawing::Size(632, 319);
			this->readLL_EepromTab_TabPage->TabIndex = 0;
			this->readLL_EepromTab_TabPage->Text = L"Read (Low Level)";
			// 
			// destination_readLL_EepromTab_Label
			// 
			this->destination_readLL_EepromTab_Label->AutoSize = true;
			this->destination_readLL_EepromTab_Label->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(177)));
			this->destination_readLL_EepromTab_Label->Location = System::Drawing::Point(46, 190);
			this->destination_readLL_EepromTab_Label->Name = L"destination_readLL_EepromTab_Label";
			this->destination_readLL_EepromTab_Label->Size = System::Drawing::Size(83, 17);
			this->destination_readLL_EepromTab_Label->TabIndex = 13;
			this->destination_readLL_EepromTab_Label->Text = L"Destination:";
			// 
			// destination_readLL_EepromTab_TextBox
			// 
			this->destination_readLL_EepromTab_TextBox->Location = System::Drawing::Point(172, 190);
			this->destination_readLL_EepromTab_TextBox->Name = L"destination_readLL_EepromTab_TextBox";
			this->destination_readLL_EepromTab_TextBox->Size = System::Drawing::Size(71, 20);
			this->destination_readLL_EepromTab_TextBox->TabIndex = 12;
			// 
			// read_readLL_EepromTab_Button
			// 
			this->read_readLL_EepromTab_Button->AutoSize = true;
			this->read_readLL_EepromTab_Button->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
			this->read_readLL_EepromTab_Button->Location = System::Drawing::Point(276, 250);
			this->read_readLL_EepromTab_Button->Name = L"read_readLL_EepromTab_Button";
			this->read_readLL_EepromTab_Button->Size = System::Drawing::Size(80, 28);
			this->read_readLL_EepromTab_Button->TabIndex = 11;
			this->read_readLL_EepromTab_Button->Text = L"Read";
			this->read_readLL_EepromTab_Button->UseVisualStyleBackColor = true;
			this->read_readLL_EepromTab_Button->Click += gcnew System::EventHandler(this, &GUI::read_readLL_EepromTab_Button_Click);
			// 
			// address_readLL_EepromTab_Label
			// 
			this->address_readLL_EepromTab_Label->AutoSize = true;
			this->address_readLL_EepromTab_Label->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->address_readLL_EepromTab_Label->Location = System::Drawing::Point(46, 220);
			this->address_readLL_EepromTab_Label->Name = L"address_readLL_EepromTab_Label";
			this->address_readLL_EepromTab_Label->Size = System::Drawing::Size(64, 17);
			this->address_readLL_EepromTab_Label->TabIndex = 10;
			this->address_readLL_EepromTab_Label->Text = L"Address:";
			// 
			// address_readLL_EepromTab_TextBox
			// 
			this->address_readLL_EepromTab_TextBox->Location = System::Drawing::Point(172, 217);
			this->address_readLL_EepromTab_TextBox->Name = L"address_readLL_EepromTab_TextBox";
			this->address_readLL_EepromTab_TextBox->Size = System::Drawing::Size(71, 20);
			this->address_readLL_EepromTab_TextBox->TabIndex = 9;
			this->address_readLL_EepromTab_TextBox->Text = L"0";
			// 
			// readLL_EepromTab_TextBox
			// 
			this->readLL_EepromTab_TextBox->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->readLL_EepromTab_TextBox->Location = System::Drawing::Point(6, 5);
			this->readLL_EepromTab_TextBox->Multiline = true;
			this->readLL_EepromTab_TextBox->Name = L"readLL_EepromTab_TextBox";
			this->readLL_EepromTab_TextBox->ReadOnly = true;
			this->readLL_EepromTab_TextBox->Size = System::Drawing::Size(621, 106);
			this->readLL_EepromTab_TextBox->TabIndex = 8;
			this->readLL_EepromTab_TextBox->Text = L"Read a byte from Wistone\'s EEPROM:\r\n\r\n^ Choose \"Destination\" ( Network ID of Wist" 
				L"one or 0 for Broadcast )\r\n^ Enter: \"Address\" ( of the byte within 32K memory spa" 
				L"ce )\r\n^ Press \"Read\"";
			// 
			// writeLL_EepromTab_TabPage
			// 
			this->writeLL_EepromTab_TabPage->BackColor = System::Drawing::Color::WhiteSmoke;
			this->writeLL_EepromTab_TabPage->Controls->Add(this->destination_writeLL_EepromTab_Label);
			this->writeLL_EepromTab_TabPage->Controls->Add(this->destination_writeLL_EepromTab_TextBox);
			this->writeLL_EepromTab_TabPage->Controls->Add(this->write_writeLL_EepromTab_Button);
			this->writeLL_EepromTab_TabPage->Controls->Add(this->data_writeLL_EepromTab_Label);
			this->writeLL_EepromTab_TabPage->Controls->Add(this->data_writeLL_EepromTab_TextBox);
			this->writeLL_EepromTab_TabPage->Controls->Add(this->address_writeLL_EepromTab_TextBox);
			this->writeLL_EepromTab_TabPage->Controls->Add(this->writeLL_EepromTab_TextBox);
			this->writeLL_EepromTab_TabPage->Controls->Add(this->address_writeLL_EepromTab_Label);
			this->writeLL_EepromTab_TabPage->Location = System::Drawing::Point(4, 22);
			this->writeLL_EepromTab_TabPage->Name = L"writeLL_EepromTab_TabPage";
			this->writeLL_EepromTab_TabPage->Padding = System::Windows::Forms::Padding(3);
			this->writeLL_EepromTab_TabPage->Size = System::Drawing::Size(632, 319);
			this->writeLL_EepromTab_TabPage->TabIndex = 1;
			this->writeLL_EepromTab_TabPage->Text = L"Write (Low Level)";
			// 
			// destination_writeLL_EepromTab_Label
			// 
			this->destination_writeLL_EepromTab_Label->AutoSize = true;
			this->destination_writeLL_EepromTab_Label->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(177)));
			this->destination_writeLL_EepromTab_Label->Location = System::Drawing::Point(46, 161);
			this->destination_writeLL_EepromTab_Label->Name = L"destination_writeLL_EepromTab_Label";
			this->destination_writeLL_EepromTab_Label->Size = System::Drawing::Size(83, 17);
			this->destination_writeLL_EepromTab_Label->TabIndex = 14;
			this->destination_writeLL_EepromTab_Label->Text = L"Destination:";
			// 
			// destination_writeLL_EepromTab_TextBox
			// 
			this->destination_writeLL_EepromTab_TextBox->Location = System::Drawing::Point(172, 161);
			this->destination_writeLL_EepromTab_TextBox->Name = L"destination_writeLL_EepromTab_TextBox";
			this->destination_writeLL_EepromTab_TextBox->Size = System::Drawing::Size(71, 20);
			this->destination_writeLL_EepromTab_TextBox->TabIndex = 13;
			// 
			// write_writeLL_EepromTab_Button
			// 
			this->write_writeLL_EepromTab_Button->AutoSize = true;
			this->write_writeLL_EepromTab_Button->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
			this->write_writeLL_EepromTab_Button->Location = System::Drawing::Point(276, 250);
			this->write_writeLL_EepromTab_Button->Name = L"write_writeLL_EepromTab_Button";
			this->write_writeLL_EepromTab_Button->Size = System::Drawing::Size(80, 28);
			this->write_writeLL_EepromTab_Button->TabIndex = 12;
			this->write_writeLL_EepromTab_Button->Text = L"Write";
			this->write_writeLL_EepromTab_Button->UseVisualStyleBackColor = true;
			this->write_writeLL_EepromTab_Button->Click += gcnew System::EventHandler(this, &GUI::write_writeLL_EepromTab_Button_Click);
			// 
			// data_writeLL_EepromTab_Label
			// 
			this->data_writeLL_EepromTab_Label->AutoSize = true;
			this->data_writeLL_EepromTab_Label->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->data_writeLL_EepromTab_Label->Location = System::Drawing::Point(46, 220);
			this->data_writeLL_EepromTab_Label->Name = L"data_writeLL_EepromTab_Label";
			this->data_writeLL_EepromTab_Label->Size = System::Drawing::Size(42, 17);
			this->data_writeLL_EepromTab_Label->TabIndex = 11;
			this->data_writeLL_EepromTab_Label->Text = L"Data:";
			// 
			// data_writeLL_EepromTab_TextBox
			// 
			this->data_writeLL_EepromTab_TextBox->Location = System::Drawing::Point(172, 217);
			this->data_writeLL_EepromTab_TextBox->Multiline = true;
			this->data_writeLL_EepromTab_TextBox->Name = L"data_writeLL_EepromTab_TextBox";
			this->data_writeLL_EepromTab_TextBox->Size = System::Drawing::Size(71, 20);
			this->data_writeLL_EepromTab_TextBox->TabIndex = 10;
			// 
			// address_writeLL_EepromTab_TextBox
			// 
			this->address_writeLL_EepromTab_TextBox->Location = System::Drawing::Point(172, 190);
			this->address_writeLL_EepromTab_TextBox->Name = L"address_writeLL_EepromTab_TextBox";
			this->address_writeLL_EepromTab_TextBox->Size = System::Drawing::Size(71, 20);
			this->address_writeLL_EepromTab_TextBox->TabIndex = 8;
			this->address_writeLL_EepromTab_TextBox->Text = L"0";
			// 
			// writeLL_EepromTab_TextBox
			// 
			this->writeLL_EepromTab_TextBox->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->writeLL_EepromTab_TextBox->Location = System::Drawing::Point(6, 5);
			this->writeLL_EepromTab_TextBox->Multiline = true;
			this->writeLL_EepromTab_TextBox->Name = L"writeLL_EepromTab_TextBox";
			this->writeLL_EepromTab_TextBox->ReadOnly = true;
			this->writeLL_EepromTab_TextBox->Size = System::Drawing::Size(621, 111);
			this->writeLL_EepromTab_TextBox->TabIndex = 7;
			this->writeLL_EepromTab_TextBox->Text = resources->GetString(L"writeLL_EepromTab_TextBox.Text");
			// 
			// address_writeLL_EepromTab_Label
			// 
			this->address_writeLL_EepromTab_Label->AutoSize = true;
			this->address_writeLL_EepromTab_Label->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->address_writeLL_EepromTab_Label->Location = System::Drawing::Point(46, 190);
			this->address_writeLL_EepromTab_Label->Name = L"address_writeLL_EepromTab_Label";
			this->address_writeLL_EepromTab_Label->Size = System::Drawing::Size(64, 17);
			this->address_writeLL_EepromTab_Label->TabIndex = 9;
			this->address_writeLL_EepromTab_Label->Text = L"Address:";
			// 
			// tabADC
			// 
			this->tabADC->BackColor = System::Drawing::Color::WhiteSmoke;
			this->tabADC->Controls->Add(this->ADCTab);
			this->tabADC->Location = System::Drawing::Point(4, 22);
			this->tabADC->Name = L"tabADC";
			this->tabADC->Padding = System::Windows::Forms::Padding(3);
			this->tabADC->Size = System::Drawing::Size(633, 339);
			this->tabADC->TabIndex = 12;
			this->tabADC->Text = L"ADC";
			// 
			// ADCTab
			// 
			this->ADCTab->Controls->Add(this->readLL_AdcTab_TabPage);
			this->ADCTab->Controls->Add(this->writeLL_AdcTab_TabPage);
			this->ADCTab->Location = System::Drawing::Point(0, 0);
			this->ADCTab->Name = L"ADCTab";
			this->ADCTab->SelectedIndex = 0;
			this->ADCTab->Size = System::Drawing::Size(640, 345);
			this->ADCTab->TabIndex = 1;
			// 
			// readLL_AdcTab_TabPage
			// 
			this->readLL_AdcTab_TabPage->BackColor = System::Drawing::Color::WhiteSmoke;
			this->readLL_AdcTab_TabPage->Controls->Add(this->destination_readLL_AdcTab_Label);
			this->readLL_AdcTab_TabPage->Controls->Add(this->destination_readLL_AdcTab_TextBox);
			this->readLL_AdcTab_TabPage->Controls->Add(this->read_readLL_AdcTab_Button);
			this->readLL_AdcTab_TabPage->Controls->Add(this->address_readLL_AdcTab_Label);
			this->readLL_AdcTab_TabPage->Controls->Add(this->address_readLL_AdcTab_TextBox);
			this->readLL_AdcTab_TabPage->Controls->Add(this->readLL_AdcTab_TextBox);
			this->readLL_AdcTab_TabPage->Location = System::Drawing::Point(4, 22);
			this->readLL_AdcTab_TabPage->Name = L"readLL_AdcTab_TabPage";
			this->readLL_AdcTab_TabPage->Padding = System::Windows::Forms::Padding(3);
			this->readLL_AdcTab_TabPage->Size = System::Drawing::Size(632, 319);
			this->readLL_AdcTab_TabPage->TabIndex = 0;
			this->readLL_AdcTab_TabPage->Text = L"Read(Low Level)";
			// 
			// destination_readLL_AdcTab_Label
			// 
			this->destination_readLL_AdcTab_Label->AutoSize = true;
			this->destination_readLL_AdcTab_Label->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(177)));
			this->destination_readLL_AdcTab_Label->Location = System::Drawing::Point(46, 178);
			this->destination_readLL_AdcTab_Label->Name = L"destination_readLL_AdcTab_Label";
			this->destination_readLL_AdcTab_Label->Size = System::Drawing::Size(83, 17);
			this->destination_readLL_AdcTab_Label->TabIndex = 17;
			this->destination_readLL_AdcTab_Label->Text = L"Destination:";
			// 
			// destination_readLL_AdcTab_TextBox
			// 
			this->destination_readLL_AdcTab_TextBox->Location = System::Drawing::Point(142, 176);
			this->destination_readLL_AdcTab_TextBox->Name = L"destination_readLL_AdcTab_TextBox";
			this->destination_readLL_AdcTab_TextBox->Size = System::Drawing::Size(71, 20);
			this->destination_readLL_AdcTab_TextBox->TabIndex = 16;
			// 
			// read_readLL_AdcTab_Button
			// 
			this->read_readLL_AdcTab_Button->AutoSize = true;
			this->read_readLL_AdcTab_Button->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
			this->read_readLL_AdcTab_Button->Location = System::Drawing::Point(276, 250);
			this->read_readLL_AdcTab_Button->Name = L"read_readLL_AdcTab_Button";
			this->read_readLL_AdcTab_Button->Size = System::Drawing::Size(80, 28);
			this->read_readLL_AdcTab_Button->TabIndex = 15;
			this->read_readLL_AdcTab_Button->Text = L"Read";
			this->read_readLL_AdcTab_Button->UseVisualStyleBackColor = true;
			this->read_readLL_AdcTab_Button->Click += gcnew System::EventHandler(this, &GUI::read_readLL_AdcTab_Button_Click);
			// 
			// address_readLL_AdcTab_Label
			// 
			this->address_readLL_AdcTab_Label->AutoSize = true;
			this->address_readLL_AdcTab_Label->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->address_readLL_AdcTab_Label->Location = System::Drawing::Point(46, 217);
			this->address_readLL_AdcTab_Label->Name = L"address_readLL_AdcTab_Label";
			this->address_readLL_AdcTab_Label->Size = System::Drawing::Size(64, 17);
			this->address_readLL_AdcTab_Label->TabIndex = 14;
			this->address_readLL_AdcTab_Label->Text = L"Address:";
			// 
			// address_readLL_AdcTab_TextBox
			// 
			this->address_readLL_AdcTab_TextBox->Location = System::Drawing::Point(142, 213);
			this->address_readLL_AdcTab_TextBox->Name = L"address_readLL_AdcTab_TextBox";
			this->address_readLL_AdcTab_TextBox->Size = System::Drawing::Size(71, 20);
			this->address_readLL_AdcTab_TextBox->TabIndex = 13;
			this->address_readLL_AdcTab_TextBox->Text = L"0";
			// 
			// readLL_AdcTab_TextBox
			// 
			this->readLL_AdcTab_TextBox->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->readLL_AdcTab_TextBox->Location = System::Drawing::Point(6, 0);
			this->readLL_AdcTab_TextBox->Multiline = true;
			this->readLL_AdcTab_TextBox->Name = L"readLL_AdcTab_TextBox";
			this->readLL_AdcTab_TextBox->ReadOnly = true;
			this->readLL_AdcTab_TextBox->Size = System::Drawing::Size(620, 96);
			this->readLL_AdcTab_TextBox->TabIndex = 12;
			this->readLL_AdcTab_TextBox->Text = L"Read Wistone\'s ADC:\r\n\r\n^ Choose \"Destination\" ( Network ID of Wistone or 0 for Br" 
				L"oadcast )\r\n^ Enter: \"Address\" ( of ADC )\r\n^ Press \"Read\"";
			// 
			// writeLL_AdcTab_TabPage
			// 
			this->writeLL_AdcTab_TabPage->BackColor = System::Drawing::Color::WhiteSmoke;
			this->writeLL_AdcTab_TabPage->Controls->Add(this->destination_writeLL_AdcTab_Label);
			this->writeLL_AdcTab_TabPage->Controls->Add(this->destination_writeLL_AdcTab_TextBox);
			this->writeLL_AdcTab_TabPage->Controls->Add(this->data_writeLL_AdcTab_ComboBox);
			this->writeLL_AdcTab_TabPage->Controls->Add(this->write_writeLL_AdcTab_Button);
			this->writeLL_AdcTab_TabPage->Controls->Add(this->data_readLL_AdcTab_Label);
			this->writeLL_AdcTab_TabPage->Controls->Add(this->address_writeLL_AdcTab_TextBox);
			this->writeLL_AdcTab_TabPage->Controls->Add(this->writeLL_AdcTab_TextBox);
			this->writeLL_AdcTab_TabPage->Controls->Add(this->address_writeLL_AdcTab_Label);
			this->writeLL_AdcTab_TabPage->Location = System::Drawing::Point(4, 22);
			this->writeLL_AdcTab_TabPage->Name = L"writeLL_AdcTab_TabPage";
			this->writeLL_AdcTab_TabPage->Padding = System::Windows::Forms::Padding(3);
			this->writeLL_AdcTab_TabPage->Size = System::Drawing::Size(632, 319);
			this->writeLL_AdcTab_TabPage->TabIndex = 1;
			this->writeLL_AdcTab_TabPage->Text = L"Write(Low Level)";
			// 
			// destination_writeLL_AdcTab_Label
			// 
			this->destination_writeLL_AdcTab_Label->AutoSize = true;
			this->destination_writeLL_AdcTab_Label->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(177)));
			this->destination_writeLL_AdcTab_Label->Location = System::Drawing::Point(46, 137);
			this->destination_writeLL_AdcTab_Label->Name = L"destination_writeLL_AdcTab_Label";
			this->destination_writeLL_AdcTab_Label->Size = System::Drawing::Size(83, 17);
			this->destination_writeLL_AdcTab_Label->TabIndex = 22;
			this->destination_writeLL_AdcTab_Label->Text = L"Destination:";
			// 
			// destination_writeLL_AdcTab_TextBox
			// 
			this->destination_writeLL_AdcTab_TextBox->Location = System::Drawing::Point(142, 136);
			this->destination_writeLL_AdcTab_TextBox->Name = L"destination_writeLL_AdcTab_TextBox";
			this->destination_writeLL_AdcTab_TextBox->Size = System::Drawing::Size(71, 20);
			this->destination_writeLL_AdcTab_TextBox->TabIndex = 21;
			// 
			// data_writeLL_AdcTab_ComboBox
			// 
			this->data_writeLL_AdcTab_ComboBox->FormattingEnabled = true;
			this->data_writeLL_AdcTab_ComboBox->Items->AddRange(gcnew cli::array< System::Object^  >(1) {L"8"});
			this->data_writeLL_AdcTab_ComboBox->Location = System::Drawing::Point(142, 213);
			this->data_writeLL_AdcTab_ComboBox->Name = L"data_writeLL_AdcTab_ComboBox";
			this->data_writeLL_AdcTab_ComboBox->Size = System::Drawing::Size(71, 21);
			this->data_writeLL_AdcTab_ComboBox->TabIndex = 20;
			// 
			// write_writeLL_AdcTab_Button
			// 
			this->write_writeLL_AdcTab_Button->AutoSize = true;
			this->write_writeLL_AdcTab_Button->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
			this->write_writeLL_AdcTab_Button->Location = System::Drawing::Point(276, 250);
			this->write_writeLL_AdcTab_Button->Name = L"write_writeLL_AdcTab_Button";
			this->write_writeLL_AdcTab_Button->Size = System::Drawing::Size(80, 28);
			this->write_writeLL_AdcTab_Button->TabIndex = 18;
			this->write_writeLL_AdcTab_Button->Text = L"Write";
			this->write_writeLL_AdcTab_Button->UseVisualStyleBackColor = true;
			this->write_writeLL_AdcTab_Button->Click += gcnew System::EventHandler(this, &GUI::write_writeLL_AdcTab_Button_Click);
			// 
			// data_readLL_AdcTab_Label
			// 
			this->data_readLL_AdcTab_Label->AutoSize = true;
			this->data_readLL_AdcTab_Label->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->data_readLL_AdcTab_Label->Location = System::Drawing::Point(46, 217);
			this->data_readLL_AdcTab_Label->Name = L"data_readLL_AdcTab_Label";
			this->data_readLL_AdcTab_Label->Size = System::Drawing::Size(42, 17);
			this->data_readLL_AdcTab_Label->TabIndex = 17;
			this->data_readLL_AdcTab_Label->Text = L"Data:";
			// 
			// address_writeLL_AdcTab_TextBox
			// 
			this->address_writeLL_AdcTab_TextBox->Location = System::Drawing::Point(142, 176);
			this->address_writeLL_AdcTab_TextBox->Name = L"address_writeLL_AdcTab_TextBox";
			this->address_writeLL_AdcTab_TextBox->Size = System::Drawing::Size(71, 20);
			this->address_writeLL_AdcTab_TextBox->TabIndex = 14;
			this->address_writeLL_AdcTab_TextBox->Text = L"0";
			// 
			// writeLL_AdcTab_TextBox
			// 
			this->writeLL_AdcTab_TextBox->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->writeLL_AdcTab_TextBox->Location = System::Drawing::Point(6, 0);
			this->writeLL_AdcTab_TextBox->Multiline = true;
			this->writeLL_AdcTab_TextBox->Name = L"writeLL_AdcTab_TextBox";
			this->writeLL_AdcTab_TextBox->ReadOnly = true;
			this->writeLL_AdcTab_TextBox->Size = System::Drawing::Size(617, 104);
			this->writeLL_AdcTab_TextBox->TabIndex = 13;
			this->writeLL_AdcTab_TextBox->Text = L"Write Wistone\'s ADC:\r\n\r\n^ Choose \"Destination\" ( Network ID of Wistone or 0 for B" 
				L"roadcast )\r\n^ Enter: \"Address\" (of ADC) and \"Data\"\r\n^ Press \"Write\"\r\n";
			// 
			// address_writeLL_AdcTab_Label
			// 
			this->address_writeLL_AdcTab_Label->AutoSize = true;
			this->address_writeLL_AdcTab_Label->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->address_writeLL_AdcTab_Label->Location = System::Drawing::Point(46, 176);
			this->address_writeLL_AdcTab_Label->Name = L"address_writeLL_AdcTab_Label";
			this->address_writeLL_AdcTab_Label->Size = System::Drawing::Size(64, 17);
			this->address_writeLL_AdcTab_Label->TabIndex = 15;
			this->address_writeLL_AdcTab_Label->Text = L"Address:";
			// 
			// tabHub
			// 
			this->tabHub->BackColor = System::Drawing::Color::WhiteSmoke;
			this->tabHub->Controls->Add(this->hubTab);
			this->tabHub->Location = System::Drawing::Point(4, 22);
			this->tabHub->Name = L"tabHub";
			this->tabHub->Padding = System::Windows::Forms::Padding(3);
			this->tabHub->Size = System::Drawing::Size(633, 339);
			this->tabHub->TabIndex = 13;
			this->tabHub->Text = L"Hub";
			// 
			// hubTab
			// 
			this->hubTab->Controls->Add(this->statistics_HubTab_TabPage);
			this->hubTab->Location = System::Drawing::Point(0, 0);
			this->hubTab->Name = L"hubTab";
			this->hubTab->SelectedIndex = 0;
			this->hubTab->Size = System::Drawing::Size(641, 343);
			this->hubTab->TabIndex = 0;
			// 
			// statistics_HubTab_TabPage
			// 
			this->statistics_HubTab_TabPage->BackColor = System::Drawing::SystemColors::Control;
			this->statistics_HubTab_TabPage->Location = System::Drawing::Point(4, 22);
			this->statistics_HubTab_TabPage->Name = L"statistics_HubTab_TabPage";
			this->statistics_HubTab_TabPage->Padding = System::Windows::Forms::Padding(3);
			this->statistics_HubTab_TabPage->Size = System::Drawing::Size(633, 317);
			this->statistics_HubTab_TabPage->TabIndex = 0;
			this->statistics_HubTab_TabPage->Text = L"Statistics";
			// 
			// x_TextBox
			// 
			this->x_TextBox->Location = System::Drawing::Point(159, 24);
			this->x_TextBox->Name = L"x_TextBox";
			this->x_TextBox->Size = System::Drawing::Size(69, 20);
			this->x_TextBox->TabIndex = 27;
			// 
			// z_TextBox
			// 
			this->z_TextBox->Location = System::Drawing::Point(388, 25);
			this->z_TextBox->Name = L"z_TextBox";
			this->z_TextBox->Size = System::Drawing::Size(69, 20);
			this->z_TextBox->TabIndex = 28;
			// 
			// y_TextBox
			// 
			this->y_TextBox->Location = System::Drawing::Point(272, 25);
			this->y_TextBox->Name = L"y_TextBox";
			this->y_TextBox->Size = System::Drawing::Size(69, 20);
			this->y_TextBox->TabIndex = 29;
			// 
			// z_Label
			// 
			this->z_Label->AutoSize = true;
			this->z_Label->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->z_Label->Location = System::Drawing::Point(352, 26);
			this->z_Label->Name = L"z_Label";
			this->z_Label->Size = System::Drawing::Size(25, 17);
			this->z_Label->TabIndex = 31;
			this->z_Label->Text = L"Z :";
			// 
			// x_Label
			// 
			this->x_Label->AutoSize = true;
			this->x_Label->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->x_Label->Location = System::Drawing::Point(127, 25);
			this->x_Label->Name = L"x_Label";
			this->x_Label->Size = System::Drawing::Size(21, 17);
			this->x_Label->TabIndex = 34;
			this->x_Label->Text = L"X:";
			// 
			// y_Label
			// 
			this->y_Label->AutoSize = true;
			this->y_Label->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->y_Label->Location = System::Drawing::Point(239, 26);
			this->y_Label->Name = L"y_Label";
			this->y_Label->Size = System::Drawing::Size(25, 17);
			this->y_Label->TabIndex = 33;
			this->y_Label->Text = L"Y :";
			// 
			// meanOfAxes_Label
			// 
			this->meanOfAxes_Label->AutoSize = true;
			this->meanOfAxes_Label->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->meanOfAxes_Label->Location = System::Drawing::Point(6, 25);
			this->meanOfAxes_Label->Name = L"meanOfAxes_Label";
			this->meanOfAxes_Label->Size = System::Drawing::Size(101, 17);
			this->meanOfAxes_Label->TabIndex = 35;
			this->meanOfAxes_Label->Text = L"Mean of Axes :";
			// 
			// packetErrorRate_TextBox
			// 
			this->packetErrorRate_TextBox->Location = System::Drawing::Point(272, 60);
			this->packetErrorRate_TextBox->Name = L"packetErrorRate_TextBox";
			this->packetErrorRate_TextBox->Size = System::Drawing::Size(69, 20);
			this->packetErrorRate_TextBox->TabIndex = 36;
			// 
			// statistics_Timer
			// 
			this->statistics_Timer->Enabled = true;
			this->statistics_Timer->Interval = 500;
			this->statistics_Timer->Tick += gcnew System::EventHandler(this, &GUI::statistics_Timer_Tick);
			// 
			// application_Timer
			// 
			this->application_Timer->Enabled = true;
			this->application_Timer->Tick += gcnew System::EventHandler(this, &GUI::application_Timer_Tick);
			// 
			// bytes_Label
			// 
			this->bytes_Label->AutoSize = true;
			this->bytes_Label->Location = System::Drawing::Point(352, 131);
			this->bytes_Label->Name = L"bytes_Label";
			this->bytes_Label->Size = System::Drawing::Size(36, 13);
			this->bytes_Label->TabIndex = 38;
			this->bytes_Label->Text = L"Bytes:";
			// 
			// bytes_TextBox
			// 
			this->bytes_TextBox->Location = System::Drawing::Point(388, 128);
			this->bytes_TextBox->Name = L"bytes_TextBox";
			this->bytes_TextBox->ReadOnly = true;
			this->bytes_TextBox->Size = System::Drawing::Size(69, 20);
			this->bytes_TextBox->TabIndex = 39;
			// 
			// hw_TextBox
			// 
			this->hw_TextBox->Location = System::Drawing::Point(272, 95);
			this->hw_TextBox->Name = L"hw_TextBox";
			this->hw_TextBox->ReadOnly = true;
			this->hw_TextBox->Size = System::Drawing::Size(69, 20);
			this->hw_TextBox->TabIndex = 40;
			// 
			// hw_Label
			// 
			this->hw_Label->AutoSize = true;
			this->hw_Label->Location = System::Drawing::Point(231, 96);
			this->hw_Label->Name = L"hw_Label";
			this->hw_Label->Size = System::Drawing::Size(29, 13);
			this->hw_Label->TabIndex = 41;
			this->hw_Label->Text = L"HW:";
			// 
			// sw_Label
			// 
			this->sw_Label->AutoSize = true;
			this->sw_Label->Location = System::Drawing::Point(353, 98);
			this->sw_Label->Name = L"sw_Label";
			this->sw_Label->Size = System::Drawing::Size(28, 13);
			this->sw_Label->TabIndex = 42;
			this->sw_Label->Text = L"SW:";
			// 
			// sw_TextBox
			// 
			this->sw_TextBox->Location = System::Drawing::Point(388, 97);
			this->sw_TextBox->Name = L"sw_TextBox";
			this->sw_TextBox->ReadOnly = true;
			this->sw_TextBox->Size = System::Drawing::Size(69, 20);
			this->sw_TextBox->TabIndex = 43;
			// 
			// blocks_Label
			// 
			this->blocks_Label->AutoSize = true;
			this->blocks_Label->Location = System::Drawing::Point(231, 132);
			this->blocks_Label->Name = L"blocks_Label";
			this->blocks_Label->Size = System::Drawing::Size(42, 13);
			this->blocks_Label->TabIndex = 44;
			this->blocks_Label->Text = L"Blocks:";
			// 
			// blocks_TextBox
			// 
			this->blocks_TextBox->Location = System::Drawing::Point(272, 130);
			this->blocks_TextBox->Name = L"blocks_TextBox";
			this->blocks_TextBox->ReadOnly = true;
			this->blocks_TextBox->Size = System::Drawing::Size(69, 20);
			this->blocks_TextBox->TabIndex = 45;
			// 
			// scriptFile_Label
			// 
			this->scriptFile_Label->AutoSize = true;
			this->scriptFile_Label->Location = System::Drawing::Point(36, 169);
			this->scriptFile_Label->Name = L"scriptFile_Label";
			this->scriptFile_Label->Size = System::Drawing::Size(56, 13);
			this->scriptFile_Label->TabIndex = 52;
			this->scriptFile_Label->Text = L"Script File:";
			// 
			// commandScript_Label
			// 
			this->commandScript_Label->AutoSize = true;
			this->commandScript_Label->Location = System::Drawing::Point(6, 32);
			this->commandScript_Label->Name = L"commandScript_Label";
			this->commandScript_Label->Size = System::Drawing::Size(87, 13);
			this->commandScript_Label->TabIndex = 53;
			this->commandScript_Label->Text = L"Command Script:";
			// 
			// connection_GroupBox
			// 
			this->connection_GroupBox->Controls->Add(this->internet_RadioButton);
			this->connection_GroupBox->Controls->Add(this->comPort_RadioButton);
			this->connection_GroupBox->Controls->Add(this->disconnect_Button);
			this->connection_GroupBox->Controls->Add(this->comPort_ComboBox);
			this->connection_GroupBox->Controls->Add(this->connect_Button);
			this->connection_GroupBox->Location = System::Drawing::Point(328, 11);
			this->connection_GroupBox->Name = L"connection_GroupBox";
			this->connection_GroupBox->Size = System::Drawing::Size(336, 83);
			this->connection_GroupBox->TabIndex = 55;
			this->connection_GroupBox->TabStop = false;
			this->connection_GroupBox->Text = L"Connection:";
			// 
			// internet_RadioButton
			// 
			this->internet_RadioButton->AutoSize = true;
			this->internet_RadioButton->Location = System::Drawing::Point(25, 47);
			this->internet_RadioButton->Name = L"internet_RadioButton";
			this->internet_RadioButton->Size = System::Drawing::Size(61, 17);
			this->internet_RadioButton->TabIndex = 18;
			this->internet_RadioButton->TabStop = true;
			this->internet_RadioButton->Text = L"Internet";
			this->internet_RadioButton->UseVisualStyleBackColor = true;
			this->internet_RadioButton->CheckedChanged += gcnew System::EventHandler(this, &GUI::internet_RadioButton_CheckedChanged);
			// 
			// comPort_RadioButton
			// 
			this->comPort_RadioButton->AutoSize = true;
			this->comPort_RadioButton->Location = System::Drawing::Point(25, 25);
			this->comPort_RadioButton->Name = L"comPort_RadioButton";
			this->comPort_RadioButton->Size = System::Drawing::Size(74, 17);
			this->comPort_RadioButton->TabIndex = 17;
			this->comPort_RadioButton->TabStop = true;
			this->comPort_RadioButton->Text = L"COM Port:";
			this->comPort_RadioButton->UseVisualStyleBackColor = true;
			this->comPort_RadioButton->CheckedChanged += gcnew System::EventHandler(this, &GUI::comPort_RadioButton_CheckedChanged);
			// 
			// password_TextBox
			// 
			this->password_TextBox->Location = System::Drawing::Point(93, 48);
			this->password_TextBox->Name = L"password_TextBox";
			this->password_TextBox->Size = System::Drawing::Size(110, 20);
			this->password_TextBox->TabIndex = 47;
			this->password_TextBox->Text = L"Enter Password...";
			// 
			// userName_TextBox
			// 
			this->userName_TextBox->Location = System::Drawing::Point(93, 21);
			this->userName_TextBox->Name = L"userName_TextBox";
			this->userName_TextBox->Size = System::Drawing::Size(110, 20);
			this->userName_TextBox->TabIndex = 46;
			this->userName_TextBox->Text = L"Enter UserName...";
			// 
			// userName_Label
			// 
			this->userName_Label->AutoSize = true;
			this->userName_Label->Location = System::Drawing::Point(24, 27);
			this->userName_Label->Name = L"userName_Label";
			this->userName_Label->Size = System::Drawing::Size(63, 13);
			this->userName_Label->TabIndex = 50;
			this->userName_Label->Text = L"User Name:";
			// 
			// password_Label
			// 
			this->password_Label->AutoSize = true;
			this->password_Label->Location = System::Drawing::Point(26, 52);
			this->password_Label->Name = L"password_Label";
			this->password_Label->Size = System::Drawing::Size(56, 13);
			this->password_Label->TabIndex = 49;
			this->password_Label->Text = L"Password:";
			// 
			// login_Button
			// 
			this->login_Button->Location = System::Drawing::Point(216, 46);
			this->login_Button->Name = L"login_Button";
			this->login_Button->Size = System::Drawing::Size(75, 23);
			this->login_Button->TabIndex = 48;
			this->login_Button->Text = L"Login";
			this->login_Button->UseVisualStyleBackColor = true;
			this->login_Button->Click += gcnew System::EventHandler(this, &GUI::login_Button_Click);
			// 
			// login_GroupBox
			// 
			this->login_GroupBox->BackColor = System::Drawing::SystemColors::GradientInactiveCaption;
			this->login_GroupBox->Controls->Add(this->password_Label);
			this->login_GroupBox->Controls->Add(this->login_Button);
			this->login_GroupBox->Controls->Add(this->password_TextBox);
			this->login_GroupBox->Controls->Add(this->userName_Label);
			this->login_GroupBox->Controls->Add(this->userName_TextBox);
			this->login_GroupBox->Location = System::Drawing::Point(23, 12);
			this->login_GroupBox->Name = L"login_GroupBox";
			this->login_GroupBox->Size = System::Drawing::Size(299, 82);
			this->login_GroupBox->TabIndex = 56;
			this->login_GroupBox->TabStop = false;
			this->login_GroupBox->Text = L"Login:";
			// 
			// console_GroupBox
			// 
			this->console_GroupBox->Controls->Add(this->clearConsole_Button);
			this->console_GroupBox->Controls->Add(this->console_TextBox);
			this->console_GroupBox->Location = System::Drawing::Point(670, 12);
			this->console_GroupBox->Name = L"console_GroupBox";
			this->console_GroupBox->Size = System::Drawing::Size(481, 459);
			this->console_GroupBox->TabIndex = 57;
			this->console_GroupBox->TabStop = false;
			this->console_GroupBox->Text = L"Console:";
			// 
			// debugInformation_GroupBox
			// 
			this->debugInformation_GroupBox->Controls->Add(this->receivedDataCounters_Label);
			this->debugInformation_GroupBox->Controls->Add(this->sensorOverflow_Label);
			this->debugInformation_GroupBox->Controls->Add(this->packetErrorRate_Label);
			this->debugInformation_GroupBox->Controls->Add(this->errorRates_Label);
			this->debugInformation_GroupBox->Controls->Add(this->countNumberOfCharacters_Button);
			this->debugInformation_GroupBox->Controls->Add(this->bytes_TextBox);
			this->debugInformation_GroupBox->Controls->Add(this->blocks_TextBox);
			this->debugInformation_GroupBox->Controls->Add(this->bytes_Label);
			this->debugInformation_GroupBox->Controls->Add(this->countNumberOfCharacters_TextBox);
			this->debugInformation_GroupBox->Controls->Add(this->blocks_Label);
			this->debugInformation_GroupBox->Controls->Add(this->meanOfAxes_Label);
			this->debugInformation_GroupBox->Controls->Add(this->x_TextBox);
			this->debugInformation_GroupBox->Controls->Add(this->z_TextBox);
			this->debugInformation_GroupBox->Controls->Add(this->sw_TextBox);
			this->debugInformation_GroupBox->Controls->Add(this->y_TextBox);
			this->debugInformation_GroupBox->Controls->Add(this->sw_Label);
			this->debugInformation_GroupBox->Controls->Add(this->z_Label);
			this->debugInformation_GroupBox->Controls->Add(this->hw_TextBox);
			this->debugInformation_GroupBox->Controls->Add(this->hw_Label);
			this->debugInformation_GroupBox->Controls->Add(this->y_Label);
			this->debugInformation_GroupBox->Controls->Add(this->x_Label);
			this->debugInformation_GroupBox->Controls->Add(this->packetErrorRate_TextBox);
			this->debugInformation_GroupBox->Location = System::Drawing::Point(670, 479);
			this->debugInformation_GroupBox->Name = L"debugInformation_GroupBox";
			this->debugInformation_GroupBox->Size = System::Drawing::Size(481, 207);
			this->debugInformation_GroupBox->TabIndex = 58;
			this->debugInformation_GroupBox->TabStop = false;
			this->debugInformation_GroupBox->Text = L"Debug Information:";
			// 
			// receivedDataCounters_Label
			// 
			this->receivedDataCounters_Label->AutoSize = true;
			this->receivedDataCounters_Label->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->receivedDataCounters_Label->Location = System::Drawing::Point(6, 127);
			this->receivedDataCounters_Label->Name = L"receivedDataCounters_Label";
			this->receivedDataCounters_Label->Size = System::Drawing::Size(170, 17);
			this->receivedDataCounters_Label->TabIndex = 46;
			this->receivedDataCounters_Label->Text = L"Received Data Counters :";
			// 
			// sensorOverflow_Label
			// 
			this->sensorOverflow_Label->AutoSize = true;
			this->sensorOverflow_Label->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->sensorOverflow_Label->Location = System::Drawing::Point(6, 93);
			this->sensorOverflow_Label->Name = L"sensorOverflow_Label";
			this->sensorOverflow_Label->Size = System::Drawing::Size(120, 17);
			this->sensorOverflow_Label->TabIndex = 45;
			this->sensorOverflow_Label->Text = L"Sensor Overflow :";
			// 
			// packetErrorRate_Label
			// 
			this->packetErrorRate_Label->AutoSize = true;
			this->packetErrorRate_Label->Location = System::Drawing::Point(170, 63);
			this->packetErrorRate_Label->Name = L"packetErrorRate_Label";
			this->packetErrorRate_Label->Size = System::Drawing::Size(95, 13);
			this->packetErrorRate_Label->TabIndex = 37;
			this->packetErrorRate_Label->Text = L"Packet Error Rate:";
			// 
			// errorRates_Label
			// 
			this->errorRates_Label->AutoSize = true;
			this->errorRates_Label->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->errorRates_Label->Location = System::Drawing::Point(6, 59);
			this->errorRates_Label->Name = L"errorRates_Label";
			this->errorRates_Label->Size = System::Drawing::Size(89, 17);
			this->errorRates_Label->TabIndex = 36;
			this->errorRates_Label->Text = L"Error Rates :";
			// 
			// commandLineInterface_GroupBox
			// 
			this->commandLineInterface_GroupBox->Controls->Add(this->commandScript_Label);
			this->commandLineInterface_GroupBox->Controls->Add(this->scriptFile_Label);
			this->commandLineInterface_GroupBox->Controls->Add(this->scriptFile_TextBox);
			this->commandLineInterface_GroupBox->Controls->Add(this->browse_Button);
			this->commandLineInterface_GroupBox->Controls->Add(this->sendCommands_Button);
			this->commandLineInterface_GroupBox->Controls->Add(this->sendCommands_TextBox);
			this->commandLineInterface_GroupBox->Location = System::Drawing::Point(23, 479);
			this->commandLineInterface_GroupBox->Name = L"commandLineInterface_GroupBox";
			this->commandLineInterface_GroupBox->Size = System::Drawing::Size(642, 207);
			this->commandLineInterface_GroupBox->TabIndex = 59;
			this->commandLineInterface_GroupBox->TabStop = false;
			this->commandLineInterface_GroupBox->Text = L"Command Line Interface:";
			// 
			// sessionTimeout_Timer
			// 
			this->sessionTimeout_Timer->Interval = 1000;
			this->sessionTimeout_Timer->Tick += gcnew System::EventHandler(this, &GUI::sessionTimeout_Timer_Tick);
			// 
			// outputFile_start_AppTab_ToolTip
			// 
			this->outputFile_start_AppTab_ToolTip->AutoPopDelay = 10000;
			this->outputFile_start_AppTab_ToolTip->InitialDelay = 100;
			this->outputFile_start_AppTab_ToolTip->ReshowDelay = 100;
			// 
			// server_Timer
			// 
			this->server_Timer->Interval = 5000;
			this->server_Timer->Tick += gcnew System::EventHandler(this, &GUI::server_Timer_Tick);
			// 
			// GUI
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->AutoSize = true;
			this->BackColor = System::Drawing::SystemColors::ButtonFace;
			this->ClientSize = System::Drawing::Size(1172, 742);
			this->Controls->Add(this->commandLineInterface_GroupBox);
			this->Controls->Add(this->debugInformation_GroupBox);
			this->Controls->Add(this->console_GroupBox);
			this->Controls->Add(this->login_GroupBox);
			this->Controls->Add(this->connection_GroupBox);
			this->Controls->Add(this->connectionTime_TextBox);
			this->Controls->Add(this->allTabs);
			this->Icon = (cli::safe_cast<System::Drawing::Icon^  >(resources->GetObject(L"$this.Icon")));
			this->Name = L"GUI";
			this->Text = L"Wisdom Stone Application v 1.05";
			this->Load += gcnew System::EventHandler(this, &GUI::GUI_Load);
			this->tabSwitch->ResumeLayout(false);
			this->switchTab->ResumeLayout(false);
			this->readLL_SwitchTab_TabPage->ResumeLayout(false);
			this->readLL_SwitchTab_TabPage->PerformLayout();
			this->tabBuzzer->ResumeLayout(false);
			this->buzzerTab->ResumeLayout(false);
			this->writeLL_BuzzerTab_TabPage->ResumeLayout(false);
			this->writeLL_BuzzerTab_TabPage->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->beepDuration_writeLL_BuzzerTab_NumericUpDown))->EndInit();
			this->tabLed->ResumeLayout(false);
			this->ledTab->ResumeLayout(false);
			this->setLed_LedTab_TabPage->ResumeLayout(false);
			this->setLed_LedTab_TabPage->PerformLayout();
			this->tabAccelerometer->ResumeLayout(false);
			this->accelerometerTab->ResumeLayout(false);
			this->config_AccmtrTab_TabPage->ResumeLayout(false);
			this->config_AccmtrTab_TabPage->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->option_config_AccmtrTab_NumericUpDown))->EndInit();
			this->readLL_AccmtrTab_TabPage->ResumeLayout(false);
			this->readLL_AccmtrTab_TabPage->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->address_readLL_AccmtrTab_NumericUpDown))->EndInit();
			this->writeLL_AccmtrTab_TabPage->ResumeLayout(false);
			this->writeLL_AccmtrTab_TabPage->PerformLayout();
			this->tabTemperature->ResumeLayout(false);
			this->temperatureTab->ResumeLayout(false);
			this->getTemperature_TempTab_TabPage->ResumeLayout(false);
			this->getTemperature_TempTab_TabPage->PerformLayout();
			this->readLL_TempTab_TabPage->ResumeLayout(false);
			this->readLL_TempTab_TabPage->PerformLayout();
			this->writeLL_TempTab_TabPage->ResumeLayout(false);
			this->writeLL_TempTab_TabPage->PerformLayout();
			this->tabWakeUp->ResumeLayout(false);
			this->wakeUpTab->ResumeLayout(false);
			this->setTime_WakeUpTab_TabPage->ResumeLayout(false);
			this->setTime_WakeUpTab_TabPage->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->second_setTime_WakeUpTab_NumericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->minute_setTime_WakeUpTab_NumericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->hour_setTime_WakeUpTab_NumericUpDown))->EndInit();
			this->setDate_WakeUpTab_TabPage->ResumeLayout(false);
			this->setDate_WakeUpTab_TabPage->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->weekDay_setDate_WakeUpTab_NumericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->year_setDate_WakeUpTab_NumericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->month_setDate_WakeUpTab_NumericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->day_setDate_WakeUpTab_NumericUpDown))->EndInit();
			this->getTime_WakeUpTab_TabPage->ResumeLayout(false);
			this->getTime_WakeUpTab_TabPage->PerformLayout();
			this->getDate_WakeUpTab_TabPage->ResumeLayout(false);
			this->getDate_WakeUpTab_TabPage->PerformLayout();
			this->setWakeUp_WakeUpTab_TabPage->ResumeLayout(false);
			this->setWakeUp_WakeUpTab_TabPage->PerformLayout();
			this->setAlarm_WakeUpTab_TabPage->ResumeLayout(false);
			this->setAlarm_WakeUpTab_TabPage->PerformLayout();
			this->setBoot_WakeUpTab_TabPage->ResumeLayout(false);
			this->setBoot_WakeUpTab_TabPage->PerformLayout();
			this->readLL_WakeUpTab_TabPage->ResumeLayout(false);
			this->readLL_WakeUpTab_TabPage->PerformLayout();
			this->writeLL_WakeUpTab_TabPage->ResumeLayout(false);
			this->writeLL_WakeUpTab_TabPage->PerformLayout();
			this->tabLCD->ResumeLayout(false);
			this->lcdTab->ResumeLayout(false);
			this->clearScreen_LcdTab_TabPage->ResumeLayout(false);
			this->clearScreen_LcdTab_TabPage->PerformLayout();
			this->writeLL_LcdTab_TabPage->ResumeLayout(false);
			this->writeLL_LcdTab_TabPage->PerformLayout();
			this->tabFlash->ResumeLayout(false);
			this->flashTab->ResumeLayout(false);
			this->format_FlashTab_TabPage->ResumeLayout(false);
			this->format_FlashTab_TabPage->PerformLayout();
			this->initialize_FlashTab_TabPage->ResumeLayout(false);
			this->initialize_FlashTab_TabPage->PerformLayout();
			this->cardDetect_FlashTab_TabPage->ResumeLayout(false);
			this->cardDetect_FlashTab_TabPage->PerformLayout();
			this->writeSector_FlashTab_TabPage->ResumeLayout(false);
			this->writeSector_FlashTab_TabPage->PerformLayout();
			this->readSector_FlashTab_TabPage->ResumeLayout(false);
			this->readSector_FlashTab_TabPage->PerformLayout();
			this->getCapacity_FlashTab_TabPage->ResumeLayout(false);
			this->getCapacity_FlashTab_TabPage->PerformLayout();
			this->readLL_FlashTab_TabPage->ResumeLayout(false);
			this->readLL_FlashTab_TabPage->PerformLayout();
			this->writeLL_FlashTab_TabPage->ResumeLayout(false);
			this->writeLL_FlashTab_TabPage->PerformLayout();
			this->tabApplication->ResumeLayout(false);
			this->applicationTab->ResumeLayout(false);
			this->start_AppTab_TabPage->ResumeLayout(false);
			this->start_AppTab_TabPage->PerformLayout();
			this->applicationMode_GroupBox->ResumeLayout(false);
			this->applicationMode_GroupBox->PerformLayout();
			this->shutdown_AppTab_TabPage->ResumeLayout(false);
			this->shutdown_AppTab_TabPage->PerformLayout();
			this->config_AppTab_TabPage->ResumeLayout(false);
			this->config_AppTab_TabPage->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->value_config_AppTab_NumericUpDown))->EndInit();
			this->tabSystem->ResumeLayout(false);
			this->systemTab->ResumeLayout(false);
			this->systemGetVersion_SystemTab_TabPage->ResumeLayout(false);
			this->systemGetVersion_SystemTab_TabPage->PerformLayout();
			this->systemSetId_SystemTab_TabPage->ResumeLayout(false);
			this->systemSetId_SystemTab_TabPage->PerformLayout();
			this->getPower_SystemTab_TabPage->ResumeLayout(false);
			this->getPower_SystemTab_TabPage->PerformLayout();
			this->setPower_SystemTab_TabPage->ResumeLayout(false);
			this->setPower_SystemTab_TabPage->PerformLayout();
			this->getLastError_SystemTab_TabPage->ResumeLayout(false);
			this->getLastError_SystemTab_TabPage->PerformLayout();
			this->tabHelp->ResumeLayout(false);
			this->helpTab->ResumeLayout(false);
			this->introduction_HelpTab_TabPage->ResumeLayout(false);
			this->introduction_HelpTab_TabPage->PerformLayout();
			this->instruction_HelpTab_TabPage->ResumeLayout(false);
			this->instruction_HelpTab_TabPage->PerformLayout();
			this->allTabs->ResumeLayout(false);
			this->tabEEPROM->ResumeLayout(false);
			this->eepromTab->ResumeLayout(false);
			this->format_EepromTab_TabPage->ResumeLayout(false);
			this->format_EepromTab_TabPage->PerformLayout();
			this->read_EepromTab_TabPage->ResumeLayout(false);
			this->read_EepromTab_TabPage->PerformLayout();
			this->write_EepromTab_TabPage->ResumeLayout(false);
			this->write_EepromTab_TabPage->PerformLayout();
			this->setBoot_EepromTab_TabPage->ResumeLayout(false);
			this->setBoot_EepromTab_TabPage->PerformLayout();
			this->getBoot_EepromTab_TabPage->ResumeLayout(false);
			this->getBoot_EepromTab_TabPage->PerformLayout();
			this->readLL_EepromTab_TabPage->ResumeLayout(false);
			this->readLL_EepromTab_TabPage->PerformLayout();
			this->writeLL_EepromTab_TabPage->ResumeLayout(false);
			this->writeLL_EepromTab_TabPage->PerformLayout();
			this->tabADC->ResumeLayout(false);
			this->ADCTab->ResumeLayout(false);
			this->readLL_AdcTab_TabPage->ResumeLayout(false);
			this->readLL_AdcTab_TabPage->PerformLayout();
			this->writeLL_AdcTab_TabPage->ResumeLayout(false);
			this->writeLL_AdcTab_TabPage->PerformLayout();
			this->tabHub->ResumeLayout(false);
			this->hubTab->ResumeLayout(false);
			this->connection_GroupBox->ResumeLayout(false);
			this->connection_GroupBox->PerformLayout();
			this->login_GroupBox->ResumeLayout(false);
			this->login_GroupBox->PerformLayout();
			this->console_GroupBox->ResumeLayout(false);
			this->console_GroupBox->PerformLayout();
			this->debugInformation_GroupBox->ResumeLayout(false);
			this->debugInformation_GroupBox->PerformLayout();
			this->commandLineInterface_GroupBox->ResumeLayout(false);
			this->commandLineInterface_GroupBox->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

/***A C C M T R***/			 
	private: System::Void	read_readLL_AccmtrTab_Button_Click(System::Object^ sender, System::EventArgs^ e); 			 
	private: System::Void	updateValue_config_AccmtrTab_Button_Click(System::Object^ sender, System::EventArgs^ e); 
	private: System::Void	option_config_AccmtrTab_NumericUpDown_ValueChanged(System::Object^ sender, System::EventArgs^ e);	 
	private: System::Void	write_writeLL_AccmtrTab_Button_Click(System::Object^ sender, System::EventArgs^ e); 
	
/***A D C***/	
	private: System::Void	read_readLL_AdcTab_Button_Click(System::Object^ sender, System::EventArgs^ e);
	private: System::Void	write_writeLL_AdcTab_Button_Click(System::Object^ sender, System::EventArgs^ e);

/***A P P L I C A T I O N***/		 
	private: System::Void	detectAppStart();		 
	private: System::Void	ss_RadioButton_Click(System::Object^ sender, System::EventArgs^ e); 			 
	private: System::Void	ts_RadioButton_Click(System::Object^ sender, System::EventArgs^ e); 			 
	private: System::Void	ost_RadioButton_Click(System::Object^ sender, System::EventArgs^ e); 			 
	private: System::Void	startApplication_AppTab_Button_Click(System::Object^ sender, System::EventArgs^ e); 
	private: System::Void	stopApplication_AppTab_Button_Click(System::Object^ sender, System::EventArgs^ e);	 	 
	private: System::Void	updateValue_config_AppTab_Button_Click(System::Object^ sender, System::EventArgs^ e);	 
	private: System::Void	shutdown_AppTab_Button_Click(System::Object^ sender, System::EventArgs^ e); 

/***B U Z Z E R***/			 
	private: System::Void	apply_writeLL_BuzzerTab_Button_Click(System::Object^ sender, System::EventArgs^ e);

/***E E P R O M***/	 
	private: System::Void	read_readLL_EepromTab_Button_Click(System::Object^ sender, System::EventArgs^ e);
	private: System::Void	write_writeLL_EepromTab_Button_Click(System::Object^ sender, System::EventArgs^ e); 		 
	private: System::Void	format_EepromTab_Button_Click(System::Object^ sender, System::EventArgs^ e); 
	private: System::Void	read_EepromTab_Button_Click(System::Object^ sender, System::EventArgs^ e);		 
	private: System::Void	write_EepromTab_Button_Click(System::Object^ sender, System::EventArgs^ e); 
	private: System::Void	apply_getBoot_EepromTab_Button_Click(System::Object^ sender, System::EventArgs^ e);		 
	private: System::Void	apply_setBoot_EepromTab_Button_Click(System::Object^ sender, System::EventArgs^ e); 
		
/***F L A S H***/
	private: System::Void	format_FlashTab_Button_Click(System::Object^ sender, System::EventArgs^ e);			 
	private: System::Void	initialize_FlashTab_Button_Click(System::Object^ sender, System::EventArgs^ e);			 
	private: System::Void	cardDetect_FlashTab_Button_Click(System::Object^ sender, System::EventArgs^ e);			 
	private: System::Void	write_writeSector_FlashTab_Button_Click(System::Object^ sender, System::EventArgs^ e);		 
	private: System::Void	read_readSector_FlashTab_Button_Click(System::Object^ sender, System::EventArgs^ e); 		 
	private: System::Void	getCapacity_FlashTab_Button_Click(System::Object^ sender, System::EventArgs^ e); 
	private: System::Void	read_readLL_FlashTab_Button_Click(System::Object^ sender, System::EventArgs^ e); 			 
	private: System::Void	write_writeLL_FlashTab_Button_Click(System::Object^ sender, System::EventArgs^ e); 

/***L C D***/			 
	private: System::Void	clear_clearScreen_LcdTab_Button_Click(System::Object^ sender, System::EventArgs^ e); 			 
	private: System::Void	write_writeLL_LcdTab_Button_Click(System::Object^ sender, System::EventArgs^ e);	
	
/***L E D***/		 
	private: System::Void	ledOn_setLed_LedTab_RadioButton_Click(System::Object^ sender, System::EventArgs^ e); 			 
	private: System::Void	ledOff_setLed_LedTab_RadioButton_Click(System::Object^ sender, System::EventArgs^ e);	 
	private: System::Void	apply_setLed_LedTab_Button_Click(System::Object^ sender, System::EventArgs^ e); 
			
/***S W I T C H***/		
	private: System::Void	apply_readLL_SwitchTab_Button_Click(System::Object^ sender, System::EventArgs^ e);

/***S Y S T E M***/				 
	private: System::Void	systemGetVersion_SystemTab_Button_Click(System::Object^ sender, System::EventArgs^ e);			 
	private: System::Void	systemSetId_SystemTab_Button_Click(System::Object^ sender, System::EventArgs^ e);	 
	private: System::Void	getPower_SystemTab_Button_Click(System::Object^ sender, System::EventArgs^ e);		 
	private: System::Void	getLastError_SystemTab_Button_Click(System::Object^ sender, System::EventArgs^ e);	 
	private: System::Void	setPower_SystemTab_Button_Click(System::Object^ sender, System::EventArgs^ e);
	
/***T E M P***/			 
	private: System::Void	getTemperature_TempTab_Button_Click(System::Object^ sender, System::EventArgs^ e); 
	private: System::Void	read_readLL_TempTab_Button_Click(System::Object^ sender, System::EventArgs^ e);
	private: System::Void	write_writeLL_TempTab_Button_Click(System::Object^ sender, System::EventArgs^ e); 	
	
/***W A K E U P***/	
	private: System::Void	getTime_WakeUpTab_Button_Click(System::Object^ sender, System::EventArgs^ e);			 
	private: System::Void	setTime_WakeUpTab_Button_Click(System::Object^ sender, System::EventArgs^ e);
	private: System::Void	setDate_WakeUpTab_Button_Click(System::Object^ sender, System::EventArgs^ e); 	 
	private: System::Void	getDate_WakeUpTab_Button_Click(System::Object^ sender, System::EventArgs^ e);			 
	private: System::Void	read_readLL_WakeUpTab_Button_Click(System::Object^ sender, System::EventArgs^ e); 	 
	private: System::Void	write_writeLL_WakeUpTab_Button_Click(System::Object^ sender, System::EventArgs^ e);			 
	private: System::Void	apply_setAlarm_WakeUpTab_Button_Click(System::Object^ sender, System::EventArgs^ e); 		 
	private: System::Void	apply_setWakeUp_WakeUpTab_Button_Click(System::Object^ sender, System::EventArgs^ e);  
	private: System::Void	apply_setBoot_WakeUpTab_Button_Click(System::Object^ sender, System::EventArgs^ e); 

/***U T I L I T I E S***/
	private: void			updateCOMPortList(void);
	private: void			sendReconnectSession();
	private: void			stringToCharArray(String^ from_string, int max_to_char_len, char* to_chars);
	private: System::Void	internet_RadioButton_CheckedChanged(System::Object^ sender, System::EventArgs^ e); 
	private: System::Void	comPort_RadioButton_CheckedChanged(System::Object^ sender, System::EventArgs^ e); 
	private: System::Void	connect_Button_Click(System::Object^ sender, System::EventArgs^ e); 
	private: System::Void	disconnect_Button_Click(System::Object^ sender, System::EventArgs^ e); 
	private: System::Void	serialPort1_DataReceived(System::Object^ sender, System::IO::Ports::SerialDataReceivedEventArgs^ e); 
	private: System::Void	browse_Button_Click(System::Object^ sender, System::EventArgs^ e); 			 
	private: System::Void	sendCommands_Button_Click(System::Object^ sender, System::EventArgs^ e); 
	private: System::Void	clearConsole_Button_Click(System::Object^ sender, System::EventArgs^ e);			 
	private: System::Void	countNumberOfCharacters_Button_Click(System::Object^ sender, System::EventArgs^ e);			 		 
	private: System::Void	GUI_Load(System::Object^ sender, System::EventArgs^ e);     
	private: System::Void	login_Button_Click(System::Object^ sender, System::EventArgs^ e);	 
	private: System::Void	updateAppParametersWithOffsets();

/***T I M E R 1 COM ports***/	
	private: System::Void	comPorts_Timer_Tick(System::Object^ sender, System::EventArgs^ e);

/***T I M E R 2 connection***/		
	private: System::Void	connection_Timer_Tick(System::Object^ sender, System::EventArgs^ e);

/***T I M E R 3 commands***/		
	private: System::Void	commands_Timer_Tick(System::Object^ sender, System::EventArgs^ e);

/***T I M E R 4 statistics***/
	private: System::Void	statistics_Timer_Tick(System::Object^ sender, System::EventArgs^ e);
	private: System::Void	meanAndStatsUpdate(int block_num, String^ sampleFileName);
	private: double			mean(array<int> ^tmp_axis, int length);	 
	private: void			twosCompement(Byte* MSB, Byte* LSB);			 
	private: int			getSign(Byte number);	
	private: void			removeLsb(Byte* MSB, Byte* LSB);		 

/***T I M E R 5 application***/	
	private: System::Void	application_Timer_Tick(System::Object^ sender, System::EventArgs^ e);

/***T I M E R 6 session timeout***/
	private: System::Void	sessionTimeout_Timer_Tick(System::Object^ sender, System::EventArgs^ e);

/***T I M E R 7 server***/
	private: System::Void	server_Timer_Tick(System::Object^  sender, System::EventArgs^  e);
};
}
