#include "index_manager.h"

#include<iostream>
#include<vector>
#include<algorithm>
void *memory = malloc(1000000);
bool exist = false;
namespace IndexManager {

	IndexManager::IndexManager() {
		if (!exist) {
			std::cout << "Creating Indexing";
			currentMemory = memory;
			MetaData metaData;
			metaData.rootPosition = 0;
			metaData.num = 0;
			for (int i = 0; i < MAX_NODE_NUM; i++) {
				metaData.nodeMemoryTable.used[i] = false;
			}
			memcpy(currentMemory, &metaData, sizeof(MetaData));
			exist = true;

		}
		else {
			currentMemory = memory;
		}
		
	}

	IndexManager::~IndexManager() {
		if (!bPlusTreeChar) {
			delete bPlusTreeChar;
		}
		if (!bPlusTreeFloat) {
			delete bPlusTreeFloat;
		}
		if (!bPlusTreeInt) {
			delete bPlusTreeInt;
		}
	
	}



	void IndexManager::setName(std::string name, std::string attr) {
		
		currentIndexName = name;
		currentType = INT;
		bPlusTreeType = INT;
		attrName = attr;
		
		
		switch (currentType) {
		case INT:
			bPlusTreeInt = new BPlusTree<BPlusNodeInt, IntKey>(currentMemory);
			break;
		case CHAR:
			bPlusTreeChar = new BPlusTree<BPlusNodeChar, CharKey>(currentMemory);
			break;
		case FLOAT:
			bPlusTreeFloat = new BPlusTree<BPlusNodeFloat, FloatKey>(currentMemory);
			break;
		}

	}

	int IndexManager::createIndex(std::string name, rua type) {
		saveIndexFileCatalog(name, name + ".index");
		bPlusTreeType = type;
		currentType = type;
		switch (type) {
		case INT:
			bPlusTreeInt = new BPlusTree<BPlusNodeInt, IntKey>(currentMemory);
			break;
		case CHAR:
			bPlusTreeChar = new BPlusTree<BPlusNodeChar, CharKey>(currentMemory);
			break;
		case FLOAT:
			bPlusTreeFloat = new BPlusTree<BPlusNodeFloat, FloatKey>(currentMemory);
			break;
		}
	}

	int IndexManager::Insert(int key, addressType data) {
		IntKey keyStructureInt;
		keyStructureInt.key = key;
		keyStructureInt.data = data;
		return bPlusTreeInt->insert(keyStructureInt);
		/* switch(currentType){
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
		}*/


	}

	int IndexManager::Insert(std::string key, addressType data) {
		CharKey keyStructureChar;
		keyStructureChar.key = 'r';
		keyStructureChar.data = data;
		return bPlusTreeChar->insert(keyStructureChar);
	}

	int IndexManager::Insert(float key, addressType data) {
		FloatKey keyStructureFloat;
		keyStructureFloat.key = key;
		keyStructureFloat.data = data;
		return bPlusTreeFloat->insert(keyStructureFloat);

	}
	int IndexManager::Delete(int key) {
		IntKey keyStructure;
		keyStructure.key = key;
		keyStructure.data = 0;
		return bPlusTreeInt->remove(keyStructure);
	}
	int IndexManager::Delete(float key) {
		FloatKey keyStructure;
		keyStructure.key = key;
		keyStructure.data = 0;
		return bPlusTreeFloat->remove(keyStructure);
	}
	int IndexManager::Delete(std::string key) {

		CharKey keyStructure;
		keyStructure.key = 's';
		keyStructure.data = 0;
		return bPlusTreeChar->remove(keyStructure);
		

	}

	std::vector<addressType> IndexManager::Search(int key, int searchType) {
		IntKey keyStructure;
		keyStructure.key = key;
		keyStructure.data = 0;
		keyStructure.type = searchType;
		return bPlusTreeInt->searchNodes(keyStructure);
	}
	std::vector<addressType> IndexManager::Search(float key, int searchType) {
		FloatKey keyStructure;
		keyStructure.key = key;
		keyStructure.data = 0;
		keyStructure.type = searchType;
		return bPlusTreeFloat->searchNodes(keyStructure);
	}

	std::vector<addressType> IndexManager::Search(std::string key, int searchType) {
		CharKey keyStructure;
		keyStructure.key = 'a';
		keyStructure.data = 0;
		keyStructure.type = searchType;
		return bPlusTreeChar->searchNodes(keyStructure);
	}

	void IndexManager::saveIndexFileCatalog(std::string indexName, std::string indexFile) {
		;
	}

}
