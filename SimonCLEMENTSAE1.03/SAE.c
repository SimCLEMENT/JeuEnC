/** 
 * @file SAE1.1.c 
 * @brief A simple illustration of how to include and use board.h and board.o.
 * @author Simon CLEMENT 
 */


#include <stdio.h>
#include <stdlib.h>  // Assure-toi d'inclure stdlib.h
#include "board.h"

#define RED "\x1b[31m"
#define GREEN "\x1b[32m"
#define BLUE "\x1b[34m"
#define RESET "\x1b[0m"


/**
 * @file SAE1.1.c
 * @brief a simple illustration of how to include and use board.h and board.o.
 * @author Simon CLEMENT
 */

void interface(board game) {
    printf("    "); 
    for (int c = 0; c < DIMENSION; c++) printf("%2d ", c); 
    printf("\n");

    printf("   +");
    for (int c = 0; c < DIMENSION; c++) printf("---");
    printf("+\n");

    for (int l = DIMENSION - 1; l >= 0; l--) {
        printf("%2d |", l); 
        for (int c = 0; c < DIMENSION; c++) {
            int piece = get_piece_size(game, l, c);
            if (piece == NONE) printf(" . ");
            else if (piece > 0) printf(RED " %d " RESET, piece); // Joueur Nord en rouge
            else printf(GREEN " %d " RESET, piece); // Joueur Sud en vert
        }
        printf("|\n");
    }

    printf("   +");
    for (int c = 0; c < DIMENSION; c++) printf("---");
    printf("+\n");
}




void initialisation(board game) {
    // Initialisation du plateau
    int insere1 = 0;
    while (insere1 < DIMENSION) {  // Joueur Nord
        printf("Joueur Nord : Veuillez choisir une pièce à ajouter sur le plateau : ");
        int piece_choisie;
        scanf("%d", &piece_choisie);
        if (nb_pieces_available(game, piece_choisie, NORTH_P) > 0) {
            place_piece(game, piece_choisie, NORTH_P, insere1);
            printf("%d placé avec succès\n", piece_choisie);
            insere1++;
        } else {
            printf("Désolé, vous ne pouvez pas choisir cette pièce.\n");
        }
    }

    printf("\n");

    int insere2 = 0;
    while (insere2 < DIMENSION) {  // Joueur Sud
        printf("Joueur Sud : Veuillez choisir une pièce à ajouter sur le plateau : ");
        int piece_choisie;
        scanf("%d", &piece_choisie);
        if (nb_pieces_available(game, piece_choisie, SOUTH_P) > 0) {
            place_piece(game, piece_choisie, SOUTH_P, insere2);
            printf("%d placé avec succès\n", piece_choisie);
            insere2++;
        } else {
            printf("Désolé, vous ne pouvez pas choisir cette pièce.\n");
        }
    }
}

int selection_piece(board game, player player) {
    // Sélectionne la pièce à déplacer
    int l, c;
    printf("Choisir pièce pour jouer :\n");
    printf("Ligne : ");
    scanf("%d", &l);
    printf("Colonne : ");
    scanf("%d", &c);

    int nb_cout = get_piece_size(game, l, c);
    while (pick_piece(game, player, l, c) != OK) {
        printf("Nombre incorrect\n");
        printf("Choisir un autre nombre pour jouer :\n");
        printf("Ligne : ");
        scanf("%d", &l);
        printf("Colonne : ");
        scanf("%d", &c);
    }

    return nb_cout;
}

void coup_possible(board game, player player) {
    // Vérifie si le coup est possible et l'effectue si oui
    int valide = 0;
    while (valide != 1) {
        printf("Veuillez renseigner votre mouvement parmi ceux ci-dessous : S N E W\n");
        char direction;
        scanf(" %c", &direction);

        if (direction == 'S' || direction == 's') {
            if (is_move_possible(game, SOUTH)) {
                valide++;
                move_piece(game, SOUTH);
            }
        } else if (direction == 'N' || direction == 'n') {
            if (is_move_possible(game, NORTH)) {
                valide++;
                move_piece(game, NORTH);
            }
        } else if (direction == 'E' || direction == 'e') {
            if (is_move_possible(game, EAST)) {
                valide++;
                move_piece(game, EAST);
            }
        } else if (direction == 'W' || direction == 'w') {
            if (is_move_possible(game, WEST)) {
                valide++;
                move_piece(game, WEST);
            }
        }
    }
}

void echange(board game, player player) {
    // Échange une pièce avec une autre inscrite par le joueur
    if (movement_left(game) != 0) {
        printf("Vous ne pouvez pas échanger\n");
        return;
    }

    printf("Vous pouvez échanger la pièce.\n");

    int target_l, target_c;
    printf("Choisissez une case VIDE pour placer la pièce échangée :\n");
    printf("Ligne : ");
    scanf("%d", &target_l);
    printf("Colonne : ");
    scanf("%d", &target_c);

    if (target_l < 0 || target_l >= DIMENSION || target_c < 0 || target_c >= DIMENSION) {
        printf("Hors du plateau. Échange impossible.\n");
        return;
    }

    if (get_piece_size(game, target_l, target_c) != NONE) {
        printf("Case occupée. Échange impossible.\n");
        return;
    }

    return_code code = swap_piece(game, target_l, target_c);
    if (code == OK) {
        printf("Échange effectué avec succès !\n");
    } else {
        printf("Échec de l'échange : %d\n", code);
    }
}

void bounced(board game, player player) {
    // Fait rebondir une pièce sur une autre
    if (movement_left(game) != 0) {
        printf("Vous ne pouvez pas rebondir\n");
        return;
    }

    int valide = 0;
    while (valide != 2) {
        printf("Veuillez renseigner votre mouvement parmi ceux ci-dessous : S N E W\n");
        char direction;
        scanf(" %c", &direction);

        if (direction == 'S' || direction == 's') {
            if (is_move_possible(game, SOUTH)) {
                valide++;
                move_piece(game, SOUTH);
            }
        } else if (direction == 'N' || direction == 'n') {
            if (is_move_possible(game, NORTH)) {
                valide++;
                move_piece(game, NORTH);
            }
        } else if (direction == 'E' || direction == 'e') {
            if (is_move_possible(game, EAST)) {
                valide++;
                move_piece(game, EAST);
            }
        } else if (direction == 'W' || direction == 'w') {
            if (is_move_possible(game, WEST)) {
                valide++;
                move_piece(game, WEST);
            }
        }
    }

    printf("Rebond réussi\n");
}

void cancelled(board game) {
    // Vérification si un mouvement a été effectué
    if (movement_left(game) == 0) {  // Si le joueur a effectué un mouvement complet, on annule le mouvement
        printf("Mouvement complet annulé.\n");
        cancel_movement(game);  // Annule tout le mouvement
    } else {  // Sinon, on annule juste un pas de mouvement (un déplacement partiel)
        printf("Annulation du dernier pas effectué.\n");
        cancel_step(game);  // Annule un seul pas
    }
}

player victoire(board game) {
    // Vérifie si un joueur a gagné
    player gagnant = get_winner(game);
    if (gagnant != NO_PLAYER) {
        printf("Le joueur %d a gagné la partie !\n", gagnant);
        return gagnant;
    } else {
        return NO_PLAYER;
    }
}

void mouvement(board game, player player) {
    // Gère tous les mouvements du jeu
    int nb_cout = selection_piece(game, player);
    interface(game);

    for (int i = 0; i < nb_cout; i++) {
        coup_possible(game, player);
    }

    if (movement_left(game) == 0) {
        printf("Souhaitez-vous échanger ? (o/n) : ");
        char rep;
        scanf(" %c", &rep);

        if (rep == 'o' || rep == 'O') {
            echange(game, player);
        } else {
            printf("Voulez-vous rebondir ? (o/n) : ");
            char rep;
            scanf(" %c", &rep);

            if (rep == 'o' || rep == 'O') {
                bounced(game, player);
            }
        }
    }

    printf("Souhaitez-vous annuler le dernier mouvement ? (o/n) : ");
    char rep;
    scanf(" %c", &rep);

    if (rep == 'o' || rep == 'O') {
        cancelled(game);
    }
}

int main() {
    // Création et gestion du jeu
    board game = new_game();
    printf("Un plateau est créé.\n");
    initialisation(game);
    interface(game);

    player player = NORTH_P;
    while (get_winner(game) == NO_PLAYER) {
        mouvement(game, player);
        interface(game);
        player = next_player(player);
    }

    return 0;
}
