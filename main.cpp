#include "main.h"
#include "restaurant.cpp"

int main(int argc, char* argv[]) {
    clock_t start,end;
    double cpu_time_used;
    start=clock();
    string fileName = "test.txt";
    simulate(fileName);
    end=clock();
    cpu_time_used=(double)(end-start)/ CLOCKS_PER_SEC;
    cout<<endl<<cpu_time_used;
    return 0;
}
