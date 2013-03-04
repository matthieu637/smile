

#include "test.h"

void OnDestroy(GtkWidget *pWidget, gpointer pData)
{
/*arret de la boucle événementielle*/
gtk_main_quit();
}

void test_gtk()
{
	
/*Déclaration des widgets */
GtkWidget *pWindow; /*la fenêtre*/
GtkWidget *pButton1, *pButton2; /*2 boutons*/
GtkWidget *pLabel1, *pLabel2; /*2 libellés*/
GtkWidget *pTable; /*boîte horizontalee*/
/*Initialisation de GTK+*/
gtk_init(NULL, NULL);
/*Creation de la fenêtre*/
pWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
/*Définition de la taille de la fenêtre*/
gtk_window_set_default_size(GTK_WINDOW(pWindow), 320,200);
/*titre de la fenêtre*/
gtk_window_set_title(GTK_WINDOW(pWindow), "Les arrangements");
/*Connexion du signal "destroy"*/
g_signal_connect(G_OBJECT(pWindow),"destroy", G_CALLBACK(OnDestroy), NULL);
/*Création de la table*/
pTable = gtk_table_new(3,2,FALSE);
/*ajout de la table à la fenêtre*/
gtk_container_add(GTK_CONTAINER(pWindow),pTable);
/*Création des boutons*/
pButton1 = gtk_button_new_with_label("Bouton 1");
g_signal_connect(G_OBJECT(pButton1),"clicked",G_CALLBACK(Choix),(gpointer)pWindow);

pButton2 = gtk_button_new_with_label("Bouton 2");
/*Création des libellés*/
pLabel1 = gtk_label_new(NULL);
gtk_label_set_markup(GTK_LABEL(pLabel1), "<span font_desc = \"Comic Sans MS 24\"foreground=\"#FFFFFF\" background=\"#FF0000\"> <b>Titre 1</b></span>");
pLabel2 = gtk_label_new("Titre 2");
/*ajout du bouton 2 et de la boîte verticale dans la boîte horizontale*/
gtk_table_attach(GTK_TABLE(pTable),pButton2,1,2,0,3,GTK_FILL, GTK_FILL,0,0);
/*Ajout du bouton 1 et des 2 libellés dans la table*/
gtk_misc_set_alignment(GTK_MISC(pLabel1), 0.9f, 0);
gtk_misc_set_alignment(GTK_MISC(pLabel2), 0.85f, 0);
gtk_table_attach(GTK_TABLE(pTable),pLabel1, 0,1,0,1, (GtkAttachOptions)(GTK_EXPAND|GTK_FILL), (GtkAttachOptions)GTK_EXPAND,0,
0);
gtk_table_attach(GTK_TABLE(pTable),pLabel2, 0,1,1,2, (GtkAttachOptions)(GTK_EXPAND|GTK_FILL), (GtkAttachOptions)GTK_EXPAND,0,
0);
gtk_table_attach(GTK_TABLE(pTable),pButton1, 0,1,2,3,(GtkAttachOptions)(GTK_FILL), (GtkAttachOptions)GTK_FILL,0,0);
/*Affichage de la fenêtre et son contenu*/
gtk_widget_show_all(pWindow);
/*Démattage de la boucle événementielle*/
//gtk_main();
	

}

void Choix(GtkWidget* pWidget,gpointer pData )
{
  printf("I'm here\n");
}