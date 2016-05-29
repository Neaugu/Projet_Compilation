#include "interpret.h"

FILE * logfichier;

int buildtemplate(FILE * output,char * filepath){
	FILE * template;
	template=fopen(filepath,"r");

	if(template==NULL){
		printf("Missing source for prettyprint\n");
		return 1;
	}
	char temp;
	temp=fgetc(template);

	while(temp!=EOF){
		fprintf(output,"%c",temp);
		temp=fgetc(template);
	}

	fclose(template);
	return 0;

}

void prettyLabel(Tableau *tab,char *Buffer){

	if(tab->Codeword==3){
		strcat(Buffer,"<span class=label>");
		strcat(Buffer,tab->mot);
		strcat(Buffer,"</span>\n");
	}
}

void prettyOperande(Tableau *tab,char *Buffer){


	while(tab->Codeword!=7){
		switch(tab->Codeword){
			/* registre */
			case 1:
			strcat(Buffer,"<span class=register>");
			strcat(Buffer,tab->mot);
			strcat(Buffer,"</span>\n");
			tab=tab->suivant;
			break;
			/* operande */
			case 2:
			strcat(Buffer,"<span class=operand>");
			strcat(Buffer,tab->mot);
			strcat(Buffer,"</span>\n");
			tab=tab->suivant;
			break;
			/* label */
			case 3:
			strcat(Buffer,"<span class=label>");
			strcat(Buffer,tab->mot);
			strcat(Buffer,"</span>\n");
			tab=tab->suivant;
			break;
			/* entier */
			case 4:
			strcat(Buffer,"<span class=entier>");
			strcat(Buffer,tab->mot);
			strcat(Buffer,"</span>\n");
			tab=tab->suivant;
			break;
			/* Separateur , */
			case 6:
			strcat(Buffer,"<span class=separateur>");
			strcat(Buffer,tab->mot);
			strcat(Buffer,"</span>\n");
			tab=tab->suivant;
			break;
		}
	}
}	

/* ecrit un prettyprint sur un format html (afin d'avoir de la couleur) */
int Prettyprint(Tableau *tab,Instruction * CodeMachine){

	FILE * output;
	char *BufferHtml=malloc(1000*sizeof(char));
	system("rm PrettyMips.html 2> /dev/null");
	output=fopen("PrettyMips.html","a");

	/* creer le fichier a partir d'un fichier template source */
	buildtemplate(output,"src/Prettytemplate.css");
	fprintf(output, "<body>\n");
	fprintf(output, "<div>\n");

	if(tab->Codeword==0){

		fprintf(output, "<p>\n");
		fprintf(output,"<span class=header>");
		fprintf(output,"Autheur : Guillaume Neau IETA 2017<br>");
		fprintf(output,"Compilateur pour Mips-X<br>");
		fprintf(output,"<br>");
		fprintf(output,"%s\n",tab->mot);
		fprintf(output,"</span>\n");
		fprintf(output, "</p>\n");
	}

	tab=tab->suivant;
	tab=tab->suivant;
	fprintf(output, "<table>\n");

	while(tab->suivant!=NULL){

		if(tab->Codeword==5){
			break;
		}

		while(tab->Codeword!=7){

			fprintf(output,"<tr>\n");
			switch(tab->Codeword){

				/* balise operande */
				case 2:

					fprintf(output,"<td>\n");
					fprintf(output,"</td>\n");

					fprintf(output,"<td>\n");
					prettyOperande(tab,BufferHtml);
					fprintf(output,"%s\n",BufferHtml);
					fprintf(output,"</td>\n");

					fprintf(output,"<td class=binaire>");
					fprintf(output,"%s",strbin(CodeMachine->Instruct,32));
					fprintf(output,"</td>\n");
					CodeMachine=CodeMachine->suivant;

				break;

				/* balise label */
				case 3:
					fprintf(output,"<td>\n");
					prettyLabel(tab,BufferHtml);
					fprintf(output,"%s\n",BufferHtml);
					fprintf(output,"</td>\n");

					fprintf(output,"<td>\n");
					fprintf(output,"</td>\n");
				break;
			}

			fprintf(output, "</tr>\n");
			while(tab->Codeword!=7){
				tab=tab->suivant;
			}

		}
		resetBuffer(BufferHtml,1000);
		tab=tab->suivant;
	}

	if(tab->Codeword==5){

		fprintf(output,"<tr>\n");

		fprintf(output,"<td>\n");
		fprintf(output,"</td>\n");

		fprintf(output,"<td>\n");
		fprintf(output,"<span class=end>");
		fprintf(output,"%s",tab->mot);
		fprintf(output,"</span>\n");
		fprintf(output,"</td>\n");

		fprintf(output,"<td class=binaire>");
		fprintf(output,"%s",strbin(CodeMachine->Instruct,32));
		fprintf(output,"</td>\n");

		fprintf(output, "</tr>\n");
	}

	fprintf(output, "</table>\n");
	fprintf(output,"</div>");
	fprintf(output,"</body>");
	fprintf(output,"</html>\n");

	fclose(output);
	return 0;
}