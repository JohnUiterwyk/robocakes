#include "Hello.h"

int main()
{
  Hello *h = new Hello();
  h->setFormal(true);
  h->greeting();
  delete h;
  return 0;
}