/*
 * ModbusRTU.c
 *
 *  Библиотека для работы по протоколу ModbusRTU в режиме Master.
 *  Функции: формировать запрос, вычислять CRC16.
 *
 *  Created on: Nov 16, 2021
 *      Author: Oleg Volkov
 */

#ifndef SRC_MODBUSRTU_C_
#define SRC_MODBUSRTU_C_

#include "ModbusRTU.h"

uint8_t ModbusRTU_tx_buffer[64] = { 0, };

/*------------------------------CRC16(ModbusRTU)----------------------------------*/
uint16_t ModbusRTU_CRC16_Calculate(uint8_t *data, uint8_t lenght, uint8_t byte_order) {
///Функция рассчета CRC16.
/// \param *data - массив данных
///	\param lenght - длина массива данных
/// \param byte_order - порядок байт:
///0 - 1234(младшим регистром вперед, младшим байтом вперед),
///1 - 3412(старшим регистром вперед, младшим байтом вперед),
///2 - 2143(младшим регистром вперед, старшим байтом вперед),
///3 - 4321(старшим регистром вперед, старшим байтом вперед).

	uint16_t crc = 0xFFFF;
	while (lenght--) {
		crc ^= *data++;
		for (int i = 0; i < 8; i++) {
			if (crc & 0x01) {
				crc = (crc >> 1u) ^ 0xA001;
			} else {
				crc = crc >> 1u;
			}
		}
	}
	switch (byte_order) {
	case (0): //1234(младшим регистром вперед, младшим байтом вперед),
		break;
	case (1): //3412(старшим регистром вперед, младшим байтом вперед),
		crc = (crc << 8u) | (crc >> 8u);
		break;
	case (2): //2143(младшим регистром вперед, старшим байтом вперед),
		crc = (((crc >> 8u) & 0x0F) << 12u) | ((crc >> 12u) << 8u) | ((crc << 12u) << 4u) | ((crc >> 4u) & 0x00F);
		break;
	case (3): //4321(старшим регистром вперед, старшим байтом вперед).
		crc = (((crc >> 8u) & 0x0F) << 4u) | (crc >> 12u) | ((crc << 12u) << 12u) | (((crc >> 4u) & 0x00F) << 8u);
		break;
	}
	return crc;
}
/*------------------------------CRC16(ModbusRTU)----------------------------------*/

/*------------------------------------Функция 00x01 Read Coils----------------------------------- */
void ModbusRTU_Read_Coils_0x01(uint8_t Slave_ID, uint16_t Read_adress, uint16_t Quantity, uint8_t Slave_byte_order) {
///Функция 0x01 Read Coils
/// \param Slave_ID - ID Modbus RTU Slave устройства, к которому обращаемся
/// \param Read_adress - Адрес, с которого начинаем чтение данных
/// \param Quantity - количество запрашиваемых 16ти битных данных.
	//Quantity должен быть от 1 до 2000.
/// \param Slave_byte_order - Порядок регистра и байт в ModbusRTU Slave устройстве

	ModbusRTU_tx_buffer[0] = Slave_ID;
	ModbusRTU_tx_buffer[1] = 0x01;
	ModbusRTU_tx_buffer[2] = (uint16_t) Read_adress >> 8u;
	ModbusRTU_tx_buffer[3] = (uint16_t) Read_adress & 0x00FF;
	ModbusRTU_tx_buffer[4] = (uint16_t) Quantity >> 8u;
	ModbusRTU_tx_buffer[5] = (uint16_t) Quantity & 0x00FF;
	uint16_t CRC16 = ModbusRTU_CRC16_Calculate(ModbusRTU_tx_buffer, 6, Slave_byte_order);
	ModbusRTU_tx_buffer[6] = (uint16_t) CRC16 >> 8u;
	ModbusRTU_tx_buffer[7] = (uint16_t) CRC16 & 0x00FF;
}
/*------------------------------------Функция 00x01 Read Coils----------------------------------- */

/*--------------------------------Функция 00x02 Read Discrete Inputs----------------------------- */
void ModbusRTU_Read_Discrete_Inputs_0x02(uint8_t Slave_ID, uint16_t Read_adress, uint16_t Quantity, uint8_t Slave_byte_order) {
///Функция 0x02 Read Discrete Inputs
/// \param Slave_ID - ID Modbus RTU Slave устройства, к которому обращаемся
/// \param Read_adress - Адрес, с которого начинаем чтение данных
/// \param Quantity - количество запрашиваемых 16ти битных данных.
	//Quantity должен быть от 1 до 2000.
/// \param Slave_byte_order - Порядок регистра и байт в ModbusRTU Slave устройстве

	ModbusRTU_tx_buffer[0] = Slave_ID;
	ModbusRTU_tx_buffer[1] = 0x02;
	ModbusRTU_tx_buffer[2] = (uint16_t) Read_adress >> 8u;
	ModbusRTU_tx_buffer[3] = (uint16_t) Read_adress & 0x00FF;
	ModbusRTU_tx_buffer[4] = (uint16_t) Quantity >> 8u;
	ModbusRTU_tx_buffer[5] = (uint16_t) Quantity & 0x00FF;
	uint16_t CRC16 = ModbusRTU_CRC16_Calculate(ModbusRTU_tx_buffer, 6, Slave_byte_order);
	ModbusRTU_tx_buffer[6] = (uint16_t) CRC16 >> 8u;
	ModbusRTU_tx_buffer[7] = (uint16_t) CRC16 & 0x00FF;
}
/*--------------------------------Функция 00x02 Read Discrete Inputs----------------------------- */

/*------------------------------Функция 00x03 Read Holding Registers----------------------------- */
void ModbusRTU_Read_Holding_Registers_0x03(uint8_t Slave_ID, uint16_t Read_adress, uint8_t Quantity, uint8_t Slave_byte_order) {
///Функция 0x03 Read Holding Registers
/// \param Slave_ID - ID Modbus RTU Slave устройства, к которому обращаемся
/// \param Read_adress - Адрес, с которого начинаем чтение данных
/// \param Quantity - количество запрашиваемых 16ти битных данных. Если мы запрашиваем Float, то Quantity кратно 2, т.е. 32 бита. 2 Float переменные - соответственно Quantity = 4. И т.д.
	//Quantity должен быть от 1 до 125.
/// \param Slave_byte_order - Порядок регистра и байт в ModbusRTU Slave устройстве

	ModbusRTU_tx_buffer[0] = Slave_ID;
	ModbusRTU_tx_buffer[1] = 0x03;
	ModbusRTU_tx_buffer[2] = (uint16_t) Read_adress >> 8u;
	ModbusRTU_tx_buffer[3] = (uint16_t) Read_adress & 0x00FF;
	ModbusRTU_tx_buffer[4] = 0x00;
	ModbusRTU_tx_buffer[5] = Quantity;
	uint16_t CRC16 = ModbusRTU_CRC16_Calculate(ModbusRTU_tx_buffer, 6, Slave_byte_order);
	ModbusRTU_tx_buffer[6] = (uint16_t) CRC16 >> 8u;
	ModbusRTU_tx_buffer[7] = (uint16_t) CRC16 & 0x00FF;
}
/*------------------------------Функция 00x03 Read Holding Registers----------------------------- */

/*--------------------------------Функция 00x04 Read Input Registers----------------------------- */
void ModbusRTU_Read_Input_Registers_0x04(uint8_t Slave_ID, uint16_t Read_adress, uint8_t Quantity, uint8_t Slave_byte_order) {
///Функция 0x04 Read Discrete Inputs
/// \param Slave_ID - ID Modbus RTU Slave устройства, к которому обращаемся
/// \param Read_adress - Адрес, с которого начинаем чтение данных
/// \param Quantity - количество запрашиваемых 16ти битных данных.
	//Quantity должен быть от 1 до 125.
/// \param Slave_byte_order - Порядок регистра и байт в ModbusRTU Slave устройстве

	ModbusRTU_tx_buffer[0] = Slave_ID;
	ModbusRTU_tx_buffer[1] = 0x04;
	ModbusRTU_tx_buffer[2] = (uint16_t) Read_adress >> 8u;
	ModbusRTU_tx_buffer[3] = (uint16_t) Read_adress & 0x00FF;
	ModbusRTU_tx_buffer[4] = 0x00;
	ModbusRTU_tx_buffer[5] = Quantity;
	uint16_t CRC16 = ModbusRTU_CRC16_Calculate(ModbusRTU_tx_buffer, 6, Slave_byte_order);
	ModbusRTU_tx_buffer[6] = (uint16_t) CRC16 >> 8u;
	ModbusRTU_tx_buffer[7] = (uint16_t) CRC16 & 0x00FF;
}
/*--------------------------------Функция 00x04 Read Input Registers----------------------------- */

#endif /* SRC_MODBUSRTU_C_ */
