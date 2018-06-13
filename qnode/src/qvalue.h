/*
 * See Copyright Notice in qnode.h
 */

#ifndef __QVALUE_H__
#define __QVALUE_H__

#include "qcore.h"
#include "qtype.h"

#define QSTRING_TYPE 1
#define QNUMBER_TYPE 2
#define QDATA_TYPE   3 

typedef qid_t qnumber_t;

typedef void (qvalue_free_pt)(void *data);

struct qvalue_t {
  unsigned short  type;

  union {
    qstring_t str;
    qnumber_t num;
    struct {
      void           *data;
      qvalue_free_pt *free; 
    } data;
  } data;
};

#define qvalue_isnumber(value)  ((value)->type == QNUMBER_TYPE)
#define qvalue_isstring(value)  ((value)->type == QSTRING_TYPE)
#define qvalue_isdata(value)    ((value)->type == QDATA_TYPE)

#define qvalue_number(n) { .type = QNUMBER_TYPE, {.num = (n)} }
#define qvalue_string(s) { .type = QSTRING_TYPE, {.str = (char*)(s)} }
#define qvalue_data(d, f)   { .type = QDATA_TYPE,{.data.data = (void*)(d), .data.free = f} }

void  qvalue_free(qvalue_t *value);
void  qvalue_clone(qvalue_t *value1, qvalue_t *value2);

void  qvalue_newstr(qvalue_t *value, const char *str);
void  qvalue_newnum(qvalue_t *value, qnumber_t num);

#endif  /* __QVALUE_H__ */
