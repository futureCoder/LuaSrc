/*
 * See Copyright Notice in qnode.h
 */

#ifndef __QACTOR_H__
#define __QACTOR_H__

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include "qcore.h"
#include "qdict.h"
#include "qtype.h"
#include "qlist.h"
#include "qmailbox.h"
#include "qmutex.h"

struct qactor_t {
  /* lua coroutine */
  lua_State     *state;

  /* lua coroutine ref */
  int            ref;

  /*
   * lua thread YIELD reason
   */
  /* waiting for network I/O */
  unsigned int    waiting_netio:1;

  /* waiting for message */
  unsigned int    waiting_msg:1;

  /* active flag */
  unsigned int    active:1;

  /* thread id which belongs to */
  qid_t           tid;

  /* actor id */
  qid_t           aid;

  /* parent actor id */
  qid_t           parent;

  /* socket list */
  qlist_t         sock_list;

  /* socket list lock */
  qspinlock_t     sock_list_lock;

  /* actor message list */
  qlist_t         msg_list;

  /* worker active actor list entry */
  qlist_t         actor_entry;

  qdict_t        *listen_params;

  /* lua add timer id dict */
  qdict_t        *timers;
};

qactor_t*   qactor_new(qid_t aid);
void        qactor_attach(qactor_t *actor, lua_State *state);
void        qactor_free(qactor_t *actor);

/* spawn an actor as child, return the actor ID */
qid_t       qactor_spawn(qactor_t *actor, lua_State *state, int ref);

qactor_t*   qactor_get(qid_t aid);
qengine_t*  qactor_get_engine(qid_t aid);
qworker_t*  qactor_get_worker(qid_t aid);

#endif  /* __QACTOR_H__ */
