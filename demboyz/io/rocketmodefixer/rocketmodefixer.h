
#pragma once

#include "../demowriter.h"
#include <stdio.h>

class RocketModeFixer : public DemoWriter
{
public:
    RocketModeFixer(FILE* outputFp);

    virtual void StartCommandPacket(const CommandPacket& packet) override;
};
