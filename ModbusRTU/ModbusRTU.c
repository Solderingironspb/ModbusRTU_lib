/*
 * ModbusRTU.c
 *
 *  Created on: Nov 16, 2021
 *      Author: Oleg Volkov
 */

#ifndef SRC_MODBUSRTU_C_
#define SRC_MODBUSRTU_C_

#include "ModbusRTU.h"

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

#endif /* SRC_MODBUSRTU_C_ */
