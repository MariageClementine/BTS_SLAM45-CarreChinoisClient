/*!
  * \file mainwindow.h
  * \brief Classe principale
  * \author Mariage Clementine
  * \version 1.2
  * \date 25/11/13-06/12/13
  * \bug Le joueur change une fois de trop a la fin d'une partie,
  * problème de comptage des scores: quand le serveur gagne, l'application met 1 point au serveur et 1 au client.
  * \warning Sortir les pions de la fenetre peut faire crasher l'application.
*/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QCursor>
#include <QMainWindow>
#include "qemplacement.h"
#include "qjoueur.h"
#include "qemplacement.h"
#include <QTcpSocket>

namespace Ui {
class MainWindow;
}
class QEmplacement;
/*!
  *\class MainWindow
  *\brief Cette classe prend en charge l'entièreté du jeu
  *\details Hérite de QMainWindow\n
  *Cette classe permet : \n
  *la création des joueurs (QJoueur * joueurHumain, QJoueur * joueurOrdi, QJoueur * joueurActif),\n
  *leur gestion (void changeJoueurActif()),\n
  *le déroulement du jeu (bool partieFinie, bool gagne(QJoueur* joueur), compteurOrdi, compteurHumain...),\n
  *la connexion réseau (void traiterMessRecu(QString messRecu), void envoyerAuServeur(int pionX,int pionY)),\n
  *liste non exhaustive....
  */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /*!
      *\fn MainWindow(QWidget *parent=0)
      *\brief Constructeur de la mainwindow:
      * met en place les joueurs, les compteurs, les pions;
      * initialise le jeu.

      *\param QWidget parent = utilisé pour s'initialiser.
      *\return Ne retourne rien.
   */
    explicit MainWindow(QWidget *parent = 0);
    /*! ~MainWindow();
      *\brief Destrucuteur.
      * Met fin à l'instance,
      * supprime l'ui.
      *\return Ne retourne rien.
    */
    ~MainWindow();
    /*!
      *\property QJoueur * joueurHumain
      *\brief Instance de la classe QJoueur. Représente le joueur humain.
      */
    QJoueur * joueurHumain;
    /*!
      *\property QJoueur * joueurOrdi
      *\brief Instance de la classe QJoueur. Représente le joueur ordinateur.
      */
    QJoueur * joueurOrdi;
    /*!
      *\property QJoueur * joueurActif
      *\brief Instance de la classe QJoueur. Représente le joueur en train de jouer.
      */
    QJoueur * joueurActif;
    /*!
      *\property QPixmap pixmapVide
      *\brief Contient l'image de l'emplacement vide
      */
    QPixmap pixmapVide;
    /*!
      *\property QEmplacement * emplacementDeplace
      *\brief Lors d'un deplacement, l'emplacement de début est mémorisé dans cette variable
      */
    QEmplacement * emplacementDeplace;
    /*!
      *\property bool partieFinie
      *\brief Renvoie true si la partie est finie, false sinon.
      */
    bool partieFinie;
    /*!
      *\property int compteurOrdi
      *\brief Garde en mémoire le nombre de parties gagnées par l'ordinateur.
      */
    int compteurOrdi;
    /*!
      *\property int compteurHumain
      *\brief Garde en mémoire le nombre de parties gagnées par le joueur Humain.
      */
    int compteurHumain;

    /*!
      *\fn void setEmplacementDeplace(QEmplacement *e)
      *\brief Aloue au pointeur QEmplacement * emplacementDeplace l'emplacement actuel.
      *\param QEmplacement * e =  pointeur qui sera mémorisé dans emplacementDeplace.
      *\return Ne retourne rien.
      */
    void setEmplacementDeplace(QEmplacement *e) {emplacementDeplace=e;}
    /*!
      *\fn void changeJoueurActif()
      *\brief Change de joueur actif.
      *\return Ne retourne rien.
      */
    void changeJoueurActif();
    /*!
      *\fn void resetCurseur()
      *\brief Remet le curseur de la souris comme il est à l'origine.
      *\return Ne retourne rien.
      */
    void resetCurseur();
    /*!
      *\fn void majCompteur()
      *\brief Met à jour les labels "compteurs" de parties gagnées de l'ui.
      *\return Ne retourne rien.
      */
    void majCompteur();
    /*!
      \fn void initJeu()
      \brief Initialise le jeu
      \return Ne retourne rien.
      */
    void initJeu();

    /*!
      *\fn void traiterMessRecu(QString messRecu)
      *\brief Traite le message recu du serveur.
      *\param QString messrecu = le message reçu du serveur.
      *\return Ne retourne rien.
      */
    void traiterMessRecu(QString messRecu);
    /*!
      *\fn void envoyerAuServeur(int posVieuxX,int posVieuxY, int posNouvX,int posNouvY)
      *\brief Envoie au serveur le nouvel emplacement (du tableau de 3 par 3) du pion lors d'un déplacement.
      *\param int posVieuxX = position horizontale de l'ancien emplacement,
      *\param int posVieuxY = position verticale de l'ancien emplacement,
      *\param int posNouvX = position horizontale du nouvel emplacement,
      *\param int posNouvY = position verticale du nouvel emplacement
      *\return Ne retourne rien.
      */    
    void envoyerAuServeur(int posVieuxX,int posVieuxY, int posNouvX,int posNouvY);
    /*!
      *\fn  void envoyerAuServeur(int pionX, int pionY)
      *\brief Envoie au serveur l'emplacement (du tableau de 3 par 3) cliqué lors d'une pose de pion
      *\param int pionX = position horizontale de l'emplacement cliqué.
      *\param int pionY = position verticale de l'emplacement cliqué.
      *\return Ne retourne rien.
    */
    void envoyerAuServeur(int pionX, int pionY);

    //les fonctions qui vont determiner une partie gagnée
    /*!
      *\fn  bool ligne(int noL)
      *\brief Définit si une ligne est pleine ou non.
      *\param int noL = numéro de ligne testée
      *\return True si la ligne est pleine, false sinon.
      */
    bool ligne(int noL);
    /*!
      *\fn bool colonne(int noC)
      *\brief Définit si une colonne est pleine ou non.
      *\param int noC = numéro de colonne testée
      *\return True si la colonne est pleine, false sinon.
      */
    bool colonne(int noC);
    /*!
      *\fn bool diagonale(int noD)
      *\brief Définit si une diagonale est pleine ou non.
      *\param int noD = numéro de diagonale testée
      *\return True si la diagonale est pleine, false sinon.
      */
    bool diagonale(int noD);
    /*!
      *\fn bool gagne(QJoueur * joueur)
      *\brief Vérifie si un joueur a gagné
      *\param QJoueur * joueur = paramètre devenu inutile, la fonction récupérant le joueur directement de la MainWindow
      *\return True si un joueur a gagné, false sinon. Incrémente le compteur du joueur actif si il a gagné et le met à jour via MainWindow::majCompteur() .
     */
    bool gagne(QJoueur * joueur);

public slots:
    /*!
      *\fn void setCurseur(QCursor & curseur)
      *\brief Attribue au curseur une image selon les possibilités de déplacement du pion
      *\param QCursor & curseur = le type de curseur a attribuer
      *\return Ne retourne rien.
      */
    void setCurseur(QCursor & curseur);

private slots:
    /*!
      *\fn void on_pushButtonNouvellePartie_clicked()
      *\brief Initialise le jeu quand on clique sur le pushButtonNouvellePartie
      *\return Ne retourne rien
      */
    void on_pushButtonNouvellePartie_clicked();
    /*!
      *\fn void recupMessServ()
      *\brief Récupère et enregistre les informations envoyées par le serveur au moindre signal
      */
    void recupMessServ();

private:

    Ui::MainWindow *ui;
        /*!
          *\property QCursor normal
          *\brief Curseur de la souris
          */
    QCursor normal;
    /*!
      *\property QEmplacement * tab[3][3]
      *\brief contient les QEmplacement du plateau
      */
    QEmplacement * tab[3][3];

    //connexion au serveur
    /*!
      *\property QTcpSocket * laSocket
      *\brief Socket permettant la connexion au serveur
      */
    QTcpSocket * laSocket;


};

#endif // MAINWINDOW_H


