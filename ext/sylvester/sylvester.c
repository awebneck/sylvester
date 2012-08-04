#include <sylvester.h>

/**
 * Sylvester system initializer
 */

/* ================= EXTERNAL VARIABLES ================== */

/**
 * module Sylvester
 */
VALUE mSylvester;

/* ================ RUBY ROOT INITIALIZER ================ */

void Init_sylvester(void) {

  /**
   * Define namespace modules
   */
  mSylvester = rb_define_module("Sylvester");

  /**
   * Initialize all systems in order of dependency
   */
  sylvester_init_errors();
  sylvester_init_share();
}
