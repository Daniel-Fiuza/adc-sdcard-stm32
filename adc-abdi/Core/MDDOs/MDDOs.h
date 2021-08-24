/*
 * MDDOs.h
 *
 *  Created on: 13 de abr de 2021
 *      Author: administrador
 */

#ifndef MDDOS_MDDOS_H_
#define MDDOS_MDDOS_H_

#include <stm32f4xx_hal.h>
#include <stm32f4xx_hal_iwdg.h>

#include <cstdint>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <setjmp.h>
#include <list>
#include <vector>

/* Autoress: Marcus-Daniel-Denilson Operating System
 * Data: 03/03/2021
 */

/* Funções importantes
 * Registrar tarefa + periodicidade
 * Método Yield() -> retorna controle para OS
 * Método Delay()
 */

class MDDOs {

public:
	using FunPtr = void (*)();
	typedef enum {
		RUNNING = 1, STOPPED = 2, DELAYED = 3
	} STATUS;

	typedef enum {
		OK = 0, NO_MEMMORY = 1
	} OS_STATUS ;

	struct Task {
		FunPtr funPtr;
		uint32_t millis;
		uint32_t lastTime;
		STATUS status;

		const char *function_name;
		jmp_buf context;
		jmp_buf scheduler_;

		void *stack_top;
		void *stack_bottom;
		uint32_t stack_size;

		int ID;
	};


//	using task_iterator = std::vector<Task>::iterator;
	using task_iterator = std::list<Task>::iterator;


	MDDOs();

//	void RegisterTask(FunPtr, const char *function_name); // executa sempre
	OS_STATUS RegisterTask(FunPtr, const char *function_name, int stack_size = 256); // executa sempre

	// Procura as proximas execuções
	void spin();
	void spinOnce();

	inline void setPeriod(uint32_t milis){
		current_task->lastTime = HAL_GetTick();
		current_task->millis = milis;
	}

	// start OS
	inline void start() {
		status = STATUS::RUNNING;
	}

	// Stop OS
	inline void stop() {
		status = STATUS::STOPPED;
	}

	inline void sleep(uint32_t milis) {

		// salva contexto
		int i = setjmp(current_task->context);
		if (i == 1) {
			current_task->status = STATUS::RUNNING;

			//vem do scheduler após o delay
			return;



		} else if (i == 0) { // estabelece delay
			// salva contexto
			current_task->status = STATUS::DELAYED;
			current_task->millis = milis;
			current_task->lastTime = HAL_GetTick();
//			longjmp(current_task->scheduler_, STATUS::DELAYED);
			longjmp(scheduler, STATUS::DELAYED);
		} else {
			// ??

		}
//		tarefas
	}

	inline void yield() {
		// vai embora
		longjmp(scheduler, 10);
	}

	virtual ~MDDOs();
private:

//	Task* tarefas;
//	std::vector<Task> tarefas;
	std::list<Task> tarefas;
	Task *current_task;
	STATUS status = MDDOs::STATUS::STOPPED;
	jmp_buf scheduler;

};

#endif /* MDDOS_MDDOS_H_ */
