MeasBuffer::MeasBuffer(unsigned long int _maxSize = 1000000) {
  maxSize = _maxSize;
  offset = 0;
  count = 0;
  firstTime = 0;
  elementSize = sizeof(_maxSize);
  
  buffer.resize(maxSize);
}

unsigned long int MeasBuffer::add(unsigned int raw) {
  offset++;
  if (offset >= maxSize) {
    offset = 0;
  }
  if (offset > count) {
    count = offset;
    lastTimeForCount = mTime();
  }
  if (firstTime == 0) {
    firstTime = mTime();
  }
  lastTime = mTime();
  
  buffer[offset] = raw;
  
  return offset;
}

unsigned long int MeasBuffer::calcInterval() {
  if (offset == 0) {
    return 1; // not to fuck up all this shit somewhere
  } else {
    unsigned long long timeCount = lastTimeForCount - firstTime;
    return timeCount / count;
  }
}

unsigned int MeasBuffer::at(unsigned long int i) {
  return buffer[index(i)];
}

unsigned long int MeasBuffer::index(unsigned long int i) {
  long int tmpI = offset - i;
  if (tmpI >= 0) {
    return tmpI;
  } else {
    return maxSize + offset - i;
  }
}

unsigned long int MeasBuffer::memorySize() {
  return buffer.capacity() * elementSize;
}

bool MeasBuffer::stored(unsigned long int i) {
  if (i>count) {
    return false;
  } else {
    return true;
  }
}

vector < unsigned int > MeasBuffer::getFromBuffer(unsigned long int from, unsigned long int to) {
  unsigned long int i;
  vector < unsigned int > result;
  
  if (from <= to) {
    // safety
    if (to >= maxSize) {
      to = maxSize - 1;
    }
    
    logInfo("MeasBuffer: UP getFromBuffer(" + to_string(from) + ", " + to_string(to) + ")");
    
    for (i = from; i <= to; i++) {
      if (stored(i)) {
        result.push_back(at(i));
      }
    }
    return result;
  } else {
    // safety
    if (from >= maxSize) {
      from = maxSize - 1;
    }
    
    logInfo("MeasBuffer: DOWN getFromBuffer(" + to_string(from) + ", " + to_string(to) + ")");
    
    for (i = from; i > to; i--) {
      if (stored(i)) {
        result.push_back(at(i));
      }
    }
    return result;
  }
}

string MeasBuffer::jsonArray(unsigned long int from, unsigned long int to) {
  string s = "[";
  vector < unsigned int > tmp = getFromBuffer(from, to);
  for(vector<unsigned int>::iterator it = tmp.begin(); it != tmp.end(); ++it) {
    s += to_string(*it);
    s += ",";
  }
  
  // remove last coma
  if (s[s.size() - 1] == ',') {
    s.resize(s.size() - 1);
  }
  
  s += "]";
  
  return s;
}

string MeasBuffer::toJson() {
  string response;
  response = "{";
  response += "\"count\":" + to_string(count) + ",";
  response += "\"offset\":" + to_string(offset) + ",";
  response += "\"maxSize\":" + to_string(maxSize) + ",";
  response += "\"firstTime\":" + to_string(firstTime);
  response += "}";
  return response;  
}