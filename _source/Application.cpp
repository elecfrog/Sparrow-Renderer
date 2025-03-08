#include "Application.hpp"

int main() try {

    Application app;
    app.Run();
}
catch (const std::exception &e) {
    spdlog::critical(" Application Fatal: {0}", e.what());
    return EXIT_FAILURE;
}