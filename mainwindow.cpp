#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDialog>
#include <QInputDialog>
#include <QStringList>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    compteurHumain=0;
    compteurOrdi=0;
    majCompteur();
    normal=cursor();
    partieFinie=false;
    emplacementDeplace=NULL;

    //mise en place des pions
    joueurHumain = new QJoueur("blanc",this);
    ui->verticalLayoutHumain->addWidget(joueurHumain);
    joueurOrdi = new QJoueur("noir",this);
    ui->verticalLayoutOrdi->addWidget(joueurOrdi);

    //mise en place du plateau
    pixmapVide=QPixmap(":/images/vide");
    for(int noL=0;noL<3;noL++)
    {
        for(int noC=0;noC<3;noC++)
        {
            tab[noL][noC]=new QEmplacement(this,this);
            tab[noL][noC]->ligne=noL;
            tab[noL][noC]->col=noC;
            ui->gridLayoutPlateau->addWidget(tab[noL][noC],noL,noC);

        }
    }
    joueurActif=joueurHumain;
    ui->labelJoueurCourant->setText(tr("Human"));

    bool ok;
    QString adresseDuServeur = QInputDialog::getText(this,tr("client network"),tr("server's address"),QLineEdit::Normal,"172.16.56.102",&ok);
    if(ok)
    {
        laSocket = new QTcpSocket(this);
        laSocket->connectToHost(adresseDuServeur,6666);
        connect(laSocket,SIGNAL(readyRead()),this,SLOT(recupMessServ()));
    }

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changeJoueurActif()
{
    if(joueurActif==joueurHumain)
    {
        joueurActif=joueurOrdi;
        ui->labelJoueurCourant->setText(tr("Computer"));
    }
    else
    {
        joueurActif=joueurHumain;
        ui->labelJoueurCourant->setText(tr("Human"));
    }
}

void MainWindow::setCurseur(QCursor &curseur)
{
    setCursor(curseur);
}

void MainWindow::resetCurseur()
{
    setCursor(normal);
}

bool MainWindow::ligne(int noL)
{
    return(!tab[noL][0]->estVide() && tab[noL][0]->leJoueur==tab[noL][1]->leJoueur && tab[noL][1]->leJoueur==tab[noL][2]->leJoueur);
}

bool MainWindow::colonne(int noC)
{
    return(!tab[0][noC]->estVide() && tab[0][noC]->leJoueur==tab[1][noC]->leJoueur && tab[1][noC]->leJoueur==tab[2][noC]->leJoueur);
}

bool MainWindow::diagonale(int noD)
{
    if(noD==0) return(!tab[0][0]->estVide() && tab[0][0]->leJoueur==tab[1][1]->leJoueur && tab[1][1]->leJoueur==tab[2][2]->leJoueur);
    else return (!tab[0][2]->estVide() && tab[0][2]->leJoueur==tab[1][1]->leJoueur && tab[1][1]->leJoueur==tab[2][0]->leJoueur);
}

bool MainWindow::gagne(QJoueur * joueur)
{
    bool aGagne=ligne(0) || ligne(1) || ligne(2) || colonne(0) || colonne(1) || colonne(2) || diagonale(0) || diagonale(1);
    if (aGagne)
    {
        partieFinie=true;
        ui->pushButtonNouvellePartie->setEnabled(true);
        ui->labelJoueurCourant->clear();
        if(joueurActif==joueurOrdi)
        {
            compteurOrdi++;
            majCompteur();
        }
        else
        {
            if(joueurActif==joueurHumain)
            {
                compteurHumain++;
                majCompteur();
            }
            else
            {ui->labelInfoCo->setText("erreur!");}
        }
    }
    return( aGagne);
}

void MainWindow::majCompteur()
{
    ui->labelCompteurHumain->setText(QString::number(compteurHumain));
    ui->labelCompteurOrdi->setText(QString::number(compteurOrdi));
}

void MainWindow::initJeu()
{
    joueurHumain->setNbJetons(3);
    joueurOrdi->setNbJetons(3);
    for(int noL=0;noL<3;noL++)
    {
        for(int noC=0;noC<3;noC++)
        {
            tab[noL][noC]->vider();
        }
    }
    emplacementDeplace=NULL;
    partieFinie=false;
    ui->labelInfoCo->clear();
}

void MainWindow::on_pushButtonNouvellePartie_clicked()
{
    initJeu();
    QString messAEnvoyer = "reset";
    laSocket->write(messAEnvoyer.toLatin1());
    ui->pushButtonNouvellePartie->setEnabled(false);
}

void MainWindow::recupMessServ()
{
    QString messRecu=laSocket->readAll();
    traiterMessRecu(messRecu);
}

void MainWindow::traiterMessRecu(QString messRecu)
{
    QStringList messTraite=messRecu.split(" ");
    QString coulJoueurActif=messTraite[0];
    int ligneRecuperee=messTraite[1].toInt();
    int colonneRecuperee=messTraite[2].toInt();

    //si c'est un clic
    if(messTraite.size()==3)
    {
        //on ajoute le pion
        tab[ligneRecuperee][colonneRecuperee]->posePion();
    }
    //si c'est un deplacement
    else
    {
        int nouvLigne=messTraite[3].toInt();
        int nouvCol= messTraite[4].toInt();
        //on deplace le pion
        if(coulJoueurActif=="blanc")
        {
            tab[nouvLigne][nouvCol]->setJoueur(joueurHumain);
        }
        else
        {
            tab[nouvLigne][nouvCol]->setJoueur(joueurOrdi);
        }
        tab[ligneRecuperee][colonneRecuperee]->vider();

        //si le message recu est bien une donnée de jeu
        if((!gagne(joueurActif))&& (messTraite.size()==3 || messTraite.size()==5)) changeJoueurActif();
     }
    //il faut bien vérifier si un joueur a gagné
   // if(gagne(joueurActif)) ui->labelInfoCo->setText("Partie terminée!");
}

void MainWindow::envoyerAuServeur(int posVieuxX, int posVieuxY, int posNouvX, int posNouvY)
{
    QString vieuxX= QString::number(posVieuxX);
    QString vieuxY= QString::number(posVieuxY);
    QString nouvX= QString::number(posNouvX);
    QString nouvY = QString::number(posNouvY);
    QString message =joueurActif->getCoul()+" "+vieuxX+" "+vieuxY+" "+nouvX+" "+nouvY;
    laSocket->write(message.toLatin1());

}

void MainWindow::envoyerAuServeur(int pionX, int pionY)
{
    QString ligne= QString::number(pionX);
    QString colonne = QString::number(pionY);
    QString message =joueurActif->getCoul()+" "+ligne+" "+colonne;
    laSocket->write(message.toLatin1());
}

