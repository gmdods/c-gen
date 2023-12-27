#ifndef GEN_MACROS
#define GEN_MACROS

// indirection pattern
#define CAT(lhs, rhs) lhs##rhs
#define CONCAT(lhs, rhs) CAT(lhs, rhs)
#define STRING(macro) #macro

#endif // !GEN_MACROS
