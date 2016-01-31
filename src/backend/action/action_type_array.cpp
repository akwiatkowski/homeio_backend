#include "action_type_array.hpp"

ActionTypeArray::ActionTypeArray() {
}

ActionTypeArray::~ActionTypeArray() {
  for(std::vector<ActionType>::iterator it = actionTypes.begin(); it != actionTypes.end(); ++it) {
    delete &(*it);
  }
  actionTypes.clear();
}

unsigned int ActionTypeArray::add(ActionType *a) {
  a->logArray = logArray;
  actionTypes.push_back(*a);
  logArray->log("ActionType", "[" + a->name + "] added (" + std::to_string(actionTypes.size()) + " total action types)");
  return 0;
}

ActionType *ActionTypeArray::byName(std::string s) {
  for(std::vector<ActionType>::iterator it = actionTypes.begin(); it != actionTypes.end(); ++it) {
    if (it->name == s) {
      return &*it;
    }
  }
  return NULL;
}
