#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "board.h"
/**
 * \file board.c
 *
 * \brief Source code associated with \ref board.h
 *
 * \author Simon CLEMENT
 */

/**
 * @brief The board of the game.
 */

typedef struct {
    size size;     
    player owner;  
} Piece;

// Génération du tableau du jeu
struct board_s {
    Piece grid[DIMENSION][DIMENSION];
    int remaining[NB_PLAYERS + 1][NB_SIZE + 1]; // Tableau à deux dimmensions pour gérer les pièces restantes lors de l'initialisation du jeu
    bool has_moving;
    player picked_owner;
    size picked_size;
    int picked_line, picked_col;
    int origin_line, origin_col;
    int last_line, last_col;
    int moves_left;
    player winner;
};

/**
 * @brief permet de gérer la fin de l'initialisation au début de partie :
 * Tant que toute les pièce n'ont pas été placés on ne peut pas jouer
 *
 * La partie ne peut commencer que lorsque tous les joueurs
 * ont placé toutes leurs pièces.
 *
 * @param g Plateau de jeu
 * @return true si toutes les pièces ont été placées, false sinon
 */
static bool setup_finished(board g) {
    for (int p = SOUTH_P; p <= NORTH_P; p++) {
        for (int s = ONE; s <= THREE; s++) {
            if (g->remaining[p][s] > 0) return false;
        }
    }
    return true;
}

/**
 * @brief Vérifie si une un pion est à l'intérieur du plateau.
 *
 * @param l Ligne à tester
 * @param c Colonne à tester
 * @return true si la position est valide, false sinon
 */
static bool inside(int l, int c) {
    return l >= 0 && l < DIMENSION && c >= 0 && c < DIMENSION;
}

/*-----------------------------------------------------------------------------------
                        Création, Copie et Suppression de la game                   
-------------------------------------------------------------------------------------*/
board new_game(void) {
    board g = malloc(sizeof(*g));
    if (!g) return NULL; // Test pour savoir si le plateau (g) existe, si il n'existe pas on arrête la fonction immédiatement

    for (int i = 0; i < DIMENSION; i++) {
        for (int j = 0; j < DIMENSION; j++) {
            g->grid[i][j].size = NONE;
            g->grid[i][j].owner = NO_PLAYER;
        }
    }

    for (int p = SOUTH_P; p <= NORTH_P; p++) {
        for (int s = ONE; s <= THREE; s++) {
            g->remaining[p][s] = NB_INITIAL_PIECES;
        }
    }

    g->has_moving = false;
    g->picked_owner = NO_PLAYER;
    g->picked_size = NONE;
    g->picked_line = g->picked_col = -1;
    g->origin_line = g->origin_col = -1;
    g->last_line = g->last_col = -1;
    g->moves_left = -1;
    g->winner = NO_PLAYER;

    return g;
}

board copy_game(board src) {
    if (!src) return NULL;
    board g = malloc(sizeof(*g));
    if (!g) return NULL; 
    *g = *src; 
    return g;
}

void destroy_game(board g) {
    free(g);
}

/*-----------------------------------------------------------------------------------
                                    Fonction Get                   
-------------------------------------------------------------------------------------*/
/**
 * @brief Retourne la taille de la pièce située à une position donnée.
 *
 * @param g Plateau de jeu
 * @param line Ligne
 * @param column Colonne
 * @return Taille de la pièce ou NONE si la case est invalide ou vide
 */
size get_piece_size(board g, int line, int column) {
    if (!g || !inside(line, column)) {
        return NONE;
    }
    return g->grid[line][column].size;
}

/**
 * @brief Retourne le joueur gagnant.
 *
 * @param g Plateau de jeu
 * @return Joueur gagnant ou NO_PLAYER si la partie n'est pas terminée
 */
player get_winner(board g) {
    if (!g) return NO_PLAYER;
    return g->winner;
}

/*-----------------------------------------------------------------------------------
                        Fonction Southmost et northmost                   
-------------------------------------------------------------------------------------*/
/**
 * @brief Donne la l'indice de la ligne la plus au sud occupée par une pièce du joueur SOUTH.
 *
 * @param g Plateau de jeu
 * @return Indice de ligne ou -1 si aucune pièce n'est trouvée
 */
int southmost_occupied_line(board g) {
    if (!g) return -1;
    for (int i = 0; i < DIMENSION; i++) {
        for (int j = 0; j < DIMENSION; j++) {
            if (g->grid[i][j].owner == SOUTH_P) {
                return i;
            }
        }
    }
    return -1;
}

/**
 * @brief Donne la l'indice de la ligne la plus au nord occupée par une pièce du joueur NORTH.
 *
 * @param g Plateau de jeu
 * @return Indice de ligne ou -1 si aucune pièce n'est trouvée
 */
int northmost_occupied_line(board g) {
    if (!g) return -1;
    for (int i = DIMENSION - 1; i >= 0; i--) {
        for (int j = 0; j < DIMENSION; j++) {
            if (g->grid[i][j].owner == NORTH_P) {
                return i;
            }
        }
    }
    return -1;
}

/*-----------------------------------------------------------------------------------
                                Fonction picked                   
-------------------------------------------------------------------------------------*/
/**
 * @brief Retourne le propriétaire de la pièce actuellement en train de ce déplacée.
 *
 * @param g Plateau de jeu
 * @return Joueur propriétaire ou NO_PLAYER s'il n'y a pas de mouvement
 */
player picked_piece_owner(board g) {
    if (!g || !g->has_moving) { // Si le jeu n'existe pas ou qu'il n'y a pas de mouvement alors ...
        return NO_PLAYER;
    }
    return g->picked_owner;
}

/**
 * @brief Retourne la taille de la pièce actuellement en train de ce déplacée.
 *
 * @param g Plateau de jeu
 * @return Taille de la pièce ou NONE s'il n'y a pas de mouvement
 */
size picked_piece_size(board g) {
    if (!g || !g->has_moving) {
        return NONE;
    }
    return g->picked_size;
}

/**
 * @brief Retourne la ligne / colonne de la pièce en cours de déplacement.
 *
 * @param g Plateau de jeu
 * @return Coordonnée ou -1 s'il n'y a pas de mouvement
 */
int picked_piece_line(board g) {
    if (!g || !g->has_moving) {
        return -1;
    }
    return g->picked_line;
}

int picked_piece_column(board g) {
    if (!g || !g->has_moving) {
        return -1;
    }
    return g->picked_col;
}

/**
 * @brief Sélectionne une pièce pour commencer un déplacement.
 *
 * Vérifie que la pièce appartient bien au joueur,
 * qu'elle est sur la bonne ligne de départ,
 * et initialise l'état du mouvement.
 *
 * @param g Plateau de jeu
 * @param p Joueur
 * @param line Ligne
 * @param col Colonne
 * @return Code indiquant le succès ou l'échec
 */
return_code pick_piece(board g, player p, int line, int col) {
    if (!g) return PARAM;
    if (!setup_finished(g) || g->winner != NO_PLAYER) {
        return FORBIDDEN;
    }
    if (p < SOUTH_P || p > NORTH_P || !inside(line, col)) {
        return PARAM;
    }

    Piece pc = g->grid[line][col];
    if (pc.size == NONE) {
        return EMPTY;
    }
    if (pc.owner != p) {
        return FORBIDDEN;
    }
    if ((p == SOUTH_P && line != 0) || (p == NORTH_P && line != DIMENSION - 1)) {
        return FORBIDDEN;
    }

    g->has_moving = true;
    g->picked_owner = pc.owner;
    g->picked_size = pc.size;
    g->picked_line = g->origin_line = line;
    g->picked_col = g->origin_col = col;
    g->last_line = line;
    g->last_col = col;
    g->moves_left = pc.size;

    g->grid[line][col].size = NONE;
    g->grid[line][col].owner = NO_PLAYER;

    return OK;
}

/*-----------------------------------------------------------------------------------
                                Fonction piece                   
-------------------------------------------------------------------------------------*/
/**
 * @brief Retourne le nombre de déplacements restants pour la pièce en cours.
 *
 * Cette fonction permet de connaître combien de cases la pièce sélectionnée
 * peut encore parcourir avant la fin de son déplacement.
 *
 * @param g Plateau de jeu
 * @return Nombre de déplacements restants ou -1 s'il n'y a pas de mouvement en cours
 */
int movement_left(board g) {
    if (!g || !g->has_moving) {
        return -1;
    }
    return g->moves_left;
}

/**
 * @brief Indique le nombre de pièces restantes à placer pour un joueur.
 *
 * Cette fonction est utilisée pendant la phase d'initialisation
 * pour savoir combien de pièces d'une taille donnée
 * un joueur peut encore poser sur le plateau.
 *
 * @param g Plateau de jeu
 * @param piece Taille de la pièce
 * @param p Joueur
 * @return Nombre de pièces disponibles, 0 si la phase de placement est terminée,
 *         ou -1 en cas de paramètres invalides
 */
int nb_pieces_available(board g, size piece, player p) {
    if (!g || p < SOUTH_P || p > NORTH_P || piece < ONE || piece > THREE) {
        return -1;
    }
    if (setup_finished(g)) {
        return 0;
    }
    return g->remaining[p][piece];
}

/**
 * @brief Place une pièce lors de la phase d'initialisation.
 *
 * @param g Plateau de jeu
 * @param piece Taille de la pièce
 * @param p Joueur
 * @param col Colonne de placement
 * @return Code indiquant le succès ou l'échec de l'opération
 */
return_code place_piece(board g, size piece, player p, int col) {
    if (!g || p < SOUTH_P || p > NORTH_P || piece < ONE || piece > THREE || col < 0 || col >= DIMENSION)
        return PARAM;

    int line = (p == SOUTH_P) ? 0 : DIMENSION - 1; // Format compacte d'un if - else vu sur internet. Si p==SOUTH_pline vaut 0 sinon line vaut DIMMENSION - 1

    if (g->grid[line][col].size != NONE) {
        return EMPTY;
    }
    if (g->remaining[p][piece] <= 0) {
        return FORBIDDEN;
    }

    g->grid[line][col].size = piece;
    g->grid[line][col].owner = p;
    g->remaining[p][piece]--;

    return OK;
}

/*-----------------------------------------------------------------------------------
                        Fonction pour les moves et les swaps                   
-------------------------------------------------------------------------------------*/
/**
 * @brief Vérifie si un déplacement est autorisé dans une direction donnée.
 *
 * Gère les déplacements classiques, l'arrivée sur une pièce
 * ainsi que le cas particulier du rebond.
 *
 * @param g Plateau de jeu
 * @param d Direction demandée
 * @return true si le déplacement est possible, false sinon
 */
bool is_move_possible(board g, direction d) {
    if (!g || !g->has_moving) {
        return false;
    }

    int l = g->picked_line;
    int c = g->picked_col;

    if (d == GOAL) {
        if (g->picked_owner == SOUTH_P && l == DIMENSION - 1) {
            return true;
        }
        if (g->picked_owner == NORTH_P && l == 0) {
            return true;
        }
        return false;
    }

    int nl = l, nc = c;
    if (d == NORTH) {
        nl++;
    }
    if (d == SOUTH) {
        nl--;
    }
    if (d == EAST)  {
        nc++;
    }
    if (d == WEST)  {
        nc--;
    }

    if (!inside(nl, nc)) {
        return false;
    }
    if (g->moves_left == 0 && g->grid[l][c].size != NONE) {
        return true;
    }

    if (g->grid[nl][nc].size == NONE) {
        return g->moves_left > 0;
    }

    return g->moves_left == 1;
}

/**
 * @brief Effectue un déplacement de la pièce sélectionnée.
 *
 * Met à jour la position, le nombre de mouvements restants,
 * gère le rebond, la pose finale de la pièce ou la victoire.
 *
 * @param g Plateau de jeu
 * @param d Direction du déplacement
 * @return Code indiquant le résultat de l'action
 */
return_code move_piece(board g, direction d) {
    if (!g || !g->has_moving) {
        return EMPTY;
    }
    if (g->moves_left == 0 && g->grid[g->picked_line][g->picked_col].size != NONE && d != GOAL) {
        g->moves_left = g->grid[g->picked_line][g->picked_col].size;
    }

    if (d == GOAL) {
        if (!is_move_possible(g, GOAL)) {
            return FORBIDDEN;
        }
        g->winner = g->picked_owner;
        g->has_moving = false;
        return OK;
    }

    if (!is_move_possible(g, d)) {
        return FORBIDDEN;
    }

    int nl = g->picked_line;
    int nc = g->picked_col;
    if (d == NORTH) {
        nl++;
    }
    if (d == SOUTH) {
        nl--;
    }
    if (d == EAST)  {
        nc++;
    }
    if (d == WEST)  {
        nc--;
    }

    g->last_line = g->picked_line;
    g->last_col  = g->picked_col;
    g->picked_line = nl;
    g->picked_col  = nc;
    g->moves_left--;

    if (g->moves_left == 0 && g->grid[nl][nc].size != NONE) {
        return OK;
    }

    if (g->moves_left == 0) {
        g->grid[nl][nc].size = g->picked_size;
        g->grid[nl][nc].owner = g->picked_owner;
        g->has_moving = false;
    }

    return OK;
}

/**
 * @brief Échange la pièce déplacée avec celle située en dessous.
 *
 * Cette action est possible uniquement lorsque le déplacement
 * est terminé sur une autre pièce.
 *
 * @param g Plateau de jeu
 * @param tl Ligne cible
 * @param tc Colonne cible
 * @return Code indiquant le succès ou l'échec
 */
return_code swap_piece(board g, int tl, int tc) {
    if (!g || !g->has_moving) {
        return EMPTY;
    }
    if (!inside(tl, tc)) {
        return PARAM;
    }
    if (g->moves_left != 0) {
        return FORBIDDEN;
    }

    if (g->grid[g->picked_line][g->picked_col].size == NONE) {
        return FORBIDDEN;
    }
    if (g->grid[tl][tc].size != NONE) {
        return FORBIDDEN;
    }

    Piece under = g->grid[g->picked_line][g->picked_col];
    g->grid[tl][tc] = under;

    g->grid[g->picked_line][g->picked_col].size = g->picked_size;
    g->grid[g->picked_line][g->picked_col].owner = g->picked_owner;

    g->has_moving = false;
    return OK;
}

/*-----------------------------------------------------------------------------------
                            Fonction pour les cancels                   
-------------------------------------------------------------------------------------*/
/**
 * @brief Annule complètement le déplacement en cours.
 *
 * La pièce revient à sa position d'origine.
 *
 * @param g Plateau de jeu
 * @return Code indiquant le résultat
 */
return_code cancel_movement(board g) {
    if (!g || !g->has_moving) {
        return EMPTY;
    }

    g->grid[g->origin_line][g->origin_col].size = g->picked_size;
    g->grid[g->origin_line][g->origin_col].owner = g->picked_owner;

    g->has_moving = false;
    return OK;
}

/**
 * @brief Annule uniquement le dernier déplacement effectué.
 *
 * Permet de revenir en arrière d'une case.
 *
 * @param g Plateau de jeu
 * @return Code indiquant le résultat
 */
return_code cancel_step(board g) {
    if (!g || !g->has_moving) {
        return EMPTY;
    }

    if (g->picked_line == g->origin_line && g->picked_col == g->origin_col){
        return cancel_movement(g);
    }

    g->picked_line = g->last_line;
    g->picked_col  = g->last_col;
    g->moves_left++;

    return OK;
}

/*-----------------------------------------------------------------------------------
                    Fonction pour la gestion du prochain joueur                   
-------------------------------------------------------------------------------------*/
/**
 * @brief Retourne le joueur suivant.
 *
 * @param p Joueur courant
 * @return Joueur suivant ou NO_PLAYER si invalide
 */
player next_player(player p) {
    if (p == SOUTH_P) {
        return NORTH_P;
    }
    if (p == NORTH_P) {
        return SOUTH_P;
    }
    return NO_PLAYER;
}

player get_piece_owner(board g, int line, int col) {
    if (!g || !inside(line, col)) return NO_PLAYER;
    return g->grid[line][col].owner;
}
