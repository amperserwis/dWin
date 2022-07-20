/* *******************************************************************************
 *  Copyright (C) 2014-2022 Mehmet Gunce Akkoyun Can not be copied and/or
 *	distributed without the express permission of Mehmet Gunce Akkoyun.
 *
 *	Library				: dWin HMI LCD Screen Library.
 *	Code Developer		: Mehmet Gunce Akkoyun (akkoyun@me.com)
 *	Code Developer		: Recep Senbas (recepsenbas@gmail.com)
 *
 *********************************************************************************/

#ifndef __dWin__
#define __dWin__

// Define Arduino Library
#ifndef __Arduino__
	#include <Arduino.h>
#endif

// Include Libraries
#include "dWin_Definitions.h"

class dwin {

	private:

		/**
		 * @brief Register Definition Variable
		 */
		struct Register {
			const uint8_t High_Address;
			const uint8_t Low_Address;
		};

		/**
		 * @brief Color Definition Variable
		 */
		struct Color {
			const uint8_t High_Address;
			const uint8_t Low_Address;
		};

		/**
		 * @brief LCD Variables Structure.
		 */
		struct Variables_Struct {

			/**
			 * @brief Serial Communication Object Definition.
			 */
			Stream * HMI_Serial;

			/**
			 * @brief Enable LCD Variable.
			 */
			bool LCD_Enable;

			/**
			 * @brief Color Variables.
			 */
			struct Color_Struct {

				/**
				 * @brief Color Registers.
				 */
				Color Black {0x00, 0x00};
				Color White {0xFF, 0xFF};
				Color Red {0xF8, 0x00};
				Color Green {0x07, 0x40};
				Color Gray {0xAD, 0x55};

			} Colors;

			/**
			 * @brief Register Variables.
			 */
			struct Register_Struct {

				/**
				 * @brief LCD Reset Register.
				 */
				Register Reset_Register 				{0x00, 0x04};

				/**
				 * @brief LCD Sleep Register.
				 */
				Register Sleep_Register 				{0x00, 0x82};

				/**
				 * @brief Time Stamp Register.
				 */
				Register Time_Stamp_Register 			{0x00, 0x10};

				/**
				 * @brief Device Status Register.
				 */
				Register Device_Status_Register 		{0x60, 0x48};

				/**
				 * @brief Firmware Register.
				 */
				Register Firmware_Register 				{0x70, 0x60};

				/**
				 * @brief Voltage Color Registers.
				 */
				Register Voltage_R_Color_Register 		{0x80, 0x13};
				Register Voltage_S_Color_Register 		{0x80, 0x23};
				Register Voltage_T_Color_Register 		{0x80, 0x33};

				/**
				 * @brief Voltage Registers.
				 */
				Register Voltage_R_Register 			{0x60, 0x32};
				Register Voltage_S_Register 			{0x60, 0x34};
				Register Voltage_T_Register 			{0x60, 0x36};

				/**
				 * @brief Frequency Register.
				 */
				Register Frequency_Register 			{0x60, 0x10};

				/**
				 * @brief Frequency Color Register.
				 */
				Register Frequency_Color_Register 		{0x70, 0x73};

				/**
				 * @brief Power Factor Register.
				 */
				Register PowerFactor_Register 			{0x60, 0x12};

				/**
				 * @brief Power Consumption Register.
				 */
				Register PowerConsumption_Register 		{0x60, 0x46};

				/**
				 * @brief Current Registers.
				 */
				Register Current_R_Register 			{0x61, 0x38};
				Register Current_S_Register 			{0x61, 0x40};
				Register Current_T_Register 			{0x61, 0x42};

				/**
				 * @brief Pressure Register.
				 */
				Register Pressure_Register 				{0x60, 0x30};

				/**
				 * @brief Pressure Color Register.
				 */
				Register Pressure_Color_Register 		{0x80, 0x83};

				/**
				 * @brief Battery Registers
				 */
				Register Battery_Icon_Register 			{0x60, 0x02};
				Register Battery_Voltage_Register 		{0x60, 0x50};
				Register Battery_Current_Register 		{0x60, 0x54};
				Register Battery_SOC_Register 			{0x60, 0x56};
				Register Battery_Voltage_Color_Register {0x70, 0xB3};
				Register Battery_Current_Color_Register {0x81, 0x03};

				/**
				 * @brief GSM Registers.
				 */
				Register GSM_Icon_Register 				{0x60, 0x01};
				Register GSM_Send_Icon_Register 		{0x60, 0x09};
				Register GSM_RSSI_Register 				{0x60, 0x70};
				Register GSM_Operator_Register 			{0x60, 0x72};
				Register GSM_ConnectionTime_Register 	{0x60, 0x68};
				Register GSM_IMEI_Register 				{0x70, 0x10};
				Register GSM_ICCID_Register 			{0x70, 0x30};
				Register GSM_IP_Register 				{0x70, 0x40};

			} Registers;

		} Variables;
		
		/**
		 * @brief Limit Control Function
		 * @param _Value Value to control
		 * @param _Min Minimum value
		 * @param _Max Maximum value
		 * @return true Value out of limit
		 * @return false Value in limit
		 */
		bool Limit_Control(float _Value, float _Min, float _Max) {

			// Control Minimum Limit
			if (_Value < _Min) return(true);
			
			// Control Maximum Limit
			if (_Value > _Max) return(true);

			// End Function
			return(false);

		}

		/**
		 * @brief Text Color Set Function
		 * @param _Register Text Register
		 * @param _Color Color Register
		 */
		void Set_Color(Register _Register, Color _Color) {

			// Set Pack Header
			this->Variables.HMI_Serial->write(0x5A);
			this->Variables.HMI_Serial->write(0xA5);
			this->Variables.HMI_Serial->write(0x05);
			this->Variables.HMI_Serial->write(0x82);
			this->Variables.HMI_Serial->write(_Register.High_Address);
			this->Variables.HMI_Serial->write(_Register.Low_Address);
			this->Variables.HMI_Serial->write(_Color.High_Address);
			this->Variables.HMI_Serial->write(_Color.Low_Address);

			// Command Delay
			delay(100);
			
		}

		/**
		 * @brief Write Register Function.
		 * @param _Register Register Address
		 * @param _Data Value to Write.
		 */
		void Write_Register(Register _Register, uint8_t *_Data) {

			// Set Pack Header
			this->Variables.HMI_Serial->write(0x5A);
			this->Variables.HMI_Serial->write(0xA5);
			this->Variables.HMI_Serial->write(0x05);
			this->Variables.HMI_Serial->write(0x82);
			this->Variables.HMI_Serial->write(_Register.High_Address);
			this->Variables.HMI_Serial->write(_Register.Low_Address);
			this->Variables.HMI_Serial->write(_Data[0]);
			this->Variables.HMI_Serial->write(_Data[1]);

			// Command Delay
			delay(100);
			
		}
	
		/**
		 * @brief dWin data pack send function.
		 * @param _Register Register address
		 * @param _Data_Type Data type
		 * @param _Data Data array
		 * @param _Size Data array size
		 * @return true 
		 * @return false 
		 */
		void Write_Register_Long(Register _Register, uint8_t *_Data, uint8_t _Size) {

			// Declare Length
			uint8_t _Pack_Size = 3;

			// Set Pack Header
			this->Variables.HMI_Serial->write(0x5A);
			this->Variables.HMI_Serial->write(0xA5);
			this->Variables.HMI_Serial->write(_Size + _Pack_Size);
			this->Variables.HMI_Serial->write(0x82);
			this->Variables.HMI_Serial->write(_Register.High_Address);
			this->Variables.HMI_Serial->write(_Register.Low_Address);

			// Send Data Pack
			for (size_t i = 0; i < _Size; i++) this->Variables.HMI_Serial->write(_Data[i]);

			// Command Delay
			delay(100);
			
		}

	public:

		/**
		 * @brief Construct a new dwin object
		 */
		dwin(bool _LCD_Enable) {

			// Set Variable
			this->Variables.LCD_Enable = _LCD_Enable;

		}

		/**
		 * @brief HMI Start function.
		 * @param _Serial Serial connection variable
		 */
		void Begin(Stream &_Serial) {

			// Set serial port
			this->Variables.HMI_Serial = &_Serial;

		}

		/**
		 * @brief LCD reset function.
		 */
		void Reset(void) {

			// Declare Default Data Array
			uint8_t Data[4] = {0x55, 0xAA, 0x5A, 0xA5};

			// Write Data
			this->Write_Register_Long(this->Variables.Registers.Reset_Register, Data, 4);

		}

		/**
		 * @brief LCD Sleep enable function.
		 * @param _Status Sleep status.
		 */
		void Sleep(bool _Status) {

			// Declare Variables
			uint8_t Data[4];

			// Data[0] - ON State Brightnes
			Data[0] = 0x64;

			// Data[1] - Sleep State Brightnes
			if (_Status) Data[1] = 0x64;
			if (!_Status) Data[1] = 0x00;
			
			// Data[2] - Sleep Time
			Data[2] = 0x0B;

			// Data[3] - Sleep Time
			Data[3] = 0xB8;

			// Write Data
			this->Write_Register_Long(this->Variables.Registers.Sleep_Register, Data, 4);

		}

		/**
		 * @brief Time stamp function.
		 */
		void Time_Stamp(uint8_t _Day, uint8_t _Month, uint8_t _Year, uint8_t _Hour, uint8_t _Minute, uint8_t _Second) {

			// Declare Default Data Array
			uint8_t Data[8];

			// Set Array Values
			Data[0] = _Year;
			Data[1] = _Month;
			Data[2] = _Day;
			Data[4] = _Hour;
			Data[5] = _Minute;
			Data[6] = _Second;

			// Write Data
			this->Write_Register_Long(this->Variables.Registers.Time_Stamp_Register, Data, 8);

		}

		/**
		 * @brief Status function.
		 * @param _State Status value
		 */
		void Status(uint16_t _State) {

			// Declare Default Data Array
			uint8_t Data[2] = {0x00, 0x00};

			// Set Data Low/High Byte
			Data[1] = (_State & (uint16_t)0x00FF);
			Data[0] = (_State & (uint16_t)0xFF00) >> 8;

			// Write Data
			this->Write_Register(this->Variables.Registers.Device_Status_Register, Data);

		}

		/**
		 * @brief Device Firmware Function.
		 * @param _Firmware Firmware value
		 */
		void Firmware(char * _Firmware) {

			// Declare Data
			uint8_t Data[8];

			// Convert char to uint array
			for (size_t i = 0; i < 8; i++) Data[i] = uint8_t(_Firmware[i]);

			// Write Data
			this->Write_Register_Long(this->Variables.Registers.Firmware_Register, Data, 16);

		}

		/**
		 * @brief HMI voltage display function.
		 * @param _Phase Phase id (1:R - 2:S - 3:T)
		 * @param _Value Phase value
		 * @version 01.00.00
		 * @return true Function OK
		 * @return false Function Fail
		 */
		bool Voltage(uint8_t _Phase, float _Value) {

			// Control for LCD Enable
			if (this->Variables.LCD_Enable) {

				// Control Phase
				if (_Phase != __Voltage_R__ and _Phase != __Voltage_S__ and _Phase != __Voltage_T__) return(false);

				// Declare Default Data Array
				uint8_t Data[2] = {0x00, 0x00};

				// Convert Value
				uint16_t _Value_RAW = uint16_t(_Value * __Voltage_Precision__);

				// Set Data Low/High Byte
				Data[1] = (_Value_RAW & (uint16_t)0x00FF);
				Data[0] = (_Value_RAW & (uint16_t)0xFF00) >> 8;

				// Control Value Limit
				if (this->Limit_Control(_Value, __Voltage_Min_Limit__, __Voltage_Max_Limit__)) {

					if (_Phase == __Voltage_R__) this->Set_Color(this->Variables.Registers.Voltage_R_Color_Register, this->Variables.Colors.Red);
					if (_Phase == __Voltage_S__) this->Set_Color(this->Variables.Registers.Voltage_S_Color_Register, this->Variables.Colors.Red);
					if (_Phase == __Voltage_T__) this->Set_Color(this->Variables.Registers.Voltage_T_Color_Register, this->Variables.Colors.Red);
					
				} else {

					if (_Phase == __Voltage_R__) this->Set_Color(this->Variables.Registers.Voltage_R_Color_Register, this->Variables.Colors.White);
					if (_Phase == __Voltage_S__) this->Set_Color(this->Variables.Registers.Voltage_R_Color_Register, this->Variables.Colors.White);
					if (_Phase == __Voltage_T__) this->Set_Color(this->Variables.Registers.Voltage_R_Color_Register, this->Variables.Colors.White);

				}

				// Write Register Value
				if (_Phase == __Voltage_R__) this->Write_Register(this->Variables.Registers.Voltage_R_Register, Data);
				if (_Phase == __Voltage_S__) this->Write_Register(this->Variables.Registers.Voltage_S_Register, Data);
				if (_Phase == __Voltage_T__) this->Write_Register(this->Variables.Registers.Voltage_T_Register, Data);

				// End Function
				return(true);

			}

			// End Function
			return(false);
			
		}

		/**
		 * @brief HMI Current Display Function.
		 * @param _Phase Phase id (1:R - 2:S - 3:T)
		 * @param _Value Phase value
		 * @version 01.00.00
		 * @return true Function OK
		 * @return false Function Fail
		 */
		bool Current(uint8_t _Phase, float _Value) {

			// Control for LCD Enable
			if (this->Variables.LCD_Enable) {

				// Control Phase
				if (_Phase != __Current_R__ and _Phase != __Current_S__ and _Phase != __Current_T__) return(false);

				// Declare Default Data Array
				uint8_t Data[2] = {0x00, 0x00};

				// Convert Value
				uint16_t _Value_RAW = uint16_t(_Value * __Current_Precision__);

				// Set Data Low/High Byte
				Data[1] = (_Value_RAW & (uint16_t)0x00FF);
				Data[0] = (_Value_RAW & (uint16_t)0xFF00) >> 8;

				// Write Register Value
				if (_Phase == __Current_R__) this->Write_Register(this->Variables.Registers.Current_R_Register, Data);
				if (_Phase == __Current_S__) this->Write_Register(this->Variables.Registers.Current_S_Register, Data);
				if (_Phase == __Current_T__) this->Write_Register(this->Variables.Registers.Current_T_Register, Data);

				// End Function
				return(true);

			}

			// End Function
			return(false);
			
		}

		/**
		 * @brief HMI frequency display function.
		 * @param _Value Phase value
		 */
		void Frequency(float _Value) {

			// Declare Default Data Array
			uint8_t Data[2] = {0x00, 0x00};

			// Convert Value
			uint16_t _Value_RAW = uint16_t(_Value * __Frequency_Precision__);

			// Set Data Low/High Byte
			Data[1] = (_Value_RAW & (uint16_t)0x00FF);
			Data[0] = (_Value_RAW & (uint16_t)0xFF00) >> 8;

			// Control Value Limit
			if (this->Limit_Control(_Value, __Frequency_Min_Limit__, __Frequency_Max_Limit__)) {

				this->Set_Color(this->Variables.Registers.Frequency_Color_Register, this->Variables.Colors.Red);
				
			} else {

				this->Set_Color(this->Variables.Registers.Frequency_Color_Register, this->Variables.Colors.White);

			}

			// Write Data
			this->Write_Register(this->Variables.Registers.Frequency_Register, Data);

		}

		/**
		 * @brief HMI power factor display function.
		 * @version 01.00.01
		 * @param _Value Phase value
		 */
		void PowerFactor(float _Value) {

			// Declare Default Data Array
			uint8_t Data[2] = {0x00, 0x00};

			// Convert Value
			uint16_t _Value_RAW = uint16_t(_Value * __PowerFactor_Precision__);

			// Handle Negative
			if (_Value > 0) {

				// Set Data Low/High Byte
				Data[1] = (_Value_RAW & (uint16_t)0x00FF);
				Data[0] = (_Value_RAW & (uint16_t)0xFF00) >> 8;


			} else {

				// Set negative Value
				_Value_RAW = 0xFFFF & (~_Value_RAW + 1);

				// Set Data Low/High Byte
				Data[1] = (_Value_RAW & (uint16_t)0x00FF);
				Data[0] = (_Value_RAW & (uint16_t)0xFF00) >> 8;

			}

			// Write Data
			this->Write_Register(this->Variables.Registers.PowerFactor_Register, Data);

		}

		/**
		 * @brief HMI power consumption display function.
		 * @param _Value Phase value
		 */
		void PowerConsumption(uint16_t _Value) {

			// Declare Default Data Array
			uint8_t Data[2] = {0x00, 0x00};

			// Convert Value
			uint16_t _Value_RAW = uint16_t(_Value);

			// Set Data Low/High Byte
			Data[1] = (_Value_RAW & (uint16_t)0x00FF);
			Data[0] = (_Value_RAW & (uint16_t)0xFF00) >> 8;

			// Write Data
			this->Write_Register(this->Variables.Registers.PowerConsumption_Register, Data);

		}

		/**
		 * @brief HMI pressure display function.
		 * @param _Value Pressure value
		 */
		void Pressure(float _Value) {

			// Declare Default Data Array
			uint8_t Data[2] = {0x00, 0x00};

			// Convert Value
			uint16_t _Value_RAW = uint16_t(_Value * __Pressure_Precision__);

			// Set Data Low/High Byte
			Data[1] = (_Value_RAW & (uint16_t)0x00FF);
			Data[0] = (_Value_RAW & (uint16_t)0xFF00) >> 8;

			// Control Value Limit
			if (this->Limit_Control(_Value, 0, __Pressure_Max_Limit__)) {

				this->Set_Color(this->Variables.Registers.Pressure_Color_Register, this->Variables.Colors.Red);
				
			} else {

				this->Set_Color(this->Variables.Registers.Pressure_Color_Register, this->Variables.Colors.White);

			}

			// Write Data
			this->Write_Register(this->Variables.Registers.Pressure_Register, Data);

		}

		/**
		 * @brief Battery detail function.
		 * @param _Level Battery level
		 * @param _IV Instant voltage
		 * @param _AC Average current
		 * @param _SOC State of charge
		 */
		void Battery(uint8_t _Level, float _IV, float _AC, float _SOC) {

			// Set Battery Icon
			this->Battery_Icon(_Level);

			// Set IV Value
			this->Battery_Voltage(_IV);

			// Set AC Value
			this->Battery_Current(_AC);

			// Set SOC Value
			this->Battery_SOC(_SOC);

		}

		/**
		 * @brief HMI battery icon display function.
		 * @param _Level Battery value
		 */
		void Battery_Icon(uint8_t _Level) {

			// Declare Default Data Array
			uint8_t Data[2] = {0x00, 0x00};

			// Set Data Array
			if (_Level == 0) Data[1] = 0x00;
			if (_Level == 1) Data[1] = 0x01;
			if (_Level == 2) Data[1] = 0x02;
			if (_Level == 3) Data[1] = 0x03;
			if (_Level == 4) Data[1] = 0x04;
			if (_Level == 5) Data[1] = 0x05;

			// Write Data
			this->Write_Register(this->Variables.Registers.Battery_Icon_Register, Data);

		}

		/**
		 * @brief HMI battery voltage display function.
		 * @param _Value voltage value
		 */
		void Battery_Voltage(float _Value) {

			// Declare Default Data Array
			uint8_t Data[2] = {0x00, 0x00};

			// Convert Value
			uint16_t _Value_RAW = uint16_t(_Value * __Battery_Voltage_Precision__);

			// Set Data Low/High Byte
			Data[1] = (_Value_RAW & (uint16_t)0x00FF);
			Data[0] = (_Value_RAW & (uint16_t)0xFF00) >> 8;

			// Control Value Limit
			if (this->Limit_Control(_Value, __Battery_Voltage_Min_Limit__, __Battery_Voltage_Max_Limit__)) {

				this->Set_Color(this->Variables.Registers.Battery_Voltage_Color_Register, this->Variables.Colors.Red);
				
			} else {

				this->Set_Color(this->Variables.Registers.Battery_Voltage_Color_Register, this->Variables.Colors.White);

			}

			// Write Data
			this->Write_Register(this->Variables.Registers.Battery_Voltage_Register, Data);

		}

		/**
		 * @brief HMI battery current display function.
		 * @param _Value Current value
		 */
		void Battery_Current(float _Value) {

			// Declare Default Data Array
			uint8_t Data[2] = {0x00, 0x00};

			// Convert Value
			uint16_t _Value_RAW = uint16_t(_Value * __Battery_Current_Precision__);

			// Set Data Low/High Byte
			Data[1] = (_Value_RAW & (uint16_t)0x00FF);
			Data[0] = (_Value_RAW & (uint16_t)0xFF00) >> 8;

			// Control Value Limit
			if (this->Limit_Control(_Value, __Battery_Current_Min_Limit__, __Battery_Current_Max_Limit__)) {

				this->Set_Color(this->Variables.Registers.Battery_Current_Color_Register, this->Variables.Colors.Red);
				
			} else {

				this->Set_Color(this->Variables.Registers.Battery_Current_Color_Register, this->Variables.Colors.White);

			}

			// Write Data
			this->Write_Register(this->Variables.Registers.Battery_Current_Register, Data);

		}

		/**
		 * @brief HMI battery SOC display function.
		 * @param _Value SOC value
		 */
		void Battery_SOC(float _Value) {

			// Declare Default Data Array
			uint8_t Data[2] = {0x00, 0x00};

			// Convert Value
			uint16_t _Value_RAW = uint16_t(_Value * __Battery_SOC_Precision__);

			// Set Data Low/High Byte
			Data[1] = (_Value_RAW & (uint16_t)0x00FF);
			Data[0] = (_Value_RAW & (uint16_t)0xFF00) >> 8;

			// Write Data
			this->Write_Register(this->Variables.Registers.Battery_SOC_Register, Data);

		}

		/**
		 * @brief GSM details page function
		 * @param _Level Signal level
		 * @param _Manufacturer Manufacturer
		 * @param _Model Model
		 * @param _Conn Connection time
		 * @param _RSSI RSSI
		 * @param _Operator Operator
		 * @param _Firmware Firmware
		 * @param _IMEI IMEI
		 * @param _Serial Serial
		 * @param _ICCID ICCID
		 * @param _IP IP
		 */
		void GSM(uint8_t _Level, uint16_t _Conn, uint16_t _RSSI, uint16_t _Operator, char * _IMEI, char * _ICCID, char * _IP) {

			// Set Data Set
			this->GSM_Icon(_Level);
			this->GSM_Connection_Time(_Conn);
			this->GSM_RSSI(_RSSI);
			this->GSM_Operator(_Operator);
			this->GSM_IMEI(_IMEI);
			this->GSM_ICCID(_ICCID);
			this->GSM_IP(_IP);

		}

		/**
		 * @brief HMI GSM icon display function.
		 * @param _Level GSM signal value
		 */
		void GSM_Icon(uint8_t _Level) {

			// Declare Default Data Array
			uint8_t Data[2] = {0x00, 0x00};

			// Set Data Array
			if (_Level == 0) Data[1] = 0x00;
			if (_Level == 1) Data[1] = 0x01;
			if (_Level == 2) Data[1] = 0x02;
			if (_Level == 3) Data[1] = 0x03;
			if (_Level == 4) Data[1] = 0x04;
			if (_Level == 5) Data[1] = 0x05;
			if (_Level == 6) Data[1] = 0x06;

			// Write Data
			this->Write_Register(this->Variables.Registers.GSM_Icon_Register, Data);

		}

		/**
		 * @brief HMI GSM data send display function.
		 * @param _Level Send progress value
		 */
		void GSM_Send(uint8_t _Level) {

			// Declare Default Data Array
			uint8_t Data[2] = {0x00, 0x00};

			// Set Data Array
			if (_Level == 0) Data[1] = 0x00; // Blank
			if (_Level == 1) Data[1] = 0x01; // Sending
			if (_Level == 2) Data[1] = 0x02; // Clock

			// Write Data
			this->Write_Register(this->Variables.Registers.GSM_Send_Icon_Register, Data);

		}

		/**
		 * @brief HMI GSM RSSI display function.
		 * @param _Level GSM RSSI value
		 */
		void GSM_RSSI(uint16_t _Value) {

			// Declare Default Data Array
			uint8_t Data[2] = {0x00, 0x00};

			// Convert Value
			uint16_t _Value_RAW = uint16_t(_Value);

			// Set Data Low/High Byte
			Data[1] = (_Value_RAW & (uint16_t)0x00FF);
			Data[0] = (_Value_RAW & (uint16_t)0xFF00) >> 8;

			// Write Data
			this->Write_Register(this->Variables.Registers.GSM_RSSI_Register, Data);

		}

		/**
		 * @brief HMI GSM operator display function.
		 * @param _Level GSM operator value
		 */
		void GSM_Operator(uint16_t _Value) {

			// Declare Default Data Array
			uint8_t Data[2] = {0x00, 0x00};

			// Convert Value
			uint16_t _Value_RAW = uint16_t(_Value);

			// Set Data Low/High Byte
			Data[1] = (_Value_RAW & (uint16_t)0x00FF);
			Data[0] = (_Value_RAW & (uint16_t)0xFF00) >> 8;

			// Write Data
			this->Write_Register(this->Variables.Registers.GSM_Operator_Register, Data);

		}

		/**
		 * @brief HMI GSM connection time display function.
		 * @param _Level GSM connection time value
		 */
		void GSM_Connection_Time(uint16_t _Value) {

			// Declare Default Data Array
			uint8_t Data[2] = {0x00, 0x00};

			// Convert Value
			uint16_t _Value_RAW = uint16_t(_Value);

			// Set Data Low/High Byte
			Data[1] = (_Value_RAW & (uint16_t)0x00FF);
			Data[0] = (_Value_RAW & (uint16_t)0xFF00) >> 8;

			// Write Data
			this->Write_Register(this->Variables.Registers.GSM_ConnectionTime_Register, Data);

		}

		/**
		 * @brief GSM IMEI function.
		 * @param _IMEI GSM IMEI value
		 */
		void GSM_IMEI(char * _IMEI) {

			// Declare Data
			uint8_t Data[20];

			// Convert char to uint array
			for (size_t i = 0; i < 20; i++) Data[i] = uint8_t(_IMEI[i]);

			// Write Data
			this->Write_Register(this->Variables.Registers.GSM_IMEI_Register, Data);

		}

		/**
		 * @brief GSM ICCID function.
		 * @param _ICCID GSM ICCID value
		 */
		void GSM_ICCID(char * _ICCID) {

			// Declare Data
			uint8_t Data[20];

			// Convert char to uint array
			for (size_t i = 0; i < 20; i++) Data[i] = uint8_t(_ICCID[i]);

			// Write Data
			this->Write_Register_Long(this->Variables.Registers.GSM_ICCID_Register, Data, 20);

		}

		/**
		 * @brief GSM IP function.
		 * @param _IP GSM IP value
		 */
		void GSM_IP(char * _IP) {

			// Declare Data
			uint8_t Data[16] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

			// Convert char to uint array
			for (size_t i = 0; i < 16; i++) Data[i] = uint8_t(_IP[i]);

			// Write Data
			this->Write_Register_Long(this->Variables.Registers.GSM_IP_Register, Data, 16);

		}

};

#endif /* defined(__dWin_Arduino__) */