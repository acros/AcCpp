#include "leptjson.h"
#include "stdio.h"
#include "stdlib.h"
#include "memory.h"

#define EXPECT(c, ch) do { assert(*c->json == (ch)); c->json++; } while(0)

#ifndef LEPT_PARSE_STACK_INIT_SIZE
#define LEPT_PARSE_STACK_INIT_SIZE 256
#endif

lept_result lept_parse_value(lept_context* c, lept_value* v);

//////////////////////////////////////////////////////////////////////////
//Stack handle
static void* lept_context_push(lept_context* c, size_t size)
{
	void* ret;
	assert(size > 0);
	if (c->top + size >= c->size)
	{
		if (c->size == 0)
			c->size = LEPT_PARSE_STACK_INIT_SIZE;
		while (c->top + size >= c->size)
		{
			//  c->size * 1.5
			c->size += c->size >> 1;
		}
		c->stack = (char*)realloc(c->stack, c->size);
	}

	ret = c->stack + c->top;
	c->top += size;
	return ret;
}

static void* lept_context_pop(lept_context* c, size_t size) {
	assert(c->top >= size);
	return c->stack + (c->top -= size);
}

//////////////////////////////////////////////////////////////////////////


static void lept_parse_whitespace(lept_context* c)
{
	const char*p = c->json;
	/*
		\t	Tab
		\r	CR(Enter)
		\n  New Line
	*/
	while (*p == ' ' || *p == '\t' || *p == '\n' || *p == '\r')
		p++;
	c->json = p;
}

static lept_result lept_parse_number(lept_context* c, lept_value* v)
{
	char* end;
	v->u.n = strtod(c->json, &end);
	if (c->json == end)
		return LEPT_PARSE_INVALID_VALUE;

	c->json = end;
	v->type = LEPT_NUMBER;
	return LEPT_PARSE_OK;
}

static lept_result lept_parse_null(lept_context* c, lept_value* v) 
{
	//Check 'null'
	EXPECT(c,'n');
	if (c->json[0] != 'u' || c->json[1] != 'l' || c->json[2] != 'l')
		return LEPT_PARSE_INVALID_VALUE;
	c->json += 3;
	v->type = LEPT_NULL;
	return LEPT_PARSE_OK;
}

static lept_result lept_parse_true(lept_context* c, lept_value* v)
{
	EXPECT(c, 't');
	if (c->json[0] != 'r' || c->json[1] != 'u' || c->json[2] != 'e')
		return LEPT_PARSE_INVALID_VALUE;
	c->json += 3;
	v->type = LEPT_TRUE;
	return LEPT_PARSE_OK;
}

static lept_result lept_parse_false(lept_context* c, lept_value* v)
{
	EXPECT(c, 'f');
	if (c->json[0] != 'a' || c->json[1] != 'l' || c->json[2] != 's' || c->json[3] != 'e')
		return LEPT_PARSE_INVALID_VALUE;
	c->json += 4;
	v->type = LEPT_FALSE;
	return LEPT_PARSE_OK;
}

void lept_free(lept_value* v)
{
	if (v != NULL && v->type == LEPT_STRING && v->u.s.s != NULL && v->u.s.len > 0)
	{
		free(v->u.s.s);
		v->u.s.len = 0;
		v->type = LEPT_NULL;
	}
}

//Copy string to lept_value
void lept_set_string(lept_value* v, const char* s, size_t len)
{
	assert(v != NULL && (s != NULL || len == 0));
	lept_free(v);
	v->u.s.s = (char*)malloc(len + 1);
	memcpy(v->u.s.s, s, len);
	v->u.s.s[len] = '\0';
	v->u.s.len = len;
	v->type = LEPT_STRING;
}

#define PUTC(c, ch) do { *(char*)lept_context_push(c, sizeof(char)) = (ch); } while(0)

static int lept_parse_string_raw(lept_context* c, char**  str, size_t* len)
{

}

static int lept_parse_string(lept_context* c, lept_value* v)
{
	size_t head = c->top, len;
	const char* p;
	EXPECT(c, '\"');
	p = c->json;
	for (;;)
	{
		char ch = *p++;
		switch (ch)
		{
		case '\"':
			len = c->top - head;
			lept_set_string(v, (const char*)lept_context_pop(c, len), len);
			return LEPT_PARSE_OK;
		case '\0':
			c->top = head;
			return LEPT_PARSE_MISS_QUOTATION_MARK;
		default:
			PUTC(c, ch);
			break;
		}
	}
}

static lept_result lept_parse_array(lept_context* c, lept_value* v)
{
	size_t size = 0;
	lept_result ret;
	EXPECT(c, '[');

	if (*c->json == ']')
	{
		//Empty array
		c->json++;
		v->type = LEPT_ARRAY;
		v->u.a.size = 0;
		v->u.a.e = NULL;
		return LEPT_PARSE_OK;
	}

	//TODO:
	//lept_parse_whitespace(c);

	for (;;)
	{
		lept_value e;
		lept_init(&e);
		ret = lept_parse_value(c, &e);
		if (ret != LEPT_PARSE_OK)
			return ret;
		memcpy(lept_context_push(c, sizeof(lept_value)), &e, sizeof(lept_value));
		size++;
		if (*c->json == ',')
			c->json++;
		else if (*c->json == ']')
		{
			c->json++;
			v->type = LEPT_ARRAY;
			v->u.a.size = size;
			size *= sizeof(lept_value); 
			//Copy stack data to target value area
			memcpy(v->u.a.e = (lept_value*)malloc(size), lept_context_pop(c, size), size);
			return LEPT_PARSE_OK;
		}
		else
			return LEPT_PARSE_MISS_COMMA_OR_SQUARE_BRACKET;
	}

	return LEPT_PARSE_OK;
}

static int lept_parse_object(lept_context* c, lept_value* v) {
	size_t size;
	lept_member m;
	int ret;
	EXPECT(c, '{');
	lept_parse_whitespace(c);
	if (*c->json == '}') {
		c->json++;
		v->type = LEPT_OBJECT;
		v->u.o.m = 0;
		v->u.o.size = 0;
		return LEPT_PARSE_OK;
	}
	m.k = NULL;
	size = 0;
	for (;;) {
		lept_init(&m.v);

		/* \todo parse key to m.k, m.klen */
		/* \todo parse ws colon ws */

		/* parse value */
		if ((ret = lept_parse_value(c, &m.v)) != LEPT_PARSE_OK)
			break;
		memcpy(lept_context_push(c, sizeof(lept_member)), &m, sizeof(lept_member));
		size++;
		m.k = NULL; 
		/* ownership is transferred to member on stack */

		/* \todo parse ws [comma | right-curly-brace] ws */

	}
	
	/* \todo Pop and free members on the stack */

	return ret;
}

static lept_result lept_parse_value(lept_context* c, lept_value* v)
{
	switch (*c->json)
	{
	case 'n':	
		return lept_parse_null(c, v);
	case 't':
		return lept_parse_true(c, v);
	case 'f':
		return lept_parse_false(c, v);
	case '[':
		return lept_parse_array(c, v);
	case '\0':
		return LEPT_PARSE_EXPECT_VALUE;
		//TODO: more parser
	default:
		return lept_parse_number(c, v);
		break;
	}

	return LEPT_PARSE_OK;
}

/*
	n ➔ null
	t ➔ true
	f ➔ false
	" ➔ string
0-9/- ➔ number
	[ ➔ array
	{ ➔ object
*/
lept_result lept_parse(lept_value* v, const char* json)
{
	lept_context c;

	assert(v != NULL);

	c.json = json;
	c.stack = NULL;
	c.size = c.top = 0;
	
	lept_init(v);
	lept_parse_whitespace(&c);
	lept_result ret = lept_parse_value(&c, v);
	if (ret == LEPT_PARSE_OK)
	{
		lept_parse_whitespace(&c);
		if (*c.json != '\0')
			ret = LEPT_PARSE_ROOT_NOT_SINGULAR;
	}

	assert(c.top == 0);
	free(c.stack);
//	return v->type != LEPT_NULL ? ret : LEPT_PARSE_INVALID_VALUE;
	return ret;
}

lept_type lept_get_type(const lept_value * v)
{
	return v->type;
}

double lept_get_number(const lept_value * v)
{
	assert(v != NULL && v->type == LEPT_NUMBER);
	return v->u.n;
}

char * lept_get_string(const lept_value * v)
{
	assert(v != NULL && v->type == LEPT_STRING);
	return v->u.s.s;
}


size_t lept_get_array_size(const lept_value* v) {
	assert(v != NULL && v->type == LEPT_ARRAY);
	return v->u.a.size;
}

lept_value* lept_get_array_element(const lept_value* v, size_t index) {
	assert(v != NULL && v->type == LEPT_ARRAY);
	assert(index < v->u.a.size);
	return &v->u.a.e[index];
}

size_t lept_get_object_size(const lept_value* v)
{
	assert(v != NULL && v->type == LEPT_OBJECT);
	return v->u.o.size;
}

const char* lept_get_object_key(const lept_value* v, size_t index)
{
	assert(v != NULL && v->type == LEPT_OBJECT);
	return v->u.o.m[index].k;
}

size_t lept_get_object_key_length(const lept_value* v, size_t index)
{
	assert(v != NULL && v->type == LEPT_OBJECT);
	return v->u.o.m[index].klen;
}

lept_value* lept_get_object_value(const lept_value* v, size_t index)
{
	assert(v != NULL && v->type == LEPT_OBJECT);
	return &v->u.o.m[index].v;
}
