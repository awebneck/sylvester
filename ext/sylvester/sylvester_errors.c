#include <sylvester_errors.h>

/**
 * Contains all Sylvester system error classes
 */

/* ================= EXTERNAL VARIABLES ================== */

/**
 * Sylvester error classes
 */
VALUE cSylvesterAttachmentError;
VALUE cSylvesterDetachmentError;
VALUE cSylvesterMetadataError;

/* ================== RUBY INITIALIZER =================== */

void sylvester_init_errors(void) {

  /**
   * Acquire namespace module
   */
  VALUE sylvester = rb_define_module("Sylvester");

  /**
   * Define error classes
   */
  cSylvesterAttachmentError = rb_define_class_under(sylvester, "AttachmentError", rb_eException);
  cSylvesterDetachmentError = rb_define_class_under(sylvester, "DetachmentError", rb_eException);
  cSylvesterMetadataError = rb_define_class_under(sylvester, "MetadataError", rb_eException);
}
