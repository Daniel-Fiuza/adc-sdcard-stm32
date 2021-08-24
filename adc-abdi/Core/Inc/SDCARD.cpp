/*
 * SDCARD.cpp
 *
 *  Created on: May 21, 2021
 *      Author: daniel
 */

#include <SDCARD.h>

SDCARD::SDCARD() {
	fs = { 0 };
}

SDCARD::~SDCARD() {

}


FRESULT SDCARD::delete_file(const char *path) {
	fsres = f_unlink(path);
	return fsres;
}

FRESULT SDCARD::count_files(const char *path_dir, int &count) {
	DIR dir;
	static FILINFO fno;
	numFiles = 0;
	fsres = f_opendir(&dir, path_dir); /* Abre diretorio */
	if (fsres == FR_OK) {
		for (;;) {
			fsres = f_readdir(&dir, &fno); /* Ler um item do diretorio */
			if (fsres != FR_OK || fno.fname[0] == 0) {
				break;
			} else {
				numFiles++;
//				break;
			}
		}
		count = numFiles;
	}
	f_closedir(&dir);
	return fsres; /* Retorna fsres */
}

FRESULT SDCARD::scan_files(const char *path_dir, char *filename_found) {
	DIR dir;
	static FILINFO fno;

	fsres = f_opendir(&dir, path_dir); /* Abre diretorio */
	if (fsres == FR_OK) {
		for (;;) {
			fsres = f_readdir(&dir, &fno); /* Ler um item do diretorio */
			if (fsres != FR_OK || fno.fname[0] == 0) {
				f_closedir(&dir);
				return fsres; /* Para em um erro ou fim do diretorio */
			}
			if (fno.fattrib & AM_DIR) { /* É um diretorio */
				f_closedir(&dir);
				return FR_NO_FILE;
			} else {
				//i = strlen(path_dir);
				//sprintf(&filename_found[i], "%s/%s", path_dir, fno.fname); /* Diretorio do arquivo encontrado */
				sprintf(filename_found, "%s/%s", path_dir, fno.fname);
				break;
			}
		}
	}
	f_closedir(&dir);
	return fsres; /* Retorna fsres */
}

FRESULT SDCARD::write_file(const char *file_name, const char *text,unsigned int str_len) {

	fsres = f_open(&arquivo, file_name, FA_WRITE | FA_CREATE_ALWAYS);

	if (fsres == FR_NO_PATH) { // Cria pasta
		fsres = f_mkdir("LOGS");
		if (fsres != FR_OK)
			return fsres;
		else
			fsres = f_open(&arquivo, file_name, FA_WRITE | FA_CREATE_ALWAYS);

	}

	fsres = f_write(&arquivo, text, str_len, &test_bytes);

	if (fsres != FR_OK) { // avaliar isso aqui
		f_close(&arquivo);
		return fsres;
	}

//	numFiles++;

	return f_close(&arquivo);
}

FRESULT SDCARD::read_file(const char *file_name, char *read_text) {
	uint32_t i = 0;
	fsres = f_open(&arquivo, file_name, FA_READ | FA_OPEN_ALWAYS);

	if (fsres != FR_OK)
		return fsres;

	while (1) {
		fsres = f_read(&arquivo, read_char, 1, &test_bytes);
		if (read_char[0] == '\0' || read_char[0] == '\r' || read_char[0] == '\n')
			break;

		if (fsres || !test_bytes)
			break;

		read_text[i++] = read_char[0];

	}

	if (fsres != FR_OK) {
		f_close(&arquivo);
		return fsres;
	}

	f_close(&arquivo);
	return fsres;
}

FRESULT SDCARD::mount_disk(const char *path) {

	return f_mount(&fs, path, 1);
}

FRESULT SDCARD::unmount_disk(){
	return f_mount(0,"/",1);
}
