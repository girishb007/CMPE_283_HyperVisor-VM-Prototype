#include <iostream>
#include <sys/time.h>
#include <sys/resource.h>
#include <unistd.h>

using namespace std;

int main() {
    struct rusage usage;
    long int prev_voluntary_switches = 0;
    long int prev_involuntary_switches = 0;

    while (true) {
        getrusage(RUSAGE_SELF, &usage);

        if (usage.ru_nvcsw > prev_voluntary_switches) {
            cout << "Voluntary context switch to another process or VM" << endl;
        }

        if (usage.ru_nivcsw > prev_involuntary_switches) {
            cout << "Involuntary context switch to another process or VM" << endl;
        }

        prev_voluntary_switches = usage.ru_nvcsw;
        prev_involuntary_switches = usage.ru_nivcsw;

        // Wait for some time before checking again
        sleep(1);
    }

    return 0;
}
