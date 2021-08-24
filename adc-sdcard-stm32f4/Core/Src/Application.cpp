/*
 * Application.cpp
 */

#include "Application.h"

// SDCard
#include "SDCARD.h"
#include "fatfs.h"
#include "ff_gen_drv.h"

// MDDOS
#include "MDDOs.h"

// ADC
#include "ADC.h"

// Estruturas Hardware
extern UART_HandleTypeDef huart3;
extern UART_HandleTypeDef huart1;
extern RTC_HandleTypeDef hrtc;
extern RTC_DateTypeDef date;
extern RTC_TimeTypeDef time;
extern I2C_HandleTypeDef hi2c1;
extern ADC_HandleTypeDef hadc1;
extern SD_HandleTypeDef hsd;

// MDD Operating System
MDDOs OS;

// SDCARD
SDCARD sdcard;

// Prototipo Funcoes
void error_handle(int);
void getNameFile(RTC_HandleTypeDef*, char*);

// variaveis globais / compartilhadas
bool g_saveSD = false; // armazenar dados no SD CARD
float g_vbat;
bool unlink_result = false; // gerencia unmount do sd card

// OS rodando.. Testa Malloc
void PiscaLED() {
//	static int count = 0;
	HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_RESET);
	OS.sleep(100);
	HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_SET);
	OS.sleep(1000);
}

//void PiscaLED2() {
//	HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
//	OS.sleep(50);
//}

void SDCARDLoop() {
	static FRESULT fres, fres_mount = FR_INVALID_DRIVE;
	char file_name[50] = "";
	char write_text[150] = "";
	static bool initSDCard = false;

	if (initSDCard == false) {
		if (unlink_result) {
			MX_FATFS_Init();
			unlink_result = false;
		}

		fres_mount = sdcard.mount_disk("0:/"); // TODO bloqueia se nao der certo

		if (fres_mount == FR_OK) {
			int count_files = 0;
			sdcard.count_files("LOGS", count_files);
			printf("Montagem OK. # de arquivos: %d\r\n", count_files);
			initSDCard = true;
		} else {
			if (!unlink_result) {
				FATFS_UnLinkDriver("0:/");
				unlink_result = true;
			}

			printf("Problema ao montar SDCard. INFO: %d\r\n", fres_mount);
			initSDCard = false;
			OS.sleep(5000);
			return;
		}
	} else { // cartão OK aqui
		getNameFile(&hrtc, file_name);
		sprintf(write_text, "%f", g_vbat);
		fres = sdcard.write_file(file_name, write_text,sizeof(write_text));
		if (fres != FR_OK) {
			f_mount(0, "", 0);
			initSDCard = false;
			printf("Erro ao escrever arquivo: %s\r\n", file_name);
		} else {
			printf("Escrito com sucesso no arquivo: %s\r\n", file_name);
		}
	}

	OS.sleep(5000);
}

void ADCLoop() {
	static ADCManager adc(&hadc1, 0); // memoria flash é lida aqui
	static bool init = false;
	uint16_t adc_value;

	if(!init){
		init = true;
	}

//	adc.setChannel(ADC_CHANNEL_1, 1);
	adc.convert();
	adc.filter();
	adc.setFactor((float) (13.425/4096));	// ADC_resolucao / (Vmax - Vmin)
	adc_value = adc.getFiltered();
	g_vbat = adc.getVolts();
	printf("[ADC] value: %d, vbat: %f\r\n",adc_value, g_vbat);

	OS.sleep(1000);

}

void Application::Run() {

	printf("Definindo tarefas\r\n");
	OS.RegisterTask(PiscaLED, "led", 4096);
//	OS.RegisterTask(PiscaLED2, "led2", 1024); // DEBUG
	OS.RegisterTask(SDCARDLoop, "sdcard", 8192);
	OS.RegisterTask(ADCLoop, "adc", 1024);

	OS.start();

	OS.spin();

	while (1)
		;
}

void getNameFile(RTC_HandleTypeDef *hrtc, char *file_name) {
	RTC_DateTypeDef date;
	RTC_TimeTypeDef time;

	HAL_RTC_GetTime(hrtc, &time, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(hrtc, &date, RTC_FORMAT_BIN);

	sprintf(file_name, "LOGS/%d_%d_%d_%d_%d_%d.txt", date.Date, date.Month,
			date.Year, time.Hours, time.Minutes, time.Seconds);
}

void error_handle(int time) {
	while (1) {
		HAL_GPIO_TogglePin(LED0_GPIO_Port, LED0_Pin);
		HAL_Delay(time);
	}
}
//
//
//Application::Application() {
//	// TODO Auto-generated constructor stub
//
//}
//
//Application::~Application() {
//	// TODO Auto-generated destructor stub
//}

