#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>
#include <string.h>

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

/*********************** Ajouter **************************************************/
void Ajouter(char *nom_fich,Contact c){
    FILE *f=fopen(nom_fich,"a");
    if(f!=NULL){
        fprintf(f,"%s %s %ld %s %s %s %s\n",c.nom,c.prenom,c.GSM,c.email,c.adresse.rue,c.adresse.ville,c.adresse.pays);
        fclose(f);
        printf("\n------- un Contact a été ajouté-------!!!\n\n");
    }
    else
        printf("impossible d'ouvrire le fichier !!!");

}
/************************* Rechercher *****************************************************/
int RechercherParGSM(char *nom_fichier, long gsm) {
    FILE *f=fopen(nom_fichier,"r");
    Contact c;
    int r=0;
    while(!feof(f)) {
        fscanf(f,"%s%s%ld%s%s%s%s",c.nom,c.prenom,&c.GSM,c.email,c.adresse.rue,c.adresse.ville,c.adresse.pays);
        if (gsm==c.GSM) {
            r=1;
        }
    }
    fclose(f);
    if (r == 0) {
        printf("\nContact introuvable !!!!");
    }
    return r;
}
/*********************************** Supprimer **************************************************************/
int Supprimer(char *nom_fichier,long gsm) {
    char nom_fichier1[]="ListesContactes.txt";
    Contact c;
    int r=0;
    FILE *F_in=fopen(nom_fichier,"r");
    FILE *F_out=fopen(nom_fichier1,"w");
    while(!feof(F_in)) {
    fscanf(F_in,"%s%s%ld%s%s%s%s",c.nom,c.prenom,&c.GSM,c.email,c.adresse.rue,c.adresse.ville,c.adresse.pays);
        if (gsm == c.GSM) {
            r=1;
        }
        else {
            fprintf(F_out,"%s %s %ld %s %s %s %s\n",c.nom,c.prenom,c.GSM,c.email,c.adresse.rue,c.adresse.ville,c.adresse.pays);
        }
    }
    fclose(F_out);
    fclose(F_in);
    remove(nom_fichier);
    rename(nom_fichier1,nom_fichier);
    return r;
}

/***************************************** Modifier *************************************************************/
//La fonction Modifier

int Modifier(char *nom_fichier,long gsm, Contact c1) {
    Contact c;
    int r=0;
    char nom_fichier1[]="ListesContactes.txt";
    FILE *F_in=fopen(nom_fichier,"r");
    FILE *F_out=fopen(nom_fichier1,"w");
    while(!feof(F_in)) {
    fscanf(F_in,"%s%s%ld%s%s%s%s",c.nom,c.prenom,&c.GSM,c.email,c.adresse.rue,c.adresse.ville,c.adresse.pays);
        if (gsm == c.GSM) {
            fprintf(F_out,"%s %s %ld %s %s %s %s\n",c1.nom,c1.prenom,c1.GSM,c1.email,c1.adresse.rue,c1.adresse.ville,c1.adresse.pays);
            r=1;
        }
        else {
            fprintf(F_out,"%s %s %ld %s %s %s %s\n",c.nom,c.prenom,c.GSM,c.email,c.adresse.rue,c.adresse.ville,c.adresse.pays);
        }
    }
    fclose(F_out);
    fclose(F_in);
    remove(nom_fichier);
    rename(nom_fichier1,nom_fichier);
    return r;
}

int main()
{
    char server_message[256] = "vous avez connectez au serveur";
    // création du server socket
    int server_socket;
    server_socket=socket(AF_INET,SOCK_STREAM,0);
    // definir une adresse pour socket
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(9002);
    server_address.sin_addr.s_addr = INADDR_ANY;

    // bind socket to our specified IP address
    bind(server_socket,(struct sockaddr *) &server_address, sizeof(server_address));
    listen(server_socket,5); //5 combien des sockets seront connectés a celui là

     int client_socket;
     client_socket = accept(server_socket, NULL, NULL);
     //lire

    // envoi des données
    send(client_socket,server_message,sizeof(server_message),0);


    ////////////////////////////////va et vian//////////////////////////////////


    /////////////////////////////////////////////// test login ///////////////////////////////////////////////////////////////////////////////////////////////////////////
    int testvalue=0;
    char mdp[30];
    char login[50],nom[50];
    char motpasse[10];
    char type[50];
    int res=0;
    FILE *f=fopen("comptes.txt","r");
    if(f==NULL){
       printf("impossible d'ouvrire le fichier comptes.txt \n");
       exit(1);
    }
    else{
    printf("Authentification \n ");
    bzero(nom,50);
    recv( client_socket ,nom, sizeof(nom),0);
    printf("%s \n ",nom);
    while(!feof(f)){
    fscanf(f,"%s%s%s",login,motpasse,type);
    if(strcmp(nom,login)==0){
         while( testvalue <=3){
            bzero(mdp,30);
            recv( client_socket , mdp, sizeof(mdp),0);
            printf("%s \n ",mdp);
               if(strcmp(mdp,motpasse)==0) {
                      if(strcmp(type,"admin")==0){
                          res=1;
                          send(client_socket , &res , sizeof(res) , 0 );
                          break;

                      }
                     else if(strcmp(type,"visit")==0){
                           res=2;
                           send(client_socket , &res , sizeof(res) , 0 );
                           break;
                     }
               }
         res=0;
         send(client_socket , &res , sizeof(res) , 0 );
         testvalue++;
       }

    }
    else{
    res=-1;
    }
       }
       send(client_socket , &res , sizeof(res) , 0 );
    }

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int choix=0;
Contact c;
int rep=0;
long  gsm;
  do {
        recv(client_socket , &choix , sizeof(choix),0);
                      switch (choix){
                        case 1 : {
                                 recv( client_socket , &c , sizeof(c),0);
                                 Ajouter("contacts.txt",c);
                                 rep=1;
                                 send(client_socket , &rep , sizeof(rep), 0);
                                  break;}
                       case 2 :{
                              recv(client_socket , &gsm, sizeof(gsm), 0);
                              rep=RechercherParGSM("contacts.txt",gsm);
                              send(client_socket , &rep , sizeof(rep), 0);
                              break;}
                         case 3 :{
                              recv(client_socket , &gsm, sizeof(gsm), 0);
                              rep=Supprimer("contacts.txt",gsm);
                              send(client_socket , &rep , sizeof(rep), 0);
                              break;}
                          case 4 :{
                              recv(client_socket , &gsm, sizeof(gsm), 0);
                              recv(client_socket , &c , sizeof(c), 0);
                              rep=Modifier("contacts.txt",gsm,c);
                              send(client_socket , &rep , sizeof(rep), 0);
                              break;}
                        case 5 : break;
                        case 6 :     close(server_socket); break;
                        default :
                                 printf("votre choix est %d \n\n ",choix);
                                 break;
                      }
        } while(choix !=6);


    close(server_socket);
    return 0;
}
