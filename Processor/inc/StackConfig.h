#ifndef STACKCONFIG_H
#define STACKCONFIG_H
#define createStack(type) createStack_(sizeof(type))

#include <cstddef>
typedef char* ptr_t;

#define PROTECTION_LEVEL 0

#if PROTECTION_LEVEL == 1
	#define ENABLE_PROTECTION(code) code
	#define ENABLE_DUMP(code) code
	#define ENABLE_VALID_FUNCTION(code) code
	#define PRINT_FUNCTION(code) code
	typedef size_t canary_t;
#else
	#define ENABLE_PROTECTION(code)
	#define ENABLE_DUMP(code)
	#define ENABLE_VALID_FUNCTION(code)
	#define PRINT_FUNCTION(code)
#endif

const size_t STACK_BLOCK_CAPACITY = 256;

#if PROTECTION_LEVEL == 1
enum STACK_ERRORS
{
	NO_ERROR					= 0,
	NULL_PTR_ERROR				= 1,
	SIZE_ERROR					= 2,
	ARRAY_PTR_ERROR				= 3,
	HASH_ERROR					= 4,
	LEFT_CANARY_ERROR			= 5,
	RIGHT_CANARY_ERROR			= 6,
	ARRAY_LEFT_CANARY_ERROR		= 7,
	ARRAY_RIGHT_CANARY_ERROR	= 8
};
#endif

#endif // !STACKCONFIG_H
