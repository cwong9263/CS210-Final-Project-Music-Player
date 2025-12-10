#include <vector>
#include <iostream>
#include <stack>
#include <unordered_map>
#include <string>
using namespace std;

class Session {
public:
    stack<int> songHistory;

    vector<string> allSongs = {
        "Come Back", 
        "Come Back To Earth", 
        "Pink + White", 
        "Sugar On My Tongue", 
        "Baby I'm Yours", 
        "Best Time", 
        "Dodger Blue", 
        "Drive Me Crazy", 
        "Don't Smile",
        "Sunday Morning",
        "Johnny",
        "Funny Thing",
        "Sienna",
        "Daises",
        "Saving All My Love For You",
        "Naturally",
        "Amber",
        "Field Trip",
        "Dark Thoughts",
        "Twilight Zone",
        "Fantasy",
        "Scorsese Baby Daddy",
        "A Cold Sunday",
        "How Deep Is Your Love",
        "Glory Of The Snow",
        "Saturn",
        "Here You Come Again",
        "Unwritten",
        "Amoeba",
        "Will I See You Again"
    };

    vector<string> allSongsGenres = {
        "R&B", "Indie", "R&B", "Funk", "Soul",
        "R&B", "Funk", "Pop", "Pop", "Pop",
        "Indie", "Funk", "Indie", "Pop", "Soul",
        "Pop", "Indie", "Rap", "Rap", "Pop",
        "Pop", "Rock", "Rap", "Soul", "Indie",
        "Rock", "Pop", "Pop", "Indie", "Soul"
    };

    unordered_map<int, int> allSongsMap = {
        {10065, 0}, {10066, 1}, {10067, 2}, {10068, 3}, {10069, 4},
        {10070, 5}, {10071, 6}, {10072, 7}, {10073, 8}, {10074, 9},
        {10075, 10}, {10076, 11}, {10077, 12}, {10078, 13}, {10079, 14},
        {10080, 15}, {10081, 16}, {10082, 17}, {10083, 18}, {10084, 19},
        {10085, 20}, {10086, 21}, {10087, 22}, {10088, 23}, {10089, 24},
        {10090, 25}, {10091, 26}, {10092, 27}, {10093, 28}, {10094, 29}
    };

    unordered_map<string, int> titleToID;
    unordered_map<string, vector<int>> genreTree;
    unordered_map<int, vector<int>> songGraph;

    Session() {
        for (const auto &pair : allSongsMap) {
            titleToID[allSongs[pair.second]] = pair.first;
        }

        for (int i = 0; i < allSongs.size(); i++) {
            string genre = allSongsGenres[i];
            int songID = titleToID[allSongs[i]];
            genreTree[genre].push_back(songID);
        }

        for (auto &entry : genreTree) {
            vector<int> &songs = entry.second;
            for (int i = 0; i < songs.size(); i++) {
                for (int j = 0; j < songs.size(); j++) {
                    if (i != j) {
                        songGraph[songs[i]].push_back(songs[j]);
                    }
                }
            }
        }
    }

    void addSong(int songNum) { 
        songHistory.push(songNum); 
    }

    string getSimilarSongs(int songID) {
        if (songGraph.find(songID) == songGraph.end()) {
            return "None";
        }

        string result = "";
        int count = 0;
        stack<int> tempHistory = songHistory;
        unordered_map<int, bool> played;
        while (!tempHistory.empty()) {
            played[tempHistory.top()] = true;
            tempHistory.pop();
        }

        for (int id : songGraph[songID]) {
            if (played.find(id) == played.end()) {
                result += to_string(id) + " - " + allSongs[allSongsMap[id]] + ", ";
                count++;
            }
            if (count == 2) {
                break;
            }
        }

        if (!result.empty()) {
            result.pop_back();
            result.pop_back();
        } 
        else {
            result = "None";
        }

        return result;
    }

    void play() {
        if (!songHistory.empty()) {
            int currentID = songHistory.top();
            cout << "\nPlaying: " << allSongs[allSongsMap.at(currentID)] << " ...\n";
            cout << "Similar songs: " << getSimilarSongs(currentID) << "\n";
        } 
        else {
            cout << "\nNo songs in history!\n";
        }
    }

    void rewind() {
        if (!songHistory.empty()) {
            songHistory.pop();
            play();
        } 
        else {
            cout << "Nothing in rewind!\n";
        }
    }

    int getSongID(string songTitle) {
        if (titleToID.find(songTitle) != titleToID.end()) {
            return titleToID[songTitle];
        }
        return -1;
    }

    void listAllSongs() {
        cout << "\n--- All Songs ---\n";
        for (const auto &pair : allSongsMap) {
            cout << pair.first << ": " << allSongs[pair.second] << "\n";
        }
    }

    void listGenres() {
        cout << "\n--- Songs by Genre ---\n";
        for (const auto &g : genreTree) {
            cout << "\n" << g.first << ":\n";
            for (int id : g.second) {
                cout << "  " << id << " - " 
                     << allSongs[allSongsMap[id]] << "\n";
            }
        }
    }
};

int main() {
    Session mySession;

    cout << "Welcome to Cherise's Music Player!\n";
    cout << "\nTo play a song, type its song number or type the song name.\n";
    cout << "Type \"all\" to see all songs\n";
    cout << "Type \"genres\" to see songs by genre\n";
    cout << "Type \"rewind\" to play the previous song\n";
    cout << "Type \"exit\" to exit\n";
    cout << "----------------------------------------------------\n";

    string userInput;

    while (true) {
        cout << "\nEnter song number, song name, or command: ";
        getline(cin, userInput);

        if (userInput == "exit") {
            break;
        }

        if (userInput == "all") {
            mySession.listAllSongs();
            continue;
        }

        if (userInput == "genres") {
            mySession.listGenres();
            continue;
        }

        if (userInput == "rewind") {
            mySession.rewind();
            continue;
        }

        try {
            int songNum = stoi(userInput);
            if (mySession.allSongsMap.find(songNum) != mySession.allSongsMap.end()) {
                mySession.addSong(songNum);
                mySession.play();
            } 
            else {
                cout << "Song ID not found. Please try again.\n";
            }
        } 
        catch (...) {
            int songID = mySession.getSongID(userInput);
            if (songID != -1) {
                mySession.addSong(songID);
                mySession.play();
            } 
            else {
                cout << "Song title not found. Please try again.\n";
            }
        }
    }

    cout << "\nThanks for using Cherise's Music Player!\n";

    return 0;
}
