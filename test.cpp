#include "catch.hpp"
#include "AVLTree.h"
#include "word.h"
#include "HashTable.h"

using namespace std;

TEST_CASE("Testing the AVLTree"){
    AVLTree<int> intTree;
    SECTION("Testing basic functions"){
        for(int i = 0; i < 10; i++){
            intTree.insert(i);
        }
        REQUIRE(intTree.findMin() == 0);
        REQUIRE(intTree.findMax() == 9);
        REQUIRE(intTree.contains(2) == true);
        REQUIRE(intTree.contains(10) == false);
        REQUIRE(intTree.isEmpty() == false);
        REQUIRE(intTree.getTotalNodes() == 10);
        intTree.makeEmpty();
        REQUIRE(intTree.isEmpty() == true);

        //use this set of data to test all of the rotate functions
        intTree.insert(50);
        intTree.insert(30);
        intTree.insert(20);//there's a case 1 at this point
        intTree.insert(80);
        intTree.insert(70);//there's a case 3 at this point
        intTree.insert(10);
        intTree.insert(15);//there's a case 2 at this point
        intTree.insert(90);
        intTree.insert(100);//there's a case 4 at this point
        //print the tree out to see if all the values are inserted properly
        intTree.printTree();
    }

}

TEST_CASE("Testing the word class"){
    FreqInDoc freq(5,"Doc1");
    Word a("a",freq);
    Word b("b",freq);

    SECTION("Testing all functions"){
        REQUIRE(a<b);
    }
}

TEST_CASE("Testing the Hash Table"){
    HashTable<string,int> table;
    table.insert("1",1);
    table.insert("2",2);
    table.insert("3",3);
    SECTION("Testing constructors and assignment operator"){
        REQUIRE(table.get("1") == 1);
        //copy constryctor
        HashTable<string,int> table2 = table;
        REQUIRE(table2.getSize() == 3);
        REQUIRE(table2.get("2") == 2);
        HashTable<string,int> table3(50);
        table3.insert("word",10);
        //overloaded assignment operator
        table3 = table;
        REQUIRE(table3.getCapacity() == 100);
    }
    SECTION("Testing other functions"){
        //testing contains function
        REQUIRE(table.contains("2"));
        //testing remove function
        table.remove("3");
        REQUIRE(table.getSize() == 2);
        table.display();
    }
}

