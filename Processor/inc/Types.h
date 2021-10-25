#ifndef TYPES_H
#define TYPES_H

#include <string.h>

typedef char* c_string;

struct MyString
{
	c_string string;
	size_t len;
};
#endif // !TYPES_H
