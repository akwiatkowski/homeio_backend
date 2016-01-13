#ifndef MEAS_TYPE_ARRAY
#define	MEAS_TYPE_ARRAY

#include <iostream>
#include <vector>
#include "meas_type.hpp"
#include "io/io_proxy.hpp"

using namespace std;

class MeasTypeArray {
public:
  MeasTypeArray();
  unsigned int add(MeasType *m);
  MeasType *byName(std::string s);

  vector <MeasType> measTypes;
  IoProxy *ioProxy;
  unsigned int i;

  // used by other utils like overseers, storage, to
  // delay them
  bool isReady;
  void delayTillReady();
};

//#include "meas_type_array.cpp"
#endif
