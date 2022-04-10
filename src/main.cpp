#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#include "../include/Bank.h"

int main()
{
    Bank bank;
    bank.run();
    return 0;
}