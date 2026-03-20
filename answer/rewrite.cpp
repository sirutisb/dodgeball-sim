#include <climits>
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

struct Vec2 {
    long long x, y;
    Vec2(long long x, long long y) : x{x}, y{y} {}
};


int getDir(const std::string& d) {
    if (d == "N") return 0;
    if (d == "NE") return 1;
    if (d == "E") return 2;
    if (d == "SE") return 3;
    if (d == "S") return 4;
    if (d == "SW") return 5;
    if (d == "W") return 6;
    if (d == "NW") return 7;
    throw std::runtime_error("invalid dir");
}

Vec2 getDirVec(int d) {
    if (d == 0) return { 0, 1 }; // N
    if (d == 1) return { 1, 1 }; // NE
    if (d == 2) return { 1, 0 }; // E
    if (d == 3) return { 1, -1 }; // SE
    if (d == 4) return { 0, -1 }; // S
    if (d == 5) return { -1, -1 }; // SW
    if (d == 6) return { -1, 0 }; // W
    if (d == 7) return { -1, 1 }; // NW
    throw std::runtime_error("invalid dir");
}


struct Data {
    std::vector<Vec2> players;
    std::string d;
    int startingPlayer;
};

Data parse() {
    int n;
    std::cin >> n;

    Data data;
    data.players.reserve(n);
    for (int i = 0; i < n; ++i) {
        int x_p, y_p;
        std::cin >> x_p >> y_p;
        data.players.emplace_back(x_p, y_p);
    }
    std::cin >> data.d;

    int s;
    std::cin >> s;
    data.startingPlayer = s - 1;
    return data;
}

bool castRay(const Vec2& from, const Vec2& to, int dir) {
    long long dx = to.x - from.x, dy = to.y - from.y;
    Vec2 v = getDirVec(dir);

    // vertical check
    if (v.x == 0) {
        if (dx != 0) return false; // not on same line
        if (v.y > 0 && dy > 0) return true;
        if (v.y < 0 && dy < 0) return true;
        return false;
    }

    // horizontal check
    if (v.y == 0) {
        if (dy != 0) return false;
        if (v.x > 0 && dx > 0) return true;
        if (v.x < 0 && dx < 0) return true;
        return false;
    }

    // if we get here, we are going diagonal

    // must have same magnitude.
    if (labs(dx) != labs(dy)) return false;

    if (v.x > 0 && v.y > 0) return dx > 0 && dy > 0; // NE
    if (v.x > 0 && v.y < 0) return dx > 0 && dy < 0; // SE
    if (v.x < 0 && v.y > 0) return dx < 0 && dy > 0; // NW
    if (v.x < 0 && v.y < 0) return dx < 0 && dy < 0; // SW


    return false;
}

int reflectDir(int dir) {
    return (dir + 4) % 8;
}

std::pair<int, int> simulate(const std::vector<Vec2>& players, int dir, int startingPlayer) {
    int curr = startingPlayer;
    int throws = 0;

    int n = players.size();
    int receiveDir = dir;

    std::vector<bool> visited(n, false);

    while (true) {
        long long targetPlayer = -1;
        int throwDir = -1;

        for (int d = 1; d <= 8; ++d) {

            int newDir = (receiveDir + d) % 8;
            long long best = LLONG_MAX;
            for (int i = 0; i < n; ++i) {
                if (visited[i] || i == curr) continue;
                if (castRay(players[curr], players[i], newDir)) {
                    long long dx = players[i].x - players[curr].x, dy = players[i].y - players[curr].y;
                    long long dist = dx * dx + dy * dy;
                    if (dist < best) {
                        best = dist;
                        throwDir = newDir;
                        targetPlayer = i;
                    }
                }
            }

            // if player found we can early return
            if (targetPlayer != -1) break;
        }

        // no player found for all directions
        if (targetPlayer == -1) break;

        // throw the ball
        throws++;
        visited[curr] = true;
        curr = targetPlayer;
        receiveDir = reflectDir(throwDir);
    }

    // add one to curr (last) for base 1 starting index
    return {throws, curr + 1};
}

int main() {
    int t;
    std::cin >> t;

    while (t--) {
        // parse current test case
        Data data = parse();
        auto& [players, d, startingPlayer] = data;

        // simulate the game
        int dir = getDir(d);
        auto [throws, last] = simulate(players, dir, startingPlayer);
        std::cout << throws << ' ' << last << '\n';
    }


    return 0;
}