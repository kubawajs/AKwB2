#include <iostream>
#include <cstdlib>
#include <string>
#include <fstream>
#include <list>

using namespace std;

/*******************
*                  *
*  AKB zadanie 2   *
*                  *
* vers. 11-12-15   *
*                  *
*******************/

//Dodano poprawki przy wczytywaniu - uwzględnia nazwy wierzcholkow wieksze niz liczba wierzcholkow --- TESTING

//DEFINICJE FUNKCJI============================================================================================================

bool checkAdjoint(int vertex, int **matrix)
{
    for(int a=0; a<vertex-1; a++)//petla od poruszania sie po wierszach macierzy
    {
        for(int b=a+1; b<vertex; b++)//petla od poruszania sie po wierszach porownywanych z wierszem powyzej
        {
            for (int c=0; c<vertex; c++)//dodatkowa petla porownujaca nastepniki w kolumnach macierzy pomiedzy porownywanymi wierzcholkami a i b
            {
                if(matrix[a][c]!=0 && matrix[b][c]!=0)//szukam czy zbior nastepnikow nie jest rozlaczny
                {
                    for(int d=0; d<vertex; d++)//jesli nie jest rozlaczny to sprawdzam czy jest sobie rowny
                    {
                        if((matrix[a][d] == 0 && matrix[b][d] != 0)||(matrix[a][d] != 0 && matrix[b][d] == 0))
                            //jesli w jednym wierszu istnieje luk a w porownywanym nie, to zwroc falsz
                        {
                            return false;
                        }
                    }
                }
            }
        }
    }
    return true;//jesli powyzsze sprawdzenia dla wszystkich par wierzcholkow przebiegly pomyslnie to zwroc prawde
}

bool checkLine(int vertex, int **matrix)
{
//sprawdza wszystkie pionowe pary kolumn w macierzy, jesli gdzies informacje o poprzedniku sie pokrywaja - graf nie jest liniowy
    for(int a=0; a<vertex-1; a++)//petla od poruszania sie po wierszach macierzy
    {
        for(int b=a+1; b<vertex; b++)//petla od poruszania sie po wierszach porownywanych z wierszem powyzej
        {
            for (int c=0; c<vertex; c++)//dodatkowa petla porownujaca nastepniki w kolumnach macierzy pomiedzy porownywanymi wierzcholkami a i b
            {
                if(matrix[a][c]!=0 && matrix[b][c]!=0)//szukam czy zbior nastepnikow nie jest rozlaczny
                {
                    for(int d=0; d<vertex; d++)//jesli nie jest rozlaczny to sprawdzam czy jest sobie rowny
                    {
                        //pierwsza czesc warunku wykazana w funkcji checkAdj
                        if(matrix[d][a] != 0 && matrix[d][b] != 0)//porownywuje zbiory poprzednikow - jesli istnieje wspolny poprzednik to zwroc falsz
                        {
                            return false;
                        }
                    }
                }
            }
        }
    }
    return true;//jesli powyzsze sprawdzenia dla wszystkich par wierzcholkow przebiegly pomyslnie to zwroc prawde
}

void transformer1(int vertex, int **matrix, int **tabkapomocka)//tworzy nowa liste krawedzi dla grafu oryginalnego
{
    int l_krawedzi=1;//zmienna od nazywania wierzcholkow grafu oryginalnego
    for(int a=0; a<vertex; a++)//petla od kolejnych wierzcholkow (wiersze macierzy)
    {
        for(int b=0; b<vertex; b++)//petla od kolejnych potencjalnych nastepnikow (kolumny macierzy)
        {
            if(tabkapomocka[a][0]==0 && b==0)//jesli wierzcholek wyjsciowy powstalej krawedzi nie zostal wczesniej okreslony
            {
                tabkapomocka[a][0]=l_krawedzi;
                l_krawedzi++;
            }
            if(tabkapomocka[a][1]==0 && b==0)//jesli wierzcholek wejsciowy powstalej krawedzi nie zostal wczesniej okreslony
            {
                tabkapomocka[a][1]=l_krawedzi;
                l_krawedzi++;
            }
            if(matrix[a][b]==1)
            {
                tabkapomocka[b][0]=tabkapomocka[a][1];//jesli istnieje nastepnik badanego wierzcholka, to powstala z niego krawedz
                //musi rozpoczynac sie z wierzcholka, do ktorego wchodzila krawedz powstala z wierzcholka, ktory jest jego poprzednikiem
            }
            for(int c=0; c<vertex; c++)
            {
                if(matrix[c][a]==1)//przegladam poprzedniki
                {
                    tabkapomocka[c][1]=tabkapomocka[a][0];//jesli istnieje poprzednik badanego wierzcholka, to powstala z niego krawedz
                    //musi konczyc sie w wierzcholku, z ktorego wychodzila krawedz powstala z wierzcholka, ktory jest jego nastepnikiem
                }
            }
        }
    }
}

void showInfo(bool checkAdj, bool checkL, bool checkM, int vertex, int **tabkapomocka, int vertex2, int *vertexTab)
{
    cout<<endl<<"Wynik sprawdzenia: "<<endl;
    if(checkAdj && !checkM)                                            //na podst wartosci wynikowej funkcji checkAdjoint
        cout<<"1. Graf JEST sprzezony."<<endl;
    if(!checkAdj || checkM)                                   //na podst wartosci wynikowej funkcji checkAdjoint lub sprawdzenia czy graf jest multigrafem (przy wczytywaniu z pliku)
        cout<<"1. Graf NIE JEST sprzezony."<<endl;
    if(checkL && checkAdj)
    {
        cout<<"2. Graf JEST liniowy."<<endl;
        cout<<endl<<"Graf oryginalny: "<<endl<<endl<<"Liczba wierzcholkow: "<<vertex2<<endl;
        cout<<"Lista krawedzi: "<<endl;
        for(int a=0; a<vertex; a++)
        {
            cout<<tabkapomocka[a][0]<<" "<<tabkapomocka[a][1]<<"\t"<<"Krawedz powstala z wierzcholka: "<<vertexTab[a]<<endl;
        }
    }
    if(!checkL && checkAdj)
    {
        cout<<"2. Graf NIE JEST liniowy."<<endl;
        cout<<endl<<"Graf oryginalny: "<<endl<<endl<<"Liczba wierzcholkow: "<<vertex2<<endl;
        cout<<"Lista krawedzi: "<<endl;
        for(int a=0; a<vertex; a++)
        {
            cout<<tabkapomocka[a][0]<<" "<<tabkapomocka[a][1]<<"\t"<<"Krawedz powstala z wierzcholka: "<<vertexTab[a]<<endl;
        }
    }
}

int count_vertex(int **tabkapomocka, int **tabkacopy, int vertex, int vertex2)
{
    for(int a=0; a<vertex; a++)
    {
        for(int b=0; b<2; b++)
        {
            tabkacopy[a][b]=tabkapomocka[a][b];
        }
    }
    for(int a=0; a<vertex; a++)
    {
        for(int b=0; b<2; b++)
        {
            if(tabkacopy[a][b]!=0)
            {
                int temp = tabkacopy[a][b];
                vertex2++;
                for(int c=0; c<vertex; c++)
                {
                    for(int d=0; d<2; d++)
                    {
                        if(temp==tabkacopy[c][d])
                            tabkacopy[c][d]=0;
                    }
                }
            }
        }
    }
    return vertex2;
}

int saveInVertexTab(int *vertexTab, int vertex, int k)
{
    int t;
    for(t=0; t<vertex; t++)
    {
        if(vertexTab[t]==k)
        {
            break;
        }
        if(vertexTab[t]==0)
        {
            vertexTab[t]=k;
            break;
        }
    }
    return t;
}

//MAIN====================================================================================================================

int main()
{
    //DEFINICJE ZMIENNYCH=================================================================================================

    int choose = 1;                                         //zmienna od wyjscia
    bool checkAdj = false;                                  //zmienna od informacji czy graf jest sprzezony
    bool checkL = false;                                    //zmienna od informacji czy graf jest liniowy
    bool checkM = false;                                    //zmienna od informacji czy graf jest multigrafem

    int t1=0;
    int t2=0;

    int **matrix;                                           //macierz dynamiczna dla grafu badanego
    int **tabkapomocka;                                     //tablica pomocnicza przy transformacji
    int **tabkacopy;                                        //tablica pomocnicza do liczenia wierzcholkow grafu oryginalnego
    int *vertexTab;                                         //tablica rzutujaca nazwy wierzcholkow na miejsca w macierzy
    int vertex=0;                                           //zmienna przechowujaca informacje o liczbie wierzcholkow badanego grafu
    int vertex2=0;                                          //zmienna przechowująca informacje o liczbie wierzcholkow grafu oryginalnego
    int l_w=0;                                             //licznik wierzcholkow grafu oryginalnego

    //WCZYTYWANIE=========================================================================================================

    cout<<"AKwB - Zadanie 2 - Jakub Wajs 2015"<<endl<<endl;
    string name;
    cout<<"Podaj nazwe pliku, ktory chcesz otworzyc: "<<endl;
    cin>>name;
    string fileName = name + ".txt";
    fstream file;
    file.open(fileName.c_str(), ios::in);
    if(!file.good())
    {
        cout<<endl<<endl<<"BLAD!!! Nie wczytano pliku.";
        return 0;
    }

    file>>vertex;                                     //wczytuje informacje o liczbie wierzcholkow badanego grafu

    matrix = new int *[vertex];                       //tworzenie macierzy o wielkosci wczytanej z pliku
    vertexTab = new int [vertex];

    for (int i = 0; i<vertex; i++)
        matrix[i] = new int [vertex];

    for(int i=0; i<vertex; i++)                       //zerowanie macierzy
    {
        for(int j=0; j<vertex; j++)
        {
            matrix[i][j]=0;
        }
    }

    for(int kl=0; kl<vertex; kl++)
    {
        vertexTab[kl]=0;
    }

    cout<<endl<<"Badany graf: "<<endl<<endl<<"Liczba wierzcholkow: "<<vertex<<endl;
    cout<<"Lista krawedzi: "<<endl;

    while(!file.eof())                              //wczytuje z pliku dane o lukach
    {
        int k, l;
        file>>k;                                    //przypisz numer wierzcholka wyjsciowego
        file>>l;                            //przypisz numer wierzcholka wejsciowego

        t1 = saveInVertexTab(vertexTab, vertex, k);//tworzenie odwzorowania wierzcholka wczytanego k lub szukanie juz powstalego odpowiednika
        t2 = saveInVertexTab(vertexTab, vertex, l);//tworzenie odwzorowania wierzcholka wczytanego l lub szukanie juz powstalego odpowiednika

        cout<<vertexTab[t1]<<" "<<vertexTab[t2]<<endl;
        if(matrix[t1][t2]==1)                         //jesli luk wystepuje ponownie, dodaj informacje ze badany graf jest multigrafem
        {
            checkM = true;
        }
        else
        {
            matrix[t1][t2]=1;                    //jest luk
        }
    }
    file.close();

    //TESTY NA SPRZEZENIE I LINIOWOSC=============================================================================

    if(!checkM) checkAdj = checkAdjoint(vertex, matrix);//sprawdz czy graf jest sprzezony (funkcja)

    if(checkAdj) checkL = checkLine(vertex, matrix);  //sprawdz czy graf jest liniowy (funkcja)

    //TRANSFORMACJA================================================================================================

    if(checkAdj)                                    //jesli graf jest sprzezony, to wykonaj transformacje do grafu oryginalnego
    {
        tabkapomocka = new int *[vertex];

        for (int i = 0; i<vertex; i++)
            tabkapomocka[i] = new int [2];

        for(int i=0; i<vertex; i++)                   //zerowanie macierzy
        {
            for(int j=0; j<2; j++)
            {
                tabkapomocka[i][j]=0;
            }
        }

        transformer1(vertex, matrix, tabkapomocka);

        //LICZENIE WIERZCHOLKOW G. ORYG.==========================================================================

        tabkacopy = new int *[vertex];

        for (int i = 0; i<vertex; i++)
            tabkacopy[i] = new int [2];

        vertex2 = count_vertex(tabkapomocka, tabkacopy, vertex, 0);//liczenie wierzcholkow grafu oryginalnego

        //ZAPIS DO PLIKU==========================================================================================

        string fileName = name + "-result.txt";     //zapis do pliku wynikowego
        fstream file;
        file.open(fileName.c_str(), ios::out);
        file<<vertex2<<endl;
        for(int i=0; i<vertex; i++)                   //przepisanie informacji o lukach z macierzy do pliku
        {
            file<<tabkapomocka[i][0]<<" ";
            file<<tabkapomocka[i][1]<<"\t";
        }
        file.close();
    }

    //KOMUNIKAT KONCOWY===============================================================================================

    showInfo(checkAdj, checkL, checkM, vertex, tabkapomocka, vertex2, vertexTab);     //wyswietlanie komunikatu wynikowego

    cout<<endl<<"0. Zakoncz program"<<endl<<endl;
    cin>>choose;

    while(choose!=0)                         //zabezpieczenie przed wpisaniem nieodpowiedniej opcji z menu
    {
        cout<<"\nBLAD! Wpisano bledna wartosc. Wpisz 0, aby zakonczyc.";
        cin>>choose;
    }

    //CZYSZCZENIE PAMIECI============================================================================================

    if(checkAdj)
    {
        for (int i = 0; i<2; i++)
            delete [] tabkapomocka[i];
        delete [] tabkapomocka;                  // zwalnianie pamieci
        for (int i = 0; i<2; i++)
            delete [] tabkacopy[i];
        delete [] tabkacopy;                  // zwalnianie pamieci
    }

    for (int i = 0; i<vertex; i++)
        delete [] matrix[i];
    delete [] matrix;                   // zwalnianie pamieci

    return 0;
}
