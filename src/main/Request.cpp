#include "Request.h"



Request::Request(int floor, Dir dir) : floor_(floor), dir_(dir) {}


int Request::floor() const 
{ 
  return floor_; 
}


Dir Request::direction() const 
{ 
  return dir_; 
}
