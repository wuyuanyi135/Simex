//
// Created by wuyua on 2019-07-07.
//

#ifndef SIMEX_BLOCK_H
#define SIMEX_BLOCK_H

// TODO: USE MACROS TO DEFINE ALL NEEDED INFORMATIONS IN MDINITIALIZE?

#include <vector>
#include <memory>
#include <unordered_map>
#include "port.h"
#include "simstruc.h"
#include "block_meta.h"

class Block {
public:
    SimStruct* _simulinkBlockReference;
    std::shared_ptr<BlockMeta> _blockMetaReference;
public:
    std::vector<std::shared_ptr<RuntimeInputPort>> inputPorts;
    std::vector<std::shared_ptr<RuntimeOutputPort>> outputPorts;

public:
    explicit Block(SimStruct&);
public:
    // Event callbacks
    virtual void onStart() = 0;
    virtual void onTerminate() = 0;
    virtual void onUpdate() = 0;
    virtual void onOutput() = 0;
    // Managed ports;

    // Managed states;



public:

};

#endif //SIMEX_BLOCK_H
