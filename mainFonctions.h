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
void printScreen(char * message, char posX, char posY);
//nettoyer
void clearScreen();

/*********************
** Terminal
*********************/

void instructions(uint8_t * instru, bool finInstructions);

/*********************
** Émission IR 
*********************/

void emitMessage(typeMessage * tableau);

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

void detection (void);
//édition
