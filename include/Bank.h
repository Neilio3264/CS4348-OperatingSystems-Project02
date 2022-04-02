#include <pthread.h>
#include <semaphore.h>

class Bank
{
private:
    static const int MAX_TELLER = 3;
    static const int MAX_CUSTOMER = 100;

    sem_t TellerService, ChooseTeller, AskManager, EnterVault;

public:
    Bank(/* args */);
    ~Bank();

    class Teller
    {
    private:
        bool is_ready;
        int has_customer = -1;

        /**
         * @brief Transition is state for which action a customer has prompted
         *
         * @param State0 null or no action received
         * @param State1 Withdraw
         * @param State2 Deposit
         */
        int transition;

    public:
        Teller(/* args */);
        ~Teller();

        bool isReady();
        bool hasCustomer();

        int getState();

        void updateCustomer(int id);
        void updateState(int state);
    };
};

Bank::Bank(/* args */)
{
}

Bank::~Bank()
{
}

Bank::Teller::Teller(/* args */)
{
    is_ready = false;
    has_customer = false;
    transition = 0;
}

Bank::Teller::~Teller()
{
}

bool Bank::Teller::isReady()
{
    return is_ready;
}

bool Bank::Teller::hasCustomer()
{
    if (has_customer < 0)
        return false;

    return true;
}

int Bank::Teller::getState()
{
    return transition;
}

void Bank::Teller::updateCustomer(int id)
{
    has_customer = id;
}

void Bank::Teller::updateState(int state)
{
    if (state < 3 && state > -1)
        transition = state;
}