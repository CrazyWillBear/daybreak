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
#include "src/Daybreak.hpp"

#include "src/responses/HTML.hpp"

TARGET(index) {
    return {Responses::OK, std::make_shared<HTML>(
        /* custom HTML DSL built into C++ */
        $ html({
            $ head({
                $ title("index")
            }),
            $ body({
            /* map like initialization for html attributes */
                {"style", "color: red;"}
            }, {
                $ p("this is the index page, this is your request: "),
                $ code(request.getMethod().method + " " + request.getMethod().path + " " + request.getMethod().version)
            })
        })
    )};
}

ROUTE(Methods::GET, "/", index)

int main() {
    /* this is subject to change */
    Daybreak::instance->start();

    return 0;
}


```
