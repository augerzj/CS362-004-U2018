#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include <stdlib.h>

int victoryKingdomCompare(struct gameState *newState, struct gameState *oldState);

int main() {
	int i, gameStateUnchanged, othersDrewOne;
	int testPass = 0;
	int seed = 1000;
	int bonus = 0;
	int numPlayers = 4;
	struct gameState G, initG;
	int k[10] = {adventurer, embargo, village, minion, mine, cutpurse,sea_hag, tribute, smithy, council_room};

	// initialize a game state and player cards
	initializeGame(numPlayers, k, seed, &initG);
	initG.hand[initG.whoseTurn][2] = council_room;
	memcpy(&G, &initG, sizeof(struct gameState));

	printf("----------CARD TEST #3 - TEST OF COUNCIL ROOM----------\n");

	cardEffect(council_room, 0, 0, 0, &G, 2, &bonus);
	testPass = 1;

	// check that hand count increased
	if(G.handCount[G.whoseTurn] == initG.handCount[initG.whoseTurn] + 3) {
		printf("PASS: ");
	}
	else{
		printf("FAIL: ");
		testPass = 0;
	}
	printf("3 cards were drawn into hand b/c played council_room (actual number drawn: %d)\n", G.handCount[G.whoseTurn] - initG.handCount[initG.whoseTurn]);

	// check that deck has been drawn from
	if(G.deckCount[G.whoseTurn] == initG.deckCount[initG.whoseTurn] - 4) {
		printf("PASS: ");
	}
	else{
		printf("FAIL: ");
		testPass = 0;
	}
	printf("4 cards were drawn from current players deck (actual number taken from deck: %d)\n", initG.deckCount[initG.whoseTurn] - G.deckCount[G.whoseTurn]);

	// check that council room has been played
	if(G.playedCards[G.playedCardCount - 1] == council_room) {
		printf("PASS: ");
	}
	else{
		printf("FAIL: ");
		testPass = 0;
	}
	printf("Council Room is on top of played pile\n");

	// check that buys has increased
	if(G.numBuys == initG.numBuys + 1) {
		printf("PASS: ");
	}
	else{
		printf("FAIL: ");
		testPass = 0;
	}
	printf("Number of buys increased by 1 (actual increase: %d)\n", G.numBuys - initG.numBuys);

	// check that other players drew card
	othersDrewOne = 1;
	for(i = 0; i < G.numPlayers; i++){
		if(i != G.whoseTurn){
			if(G.handCount[i] != initG.handCount[i] + 1){
				othersDrewOne = 0;
			}
		}
	}

	if(othersDrewOne == 1){
		printf("PASS: ");
	}
	else{
		printf("FAIL: ");
		testPass = 0;
	}
	printf("Each other player drew one card\n");

	// check that others drew card from own deck
	othersDrewOne = 1;
	for(i = 0; i < G.numPlayers; i++){
		if(i != G.whoseTurn){
			if(G.deckCount[i] != initG.deckCount[i] - 1){
				othersDrewOne = 0;
			}
		}
	}

	if(othersDrewOne == 1){
		printf("PASS: ");
	}
	else{
		printf("FAIL: ");
		testPass = 0;
	}
	printf("Each other player drew the card from thier deck\n");

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

	// check if all tests passed
	if (testPass == 1){
		printf("PASS: Council Room worked correctly\n\n\n\n");
	}
	else{
		printf("FAIL: Council Room did not work correctly\n\n\n\n");
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