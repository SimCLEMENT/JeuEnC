# SAÉ 1.1 / 1.2 — Jeu de stratégie à deux joueurs

Projet réalisé dans le cadre du BUT Informatique, IUT Grand Ouest Normandie (campus d'Ifs), en première année.

Ce dépôt contient l'implémentation d'un jeu de stratégie à deux joueurs, développée en deux sprints successifs.

## 🎮 Le jeu

Deux joueurs (**Nord** et **Sud**) s'affrontent sur un plateau de 6x6 cases, avec un en-but de chaque côté. Chaque joueur cherche à amener une pièce jusqu'à l'en-but adverse.

- 12 pièces communes aux deux joueurs, réparties en 3 tailles (1, 2 et 3)
- Chaque pièce se déplace d'un nombre de pas égal à sa taille, dans une direction cardinale
- Si une pièce en rencontre une autre en fin de déplacement, elle **rebondit** et poursuit son mouvement avec le nombre de pas correspondant à la taille de la pièce rencontrée (ou peut **échanger** sa place avec elle)
- Le premier joueur à placer une pièce dans l'en-but adverse gagne

Le règlement détaillé est disponible dans [`sujet.pdf`](./sujet.pdf).

## 🧩 Structure du projet

Le projet est divisé en deux sprints indépendants, correspondant à deux objectifs pédagogiques distincts.

### Sprint 1 — Interface utilisateur (`SAE.c`)

Objectif : développer une interface console permettant de jouer une partie complète, **en utilisant un moteur de jeu déjà fourni** (`board.o` précompilé, avec son en-tête `board.h`).

- [`SAE.c`](./SAE.c) : interface de jeu en console (affichage du plateau, gestion des tours, saisie des déplacements, échanges, annulations, détection de victoire)
- [`board.h`](./board.h) : en-tête fourni décrivant l'API du moteur de jeu (non modifiable)
- [`main_example.c`](./main_example.c) : exemple minimal fourni par l'enseignant illustrant l'usage de `board.h`

> Le fichier `board.o` fourni pour ce sprint n'est pas inclus dans ce dépôt (fichier précompilé propre à l'environnement de rendu).

### Sprint 2 — Moteur de jeu (`board.c`)

Objectif : implémenter soi-même le moteur du jeu, **en remplacement du `board.o` fourni**, en respectant scrupuleusement les spécifications du `board.h` fourni (sans le modifier). Le code est ensuite validé par une suite de tests automatiques.

- [`board.c`](./board.c) : implémentation complète du moteur de jeu (structure du plateau, placement, déplacement, rebonds, échanges, annulation, détection de victoire...)

Contrainte importante du sujet : aucune modification du `board.h` original n'est autorisée, y compris l'ajout usuel de la définition de la structure `board_s` dans l'en-tête — celle-ci reste entièrement interne à `board.c`.

## ⚙️ Compilation

Le projet se compile avec `gcc` :

```bash
gcc -Wall SAE.c board.c -o jeu.exe
./jeu.exe
```

## 📖 Documentation

La documentation complète des fonctions du moteur de jeu (générée à partir de `board.h`) est disponible en ligne : https://dorbec.users.greyc.fr/SAE

## ✍️ Auteur

Simon CLEMENT — BUT Informatique, IUT Grand Ouest Normandie
