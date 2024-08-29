#include "src/Daybreak.hpp"

#include "src/responses/HTML.hpp"

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

ROUTE(Methods::GET, "/", index)

int main() {
    Daybreak::instance->start();

    return 0;
}
