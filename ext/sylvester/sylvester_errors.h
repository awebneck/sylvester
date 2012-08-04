#ifndef SYLVESTER_ERRORS
#define SYLVESTER_ERRORS

#include <sylvester.h>

/* ====================== EXTERNALS ====================== */

extern VALUE cSylvesterAttachmentError;
extern VALUE cSylvesterDetachmentError;
extern VALUE cSylvesterMetadataError;

extern void sylvester_init_errors(void);

#endif
