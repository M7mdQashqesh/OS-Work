// Edit by: Mohammed Qashqesh
// ID: 211014
// Date and Time: 30/3/2024  ,  11:59 PM
// This is cpp ( C++ ) Program.

#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>

using namespace std;
using namespace chrono;
using namespace this_thread;

mutex mtx;
condition_variable cv;
bool ready = false;

// Display a message, preceded by
// the name of the current thread
void threadMessage(const string& message) {
    cout << "Thread " << get_id() << ": " << message << endl;
}
void messageLoop() {
    string importantInfo[] = {
        "Mares eat oats",
        "Does eat oats",
        "Little lambs eat ivy",
        "A kid will eat ivy too"
    };
    try {
        for (const string& info : importantInfo) {
            {
                lock_guard<mutex> lock(mtx);
                // Print a message
                threadMessage(info);
            }
            sleep_for(seconds(4));
        }
    }
    catch (const exception& e) {
        threadMessage("I wasn't done!");
    }
    ready = true;
    cv.notify_one();
}

int main(int argc, char* argv[]) {
    // Delay, in milliseconds before
    // we interrupt MessageLoop
    // thread (default one hour).
    long patience = 1000 * 60 * 60;

    // If command line argument
    // present, gives patience
    // in seconds.
    if (argc > 1) {
        try {
            patience = stol(argv[1]) * 1000;
        }
        catch (const invalid_argument& e) {
            cerr << "Argument must be an integer." << endl;
            return 1;
        }
    }

    threadMessage("Starting MessageLoop thread");
    std::thread t(messageLoop);

    auto startTime = steady_clock::now();
    {
        unique_lock<mutex> lock(mtx);
        while (!ready) {
            threadMessage("Still waiting...");
            // Wait maximum of 1 second
            // for MessageLoop thread
            // to finish.
            if (cv.wait_for(lock, milliseconds(1000)) == cv_status::timeout) {
                if ((steady_clock::now() - startTime) > milliseconds(patience)) {
                    threadMessage("Tired of waiting!");
                    if (t.joinable()) {
                        t.join();
                    }
                    return 1;
                }
            }
        }
    }

    if (t.joinable()) {
        threadMessage("Finally!");
        t.join();
    }

    system("pause");
    return 0;
}