#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include <stdlib.h>
#include <limits.h>

int main() {
	int a, b, result;
	int testPass = 1;
	void *pa, *pb;

	printf("----------UNIT TEST #3 - TEST OF COMPARE FUNCTION----------\n");
	// a > b
	a = 5;
	pa = &a;
	b = 0;
	pb = &b;
	result = compare(pa, pb);

	if(result == 1){
		printf("PASS: ");
	}
	else{
		printf("FAIL: ");
		testPass = 0;
	}
	printf("a should be greater than b (a: %d, b: %d)\n", a, b);

	// a < b
	a = -5;
	pa = &a;
	b = 0;
	pb = &b;
	result = compare(pa, pb);

	if(result == -1){
		printf("PASS: ");
	}
	else{
		printf("FAIL: ");
		testPass = 0;
	}
	printf("a should be less than b (a: %d, b: %d)\n", a, b);

	// a == b
	a = 0;
	pa = &a;
	b = 0;
	pb = &b;
	result = compare(pa, pb);

	if(result == 0){
		printf("PASS: ");
	}
	else{
		printf("FAIL: ");
		testPass = 0;
	}
	printf("a should be equal to b (a: %d, b: %d)\n", a, b);

	// a = INT_MIN
	a = INT_MIN;
	pa = &a;
	b = 0;
	pb = &b;
	result = compare(pa, pb);

	if(result == -1){
		printf("PASS: ");
	}
	else{
		printf("FAIL: ");
		testPass = 0;
	}
	printf("a should be less than b (a: %d, b: %d)\n", a, b);

	// a = INT_MAX
	a = INT_MAX;
	pa = &a;
	b = 0;
	pb = &b;
	result = compare(pa, pb);

	if(result == 1){
		printf("PASS: ");
	}
	else{
		printf("FAIL: ");
		testPass = 0;
	}
	printf("a should be greater than b (a: %d, b: %d)\n", a, b);

	// b = INT_MIN
	a = 0;
	pa = &a;
	b = INT_MIN;
	pb = &b;
	result = compare(pa, pb);

	if(result == 1){
		printf("PASS: ");
	}
	else{
		printf("FAIL: ");
		testPass = 0;
	}
	printf("a should be greater than b (a: %d, b: %d)\n", a, b);

	// b = INT_MAX
	a = 0;
	pa = &a;
	b = INT_MAX;
	pb = &b;
	result = compare(pa, pb);

	if(result == -1){
		printf("PASS: ");
	}
	else{
		printf("FAIL: ");
		testPass = 0;
	}
	printf("a should be less than b (a: %d, b: %d)\n", a, b);

	// INT_MIN cmp
	a = INT_MIN;
	pa = &a;
	b = INT_MIN;
	pb = &b;
	result = compare(pa, pb);

	if(result == 0){
		printf("PASS: ");
	}
	else{
		printf("FAIL: ");
		testPass = 0;
	}
	printf("a should be equal to b (a: %d, b: %d)\n", a, b);

	// INT_MAX cmp
	a = INT_MAX;
	pa = &a;
	b = INT_MAX;
	pb = &b;
	result = compare(pa, pb);

	if(result == 0){
		printf("PASS: ");
	}
	else{
		printf("FAIL: ");
		testPass = 0;
	}
	printf("a should be equal to b (a: %d, b: %d)\n", a, b);

	// check that all tests passed
	if (testPass == 1){
		printf("PASS: all comparison tests worked\n\n\n\n");
	}
	else{
		printf("FAIL: all comparison tests did not work\n\n\n\n");
	}
	return 0;
}