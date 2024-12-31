#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "common.h"

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE] = {0};
    int compteur;

    // Création du socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Erreur de création du socket");
        exit(EXIT_FAILURE);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Conversion de l'adresse IP
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        perror("Adresse invalide");
        close(sock);
        exit(EXIT_FAILURE);
    }

    // Connexion au serveur
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Erreur de connexion");
        close(sock);
        exit(EXIT_FAILURE);
    }

    printf("Connecté au serveur.\n");

    while (1) {
        // Réception du compteur depuis le serveur
        memset(buffer, 0, BUFFER_SIZE);
        read(sock, buffer, BUFFER_SIZE);
        compteur = atoi(buffer);
        printf("Client: reçu compteur = %d\n", compteur);

        // Incrémentation et envoi au serveur
        compteur++;
        sprintf(buffer, "%d", compteur);
        send(sock, buffer, strlen(buffer), 0);
        printf("Client: envoyé compteur = %d\n", compteur);

        sleep(1);
    }

    close(sock);
    return 0;
}
