class Teller
{
private:
    bool is_ready;
    bool has_customer;

    /**
     * @brief Transition is state for which action a customer has prompted
     * State 0 = null or no action received
     * State 1 = Withdraw
     * State 2 = Deposit
     */
    int transition;

public:
    Teller(/* args */);
    ~Teller();

    bool isReady();
    bool hasCustomer();

    int getState();

    void updateCustomer();
    void updateState(int state);
};

Teller::Teller(/* args */)
{
    is_ready = false;
    has_customer = false;
    transition = 0;
}

Teller::~Teller()
{
}

bool Teller::isReady()
{
    return is_ready;
}

bool Teller::hasCustomer()
{
    return has_customer;
}

int Teller::getState()
{
    return transition;
}

void Teller::updateCustomer()
{
    has_customer = !has_customer;
}

void Teller::updateState(int state)
{
    if (state < 3 && state > -1)
        transition = state;
}