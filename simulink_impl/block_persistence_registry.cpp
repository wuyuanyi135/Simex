//
// Created by wuyua on 2019-07-07.
//

#include "block_persistence_registry.h"
#include "block_exceptions.h"
#include "block_lifecycle.h"
#include "debug_utils.h"
PersistenceMapType BlockPersistenceRegistry::blockPersistenceRegistryMap;



void BlockPersistenceRegistry::unregister(SimStruct* S) {
    auto it = BlockPersistenceRegistry::blockPersistenceRegistryMap.find(S->blkInfo.block);
    if (it == BlockPersistenceRegistry::blockPersistenceRegistryMap.end()) {
        // not found
        throw KeyError(S->blkInfo.block);
    }

    PersistenceMap& pm = *(it->second);

    // remove
    BlockPersistenceRegistry::blockPersistenceRegistryMap.erase(S->blkInfo.block);
    DEBUG_PRINTF("Unregistered block: %d\n", reinterpret_cast<intptr_t>(S->blkInfo.block));
}

PersistenceMap &BlockPersistenceRegistry::getOrCreateRegistry(SimStruct* S) {
    auto it = BlockPersistenceRegistry::blockPersistenceRegistryMap.find(S->blkInfo.block);
    if (it == BlockPersistenceRegistry::blockPersistenceRegistryMap.end()) {
        // not found, create
        auto pm = new PersistenceMap();
        BlockPersistenceRegistry::blockPersistenceRegistryMap[S->blkInfo.block] = std::unique_ptr<PersistenceMap>(pm);
        DEBUG_PRINTF("Created new PersistenceMap for block: %d\n", reinterpret_cast<intptr_t>(S->blkInfo.block));
        return *pm;
    } else {
        // found, return
        DEBUG_PRINTF("Reuse a PersistenceMap for block: %d\n", reinterpret_cast<intptr_t>(S->blkInfo.block));
        return *(it->second);
    }
}

PersistenceMap &BlockPersistenceRegistry::getRegistry(SimStruct *S) {
    auto it = BlockPersistenceRegistry::blockPersistenceRegistryMap.find(S->blkInfo.block);
    if (it == BlockPersistenceRegistry::blockPersistenceRegistryMap.end()) {
        // not found, error
        throw std::exception("Block not found");
    }
    return *(it->second);
}
// TODO: how to remove ununsed blockMeta? maybe not save it at all!