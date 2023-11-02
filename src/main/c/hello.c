#include <jni.h>
#include "com_example_demo_HelloJNI.h"
#include "minimax.h"
#include <stdio.h>
#include <stdlib.h>


double Max_Value(Node *p, double alpha, double beta, int level) {

  if (p->n_sons == 0)
    return (functionHeur(p->board));

  p->value = -Inf;
  double v1;

  for (int i = 0; i < p->n_sons; i++) {
    p->sons[i] = createNode(p, i, level + 1);
    v1 = Min_Value(p->sons[i], alpha, beta, level + 1);

    if (v1 > p->value)
      p->value = v1;

    if (v1 >= beta) {
      double v = p->value;
      free(p->sons[i]->sons);
      free(p->sons[i]);
      return v;
    }

    if (v1 > alpha)
      alpha = v1;

    if (level != 0) { // don't kill the sons of the root, because is needed
      free(p->sons[i]->sons);
      free(p->sons[i]);
    }
  }

  return p->value;
}

double Min_Value(Node *p, double alpha, double beta, int level) {
  if (p->n_sons == 0)
    return (functionHeur(p->board));

  p->value = +Inf;
  double v1;

  for (int i = 0; i < p->n_sons; i++) {
    p->sons[i] = createNode(p, i, level + 1);
    v1 = Max_Value(p->sons[i], alpha, beta, level + 1);

    if (v1 < p->value)
      p->value = v1;

    if (v1 <= alpha) {
      double v = p->value;
      free(p->sons[i]->sons);
      free(p->sons[i]);
      return v;
    }

    if (v1 < beta)
      beta = v1;

    free(p->sons[i]->sons);
    free(p->sons[i]);
  }

  return p->value;
}

Node *createNode(Node *father, int numSon, int level) {
  Node *p = malloc(sizeof(Node));
  copyBoard(p->board, father->board);
  tokenRoll(p->board, numSon,
            level); // numSon is the son's number, that goes between  0 to ...

  if (level < K) {
    p->n_sons = numSons(p->board);
    p->sons = malloc(p->n_sons * sizeof(Node *));
  } else {
    p->n_sons = 0;
    p->sons = NULL; // malloc(0);
  }

  return p;
}

Node *createRoot(char table[N][N]) {
  Node *p = malloc(sizeof(Node));
  copyBoard(p->board, table);
  p->n_sons = numSons(p->board);
  p->sons = malloc(p->n_sons * sizeof(Node *));

  return p;
}

void copyBoard(char board[N][N], char father_board[N][N]) {

  int i, j;

  for (i = 0; i < N; i++) {
	for (j = 0; j < N; j++) {
	  board[i][j] = father_board[i][j];
	}
  }
}


int numSons(char board[N][N]) {

  int v[N];
  lastTokenColumn(v, board);
  int count = 0;

  for (int i = 0; i < N; i++) {
	if (v[i] != -1) {
	  count++;
	}
  }

  return count;
}


void putToken(char table[N][N], int column, int player) {

  int i;

  if (!(0 <= column && column < N)) {
	printf("column erronea\n");
	return;
  }

  for (i = N - 1; i >= 0; i--)
	if (table[i][column] == 0) {
	  table[i][column] = player;
	  return;
	}
}


void lastTokenColumn(int v[N], char table[N][N]) {

  int i;

  for (int j = 0; j < N; j++) {
	v[j] = -1; // column j is full until the contrary proven

	for (i = N - 1; i >= 0; i--) {
	  if (table[i][j] == 0) {
		v[j] = i;
		break;
	  }
	}
  }
}


void showValue(Node *p, int level) {
  for (int i = 0; i < level; i++)
    printf("\t");

  printf("%f\n", p->value);
}

// start function heur
int checkLineHeur(int a1, int a2, int a3, int a4, int player,
                  int lengthConnect) {
  switch (lengthConnect) {
  case 2:
    return (a1 == player && a2 == player && a3 == 0) +
           (a2 == player && a3 == player && a4 == 0) +
           (a1 == 0 && a2 == player && a3 == player) +
           (a2 == 0 && a3 == player && a4 == player);
  case 3:
    return (a1 == player && a2 == player && a3 == player && a4 == 0) +
           (a1 == 0 && a2 == player && a3 == player && a4 == player);
  case 4:
    return (a1 == player && a2 == player && a3 == player && a4 == player);
  }

  return 0; // just in case
}

int resultTableHeur(
    char table[N][N], int player,
    int lengthConnect) { // player 1 1, player 2 2, draw 3, nothing 4
  int i, j;
  int p = 0;

  for (j = 0; j < N; j++)
    for (i = 0; i <= N - 4; i++) {
      p += checkLineHeur( // by columns
          table[i][j], table[i + 1][j], table[i + 2][j], table[i + 3][j],
          player, lengthConnect);

      p += checkLineHeur( // by rows
          table[j][i], table[j][i + 1], table[j][i + 2], table[j][i + 3],
          player, lengthConnect);
    }

  for (i = 0; i <= N - 4; i++) // by diagonal inclined down
    for (j = 0; j <= N - 4; j++)
      p += checkLineHeur(table[i][j], table[i + 1][j + 1], table[i + 2][j + 2],
                         table[i + 3][j + 3], player, lengthConnect);

  for (i = 3; i < N; i++) // by diagonal inclined up
    for (j = 0; j <= N - 4; j++)
      p += checkLineHeur(table[i][j], table[i - 1][j + 1], table[i - 2][j + 2],
                         table[i - 3][j + 3], player, lengthConnect);

  return p;
}

int functionHeur(char table[N][N]) {

  if (K == 2) {
    if (resultTableHeur(table, 2, 4))
      return 100000;
    else if (resultTableHeur(table, 1, 4))
      return -100000 * resultTableHeur(table, 1, 4);
  }

  if (resultTableHeur(table, 1, 4)) // if human makes connect 4
    return -100000 * resultTableHeur(table, 1, 4);
  else if (resultTableHeur(table, 2, 4)) // if machine makes connect 4
    return 100000;

  int p = 0;
  p += resultTableHeur(table, 2, 3) * 10;
  // penalize if human makes connect 3
  p += resultTableHeur(table, 1, 3) * (-10);
  p += resultTableHeur(table, 2, 2) * 2;
  // penalize if human makes connect 4
  p += resultTableHeur(table, 1, 2) * (-2);

  return p;
}
// End function heur

int tossRoot(Node *p) { // what column toss before  minimax
  double m;
  int j = 0;
  m = p->sons[0]->value;

  for (int i = 1; i < p->n_sons; i++)
    if (p->sons[i]->value > m) {
      m = p->sons[i]->value;
      j = i;
    }

  // the son number j what column correspond to the p->board?
  return numSonToColumn(p->board, j);
}
int numSonToColumn(char board[N][N], int numSon) { // to respond
  // what father's column is son's number numSon?
  int v[N];
  lastTokenColumn(v, board);
  int count = -1; // is important to undertant the why

  for (int i = 0; i < N; i++) {
	if (v[i] != -1) { // ex v=[-1,1(son num 0),-1,4(son num 1)]
	  count++;
	}

	if (count == numSon) {
	  return i;
	}
  }

  return 0; // to avoid error
}

void tokenRoll(char board[N][N], int numSon,
               int level) { // numSon starts with 0

  int column = numSonToColumn(board, numSon);
  putToken(board, column, level % 2 + 1);
}

int getBestColumn(char table[N][N]) { // minimax
  Node *root;
  int alpha = -Inf, beta = Inf;
  root = createRoot(table);

  Max_Value(root, alpha, beta, 0);
  int column = tossRoot(root);

  for (int i = 0; i < root->n_sons; i++) {
    free(root->sons[i]->sons);
    free(root->sons[i]);
  }
  free(root->sons);
  free(root);

  return column;
}

JNIEXPORT void JNICALL Java_com_example_demo_HelloJNI_sayHello
  (JNIEnv *env, jobject thisObj) {
   printf("Hello World from C!\n");
   return;
}

JNIEXPORT jint JNICALL Java_com_example_demo_HelloJNI_sumMatrix
  (JNIEnv *env, jobject thisObj, jobjectArray matrix) {
    char c_matrix[N][N];
    int size = N;
    for (int i = 0; i < size; i++) {
        jcharArray row = (jcharArray)(*env)->GetObjectArrayElement(env, matrix, i);
        jchar *rowElements = (*env)->GetCharArrayElements(env, row, 0);
        for (int j = 0; j < size; j++) {
            c_matrix[i][j] = (char) rowElements[j];
        }
        (*env)->ReleaseCharArrayElements(env, row, rowElements, 0);
    }

    return getBestColumn(c_matrix);
}

