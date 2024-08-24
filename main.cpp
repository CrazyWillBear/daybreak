#include "src/Server.hpp"

int main(void) {
    Server server(7878);
    server.start();

    return 0;
}
