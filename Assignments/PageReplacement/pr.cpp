#include <stdio.h>
#include <iostream>
#include <vector>
#include <list>


using namespace std;

#define DEFAULT_CACHE_SIZE 1024
#define DEFAULT_ALGORITHM 1

int cache_size = DEFAULT_CACHE_SIZE;
int algo = DEFAULT_ALGORITHM;
int hit = 0;
int miss = 0;
int page_num = 0;
vector<int> pages;

void set_cache_size();

void set_page_replacement_algorithm();

void read_pages();

void FIFO_algo();

void LRU_algo();

void MIN_algo();

int predict(int index, vector<int> cache_vector, vector<int> &index_vector);

void CLOCK_algo();


int main() {
//    freopen("3.in", "r", stdin);
    set_cache_size();
    set_page_replacement_algorithm();
    read_pages();
    switch (algo) {
        case 0:
            FIFO_algo();
            break;
        case 1:
            LRU_algo();
            break;
        case 2:
            MIN_algo();
            break;
        case 3:
            CLOCK_algo();
            break;
        default:
            break;
    }
    printf("Hit ratio = %05.2f %%\n", 1.0 * hit / (miss + hit) * 100);
}

void set_cache_size() {
    int temp_cache_size;
    cin >> temp_cache_size;
    if (temp_cache_size <= 0) {
        return;
    }
    cache_size = temp_cache_size;
}

void set_page_replacement_algorithm() {
    int id;
    cin >> id;
    if (0 <= id && id <= 3) {
        algo = id;
    } else {
        algo = 1;
    }
}

void read_pages() {
    pages.clear();
    cin >> page_num;
    for (int i = 0; i < page_num; i++) {
        int page_index;
        cin >> page_index;
        pages.push_back(page_index);
    }
}

void FIFO_algo() {
    list<int> cache_list;
    hit = 0;
    miss = 0;
    cache_list.clear();
    list<int>::iterator j;
    for (int i = 0; i < page_num; ++i) {
        int target = pages[i];
        if (cache_list.empty()) {
            cache_list.push_front(target);
            miss += 1;
            continue;
        }
        bool find = false;
        for (j = cache_list.begin(); j != cache_list.end(); ++j) {
            if (*j == target) {
                find = true;
                break;
            }
        }
        if (find) {
            hit++;
        } else {
            if (cache_list.size() == cache_size) {
                cache_list.pop_back();
            }
            cache_list.push_front(target);
            miss++;
        }
    }
}

void LRU_algo() {
    list<int> cache_list;
    hit = 0;
    miss = 0;
    cache_list.clear();
    list<int>::iterator j;
    for (int i = 0; i < page_num; ++i) {
        int target = pages[i];
        if (cache_list.empty()) {
            cache_list.push_front(target);
            miss += 1;
            continue;
        }
        bool find = false;
        for (j = cache_list.begin(); j != cache_list.end(); ++j) {
            if (*j == target) {
                find = true;
                cache_list.erase(j);
                cache_list.push_front(target);
                break;
            }
        }
        if (find) {
            hit++;
        } else {
            if (cache_list.size() == cache_size) {
                cache_list.pop_back();
            }
            cache_list.push_front(target);
            miss++;
        }
    }
}

int predict(int index, vector<int> cache_vector, vector<int> &index_vector) {
    int res = -1;
    int farthest = -1;
    for (int i = 0; i < cache_vector.size(); ++i) {
        if (index_vector[i] >= index) {
            if (index_vector[i] > farthest) {
                farthest = index_vector[i];
                res = i;
            }
            continue;
        }
        int j;
        for (j = index; j < page_num; j++) {
            if (cache_vector[i] == pages[j]) {
                index_vector[i] = j;
                if (j > farthest) {
                    farthest = j;
                    res = i;
                }
                break;
            }
        }
        if (j == page_num) {
            return i;
        }
    }
    return (res == -1) ? 0 : res;
}

void MIN_algo() {
    vector<int> cache_vector;
    vector<int> index_vector;
    hit = 0;
    miss = 0;
    cache_vector.clear();

    for (int i = 0; i < page_num; ++i) {
        int target = pages[i];
        // Page found in a frame : HIT
        if (cache_vector.empty()) {
            cache_vector.push_back(target);
            index_vector.push_back(i);
            miss += 1;
            continue;
        }
        bool find = false;
        for (int j = 0; j < cache_vector.size(); ++j) {
            if (cache_vector[j] == target) {
                find = true;
                break;
            }
        }
        if (find) {
            hit++;
        } else {
            if (cache_vector.size() == cache_size) {
                int res = predict(i + 1, cache_vector, index_vector);
                cache_vector[res] = target;
                index_vector[res] = i;
            } else {
                cache_vector.push_back(target);
                index_vector.push_back(i);
            }
            miss++;
        }
    }
}

void CLOCK_algo() {
    int *cache_arr = new int[cache_size];
    int *valid_arr = new int[cache_size];
    hit = 0;
    miss = 0;
    int cache_num = 0;
    for (int i = 0; i < cache_size; ++i) {
        cache_arr[i] = -1;
        valid_arr[i] = 0;
    }
    int pointer = 0;
    for (int i = 0; i < page_num; ++i) {
        int target = pages[i];
        bool find = false;
        for (int j = 0; j < cache_num; ++j) {
            if (cache_arr[j] == target) {
                valid_arr[j] = 1;
                find = true;
                break;
            }
        }
        if (find) {
            hit++;
        } else {
            if (cache_num < cache_size) {
                cache_arr[cache_num] = target;
                valid_arr[cache_num] = 1;
                cache_num++;
                pointer = cache_num % cache_size;
            } else {
                while (true) {
                    if (valid_arr[pointer] == 0) {
                        cache_arr[pointer] = target;
                        valid_arr[pointer] = 1;
                        pointer = (pointer + 1) % cache_size;
                        break;
                    }
                    valid_arr[pointer] = 0;
                    pointer = (pointer + 1) % cache_size;
                }
            }
            miss++;
        }
    }
    delete[] cache_arr;
    delete[] valid_arr;
}