// Implementatie van klasse VierOpEenRij

#include "vieropeenrij.h"
#include "standaard.h"
#include <fstream>   // voor inlezen van bord
#include <iostream>

//*************************************************************************

//Constructor voor een lege bord.
//Membervariabel 'breedte' & 'hoogte' zijn de afmetingen van het bord.
//Membervariabel 'aanBeurt' is 1 omdat speler 1 altijd begint.
VierOpEenRij::VierOpEenRij ()
{
  for (int i = hoogte -1; i >= 0; i--){ // MOOI OF NIET?
    for (int j = 0; j < breedte; j++){
      bord[j][i] = 0;
    }
  }
  aanBeurt = 1;
}  // default constructor

//*************************************************************************

//Constructor voor de experimenten. 
//De parameters 'nwBreedte' & 'nwHoogte' zijn voor de dimensies van het bord.
//Ze worden gelijk gesteld aan 'breedte' & 'hoogte'.
VierOpEenRij::VierOpEenRij (int nwBreedte, int nwHoogte)
{
  for (int i = nwHoogte -1; i >= 0; i--){ 
    for (int j = 0; j < nwBreedte; j++){
      bord[j][i] = 0;
    }
  }
  aanBeurt = 1;
  breedte = nwBreedte;
  hoogte = nwHoogte; 
}  // constructor met parameters

//*************************************************************************

//Leest het bord in van een .txt file.
//Parameter 'invoernaam' is de naam van de .txt file.
//'breedte' is gelijk aan de 1e gelezen char, 'hoogte' is gelijk aan de 2e gelezen char.
//Als 'breedte' en 'hoogte' geldige waardes hebben(dus meer dan 0 en kleiner dan MaxDimentie)
//en het aantal schijven kloppen, dan wordt een bord gemaakt.
bool VierOpEenRij::leesIn (const char* invoernaam)
{
  ifstream fin(invoernaam); 
  if(fin.is_open()){ 
    fin >> breedte;
    cout << "Breedte is: " << breedte << endl;
    fin >> hoogte;
    cout << "Hoogte is: " << hoogte << endl; 
    if (breedte <= MaxDimensie && hoogte <= MaxDimensie && breedte > 0 && hoogte > 0){
      int teller1 = 0, teller2 = 0;//telt het aantal schijven van elk speler  
      for (int i = hoogte -1; i >= 0; i--){ 
        for (int j = 0; j < breedte; j++){
          fin >> bord[j][i];//plaatst de char op positie [j][i]
          //checkt of alle stenen goed 'gevallen' zijn.
          if ((bord[j][i+1] != 0 && bord[j][i] == 0 && i >= 0 && (i <= hoogte -2))){
            return false;
          }
          if (bord[j][i] == 1){
            teller1++;
          }else if (bord[j][i] == 2){
            teller2++;
          }
        }
      }
      //als het aantal schijven kloppen in verhouding met elkaar, bord wordt gemaakt.
      if (teller2 >= 0 && teller1 >= teller2 && teller2 + 1 >= teller1){
        return true;
      }
    }
  }
return false;
}// leesIn

//*************************************************************************

//Checkt of er een eindstand is bereikt.
//Het checkt voor ieder schijf na elke zet of er een vier op een rij is.
//Membervariabelen 'hoogte' & 'breedte' zijn de dimensies van het bord.
bool VierOpEenRij::eindstand ()
{
  bool bordvol = true;//variabel of bord vol is
  for (int i = hoogte -1 ; i >= 0; i--){
    for (int j = 0; j < breedte; j++){
      if (bord[j][i] == 0){//als een schijf in het bord waarde 0 heeft, bord niet vol. 
        bordvol = false;
      }
      //checkt 4 richtingen voor elke schijf of er vier op een rij is
      for (int richtingen = -1; richtingen <= 1; richtingen++){
        for (int richtingen2 = -1; richtingen2 <= 1; richtingen2++){
          if (richtingen != 0 || richtingen2  != 0){
            if (telSchijvenInRichting(1, i, j, richtingen, richtingen2) >= 4){
              return true;
            }
            if (telSchijvenInRichting(2, i, j, richtingen, richtingen2) >= 4){
              return true;
            }
          }
        }
      }
    }
  }
  if (bordvol == true){
    return true; 
  }
  return false;
} // eindstand

//*************************************************************************

//Druk de hele stand (bord met schijven, speler aan beurt) af op het scherm.
//Membervariabel 'aanBeurt' zegt wie er aan de beurt is.
//Membervariabelen 'hoogte' & 'breedte' zijn de dimensies van het bord.
//
void VierOpEenRij::drukAf ()
{
int teller1 = 0, teller2 = 0;//teller voor aantal schijven van elk speler  
  for (int i = hoogte -1 ; i >= 0; i--){//onderaan is hoogte 0
    cout << endl;
    for (int j = 0; j < breedte; j++){
     cout << bord[j][i] << " ";
     if (bord[j][i] == 1){
      teller1++;
     }else if (bord[j][i] == 2){
      teller2++;
     }
    }
  }
  cout << endl;
  if(teller1 == teller2){//wie er aan de beurt is wordt geprint.
    cout << "Speler 1 is aan de beurt";
    aanBeurt = 1;
  }else{
    cout << "Speler 2 is aan de beurt";
    aanBeurt = 2;
  }

}  // drukAf

//*************************************************************************

//Functie om een zet te doen voor de huidige speler in de gekozen kolom.
//Als aanBeurt 1 is en een zet wordt gedaan, aanBeurt wordt 2.
//De laatste zet wordt in een vector achter in gepushed.
bool VierOpEenRij::doeZet (int kolom)
{
  int hoogtecopy = hoogte -1;//variabel van de hoogte van het bord
  if (bord[kolom][hoogtecopy] != 0){//als de kolom vol is, return false
    return false;
  }else{//totdat het geen 0 tegenkomt, hoogtecopy--
    while (bord[kolom][hoogtecopy] == 0 && (hoogtecopy >=0)){
      hoogtecopy--;
    }
    bord[kolom][hoogtecopy+1] = aanBeurt;//schijf van aanBeurt wordt geplaatst 
    zetten.push_back(kolom);//laatste kolom wordt in de vector gezet
    if (aanBeurt == 1){//speler wordt van beurt gewisseld
      aanBeurt = 2;
    }else{
      aanBeurt = 1;
    }  
  return true;
  }
}  // doeZet

//*************************************************************************
//Maakt laatst gedane zet ongedaan.
//Haalt de laatste kolom van de vector.
bool VierOpEenRij::unDoeZet ()
{ 
  int hoogtecopy = hoogte -1;//variabel van de hoogte van het bord
  int n = zetten.size();//aantal zetten in de vector
  if (zetten.empty() == true){//als er geen zetten gedaan zijn, return false
    return false;
  }else{//hoogte van kolom wordt verlaagd
    while (bord[zetten[n-1]][hoogtecopy] == 0 && (hoogtecopy >=0)){
      hoogtecopy--;
    }
    bord[zetten[n-1]][hoogtecopy] = 0;//op deze positie wordt veranderd naar een 0
    if (aanBeurt == 1){//speler wordt van beurt gewisseld
      aanBeurt = 2;
    }else{
      aanBeurt = 1;
    }
    zetten.pop_back();
  }
  return true;
}  // unDoeZet

//*************************************************************************
//Doormiddel van bruteforce en recursie wordt de eindscore van de huidige 
//speler wanneer vanaf dat punt beide spelers optimaal spelen.
//Parameter besteKolom is de beste zet die de huidige speler kan zetten op 
//dit punt. Het aantalStanden is het aantal borden gemaakt met recursie
//om de eindstand te berekenen.
int VierOpEenRij::besteScore (int &besteKolom, long long &aantalStanden)
{
  int score; 
  aantalStanden++;
  int bkolom = -1;//nodig voorrecursief aanroepen van bestescore
  int uitkomst;//
  int bestescore = -2; 
  if (scoregeef(aanBeurt,uitkomst)){
    return uitkomst;    
   // laatste zet pakken van vector 
  }else{
    for(int column = 0; column < breedte; column++){
      if (bord[column][hoogte-1] == 0){
        doeZet(column);
        score = -besteScore(bkolom, aantalStanden);
        unDoeZet();
        if (score > bestescore){
          bestescore = score;
          besteKolom = column;
          if (bestescore == 1){
            return bestescore;
          }
        }
      }

    } 
  }
  return bestescore;
}  // besteScore

//*************************************************************************
//Hier wordt de beste zet berekent voor de huidige speler door middel van 
//het spelen van aantal nrSimulaties random uitspeelt het de kolom kiest
//met de hoogste gemiddelde score.
//nrSimulaties in de parameter is standaard 100.
//Voor het berekenen van score: +1 als gewonnen, 0 als remise
//-1 als verloren.
//
int VierOpEenRij::bepaalGoedeZet (int nrSimulaties)
{ 
  int max = 0, punten[MaxDimensie] = {0};//hoogste score
  int oudezetten = zetten.size();//aantal zetten voordat functie is aangeroepen
  int speleraanbeurt = aanBeurt;//speleraanbeurt = speler die functie heeft geroepen
  int random = randomGetal(0, breedte -1);//random kolom
  int uitkomst; //integer voor uitkomst van het bord
  if (!eindstand()){//als de kolom al vol is,...
    for (int i = 0; i < breedte; i++){
      if (bord[i][hoogte-1] != 0){
        punten[i]= -10000;//kan sws niet gekozen als beste kolom
      }
    }
    for(int kopties = 0; kopties < breedte; kopties++){
      for(int simulaties = nrSimulaties; simulaties > 0; simulaties--){
        if (bord[kopties][hoogte-1] == 0){
          doeZet(kopties);//gaat alle kolommen na als begin zet
          while (!scoregeef(speleraanbeurt, uitkomst)){//als nog geen eindstand, doe random zetten
            random = randomGetal(0, breedte-1);
            doeZet(random);
          }
          punten[kopties] += uitkomst;
          while(zetten.size() - oudezetten > 0){
            unDoeZet();
          }
        }
      }
    }
  }
  for (int j = 0; j < breedte; j++){
    if (punten[j] > punten[max]){
      max = j;//kiest de kolom met de hoogste score
    }
  }
  return max;
}  // bepaalGoedeZet

//*************************************************************************

int VierOpEenRij::bepaalGoedeScore (int nrSimulaties)
{
  int oudezetten = zetten.size(), speleraanbeurt = aanBeurt;
  int uitkomst; //integer opslaan uitkomst
  scoregeef(speleraanbeurt,uitkomst);//uitkomst bepalen als al eindstand
  int returnwaarde= uitkomst;//returnwaarde is uitkomst
  int bestekolom;//beste kolom voor beste zet
  int goedekolom;//goede kolom voor goede zet
  long long aantalStanden = 0;
  if (!eindstand()){
    while (!scoregeef(speleraanbeurt, uitkomst)){
      if (speleraanbeurt == aanBeurt){
        goedekolom = bepaalGoedeZet(nrSimulaties);
        //bepalen goede zet
        doeZet(goedekolom);//zet goede zet
      }else{
        besteScore(bestekolom, aantalStanden);
        //bepalen beste kolom
        doeZet(bestekolom);// zet beste zet
      }
    }
    returnwaarde = uitkomst;
    while(zetten.size() - oudezetten > 0){
      unDoeZet();//plaats bord terug naar oude situatie
    }
  }

// TODO: implementeer deze memberfunctie

  return returnwaarde;
// TODO: implementeer deze memberfunctie

}  // bepaalGoedeScore

//*************************************************************************
/* Deze member*/
int VierOpEenRij::bepaalRandomScore (int nrSimulaties)
{
  int oudezetten = zetten.size();//aantal zetten voordat functie is aangeroepen
  int speleraanbeurt = aanBeurt;//speleraanbeurt = speler die functie heeft geroepen
  int uitkomst;//uitkomst van het spel
  scoregeef(speleraanbeurt, uitkomst);//goede returnwaarde als eindstand
  int returnwaarde = uitkomst;
  int random = randomGetal(0, breedte -1);//random kolom
  int goedezet;//kolom van goedezet 
  if (!eindstand()){
    while (!scoregeef(speleraanbeurt, uitkomst)){
      if (speleraanbeurt == aanBeurt){//als huidige speler,
        while(bord[random][hoogte-1] != 0){//doe randomzet
          random = randomGetal(0, breedte -1);
        }
        doeZet(random);
      }else{
        goedezet = bepaalGoedeZet(nrSimulaties);
        doeZet(goedezet);//doe goede zet
      }
    }
    returnwaarde = uitkomst;//score van eindstand
    while(zetten.size() - oudezetten > 0){
      unDoeZet();//terugzetten van oudesituatie van het bord
    }
  }

// TODO: implementeer deze memberfunctie

  return returnwaarde;

}  // bepaalRandomScore

//*************************************************************************
/* Deze Memberfunctie telt het aantal schijven van een specifieke 'kleur' op een punt met 'kolom0',
'rij0' 
in de richting 'deltakolom' en 'deltarij'. 'deltarij' is de delta richting van rij, 'deltarij' = 1
zou dus een verplaatsing naar boven. 'deltakolom' is hetzelfde maar dan voor de kolom. 
Deze memberfunctie veranderd niets aan de membervariabelen. */ 
int VierOpEenRij::telSchijvenInRichting (int kleur, int kolom0, int rij0,
          int deltakolom, int deltarij)
{
  int kolom = kolom0, rij = rij0;//copy van kolom0 en rij0 voor aanpassen
  int inderichting = 0;// Teller voor aantalschijven in een richting
  while (rij < hoogte && rij >= 0 && kolom < breedte && kolom >=0 && bord[kolom][rij] == kleur){
    inderichting++;
    rij = rij + deltarij;
    kolom = kolom + deltakolom;
  }
  return inderichting;
}  // telSchijvenInRichting


/* Deze memberfunctie kijkt of er een zet is die al eerder in het programma is gedaan.
 Zo ja dan gaat de functie voor deze kolom/zet kijken wat bijbehorende hoogte ervan is. 
 Aan de hand van deze bijbehorende hoogte gaat de functie in alle 8 de richtingen 
 kijken of er een 4 of meer op een rij is, met de functie telSchijvenInRichting. 
 Wij moeten beide richtingen checken want het kan het geval zijn dat de huidige zet net tussen een 1 links en een 2 rechs wordt geplaats.  */ 

bool VierOpEenRij::scoregeef(int speleraanbeurt, int& uitkomst){
  int kolom0 = zetten[zetten.size()-1];// pakt vorige zet
  int rij0 = hoogte - 1;//hoogst mogelijke rij
  int kleur;//integer voor het bewaren van de kleur van de vorige zet
  if (zetten.empty() == false){
    while (bord[kolom0][rij0] == 0){
      rij0--;
    }
    kleur = getVakje(kolom0, rij0);
    for (int richtingen = 0; richtingen <= 1; richtingen++){
      for (int richtingen2 = -1; richtingen2 <= 1; richtingen2++){
        if (richtingen != 0 || richtingen2  != 0){
          if ((telSchijvenInRichting(kleur, kolom0, rij0, richtingen, richtingen2) + telSchijvenInRichting(kleur, kolom0, rij0, -richtingen, -richtingen2) -1) >= 4){ 
            if (speleraanbeurt == kleur){
              uitkomst = 1; // de speler die de zet deed heeft gewonnen
              return true;
            }else{
              uitkomst = -1;// de speler die de zet deed heeft verloren
              return true;
            }
          }
        }
      }
    }
    //wel iets doen
  }
  for (int i = 0; i < breedte; i++){//kijken of het bord niet vol is 
    if (bord[i][hoogte-1] == 0){
      uitkomst = -2;//uitkomst boeit niet, je speelt verder
      return false;//het bord is nog niet vol
    }
  }
  uitkomst = 0;//het bord heeft geen vier op een rij en is vol, remise
  return true;//Eindstand is remise
}
































