#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

#define REGISTRE "^(r1)([0-9]){1}$|^(r2)([0-9]){1}$|^(r3)([0-1]){1}$|^r([0-9]){1}$"
#define ARGINT	"^[0-9]{1,6}"
#define SEPARATOR ","
#define LABEL "[[:alnum:]]{0,10}:$"
#define BALISEDEBUT "@Mips"
#define BALISEFIN "stop"
#define FINLIGNE ";"

typedef struct Tableau_ Tableau;
struct Tableau_{
    char mot[100];
    int Codeword;
    Tableau * suivant;
};

/* Permet de Lexer un texte et de faire le reporting d'erreur associé */
int Lex(Tableau *tab,Tableau *ERR);

/* seekop,Permet de retourner le numéros d'un opérande */
int seekop(char * word);


