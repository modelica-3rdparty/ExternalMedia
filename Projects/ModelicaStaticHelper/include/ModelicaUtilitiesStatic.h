
#ifndef MODELICA_STATIC_H
#define MODELICA_STATIC_H

#include "Export.h"
#include <stddef.h>
#include <stdarg.h>

#if defined(__cplusplus)
extern "C" {
#endif

/*
  Some of the functions never return to the caller. In order to compile
  external Modelica C-code in most compilers, noreturn attributes need to
  be present to avoid warnings or errors.

  The following macros handle noreturn attributes according to the latest
  C11/C++11 standard with fallback to GNU, Clang or MSVC extensions if using
  an older compiler.
*/
#undef MODELICA_NORETURN
#undef MODELICA_NORETURNATTR
#if __STDC_VERSION__ >= 201112L
#define MODELICA_NORETURN _Noreturn
#define MODELICA_NORETURNATTR
#elif defined(__cplusplus) && __cplusplus >= 201103L
#if (defined(__GNUC__) && __GNUC__ >= 5) || \
    (defined(__GNUC__) && defined(__GNUC_MINOR__) && __GNUC__ == 4 && __GNUC_MINOR__ >= 8)
#define MODELICA_NORETURN [[noreturn]]
#define MODELICA_NORETURNATTR
#elif (defined(__GNUC__) && __GNUC__ >= 3) || \
      (defined(__GNUC__) && defined(__GNUC_MINOR__) && __GNUC__ == 2 && __GNUC_MINOR__ >= 8)
#define MODELICA_NORETURN
#define MODELICA_NORETURNATTR __attribute__((noreturn))
#elif defined(__GNUC__)
#define MODELICA_NORETURN
#define MODELICA_NORETURNATTR
#else
#define MODELICA_NORETURN [[noreturn]]
#define MODELICA_NORETURNATTR
#endif
#elif defined(__clang__)
/* Encapsulated for Clang since GCC fails to process __has_attribute */
#if __has_attribute(noreturn)
#define MODELICA_NORETURN
#define MODELICA_NORETURNATTR __attribute__((noreturn))
#else
#define MODELICA_NORETURN
#define MODELICA_NORETURNATTR
#endif
#elif (defined(__GNUC__) && __GNUC__ >= 3) || \
      (defined(__GNUC__) && defined(__GNUC_MINOR__) && __GNUC__ == 2 && __GNUC_MINOR__ >= 8) || \
      (defined(__SUNPRO_C) && __SUNPRO_C >= 0x5110)
#define MODELICA_NORETURN
#define MODELICA_NORETURNATTR __attribute__((noreturn))
#elif (defined(_MSC_VER) && _MSC_VER >= 1200) || \
       defined(__BORLANDC__)
#define MODELICA_NORETURN __declspec(noreturn)
#define MODELICA_NORETURNATTR
#else
#define MODELICA_NORETURN
#define MODELICA_NORETURNATTR
#endif

/*
  The following macros handle format attributes for type-checks against a
  format string.
*/

#if defined(__clang__)
/* Encapsulated for Clang since GCC fails to process __has_attribute */
#if __has_attribute(format)
#define MODELICA_FORMATATTR_PRINTF __attribute__((format(printf, 1, 2)))
#define MODELICA_FORMATATTR_VPRINTF __attribute__((format(printf, 1, 0)))
#else
#define MODELICA_FORMATATTR_PRINTF
#define MODELICA_FORMATATTR_VPRINTF
#endif
#elif defined(__GNUC__) && __GNUC__ >= 3
#define MODELICA_FORMATATTR_PRINTF __attribute__((format(printf, 1, 2)))
#define MODELICA_FORMATATTR_VPRINTF __attribute__((format(printf, 1, 0)))
#else
#define MODELICA_FORMATATTR_PRINTF
#define MODELICA_FORMATATTR_VPRINTF
#endif


EXTERNALMEDIA_EXPORT MODELICA_NORETURN void
ModelicaError(const char *string) MODELICA_NORETURNATTR;
/*
Output the error message string (no format control). This function
never returns to the calling function, but handles the error
similarly to an assert in the Modelica code.
*/

EXTERNALMEDIA_EXPORT void
ModelicaWarning(const char *string);
/*
Output the warning message string (no format control).
*/


#if defined(__cplusplus)
}
#endif

#endif
