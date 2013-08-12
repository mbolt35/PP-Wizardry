PP-Wizardry
===========

GCC (C99) and MSVC Pre-Processor Macro Magic.


## About
PP-Wizardry is a set of C99 pre-processor macro definitions which allow you to perform some "outside of the box" functionality with other macro definitions. **This code was not meant to be used in a production quality software. It is simply to explore the depth of the C99 pre-processor.**


### What does this code do?

There are two "main features" of the included headers:
 
 1. Reduced template syntax
 2. Reduced namespace syntax

There's not much of a real need for such a "reduced syntax," but I do know that it's possible for template syntax to become unwieldy if used incorrectly. Again, this is not intended to be a used in production code.

### Reduced Template and Namespace Syntax

**main.cpp**
```C++
#include "macro_template.h"

// Create a namespace definition bolt::geom
NS_START(bolt, geom) { // <-- Open Bracket

  // Create a class with template<class T>
  class(T) Vector3 {
  private:
    T x, y, z;
    
  public: 
    Vector3() : x(T()), y(T()), z(T()) { }
    Vector3(T x, T y, T z) : x(x), y(y), z(z) { }

    inline T GetX() const { 
      return x;
    }
   
    inline T GetY() const {
      return y;
    }
    
    inline T GetZ() const {
      return z;
    }
  };
}; // <-- Closing Bracket

// Closing brackets for the other namespace definitions
NS_END(bolt, geom)

int main(int argc, char* argv[]) {
  bolt::geom::Vector3<int>* v3 = new bolt::geom::Vector3<int>(1, 2, 3);

  return 0;
}
```

After running through the pre-processor,

`g++ -E -P main.cpp`

or with VC++

`cl.exe /EP main.cpp`

You will see the following:
```C++
#include "macro_template.h"

namespace bolt { namespace geom {

  // Create a class with template<class T>
  template<class T> class Vector3 {
  private:
    T x, y, z;
    
  public: 
    Vector3() : x(T()), y(T()), z(T()) { }
    Vector3(T x, T y, T z) : x(x), y(y), z(z) { }

    inline T GetX() const { 
      return x;
    }
   
    inline T GetY() const {
      return y;
    }
    
    inline T GetZ() const {
      return z;
    }
  };
};

};

int main(int argc, char* argv[]) {
  bolt::geom::Vector3<int>* v3 = new bolt::geom::Vector3<int>(1, 2, 3);

  return 0;
}
```

### More Examples

#### Input:
```C++
// Structs
struct(T, E) Foo {
private:
  T tValue;
  E eValue;

public:
  Foo() : tValue(T()), eValue(E()) { }
};
```

#### Output:
```C++
template<class T, class E> struct Foo {
private:
  T tValue;
  E eValue;

public:
  Foo() : tValue(T()), eValue(E()) { }
};
```

#### Input:
```C++
// Template Methods and Default Parameters
generic(T, E=float, U=int) static T Combine(E input1, U input2) {
  T result = (T) combinator(input1, input2);
  return result;
}
```

#### Output:
```C++
template<class T, class E=float, class U=int> static T Combine(E input1, U input2) {
  T result = (T) combinator(input1, input2);
  return result;
}
```

These template generating macros can accept up to 10 parameters. You can probably add support for more, but 10 seemed fairly reasonable.

The main driver macro behind these template lists is the `#define TEMPLATE(NEXT, END, ...)`, which allows you to substitute in your own list creation macros. For example, `#define generic(...)` simply uses `#define CLASS_T(T) class T` and `#define CLASS_T_(T) CLASS_T(T),` with our `TEMPLATE` macro like so: `#define generic(...) template<TEMPLATE(CLASS_T_, CLASS_T, __VA_ARGS__)>`. In short, the `CLASS_T_(T)` macro (`NEXT`) is passed variadic arguments `0..[N-1]` one at a time. The `CLASS_T(T)` macro (`END`) is passed the last variadic argument `N`. For the template definitions, this inserts a `class ` in front of each argument and a comma after each argument (except for the last). 

Of course, you can also pass a single argument, so we needed our macro to determine the argument length and apply the correct sub-macro based on the result. Solving some of these problems reminded me a lot of my first Electrical Engineering course in college. It's very similar to programming, but it requires a completely different perspective of the problem in order to move forward. 

## Acknowledgements
### P99
There are some _real_ preprocessor utility libraries that are out there. [P99](http://p99.gforge.inria.fr/) seems to be the nicest syntactically.

### Boost Preprocessor
While I didn't personally dive into the [Boost Preprocessor](http://www.boost.org/doc/libs/1_54_0/libs/preprocessor/doc/index.html) code, almost all StackOverflow answers contained at least one Boost solution. 

### Other
Yeah, I know, using `#define` can be bad, and while it's possible to completely avoid defining pre-processor macros in your C/C++ applications, the C99 pre-processor is a pretty nifty contraption. In order to better understand this sometimes overlooked creature, I decided to tinker with it for a day. I've derived many of my solutions from answers discovered on StackOverflow, especially from [Paul](http://stackoverflow.com/users/375343/paul), who just so happens to be a pre-processor **NINJA**. Here's his github page: https://github.com/pfultz2
