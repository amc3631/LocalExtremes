// Author: Michael Boisvert

#include "stdafx.h"
#include <iostream>
#include <cmath>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

bool notFound = 0;

float polyFunction(float xVal, int polyDegree, float polyArray[]) {
	float y = 0;
	for (int i = 0; i <= polyDegree; i++) {
		y += polyArray[i] * pow(xVal, polyDegree - i);
	}
	return y;
}

bool approxEqual(float yVal) {
	if ((-0.001 < yVal) && (yVal < 0.001)) {
		return 1;
	}
	else {
		return 0;
	}
}

float derivSegments(float xInit, float xFinal, int polyDegree, int divisions, float derivArray[]) {
	// find critical point of a polynomial function by incrementing x until sign of y value changes
	// once sign change found, call function recursively with smaller increments to approximate crtical point
	float xTest = xInit;
	float inc = 500;
	bool isPos = 0;
	bool isNeg = 0;
	float prevY = polyFunction(xTest, polyDegree-1, derivArray);
	if (approxEqual(prevY)) {
		return xTest;
	}
	else if (prevY < 0) {
		isNeg = 1;
	}
	else if (prevY > 0) {
		isPos = 1;
	}
	xTest += (inc / divisions);
	float nextY = polyFunction(xTest, polyDegree-1, derivArray);

	if ((isPos && nextY < 0) || (isNeg && nextY > 0)) {
		xTest = derivSegments(xInit, xTest, polyDegree, divisions * 10, derivArray);
		return xTest;
	}
	else if (xTest > xFinal) {
		notFound = 1;
		return 0;
	}

	while ((isPos && nextY > 0) || (isNeg && nextY < 0)) {
		if (xTest > xFinal) {
			notFound = 1;
			return 0;
		}
		prevY = nextY;
		xInit = xTest;
		xTest += (inc / divisions);
		nextY = polyFunction(xTest, polyDegree-1, derivArray);
	}

	xTest = derivSegments(xInit, xTest, polyDegree, divisions * 10, derivArray);
	return xTest;
}

int main() {

	// user input
	// catch invalid entries

	int polyDegree;

	cout << "Please enter degree of polynomial. Maximum degree is 6." << endl;
	cin >> polyDegree;

	if (!(polyDegree <= 6 && polyDegree >= 1)) {
		cin.clear();
		cin.ignore();
		while (true) {
			cout << "Please enter a valid number from 1 to 6 for the degree of the polynomial." << endl;
			cin >> polyDegree;
			if (polyDegree <= 6 && polyDegree >= 1) {
				break;
			}
			else {
				cin.clear();
				cin.ignore(1000000000000, '\n');
			}
		}
	}


	float* polyArray = new float[polyDegree + 1];
	float* derivArray = new float[polyDegree + 1];
	float xInit;
	float xFinal;

	cout << "Enter polynomial coefficients (Examples: 5, 0.0002, 1e-19)" << endl;
	for (int j = 0; j < polyDegree + 1; j++) {
		printf("Please enter coefficient of term with exponent of '%i'.\n", polyDegree - j);
		scanf_s("%e", &polyArray[j]);
	}

	cout << "Please enter a minimum x value for possible extrema" << endl;
	cin >> xInit;

	if (cin.bad()) {
		while (true) {
			cin.clear();
			cin.ignore();
			cout << "Please enter a valid number" << endl;
			cin >> xInit;
			if (!cin.bad()) { break; }
		}
	}

	cout << "Please enter a maximum x value for possible extrema" << endl;
	cin >> xFinal;

	if (cin.bad()) {
		while (true) {
			cin.clear();
			cin.ignore();
			cout << "Please enter a valid number" << endl;
			cin >> xFinal;
			if (!cin.bad()) { break; }
		}
	}

	// calculate derivative
	for (int k = 0; k < polyDegree + 1; k++) {
		derivArray[k] = polyArray[k] * (polyDegree - k);
	}
	
	float y;
	float yPlus;
	bool anyFound = 0;

	// find critical points for derivative
	float* critPoints = new float[polyDegree - 1];
	for (int i = 0; i < polyDegree - 1; i++) {
		critPoints[i] = 0;
	}

	for (int i = 0; i < polyDegree - 1; i++) {
		critPoints[i] = derivSegments(xInit, xFinal, polyDegree, 10, derivArray);
		xInit = critPoints[i] + 1;
		if (notFound == 0) {
			anyFound = 1;
			y = polyFunction(critPoints[i], polyDegree, polyArray);
			yPlus = polyFunction(critPoints[i]+1, polyDegree, polyArray);
			if (yPlus > y) {
				cout << "Local maximum: " << y << " at x = " << critPoints[i] << endl;
			}
			else {
				cout << "Local minimum: " << y << " at x = " << critPoints[i] << endl;
			}
		}
		notFound = 0;
	}

	if (anyFound == 0) {
		cout << "No extrema found" << endl;
	}

	return 0;

}