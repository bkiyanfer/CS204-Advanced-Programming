#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <ctime>
#include <random>
#include <chrono>
#include <iomanip>
#include <sstream>

using namespace std;

//beril kiyanfer 30565



mutex mtx;
mutex cout_mtx; // for couts
vector<int> player_scores;
int target_number;
bool game_over = false;
bool round_over = false;
bool start_round = false;
int num_players, num_rounds, range_lower, range_upper;


//for getting user related inputs
void get_inputs() {
    cout << "Please enter number of players: ";
    cin >> num_players;
    while (num_players < 1) {
        cout << "Number of players cannot be lower than 1! Please enter number of players: ";
        cin >> num_players;
    }

    cout << "Please enter number of rounds: ";
    cin >> num_rounds;
    while (num_rounds < 1) {
        cout << "Number of rounds cannot be lower than 1! Please enter number of rounds: ";
        cin >> num_rounds;
    }

    cout << "Please enter the randomization range: ";
    cin >> range_lower >> range_upper;
    while (range_lower > range_upper) {
        cout << "Lower bound has to be smaller than or equal to higher bound! Please enter the randomization range: ";
        cin >> range_lower >> range_upper;
    }
}

string current_time() {
    auto now = chrono::system_clock::now();
    time_t now_c = chrono::system_clock::to_time_t(now);
    struct tm *parts = localtime(&now_c);
    stringstream ss;
    ss << put_time(parts, "%H:%M:%S");
    return ss.str();
}
//thread to make guesses
void player_function(int player_id) {
    default_random_engine generator(random_device{}());
    uniform_int_distribution<int> distribution(range_lower, range_upper);

    this_thread::sleep_until(chrono::system_clock::now() + chrono::seconds(3));

    while (!game_over) {
        this_thread::sleep_for(chrono::seconds(1));

        if (!start_round || round_over) continue;

        int guess = distribution(generator);

        {
            
            lock_guard<mutex> lock(mtx);
            if (round_over) continue;

            cout_mtx.lock();
            cout << "Player with id " << player_id << " guessed " << guess;
            if (guess == target_number) {
                cout << " correctly ";
                player_scores[player_id]++;
                round_over = true;
            } else {
                cout << " incorrectly ";
            }
            cout << "at: " << current_time() << endl;
            cout_mtx.unlock();
        }
    }
}
//game rounds
void host_function() {
    default_random_engine generator(random_device{}());
    uniform_int_distribution<int> distribution(range_lower, range_upper);

    for (int round = 1; round <= num_rounds; ++round) {
        {
            lock_guard<mutex> lock(cout_mtx);
            cout << "---------------------------------------------------" << endl;
            cout << "Game started at: " << current_time() << endl;
            cout << "Round " << round << " will start 3 seconds later" << endl;
        }

        this_thread::sleep_for(chrono::seconds(3));
        target_number = distribution(generator);

        {
            lock_guard<mutex> lock(cout_mtx);
            cout << "Target is " << target_number << endl;
        }

        {
            lock_guard<mutex> lock(mtx);
            start_round = true;
            round_over = false;
        }

        while (!round_over) {
            this_thread::sleep_for(chrono::milliseconds(100));
        }

        {
            lock_guard<mutex> lock(cout_mtx);
            if (round < num_rounds) {
                cout << "---------------------------------------------------" << endl;
                cout << "Round " << (round + 1) << " started at: " << current_time() << endl;
            }
        }
    }

    game_over = true;
}

int main() {
    get_inputs();

    player_scores.resize(num_players, 0);
    vector<thread> player_threads;
    thread host_thread(host_function);

    for (int i = 0; i < num_players; ++i) {
        player_threads.emplace_back(player_function, i);
    }

    host_thread.join();
    for (auto &thread : player_threads) {
        thread.join();
    }

    cout << "Game is over!" << endl;
    cout << "Leaderboard:" << endl;
    for (int i = 0; i < num_players; ++i) {
        cout << "Player " << i << " has won " << player_scores[i] << " times" << endl;
    }

    return 0;
}
