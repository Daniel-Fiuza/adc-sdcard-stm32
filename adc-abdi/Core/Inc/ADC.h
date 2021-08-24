/*
 * ADC.h
 *
 *  Created on: Apr 20, 2021
 *      Author: daniel
 */
#ifndef INC_ADC_H_
#define INC_ADC_H_

// HAL
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_adc.h"
#include "FlashHelper.h"
//#include "stm32f4xx_hal_adc_ex.h"

extern ADC_HandleTypeDef hadc1;

/* API para gestão dos sinais ADC do sensor de combustivel */

class ADCManager {
public:
	ADCManager(ADC_HandleTypeDef *hadc_, uint32_t flash_offset_);
	virtual ~ADCManager();

	// Fator de conversão
	inline void setFactor(float factor_val) {
		factor = factor_val;
	}


	// realiza conversão
	void convert();

	// define canal de conversão
	void setChannel(uint32_t channel, uint8_t rank);

	// Aplica filtro
	void filter();

	// saida sem filtro
	uint16_t getRaw();

	// saida filtrada
	uint16_t getFiltered();

	// saida filtrada em volts
	float getVolts();


	// Saida filtrada em percentual dentro do minmax
	uint16_t getPercentage(bool inverted = false);

	// Atualiza minmax (chamar sempre)
	void UpdateMinMax();

	// Escreve minmax na FLASH (Executar ocasionalmente)
	void writeToMemory();

	// chamar no botão de autocalibração
	inline void resetMinMax(){
		min_val = 65535;
		max_val = 0;
	}

	inline void getMinMax(uint16_t &min, uint16_t &max){
		min = min_val;
		max = max_val;
	}

private:
	ADC_HandleTypeDef *adc_handle; // handle para hardware
	uint32_t flash_offset; //offset de uso da flash

	uint16_t adc_last_raw_value; // ultimo valor lido
	uint16_t adc_last_filtered_value; // ultimo valor filtrado
	float factor = 0.0; // fator de conversão
	const uint16_t RESOLUTION = 4096;
	const float VMAX = 3.22;
	const float alpha = 0.1; // coeficiente de filtragem

	// minmax RAM
	uint16_t max_val;
	uint16_t min_val;

	void readFromMemory(); // feito no reset

};

#endif /* INC_ADC_H_ */
