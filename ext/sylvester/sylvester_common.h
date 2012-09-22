#ifndef SYLVESTER_COMMON
#define SYLVESTER_COMMON

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

extern VALUE sshare_init(int argc, VALUE *argv, VALUE self);

#endif
