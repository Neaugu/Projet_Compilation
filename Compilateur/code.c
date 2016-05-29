#include "interpret.h"

FILE * logfichier;

/*permet de retourner le numeros de registre associé */
int seekreg(char *word){

	/* Oui c'est moche mais les autre solution ne sont pas plus économe en ligne ...... */
	char REGTBL[32][4]={"r0","r1","r2","r3","r4","r5"
						,"r6","r7","r8","r9","r10"
						,"r11","r12","r13","r14","r15"
						,"r16","r17","r18","r19","r20"
						,"r21","r22","r23","r24","r25"
						,"r26","r27","r28","r29","r30"
						,"r31"};
	int i=0;
	for(i=0;i<=32;i++){
		if(strcmp(word,REGTBL[i])==0){
			return i;
		}
	}
	return -1;	
}

/* Permet la recherche de label en debut de ligne */
int seeklabel(char *word,Tableau *copie){
	int compteurligne=0;

	while(copie->suivant!=NULL){

		if(copie->Codeword==5){
		break;
		}

		while(copie->Codeword!=7){

			if(strcmp(word,copie->mot)==0){
				return compteurligne;
			}
		

			while(copie->Codeword!=7){
				copie=copie->suivant;
			}
			compteurligne++;
		}
		copie=copie->suivant;
	}

	return -1;
}
/* permet de creer les instructions pour les syntaxe générique */
void CreateInstruc(Tableau *sequence,int cpt,unsigned *breg1,unsigned *boo, unsigned * bparam,unsigned *breg2){

	*breg1=0;
	*breg2=0;
	*boo=0;
	*bparam=0;

	if (cpt==5){
		*breg1=seekreg(sequence->mot)<<22;

		/* On passe le separateur */
		sequence=sequence->suivant;
		sequence=sequence->suivant;
		/*fin de passage de separateur */

		switch(sequence->Codeword){

		/* l'argument est un entier */
		case 4:
		* boo=0<<21;
		* bparam=atoi(sequence->mot)<<5;
		break;

		/* l'argument est un registre */
		case 1:
		* boo=1<<21;
		*bparam=seekreg(sequence->mot)<<5;
		break;
		}

		/* On passe le separateur */
		sequence=sequence->suivant;
		sequence=sequence->suivant;
		/*fin de passage de separateur */
		* breg2=seekreg(sequence->mot);
	}

	if (cpt==3){

		switch(sequence->Codeword){

			/* l'argument est un entier */
			case 4:
			* boo=0<<21;
			* bparam=atoi(sequence->mot)<<5;
			break;

			/* l'argument est un registre */
			case 1:
			* boo=1<<21;
			* bparam=seekreg(sequence->mot)<<5;
			break;
		}

		/* On passe le separateur */
		sequence=sequence->suivant;
		sequence=sequence->suivant;
		/*fin de passage de separateur */	
		* breg2=seekreg(sequence->mot);
	}

} 

/* Permet de creer le code associe a chaque operande, CreateSeq() est une méthode issue de l'ast permettant d'extraire une ligne de code */
int CreatecodeOperande14(Tableau *tab,unsigned *INSTRUC){

	Tableau *sequence=malloc(sizeof(Tableau));
	int cpt=0;
	unsigned bopcode=0;
	unsigned *breg1=malloc(5*sizeof(unsigned));
	unsigned *boo=malloc(sizeof(unsigned));
	unsigned *bparam=malloc(16*sizeof(unsigned));
	unsigned *breg2=malloc(5*sizeof(unsigned));

	bopcode=seekop(tab->mot)<<27;
	cpt=CreateSeq(tab,sequence);
	CreateInstruc(sequence,cpt,breg1,boo,bparam,breg2);
	*INSTRUC= bopcode | *breg1 | *boo | *bparam | *breg2;

	return 0;
}
/* Permet de cree le code associe aux jump */
int CreatecodeOperandeJump(Tableau *tab,Tableau *copie,unsigned *INSTRUC){

	unsigned bopcode=0;
	unsigned breg1=0;
	unsigned boo=0;
	unsigned bparam=0;
	unsigned breg2=0;
	bopcode=seekop(tab->mot)<<27;

	/* On pointe l'argument suivant */
	tab=tab->suivant;

	bparam=seeklabel(tab->mot,copie)+1;
	if(bparam==-1){
		*INSTRUC= bopcode | breg1 | boo | bparam | breg2;
		return 1;
	}

	bparam=bparam<<5;
	*INSTRUC= bopcode | breg1 | boo | bparam | breg2;
	return 0;
}

int CreatecodeOperandeBranzBraz(Tableau *tab,Tableau *copie,unsigned *INSTRUC){

	unsigned bopcode=0;
	unsigned breg1=0;
	unsigned boo=0;
	unsigned bparam=0;
	unsigned breg2=0;
	bopcode=seekop(tab->mot)<<27;

	/* On pointe l'argument suivant*/
	tab=tab->suivant;
	breg1=seekreg(tab->mot)<<22;

	/* On pointe l'argument suivant*/
	tab=tab->suivant;
	tab=tab->suivant;
	/* deux saut à cause du séparateur */

	bparam=seeklabel(tab->mot,copie);

	if(bparam==-1){
		*INSTRUC= bopcode | breg1 | boo | bparam | breg2;
		return 1;
	}

	bparam=bparam<<5;

	*INSTRUC= bopcode | breg1 | boo | bparam | breg2;

	return 0;
}

int CreatecodeOperandeScall(Tableau *tab,unsigned *INSTRUC){
	
	unsigned bopcode=0;
	unsigned breg1=0;
	unsigned boo=0;
	unsigned bparam=0;
	unsigned breg2=0;
	bopcode=seekop(tab->mot)<<27;

	/* On pointe l'argument suivant*/
	tab=tab->suivant;
	bparam=atoi(tab->mot)<<5;

	*INSTRUC= bopcode | breg1 | boo | bparam | breg2;
	return 0;
}	
/* Sachant que tout les vérifications d'erreurs on été faite en amont, on génère le code en s'appuyant sur la syntaxte optimale */
int CreateCode(Tableau *tab,Instruction *CodeMachine,Tableau *ERR){

	logfichier = fopen("logfichier.txt","a");
	unsigned *INSTRUC=malloc(sizeof(unsigned));
	Tableau *temp;

	/* Permet de gérer la balise de début de programme */
	if(tab->Codeword!=0){
		return 1;
	}

	tab=tab->suivant;
	temp=tab;
	/* fin de gestion de la balise de début de programme */

	while(tab->suivant!=NULL){

		if(tab->Codeword==5){
			break;
		}

		while(tab->Codeword!=7){

				/* On va creer le code associer à chaque operande */
				if(tab->Codeword==2){

					if(seekop(tab->mot)==0){
						CodeMachine->Instruct=0;
						CodeMachine->suivant=malloc(sizeof(Instruction));
						CodeMachine=CodeMachine->suivant;
					}

					if( seekop(tab->mot)>0 && seekop(tab->mot)<=14){
						CreatecodeOperande14(tab,INSTRUC);
						CodeMachine->Instruct=*INSTRUC;
						CodeMachine->suivant=malloc(sizeof(Instruction));
						CodeMachine=CodeMachine->suivant;
					}
					if(seekop(tab->mot)==15){
						if(CreatecodeOperandeJump(tab,temp,INSTRUC)==1){

							fprintf(logfichier,"Syntax error : missing label reference @%s when refering to '%s'\n",tab->mot,tab->suivant->mot);
							sprintf(ERR->mot,"Syntax error : missing label reference @%s when refering to '%s'",tab->mot,tab->suivant->mot);
							ERR->suivant=malloc(sizeof(Tableau));
							ERR=ERR->suivant;
							return 1;
						}
						CodeMachine->Instruct=*INSTRUC;
						CodeMachine->suivant=malloc(sizeof(Instruction));
						CodeMachine=CodeMachine->suivant;

					}

					if(seekop(tab->mot)>15 && seekop(tab->mot)<18){

						if(CreatecodeOperandeBranzBraz(tab,temp,INSTRUC)==1){

							fprintf(logfichier,"Syntax error : missing label reference @%s when refering to '%s'\n",tab->mot,tab->suivant->suivant->suivant->mot);
							sprintf(ERR->mot,"Syntax error : missing label reference @%s when refering to '%s'",tab->mot,tab->suivant->suivant->suivant->mot);
							ERR->suivant=malloc(sizeof(Tableau));
							ERR=ERR->suivant;
							return 1;
						}
						CodeMachine->Instruct=*INSTRUC;
						CodeMachine->suivant=malloc(sizeof(Instruction));
						CodeMachine=CodeMachine->suivant;

					}

					if(seekop(tab->mot)==18){

						CreatecodeOperandeScall(tab,INSTRUC);
						CodeMachine->Instruct=*INSTRUC;
						CodeMachine->suivant=malloc(sizeof(Instruction));
						CodeMachine=CodeMachine->suivant;
					}
			}
			/*Permet de revenir en début de ligne */
			while(tab->Codeword!=7){
				tab=tab->suivant;
			}
		}
		tab=tab->suivant;
	}

	if(tab->Codeword==5){

		CodeMachine->Instruct=0b10011000000000000000000000000000;
		CodeMachine->suivant=malloc(sizeof(Instruction));
		CodeMachine=CodeMachine->suivant;
	}
	fclose(logfichier);
	return 0;
}
