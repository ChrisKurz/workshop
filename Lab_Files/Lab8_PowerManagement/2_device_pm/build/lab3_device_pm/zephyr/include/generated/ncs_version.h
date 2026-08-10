#ifndef _NCS_VERSION_H_
#define _NCS_VERSION_H_

/* The template values come from cmake/modules/version.cmake
 * BUILD_VERSION related template values will be 'git describe',
 * alternatively user defined BUILD_VERSION.
 */

/* #undef ZEPHYR_VERSION_CODE */
/* #undef ZEPHYR_VERSION */

#define NCSVERSION                   0x3040000
#define NCS_VERSION_NUMBER           0x30400
#define NCS_VERSION_MAJOR            3
#define NCS_VERSION_MINOR            4
#define NCS_PATCHLEVEL               0
#define NCS_TWEAK                    0
#define NCS_EXTRAVERSION             
#define NCS_VERSION_STRING           "3.4.0"
#define NCS_VERSION_EXTENDED_STRING  "3.4.0+0"
#define NCS_VERSION_TWEAK_STRING     "3.4.0+0"

#define NCS_BUILD_VERSION v3.4.0


#endif /* _NCS_VERSION_H_ */
