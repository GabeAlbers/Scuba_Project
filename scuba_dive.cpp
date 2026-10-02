#include <iostream>

#include <limits>

#include <string>

#include <vector>

#include <random>

#include <ctime>

using namespace std;

//i need this so subarine knows game map exists
class game_map;

class submarine
{

    public:
        int x;
        int y;
        int max_oxygen;
        int current_oxygen;
        int total_earnings;

        int gold_coins_found = 0;
        int relics_found = 0;
        int oxygen_pockets_found = 0;
        int fish_descovered_count = 0;


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
        void player_input(char input, const game_map& map);


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

        void print_map_with_sub(const submarine& sub)
        {
            for (int y = 0; y < height; y++)
            {
                for (int x = 0; x < width; x++)
                {   
                    //if the chord matches the sub chord print '@'
                    if (x == sub.x && y == sub.y)
                    {
                        cout << '@';
                    }
                    else 
                    {
                        cout << grid[y][x];
                    }
                }
                cout << endl;
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
            int pillar_buffer_left = 12;
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

        bool is_valid_move(int target_x, int target_y) const
        {

            //check boundries
            if (target_x < 0 || target_x >= width || target_y < 0 || target_y >= height)
            {
                return false;
            } else if (grid[target_y][target_x] == '#') //check walls or seafloors
            {
                return false;
            }

            //otherwise its true
            return true;
        }

//inbetween the "=" is the logic for all of the treasure interaction stuff. abobe they just spawn in as "?"
//===================================================================================================================================

        void process_treasure_interaction(char& tile_content, submarine& sub)
        {
            if (tile_content == '?')
            {
                //roll to see if its a fish (25% chance)
                int roll = 1 + (rand() % 100);
                if (roll <= 25 )
                {
                    tile_content = '2';
                    cout << "\nYou find... A FISH! thats a pretty cool looking fish!" << endl;
                    sub.fish_descovered_count++;
                }
                else //75% chance or this
                {
                    tile_content = ' ';

                    int loot_type = 1 + (rand() % 3); //3 diffrent loot types
                    if (loot_type == 1)
                    {
                        int reward = 150;
                        cout << "\nThats a nice gold coin! Gained $" << reward << "!" << endl;
                        sub.gold_coins_found++;
                        sub.total_earnings += reward;
                    } 
                    else if (loot_type == 2)
                    {
                        int reward = 300;
                        cout << "\n  Ancient Relic!! Nice! Gained $" << reward << endl;
                        sub.relics_found++;
                        sub.total_earnings += reward;

                    }else 
                    {
                        sub.oxygen_pockets_found++;
                        sub.current_oxygen = 100;
                        cout << "\nYou found an air pocket! oxygen had been restored up to 100!" << endl;
                    }
                }
            } 
            else if (tile_content == '2') //im adding a 1% chance that if you look at the fish again it becomes a gold coin
            {
                int roll = 1 + (rand() % 100);
                if (roll <= 99)
                {
                cout << "\nYou look at it again.. umm... its still a fish." << endl;
                }else 
                {
                    int reward = 150;
                    cout << "\nWhat?! the fish turned into a gold coin! Gained $" << reward << "!" << endl;
                    sub.gold_coins_found++;
                    sub.total_earnings += reward;
                    tile_content = ' ';
                    
                }
            }
        }

        //inspects and then collects treasure
        void inspect_menu(submarine& sub)
        {

            //check all directuons if treasure is nearby
            bool has_up = (sub.y > 0 && (grid[sub.y - 1][sub.x] == '?' || grid[sub.y - 1][sub.x] == '2'));
            bool has_down = (sub.y < height - 1 && (grid[sub.y + 1][sub.x] == '?' || grid[sub.y + 1][sub.x] == '2'));
            bool has_left = (sub.x > 0 && (grid[sub.y][sub.x - 1] == '?' || grid[sub.y][sub.x - 1] == '2'));
            bool has_right = (sub.x < width - 1 && (grid[sub.y][sub.x + 1] == '?' || grid[sub.y][sub.x + 1] == '2'));
            int count = 0;

            //count how many there are
            if (has_up) count++;
            if (has_down) count++;
            if (has_left) count++;
            if (has_right) count++;

            if (count == 0)
            {
                cout << "\nNo treasure close enough to inspect." << endl;
                return;
            }

            char choice = ' ';
            bool valid_choice = false;

            while (!valid_choice)
            {
                cout << "\n===Inspection Menu==" << endl
                     << "Avalable Options: ";
                    if (has_up) cout << "[W: up] ";
                    if (has_down) cout << "[S: down] ";
                    if (has_left) cout << "[A: left] ";
                    if (has_right) cout << "[D: Right] ";
                    cout << "\nChoose a direction (W/A/S/D) to inspect or enter 'C' to cancel: ";

                //input time
                cin >> choice;

                cout << "=====================================================" 
                     << endl
                     << "=====================================================" 
                     << endl;

                if (cin.fail())
                {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "\nInput stream error! please try again." << endl;
                    continue;
                }

                choice = tolower(choice);

                if (choice == 'c')
                {
                    cout << "Exiting inspection menu." << endl;
                    cin.ignore(numeric_limits<streamsize>::max(), '\n'); //this clears out anny buffer inputs i think
                    return;              
                }

                //using these to set target chords later
                int target_x = sub.x;
                int target_y = sub.y;

                if (choice == 'w' && has_up) {target_y--; valid_choice = true;}
                else if (choice == 's' && has_down) {target_y++; valid_choice = true;}
                else if (choice == 'a' && has_left) {target_x--; valid_choice = true;}
                else if (choice == 'd' && has_right) {target_x++; valid_choice = true;}
                else
                {
                    cout << "\n Invalid choice, or no treasure in that direction. Try Again.\n";
                    cin.ignore(numeric_limits<streamsize>::max(), '\n'); //again clear out any buffers

                }

                if (valid_choice)
                {
                    process_treasure_interaction(grid[target_y][target_x], sub);
                }
            }
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); //again...

        }




};

//this needs to be declared after the game_map class
void submarine::player_input(char input, const game_map& map)
{

    int target_x = x;
    int target_y = y;

    //convert input to lower
    input = tolower(input);

    if (input == 'w') target_y--; //move up (subtract because it prints 0 -> max)
    else if (input == 's') target_y++; //down
    else if (input == 'a') target_x--; //left 
    else if (input == 'd') target_x++; //right

    //if the map says its valid do it
    if (map.is_valid_move(target_x, target_y))
    {
        x = target_x;
        y = target_y;
    }
}







int main ()
{

    srand(time(0));

    game_map ocean_map(50,12);
    submarine my_sub(5, 2, 100); //collumn 5 row 2 with 100 oxygen

    char input = ' ';

    while (input != 'q')
    {
        cout << "==================================================" << endl
             << "Status: " << endl;
             << "==================================================" 
             << endl;
        ocean_map.print_map_with_sub(my_sub);
        cout << "controls: movment (w/a/s/d) up, left, down, and right | i (inspect) | q = quit" << endl;
        cout << "enter move: "; 

        cin >> input;

        if (input == 'i')
        {
            ocean_map.inspect_menu(my_sub);
        }
        else if (input != 'q')
        {

            my_sub.player_input(input, ocean_map);

            //if the su is at the surface then this happens
            if (my_sub.y == 0  || ocean_map.get_tile(my_sub.x, my_sub.y) == '~')
            {
                int gold_total = my_sub.gold_coins_found * 150;
                int relic_total = my_sub.relics_found * 300;

                cout << "\n==================================================" << endl
                     << "            Succsessfull Surface!                " << endl
                     << "Surface air has been reached and your oxygen has been refilled!" << endl
                     << "----------------------------------------------------" << endl
                     << "                   Rewards                          " << endl
                     << "Gold Coins Found     :" << my_sub.gold_coins_found << " x 150 = $" << gold_total << endl
                     << "Relics found         :" << my_sub.relics_found << " x 300 = $" << relic_total << endl
                     << "Air pockets decovered:" << my_sub.oxygen_pockets_found << endl
                     << "Fish seen            :" << my_sub.fish_descovered_count << endl
                     << "==================================================" << endl
                     << "Total Earnings : $" << my_sub.total_earnings << endl
                     << "==================================================" << endl;

                    break;
            }
        }

    }

    cout << "TEST\n\n";

    ocean_map.print_map();





}
