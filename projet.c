#include "projet.h"

Maillon *CreerMaillon(int IdFilm, int IdUser, int note){
    Maillon *M=malloc(sizeof(Maillon));
    if(M!=NULL){
        M->IdFilm=IdFilm;
        M->IdUser=IdUser;
        M->note=note;
        M->NextFilm=NULL;
        M->NextUser=NULL;
    }
    return M;
}

void AjoutMaillon(Maillon **Films, Maillon **Utilisateurs, int IdFilm, int IdUser, int note){
    Maillon *M=CreerMaillon(IdFilm,IdUser,note);
    if(Utilisateurs[IdUser-1]==NULL){ //Si on n'avait pas encore enregistré de note pour cet utilisateur
        Utilisateurs[IdUser-1]=M;
        if(Films[IdFilm-1]!=NULL){ //Le maillon est inséré en début de chaîne. Dans les fichiers de Netflix, les identifiants de films apparaîssent dans l'ordre, donc si on veut parcourir les films notés par un utilisateur en particulier, les identifiants de films seront affichées dans l'ordre décroissant.
            Maillon *tmp=Films[IdFilm-1];
            Films[IdFilm-1]=M;
            Films[IdFilm-1]->NextUser=tmp;
        }
        else{ //S'il n'y avait aucune note enregistrée pour ce film
            Films[IdFilm-1]=Utilisateurs[IdUser-1];
        }
    }
    else{ //Si cet utilisateur avait déjà des notes enregistrées
        Maillon *tmp=Utilisateurs[IdUser-1];
        Utilisateurs[IdUser-1]=M;
        Utilisateurs[IdUser-1]->NextFilm=tmp;
        if(Films[IdFilm-1]!=NULL){
            tmp=Films[IdFilm-1];
            Films[IdFilm-1]=M;
            Films[IdFilm-1]->NextUser=tmp;
        }
        else{
            Films[IdFilm-1]=M;
        }
    }
}

int RecupDonnees(Maillon **Films, Maillon **Utilisateurs, char *argv[]){
    int NbMaillons=0;

    #ifdef CHRONO
    double debut=time(NULL);
    #endif

    int i;
    for(i=1;i<5;i++){ //i parcourt les noms de fichiers entrés en ligne de commande dans le terminal
        FILE *source=fopen(argv[i],"r");
        char ligne[T_MAX];
        int NumFilm=0;
        while(fgets(ligne,T_MAX,source)){
            int p=0; //p est l'indice qui permet de parcourir la ligne
            while(ligne[p]!=':'&&ligne[p]!=','){
                p++;
            }
            if(ligne[p]==':'){ //si p a rencontré un ':', on est dans une ligne qui contient un identifiant de film.
                NumFilm=atoi(ligne); //On récupère l'identifiant du film
                if(NumFilm%1000==0) printf("Données traitées : %d%c\n",NbMaillons/1004805,'%');
            }
            else{ //On est dans le cas où p a rencontré une virgule, donc la ligne contient la note donnée par un utilisateur. C'est le seul autre cas possible.
                int x=atoi(ligne); //x correspond donc à l'identifiant de l'utilisateur
                int n=ligne[p+1]-'0'; //p+1 est placé pile entre les deux virgules, sur la note qui tient en un seul caractère, donc on récupère sa valeur entière en utilisant le code ASCII de 0 (si la note vaut 4 par exemple, '4'-'0'=52-48=4)
                AjoutMaillon(Films, Utilisateurs, NumFilm, x, n);
                NbMaillons++;
            }
        }
        fclose(source);
    }
    printf("100%c des données ont été récupérées.\n",'%');
    #ifdef CHRONO
    double fin=time(NULL);
    printf("Temps RecupDonnees : %f\n",fin-debut);
    #endif

    return NbMaillons;
}

Voisin *Voisins(Maillon **Films, Maillon **Utilisateurs, int U, int k){

    #ifdef CHRONO
    double debut=time(NULL);
    #endif
    
    Voisin *V=malloc(k*sizeof(Voisin)); //On alloue la place nécessaire pour un tableau de k voisins. V est le pointeur qu'on renvoie à la fin de la fonction
    int i,cmp=0;
    double SPNotes, SNotesU2, SNotesi2;
    Maillon *pU, *pi; //pU va parcourir les notes données par U, et pi les notes données par un autre utilisateur dont on calcule la similarité avec U
    for(i=0;i<NB_USERS;i++){
        if(i!=U-1){
            SPNotes=0;
            SNotesU2=0;
            SNotesi2=0;
            pU=Utilisateurs[U-1];
            pi=Utilisateurs[i];
            while(pU!=NULL&&pi!=NULL){
                if(pU->IdFilm==pi->IdFilm){ //Si U et i on noté le même film
                    SPNotes+=(pU->note-2.9)*(pi->note-2.9);
                    SNotesU2+=pow((pU->note-2.9),2);
                    SNotesi2+=pow((pi->note-2.9),2);
                    pU=pU->NextFilm;
                    pi=pi->NextFilm;
                }
                else if(pU->IdFilm<pi->IdFilm){
                    pi=pi->NextFilm;
                }
                else{
                    pU=pU->NextFilm;
                }
            }
            if(SPNotes==0&&SNotesU2==0&&SNotesi2==0){
                continue;
            }
            double s=SPNotes/sqrt(SNotesU2*SNotesi2); //On finit le calcul de la similarité
            if(cmp<k){ //cmp compte le nombre de voisins déjà insérés dans le tableau. Si cmp<k, il reste encore de la place dans le tableau
                V[cmp].IdUser=U;
                V[cmp].IdVoisin=i+1;
                V[cmp].similarite=s;
                cmp++;
            }
            else{ //Sinon, il faut remplacer les voisins qui ont une trop petite similarité
                int min=MinSimilarite(V,k);
                if(V[min].similarite<s){
                    V[min].IdVoisin=i+1;
                    V[min].similarite=s;
                }
            }
        }
    }

    #ifdef CHRONO
    double fin=time(NULL);
    printf("Temps Voisins : %f\n",fin-debut);
    #endif

    return V;
}

void PGMVoisin(Maillon **Utilisateurs, Voisin *V, int k){

    #ifdef CHRONO
    double debut=time(NULL);
    #endif

    FILE *sortie=fopen("voisins.pgm","w");
    int i,j,T[NB_FILMS]={0}; //On veut compter le nombre de films pour connaître la largeur à indiquer au début du fichier PGM. On crée donc un tableau de taille NB_FILMS initialisé à 0, on parcourt les films notés par les voisins, et on incrémente le contenu de la case correspondante du tableau pour chaque identifiant de film.
    Maillon *p;
    for(i=0;i<k;i++){
        p=Utilisateurs[V[i].IdVoisin-1];
        while(p!=NULL){
            T[p->IdFilm-1]++;
            p=p->NextFilm;
        }
    }
    int nf=0;
    for(i=0;i<NB_FILMS;i++){
        if(T[i]!=0){
            nf++;
        }
    }
    fprintf(sortie,"P2\n");
    fprintf(sortie,"%d %d\n",nf,k);
    fprintf(sortie,"%d\n",5);
    for(i=0;i<k;i++){
        for(j=0;j<NB_FILMS;j++){
            if(T[j]!=0){
                p=Utilisateurs[V[i].IdVoisin-1];
                while(p!=NULL){
                    if(p->IdFilm==j+1){
                        fprintf(sortie,"%d ",5-p->note); //On met un pixel de niveau 5-p->note au lieu de p->note car on veut avoir un pixel noir pour une note de 5 et un pixel blanc lorsqu'il n'y a pas de note
                        break;
                    }
                    p=p->NextFilm;
                }
                if(p==NULL){
                    fprintf(sortie,"%d ",5);
                }
            }
        }
        fprintf(sortie,"\n");
    }
    fclose(sortie);

    #ifdef CHRONO
    double fin=time(NULL);
    printf("Temps PGMVoisins : %f\n",fin-debut);
    #endif
}

void LibereMemoire(Maillon **Films, Maillon **Utilisateurs){
	int i;
	Maillon *p, *sav;
	for(i=0;i<NB_FILMS;i++){
		p=(Films[i]);
		while(p->NextUser!=NULL){
			sav=p->NextUser->NextUser;
			free(p->NextUser);
			p->NextUser=sav;
		}
		free(p);
	}
	free(Films);
	free(Utilisateurs);
}
