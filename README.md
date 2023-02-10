# Parseman Library {#mainpage}
This library is a header-only C++ library meant for parsing command line arguments.

## Design
This library is header-only (meaning you don't need to link against it). It's written using C++, and tested against C++20, and is likely to work with as far back as C++11. Naming conventions are Java-esque, and you can [submit a pull request](https://git.vlpatton.one/VLPatton/parseman) if you would like to change this.

## Usage
You can use `parseman` by including `<parseman/Parser.hpp>` to get started. This file includes a `parseman::Parser` class with an enum template that allows you to specify what commands you want to search for, as well as the types of their values when applicable.

### Specifying an Enum
The first thing to do, after including the appropriate headers, is to create an `enum` with the commands you would like to use. Technically, this does not *have* to be an `enum` but it is strongly recommended.

### Converting argc and argv
Within your `main` function, there should be `int argc, char** argv` within the arguments list; these need to be converted into `std::vector<std::string>` or a flat `std::string` representing the whole command line.
<dl class="section warning">
<dt>Warning</dt>
<dd>
<strong>Currently, a flat string is not yet supported, but there are plans to do so in the future.</strong>
</dd>
</dl>

### Examples
```cpp
#include <parseman/Parser.hpp>
#include <string>
#include <vector>

enum ExampleCmd {
    kHelp = 0,
    kVersion
};

int main(int argc, char** argv) {
    using namespace std;
    vector<string> args {};

    for (int i = 0; i < argc; i++) {
        args.push_back(string(argv[i]) + " ");
    }

    parseman::Parser p {args};

    p.setPattern(ExampleCmd::kHelp, string(".*(--help)"));
    p.setType(ExampleCmd::kHelp, typeid(bool));

    bool is_help = p.get<bool>(ExampleCmd::kHelp);

    // ...

    return 0;
}
```

