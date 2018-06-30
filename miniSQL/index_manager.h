//
// Created by ktwzj on 2018/6/27.
//

#ifndef INDEXMANAGER_INDEX_MANAGER_H
#define INDEXMANAGER_INDEX_MANAGER_H


#include <iostream>
#include "b_plus_tree.h"
namespace CatalogManager {
    class CatalogManager{
        std::string getFileName(std::string);
    };

}


namespace IndexManager{
    enum rua {
        INT,FLOAT,CHAR
    };
    class singleAttr{
    public:
        std::string attrname;
        rua type;
        int length;
    };
    union ourType {
        int intData;
        float floatData;
        char charData;
    };

    class IndexManager {
    public:
        IndexManager();
        ~IndexManager();
        int createIndex(std::string name, singleAttr attr);
        void setName(std::string name);
        void deleteMany();
        void deleteOne();
        int insert(ourType key, addressType data);
        vector<addressType> search(ourType key, int searchType);

    private:
        void *currentMemory;
        int currentMemorySize;
        rua currentType;
        std::string currentIndexName;
        void saveIndexFileCatalog(std::string indexName, std::string indexFile);
    };


    IndexManager::IndexManager(CatalogManager *catalogManager, BufferManager* bufferManager) {

    }
    void IndexManager::switchIndex(std::string name);
    void IndexManager::setName(std::string name) {
        currentIndexName = name;
    }

    int IndexManager::createIndex(std::string name, singleAttr attr){
        saveIndexFileCatalog(name, name+".index");
        switch(attr.type){
            case INT:
                BPlusTree<BPlusNodeInt,IntKey> bPlusTree = BPlusTree<BPlusNodeInt,IntKey>(currentMemory);
                break;
            case CHAR:
                BPlusTree<BPlusNodeChar,CharKey> bPlusTree = BPlusTree<BPlusNodeChar,CharKey>(currentMemory);
                break;
            case FLOAT:
                BPlusTree<BPlusNodeFloat,FloatKey> bPlusTree = BPlusTree<BPlusNodeFloat,FloatKey>(currentMemory);
                break;
        }
    }

    int IndexManager::insert(ourType key, addressType data) {

        switch(currentType){
            case INT:
                BPlusTree<BPlusNodeInt,IntKey> bPlusTree = BPlusTree<BPlusNodeInt,IntKey>(currentMemory);
                IntKey keyStructure;
                break;
            case CHAR:
                BPlusTree<BPlusNodeChar,CharKey> bPlusTree = BPlusTree<BPlusNodeChar,CharKey>(currentMemory);
                CharKey keyStructure;
                break;
            case FLOAT:
                BPlusTree<BPlusNodeFloat,FloatKey> bPlusTree = BPlusTree<BPlusNodeFloat,FloatKey>(currentMemory);
                FloatKey keyStructure;
                break;
        }
        keyStruture = key;
        keyStruture.data = data;
        return  bPlusTree.insert(keyStructure);
    }

    int IndexManager::deleteOne(ourType key) {

        switch(currentType){
            case INT:
                BPlusTree<BPlusNodeInt,IntKey> bPlusTree = BPlusTree<BPlusNodeInt,IntKey>(currentMemory);
                IntKey keyStructure;
                break;
            case CHAR:
                BPlusTree<BPlusNodeChar,CharKey> bPlusTree = BPlusTree<BPlusNodeChar,CharKey>(currentMemory);
                CharKey keyStructure;
                break;
            case FLOAT:
                BPlusTree<BPlusNodeFloat,FloatKey> bPlusTree = BPlusTree<BPlusNodeFloat,FloatKey>(currentMemory);
                FloatKey keyStructure;
                break;
        }
        keyStruture = key;
        keyStruture.data = data;
        return bPlusTree.remove(keyStructure);
    }
    vector<addressType> search(ourType key, int searchType){
        switch(currentType){
            case INT:
                BPlusTree<BPlusNodeInt,IntKey> bPlusTree = BPlusTree<BPlusNodeInt,IntKey>(currentMemory);
                IntKey keyStructure;
                break;
            case CHAR:
                BPlusTree<BPlusNodeChar,CharKey> bPlusTree = BPlusTree<BPlusNodeChar,CharKey>(currentMemory);
                CharKey keyStructure;
                break;
            case FLOAT:
                BPlusTree<BPlusNodeFloat,FloatKey> bPlusTree = BPlusTree<BPlusNodeFloat,FloatKey>(currentMemory);
                FloatKey keyStructure;
                break;
        }
        keyStructure = key;
        keyStructure.data = data;
        keyStructure.type = searchType;
        return bPlusTree.searchNodes(keyStructure);
    }

    void IndexManager::saveIndexFileCatalog(std::string indexName, std::string indexFile) {
        ;
    }

}

#endif //INDEXMANAGER_INDEX_MANAGER_H
