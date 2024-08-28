#include "src/Server.hpp"

#include "HTML.hpp"

auto index(const Pattern &pattern) {
    return Response(Responses::OK, std::make_shared<HTML>(
        $ html({
            $ head({
                $ title("index")
            }),
            $ body({
                $ p("this is the index page")
            })
        })
    ));
}

int main() {
    Server server(7878);
    server.addTarget({
        .method = Methods::GET,
        .path = "/"
    }, index);
    server.start();

    return 0;
}
