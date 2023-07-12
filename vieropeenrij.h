// Definitie van klasse VierOpEenRij

#ifndef VierOpEenRijHVar // voorkom dat dit bestand meerdere keren
#define VierOpEenRijHVar // ge-include wordt

#include <vector>
#include "constantes.h"
using namespace std;

class VierOpEenRij
{
public:
  // Default constructor.
  VierOpEenRij();

  // Constructor met parameters voor breedte en hoogte van het bord.
  // Controleer eerst nog wel of deze parameters geldig zijn.
  VierOpEenRij(int nwBreedte, int nwHoogte);

  // Getter voor breedte
  int getBreedte()
  {
    return breedte;
  }

  // Getter voor de inhoud van vakje (kolom,rij) op het bord.
  int getVakje(int kolom, int rij)
  {
    return bord[kolom][rij];
  }

  // Lees een bord in vanuit tekstbestand invoernaam, mogelijk al (deels)
  // gevuld met schijven.
  // Controleer daarbij
  // * of het bestand wel te openen is,
  // * of breedte en hoogte binnen de grenzen vallen,
  // * of 0 <= aantalBlauw <= aantalGeel <= aantalBlauw+1
  // * of er geen lege vakjes onder gevulde vakjes liggen
  // Retourneer:
  // * true, als aan alle voorwaarden is voldaan
  // * false, als niet aan alle voorwaarden is voldaan
  // Post:
  // * Als aan alle voorwaarden is voldaan, is het bord met de schijven
  //   opgeslagen in membervariabelen, en staat ook aanBeurt goed.
  bool leesIn(const char *invoernaam); // check

  // Controleer of we een eindstand hebben bereikt:
  // * een van de twee spelers heeft vier op een rij
  // * of het bord is vol
  // Retourneer:
  // * true, als we een eindstand hebben bereikt
  // * false, als we geen eindstand hebben bereikt
  bool eindstand(); // check

  // Druk de hele stand (bord met schijven, speler aan beurt) af op
  // het scherm.
  void drukAf(); // check

  // Doe een zet voor de speler die aan de beurt is:
  // een schijf laten vallen in kolom `kolom'
  // Controleer eerst of het wel een geldige zet is, dat wil zeggen,
  // of het nog geen eindstand is, of kolom een geldig kolomnummer is,
  // en die kolom nog niet vol is.
  // Retourneer:
  // * true, als dit een geldige zet is
  // * false, als dit geen geldige zet is.
  // Pre:
  // * huidig bord is geldig
  // Post:
  // * als het een geldige zet is, is de zet uitgevoerd:
  //   - de schijf ligt op het bord in de aangegeven kolom
  //   - de speler aan beurt is gewisseld,
  //   - de zet is toegevoegd aan de lijst met gedane zetten
  // * als het geen geldige zet is, is de stand niet veranderd.
  bool doeZet(int kolom); // check

  // Maak de laatst gedane zet ongedaan.
  // Controleer eerst of er wel een zet is om ongedaan te maken,
  // opgeslagen in de lijst met zetten.
  // Retourneer:
  // * true, als er een zet was om ongedaan te maken
  // * false, anders
  // Post:
  // * als returnwaarde true is, is de zet ongedaan gemaakt:
  //   - de schijf is van het bord gehaald
  //   - de zet is van de lijst met uitgevoerde zetten gehaald
  //   - aanBeurt is teruggezet
  // * als returnwaarde false is, is er niets veranderd
  bool unDoeZet(); // check

  // Bepaal met behulp van brute force en recursie de eindscore voor
  // de speler die in de huidige stand (= de stand van de huidige
  // recursieve aanroep) aan de beurt is, wanneer beide spelers vanaf
  // dit punt optimaal verder spelen.
  // De score is
  // * 1 als de speler gaat winnen
  // * 0 als het remise wordt
  // * -1 als de speler gaat verliezen
  // Pre:
  // * de huidige stand is het resultaat van correct spel
  // Post:
  // * als de huidige stand geen eindstand was, bevat parameter
  //   besteKolom het nummer van de kolom waarin de huidige speler
  //   de volgende schijf moet spelen, om de beste score te bereiken
  // * anders bevat parameter besteKolom een passende default waarde
  // * aantalStanden is gelijk aan het aantal standen dat we hebben
  //   bekeken bij het bepalen van de beste eindscore
  // * de stand in het spel is nog onveranderd
  int besteScore(int &besteKolom, long long &aantalStanden);

  // Bepaal een `goede zet' voor de speler die in de huidige stand aan
  // aan de beurt is: de zet die ertoe leidt dat hij bij nrSimulaties keer
  // random uitspelen een zo goed mogelijke gemiddelde score haalt.
  // Controleer eerst
  // * of het geen eindstand is
  // Retourneer:
  // * de gevonden kolom, als het geen eindstand is
  // * een passende default waarde, als het al wel een eindstand is
  int bepaalGoedeZet(int nrSimulaties); // check

  // Speel het spel uit vanaf de huidige stand. Laat hierbij de speler
  // die in de huidige stand aan de beurt is, steeds een `goede zet'
  // (gevonden met bepaalGoedeZet) doen, terwijl de andere speler steeds
  // een beste zet (gevonden met besteScore) doet.
  // Parameter nrSimulaties wordt gebruikt voor aanroep bepaalGoedeZet.
  // Retourneer:
  // * de score aan het eind van het spel voor de speler die steeds
  //   een `goede zet' gedaan heeft
  // Post:
  // * de huidige stand is weer hetzelfde als aan het begin van de functie
  //   (zetten zijn dus weer ongedaan gemaakt)
  int bepaalGoedeScore(int nrSimulaties);

  // Speel het spel uit vanaf de huidige stand. Laat hierbij de speler
  // die in de huidige stand aan de beurt is, steeds een random
  // (maar wel geldige) zet doen, terwijl de andere speler steeds
  // een `goede zet' (gevonden met bepaalGoedeZet) doet.
  // Parameter nrSimulaties wordt gebruikt voor aanroep bepaalGoedeZet.
  // Retourneer:
  // * de score aan het eind van het spel voor de speler die steeds
  //   een random zet gedaan heeft
  // Post:
  // * de huidige stand is weer hetzelfde als aan het begin van de functie
  //   (zetten zijn dus weer ongedaan gemaakt)
  int bepaalRandomScore(int nrSimulaties); // check
private:
  // Tel het aantal aaneensluitende schijven van een bepaalde kleur,
  // vanaf (en inclusief) positie (kolom0,rij0) op het bord,
  // in richting (deltakolom,deltarij).
  // Retourneer:
  // * het gevonden aantal
  int telSchijvenInRichting(int kleur, int kolom0, int rij0,
                            int deltakolom, int deltarij); // check
  bool scoregeef(int speleraanbeurt, int &uitkomst);       // check
                                                     //  membervariabelen
  int bord[MaxDimensie][MaxDimensie]; // [kolom][rij]: inhoud van bord;
                                      // kolommen genummerd van 0..breedte-1, van links naar rechts;
                                      // rijen genummerd van 0..hoogte-1, van beneden naar boven
  vector<int> zetten;                 // uitgevoerde zetten (kolomnummers) die tot
                      // huidige stand hebben geleid (bij ingelezen bord alleen de zetten
                      // die na inlezen zijn uitgevoerd)
  int breedte, hoogte, // van het bord
      aanBeurt;        // speler die aan de beurt is
};

#endif
