# DLManager
## Purpose
To provide a simple way to access dynamic libraries across multiple 
platforms.

## Currently tested Platforms
- Windows: 
- Linux: 

## Adding to your project
DLManager is a header only library, so you only need  
`#include "DLManager.h"`.  
However, in order to link, you might need a flag for your compiler.

DLManager uses `dlopen` on Linux, so you would need to pass `-ldl` to your
compiler.

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

## Can I pass a std::function to getFunction?
