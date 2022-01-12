#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

bool msg_sent;
bool notif_sent;


void inputTroisCapteurs();
void inputTemperature();
void inputOxydo();
void inputPh();
float moyenneDeux(float a, float b);
bool ecartVal(float a, float moyenne);
void mesuresValides(float capteur[3], bool valide[3]);
float moyenneMesuresValides(float capteur[3], bool valide[3]);
float moyenneArray(float tab[10]);
bool detectionPollution(float moyenneT, float moyenneO, float moyenneP);
void vote();
void evalBool(bool testBool, bool attendu);
void tests_print();
