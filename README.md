# little big int

A C++11 big-integer library, highly optimized for the small-number case.

### Why would I use it?

If you need to work with integers that may grow beyond the native word size (but generally don't) - and native-like performance is important to you - then you may find this library useful.

### Tell me more

For numbers that fit within a native word size (or some other configurable amount), a custom stack-allocator is used to avoid unnecessary heap allocations.  Most operations are also optimized for this "trivial" case, so their performance will closely mimic the performance of a native number.

In general, you can treat these big-integers as if they were native and everything should "just work".  Square-roots, logarithms, hashing for unordered containers, stream operators, etc. are all implemented for you.

### Some examples

```cpp
// String construction, could also construct from regular integers.
BigUint i("819389123891283912894885748579328493284");
i *= BigUint::log2(i) * 824838284;
std:cout << 
    "I: " << i << "\n" <<
    "Explicit string conversion: " << i.str() << "\n" <<
    "To binary: " << i.bin() << std::endl;
```

### License

MIT.
