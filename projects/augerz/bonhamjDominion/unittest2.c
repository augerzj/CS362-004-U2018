#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include <stdlib.h>

int gameStateCompare(struct gameState *newState, struct gameState *oldState);

int main() {
	int i, coinsUpdated, gameStateUnchanged;
	int testPass = 0;
	int seed = 1000;
	int numPlayers = 4;
	struct gameState G, initG;
	int k[10] = {adventurer, embargo, village, minion, mine, cutpurse,sea_hag, tribute, smithy, council_room};

	// initialize a game state and player cards
	initializeGame(numPlayers, k, seed, &initG);
	// put treasure in deck and discard to ensure they aren't counted
	for(i = 0; i < numPlayers; i++){
		initG.deckCount[i] = 3;
		initG.deck[i][0] = copper;
		initG.deck[i][1] = silver;
		initG.deck[i][2] = gold;
		initG.discardCount[i] = 3;
		initG.discard[i][0] = copper;
		initG.discard[i][1] = silver;
		initG.discard[i][2] = gold;
	}

	// remove any treasure from hand
	for (i = 0; i < initG.handCount[initG.whoseTurn]; i++){
		initG.hand[initG.whoseTurn][i] = estate;
	}

	initG.coins = -1;

	printf("----------UNIT TEST #2 - TEST OF UPDATECOINS FUNCTION----------\n");

	// NO COINS TEST
	printf("Testing Hand with No Coins\n");

	testPass = 1;
	memcpy(&G, &initG, sizeof(struct gameState));

	coinsUpdated = updateCoins(G.whoseTurn, &G, 0);

	// make sure function returned successfully
	if (coinsUpdated == 0) {
		printf("PASS: ");
	}
	else {
		printf("FAIL: ");
		testPass = 0;
	}
	printf("Return value is 0 (actual return val: %d)\n", coinsUpdated);

	// make sure coin total is 0
	if (G.coins == 0) {
		printf("PASS: ");
	}
	else {
		printf("FAIL: ");
		testPass = 0;
	}
	printf("Coin total is 0 (actual coin total: %d)\n", G.coins);

	// make sure rest of game state hasn't been changed
	gameStateUnchanged = gameStateCompare(&initG, &G);

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
		printf("PASS: hand with no coins\n\n");
	}
	else{
		printf("FAIL: hand with no coins\n\n");
	}


	// COPPER TEST
	printf("Testing Hand with Copper Coins\n");

	testPass = 1;

	initG.hand[initG.whoseTurn][0] = copper;
	memcpy(&G, &initG, sizeof(struct gameState));

	coinsUpdated = updateCoins(G.whoseTurn, &G, 0);

	// make sure coin total correct
	if (G.coins == 1) {
		printf("PASS: ");
	}
	else {
		printf("FAIL: ");
		testPass = 0;
	}
	printf("1 copper in hand: Coin total is 1 (actual coin total: %d)\n", G.coins);

	// make sure function returned successfully
	if (coinsUpdated == 0) {
		printf("PASS: ");
	}
	else {
		printf("FAIL: ");
		testPass = 0;
	}
	printf("Return value is 0 (actual return val: %d)\n", coinsUpdated);

	initG.hand[initG.whoseTurn][1] = copper;
	initG.hand[initG.whoseTurn][initG.handCount[initG.whoseTurn] - 1] = copper;
	memcpy(&G, &initG, sizeof(struct gameState));

	coinsUpdated = updateCoins(G.whoseTurn, &G, 0);

	// make sure coin total correct
	if (G.coins == 3) {
		printf("PASS: ");
	}
	else {
		printf("FAIL: ");
		testPass = 0;
	}
	printf("3 copper in hand: Coin total is 3 (actual coin total: %d)\n", G.coins);

	// make sure function returned successfully
	if (coinsUpdated == 0) {
		printf("PASS: ");
	}
	else {
		printf("FAIL: ");
		testPass = 0;
	}
	printf("Return value is 0 (actual return val: %d)\n", coinsUpdated);

	// make sure rest of game state hasn't been changed
	gameStateUnchanged = gameStateCompare(&initG, &G);

	if (gameStateUnchanged == 0) {
		printf("PASS: ");
	}
	else {
		printf("FAIL: ");
		testPass = 0;
	}
	printf("Rest of game state remained the same\n");

	if (testPass == 1){
		printf("PASS: hand with copper coins\n\n");
	}
	else{
		printf("FAIL: hand with copper coins\n\n");
	}


	// SILVER TEST
	printf("Testing Hand with Silver Coins\n");

	testPass = 1;

	initG.hand[initG.whoseTurn][0] = silver;
	initG.hand[initG.whoseTurn][1] = estate;
	initG.hand[initG.whoseTurn][initG.handCount[initG.whoseTurn] - 1] = estate;
	memcpy(&G, &initG, sizeof(struct gameState));

	coinsUpdated = updateCoins(G.whoseTurn, &G, 0);

	// make sure coin total correct
	if (G.coins == 2) {
		printf("PASS: ");
	}
	else {
		printf("FAIL: ");
		testPass = 0;
	}
	printf("1 silver in hand: Coin total is 2 (actual coin total: %d)\n", G.coins);

	// make sure function returned successfully
	if (coinsUpdated == 0) {
		printf("PASS: ");
	}
	else {
		printf("FAIL: ");
		testPass = 0;
	}
	printf("Return value is 0 (actual return val: %d)\n", coinsUpdated);

	initG.hand[initG.whoseTurn][1] = silver;
	initG.hand[initG.whoseTurn][initG.handCount[initG.whoseTurn] - 1] = silver;
	memcpy(&G, &initG, sizeof(struct gameState));

	coinsUpdated = updateCoins(G.whoseTurn, &G, 0);

	// make sure coin total correct
	if (G.coins == 6) {
		printf("PASS: ");
	}
	else {
		printf("FAIL: ");
		testPass = 0;
	}
	printf("3 silver in hand: Coin total is 6 (actual coin total: %d)\n", G.coins);

	// make sure function returned successfully
	if (coinsUpdated == 0) {
		printf("PASS: ");
	}
	else {
		printf("FAIL: ");
		testPass = 0;
	}
	printf("Return value is 0 (actual return val: %d)\n", coinsUpdated);

	// make sure rest of game state hasn't been changed
	gameStateUnchanged = gameStateCompare(&initG, &G);

	if (gameStateUnchanged == 0) {
		printf("PASS: ");
	}
	else {
		printf("FAIL: ");
		testPass = 0;
	}
	printf("Rest of game state remained the same\n");

	if (testPass == 1){
		printf("PASS: hand with silver coins\n\n");
	}
	else{
		printf("FAIL: hand with silver coins\n\n");
	}


	// GOLD TEST
	printf("Testing Hand with Gold Coins\n");

	testPass = 1;

	initG.hand[initG.whoseTurn][0] = gold;
	initG.hand[initG.whoseTurn][1] = estate;
	initG.hand[initG.whoseTurn][initG.handCount[initG.whoseTurn] - 1] = estate;
	memcpy(&G, &initG, sizeof(struct gameState));

	coinsUpdated = updateCoins(G.whoseTurn, &G, 0);

	// make sure coin total correct
	if (G.coins == 3) {
		printf("PASS: ");
	}
	else {
		printf("FAIL: ");
		testPass = 0;
	}
	printf("1 gold in hand: Coin total is 3 (actual coin total: %d)\n", G.coins);

	// make sure function returned successfully
	if (coinsUpdated == 0) {
		printf("PASS: ");
	}
	else {
		printf("FAIL: ");
		testPass = 0;
	}
	printf("Return value is 0 (actual return val: %d)\n", coinsUpdated);

	initG.hand[initG.whoseTurn][1] = gold;
	initG.hand[initG.whoseTurn][initG.handCount[initG.whoseTurn] - 1] = gold;
	memcpy(&G, &initG, sizeof(struct gameState));

	coinsUpdated = updateCoins(G.whoseTurn, &G, 0);

	// make sure coin total correct
	if (G.coins == 9) {
		printf("PASS: ");
	}
	else {
		printf("FAIL: ");
		testPass = 0;
	}
	printf("3 gold in hand: Coin total is 9 (actual coin total: %d)\n", G.coins);

	// make sure function returned successfully
	if (coinsUpdated == 0) {
		printf("PASS: ");
	}
	else {
		printf("FAIL: ");
		testPass = 0;
	}
	printf("Return value is 0 (actual return val: %d)\n", coinsUpdated);

	// make sure rest of game state hasn't been changed
	gameStateUnchanged = gameStateCompare(&initG, &G);

	if (gameStateUnchanged == 0) {
		printf("PASS: ");
	}
	else {
		printf("FAIL: ");
		testPass = 0;
	}
	printf("Rest of game state remained the same\n");

	if (testPass == 1){
		printf("PASS: hand with gold coins\n\n");
	}
	else{
		printf("FAIL: hand with gold coins\n\n");
	}


	// BONUS TEST
	printf("Testing Hand with Bonus Coins\n");

	testPass = 1;

	initG.hand[initG.whoseTurn][0] = estate;
	initG.hand[initG.whoseTurn][1] = estate;
	initG.hand[initG.whoseTurn][initG.handCount[initG.whoseTurn] - 1] = estate;
	memcpy(&G, &initG, sizeof(struct gameState));

	coinsUpdated = updateCoins(G.whoseTurn, &G, 5);

	// make sure coin total correct
	if (G.coins == 5) {
		printf("PASS: ");
	}
	else {
		printf("FAIL: ");
		testPass = 0;
	}
	printf("0 coins in hand, 5 bonus: Coin total is 5 (actual coin total: %d)\n", G.coins);

	// make sure function returned successfully
	if (coinsUpdated == 0) {
		printf("PASS: ");
	}
	else {
		printf("FAIL: ");
		testPass = 0;
	}
	printf("Return value is 0 (actual return val: %d)\n", coinsUpdated);

	initG.handCount[initG.whoseTurn] = 6;
	initG.hand[initG.whoseTurn][0] = gold;
	initG.hand[initG.whoseTurn][1] = silver;
	initG.hand[initG.whoseTurn][initG.handCount[initG.whoseTurn] - 1] = copper;
	memcpy(&G, &initG, sizeof(struct gameState));

	coinsUpdated = updateCoins(G.whoseTurn, &G, 5);

	// make sure coin total correct
	if (G.coins == 11) {
		printf("PASS: ");
	}
	else {
		printf("FAIL: ");
		testPass = 0;
	}
	printf("1 gold, silver, copper, 5 bonus: Coin total is 11 (actual coin total: %d)\n", G.coins);

	// make sure function returned successfully
	if (coinsUpdated == 0) {
		printf("PASS: ");
	}
	else {
		printf("FAIL: ");
		testPass = 0;
	}
	printf("Return value is 0 (actual return val: %d)\n", coinsUpdated);

	// make sure rest of game state hasn't been changed
	gameStateUnchanged = gameStateCompare(&initG, &G);

	if (gameStateUnchanged == 0) {
		printf("PASS: ");
	}
	else {
		printf("FAIL: ");
		testPass = 0;
	}
	printf("Rest of game state remained the same\n");

	if (testPass == 1){
		printf("PASS: hand with bonus coins\n\n");
	}
	else{
		printf("FAIL: hand with bonus coins\n\n");
	}


	// COMBO TEST
	printf("Testing Hand with Combo Coins\n");

	testPass = 1;

	initG.handCount[initG.whoseTurn] = 5;
	initG.hand[initG.whoseTurn][0] = gold;
	initG.hand[initG.whoseTurn][1] = silver;
	initG.hand[initG.whoseTurn][initG.handCount[initG.whoseTurn] - 1] = copper;

	memcpy(&G, &initG, sizeof(struct gameState));

	coinsUpdated = updateCoins(G.whoseTurn, &G, 0);

	// make sure coin total correct
	if (G.coins == 6) {
		printf("PASS: ");
	}
	else {
		printf("FAIL: ");
		testPass = 0;
	}
	printf("1 gold, silver, copper: Coin total is 6 (actual coin total: %d)\n", G.coins);

	// make sure function returned successfully
	if (coinsUpdated == 0) {
		printf("PASS: ");
	}
	else {
		printf("FAIL: ");
		testPass = 0;
	}
	printf("Return value is 0 (actual return val: %d)\n", coinsUpdated);

	initG.handCount[initG.whoseTurn] = 6;
	initG.hand[initG.whoseTurn][2] = gold;
	initG.hand[initG.whoseTurn][3] = silver;
	initG.hand[initG.whoseTurn][initG.handCount[initG.whoseTurn] - 1] = copper;

	memcpy(&G, &initG, sizeof(struct gameState));

	coinsUpdated = updateCoins(G.whoseTurn, &G, 0);

	// make sure coin total correct
	if (G.coins == 12) {
		printf("PASS: ");
	}
	else {
		printf("FAIL: ");
		testPass = 0;
	}
	printf("2 gold, silver, copper: Coin total is 12 (actual coin total: %d)\n", G.coins);

	// make sure function returned successfully
	if (coinsUpdated == 0) {
		printf("PASS: ");
	}
	else {
		printf("FAIL: ");
		testPass = 0;
	}
	printf("Return value is 0 (actual return val: %d)\n", coinsUpdated);

	// make sure rest of game state hasn't been changed
	gameStateUnchanged = gameStateCompare(&initG, &G);

	if (gameStateUnchanged == 0) {
		printf("PASS: ");
	}
	else {
		printf("FAIL: ");
		testPass = 0;
	}
	printf("Rest of game state remained the same\n");

	if (testPass == 1){
		printf("PASS: hand with combo coins\n\n\n\n");
	}
	else{
		printf("FAIL: hand with combo coins\n\n\n\n");
	}


	return 0;
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
		if(newState->deckCount[i] != oldState->deckCount[i]){
			return -1;
		}
	}

	for(i = 0; i < newState->numPlayers; i++){
		for(j = 0; j < newState->deckCount[i]; j++){
			if(newState->deck[i][j] != oldState->deck[i][j]){
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