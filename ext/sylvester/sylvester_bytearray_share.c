#include <sylvester_share.h>
#include <sylvester_errors.h>

/**
 * A Sylvester::Share is an attachment to shared memory
 */

/* ================= EXTERNAL VARIABLES ================== */

/**
 * class Sylvester::ByteArrayShare
 */
VALUE cSylvesterByteArrayShare;

/* ================== C-LEVEL FUNCTIONS ================== */

long sbashare_set_length(SsharePtr sshare, long len) {
  long *length;

  length = (long *)sshare->ptr;
  *length = len;
}

long sbashare_get_length(SsharePtr sshare) {
  return *(long *)sshare->ptr;
}

char *sbashare_byteptr_at_index(SsharePtr sshare, int index) {
  return (char *)sshare->ptr + sizeof(long) + index;
}

char sbashare_append_byte(SsharePtr sshare, char byte) {
  char *byteptr;
  long len;

  len = sbashare_get_length(sshare);
  byteptr = sbashare_byteptr_at_index(sshare,len);
  *byteptr = byte;
  sbashare_set_length(sshare, len + 1);
}

int sbashare_full(SsharePtr sshare) {
  return (sbashare_get_length(sshare) + sizeof(long) < sshare->sz) ? 0 : 1;
}

/* ================ RUBY SYSTEM FUNCTIONS ================ */

/* ================ RUBY INSTANCE METHODS ================ */

/**
 * Constructor:
 *   Takes 3 arguments (1 optional), being the refkey for
 *   shared memory, the size to requisition, and a hash of
 *   options. Attaches said memory to the process, allowing
 *   it to be queried
 */
static VALUE sbashare_init(int argc, VALUE *argv, VALUE self) {
  SsharePtr sshare;

  sshare_init(argc, argv, self);
  Data_Get_Struct(self, Sshare, sshare);
  sbashare_set_length(sshare, 0);

  return self;
}

/**
 * name: :length
 */
static VALUE sbashare_length(VALUE self) {
  SsharePtr sshare;

  Data_Get_Struct(self, Sshare, sshare);
  return LONG2FIX(sbashare_get_length(sshare));
}

/**
 * name: :>>, :push
 */
static VALUE sbashare_push(VALUE self, VALUE byte) {
  SsharePtr sshare;
  char charbyte;
  int charbyte_initialized;

  Data_Get_Struct(self, Sshare, sshare);
  if (!sbashare_full(sshare)) {
    charbyte = 0x00;
    charbyte_initialized = 0;
    if (rb_obj_is_kind_of(byte, rb_cInteger)) {
      charbyte = 0xff & FIX2INT(byte);
      charbyte_initialized = 1;
    } else if (rb_obj_is_kind_of(byte, rb_cString)) {
      charbyte = *(char *)(RSTRING_PTR(StringValue(byte)));
      charbyte_initialized = 1;
    }
    if (charbyte_initialized)
      sbashare_append_byte(sshare, charbyte);
  }
  return byte;
}

static VALUE sbashare_write(VALUE self, VALUE string) {
  SsharePtr sshare;
  int i;

  Data_Get_Struct(self, Sshare, sshare);
  for (i = 0; i < RSTRING_LEN(StringValue(string)); i++) {
    sbashare_append_byte(sshare, *(char *)(RSTRING_PTR(StringValue(string))+i));
  }

  return string;
}

static VALUE sbashare_read(VALUE self, VALUE string) {
  SsharePtr sshare;

  Data_Get_Struct(self, Sshare, sshare);

  return rb_str_new((char *)sshare->ptr + sizeof(long), *(long *)sshare->ptr);
}

static VALUE sbashare_clear(VALUE self, VALUE string) {
  SsharePtr sshare;

  Data_Get_Struct(self, Sshare, sshare);
  sbashare_set_length(sshare, 0);

  return Qnil;
}

static VALUE sbashare_get_range(VALUE self, VALUE range) {
  return Qnil;
}

static VALUE sbashare_set_range(VALUE self, VALUE range, VALUE val) {
  return Qnil;
}


/* ================== RUBY INITIALIZER =================== */

void sylvester_init_ba_share(void) {

  /**
   * Acquire namespace module
   */
  VALUE sylvester = rb_define_module("Sylvester");
  /**

   * Define and describe share class
   */
  cSylvesterByteArrayShare = rb_define_class_under(sylvester, "ByteArrayShare", cSylvesterShare);
  rb_define_method(cSylvesterByteArrayShare, "length", sbashare_length, 0);
  rb_define_method(cSylvesterByteArrayShare, "<<", sbashare_push, 1);
  rb_define_method(cSylvesterByteArrayShare, "push", sbashare_push, 1);
  rb_define_method(cSylvesterByteArrayShare, "write", sbashare_write, 1);
  rb_define_method(cSylvesterByteArrayShare, "read", sbashare_read, 0);
  rb_define_method(cSylvesterByteArrayShare, "to_s", sbashare_read, 0);
  rb_define_method(cSylvesterByteArrayShare, "clear!", sbashare_clear, 0);
  rb_define_method(cSylvesterByteArrayShare, "[]", sbashare_get_range, 1);
  rb_define_method(cSylvesterByteArrayShare, "[]=", sbashare_set_range, 2);
}
