#include <string>
#include <cstdlib>
#include <ctime>
#include <json/value.h>
#include <json/writer.h>
#include <json/json.h>
#include <fstream>
#include <iostream>

// Partie avec les modules boosts

// Partie pour les logs
#include <boost/log/core.hpp> // pour le logging
#include <boost/log/trivial.hpp> // Le trivial est le niveau le plus bas.
#include <boost/log/expressions.hpp> // pour le filtre de logging
#include <boost/log/utility/setup/file.hpp> // pour le fichier de log BOOST 
#include <boost/log/utility/setup/common_attributes.hpp> // pour les attributs BOOST (date, heure, etc.)
#include <boost/log/sources/logger.hpp> // Pour le logger BOOST (logger)
#include <boost/log/sources/record_ostream.hpp> // Pour le logger BOOST (pour le cout)
#include <boost/log/sources/severity_logger.hpp> // Pour le logger BOOST (avec le niveau)
#include <boost/log/sources/global_logger_storage.hpp> // Pour le logger BOOST
#include <boost/log/sources/basic_logger.hpp> // Pour le logger BOOST 
#include <boost/log/support/date_time.hpp> // Pour le logger BOOST (afficher l'heure)

// Partie pour la date
#include <boost/date_time/gregorian/gregorian.hpp> // Pour les dates que l'on sauvegarde dans le fichier log et draws
#include <boost/date_time/posix_time/posix_time.hpp> // Pour les dates que l'on sauvegarde dans le fichier log et draws


#ifndef CDRAW_H
#define CDRAW_H

#define BOOST_LOG_DYN_LINK 1
class cDraw
{
    private:
        long int id; // Identifitant du tirage
        boost::gregorian::date localday; // Date du tirage
        struct tm *utc; // Date du tirage 
        int tirages[9]; // Liste des numéros tirés
        boost::log::sources::severity_logger< boost::log::trivial::severity_level > slg; // Pour le logger
        
        
    public:
        cDraw();
        ~cDraw();
        long int get_id();
        void tirage_aleatoire();
        void afficher_tirage();
        void save_tirage();
        int get_tirage(int i);
        time_t get_utc();
        struct tm get_localday();
        std::string get_tirages();
        // TODO: add &operator= to Tirage 
};



#endif // CDRAW_H