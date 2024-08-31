#include "src/Daybreak.hpp"

#include "src/responses/HTML.hpp"

// can be simplified to TARGET(index) {}
Response index(const Pattern &pattern, const Request &request) {
    std::vector<HTMLElement*> cookies;

    /*
     * All HTML elements are under the el namespace to not
     * conflict with existing types.
     */
    using namespace el;

    /*
     * Showcase of using C++ data in our HTML page:
     */
    for (const auto &cookie : request.getCookies())
        cookies.emplace_back($ code(cookie.getName() + "=" + cookie.getValue()));

    /*
     * Custom HTML DSL-like usage:
     */
    Response response = {Responses::OK, std::make_shared<HTML>(
        $ html({
            $ head({
                $ title("index")
            }),
            $ body({
                {"style", "color: red;"}
            }, {
                $ p("this is the index page, this is your request: "),
                $ code(request.getMethod().method + " " + request.getMethod().path + " " + request.getMethod().version),
                $ p("here are your cookies sent from your request:"),
                $ el::div(cookies)
            })
        })
    )};

    /*
     * Add cookies to the server's response:
     */
    response.addCookie(Cookie({"test", "hello"}, CookieAttributes {
        .httpOnly = true
    }));

    response.addCookie(Cookie({"test2", "hello2"}, CookieAttributes {
        .httpOnly = true
    }));

    return response;
}

ROUTE(Methods::GET, "/", index)

int main() {
    Daybreak::instance->start();

    return 0;
}
