/*
 * Application.h
 *
 *  Created on: Feb 15, 2021
 *      Author: marcus
 */

#ifndef SRC_APPLICATION_H_
#define SRC_APPLICATION_H_

#include "main.h"
#include "stdio.h"


class Application {
public:

	static void Run();

private:

	Application();
	virtual ~Application();


};

// Wrapper para ser chamado via C
extern "C" void Run(Application* App){
App->Run();
}

#endif /* SRC_APPLICATION_H_ */
