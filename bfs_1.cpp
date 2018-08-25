#include <iostream>
#include <fstream>
#include <cstdlib>
#include <climits>
#include <cstring>
#include <vector>
#include <queue>
#include <iomanip>

using namespace std;

class Grid
{
public:
    // constructor
    Grid()
    {
        _idx = INT_MAX;
        _isObstacle = false;
        _isPath = false;
    }

    // public member functions
    int getX() { return _x; }
    int getY() { return _y; }
    int getIdx() { return _idx; }
    bool isObstacle() { return _isObstacle; }
    bool isPath() { return _isPath; }

    void setX(int x) { _x = x; }
    void setY(int y) { _y = y; }
    void setIdx(int idx) { _idx = idx; }
    void setObstacle() { _isObstacle = true; }
    void setPath() { _isPath = true; }

private:
    // data members
    int _x, _y; // x & y coordinates
    int _idx; // bfs propagation number
    bool _isObstacle; // true if the grid is an obstacle
    bool _isPath; // true if the grid is on the final path
};

class Obstacle
{
public:
    // constructor
    Obstacle(int x1, int y1, int x2, int y2)
        :_x1(x1), _y1(y1), _x2(x2), _y2(y2) {}

    int getX1() { return _x1; }
    int getY1() { return _y1; }
    int getX2() { return _x2; }
    int getY2() { return _y2; }

private:
    int _x1, _y1, _x2, _y2;
};

// global variable
vector< vector<Grid> > map;
vector<Obstacle> obstacles;




// functions
void printMap();
void printIdx();

int main(int argc, char **argv)
{
    if(argc != 2)
    {
        cout << "Usage: ./bfs [input_case]" << endl;
        system("pause");
        exit(EXIT_FAILURE);
    }

    /* open the input file */
    fstream inFile;
    inFile.open(argv[1], fstream::in);
    if(!inFile.is_open())
    {
        cout << "The input file is not opened!" << endl;
        system("pause");
        exit(EXIT_FAILURE);
    }

    /* parse the input file */
    char buffer[100];
    int boundaryX, boundaryY;
    int sourceX, sourceY;
    int targetX, targetY;
	
	
    while(inFile >> buffer)
    {
        if(strcmp(buffer, "boundary") == 0)
        {
            inFile >> buffer;
            boundaryX = atoi(buffer);
            inFile >> buffer;
            boundaryY = atoi(buffer);
        }
        else if(strcmp(buffer, "source") == 0)
        {
            inFile >> buffer;
            sourceX = atoi(buffer);
            inFile >> buffer;
            sourceY = atoi(buffer);
        }
        else if(strcmp(buffer, "target") == 0)
        {
            inFile >> buffer;
            targetX = atoi(buffer);
            inFile >> buffer;
            targetY = atoi(buffer);
        }
        else if(strcmp(buffer, "obstacles") == 0)
        {
            inFile >> buffer;
            int numObstacles = atoi(buffer);
            for(int i = 0; i < numObstacles; ++i)
            {
                inFile >> buffer;
                int x1 = atoi(buffer);
                inFile >> buffer;
                int y1 = atoi(buffer);
                inFile >> buffer;
                int x2 = atoi(buffer);
                inFile >> buffer;
                int y2 = atoi(buffer);
                obstacles.push_back(Obstacle(x1, y1, x2, y2));
            }
        }
        else
        {
            cout << "Unmatched string in the input file!" << endl;
        }
    }

    /* initialize the routing map */
    map.resize(boundaryX);
    for(int i = 0; i < boundaryX; ++i)
    {
        map[i].resize(boundaryY);
    }

    for(int i = 0; i < boundaryX; ++i)
    {
        for(int j = 0; j < boundaryY; ++j)
        {
            map[i][j].setX(i);
            map[i][j].setY(j);
            if((i == sourceX && j == sourceY) || (i == targetX && j == targetY))
                map[i][j].setPath();
        }
    }

    for(int i = 0; i < obstacles.size(); ++i)
    {
        for(int j = obstacles[i].getX1(); j <= obstacles[i].getX2(); ++j)
        {
            for(int k = obstacles[i].getY1(); k <= obstacles[i].getY2(); ++k)
            {
                map[j][k].setObstacle();
            }
        }
    }

    cout << "--------------------" << endl;
    cout << "   Initialization" << endl;
    cout << "--------------------" << endl;
    cout << "Routing map" << endl;
    printMap();
    cout << "Index map:" << endl;
    printIdx();

    /* BFS propagation */

 
 	queue<Grid *> q;
    map[sourceX][sourceY].setIdx(0);
    q.push(&map[sourceX][sourceY]);
    
    // To Do
    
    // hint: as you want to pop an element from the queue, 
    // you may use the following code:
    //     Grid *g = q.front();
    //     
	const int dx[4] = {1, 0, -1, 0};
	const int dy[4] = {0, 1, 0, -1};
			
	
	while(map[targetX][targetY].getIdx()==INT_MAX)
	{
		Grid *g = q.front();
		if(g->getX() < boundaryX-1 && g->getX()>0 && g->getY() < boundaryY && g->getY()>=0)
		for(int i=0; i<4; ++i)
		{
			if(map[g->getX()+dx[i]][g->getY()+dy[i]].getIdx() == INT_MAX && map[g->getX()+dx[i]][g->getY()+dy[i]].isObstacle() == false) 
			{
				map[g->getX()+dx[i]][g->getY()+dy[i]].setIdx(g->getIdx()+1);
				q.push(&map[(g->getX())+dx[i]][(g->getY())+dy[i]]);		
			}
		}
		q.pop();  
	}
    /* BFS trace back */
    // To Do
  	
     while(!q.empty()) 
        q.pop();
        
    q.push(&map[targetX][targetY]);
    
    while(q.front()->getIdx()!=0)
    {
        Grid *g = q.front();
		for(int i=0; i<4; ++i)
        {
			
			if(map[g->getX()+dx[i]][g->getY()+dy[i]].getIdx()==map[g->getX()][g->getY()].getIdx()-1)
		    {
		    	map[g->getX()+dx[i]][g->getY()+dy[i]].setPath();	
		        q.push(&map[g->getX()+dx[i]][g->getY()+dy[i]]);
		    }
			if(map[g->getX()+dx[i]][g->getY()+dy[i]].getIdx() < map[g->getX()][g->getY()].getIdx()) break;			

		}
        q.pop();                                                                                                                                                      
    } 
    
	cout << "Routing map" << endl;
    printMap();
    cout << "Index map:" << endl;
    printIdx();



    system("pause");
    return 0;
}

void printMap()
{
    for(int i = 0; i < map.size(); ++i)
    {
        for(int j = 0; j < map[0].size(); ++j)
        {
            if(map[i][j].isObstacle())
                cout << setw(2) << "#";
            else if(map[i][j].isPath())
                cout << setw(2) << "*";
            else
                cout << setw(2) << "-";
        }
        cout << endl;
    }
}

void printIdx()
{
    for(int i = 0; i < map.size(); ++i)
    {
        for(int j = 0; j < map[0].size(); ++j)
        {
            if(map[i][j].getIdx() == INT_MAX)
                cout << setw(2) << "x";
            else
                cout << setw(2) << map[i][j].getIdx();
        }
        cout << endl;
    }
}
