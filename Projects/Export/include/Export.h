#ifndef EXPORT_MACRO_H_
#define EXPORT_MACRO_H_
/*!
Portable definitions of the EXPORT macro
 */
#if !defined(EXTERNALMEDIA_EXPORT)
#  if !defined(EXTERNALMEDIA_LIBRARY_EXPORTS)
#    define EXTERNALMEDIA_EXPORT
#  else
#    if (EXTERNALMEDIA_LIBRARY_EXPORTS == 1)
#      if defined(_WIN32) || defined(__WIN32__) || defined(_WIN64) || defined(__WIN64__)
#        if !defined(__EXTERNALMEDIA_ISWINDOWS__)
#          define __EXTERNALMEDIA_ISWINDOWS__
#        endif
#      elif __APPLE__
#        if !defined(__EXTERNALMEDIA_ISAPPLE__)
#          define __EXTERNALMEDIA_ISAPPLE__
#        endif
#      elif __linux
#        if !defined(__EXTERNALMEDIA_ISLINUX__)
#          define __EXTERNALMEDIA_ISLINUX__ 
#        endif
#      endif
#
#      if defined(__EXTERNALMEDIA_ISLINUX__)
#        define EXTERNALMEDIA_EXPORT __attribute__((visibility("default")))
#      elif defined(__EXTERNALMEDIA_ISAPPLE__)
#        define EXTERNALMEDIA_EXPORT __attribute__((visibility("default")))
#      else
#        define EXTERNALMEDIA_EXPORT __declspec(dllexport)
#      endif
#    else
#      define EXTERNALMEDIA_EXPORT
#    endif
#  endif
#endif

#endif /*EXPORT_MACRO_H_*/
