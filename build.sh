mkdir -p build
test -f build/CMakeCache.txt || cmake -S . -B build
cmake --build build --target all