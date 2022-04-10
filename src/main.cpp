#include <pthread.h>
#include <semaphore.h>
#include <queue>
#include <stdlib.h>
#include <unistd.h>
#include <thread>
#include <chrono>
#include <iostream>
#include <random>

#include "../include/Log/logger.h"

// typedef void *(*THREADFUNCPTR)(void *);

// Section to initialize global calls and shared resources
// ==================================================
struct Communication
{
    int tellerID = -1;
    int customerID = -1;
    int transaction = -1;
    bool complete = false;
};

static const int MAX_TELLER = 3;
static const int MAX_CUSTOMER = 2;

bool complete;
int served;

bool tell1_ready, tell2_ready, tell3_ready;
sem_t tell1, tell2, tell3;
Communication coms1, coms2, coms3;

sem_t Tellers, Manager, Vault;
std::queue<int> line;

void *customer(void *tid);

void *teller1(void *arg);
void *teller2(void *arg);
void *teller3(void *arg);

void run();
// ==================================================

int main()
{
    // Initialize all variables and semaphores
    // ==================================================
    tell1_ready = false;
    tell2_ready = false;
    tell3_ready = false;

    complete = false;
    served = 0;

    sem_init(&Tellers, 0, 3);
    sem_init(&Manager, 0, 1);
    sem_init(&Vault, 0, 2);

    sem_init(&tell1, 0, 1);
    sem_init(&tell2, 0, 1);
    sem_init(&tell3, 0, 1);
    // ==================================================

    run();
    zotikos::logger::log(zotikos::log_state::QUIT) << "The bank closes for the day.";

    // Destroy semaphores before exiting program
    // ==================================================
    sem_destroy(&Tellers);
    sem_destroy(&Manager);
    sem_destroy(&Vault);
    sem_destroy(&tell1);
    sem_destroy(&tell2);
    sem_destroy(&tell3);
    // ==================================================

    return 0;
}

// Method implementations
// ==================================================
void *customer(void *arg)
{
    int id = *(static_cast<int *>(arg));
    delete (int *)arg;

    // Customer decides on a transaction
    // ==================================================
    int transaction = std::rand() % 2;
    if (transaction == 0)
        zotikos::logger::log() << "Customer " << id << " wants to perform a withdrawal transaction.";
    else
        zotikos::logger::log() << "Customer " << id << " wants to perform a deposit transaction.";
    // ==================================================

    // Customer decides when to leave for the bank
    // ==================================================
    int sleep = std::rand() % 5000;
    std::this_thread::sleep_for(std::chrono::milliseconds(sleep));

    zotikos::logger::log() << "Customer " << id << " is going to the bank.";
    // ==================================================

    // Customer gets in line at the bank
    // ==================================================
    line.push(id);
    served++;
    zotikos::logger::log() << "Customer " << id << " is getting in line.";
    // ==================================================

    while (line.front() != id)
    {
        continue;
    }
    line.pop();

    // Customer chooses a teller and communicates with them
    // ==================================================
    sem_wait(&Tellers);
    zotikos::logger::log() << "Customer " << id << " is selecting a teller.";
    Communication *temp;
    sem_t *talk;
    if (tell1_ready)
    {
        temp = &coms1;
        talk = &tell1;
    }
    else if (tell2_ready)
    {
        temp = &coms2;
        talk = &tell2;
    }
    else if (tell3_ready)
    {
        temp = &coms3;
        talk = &tell3;
    }

    zotikos::logger::log() << "Customer " << id << " goes to Teller " << temp->tellerID << ".";
    sem_wait(talk);
    zotikos::logger::log() << "Customer " << id << " introduces itself to Teller " << temp->tellerID << ".";
    temp->customerID = id;
    sem_post(talk);

    std::this_thread::sleep_for(std::chrono::milliseconds(50));

    sem_wait(talk);
    if (transaction == 0)
        zotikos::logger::log() << "Customer " << id << " asks for a withdrawal transaction.";
    else
        zotikos::logger::log() << "Customer " << id << " asks for a deposit transaction.";

    temp->transaction = transaction;
    sem_post(talk);
    // ==================================================

    std::this_thread::sleep_for(std::chrono::milliseconds(50));

    // Customer leaves the bank
    // ==================================================
    sem_wait(talk);
    zotikos::logger::log() << "Customer " << id << " thanks Teller " << temp->tellerID << " and leaves.";
    temp->complete = true;
    sem_post(talk);
    sem_post(&Tellers);
    // ==================================================

    temp = NULL;
    delete temp;
    sem_destroy(talk);
    return NULL;
}

void teller(sem_t &tell, int &id, Communication &shared)
{
    while (true)
    {
        if (id == 0)
        {
            tell1_ready = true;
        }
        else if (id == 1)
        {
            tell2_ready = true;
        }
        else
        {
            tell3_ready = true;
        }
        shared.complete = false;
        shared.customerID = -1;
        shared.transaction = -1;
        zotikos::logger::log() << "Teller " << id << " is ready to serve.";
        zotikos::logger::log() << "Teller " << id << " waiting for a customer.";

        while (shared.customerID == -1 && !complete)
        {
            continue;
        }

        if (complete && shared.customerID == -1)
            break;

        sem_wait(&tell);
        zotikos::logger::log() << "Teller " << id << " is serving Customer " << shared.customerID << ".";

        if (id == 0)
        {
            tell1_ready = false;
        }
        else if (id == 1)
        {
            tell2_ready = false;
        }
        else
        {
            tell3_ready = false;
        }
        sem_post(&tell);

        while (shared.transaction == -1)
        {
            continue;
        }

        sem_wait(&tell);
        if (shared.transaction == 0)
        {
            zotikos::logger::log() << "Teller " << id << " is handling the withdrawal transaction.";
            zotikos::logger::log() << "Teller " << id << " is going to the manager.";
            sem_wait(&Manager);
            int sleep = 5 + (std::rand() % 26);
            std::this_thread::sleep_for(std::chrono::milliseconds(sleep));
            zotikos::logger::log() << "Teller " << id << " got the manager's permission.";
            sem_post(&Manager);
        }
        else
        {
            zotikos::logger::log() << "Teller " << id << " is handling the deposit transaction.";
        }

        zotikos::logger::log() << "Teller " << id << " is going to the safe.";
        sem_wait(&Vault);
        zotikos::logger::log() << "Teller " << id << " is in the safe.";

        int sleep = 10 + (std::rand() % 41);
        std::this_thread::sleep_for(std::chrono::milliseconds(sleep));

        zotikos::logger::log() << "Teller " << id << " is leaving the safe.";
        sem_wait(&Vault);

        if (shared.transaction == 0)
        {
            zotikos::logger::log() << "Teller " << id << " finishes Customer " << shared.customerID << "'s withdrawal transaction.";
        }
        else
        {
            zotikos::logger::log() << "Teller " << id << " finishes Customer " << shared.customerID << "'s deposit transaction.";
        }
        sem_post(&tell);

        while (!shared.complete)
        {
            continue;
        }

        if (served == MAX_CUSTOMER)
            complete = true;
    }

    zotikos::logger::log(zotikos::log_state::QUIT) << "Teller " << id << " is leaving for the day.";

    return;
}

void *teller1(void *arg)
{
    int id = 0;

    coms1.tellerID = id;
    teller(tell1, id, coms1);

    return NULL;
}

void *teller2(void *arg)
{
    int id = 1;

    coms2.tellerID = id;
    teller(tell1, id, coms2);

    return NULL;
}

void *teller3(void *arg)
{
    int id = 2;

    coms3.tellerID = id;
    teller(tell3, id, coms3);

    return NULL;
}

// No Teller or customer class, instead create method. Create a shared Communication struct for the interaction
// Hard code 3 tellers and 3 communication structs
// Also shared is a manager semaphore and safe semaphore
void run()
{
    pthread_t t_threads[MAX_TELLER];
    pthread_t c_threads[MAX_CUSTOMER];

    int rc;
    rc = pthread_create(&t_threads[0], NULL, teller1, NULL);

    if (rc)
    {
        zotikos::logger::log(zotikos::log_state::ERROR) << "Teller 0 Thread creation failed : " << rc;
        return;
    }

    rc = pthread_create(&t_threads[1], NULL, teller2, NULL);

    if (rc)
    {
        zotikos::logger::log(zotikos::log_state::ERROR) << "Teller 1 Thread creation failed : " << rc;
        return;
    }

    rc = pthread_create(&t_threads[2], NULL, teller3, NULL);

    if (rc)
    {
        zotikos::logger::log(zotikos::log_state::ERROR) << "Teller 2 Thread creation failed : " << rc;
        return;
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    for (int i = 0; i < MAX_CUSTOMER; i++)
    {
        int *tmp = new int;
        *tmp = i;
        rc = pthread_create(&c_threads[i], NULL, customer, static_cast<void *>(tmp));

        if (rc)
        {
            zotikos::logger::log(zotikos::log_state::ERROR) << "Customer " << i << " Thread creation failed : " << rc;
            return;
        }
    }

    for (int i = 0; i < MAX_CUSTOMER; i++)
    {
        pthread_join(c_threads[i], NULL);
    }

    for (int i = 0; i < MAX_TELLER; i++)
    {
        pthread_join(t_threads[i], NULL);
    }
}
// ==================================================