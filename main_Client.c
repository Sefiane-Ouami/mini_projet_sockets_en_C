#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> //biblio pour les fct read write close

#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>
#include <string.h>



/*********************************************** menuAdmin() Admin ****************************************/
int menuAdmin(){
int choix;
printf("\n*************************************MENU******************************************\n");
printf("         1-Ajouter un contact \n");
printf("         2-Rechercher un contact \n");
printf("         3-Supprimer un contact \n");
printf("         4-Modifier un contact \n");
printf("         5-Afficher tous les contacts \n");
printf("         6-Quitter \n");
printf("\n\n   Entrez votre choix : ");
scanf("%d",&choix);
return choix;
}

/*********************************************** menu() visite ****************************************/
int menuVisit(){
int choix;
printf("\n*************************************MENU******************************************\n");
printf("         1-Afficher tous les contacts \n");
printf("         2-Rechercher un contact \n");
printf("         3-Quitter \n");
printf("\n\n   Enrez votre choix : ");
scanf("%d",&choix);
return choix;
}

/******************************** les structures *********************************************/
typedef struct {
    char rue[50];
    char ville[30];
    char pays[30];
}Adresse;
typedef struct Contact{
    char    nom[50];
    char prenom[50];
    long        GSM;
    char  email[50];
    Adresse  adresse;
}Contact;



/******************************* la fonction Saisir *********************************/
Contact Saisir(){
   Contact c;
   printf("Donner le nom du contact : ");
   scanf("%s",c.nom);
   printf("Entrez le prenom du contact: ");
   scanf("%s",c.prenom);
   printf("Entrez le GSM : ");
   scanf("%ld",&c.GSM);
   printf("Entrez l'email' : ");
   scanf("%s",c.email);

   printf("  \n Saisie de l'adresse:\n\n");
   printf("veuillez entrer le nom de la rue:  ");
   scanf("%s",c.adresse.rue);
   printf("veuillez entrer la ville du contact:  ");
   scanf("%s",c.adresse.ville);
   printf("veuillez entrer le pays du contact:  ");
   scanf("%s",c.adresse.pays);
   printf("\n\nContact saisi %s#%s#%s#%ld#%s#%s#%s \n",c.nom,c.prenom,c.email,c.GSM,c.adresse.rue,c.adresse.ville,c.adresse.pays);
return c;
}

/********************************** Afficher ********************************************/
void Afficher(char *nom_fichier) {
    Contact c;
    int n;
    FILE *f=fopen(nom_fichier,"r");
    if(f==NULL) printf("Impossible d'ouvrire le fichier !!!! \n");
    else{
    while(!feof(f)) {
        fscanf(f,"%s%s%ld%s%s%s%s",c.nom,c.prenom,&c.GSM,c.email,c.adresse.rue,c.adresse.ville,c.adresse.pays);
        printf("\nContact %d :\n",n+1);
        printf("\n\tNom : %s",c.nom);
        printf("\n\tPrenom : %s",c.prenom);
        printf("\n\tGSM : %ld",c.GSM);
        printf("\n\tEmail : %s",c.email);
        printf("\n\tNom de rue : %s",c.adresse.rue);
        printf("\n\tNom de ville : %s",c.adresse.ville);
        printf("\n\tNom de pays : %s",c.adresse.pays);
        n++;
    }
    fclose(f);
    }
}




////////////////////////////////// main function ////////////////////////////////////////
int main()
{
    // création socket
    int network_socket;
    network_socket=socket(AF_INET,SOCK_STREAM,0);
    // definir une adresse pour socket
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(9002);
    server_address.sin_addr.s_addr = INADDR_ANY;
    // conexion au socket
    int connection_status = connect(network_socket,(struct sockaddr *) &server_address, sizeof(server_address));
    // tester la connexion
    if (connection_status == -1){
        printf("Erreur de connexion \n\n");
    }

    // reception des données
    char server_response[256]; // pour stocker les données
    recv(network_socket,&server_response,sizeof(server_response),0);
    // printout les données
    printf("le serveur a envoyer : %s \n",server_response);


    ////////////////////////// va et vian /////////////////////////////////////////////
    char user[100];
    char mdp[30];
    int n=0;
    int res=0;
    int valread;
         printf("RQ : vous avez le droit d'enter comme -Admin ou -Invite seulement \n");
         printf("votre login : ");
         scanf("%s",user);
         send(network_socket ,user , strlen(user), 0 );
    do{
        printf("votre mot de passe (vouz avez %d essai!) : ",3-n);
        scanf("%s",mdp);
        send(network_socket , mdp , strlen(mdp), 0 );
        recv( network_socket , &res, sizeof(res),0);
        printf("message envoye \n");
        fflush(stdin);
        n++;
    }while(res==0 && n<3 );
    int choix;
    int rep;
    Contact c;
    long gsm;
     if (res==1) {
               printf("--------------Vous etes indministrateur ----------------------\n\n");
               do {
                      choix=menuAdmin();
                      printf("choix est %d \n",choix);
                      send(network_socket , &choix ,sizeof(choix), 0 );
                      switch (choix){
                        case 1 :  c=Saisir();
                                 send(network_socket , &c , sizeof(c) , 0 );
                                 break;
                        case 2 : { printf("entrez le gsm du contact recherer: ");
                                   scanf("%ld",&gsm);
                                   send(network_socket , &gsm, sizeof(gsm) , 0 );
                                   recv(network_socket , &rep , sizeof(rep) , 0 );
                                    if(rep){
                                      printf("le contact existe \n ");
                                    }
                                    else printf("le contact n'existe pas \n ");
                                    break;
                        }
                        case 3 : { printf("entrez le gsm du contact a supprimer: ");
                                   scanf("%ld",&gsm);
                                   send(network_socket , &gsm, sizeof(gsm) , 0 );
                                   recv(network_socket , &rep , sizeof(rep) , 0 );
                                    if(rep){
                                      printf("suppression avec succes \n ");
                                    }
                                    else printf("le contact n'existe pas \n ");
                                    break;
                        }
                        case 4 : { printf("entrez le gsm  de contact a modifier: ");
                                   scanf("%ld",&gsm);
                                   send(network_socket , &gsm, sizeof(gsm) , 0 );
                                   printf(" Saise les nouvelles données du contatcs  \n");
                                   c = Saisir();
                                   send(network_socket , &c, sizeof(c) , 0 );
                                   getchar();
                                   recv(network_socket , &rep , sizeof(rep) , 0 );
                                    if(rep){
                                      printf("modificatin avec avec succes \n ");
                                    }
                                    else printf("le contact n'existe pas \n ");
                                    break;
                        }
                        case 5 : Afficher("contacts.txt"); break;
                        default :
                                 printf("votre choix %d  est invalide\n\n ",choix);
                                 break;
                      }
               } while(choix !=6);
     }
     else if (res==2){
               printf("--------------Vous etes ivite ----------------------\n");
              do {
                      choix=menuVisit();
                      send(network_socket , &choix ,sizeof(choix), 0 );
                      switch (choix){
                        case 1 : Afficher("contacts.txt"); break;
                        case 2: { printf("entrez le gsm : ");
                                   scanf("%ld",&gsm);
                                   send(network_socket , &gsm, sizeof(gsm) , 0 );
                                   recv(network_socket , &rep , sizeof(rep) , 0 );
                                    if(rep){
                                      printf("le contact existe \n ");
                                    }
                                    else printf("le contact n'existe pas \n ");
                                    break;
                        }
                        default :
                                 printf("votre choix est invalide \n\n ");
                                 break;
                      }
               } while(choix !=6);
     }
     else{
        printf(" mode non spécifier \n ");
        exit(1);
     }
    //////////////////////////////////////////////////////////////////////////////////////



    //fermer socket
    close(network_socket);
    return 0;
}


