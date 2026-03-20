#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <climits>

using namespace std;

struct Vec2 {
    long long x, y;
};

struct Data {
    vector<Vec2> players;
    string direction;
    int startingPlayer;
};

Data parse(istream& input) {
    Data data;
    int n;
    input >> n;
    data.players.resize(n);
    for (int i = 0; i < n; ++i) {
        input >> data.players[i].x >> data.players[i].y;
    }
    int s;
    input >> data.direction >> s;
    data.startingPlayer = s - 1;
    return data;
}

int getDir(const string& dir) {
    if (dir == "N") return 0;
    if (dir == "NE") return 1;
    if (dir == "E") return 2;
    if (dir == "SE") return 3;
    if (dir == "S") return 4;
    if (dir == "SW") return 5;
    if (dir == "W") return 6;
    if (dir == "NW") return 7;
    throw std::runtime_error("invalid dir");
}

Vec2 getDirVec(int dir) {
    if (dir == 0) return { 0, 1 };
    if (dir == 1) return { 1, 1 };
    if (dir == 2) return { 1, 0 };
    if (dir == 3) return { 1, -1 };
    if (dir == 4) return { 0, -1 };
    if (dir == 5) return { -1, -1 };
    if (dir == 6) return { -1, 0 };
    if (dir == 7) return { -1, 1 };
    throw std::runtime_error("invalid dir");
}

bool castRay(Vec2 from, Vec2 to, int dir) {
    Vec2 v = getDirVec(dir);
    long long dx = to.x - from.x;
    long long dy = to.y - from.y;

    // Horizontal check
    if (v.y == 0) {
        if (dy != 0) return false; // not on same line
        if (v.x > 0 && dx <= 0) return false; // Target would be to our right
        if (v.x < 0 && dx >= 0) return false; // target left
        return true;
    }

    // Veritcal check
    if (v.x == 0) {
        if (dx != 0) return false;
        if (v.y > 0 && dy <= 0) return false;
        if (v.y < 0 && dy >= 0) return false;
        return true;
    }

    // diags shoud hve the same mag
    if (abs(dx) != abs(dy)) return false; 

    if (v.x > 0 && v.y > 0) return (dx > 0 && dy > 0); // ne
    if (v.x < 0 && v.y > 0) return (dx < 0 && dy > 0); // nW
    if (v.x > 0 && v.y < 0) return (dx > 0 && dy < 0); // SE
    if (v.x < 0 && v.y < 0) return (dx < 0 && dy < 0); // SW

    return false;
}

// refelct direction so we can rotate from this
int reflectDir(int dir) {
    return (dir + 4) % 8;
}

int main() {
    // ifstream infile("examples/example_big.in.txt");
    // ifstream file("examples/example.in.txt");
    int t;
    // file >> t;
    cin >> t;
    while (t--) {
        // Data data = parse(file);
        Data data = parse(cin); // test
        auto& players = data.players;
        int n = players.size();
        vector<bool> visited(n, false);

        int throws = 0;
        int curr = data.startingPlayer;
        int last = curr;
        int receiveDir = getDir(data.direction);

        while (true) {
            int found = -1;
            int throwDir = -1;
            auto& currPlayer = players[curr];

            for (int deltaDir = 0; deltaDir < 8; ++deltaDir) {
                if (found != -1) break;
                // off by one error
                int newDir = (receiveDir + deltaDir + 1) % 8;
                long long best = LLONG_MAX;

                for (int i = 0; i < n; ++i) {
                    if (visited[i] || i == curr) continue;
                    auto& p = data.players[i];
                    if (castRay(currPlayer, p, newDir)) {
                        long long dx = p.x - currPlayer.x, dy = p.y - currPlayer.y;
                        long long dist = dx * dx + dy * dy;
                        if (dist < best) {
                            best = dist;
                            throwDir = newDir;
                            found = i;
                        }
                    }
                }
            }
            // didnt find anyone
            if (found == -1) break;

            throws++;
            receiveDir = reflectDir(throwDir); // transfer to the next person just reflected
            visited[curr] = true;
            curr = found;
            last = curr;
        }

        cout << throws << " " << (last + 1) << "\n";
    }
}
