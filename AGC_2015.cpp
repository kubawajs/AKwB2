#include <iostream>
#include <cstdlib>
#include <string>
#include <fstream>
#include <list>

using namespace std;

/*******************
* Created by JWajs *
*                  *
*      v.1.02      * 
*	  30-11-15     *
*                  *
*******************/


// FUNCTION DEFINITIONS ========================================================================================

bool checkAdjoint(int vertex, int **matrix)
{
	///<summary>
	///Function for checking if graph is adjoint graph
	///</summary>
    for(int a=0; a<vertex-1; a++)
    {
        for(int b=a+1; b<vertex; b++)
        {
            for (int c=0; c<vertex; c++)
            {
                if(matrix[a][c]!=0 && matrix[b][c]!=0)
                {
                    for(int d=0; d<vertex; d++)
                    {
                        if((matrix[a][d] == 0 && matrix[b][d] != 0)||(matrix[a][d] != 0 && matrix[b][d] == 0))
                        {
                            return false;
                        }
                    }
                }
            }
        }
    }
    return true;
}


bool checkLine(int vertex, int **matrix)
{
	///<summary>
	///Function for checking if adjoint graph is line graph
	///</summary>
    for(int a=0; a<vertex-1; a++)
    {
        for(int b=a+1; b<vertex; b++)
        {
            for (int c=0; c<vertex; c++)
            {
                if(matrix[a][c]!=0 && matrix[b][c]!=0)
                {
                    for(int d=0; d<vertex; d++)
                    {
                        if(matrix[d][a] != 0 && matrix[d][b] != 0)
                        {
                            return false;
                        }
                    }
                }
            }
        }
    }
    return true;
}

void transformer(int vertex, int **matrix, int **tabkapomocka)
{
	///<summary>
	///Function for transformating adjoint graph into his original graph
	///</summary>
    int l_krawedzi=1;
    for(int a=0; a<vertex; a++)
    {
        if(tabkapomocka[a][0] == 0)
        {
            tabkapomocka[a][0] = l_krawedzi;
            l_krawedzi++;
        }
        if(tabkapomocka[a][1] == 0)
        {
            tabkapomocka[a][1] = l_krawedzi;
            l_krawedzi++;
        }
        for(int c=0; c<vertex; c++)
        {
            if(matrix[c][a]==1)
            {
                tabkapomocka[c][1]=tabkapomocka[a][0];
            }
        }
        for(int b=0; b<vertex; b++)
        {
            if(matrix[a][b]==1)
            {
                tabkapomocka[b][0]=tabkapomocka[a][1];
            }
        }
    }
}

void showInfo(bool checkAdj, bool checkL, bool checkM, int vertex, int **tabkapomocka, int vertex2, int *vertexTab)
{
	///<summary>
	///Function for showing information about result of graph analysis
	///</summary>
    cout<<endl<<"Result of checking: "<<endl;
    if(checkAdj && !checkM)
        cout<<"1. Graph IS an adjoint graph."<<endl;
    if(!checkAdj || checkM)
        cout<<"1. Graph IS NOT an adjoint graph."<<endl;
    if(checkL && checkAdj)
    {
        cout<<"2. Graph IS a line graph."<<endl;
        cout<<endl<<"Original graph: "<<endl<<endl<<"Number of vertices: "<<vertex2<<endl;
        cout<<"List of edges: "<<endl;
        for(int a=0; a<vertex; a++)
        {
            cout<<tabkapomocka[a][0]<<" -> "<<tabkapomocka[a][1]<<"\t"<<"Edge created from vertex: "<<vertexTab[a]<<endl;
        }
    }
    if(!checkL && checkAdj)
    {
        cout<<"2. Graph IS a line graph."<<endl;
        cout<<endl<<"Original graph: "<<endl<<endl<<"Number of vertices: "<<vertex2<<endl;
        cout<<"List of edges: "<<endl;
        for(int a=0; a<vertex; a++)
        {
            cout<<tabkapomocka[a][0]<<" -> "<<tabkapomocka[a][1]<<"\t"<<"Krawedz powstala z wierzcholka: "<<vertexTab[a]<<endl;
        }
    }
}

int countVertex(int **tabkapomocka, int **tabkacopy, int vertex, int vertex2)
{
	///<summary>
	///Function for counting vertices in graph
	///</summary>
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
	///<summary>
	///Function for saving name of vertices into array
	///</summary>
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

void nameIsolatedVertices(int *vertexTab, int vertex)
{
	///<summary>
	///Function for naming isolated vertices of graph
	///</summary>
    for(int i=vertex-1; i>=0; i--)
    {
        if(vertexTab[i] == 0)
        {
            bool notFind = true;
            int name = 1;
            while(notFind)
            {
                for(int j=0; j<vertex; j++)
                {
                    if(name == vertexTab[j])
                    {
                        break;
                    }
                    else if(j==vertex-1)
                    {
                        vertexTab[i] = name;
                        notFind = false;
                    }
                }
                name++;
            }
        }
    }
}

//MAIN====================================================================================================================

int main()
{
    //VARIABLES DEFINITIONS===============================================================================================

    int choose = 1;
    bool checkAdj = false;
    bool checkL = false;
    bool checkM = false;

    int t1=0;
    int t2=0;

    int **matrix;
    int **tabkapomocka;
    int **tabkacopy;
    int *vertexTab;
    int vertex=0;
    int vertex2=0;
    int l_w=0;

    //WCZYTYWANIE=========================================================================================================

    cout<<"Adjoint Graph Checker - Jakub Wajs 2015"<<endl<<endl;
    string name;
    cout<<"Please enter file name (without extension): "<<endl;
    cin>>name;
    string fileName = name + ".txt";

    //Creating matrix V x V where V is number of vertices in a loaded graph
    fstream file;
    file.open(fileName.c_str(), ios::in);
    if(!file.good())
    {
        cout<<endl<<endl<<"ERROR!!! Can't load the file.";
        return 0;
    }

    file>>vertex;

    matrix = new int *[vertex];
    vertexTab = new int [vertex];

    for (int i = 0; i<vertex; i++)
        matrix[i] = new int [vertex];

    for(int i=0; i<vertex; i++)
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

    cout<<endl<<"Analyzed graph: "<<endl<<endl<<"Number of vertices: "<<vertex<<endl;
    cout<<"List of edges: "<<endl;

    //Loading informations about graph edges
    while(!file.eof())
    {
        int k, l;
        file>>k;
        file>>l;

        t1 = saveInVertexTab(vertexTab, vertex, k);
        t2 = saveInVertexTab(vertexTab, vertex, l);

        cout<<vertexTab[t1]<<" -> "<<vertexTab[t2]<<endl;

        //Checking if graph is not a multigraph
        if(matrix[t1][t2]==1)
        {
            checkM = true;
        }
        else
        {
            matrix[t1][t2]=1;
        }
    }
    file.close();

    //Check whether there are unnamed vertices e.g. isolated
    nameIsolatedVertices(vertexTab, vertex);

    //TESTING======================================================================================================

    if(!checkM) checkAdj = checkAdjoint(vertex, matrix);

    if(checkAdj) checkL = checkLine(vertex, matrix);

    //TRANSFORMATION===============================================================================================

    if(checkAdj)
    {
        tabkapomocka = new int *[vertex];

        for (int i = 0; i<vertex; i++)
            tabkapomocka[i] = new int [2];

        for(int i=0; i<vertex; i++)
        {
            for(int j=0; j<2; j++)
            {
                tabkapomocka[i][j]=0;
            }
        }

        transformer(vertex, matrix, tabkapomocka);

        //Counting vertices of oryginal graph

        tabkacopy = new int *[vertex];

        for (int i = 0; i<vertex; i++)
            tabkacopy[i] = new int [2];

        vertex2 = countVertex(tabkapomocka, tabkacopy, vertex, 0);

        //SAVE TO FILE=========================================================================================

        string fileName = name + "-result.txt";
        fstream file;
        file.open(fileName.c_str(), ios::out);
        file<<vertex2<<endl;
        for(int i=0; i<vertex; i++)
        {
            file<<tabkapomocka[i][0]<<" ";
            file<<tabkapomocka[i][1]<<"\n";
        }
        file.close();
    }

    //PRINTING RESULT=========================================================================================

    showInfo(checkAdj, checkL, checkM, vertex, tabkapomocka, vertex2, vertexTab);

    cout<<endl<<"0. Zakoncz program"<<endl<<endl;
    cin>>choose;

    while(choose!=0)
    {
        cout<<"\nBLAD! Wpisano bledna wartosc. Wpisz 0, aby zakonczyc.";
        cin>>choose;
    }

    //RELEASING MEMORY========================================================================================
    if(checkAdj)
    {
        for (int i = 0; i<2; i++)
            delete [] tabkapomocka[i];
        delete [] tabkapomocka;
        for (int i = 0; i<2; i++)
            delete [] tabkacopy[i];
        delete [] tabkacopy;
    }

    for (int i = 0; i<vertex; i++)
        delete [] matrix[i];
    delete [] matrix;

    return 0;
}
