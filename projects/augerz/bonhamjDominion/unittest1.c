#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include <stdlib.h>

int cmpNums(const void* a, const void* b) {
  if (*(int*)a > *(int*)b)
    return 1;
  if (*(int*)a < *(int*)b)
    return -1;
  return 0;
}

int gameStateCompare(struct gameState *newState, struct gameState *oldState);

int main() {
	int i, j, deckShuffled, sameDeckCards, gameStateUnchanged;
	int testPass = 0;
	int seed = 1000;
	int numPlayers = 4;
	struct gameState G, initG, prevG;
	int k[10] = {adventurer, embargo, village, minion, mine, cutpurse,sea_hag, tribute, smithy, council_room};

	// initialize a game state and player cards
	initializeGame(numPlayers, k, seed, &initG);

	printf("----------UNIT TEST #1 - TEST OF SHUFFLE FUNCTION----------\n");

	// shuffle empty deck
	printf("Testing Shuffle of Empty Deck\n");

	memcpy(&G, &initG, sizeof(struct gameState));

	// empty hand
	for (i = 0; i < 5; i++) {
		G.deck[0][i] = 0;
		G.deckCount[0]--;
	}

	memcpy(&prevG, &G, sizeof(struct gameState));

	// shuffle deck
	deckShuffled = shuffle(0, &G);
	testPass = 1;

	// ensure returned error val
	if (deckShuffled == -1) {
		printf("PASS: ");
	}
	else {
		printf("FAIL: ");
		testPass = 0;
	}
	printf("Return value is -1 (actual return val: %d)\n", deckShuffled);

	// make sure deck count didn't increase
	if (G.deckCount[0] == 0) {
		printf("PASS: ");
	}
	else {
		printf("FAIL: ");
		testPass = 0;
	}
	printf("Player deck count is 0 (actual deck count: %d)\n", G.deckCount[0]);

	// make sure game state is unchanged
	gameStateUnchanged = gameStateCompare(&G, &prevG);

	// check that other player decks weren't shuffled
	for(i = 1; i < numPlayers; i++){
		if(G.deckCount[i] != prevG.deckCount[i]){
			gameStateUnchanged = 1;
		}
	}

	for(i = 1; i < numPlayers; i++){
		for(j = 0; j < G.deckCount[i]; j++){
			if(G.deck[i][j] != prevG.deck[i][j]){
				gameStateUnchanged = 1;
			}
		}
	}

	if (gameStateUnchanged == 0) {
		printf("PASS: ");
	}
	else {
		printf("FAIL: ");
		testPass = 0;
	}
	printf("Rest of game state remained the same\n");

	// print if all tests passed
	if (testPass == 1){
		printf("PASS: empty deck shuffle\n\n");
	}
	else{
		printf("FAIL: empty deck shuffle\n\n");
	}


	// shuffle starting deck of 5 cards
	printf("Testing Shuffle of Starting Deck\n");

	memcpy(&G, &initG, sizeof(struct gameState));

	// shuffle deck
	deckShuffled = shuffle(0, &G);
	testPass = 1;

	// ensure return value valid
	if (deckShuffled == 0) {
		printf("PASS: ");
	}
	else {
		printf("FAIL: ");
		testPass = 0;
	}
	printf("Return value is 0 (actual return val: %d)\n", deckShuffled);

	// make sure same number of cards in deck
	if (G.deckCount[0] == initG.deckCount[0]) {
		printf("PASS: ");
	}
	else {
		printf("FAIL: ");
		testPass = 0;
	}
	printf("Player deck count is 5 (actual deck count: %d)\n", G.deckCount[0]);

	// make sure cards in different order
	deckShuffled = 0;
	for(i = 0; i < G.deckCount[0]; i++){
		if(G.deck[i] != initG.deck[i]){
			deckShuffled = 1;
		}
	}

	if (deckShuffled == 1) {
		printf("PASS: ");
	}
	else {
		printf("FAIL: ");
		testPass = 0;
	}
	printf("Player deck is in a different order\n");


	// make sure all cards are the same by sorting and comparing to previous state
	sameDeckCards = 1;
	qsort ((void*)(G.deck[0]), G.deckCount[0], sizeof(int), cmpNums);
	qsort ((void*)(initG.deck[0]), initG.deckCount[0], sizeof(int), cmpNums);
	for(i = 0; i < G.deckCount[0]; i++){
		if(G.deck[0][i] != initG.deck[0][i]){
			sameDeckCards = 0;
		}
	}

	if (sameDeckCards == 1) {
		printf("PASS: ");
	}
	else {
		printf("FAIL: ");
		testPass = 0;
	}
	printf("Player deck contains the same cards\n");

	// ensure the rest of the game state hasn't changed
	gameStateUnchanged = gameStateCompare(&G, &initG);

	// make sure other player decks untouched
	for(i = 1; i < numPlayers; i++){
		if(G.deckCount[i] != initG.deckCount[i]){
			gameStateUnchanged = 1;
		}
	}

	for(i = 1; i < numPlayers; i++){
		for(j = 0; j < G.deckCount[i]; j++){
			if(G.deck[i][j] != initG.deck[i][j]){
				gameStateUnchanged = 1;
			}
		}
	}

	if (gameStateUnchanged == 0) {
		printf("PASS: ");
	}
	else {
		printf("FAIL: ");
		testPass = 0;
	}
	printf("Rest of game state remained the same\n");

	// print if any of the checks failed
	if (testPass == 1){
		printf("PASS: starting deck shuffle\n\n");
	}
	else{
		printf("FAIL: starting deck shuffle\n\n");
	}


	// shuffle max player deck
	printf("Testing Shuffle of Full Deck\n");

	// make deck of 500 random cards
	initG.deckCount[3] = 0;
	for(i = 0; i < MAX_HAND; i++){
		initG.deck[3][i] = i % 24;
		initG.deckCount[3]++;
	}

	memcpy(&G, &initG, sizeof(struct gameState));

	// shuffle deck
	deckShuffled = shuffle(3, &G);
	testPass = 1;

	// check return value
	if (deckShuffled == 0) {
		printf("PASS: ");
	}
	else {
		printf("FAIL: ");
		testPass = 0;
	}
	printf("Return value is 0 (actual return val: %d)\n", deckShuffled);

	// check that the same amount of cards are in the deck
	if (G.deckCount[3] == initG.deckCount[3]) {
		printf("PASS: ");
	}
	else {
		printf("FAIL: ");
		testPass = 0;
	}
	printf("Player deck count is 500 (actual deck count: %d)\n", G.deckCount[3]);

	// check that the cards aren't in the same order
	deckShuffled = 0;
	for(i = 0; i < G.deckCount[3]; i++){
		if(G.deck[i] != initG.deck[i]){
			deckShuffled = 1;
		}
	}

	if (deckShuffled == 1) {
		printf("PASS: ");
	}
	else {
		printf("FAIL: ");
		testPass = 0;
	}
	printf("Player deck is in a different order\n");

	// check that all the same cards are in the deck
	sameDeckCards = 1;
	qsort ((void*)(G.deck[3]), G.deckCount[3], sizeof(int), cmpNums);
	qsort ((void*)(initG.deck[3]), initG.deckCount[3], sizeof(int), cmpNums);
	for(i = 0; i < G.deckCount[3]; i++){
		if(G.deck[3][i] != initG.deck[3][i]){
			sameDeckCards = 0;
		}
	}

	if (sameDeckCards == 1) {
		printf("PASS: ");
	}
	else {
		printf("FAIL: ");
		testPass = 0;
	}
	printf("Player deck contains the same cards\n");

	// make sure rest of game unchanged
	gameStateUnchanged = gameStateCompare(&G, &initG);

	for(i = 0; i < numPlayers - 1; i++){
		if(G.deckCount[i] != initG.deckCount[i]){
			printf("dcg: %d, dcpg: %d\n", G.deckCount[i], initG.deckCount[i]);
			gameStateUnchanged = 1;
		}
	}

	for(i = 0; i < numPlayers - 1; i++){
		for(j = 0; j < G.deckCount[i]; j++){
			if(G.deck[i][j] != initG.deck[i][j]){
				printf("i: %d, j: %d\n", i, j);
				gameStateUnchanged = 1;
			}
		}
	}

	if (gameStateUnchanged == 0) {
		printf("PASS: ");
	}
	else {
		printf("FAIL: ");
		testPass = 0;
	}
	printf("Rest of game state remained the same\n");

	// print if any of the tests failed
	if (testPass == 1){
		printf("PASS: full deck shuffle\n\n\n\n");
	}
	else{
		printf("FAIL: full deck shuffle\n\n\n\n");
	}
}

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
		if(newState->handCount[i] != oldState->handCount[i]){
			return -1;
		}
	}

	for(i = 0; i < newState->numPlayers; i++){
		for(j = 0; j < newState->handCount[i]; j++){
			if(newState->hand[i][j] != oldState->hand[i][j]){
				return -1;
			}
		}
	}

	for(i = 0; i < newState->numPlayers; i++){
		if(newState->discardCount[i] != oldState->discardCount[i]){
			return -1;
		}
	}

	for(i = 0; i < newState->numPlayers; i++){
		for(j = 0; j < newState->discardCount[i]; j++){
			if(newState->discard[i][j] != oldState->discard[i][j]){
				return -1;
			}
		}
	}

	if(newState->playedCardCount != oldState->playedCardCount){
		return 1;
	}

	for(i = 0; i < newState->playedCardCount; i++){
		if(newState->playedCards[i] != oldState->playedCards[i]){
			return -1;
		}
	}
	return 0;
}