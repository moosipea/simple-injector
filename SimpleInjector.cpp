#include "Injector.h"

int main(int argc, char** argv)
{
    if (argc < 3)
    {
        fprintf(stdout, "Not enough arguments provided (expected [process_name, dll_name])\n");
        exit(EXIT_FAILURE);
    }

    DWORD process = Injector::GetProcessIdFromExecutable(argv[1]);
    Injector::Inject(process, argv[2]);

    return 0;
}