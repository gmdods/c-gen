#ifndef GEN_MACROS
#define GEN_MACROS

// indirection pattern
#define CAT(lhs, rhs) lhs##rhs
#define CONCAT(lhs, rhs) CAT(lhs, rhs)

// declarations
#define DECLARE(rettype, name, ...) rettype name(__VA_ARGS__);

#endif // !GEN_MACROS
