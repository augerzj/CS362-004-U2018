#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include <stdlib.h>

int gameStateCompare(struct gameState *newState, struct gameState *oldState);

int main() {
	int gameStateUnchanged;
	int testPass = 0;
	int seed = 1000;
	int handPos, trashFlag;
	int numPlayers = 4;
	struct gameState G, initG;
	int k[10] = {adventurer, embargo, village, minion, mine, cutpurse,sea_hag, tribute, smithy, council_room};

	// initialize a game state and player cards
	initializeGame(numPlayers, k, seed, &initG);
	initG.hand[initG.whoseTurn][2] = smithy;
	handPos = 2;
	memcpy(&G, &initG, sizeof(struct gameState));

	printf("----------UNIT TEST #4 - TEST OF DISCARDCARD FUNCTION----------\n");

	printf("Testing DiscardCard on Trashed Card\n");
	testPass = 1;
	trashFlag = 1;

	discardCard(handPos, G.whoseTurn, &G, trashFlag);

	// check that card not in played pile
	if(G.playedCardCount == 0){
		printf("PASS: ");
	}
	else{
		printf("FAIL: ");
		testPass = 0;
	}
	printf("Discarded card not in played pile\n");

	// check that card isn't in hand
	if(G.hand[G.whoseTurn][handPos] != smithy){
		printf("PASS: ");
	}
	else{
		printf("FAIL: ");
		testPass = 0;
	}
	printf("Discarded card not in hand\n");

	// check that hand count was decremented
	if(G.handCount[G.whoseTurn] == initG.handCount[initG.whoseTurn] - 1) {
		printf("PASS: ");
	}
	else{
		printf("FAIL: ");
		testPass = 0;
	}
	printf("-1 hand size after discarding (actual difference: %d)\n", G.handCount[G.whoseTurn] - initG.handCount[initG.whoseTurn]);

	// check that card wasn't put in discard or deck
	if(G.discardCount[G.whoseTurn] == initG.discardCount[initG.whoseTurn] &&
		G.deckCount[G.whoseTurn] == initG.deckCount[initG.whoseTurn]){
		printf("PASS: ");
	}
	else{
		printf("FAIL: ");
		testPass = 0;
	}
	printf("Discarded card not in discard pile or deck\n");

	// check that no other part of game state was altered
	gameStateUnchanged = gameStateCompare(&G, &initG);

	if (gameStateUnchanged == 0) {
		printf("PASS: ");
	}
	else {
		printf("FAIL: ");
		testPass = 0;
	}
	printf("Rest of game state remained the same\n");

	// check if any tests failed
	if (testPass == 1){
		printf("PASS: discardCard on trashed card worked\n\n");
	}
	else{
		printf("FAIL: discardCard on trashed card did not work\n\n");
	}

	memcpy(&G, &initG, sizeof(struct gameState));

	printf("Testing DiscardCard on Non-Trashed Card\n");
	testPass = 1;
	trashFlag = 0;

	discardCard(handPos, G.whoseTurn, &G, trashFlag);

	// check that card in played pile
	if(G.playedCards[G.playedCardCount - 1] == smithy){
		printf("PASS: ");
	}
	else{
		printf("FAIL: ");
		testPass = 0;
	}
	printf("Discarded card on top of played pile\n");

	// check that card isn't in hand
	if(G.hand[G.whoseTurn][handPos] != smithy){
		printf("PASS: ");
	}
	else{
		printf("FAIL: ");
		testPass = 0;
	}
	printf("Discarded card not in hand\n");

	// check that hand count was decremented
	if(G.handCount[G.whoseTurn] == initG.handCount[initG.whoseTurn] - 1) {
		printf("PASS: ");
	}
	else{
		printf("FAIL: ");
		testPass = 0;
	}
	printf("-1 hand size after discarding (actual difference: %d)\n", G.handCount[G.whoseTurn] - initG.handCount[initG.whoseTurn]);

	// check that card isn't in deck or discard
	if(G.discardCount[G.whoseTurn] == initG.discardCount[initG.whoseTurn] &&
		G.deckCount[G.whoseTurn] == initG.deckCount[initG.whoseTurn]){
		printf("PASS: ");
	}
	else{
		printf("FAIL: ");
		testPass = 0;
	}
	printf("Discarded card not in discard pile or deck\n");

	// check that rest of game state the same
	gameStateUnchanged = gameStateCompare(&G, &initG);

	if (gameStateUnchanged == 0) {
		printf("PASS: ");
	}
	else {
		printf("FAIL: ");
		testPass = 0;
	}
	printf("Rest of game state remained the same\n");

	// check if any tests failed
	if (testPass == 1){
		printf("PASS: discardCard on non-trashed card worked\n\n\n\n");
	}
	else{
		printf("FAIL: discardCard on non-trashed card did not work\n\n\n\n");
	}

	return 0;
}

// checks game state for changes
int gameStateCompare(struct gameState *newState, struct gameState *oldState) {
	int i, j;

	if(newState->numPlayers != oldState->numPlayers){
		return 1;
	}

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

	if(newState->outpostPlayed != oldState->outpostPlayed){
		return 1;
	}

	if(newState->outpostTurn != oldState->outpostTurn){
		return 1;
	}

	if(newState->whoseTurn != oldState->whoseTurn){
		return 1;
	}

	if(newState->phase != oldState->phase){
		return 1;
	}

	if(newState->numActions != oldState->numActions){
		return 1;
	}

	if(newState->coins != oldState->coins){
		return 1;
	}

	if(newState->numBuys != oldState->numBuys){
		return 1;
	}

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