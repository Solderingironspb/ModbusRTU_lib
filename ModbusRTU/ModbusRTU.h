/*
 * ModbusRTU.h
 *
 *  Библиотека для работы по протоколу ModbusRTU в режиме Master.
 *  Функции: формировать запрос, вычислять CRC16.
 *
 *  Created on: Nov 16, 2021
 *      Author: Oleg Volkov
 */

#ifndef INC_MODBUSRTU_H_
#define INC_MODBUSRTU_H_

#include <main.h>

uint16_t ModbusRTU_CRC16_Calculate(uint8_t *data, uint8_t lenght, uint8_t byte_order);
void ModbusRTU_Read_Holding_Registers(uint8_t Slave_ID, uint16_t Read_adress, uint8_t Quantity, uint8_t Slave_byte_order);

#endif /* INC_MODBUSRTU_H_ */
