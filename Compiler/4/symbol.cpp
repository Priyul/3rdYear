#include "symbol.h"

Symbol::Symbol() : nodeId(-1), scopeId(-1), name("") {} // Add this line

Symbol::Symbol(int nodeId, int scopeId, const std::string &name) : nodeId(nodeId), scopeId(scopeId), name(name) {
    this->isCalled = false;
}
