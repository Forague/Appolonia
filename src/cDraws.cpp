#include "cDraws.hpp"

using namespace std;

namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace websocket = beast::websocket; // from <boost/beast/websocket.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>


mutex mtx; // mutex pour protéger l'accès aux données de this->draws

cDraws::cDraws(){
    this->run = true;
    boost::thread t1(boost::bind(&cDraws::runDraws, this)); // thread qui va lancer les tirages
    t1.detach();
    boost::thread t2(boost::bind(&cDraws::launchAPI, this)); // thread qui va lancer l'API
    t2.detach();
}; // Constructeur.

cDraws::~cDraws(){this->run = false;}; // Destructeur.

void cDraws::add_draw(cDraw tirage){
    mtx.lock();
    this->draws.push_back(tirage);
    mtx.unlock();
};

cDraw cDraws::getLastDraw(string _p_dt = "") noexcept(false){
    // convert string _p_dt to 
    if (_p_dt == ""){
        time_t now = time(0);
        _p_dt = to_string(now);
    }
    long int _p_dt_long = stol(_p_dt);
    time_t t = time_t(_p_dt_long);

    // find the last draw with the same date
    bool found = false;
    cDraw lastTirage;
    mtx.lock();
    for (cDraw tirage : this->draws)
    {
        
        if (tirage.get_utc() < t)
        {
            lastTirage = tirage;
            found = true;
        }
    }
    mtx.unlock();
    // return the last draw
    if (found){
        return lastTirage;
    } else{
        throw logic_error("Aucun tirage n'a été trouvé (il est possible qu'aucun tirage n'ait été effectué ou que le temps soit incorrect)");
    }
};


void cDraws::runDraws(){
    ifstream configuration("./Config.json", ifstream::binary);
    Json::Value config;
    configuration >> config;
    configuration.close();

    int interval = config["Interval"].asInt(); // Accède à la valeur intervalle dans le fichier json. (format seconde dans le Json) 

    string schedule = config["Schedule"].asString(); // Accède à la valeur schedule dans le fichier json. (format string)

    string debut = schedule.substr(0, schedule.find("-")); // Accède à la valeur debut dans le fichier json. (format string)
    int hour_debut = stoi(debut.substr(0, 2)); // Accède à la valeur heure dans le fichier json. (format string)
    int min_debut = stoi(debut.substr(3, 2)); // Accède à la valeur minute dans le fichier json. (format string)

    string fin = schedule.substr(schedule.find("-") + 1); // Accède à la valeur fin dans le fichier json. (format string)
    int hour_fin = stoi(fin.substr(0, 2)); // Accède à la valeur heure dans le fichier json. (format string)
    int min_fin = stoi(fin.substr(3, 2)); // Accède à la valeur minute dans le fichier json. (format string)


    // Création d'un objet cDraw
    while (this->run){
        
        struct tm *t;
        time_t now = time(0);

        t = localtime(&now);

        string day = transform(t -> tm_wday);
        // check if t->tm_hour is in hour_debut and hour_fin


        this->DRAW_IS_CLOSED = config[day] == 1 && (t->tm_hour > hour_debut || (t->tm_hour == hour_debut && t->tm_hour >= min_debut)) && (t->tm_hour < hour_fin || (t->tm_hour == hour_fin && t->tm_min <= min_fin)); // Vérifier si le tirage est ouvert ou pas.
        if (this->DRAW_IS_CLOSED){ // Si le tirage est ouvert, on lance le tirage.
            cDraw tirage_actuel;
            tirage_actuel.tirage_aleatoire();
            // tirage_actuel.afficher_tirage();
            tirage_actuel.save_tirage();
            this->add_draw(tirage_actuel);
            sleep(interval);
        } 
    }
};


void cDraws::launchAPI(){
    // use boost to create an Web server (synchronous)

    try {
        auto const address = net::ip::make_address("127.0.0.1");
        auto const port = static_cast<unsigned short>(1664);

        // The io_context is required for all I/O
        net::io_context ioc{1};

        // The acceptor receives incoming connections
        tcp::acceptor acceptor{ioc, {address, port}};
        for(;;)
        {
            // This will receive the new connection
            tcp::socket socket{ioc};

            // Block until we get a connection
            acceptor.accept(socket);

            // Launch the session, transferring ownership of the socket
            thread(&cDraws::session_ws,this, move(socket)).detach();
        }
    }
    catch (const exception& e)
    {
        cerr << "Error: " << e.what() << endl;
    }

};

void cDraws::session_ws(tcp::socket socket){
    try
    {
        // Construct the stream by moving in the socket
        websocket::stream<tcp::socket> ws{std::move(socket)};

        // Set a decorator to change the Server of the handshake
        ws.set_option(websocket::stream_base::decorator(
            [](websocket::response_type& res)
            {
                res.set(http::field::server,
                    std::string(BOOST_BEAST_VERSION_STRING) +
                        " websocket-server-sync");
            }));

        // Accept the websocket handshake
        ws.accept();

        for(;;)
        {
            // This buffer will hold the incoming message
            beast::flat_buffer buffer;

            // Read a message
            ws.read(buffer);

            // Echo the message back
            string _p_dt = beast::buffers_to_string(buffer.data());
            string message = "Error";
            try {
                if (_p_dt == "getLastDraw"){
                    cDraw tirage = this->getLastDraw("");
                    message = tirage.get_tirages();
                } else if (regex_match(_p_dt, regex("^getLastDraw [0-9]+$"))){
                    cDraw tirage = this->getLastDraw(_p_dt);
                    message = tirage.get_tirages();
                } else if (regex_match(_p_dt, regex("^getDraw_uid [0-9]+$"))){
                    cDraw tirage = this->getDraw_uid(_p_dt);
                    message = tirage.get_tirages();
                } else if (regex_match(_p_dt, regex("^getDraw_log [0-9]+$"))){
                    cDraw tirage = this->getDraw_uid(_p_dt);
                    message = tirage.getDraw_log();
                }
            } catch (exception& e){
                message = e.what();
            }
            
            ws.text(ws.got_text());
            // transfer the message to the buffer
            buffer.clear();
            beast::ostream(buffer) << message;
            ws.write(buffer.data());
        }
    }
    catch(beast::system_error const& se)
    {
        // This indicates that the session was closed
        if(se.code() != websocket::error::closed)
            cerr << "Error: " << se.code().message() << endl;
    }
    catch(exception const& e)
    {
        cerr << "Error: " << e.what() << endl;
    }
}

string cDraws::transform(int day){
    switch (day){
        case 1:
            return "Lundi";
        case 2:
            return "Mardi";
        case 3:
            return "Mercredi";
        case 4:
            return "Jeudi";
        case 5:
            return "Vendredi";
        case 6:
            return "Samedi";
        case 7:
            return "Dimanche";
        default:
            return "Erreur";
    }
};

cDraw cDraws::getDraw_uid(string _p_id) noexcept(false) {
    // open file ./DRAWS/tirage_p_id.json
    cDraw tirage;
    tirage.load_tirage(_p_id);
    return tirage; 
}