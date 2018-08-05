#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include <stdlib.h>
#include <time.h>

void randomizeGameState(struct gameState *G);
int testAdventurer(struct gameState *G, int adventurerPosition);
int victoryKingdomCompare(struct gameState *newState, struct gameState *oldState);
int otherPlayerCompare(struct gameState *newState, struct gameState *oldState);

int main() {
	int tries;
	int currTestResult;
	int testPass = 1;
	int adventurerPosition;
	struct gameState G;
	srand(time(NULL));

	printf("----------RANDOM CARD TEST #3 - TEST OF ADVENTURER----------\n");

	// run random tests 1000000 times
	for (tries = 0; tries < 1000000; tries++) {
		printf("Test %d\n", tries + 1);

		randomizeGameState(&G);

		// set position of village in hand
		adventurerPosition = rand() % G.handCount[G.whoseTurn];
		G.hand[G.whoseTurn][adventurerPosition] = adventurer;

		currTestResult = testAdventurer(&G, adventurerPosition);
		if(currTestResult != 1) {
			testPass = 0;
		}
	}

	if(testPass == 0) {
		printf("ADVENTURER DID NOT PASS ALL TESTS\n\n\n");
	}
	else {
		printf("ADVENTURER PASSED ALL TESTS\n\n\n");
	}

	return 0;
}

void randomizeGameState(struct gameState *G){
	int i, j;

	// individually set each member to a random value
	// in the valid range
	G->numPlayers = rand() % (MAX_PLAYERS - 1) + 2; // 2 - 4 players
	for(i = 0; i < treasure_map + 1; i++){
		G->supplyCount[i] = rand() % 50;
		G->embargoTokens[i] = rand() % 50;
	}
	G->outpostPlayed = rand() % 2;
	G->outpostTurn = rand() % G->numPlayers;
	G->whoseTurn = rand() % G->numPlayers;
	G->phase = rand() % 2;
	G->numActions = rand() % 500;
	G->coins = rand() % 100000;
	G->numBuys = rand() % 500;
	for(i = 0; i < G->numPlayers; i++){
		G->handCount[i] = rand() % MAX_HAND + 1;

		for(j = 0; j < G->handCount[i]; j++) {
			G->hand[i][j] = rand() % 27; // 27 cards in dominion
		}
	}
	for(i = 0; i < G->numPlayers; i++){
		G->deckCount[i] = rand() % MAX_DECK + 1;

		for(j = 0; j < G->deckCount[i]; j++) {
			G->deck[i][j] = rand() % 27;
		}
	}
	for(i = 0; i < G->numPlayers; i++){
		G->discardCount[i] = rand() % MAX_DECK + 1;

		for(j = 0; j < G->discardCount[i]; j++) {
			G->discard[i][j] = rand() % 27;
		}
	}
	G->playedCardCount = rand() % (MAX_DECK - 1);
	for(i = 0; i < G->playedCardCount; i++){
		G->playedCards[i] = rand() % 27;
	}
}

int testAdventurer(struct gameState *G_point, int adventurerPosition) {
	int testPass = 1;
	int bonus = rand();
	int gameStateUnchanged;
	int i;

	struct gameState G, initG;
	memcpy(&G, G_point, sizeof(struct gameState));
	memcpy(&initG, &G, sizeof(struct gameState));

	cardEffect(adventurer, rand(), rand(), rand(), &G, adventurerPosition, &bonus);

	if(G.handCount[G.whoseTurn] == initG.handCount[initG.whoseTurn] + 1) {
		printf("PASS: ");
	}
	else{
		printf("FAIL: ");
		testPass = 0;
	}
	printf("Hand size increased by 1 b/c Adventurer played (actual increase: %d)\n", G.handCount[G.whoseTurn] - initG.handCount[initG.whoseTurn]);

	// check that adventurer has been played
	if(G.playedCards[G.playedCardCount - 1] == adventurer) {
		printf("PASS: ");
	}
	else{
		printf("FAIL: ");
		testPass = 0;
	}
	printf("Adventurer is on top of played pile\n");

	// check that treasure cards in hand has increased by 2
	int treasureCount = 0;
	for(i = 0; i < G.handCount[G.whoseTurn]; i++){
		int currCard = G.hand[G.whoseTurn][i];
		if(currCard == copper || currCard == silver || currCard == gold){
			treasureCount++;
		}
	}

	for(i = 0; i < initG.handCount[initG.whoseTurn]; i++){
		int currCard = initG.hand[initG.whoseTurn][i];
		if(currCard == copper || currCard == silver || currCard == gold){
			treasureCount--;
		}
	}

	if(treasureCount == 2) {
		printf("PASS: ");
	}
	else{
		printf("FAIL: ");
		testPass = 0;
	}
	printf("Two treasure were added to hand (actual num added: %d)\n", treasureCount);

	// check that total cards for player has not changed
	if(G.deckCount[G.whoseTurn] + G.handCount[G.whoseTurn] + G.discardCount[G.whoseTurn] ==
		initG.deckCount[initG.whoseTurn] + initG.handCount[initG.whoseTurn] + initG.discardCount[initG.whoseTurn]) {
		printf("PASS: ");
	}
	else{
		printf("FAIL: ");
		testPass = 0;
	}
	printf("There are the same total num cards between hand, deck, and discard\n");

	// check that deck has been drawn from
	if(G.deckCount[G.whoseTurn] <= initG.deckCount[initG.whoseTurn] - 2) {
		printf("PASS: ");
	}
	else{
		printf("FAIL: ");
		testPass = 0;
	}
	printf("Cards were drawn from the player's deck\n");

	// check that cards have been added to discard
	if(G.discardCount[G.whoseTurn] >= initG.discardCount[initG.whoseTurn]) {
		printf("PASS: ");
	}
	else{
		printf("FAIL: ");
		testPass = 0;
	}
	printf("Discarded cards were put in the player's discard pile\n");

	// check that victory and kingdom piles unchanged
	gameStateUnchanged = victoryKingdomCompare(&G, &initG);
	if(gameStateUnchanged == 0){
		printf("PASS: ");
	}
	else{
		printf("FAIL: ");
		testPass = 0;
	}
	printf("Victory and Kingdom card piles not changed\n");

	// check that other players unaffected
	gameStateUnchanged = otherPlayerCompare(&G, &initG);
	if(gameStateUnchanged == 0){
		printf("PASS: ");
	}
	else{
		printf("FAIL: ");
		testPass = 0;
	}
	printf("No state changes for other players\n");

	// check if all tests passed
	if (testPass == 1){
		printf("PASS: Adventurer worked correctly\n\n\n\n");
	}
	else{
		printf("FAIL: Adventurer did not work correctly\n\n\n\n");
	}

	return testPass;
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