#include "projet.h"

int MinSimilarite(Voisin *V, int k){
    int i,min=0;
    for(i=1;i<k;i++){
        if(V[i].similarite<V[min].similarite){
            min=i;
        }
    }
    return min;
}

int MaxSimilarite(Voisin *V, int k){
    int i,max=0;
    for(i=1;i<k;i++){
        if(V[i].similarite>V[max].similarite){
            max=i;
        }
    }
    return max;
}

void TriSimilarite(Voisin *V,int k){
    if(k==1){
        return;
    }
    int max=MaxSimilarite(V,k);
    int tmp1=V[max].IdVoisin;
    double tmp2=V[max].similarite;
    V[max].IdVoisin=V[k-1].IdVoisin;
    V[max].similarite=V[k-1].similarite;
    V[k-1].IdVoisin=tmp1;
    V[k-1].similarite=tmp2;
    TriSimilarite(V,k-1);
}

int MinNote(FilmReco *F, int k){
    int i,min=0;
    for(i=1;i<k;i++){
        if(F[i].estimation<F[min].estimation){
            min=i;
        }
    }
    return min;
}

int MaxNote(FilmReco *F, int k){
    int i,max=0;
    for(i=1;i<k;i++){
        if(F[i].estimation>F[max].estimation){
            max=i;
        }
    }
    return max;
}

void TriNote(FilmReco *F,int k){
    if(k==1){
        return;
    }
    int min=MinNote(F,k);
    int tmp1=F[min].IdFilm;
    double tmp2=F[min].estimation;
    F[min].IdFilm=F[k-1].IdFilm;
    F[min].estimation=F[k-1].estimation;
    F[k-1].IdFilm=tmp1;
    F[k-1].estimation=tmp2;
    TriNote(F,k-1);
}
