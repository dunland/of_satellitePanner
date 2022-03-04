#include "TriggerFunctions.h"
#include "Globals.h"

///////////////////////// TRIGGER FUNCTIONS ///////////////////////////
bool TriggerFunctions::bUpdateCymbal = false;

// ----------------------------- Kick ---------------------------------
void TriggerFunctions::kickTrigger()
{
    for (int i = 0; i < CircleControls::circles.size() / 50; i++)
    {
        int random = int(ofRandom(CircleControls::circles.size()));
        CircleControls::circles[random]->size_bonus = 10;
    }
}

// ---------------------------- Snare ---------------------------------
void TriggerFunctions::snareTrigger()
{
    ofPixels &vidPixels = Globals::video.getPixels();
    int r = CircleControls::radius;

    for (int i = r * 2; i < Globals::video.getWidth(); i += r * 2)
    {
        for (int j = r * 2; j < Globals::video.getHeight(); j += r * 2)
        {
            float threshold_val;
            if (CircleControls::spawn_mode[CircleControls::spawn_index] == "brightness")
                threshold_val = vidPixels.getColor(i, j).getBrightness();
            else
                threshold_val = vidPixels.getColor(i, j).getLightness();
            CircleControls::checkPixelThreshold(i, j, threshold_val); // checks pixel brightness threshold and creates/increases circles

            CircleControls::checkPixelThreshold(i, j, threshold_val);
        }
    }
}

// ---------------------------- Cymbal --------------------------------
void TriggerFunctions::cymbalTrigger()
{
    // increase circle radius
    double max_size = 100;
    CircleControls::radius = min(max_size, (CircleControls::radius + 0.3));
    bUpdateCymbal = true;
}

void TriggerFunctions::cymbalUpdate()
{
    if (bUpdateCymbal)
    {
        // decay circle radius until standard
        CircleControls::radius -= 0.1;
        if (CircleControls::radius <= CircleControls::radius_standard)
        {
            bUpdateCymbal = false;
        }
    }
}

// ----------------------------- Bass ---------------------------------
void TriggerFunctions::bassTrigger()
{
    // create some lines
}

void TriggerFunctions::bassUpdate()
{
    // decay lines
}