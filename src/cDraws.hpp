#include "cDraw.hpp"


// include boost wss features
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ssl/stream.hpp>
#include <boost/beast/http.hpp>

#include <boost/filesystem.hpp>

#include <stdexcept> // pour la gestion des erreurs si aucun tirage n'a été fait.
#include <thread>
#include <list>
#include <mutex>
#include <regex>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread/thread.hpp>
#include <boost/bind.hpp>


#ifndef CDRAWS_H
#define CDRAWS_H

class cDraws{
    private:
        std::list<cDraw> draws; // Liste des tirages
        void runDraws(); // Fonction qui va lancer les tirages
        void add_draw(cDraw tirage);
        bool run; // Variable qui va déterminer si le programme est en cours ou pas  
        std::string transform(int day);
        void launchAPI();
        void session_ws(boost::asio::ip::tcp::socket socket);
        
    public:
        cDraws(); // constructeur
        ~cDraws(); // destructeur

        // constantes 
        std::string DRAW_DTH; // Pas compris
        bool DRAW_IS_CLOSED;

        //méthodes 
        cDraw getLastDraw(std::string _p_dt) noexcept(false); // On récupère le dernier tirage avec la date _p_dt, TODO: changer le nom des classes.
        cDraw getDraw_uid(std::string _p_id) noexcept(false); // On récupère le tirage avec l'id _p_id, TODO: changer le nom des classes.
};


#endif