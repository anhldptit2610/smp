#include "Controller.h"
#include "SDL.h"

int main(int argc, char *argv[])
{
    if (InitSDL() != 0)
        exit(EXIT_FAILURE);

    Controller sol(argc, argv);
    if (sol.IsOK())
        sol.Run();

    DestroySDL();

    return 0;
}