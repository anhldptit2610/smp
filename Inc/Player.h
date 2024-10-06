#ifndef PLAYER_H_
#define PLAYER_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <string>

class Player {
private:
    static Mix_Music *song;
    int volume;
public:
    Player();
    ~Player();

    void PlayTheSong(int index, std::string path);
    void PauseTheSong(void);
    void ResumeTheSong(void);
    void StopTheSong(void);
    void TurnVolumeUp(void);
    void TurnVolumeDown(void);

    friend void PlayWithoutObject(std::string path);
};

void PlaySong(std::string path);


#endif