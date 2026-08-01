#ifndef _BOARD_H_
#define _BOARD_H_

#include <stdbool.h>

/**
 * \file board.h
 *
 * \brief This SAE game engine functions.
 *
 * In this file, all the functions for having a game run are defined and documented.
 *  
 * \author Paul Dorbec
 *
 */

/**
 * \mainpage Project for IUT Grand Ouest Normandie, 
 * computer science department.
 *
 * \section description Description of the project
 * This project aims to implement a two-player board game. 
 * The rules of the game are described below.
 * 
 * The project is divided into several files. The game engine, which
 * implements the detailed rules, is mainly provided in the files board.c and board.h
 *
 * \section rules Detailed rules of the game.
 * 
 * The game is played by two players, referred to as ::NORTH_P and ::SOUTH_P,
 * on a six by six square board with two goals.
 * One goal is located at the north of the board,
 * adjacent to the six northernmost squares, 
 * and the other is at the south, 
 * adjacent to the six southernmost squares.
 *
 * The objective of each player is to be the first to bring a piece 
 * to the opponent's goal.
 * 
 * There are 12 pieces on the board: four pieces of each of the three sizes 
 * (::ONE, ::TWO and ::THREE).
 * Pieces do not belong to any player and may be used by either player.
 *
 * \subsection setting Setting up of the game
 * During the first phase of the game, players must place their pieces on the board,
 * on the row closest to their side. 
 * Each player places two pieces of each size. 
 * They may or may not alternate turns when placing pieces 
 * (using the function ::place_piece).
 * Once each player has placed six pieces, the game begins. 
 *
 * \subsection moves Moving pieces
 * On their turn, each player picks a piece from the row closest 
 * to their side on the board (using the function ::pick_piece) and moves it 
 * (using function ::move_piece).
 * The piece must move a number of steps equal to its ::size, 
 * with each step in one of the four possible directions
 * (::SOUTH, ::NORTH, ::EAST, ::WEST) toward an empty square. 
 * Only the last move may be toward an occupied position, 
 * in which case the piece _bounces_ off the other piece
 * and continues its move with a number of steps 
 * equal to the ::size of the other piece.
 * 
 * A piece may only enter the opponent goal with its last move 
 * (possibly after several bounces),
 * resulting into the current player winning the game.
 * 
 * \subsection special Special move
 * When a piece reaches another piece's position at the end of its move,
 * the player has an additional option (function ::swap_piece).
 * Instead of bouncing off the piece, 
 * the player may replace the reached piece with the moving piece 
 * and then place the removed piece on any empty square of the board
 * (excluding the goals). This concludes the current player's turn. 
 *
 */

/**
 * @brief Number of rows and columns of the game.
 *
 * In the following, all indices are given from 0 to ::DIMENSION - 1.
 * Small line numbers correspond to the south.
 */
#define DIMENSION 6

/**
 * @brief Pointer to the structure that holds the game. 
 *
 * Details of the content are not necessary for its use, so the structure is not 
 * included here.
 */
typedef struct board_s* board;

/**
 * @brief the different sizes of pieces.
 *
 * This set includes a reference to a size ::NONE that represents the absence of a piece. 
 * Sizes are ordered increasingly in the enumerator,
 * so that they can be compared with < or >.
 */
typedef enum sizes_e {
	NONE, /**< represents the absence of a piece */
	ONE, /**< size 1 */
	TWO, /**< size 2 */
	THREE, /**< size 3 */
	} size; 

/**
 * @brief number of non-empty sizes
 */
#define NB_SIZE 3

/**
 * @brief the different players for further reference. 
 */
typedef enum players_e {
	NO_PLAYER, /**< used when informing that a square is empty */
	SOUTH_P, /**< South player, starting on line 0 */
	NORTH_P, /**< North player, starting on line ::DIMENSION - 1 */
	} player;


/**
 * @brief the different directions in the game.
 * ::GOAL is legal only on the northernmost line for south player, 
 * southernmost line for north player.
 * other directions are natural.
 */
typedef enum direction_e {
	GOAL, /**< reaching the goal, legal only at a special stage of the movement.*/
	SOUTH, /**< south, toward decreasing line numbers */
	NORTH, /**< north, toward increasing line numbers */
	EAST, /**< east, toward increasing column number */
	WEST /**< west, toward decreasing column number */
	} direction ;

/**
 * @brief number of players in the game.
 */
#define NB_PLAYERS 2

/**
 * @brief returns the next player
 * 
 * This function simply returns the player following current_player in the game turn.
 * It does not use any information from the board game.
 *
 * @param player the player to change
 * @return the next player
 */
player next_player(player current_player);

/**
 * @brief number of pieces of each size on each player's line at the beginning.
 * Usually, this value is 2.
 */
#define NB_INITIAL_PIECES 2

/**
 * @brief return codes give semantics to the values returned by functions.
 */
typedef enum return_code_e {
	OK, /**< success */
	EMPTY, /**< given space should or should not be empty */
	FORBIDDEN, /**< forbidden request */
	PARAM /**< invalid parameter */
	} return_code;

/**@{
 * \name Creation/deletion functionalities.
 */

/**
 * @brief Defines a new empty ::board for starting a game.
 */
board new_game();

/**
 * @brief Makes a deep copy of the game.
 * @param original_game the game to copy.
 * @return a new copy fully independent of the original game.
 */
board copy_game(board original_game);

/**
 * @brief Delete the game and frees all required memory.
 * @param game the game to destroy.
 */
void destroy_game(board game);

/**@}*/

/**@{
 * \name Accessing game data functionalities.
 */

/**
 * @brief returns the ::size of the piece at the given position on the board, if any.
 * 
 * Return the ::size of a piece on the board, ::NONE if there is no piece.
 * If there is a piece currently moving
 * it is not taken into account by this function.
 * If the coordinates do not correspond to a valid place on the board, returns ::NONE.
 *
 * @param game the game from which to collect information.
 * @param line the line number
 * @param column the column number
 * @return the ::size of the holding piece
 */
size get_piece_size(board game, int line, int column);

/**
 * @brief Tells if the game has a winner
 *
 * Recall that a player wins if he manages to place a piece on the opponent's goal.
 * This returns the winning ::player, ::NO_PLAYER if nobody won yet. 
 *
 * @param game the game to test.
 * @return the player who wins, possibly ::NO_PLAYER.
 */
player get_winner(board game);

/**
 * @brief Returns the southernmost line number which contains a piece
 * 
 * This is the smallest line number to carry a piece that can move for player South.
 * If there is no piece that can move on any line, returns -1 (implying the board is empty).
 * This can be used for deciding whether a move for ::SOUTH_P is legal
 *
 * @param game the game to consider.
 * @return the line number.
 **/
int southmost_occupied_line(board game);

/**
 * @brief Returns the northernmost line number which contains a piece.
 * 
 * This is the largest line number to carry a piece that can move for player North.
 * If there is no piece that can move on any line, return -1 (implying the board is empty)
 * This can be used for deciding whether a move from ::NORTH_P is legal.
 *
 * @param game the game to consider.
 * @return the line number.
 **/
int northmost_occupied_line(board game);


/**
 * @brief returns the player whose piece is currently moving
 * 
 * If a player is currently in a move, returns that ::player.
 * Otherwise, returns ::NO_PLAYER.
 *
 * @param game the game to consider.
 * @return the player who is currently moving.
 */
player picked_piece_owner(board game);

/**
 * @brief returns the ::size of the piece currently moving (if any)
 *
 * If a player is currently in a move,
 * returns the piece ::size, 
 * returns ::NONE if there is no piece currently in hand.
 * 
 * @param game the game to consider.
 * @return the ::size of the piece currently moving.
 */
size picked_piece_size(board game);

/**
 * @brief returns the line number of the piece currently moving (if any).
 * 
 * If a player is currently moving, return the line number of the moving piece.
 * returns -1 if no piece is being moved.
 *
 * @param game the game to consider.
 * @return the line number of the moving piece.
 */
int picked_piece_line(board game);

/**
 * @brief returns the column number of the piece currently moving (if any)
 * 
 * If a player is currently moving, return the column number of the moving piece.
 * Returns -1 if no piece is being moved.
 *
 * @param game the game to consider.
 * @return the column number of the moving piece.
 */
int picked_piece_column(board game);

/**
 * @brief returns the number of movement units left to the current moving piece
 * 
 * If a piece is currently being moved, this function returns the number of movement
 * unit left to that piece (ignoring future bouncing)
 * This function returns 0 if and only if the piece just finished its movement over another piece
 * (i.e. the player must choose whether to bounce or to substitute the piece with the one under.) 
 * If there is no piece under movement, this function returns -1.
 * 
 * @param game the game to consider.
 * @return the number of moves left to the piece.
 */
int movement_left(board game);

/**@}*/

/**@{
 * \name Game setting up
 * 
 * Functionalities for the first phase, consisting in placing the pieces on the board.
 */

/**
 * @brief Indicates whether the size of piece is still to be placed by the suggested player
 *
 * returns the number of pieces of the given size the players still need to place, 
 * -1 if the parameters are invalid.
 * After the setup of the game is finished, returns 0.
 * 
 * @param game the game to be considered
 * @param piece the piece size requested
 * @param player the player whose piece is to check
 * @return the number of pieces of that size to be placed by the player
 */
int nb_pieces_available(board game, size piece, player player);

/**
 * @brief places a piece on the board, during the initial setting up of the game. 
 * 
 * The piece is placed on the given player's side, which determines the line number.
 * If placing the piece is not possible, the function returns in this order:
 * * ::PARAM  if the parameters are invalid
 *    (wrong player, piece size or position out of the board)
 * * ::EMPTY if the indicated position is not empty.
 * * ::FORBIDDEN if this player has already placed 
 *    his maximum number of pieces of that size 
 *    (and in particular, if the setup phase is over).
 * 
 * @param game the game to consider
 * @param piece the piece size requested
 * @param player the player whose piece to place
 * @param column the column where to place the piece (from 0 to ::DIMENSION - 1)
 * @return a ::return_code stating the result of the placement
 */
return_code place_piece(board game, size piece, player player, int column);



/**@}*/

/**@{
 * \name Playing functionalities
 */

/**
 * @brief Select a piece to start with.
 *
 * The piece must be taken from the player's line. 
 * Whether the player is allowed to play the piece is tested. 
 * The piece is then ready to be moved with the other functions, 
 * 
 * If picking the piece is not possible, the ::return_code indicates why, 
 * in this order:
 * * ::FORBIDDEN if the setup phase is not over or if the game has a winner
 * * ::PARAM  if the parameters are invalid
 *    (wrong player, piece size or position out of the board)
 * * ::EMPTY if the position is empty.
 * * ::FORBIDDEN if the piece does not belong to the line 
 *    the player is supposed to move from.
 * Otherwise, returns ::OK for success.
 *
 * @param game the game where to pick a piece.
 * @param current_player the player who is supposed to be playing.
 * @param line the line number of where to pick the piece (from 0 to ::DIMENSION - 1).
 * @param column the column number of where to pick the piece (from 0 to ::DIMENSION - 1).
 * @return a ::return_code, ::OK if everything went smoothly
 */
return_code pick_piece(board game, player current_player, int line, int column);

/**
 * @brief Indicates whether a move is possible.
 *
 * Returns true if the move is possible for the current piece.
 * The direction GOAL is possible if the piece is on the northernmost
 * line and south is playing, or reversely.
 * Other moves are possible if the place indicated by the direction, or
 * if it contains a piece and the piece has only one movement unit left.
 * Additionally, if the piece has already made a move along this line, 
 * the movement should no longer be possible.
 *
 * @param game the game where to move a piece.
 * @param direction the ::direction where the movement is aimed 
 * @return whether a move in that direction is possible.
 */
bool is_move_possible(board game, direction direction);


/**
 * @brief Moves the current picked piece, if possible.
 *
 * Moves the piece according to the given direction.
 * If the piece arrived over another piece at the end of its previous movement,
 * calling this function set that the piece bounced and continues its movement.
 * The direction may be GOAL if the piece is on the closest line to the goal.
 * If moving the piece is not possible, returns a ::return_code encoding the 
 * identified problem.
 * * ::EMPTY there is no piece currently in the players hand.
 * * ::PARAM the target position is off the board
 * * ::FORBIDDEN moving toward that position is not allowed 
 *      (e.g. occupied while the piece movement is not finished 
 * 	 or target is GOAL but not reachable)
 * Otherwise, returns ::OK for success.
 *
 * @param game the game where to move a piece.
 * @param direction the direction where the movement is aimed.
 * @return a ::return_code encoding the result of the placement.
 */
return_code move_piece(board game, direction direction);

/**
 * @brief swap the current piece with the piece below it,
 * and place the piece below at the given position.
 * 
 * If the piece moving just finished its movement over another piece, 
 * this function allows the player to swap the two pieces, and place the piece below anywhere.
 * A successful swap finishes the movement of the player (no picked piece, no moves left, etc.)
 * The function returns ::OK when the operation succeeds, 
 * otherwise a ::return_code corresponding to the problem met:
 * * ::EMPTY if the swapping is not possible at the moment 
 *      (the current moving piece has not finished its movement over another piece)
 * * ::PARAM when the position proposed is off the board.
 * * ::FORBIDDEN when the position proposed is not free or the move is not currently possible.
 * @param game the game where to move a piece.
 * @param target_line the line number of where to move the swapped piece.
 * @param target_column the column number of where to move the swapped piece.
 * @return a ::return_code encoding the result of swap, ::OK if things went smoothly.
 */
return_code swap_piece(board game, int target_line, int target_column);

/**
 * @brief cancels the current movement of the player, putting the piece back where it started.
 * 
 * This is necessary since a player could start a movement with some piece which have no possible issue.
 * Using this function, the moving piece is placed back where it started, and no piece is considered picked anymore.
 * The function returns ::OK when the operation succeeds, 
 * otherwise a ::return_code corresponding to the problem met:
 * * ::EMPTY if there is no piece currently in movement.
 * 
 * @param game the game where to cancel the movement.
 * @return a ::return_code encoding the result of swap, ::OK if things went smoothly.
 */
return_code cancel_movement(board game);

/**
 * @brief cancels the last step of the current move.
 *
 * This function cancels only the last step of the current move. 
 * If the last step was just picking the piece, this is equivalent to cancelling the movement.
 * 
 * The function returns ::OK when the operation succeeds, 
 * otherwise a ::return_code corresponding to the problem met:
 * * ::EMPTY if there is no piece currently in movement.
 *
 * @param game the game where to cancel the movement.
 * @return a ::return_code encoding the result of the swap, ::OK if things went smoothly.
 */
return_code cancel_step(board game);


/**@}*/

#endif /*_BOARD_H_*/
