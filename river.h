const int SCENE_HEIGHT=24, SCENE_WIDTH=72;

enum Status {VALID_GOAL_STATE=2, VALID_NONGOAL_STATE=1,ERROR_INVALID_CANNIBAL_COUNT=-1, ERROR_INVALID_MISSIONARY_COUNT=-2, ERROR_INVALID_MOVE=-3, ERROR_MISSIONARIES_EATEN=-4, ERROR_INPUT_STREAM_ERROR=-5, ERROR_BONUS_NO_SOLUTION=-6};
char **create_scene();
void destroy_scene(char **scene);
void print_scene(char **scene);
bool add_to_scene(char **scene, int row, int col, const char *filename);
const char *status_description(int code);

char** make_river_scene(const char left[7], const char boat[2]);
Status perform_crossing(char left[7], const char targets[2]);
void count_people(const char left[7], const char boat[2], int &mis_count, int &can_count, int &boat_mis_count, int &boat_can_count, bool &boat_right);
Status play_game();
