/*
 *  * This is not meant to be a C++ program at all.  It is
 *   * meant to be processed with the C++ Preprocessor, to produce
 *    * a gv file, which can be used to visualize the Clang type
 *     * hierarchy.
 *      *
 *       * One way to do this is to run:
 *        *    % clang++ -E -c -I $CLANG_SRC/tools/clang/include inhier.cpp | sed '/^#/d' > inhier.gv
 *         *    % dot -Tps inhier.gv
 *          */
#define TYPE_WANTED
#undef TYPELOC_WANTED

digraph {
      rankdir=LR;
          size="8.5,11";
#if defined(TYPE_WANTED)
#define TYPE(a, b) a##Type##_Type [ label=a##Type; shape=oval ]; b##_Type [ label=#b; shape=oval];
#include "clang/AST/TypeNodes.def"
#undef TYPE
#endif

#if  defined(TYPELOC_WANTED)
#define TYPELOC(a, b) a##Type##_Loc [ label=a##Type; shape=box ]; b##_Loc [ label=#b; shape=box ]; 
#include "clang/AST/TypeLocNodes.def"
#undef TYPELOC
#endif    /* TYPELOC_WANTED */

#if defined(TYPE_WANTED)
#define TYPE(a, b) { a##Type##_Type -> b##_Type };
#include "clang/AST/TypeNodes.def"
#undef TYPE
#endif  /* TYPE_WANTED */

#if defined(TYPELOC_WANTED)
#define TYPELOC(a, b) { a##Type##_Loc -> b##_Loc };
#include "clang/AST/TypeLocNodes.def"
#undef TYPELOC
#endif
}

