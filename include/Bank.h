#include <pthread.h>
#include <semaphore.h>
#include <queue>
#include <stdlib.h>
#include <unistd.h>
#include <thread>
#include <chrono>
#include <iostream>
#include <random>

#include "Log/logger.h"

struct Communication
{
    int tellerID = -1;
    int customerID = -1;
    int transaction = -1;
    bool complete = false;
};

class Bank
{
private:
    static const int MAX_TELLER = 3;
    static const int MAX_CUSTOMER = 6;
    bool complete;

    bool tell1_ready, tell2_ready, tell3_ready;
    sem_t tell1, tell2, tell3;
    Communication coms1, coms2, coms3;

    sem_t Tellers, Manager, Vault;
    std::queue<int> line;

    void *customer(void *tid);

    void teller(sem_t &tell, int &id, Communication &shared);
    void *teller1(void *arg);
    void *teller2(void *arg);
    void *teller3(void *arg);

public:
    Bank();
    ~Bank();

    void run();
};