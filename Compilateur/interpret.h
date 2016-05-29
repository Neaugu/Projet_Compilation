#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lex.h"

typedef struct Instruc_ Instruction;
struct Instruc_{
    unsigned Instruct;
    Instruction * suivant;
};

/*Fonction de bas niveau permettant le traitement de texte */
int Open(const char * filepath, Tableau *tab);
/* Fonction verifiant la grammaire */
int AST(Tableau * tab,Tableau * ERR);
/* Pretty printer */
int Prettyprint(Tableau *tab,Instruction *CodeMachine);
int PrettySource(Tableau *tab,char *path);
/*génère le code */
int CreateCode(Tableau *tab, Instruction *CodeMachine,Tableau *ERR);

/*--Partie Graphique--*/
void PrintCode(Instruction * CodeMachine);
char *strbin(const unsigned int n, const short nbBits );

/* Utilitaire */
void resetBuffer(char *buffer,int cpt);
int CreateSeq(Tableau *tab,Tableau *seq);


