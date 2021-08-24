/*
 * MDDOs.cpp
 *
 *  Created on: 13 de abr de 2021
 *      Author: administrador
 */

#include "MDDOs.h"


extern IWDG_HandleTypeDef hiwdg;

int state;

MDDOs::MDDOs() {
	// TODO Auto-generated constructor stub

}

MDDOs::~MDDOs() {
	// TODO Auto-generated destructor stub
}
// Registra tarefa com periodo de execução predefnido
MDDOs::OS_STATUS MDDOs::RegisterTask(MDDOs::FunPtr function, const char* function_name, int stack_size) {
	Task t;
	t.funPtr = function;
	t.millis = 0;
	t.stack_size = stack_size; // align 32 ?

	t.stack_bottom = malloc(t.stack_size); // para salvar contexto stack
	if (t.stack_top == nullptr){
		return OS_STATUS::NO_MEMMORY;
	}
	t.stack_top = t.stack_bottom + t.stack_size; // forma correta pois stack cresce para baixo
//		t.stack_top = t.stack_bottom; // undefined behaviour
	t.ID = tarefas.size();
	t.function_name = function_name;
	t.status = STATUS::RUNNING;

//	t.context = 0;
	tarefas.push_back(t);
	return OS_STATUS::OK;

}

void MDDOs::spin() {
	while (status == RUNNING) {
		HAL_IWDG_Refresh(&hiwdg);
		spinOnce();
	}
}



void MDDOs::spinOnce() {

//	for (size_t i = 0; i < tarefas.size(); ++i) {
//		for ( std::list<Task>::i)

	for (task_iterator it = tarefas.begin(); it != tarefas.end(); ++ it)
	{

//		const Task &tarefa = (*it);
		current_task = &(*it);

		state = setjmp(scheduler);
//		state = setjmp(current_task->scheduler_);
		uint32_t currentTime = HAL_GetTick();
		uint32_t delta = currentTime - current_task->lastTime;

		if(delta > current_task->millis){

			if(current_task->status == STATUS::DELAYED){
				// restore context
				longjmp(current_task->context, 1);
			} else {
				register void *sp asm ("sp");
				sp = current_task->stack_top; //para salvar o contexto

				current_task->funPtr(); // executa normalmente
			}

		}



	}

}


