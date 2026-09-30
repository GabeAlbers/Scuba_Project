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
        Submarine(int start_x, int start_y, int oxygen)
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

}


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
            level_grid = vector<string>(height, string(width, ' '));

        }

        void add_random_hills()
        {
            for (int i = 0; i < 4; i++)
            {
                
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
                level_grid[0][x] = '~';
            }

            //add sea floor
            for (int x = 0; x < width; x++)
            {
                level_grid[height - 1][x] = '#';
            }

            //add hills
            add_random_hills();

        }

}



int main ()
{

    cout << "TEST";

}
