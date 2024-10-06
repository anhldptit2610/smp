#include "Player.h"
#include "MediaFile.h"
#include "UI.h"

Mix_Music *Player::song = nullptr;

Player::Player() : volume(10)
{
}

Player::~Player()
{
}

void Player::TurnVolumeUp(void)
{
    if (volume < 10)
        volume++;
    Mix_VolumeMusic(12.8 * volume);
}

void Player::TurnVolumeDown(void)
{
    if (volume > 0)
        volume--;
    Mix_VolumeMusic(12.8 * volume);
}

void Player::PlayTheSong(int index, std::string path)
{
    GetFileManage().SetMin(index);
    GetFileManage().SetSec(index);
    ResetTime();
    SetPlayState(true);
    Mix_PauseAudio(0);
    std::thread timerThread(timeTracker, GetTimeWindow()); // Start the timer
    timerThread.detach();  // Detach it so it runs independently

    song = Mix_LoadMUS(path.c_str());
    if (!song) {
        SDL_Log("Mix_LoadMUS error: %s\n", Mix_GetError());
        return;
    }
    if (Mix_PlayMusic(song, 0) == -1) {
        SDL_Log("Mix_PlayMusic error: %s\n", Mix_GetError());
        Mix_FreeMusic(song);
    }
}

void Player::PauseTheSong(void)
{
    SetPlayState(false);
    Mix_PauseAudio(1);
}
    
void Player::ResumeTheSong(void)
{
    SetPlayState(true);
    Mix_PauseAudio(0);
}

void Player::StopTheSong(void)
{
    Mix_FreeMusic(song);
}

void PlayWithoutObject(std::string path)
{
    SetPlayState(true);
    ResetTime();
    Mix_PauseAudio(0);
    GetFileManage().NextTrack();
    Player::song = Mix_LoadMUS(GetFileManage().GetTrackPath(GetFileManage().GetCurrentTrack()).c_str());
    if (!Player::song) {
        SDL_Log("Mix_LoadMUS error: %s\n", Mix_GetError());
        return;
    }
    if (Mix_PlayMusic(Player::song, 0) == -1) {
        SDL_Log("Mix_PlayMusic error: %s\n", Mix_GetError());
        Mix_FreeMusic(Player::song);
    }
    std::thread timerThread(timeTracker, GetTimeWindow()); // Start the timer
    timerThread.detach();  // Detach it so it runs independently
}

void PlaySong(std::string path)
{
    PlayWithoutObject(path);
}