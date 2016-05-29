#include "interpret.h"

void printErr(Tableau *ERR){

			while (ERR->suivant!=NULL){
			printf("%s\n",ERR->mot);
			ERR=ERR->suivant;
			}
}

int main (int argc, char * argv[]){
	
	system("rm logfichier.txt 2> /dev/null");
	Tableau *tab = malloc(sizeof(Tableau));	
	Tableau *ERR = malloc(sizeof(Tableau));
	Instruction *CodeMachine=malloc(sizeof(Instruction));

	if(argc<2){
		printf("File needed\n");
		return 0;
	}

	if(Open(argv[1],tab)==0){

		if (Lex(tab,ERR)==1){
			printErr(ERR);
			return 0;
		}

		AST(tab,ERR);

		if (ERR->suivant!=NULL){
			printErr(ERR);
			return 0;
		}

		if(CreateCode(tab,CodeMachine,ERR)==1){
			printErr(ERR);
			return 0;
		}

		PrintCode(CodeMachine);
		
		Prettyprint(tab,CodeMachine);
		PrettySource(tab,argv[1]);
	}

	return 0;
}
