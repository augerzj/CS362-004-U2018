#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include <stdlib.h>

int victoryKingdomCompare(struct gameState *newState, struct gameState *oldState);
int otherPlayerCompare(struct gameState *newState, struct gameState *oldState);

int main() {
	int gameStateUnchanged;
	int testPass = 0;
	int seed = 1000;
	int bonus = 0;
	int numPlayers = 4;
	struct gameState G, initG;
	int k[10] = {adventurer, embargo, village, minion, mine, cutpurse,sea_hag, tribute, smithy, council_room};

	// initialize a game state and player cards
	initializeGame(numPlayers, k, seed, &initG);
	initG.hand[initG.whoseTurn][2] = village;
	memcpy(&G, &initG, sizeof(struct gameState));

	printf("----------CARD TEST #4 - TEST OF VILLAGE----------\n");

	cardEffect(village, 0, 0, 0, &G, 2, &bonus);
	testPass = 1;

	// check that card was drawn
	if(G.handCount[G.whoseTurn] == initG.handCount[initG.whoseTurn]) {
		printf("PASS: ");
	}
	else{
		printf("FAIL: ");
		testPass = 0;
	}
	printf("0 cards were drawn into hand b/c played Village (actual number drawn: %d)\n", G.handCount[G.whoseTurn] - initG.handCount[initG.whoseTurn]);

	// check that village was played
	if(G.playedCards[G.playedCardCount - 1] == village) {
		printf("PASS: ");
	}
	else{
		printf("FAIL: ");
		testPass = 0;
	}
	printf("Village is on top of played pile\n");

	// check that card was taken from deck
	if(G.deckCount[G.whoseTurn] == initG.deckCount[initG.whoseTurn] - 1) {
		printf("PASS: ");
	}
	else{
		printf("FAIL: ");
		testPass = 0;
	}
	printf("1 card was drawn from current players deck (actual number taken from deck: %d)\n", initG.deckCount[initG.whoseTurn] - G.deckCount[G.whoseTurn]);

	// check that actions increased
	if(G.numActions == initG.numActions + 2) {
		printf("PASS: ");
	}
	else{
		printf("FAIL: ");
		testPass = 0;
	}
	printf("Number of actions increased by 2 (actual increase: %d)\n", G.numActions - initG.numActions);

	// check that victory and kingdom piles were untouched
	gameStateUnchanged = victoryKingdomCompare(&G, &initG);
	if(gameStateUnchanged == 0){
		printf("PASS: ");
	}
	else{
		printf("FAIL: ");
		testPass = 0;
	}
	printf("Victory and Kingdom card piles not changed\n");

	// make sure other players were not updated at all
	gameStateUnchanged = otherPlayerCompare(&G, &initG);
	if(gameStateUnchanged == 0){
		printf("PASS: ");
	}
	else{
		printf("FAIL: ");
		testPass = 0;
	}
	printf("No state changes for other players\n");

	// check if any tests failed
	if (testPass == 1){
		printf("PASS: Village worked correctly\n\n\n\n");
	}
	else{
		printf("FAIL: Village did not work correctly\n\n\n\n");
	}

	return 0;
}

int victoryKingdomCompare(struct gameState *newState, struct gameState *oldState) {
	int i;

	for(i = 0; i < treasure_map + 1; i++){
		if(newState->supplyCount[i] != oldState->supplyCount[i]){
			return -1;
		}
	}

	for(i = 0; i < treasure_map + 1; i++){
		if(newState->embargoTokens[i] != oldState->embargoTokens[i]){
			return -1;
		}
	}
	return 0;
}

int otherPlayerCompare(struct gameState *newState, struct gameState *oldState) {
	int i, j;

	for(i = 0; i < newState->numPlayers; i++){
		if(i != newState->whoseTurn){
			if(newState->handCount[i] != oldState->handCount[i]){
				return -1;
			}
		}
	}

	for(i = 0; i < newState->numPlayers; i++){
		if(i != newState->whoseTurn){
			for(j = 0; j < newState->handCount[i]; j++){
				if(newState->hand[i][j] != oldState->hand[i][j]){
					return -1;
				}
			}
		}
	}

	for(i = 0; i < newState->numPlayers; i++){
		if(i != newState->whoseTurn){
			if(newState->deckCount[i] != oldState->deckCount[i]){
				return -1;
			}
		}
	}

	for(i = 0; i < newState->numPlayers; i++){
		if(i != newState->whoseTurn){
			for(j = 0; j < newState->deckCount[i]; j++){
				if(newState->deck[i][j] != oldState->deck[i][j]){
					return -1;
				}
			}
		}
	}

	for(i = 0; i < newState->numPlayers; i++){
		if(i != newState->whoseTurn){
			if(newState->discardCount[i] != oldState->discardCount[i]){
				return -1;
			}
		}
	}

	for(i = 0; i < newState->numPlayers; i++){
		if(i != newState->whoseTurn){
			for(j = 0; j < newState->discardCount[i]; j++){
				if(newState->discard[i][j] != oldState->discard[i][j]){
					return -1;
				}
			}
		}
	}
	return 0;
}
