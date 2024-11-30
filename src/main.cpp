/*
    Authors:
    >> Nathan TIROLF - { nathan.tirolf@epitech.eu }

    („• ֊ •„)❤  <  Have a good day !
    --U-----U------------------------
*/

/*  ---- INCLUDES ---- */
#include "RType.hpp"
#include "Parsing.hpp"
#include "Server.hpp"

/*  ---- FUNCTION ---- */
int main(int argc, char **argv)
{
    try {
        srand(time(NULL));
        RType::Parsing parsing(argc, argv);
        std::cout << "Port: " << parsing.getPort() << std::endl;

        boost::asio::io_context io_context;
        Server::TCP server(io_context, parsing.getPort());
        Server::Command command(server);
        io_context.run();

    } catch (const RType::Parsing::ParsingError &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        std::cerr << ">> Use -help for help." << std::endl;
        return 84;
    } catch (const RType::Parsing::Help &e) {
        e.what();
        return 0;
    }
    return 0;
}
