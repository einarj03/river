#include <iostream>
#include <fstream>
#include <cstring>
#include <cassert>

using namespace std;

#include "river.h"

/* You are pre-supplied with the functions below. Add your own 
   function definitions to the end of this file. */

/* internal helper function which allocates a dynamic 2D array */
char **allocate_2D_array(int rows, int columns) {
  char **m = new (nothrow) char *[rows];
  assert(m);
  for (int r=0; r<rows; r++) {
    m[r] = new (nothrow) char[columns];
    assert(m[r]);
  }
  return m;
}

/* internal helper function which deallocates a dynamic 2D array */
void deallocate_2D_array(char **m, int rows) {
  for (int r=0; r<rows; r++)
    delete [] m[r];
  delete [] m;
}

/* pre-supplied function which creates an empty ASCII-art scene */
char **create_scene() {
  char **scene = allocate_2D_array(SCENE_HEIGHT, SCENE_WIDTH);

  for (int i=0; i<SCENE_HEIGHT; i++) 
    for (int j=0; j<SCENE_WIDTH; j++)
      scene[i][j] = ' ';

  return scene;
}

/* pre-supplied function which frees up memory allocated for an ASCII-art scene */
void destroy_scene(char **scene) {
  deallocate_2D_array(scene, SCENE_HEIGHT);
}

/* pre-supplied function which displays an ASCII-art scene */
void print_scene(char **scene) {
  for (int i=0; i<SCENE_HEIGHT; i++) {
    for (int j=0; j<SCENE_WIDTH; j++)
      cout << scene[i][j];
    cout << endl;
  }
}

/* helper function which removes carriage returns and newlines from strings */
void filter(char *line) {
  while (*line) {
    if (*line >= ' ')
      line++;
    else 
      *line = '\0';
  }
}

/* pre-supplied function which inserts an ASCII-art drawing stored in a file
   into a given ASCII-art scene starting at coordinates (row,col)  */
bool add_to_scene(char **scene, int row, int col, const char *filename) {

  ifstream in(filename);
  if (!in)
    return false;

  int start_col = col;
  char line[512];
  in.getline(line,512);
  filter(line);
  while (!in.fail()) {
    for (int n=0; n<strlen(line); n++) {
      if (row >= SCENE_HEIGHT)
	return false;
      if (col >= SCENE_WIDTH)
	break;
      scene[row][col++] = line[n];
    }
    row++;
    col = start_col;
    in.getline(line,512);
    filter(line);
  }
  return true;
}

/* pre-supplied helper function to report the status codes encountered in Question 3 */
const char *status_description(int code) {
  switch(code) {
  case ERROR_INVALID_CANNIBAL_COUNT:
    return "Invalid cannibal count";
  case ERROR_INVALID_MISSIONARY_COUNT:
    return "Invalid missionary count";
  case ERROR_INVALID_MOVE:
    return "Invalid move";
  case ERROR_MISSIONARIES_EATEN:
    return "The missionaries have been eaten! Oh dear!";
  case ERROR_INPUT_STREAM_ERROR:
    return "Unexpected input stream error";
  case ERROR_BONUS_NO_SOLUTION:
    return "No solution";
  case VALID_GOAL_STATE:
    return "Problem solved!";
  case VALID_NONGOAL_STATE:
    return "Valid state, everything is OK!";
  }
  return "Unknown error";
}


/* insert your functions here */

char** make_river_scene(const char left[7], const char boat[2]) {
  char **scene = create_scene();
  add_to_scene(scene, 0, 0, "bank.txt");
  add_to_scene(scene, 0, 53, "bank.txt");
  add_to_scene(scene, 3, 30, "sun.txt");
  add_to_scene(scene,19, 19, "river.txt");

  bool boat_right = true;
  int mis_count = 0;
  int can_count = 0;
  int boat_mis_count = 0;
  int boat_can_count = 0;

  count_people(left, boat, mis_count, can_count, boat_mis_count, boat_can_count, boat_right);

  int boat_count = 0;
  for (int i = 0; i < boat_mis_count; ++i) {
    add_to_scene(scene, 11, 22 + boat_right * 17 + 6 * boat_count, "missionary.txt");
    ++boat_count;
  }
  
  for (int i = 0; i < boat_can_count; ++i) {
    add_to_scene(scene, 11, 22 + boat_right * 17 + 6 * boat_count, "cannibal.txt");
    ++boat_count;
  }

  add_to_scene(scene, 17, 19 + boat_right * 17, "boat.txt");

  for (int i = 0; i < mis_count; ++i)
    add_to_scene(scene, 2, 1 + 6*i, "missionary.txt");

  for (int i = 0; i < can_count; ++i)
    add_to_scene(scene, 11, 1 + 6*i, "cannibal.txt");

  for (int i = 0; i < 3 - mis_count - boat_mis_count; ++i)
    add_to_scene(scene, 2, 54 + 6*i, "missionary.txt");

  for (int i = 0; i < 3 - can_count - boat_can_count; ++i)
    add_to_scene(scene, 11, 54 + 6*i, "cannibal.txt");

  return scene;
}

Status perform_crossing(char left[7], const char targets[2]) {
  bool boat_right = true;
  int mis_count = 0;
  int can_count = 0;
  int target_mis_count = 0;
  int target_can_count = 0;

  for (int i = 0; i < strlen(targets); ++i) {
    if (targets[i] != 'B' && targets[i] != 'M' && targets[i] != 'C')
      return ERROR_INPUT_STREAM_ERROR;
  }

  count_people(left, targets, mis_count, can_count, target_mis_count, target_can_count, boat_right);

  int left_mis;
  int left_can;
  int right_mis;
  int right_can;

  if (boat_right) {
    left_mis = mis_count + target_mis_count;
    left_can = can_count + target_can_count;
    right_mis = 3 - left_mis;
    right_can = 3 - left_can;  
  } else {
    left_mis = mis_count - target_mis_count;
    left_can = can_count - target_can_count;
    right_mis = 3 - left_mis;
    right_can = 3 - left_can;
  }

  if (left_mis < 0 || left_can < 0 || right_mis < 0 || right_can < 0 || target_can_count + target_mis_count == 0)
    return ERROR_INVALID_MOVE;

  strcpy(left, boat_right ? "B" : "");
  for (int i = 0; i < left_mis; ++i)
    strcat(left, "M");

  for (int i = 0; i < left_can; ++i)
    strcat(left, "C");

  if ((left_can > left_mis && left_mis != 0) || (right_can > right_mis && right_mis != 0))
    return ERROR_MISSIONARIES_EATEN;

  if (right_mis == 3 && right_can == 3)
    return VALID_GOAL_STATE;

  return VALID_NONGOAL_STATE;
}

void count_people(const char left[7], const char boat[2], int &mis_count, int &can_count, int &boat_mis_count, int &boat_can_count, bool &boat_right) {
  for (int i = 0; i < strlen(left); ++i) {
    switch (left[i]) {
      case 'B': boat_right = false; break;
      case 'M': ++mis_count; break;
      case 'C': ++can_count; break;
    }
  }

  for (int i = 0; i < strlen(boat); ++i) {
    switch (boat[i]) {
      case 'M': ++boat_mis_count; break;
      case 'C': ++boat_can_count; break;
    }
  }
}

Status play_game() {
  char left[] = "BCCCMMM";
  char **scene = make_river_scene(left,"");
  print_scene(scene);
  Status game_state = VALID_NONGOAL_STATE;

  cout << "\nType who you would like to send across the river." << endl;
  cout << "M for missionary, C for cannibal, and q to quit." << endl;

  char input[2];

  while (strcmp(input, "q") != 0 && game_state != VALID_GOAL_STATE && game_state != ERROR_MISSIONARIES_EATEN) {
    cout << "Next move: " << endl;
    cin >> input;
    filter(input);

    game_state = perform_crossing(left, input);
    scene = make_river_scene(left, "");
    print_scene(scene);
    cout << "After crossing left = " << left << endl;
    cout << status_description(game_state) << endl << endl;
  }

  return game_state;
}