#include <string>

int main(int argc, char** argv)
{
    std::string args, horizonpull_command, horizonread_command;

    auto generate_horizonpull_command = [&args]() -> std::string
    {
        return "./horizonpull " + args;
    };
    auto generate_horizonread_command = [&args]() -> std::string
    {
        return "./horizonread " + args;
    };

    for (int i = 1; i < argc; i++)
    {
        args += std::string(argv[i]) + " ";
    }

    horizonpull_command = generate_horizonpull_command();
    horizonread_command = generate_horizonread_command();

    system(horizonpull_command.c_str());
    system(horizonread_command.c_str());

    return 0;
}