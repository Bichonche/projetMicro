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

//variables globales de deplacement
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
** emission IR 
*********************/

void emitMessage(typeMessage * tableau);

/****************
** reception IR
****************/

//TODO Etienne, à toi de remplir cette partie

/***************
** SON
****************/



