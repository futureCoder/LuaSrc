/*
 * See Copyright Notice in qnode.h
 */

#include "qdefines.h"
#include "qlog.h"
#include "qserver.h"
#include "qwmsg.h"
#include "qworker.h"

qmsg_t*
qwmsg_start_new(qid_t sender, qid_t recver) {
  qmsg_t        *msg;
  qwmsg_start_t *start;
  qworker_t     *worker;

  msg = qmsg_new(sender, recver, sizeof(qwmsg_start_t), W_START);
  if (msg == NULL) {
    return NULL;
  }

  worker      = workers[recver];
  start       = (qwmsg_start_t*)msg;
  start->aid  = qworker_new_aid(worker);

  return msg;
}

qmsg_t*
qwmsg_spawn_new(qactor_t *actor, qactor_t *parent,
                lua_State *state, qid_t sender, qid_t recver) {
  qmsg_t        *msg;
  qwmsg_spawn_t *spawn;

  msg = qmsg_new(sender, recver, sizeof(qwmsg_spawn_t), W_SPAWN);
  if (msg == NULL) {
    return NULL;
  }

  spawn         = (qwmsg_spawn_t*)msg;
  spawn->aid    = actor->aid;
  spawn->parent = parent->aid;
  spawn->state  = state;
  spawn->actor  = actor;

  qactor_attach(actor, state);
  actor->parent = parent->aid;

  return msg;
}

qmsg_t*
qwmsg_signal_new(qid_t recver, int signo) {
  qmsg_t         *msg;
  qwmsg_signal_t *signal;

  msg = qmsg_new(QMAINTHREAD_TID, recver,
                 sizeof(qwmsg_signal_t), W_SIGNAL);
  if (msg == NULL) {
    return NULL;
  }

  signal        = (qwmsg_signal_t*)msg;
  signal->signo = signo;

  return msg;
}

qmsg_t*
qwmsg_actor_new(qid_t sender, qid_t recver, void *data) {
  qmsg_t         *msg;
  qwmsg_actor_t  *actor;

  msg = qmsg_new(sender, recver,
                 sizeof(qwmsg_actor_t), W_ACTOR);
  if (msg == NULL) {
    return NULL;
  }

  actor        = (qwmsg_actor_t*)msg;
  actor->data  = data;

  return msg;
}
