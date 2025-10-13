#pragma once

// In this project, the observer pattern can 
// be useful for decoupled communication between 
// layers. Take for instance, HUD layer and GameLayer. 
// HUD react on changes in the GameLayer. For example 
// when player takes damage HUD diplayes the current 
// health that it received from the GameLayer.

#include "Payload.h"

namespace ZPG {

class Observer {
public:
    virtual void OnNotify(Payload& payload) = 0;
};

}
