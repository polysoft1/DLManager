# DLManager
## Purpose
To provide a simple way to dyamically load dynamic libraries across
multiple platforms.

## Currently tested Platforms
- Windows: [![](https://github.com/polysoft1/DLManager/workflows/CI-Windows/badge.svg)](https://github.com/polysoft1/DLManager/actions)
- Linux and MacOS: [![](https://github.com/polysoft1/DLManager/workflows/CI-Unix/badge.svg)](https://github.com/polysoft1/DLManager/actions)

## Adding to your project
DLManager is a header only library, so you only need  
`#include "DLManager.h"`.  
However, in order to link, you might need a flag for your compiler.

DLManager uses `dlopen` on Linux, so you would need to pass `-ldl` to your
compiler. If using cmake, you can instruct it to do that with:
`target_link_libraries(target ${CMAKE_DL_LIBS})`
replacing target with your cmake target.

## Using DLManager
Each platform may require different workflows in order for DLManager to
work properly, especially with 
[name mangling](https://en.wikipedia.org/wiki/Name_mangling).

For GCC, you can use `extern "C"` before a non-member function.
```
extern "C" void hello(){
    std::cout << "Hello!" << std::endl;
}
```
Then you can access it by passing "hello" as the name to `getFunction`.  
Check out our [examples](examples) for demonstrations