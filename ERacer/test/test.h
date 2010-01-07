#ifdef SWIG
%module test
%{
#include "test.h"
%}
#endif


class Test
{
  private: // Python can't see this
	static const int priv = 38;
  public:  // But it can see all of these
    int n;
	Test():n(0) { } 
	int getn();
	void hello(char* name);
};

