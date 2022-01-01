#include <iostream>
#include <omp.h>
#include <vector>
#include <chrono>
#include <stdio.h>
#include <fstream>

#pragma GCC target("avx2")
#pragma GCC optimize("O3")

#define uc unsigned char
using namespace std;
using namespace std::chrono;

int i = 0;
long long width = 0;
long long height = 0;
int mx = 0;
float COEF;

uc MAX = 0, MIN = 255;
vector<uc> data;

long long count[256];
uc new_color[256];

struct Timer
{
    string msg;
    system_clock::time_point start = high_resolution_clock::now();

    Timer(const string message)
    {
        msg = message;
    }

    int end()
    {
        system_clock::time_point end = high_resolution_clock::now();
        milliseconds duration = duration_cast<milliseconds>(end - start);
        cout << msg << "; Time: " << duration.count() << " ms"
             << "\n";
        return duration.count();
    }
};

void wrong()
{
    cout << "Wrong file header";
    exit(-1);
}

void readFile(const string &filename)
{
    streampos Size;
    ifstream file(filename, ios::binary);

    file.seekg(0, ios::end);
    Size = file.tellg();
    file.seekg(0, ios::beg);

    vector<uc> bytes(Size);

    file.read((char *)&bytes[0], Size);
    data = bytes;
    file.close();
}

uc shift_color(const uc value)
{
    int new_value = (value - MIN) * COEF;
    return max(0, min(new_value, 255));
}

void precalc() {
    #pragma omp parallel for
    for(int j = 0; j < 256; j++) {
        new_color[j] = shift_color(j);
    }
}

int input(const uc stopWord)
{
    int res = 0;
    while (true)
    {
        if (data[i] == stopWord)
        {
            i++;
            break;
        }
        res = res * 10 + (data[i++] - '0');
    }
    return res;
}

void init(const string &fileName)
{
    readFile(fileName);
    i = 3;
    width = input(' ');
    height = input('\n');
    mx = input('\n');
}

void find_min(const long long value) {
    int l = 0; 
    int r = 256;
    while (l + 1 < r) {
        int m = (l + r) / 2;
        if (count[m] - 1 >= value) 
        {
            r = m;
        }
        else 
        {
            l = m;
        }
    }
    MIN = r;
}

void find_max(const long long value) {
    int l = 0; 
    int r = 256;
    while (l + 1 < r) {
        int m = (l + r) / 2;
        if (count[m] + 1 >= value) 
        {
            r = m;
        }
        else 
        {
            l = m;
        }
    }
    MAX = r;
}

int main(int argc, char* argv[])
{   
    int threads = atoi(argv[1]);
    float MISS_COEF = atof(argv[4]);
    // int chunk_size = 7;

    omp_set_num_threads(threads);
    // omp_set_schedule(omp_sched_static, chunk_size);

    bool min_found = false, max_found = false;
    
    ofstream tester("test.txt", ios::app);
    ofstream output(argv[3], ios::binary);

    init(argv[2]);

    Timer algo = Timer("");

    if (data[0] == 'P')
    {
        if (data[1] == '5' || data[1] == '6')
        {

            long long pixels;
            if (data[1] == '5')
            {
                pixels = height * width;
            }
            else
            {
                pixels = height * width * 3;
            }
            
            long long cnt = 0;
            long long left = pixels * MISS_COEF;
            long long right = pixels * (1.0 - MISS_COEF);
            
            // cout << left << " " << right << "\n";
            
            for (long long t = i; t < pixels + i - 1; t += 1)
            {
                count[data[t]] += 1;
            }

            for(int j = 1; j < 256; j++) {
                count[j] += count[j - 1];
            }
            #pragma omp parallel 
            {
                find_min(left);
                find_max(right);
            }
            
            // for (int j = 0; j < 256; j++)
            // {

            //     cnt += count[j];

            //     #pragma omp parallel
            //     {
            //         if ((!min_found) && (cnt - 1 >= left))
            //         {
            //             min_found = true;
            //             MIN = j;
            //         }
            //         if ((!max_found) && (cnt + 1 >= right))
            //         {

            //             max_found = true;
            //             MAX = j;
            //         }
            //     }
            // }
            // cout << (int)MIN << " " << (int)MAX << "\n";

            if (MAX == MIN)
            {
                COEF = 1.0;
                MIN = 0;
            }
            else
            {
                COEF = (255 / (float)(MAX - MIN));
            }
            precalc();

        #pragma omp parallel for
            for (int t = i; t < pixels + i - 1; t++)
            {
                data[t] = new_color[data[t]];
            }
        }
        else
        {
            wrong();
        }
    }
    else
        wrong();

    // algo.end();
    // tester << "Threads: " << threads << "   Time: " << algo.end() << " ms" << "\n";
    
    tester << algo.end() << " ";
    output.write((char *)&data[0], data.size());

    output.close();
    tester.close();

    return 0;
}