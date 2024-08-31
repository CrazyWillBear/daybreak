#include "src/Daybreak.hpp"

#include "src/responses/HTML.hpp"

TARGET(index) {
    Response response = {Responses::OK, std::make_shared<HTML>(
        $ html({
            $ head({
                $ title("index")
            }),
            $ body({
                {"style", "color: red;"}
            }, {
                $ p("this is the index page, this is your request: "),
                $ code(request.getMethod().method + " " + request.getMethod().path + " " + request.getMethod().version)
            })
        })
    )};

    response.addCookie(Cookie({"test", "hello"}, CookieAttributes {
        .httpOnly = true
    }));

    return response;
}

ROUTE(Methods::GET, "/", index)

int main() {
    Daybreak::instance->start();

    return 0;
}
