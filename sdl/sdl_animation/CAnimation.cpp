//==============================================================================
#include "CAnimation.h"

//==============================================================================
CAnimation::CAnimation() {
    CurrentFrame    = 0;
    MaxFrames       = 0;
    FrameInc        = 1;

    FrameRate       = 100; //Milliseconds
    OldTime         = 0;

    Oscillate       = false;
}

//------------------------------------------------------------------------------
void CAnimation::OnAnimate() {
    if(OldTime + FrameRate > SDL_GetTicks()) {
        return;
    }

    OldTime = SDL_GetTicks();

    if(Oscillate) {
        if(FrameInc > 0) {
            if(CurrentFrame >= MaxFrames - 1) {
                FrameInc = -FrameInc;
            }
        }else{
            if(CurrentFrame <= 0) {
                FrameInc = -FrameInc;
            }
        }
    }else{
        if(CurrentFrame >= MaxFrames - 1) {
            CurrentFrame = 0;
        }
    }

    CurrentFrame += FrameInc;
}

//==============================================================================
void CAnimation::SetFrameRate(int Rate) {
    FrameRate = Rate;
}

//------------------------------------------------------------------------------
void CAnimation::SetCurrentFrame(int Frame) {
    if(Frame < 0 || Frame >= MaxFrames) return;

    CurrentFrame = Frame;
}

//------------------------------------------------------------------------------
int CAnimation::GetCurrentFrame() {
    return CurrentFrame;
}

//==============================================================================
