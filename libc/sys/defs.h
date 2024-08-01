#ifndef __WASMNIX_DEFS__
#define __WASMNIX_DEFS__

typedef signed int i32;
typedef signed long i64;
typedef float f32;
typedef double f64;

#define WNGLUE(__str) __attribute__((import_module("wnglue"), import_name(__str))) extern
#define WNEXPORT(__str) __attribute__((export_name(__str)))

#endif

