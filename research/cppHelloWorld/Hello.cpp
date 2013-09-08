#include "Hello.h"
#include <iostream>
using namespace std;

void Hello::greeting()
{
  if (formal)
    cout << "Hello, nice to meet you!" << endl;
  else 
    cout << "What's up?" << endl;
}

void Hello::setFormal(bool f)
{
  formal = f;
}

bool Hello::getFormal()
{
  return formal;
}