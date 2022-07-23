#include "cDraw.hpp"
#include "cDraws.hpp"

#define port "1664"


using namespace std;


int main(int argc, char* argv[]){

    cDraws draws; // when created, the threads with the API and the Draws picks will be launched
    string exit;
    exit = "continue";
    while(exit != "exit"){
        // Keep the program running until the user quits
        cin >> exit;
        if (exit == "getLastDraw"){
            cDraw t = draws.getLastDraw("");
            t.afficher_tirage();
        }
    }
    return 0;

}