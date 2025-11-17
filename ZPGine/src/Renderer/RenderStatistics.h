//
// Created by phatt on 10/26/25.
//

#pragma once

namespace ZPG
{

    struct RenderStatistics
    {
        u32 Submissions = 0;

        void Reset()
        {
            *this = RenderStatistics{};
        }
    };


}
