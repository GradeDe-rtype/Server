/*
    Authors:
    >> Caroline Boilly @Aeliondw
    >> Nathan Tirolf @dragusheen
    >> Daniil Stepanov @dan13615

    („• ֊ •„)❤  <  Have a good day !
    --U-----U------------------------
*/

#include "Parsing.hpp"

namespace RType
{
    Parsing::Parsing(int argc, char **argv)
    {
        if (argc == 2 && (std::string(argv[1]) == "--help" || std::string(argv[1]) == "-h"))
            throw Help();

        if (argc == 1) {
            _parseNoArgs();
            return;
        }
        if (argc != 3)
            throw ParsingError("Invalid number of arguments. Got " + std::to_string(argc) + " expected 5.");
        for (int i = 1; i < argc; i++) {
            if (std::string(argv[i]) == "-p") {
                _parseArgPort(argc, argv, i);
                i++;
                continue;
            }
            throw ParsingError("Invalid argument: \"" + std::string(argv[i]) + "\".");
        }
        if (_port == -1)
            throw ParsingError("Invalid argument: \"-p\". Port not set.");
    }

    int Parsing::getPort() { return _port; }

    void Parsing::_parseArgPort(int argc, char **argv, int i)
    {
        if (_port != -1)
            throw ParsingError("Invalid argument: \"-p\". Port already set.");
        if (i + 1 >= argc)
            throw ParsingError("Invalid argument for \"-p\": \". Expect a value.");
        if (!Utils::isNumber(argv[i + 1]))
            throw ParsingError("Invalid argument for \"-p\": \"" + std::string(argv[i + 1]) + "\". Expect a number.");
        if (std::stoi(argv[i + 1]) < 0 || std::stoi(argv[i + 1]) > 65535)
            throw ParsingError("Invalid argument for \"-p\": \"" + std::string(argv[i + 1]) +
                               "\". Expect a number between 0 and 65535.");
        _port = std::stoi(argv[i + 1]);
    }

    void Parsing::_parseNoArgs()
    {
        Papaya tmp = Papaya(Utils::Path::get()->path() + "data", "connect");
        std::string tmpPort = tmp.getData("ref", "ref", "port");
        if (tmpPort.empty())
            throw ParsingError("No port in the data file.");
        if (!Utils::isNumber(tmpPort))
            throw ParsingError("Invalid port in the data file. Expect a number.");
        _port = std::stoi(tmpPort);
    }
} // namespace RType
