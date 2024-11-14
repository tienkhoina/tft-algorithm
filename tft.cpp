#include <bits/stdc++.h>
using namespace std;

// Sử dụng unordered_set và unordered_map để tăng tốc độ truy cập
unordered_set<string> tg;
unordered_set<string> he;
unordered_map<string, pair<int, int>> hbd;
unordered_map<string, int> skh;
unordered_map<string, vector<string>> tht;
unordered_map<string, int> point;
vector<vector<string>> population;
vector<string> best_solution;
int population_size = 100;
int generations = 1000;
double mutation_rate = 0.1;
int thht = 0;
int maxhe = 0;
int target_score = 34;
int maxhe_limit = 11;
int x_size = 9;

// Hàm tính toán độ phù hợp (fitness) đã được tối ưu
int fitness(vector<string>& individual) {
    int thht = 0;
    int total_points = 0;
    unordered_map<string, int> local_skh;
    for (string &tu : individual) {
        total_points += point[tu];
        for (string &h : tht[tu]) {
            local_skh[h]++;
            if (local_skh[h] == hbd[h].first) thht++;
        }
    }
    return total_points >= target_score ? thht : -1;
}

// Tối ưu hàm crossover để tránh lặp từ khóa
vector<string> crossover(const vector<string>& parent1, const vector<string>& parent2) {
    vector<string> child;
    unordered_set<string> used;
    size_t size = parent1.size();
    for (size_t i = 0; i < size; i++) {
        string word = (rand() % 2) ? parent1[i] : parent2[i];
        while (used.count(word)) {
            word = *next(tg.begin(), rand() % tg.size());
        }
        child.push_back(word);
        used.insert(word);
    }
    return child;
}

// Tối ưu hàm mutate
void mutate(vector<string>& individual) {
    if ((double)rand() / RAND_MAX < mutation_rate) {
        int idx = rand() % individual.size();
        string new_word = *next(tg.begin(), rand() % tg.size());
        unordered_set<string> current_words(individual.begin(), individual.end());
        while (current_words.count(new_word)) {
            new_word = *next(tg.begin(), rand() % tg.size());
        }
        individual[idx] = new_word;
    }
}

// Hàm chọn cha mẹ tối ưu bằng cách sử dụng Roulette Wheel Selection
vector<string> select_parent(const vector<pair<int, vector<string>>>& fitness_population) {
    int total_fitness = 0;
    for (const auto& fp : fitness_population) {
        total_fitness += fp.first;
    }

    int random_value = rand() % total_fitness;
    int cumulative_fitness = 0;
    for (const auto& fp : fitness_population) {
        cumulative_fitness += fp.first;
        if (cumulative_fitness >= random_value) {
            return fp.second;
        }
    }
    return fitness_population.back().second; // Phòng khi xảy ra lỗi chọn
}

void genetic_algorithm() {
    // Khởi tạo lại quần thể ngẫu nhiên mỗi lần gọi hàm
    population.clear();
    for (int i = 0; i < population_size; i++) {
        vector<string> individual;
        unordered_set<string> used;
        for (int j = 0; j < x_size; j++) {
            string word = *next(tg.begin(), rand() % tg.size());
            while (used.count(word)) {
                word = *next(tg.begin(), rand() % tg.size());
            }
            individual.push_back(word);
            used.insert(word);
        }
        population.push_back(individual);
    }

    for (int gen = 0; gen < generations; gen++) {
        vector<pair<int, vector<string>>> fitness_population;
        for (auto& individual : population) {
            int fit = fitness(individual);
            if (fit != -1) {
                fitness_population.push_back({fit, individual});
                if (fit > maxhe) {
                    maxhe = fit;
                    best_solution = individual;
                }
            }
        }

        // Dừng nếu quần thể không có cá thể nào thỏa mãn điều kiện
        if (fitness_population.empty()) break;

        sort(fitness_population.rbegin(), fitness_population.rend());
        population.clear();

        while (population.size() < population_size) {
            vector<string> parent1 = select_parent(fitness_population);
            vector<string> parent2 = select_parent(fitness_population);
            vector<string> child = crossover(parent1, parent2);
            mutate(child);
            population.push_back(child);
        }
    }
}

int main() {
    srand(time(0)); // Khởi tạo lại seed ngẫu nhiên mỗi lần chạy

    while (1) {
        string tu;
        cin >> tu;
        if (tu == "#") break;
        tg.insert(tu);
        int so;
        cin >> so;
        for (int i = 0; i < so; i++) {
            string h;
            cin >> h;
            he.insert(h);
            cin >> hbd[h].first >> hbd[h].second;
            tht[tu].push_back(h);
        }
        cin >> point[tu];
    }

    // Tùy chỉnh các thông số
    cout << "Nhập số phần tử của X: ";
    cin >> x_size;
    cout << "Nhập giới hạn chấp nhận của maxhe: ";
    cin >> maxhe_limit;
    cout << "Nhập target_score: ";
    cin >> target_score;

    for (string str : he) {
        skh[str] = 0;
    }

    while (maxhe < maxhe_limit) {
        genetic_algorithm();
        for (string &x : best_solution) {
            cout << x << " ";
        }
        cout << "Maxhe hiện tại: " << maxhe << endl;
    }

    cout << "Maxhe cuối cùng: " << maxhe << endl;
    cout << "Các phần tử của X ứng với maxhe: ";
    for (string &x : best_solution) {
        cout << x << " ";
    }
    cout << endl;
    return 0;
}
