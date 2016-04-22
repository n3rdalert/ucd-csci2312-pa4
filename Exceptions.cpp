//
// Created by Jeffrey Cayton on 4/16/16.
//

#include "Exceptions.h"
using namespace std;

namespace Gaming {

    DimensionEx::DimensionEx(unsigned expWidth, unsigned expHeight, unsigned width, unsigned height) {
        __exp_width = expWidth;
        __exp_height = expHeight;
        __width = width;
        __height = height;
        setName("DimensionsEX");
    }

    std::ostream &operator <<(std::ostream &os, const GamingException &ex) {
        os << ex.__name << "\n";
        ex.__print_args(os);
        return os;
    }

    InsufficientDimensionsEx::InsufficientDimensionsEx(unsigned minWidth, unsigned minHeight, unsigned width,
                                                       unsigned height) : DimensionEx(minWidth, minHeight, width, height) {
        setName("InsufficientDimensionsEx");
    }

    void InsufficientDimensionsEx::__print_args(std::ostream &os) const {
        os << "(minWidth: " << getExpWidth() << ", minHeight: " << getExpHeight() << ", PassedWidth: "
        << getWidth() << ", PassedHeight: " << getHeight() << ")";
    }

    OutOfBoundsEx::OutOfBoundsEx(unsigned maxWidth, unsigned maxHeight, unsigned width, unsigned height) : DimensionEx(maxWidth, maxHeight, width, height) {
        setName("OutOfBoundsEx");
    }

    void OutOfBoundsEx::__print_args(std::ostream &os) const {
        os << "(minWidth: " << getExpWidth() << ", minHeight: " << getExpHeight() << ", PassedWidth: "
        << getWidth() << ", PassedHeight: " << getHeight() << ")";
    }

    void PositionEx::__print_args(std::ostream &os) const {
        os << "x: " << __x << " y: " << __y;
    }

    PositionEx::PositionEx(unsigned x, unsigned y) {
        __x = x;
        __y = y;
        setName("PositionEx");
    }

    PositionNonemptyEx::PositionNonemptyEx(unsigned x, unsigned y) : PositionEx(x, y) {
        setName("PositionNonemptyEx");
    }

    PositionEmptyEx::PositionEmptyEx(unsigned x, unsigned y) : PositionEx(x, y) {
        setName("PositionEmptyEx");
    }

    void PosVectorEmptyEx::__print_args(std::ostream &os) const {

    }

    PosVectorEmptyEx::PosVectorEmptyEx() {
        setName("PosVectorEmptyEx");
    }

    void GamingException::setName(std::string name) {
        __name = name;
    }

    unsigned DimensionEx::getExpWidth() const {
        return __exp_width;
    }

    unsigned DimensionEx::getExpHeight() const {
        return __exp_height;
    }

    unsigned DimensionEx::getWidth() const {
        return __width;
    }

    unsigned DimensionEx::getHeight() const {
        return __height;
    }
} //end of the exceptions file.