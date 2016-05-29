#include "interpret.h"

int PrettyStandard(Tableau *tab,char *BufferStandard){

	strcat(BufferStandard,tab->mot);
	strcat(BufferStandard," ");

	tab=tab->suivant;

	while(tab->Codeword!=7){

		strcat(BufferStandard,tab->mot);
		tab=tab->suivant;

	}

	return 0;
}

int PrettySource(Tableau *tab,char *path){

	FILE * output;
	char *BufferStandard=malloc(1000*sizeof(char));
	output=fopen(path,"w+");
;
	if(tab->Codeword==0){
		fprintf(output,"%s\n",tab->mot);
	}

	tab=tab->suivant;
	tab=tab->suivant;

	while(tab->suivant!=NULL){

		if(tab->Codeword==5){
			break;
		}

		while(tab->Codeword!=7){

			switch(tab->Codeword){

				case 2:

				fprintf(output,"\t");
				PrettyStandard(tab,BufferStandard);
				fprintf(output,"%s\n",BufferStandard);

				break;

				case 3:
				fprintf(output, "%s\n",tab->mot);
				break;
			}


			while(tab->Codeword!=7){
				tab=tab->suivant;
			}

		}
		resetBuffer(BufferStandard,1000);
		tab=tab->suivant;
	}

	if(tab->Codeword==5){

		fprintf(output,"\t");
		fprintf(output, "%s\n",tab->mot);

	}
	fclose(output);
	return 0;
}