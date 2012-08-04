#ifndef SYLVESTER_SHARE
#define SYLVESTER_SHARE

#include <sylvester.h>

/* ================ STRUCTS AND TYPEDEFS ================= */

struct _sshare {
  unsigned int attached, shmid, sz, autoattach;
  key_t key;
  void *ptr;
};

typedef struct _sshare Sshare;
typedef Sshare * SsharePtr;

/* ====================== EXTERNALS ====================== */

extern VALUE cSylvesterShare;

extern void sylvester_init_share(void);

#endif
