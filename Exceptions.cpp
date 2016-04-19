//
// Created by Jeffrey Cayton on 4/16/16.
//

#include "Exceptions.h"

namespace Gaming {

    void Gaming::GamingException::setName(std::string name)
    { __name = name; }

    std::ostream &operator<<(std::ostream &os, const GamingException &ex)
    {
        os << ex.__name << "\n";
        ex.__print_args(os);
        return os;
    }

    DimensionEx::DimensionEx(unsigned expWidth, unsigned expHeight, unsigned width, unsigned height)
    {
        __exp_width = expWidth;
        __exp_height = expHeight;
        __width = width;
        __height = height;
        setName("DimensionEx");
    }

    unsigned DimensionEx::getExpWidth() const
    { return __exp_width; }

    unsigned DimensionEx::getExpHeight() const
    { return __exp_height; }

    unsigned DimensionEx::getWidth() const
    { return __width; }

    unsigned DimensionEx::getHeight() const
    { return __height; }


    void InsufficientDimensionsEx::__print_args(std::ostream &os) const
    {
        os << "minimum width: " << getExpWidth() << " minimum height: " << getExpHeight();
        os << "\nwidth: " << getWidth() << " height: " << getHeight() << "\n";
    }

    InsufficientDimensionsEx::InsufficientDimensionsEx(unsigned minWidth, unsigned minHeight, unsigned width, unsigned height) : DimensionEx(minWidth, minHeight, width, height)
    {
        setName("InsufficientDimensionsEx");
    }

    void OutOfBoundsEx::__print_args(std::ostream &os) const
    {
        os << "minWidth: " << getExpWidth() << " minHeight: " << getExpHeight();
        os << "\nwidth: " << getWidth() << " height: " << getHeight() << "\n";
    }

    OutOfBoundsEx::OutOfBoundsEx(unsigned maxWidth, unsigned maxHeight, unsigned width, unsigned height) : DimensionEx(maxWidth, maxHeight, width, height)
    {
        setName("OutOfBoundsEx");
    }


    void PositionEx::__print_args(std::ostream &os) const
    {
        os << "x: " << __x << " y: " << "\n";
    }

    PositionEx::PositionEx(unsigned x, unsigned y)
    {
        __x = x;
        __y = y;
        setName("PositionEx");
    }

    void PosVectorEmptyEx::__print_args(std::ostream &os) const
    {
        os << "Position vector empty ex";
    }

    PosVectorEmptyEx::PosVectorEmptyEx() : GamingException()
    {
        setName("PosVectorEmptyEx");
    }


} //end of the exceptions file.