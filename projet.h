#ifndef PROJET_H
#define PROJET _H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define NB_FILMS 17770
#define NB_USERS 2649429
#define T_MAX 100
#define NB_VOISINS 30000

#define CHRONO

typedef struct Maillon{
	struct Maillon *NextFilm;
	struct Maillon *NextUser;
	int IdUser;
	short IdFilm;
	char note;
} Maillon;

typedef struct{
	int IdUser;
	int IdVoisin;
	float similarite;
}Voisin; //Structure Voisin qui contient le numéro IdVoisin d'un utilisateur voisin de l'utilisateur IdUser et leur similarité entre -1 et 1

typedef struct{
	int IdUser;
	int IdFilm;
	float estimation;
}FilmReco; //Structure qui contient un identifiant d'utilisateur, un identifiant de film que cet utilisateur n'a pas vu et sa note estimée pour ce film

Maillon *CreerMaillon(int IdFilm, int IdUser, int note);

void AjoutMaillon(Maillon **Films, Maillon **Utilisateurs, int IdFilm, int IdUser, int note);

int RecupDonnees(Maillon **Films, Maillon **Utilisateurs, char *argv[]);
//Fonction qui répond à la question 1 : elle récupère les données des fichiers Netflix et les range dans les structures présentées dans le sujet. Elle renvoie le nombre de maillons créés.

Voisin *Voisins(Maillon **Films, Maillon **Utilisateurs, int U, int k);
//Fonction qui répond à la question 2 : elle renvoie un pointeur sur un tableau de k voisins de l'utilisisateur U. Les voisins ne sont pas rangés dans un ordre particulier.

void PGMVoisin(Maillon **Utilisateurs, Voisin *V, int k);
//Fonction qui répond à la question 3 : elle génère un fichier PGM à partir d'un tableau de voisins

void LibereMemoire(Maillon **Films, Maillon **Utilisateurs);

int MinSimilarite(Voisin *V, int k);
//Renvoie l'indice du voisin avec la plus petite similarité dans V

int MaxSimilarite(Voisin *V, int k);
//Renvoie l'indice du voisin avec la plus grande similarité dans V

void TriSimilarite(Voisin *V,int k);
//Fonction qui range un tableau de voisins par similarité croissante

int MinNote(FilmReco *F, int k);
//Renvoie l'indice du film avec la plus faible estimation dans F

int MaxNote(FilmReco *F, int k);
//Renvoie l'indice du film avec la plus grande estimation dans F

void TriNote(FilmReco *F,int k);
//Fonction qui range un tableau de films recommandés à un utilisateur par ordre DECROISSANT des notes

char *TitreFilm(int IdFilm);
//Récupère le titre d'un film à partir de son identifiant

int NouvelUtilisateur(Maillon **Utilisateurs);
//Recherche un identifiant d'utilisateur qui n'a pas donné de notes

double Reco1(Maillon **Utilisateurs, int IdFilm, Voisin *V, int k);

FilmReco *Reco2(Maillon **Utilisateurs, int U, Voisin *V, int k);

void Reco3(char* argv[],Maillon **Films, Maillon **Utilisateurs, int k);

FilmReco *Reco4(Maillon **Films, Maillon **Utilisateurs);

void Reco(char *argv[], Maillon **Films, Maillon **Utilisateurs);

double ErreurSimilarite(Maillon **Films, Maillon **Utilisateurs,int NbTotaldeMaillons);

Voisin *VoisinsCos(Maillon **Films, Maillon **Utilisateurs, int U, int k);

double ErreurCos(Maillon **Films, Maillon **Utilisateurs,int NbTotaldeMaillons);

#endif
