// Check MEGA links
// Copyright (C) 2018 Slek.

//STL
#include <iostream>
#include <string>

//MEGA SDK
#include "mega.h"

using namespace mega;

bool finished;
MegaClient* client;

string errorstring(error e) {
    switch (e) {
        case API_OK:
            return "No error";
        case API_EINTERNAL:
            return "Internal error";
        case API_EARGS:
            return "Invalid argument";
        case API_EAGAIN:
            return "Request failed, retrying";
        case API_ERATELIMIT:
            return "Rate limit exceeded";
        case API_EFAILED:
            return "Transfer failed";
        case API_ETOOMANY:
            return "Too many concurrent connections or transfers";
        case API_ERANGE:
            return "Out of range";
        case API_EEXPIRED:
            return "Expired";
        case API_ENOENT:
            return "Not found";
        case API_ECIRCULAR:
            return "Circular linkage detected";
        case API_EACCESS:
            return "Access denied";
        case API_EEXIST:
            return "Already exists";
        case API_EINCOMPLETE:
            return "Incomplete";
        case API_EKEY:
            return "Invalid key/integrity check failed";
        case API_ESID:
            return "Bad session ID";
        case API_EBLOCKED:
            return "Blocked";
        case API_EOVERQUOTA:
            return "Over quota";
        case API_ETEMPUNAVAIL:
            return "Temporarily not available";
        case API_ETOOMANYCONNECTIONS:
            return "Connection overflow";
        case API_EWRITE:
            return "Write error";
        case API_EREAD:
            return "Read error";
        case API_EAPPKEY:
            return "Invalid application key";
        case API_EGOINGOVERQUOTA:
            return "Not enough quota";
        default:
            return "Unknown error";
    }
}

struct CheckApp : public MegaApp {
    virtual void checkfile_result(handle, error e) override {
        cout << "Link check failed: " << errorstring(e) << endl;
        finished = true;
    }
    virtual void checkfile_result(handle, error e, byte*, m_off_t, m_time_t, m_time_t, string* filename, string*, string*) override {
        if (e) cout << "Not available: " << errorstring(e) << endl;
        else cout << *filename << endl;
        finished = true;
    }
};

struct DummyWaiter : public Waiter {
    int wait() { return Waiter::NEEDEXEC; }
    void notify() { }
};

int main () {
    // instantiate app components: the callback processor (CheckApp),
    // the HTTP I/O engine and the MegaClient itself
    client = new MegaClient(new CheckApp, new DummyWaiter,
                            new HTTPIO_CLASS, new FSACCESS_CLASS,
                            nullptr,
                            nullptr,
                            "Gk8DyQBS",
                            "megack");

    // main loop
    for (string line; getline(cin, line); ) {
        finished = false;

        if (error e = client->openfilelink(line.c_str(), 0)) {
            cout << "Link check failed: " << errorstring(e) << endl;
            continue;
        }

        // polling loop
        while (!finished) {
            client->exec();
            client->wait();
        }
    }

    return 0;
}
