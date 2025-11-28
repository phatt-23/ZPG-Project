#pragma once 

namespace ZPG {

// I choose to keep the events and payloads separate.
// The idea is that payloads are opaque and events are fully defined.
// Payloads are faster and easier to write than Events.
// They make sense when the information being transmitted is some singular data.
// The downside is, it isn't type-safe.

enum class PayloadType : u32 {
    NotSpecified = 0,
    // some others...
    CameraPositionChanged,
    CameraOrientationChanged,
    SceneChanged,
};

class Payload {
public:
    Payload() : Type(), Value(nullptr) {
    }

    Payload(const PayloadType type, void* value)
    : Type(type), Value(value) {
    }

    PayloadType Type;
    void* Value;    
};

}
