/*
 * SDCARD.h
 *
 *  Created on: May 21, 2021
 *      Author: daniel
 */

#ifndef INC_SDCARD_H_
#define INC_SDCARD_H_

// HAL
#include "stm32f4xx_hal.h"
#include "ff.h"
#include "string.h"
#include <cstdio>

#define SIZE_PATH 50

typedef struct resultFile{
	int numFile;
	char *pathFile;
	char *dataFile;
}resultFile;

class SDCARD {
public:
	SDCARD();
	virtual ~SDCARD();

	uint32_t count_lines;

	FRESULT write_file(const char *file_name,const char *text,unsigned int str_len);
	FRESULT read_file(const char *file_name, char *read_text);
	FRESULT scan_files(const char *path_dir, char *path_file);
	FRESULT count_files(const char *path_dir, int &count);
	FRESULT delete_file(const char *path);
	FRESULT mount_disk(const char *path);
	FRESULT unmount_disk();


private:
	FATFS fs;
	FIL arquivo;
	FRESULT fsres;
	UINT test_bytes;
	int numFiles = 0;
	char read_char[1];
	char buff_debug[50];
};

#endif /* INC_SDCARD_H_ */
