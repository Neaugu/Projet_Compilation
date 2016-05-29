#include "interpret.h"

FILE * logfichier;

/* Permet de reset un buffer de texte */
void resetBuffer(char *buffer,int cpt){
	
	int i;
	for(i=0;i<=cpt;i++){
		buffer[i]='\0';
	}


}

/* Permet de lexer le texte */
int Open(const char * filepath, Tableau *tab){

	logfichier = fopen("logfichier.txt","a");
	fprintf(logfichier,"Ouverture de : %s\n",filepath);
	fprintf(logfichier,"\n");
	FILE * file=NULL;
	
	char temp;
	char *temp1=malloc(100*sizeof(char));
	int i=0;
	
	file =fopen(filepath,"r");
	
	if (file!=NULL) 
	{
		temp=fgetc(file);
		
		while(temp!=EOF)
		{
			/* evite les espace automatiquement, ils n'ont pas de sens */
			if(temp!=' ' && temp!='\t' && temp!='\0'){
				/* premier separateur */
				if(temp==','){

					strcpy(tab->mot,temp1); 
					resetBuffer(temp1,i);
					tab->suivant = malloc(sizeof(Tableau));
					tab = tab->suivant;
					i=0;
					
					*tab->mot=',';
					tab->suivant = malloc(sizeof(Tableau));
					tab = tab->suivant;
					temp=fgetc(file);

				}
				/* deuxieme separateur */
				if(temp=='\n' && temp!='\r'){
					
					if(strcmp(temp1,"")!=0){

						strcpy(tab->mot,temp1); 
						resetBuffer(temp1,i);
						tab->suivant = malloc(sizeof(Tableau));
						tab = tab->suivant;
						i=0;
					}
					
					*tab->mot=';';
					tab->suivant = malloc(sizeof(Tableau));
					tab = tab->suivant;
					temp=fgetc(file);
					
				}
				else{
					if(temp!=' '){
						temp1[i]= (char) temp;	
						i++;
						temp=fgetc(file);
					}
				}
			}

			else{
				
				if(strcmp(temp1,"")!=0){
				
					strcpy(tab->mot,temp1); 
					resetBuffer(temp1,i);
					tab->suivant = malloc(sizeof(Tableau));
					tab = tab->suivant;
					i=0;
					
				}
				temp=fgetc(file);
			}
			
		}
		fclose(file);
	}
	else
	{
		fprintf(logfichier,"Fichier introuvable\n");
		printf("Fichier introuvable\n");
		return 1;
	} 

	fclose(logfichier);
	return 0;
}
