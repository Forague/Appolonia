// This client was taken from the following website: https://www.boost.org/doc/libs/1_79_0/libs/beast/doc/html/beast/quick_start/websocket_client.html

#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <cstdlib>
#include <iostream>
#include <string>

using namespace std;

namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace websocket = beast::websocket; // from <boost/beast/websocket.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>

// Sends a WebSocket message and prints the response
int main(int argc, char** argv)
{
    try
    {
        // Check command line arguments.
        if(argc == 1 && argc > 3)
        {
            cerr <<
                "Usage: ./client [message]>\n" <<
                "Attention, le temps doit être rentré en secondes.\n" <<
                "Example:\n" <<
                "    ./client \"1664455815\"\n";
            return EXIT_FAILURE;
        }

        if (argc == 2 and argv[1] == "-h")
        {
            cout << "Usage: ./client [option] [param]>\n" <<
                "Option : getLastDraw ([temps en seconde], optionnel)\n" <<
                "         getDraw_uid [uid du tirage]\n" <<
                "         getDraw_log [uid du tirage]\n" <<
                "Example:\n" <<
                "    ./client getDraw_uid \"1664455815\"\n";
            return EXIT_SUCCESS;
        }

        string host = "127.0.0.1";
        auto const port = "1664";
        string text;
        if (argc == 3){
            text = argv[1] + string(" ") + argv[2];
        } else if (argc == 2 && argv[1] != "getLastDraw"){
            text = "getLastDraw";
        } else {
            // cout error
            cout << "Usage: ./client [option] [param]>\n" <<
                "Option : getLastDraw ([temps en seconde], optionnel)\n" <<
                "         getDraw_uid [uid du tirage]\n" <<
                "         getDraw_log [uid du tirage]\n" <<
                "Example:\n" <<
                "    ./client getDraw_uid \"1664455815\"\n";
            return 1;
        }
        

        // The io_context is required for all I/O
        net::io_context ioc;

        // These objects perform our I/O
        tcp::resolver resolver{ioc};
        websocket::stream<tcp::socket> ws{ioc};

        // Look up the domain name
        auto const results = resolver.resolve(host, port);

        // Make the connection on the IP address we get from a lookup
        auto ep = net::connect(ws.next_layer(), results);

        // Update the host_ string. This will provide the value of the
        // Host HTTP header during the WebSocket handshake.
        // See https://tools.ietf.org/html/rfc7230#section-5.4
        host += ':' + to_string(ep.port());

        // Set a decorator to change the User-Agent of the handshake
        ws.set_option(websocket::stream_base::decorator(
            [](websocket::request_type& req)
            {
                req.set(http::field::user_agent,
                    string(BOOST_BEAST_VERSION_STRING) +
                        " client");
            }));

        // Perform the websocket handshake
        ws.handshake(host, "/");

        // Send the message
        ws.write(net::buffer(string(text)));

        // This buffer will hold the incoming message
        beast::flat_buffer buffer;
        net::transfer_at_least(2048*1024);

        // Read a message into our buffer
        ws.read(buffer);

        // Close the WebSocket connection
        ws.close(websocket::close_code::normal);

        // If we get here then the connection is closed gracefully

        // The make_printable() function helps print a ConstBufferSequence
        cout << beast::make_printable(buffer.data()) << endl;
    }
    catch(exception const& e)
    {
        cerr << "Error: " << e.what() << endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}