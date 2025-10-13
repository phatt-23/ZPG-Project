#pragma once 

namespace ZPG {

enum PayloadType {
    NotSpecified = 0,
    // some others...
    CameraPositionChanged,
    CameraOrientationChanged,
};

struct Payload {
    Payload() {
    }

    Payload(PayloadType type, void* value) 
    : Type(type), Value(value) {
    }

    PayloadType Type;
    void* Value;    
};

}
