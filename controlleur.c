#include "controlleur.h"

//Tableaux représentant les mesures des capteurs
float temperature[3];
float oxydo[3];
float ph[3];


// Tableaux spécifiant si capteur[i] est une mesure valide pour chaque type de capteur
bool temperatureValide[3];
bool oxydoValide[3];
bool phValide[3];

//Tableau des 10 dernières moyennes pour chaque type de capteur
float lastTemperature[10] = {10.0,9.6,10.2,10.3,10.5,9.8,10.1,10.8,9.3,10.0};
float lastPh[10] = {7.5,7.3,6.9,6.8,7.1,7.1,7.2,7.3,7.0,7.2};
float lastOxydo[10] = {5.3,4.6,5.1,5.0,5.2,5.3,5.0,5.7,5.0,4.9};


bool msg_sent = false;
bool notif_sent = false;


float float_rand( float min, float max )
{
    float scale = rand() / (float) RAND_MAX; /* [0, 1.0] */
    return min + scale * ( max - min );      /* [min, max] */
}


//Input capteur
void inputTroisCapteurs(){


    for(int i=0; i<3; i++){
        temperature[i] = float_rand(9.0,11.0);
        //printf("%f\n",temperature[i]);
    }

    for(int i=0; i<3; i++){
        oxydo[i] = float_rand(4.0,6.0);
        //printf("%f\n",oxydo[i]);

    }


    for(int i=0; i<3; i++){
        ph[i] = float_rand(6.0,8.0);
        //printf("%f\n",ph[i]);
    }

}

void inputTemperature(){

    for(int i=0; i<3; i++){
        printf("Température %d\n",i);
        scanf("%f\n",&temperature[i]);
    }
}

void inputOxydo(){

    for(int i=0; i<3; i++){
        printf("Oxydo %d\n",i);
        scanf("%f\n",&oxydo[i]);
    }
}

void inputPh(){

    for(int i=0; i<3; i++){
        printf("Ph %d\n",i);
        scanf("%f\n",&ph[i]);
    }
}

//Moyenne de deux valeurs
float moyenneDeux(float a, float b){

    return (a+b)/2.0;
}

// True si la valeur a est eloignée de 30% de la valeur moyenne

bool ecartVal(float a, float moyenne){

    float moyenneSup, moyenneInf;

    moyenneSup = moyenne + (0.3 * moyenne);
    moyenneInf = moyenne - (0.3 * moyenne);

    if(a < moyenneInf || a > moyenneSup ) return true;
    else return false;

}



//Remplit le tableau des mesures valides d'un type de capteur
void mesuresValides(float capteur[3], bool valide[3]){

    if(!ecartVal(capteur[0],moyenneDeux(capteur[1],capteur[2]))) valide[0] = true;
    else valide[0] = false;

    if(!ecartVal(capteur[1],moyenneDeux(capteur[0],capteur[2]))) valide[1] = true;
    else valide[1] = false;

    if(!ecartVal(capteur[2],moyenneDeux(capteur[0],capteur[1]))) valide[2] = true;
    else valide[2] = false;
    
}


int compteurMesuresValides(bool arrayValide[3]){

    int cnt=0;
    for(int i=0; i<3;i++){
        if(arrayValide[i]) cnt++;
    }
    return cnt;

}

//Calcule la moyenne des mesures valides d'un type de capteur
float moyenneMesuresValides(float capteur[3], bool valide[3]){

    float moyenne = 0;
    
    //Compteur mesures valides
    int compteurValide = 0;

    for(int i=0; i<3; i++){
        if(valide[i])
        {
            moyenne += capteur[i];
            compteurValide++;
        } 
    }
    moyenne /= compteurValide;

    return moyenne;
}

//Calcule moyenne d'un tableau de 10 float
float moyenneArray(float tab[10]){

    float moyenne=0.0;

    for(int i=0; i<10; i++) moyenne+= tab[i];

    return moyenne/= 10.0;

}



void send_msg(){

    msg_sent = true;    

}

void send_notif(){

    notif_sent = true;
}


//True si il y a detection d'un pic de pollution
bool detectionPollution(float moyenneT, float moyenneO, float moyenneP){

    bool pic;

    //Check si les 3 moyennes sont éloignées de 50 de la moyenne des 10 cycles précédents
    float lastMoyenneTemp = moyenneArray(lastTemperature);
    float lastMoyenneOxydo = moyenneArray(lastOxydo);
    float lastMoyennePh = moyenneArray(lastPh);

    if(moyenneT < (lastMoyenneTemp - 0.5*lastMoyenneTemp) || moyenneT > (lastMoyenneTemp + 0.5*lastMoyenneTemp) ||
    moyenneO < (lastMoyenneOxydo - 0.5*lastMoyenneOxydo) || moyenneO > (lastMoyenneOxydo + 0.5*lastMoyenneOxydo) ||
    moyenneT < (lastMoyennePh - 0.5*lastMoyennePh) || moyenneP > (lastMoyennePh + 0.5*lastMoyennePh))
    {
        pic=true;
        printf("*** PIC DE POLLUTION DETECTE ***\n");
        send_notif();
    }
    else {
        pic=false;
        printf("*** PAS DE PIC DE POLLUTION ***\n");
    }

    return pic;
}




/**
  * 1. SI 2 jeux de mesures consécutifs sont invalides, mode defaillance
  * 2. Calcul des moyennes
  * 3. Detection pollution
*/

void vote(){

    
    float moyenneTemperature;
    float moyenneOxydo;
    float moyennePh;

    mesuresValides(temperature, temperatureValide);
    mesuresValides(oxydo, oxydoValide);
    mesuresValides(ph, phValide);

    if(compteurMesuresValides(temperatureValide)==0 || compteurMesuresValides(oxydoValide)==0 || compteurMesuresValides(phValide)==0){
        printf("ERREUR - MESURES NON VALIDES : NOUVELLES MESURES");


        mesuresValides(temperature, temperatureValide);
        mesuresValides(oxydo, oxydoValide);
        mesuresValides(ph, phValide);

        if(compteurMesuresValides(temperatureValide)==0 || compteurMesuresValides(oxydoValide)==0 || compteurMesuresValides(phValide)==0){
            printf("ERREUR - DEUX FOIS MESURES NON VALIDES : MODE DEFAILLANCE\n");
            send_notif();
            exit(0);
        }
    }


    moyenneTemperature = moyenneMesuresValides(temperature,temperatureValide);
    moyenneOxydo = moyenneMesuresValides(oxydo,oxydoValide);
    moyennePh = moyenneMesuresValides(ph,phValide);

    printf("Moyenne Temp %f\n",moyenneTemperature);
    printf("Moyenne Oxydo %f\n",moyenneOxydo);
    printf("Moyenne ph %f\n",moyennePh);

    // Détection pollution puis décalage pour acceuillir les nouvelles valeurs

    detectionPollution(moyenneTemperature, moyenneOxydo, moyennePh);

    for(int i=1; i<9;i++){
        lastTemperature[i-1] = lastTemperature[i];
        lastOxydo[i-1] = lastOxydo[i];
        lastPh[i-1] = lastPh[i];
    }
    lastTemperature[9] = moyenneTemperature;
    lastOxydo[9] = moyenneOxydo;
    lastPh[9] = moyennePh;

    printf("**  10 dernières moyennes Temp: \n**");
    for(int i=0; i<10;i++){
        printf("%f, ", lastTemperature[i]);
    }
    printf("\n\n");

    send_msg();

}


void evalBool(bool testBool, bool attendu){
    printf("%d\n",testBool);
    if(testBool==attendu) printf(" ==== SUCCES\n");
    else printf("==== FAIL\n");
}


/********************************** TEST PRINTF *************************/

void tests_print(){


    bool testBool;

    // Tests exigence 3
    printf("******** Tests EXIG 3 (fonction ecartVal) *********\n");
    printf("\tTest 1: Inputs a=3, mean=10\n\t\t => Output : ");
    testBool= ecartVal (3,10);
    evalBool(testBool, true);

    printf("\tTest 1: Inputs a=15, mean=10\n\t\t => Output : ");
    testBool= ecartVal (15,10);
    evalBool(testBool, true);

    printf("\tTest 1: Inputs a=7, mean=10\n\t\t => Output : ");
    testBool= ecartVal (7,10);
    evalBool(testBool, false);

    printf("\tTest 1: Inputs a=6, mean=10\n\t\t => Output : ");
    testBool= ecartVal (6,10);
    evalBool(testBool, true);

    // Tests exigence 6

    printf("\n******** Tests EXIG 6 (fonction detectionPollution) *********\n");
    printf("\tTest 1: Inputs moyenneT=18, moyenneO=16, moyenneP=7\n\t\t => Output : ");
    testBool= detectionPollution(18,16,7);
    evalBool(testBool, true);


    printf("\n******** Tests EXIG 6 (fonction detectionPollution) *********\n");
    printf("\tTest 1: Inputs moyenneT=18, moyenneO=16, moyenneP=7\n");
    testBool= detectionPollution(10,5,7);
    printf("\t\t => Output : ");
    evalBool(testBool, false);



}

/*
int main(){

    printf("Démarrage système");
    inputTroisCapteurs();
    tests_print();




    return 0;
}
*/
