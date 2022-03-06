#pragma once
#include "ofMain.h"
#include "Globals.h"

class TriggerFunctions
{
    public:
    static void snareTrigger();
    static bool bUpdateSnare;
    static void snareUpdate();

    static void kickTrigger();
    static bool bUpdateKick;
    static void kickUpdate();

    static void cymbalTrigger();
    static bool bUpdateCymbal;
    static void cymbalUpdate();

    static void hihatTrigger();
    static bool bUpdateHihat;
    static void hihatUpdate();

	// ------------------------------ Visuals ---------------------------
    static string song;
};