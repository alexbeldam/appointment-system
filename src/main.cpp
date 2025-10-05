#include <cstdlib>
#include <iostream>
#include <limits>

#include "consoleUI.hpp"

using namespace std;

int main() {
    const AppComposer app;
    const ConsoleUI ui(app);

    ui.run();

    return 0;
}
