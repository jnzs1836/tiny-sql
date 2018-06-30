//
// Created by ktwzj on 2018/6/19.
//

#ifndef INDEXMANAGER_TREE_H
#define INDEXMANAGER_TREE_H
#define MAX_INT 100
//#define maxChildrenNum 100
#define MAX_CHAR 4

#include <iostream>
namespace IndexManager{

//    typedef struct {
//        int file;
//    } recordPointer;


    template <class T, class KeyType>
    class Tree{
    public:
        Tree(){
        };
        virtual ~Tree(){

        };
//        virtual bool insert(T key);
//        virtual bool remove(T key);
//        virtual bool update(KeyType oldKey, KeyType newKey);
//        virtual bool search();

    };
}


#endif //INDEXMANAGER_TREE_H
