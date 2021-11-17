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
				crc = (crc >> 1) ^ 0xA001;
			} else {
				crc = crc >> 1;
			}
		}
	}
	switch (byte_order) {
	case (0): //1234(младшим регистром вперед, младшим байтом вперед),
		break;
	case (1): //3412(старшим регистром вперед, младшим байтом вперед),
		crc = (crc << 8) | (crc >> 8);
		break;
	case (2): //2143(младшим регистром вперед, старшим байтом вперед),
		crc = (((crc >> 8) & 0x0F) << 12) | ((crc >> 12) << 8) | ((crc << 12) << 4) | ((crc >> 4) & 0x00F);
		break;
	case (3): //4321(старшим регистром вперед, старшим байтом вперед).
		crc = (((crc >> 8) & 0x0F) << 4) | (crc >> 12) | ((crc << 12) << 12) | (((crc >> 4) & 0x00F) << 8);
		break;
	}
	return crc;
}
/*------------------------------CRC16(ModbusRTU)----------------------------------*/

/*------------------------------Функция 00x03 Read Analog Output Holding Registers----------------------------- */
void ModbusRTU_Read_Holding_Registers(uint8_t Slave_ID, uint16_t Read_adress, uint8_t Quantity, uint8_t Slave_byte_order) {
///Функция 0x03 Read Holding Registers
/// \param Slave_ID - ID Modbus RTU Slave устройства, к которому обращаемся
/// \param Read_adress - Адрес, с которого начинаем чтение данных
/// \param Quantity - количество запрашиваемых 16ти битных данных. Если мы запрашиваем Float, то Quantity кратно 2, т.е. 32 бита. 2 Float переменные - соответственно Quantity = 4. И т.д.
	//Quantity должен быть от 1 до 125.
/// \param Slave_byte_order - Порядок регистра и байт в ModbusRTU Slave устройстве

	ModbusRTU_tx_buffer[0] = Slave_ID;
	ModbusRTU_tx_buffer[1] = 0x03;
	ModbusRTU_tx_buffer[2] = (uint8_t) (Read_adress >> 8);
	ModbusRTU_tx_buffer[3] = (uint8_t) (Read_adress << 8);
	ModbusRTU_tx_buffer[4] = 0x00;
	ModbusRTU_tx_buffer[5] = Quantity;
	uint16_t CRC16 = ModbusRTU_CRC16_Calculate(ModbusRTU_tx_buffer, 6, Slave_byte_order);
	ModbusRTU_tx_buffer[6] = (uint8_t) (CRC16 >> 8);
	ModbusRTU_tx_buffer[7] = (uint8_t) (CRC16 << 8);
}
/*------------------------------Функция 00x03 Read_adress Analog Output Holding Registers----------------------------- */
#endif /* SRC_MODBUSRTU_C_ */
