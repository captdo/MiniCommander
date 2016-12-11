# MiniCommander
A simple, minimalistic but still powerful command line parser in less than 100 lines of C++11 code.

The library is header-only and only depends on the C++11 STL. This is not intended as a replacement for big libraries (e.g. boost::filesystem), rather as an option for quick tests and small projects.

***Features***

* *organize flags in groups*
* *mark groups as required, optional or "any of"*
* *add flags and their descriptions to the groups*
* *check if all flags are valid*
* *get parameters from flags*

## Installation
Just drop the header file "MiniCommander.hpp" in your project and include it in your program.

## Example Usage
This code example [test.cpp](https://github.com/MichaelGrupp/MiniCommander/blob/master/test/test.cpp) shows how to use the command line interface functions offered by MiniCommander:

```c++

#include <iostream>
#include "MiniCommander.hpp"

using namespace std;  // just for example

int main(int argc, char *argv[])
{
    MiniCommander cmd(argc, argv);

    OptionGroup paths(Policy::required, "required paths");
    paths.addOption("-d", "path to data folder");
    paths.addOption("-t", "path to test folder");
    cmd.addOptionGroup(paths);

    OptionGroup formats(Policy::anyOf, "formats, choose one of them");
    formats.addOption("-x", "use x format");
    formats.addOption("-y", "use y format");
    formats.addOption("-z", "use z format");
    cmd.addOptionGroup(formats);

    OptionGroup optionals(Policy::optional, "optional parameters");
    optionals.addOption("-a", "activate something");
    optionals.addOption("--help", "show info and usage");
    cmd.addOptionGroup(optionals);

    if (!cmd.checkFlags() || cmd.optionExists("--help")) {
        cmd.printHelpMessage("MiniCommander Example\n\nUSAGE:");
        return EXIT_FAILURE;
    }

    string dataFolder = cmd.getParameter("-d");
    string testFolder = cmd.getParameter("-t");
    if (dataFolder.empty() || testFolder.empty()) {
        cerr << "error: please specify required paths" << endl;
        cmd.printHelpMessage();
        return EXIT_FAILURE;
    }

    if (cmd.optionExists("-x"))
        cout << "using x format!" << endl;
    else if (cmd.optionExists("-y"))
        cout << "using y format!" << endl;
    else if (cmd.optionExists("-z"))
        cout << "using z format!" << endl;

    if (cmd.optionExists("-a"))
        cout << "activating something optional!" << endl;

    return EXIT_SUCCESS;
}
```
If [test.cpp](https://github.com/MichaelGrupp/MiniCommander/blob/master/test/test.cpp) is compiled and called with the wrong parameters: `./test -d ~/Documents/data -t ~/Documents/test -a`
(here, none of the options from the ```formats``` group is given and ```checkFlags()``` fails), we get the following output:
```
MiniCommander Example

USAGE:

[required paths]
-d	path to data folder
-t	path to test folder

[formats, choose one of them]
-x	use x format
-y	use y format
-z	use z format

[optional parameters]
--help	show info and usage
-a	activate something
```

With correct parameters, e.g.: `./test -d ~/Documents/data -t ~/Documents/test -a -z`, we get:
```
using z format!
activating something optional!
```
