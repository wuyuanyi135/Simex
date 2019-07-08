//
// Created by wuyua on 2019-07-07.
//

#ifndef SIMEX_BLOCK_PERSISTENCE_REGISTRY_H
#define SIMEX_BLOCK_PERSISTENCE_REGISTRY_H

#include <unordered_map>
#include <memory>
#include "block_meta.h"
#include "block.h"
#include "simstruc.h"
class PersistenceMap {
public:
    Block* block {nullptr};
};

typedef std::unordered_map<void*, std::unique_ptr<PersistenceMap>> PersistenceMapType;
class BlockPersistenceRegistry {
public:
    static PersistenceMapType blockPersistenceRegistryMap;
    static void unregister(SimStruct* S);
    static PersistenceMap& getOrCreateRegistry(SimStruct* S);
};
#endif //SIMEX_BLOCK_PERSISTENCE_REGISTRY_H
