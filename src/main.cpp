/*
    Authors:
    >> Nathan TIROLF - { nathan.tirolf@epitech.eu }

    („• ֊ •„)❤  <  Have a good day !
    --U-----U------------------------
*/

/*  ---- INCLUDES ---- */
#include <Parsing.hpp>
#include <RType.hpp>
#include <Server.hpp>

/*  ---- FUNCTION ---- */
int main(int argc, char **argv)
{
    try {
        srand(time(NULL));
        RType::Parsing parsing(argc, argv);
        std::cout << "Port: " << parsing.getPort() << std::endl;

        boost::asio::io_context io_context;
        Server::TCP server(io_context, parsing.getPort());
        std::cout << "Server starting..." << std::endl;
        std::thread io_thread([&io_context]() { io_context.run(); });
        std::cout << "Server started." << std::endl;
        while (server.getRunning()) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
        io_context.stop();
        io_thread.join();
        std::cout << "Server stopped." << std::endl;
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
