//
// Created by wuyua on 2019-07-07.
//

#ifndef SIMEX_BLOCK_PERSISTENCE_REGISTRY_H
#define SIMEX_BLOCK_PERSISTENCE_REGISTRY_H

#include <unordered_map>
#include <memory>
#include "block.h"
#include "simstruc.h"
class PersistenceMap {
public:
    std::shared_ptr<Block> block {nullptr};
};

typedef std::unordered_map<void*, std::shared_ptr<PersistenceMap>> PersistenceMapType;
class BlockPersistenceRegistry {
public:
    static PersistenceMapType blockPersistenceRegistryMap;
    static void unregister(SimStruct* S);
    static std::shared_ptr<PersistenceMap> getOrCreateRegistry(SimStruct* S);
    static std::shared_ptr<PersistenceMap> getRegistry(SimStruct* S);
};
#endif //SIMEX_BLOCK_PERSISTENCE_REGISTRY_H
