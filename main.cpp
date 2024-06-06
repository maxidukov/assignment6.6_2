#include <iostream>
#include <string>

#include <Wt/Dbo/Dbo.h>
#include <Wt/Dbo/backend/Postgres.h>


#ifdef _WIN32
#include <Windows.h>
#endif

#pragma execution_character_set("utf-8")

#include "all_classes.h"

int main()
{

    setlocale(LC_ALL, "Russian");
    #ifdef _WIN32
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);
    #endif

    try{
        std::string connectionString =
            "host=localhost"
            " port=5432"
            " dbname=postgres"
            " user=postgres"
            " password=password";
        //std::unique_ptr<Wt::Dbo::backend::Postgres> connection;
        auto connection = std::make_unique<Wt::Dbo::backend::Postgres>(connectionString);
        Wt::Dbo::Session session;
        session.setConnection(std::move(connection));
        session.mapClass<Book>("book");
        session.mapClass<Publisher>("publisher");
        session.mapClass<Sale>("sale");
        session.mapClass<Shop>("shop");
        session.mapClass<Stock>("stock");
        session.createTables();

        std::unique_ptr<Book> book1{ new Book() };
        book1->title = "Book1";
        std::unique_ptr<Book> book2 = std::make_unique<Book>();
        book2->title = "Book2";
        std::unique_ptr<Book> book3{ new Book{"Book3"}};
        std::unique_ptr<Book> book4{ new Book{"Book4"}};

        std::unique_ptr<Publisher> publisher1{ new Publisher{"Publisher1"}};
        std::unique_ptr<Publisher> publisher2{ new Publisher{"Publisher2"}};

        std::unique_ptr<Shop> shop1{ new Shop{"Shop1"}};
        std::unique_ptr<Shop> shop2{ new Shop{"Shop2"}};
        std::unique_ptr<Shop> shop3{ new Shop{"Shop3"}};

        std::unique_ptr<Stock> stock1{ new Stock{30}};
        std::unique_ptr<Stock> stock2{ new Stock{30}};
        std::unique_ptr<Stock> stock3{ new Stock{30}};
        std::unique_ptr<Stock> stock4{ new Stock{30}};
        std::unique_ptr<Stock> stock5{ new Stock{30}};
        std::unique_ptr<Stock> stock6{ new Stock{30}};

        std::unique_ptr<Sale> sale1{ new Sale{100,"2024-01-23",2}};
        std::unique_ptr<Sale> sale2{ new Sale{100,"2024-01-24",1}};
        std::unique_ptr<Sale> sale3{ new Sale{100,"2024-02-10",3}};

        Wt::Dbo::Transaction transaction{session};

        //PUBLISHERS AND SHOPS, DO NOT BELONG TO ANYTHING
        Wt::Dbo::ptr<Publisher> publisher1ptr = session.add<Publisher>(std::move(publisher1));
        Wt::Dbo::ptr<Publisher> publisher2ptr = session.add<Publisher>(std::move(publisher2));
        Wt::Dbo::ptr<Shop> shop1ptr = session.add<Shop>(std::move(shop1));
        Wt::Dbo::ptr<Shop> shop2ptr = session.add<Shop>(std::move(shop2));
        Wt::Dbo::ptr<Shop> shop3ptr = session.add<Shop>(std::move(shop3));
        //BOOKS
        Wt::Dbo::ptr<Book> book1ptr = session.add<Book>(std::move(book1));
        book1ptr.modify()->publisher = publisher1ptr;
        Wt::Dbo::ptr<Book> book2ptr = session.add<Book>(std::move(book2));
        book2ptr.modify()->publisher = publisher1ptr;
        Wt::Dbo::ptr<Book> book3ptr = session.add<Book>(std::move(book3));
        book3ptr.modify()->publisher = publisher1ptr;
        Wt::Dbo::ptr<Book> book4ptr = session.add<Book>(std::move(book4));
        book4ptr.modify()->publisher = publisher2ptr;
        //STOCKS
        Wt::Dbo::ptr<Stock> stock1ptr = session.add<Stock>(std::move(stock1));
        stock1ptr.modify()->shop = shop1ptr;
        stock1ptr.modify()->book = book1ptr;
        Wt::Dbo::ptr<Stock> stock2ptr = session.add<Stock>(std::move(stock2));
        stock2ptr.modify()->shop = shop1ptr;
        stock2ptr.modify()->book = book2ptr;
        Wt::Dbo::ptr<Stock> stock3ptr = session.add<Stock>(std::move(stock3));
        stock3ptr.modify()->shop = shop1ptr;
        stock3ptr.modify()->book = book3ptr;
        Wt::Dbo::ptr<Stock> stock4ptr = session.add<Stock>(std::move(stock4));
        stock4ptr.modify()->shop = shop2ptr;
        stock4ptr.modify()->book = book4ptr;
        Wt::Dbo::ptr<Stock> stock5ptr = session.add<Stock>(std::move(stock5));
        stock5ptr.modify()->shop = shop2ptr;
        stock5ptr.modify()->book = book1ptr;
        Wt::Dbo::ptr<Stock> stock6ptr = session.add<Stock>(std::move(stock6));
        stock6ptr.modify()->shop = shop3ptr;
        stock6ptr.modify()->book = book4ptr;
        //SALES
        Wt::Dbo::ptr<Sale> sale1ptr = session.add<Sale>(std::move(sale1));
        sale1ptr.modify()->stock = stock1ptr;
        Wt::Dbo::ptr<Sale> sale2ptr = session.add<Sale>(std::move(sale2));
        sale2ptr.modify()->stock = stock1ptr;
        Wt::Dbo::ptr<Sale> sale3ptr = session.add<Sale>(std::move(sale3));
        sale3ptr.modify()->stock = stock4ptr;

        //QUERY
        typedef Wt::Dbo::collection<Wt::Dbo::ptr<Shop>> Shops;
        auto pnames = session.query<std::string>("select publisher.name from publisher").resultList();
        std::cout << "В базе данных есть следующие издатели:\n";
        for(const auto& elem : pnames){
            std::cout << elem << "\n";
        }
        std::string pname;
        std::cout << "Введите название интересующего Вас издателя: ";
        std::cin >> pname;
        auto res = session.query<std::string>("select sh.name from shop sh"
                                       "").join<Stock>("st","sh.id = st.shop_id").join<Book>("b","b.id=st.book_id").join<Publisher>("p","p.id = b.publisher_id"
                                             "").groupBy("sh.name").where("p.name = ?").bind(pname).resultList();
        // auto res = session.find<Shop>().join<Stock>("st","sh.id = st.shop_id").join<Book>("b","b.id=st.book_id").join<Publisher>("p","p.id = b.publisher_id"
        //                 "").groupBy("sh.name").where("p.name = ?").bind("Publisher2");

        std::cout << "Cписок магазинов, в которых можно купить книги выбранного издателя:\n";
        for(const auto& elem : res){
            std::cout << elem << "\n";
        }
        auto res2 = session.query<std::string>("select sh.name from shop sh"
                                              "").join<Stock>("st","sh.id = st.shop_id"
                                           "").join<Sale>("s","s.stock_id = st.id").join<Book>("b","b.id=st.book_id").join<Publisher>("p","p.id = b.publisher_id"
                                             "").groupBy("sh.name").where("p.name = ?").bind(pname).resultList();
        std::cout << "Cписок магазинов, в которых были фактические продажи книг выбранного издателя:\n";
        for(const auto& elem : res2){
            std::cout << elem << "\n";
        }

        // auto res_test = session.find<Shop>().where("name <> ?").bind("Shop10").resultList();
        // for(const auto& elem : res2){
        //     std::cout << elem->name << "\n";
        // }
        transaction.commit();

        session.dropTables();

    }catch(const Wt::Dbo::Exception& e){
        std::cout << e.what() << std::endl;
    }

    return 0;
}



