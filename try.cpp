#include <stdio.h>
#include <vector>
#include <iostream>
using namespace std;

int main()
{
  unsigned addr = 7331500;
  addr = addr >>6;
  cout << addr << endl;
  unsigned temp = addr & 63;
  cout << temp << endl;
  addr = addr >> 6;
  cout << addr << endl;
}
