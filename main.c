#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define LATWY 8
#define ILOSCMINL 10
#define SREDNI 16
#define ILOSCMINS 40
#define TRUDNY 22
#define ILOSCMINT 99


int pozycja_x = 0, pozycja_y = 0; //Globalne zmienne do ustawienie pozycje (dane sa pobierane z klawiatury w fukcji sterowanie())
int koniec = 0;

struct pole
{
	int wartosc;
	bool odkrycie;
};

void ustawMine(struct pole **tab, int, int, const int);
void odkryjePlansze(struct pole **tab,int, int, const int);


void generujPlansze(struct pole **plansza, const int rozmiarplanszy) //Uzupelnianie tablicy plansza 
{
	for(int i = 0; i < rozmiarplanszy; i++)
	{
		for(int j = 0; j < rozmiarplanszy; j++)
		{
			plansza[i][j].wartosc = 0;
			plansza[i][j].odkrycie = false;
		}
	}
}

void losujPozycjeMiny(struct pole **plansza,int o_poz_x, int o_poz_y, int iloscmin, const int rozmiarplanszy) //losowanie min
{
	time_t tt;
	int poz_x;
	int poz_y;
	int ilosc = iloscmin;

	srand((unsigned)time(&tt)); //za zarodek wstawiamy pobrany czas w sekundach

	while(ilosc > 0)
	{
		poz_x = rand()%rozmiarplanszy;
		poz_y = rand()%rozmiarplanszy;

		if(o_poz_y == poz_x && o_poz_y == poz_y)
			continue;

		if(plansza[poz_x][poz_y].wartosc != 9) //sprawdzenie czy na danym polu nie ma juz miny
		{
			ustawMine(plansza,poz_x,poz_y, rozmiarplanszy); //wywolanie funkcji ustawiajacej miny
			ilosc--;
		}
	}
}

void ustawMine(struct pole **plansza,int poz_x, int poz_y, const int rozmiarplanszy) //funkcja ustawiajaca miny oraz wartosci wokol nich
{
	if(plansza[poz_x][poz_y].wartosc != 9) //sprawdznie czy nie ma miy
	{
		plansza[poz_x][poz_y].wartosc = 9; //ustawienie miny

		//ustawienie wartosc wokol miny
		for(int i = -1; i < 2; i++)
			for(int j = -1; j < 2; j++)
			{
				if((poz_x + i) < 0 || (poz_y+j) < 0) //sprawdzenie czy nie ustawiamy watosci poza plansza
					continue;
				if((poz_x + i) > (rozmiarplanszy - 1) || (poz_y+j) > (rozmiarplanszy - 1)) //sprawdzenie czy nie ustawiamy wartosci poza plansza
					continue;

				if(plansza[poz_x + i][poz_y + j].wartosc == 9) //sprawdzenie czy to nie mina
					continue;

				plansza[poz_x + i][poz_y + j].wartosc += 1; //dodanie jeden do wartosci
			}
	}
}

void pokazPlansze(struct pole **plansza, const int rozmiarplanszy) //funkcja pokazuje plasze
{
	system("clear"); //czyszczenie terminala

	printf("  ");
	for(int i = 0; i < rozmiarplanszy; i++) //wyswietlanie wspolrzednych kolumn
		if(i < 10)
			printf("%i  ", i);  
		else
			printf("%i ", i);

	printf("\n");

	for(int i = 0; i < rozmiarplanszy; i++)
	{
		if(i < 10)
			printf("%i ", i);
		else
			printf("%i", i); //wyswietlenie wspolrzednych wierszy

		for(int j = 0; j < rozmiarplanszy; j++)
		{
			if(plansza[i][j].odkrycie == true) //jesli odkrylismy pole
			{
				if(plansza[i][j].wartosc == 0) //jesli wartosc 0 wyswietl spacje
				{
					printf("   ");
				}
				else //inaczej wyswietl numer pola
				{
					printf("%i  ", plansza[i][j].wartosc); 
				}
			}
			if(plansza[i][j].odkrycie == false) //jesli nie odkryte wyswietl #
			{
				printf("\033[1;31m");
				printf("#  ");
				printf("\033[0m");
			}

		}
		printf("\n");
	}
}

void sterowanie(struct pole **plansza, const int rozmiarplanszy) //funkcja za pomoca ktorej wprowadzamy wspolrzedne ktore chcemy sprawdzic
{
	printf("Podaj wiersz: ");
	scanf("%i", &pozycja_x);
	printf("Podaj kolumne: ");
	scanf("%i", &pozycja_y);

	if(plansza[pozycja_x][pozycja_y].wartosc == 9) //trafiles na mine
		koniec = 2;

	odkryjePlansze(plansza, pozycja_x, pozycja_y, rozmiarplanszy); //odkrywanie planszy
	pokazPlansze(plansza, rozmiarplanszy); //wywietlanie planszy
}

//funkcja odkrywa plansze, wykonuje sie rekurencyjnie do napotkania nastepujacych warunkow:
// - czy nie wychodzi poza plansze
// - czy pole juz odkryte, aby nie odkrywac dwa razy
// - czy pole nie jest miny, aby nie odkryc miny
//odkrywanie planszy nastepuje rekurencyjnie i odkrywa wszystkie pola do okola tak oby odkryc wszystkie pola z wartoscia 0 oraz ich sasiadow
//gdy pole ma inna wartosc niz 0, odkrywamy je i wychodzimy zeby nie odkryc za duzo
void odkryjePlansze(struct pole **plansza, int x, int y, const int rozmiarplanszy)
{
	if(x < 0 || x > (rozmiarplanszy - 1))
		return;
	if(y < 0 || y > (rozmiarplanszy - 1))
		return;
	if(plansza[x][y].odkrycie == true)
		return;
	if(plansza[x][y].wartosc != 9 && plansza[x][y].odkrycie == false)
		plansza[x][y].odkrycie = true;

	if(plansza[x][y].wartosc != 0)
		return;

	//wywolujemy funkcje dla kazdego sasiada
	odkryjePlansze(plansza,x - 1, y -1, rozmiarplanszy);
	odkryjePlansze(plansza,x - 1, y, rozmiarplanszy);
	odkryjePlansze(plansza,x - 1, y +1, rozmiarplanszy);
	odkryjePlansze(plansza,x + 1, y -1, rozmiarplanszy);
	odkryjePlansze(plansza,x + 1, y, rozmiarplanszy);
	odkryjePlansze(plansza,x + 1, y +1, rozmiarplanszy);
	odkryjePlansze(plansza,x, y -1, rozmiarplanszy);
	odkryjePlansze(plansza,x, y, rozmiarplanszy);
	odkryjePlansze(plansza,x, y +1, rozmiarplanszy);
}

bool czyWygrane(struct pole ** plansza, const int rozmiarplanszy) // sprwdzenie czy sie wygralo, liczy liczbe nieodkrytych pol
{
	int miny = 0;

	for(int i = 0; i < rozmiarplanszy; i++)
	{
		for(int j = 0; j < rozmiarplanszy; j++)
		{
			if(plansza[j][i].odkrycie == false)
				miny++;
		}
	}

	if(miny == 10)
		return true;
	return false;
}

int main()
{
	int x, y;

	int wybor;
	int rozmiarplanszy;
	int iloscmin;
	int wyjdz = 0;
	clock_t t; //zmiena do mierzenia czas	

	struct pole **tab;

	while(wyjdz == 0)
	{
		system("clear");		

		printf("Witaj w grze Saper\n");
		printf("Wybierz poziom trudnosi:\n[1] Latwy\n[2] Normalny\n[3] Trudny\n[4] Wyjdz\n"); 

		scanf("%i", &wybor);

		switch(wybor)
		{
			case 1:	
				rozmiarplanszy = LATWY;
				iloscmin = ILOSCMINL;
				break;
			case 2:
				rozmiarplanszy = SREDNI;
				iloscmin = ILOSCMINS;
				break;
			case 3:
				rozmiarplanszy = TRUDNY;
				iloscmin = ILOSCMINT;
				break;
			case 4:
				wyjdz++;
				//continue;
				exit(1);	
				break;
		}	

		tab = (struct pole **)malloc(rozmiarplanszy*sizeof(struct pole*));

		for(int i = 0; i < rozmiarplanszy; i++)
			tab[i] = (struct pole*)malloc(rozmiarplanszy*sizeof(struct pole));


		generujPlansze(tab, rozmiarplanszy);

		pokazPlansze(tab, rozmiarplanszy);
		//t = clock(); // zaczecie mierzenia czasu
		printf("Podaj wiersz: ");
		scanf("%i", &x);
		printf("Podaj kolumne: ");
		scanf("%i", &y);
		losujPozycjeMiny(tab, x, y, iloscmin, rozmiarplanszy);
		odkryjePlansze(tab, x, y, rozmiarplanszy);
		pokazPlansze(tab,rozmiarplanszy);	

		while(koniec == 0)
		{
			sterowanie(tab, rozmiarplanszy);
			if(czyWygrane(tab, rozmiarplanszy) == true)
				koniec = 1;
		}

		//t = clock() - t; //koniec mierzenia czasu
		if(koniec == 1)
			printf("\nKoniec gry, Wygrales!!!\n");
		if(koniec == 2)
			printf("\nKoniec gry, Przegrales\n");
		//double time_taken = ((double)t)/CLOCKS_PER_SEC;
		//printf("%f\n", time_taken);
		
		koniec = 0;

		printf("Nacisnij enter...\n");
		getchar();
		getchar();
	}
	for(int i = 0; i < rozmiarplanszy; i++)
		free(tab[i]);
	free(tab);

	return 0;
}
