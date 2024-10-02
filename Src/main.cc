#include "Controller.h"

int main(int argc, char *argv[])
{
    // if (argc < 2) {
    //     std::cerr << "Need at least one argument to work..." << std::endl;
    //     exit(EXIT_FAILURE);
    // }

    if (InitSDL() != 0)
        exit(EXIT_FAILURE);

    Controller sol(argc, argv);
    if (sol.IsOK())
        sol.Run();

    DestroySDL();

    return 0;
}