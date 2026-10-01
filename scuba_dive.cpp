#include <iostream>

#include <limits>

#include <string>

#include <vector>

#include <random>

#include <ctime>

using namespace std;

class submarine
{

    public:
        int x;
        int y;
        int max_oxygen;
        int current_oxygen;
        int total_earnings;


        //this constructor sets the initial position and filles the oxygen to the level it needs to be
        submarine(int start_x, int start_y, int oxygen)
        {
            x = start_x;
            y = start_y;
            max_oxygen = oxygen;
            current_oxygen = oxygen;
            total_earnings = 0;
        }

        //this can later be used to reset the sumarine starting and ending values
        void reset(int start_x, int start_y)
        {
            x = start_x;
            y = start_y;
            current_oxygen = max_oxygen;
        }

        //tis will check player movment aginst map collisions on map
        void player_input()

};


class game_map
{
    private:
        int width; 
        int height;
        vector<string> grid;

    public:

        game_map(int w, int h)
        {
            width = w;
            height = h;
            
            //this will Initialize the grid the vecotor with "open ocean (' ')"
            grid = vector<string>(height, string(width, ' '));


            setup_level();


        }

        //simple printer for testing
        void print_map() 
        {
            for (int y = 0; y < height; y++)
            {
                cout << grid[y] << endl;
            }
        }

        char get_tile(int x, int y)
        {
            return grid[y][x];
        }


        //this will randomly add small hills to the landscape
        void add_random_hills()
        {

            srand(time(0));

            //this will set 4 random liitle hill functions around the ocean
            for (int i = 0; i < 4; i++) 
            {
                //random center point the +2 and width -4 keeps it from
                //toutching the edges of the game map
                int hill_x = 2 + (rand() % (width - 4));

                //random height between 2 and 4 blocks tall
                int hill_height = 2 + (rand() % 3);

                //build the hill upwards 
                for (int h = 0; h < hill_height; h++)
                {

                    //this sets where i "sink" my hill values
                    int current_y = (height - 1) - h; 

                    //by subtracting h it tapers the base of the hills
                    int spread = hill_height - h;

                    for (int i_2 = -spread; i_2 <= spread; i_2++)
                    {
                        int target_x = hill_x + i_2; 

                        //final check to make sure we are in map borders
                        if (target_x >= 0 && target_x < width)
                        {
                            //this finnal sets a value to '#'
                            grid[current_y][target_x] = '#'; 
                        }
                    }
                }
                
            }
        }

        //this will add one pillar in a random 
        //spot in the level as a kind of cliff
        void add_random_pillar()
        {
            int pillar_width = 2;

            int pillar_height = 5;

            //this is used to calculate how far away the pillar has to be from the edge of the level
            int pillar_buffer_left = 7;
            int pillar_buffer_right = 3;
            //picks a random x chord 
            int pillar_x = pillar_buffer_left + (rand() % (width - (pillar_buffer_left + pillar_buffer_right)));

            for (int h = 0; h < pillar_height; h++)
            {
                //moves upward from the bottom row
                int current_y = (height - 1) - h;

                //this is only 3 wide -1 to the left -1 to the right
                for (int z = -1; z <= 1; z++)
                {   
                    //find target x
                    int target_x = pillar_x + z;

                    if (target_x >= 0 && target_x < width)
                    {
                        grid[current_y][target_x] = '#';
                    }
                }
            }
        }


        //this will work the same as the pillar random but 
        // adds a cliff at the start of the level 
        void add_beginning_cliff()
        {
            int pillar_width = 5;

            int cliff_height = 9;

            //i cant figure out the calculation for this right now but because 
            // its at the start and its width is always 5 its gonna be 3
            int cliff_x = 3;

            for (int h = 0; h < cliff_height; h++)
            {
                //moves upward from the bottom row
                int current_y = (height - 1) - h;

                //this is 5 wide this time so to -3 and positive 3
                for (int z = -3; z <= 3; z++)
                {   
                    //find target x
                    int target_x = cliff_x + z;

                    if (target_x >= 0 && target_x < width)
                    {
                        grid[current_y][target_x] = '#';
                    }
                }
            }
        }

        //this will add random treasures that are ' ' and toutching a '#'
        void add_treasures(int amount_of_mystery_blocks)
        {
            //Im going to use these to sore valid 
            //chords and choose them randomly
            vector<int> valid_x;
            vector<int> valid_y;

            //scan the map
            for (int y = 0; y < height; y++)
            {
                for (int x = 0; x < width; x++)
                {

                    //check if is ocean
                    if (grid[y][x] == ' ')
                    {
                        //set it to flase first
                        bool toutching_wall = false;

                        //now check all 4 directions 
                        //up
                        if (y > 0 && grid[y -1][x] == '#') toutching_wall = true;
                        //down
                        if (y < height - 1 && grid[y + 1][x] == '#') toutching_wall = true;
                        //left 
                        if (x > 0 && grid[y][x -1] == '#') toutching_wall = true;
                        //right 
                        if (x < width - 1 && grid[y][x + 1] == '#') toutching_wall = true;

                        //if the tile turns true record it
                        if (toutching_wall)
                        {
                            valid_x.push_back(x);
                            valid_y.push_back(y);
                        }
                    }

                }

            }

            int placed = 0;
            while (placed < amount_of_mystery_blocks && !valid_x.empty())
            {

                //pick a random index time from the valid inputs
                int index = rand() % valid_x.size();

                int place_x = valid_x[index];
                int place_y = valid_y[index];

                //place treasure
                grid[place_y][place_x] = '?';

                placed++;
            }

        }

        void setup_level()
        {
            //(for this to be more readble things that are being put in 
            // horizontaly will be using int x and verticaly wil be y and 
            // randoms will be z)

            //this fills the top layer with "~" for the surface
            for (int x = 0; x < width; x++)
            {
                grid[0][x] = '~';
            }

            //add sea floor
            for (int x = 0; x < width; x++)
            {
                grid[height - 1][x] = '#';
            }

            //add hills
            add_random_hills();
            //adds a random pillar 
            add_random_pillar();
            //adds a cliff at he start
            add_beginning_cliff();

            //add 10 treasure '?' places
            add_treasures(10);
        }

        bool is_valid_move(int target_x, int target_y)
        {

            //check boundries
            if (target_x < 0 || target_x >= width || target_y < 0 || target_y >= height)
            {
                return false;
            } else if (grid[target_y][target_x] == '#') //check walls or seafloors
            {
                return false 
            }

            //otherwise its true
            return true;
        }

};




int main ()
{

    srand(time(0));

    game_map ocean_map(50,12);

    cout << "TEST\n\n";

    ocean_map.print_map();





}
