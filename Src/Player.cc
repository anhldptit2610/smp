#include "Player.h"

Mix_Music *Player::song = nullptr;

Player::Player() : running(false), volume(10)
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

void Player::PlayTheSong(std::string path)
{
    Mix_PauseAudio(0);
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
    Mix_PauseAudio(1);
}
    
void Player::ResumeTheSong(void)
{
    Mix_PauseAudio(0);
}


void Player::StopTheSong(void)
{
    Mix_FreeMusic(song);
}

void PlayWithoutObject(std::string path)
{
    Mix_PauseAudio(0);
    Player::song = Mix_LoadMUS(path.c_str());
    if (!Player::song) {
        SDL_Log("Mix_LoadMUS error: %s\n", Mix_GetError());
        return;
    }
    if (Mix_PlayMusic(Player::song, 0) == -1) {
        SDL_Log("Mix_PlayMusic error: %s\n", Mix_GetError());
        Mix_FreeMusic(Player::song);
    }
}

void PlaySong(std::string path)
{
    PlayWithoutObject(path);
}