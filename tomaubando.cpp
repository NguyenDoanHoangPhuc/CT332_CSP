#include <bits/stdc++.h>
using namespace std;

#define MAX_REGION 20
#define NO_COLOR 0
#define MAX_COLOR 3
// Cai dat cac bien va mien gia tri
// Khai bao cau truc Map gom color (gia tri cua bien) va n (tong so bien)
typedef struct {
	int color[MAX_REGION];
	int n;
} Map;

// Ham khoi tao gan cac gia tri = NO_COLOR
void initMap(Map *map_value, int n){
	map_value->n = n;
	
	for (int region = 1; region <= n; region++)
		map_value->color[region] = NO_COLOR;
}

// Tao mau sac, 0 = red, 1 = blue, 2 = yellow
const char* color_name[] = {
	"No color",
	"Red",
	"Blue",
	"Yellow"
};

// In ban do
void printMap(Map map){
	for (int region = 1; region <= map.n; region++)
		printf("Region %d: %s\n", region, color_name[map.color[region]]);
}


// Kiem tra xem da to het ban do hay chua
int isFullMap(Map map){
	for (int region = 1; region <= map.n; region++)
		if (map.color[region] == NO_COLOR) return 0;
	return 1; 
}


// Cai dat ranh buoc
// Cau truc ranh buoc la mot ma tran ke cua do thi ranh buoc
typedef struct {
	int A[MAX_REGION][MAX_REGION];
	int n;
} Constrains;

// Khoi tao ranh buoc
void initConstrains(Constrains *con, int n){
	con->n = n;
	for (int region = 1; region <= n; region++)
		for (int j = 1; j <= n; j++)
			con->A[region][j] = 0;
}

// Them ranh buoc
int addConstrain(Constrains *con, int u, int v){
	if (con->A[u][v] == 0){
		con->A[u][v] = 1;
		con->A[v][u] = 1;
		return 1;
	}
	return 0;
}

// Lay nhung dinh ranh buoc voi dinh da cho
vector<int> getConstrains(Constrains *con, int u){
	vector<int> result;
	result.clear();
	for (int v = 1; v <= con->n; v++)
		if (con->A[u][v] != 0) result.push_back(v);
	return result;
}

// In ranh buoc
void printConstrains(Constrains *con){
	for (int region = 1; region <= con->n; region++){
		for (int j = 1; j <= con->n; j++){
			printf("%3d", con->A[region][j]);
		}
		printf("\n");
	}
}

// Tinh bac cua dinh
int degree(Constrains constrain, int x){
	int deg = 0;
	for (int region = 1; region <= constrain.n; region++)
		if (constrain.A[x][region] != 0) deg++;
	return deg;
}

// Lay nhung gia tri ma dinh co the gan
typedef vector<int>::iterator VectorIndex;

vector<int> getAvailableValues(Constrains* constrain, Map* map, int id){
	vector<int> posList = getConstrains(constrain, id);
	
	int availables[MAX_COLOR+1];
	for (int region = 1; region <= MAX_COLOR; region++)
		availables[region] = 1;
	
	for (VectorIndex it = posList.begin(); it != posList.end(); it++){
		if (map->color[*it] != NO_COLOR)
			availables[map->color[*it]] = 0;
	}
	
	vector<int> result;
	
	for (int region = 1; region <= MAX_COLOR; region++)
		if (availables[region] == 1) result.push_back(region);
	
	return result;
}

// Chon dinh theo phuong phap dinh chua to dau tien
int getNextRegion1(Map map){
	for (int region = 1; region <= map.n; region++)
		if (map.color[region] == NO_COLOR) return region; 
}

// Chon dinh theo phuong phap dinh co bac lon nhat
int getNextRegion2(Map map, Constrains constrain){
	int maxDeg = -1;
	int maxRegion = 0;
	
	for (int r = 1; r <= map.n; r++){
		int deg = degree(constrain, r);
		if (map.color[r] == NO_COLOR && deg > maxDeg){
			maxDeg = deg;
			maxRegion = r;
		}
	}
	return maxRegion;
}

// Ham quay lui
int exploredCounter;

int backTracking(Map *map, Constrains* constrain){

	if (isFullMap(*map)) return 1;

	// int region = getNextRegion1(*map);
	int region = getNextRegion2(*map, *constrain);
	
	vector<int> availables = getAvailableValues(constrain, map, region);

	if (availables.size() == 0) return 0;
	
	for (VectorIndex it = availables.begin(); it != availables.end(); it++){
		int value = *it;
		exploredCounter++;
		map->color[region] = value;
		if (backTracking(map, constrain)) return 1;
		map->color[region] = NO_COLOR;
	}
	return 0;
}

Map solveMap(Map *map, Constrains *constrain){
	exploredCounter = 0;
	if (backTracking(map, constrain)) printf("Solved\n");
	else printf("Can not solved!\n");
	printf("Explored counter: %d\n", exploredCounter);
	return *map;
}


int main(){
	ifstream file("test.txt"); 
	
    if (!file) {
        printf("File khong hop le!\n");
        return 1;
    }

 	int n = 0; 
    int m = 0;
	
	file >> n;

    int a, b;
    int x[MAX_REGION*MAX_REGION], y[MAX_REGION*MAX_REGION];
    while (file >> a >> b) {
        n = max(n, max(a, b));
        x[m] = a;
        y[m] = b;
        m++; 
    }

    file.close(); 
	
	Map map;
	initMap(&map, n);
	
	Constrains cons;
	initConstrains(&cons, n);
	
	for (int i = 0; i < m; i++)
		addConstrain(&cons, x[i], y[i]);
	
	printMap(solveMap(&map, &cons));
	
	return 0;
}


