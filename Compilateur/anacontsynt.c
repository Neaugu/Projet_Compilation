#include "interpret.h"

FILE * logfichier;

int CreateSeq(Tableau *tab,Tableau *seq){

	int cpt=0;

	tab=tab->suivant;

	while(tab->Codeword!=7){
	seq->Codeword=tab->Codeword;
	strcpy(seq->mot,tab->mot);

	seq->suivant = malloc(sizeof(Tableau));
	seq=seq->suivant;

	tab=tab->suivant;
	cpt++;
	}

	return cpt;

}

int ExpectSequenceScall(Tableau *seq){

	if(seq->Codeword!=4){
		return 1;
	}

	return 0;
}

int ExpectSequenceBrazBranz(Tableau *seq){
	int seq1[3]={1,6,3};
	int i=0;

	while(seq->suivant!=NULL){
		if(seq->Codeword!=seq1[i]){
			return 1;
		}
	i++;
	seq=seq->suivant;
	}
	return 0;
}

int ExpectSequenceJmp(Tableau *seq){

	if(seq->Codeword!=3){
		return 1;
	}

	return 0;

}

int ExpectSequenceStore(Tableau *seq){

	int seq1[5]={1,6,1,6,1};
	int seq2[5]={1,6,4,6,1};
	int i=0;

	while(seq->suivant!=NULL){
		if(seq->Codeword!=seq1[i] && seq->Codeword!=seq2[i]){
			return 1;
		}
		i++;
		seq=seq->suivant;
	}

	return 0;
}

int ExpectSequence14(Tableau *seq){

	int seq1[5]={1,6,1,6,1};
	int seq2[5]={1,6,4,6,1};
	int seq3[3]={1,6,1};
	int seq4[3]={4,6,1};
	int i=0;

	while(seq->suivant!=NULL){
		if(seq->Codeword!=seq1[i] && seq->Codeword!=seq2[i] && seq->Codeword!=seq3[i] && seq->Codeword!=seq4[i] ){
			return 1;
		}
		i++;
		seq=seq->suivant;
	}

	return 0;
}

int ExpectSyntOperande(Tableau *tab,Tableau *ERR){
	int classOperande=-1;
	int cpt=0;
	Tableau *seq = malloc(sizeof(Tableau));

	classOperande=seekop(tab->mot);

	if(classOperande==0){

		if(tab->suivant->Codeword!=7){

			fprintf(logfichier,"Syntax error : wrong construction of line @%s.\n",tab->mot);
			sprintf(ERR->mot,"Syntax error : wrong construction of line @%s.",tab->mot);
			ERR->suivant=malloc(sizeof(Tableau));
			ERR=ERR->suivant;

		}
	}

	if(classOperande>0 && classOperande<=13){

		cpt=CreateSeq(tab,seq);

		if(cpt>5){
			fprintf(logfichier,"Syntax error : Line overflow @%s.\n",tab->mot);
			sprintf(ERR->mot,"Syntax error : Line overflow @%s.",tab->mot);
			ERR->suivant=malloc(sizeof(Tableau));
			ERR=ERR->suivant;
		}

		if(cpt!=3 && cpt!=5){

			fprintf(logfichier,"Syntax error : wrong construction of line @%s.\n",tab->mot);
			sprintf(ERR->mot,"Syntax error : wrong construction of line @%s.",tab->mot);
			ERR->suivant=malloc(sizeof(Tableau));
			ERR=ERR->suivant;
		}
		
		if(ExpectSequence14(seq)==1){
			fprintf(logfichier,"Syntax error : wrong use of operande @%s.\n",tab->mot);
			sprintf(ERR->mot,"Syntax error : wrong use of operande @%s.",tab->mot);
			ERR->suivant=malloc(sizeof(Tableau));
			ERR=ERR->suivant;
		}

	}

	if(classOperande==14){
		cpt=CreateSeq(tab,seq);

		if(cpt>5){
			fprintf(logfichier,"Syntax error : Line overflow @%s.\n",tab->mot);
			sprintf(ERR->mot,"Syntax error : Line overflow @%s.",tab->mot);
			ERR->suivant=malloc(sizeof(Tableau));
			ERR=ERR->suivant;
		}
		if(ExpectSequenceStore(seq)==1){
			fprintf(logfichier,"Syntax error : wrong use of operande @%s.\n",tab->mot);
			sprintf(ERR->mot,"Syntax error : wrong use of operande @%s.",tab->mot);
			ERR->suivant=malloc(sizeof(Tableau));
			ERR=ERR->suivant;
		}
	}

	else{
		switch(classOperande){

			/* jump */
			case 15:

				cpt=CreateSeq(tab,seq);
				if(cpt>1){
					fprintf(logfichier,"Syntax error : Line overflow @%s.\n",tab->mot);
					sprintf(ERR->mot,"Syntax error : Line overflow @%s.",tab->mot);
					ERR->suivant=malloc(sizeof(Tableau));
					ERR=ERR->suivant;
				}
				if(ExpectSequenceJmp(seq)==1){
					fprintf(logfichier,"Syntax error : wrong use of operande @%s.\n",tab->mot);
					sprintf(ERR->mot,"Syntax error : wrong use of operande @%s.",tab->mot);
					ERR->suivant=malloc(sizeof(Tableau));
					ERR=ERR->suivant;
				}
			break;

			/* braz */
			case 16:
				cpt=CreateSeq(tab,seq);
				if(cpt>3){
					fprintf(logfichier,"Syntax error : Line overflow @%s.\n",tab->mot);
					sprintf(ERR->mot,"Syntax error : Line overflow @%s.",tab->mot);
					ERR->suivant=malloc(sizeof(Tableau));
					ERR=ERR->suivant;
				}
				if(ExpectSequenceBrazBranz(seq)==1){
					fprintf(logfichier,"Syntax error : wrong use of operande @%s.\n",tab->mot);
					sprintf(ERR->mot,"Syntax error : wrong use of operande @%s.",tab->mot);
					ERR->suivant=malloc(sizeof(Tableau));
					ERR=ERR->suivant;
				}
			break;
			/* branz */
			case 17:
				cpt=CreateSeq(tab,seq);
				if(cpt>3){
					fprintf(logfichier,"Syntax error : Line overflow @%s.\n",tab->mot);
					sprintf(ERR->mot,"Syntax error : Line overflow @%s.",tab->mot);
					ERR->suivant=malloc(sizeof(Tableau));
					ERR=ERR->suivant;
				}
				if(ExpectSequenceBrazBranz(seq)==1){
					fprintf(logfichier,"Syntax error : wrong use of operande @%s.\n",tab->mot);
					sprintf(ERR->mot,"Syntax error : wrong use of operande @%s.",tab->mot);
					ERR->suivant=malloc(sizeof(Tableau));
					ERR=ERR->suivant;
				}
			break;
			/* scall */
			case 18:
				cpt=CreateSeq(tab,seq);

				if(cpt>1){
					fprintf(logfichier,"Syntax error : Line overflow @%s.\n,",tab->mot);
					sprintf(ERR->mot,"Syntax error : Line overflow @%s.",tab->mot);
					ERR->suivant=malloc(sizeof(Tableau));
					ERR=ERR->suivant;
				}
				if(ExpectSequenceScall(seq)==1){
					fprintf(logfichier,"Syntax error : wrong use of operande @%s.\n",tab->mot);
					sprintf(ERR->mot,"Syntax error : wrong use of operande @%s.",tab->mot);
					ERR->suivant=malloc(sizeof(Tableau));
					ERR=ERR->suivant;
				}
			break;
		}
	}
	return 0;
}

int ExpectSyntLabel(Tableau *tab,Tableau *ERR){

	if(tab->suivant->Codeword!=7){

		fprintf(logfichier,"Syntax error : Wrong use of label @%s.\n",tab->mot);
		sprintf(ERR->mot,"Syntax error : Wrong use of label @%s.",tab->mot);
		ERR->suivant=malloc(sizeof(Tableau));
		ERR=ERR->suivant;
		return 1;
	}
	return 0;
}

int ExpectDebutligne(int code,Tableau * ERR){

	if(code!=2 && code!=3){
		return 1;

	}

	return 0;
}

int AST(Tableau * tab,Tableau * ERR){

	logfichier = fopen("logfichier.txt","a");
	
	if(tab->Codeword!=0){

		fprintf(logfichier,"Syntax error : <@Mips> must be written at the beginning of the program.\n");
		sprintf(ERR->mot,"Syntax error : <@Mips> must be written at the beginning of the program.");
		ERR->suivant = malloc(sizeof(Tableau));
		ERR=ERR->suivant;

		return 1;
	}

	tab=tab->suivant;

	while(tab->suivant!=NULL){

		if(tab->Codeword==5){
			break;
		}

		while(tab->Codeword!=7){

			if(ExpectDebutligne(tab->Codeword,ERR)==0){

				switch (tab->Codeword){
					/* Operande */
					case 2:
						ExpectSyntOperande(tab,ERR);
					break;

					/* Label */
					case 3:
						ExpectSyntLabel(tab,ERR);	
					break;
				}
			}

			else{
				fprintf(logfichier,"Syntax error : line must begin by either label or operand.\n");
				sprintf(ERR->mot,"Syntax error : line must begin by either label or operand.");
				ERR->suivant = malloc(sizeof(Tableau));
				ERR=ERR->suivant;
			}

			while(tab->Codeword!=7){
				tab=tab->suivant;
			}
		}
		
		tab=tab->suivant;
	}

	if (tab->Codeword!=5){

		fprintf(logfichier,"Syntax error : <stop> must be written at the end of the program.\n");
		sprintf(ERR->mot,"Syntax error : <stop> must be written at the end of the program.");
		ERR->suivant = malloc(sizeof(Tableau));
		ERR=ERR->suivant;
		return 1;

	}

	fclose(logfichier);
	return 0;
}

