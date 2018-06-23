#include "stdafx.h"
#include "iostream"
#include "vector"
#include "ctime"		// losowanie
#include "conio.h"		// klawa
#include "fstream"		// plik
#include "windows.h"	// do Sleep()
#include "string"		//getline bez tego nie działa

#define GORA 72 //przypisuje wartosc pod tekst dyrektywa preprocesora nie kompilator
#define DOL 80
#define LEWO 75
#define PRAWO 77
#define ESC 27

using namespace std;


pair<int, int>plansza(21,31);
pair<int, int>glowa(plansza.first/2,plansza.second/2);
pair<int, int>jedzenie(glowa.first - 1,glowa.second);
pair<int, int>poprzednia_pozycja_glowy;

vector<pair<int,int>>ogon;

int dlugosc_ogona = 0;
int zmienna = 0;

bool przegrana = 0;
bool przechodzenie_przez_sciany = 1;
bool czyReplay = 0;


vector<int>generuj(int ile)
{
	vector<int>wynik;
	for (int i = 0; i < ile; i++)
	{
		wynik.push_back(0);
	}
	return wynik;
}
vector<vector<int>>generuj(int kolumny, int wiersze)
{
	vector<vector<int>>wynik;
	for (int i = 0; i < kolumny; i++)
	{
		vector<int>element = generuj(wiersze);
		wynik.push_back(element);
	}
	return wynik;
}

void wyswietl(vector<int>pole)
{
	for (int i = 0; i < pole.size(); i++)
	{
		if (pole[i] == 0)
		{
			cout << " ";
		}
		else if (pole[i] == 8)
		{
			cout << "!";
		}
		else if (pole[i] == 1)
		{
			cout << "G";
		}
		else if (pole[i] == 2)
		{
				cout << "o";
		}
		else if (pole[i] == 3)
		{
			cout << "*";
		}
	}
	cout << endl;
}

void wyswietl(vector<vector<int>>pole)
{
	for (int i = 0; i < pole.size(); i++)
	{
		wyswietl(pole[i]);
	}
	cout << endl;
}

void rysuj(vector<vector<int>>pole)
{
	pole[jedzenie.first][jedzenie.second] = 3;
	pole[glowa.first][glowa.second] = 1;
	
	//ramka

	for (int i = 0; i < plansza.first; i++)
	{
		pole[i][0] = 8;
		pole[i][plansza.second - 1] = 8;
	}
	for (int i = 0; i < plansza.second; i++)
	{
		pole[0][i] = 8;
		pole[plansza.first - 1][i] = 8;
	}
	// ogon
	for (int i = 0; i < dlugosc_ogona; i++)
	{
		pair<int, int>poprzednia_pozycja_ogona;
		poprzednia_pozycja_ogona = ogon[i]; 
		ogon[i] = poprzednia_pozycja_glowy;
		pole[ogon[i].first][ogon[i].second] = 2; 
		poprzednia_pozycja_glowy = poprzednia_pozycja_ogona;
	}

	wyswietl(pole);
	cout << "ogon " << dlugosc_ogona << endl;
	cout << "glowaX: " << glowa.second << " glowaY: " << glowa.first;
}

int kontrolki()
{
	if (_kbhit())
	{
		switch ((_getch()))
		{
		case GORA:
			if (zmienna != 2)
			{
				zmienna = 1;
			}
			break;
		case DOL:
			if (zmienna != 1)
			{
				zmienna = 2;
			}			
			break;
		case LEWO:
			if (zmienna != 4)
			{
				zmienna = 3;
			}
			break;
		case PRAWO:
			if (zmienna != 3)
			{
				zmienna = 4;
			}
			break;
		case ESC:
			przegrana = 1;
			break;
		default:
			break;
		}
	}
	return zmienna;
}

void poruszanie()
{
	int trzymajdlo = kontrolki();
	poprzednia_pozycja_glowy = glowa;
	if (trzymajdlo == 0)
	{

	}
	else if (trzymajdlo == 1)
	{
		glowa.first--;
	}
	else if (trzymajdlo == 2)
	{
		glowa.first++;
	}
	else if (trzymajdlo == 3)
	{
		glowa.second--;
	}
	else if (trzymajdlo == 4)
	{
		glowa.second++;
	}
}

void sciany()
{
	if (przechodzenie_przez_sciany == 1)
	{
		if (glowa.first == 0)
		{
			glowa.first = plansza.first - 1;
		}
		else if (glowa.first == plansza.first-1)
		{
			glowa.first = 1;
		}
		else if (glowa.second == 0)
		{
			glowa.second = plansza.second - 1;
		}
		else if (glowa.second == plansza.second-1)
		{
			glowa.second = 1;
		}
	}
	else if (przechodzenie_przez_sciany == 0)
	{
		if (glowa.first == 0 || glowa.first == plansza.first - 1)
		{
			przegrana = 1;
		}
		else if (glowa.second == 0 || glowa.second == plansza.second - 1)
		{
			przegrana = 1;
		}
	}
}

void generujJedzenie(vector<vector<int>>pole)
{
	fstream jedzeniePlik;
	jedzeniePlik.open("../jedzenie.txt", ios::out | ios::app);
	jedzenie.first = rand() % (plansza.first - 2) + 1;
	jedzenie.second = rand() % (plansza.second - 2) + 1;
	for (int i = 0; i < dlugosc_ogona; i++)
	{
		if (jedzenie.first == ogon[i].first && ogon[i].second)
		{
			jedzenie.first = rand() % (plansza.first - 2) + 1;
			jedzenie.second = rand() % (plansza.second - 2) + 1;
		}
	}
	jedzeniePlik << jedzenie.first << endl;
	jedzeniePlik << jedzenie.second << endl;
}

void kolizje(vector<vector<int>>pole)
{
	for (int i = 0; i < dlugosc_ogona; i++)
	{
		if (glowa.first == ogon[i].first && glowa.second == ogon[i].second)
		{
			przegrana = 1;
		}
	}
	if (glowa.first == jedzenie.first && glowa.second == jedzenie.second)
	{
		ogon.push_back(pair<int,int>());
		dlugosc_ogona++;
		generujJedzenie(pole);
	}
}

void odswiez()
{
	Sleep(50);
	system("cls");
}

vector<pair<int, int>>przerzut()
{
	vector<pair<int, int>>wynik;
	int licznik = 1;
	int x,y;
	string schowekJedzenie;
	fstream jedzeniePlik("../jedzenie.txt");
	while (!jedzeniePlik.eof())
	{
		getline(jedzeniePlik, schowekJedzenie);
		if (licznik % 2 != 0)
		{
			x = atoi(schowekJedzenie.c_str());
		}
		else {
			y = atoi(schowekJedzenie.c_str());
			wynik.push_back(pair<int, int>(x, y));
		}
		licznik++;
	}
	return wynik;
}

void kolizjeReplay()
{
	for (int i = 0; i < dlugosc_ogona; i++)
	{
		if (glowa.first == ogon[i].first && glowa.second == ogon[i].second)
		{
			przegrana = 1;
		}
	}
	if (glowa.first == jedzenie.first && glowa.second == jedzenie.second)
	{
		dlugosc_ogona++;
	}
}

void replay(vector<vector<int>>pole)
{
	fstream glowaPlik("../replay.txt");
	vector<pair<int, int>>pary = przerzut();
	string schowek;
	glowa.first = plansza.first / 2;
	glowa.second = plansza.second / 2;
	dlugosc_ogona = 0;
	while (!glowaPlik.eof())
	{
		jedzenie = pary[dlugosc_ogona];
		getline(glowaPlik, schowek);
		zmienna = atoi(schowek.c_str());
		poruszanie();
		kolizjeReplay();
		rysuj(pole);
		sciany();
		odswiez();
	}
	glowaPlik.close();
	glowaPlik.clear();
}

void Snake(vector<vector<int>>pole)
{
	fstream glowaPlik;
	glowaPlik.open("../replay.txt", ios::out);
	if (glowaPlik.good())
	{
		while (przegrana == 0)
		{
			glowaPlik << zmienna << endl;
			kontrolki();
			poruszanie();
			kolizje(pole);
			rysuj(pole);
			sciany();
			odswiez();
		}
		if (dlugosc_ogona == ((plansza.first - 2)*(plansza.second - 2)) - 1)
		{
			przegrana = 1;
			cout << "Wygrana" << endl;
			cout << endl;
			cout << "Powtrorka ?  1-tak 0-nie: " << endl;
			cin >> czyReplay;
			if (czyReplay == 1)
			{
				replay(pole);
				cout << "Zapraszamy ponownie" << endl;
			}
			else {
				cout << "Zapraszamy ponownie" << endl;
			}
		}
		else if (przegrana == 1)
		{
			cout << "Przegrana" << endl;
			cout << endl;
			cout << "Powtorka ?  1-tak 0-nie: ";
			cin >> czyReplay;
			cout << endl;
			if (czyReplay == 1)
			{
				replay(pole);
				cout << "Zapraszam ponownie" << endl;
			}
			else {
				cout << "Zapraszam ponownie" << endl;
			}
			
		}
		glowaPlik.close();
		glowaPlik.clear();
	}
}

int main()
{
	srand(time(NULL));
	vector<vector<int>>pole = generuj(plansza.first, plansza.second);

	fstream jedzeniePlik;

	jedzeniePlik.open("../jedzenie.txt", ios::out | ios::trunc); // czysci
	jedzeniePlik.close();

	jedzeniePlik.open("../jedzenie.txt", ios::app); //dodaje 1 koordy
	jedzeniePlik << jedzenie.first << endl;
	jedzeniePlik << jedzenie.second << endl;
	jedzeniePlik.close();

	Snake(pole);
	jedzeniePlik.close();
	jedzeniePlik.clear();
	return 0;
}

