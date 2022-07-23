# Test emploi développeur C++

## Requirements
Le programme fonctionne avec les plusieurs librairies en dehors de celles de bases. En plus de boost, la librairie Jsoncpp est utilisée.
Pour installer cette dernière (sur Debian/Ubuntu), il faut faire les lignes de commandes suivantes:
```
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
./bootstrap-vcpkg.sh
./vcpkg integrate install
./vcpkg install jsoncpp
```
OU
```
sudo apt-get install libsjoncpp-dev
sudo ln -s /usr/include/jsoncpp/json /usr/include/json
sudo apt-get install libssl-dev
```

Il est aussi intéressant de pouvoir utiliser les makefiles.

## Comment faire fonctionner

Pour lancer le projet, il suffit de taper:
```
make
```
Pour nettoyer les fichiers indésirable (.o), il suffit taper:
```
make clean
```
Pour vider les dossier LOGS et DRAWS, ainsi que les fichiers indésirables (.o), il suffit de taper:
```
make mrproper
```

## Comment l'utiliser

### Fichier Config

Le fichier config contient l'ensemble des paramètres initiaux du code.
Il est possible de paramétrer les jours où le code fonctionne, ainsi que les horaires et l'intervalle entre deux séries de tirage.
    `Interval` -> Le temps de l'intervalle en secondes
    `Lundi` -> 
        0 le programme ne fonctionne pas le lundi
        1 le programme fonctionne le lundi
    ...
    `Dimanche` ->
        0 le programme ne fonctionne pas le dimanche
        1 le programme fonctionne le dimanche
    `Schedule` -> Le planning de lancement (format "00h00 - 23h59". différents formats pas encore implémenté)


### Utilisation
Une fois configuré, il suffit de suivre les informations données dans la partie `Comment faire fonctionner`
Unis fois que le programme est compilé et prêt, il suffit de taper
```
./main
```

Feature:
    Taper `getLastDraw` lorsque le programme est lancé permet d'affiche le dernier tirage.

# Détails du code

Terminologie:
    - cDraw correspon à un seul tirage.
    - cDraws correspong à l'ensemble des tirages.

Dès que l'objet cDraws est créé, l'application se lance. Deux threads sont aussi lancés en parallèles lors de la construction:
    - Le premier s'occupe de faire les tirages toutes les X secondes.
    - Le second fait tourner l'API qui tourne en fond.

L'API écoute passivement les messages envoyés et répond à ces derniers.
