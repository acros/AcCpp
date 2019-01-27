#pragma once

#include "assert.h"

/*
	Tutorial of Json Parser in C by Milo Yip
	https://zhuanlan.zhihu.com/json-tutorial
*/

typedef enum {
	LEPT_PARSE_OK = 0,
	LEPT_PARSE_EXPECT_VALUE,
	LEPT_PARSE_INVALID_VALUE,
	LEPT_PARSE_ROOT_NOT_SINGULAR,
	LEPT_PARSE_MISS_QUOTATION_MARK,
	LEPT_PARSE_MISS_COMMA_OR_SQUARE_BRACKET
}lept_result;

typedef enum { 
	LEPT_NULL, 
	LEPT_FALSE, 
	LEPT_TRUE, 
	LEPT_NUMBER, 
	LEPT_STRING, 
	LEPT_ARRAY, 
	LEPT_OBJECT } 
lept_type;

typedef struct lept_value lept_value;
typedef struct lept_member lept_member;

struct lept_value{
	lept_type type;

	union {
		double n;		//number
		struct {
			size_t len;
			char* s;
		}s;				//string
		struct {
			lept_value* e;	
			size_t size;	//array list size
		}a;				//array
		struct {
			lept_member* m;
			size_t size;
		}o;
	}u;

};

struct lept_member {
	char* k;
	size_t klen;
	lept_value v;
};

typedef struct {
	const char* json;
	char*	stack;	//stack [begin] address
	size_t	size, top;
}lept_context;


void lept_free(lept_value* v);

lept_result lept_parse(lept_value* v, const char* json);

lept_type lept_get_type(const lept_value* v);

//Number
double lept_get_number(const lept_value* v);

//String
char* lept_get_string(const lept_value* v);

//Array
size_t lept_get_array_size(const lept_value* v);
lept_value* lept_get_array_element(const lept_value* v, size_t index);

//Object
size_t lept_get_object_size(const lept_value* v);
const char* lept_get_object_key(const lept_value* v, size_t index);
size_t lept_get_object_key_length(const lept_value* v, size_t index);
lept_value* lept_get_object_value(const lept_value* v, size_t index);

//Test code
void test_parse();


#define lept_init(v) do { (v)->type = LEPT_NULL; } while(0)
