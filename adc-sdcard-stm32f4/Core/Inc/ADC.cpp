/*
 * ADC.cpp
 *
 *  Created on: Apr 20, 2021
 *      Author: daniel
 */

#include <ADC.h>

ADCManager::ADCManager(ADC_HandleTypeDef *hadc_, uint32_t flash_offset_) :
		adc_handle(hadc_), flash_offset(flash_offset_) {
	// Recebe valores da flash
//	readFromMemory();
}

ADCManager::~ADCManager() {
	// TODO Auto-generated destructor stub
}

/* Método para converter leitura e armazenar em variável da classe
 * Esse método deve ser chamado antes de qualquer outro que envolva o uso da leitura
 * i.e getVolts, getFiltered
 */
void ADCManager::convert() {
	HAL_ADC_Start(adc_handle);
	HAL_ADC_PollForConversion(adc_handle, 1);
	adc_last_raw_value = HAL_ADC_GetValue(adc_handle);
}

void ADCManager::setChannel(uint32_t channel, uint8_t rank) {
	ADC_ChannelConfTypeDef sConfig;
	sConfig.Channel = channel;
	sConfig.Rank = rank;
	sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;

	HAL_ADC_ConfigChannel(adc_handle, &sConfig);
}

// Aplica filtro
void ADCManager::filter() {
	static uint16_t last_conversion = 0;
	uint16_t current_conversion = adc_last_raw_value;
	float filtered = alpha * (float) current_conversion
			+ (1 - alpha) * (float) last_conversion;
	adc_last_filtered_value = (uint16_t) filtered;
	last_conversion = (uint16_t) filtered;

}

// recebe saida em volts
float ADCManager::getVolts() {

//	return (adc_last_raw_value * factor);
	return (adc_last_filtered_value * factor);
//		return (adc_last_filtered_value * VMAX) / RESOLUTION;
}

uint16_t ADCManager::getRaw() {
	return adc_last_raw_value;
}

uint16_t ADCManager::getFiltered() {
	return adc_last_filtered_value;
}

// Retorna porcentagem do valor lido dentro do range MinMax (0 - 100 , int)
uint16_t ADCManager::getPercentage(bool inverted) {
	if (inverted) {
		float pct = ((float) max_val - (float) adc_last_filtered_value)
						/ (float) (max_val - min_val);
		return (uint16_t) (pct * 100);
	} else {
		float pct = ((float) adc_last_filtered_value - (float) min_val)
					/ (float) (max_val - min_val);
		return (uint16_t) (pct * 100);
	}

}

// Atualiza minmax RAM
void ADCManager::UpdateMinMax() {
	if (adc_last_filtered_value > max_val) {
		max_val = adc_last_filtered_value;
	}
	if (adc_last_filtered_value < min_val) {
		min_val = adc_last_filtered_value;

	}

}

/* Os dois métodos abaixo podem ser mudados, mas preservando a lógica de atualizar os valores min max
 * Podemos por exemplo usar a memória RAM do RTC em vez da flash
 */

// lê minmax da flash
void ADCManager::readFromMemory() {

	FlashHelper::Read(flash_offset, min_val);
	FlashHelper::Read(flash_offset + sizeof(uint16_t), max_val);
}

// Escreve minmax na flash
void ADCManager::writeToMemory() {
	FlashHelper::Write(flash_offset, min_val);
	FlashHelper::Write(flash_offset + sizeof(uint16_t), max_val);
}

