/*
 * FlashHelper.h
 *
 *  Created on: May 11, 2021
 *      Author: Marcus
 */


/* Essa classe é dedicada para escrita e leitura de variáveis na memória flash de forma facilitada
 * Aqui se utiliza os utimos 1023 bytes da memória flash (supõe-se que está livre)*/

#ifndef INC_FLASHHELPER_H_
#define INC_FLASHHELPER_H_

#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_flash.h"

extern RTC_HandleTypeDef hrtc; // TODO BAD


// FLASH    (rx)    : ORIGIN = 0x8000000,   LENGTH = 1024K
#define FINAL_FLASH_ADDR_USER (0x80fffff - 0x3ff) // OK

class FlashHelper {
public:

	static HAL_StatusTypeDef Write(const uint32_t offset, const uint16_t& data);
	static HAL_StatusTypeDef Read(const uint32_t offset, uint16_t& data);
	static inline uint32_t getFlashOrigin(){
		return FINAL_FLASH_ADDR_USER;
	}
private:
	// Singleton.. N há constructor acessível
	FlashHelper();
	virtual ~FlashHelper();
};

#endif /* INC_FLASHHELPER_H_ */
