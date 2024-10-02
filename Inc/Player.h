#ifndef PLAYER_H_
#define PLAYER_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <string>

class Player {
private:
    bool running;
    static Mix_Music *song;
public:
    Player();
    ~Player();

    void PlayTheSong(std::string path);
    void PauseTheSong(void);
    void ResumeTheSong(void);
    void StopTheSong(void);

    friend void PlayWithoutObject(std::string path);
};

void PlaySong(std::string path);


#endif