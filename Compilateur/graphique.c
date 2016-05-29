#include "interpret.h"

FILE * logfichier;

/* Permet de faire un affichage en binaire */
char* strbin(const unsigned int n, const short nbBits )
{  
  unsigned bit = 0 ;
  unsigned mask = 1 ;
  char* buffer = calloc( nbBits + 1, sizeof( char ) );
  int i;
  for (i = 0 ; i < nbBits ; ++i) {
    bit = (n & mask) >> i ;
    buffer[ nbBits - 1 - i ] = (char)('0' + bit);
    mask <<= 1 ;
  }
  buffer[nbBits] = '\0';
  return buffer;
}

/* Permet d'afficher le code dans la console en une seule ligne */
void PrintCode(Instruction * CodeMachine){

	logfichier = fopen("logfichier.txt","a");
	char *bina;

	fprintf(logfichier,"Code binaire généré : \n");
	fprintf(logfichier,"\n");

	while(CodeMachine->Instruct!=0b10011000000000000000000000000000){

	bina=strbin(CodeMachine->Instruct,32);
	printf("%s\n",bina);
	fprintf(logfichier,"%s\n",bina);
	CodeMachine=CodeMachine->suivant;

	}

	bina=strbin(CodeMachine->Instruct,32);
	fprintf(logfichier,"%s\n",bina);
	printf("%s\n",bina);

	fclose(logfichier);

}