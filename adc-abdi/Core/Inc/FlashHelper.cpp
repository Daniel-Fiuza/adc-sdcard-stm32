/*
 * FlashHelper.cpp
 *
 *  Created on: May 11, 2021
 *      Author: Marcus
 */

#include <FlashHelper.h>

HAL_StatusTypeDef FlashHelper::Write(const uint32_t offset,
		const uint16_t &data) {

	HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR1 + offset, data);

	//	HAL_FLASH_Unlock();
////	HAL_FLASH_OB_Unlock();
//
//	FLASH_EraseInitTypeDef config;
//	config.TypeErase = FLASH_TYPEERASE_SECTORS;
//	config.NbSectors = 1;
//	config.Sector = FLASH_SECTOR_11; // TODO confirmar
//	config.VoltageRange = FLASH_VOLTAGE_RANGE_3;
//	uint32_t eraseError;
//	// TODO rotina Write
//	HAL_FLASHEx_Erase(&config, &eraseError);
//	HAL_StatusTypeDef status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, FINAL_FLASH_ADDR_USER + offset, data);
//
//
////	HAL_FLASH_OB_Lock();
//	HAL_FLASH_Lock();
//	return status;
}

// Funciona bem
HAL_StatusTypeDef FlashHelper::Read(const uint32_t offset, uint16_t &data) {
//	data = *(__IO uint32_t*) (FINAL_FLASH_ADDR_USER + offset);
//	HAL_FLASH_Unlock();
//	HAL_FLASH_OB_Unlock();
//
//
//
//	HAL_FLASH_OB_Lock();
//	HAL_FLASH_Lock();
	data = HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR1 + offset);
	return HAL_OK;
}

FlashHelper::FlashHelper() {
	// TODO Auto-generated constructor stub

}

FlashHelper::~FlashHelper() {
	// TODO Auto-generated destructor stub
}

