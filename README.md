<h1 style="text-align: center">
Daybreak
</h1>
<h3 style="text-align: center">
    A C++ web framework
</h3>

## Showcase:
```c++
#include "src/Server.hpp"
#include "src/responses/HTML.hpp"

/*
    Declare pages as C++ functions:
    
    This allows for ENDLESS possibilities, displaying C++ 
    datatypes on the website, integrating C++ libraries 
    onto the web, have raw access to http request data and
    return ANYTHING through polymorphism of the ResponseData
    class, maybe even display system data segments onto a
    page if you wanted. 
*/

/* this is just a shortcut macro, you can do this manually like so: */
/* auto index(const Pattern &pattern, const Request &request) */
TARGET(index) {
    return {Responses::OK, std::make_shared<HTML>(
        $ html({
            $ head({
                $ title("index")
            }),
            $ body({
                $ p("this is the index page, this is your request: "),
                $ code(request.getMethod().method + " " + request.getMethod().path + " " + request.getMethod().version)
            })
        })
    )};
}

/* route the target to the global target list */
ROUTE(Methods::GET, "/", index)

int main() {
    Server(7878).start();

    return 0;
}

```
