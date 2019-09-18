#include "projet.h"

int main(int argc, char *argv[]){
	printf("\n      Bienvenue au filtrage collaboratif réalisé par Emma et Noam\n\nLecture des fichiers en cours...\n");
    Maillon **Films=malloc(NB_FILMS*sizeof(Maillon));
    Maillon **Utilisateurs=malloc(NB_USERS*sizeof(Maillon));
    int i;
    for(i=0;i<NB_FILMS;i++){
        Films[i]=NULL;
    }
    for(i=0;i<NB_USERS;i++){
        Utilisateurs[i]=NULL;
    }
    int NbMaillons=RecupDonnees(Films,Utilisateurs,argv);
    int rep,verif;
    printf("Voulez-vous tester PGMVoisins ? (1 pour oui, 0 pour non)\n");
    verif=scanf("%d",&rep);
    printf("verif : %d\n",verif);
    while(verif!=1||(rep!=1&&rep!=0)){
        printf("Voulez-vous tester PGMVoisins ? (1 pour oui, 0 pour non)\n");
        int c;
        while((c=getchar())!='\n'&&c!=EOF);
        verif=scanf("%d",&rep);
    }
    if(rep==1){
        int IdUser;
        while(verif!=1||IdUser<1||IdUser>2649429||Utilisateurs[IdUser-1]==NULL){
            printf("Saisir un identifiant d'utilisateur : ");
            int c;
            while((c=getchar())!='\n'&&c!=EOF);
            verif=scanf("%d",&IdUser);
            if(Utilisateurs[IdUser-1]==NULL) printf("Cet utilisateur n'a noté aucun film.\n");
        }
        printf("Exécution de PGMVoisins...\n");
        Voisin *V=Voisins(Films, Utilisateurs, IdUser, NB_VOISINS/10);
        TriSimilarite(V,NB_VOISINS/10);
        PGMVoisin(Utilisateurs,V,NB_VOISINS/10);
    }
    Reco(argv,Films,Utilisateurs);
    verif=0; rep=-1;
    while(verif!=1||rep!=0){
        printf("Voulez-vous tester un autre type de recommandation ?(1 pour oui, 0 pour non)\n");
        int c;
        while((c=getchar())!='\n'&&c!=EOF);
        verif=scanf("%d",&rep);
        if(rep==1) Reco(argv,Films,Utilisateurs);
        if(rep==0) break;
    }
    printf("Voulez-vous calculer le taux d'erreur ? (1 pour oui, 0 pour non)\n");
    verif=scanf("%d",&rep);
    while(verif!=1||(rep!=1&&rep!=0)){
        printf("Voulez-vous calculer le taux d'erreur ? (1 pour oui, 0 pour non)\n");
        int c;
        while((c=getchar())!='\n'&&c!=EOF);
        verif=scanf("%d",&rep);
    }
    if(rep==1){
        printf("Pourcentage d'erreur : %f\n",ErreurSimilarite(Films, Utilisateurs,NbMaillons)*100);
    }
    printf("Libération de la mémoire...\n");
	LibereMemoire(Films,Utilisateurs);
    return 0;
}
