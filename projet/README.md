//////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////INF 1900///////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
Projet 1: Initiation aux systems embarques avec la conception d'un robot suiveur de ligne

Robot Name: Strix
Equipe: Nawras Mohammed Amin, John Maliha, Johnny Khoury, Fadi Nourreddine.
Equipe 0809.

Le repertoire test_dir ne fait pas partie du projet final et n'est que la pour effectuer les tests
necessaire lors de la programmation.

//////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////ABOUT////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
Le premier projet d'integration du parcours conciste a la conception du code d'un robot suiveur de 
ligne. Le robot doit pouvoir passer a travers une serie de section dans un trajet donne. Le robot doit utiliser
des algorithmes jumeles avec les APIs des materiaux utilises pour la conception du robot. 


//////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////ROBOT////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
Avant tout parlons du robot:

Le robot est un 4-wheeled robot powered par une batterie de 9V et qui est controller par un
microcontrolleur de Atmel: le AVR ATMega324p. 
Ce Microcontrolleur a une frequence de CPU de 8MHz.

Le microcontrolleur a du etre utiliser pour programmer le robot entierement.

Pour le programmer, nous avons pu utiliser les composantes internes du ATMega324p:

    -3 Timers: TIMER0 (8Bit), TIMER1 (16Bit) et TIMER2 (8Bit).
    -PWM Ports
    -Un bouton interrupteur
    -Une del
    -UART and External Memory(Pas dans le ATMega324p).
    et beaucoup plus.

Les composantes physiques:

Les composantes particuliers du robot utilise dans le trajet sont:

    -Cytron Line Tracker (Detecteur de ligne)
    -SRF05 Sonar Ultrasound Detector (Detecteur de distance)
    -LCD Display (Utilise pour montrer la section courrante.)

Le robot utilisait aussi:
    -Des moteurs et un Pont-H, jumele avec 2 paires de roues controlles a travers les portD.
    (La specifications des ports complet se situe plus bas).
    -Un piezo electric programmer avec les TIMER2 du Atmega324p
    -Une memoire externe
    -Un breadboard jumele avec des resistances et un bouton de plus.

Les composantes logiciels:
    Le professeur (Jerome Collin) fournissait un programme appele serieViaUSB.
    Le programme permettiat de display dans le commandLine les inputs du robot en temps reel.

    Nous avions aussi ete fournie le code complet du LCD Display. 

//////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////PROGRAMMATION////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

Le robot devait etre programmer en utilisant la librairie que nous devions faire nous meme, ainsi qu'un Makefile, car nous programmions sur Linux.

Voici la liste des dependencies necessaire pour la compilation des programmes du robot sur Linux:

avrdude-5.3.1.tar.gz                avr-libc-user-manual-1.4.5.tar.bz2
avr-libc-1.4.5.tar.bz2              binutils-2.14.tar.gz
avr-libc-user-manual-1.4.5.pdf.bz2  gcc-4.1.1.tar.bz2

Si toute les composantes sont bien installer, en ce dirigeant dans le repertoire 
~/{PATH TO PROJECT DIR}/projet/exec_dir/
on peut executer la commande:
    ./make
qui est un script bash pour pouvoir lancer la procedure make install. 

Sinon, simplement ecrire
    make clean; make; make install
dans votre bash.

Nous avons utiliser avr-gcc qui est une derive de gcc (le Gnu C++ Compiler) pour programmer le robot
Nous avons utiliser avrdude, qui est un package developpe pour pouvoir make INSTALL (nous parlons de ca plus bas)

Nous avons grandement utiliser les informatiosn dans la documentation officiel du Atmega324p.

Pour pouvoir installer le code du logiciel dans le robot, il fallait utiliser des makefiles. 
Nous avons utiliser trois makefiles. Un pour la librairie, un commun et un pour le dir d'executables.

Le squelette du Makefile etait fournit par le professeur et les charges, mais nousa vions du modifies
quelques endroits pour pouvoir permettre la compilation de la librairie, ainsi que le linking de la lib 
avec les executables. Le makefile commun etait fait pour eviter les doublons de codes.

//////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////LIBRAIRIE//////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

La librairie a ete faite par nous et contient les classes necessaire pour utiliser les composantes du robot

Robot.cpp est notre classe dans laquelle nous avons stockee toute les methodes et les attributs necessaire
pour le fonctionnement des sections. Cette classe est le coeur de l'operation et englobe toute les autres
classes.

Timer.Cpp est une classe que nous avons concu pour permettre l'usage de n'importe quel timer dans n'importe
quel composante.

Minuterie.cpp est une version plus simple de l'appel d'un timer pour permettre de faire un compte a rebours ou
un compte en montee.

UART.cpp permet l'utilisation du uart pour transmettre de l'information dans la memoire et d'en recevoir aussi

Piezo.cpp permet l'utilisation du Piezo du robot et de faire du bruit et jouer des notes.

Moteur.cpp permet l'utilisation des moteurs du robot a travers une fonctionnalite PWM du Timer1.

Memoire_24.cpp permet l'utilisation des methodes pour gerer les operations sur la memoire externe.
Cette classe n'a pas ete ecrite par nous.

LightSensor.cpp est une classe creer pour gerer les capteurs. Elle n'est pas finale.

Lcm_So1602dtr_m_fw.cpp est une classe que nous n'avons pas ecrite. C'est la classe contenant les methode necessaire pour gerer l'ecran LCD.

Del.cpp est la classe que nous utilisons pour allumer la DEL.

CustomProcs.cpp est une classe que nous n'avons pas ecrite. C'est une autre classe utilisee pour permettre
le fonctionnement de l'ecran LCD.

Can.cpp est une classe que nous n'avons pas utiliser dans le projet final. Elle est necessaire pour convertir
les donnes analogique en numerique.

Bouton.Cpp permet d'obtenir l'information sur le stade du bouton poussoir interrupt


//////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////PORTS////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
DDRA: 
    6 4 2 0
    -------
    1 0 0 0     6 = Trigger du sonar
    0 0 0 0     0,1,2,3,4,5,7 = Cytron Line Tracker
    -------
    7 5 3 1

DDRB:
    6 4 2 0
    -------
    1 1 1 1     0,1 = Lumiere DEL libre
    1 1 1 1     2,3,4,5,6,7 = Unused
    -------
    7 5 3 1 

DDRC:
    6 4 2 0
    -------
    x x x x     DDRC Dedie au LCD Display. Les ports ne sont pas configures par nous.    
    x x x x     
    -------
    7 5 3 1 

DDRD: 
    6 4 2 0
    -------
    1 1 0 0     4,5,6,7 = Roues moteurs
    1 1 0 0     0,1,2,3 = boutons poussoirs.
    -------
    7 5 3 1

//////////////////////////////////////////////////////////////////////////////////
////////////////////////////////POUR TERMINER/////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

Nous avons beaucoup appris de ce projet et nous continuerons a explorer les systems embarques.


Merci!

