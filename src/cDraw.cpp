#include "cDraw.hpp"


using namespace std;

namespace logging = boost::log;
namespace keywords = boost::log::keywords;

cDraw::cDraw()
{
    this->id = static_cast<long int> (time(NULL));

    // Initialisation du fichier du sink
    logging::add_common_attributes();

    logging::add_file_log(
        keywords::file_name = "./LOGS/tirage_" + to_string(this-> id) + ".log",
        keywords::auto_flush = true
    );

    logging::core::get()->set_filter
    (
        logging::trivial::severity >= logging::trivial::info
    );

};

cDraw::~cDraw(){}; // Destructeur.

long int cDraw::get_id()
{
    return this->id;
};

void cDraw::tirage_aleatoire()
{ // On effectue 9 tirages aléatoires entre 0 et 9 que l'on place dans la list tirages.
    srand(time(NULL)); // Pas de réel aléatoire mais je pense que ce n'est pas le but de l'exercice.
    for (int i = 0; i < 9; ++i)
    {
        tirages[i] = rand() % 10;
    }
    this->localday = boost::gregorian::day_clock::local_day();
    time_t now = time(0);
    this->utc = gmtime(&now); // utc time
};

void cDraw::afficher_tirage()
{
    cout << "Tirage " << this->id << ": ";
    for (int numero : this->tirages)
    {
        cout << numero << " ";
    }
    cout << endl;
};

void cDraw::save_tirage()
{
    // On crée un fichier json contenant les données du tirage dans DRAWS puis un ficher log dans LOGS

    ofstream tirage_file("./DRAWS/tirage_" + to_string(this->id) + ".json", ofstream::app);
    Json::Value tirage_actuel;                   // Création d'un objet json pour stocker les données du tirage actuel.
    Json::Value liste_tirages(Json::arrayValue); // Création d'un objet json pour mettre au bon format les valeurs du tirages.
    
    string temp = ""; // Variable temporaire pour stocker les valeurs du tirage, que l'on placera ensuite dans rec.attribute_values["Tirage"] et BOOST_LOG_TRIVIAL(info)
    for (int i = 0; i < 9; ++i)
    {
        liste_tirages.append(Json::Value(this->tirages[i]));
        temp += to_string(this->tirages[i]) + " " ;
    }

    string UTC_format = to_string(this->utc->tm_year+1900) + '-' + to_string(this->utc->tm_mon+1) + '-' + to_string(this->utc->tm_mday) + ' ' + to_string(this->utc->tm_hour) + ':' + to_string(this->utc->tm_min) + ':' + to_string(this->utc->tm_sec);
    
    BOOST_LOG_TRIVIAL(info) << "Tirage " << to_string(this->id) << " : " << UTC_format << endl << temp << endl;

    string date = to_simple_string(this->localday);

    // On écrit toutes les informations au format Json.
    Json::StyledWriter writer;
    tirage_actuel["UID"] = Json::Value((int)(this->id));
    tirage_actuel["tirage"] = liste_tirages;
    tirage_actuel["date"] = date;
    tirage_actuel["date_UTC"] =  UTC_format;
    tirage_file << writer.write(tirage_actuel);
    tirage_file.close();
    cout << "Tirage " << this->id << " enregistré" << endl;
    return;
};

int cDraw::get_tirage(int i)
{
    return this->tirages[i];
};

time_t cDraw::get_utc(){
    return mktime(this->utc);
};

struct tm cDraw::get_localday(){
    return to_tm(this->localday);
};

string cDraw::get_tirages()
{
    string temp = "";
    for (int i = 0; i < 9; ++i)
    {
        temp += to_string(this->tirages[i]) + " ";
    }
    return temp;
};

