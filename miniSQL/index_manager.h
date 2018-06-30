//
// Created by ktwzj on 2018/6/27.
//

#ifndef INDEXMANAGER_INDEX_MANAGER_H
#define INDEXMANAGER_INDEX_MANAGER_H


#include <iostream>
#include "type.h"
#include "b_plus_tree.h"
#define INDEX_BLOCK_SIZE 1000000
namespace CatalogManager {
    class CatalogManager{
        std::string getFileName(std::string);
    };

}


namespace IndexManager{
    
    
    
    class IndexManager {
    public:
        IndexManager();
        ~IndexManager();
		int createIndex(std::string name, rua type);
		int dropIndex(std::string name);
		MyCharType toMyCharType(std::string s);
        void setName(std::string name, std::string attr);
        //void deleteMany();
        int Delete(int key);
		int Delete(std::string key);
		int Delete(float key);
		int Insert(int key, addressType data);
		int Insert(std::string key , addressType data);
		int Insert(float key, addressType data);
		std::vector<addressType> Search(int key, int searchType);
		std::vector<addressType> Search(float key, int searchType);
		std::vector<addressType> Search(std::string key, int searchType);

    private:
		int readIndexToMemory(std::string file,void* memory, int size);
		int writeIndexFromMemory(std::string file, void* memory, int size);
		void *currentMemory;
		rua bPlusTreeType;
		std::string attrName;
		BPlusTree<BPlusNodeInt, IntKey> *bPlusTreeInt;
		BPlusTree<BPlusNodeChar, CharKey> *bPlusTreeChar;
		BPlusTree<BPlusNodeFloat, FloatKey> *bPlusTreeFloat;
        
        int currentMemorySize;
        rua currentType;
        std::string currentIndexName;
        void saveIndexFileCatalog(std::string indexName, std::string indexFile);
    };



}

#endif //INDEXMANAGER_INDEX_MANAGER_H
