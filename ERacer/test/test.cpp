#include <stdio.h>
#include "test.h"

void Test::hello(char*name)
{
	printf("Hi %s!\n", name);
}
int Test::getn()
{
	return n;
}
