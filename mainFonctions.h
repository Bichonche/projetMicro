/*********************************************************
* variables globales
**********************/
// variables globales messages
typedef struct message
{
	char dir;
	int valeur;
}typeMessage;

typeMessage tabMessage[8];

//variables globales de déplacement
int speed, distance, angle;
/**************************
**	Deplacements				**
**************************/
//avancer
void forward(int speed, int distance);
//tourner
void rotation(int angle);
//arreter
void stop();

/*******************
** Affichage  ecran LCD
********************/

//initialiser l'ecran
void initScreen();
//affichage
void printScreen(uint8_t * message, uint8_t posX, uint8_t posY);
//nettoyer
void clearScreen();

/*********************
** Terminal
*********************/

void initTerminal();
//Appele par une interruption:
void tabDonnees(uint8_t tableau[16], bool * finTab); //Apres avoir mis un caractère, mettre flag à 0.


/*********************
** Émission IR 
*********************/

void emitMessage(char, uint8_t, bool);

/****************
** Réception IR
****************/

//TODO Etienne, à toi de remplir cette partie

/***************
** SON
****************/

//TODO by Etienne et Bichonche


/***************
** Gestion de la distance
****************/

void detectionglobale (void);
//édition
