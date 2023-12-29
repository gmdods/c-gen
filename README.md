# c-gen
A generative C generic header

Using standard C11 techniques of
[_Generic](https://en.cppreference.com/w/c/language/generic)
and [X macro](https://en.wikipedia.org/wiki/X_macro)
to generate new template functions.

## Containers

- [x] `dynarray.h`
- [x] `nodelist.h`
- [x] `hashmap.h`

## Downsides

- Code bloat / monomorphism
- Macro expansion in compiler / editor
- Single-identifier types (no `struct foo` as argument)

## Testing

Testing uses [c-unittest](https://github.com/gmdods/c-unittest).

