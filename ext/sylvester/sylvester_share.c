#include <sylvester_share.h>
#include <sylvester_errors.h>

/**
 * A Sylvester::Share is an attachment to shared memory
 */

/* ================= EXTERNAL VARIABLES ================== */

/**
 * class Sylvester::Share
 */
VALUE cSylvesterShare;

/* ================== C-LEVEL FUNCTIONS ================== */

/**
 * Allocates and returns new Sshare struct
 */
static SsharePtr sshare_new_sshare(void) {
  return (SsharePtr) malloc(sizeof(Sshare));
}

/**
 * Checks validity of key
 */
static VALUE sshare_validate_key(VALUE key) {
  if (!rb_obj_is_kind_of(key, rb_cInteger) || FIX2INT(key) < 0)
    rb_raise(rb_eArgError, "Shared memory key not a positive integer!");
}

/**
 * Checks validity of key
 */
static VALUE sshare_validate_size(VALUE size) {
  if (!rb_obj_is_kind_of(size, rb_cInteger) || FIX2INT(size) < 0)
    rb_raise(rb_eArgError, "Specified size not a positive integer!");
}

/**
 * Returns the integer share reference for a given key
 */
static int sshare_get_share_reference(key_t key, int sz) {
  int shmid;

  if ((shmid = shmget(key, sz, IPC_CREAT | 0666)) < 0)
    rb_raise(cSylvesterAttachmentError, "Error acquiring id for shared memory for key %d of size %d: [%d] %s", key, sz, errno, strerror(errno));
  return shmid;
}

/**
 * Attaches a unit of shared memory at the specified
 * key and of the specified size to the current process,
 * returning a pointer thereinto for readonly purposes
 */
static void sshare_attach_share(key_t key, int sz, int shmid, void **shmaddr, int *attached) {
  if (!*attached) {
    if ((*shmaddr = shmat(shmid, NULL, 0)) == (int *)-1)
      rb_raise(cSylvesterAttachmentError, "Error attaching shared memory for key %d of size %d: [%d] %s", key, sz, errno, strerror(errno));
    *attached = 1;
  }
}

/**
 * Detaches a unit of shared memory at the specified
 * key and of the specified size from the current process.
 */
static void sshare_detach_share(key_t key, int sz, void *shmaddr, int *attached) {
  if (*attached) {
    if ((shmdt(shmaddr)) == -1)
      rb_raise(cSylvesterDetachmentError, "Error detaching shared memory for key %d of size %d: [%d] %s", (unsigned int)key, sz, errno, strerror(errno));
    *attached = 0;
  }
}

/**
 * Destroys a unit of shared memory at the specified
 * key and of the specified size from the current process.
 */
static void sshare_destroy_share(key_t key, int sz, int shmid, void *shmaddr, int *attached) {
  struct shmid_ds *databuf;

  sshare_detach_share(key, sz, shmaddr, attached);
  databuf = malloc(sizeof(struct shmid_ds));
  if ((shmctl(shmid, IPC_RMID, databuf)) == -1)
    rb_raise(cSylvesterMetadataError, "Error destroying shared memory for key %d of size %d: [%d] %s", (unsigned int)key, sz, errno, strerror(errno));
  free(databuf);
  //printf("DESTROYED!\n");
}

/**
 * Returns the number of currently attached processes
 */
static int sshare_get_attachment_count(key_t key, int sz, int shmid) {
  struct shmid_ds *databuf;
  int count;

  databuf = malloc(sizeof(struct shmid_ds));
  if ((shmctl(shmid, IPC_STAT, databuf)) == -1)
    rb_raise(cSylvesterMetadataError, "Error accessing shared memory metadata for key %d of size %d: [%d] %s", (unsigned int)key, sz, errno, strerror(errno));
  count = databuf->shm_nattch;
  free(databuf);
  return count;
  //printf("DESTROYED!\n");
}

/* ================ RUBY SYSTEM FUNCTIONS ================ */

/**
 * Class free function
 */
static void free_sshare(SsharePtr sshare) {
  if (sshare->attached)
    sshare_detach_share(sshare->key, sshare->sz, sshare->ptr, &sshare->attached);
  free(sshare);
}

/**
 * Class allocation function
 */
static VALUE sshare_alloc(VALUE klass) {
  SsharePtr sshare;
  VALUE obj;

  sshare = sshare_new_sshare();
  sshare->attached = 0;
  sshare->shmid = 0;
  sshare->key = (key_t)0;
  sshare->sz = 0;
  sshare->autoattach = 1;
  obj = Data_Wrap_Struct(klass, 0, free_sshare, sshare);
  return obj;
}

/**
 * Parse initialization options
 */
static void sshare_parse_init_opts(SsharePtr sshare, VALUE opts) {
  if (opts != Qnil) {
    if (st_lookup(RHASH(opts)->ntbl, ID2SYM(rb_intern("autoattach")), 0) && RTEST(rb_hash_aref(opts, ID2SYM(rb_intern("autoattach")))) == 0)
      sshare->autoattach = 0;
  }
}

/* ================ RUBY INSTANCE METHODS ================ */

/**
 * Constructor:
 *   Takes 3 arguments (1 optional), being the refkey for
 *   shared memory, the size to requisition, and a hash of
 *   options. Attaches said memory to the process, allowing
 *   it to be queried
 */
extern VALUE sshare_init(int argc, VALUE *argv, VALUE self) {
  VALUE key, shmsz, opts, autoattach;
  SsharePtr sshare;

  rb_scan_args(argc, argv, "21", &key, &shmsz, &opts);
  Data_Get_Struct(self, Sshare, sshare);

  sshare_validate_key(key);
  sshare_validate_size(shmsz);

  sshare->key = (key_t)FIX2INT(key);
  sshare->sz = FIX2INT(shmsz);
  sshare->shmid = sshare_get_share_reference((key_t)sshare->key, sshare->sz);

  sshare_parse_init_opts(sshare, opts);

  if (sshare->autoattach) {
    // printf("ATTACHING: %d\n", sshare->attached);
    sshare_attach_share(sshare->key, sshare->sz, sshare->shmid, &sshare->ptr, &sshare->attached);
    // printf("ATTACHED AFTER CALL: %d\n", sshare->attached);
  }

  return self;
}

/**
 * name: :attached?
 * Returns true if the share is currently attached to the current process, false otherwise
 */
static VALUE sshare_attached_qm(VALUE self) {
  SsharePtr sshare;

  Data_Get_Struct(self, Sshare, sshare);
  // printf("IS ATTACHED?: %d\n", sshare->attached);
  return (sshare->attached) ? Qtrue : Qfalse;
}

/**
 * name: :attach!
 * Attaches the share (if it is not already).
 */
static VALUE sshare_attach_ep(VALUE self) {
  SsharePtr sshare;

  Data_Get_Struct(self, Sshare, sshare);
  sshare_attach_share(sshare->key, sshare->sz, sshare->shmid, &sshare->ptr, &sshare->attached);
  return Qnil;
}

/**
 * name: :detach!
 * Detaches the share (if it is not already).
 */
static VALUE sshare_detach_ep(VALUE self) {
  SsharePtr sshare;

  Data_Get_Struct(self, Sshare, sshare);
  sshare_detach_share(sshare->key, sshare->sz, sshare->ptr, &sshare->attached);
  sshare->ptr = NULL;
  return Qnil;
}

/**
 * name: :destroy!
 * Detaches the share (if attached) and destroys the reference.
 */
static VALUE sshare_destroy_ep(VALUE self) {
  SsharePtr sshare;

  Data_Get_Struct(self, Sshare, sshare);
  sshare_destroy_share(sshare->key, sshare->sz, sshare->shmid, sshare->ptr, &sshare->attached);
  sshare->ptr = NULL;
  return Qnil;
}

/**
 * name: :key
 * Returns the key for the share.
 */
static VALUE sshare_key(VALUE self) {
  SsharePtr sshare;

  Data_Get_Struct(self, Sshare, sshare);
  return INT2FIX((unsigned int)sshare->key);
}

/**
 * name: :size
 * Returns the size of the share.
 */
static VALUE sshare_size(VALUE self) {
  SsharePtr sshare;

  Data_Get_Struct(self, Sshare, sshare);
  return INT2FIX(sshare->sz);
}

/**
 * name: :size
 * Returns the current number of attached processes.
 */
static VALUE sshare_attachment_count(VALUE self) {
  SsharePtr sshare;

  Data_Get_Struct(self, Sshare, sshare);
  return INT2FIX(sshare_get_attachment_count(sshare->key, sshare->sz, sshare->shmid));
}


/* ================== RUBY INITIALIZER =================== */

void sylvester_init_share(void) {

  /**
   * Acquire namespace module
   */
  VALUE sylvester = rb_define_module("Sylvester");

  /**
   * Define and describe share class
   */
  cSylvesterShare = rb_define_class_under(sylvester, "Share", rb_cObject);
  rb_define_alloc_func(cSylvesterShare, sshare_alloc);
  rb_define_method(cSylvesterShare, "initialize", sshare_init, -1);
  rb_define_method(cSylvesterShare, "attached?", sshare_attached_qm, 0);
  rb_define_method(cSylvesterShare, "attach!", sshare_attach_ep, 0);
  rb_define_method(cSylvesterShare, "detach!", sshare_detach_ep, 0);
  rb_define_method(cSylvesterShare, "destroy!", sshare_destroy_ep, 0);
  rb_define_method(cSylvesterShare, "key", sshare_key, 0);
  rb_define_method(cSylvesterShare, "size", sshare_size, 0);
  rb_define_method(cSylvesterShare, "attachment_count", sshare_attachment_count, 0);
}
