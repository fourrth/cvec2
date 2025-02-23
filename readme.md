# cvec
This is a library that provides dynamically sized arrays similar to Cpp [std::vector](https://en.cppreference.com/mwiki/index.php?title=cpp/container/vector&oldid=174051).

This library initalially started off as a simple *2 hour project* using c23 and t-generics, but I then saw the need of a real C library.

## Features
This library is mainly a generic *C-like* vector library with an extra optional section with some functions that are helpful for strings, however they do no assume anything and essentially operate just like an vector over [u8].

## Building
The library does not require anything past the standard C runtime. It should also run on any reasonable platform, however it will complain if you happen to not have ssh installed. Either install ssh or change ```GIT_REPOSITORY git@github.com:fourrth/Simple-Headers.git``` to  ```GIT_REPOSITORY https://github.com/fourrth/Simple-Headers.git``` in [common.cmake](cmake/common.cmake)

Simply:

```
git clone https://github.com/fourrth/cvec2.git
cd cvec2
cmake .
cmake --build .
```

## Examples
Multiple examples are given in the [examples](examples) directory.

## License

This crate — along with any subsequent additions or revisions — are all dual licensed under [MIT License](LICENSE-MIT) or [Apache License](LICENSE-APACHE) at your option.