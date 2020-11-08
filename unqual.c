/* Martin Uecker.
 *
 * macro that removes qualifiers from a type
 * */

#define typeof(x) __typeof__(x)

#define nil(T) ((typeof(T)){ 0 })
#define has_type_p(T, x) _Generic(&nil(x), typeof(T)*: 1, default: 0)

// seems to work on both clang and gcc
#define lvalue_convert(x) (0, x)
#define is_array_p(T) ((!has_type_p(lvalue_convert(T), T)) && (!has_type_p(const typeof(lvalue_convert(T)), T)))

#define choose_expr(x, a, b) _Generic(1?((void*)(x)):((int*)0), int*: (b), void*: (a))
#define choose_type(x, a, b) typeof(*choose_expr(x, &nil(a), &nil(b)))

#define array(T, N) typeof(typeof(T)[N])
#define array_length2(T, E) (sizeof(T) / sizeof(E))

#define pointer_to(x) choose_expr(is_array_p(x), (x), &(x))
#define compose(x, y) choose_type(is_array_p(x), array(y, array_length2(x, y)), y)
#define base_type(x) (*pointer_to(x))

#ifdef __clang__
#define remove_qual(x) ((0, x))
#else
// GCC does not do lvalue conversion for comma
// pedantic complains about the cast
#define remove_qual(x) ((typeof(x))(x))
#endif

// may need recursion for multi-dim. arrays
#define unqual(x) compose(x, remove_qual(base_type(x)))



struct s { int x; };
extern struct s s;
extern int i;
extern int a[3];

extern unqual(i) i;
extern unqual(s) s;
extern unqual(a) a;

#define qual const 

extern qual struct s sc;
extern qual int ic;
extern qual int ac[3];

extern unqual(ac) a;
extern unqual(ic) i;
extern unqual(sc) s;

