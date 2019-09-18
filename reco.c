#include "projet.h"

char *TitreFilm(int IdFilm){
    FILE *fichier=fopen("movie_titles.csv","r");
    char *ligne=malloc(T_MAX*sizeof(char));
    while(fgets(ligne,T_MAX,fichier)){
        int test=atoi(strtok(ligne,","));
        if(IdFilm==test){
            int i=0;
            while(ligne[i]!='\0'){
                i++;
            }
            fclose(fichier);
            char *sav=&ligne[i+6];
            free(ligne);
            return sav;
        }
    }
    return NULL;
}

double Reco1(Maillon **Utilisateurs, int IdFilm, Voisin *V, int k){
    double nom=0,denom=0;
    int i; //i parcourt le tableau de voisins en argument
    Maillon *p; //p parcourt l'ensemble des notes données par chaque voisin
    for(i=0;i<k;i++){
        p=Utilisateurs[V[i].IdVoisin-1];
        while(p!=NULL){
            if(p->IdFilm==IdFilm){
                nom+=V[i].similarite*p->note;
                if(V[i].similarite>=0){
                    denom+=V[i].similarite;
                }
                else{
                    denom-=V[i].similarite;
                }
                break;
            }
            p=p->NextFilm;
        }
    }
    if(nom==0&&denom==0){
        return 0; //On renvoie 0 lorsque la note estimée n'a pas pu être calculée, ce qui est le cas lorsque aucun des voisins n'a vu le film en question
    }
    return nom/denom;
}

FilmReco *Reco2(Maillon **Utilisateurs, int U, Voisin *V, int k){
    FilmReco *Recommandations=malloc(10*sizeof(FilmReco));
    Maillon *p=Utilisateurs[U-1];
    int NF=NB_FILMS,cmp=0,min;
    float note;
    while(p!=NULL){
        while(p->IdFilm!=NF&&NF>0){ //On calcule une estimation de la note lorsque l'utilisateur n'a pas vu le film d'identifiant NF
            if(NF%1000==0) printf("Données traitées : %d%c\n", 100-(NF*100/17770),'%');
            note=Reco1(Utilisateurs,NF,V,k);
            if(cmp<10){
                Recommandations[cmp].IdUser=U;
                Recommandations[cmp].IdFilm=NF;
                Recommandations[cmp].estimation=note;
                cmp++;
            }
            else{
                min=MinNote(Recommandations,10);
                if(Recommandations[min].estimation<note){
                	Recommandations[min].IdFilm=NF;
    	            Recommandations[min].estimation=note;
                }
        	}
            NF--;
        }
        NF--;
        p=p->NextFilm;
    }
    while(NF>0){
        note=Reco1(Utilisateurs,NF,V,k);
        min=MinNote(Recommandations,10);
        if(Recommandations[min].estimation<note){
            Recommandations[min].IdFilm=NF;
            Recommandations[min].estimation=note;
        }
        NF--;
    }
    printf("100%c des données ont été traitées\n",'%');
    return Recommandations;
}

void Reco3(char* argv[],Maillon **Films, Maillon **Utilisateurs, int k){
    FILE* f=fopen(argv[5],"r");
    char nom[12]="outputX.txt";
    nom[6]=argv[5][5];
    FILE* fOUT=fopen(nom,"w");
    if(f==NULL){
        printf("fichier pas trouvé\n");
        return;
    }
    if(fOUT!=NULL){
        char ligne[T_MAX];
        int IdFilm=0,x;
        while(fgets(ligne,T_MAX,f)){
            if(strchr(ligne,':')!=NULL){
                IdFilm=atoi(ligne);
                fprintf(fOUT,"%s",ligne);
            }
            else{
                x=atoi(ligne);
                Voisin *V=Voisins(Films, Utilisateurs,x,k);

                #ifdef DEBUG
                printf("Voisins de U%d :\n",x);
                int i;
                for(i=0;i<k;i++){
                    printf("V%d similarité=%f\n",V[i].IdVoisin,V[i].similarite);
                }
                #endif

                float estimation=Reco1(Utilisateurs, IdFilm, V, k);
                #ifdef DEBUG
                printf("estimation de U%d pour F%d : %f\n",x,IdFilm,estimation);
                #endif

                fprintf(fOUT,"%d, %f\n",x,estimation);
            }
        }
        fclose(f);
        fclose(fOUT);
    }
}

int *FilmsPop(Maillon **Films, int k){
    int NBNotes[NB_FILMS]={0};
    int *FP=malloc(k*sizeof(int));
    int i,j;
    Maillon *p;
    for(i=0;i<NB_FILMS;i++){
        p=Films[i];
        while(p!=NULL){
            NBNotes[i]++;
            p=p->NextUser;
        }
    }
    int max,imax=0;
    for(i=0;i<k;i++){
        max=0;
        for(j=0;j<NB_FILMS;j++){
            if(NBNotes[j]>max){
                max=NBNotes[j];
                imax=j;
            }
        }
        FP[i]=imax+1;
        NBNotes[imax]=0;
    }
    return FP;
}

int NouvelUtilisateur(Maillon **Utilisateurs){
    int i;
    for(i=0;i<NB_USERS;i++){
        if(Utilisateurs[i]==NULL){
            return i+1;
        }
    }
    return -1;
}

FilmReco *Reco4(Maillon **Films, Maillon **Utilisateurs){
    int *FP=FilmsPop(Films,10);
    int IdUser=NouvelUtilisateur(Utilisateurs);
    int i, verif=0;
    int note;
    printf("Merci de saisir une note pour les films suivants :\n");
    for(i=0;i<10;i++){
        printf("%s",TitreFilm(FP[i]));
        int c;
        while((c=getchar())!='\n'&&c!=EOF); //On vide le buffer pour ne pas avoir d'erreur en utilisant scanf (portion de code trouvée sur Internet)
        verif=scanf("%d",&note);
        while (verif!=1||note>5||note<1){
            printf("Veuillez entrer une note comprise entre 1 et 5.\n");
            while((c=getchar())!='\n'&&c!=EOF);
            verif = scanf("%d",&note);
        }
        AjoutMaillon(Films,Utilisateurs,FP[i],IdUser,note);
    }	
    Voisin *V=Voisins(Films,Utilisateurs,IdUser,NB_VOISINS/10);
    FilmReco *Recommandations=Reco2(Utilisateurs, IdUser, V, NB_VOISINS/10);
    free(V);
    free(FP);
    return Recommandations;
}

void Reco(char *argv[], Maillon **Films, Maillon **Utilisateurs){
    int verif, choix;
    printf("Choisir un type de recommandation (1,2,3 ou 4) : ");
    verif=scanf("%d",&choix);
    while(verif!=1||choix<1||choix>4){
        printf("Merci de saisir un choix valide : ");
        int c;
        while((c=getchar())!='\n'&&c!=EOF);
        verif=scanf("%d",&choix);
    }
    
    #ifdef CHRONO
    double debut=0,fin=0;
    #endif
    
    if(choix==1){
        int IdUser, IdFilm, verif=0;
        while(verif!=1||IdUser<1||IdUser>2649429||Utilisateurs[IdUser-1]==NULL){
        	printf("Saisir un identifiant d'utilisateur : ");
        	int c;
        	while((c=getchar())!='\n'&&c!=EOF);
        	verif=scanf("%d",&IdUser);
		if(Utilisateurs[IdUser-1]==NULL) printf("Cet utilisateur n'a noté aucun film.\n");
        }
        verif=0;
        while(verif!=1||IdFilm<1||IdFilm>17770){
        	printf("Saisir un identifiant de film : ");
        	int c;
        	while((c=getchar())!='\n'&&c!=EOF);
        	verif=scanf("%d",&IdFilm);
        }
        Voisin *V=Voisins(Films,Utilisateurs,IdUser,NB_VOISINS);
        
        #ifdef CHRONO
        debut=time(NULL);
        #endif
        
        printf("Note estimée de l'utilisateur %d au film %d : %f\n",IdUser,IdFilm,Reco1(Utilisateurs,IdFilm,V,NB_VOISINS));
        
        #ifdef CHRONO
        fin=time(NULL);
        #endif
        free(V);
    }
    else if(choix==2){
        int IdUser,verif=0;
        while(verif!=1||IdUser<1||IdUser>2649429||Utilisateurs[IdUser-1]==NULL){
        	printf("Saisir un identifiant d'utilisateur : ");
        	int c;
        	while((c=getchar())!='\n'&&c!=EOF);
        	verif=scanf("%d",&IdUser);
        	if(Utilisateurs[IdUser-1]==NULL) printf("Cet utilisateur n'a noté aucun film.\n");
        }
        Voisin *V=Voisins(Films,Utilisateurs,IdUser,NB_VOISINS/10);
        
        #ifdef CHRONO
        debut=time(NULL);
        #endif
        
        FilmReco *F=Reco2(Utilisateurs,IdUser,V,NB_VOISINS/10);
        TriNote(F,10);
        int i;
        printf("10 films recommandés pour l'utilisateur %d :\n",IdUser);
        for(i=0;i<10;i++){
            printf("%snote estimée : %f\n",TitreFilm(F[i].IdFilm),F[i].estimation);
        }
        
        #ifdef CHRONO
       	fin=time(NULL);
        #endif
        
        free(V);
        free(F);
    }
    else if(choix==3){
    
        #ifdef CHRONO
        debut=time(NULL);
        #endif
        
        Reco3(argv,Films,Utilisateurs,NB_VOISINS);
        
        #ifdef CHRONO
        fin=time(NULL);
        #endif
    }
    else{
        #ifdef CHRONO
        debut=time(NULL);
        #endif
        
        FilmReco *F=Reco4(Films,Utilisateurs);
        TriNote(F,10);
        int i;
        printf("10 films recommandés :\n");
        for(i=0;i<10;i++){
            printf("%s note estimée : %f\n",TitreFilm(F[i].IdFilm),F[i].estimation);
        }
        
        #ifdef CHRONO
        fin=time(NULL);
        #endif
        
        free(F);
    }
    #ifdef CHRONO
    printf("Temps reco : %f\n",fin-debut);
    #endif
}
