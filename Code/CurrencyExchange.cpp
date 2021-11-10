#include "CurrencyExchange.h"

CurrencyExchange::CurrencyExchange(float pS, CurrencyRatesProvider * c)
{
    pSell = pS;
    pBuy = 1 - pS;
    bernoulli = bernoulli_distribution(pSell);            // I consider selling crypto as a success
    uniform = uniform_real_distribution<float>(0.5, 1.0); // our traders are generous :)
    currencyRatesProvider = c;
}
float CurrencyExchange::calculate(float how_much, string of_what, string to_what)
{
    float rate = currencyRatesProvider->getRate(of_what, to_what);
    return how_much * rate;
}

void CurrencyExchange::toString()
{
    cout << "Welcome to best Currency Exchenge in the world! We encourage you to play here." << endl;
    cout << "PSell: " << pSell << endl;
    cout << "PBuy: " << pBuy << endl;
}

void CurrencyExchange::exchange(Currency *cur1, Currency *cur2, Wallet<Currency> *w)
{
    float how_much_to_exchange = cur1->getAmountInWallet() * uniform(generator);
    *cur1 -= how_much_to_exchange;

    float exchanged = calculate(how_much_to_exchange, cur1->getName(), cur2->getName());
    *cur2 += exchanged;
    cout << "We exchanged " << how_much_to_exchange << " of " << cur1->getName() << " to: " << exchanged << " of " << cur2->getName() << endl;

    if (cur1->getAmountInWallet() < 0.1)
    {
        cout << "Unfortunately balance is below 0.1 for " << cur1->getName() << ". It will be deleted from the wallet :(" << endl;
        *w -= cur1->getName();
    }
}

void CurrencyExchange::trade(Trader &trader)
{
    string decision;
    while (!trader.cryptoWallet->isEmpty() && !trader.fiatWallet->isEmpty())
    {
        trader.rankUp();
        //Selecting random currencies that we are going to exchange
        Currency *chosen_crypto = trader.cryptoWallet->getRandom();
        Currency *chosen_fiat = trader.fiatWallet->getRandom();
        if (bernoulli(generator))
        {
            exchange(chosen_crypto, chosen_fiat, trader.cryptoWallet);
        }
        else
        {
            exchange(chosen_fiat, chosen_crypto, trader.fiatWallet);
        }
        trader.toString(true);
        cout << "Do you want to continue (y/n)?: " << endl;
        cin >> decision;
        if (decision!="y") break;
    }
}

void CurrencyExchange::simulateRanking(){
    Trader trader("Wojtek");
    Currency * crypto1 = new CurrencyBackedCryptoCurrency<FiatCurrency>(30, "DolarCoin", rand() % 30, rand() % 10000, "dolar");
    Currency * crypto2 = new CurrencyBackedCryptoCurrency<CryptoCurrency>(60, "NiceCoin", rand() % 30, rand() % 10000, "TrololoCoin");
    trader.addCrypto(crypto1);
    trader.addCrypto(crypto2);


    cout << "Each user has it's rank. The more crypto you have the higher rank you obtain" << endl;
    trader.toString(true);
    for (int i = 2; i < 4; i++){
        Currency *chosen_crypto = trader.cryptoWallet->getRandom();
        *chosen_crypto += (float)pow(10,i);
        trader.rankUp();
        trader.toString(true);
    }
    string any;
    cout << "type anything to continue: ";
    cin >> any;
    cout << "But if you lose your crypto then you rankDown!" << endl;
    for (int i = 2; i < 4; i++){
        Currency *chosen_crypto = trader.cryptoWallet->getRandom();
        *chosen_crypto -= chosen_crypto->getAmountInWallet();
        trader.rankDown();
        trader.toString(true);
    }
    trader.toString(true);
    cout << "type anything to continue: ";
    cin >> any;
}

void CurrencyExchange::simulateMining(){
    cout << "Mining is possible, you can mine different crypto based on you rank" << endl;
    Trader trader("Lukasz");
    CryptoCurrency * crypto1 = new CurrencyBackedCryptoCurrency<FiatCurrency>(30, "DolarCoin", 5, rand() % 10000, "dolar");
    CryptoCurrency * crypto2 = new NotBackedCryptoCurrency(60, "NiceCoin", 10, "GCD");
    CryptoCurrency * crypto3 = new MemeCryptoCurrency(60, "DogeCoin", 3, "doge");
    
    crypto1->mine(trader.getRank());
    crypto2->mine(trader.getRank());

    cout << "DogeCoin amount: " << crypto3->getAmountInWallet() << endl;
    crypto3->mine(trader.getRank());

    for (size_t i = 0; i < 2; i++)
    {
        crypto3->mine(trader.getRank());
    }
    cout << "DogeCoin amount: " << crypto3->getAmountInWallet() << endl;
    crypto3->mine(trader.getRank());

    string any;
    cout << "type anything to continue: ";
    cin >> any;

    delete crypto1;
    delete crypto2;
    delete crypto3;

}

void CurrencyExchange::simulateBackedCurrencies(){
    cout << "Backed currencies can be exchanged on backing asset." << endl;
    //! Here C++ won with me :( I designed it poorly and it wouldn't generelize easily
    Wallet<CryptoCurrency> w("Sebastian");
    Wallet<FiatCurrency> w2("Sebastian");
    FiatCurrency *fiat = new FiatCurrency(0, "dolar", rand() % 2);
    CurrencyBackedCryptoCurrency<FiatCurrency> * crypto1 = new CurrencyBackedCryptoCurrency<FiatCurrency>(30, "DolarCoin", rand() % 30, rand() % 10000, "dolar");
    CurrencyBackedCryptoCurrency<CryptoCurrency> * crypto2 = new CurrencyBackedCryptoCurrency<CryptoCurrency>(60, "NiceCoin", rand() % 30, rand() % 10000, "DolarCoin");
    BackedCryptoCurrency * crypto3 = new CommodityBackedCryptoCurrency(60, "PigCoin", rand() % 30, rand() % 10000, "Pigs");

    crypto3->exchange(80);

    w+=crypto1;
    w2+=fiat;

    w.toString(true);
    w2.toString(true);

    crypto1->exchange(30, w2);
    crypto2->exchange(50, w);

    w.toString(true);
    w2.toString(true);
}