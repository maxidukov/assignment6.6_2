#ifndef ALL_CLASSES_H
#define ALL_CLASSES_H

#include <Wt/Dbo/Dbo.h>
#include <Wt/Dbo/backend/Postgres.h>
#include <Wt/WDate.h>

class Book;
class Publisher;
class Sale;
class Shop;
class Stock;

class Book{
public:
    //int id = 0;
    std::string title = "";
    Wt::Dbo::ptr<Publisher> publisher;
    Wt::Dbo::collection<Wt::Dbo::ptr<Stock>> stocks;

    template<class Action>
    void persist(Action& a){
        //Wt::Dbo::field(a,id,"id");
        Wt::Dbo::field(a,title,"title");
        Wt::Dbo::belongsTo(a,publisher,"publisher");
        Wt::Dbo::hasMany(a,stocks,Wt::Dbo::ManyToOne,"book");
    }
};

class Publisher{
public:
    //int id = 0;
    std::string name = "";
    Wt::Dbo::collection<Wt::Dbo::ptr<Book>> books;

    template<class Action>
    void persist(Action& a){
        //Wt::Dbo::field(a,id,"id");
        Wt::Dbo::field(a,name,"name");
        Wt::Dbo::hasMany(a,books,Wt::Dbo::ManyToOne,"publisher");
    }
};

class Sale{
public:
    //int id = 0;
    int price = 0;
    //Wt::WDate date_sale = Wt::WDate(2000,1,1);
    std::string date_sale  = "";
    int count = 0;
    Wt::Dbo::ptr<Stock> stock;

    template<class Action>
    void persist(Action& a){
        //Wt::Dbo::field(a,id,"id");
        Wt::Dbo::field(a,price,"price");
        Wt::Dbo::field(a,date_sale,"date_sale");
        Wt::Dbo::field(a,count,"count");
        Wt::Dbo::belongsTo(a,stock,"stock");
    }

};

class Shop{
public:
    //int id = 0;
    std::string name = "";
    Wt::Dbo::collection<Wt::Dbo::ptr<Stock>> stocks;

    template<class Action>
    void persist(Action& a){
        //Wt::Dbo::field(a,id,"id");
        Wt::Dbo::field(a,name,"name");
        Wt::Dbo::hasMany(a,stocks,Wt::Dbo::ManyToOne,"shop");
    }
};

class Stock{
public:
    //int id = 0;
    int count = 0;
    Wt::Dbo::ptr<Book> book;
    Wt::Dbo::ptr<Shop> shop;
    Wt::Dbo::collection<Wt::Dbo::ptr<Sale>> sales;

    template<class Action>
    void persist(Action& a){
        //Wt::Dbo::field(a,id,"id");
        Wt::Dbo::field(a,count,"count");
        Wt::Dbo::belongsTo(a,book,"book");
        Wt::Dbo::belongsTo(a,shop,"shop");
        Wt::Dbo::hasMany(a,sales,Wt::Dbo::ManyToOne,"stock");
    }
};

#endif // ALL_CLASSES_H
