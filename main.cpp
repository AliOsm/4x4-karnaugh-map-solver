#include <stdio.h> // printf, scanf, puts, putchar
#include <utility> // pair
#include <string>  // string

using namespace std;

int const oo = -10;

// Functions prototypes
void input(int n, int indexes[16], int k_indexes[]);
pair<int, int> check(int width, int length, int indexes[16], int k_indexes[], bool vis[4][4][5][5]);
bool allOnes(int i, int j, int width, int length, int indexes[16], int k_indexes[]);
void setVisited(int i, int j, int width, int length, bool vis[4][4][5][5]);
bool visited(int i, int j, int width, int length, bool vis[4][4][5][5]);
void printGroup(int groupNumber, int i, int j, int width, int length, int k_indexes[]);
string ith(int i);
void exit(string message);

// Main program
int main() {
  bool vis[4][4][5][5] = {0};

  int n;
  int indexes[16] = {0};
  int k_indexes[] = {0, 1, 3, 2, 4, 5, 7, 6, 12, 13, 15, 14, 8, 9, 11, 10};

  input(n, indexes, k_indexes);

  int groupCounter = 1;
  pair<int, int> xy = make_pair(oo, oo);

  int k_sizes[][2] = {{4, 4}, {2, 4}, {4, 2}, {1, 4}, {4, 1}, {2, 2}, {1, 2}, {2, 1}, {1, 1}};
  for(int i = 0, e = sizeof(k_sizes) / sizeof(int) / 2; i < e; ++i)
    do {
      xy = check(k_sizes[i][0], k_sizes[i][1], indexes, k_indexes, vis);
      if(xy.first != oo && xy.second != oo)
        printGroup(groupCounter++, xy.first, xy.second, k_sizes[i][0], k_sizes[i][1], k_indexes);
    } while(xy.first != oo && xy.second != oo);

  return 0;
}

// Takes input
void input(int n, int indexes[16], int k_indexes[]) {
  puts("Enter the number of one's in the Karnaugh map:");
  scanf("%d", &n);

  if(n < 0 || n > 16)
    exit("Wrong argument value: `n` should be between [0, 16]");

  for(int i = 0, tmp; i < n; ++i) {
    printf("Enter the index of the %s one:\n", ith(i + 1).c_str());
    scanf("%d", &tmp);
    if(tmp < 0 || tmp > 15)
      exit("Wrong argument value: `tmp` should be between [0, 15]");
    indexes[tmp] = 1;
  }

  string bin[4] = {"00", "01", "11", "10"};

  puts("\nYour current Karnaugh map:");
  puts("  WX");
  printf("YZ    ");
  for(int i = 0; i < 4; ++i) {
    if(i != 0)
      putchar(' ');
    printf("%s", bin[i].c_str());
  }
  putchar('\n');
  puts("     +-----------");
  for(int i = 0; i < 4; ++i) {
    printf("  %s |", bin[i].c_str());
    for(int j = 0; j < 4; ++j) {
      if(j != 0)
        putchar(' ');
      printf("%2d", indexes[k_indexes[i * 4 + j]]);
    }
    putchar('\n');
  }
  putchar('\n');
}

/*
  Check if there is any rectangle with `width` and `length`
  contains only ones in it.

  Returns the coordinates of the first appeared rectangle if exist,
  `(oo, oo)` otherwise.
*/
pair<int, int> check(int width, int length, int indexes[16], int k_indexes[], bool vis[4][4][5][5]) {
  for(int i = 0; i < 4; ++i)
    for(int j = 0; j < 4; ++j)
      if(!visited(i, j, width, length, vis)) {
        if(allOnes(i, j, width, length, indexes, k_indexes)) {
          setVisited(i, j, width, length, vis);
          return make_pair(i, j);
        }

        if(allOnes(i - width + 1, j, width, length, indexes, k_indexes)) {
          setVisited(i - width + 1, j, width, length, vis);
          return make_pair(i - width + 1, j);
        }

        if(allOnes(i - width + 1, j - length + 1, width, length, indexes, k_indexes)) {
          setVisited(i - width + 1, j - length + 1, width, length, vis);
          return make_pair(i - width + 1, j - length + 1);
        }

        if(allOnes(i, j - length + 1, width, length, indexes, k_indexes)) {
          setVisited(i, j - length + 1, width, length, vis);
          return make_pair(i, j - length + 1);
        }
      }

  return make_pair(oo, oo);
}

/*
  Check if there is a rectangle starts at cell `(i, j)`
  with `width` and `length` contains only ones in it.

  Returns true if there is a rectangle starts at cell `(i, j)`
  with `width` and `length` contains only ones in it,
  false otherwise.
*/
bool allOnes(int i, int j, int width, int length, int indexes[16], int k_indexes[]) {
  for(int x = i; x < i + width; ++x)
    for(int y = j; y < j + length; ++y)
      if(indexes[k_indexes[((x + 4) % 4) * 4 + ((y + 4) % 4)]] == 0)
        return false;

  return true;
}

/*
  Takes rectangle starts at cell `(i, j)`
  with `width` and `length` and set it to visited
*/
void setVisited(int i, int j, int width, int length, bool vis[4][4][5][5]) {
  for(int x = i; x < i + width; ++x)
    for(int y = j; y < j + length; ++y)
      vis[((x + 4) % 4)][((y + 4) % 4)][width][length] = true;
}

/*
  Check if cell `(i, j)` visited before with larger
  `width` and `length`

  Returns true if cell `(i, j)` visited before with larger
  `width` and `length`, false otherwise.
*/
bool visited(int i, int j, int width, int length, bool vis[4][4][5][5]) {
  for(int x = 1; x < 5; ++x)
    for(int y = 1; y < 5; ++y)
      if(vis[i][j][x][y])
        return true;
  return false;
}

/*
  Takes group information and print them
*/
void printGroup(int groupNumber, int i, int j, int width, int length, int k_indexes[]) {
  printf("Group #%d:\n", groupNumber);
  for(int x = i; x < i + width; ++x)
    for(int y = j; y < j + length; ++y) {
      if(x != i || y != j)
        putchar(' ');
      printf("%d", k_indexes[((x + 4) % 4) * 4 + ((y + 4) % 4)]);
    }
  putchar('\n');
}

/*
  Takes integer `i` and return it with the
  ordinal number suffix added to it.
*/
string ith(int i) {
  if(i == 1)
    return "1st";
  if(i == 2)
    return "2nd";
  if(i == 3)
    return "3rd";

  string on = "";
  on += char('0' + i);
  on += "th";
  return on;
}

/*
  Takes `message`, print it then exit from the program
*/
void exit(string message) {
  printf("%s\n", message.c_str());
  exit(0);
}
