#define Inf 10000000
#define K 6 // K=level, depth tree
#define N 5

typedef struct Node {
  char board[N][N];
  struct Node **sons;
  int n_sons;
  double value;
} Node;

Node *createNode(Node *father, int numSon, int level);
Node *createRoot(char table[N][N]);
void showValue(Node *p, int level);

double Min_Value(Node *p, double alpha, double beta, int level);
double Max_Value(Node *p, double alpha, double beta, int level);

void copyBoard(char board[N][N], char father_board[N][N]);
int numSonToColumn(char board[N][N], int numSon);
int numSons(char board[N][N]);
void tokenRoll(char board[N][N], int numSon, int level);
void putToken(char table[N][N], int column, int player);
void lastTokenColumn(int v[N], char table[N][N]);
int checkLineHeur(int a1, int a2, int a3, int a4, int player,
                  int lengthConnect);
int resultTableHeur(char table[N][N], int player, int lengthConnect);
int functionHeur(char table[N][N]);
int tossRoot(Node *p);
int getBestColumn(char table[N][N]);