#ifndef TWEENTOGGLEVISIBILITY_H
#define TWEENTOGGLEVISIBILITY_H

#include "oxygine-include.h"

class TweenToggleVisibility
{
public:
    typedef oxygine::Sprite type;
    /**
     * @brief TweenToggleVisibility. The sprite is visible between start and end
     * @param start fraction between 0 and 1
     * @param end fraction between 0 and 1
     */
    explicit TweenToggleVisibility(float start, float end);

    void update(oxygine::Sprite& actor, float p, const oxygine::UpdateState& us);

    void init(oxygine::Sprite& actor){}
    void done(oxygine::Sprite& actor){}
protected:
    float startFraction{0};
    float endFracrtion{0};
};

#endif // TWEENTOGGLEVISIBILITY_H
