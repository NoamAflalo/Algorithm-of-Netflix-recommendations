#include "projet.h"

double ErreurSimilarite(Maillon **Films, Maillon **Utilisateurs,int NbTotaldeMaillons){

    #ifdef CHRONO
    double debut=time(NULL);
    #endif
    
    int i=NbTotaldeMaillons/50000,j=0;
    printf("i : %d\n",i);
    double S=0;
    float estimation;
    while(i>0){
        Voisin *V=Voisins(Films,Utilisateurs,j+1,NB_VOISINS);
        Maillon *p=Utilisateurs[j];
        while(p!=NULL){
            estimation=Reco1(Utilisateurs,p->IdFilm,V,NB_VOISINS);
            S+=pow(p->note-estimation,2);
            if(i%100==0) printf("U%d F%d note : %d estimation : %f\n",p->IdUser,p->IdFilm,p->note,estimation);
            i--;
            if(i==0){
                break;
            }
            p=p->NextFilm;
        }
        free(V);
        j++;
    }
    
    #ifdef CHRONO
    double fin=time(NULL);
    printf("Temps Erreur : %f\n",fin-debut);
    #endif
    
    return sqrt(S/NbTotaldeMaillons);
}

Voisin *VoisinsCos(Maillon **Films, Maillon **Utilisateurs, int U, int k){
    #ifdef CHRONO
    double debut=time(NULL);
    #endif
    
    Voisin *V=malloc(k*sizeof(Voisin));
    int i,cmp=0;
    double SPNotes; //Somme des produits des notes données par les utilisateurs U et i à un même film
    double SNotesU2=0; //Somme des notes au carré données par U
    double SNotesi2; //Somme des notes au carré données par i
    Maillon *pU=Utilisateurs[U-1], *pi;
	while(pU!=NULL){
		SNotesU2+=pow((pU->note-2.9),2);
        pU=pU->NextFilm;
	}
    for(i=0;i<NB_USERS;i++){ //On parcourt l'ensembles des utilisateurs différents de U
        if(i!=U-1){
            SPNotes=0;
            SNotesi2=0;
            pU=Utilisateurs[U-1]; //pU parcourt l'ensemble des films notés par U
            pi=Utilisateurs[i]; //pi parcourt l'ensembles des films notés par i
            while(pU!=NULL&&pi!=NULL){
                if(pU->IdFilm==pi->IdFilm){ //Si U et i on noté le même film
                    SPNotes+=(pU->note-2.9)*(pi->note-2.9);
                    SNotesi2+=pow((pi->note-2.9),2);
                    pU=pU->NextFilm;
                    pi=pi->NextFilm;
                }
                else if(pU->IdFilm<pi->IdFilm){
					SNotesi2+=pow((pi->note-2.9),2);
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
                if(V[min].similarite<s){    //s est la similiratié avec l'éement courant.
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

double ErreurCos(Maillon **Films, Maillon **Utilisateurs,int NbTotaldeMaillons){
    #ifdef CHRONO
    double debut=time(NULL);
    #endif
    int i=NbTotaldeMaillons/50000,j=0;
    printf("i : %d\n",i);
    double S=0;
    float estimation;
    while(i>0){
        Voisin *V=VoisinsCos(Films,Utilisateurs,j+1,NB_VOISINS);
        Maillon *p=Utilisateurs[j];
        while(p!=NULL){
            estimation=Reco1(Utilisateurs,p->IdFilm,V,NB_VOISINS);
            S+=pow(p->note-estimation,2);
            if(i%100==0) printf("U%d F%d note : %d estimation : %f\n",p->IdUser,p->IdFilm,p->note,estimation);
            i--;
            if(i==0){
                break;
            }
            p=p->NextFilm;
        }
        free(V);
        j++;
    }
    #ifdef CHRONO
    double fin=time(NULL);
    printf("Temps Erreur : %f\n",fin-debut);
    #endif
    return sqrt(S/NbTotaldeMaillons);
}
