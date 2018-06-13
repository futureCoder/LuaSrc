/*
 * See Copyright Notice in qnode.h
 */

#ifndef __QTIMER_H__
#define __QTIMER_H__

#include <stdint.h>
#include "qcore.h"
#include "qfreelist.h"
#include "qidmap.h"
#include "qlist.h"
#include "qrbtree.h"
#include "qtype.h"

typedef void (qtimer_pt)(void *data);
typedef void (qtimer_destroy_pt)(void *data);

typedef struct qtimer_t {
  qfreeitem_fields;

  /* client data */
  void           *data;

  /* timeout(in ms) */
  uint64_t        timeout;

  /* timer cycle(in ms) */
  uint64_t        cycle;

  /* timer id */
  qid_t           id;

  /* timer free list entry */
  qlist_t         entry;

  /* timer handler */
  qtimer_pt      *handler;

  /* timer destroy handler */
  qtimer_destroy_pt *destroy;

  /* rbtree node */
  qrbtree_node_t  node;
} qtimer_t;

typedef struct qtimer_manager_t {
  uint64_t        now;
  uint64_t        now_ms;
  qidmap_t        id_map;
  qfreelist_t     free_list;    /* free timer list */
  qengine_t      *engine;

  /* timer rbtree */
  qrbtree_t       rbtree;
  qrbtree_node_t  sentinel;
} qtimer_manager_t;

void  qtimer_manager_init(qtimer_manager_t *mng, qengine_t *engine);
void  qtimer_manager_free(qtimer_manager_t *mng);

qid_t qtimer_add(qengine_t *engine, uint32_t timeout,
                 qtimer_pt *func, qtimer_destroy_pt *destroy,
                 uint32_t cycle, void *data);
int   qtimer_del(qengine_t *engine, qid_t id);

int   qtimer_next(qtimer_manager_t *mng);
void  qtimer_process(qtimer_manager_t *mng);

#endif  /* __QTIMER_H__ */
