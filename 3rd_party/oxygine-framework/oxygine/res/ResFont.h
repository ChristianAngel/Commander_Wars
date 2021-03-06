#pragma once
#include "../oxygine-include.h"
#include "Resource.h"

namespace oxygine
{
    DECLARE_SMART(ResFont, spResFont);
    class ResFont : public Resource
    {
    public:
        ResFont(): _size(0) {}

        virtual const Font* getFont(QString name = "", int size = 0) const = 0;
        int getSize() const { return _size; }

        virtual const Font* getClosestFont(float worldScale, int styleFontSize, float& resScale) const;
    protected:
        int _size;
    };
}
