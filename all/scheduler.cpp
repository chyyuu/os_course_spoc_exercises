#include <iostream>
#incldue <cstdlib>
#include <vector>
#include <string.h>
#include <cstring>

using namespace std;

class Scheduler {

    public:
        Scheduler();
        ~Scheduler();
        void run(int type);

    private:
        vector<pait<int,int>> jobList;




}
