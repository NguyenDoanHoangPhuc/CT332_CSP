#include <bits/stdc++.h>
using namespace std;

#define MAX_REGION 100
#define NO_COLOR 0
#define MAX_COLOR 3

#define MAXN 100
typedef struct {
	int A[MAXN][MAXN];
	int n;
} Constrains;

void initConstrains(Constrains *con, int n){
	con->n = n;
	for (int i = 1; i <= n; i++)
		for (int j = 1; j <= n; j++)
			con->A[i][j] = 0;
}

int addConstrain(Constrains *con, int u, int v){
	if (con->A[u][v] == 0){
		con->A[u][v] = 1;
		con->A[v][u] = 1;
		return 1;
	}
	return 0;
}

vector<int> getConstrains(Constrains *con, int u){
	vector<int> result;
	for (int v = 1; v <= con->n; v++)
		if (con->A[u][v] != 0) result.push_back(v);
	return result;
}

void printConstrains(Constrains con){
	for (int i = 1; i <= con.n; i++){
		for (int j = 1; j <= con.n; j++){
			printf("%3d", con.A[i][j]);
		}
		printf("\n");
	}
}

int degree(Constrains *constrain, int x){
	int deg = 0;
	for (int i = 1; i <= constrain->n; i++)
		if (constrain->A[x][i] != 0) deg++;
	return deg;
}


typedef struct {
	int color[MAX_REGION];
	int n;
	Constrains constrain;
} Map;

void initMap(Map *map_value, int n){
	map_value->n = n;
	for (int i = 1; i <= n; i++)
		map_value->color[i] = NO_COLOR;
	initConstrains(&map_value->constrain, n);
}

const char* color_name[] = {
	"No color",
	"Red",
	"Yellow",
	"Blue"
};

void printMap(Map map){
	for (int i = 1; i <= map.n; i++)
		printf("Region %d: %s\n", i, color_name[map.color[i]]);
}

int isFullMap(Map map){
	for (int i = 1; i <= map.n; i++)
		if (map.color[i] == NO_COLOR) return 0;
	return 1; 
}


typedef vector<int>::iterator VectorIndex;

vector<int> getAvailableValues(int id, Constrains* constrain, Map map){
	vector<int> posList = getConstrains(constrain, id);
	
	int availables[MAX_COLOR+1];
	for (int i = 1; i <= MAX_COLOR; i++) availables[i] = 1;
	
	for (VectorIndex it = posList.begin(); it != posList.end(); it++){
		if (map.color[*it] != NO_COLOR)
			availables[map.color[*it]] = 0;
	}
	
	vector<int> result;
	
	for (int i = 1; i <= MAX_COLOR; i++)
		if (availables[i] == 1) result.push_back(i);
	
	return result;
}

int getNextLocation(Map map){
	for (int i = 1; i <= map.n; i++)
		if (map.color[i] == NO_COLOR) return i; 
}

int getNextLocation1(Map map){
	int maxDeg = -1;
	int maxPosition = 1;
	for (int i = 1; i <= map.n; i++)
		if (map.color[i] == NO_COLOR && degree(&map.constrain, i) > maxDeg){
			maxDeg = degree(&map.constrain, i);
			maxPosition = i;
		}
	return maxPosition;
}

int exploredCounter = 0;
int backTracking(Map *map){
	Constrains constrain = map->constrain;
	if (isFullMap(*map)) return 1;
	int location = getNextLocation1(*map);
	vector<int> availables = getAvailableValues(location, &constrain, *map);
	
	if (availables.size() == 0) return 0;
	
	for (VectorIndex it = availables.begin(); it != availables.end(); it++){
		int value = *it;
		map->color[location] = value;
		exploredCounter++;
		if (backTracking(map)) return 1;
		map->color[location] = NO_COLOR;
	}
	return 0;
}

Map solveMap(Map map){
	exploredCounter = 0;
	if (backTracking(&map)) printf("Solved\n");
	else printf("Can not solved!\n");
	printf("Explored counter: %d\n", exploredCounter);
	return map;
}


int main(){
	int n,m ;
	freopen("test.txt", "r", stdin);
	scanf("%d%d", &n, &m);

	Map map;
	initMap(&map, n);
	
	for (int i = 0; i < m; i++){
		int u, v;
		scanf("%d%d", &u, &v);
		addConstrain(&(map.constrain), u, v);
	}
	
	Map result = solveMap(map);
	printMap(result);
	return 0;
}


