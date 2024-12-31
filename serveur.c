#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "common.h"

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    int compteur = 0;
    char buffer[BUFFER_SIZE] = {0};

    // Création du socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Erreur de création du socket");
        exit(EXIT_FAILURE);
    }

    // Configuration de l'adresse
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Liaison du socket
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Erreur de liaison");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Écoute des connexions
    if (listen(server_fd, 3) < 0) {
        perror("Erreur d'écoute");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Serveur en attente de connexion...\n");

    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
        perror("Erreur d'acceptation");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Client connecté.\n");

    while (1) {
        // Envoi du compteur au client
        sprintf(buffer, "%d", compteur);
        send(new_socket, buffer, strlen(buffer), 0);
        printf("Serveur: envoyé compteur = %d\n", compteur);

        // Réception du compteur modifié par le client
        memset(buffer, 0, BUFFER_SIZE);
        read(new_socket, buffer, BUFFER_SIZE);
        compteur = atoi(buffer);
        printf("Serveur: reçu compteur = %d\n", compteur);

        // Incrémentation et attente
        compteur++;
        sleep(1);
    }

    close(new_socket);
    close(server_fd);
    return 0;
}
