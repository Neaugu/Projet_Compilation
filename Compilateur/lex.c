#include "lex.h"

FILE * logfichier;

/* Permet de chercher si un mot appartient à la table des opérandes valide */
int seekop(char * word){

	char OPCODWDS[19][6]={"nop","add","sub","mult","div","and","or","xor","shl","shr","slt","sle","seq","load","store","jmp","braz","branz","scall"};
	int i=0;
	for(i=0;i<=18;i++){
		if(strcmp(word,OPCODWDS[i])==0){
			return i;
		}
	}
	return -1;	
}

/* Le Parser */
int Lex(Tableau * tab, Tableau * ERR){
	
	int reco=0;
	int flagerr=0;
	regex_t pregreg,pregint,pregsep,preglbl;
	logfichier=fopen("logfichier.txt","a");

	regcomp (&pregreg,REGISTRE, REG_NOSUB | REG_EXTENDED);
	regcomp (&pregint,ARGINT, REG_NOSUB | REG_EXTENDED);
	regcomp (&pregsep,SEPARATOR, REG_NOSUB | REG_EXTENDED);
	regcomp (&preglbl,LABEL, REG_NOSUB | REG_EXTENDED);

	fprintf(logfichier,"Phase de lexing : \n");
	fprintf(logfichier,"\n");

	while (tab->suivant!=NULL){

		if(strcmp(BALISEDEBUT,tab->mot)==0 && reco!=1){
			fprintf(logfichier,"Balise programme Mips : %s\n",tab->mot);
			tab->Codeword=0;
			reco=1;
		}
	
		if(regexec (&pregreg,tab->mot, 0, NULL, 0)==0 && reco!=1){
			fprintf(logfichier,"Registre : %s\n",tab->mot);
			tab->Codeword=1;
			reco=1;
		}
		
		if(regexec (&pregsep,tab->mot, 0, NULL, 0)==0 && reco!=1){
			fprintf(logfichier,"Separator : %s\n",tab->mot);
			tab->Codeword=6;
			reco=1;
		}
		if(seekop(tab->mot)>=0 && reco!=1){
			fprintf(logfichier,"Operande : %s\n",tab->mot	);
			tab->Codeword=2;
			reco=1;
		}

		if(regexec (&preglbl,tab->mot, 0, NULL, 0)==0 && reco!=1){
			fprintf(logfichier,"Label : %s\n",tab->mot);
			tab->Codeword=3;
			reco=1;
		}

		if(regexec (&pregint,tab->mot, 0, NULL, 0)==0 && reco!=1 ){
			fprintf(logfichier,"Entier : %s\n",tab->mot);
			tab->Codeword=4;
			reco=1;
		}

		if(strcmp(BALISEFIN,tab->mot)==0 && reco!=1){
			fprintf(logfichier,"Balise fin : %s\n",tab->mot);
			tab->Codeword=5;
			reco=1;
		}

		if(strcmp(FINLIGNE,tab->mot)==0 && reco!=1){
			fprintf(logfichier,"Fin de ligne : %s\n",tab->mot);
			tab->Codeword=7;
			reco=1;
		}

		if(reco==0){;
			sprintf(ERR->mot,"Error, wrong token used : %s",tab->mot);
			fprintf(logfichier,"Error, wrong token used : %s\n",tab->mot);
			ERR->suivant = malloc(sizeof(Tableau));
			ERR=ERR->suivant;
			flagerr=1;
		}
			
		tab=tab->suivant;
		reco=0;	
	}

	if(flagerr==1){
		return 1;
	}	

	fprintf(logfichier,"\n");
	fclose(logfichier);
	return 0;
}