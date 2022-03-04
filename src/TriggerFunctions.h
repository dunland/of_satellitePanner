#pragma once

class TriggerFunctions
{
    public:
    static void snareTrigger();
    static void kickTrigger();

    static void cymbalTrigger();
    static bool bUpdateCymbal;
    static void cymbalUpdate();

    static void bassTrigger();
    static bool bUpdateBass;
    static void bassUpdate();
};