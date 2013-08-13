#ifndef HELLO_H_
#define HELLO_H_

class Hello
{
private:
  bool formal;
public:
  void greeting();
  void setFormal(bool f);
  bool getFormal();
};

#endif /*HELLO_H_*/