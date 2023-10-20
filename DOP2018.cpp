//
// Created by zrk on 2023/10/20.
//

#include <iostream>
#include <vector>
#include <cassert>
#include "tick.h"

using namespace std;

struct Vector3
{
    float x, y, z;
    Vector3() : x(0), y(0), z(0) {}
};

struct Quaternion
{
    float w, x, y, z;
    Quaternion() : w(0), x(0), y(0), z(0) {}
};

struct Opacity
{
    float opacity;
    Opacity() : opacity(0) {}
};

struct Valid
{
    bool valid;
    Valid() : valid(true) {}
};


// fake appendix data
struct OtherData
{
    // fake stuff
    Vector3 color;
    Vector3 velocity;
    Vector3 acceleration;
    Vector3 data[16];  // impulse, torque, angularVelocity, angularAcceleration, ...
    float life;
    float decay;
    OtherData* parent;
    OtherData* child;
    OtherData* sibling;
    OtherData() : color(), velocity(), acceleration(), life(1.0), decay(0.1),
        parent(nullptr), child(nullptr), sibling(nullptr) { }
};

// =======================================================================


class OOPShape : protected Opacity, public Valid, public OtherData {
public:
    Vector3 position;
    Quaternion rotation;
    virtual void draw() = 0;  // runtime polymorphism
    virtual ~OOPShape() = default;
};

class OOPCircle : public OOPShape {
    float radius;
public:
    OOPCircle() : radius(1.0) {}

    void draw() override {
        // fake code
        position.x += radius * opacity + rotation.w * rotation.x;
        position.y += radius * opacity + rotation.w * rotation.y;
        position.z += radius * opacity + rotation.w * rotation.z;
        radius *= 0.9;
        rotation.w *= 0.9;
    }
};

class OOPTriangle : public OOPShape {
    float sharpness;
public:
    OOPTriangle() : sharpness(1.0) {}

    void draw() override {
        // fake code
        position.x -= sharpness * opacity + rotation.w * rotation.x;
        position.y -= sharpness * opacity + rotation.w * rotation.y;
        position.z -= sharpness * opacity + rotation.w * rotation.z;
        sharpness *= 0.9;
        rotation.w *= 0.9;
    }
};

class OOPRectangle : public OOPShape {
    float width, height;
public:
    OOPRectangle() : width(1.0), height(1.0) {}

    void draw() override {
        // fake code
        position.x -= width * height * opacity + rotation.w * rotation.x;
        position.y -= width * height * opacity + rotation.w * rotation.y;
        position.z -= width * height * opacity + rotation.w * rotation.z;
        width *= 0.9;
        height *= 0.9;
        rotation.w *= 0.9;
    }
};

class OOPCapsule : public OOPShape {
    float width, height, radius;
public:
    OOPCapsule() : width(1.0), height(1.0), radius(1.0) {}

    void draw() override {
        // fake code
        position.x -= width * height * opacity + rotation.w * rotation.x;
        position.y -= width * height * opacity + rotation.w * rotation.y;
        position.z -= width * height * opacity + rotation.w * rotation.z;
        width *= 0.9;
        height *= 0.9;
        radius *= 0.9;
        rotation.w *= 0.9;
    }
};

// ===============================================================

class CoupledData {
public:
    Vector3* position = nullptr;
    Quaternion* rotation = nullptr;
    Opacity* opacity = nullptr;
};

template<typename T>
class DOPShape {};

struct Triangle {
    float sharpness = 0.0f;
};
struct Circle {
    float radius = 1.0f;
};
struct Rectangle {
    float width = 1.0, height = 1.0;
};
struct Capsule {
    float width = 1.0, height = 1.0, radius = 1.0;
};


template<>
class DOPShape<Circle> {
    Circle t;
public:
    void draw(CoupledData& data) {
        auto& position = *data.position;
        auto& rotation = *data.rotation;
        auto& opacity = (*data.opacity).opacity;
        auto& radius= t.radius;
        position.x += radius * opacity + rotation.w * rotation.x;
        position.y += radius * opacity + rotation.w * rotation.y;
        position.z += radius * opacity + rotation.w * rotation.z;
        radius *= 0.9;
        rotation.w *= 0.9;
    }
};

template<>
class DOPShape<Triangle> {
    Triangle t;
public:
    void draw(CoupledData& data) {
        auto& position = *data.position;
        auto& rotation = *data.rotation;
        auto& opacity = (*data.opacity).opacity;
        auto& sharpness = t.sharpness;
        position.x -= sharpness * opacity + rotation.w * rotation.x;
        position.y -= sharpness * opacity + rotation.w * rotation.y;
        position.z -= sharpness * opacity + rotation.w * rotation.z;
        sharpness *= 0.9;
        rotation.w *= 0.9;
    }
};

template<>
class DOPShape<Rectangle> {
    Rectangle t;
public:
    void draw(CoupledData& data) {
        auto& position = *data.position;
        auto& rotation = *data.rotation;
        auto& opacity = (*data.opacity).opacity;
        auto& width = t.width;
        auto& height = t.height;

        position.x -= width * height * opacity + rotation.w * rotation.x;
        position.y -= width * height * opacity + rotation.w * rotation.y;
        position.z -= width * height * opacity + rotation.w * rotation.z;
        width *= 0.9;
        height *= 0.9;
        rotation.w *= 0.9;
    }
};

template<>
class DOPShape<Capsule> {
    Capsule t;
public:
    void draw(CoupledData& data) {
        auto& position = *data.position;
        auto& rotation = *data.rotation;
        auto& opacity = (*data.opacity).opacity;
        auto& width = t.width;
        auto& height = t.height;
        auto& radius= t.radius;

        position.x -= width * height * opacity + rotation.w * rotation.x;
        position.y -= width * height * opacity + rotation.w * rotation.y;
        position.z -= width * height * opacity + rotation.w * rotation.z;
        width *= 0.9;
        height *= 0.9;
        radius *= 0.9;
        rotation.w *= 0.9;
    }
};

template<typename T>
inline void draw(DOPShape<T>& shape, CoupledData& data)
{
    shape.draw(data);  // compile-time polymorphism
}


// ===============================================================


void DOP2018SubMain(const size_t N, const size_t K=3) {
    // valid: 50%
    auto** fullArray = new OOPShape *[N * 4];
    auto* OOPFullCircle     = new OOPCircle[N];    size_t c = 0;
    auto* OOPFullTriangle   = new OOPTriangle[N];  size_t t = 0;
    auto* OOPFullRectangle  = new OOPRectangle[N]; size_t r = 0;
    auto* OOPFullCapsule    = new OOPCapsule[N];   size_t p = 0;

    for (size_t j = 0; j < N * 4; j++) {
        // FIXME: refactor
        auto rnd = (rand() % 4);
        if (rnd == 0 && c == N) rnd = (rnd + 1) % 4;
        if (rnd == 1 && t == N) rnd = (rnd + 1) % 4;
        if (rnd == 2 && r == N) rnd = (rnd + 1) % 4;
        if (rnd == 3 && p == N) rnd = (rnd + 1) % 4;
        if (rnd == 0 && c == N) rnd = (rnd + 1) % 4;
        if (rnd == 1 && t == N) rnd = (rnd + 1) % 4;
        if (rnd == 2 && r == N) rnd = (rnd + 1) % 4;
        if (rnd == 0) fullArray[j] = &OOPFullCircle[c++];
        if (rnd == 1) fullArray[j] = &OOPFullTriangle[t++];
        if (rnd == 2) fullArray[j] = &OOPFullRectangle[r++];
        if (rnd == 3) fullArray[j] = &OOPFullCapsule[p++];
        fullArray[j]->valid = ((rand() % 2) == 0);
    }

    // valid: 100%
    auto** validArray = new OOPShape *[N * 4];
    auto* OOPValidCircle    = new OOPCircle[N];    c = 0;
    auto* OOPValidTriangle  = new OOPTriangle[N];  t = 0;
    auto* OOPValidRectangle = new OOPRectangle[N]; r = 0;
    auto* OOPValidCapsule   = new OOPCapsule[N];   p = 0;
    for (size_t i = 0; i < N*4; i++) {
        // FIXME: refactor
        auto rnd = (rand() % 4);
        if (rnd == 0 && c == N) rnd = (rnd + 1) % 4;
        if (rnd == 1 && t == N) rnd = (rnd + 1) % 4;
        if (rnd == 2 && r == N) rnd = (rnd + 1) % 4;
        if (rnd == 3 && p == N) rnd = (rnd + 1) % 4;
        if (rnd == 0 && c == N) rnd = (rnd + 1) % 4;
        if (rnd == 1 && t == N) rnd = (rnd + 1) % 4;
        if (rnd == 2 && r == N) rnd = (rnd + 1) % 4;
        if (rnd == 0) validArray[i] = &OOPValidCircle[c++];
        if (rnd == 1) validArray[i] = &OOPValidTriangle[t++];
        if (rnd == 2) validArray[i] = &OOPValidRectangle[r++];
        if (rnd == 3) validArray[i] = &OOPValidCapsule[p++];
        validArray[i]->valid = true;
    }

    // DOP-valid: 50%
    auto* pArray = new Vector3[N*4];
    auto* qArray = new Quaternion[N*4];
    auto* oArray = new Opacity[N*4];
    auto* vArray = new Valid[N*4];
    for (size_t i = 0; i < N*4; i++)
        vArray[i].valid = ((rand() % 2) == 0);
    // TODO: auto* array = new Shape[N*4];  // class DOPShape<T> : public Shape
    auto* CArray = new DOPShape<Circle>[N];
    auto* TArray = new DOPShape<Triangle>[N];
    auto* RArray = new DOPShape<Rectangle>[N];
    auto* PArray = new DOPShape<Capsule>[N];
    // other data:
    auto* otherDatArray = new OtherData[N * 4];

    Tick tick;

    // OOP: valid = 50%
    tick.Start(string("OOP-valid=50%"));
    for (size_t k = 0; k < K; k++) {
        for (size_t j = 0; j < N * 4; j++) {
            if (fullArray[j]->valid)
                fullArray[j]->draw();
        }
    }
    tick.End();

    // OOP: always valid
    tick.Start(string("OOP-valid=100%"));
    for (size_t k = 0; k < K; k++) {
        for (size_t j = 0; j < N * 4; j++) {
            if (validArray[j]->valid)  // always true
                validArray[j]->draw();
        }
    }
    tick.End(0.5);  // `draw` is called two times, reduce the time

    // DOP: no valid bins
    tick.Start(string("DOP-w/o_valid_bins(valid=50%)"));
    for (size_t k = 0; k < K; k++) {
        // register CoupledData data;
        CoupledData data;

        for (size_t j = 0; j < N; j++) {
            if (!vArray[j].valid) continue;  // 1. memory access;  2. branch misprediction
            data.position = &pArray[j];
            data.rotation = &qArray[j];
            data.opacity  = &oArray[j];
            draw(CArray[j], data);
        }
        for (size_t j = 0; j < N; j++) {
            if (!vArray[j+N].valid) continue;
            data.position = &pArray[j+N];
            data.rotation = &qArray[j+N];
            data.opacity  = &oArray[j+N];
            draw(TArray[j], data);
        }
        for (size_t j = 0; j < N; j++) {
            if (!vArray[j+N*2].valid) continue;
            data.position = &pArray[j+N*2];
            data.rotation = &qArray[j+N*2];
            data.opacity  = &oArray[j+N*2];
            draw(RArray[j], data);
        }
        for (size_t j = 0; j < N; j++) {
            if (!vArray[j+N*3].valid) continue;
            data.position = &pArray[j+N*3];
            data.rotation = &qArray[j+N*3];
            data.opacity  = &oArray[j+N*3];
            draw(PArray[j], data);
        }
    }
    tick.End();

    // DOP: valid bins
    tick.Start(string("DOP-w._valid_bins"));
    for (size_t k = 0; k < K; k++) {
        // register CoupledData data;
        CoupledData data;

        // assume we have two list, valid[j < N/2] == true and valid[j >= N/2] == false
        for (size_t j = 0; j < N/2; j++) {
            data.position = &pArray[j];
            data.rotation = &qArray[j];
            data.opacity = &oArray[j];
            draw(CArray[j], data);
        }
        for (size_t j = 0; j < N/2; j++) {
            data.position = &pArray[j+N/2];
            data.rotation = &qArray[j+N/2];
            data.opacity = &oArray[j+N/2];
            draw(TArray[j], data);
        }
        for (size_t j = 0; j < N/2; j++) {
            data.position = &pArray[j+N];
            data.rotation = &qArray[j+N];
            data.opacity = &oArray[j+N];
            draw(RArray[j], data);
        }
        for (size_t j = 0; j < N/2; j++) {
            data.position = &pArray[j+3*N/2];
            data.rotation = &qArray[j+3*N/2];
            data.opacity = &oArray[j+3*N/2];
            draw(PArray[j], data);
        }
    }
    tick.End();
}

void DOP2018Main() {
    srand(2300271055);
    {  // force
        OOPCircle a;  OOPTriangle b;  OOPRectangle c;  OOPCapsule d;
        cout << &a.color << &a.velocity << &a.acceleration << &a.life << &a.decay
             << &a.parent << &a.child << &a.sibling
             << &a.data;
        cout << &b.color << &b.velocity << &b.acceleration << &b.life << &b.decay
             << &b.parent << &b.child << &b.sibling
             << &b.data;
        cout << &c.color << &c.velocity << &c.acceleration << &c.life << &c.decay
             << &c.parent << &c.child << &c.sibling
             << &c.data;
        cout << &d.color << &d.velocity << &d.acceleration << &d.life << &d.decay
             << &d.parent << &d.child << &d.sibling
             << &d.data;
    }
    cout << endl;

    //    vector<size_t> size = { 1024, 2048, 4096, 10240, 20480 };
    vector<size_t> size = { 256000*2,
                            256000*3,
                            256000*4,
                            256000*5,
                            256000*6, };

    for (auto& N : size) {
        cout << " ======== " << N << " ======== " << endl;
        DOP2018SubMain(N);
    }
}
