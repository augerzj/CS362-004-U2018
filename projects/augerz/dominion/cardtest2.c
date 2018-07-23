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
	int i, gameStateUnchanged;
	int testPass = 0;
	int seed = 1000;
	int bonus = 0;
	int numPlayers = 4;
	struct gameState G, initG;
	int k[10] = {adventurer, embargo, village, minion, mine, cutpurse,sea_hag, tribute, smithy, council_room};

	// initialize a game state and player cards
	initializeGame(numPlayers, k, seed, &initG);
	initG.hand[initG.whoseTurn][2] = adventurer;
	initG.deck[initG.whoseTurn][0] = copper;
	initG.deck[initG.whoseTurn][1] = smithy;
	initG.deck[initG.whoseTurn][2] = silver;
	initG.deck[initG.whoseTurn][3] = estate;
	initG.deck[initG.whoseTurn][4] = gold;

	memcpy(&G, &initG, sizeof(struct gameState));

	printf("----------CARD TEST #2 - TEST OF ADVENTURER----------\n");

	cardEffect(adventurer, 0, 0, 0, &G, 2, &bonus);
	testPass = 1;

	// check that hand count has increased
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
		printf("PASS: Smithy worked correctly\n\n\n\n");
	}
	else{
		printf("FAIL: Smithy did not work correctly\n\n\n\n");
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