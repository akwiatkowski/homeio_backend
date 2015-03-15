#ifndef ACTION_TYPE_H
#define	ACTION_TYPE_H

#include <iostream>
#include <string>
#include <vector>
#include <time.h>

#include "io_proxy.hpp"

using namespace std;

class ActionType {
public:
  ActionType();
  unsigned int execute();
  
  string name; // name of action
  char command; // at this moment only 1 byte commands
  char responseSize; // amount of bytes returned from uC, normally 1 or 2
  unsigned int responseOkay; // value returned from uC when everything is okay
  
  unsigned long int executionCount;
  
  IoProxy *ioProxy;
	
};

#include "action_type.cpp"
#endif	